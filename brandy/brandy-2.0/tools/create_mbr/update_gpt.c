#include <malloc.h>
#include "types.h"
#include <string.h>
#include "crc.h"
#include "sunxi_mbr.h"
#include <ctype.h>
#include <unistd.h>
#include "gpt.h"


#define  MAX_PATH  260
#define CONFIG_MMC_LOGICAL_OFFSET   (20 * 1024 * 1024/512)
#define CONFIG_NAND_LOGICAL_OFFSET   (0/512)

int IsFullName(const char *FilePath);

int create_standard_gpt(sunxi_mbr_t *sunxi_mbr_buf, char* gpt_buf);
__s32  write_gpt_table(sunxi_mbr_t *mbr_info, __s32 mbr_count, FILE *mbr_file);

int gpt_main(sunxi_mbr_t *mbr_info, char* gpt_name)
{
	char   gpt_file_name[MAX_PATH];
	char  *mbr_buff = NULL;
	FILE  *gpt_file = NULL;
	int    ret = -1;

	memset(gpt_file_name, 0, MAX_PATH);
	GetFullPath(gpt_file_name,   gpt_name);

	gpt_file = fopen(gpt_file_name, "wb");
	ret = write_gpt_table(mbr_info, 1, gpt_file);

	fclose(gpt_file);
	gpt_file = NULL;

	if (!ret)
		printf("update gpt file ok\n");
	else
		printf("update gpt file fail\n");

	if (gpt_file)
		fclose(gpt_file);

	return ret;
}

static int gpt_show_partition_info(char *buf)
{
	int			i,j;

	char   char8_name[PARTNAME_SZ] = {0};
	gpt_header     *gpt_head  = (gpt_header*)(buf + GPT_HEAD_OFFSET);
	gpt_entry      *entry  = (gpt_entry*)(buf + GPT_ENTRY_OFFSET);


	printf("GPT----part num %d---\n", gpt_head->num_partition_entries);
	printf("gpt_entry: %ld\n", sizeof(gpt_entry));
	printf("gpt_header: %ld\n", sizeof(gpt_header));
	for(i=0; i < gpt_head->num_partition_entries; i++ ){
		
		for(j=0; j < PARTNAME_SZ; j++ ) {
			char8_name[j] = (char)(entry[i].partition_name[j]);
		}
		printf("GPT:%-12s: %-12llx  %-12llx\n", char8_name, entry[i].starting_lba, entry[i].ending_lba);
	}

	return 0;
}

__s32  write_gpt_table(sunxi_mbr_t *mbr_info, __s32 mbr_count, FILE *gpt_file)
{
	int i, ret = -1;
	unsigned crc32_total;
	unsigned gpt_data_len = 0;
	char gpt_buf[8*1024] = {0};

	memset(gpt_buf, 0x0, sizeof(gpt_buf));
	gpt_data_len = create_standard_gpt(mbr_info, gpt_buf);
	if(gpt_data_len == 0)
		return -1;
	if(gpt_data_len > sizeof(gpt_buf)) {
		printf("error:too much partition\n");
		return -1;
	}
	ret = fwrite(gpt_buf, sizeof(gpt_buf), 1, gpt_file);
	if ( ret != 1) {
		printf("write file fail,ret %d\n", ret);
		return -1;
	}
	gpt_show_partition_info(gpt_buf);

	return 0;
}

