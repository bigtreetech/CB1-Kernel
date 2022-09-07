/*
 * (C) Copyright 2015 Hans de Goede <hdegoede@redhat.com>
 *
 * Configuration settings for the Allwinner A80 (sun9i) CPU
 */

#ifndef _SUN8IW7_H
#define _SUN8IW7_H

#define BOOT_PUB_HEAD_VERSION "3000"
#define CONFIG_ARCH_SUN8IW7
#define CONFIG_ARCH_SUN8IW7P1
#define CONFIG_SUNXI_VERSION1
#define CONFIG_DRAM_PARA_V1

/* sram layout*/
#define CONFIG_SYS_SRAM_BASE             (0x0)
#define CONFIG_SYS_SRAM_SIZE             (0x10000)
#define CONFIG_SYS_SRAMA2_BASE           (0x44000)
#define CONFIG_SYS_SRAMA2_SIZE           (0x8000)
#define CONFIG_SYS_SRAMC_BASE            (0x10000)
#define CONFIG_SYS_SRAMC_SIZE            (0xB000)

#define SCP_SRAM_BASE                        (0x40000)
#define SCP_SRAM_SIZE                        (0xC000)
#define SCP_DRAM_BASE                        (0x43080000)
#define SCP_DRAM_SIZE                        (0x10000)
#define SCP_CODE_DRAM_OFFSET		         (0xC000)
#define HEADER_OFFSET                    (0x4000)
/*dram_para_offset is the numbers of u32 before dram data sturcture(dram_para) in struct arisc_para*/
#define SCP_DRAM_PARA_OFFSET             (sizeof(u32) * 2)
#define SCP_DARM_PARA_NUM     (32)

/* dram layout*/
#define SDRAM_OFFSET(x) (0x40000000 + (x))
#define CONFIG_SYS_DRAM_BASE SDRAM_OFFSET(0)
#define DRAM_PARA_STORE_ADDR SDRAM_OFFSET(0x00800000) /*fel*/
#define CONFIG_HEAP_BASE SDRAM_OFFSET(0x00800000) /*secure */
#define CONFIG_HEAP_SIZE (16 * 1024 * 1024)

#define CONFIG_BOOTPKG_BASE SDRAM_OFFSET(0x02e00000)
#define CONFIG_MONITOR_BASE SDRAM_OFFSET(0x08000000)

#define SUNXI_DRAM_PARA_MAX 32

#define SUNXI_LOGO_COMPRESSED_LOGO_SIZE_ADDR SDRAM_OFFSET(0x03000000)
#define SUNXI_LOGO_COMPRESSED_LOGO_BUFF SDRAM_OFFSET(0x03000010)
#define SUNXI_SHUTDOWN_CHARGE_COMPRESSED_LOGO_SIZE_ADDR SDRAM_OFFSET(0x03100000)
#define SUNXI_SHUTDOWN_CHARGE_COMPRESSED_LOGO_BUFF SDRAM_OFFSET(0x03100010)
#define SUNXI_ANDROID_CHARGE_COMPRESSED_LOGO_SIZE_ADDR SDRAM_OFFSET(0x03200000)
#define SUNXI_ANDROID_CHARGE_COMPRESSED_LOGO_BUFF SDRAM_OFFSET(0x03200010)

/* boot run addr */
#define FEL_BASE 0xffff0020
#define SECURE_FEL_BASE (0xffff0064)
#define CONFIG_BOOT0_RUN_ADDR (CONFIG_SYS_SRAM_BASE) /* sram a */
#define CONFIG_NBOOT_STACK (CONFIG_SYS_SRAM_BASE + CONFIG_SYS_SRAM_SIZE - 4)
#define CONFIG_TOC0_RUN_ADDR (CONFIG_SYS_SRAM_BASE + 0x480) /* sram a */
#define CONFIG_SBOOT_STACK                                                     \
	(CONFIG_SYS_SRAMC_BASE + CONFIG_SYS_SRAMC_SIZE - 4)
#
#define CONFIG_BOOT0_RET_ADDR (CONFIG_SYS_SRAM_BASE)
#define CONFIG_TOC0_HEAD_BASE (CONFIG_SYS_SRAM_BASE)
#define CONFIG_TOC0_CFG_ADDR (CONFIG_SYS_SRAM_BASE + 0x80)

#define CONFIG_SYS_INIT_RAM_SIZE         (CONFIG_SYS_SRAM_SIZE)

/* FES */
#define CONFIG_FES1_RUN_ADDR (0x8000)
#define CONFIG_FES1_RET_ADDR (CONFIG_SYS_SRAM_BASE + 0x7210)

/*CPU vol for boot*/
#define CONFIG_SUNXI_CORE_VOL 900

#endif
