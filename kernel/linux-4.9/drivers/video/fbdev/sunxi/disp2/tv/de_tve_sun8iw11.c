/*
 * Copyright (C) 2015 Allwinnertech, z.q <zengqi@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include "drv_tv.h"
#include "de_tvec.h"
#include <linux/sunxi-sid.h>

static unsigned int dac_info[TVE_DEVICE_NUM][TVE_DAC_NUM];
static void __iomem *tve_reg_base[TVE_DEVICE_NUM];
static void __iomem *tve_top_reg_base[TVE_TOP_DEVIVE_NUM];
static unsigned int dac_type_info[TVE_DEVICE_NUM][TVE_DAC_NUM];

s32 tve_low_set_reg_base(u32 sel, void __iomem *address)
{
	if (sel > 2)
		pr_err("sel is %d,err!", sel);
	else
		tve_reg_base[sel] = address;
	return 0;
}

s32 tve_low_set_top_reg_base(void __iomem *address)
{
	tve_top_reg_base[0] = address;
	return 0;
}

s32 tve_low_sw_init(u32 sel, u32 *dac_no, u32 *dac_type,
			u32 num)
{
	int i = 0;

	for (i = 0; i < TVE_DAC_NUM; i++)
		dac_type_info[sel][i] = 7;

	for (i = 0; i < num; i++) {
		dac_info[sel][dac_no[i]] = 1;
		dac_type_info[sel][dac_no[i]] = dac_type[i];
	}

	return 0;
}

s32 tve_low_init(u32 sel, u32 *dac_no, u32 *cali, s32 *offset, u32 *dac_type,
			u32 num)
{
	u32 i = 0;
	s32 dac_cal = 0;
	u32 dac_type_set = 0;

	/* init dac config as disable status first */
	for (i = 0; i < TVE_DAC_NUM; i++)
		dac_type_info[sel][i] = 7;

	for (i = 0; i < num; i++) {
		dac_info[sel][dac_no[i]] = 1;
		dac_type_info[sel][dac_no[i]] = dac_type[i];
	}

	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i]) {
			dac_cal = (s32)cali[i] + offset[i];
			TVE_TOP_WUINT32(TVE_TOP_020 + 0x020*i,
					(i<<4)|((sel+1)<<0));
			TVE_TOP_WUINT32(TVE_TOP_02C + 0x020*i, 0x023a);
			TVE_TOP_WUINT32(TVE_TOP_030 + 0x020*i, 0x0010);
			TVE_TOP_WUINT32(TVE_TOP_028 + 0x020*i,
					((0x8000|dac_cal)<<16)|0x4200);
		}

	}

	for (i = 0; i < TVE_DAC_NUM; i++)
		dac_type_set |= dac_type_info[sel][i]<<(4+3*i);

	if (num == 1) { /*cvbs*/
		if (dac_no[0] == 1) {
			dac_type_set &= 0xffffff80;
		} else if (dac_no[0] == 2) {
			dac_type_set &= 0xffff1ff0;
		}
	}

	TVE_WUINT32(sel, TVE_008, dac_type_set);

	return 0;
}

s32 tve_low_dac_enable(u32 sel)
{
	u32 i = 0;

	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i])
			TVE_TOP_SET_BIT(TVE_TOP_028 + 0x020*i, 1<<0);
	}

	return 0;
}

s32 tve_low_dac_disable(u32 sel)
{
	u32 i = 0;

	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i])
			TVE_TOP_CLR_BIT(TVE_TOP_028 + 0x020*i, 1<<0);
	}

	return 0;
}

s32 tve_low_open(u32 sel)
{
	TVE_SET_BIT(sel, TVE_000, 0x1<<31);
	TVE_SET_BIT(sel, TVE_000, 0x1<<0);
	return 0;
}

s32 tve_low_close(u32 sel)
{
	TVE_CLR_BIT(sel, TVE_000, 0x1<<31);
	TVE_CLR_BIT(sel, TVE_000, 0x1<<0);
	return 0;
}