int create_standard_gpt(sunxi_mbr_t *sunxi_mbr_buf, char* gpt_buf)
{
	legacy_mbr   *remain_mbr;
	sunxi_mbr_t  *sunxi_mbr = (sunxi_mbr_t *)sunxi_mbr_buf;

	char         *pbuf = gpt_buf;
	gpt_header   *gpt_head;
	gpt_entry    *pgpt_entry = NULL;
	char         *gpt_entry_start = NULL;
	u32           data_len = 0;
	u32           total_sectors;
	u32           logic_offset = 0;
	int           i,j = 0;
	int 		max_part = 0;

	unsigned char guid[16] = {0x88, 0x38, 0x6f, 0xab, 0x9a, 0x56, 0x26, 0x49, 0x96, 0x68,
		0x80, 0x94, 0x1d, 0xcb, 0x40, 0xbc};
	unsigned char part_guid[16] = {0x46, 0x55, 0x08, 0xa0, 0x66, 0x41, 0x4a, 0x74, 0xa3,
		0x53, 0xfc, 0xa9, 0x27, 0x2b, 0x8e, 0x45};
	
	if (strncmp((const char *)sunxi_mbr->magic, SUNXI_MBR_MAGIC, 8)) {
		printf("%s:not sunxi mbr, can't convert to GPT partition\n", __func__);
		return 0;
	}

	if (calc_crc32(( char *)(sunxi_mbr_buf) + 4, SUNXI_MBR_SIZE - 4) != sunxi_mbr->crc32) {
		printf("%s:sunxi mbr crc error, can't convert to GPT partition\n",__func__);
		return 0;
	}

	logic_offset = 0;

	/*fix me at boot stage, init sectors = 4*1024*1024*1024/512 */
	/*total_sectors = 4*1024*2048;*/

	/* 1. LBA0: write legacy mbr,part type must be 0xee */
	remain_mbr = (legacy_mbr *)pbuf;
	memset(remain_mbr, 0x0, 512);
	remain_mbr->partition_record[0].sector = 0x2;
	remain_mbr->partition_record[0].cyl = 0x0;
	remain_mbr->partition_record[0].part_type = EFI_PMBR_OSTYPE_EFI_GPT;
	remain_mbr->partition_record[0].end_head = 0xFF;
	remain_mbr->partition_record[0].end_sector = 0xFF;
	remain_mbr->partition_record[0].end_cyl = 0xFF;
	remain_mbr->partition_record[0].start_sect = 1UL;
	remain_mbr->partition_record[0].nr_sects = 0xffffffff;
	remain_mbr->end_flag = MSDOS_MBR_SIGNATURE;
	data_len += 512;

	/* 2. LBA1: fill primary gpt header */
	gpt_head = (gpt_header *)(pbuf + data_len);
	gpt_head->signature= GPT_HEADER_SIGNATURE;
	gpt_head->revision = GPT_HEADER_REVISION_V1;
	gpt_head->header_size = GPT_HEADER_SIZE;
	gpt_head->header_crc32 = 0x00;
	gpt_head->reserved1 = 0x0;
	gpt_head->my_lba = 0x01;
	gpt_head->alternate_lba = 0; /*total_sectors - 1;*/
	gpt_head->first_usable_lba = sunxi_mbr->array[0].addrlo + logic_offset;
	/*1 GPT head + 32 GPT entry*/
	gpt_head->last_usable_lba = 0; /*total_sectors - (1 + 32) - 1;*/
	memcpy(gpt_head->disk_guid.b,guid,16);
	gpt_head->partition_entry_lba = 2;
	gpt_head->num_partition_entries = sunxi_mbr->PartCount;
	gpt_head->sizeof_partition_entry = GPT_ENTRY_SIZE;
	gpt_head->partition_entry_array_crc32 = 0;
	data_len += 512;

	/*to avoid  the first boot0(offset is 16 sectors)*/
	max_part = (16-2)*512/GPT_ENTRY_SIZE;
	if (sunxi_mbr->PartCount > max_part) {
		printf("PartCount is %d, max is %d, is conflict with boot0 offset\n",
			sunxi_mbr->PartCount, max_part);
		return 0;
	}
	
    /* 3. LBA2~LBAn: fill gpt entry */
	u64 disk_size = 0;
	gpt_entry_start = (pbuf + data_len);
	for(i=0;i<sunxi_mbr->PartCount;i++) {
		pgpt_entry = (gpt_entry *)(gpt_entry_start + (i)*GPT_ENTRY_SIZE);

		pgpt_entry->partition_type_guid = PARTITION_BASIC_DATA_GUID;

		memcpy(pgpt_entry->unique_partition_guid.b,part_guid,16);
		pgpt_entry->unique_partition_guid.b[15] = part_guid[15]+i;

		pgpt_entry->starting_lba = ((u64)sunxi_mbr->array[i].addrhi<<32) + sunxi_mbr->array[i].addrlo + logic_offset;
		//UDISK partition
		if(i == sunxi_mbr->PartCount-1) {
			disk_size += 16; /*8k for test*/
			pgpt_entry->ending_lba = pgpt_entry->starting_lba+16-1; /*gpt_head->last_usable_lba - 1;*/
			gpt_head->alternate_lba = disk_size - 1;
			gpt_head->last_usable_lba = disk_size -1;
		}
		else {
			pgpt_entry->ending_lba = pgpt_entry->starting_lba +((u64)sunxi_mbr->array[i].lenhi<<32) + sunxi_mbr->array[i].lenlo-1;
			disk_size += ((u64)sunxi_mbr->array[i].lenhi<<32) + sunxi_mbr->array[i].lenlo-1;
		}

		//printf("GPT:%-12s: %-12llx  %-12llx\n", sunxi_mbr->array[i].name, pgpt_entry->starting_lba, pgpt_entry->ending_lba);
		if(sunxi_mbr->array[i].ro == 1) {
			pgpt_entry->attributes.fields.type_guid_specific = 0x6000;
		}
		else {
			pgpt_entry->attributes.fields.type_guid_specific = 0x8000;
		}
		pgpt_entry->attributes.fields.user_type =  sunxi_mbr->array[i].user_type;
		pgpt_entry->attributes.fields.keydata = sunxi_mbr->array[i].keydata >0 ? 1: 0;
		

		//ASCII to unicode
		memset(pgpt_entry->partition_name, 0,PARTNAME_SZ*sizeof(efi_char16_t));
		for(j=0;j < strlen((const char *)sunxi_mbr->array[i].name);j++ ) {
			pgpt_entry->partition_name[j] = (efi_char16_t)sunxi_mbr->array[i].name[j];
		}
		data_len += GPT_ENTRY_SIZE;

	}

	/* entry crc */
	gpt_head->partition_entry_array_crc32 = 0;
	gpt_head->partition_entry_array_crc32 = calc_crc32((unsigned char *)gpt_entry_start,
	             (gpt_head->num_partition_entries)*(gpt_head->sizeof_partition_entry));


	/* gpt crc */
	gpt_head->header_crc32 =0;
	gpt_head->header_crc32 = calc_crc32(( unsigned char *)gpt_head, sizeof(gpt_header));
	printf("gpt_head->header_crc32 = 0x%x\n",gpt_head->header_crc32);

	/* 4. LBA-1: the last sector fill backup gpt header */

	return data_len;
}
