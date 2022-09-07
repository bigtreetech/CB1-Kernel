#ifndef _PRIVATE_TEE_HEAD_
#define _PRIVATE_TEE_HEAD_

struct HASH_INFO {
	uint8_t name[16];
	uint8_t hash[32];
};

#define SUNXI_SECURE_BOOT_INFO_MAGIC "BootInfo"
#define OPTEE_HAED_HASH_INFO_MAX (10)
struct sunxi_secure_boot_head {
	unsigned char magic[8]; /* ="BootInfo"  */
	unsigned int version;
	unsigned int hash_info_count;
	struct HASH_INFO toc1_hash_info[OPTEE_HAED_HASH_INFO_MAX];
	unsigned int reserved[131]; /* pad to 1020*/
};

struct spare_optee_head {
	unsigned int jump_instruction;
	unsigned char magic[8];
	unsigned int dram_size;
	unsigned int drm_size;
	unsigned int length;
	unsigned int optee_length;
	unsigned char version[8];
	unsigned char platform[8];
	unsigned int dram_para[32];
	unsigned char expand_magic[8];
	unsigned char expand_version[8];
	unsigned int vdd_cpua;
	unsigned int vdd_cpub;
	unsigned int vdd_sys;
	unsigned int vcc_pll;
	unsigned int vcc_io;
	unsigned int vdd_res1;
	unsigned int vdd_res2;
	unsigned int pmu_count;
	unsigned int pmu_port;
	unsigned int pmu_para;
	unsigned char pmu_id[4];
	unsigned char pmu_addr[4];
	unsigned char chipinfo[8];
	unsigned int reserved[707];   /* pad to 3072, leave 1020 for secure boot info*/
	struct sunxi_secure_boot_head secure_boot_info;
};

#endif