s32 tve_low_set_tv_mode(u32 sel, enum disp_tv_mode mode, u32 *cali)
{
	u32 deflick  = 0;
	u32 reg_sync = 0;

	switch (mode) {
	case DISP_TV_MOD_PAL:
		deflick  = 1;
		reg_sync = 0x2005000a;
		TVE_WUINT32(sel, TVE_004, 0x07060001);
		TVE_WUINT32(sel, TVE_00C, 0x02004000);
		TVE_WUINT32(sel, TVE_010, 0x2a098acb);
		TVE_WUINT32(sel, TVE_014, 0x008a0018);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x00160271);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000001);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_10C, 0x00002929);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000fc);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00010000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x200f000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000000);
		TVE_WUINT32(sel, TVE_3A0, 0x00030001);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 1<<16);
		TVE_WUINT32(sel, TVE_000, 0x00300000);
		TVE_CLR_BIT(sel, TVE_000, 0x1<<0);
		TVE_WUINT32(sel, TVE_130, reg_sync);
		TVE_WUINT32(sel, TVE_014, 0x00820020);
		TVE_WUINT32(sel, TVE_130, 0x20050013);
		TVE_WUINT32(sel, TVE_380, (deflick == 0)
					? 0x00000000 : 0x0<<10 | 0x3<<8);
		break;
	case DISP_TV_MOD_NTSC:
		deflick  = 1;
		reg_sync = 0x20050368;
		TVE_WUINT32(sel, TVE_004, 0x07060000);
		TVE_WUINT32(sel, TVE_00C, 0x02004000);
		TVE_WUINT32(sel, TVE_010, 0x21f07c1f);
		TVE_WUINT32(sel, TVE_014, 0x00760020);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x0016020d);
		TVE_WUINT32(sel, TVE_020, 0x00f000f0);
		TVE_WUINT32(sel, TVE_100, 0x00000001);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000002);
		TVE_WUINT32(sel, TVE_10C, 0x0000004f);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a0a0);
		TVE_WUINT32(sel, TVE_11C, 0x001000f0);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00000000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x200f000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000001);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 1<<16);
		TVE_WUINT32(sel, TVE_000, 0x00300000);
		TVE_CLR_BIT(sel, TVE_000, 0x1<<0);
		TVE_WUINT32(sel, TVE_130, reg_sync);
		TVE_WUINT32(sel, TVE_130, 0x20050364);
		TVE_WUINT32(sel, TVE_380, (deflick == 0)
					? 0x00000000 : 0x0<<10 | 0x3<<8);
		break;
	case DISP_TV_MOD_480I:
		TVE_WUINT32(sel, TVE_004, 0x07060000);
		TVE_WUINT32(sel, TVE_00C, 0x02004000);
		TVE_WUINT32(sel, TVE_010, 0x21f07c1f);
		TVE_WUINT32(sel, TVE_014, 0x00760020);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x0016020d);
		TVE_WUINT32(sel, TVE_020, 0x00f000f0);
		TVE_WUINT32(sel, TVE_100, 0x00000001);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000002);
		TVE_WUINT32(sel, TVE_10C, 0x0000004f);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a0a0);
		TVE_WUINT32(sel, TVE_11C, 0x001000f0);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00000000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x200f000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000001);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 1<<16);
		TVE_WUINT32(sel, TVE_000, 0x00300000);

		break;
	case DISP_TV_MOD_576I:
		TVE_WUINT32(sel, TVE_004, 0x07060001);
		TVE_WUINT32(sel, TVE_00C, 0x02004000);
		TVE_WUINT32(sel, TVE_010, 0x2a098acb);
		TVE_WUINT32(sel, TVE_014, 0x008a0018);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x00160271);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000001);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_10C, 0x00002929);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000fc);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00010000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x200f000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000000);
		TVE_WUINT32(sel, TVE_3A0, 0x00030001);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 1<<16);
		TVE_WUINT32(sel, TVE_000, 0x00300000);
		break;
	case DISP_TV_MOD_480P:
		TVE_WUINT32(sel, TVE_004, 0x07040002);
		TVE_WUINT32(sel, TVE_00C, 0x00002000);
		TVE_WUINT32(sel, TVE_014, 0x00760020);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x002C020D);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000001);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000002);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000f0);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00010000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000E000C);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00400000);
		break;
	case DISP_TV_MOD_576P:
		TVE_WUINT32(sel, TVE_004, 0x07060003);
		TVE_WUINT32(sel, TVE_00C, 0x00002000);
		TVE_WUINT32(sel, TVE_014, 0x008a0018);
		TVE_WUINT32(sel, TVE_018, 0x00000016);
		TVE_WUINT32(sel, TVE_01C, 0x002C0271);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000001);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x0016447e);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000F0);
		TVE_WUINT32(sel, TVE_124, 0x000005a0);
		TVE_WUINT32(sel, TVE_128, 0x00010000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x800B000C);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x00000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00400000);
		break;
	case DISP_TV_MOD_720P_60HZ:
		TVE_WUINT32(sel, TVE_004, 0x0706000a);
		TVE_WUINT32(sel, TVE_00C, 0x00003000);
		TVE_WUINT32(sel, TVE_014, 0x01040046);
		TVE_WUINT32(sel, TVE_018, 0x05000046);
		TVE_WUINT32(sel, TVE_01C, 0x001902EE);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000001);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0xDC280228);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000F0);
		TVE_WUINT32(sel, TVE_124, 0x00000500);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000C0008);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;
	case DISP_TV_MOD_720P_50HZ:
		TVE_WUINT32(sel, TVE_004, 0x0706000a);
		TVE_WUINT32(sel, TVE_00C, 0x00003000);
		TVE_WUINT32(sel, TVE_014, 0x01040190);
		TVE_WUINT32(sel, TVE_018, 0x05000190);
		TVE_WUINT32(sel, TVE_01C, 0x001902EE);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000001);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0xDC280228);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000F0);
		TVE_WUINT32(sel, TVE_124, 0x00000500);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000E000C);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;
	case DISP_TV_MOD_1080I_60HZ:
		TVE_WUINT32(sel, TVE_004, 0x0706000c);
		TVE_WUINT32(sel, TVE_00C, 0x00003010);
		TVE_WUINT32(sel, TVE_014, 0x00C0002C);
		TVE_WUINT32(sel, TVE_018, 0x0370002C);
		TVE_WUINT32(sel, TVE_01C, 0x00140465);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x582C442C);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000F0);
		TVE_WUINT32(sel, TVE_124, 0x00000780);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000E0008);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;
	case DISP_TV_MOD_1080I_50HZ:
		TVE_WUINT32(sel, TVE_004, 0x0706000c);
		TVE_WUINT32(sel, TVE_00C, 0x00003010);
		TVE_WUINT32(sel, TVE_014, 0x00C001E4);
		TVE_WUINT32(sel, TVE_018, 0x03700108);
		TVE_WUINT32(sel, TVE_01C, 0x00140465);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x582C442C);
		TVE_WUINT32(sel, TVE_118, 0x0000a8a8);
		TVE_WUINT32(sel, TVE_11C, 0x001000F0);
		TVE_WUINT32(sel, TVE_124, 0x00000780);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000E0008);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;
	case DISP_TV_MOD_1080P_60HZ:
		TVE_WUINT32(sel, TVE_004, 0x0004000e);
		TVE_WUINT32(sel, TVE_00C, 0x00003010);
		TVE_WUINT32(sel, TVE_010, 0x21f07c1f);
		TVE_WUINT32(sel, TVE_014, 0x00c0002c);
		TVE_WUINT32(sel, TVE_018, 0x07bc002c);
		TVE_WUINT32(sel, TVE_01C, 0x00290465);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000001);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x582c022c);
		TVE_WUINT32(sel, TVE_118, 0x0000a0a0);
		TVE_WUINT32(sel, TVE_11C, 0x001000f0);
		TVE_WUINT32(sel, TVE_124, 0x00000780);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000e000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;
	case DISP_TV_MOD_1080P_50HZ:
		TVE_WUINT32(sel, TVE_004, 0x0004000e);
		TVE_WUINT32(sel, TVE_00C, 0x00003010);
		TVE_WUINT32(sel, TVE_010, 0x21f07c1f);
		TVE_WUINT32(sel, TVE_014, 0x00C001E4);
		TVE_WUINT32(sel, TVE_018, 0x07BC01E4);
		TVE_WUINT32(sel, TVE_01C, 0x00290465);
		TVE_WUINT32(sel, TVE_020, 0x00fc00fc);
		TVE_WUINT32(sel, TVE_100, 0x00000000);
		TVE_WUINT32(sel, TVE_104, 0x00000000);
		TVE_WUINT32(sel, TVE_108, 0x00000005);
		TVE_WUINT32(sel, TVE_110, 0x00000000);
		TVE_WUINT32(sel, TVE_114, 0x582c022c);
		TVE_WUINT32(sel, TVE_118, 0x0000a0a0);
		TVE_WUINT32(sel, TVE_11C, 0x001000f0);
		TVE_WUINT32(sel, TVE_124, 0x00000780);
		TVE_WUINT32(sel, TVE_128, 0x00030000);
		TVE_WUINT32(sel, TVE_12C, 0x00000101);
		TVE_WUINT32(sel, TVE_130, 0x000e000c);
		TVE_WUINT32(sel, TVE_134, 0x00000000);
		TVE_WUINT32(sel, TVE_138, 0x00000000);
		TVE_WUINT32(sel, TVE_13C, 0x07000000);
		TVE_WUINT32(sel, TVE_0F8, 0x00000200);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
		TVE_WUINT32(sel, TVE_380, 0x00000000);
		TVE_INIT_BIT(sel, TVE_004, 0x1<<16, 0<<16);
		TVE_WUINT32(sel, TVE_000, 0x00000000);
		break;

	case DISP_VGA_MOD_640_480P_60:
	case DISP_VGA_MOD_800_600P_60:
	case DISP_VGA_MOD_1024_768P_60:
	case DISP_VGA_MOD_1280_768P_60:
	case DISP_VGA_MOD_1280_800P_60:
	case DISP_VGA_MOD_1366_768P_60:
	case DISP_VGA_MOD_1440_900P_60:
	case DISP_VGA_MOD_1920_1080P_60:
	case DISP_VGA_MOD_1600_900P_60:
	case DISP_VGA_MOD_1920_1200P_60:
		TVE_WUINT32(sel, TVE_004, 0x08000000);
		break;

	default:
		break;
	}

	return 0;
}

