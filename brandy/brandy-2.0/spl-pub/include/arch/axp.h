/*
 * (C) Copyright 2018-2020
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 * axp.h
 */

#ifndef _AXP_H_
#define _AXP_H_

#include <common.h>

#define pmu_err(format, arg...) printf("[pmu]: " format, ##arg)
#define pmu_info(format, arg...) /*printf("[pmu]: "format,##arg)*/

typedef struct _axp_step_info {
	u32 step_min_vol;
	u32 step_max_vol;
	u32 step_val;
	u32 regation;
} _axp_step_info;

typedef struct _axp_contrl_info {
	char name[16];

	u32 min_vol;
	u32 max_vol;
	u32 cfg_reg_addr;
	u32 cfg_reg_mask;
	u32 ctrl_reg_addr;
	u32 ctrl_bit_ofs;
	u32 reg_addr_offest;
	_axp_step_info axp_step_tbl[4];

} axp_contrl_info;


#define AXP_BOOT_SOURCE_BUTTON         0
#define AXP_BOOT_SOURCE_IRQ_LOW                1
#define AXP_BOOT_SOURCE_VBUS_USB       2
#define AXP_BOOT_SOURCE_CHARGER                3
#define AXP_BOOT_SOURCE_BATTERY                4


#endif
