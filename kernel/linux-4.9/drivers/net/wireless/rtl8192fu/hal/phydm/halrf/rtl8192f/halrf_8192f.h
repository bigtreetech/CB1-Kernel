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

#ifndef __HALRF_8192F_H__
#define __HALRF_8192F_H__

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN))
#if RT_PLATFORM == PLATFORM_MACOSX
#include "halphyrf_win.h"
#else
#include "../halrf/halphyrf_win.h"
#endif
#elif (DM_ODM_SUPPORT_TYPE & (ODM_CE))
#include "../halphyrf_ce.h"
#elif (DM_ODM_SUPPORT_TYPE & (ODM_AP))
#include "../halphyrf_ap.h"
#endif

/*--------------------------Define Parameters-------------------------------*/

#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
#define IQK_DELAY_TIME_92F 15 /* ms */
#else
#define IQK_DELAY_TIME_92F 10
#endif

#define index_mapping_NUM_92F 15

#define AVG_THERMAL_NUM_92F 4
#define RF_T_METER_92F 0x42

void configure_txpower_track_8192f(
	struct txpwrtrack_cfg *p_config);

void get_delta_swing_table_8192f(
	void *dm_void,
	u8 **temperature_up_a,
	u8 **temperature_down_a,
	u8 **temperature_up_b,
	u8 **temperature_down_b);

void get_delta_all_swing_table_8192f(
	void *dm_void,
	u8 **temperature_up_a,
	u8 **temperature_down_a,
	u8 **temperature_up_b,
	u8 **temperature_down_b,
	u8 **temperature_up_cck_a,
	u8 **temperature_down_cck_a,
	u8 **temperature_up_cck_b,
	u8 **temperature_down_cck_b

	);

void get_delta_swing_xtal_table_8192f(
	void *dm_void,
	s8 **temperature_up_xtal,
	s8 **temperature_down_xtal);

#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
void do_iqk_8192f(
	void *dm_void,
	u8 delta_thermal_index,
	u8 thermal_value,
	u8 threshold);
#else
void do_iqk_8192f(
	void *dm_void,
	u8 delta_thermal_index,
	u8 thermal_value,
	u8 threshold);
#endif

void odm_tx_pwr_track_set_pwr92_f(
	void *dm_void,
	enum pwrtrack_method method,
	u8 rf_path,
	u8 channel_mapped_index);

void odm_tx_pwr_track_set_pwr_8192f(
	void *dm_void,
	enum pwrtrack_method method,
	u8 rf_path,
	u8 channel_mapped_index);

void odm_txxtaltrack_set_xtal_8192f(
	void *dm_void);

/* 1 7.	IQK */

void phy_iq_calibrate_8192f(
	void *dm_void,
	boolean is_recovery);

/*
 * LC calibrate
 *   */
void _phy_lc_calibrate_8192f(struct dm_struct *dm, boolean is2T);

#ifdef CONFIG_2G_BAND_SHIFT
void _phy_lck_2g_band_shift_8192f(struct dm_struct *dm, u8 band_type);
#endif

void phy_lc_calibrate_8192f(
	void *dm_void);

/*
 * AP calibrate
 *   */
#if 0
void
phy_ap_calibrate_8192e(
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct dm_struct		*dm,
#else
	struct _ADAPTER	*p_adapter,
#endif
	s8		delta);
void
phy_digital_predistortion_8192e(struct _ADAPTER	*p_adapter);
#endif

void _phy_save_adda_registers_92f(
	struct dm_struct *dm,
	u32 *adda_reg,
	u32 *adda_backup,
	u32 register_num);

void _phy_path_adda_on_92f(
	struct dm_struct *dm,
	boolean is_iqk);

void _phy_mac_setting_calibration_92f(
	struct dm_struct *dm,
	u32 *mac_reg,
	u32 *mac_backup);

#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
void _phy_path_a_stand_by(struct dm_struct *dm);
#endif

void halrf_rf_lna_setting_8192f(
	struct dm_struct *dm,
	enum halrf_lna_set type);

#endif /*__HALRF_8192F_H__*/