/* 0:unconnected; 1:connected; */
s32 tve_low_get_dac_status(u32 sel)
{
	u32 readval = 0;
	u32 result = 0;
	u32 i = 0;

	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i] == 1) {
			readval = (TVE_RUINT32(sel, TVE_038)>>(8*i))&3;
			if (readval == 1) {
				result = 1;
				break;
			}
		}
	}
	return result;
}

s32 tve_low_dac_autocheck_enable(u32 sel)
{
	u8 i = 0;

	TVE_SET_BIT(sel, TVE_000, 0x80000000);	/* tvclk enable */
	TVE_WUINT32(sel, TVE_0F8, 0x00000200);
	TVE_WUINT32(sel, TVE_0FC, 0x0A3C00FF);	/* 20ms x 10 */
	TVE_WUINT32(sel, TVE_03C, 0x00009999);
#if defined(CONFIG_ARCH_SUN50IW9)
	TVE_SET_BIT(sel, TVE_030, 0x80000000);/* new detect mode */
#endif
	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i] == 1)
			TVE_SET_BIT(sel, TVE_030, 1<<i);/* detect enable */
	}

	return 0;
}

s32 tve_low_dac_autocheck_disable(u32 sel)
{
	u8 i = 0;

	for (i = 0; i < TVE_DAC_NUM; i++) {
		if (dac_info[sel][i] == 1)
			TVE_CLR_BIT(sel, TVE_030, 1<<i);
	}
	return 0;
}

