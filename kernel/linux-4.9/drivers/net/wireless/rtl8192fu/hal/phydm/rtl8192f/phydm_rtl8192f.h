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

#ifndef __ODM_RTL8192F_H__
#define __ODM_RTL8192F_H__

#if (RTL8192F_SUPPORT == 1)
s8 phydm_cckrssi_8192f(struct dm_struct *dm, u8 lna_idx, u8 vga_idx);

void phydm_phypara_a_cut_8192f(
	struct dm_struct *dm);

void phydm_dynamic_disable_ecs_8192f(
	struct dm_struct *dm);

void phydm_dynamic_ant_weighting_8192f(
	struct dm_struct *dm);

void phydm_hwsetting_8192f(
	struct dm_struct *dm);

#endif /* #define __ODM_RTL8192F_H__ */
#endif
