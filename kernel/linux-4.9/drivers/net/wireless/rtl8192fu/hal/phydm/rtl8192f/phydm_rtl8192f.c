/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/

#if 0
	/* ============================================================
	*  include files
	* ============================================================ */
#endif

#include "mp_precomp.h"
#include "../phydm_precomp.h"

#if (RTL8192F_SUPPORT == 1)

s8 phydm_cckrssi_8192f(struct dm_struct *dm, u8 lna_idx, u8 vga_idx)
{
	s8 rx_pwr_all = 0x00;

	switch (lna_idx) {
	case 7:
		rx_pwr_all = -44 - (2 * vga_idx);
		break;
	case 5:
		rx_pwr_all = -28 - (2 * vga_idx);
		break;
	case 3:
		rx_pwr_all = -10 - (2 * vga_idx);
		break;
	case 0:
		rx_pwr_all = 14 - (2 * vga_idx);
		break;
	default:
		break;
	}

	return rx_pwr_all;
}

void phydm_dynamic_disable_ecs_8192f(
	struct dm_struct *dm)
{
	struct phydm_fa_struct *fa_cnt = (struct phydm_fa_struct *)phydm_get_structure(dm, PHYDM_FALSEALMCNT);

	if (dm->is_disable_dym_ecs == true || (*dm->mp_mode == true)) /*use mib to disable this dym function*/
		return;

	if (dm->rssi_min < 30 || (fa_cnt->cnt_all * 4 >= fa_cnt->cnt_cca_all))
		odm_set_bb_reg(dm, R_0x9ac, BIT(17), 0);
	else if ((dm->rssi_min >= 34) && (fa_cnt->cnt_all * 5 <= fa_cnt->cnt_cca_all))
		odm_set_bb_reg(dm, R_0x9ac, BIT(17), 1);
}

void phydm_dynamic_ant_weighting_8192f(
	struct dm_struct *dm)
{
	u8 rssi_l2h = 43, rssi_h2l = 37;

	if (dm->is_disable_dym_ant_weighting)
		return;

	/* force AGC weighting */
	odm_set_bb_reg(dm, R_0xc54, BIT(0), 1);
	/* MRC by AGC table */
	odm_set_bb_reg(dm, R_0xce8, BIT(30), 1);
	/* Enable antenna_weighting_shift mechanism */
	odm_set_bb_reg(dm, R_0xd5c, BIT(29), 1);

	if (dm->rssi_min_by_path != 0xFF) {
		if (dm->rssi_min_by_path >= rssi_l2h) {
			odm_set_bb_reg(dm, R_0xd5c, (BIT(31) | BIT(30)), 0); /*equal weighting*/
		} else if (dm->rssi_min_by_path <= rssi_h2l) {
			odm_set_bb_reg(dm, R_0xd5c, (BIT(31) | BIT(30)), 1); /*fix sec_min_wgt = 1/2*/
		}
	} else {
		odm_set_bb_reg(dm, R_0xd5c, (BIT(31) | BIT(30)), 1); /*fix sec_min_wgt = 1/2*/
	}
}

void phydm_dynamic_cs_soml_8192f(struct dm_struct *dm)
{
	u8 rssi_l2h = 46, rssi_h2l = 42;

	if (dm->rssi_min != 0xFF) {
		if ((dm->rssi_min >= rssi_l2h) && !(dm->support_ability & ODM_BB_ADAPTIVE_SOML)) {
			phydm_enable_adaptive_soml(dm);
			odm_set_bb_reg(dm, 0xd50, 0xff00000, 0x72);
		} else if ((dm->rssi_min <= rssi_h2l) && (dm->support_ability & ODM_BB_ADAPTIVE_SOML)) {
			phydm_stop_adaptive_soml(dm);
			odm_set_bb_reg(dm, 0xd50, 0xff00000, 0x55);
		}
	} else {
		odm_set_bb_reg(dm, 0xd50, 0xff00000, 0x72);
	}
}

void phydm_hwsetting_8192f(
	struct dm_struct *dm)
{
	/*phydm_dynamic_disable_ecs_8192f(p_dm);*/
	/*phydm_dynamic_ant_weighting_8192f(dm);*/
	phydm_dynamic_cs_soml_8192f(dm);
}

#endif /* RTL8192F_SUPPORT == 1 */