u32 tve_low_get_sid(u32 index)
{
#if defined(CONFIG_ARCH_SUN50IW9)
	u32 efuse = 0;

	efuse = (readl(ioremap(0x0300622c, 4)) >> 16) +
		(readl(ioremap(0x03006230, 4)) << 16);

	if (efuse > 5)
		efuse -= 5;

	return efuse;
#else
	u32 efuse[TVE_DAC_NUM] = {0};
	char tv_name[20] = {0};

#if defined(CONFIG_ARCH_SUN8IW12)
	snprintf(tv_name, sizeof(tv_name), EFUSE_TVE_NAME);
#else
	snprintf(tv_name, sizeof(tv_name), "tvout");
#endif

	if (sunxi_efuse_readn(tv_name, &efuse, 128))
		pr_err("get TV%d efuse fail!\n", index);
	else
		return efuse[index];
#endif
	return 0;
}

s32 tve_adjust_resync(u32 sel, s32 resync_pixel_num, s32 resync_line_num)
{
	u32 temp = 0;

	temp = TVE_RUINT32(sel, TVE_130);
	if (resync_pixel_num >= 0) {
		temp &= 0xfffff800;
		temp |= (resync_pixel_num & 0x000007ff);
	}

	if (resync_line_num >= 0) {
		temp &= 0xf800ffff;
		temp |= ((resync_line_num << 16) & 0x07ff0000);
	}

	TVE_WUINT32(sel, TVE_130, temp);
	return 0;
}


