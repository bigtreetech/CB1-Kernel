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

#ifndef __HALRF_DPK_8192F_H__
#define __HALRF_DPK_8192F_H__

/*--------------------------Define Parameters-------------------------------*/
#define DPK_RF_PATH_NUM_8192F 2
#define DPK_GROUP_NUM_8192F 3
#define DPK_MAC_REG_NUM_8192F 5
#define DPK_BB_REG_NUM_8192F 28
#define DPK_RF_REG_NUM_8192F 4
#define DPK_GAINLOSS_DBG_8192F 0
#define DPK_PAS_DBG_8192F 0
#define DPK_SRAM_IQ_DBG_8192F 0
#define DPK_SRAM_read_DBG_8192F 0
#define DPK_SRAM_write_DBG_8192F 0
#define DPK_DO_PATH_A 1
#define DPK_DO_PATH_B 1
#define RF_T_METER_8192F RF_0x42
#define DPK_THRESHOLD_8192F 6


void phy_dpk_track_8192f(
	struct dm_struct *dm);

void phy_dpkoff_8192f(
	struct dm_struct *dm);

void phy_dpkon_8192f(
	struct dm_struct *dm);

void phy_path_a_dpk_init_8192f(
	struct dm_struct *dm);

void phy_path_b_dpk_init_8192f(
	struct dm_struct *dm);

u8 phy_dpk_channel_transfer_8192f(
	struct dm_struct *dm);

u8 phy_lut_sram_read_8192f(
	struct dm_struct *dm,
	u8 k);

void phy_lut_sram_write_8192f(
	struct dm_struct *dm);

void phy_path_a_dpk_enable_8192f(
	struct dm_struct *dm);

void phy_path_b_dpk_enable_8192f(
	struct dm_struct *dm);

void phy_dpk_enable_disable_8192f(
	struct dm_struct *dm);

void dpk_sram_read_8192f(
	void *dm_void);

void dpk_reload_8192f(
	void *dm_void);

void do_dpk_8192f(
	void *dm_void);


#endif /* #ifndef __HAL_PHY_RF_8192F_H__*/
