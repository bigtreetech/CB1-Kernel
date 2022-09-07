/*
 * (C) Copyright 2013-2019
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 */
#ifndef _SUN50IW9_H
#define _SUN50IW9_H

#define BOOT_PUB_HEAD_VERSION  "3000"
#define CONFIG_ARCH_SUN50IW9
#define CONFIG_ARCH_SUN50IW9P1
#define CONFIG_SUNXI_NCAT
#define CONFIG_DRAM_PARA_V1
#define CONFIG_MONITOR
/*#define FPGA_PLATFORM*/

/* sram layout*/
#define CONFIG_SYS_SRAM_BASE             (0x20000)
#define CONFIG_SYS_SRAM_SIZE             (0x8000)
#define CONFIG_SYS_SRAMA2_BASE           (0x100000)
#define CONFIG_SYS_SRAMA2_SIZE           (0x30000)
#define CONFIG_SYS_SRAMC_BASE            (0x28000)
#define CONFIG_SYS_SRAMC_SIZE            (0x30000)

/* dram layout*/
#define SDRAM_OFFSET(x)                   (0x40000000+(x))
#define CONFIG_SYS_DRAM_BASE              SDRAM_OFFSET(0)
#define DRAM_PARA_STORE_ADDR              SDRAM_OFFSET(0x00800000) /*fel*/
#define CONFIG_HEAP_BASE                  SDRAM_OFFSET(0x00800000) /*secure */
#define CONFIG_HEAP_SIZE                  (16 * 1024 * 1024)

#define CONFIG_BOOTPKG_BASE               SDRAM_OFFSET(0x02e00000)
#define CONFIG_MONITOR_BASE               SDRAM_OFFSET(0x08000000)
#define SCP_DRAM_BASE                     SDRAM_OFFSET(0x08100000)


#define SUNXI_DRAM_PARA_MAX               32

#define SUNXI_LOGO_COMPRESSED_LOGO_SIZE_ADDR            SDRAM_OFFSET(0x03000000)
#define SUNXI_LOGO_COMPRESSED_LOGO_BUFF                 SDRAM_OFFSET(0x03000010)
#define SUNXI_SHUTDOWN_CHARGE_COMPRESSED_LOGO_SIZE_ADDR SDRAM_OFFSET(0x03100000)
#define SUNXI_SHUTDOWN_CHARGE_COMPRESSED_LOGO_BUFF  	SDRAM_OFFSET(0x03100010)
#define SUNXI_ANDROID_CHARGE_COMPRESSED_LOGO_SIZE_ADDR  SDRAM_OFFSET(0x03200000)
#define SUNXI_ANDROID_CHARGE_COMPRESSED_LOGO_BUFF   	SDRAM_OFFSET(0x03200010)

/* handle standby */
#define RTC_STANDBY_FLAG_REG		(SUNXI_RTC_BASE + 0x1f8)
#define RTC_STANDBY_SOFT_ENTRY_REG	(SUNXI_RTC_BASE + 0x1fc)

/* for dram crc check */
#define CRC_VALUE_BEFORE		3
#define CRC_EN				0
#define CRC_START			6
#define CRC_LEN				7

/* boot run addr */
#define FEL_BASE                          0x20
#define SECURE_FEL_BASE                  (0x64)
#define CONFIG_BOOT0_RUN_ADDR            (0x20000)  /* sram a */
#define CONFIG_NBOOT_STACK               (CONFIG_SYS_SRAMC_BASE+CONFIG_SYS_SRAMC_SIZE)
#define CONFIG_TOC0_RUN_ADDR             (0x20480)  /* sram a */
#define CONFIG_HASH_TABLE_STACK_GAP      (4)
#define CONFIG_HASH_INFO_TABLE_SIZE      (512 - CONFIG_HASH_TABLE_STACK_GAP)/*to pass hash info to optee*/
#define CONFIG_HASH_INFO_TABLE_BASE      (CONFIG_SYS_SRAMC_BASE + CONFIG_SYS_SRAMC_SIZE - CONFIG_HASH_INFO_TABLE_SIZE)
#define CONFIG_SBOOT_STACK               (CONFIG_HASH_INFO_TABLE_BASE - CONFIG_HASH_TABLE_STACK_GAP)
#define CONFIG_BOOT0_RET_ADDR            (CONFIG_SYS_SRAM_BASE)
#define CONFIG_TOC0_HEAD_BASE            (CONFIG_SYS_SRAM_BASE)
#define CONFIG_TOC0_CFG_ADDR             (CONFIG_SYS_SRAM_BASE + 0x80)


#define CONFIG_SYS_INIT_RAM_SIZE 0x8000

/* FES */
#define CONFIG_FES1_RUN_ADDR             (0x28000)
#define CONFIG_FES1_RET_ADDR             (CONFIG_SYS_SRAMC_BASE + 0x7210)

/*CPU vol for boot*/
#define CONFIG_SUNXI_CORE_VOL           1100
/*sys vol for boot*/
#define CONFIG_SUNXI_SYS_VOL           940


/*cpu*/
#define SUNXI_DBG_REG1						(0xc4)
#define SUNXI_CLUSTER_PWROFF_GATING			(0x44)
#define SUNXI_CPU_RST_CTRL					(0x0)
#define SUNXI_CPU_PWR_SW(cpu)				(0x50 + cpu*0x04)

#endif
