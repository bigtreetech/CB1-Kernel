/*
 * Copyright (C) 2016 Allwinner.
 * wangwei <wangwei@allwinnertech.com>
 *
 * GPT  Head.h
 *
 * SPDX-License-Identifier: GPL-2.0+
 */



#ifndef _GPT_H
#define _GPT_H

#define MSDOS_MBR_SIGNATURE 0xAA55
#define EFI_PMBR_OSTYPE_EFI 0xEF
#define EFI_PMBR_OSTYPE_EFI_GPT 0xEE

#define GPT_BLOCK_SIZE 512
#define GPT_RESERVED_SIZE (GPT_BLOCK_SIZE - 92)
#define GPT_HEADER_SIGNATURE_SIZE 8
//#define GPT_HEADER_SIGNATURE  "EFI PART"
#define GPT_HEADER_SIGNATURE 0x5452415020494645ULL
#define GPT_HEADER_REVISION_V1 0x00010000
#define GPT_HEADER_SIZE 0x5c
#define GPT_PRIMARY_PARTITION_TABLE_LBA 1ULL
#define GPT_ENTRY_NAME "gpt"

#define GPT_ENTRY_NUMBERS       128
#define GPT_ENTRY_SIZE          128
#define GPT_ENTRY_OFFSET        1024
#define GPT_HAED_OFFSET         512



#define EFI_GUID(a,b,c,d0,d1,d2,d3,d4,d5,d6,d7) \
	((efi_guid_t) \
	{{ (a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff, ((a) >> 24) & 0xff, \
		(b) & 0xff, ((b) >> 8) & 0xff, \
		(c) & 0xff, ((c) >> 8) & 0xff, \
		(d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})

#define PARTITION_SYSTEM_GUID \
	EFI_GUID( 0xC12A7328, 0xF81F, 0x11d2, \
		0xBA, 0x4B, 0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B)
#define LEGACY_MBR_PARTITION_GUID \
	EFI_GUID( 0x024DEE41, 0x33E7, 0x11d3, \
		0x9D, 0x69, 0x00, 0x08, 0xC7, 0x81, 0xF3, 0x9F)
#define PARTITION_MSFT_RESERVED_GUID \
	EFI_GUID( 0xE3C9E316, 0x0B5C, 0x4DB8, \
		0x81, 0x7D, 0xF9, 0x2D, 0xF0, 0x02, 0x15, 0xAE)
#define PARTITION_BASIC_DATA_GUID \
	EFI_GUID( 0xEBD0A0A2, 0xB9E5, 0x4433, \
		0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7)
#define PARTITION_LINUX_RAID_GUID \
	EFI_GUID( 0xa19d880f, 0x05fc, 0x4d3b, \
		0xa0, 0x06, 0x74, 0x3f, 0x0f, 0x84, 0x91, 0x1e)
#define PARTITION_LINUX_SWAP_GUID \
	EFI_GUID( 0x0657fd6d, 0xa4ab, 0x43c4, \
		0x84, 0xe5, 0x09, 0x33, 0xc8, 0x4b, 0x4f, 0x4f)
#define PARTITION_LINUX_LVM_GUID \
	EFI_GUID( 0xe6d6d379, 0xf507, 0x44c2, \
		0xa2, 0x3c, 0x23, 0x8f, 0x2a, 0x3d, 0xf9, 0x28)

/* linux/include/efi.h */
typedef unsigned short efi_char16_t;

#pragma pack(1)

typedef struct {
	unsigned char b[16];
} efi_guid_t;


/* based on linux/include/genhd.h */
struct partition {
	unsigned char boot_ind;		/* 0x80 - active */
	unsigned char head;		/* starting head */
	unsigned char sector;		/* starting sector */
	unsigned char cyl;		/* starting cylinder */
	unsigned char part_type;		/* What partition type */
	unsigned char end_head;		/* end head */
	unsigned char end_sector;	/* end sector */
	unsigned char end_cyl;		/* end cylinder */
	unsigned int start_sect;	/* starting sector counting from 0 */
	unsigned int nr_sects;	/* nr of sectors in partition */
};

/* based on linux/fs/partitions/efi.h */
typedef struct _gpt_header {
	u64           signature;
	u32           revision;
	u32           header_size;
	u32           header_crc32;
	u32           reserved1;
	u64           my_lba;
	u64           alternate_lba;
	u64           first_usable_lba;
	u64           last_usable_lba;
	efi_guid_t    disk_guid;
	u64           partition_entry_lba;
	u32           num_partition_entries;
	u32           sizeof_partition_entry;
	u32           partition_entry_array_crc32;
} gpt_header;

typedef union _gpt_entry_attributes {
	struct {
		u64 required_to_function:1;
		u64 no_block_io_protocol:1;
		u64 legacy_bios_bootable:1;
		u64 reserved:27;
		u64 user_type:16;
		u64 ro:1;
		u64 keydata:1;
		u64 type_guid_specific:16;
	} fields;
	unsigned long long raw;
}  gpt_entry_attributes;


#define PARTNAME_SZ	(72 / sizeof(efi_char16_t))

typedef struct _gpt_entry {
	efi_guid_t partition_type_guid;
	efi_guid_t unique_partition_guid;
    u64        starting_lba;
    u64        ending_lba;
	gpt_entry_attributes attributes;
	efi_char16_t partition_name[PARTNAME_SZ];
} gpt_entry;


typedef struct _legacy_mbr {
	unsigned char boot_code[440];
	unsigned char unique_mbr_signature[4];
	unsigned char unknown[2];
	struct partition partition_record[4];
	short	end_flag;
}  legacy_mbr;
#pragma pack()

#define GPT_ENTRY_OFFSET        1024
#define GPT_HEAD_OFFSET         512

#endif	/* _GPT_H */
