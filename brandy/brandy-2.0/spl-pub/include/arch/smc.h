/*
 * (C) Copyright 2018
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * wangwei@allwinnertech.com
 */

#ifndef _TZASC_SMC_H_
#define _TZASC_SMC_H_

#include <config.h>
#include <arch/cpu.h>

#if defined(CONFIG_ARCH_SUN50IW3) || defined(CONFIG_ARCH_SUN8IW18) ||          \
	defined(CONFIG_ARCH_SUN50IW9) || defined(CONFIG_ARCH_SUN50IW10) ||     \
	defined(CONFIG_ARCH_SUN8IW15)
#define SMC_CONFIG_REG                (SUNXI_SMC_BASE + 0x0000)
#define SMC_ACTION_REG                (SUNXI_SMC_BASE + 0x0004)
#define SMC_LD_RANGE_REG              (SUNXI_SMC_BASE + 0x0008)
#define SMC_LD_SELECT_REG             (SUNXI_SMC_BASE + 0x000c)
#define SMC_INT_STATUS_REG            (SUNXI_SMC_BASE + 0x0010)
#define SMC_INT_CLEAR_REG             (SUNXI_SMC_BASE + 0x0014)

#define SMC_FAIL_ADDR_REG             (SUNXI_SMC_BASE + 0x0020)
#define SMC_FAIL_CTRL_REG             (SUNXI_SMC_BASE + 0x0028)
#define SMC_FAIL_ID_REG               (SUNXI_SMC_BASE + 0x002c)
#define SMC_SPECU_CTRL_REG            (SUNXI_SMC_BASE + 0x0030)
#define SMC_INVER_EN_REG              (SUNXI_SMC_BASE + 0x0034)

#define SMC_DRM_MATER0_EN_REG         (SUNXI_SMC_BASE + 0x0050)
#define SMC_DRM_MATER1_EN_REG         (SUNXI_SMC_BASE + 0x0054)
#define SMC_DRM_ILLACCE_REG           (SUNXI_SMC_BASE + 0x0058)

#define SMC_MST0_BYP_REG              (SUNXI_SMC_BASE + 0x0070)
#define SMC_MST1_BYP_REG              (SUNXI_SMC_BASE + 0x0074)
#define SMC_MST2_BYP_REG              (SUNXI_SMC_BASE + 0x0078)

#define SMC_MST0_SEC_REG              (SUNXI_SMC_BASE + 0x0080)
#define SMC_MST1_SEC_REG              (SUNXI_SMC_BASE + 0x0084)
#define SMC_MST2_SEC_REG              (SUNXI_SMC_BASE + 0x0088)

#define SMC_MST0_ATTR_REG             (SUNXI_SMC_BASE + 0x0090)
#define SMC_MST1_ATTR_REG             (SUNXI_SMC_BASE + 0x0094)
#define SMC_MST2_ATTR_REG             (SUNXI_SMC_BASE + 0x0098)

#define DRM_BITMAP_CTRL_REG           (SUNXI_SMC_BASE + 0x00A0)
#define DRM_BITMAP_VAL_REG            (SUNXI_SMC_BASE + 0x00A4)
#define DRM_BITMAP_SEL_REG            (SUNXI_SMC_BASE + 0x00A8)
#define DRM_ERROR_REG				  (SUNXI_SMC_BASE + 0x00AC)

#define DRM_GPU_HW_RST_REG            (SUNXI_SMC_BASE + 0x00B0)
#define DRM_VERSION_REG               (SUNXI_SMC_BASE + 0x00F0)


#define SMC_REGIN_SETUP_LOW_REG(x)    (SUNXI_SMC_BASE + 0x100 + 0x10*(x))
#define SMC_REGIN_SETUP_HIGH_REG(x)   (SUNXI_SMC_BASE + 0x104 + 0x10*(x))
#define SMC_REGIN_ATTRIBUTE_REG(x)    (SUNXI_SMC_BASE + 0x108 + 0x10*(x))

#elif defined(CFG_SUNXI_SMC_10)
#define SMC_CONFIG_REG                (SUNXI_SMC_BASE + 0x0000)
#define SMC_ACTION_REG                (SUNXI_SMC_BASE + 0x0004)
#define SMC_LKDW_RANGE_REG            (SUNXI_SMC_BASE + 0x0008)
#define SMC_LKDW_SELECT_REG           (SUNXI_SMC_BASE + 0x000c)
#define SMC_INT_STATUS_REG            (SUNXI_SMC_BASE + 0x0010)
#define SMC_INT_CLEAR_REG             (SUNXI_SMC_BASE + 0x0014)

#define SMC_MASTER_BYPASS0_REG        (SUNXI_SMC_BASE + 0x0018)
#define SMC_MASTER_SECURITY0_REG      (SUNXI_SMC_BASE + 0x001c)
#define SMC_FAIL_ADDR_REG             (SUNXI_SMC_BASE + 0x0020)
#define SMC_FAIL_CTRL_REG             (SUNXI_SMC_BASE + 0x0028)
#define SMC_FAIL_ID_REG               (SUNXI_SMC_BASE + 0x002c)

#define SMC_SPECULATION_CTRL_REG      (SUNXI_SMC_BASE + 0x0030)
#define SMC_INVER_EN_REG              (SUNXI_SMC_BASE + 0x0034)

#define SMC_MST_ATTRI_REG             (SUNXI_SMC_BASE + 0x0048)
#define SMC_DRAM_EN_REG               (SUNXI_SMC_BASE + 0x0050)

#define SMC_DRAM_ILLEGAL_ACCESS0_REG  (SUNXI_SMC_BASE + 0x0058)

#define SMC_LOW_SADDR_REG             (SUNXI_SMC_BASE + 0x0060)
#define SMC_LOW_EADDR_REG             (SUNXI_SMC_BASE + 0x0068)

#define SMC_REGIN_SETUP_LOW_REG(x)    (SUNXI_SMC_BASE + 0x100 + 0x10*(x))
#define SMC_REGIN_SETUP_HIGH_REG(x)   (SUNXI_SMC_BASE + 0x104 + 0x10*(x))
#define SMC_REGIN_ATTRIBUTE_REG(x)    (SUNXI_SMC_BASE + 0x108 + 0x10*(x))

#else
#error "Unsupported plat"
#endif

#ifndef __ASSEMBLY__
struct sec_mem_region {
	u64 startAddr;
	u64 size;
};
/*
 * sec_mem_map is platform related
 * define in platform related board.c
 */
extern struct sec_mem_region sec_mem_map[];
int sunxi_smc_config(struct sec_mem_region *sec_mem_map);
int sunxi_drm_config(u32 dram_end, u32 drm_region_size);
#endif

#endif    /*  #ifndef _TZASC_SMC_H_  */