s32 tve_low_enhance(u32 sel, u32 mode)
{
	if (mode == 0) {
		TVE_CLR_BIT(sel, TVE_000, 0xf<<10); /* deflick off */
		TVE_SET_BIT(sel, TVE_000, 0x5<<10); /* deflick is 5 */
		TVE_SET_BIT(sel, TVE_00C, 0x1<<31); /* lti on */
		TVE_SET_BIT(sel, TVE_00C, 0x1<<16); /* notch off */
	} else if (mode == 1) {
		TVE_CLR_BIT(sel, TVE_000, 0xf<<10);
		TVE_SET_BIT(sel, TVE_000, 0x5<<10);
		TVE_SET_BIT(sel, TVE_00C, 0x1<<31);
		TVE_CLR_BIT(sel, TVE_00C, 0x1<<16);
	} else if (mode == 2) {
		TVE_CLR_BIT(sel, TVE_000, 0xf<<10);
		TVE_CLR_BIT(sel, TVE_00C, 0x1<<31);
		TVE_SET_BIT(sel, TVE_00C, 0x1<<16);
	} else if (mode == 3) {
		TVE_WUINT32(sel, TVE_380, 0x0<<10 | 0x3<<8);
		TVE_WUINT32(sel, TVE_3A0, 0x00030001);
		TVE_WUINT32(sel, TVE_000, 0x00300001);
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
	} else if (mode == 4) {
		TVE_WUINT32(sel, TVE_380, 0x0<<10 | 0x3<<8);
		TVE_WUINT32(sel, TVE_3A0, 0x0);
		TVE_WUINT32(sel, TVE_000, 0x00200001); /*108M*/
		TVE_WUINT32(sel, TVE_120, 0x01e80320);
	}  else if (mode == 5) {
		TVE_WUINT32(sel, TVE_380, 0x0<<10 | 0x3<<8);
		TVE_WUINT32(sel, TVE_3A0, 0x00030000);
		TVE_WUINT32(sel, TVE_000, 0x00300001);
		TVE_WUINT32(sel, TVE_120, 0x01e8031d);
	}
	return 0;
}
