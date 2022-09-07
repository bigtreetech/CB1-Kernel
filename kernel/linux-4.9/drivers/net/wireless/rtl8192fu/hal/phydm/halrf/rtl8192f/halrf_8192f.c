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

#include "mp_precomp.h"
/*#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)*/
#if (DM_ODM_SUPPORT_TYPE == 0x08) /*[PHYDM-262] workaround for SD4 compile warning*/
#if RT_PLATFORM == PLATFORM_MACOSX
#include "phydm_precomp.h"
#else
#include "../phydm_precomp.h"
#endif
#else
#include "../../phydm_precomp.h"
#endif

#if (RTL8192F_SUPPORT == 1)

/*---------------------------Define Local Constant---------------------------*/
/* 2010/04/25 MH Define the max tx power tracking tx agc power. */
#define ODM_TXPWRTRACK_MAX_IDX_92F 6

/*---------------------------Define Local Constant---------------------------*/

/* 3============================================================
 * 3 Tx Power Tracking
 * 3============================================================ */

void halrf_rf_lna_setting_8192f(
	struct dm_struct *dm,
	enum halrf_lna_set type)
{
	/*phydm_disable_lna*/
	if (type == HALRF_LNA_DISABLE) {
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x1);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, 0xfffff, 0x18000); /*select Rx mode*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x31, 0xfffff, 0x0004f);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x32, 0xfffff, 0x37f82); /*disable LNA*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x0);

		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x1);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x30, 0xfffff, 0x18000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x31, 0xfffff, 0x0004f);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x32, 0xfffff, 0x37f82);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x0);

	} else if (type == HALRF_LNA_ENABLE) {
		/*phydm_enable_lna*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x1);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, 0xfffff, 0x18000); /*select Rx mode*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x31, 0xfffff, 0x0004f);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x32, 0xfffff, 0x77f82); /*back to normal*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x0);

		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x1);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x30, 0xfffff, 0x18000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x31, 0xfffff, 0x0004f);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x32, 0xfffff, 0x77f82);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x0);
	}
}

void set_iqk_matrix_8192f(
	struct dm_struct *dm,
	u8 OFDM_index,
	u8 rf_path,
	s32 iqk_result_x,
	s32 iqk_result_y)
{
/*AP Do DPK, so modify bbswing to default*/
#if (DM_ODM_SUPPORT_TYPE == ODM_AP) && defined(CONFIG_RF_DPK_SETTING_SUPPORT)
	switch (rf_path) {
	case RF_PATH_A:
		odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, MASKDWORD, ofdm_swing_table_new[OFDM_index]);
		break;

	case RF_PATH_B:
		odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, MASKDWORD, ofdm_swing_table_new[OFDM_index]);
		break;

	default:
		break;
	}
#else

	s32 ele_A = 0, ele_D, ele_C = 0, value32;

	ele_D = (ofdm_swing_table_new[OFDM_index] & 0xFFC00000) >> 22;

	/*new element A = element D x X*/
	if (iqk_result_x != 0 && (*dm->band_type == ODM_BAND_2_4G)) {
		if ((iqk_result_x & 0x00000200) != 0) /* consider minus */
			iqk_result_x = iqk_result_x | 0xFFFFFC00;
		ele_A = ((iqk_result_x * ele_D) >> 8) & 0x000003FF;

		/* new element C = element D x Y */
		if ((iqk_result_y & 0x00000200) != 0)
			iqk_result_y = iqk_result_y | 0xFFFFFC00;
		ele_C = ((iqk_result_y * ele_D) >> 8) & 0x000003FF;

		/*if (rf_path == RF_PATH_A)// Remove this to Fix path B PowerTracking */
		switch (rf_path) {
		case RF_PATH_A:
			/* wirte new elements A, C, D to regC80 and regC94, element B is always 0 */
			value32 = (ele_D << 22) | ((ele_C & 0x3F) << 16) | ele_A;
			odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, MASKDWORD, value32);

			value32 = (ele_C & 0x000003C0) >> 6;
			odm_set_bb_reg(dm, REG_OFDM_0_XC_TX_AFE, MASKH4BITS, value32);

			value32 = ((iqk_result_x * ele_D) >> 7) & 0x01;
			odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(24), value32);
			break;
		case RF_PATH_B:
			/* wirte new elements A, C, D to regC88 and regC9C, element B is always 0 */
			value32 = (ele_D << 22) | ((ele_C & 0x3F) << 16) | ele_A;
			odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, MASKDWORD, value32);

			value32 = (ele_C & 0x000003C0) >> 6;
			odm_set_bb_reg(dm, REG_OFDM_0_XD_TX_AFE, MASKH4BITS, value32);

			value32 = ((iqk_result_x * ele_D) >> 7) & 0x01;
			odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(28), value32);

			break;
		default:
			break;
		}
	} else {
		switch (rf_path) {
		case RF_PATH_A:
			odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, MASKDWORD, ofdm_swing_table_new[OFDM_index]);
			odm_set_bb_reg(dm, REG_OFDM_0_XC_TX_AFE, MASKH4BITS, 0x00);
			odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(24), 0x00);
			break;

		case RF_PATH_B:
			odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, MASKDWORD, ofdm_swing_table_new[OFDM_index]);
			odm_set_bb_reg(dm, REG_OFDM_0_XD_TX_AFE, MASKH4BITS, 0x00);
			odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(28), 0x00);
			break;

		default:
			break;
		}
	}

	RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
	       "TxPwrTracking path %c: X = 0x%x, Y = 0x%x ele_A = 0x%x ele_C = 0x%x ele_D = 0x%x 0xeb4 = 0x%x 0xebc = 0x%x\n",
	       (rf_path == RF_PATH_A ? 'A' : 'B'), (u32)iqk_result_x,
	       (u32)iqk_result_y, (u32)ele_A, (u32)ele_C, (u32)ele_D,
	       (u32)iqk_result_x, (u32)iqk_result_y);
#endif
}

#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
void do_iqk_8192f(
	void *dm_void,
	u8 delta_thermal_index,
	u8 thermal_value,
	u8 threshold)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	odm_reset_iqk_result(dm);

	dm->rf_calibrate_info.thermal_value_iqk = thermal_value;

	halrf_iqk_trigger(dm, false);
}
#else
/*Originally config->do_iqk is hooked phy_iq_calibrate_8192f, but do_iqk_8197f and phy_iq_calibrate_8192f have different arguments*/
void do_iqk_8192f(
	void *dm_void,
	u8 delta_thermal_index,
	u8 thermal_value,
	u8 threshold)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	/*boolean is_recovery = (boolean)delta_thermal_index;*/

	halrf_iqk_trigger(dm, false);

}
#endif
/*-----------------------------------------------------------------------------
 * Function:	odm_TxPwrTrackSetPwr92F()
 *
 * Overview:	92F change all channel tx power accordign to flag.
 *				OFDM & CCK are all different.
 *
 * Input:		NONE
 *
 * Output:		NONE
 *
 * Return:		NONE
 *
 * Revised History:
 *	When		Who		Remark
 *	04/23/2012	MHC		Create version 0.
 *
 *---------------------------------------------------------------------------*/
/*#if 1*/

void odm_tx_pwr_track_set_pwr92_f(
	void *dm_void,
	enum pwrtrack_method method,
	u8 rf_path,
	u8 channel_mapped_index)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct _ADAPTER *adapter = dm->adapter;
	PHAL_DATA_TYPE hal_data = GET_HAL_DATA(adapter);
#endif
	u8 pwr_tracking_limit_ofdm = 32; /* +2dB */
	u8 pwr_tracking_limit_cck = 32; /* +2dB */
	u8 tx_rate = 0xFF;
	s8 final_ofdm_swing_index = 0;
	s8 final_cck_swing_index = 0;
	u8 i = 0;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);
	struct _hal_rf_ *rf = &(dm->rf_table);

	if (*dm->mp_mode == true) {
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN | ODM_CE))
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
#if (MP_DRIVER == 1)
		PMPT_CONTEXT p_mpt_ctx = &(adapter->MptCtx);

		tx_rate = MptToMgntRate(p_mpt_ctx->MptRateIndex);
#endif
#elif (DM_ODM_SUPPORT_TYPE & ODM_CE)
#ifdef CONFIG_MP_INCLUDED
		PMPT_CONTEXT p_mpt_ctx = &(adapter->mppriv.mpt_ctx);

		tx_rate = mpt_to_mgnt_rate(p_mpt_ctx->mpt_rate_index);
#endif
#endif
#endif
	} else {
		u16 rate = *(dm->forced_data_rate);

		if (!rate) { /*auto rate*/
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
			tx_rate = adapter->HalFunc.GetHwRateFromMRateHandler(dm->tx_rate);
#elif (DM_ODM_SUPPORT_TYPE & ODM_CE)
			if (dm->number_linked_client != 0)
				tx_rate = hw_rate_to_m_rate(dm->tx_rate);
			else
				tx_rate = rf->p_rate_index;
#endif
		} else /*force rate*/
			tx_rate = (u8)rate;
	}



	RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "Power Tracking tx_rate=0x%X\n", tx_rate);
	RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "===>ODM_TxPwrTrackSetPwr8192F\n");

	if (tx_rate != 0xFF) { /* 20130429 Mimic Modify High rate BBSwing Limit. */
		/* 2 CCK */
		if ((tx_rate >= ODM_MGN_1M && tx_rate <= ODM_MGN_5_5M) || tx_rate == ODM_MGN_11M)
			pwr_tracking_limit_cck = 32; /* +2dB */
		/* 2 OFDM */
		else if ((tx_rate >= ODM_MGN_6M) && (tx_rate <= ODM_MGN_48M))
			pwr_tracking_limit_ofdm = 32; /* +2dB */
		else if (tx_rate == ODM_MGN_54M)
			pwr_tracking_limit_ofdm = 32; /* +2dB */

		/* 2 HT */
		else if ((tx_rate >= ODM_MGN_MCS0) && (tx_rate <= ODM_MGN_MCS2)) /* QPSK/BPSK */
			pwr_tracking_limit_ofdm = 32; /* +2dB */
		else if ((tx_rate >= ODM_MGN_MCS3) && (tx_rate <= ODM_MGN_MCS4)) /* 16QAM */
			pwr_tracking_limit_ofdm = 32; /* +3dB */
		else if ((tx_rate >= ODM_MGN_MCS5) && (tx_rate <= ODM_MGN_MCS7)) /* 64QAM */
			pwr_tracking_limit_ofdm = 32; /* +2dB */

		else if ((tx_rate >= ODM_MGN_MCS8) && (tx_rate <= ODM_MGN_MCS10)) /* QPSK/BPSK */
			pwr_tracking_limit_ofdm = 32; /* +2dB */
		else if ((tx_rate >= ODM_MGN_MCS11) && (tx_rate <= ODM_MGN_MCS12)) /* 16QAM */
			pwr_tracking_limit_ofdm = 32; /* +2dB */
		else if ((tx_rate >= ODM_MGN_MCS13) && (tx_rate <= ODM_MGN_MCS15)) /* 64QAM */
			pwr_tracking_limit_ofdm = 32; /* +2dB */

		else
			pwr_tracking_limit_ofdm = cali_info->default_ofdm_index; /* Default OFDM index = 30 */
	}

	RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
	       "pwr_tracking_limit_cck=%d      pwr_tracking_limit_ofdm=%d\n",
	       pwr_tracking_limit_cck, pwr_tracking_limit_ofdm);

	if (method == TXAGC) {
		u32 pwr = 0, tx_agc = 0;
		struct _ADAPTER *adapter = dm->adapter;

		RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "odm_TxPwrTrackSetPwr92F CH=%d\n",
		       *(dm->channel));

		cali_info->remnant_ofdm_swing_idx[rf_path] = cali_info->absolute_ofdm_swing_idx[rf_path]; /* Remnant index equal to aboslute compensate value. */

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN | ODM_CE))

#if (MP_DRIVER != 1)
		/* PHY_SetTxPowerLevelByPath8192F(adapter, *dm->channel, rf_path);   */ /* Using new set power function */
		/* phy_set_tx_power_level8192f(dm->adapter, *dm->channel); */
		cali_info->modify_tx_agc_flag_path_a = true;
		cali_info->modify_tx_agc_flag_path_b = true;
		cali_info->modify_tx_agc_flag_path_a_cck = true;
		cali_info->modify_tx_agc_flag_path_b_cck = true;
		if (rf_path == RF_PATH_A) {
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, CCK);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, OFDM);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, HT_MCS0_MCS7);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, HT_MCS8_MCS15);
			/*
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, CCK);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, OFDM);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, HT_MCS0_MCS7);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, HT_MCS8_MCS15);
			*/
		} else {
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, CCK);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, OFDM);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);
			/*	PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, OFDM);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);*/
		}
#else

		if (rf_path == RF_PATH_A) {
			/* CCK path A */

			pwr = odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0xFF); /*e00[6:0] = PathA OFDM 6M*/
			pwr += cali_info->power_index_offset[RF_PATH_A];
			odm_set_bb_reg(dm, REG_TX_AGC_A_CCK_1_MCS32, MASKBYTE1, pwr);
			tx_agc = (pwr << 16) | (pwr << 8) | (pwr);
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0xffffff00, tx_agc);
			RT_DISP(FPHY, PHY_TXPWR, ("%s: CCK Tx-rf(A) Power = 0x%x\n", __func__, tx_agc));

			/* OFDM path A */
			pwr = odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0xFF);
			pwr += (cali_info->bb_swing_idx_ofdm[RF_PATH_A] - cali_info->bb_swing_idx_ofdm_base[RF_PATH_A]);
			tx_agc = ((pwr << 24) | (pwr << 16) | (pwr << 8) | pwr);

			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, MASKDWORD, tx_agc);
			RT_DISP(FPHY, PHY_TXPWR, ("%s: OFDM Tx-rf(A) Power = 0x%x\n", __func__, tx_agc));
		} else if (rf_path == RF_PATH_B) {
			/* CCK path B */
			pwr = odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0xFF); /*830[6:0] = PathB OFDM 6M*/
			pwr += cali_info->power_index_offset[RF_PATH_B];
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, MASKBYTE0, pwr);
			tx_agc = (pwr << 16) | (pwr << 8) | (pwr);
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0xffffff00, tx_agc);

			RT_DISP(FPHY, PHY_TXPWR, ("%s: CCK Tx-rf(B) Power = 0x%x\n", __func__, pwr));

			/* OFDM path B */
			pwr = odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0xFF);
			pwr += (cali_info->bb_swing_idx_ofdm[RF_PATH_B] - cali_info->bb_swing_idx_ofdm_base[RF_PATH_B]);
			tx_agc = ((pwr << 24) | (pwr << 16) | (pwr << 8) | pwr);
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, MASKDWORD, tx_agc);
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, MASKDWORD, tx_agc);
			RT_DISP(FPHY, PHY_TXPWR, ("%s: OFDM Tx-rf(B) Power = 0x%x\n", __func__, tx_agc));
		}
#endif

#endif
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
/* phy_rf6052_set_cck_tx_power(dm->priv, *(dm->channel)); */
/* phy_rf6052_set_ofdm_tx_power(dm->priv, *(dm->channel)); */
#endif

	} else if (method == BBSWING) {
		final_ofdm_swing_index = cali_info->default_ofdm_index + cali_info->absolute_ofdm_swing_idx[rf_path];
		final_cck_swing_index = cali_info->default_cck_index + cali_info->absolute_ofdm_swing_idx[rf_path];

		if (final_ofdm_swing_index >= pwr_tracking_limit_ofdm)
			final_ofdm_swing_index = pwr_tracking_limit_ofdm;
		else if (final_ofdm_swing_index < 0)
			final_ofdm_swing_index = 0;

		if (final_cck_swing_index >= CCK_TABLE_SIZE_8192F)
			final_cck_swing_index = CCK_TABLE_SIZE_8192F - 1;
		else if (cali_info->bb_swing_idx_cck < 0)
			final_cck_swing_index = 0;

		/* Adjust BB swing by OFDM IQ matrix */
		if (rf_path == RF_PATH_A) {
			set_iqk_matrix_8192f(dm, final_ofdm_swing_index, RF_PATH_A,
					     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][0],
					     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][1]);
			odm_set_bb_reg(dm, R_0xab4, 0x000007FF, cck_swing_table_ch1_ch14_8192f[final_cck_swing_index]);

		} else if (rf_path == RF_PATH_B) {
			set_iqk_matrix_8192f(dm, final_ofdm_swing_index, RF_PATH_B,
					     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][4],
					     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][5]);

			odm_set_bb_reg(dm, R_0xab4, 0x003FF800, cck_swing_table_ch1_ch14_8192f[final_cck_swing_index]); /*20170805 Winnita: pathB CCK table?*/
		}
	} else if (method == MIX_MODE) {
#if (MP_DRIVER == 1)
		u32 tx_agc = 0; /*add by Mingzhi.Guo 2015-04-10*/
		s32 pwr = 0;
#endif
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
		pwr_tracking_limit_ofdm = OFDM_TABLE_SIZE_92D - 1;
		pwr_tracking_limit_cck = CCK_TABLE_SIZE_8192F - 1;
#endif

		RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
		       "cali_info->default_ofdm_index=%d,  dm->DefaultCCKIndex=%d, cali_info->absolute_ofdm_swing_idx[rf_path]=%d, rf_path = %d\n",
		       cali_info->default_ofdm_index,
		       cali_info->default_cck_index,
		       cali_info->absolute_ofdm_swing_idx[rf_path], rf_path);
		final_ofdm_swing_index = cali_info->default_ofdm_index + cali_info->absolute_ofdm_swing_idx[rf_path];
		final_cck_swing_index = cali_info->default_cck_index + cali_info->absolute_ofdm_swing_idx[rf_path]; /* CCK Follow path-A and lower CCK index means higher power. */

		if (rf_path == RF_PATH_A) {
			if (final_ofdm_swing_index > pwr_tracking_limit_ofdm) { /* BBSwing higher than Limit */
				cali_info->remnant_ofdm_swing_idx[rf_path] = final_ofdm_swing_index - pwr_tracking_limit_ofdm;

				set_iqk_matrix_8192f(dm, pwr_tracking_limit_ofdm, RF_PATH_A,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][0],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][1]);

				cali_info->modify_tx_agc_flag_path_a = true;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A Over BBSwing Limit, pwr_tracking_limit = %d, Remnant tx_agc value = %d\n",
				       pwr_tracking_limit_ofdm,
				       cali_info->remnant_ofdm_swing_idx[rf_path
				       ]);
			} else if (final_ofdm_swing_index < 0) {
				cali_info->remnant_ofdm_swing_idx[rf_path] = final_ofdm_swing_index;

				set_iqk_matrix_8192f(dm, 0, RF_PATH_A,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][0],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][1]);

				cali_info->modify_tx_agc_flag_path_a = true;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A Lower then BBSwing lower bound  0, Remnant tx_agc value = %d\n",
				       cali_info->remnant_ofdm_swing_idx[rf_path
				       ]);
			} else {
				set_iqk_matrix_8192f(dm, final_ofdm_swing_index, RF_PATH_A,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][0],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][1]);

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A Compensate with BBSwing, final_ofdm_swing_index = %d\n",
				       final_ofdm_swing_index);

				if (cali_info->modify_tx_agc_flag_path_a) {
					/* If tx_agc has changed, reset tx_agc again */
					cali_info->remnant_ofdm_swing_idx[rf_path] = 0;

					cali_info->modify_tx_agc_flag_path_a = false;

					RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "******Path_A dm->Modify_TxAGC_Flag = false\n");
				}
			}

#if (0) /*(MP_DRIVER == 1)*/
			if ((*dm->mp_mode) == 1) {
				/* OFDM path A */
				pwr = odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0xFF);
				pwr += (cali_info->bb_swing_idx_ofdm[RF_PATH_A] - cali_info->bb_swing_idx_ofdm_base[RF_PATH_A]);
				tx_agc = ((pwr << 24) | (pwr << 16) | (pwr << 8) | pwr);
				odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, MASKDWORD, tx_agc);
				RT_DISP(FPHY, PHY_TXPWR, ("%s: OFDM Tx-rf(A) Power = 0x%x\n", __func__, tx_agc));

			} else
#endif
/*
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, OFDM);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, HT_MCS0_MCS7);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, HT_MCS8_MCS15);
			*/
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
				odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, OFDM);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, HT_MCS0_MCS7);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, HT_MCS8_MCS15);
#endif
			cali_info->modify_tx_agc_value_ofdm = cali_info->remnant_ofdm_swing_idx[RF_PATH_A];

			if (final_cck_swing_index > pwr_tracking_limit_cck) {
				cali_info->remnant_cck_swing_idx = final_cck_swing_index - pwr_tracking_limit_cck;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A CCK Over Limit, pwr_tracking_limit_cck = %d, cali_info->remnant_cck_swing_idx  = %d\n",
				       pwr_tracking_limit_cck,
				       cali_info->remnant_cck_swing_idx);

				/* Adjust BB swing by CCK filter coefficient */ /* Winnita change 20170828 */
				odm_set_bb_reg(dm, R_0xab4, 0x000007FF, cck_swing_table_ch1_ch14_8192f[pwr_tracking_limit_cck]);

				cali_info->modify_tx_agc_flag_path_a_cck = true;

				/* Set tx_agc Page C{};
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, CCK);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK); */

			} else if (final_cck_swing_index < 0) { /* Lowest CCK index = 0 */
				cali_info->remnant_cck_swing_idx = final_cck_swing_index;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A CCK Under Limit, pwr_tracking_limit_cck = %d, cali_info->remnant_cck_swing_idx  = %d\n",
				       0, cali_info->remnant_cck_swing_idx);

				odm_set_bb_reg(dm, R_0xab4, 0x000007FF, cck_swing_table_ch1_ch14_8192f[0]);

				cali_info->modify_tx_agc_flag_path_a_cck = true;

				/* Set tx_agc Page C{};
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, CCK);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK);*/

			} else {
				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A CCK Compensate with BBSwing, final_cck_swing_index = %d\n",
				       final_cck_swing_index);

				odm_set_bb_reg(dm, R_0xab4, 0x000007FF, cck_swing_table_ch1_ch14_8192f[final_cck_swing_index]);

				/* if (cali_info->modify_tx_agc_flag_path_a_cck) { If tx_agc has changed, reset tx_agc again */
				cali_info->remnant_cck_swing_idx = 0;

				/* Set tx_agc Page C{};
					PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, CCK);
					PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK);*/

				cali_info->modify_tx_agc_flag_path_a_cck = false;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A dm->Modify_TxAGC_Flag_CCK = false\n");
			}
#if (0) /*(MP_DRIVER == 1)*/
			if ((*dm->mp_mode) == 1) {
				pwr = odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, MASKBYTE3);
				pwr += cali_info->remnant_cck_swing_idx - cali_info->modify_tx_agc_value_cck;

				if (pwr > 0x3F)
					pwr = 0x3F;
				else if (pwr < 0)
					pwr = 0;

				odm_set_bb_reg(dm, REG_TX_AGC_A_CCK_1_MCS32, MASKBYTE1, pwr);
				tx_agc = (pwr << 16) | (pwr << 8) | (pwr);
				odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0xffffff00, tx_agc);
				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "ODM_TxPwrTrackSetPwr8192F: CCK Tx-rf(A) Power = 0x%x\n",
				       tx_agc);
			} else
#endif

/* Set tx_agc Page C{}; */
/* PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_A, *dm->channel, CCK); */
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
				odm_set_tx_power_index_by_rate_section(dm, RF_PATH_A, *dm->channel, CCK);
#endif

			cali_info->modify_tx_agc_value_cck = cali_info->remnant_cck_swing_idx;
		}

		if (rf_path == RF_PATH_B) {
			if (final_ofdm_swing_index > pwr_tracking_limit_ofdm) { /* BBSwing higher then Limit */
				cali_info->remnant_ofdm_swing_idx[rf_path] = final_ofdm_swing_index - pwr_tracking_limit_ofdm;

				set_iqk_matrix_8192f(dm, pwr_tracking_limit_ofdm, RF_PATH_B,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][4],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][5]);

				cali_info->modify_tx_agc_flag_path_b = true;

				/* Set tx_agc Page C{};
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, OFDM);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);*/

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B Over BBSwing Limit, pwr_tracking_limit = %d, Remnant tx_agc value = %d\n",
				       pwr_tracking_limit_ofdm,
				       cali_info->remnant_ofdm_swing_idx[rf_path
				       ]);
			} else if (final_ofdm_swing_index < 0) {
				cali_info->remnant_ofdm_swing_idx[rf_path] = final_ofdm_swing_index;

				set_iqk_matrix_8192f(dm, 0, RF_PATH_B,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][4],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][5]);

				cali_info->modify_tx_agc_flag_path_b = true;

				/* Set tx_agc Page C{};
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, OFDM);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);*/

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B Lower then BBSwing lower bound  0, Remnant tx_agc value = %d\n",
				       cali_info->remnant_ofdm_swing_idx[rf_path
				       ]);
			} else {
				set_iqk_matrix_8192f(dm, final_ofdm_swing_index, RF_PATH_B,
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][4],
						     cali_info->iqk_matrix_reg_setting[channel_mapped_index].value[0][5]);

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B Compensate with BBSwing, final_ofdm_swing_index = %d\n",
				       final_ofdm_swing_index);

				if (cali_info->modify_tx_agc_flag_path_b) { /* If tx_agc has changed, reset tx_agc again */
					cali_info->remnant_ofdm_swing_idx[rf_path] = 0;

					/* Set tx_agc Page C{};
					PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, OFDM);
					PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
					PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);*/

					cali_info->modify_tx_agc_flag_path_b = false;

					RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "******Path_B dm->Modify_TxAGC_Flag = false\n");
				}
			}
#if (0) /*(MP_DRIVER == 1)*/
			if ((*dm->mp_mode) == 1) {
				/* OFDM path B */
				pwr = odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0xFF);
				pwr += (cali_info->bb_swing_idx_ofdm[RF_PATH_B] - cali_info->bb_swing_idx_ofdm_base[RF_PATH_B]);
				tx_agc = ((pwr << 24) | (pwr << 16) | (pwr << 8) | pwr);
				odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, MASKDWORD, tx_agc);
				odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, MASKDWORD, tx_agc);
				RT_DISP(FPHY, PHY_TXPWR, ("%s: OFDM Tx-rf(B) Power = 0x%x\n", __func__, tx_agc));

			} else
#endif
/*
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, OFDM);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
			PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);
			*/
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
				odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, OFDM);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, HT_MCS0_MCS7);
			odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, HT_MCS8_MCS15);
#endif

			cali_info->modify_tx_agc_value_ofdm = cali_info->remnant_ofdm_swing_idx[RF_PATH_B];

			/*20170828 Winnita: add pathB CCK powertracking	*/
			if (final_cck_swing_index > pwr_tracking_limit_cck) {
				cali_info->remnant_cck_swing_idx = final_cck_swing_index - pwr_tracking_limit_cck;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B CCK Over Limit, pwr_tracking_limit_cck = %d, cali_info->remnant_cck_swing_idx  = %d\n",
				       pwr_tracking_limit_cck,
				       cali_info->remnant_cck_swing_idx);

				/* Adjust BB swing by CCK filter coefficient */ /* Winnita change 20170828 */
				odm_set_bb_reg(dm, R_0xab4, 0x003FF800, cck_swing_table_ch1_ch14_8192f[pwr_tracking_limit_cck]);

				cali_info->modify_tx_agc_flag_path_b_cck = true;

				/* Set tx_agc Page C{};
				PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK); */

			} else if (final_cck_swing_index < 0) { /* Lowest CCK index = 0 */
				cali_info->remnant_cck_swing_idx = final_cck_swing_index;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B CCK Under Limit, pwr_tracking_limit_cck = %d, cali_info->remnant_cck_swing_idx  = %d\n",
				       0, cali_info->remnant_cck_swing_idx);

				odm_set_bb_reg(dm, R_0xab4, 0x003FF800, cck_swing_table_ch1_ch14_8192f[0]);

				cali_info->modify_tx_agc_flag_path_b_cck = true;

			} else {
				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_A CCK Compensate with BBSwing, final_cck_swing_index = %d\n",
				       final_cck_swing_index);

				odm_set_bb_reg(dm, R_0xab4, 0x003FF800, cck_swing_table_ch1_ch14_8192f[final_cck_swing_index]);

				/* if (cali_info->modify_tx_agc_flag_path_b_cck) {  If tx_agc has changed, reset tx_agc again */
				cali_info->remnant_cck_swing_idx = 0;

				cali_info->modify_tx_agc_flag_path_b_cck = false;

				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "******Path_B dm->Modify_TxAGC_Flag_CCK = false\n");
			}

#if (0) /*(MP_DRIVER == 1)*/
			if ((*dm->mp_mode) == 1) {
				pwr = odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, MASKBYTE0);
				pwr += cali_info->remnant_cck_swing_idx - cali_info->modify_tx_agc_value_cck;

				if (pwr > 0x3F)
					pwr = 0x3F;
				else if (pwr < 0)
					pwr = 0;

				odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, MASKBYTE0, pwr);

				tx_agc = (pwr << 16) | (pwr << 8) | (pwr);
				odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0xffffff00, tx_agc);
				RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
				       "ODM_TxPwrTrackSetPwr8710B: CCK Tx-rf(A) Power = 0x%x\n",
				       tx_agc);
			} else
#endif
/* Set tx_agc Page C{}; */
/*PHY_SetTxPowerIndexByRateSection(adapter, RF_PATH_B, *dm->channel, CCK);*/
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
				odm_set_tx_power_index_by_rate_section(dm, RF_PATH_B, *dm->channel, CCK);
#endif
			cali_info->modify_tx_agc_value_cck = cali_info->remnant_cck_swing_idx;
		}
	}
	/*#endif*/

	else
		return;
} /* odm_TxPwrTrackSetPwr92F */

void get_delta_swing_table_8192f(
	void *dm_void,
	u8 **temperature_up_a,
	u8 **temperature_down_a,
	u8 **temperature_up_b,
	u8 **temperature_down_b)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct _ADAPTER *adapter = dm->adapter;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);
	struct _hal_rf_ *rf = &(dm->rf_table);
	u8 tx_rate = 0xFF;
	u8 channel = *dm->channel;

	if (*dm->mp_mode == true) {
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN | ODM_CE))
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
#if (MP_DRIVER == 1)
		PMPT_CONTEXT p_mpt_ctx = &(adapter->MptCtx);

		tx_rate = MptToMgntRate(p_mpt_ctx->MptRateIndex);
#endif
#elif (DM_ODM_SUPPORT_TYPE & ODM_CE)
#ifdef CONFIG_MP_INCLUDED
		PMPT_CONTEXT p_mpt_ctx = &(adapter->mppriv.mpt_ctx);

		tx_rate = mpt_to_mgnt_rate(p_mpt_ctx->mpt_rate_index);
#endif
#endif
#endif
	} else {
		u16 rate = *(dm->forced_data_rate);

		if (!rate) { /*auto rate*/ 
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
			tx_rate = adapter->HalFunc.GetHwRateFromMRateHandler(dm->tx_rate);
#elif (DM_ODM_SUPPORT_TYPE & ODM_CE)
			if (dm->number_linked_client != 0)
				tx_rate = hw_rate_to_m_rate(dm->tx_rate);
			else
				tx_rate = rf->p_rate_index;
#endif
		} else /*force rate*/
			tx_rate = (u8)rate;
	}
	RF_DBG(dm, DBG_RF_TX_PWR_TRACK, "Call %s Power Tracking tx_rate=0x%X\n",
	       __func__, tx_rate);

	if (1 <= channel && channel <= 14) {
		if (IS_CCK_RATE(tx_rate)) {
			*temperature_up_a = cali_info->delta_swing_table_idx_2g_cck_a_p;
			*temperature_down_a = cali_info->delta_swing_table_idx_2g_cck_a_n;
			*temperature_up_b = cali_info->delta_swing_table_idx_2g_cck_b_p;
			*temperature_down_b = cali_info->delta_swing_table_idx_2g_cck_b_n;
		} else {
			*temperature_up_a = cali_info->delta_swing_table_idx_2ga_p;
			*temperature_down_a = cali_info->delta_swing_table_idx_2ga_n;
			*temperature_up_b = cali_info->delta_swing_table_idx_2gb_p;
			*temperature_down_b = cali_info->delta_swing_table_idx_2gb_n;
		}
	} else {
		*temperature_up_a = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_a = (u8 *)delta_swing_table_idx_2ga_n_8188e;
		*temperature_up_b = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_b = (u8 *)delta_swing_table_idx_2ga_n_8188e;
	}

	return;
}

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

	)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);
	u8 channel = *(dm->channel);

	if (channel >= 1 && channel <= 14) {
		*temperature_up_cck_a = cali_info->delta_swing_table_idx_2g_cck_a_p;
		*temperature_down_cck_a = cali_info->delta_swing_table_idx_2g_cck_a_n;
		*temperature_up_cck_b = cali_info->delta_swing_table_idx_2g_cck_b_p;
		*temperature_down_cck_b = cali_info->delta_swing_table_idx_2g_cck_b_n;

		*temperature_up_a = cali_info->delta_swing_table_idx_2ga_p;
		*temperature_down_a = cali_info->delta_swing_table_idx_2ga_n;
		*temperature_up_b = cali_info->delta_swing_table_idx_2gb_p;
		*temperature_down_b = cali_info->delta_swing_table_idx_2gb_n;
	} else {
		*temperature_up_cck_a = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_cck_a = (u8 *)delta_swing_table_idx_2ga_n_8188e;
		*temperature_up_cck_b = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_cck_b = (u8 *)delta_swing_table_idx_2ga_n_8188e;
		*temperature_up_a = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_a = (u8 *)delta_swing_table_idx_2ga_n_8188e;
		*temperature_up_b = (u8 *)delta_swing_table_idx_2ga_p_8188e;
		*temperature_down_b = (u8 *)delta_swing_table_idx_2ga_n_8188e;
	}

	return;
}

#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
void get_delta_swing_xtal_table_8192f(
	void *dm_void,
	s8 **temperature_up_xtal,
	s8 **temperature_down_xtal)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);

	*temperature_up_xtal = cali_info->delta_swing_table_xtal_p;
	*temperature_down_xtal = cali_info->delta_swing_table_xtal_n;
}

void odm_txxtaltrack_set_xtal_8192f(
	void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);

	s8 crystal_cap;

	crystal_cap = dm->dm_cfo_track.crystal_cap_default & 0x3F;
	crystal_cap = crystal_cap + cali_info->xtal_offset;

	if (crystal_cap < 0)
		crystal_cap = 0;
	else if (crystal_cap > 63)
		crystal_cap = 63;

	RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
	       "crystal_cap(%d)= dm->dm_cfo_track.crystal_cap_default(%d) + cali_info->xtal_offset(%d)\n",
	       crystal_cap, dm->dm_cfo_track.crystal_cap_default, cali_info->xtal_offset);

	odm_set_mac_reg(dm, 0x24, 0x7E000000, crystal_cap);
	odm_set_mac_reg(dm, 0x28, 0x7e, crystal_cap);

	RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
	       "crystal_cap(0x24)  0x%X\n",
	       odm_get_mac_reg(dm, 0x24, 0x7E000000));
	RF_DBG(dm, DBG_RF_TX_PWR_TRACK,
	       "crystal_cap(0x28)  0x%X\n",
	       odm_get_mac_reg(dm, 0x28, 0x7e));
}
#endif

void configure_txpower_track_8192f(
	struct txpwrtrack_cfg *config)
{
	config->swing_table_size_cck = CCK_TABLE_SIZE_8192F;
	config->swing_table_size_ofdm = OFDM_TABLE_SIZE;
	config->threshold_iqk = IQK_THRESHOLD;
	config->average_thermal_num = AVG_THERMAL_NUM_92F;
	/*config->average_thermal_num = 4;*/
	config->rf_path_count = MAX_PATH_NUM_8192F;
	config->thermal_reg_addr = RF_T_METER_92F;

	config->odm_tx_pwr_track_set_pwr = odm_tx_pwr_track_set_pwr92_f;
	config->do_iqk = do_iqk_8192f;
	config->phy_lc_calibrate = halrf_lck_trigger;
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	config->get_delta_all_swing_table = get_delta_all_swing_table_8192f;
#else
	config->get_delta_swing_table = get_delta_swing_table_8192f;
	config->get_delta_swing_xtal_table = get_delta_swing_xtal_table_8192f;
	config->odm_txxtaltrack_set_xtal = odm_txxtaltrack_set_xtal_8192f;
#endif
}
/*#endif*/

/* 1 7.	IQK */
#define MAX_TOLERANCE 5
#define IQK_DELAY_TIME 1 /* ms */

u8 /* bit0 = 1 => Tx OK, bit1 = 1 => Rx OK */
	phy_path_a_iqk_8192f(
		struct dm_struct *dm,
		boolean config_path_b)
{
	u32 reg_eac, reg_e94, reg_e9c, RFreg58I, RFreg58Q, eacBIT28;
	u8 result = 0x00, ktime = 0, i =0;

	RF_DBG(dm, DBG_RF_IQK, "path A IQK!\n");

	/*8192E IQK V2.1 20150713*/
	/*1 Tx IQK*/
	/* path-A IQK setting */

	/*	PA/PAD controlled by 0x0 */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCF000C0);
	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);
	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000100);
	
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00010, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00800, 0x1);
	if (dm->rfe_type == 7 || dm->rfe_type == 8 || dm->rfe_type == 9 ||
	    dm->rfe_type == 12)
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x56, 0x003ff, 0x30);
	else
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x56, 0x003ff, 0xe9);
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

	/*	RF_DBG(dm,DBG_RF_IQK, "path-A IQK setting!\n"); */
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x18008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x38008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_A, MASKDWORD, 0x8214000f); /*82140303*/
	odm_set_bb_reg(dm, REG_RX_IQK_PI_A, MASKDWORD, 0x28140000);

	odm_set_bb_reg(dm, REG_TX_IQK, MASKDWORD, 0x01007c00);
	odm_set_bb_reg(dm, REG_RX_IQK, MASKDWORD, 0x01004800);
	/* IQK MAC seting*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x0);

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, "LO calibration setting!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x00e62911); /*8197F:0x00e62911*/

	/* One shot, path A LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, "One shot, path A LOK & IQK!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, "delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_92E); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);
	while ((!odm_get_bb_reg(dm, R_0xe98, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}

	/* IQK MAC seting reload*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x1);
	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT28 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x10000000);
	reg_e94 = odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD);
	reg_e9c = odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD);
	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT28=0x%x\n", reg_eac,
	       eacBIT28);
	RF_DBG(dm, DBG_RF_IQK, "0xe94 = 0x%x, 0xe9c = 0x%x\n", reg_e94,
	       reg_e9c);
	/*monitor image power before & after IQK*/
	RF_DBG(dm, DBG_RF_IQK,
	       "0xe90(before IQK)= 0x%x, 0xe98(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xe90, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xe98, MASKDWORD));

	/*reload 0xdf and CCK_IND off */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(4), 0x1);
	RFreg58I = odm_get_rf_reg(dm, RF_PATH_A, RF_0x58, 0xfc000);
	RFreg58Q = odm_get_rf_reg(dm, RF_PATH_A, RF_0x58, 0x003f0);

	RF_DBG(dm, DBG_RF_IQK, "0x58[19:14]= 0x%x, 0x58[9:4] = 0x%x\n",
	       RFreg58I, RFreg58Q);
	RF_DBG(dm, DBG_RF_IQK, "0x58 = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x58, 0xfffff));
	for (i=0; i<8; i++){
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x33, 0x1c000, i);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x33, 0x00fc0, RFreg58I);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x33, 0x0003f, RFreg58Q);
	}

	odm_set_rf_reg(dm, RF_PATH_A, RF_0x0, BIT(14), 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(4), 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00810, 0x0);

	if (!(reg_eac & BIT(28)) &&
	    (((reg_e94 & 0x03FF0000) >> 16) != 0x142) &&
	    (((reg_e9c & 0x03FF0000) >> 16) != 0x42))
		result |= 0x01;
	else
		RF_DBG(dm, DBG_RF_IQK, "pathA TX IQK is fail!\n");

	return result;
}

u8 /* bit0 = 1 => Tx OK, bit1 = 1 => Rx OK */
	phy_path_a_rx_iqk_92f(
		struct dm_struct *dm,
		boolean config_path_b)
{
	u32 reg_eac, reg_e94, reg_e9c, reg_ea4, u4tmp, eacBIT28, eacBIT27;
	u8 result = 0x00, ktime = 0;
	RF_DBG(dm, DBG_RF_IQK, "path A Rx IQK!\n");

	/* 1 Get TXIMR setting */
	RF_DBG(dm, DBG_RF_IQK, "Get RXIQK TXIMR(step1)!\n");

	/* modify RXIQK mode table
	* 	RF_DBG(dm,DBG_RF_IQK, "path-A Rx IQK modify RXIQK mode table!\n"); */
	/* leave IQK mode */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	/*	PA/PAD control by 0x56, and set = 0x0 */
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00002, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00800, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x56, 0x3ff, 0x27);

	/* enter IQK mode */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

	/* path-A IQK setting */
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x18008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x38008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_A, MASKDWORD, 0x82160027); /*8216031f*/
	odm_set_bb_reg(dm, REG_RX_IQK_PI_A, MASKDWORD, 0x28160000); /*6816031f*/

	/* IQK setting */
	odm_set_bb_reg(dm, REG_TX_IQK, MASKDWORD, 0x01007c00);
	odm_set_bb_reg(dm, REG_RX_IQK, MASKDWORD, 0x01004800);
	/* IQK MAC seting*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x0);

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, "LO calibration setting!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x0086a911);

	/* One shot, path A LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, "One shot, path A LOK & IQK!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, "delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_92E); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);

	while ((!odm_get_bb_reg(dm, R_0xe98, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}
	/* IQK MAC seting reload*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x1);

	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT28 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x10000000);
	reg_e94 = odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD);
	reg_e9c = odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD);
	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT28=0x%x\n", reg_eac,
	       eacBIT28);
	RF_DBG(dm, DBG_RF_IQK, "0xe94 = 0x%x, 0xe9c = 0x%x\n", reg_e94,
	       reg_e9c);
	/*monitor image power before & after IQK*/
	RF_DBG(dm, DBG_RF_IQK,
	       "0xe90(before IQK)= 0x%x, 0xe98(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xe90, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xe98, MASKDWORD));

	if (!(reg_eac & BIT(28)) &&
	    (((reg_e94 & 0x03FF0000) >> 16) != 0x142) &&
	    (((reg_e9c & 0x03FF0000) >> 16) != 0x42))
		result |= 0x01;
	else { /*if Tx not OK, ignore Rx*/
		/*	PA/PAD controlled by 0x0*/
		odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00800, 0x0);

		RF_DBG(dm, DBG_RF_IQK, "PATH A 0x0 = 0x%x, PATH B 0x0 = 0x%x\n",
		       odm_get_rf_reg(dm, RF_PATH_A, RF_0x0, 0xfffff),
		       odm_get_rf_reg(dm, RF_PATH_B, RF_0x0, 0xfffff));
		RF_DBG(dm, DBG_RF_IQK, "PATH A 0x7F = 0x%x, 0x81 = 0x%x\n",
		       odm_get_rf_reg(dm, RF_PATH_A, RF_0x7f, 0xfffff),
		       odm_get_rf_reg(dm, RF_PATH_A, RF_0x81, 0xfffff));
		RF_DBG(dm, DBG_RF_IQK, "pathA get TXIMR is fail\n");
		return result;
	}

	u4tmp = 0x80007C00 | (reg_e94 & 0x3FF0000) | ((reg_e9c & 0x3FF0000) >> 16);
	odm_set_bb_reg(dm, REG_TX_IQK, MASKDWORD, u4tmp);
	RF_DBG(dm, DBG_RF_IQK, "0xe40 = 0x%x u4tmp = 0x%x\n",
	       odm_get_bb_reg(dm, REG_TX_IQK, MASKDWORD), u4tmp);

	/* 1 RX IQK */
	/* modify RXIQK mode table */
	RF_DBG(dm, DBG_RF_IQK, "Do RXIQK(step2)!\n");
	/*	RF_DBG(dm,DBG_RF_IQK, "path-A Rx IQK modify RXIQK mode table 2!\n"); */
	/* leave IQK mode */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	/*	PA/PAD control by 0x56, and set = 0x0 */
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00002, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00800, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x56, 0x003ff, 0x1e0);
	/*odm_set_rf_reg(dm, RF_PATH_A, RF_0x56, RFREGOFFSETMASK, 0x51000 );*/

	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCF000C0);
	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);
	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000100);

	/*enter IQK mode*/
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

	/* path-A IQK setting */
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x18008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x38008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_A, MASKDWORD, 0x82170000); /*82171611*/
	odm_set_bb_reg(dm, REG_RX_IQK_PI_A, MASKDWORD, 0x28170000); /*28173187*/

	/*	odm_set_bb_reg(dm, REG_TX_IQK_PI_A, MASKDWORD, 0x82160cff);
	 *	odm_set_bb_reg(dm, REG_RX_IQK_PI_A, MASKDWORD, 0x28160cff); */

	/* IQK setting */
	odm_set_bb_reg(dm, REG_RX_IQK, MASKDWORD, 0x01004800);

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, "LO calibration setting!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x0046a8d1);

	/* One shot, path A LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, "One shot, path A LOK & IQK!\n"); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, "delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_92E); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);

	while ((!odm_get_bb_reg(dm, R_0xea8, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}

	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT27 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x08000000);
	reg_ea4 = odm_get_bb_reg(dm, REG_RX_POWER_BEFORE_IQK_A_2, MASKDWORD);
	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT27 =0x%x\n", reg_eac,
	       eacBIT27);
	RF_DBG(dm, DBG_RF_IQK, "0xea4 = 0x%x, 0xeac = 0x%x\n", reg_ea4,
	       reg_eac);
	/* monitor image power before & after IQK */
	RF_DBG(dm, DBG_RF_IQK,
	       "0xea0(before IQK)= 0x%x, 0xea8(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xea0, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xea8, MASKDWORD));

	/*	PA/PAD controlled by 0x0 */
	/* leave IQK mode */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00800, 0x0);
	/*odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00002, 0x0);*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff, 0x02000);

	if (!(reg_eac & BIT(27)) && /*if Tx is OK, check whether Rx is OK*/
	    (((reg_ea4 & 0x03FF0000) >> 16) != 0x132) &&
	    (((reg_eac & 0x03FF0000) >> 16) != 0x36))
		result |= 0x02;
	else
		RF_DBG(dm, DBG_RF_IQK, "path A Rx IQK is fail!!\n");

	return result;
}

u8 /* bit0 = 1 => Tx OK, bit1 = 1 => Rx OK */
	phy_path_b_iqk_8192f(
		struct dm_struct *dm)
{
	u32 reg_eac, reg_eb4, reg_ebc, RFreg58I, RFreg58Q, eacBIT31;
	u8 result = 0x00, ktime = 0, i = 0;
	RF_DBG(dm, DBG_RF_IQK, "path B IQK setting!\n");
	/*1 Tx IQK*/
	/* path-B IQK setting
	* 	RF_DBG(dm,DBG_RF_IQK, "path-B IQK setting!\n"); */

	/*	PA/PAD controlled by 0x0 */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	/*disable path-A PI, prevent path-A re-LOK*/
	/*odm_set_bb_reg(dm, R_0x820, BIT(8), 0x0);*/

	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCF000C0);

	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);
	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000000);

	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00010, 0x1);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00800, 0x1);
	if (dm->rfe_type == 7 || dm->rfe_type == 8 || dm->rfe_type == 9 ||
	    dm->rfe_type == 12)
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x56, 0x003ff, 0x30);
	else
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x56, 0x00fff, 0xe9);

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

#if 0
	odm_set_bb_reg(dm, R_0xe28, 0xffffff00, 0x000000);
	RF_DBG(dm, DBG_RF_IQK, "path A 0xdf = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0xdf, RFREGOFFSETMASK));
	RF_DBG(dm, DBG_RF_IQK, "path B 0xdf = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0xdf, RFREGOFFSETMASK));
	odm_set_bb_reg(dm, R_0xe28, 0xffffff00, 0x808000);
#endif

	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x18008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x38008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_B, MASKDWORD, 0x8214000F); /*82140303*/
	odm_set_bb_reg(dm, REG_RX_IQK_PI_B, MASKDWORD, 0x28140000); /*68160000*/

	/*odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);*/
	odm_set_bb_reg(dm, REG_TX_IQK, MASKDWORD, 0x01007c00);
	odm_set_bb_reg(dm, REG_RX_IQK, MASKDWORD, 0x01004800);
	/* IQK MAC seting*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x0);

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, ("LO calibration setting!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x00e62911);

	/* One shot, path B LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, ("One shot, path B LOK & IQK!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/*	odm_set_bb_reg(dm, REG_IQK_AGC_CONT, MASKDWORD, 0x00000002);
	 *	odm_set_bb_reg(dm, REG_IQK_AGC_CONT, MASKDWORD, 0x00000000); */

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, ("delay %d ms for One shot, path B LOK & IQK.\n", IQK_DELAY_TIME_92E)); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);
	while ((!odm_get_bb_reg(dm, R_0xeb8, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}

	/* IQK MAC seting reload*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x1);
	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT31 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x80000000);
	reg_eb4 = odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_B, MASKDWORD);
	reg_ebc = odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_B, MASKDWORD);
	/*RF_DBG(dm, DBG_RF_IQK, ("0xeac = 0x%x\n", reg_eac));*/
	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT31=0x%x\n", reg_eac,
	       eacBIT31);
	RF_DBG(dm, DBG_RF_IQK, "0xeb4 = 0x%x, 0xebc = 0x%x\n", reg_eb4,
	       reg_ebc);
	/*monitor image power before & after IQK*/
	RF_DBG(dm, DBG_RF_IQK,
	       "0xeb0(before IQK)= 0x%x, 0xeb8(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xeb0, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xeb8, MASKDWORD));

	/*reload 0xdf and CCK_IND off */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(4), 0x1);
	RFreg58I = odm_get_rf_reg(dm, RF_PATH_B, RF_0x58, 0xfc000);
	RFreg58Q = odm_get_rf_reg(dm, RF_PATH_B, RF_0x58, 0x003f0);

	RF_DBG(dm, DBG_RF_IQK, "0x58[19:14]= 0x%x, 0x58[9:4] = 0x%x\n",
	       RFreg58I, RFreg58Q);
	RF_DBG(dm, DBG_RF_IQK, "0x58 = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x58, 0xfffff));
	for (i=0; i<8; i++){
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0x1c000, i);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0x00fc0, RFreg58I);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0x0003f, RFreg58Q);
	}

	odm_set_rf_reg(dm, RF_PATH_B, RF_0x0, BIT(14), 0x0);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(4), 0x0);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00810, 0x0);

	if (!(reg_eac & BIT(31)) &&
	    (((reg_eb4 & 0x03FF0000) >> 16) != 0x142) &&
	    (((reg_ebc & 0x03FF0000) >> 16) != 0x42))
		result |= 0x01;
	else
		RF_DBG(dm, DBG_RF_IQK, "path B TX IQK is not success\n");

	return result;
}

u8 /* bit0 = 1 => Tx OK, bit1 = 1 => Rx OK */
	phy_path_b_rx_iqk_92f(
		struct dm_struct *dm,
		boolean config_path_b)
{
	u32 reg_eac, reg_eb4, reg_ebc, reg_ecc, reg_ec4, u4tmp, eacBIT31, eacBIT30;
	u8 result = 0x00, ktime = 0;
	RF_DBG(dm, DBG_RF_IQK, "path B Rx IQK!\n");

	/* 1 Get TXIMR setting */
	RF_DBG(dm, DBG_RF_IQK, "Get RXIQK TXIMR(step1)!\n");
	/* modify RXIQK mode table
	* 	RF_DBG(dm,DBG_RF_IQK, ("path-B Rx IQK modify RXIQK mode table!\n")); */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
	/*
	odm_set_rf_reg(dm, RF_PATH_B, RF_WE_LUT, RFREGOFFSETMASK, 0x80000);
	odm_set_rf_reg(dm, RF_PATH_B, RF_RCK_OS, RFREGOFFSETMASK, 0x30000);
	odm_set_rf_reg(dm, RF_PATH_B, RF_TXPA_G1, RFREGOFFSETMASK, 0x0006f);
	odm_set_rf_reg(dm, RF_PATH_B, RF_TXPA_G2, RFREGOFFSETMASK, 0xf1ff3);  */

	/*odm_set_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREGOFFSETMASK, 0xa8018);
	RF_DBG(dm, DBG_RF_IQK, ("0x0 = 0x%x\n", odm_get_rf_reg(dm, RF_PATH_B, RF_0x0, RFREGOFFSETMASK)));
	RF_DBG(dm, DBG_RF_IQK, ("0x80 = 0x%x\n", odm_get_rf_reg(dm, RF_PATH_B, RF_0x80, RFREGOFFSETMASK)));
	RF_DBG(dm, DBG_RF_IQK, ("0x81 = 0x%x\n", odm_get_rf_reg(dm, RF_PATH_B, RF_0x81, RFREGOFFSETMASK)));
	RF_DBG(dm, DBG_RF_IQK, ("0x7f = 0x%x\n", odm_get_rf_reg(dm, RF_PATH_B, RF_0x7f, RFREGOFFSETMASK)));
		*/
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00002, 0x1);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff, 0x0);

	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00800, 0x1);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x56, 0x003ff, 0x67);

	/*disable path-A PI, prevent path-A re-LOK*/
	/*odm_set_bb_reg(dm, R_0x820, BIT(8), 0x0);*/
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCF000C0);
	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);

	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000000);

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

	/* path-B IQK setting */
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x18008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x38008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_B, MASKDWORD, 0x82160027); /*8216031f*/
	odm_set_bb_reg(dm, REG_RX_IQK_PI_B, MASKDWORD, 0x28160000); /*6816031f*/
	/* IQK MAC seting*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x0);

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, ("LO calibration setting!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x0086a911);

	/* One shot, path B LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, ("One shot, path B LOK & IQK!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, ("delay %d ms for One shot, path B LOK & IQK.\n", IQK_DELAY_TIME_92E)); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);
	while ((!odm_get_bb_reg(dm, R_0xeb8, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}

	/* IQK MAC seting reload*/
	odm_set_bb_reg(dm, R_0x040, 0x20, 0x1);
	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT31 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x80000000);
	reg_eb4 = odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_B, MASKDWORD);
	reg_ebc = odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_B, MASKDWORD);

	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT31=0x%x\n", reg_eac,
	       eacBIT31);
	RF_DBG(dm, DBG_RF_IQK, "0xeb4 = 0x%x, 0xebc = 0x%x\n", reg_eb4,
	       reg_ebc);
	/*monitor image power before & after IQK*/
	RF_DBG(dm, DBG_RF_IQK,
	       "0xeb0(before IQK)= 0x%x, 0xeb8(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xeb0, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xeb8, MASKDWORD));

	if (!(reg_eac & BIT(31)) &&
	    (((reg_eb4 & 0x03FF0000) >> 16) != 0x142) &&
	    (((reg_ebc & 0x03FF0000) >> 16) != 0x42))
		result |= 0x01;
	else { /*if Tx not OK, ignore Rx*/
		/*	PA/PAD controlled by 0x0 */
		odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x800, 0x0);

		RF_DBG(dm, DBG_RF_IQK, "pathB get TXIMR is not success\n");
		return result;
	}

	u4tmp = 0x80007C00 | (reg_eb4 & 0x3FF0000) | ((reg_ebc & 0x3FF0000) >> 16);
	odm_set_bb_reg(dm, REG_TX_IQK, MASKDWORD, u4tmp);
	RF_DBG(dm, DBG_RF_IQK, "0xe40 = 0x%x u4tmp = 0x%x\n",
	       odm_get_bb_reg(dm, REG_TX_IQK, MASKDWORD), u4tmp);

	/* 1 RX IQK */
	RF_DBG(dm, DBG_RF_IQK, "Do RXIQK(step2)!\n");

	/* modify RXIQK mode table
	* 	RF_DBG(dm,DBG_RF_IQK, ("path-B Rx IQK modify RXIQK mode table 2!\n")); */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	/*	PA/PAD control by 0x56, and set = 0x0 */
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00002, 0x1);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff, 0x0);

	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00800, 0x1);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x56, 0x003ff, 0x1e0);

	/*	odm_set_rf_reg(dm, RF_PATH_B, RF_0x56, RFREGOFFSETMASK, 0x51000 ); */

	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCf000C0);

	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);

	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000000);

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);

	/* path-B IQK setting */
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x38008c1c);
	odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x18008c1c);

	odm_set_bb_reg(dm, REG_TX_IQK_PI_B, MASKDWORD, 0x82170000);
	odm_set_bb_reg(dm, REG_RX_IQK_PI_B, MASKDWORD, 0x28170000);

	/* IQK setting */
	odm_set_bb_reg(dm, REG_RX_IQK, MASKDWORD, 0x01004800);

	/*	odm_set_bb_reg(dm, REG_TX_IQK_PI_B, MASKDWORD, 0x82160cff);
	 *	odm_set_bb_reg(dm, REG_RX_IQK_PI_B, MASKDWORD, 0x28160cff); */

	/* LO calibration setting
	* 	RF_DBG(dm,DBG_RF_IQK, ("LO calibration setting!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_RSP, MASKDWORD, 0x0046a911);

	/* One shot, path B LOK & IQK
	* 	RF_DBG(dm,DBG_RF_IQK, ("One shot, path B LOK & IQK!\n")); */
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xfa005800);
	odm_set_bb_reg(dm, REG_IQK_AGC_PTS, MASKDWORD, 0xf8005800);

	/* delay x ms
	* 	RF_DBG(dm,DBG_RF_IQK, ("delay %d ms for One shot, path B LOK & IQK.\n", IQK_DELAY_TIME_92E)); */
	/* platform_stall_execution(IQK_DELAY_TIME_92E*1000); */
	ODM_delay_ms(IQK_DELAY_TIME_92F);

	while ((!odm_get_bb_reg(dm, R_0xec8, MASKDWORD)) && ktime < 21) {
		ODM_delay_ms(5);
		ktime = ktime + 5;
	}

	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD);
	eacBIT30 = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, 0x40000000);
	reg_ec4 = odm_get_bb_reg(dm, REG_RX_POWER_BEFORE_IQK_B_2, MASKDWORD);
	reg_ecc = odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_B_2, MASKDWORD);
	RF_DBG(dm, DBG_RF_IQK, "0xeac = 0x%x,eacBIT30 = 0x%x\n", reg_eac,
	       eacBIT30);
	RF_DBG(dm, DBG_RF_IQK, "0xec4 = 0x%x, 0xecc = 0x%x\n", reg_ec4,
	       reg_ecc);
	/* monitor image power before & after IQK */
	RF_DBG(dm, DBG_RF_IQK,
	       "0xec0(before IQK)= 0x%x, 0xec8(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xec0, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xec8, MASKDWORD));

	/*	PA/PAD controlled by 0x0 */
	/* leave IQK mode */
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000100);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00800, 0x0);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0x00002, 0x0);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff, 0x02000);

	if (!(reg_eac & BIT(30)) && /*if Tx is OK, check whether Rx is OK*/
	    (((reg_ec4 & 0x03FF0000) >> 16) != 0x132) &&
	    (((reg_ecc & 0x03FF0000) >> 16) != 0x36))
		result |= 0x02;
	else
		RF_DBG(dm, DBG_RF_IQK, "path B Rx IQK is not success!!\n");

	return result;
}

void _phy_path_a_fill_iqk_matrix_92f(
	struct dm_struct *dm,
	boolean is_iqk_ok,
	s32 result[][8],
	u8 final_candidate,
	boolean is_tx_only)
{
	u32 oldval_0, X, TX0_A, reg;
	s32 Y, TX0_C;
	RF_DBG(dm, DBG_RF_IQK, "path A IQ Calibration %s !\n",
	       (is_iqk_ok) ? "Success" : "Failed");

	if (final_candidate == 0xFF)
		return;

	else if (is_iqk_ok) {
#if (DM_ODM_SUPPORT_TYPE == ODM_AP) && defined(CONFIG_RF_DPK_SETTING_SUPPORT)

		oldval_0 = (odm_get_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, MASKDWORD) >> 22) & 0x3FF;

		odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, 0x3FF, oldval_0);

		X = result[final_candidate][0];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;

		TX0_A = (X * 0x100) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "[IQK] X = 0x%x, TX0_A = 0x%x\n", X,
		       TX0_A);

		odm_set_bb_reg(dm, R_0xe30, 0x3FF00000, TX0_A);

		Y = result[final_candidate][1];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;

		TX0_C = (Y * 0x100) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "[IQK] Y = 0x%x, TX0_C = 0x%x\n", Y,
		       TX0_C);
		odm_set_bb_reg(dm, R_0xe20, 0x000003C0, ((TX0_C & 0x3C0) >> 6));
		odm_set_bb_reg(dm, R_0xe20, 0x0000003F, (TX0_C & 0x3F));

#else

		oldval_0 = (odm_get_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, MASKDWORD) >> 22) & 0x3FF;

		X = result[final_candidate][0];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX0_A = (X * oldval_0) >> 8;
		RF_DBG(dm, DBG_RF_IQK,
		       "X = 0x%x, TX0_A = 0x%x, oldval_0 0x%x\n", X, TX0_A,
		       oldval_0);
		odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, 0x3FF, TX0_A);

		odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(31), ((X * oldval_0 >> 7) & 0x1));

		Y = result[final_candidate][1];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;

		TX0_C = (Y * oldval_0) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "Y = 0x%x, TX = 0x%x\n", Y, TX0_C);
		odm_set_bb_reg(dm, REG_OFDM_0_XC_TX_AFE, 0xF0000000, ((TX0_C & 0x3C0) >> 6));
		odm_set_bb_reg(dm, REG_OFDM_0_XA_TX_IQ_IMBALANCE, 0x003F0000, (TX0_C & 0x3F));

		odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(29), ((Y * oldval_0 >> 7) & 0x1));
#endif

		if (is_tx_only) {
			RF_DBG(dm, DBG_RF_IQK,
			       "_phy_path_a_fill_iqk_matrix_92e only Tx OK\n");
			return;
		}

		reg = result[final_candidate][2];
#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
		if (RTL_ABS(reg, 0x100) >= 16)
			reg = 0x100;
#endif
		odm_set_bb_reg(dm, REG_OFDM_0_XA_RX_IQ_IMBALANCE, 0x3FF, reg);

		reg = result[final_candidate][3] & 0x3F;
		odm_set_bb_reg(dm, REG_OFDM_0_XA_RX_IQ_IMBALANCE, 0xFC00, reg);

		reg = (result[final_candidate][3] >> 6) & 0xF;
		odm_set_bb_reg(dm, REG_OFDM_0_RX_IQ_EXT_ANTA, 0xF0000000, reg);
	}
}

void _phy_path_b_fill_iqk_matrix_92f(
	struct dm_struct *dm,
	boolean is_iqk_ok,
	s32 result[][8],
	u8 final_candidate,
	boolean is_tx_only /* do Tx only */
	)
{
	u32 oldval_1, X, TX1_A, reg;
	s32 Y, TX1_C;
	RF_DBG(dm, DBG_RF_IQK, "path B IQ Calibration %s !\n",
	       (is_iqk_ok) ? "Success" : "Failed");

	if (final_candidate == 0xFF)
		return;

	else if (is_iqk_ok) {
#if (DM_ODM_SUPPORT_TYPE == ODM_AP) && defined(CONFIG_RF_DPK_SETTING_SUPPORT)

		oldval_1 = (odm_get_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, MASKDWORD) >> 22) & 0x3FF;

		odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, 0x3FF, oldval_1);

		X = result[final_candidate][4];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;

		TX1_A = (X * 0x100) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "[IQK] X = 0x%x, TX1_A = 0x%x\n", X,
		       TX1_A);

		odm_set_bb_reg(dm, R_0xe50, 0x3FF00000, TX1_A);

		Y = result[final_candidate][5];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;

		TX1_C = (Y * 0x100) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "[IQK] Y = 0x%x, TX1_C = 0x%x\n", Y,
		       TX1_C);
		odm_set_bb_reg(dm, R_0xe24, 0x000003C0, ((TX1_C & 0x3C0) >> 6));
		odm_set_bb_reg(dm, R_0xe24, 0x0000003F, (TX1_C & 0x3F));
#else

		oldval_1 = (odm_get_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, MASKDWORD) >> 22) & 0x3FF;

		X = result[final_candidate][4];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX1_A = (X * oldval_1) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "X = 0x%x, TX1_A = 0x%x\n", X, TX1_A);
		odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, 0x3FF, TX1_A);

		odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(27), ((X * oldval_1 >> 7) & 0x1));

		Y = result[final_candidate][5];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;

		TX1_C = (Y * oldval_1) >> 8;
		RF_DBG(dm, DBG_RF_IQK, "Y = 0x%x, TX1_C = 0x%x\n", Y, TX1_C);
		odm_set_bb_reg(dm, REG_OFDM_0_XD_TX_AFE, 0xF0000000, ((TX1_C & 0x3C0) >> 6));
		odm_set_bb_reg(dm, REG_OFDM_0_XB_TX_IQ_IMBALANCE, 0x003F0000, (TX1_C & 0x3F));

		odm_set_bb_reg(dm, REG_OFDM_0_ECCA_THRESHOLD, BIT(25), ((Y * oldval_1 >> 7) & 0x1));
#endif

		if (is_tx_only)
			return;

		reg = result[final_candidate][6];
		odm_set_bb_reg(dm, REG_OFDM_0_XB_RX_IQ_IMBALANCE, 0x3FF, reg);

		reg = result[final_candidate][7] & 0x3F;
		odm_set_bb_reg(dm, REG_OFDM_0_XB_RX_IQ_IMBALANCE, 0xFC00, reg);

		reg = (result[final_candidate][7] >> 6) & 0xF;
		odm_set_bb_reg(dm, R_0xca8, 0x000000F0, reg);
	}
}

void _phy_save_adda_registers_92f(
	struct dm_struct *dm,
	u32 *adda_reg,
	u32 *adda_backup,
	u32 register_num)
{
	u32 i;

	if (odm_check_power_status(dm) == false)
		return;

	/*	RF_DBG(dm,DBG_RF_IQK, ("Save ADDA parameters.\n")); */
	for (i = 0; i < register_num; i++)
		adda_backup[i] = odm_get_bb_reg(dm, adda_reg[i], MASKDWORD);
}

void _phy_save_mac_registers_92f(
	struct dm_struct *dm,
	u32 *mac_reg,
	u32 *mac_backup)
{
	u32 i;
	/*	RF_DBG(dm,DBG_RF_IQK, ("Save MAC parameters.\n")); */
	for (i = 0; i < (IQK_MAC_REG_NUM - 1); i++)
		mac_backup[i] = odm_read_1byte(dm, mac_reg[i]);
	mac_backup[i] = odm_read_4byte(dm, mac_reg[i]);
}

void _phy_reload_adda_registers_92f(
	struct dm_struct *dm,
	u32 *adda_reg,
	u32 *adda_backup,
	u32 regiester_num)
{
	u32 i;

	RF_DBG(dm, DBG_RF_IQK, "Reload ADDA power saving parameters !\n");
	for (i = 0; i < regiester_num; i++)
		odm_set_bb_reg(dm, adda_reg[i], MASKDWORD, adda_backup[i]);
}

void _phy_reload_mac_registers_92f(
	struct dm_struct *dm,
	u32 *mac_reg,
	u32 *mac_backup)
{
	u32 i;

	RF_DBG(dm, DBG_RF_IQK, "Reload MAC parameters !\n");
#if 0
	odm_set_bb_reg(dm, R_0x520, MASKBYTE2, 0x0);
#else
	for (i = 0; i < (IQK_MAC_REG_NUM - 1); i++)
		odm_write_1byte(dm, mac_reg[i], (u8)mac_backup[i]);
	odm_write_4byte(dm, mac_reg[i], mac_backup[i]);
#endif
}

void _phy_path_adda_on_92f(
	struct dm_struct *dm,
	boolean is_iqk
)
{
	/*struct dm_struct		*dm	= (struct dm_struct *)dm_void;*/

	/*RF_DBG(dm,DBG_RF_IQK, ("[IQK] ADDA ON.\n"));*/
	if (is_iqk)
		odm_set_bb_reg(dm, R_0x87c, BIT(31), 0x1);
	else
		odm_set_bb_reg(dm, R_0x87c, BIT(31), 0x0);
}

void _phy_mac_setting_calibration_92f(
	struct dm_struct *dm,
	u32 *mac_reg,
	u32 *mac_backup)
{
	/* u32	i = 0; */
	/*	RF_DBG(dm,DBG_RF_IQK, ("MAC settings for Calibration.\n")); */
	/*
		odm_write_1byte(dm, mac_reg[i], 0x3F);

		for(i = 1 ; i < (IQK_MAC_REG_NUM - 1); i++){
			odm_write_1byte(dm, mac_reg[i], (u8)(mac_backup[i]&(~BIT(3))));
		}
		odm_write_1byte(dm, mac_reg[i], (u8)(mac_backup[i]&(~BIT(5))));
	*/

	/* odm_set_bb_reg(dm, R_0x522, MASKBYTE0, 0x7f); */
	/* odm_set_bb_reg(dm, R_0x550, MASKBYTE0, 0x15); */
	/* odm_set_bb_reg(dm, R_0x551, MASKBYTE0, 0x00); */

	odm_set_bb_reg(dm, R_0x520, 0x00ff0000, 0xff);
	/*odm_set_bb_reg(dm, R_0x040, 0x20, 0x0); */
	/*	odm_set_bb_reg(dm, R_0x550, 0x0000ffff, 0x0015); */
}

void _phy_path_a_stand_by_92f(
	struct dm_struct *dm)
{
	/*return;*/
	RF_DBG(dm, DBG_RF_IQK, "path-A standby mode!\n");

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
	/*	odm_set_bb_reg(dm, R_0x840, MASKDWORD, 0x00010000); */
	odm_set_rf_reg(dm, (enum rf_path)0x0, RF_0x0, RFREGOFFSETMASK, 0x10000);

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);
}

void _phy_path_b_stand_by_92f(
	struct dm_struct *dm)
{
	/*return;*/

	RF_DBG(dm, DBG_RF_IQK, "path-B standby mode!\n");

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
	odm_set_rf_reg(dm, (enum rf_path)0x1, RF_0x0, RFREGOFFSETMASK, 0x10000);

	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x808000);
}

void _phy_pi_mode_switch_92f(
	struct dm_struct *dm,
	boolean pi_mode)
{
	u32 mode;

	/*	RF_DBG(dm,DBG_RF_IQK, ("BB Switch to %s mode!\n", (pi_mode ? "PI" : "SI"))); */

	mode = pi_mode ? 0x01000100 : 0x01000000;
	odm_set_bb_reg(dm, REG_FPGA0_XA_HSSI_PARAMETER1, MASKDWORD, mode);
	odm_set_bb_reg(dm, REG_FPGA0_XB_HSSI_PARAMETER1, MASKDWORD, mode);
}

boolean
phy_simularity_compare_8192f(
	struct dm_struct *dm,
	s32 result[][8],
	u8 c1,
	u8 c2,
	boolean is2t)
{
	u32 i, j, diff, simularity_bit_map, bound = 0;
	u8 final_candidate[2] = {0xFF, 0xFF}; /* for path A and path B */
	boolean is_result = true;
	/*#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)*/
	/*	bool		is2T = IS_92C_SERIAL( hal_data->version_id);*/
	/*#else*/
	/*#endif*/

	s32 tmp1 = 0, tmp2 = 0;

	if (is2t)
		bound = 8;
	else
		bound = 4;

	RF_DBG(dm, DBG_RF_IQK,
	       "===> IQK:phy_simularity_compare_8192e c1 %d c2 %d!!!\n", c1,
	       c2);

	simularity_bit_map = 0;

	for (i = 0; i < bound; i++) {
		if (i == 1 || i == 3 || i == 5 || i == 7) {
			if ((result[c1][i] & 0x00000200) != 0)
				tmp1 = result[c1][i] | 0xFFFFFC00;
			else
				tmp1 = result[c1][i];

			if ((result[c2][i] & 0x00000200) != 0)
				tmp2 = result[c2][i] | 0xFFFFFC00;
			else
				tmp2 = result[c2][i];
		} else {
			tmp1 = result[c1][i];
			tmp2 = result[c2][i];
		}

		diff = (tmp1 > tmp2) ? (tmp1 - tmp2) : (tmp2 - tmp1);

		if (diff > MAX_TOLERANCE) {
			RF_DBG(dm, DBG_RF_IQK,
			       "IQK:differnece overflow %d index %d compare1 0x%x compare2 0x%x!!!\n",
			       diff, i, result[c1][i], result[c2][i]);

			if ((i == 2 || i == 6) && !simularity_bit_map) {
				if (result[c1][i] + result[c1][i + 1] == 0)
					final_candidate[(i / 4)] = c2;
				else if (result[c2][i] + result[c2][i + 1] == 0)
					final_candidate[(i / 4)] = c1;
				else
					simularity_bit_map = simularity_bit_map | (1 << i);
			} else
				simularity_bit_map = simularity_bit_map | (1 << i);
		}
	}

	RF_DBG(dm, DBG_RF_IQK,
	       "IQK:phy_simularity_compare_8192e simularity_bit_map   %x !!!\n",
	       simularity_bit_map);

	if (simularity_bit_map == 0) {
		for (i = 0; i < (bound / 4); i++) {
			if (final_candidate[i] != 0xFF) {
				for (j = i * 4; j < (i + 1) * 4 - 2; j++)
					result[3][j] = result[final_candidate[i]][j];
				is_result = false;
			}
		}
		return is_result;
	}

	if (!(simularity_bit_map & 0x03)) { /*path A TX OK*/
		for (i = 0; i < 2; i++)
			result[3][i] = result[c1][i];
	}

	if (!(simularity_bit_map & 0x0c)) { /*path A RX OK*/
		for (i = 2; i < 4; i++)
			result[3][i] = result[c1][i];
	}

	if (!(simularity_bit_map & 0x30)) { /*path B TX OK*/
		for (i = 4; i < 6; i++)
			result[3][i] = result[c1][i];
	}

	if (!(simularity_bit_map & 0xc0)) { /*path B RX OK*/
		for (i = 6; i < 8; i++)
			result[3][i] = result[c1][i];
	}

	return false;
}

void _phy_iq_calibrate_8192f(
	struct dm_struct *dm,
	s32 result[][8],
	u8 t,
	boolean is2T)
{
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);
	u32 i;
	u8 path_aok = 0, path_bok = 0;
	u8 tmp0xc50 = (u8)odm_get_bb_reg(dm, R_0xc50, MASKBYTE0);
	u8 tmp0xc58 = (u8)odm_get_bb_reg(dm, R_0xc58, MASKBYTE0);
	u32 ADDA_REG[2] = {
		0xd94, REG_RX_WAIT_CCA};
	u32 IQK_MAC_REG[IQK_MAC_REG_NUM] = {
		REG_TXPAUSE, REG_BCN_CTRL,
		REG_BCN_CTRL_1, REG_GPIO_MUXCFG};

	/*since 92C & 92D have the different define in IQK_BB_REG*/
	u32 IQK_BB_REG_92C[IQK_BB_REG_NUM] = {
		REG_OFDM_0_TRX_PATH_ENABLE, REG_OFDM_0_TR_MUX_PAR,
		REG_FPGA0_XCD_RF_INTERFACE_SW, REG_CONFIG_ANT_A, REG_CONFIG_ANT_B,
		0x92c, 0x930,
		0x938, REG_CCK_0_AFE_SETTING};

#if MP_DRIVER
	const u32 retry_count = 9;
#else
	const u32 retry_count = 2;
#endif

	/*Note: IQ calibration must be performed after loading*/
	/*PHY_REG.txt,and radio_a,radio_b.txt*/

	/* u32 bbvalue; */

	if (t == 0) {
		/*bbvalue = odm_get_bb_reg(dm, REG_FPGA0_RFMOD, MASKDWORD);*/
		/*RT_DISP(FINIT, INIT_IQK, ("_phy_iq_calibrate_8188e()==>0x%08x\n",bbvalue));*/
		/*RF_DBG(dm,DBG_RF_IQK, ("IQ Calibration for %s for %d times\n", (is2T ? "2T2R" : "1T1R"), t));*/

		/*Save ADDA parameters, turn path A ADDA on*/
		_phy_save_adda_registers_92f(dm, ADDA_REG, cali_info->ADDA_backup, 2);
		_phy_save_mac_registers_92f(dm, IQK_MAC_REG, cali_info->IQK_MAC_backup);
		_phy_save_adda_registers_92f(dm, IQK_BB_REG_92C, cali_info->IQK_BB_backup, IQK_BB_REG_NUM);
	}
	RF_DBG(dm, DBG_RF_IQK,
	       "IQ Calibration for %s for %d times\n",
	       (is2T ? "2T2R" : "1T1R"), t);

	_phy_path_adda_on_92f(dm, true);

#if 0
	if (t == 0)
		cali_info->is_rf_pi_enable = (u8)odm_get_bb_reg(dm, REG_FPGA0_XA_HSSI_PARAMETER1, BIT(8));

	if (!cali_info->is_rf_pi_enable) {
		/*  Switch BB to PI mode to do IQ Calibration. */
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
		_phy_pi_mode_switch_92e(adapter, true);
#else
		_phy_pi_mode_switch_92e(dm, true);
#endif
	}
#endif
	/* MAC settings */
	_phy_mac_setting_calibration_92f(dm, IQK_MAC_REG, cali_info->IQK_MAC_backup);

	/* BB setting */
	/* odm_set_bb_reg(dm, REG_FPGA0_RFMOD, BIT24, 0x00); */
	/*odm_set_bb_reg(dm, REG_CCK_0_AFE_SETTING, 0x0f000000, 0xf);*/
	/*odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xcc3000c0);*/
	/*odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44ffbb44);*/ /*_phy_path_adda_on_92f*/
/*odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005403);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04203400);*/
#if 0
	if (dm->ext_lna && !(dm->ext_pa)) { /* external LNA / external PA = 1 /0 */
		/*PAPE force to high*/
		/*just for high power with external LNA, without external PA*/
		odm_set_bb_reg(dm, R_0x930, 0xf, 0x7);
		odm_set_bb_reg(dm, R_0x930, 0x0f000000, 0x7);
		odm_set_bb_reg(dm, R_0x938, 0xf, 0x7);
		odm_set_bb_reg(dm, R_0x938, 0x0f000000, 0x7);
		odm_set_bb_reg(dm, R_0x92c, MASKDWORD, 0x00410041);
	} else if (dm->ext_pa) { /* external PA = 1*/
		/*PAPE force to low*/
		/*just for high power with external PA, without external LNA*/
		odm_set_bb_reg(dm, R_0x930, 0xf, 0x7);
		odm_set_bb_reg(dm, R_0x930, 0x0f000000, 0x7);
		odm_set_bb_reg(dm, R_0x938, 0xf, 0x7);
		odm_set_bb_reg(dm, R_0x938, 0x0f000000, 0x7);
		odm_set_bb_reg(dm, R_0x92c, MASKDWORD, 0x00000000);
	}
#endif
	if (dm->rfe_type == 7 || dm->rfe_type == 8 || dm->rfe_type == 9 ||
	    dm->rfe_type == 12) {
		/*in ePA IQK, rfe_func_config & SW both pull down*/
		RF_DBG(dm, DBG_RF_IQK, "[IQK] in ePA IQK, rfe_func_config & SW both pull down!!\n");
		/*pathA*/
		odm_set_bb_reg(dm, 0x930, 0xF, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x1, 0x0);
		
		odm_set_bb_reg(dm, 0x930, 0xF00, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x4, 0x0);
	
		odm_set_bb_reg(dm, 0x930, 0xF000, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x8, 0x0);
		/*pathB*/
		odm_set_bb_reg(dm, 0x938, 0xF0, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x20000, 0x0);
	
		odm_set_bb_reg(dm, 0x938, 0xF0000, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x100000, 0x0);
		
		odm_set_bb_reg(dm, 0x93c, 0xF000, 0x7);
		odm_set_bb_reg(dm, 0x92c, 0x8000000, 0x0);
	}

	if (is2T) {
		_phy_path_b_stand_by_92f(dm);
		/* Turn ADDA on */
		/*_phy_path_adda_on_92f(dm, ADDA_REG, false, is2T);*/
	}

/* path A TXIQK */
#if 1
	for (i = 0; i < retry_count; i++) {
		path_aok = phy_path_a_iqk_8192f(dm, is2T);
		/*		if(path_aok == 0x03){ */
		if (path_aok == 0x01) {
			RF_DBG(dm, DBG_RF_IQK, "path A Tx IQK Success!!\n");
			result[t][0] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD) & 0x3FF0000) >> 16;
			result[t][1] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD) & 0x3FF0000) >> 16;
			break;
		}

		RF_DBG(dm, DBG_RF_IQK, "[IQK] path A TXIQK Fail!!\n");
		result[t][0] = 0x100;
		result[t][1] = 0x0;
#if 0
		else if (i == (retry_count - 1) && path_aok == 0x01) {	/*Tx IQK OK*/
			RT_DISP(FINIT, INIT_IQK, ("path A IQK Only  Tx Success!!\n"));

			result[t][0] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD) & 0x3FF0000) >> 16;
			result[t][1] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD) & 0x3FF0000) >> 16;
		}
#endif
	}
#endif

/* path A RXIQK */
#if 1
	for (i = 0; i < retry_count; i++) {
		path_aok = phy_path_a_rx_iqk_92f(dm, is2T);
		if (path_aok == 0x03) {
			RF_DBG(dm, DBG_RF_IQK, "path A Rx IQK Success!!\n");
			/*				result[t][0] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD)&0x3FF0000)>>16;
			 *				result[t][1] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD)&0x3FF0000)>>16; */
			result[t][2] = (odm_get_bb_reg(dm, REG_RX_POWER_BEFORE_IQK_A_2, MASKDWORD) & 0x3FF0000) >> 16;
			result[t][3] = (odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_A_2, MASKDWORD) & 0x3FF0000) >> 16;
			break;
		}

		RF_DBG(dm, DBG_RF_IQK, "path A Rx IQK Fail!!\n");
	}

	if (0x00 == path_aok)
		RF_DBG(dm, DBG_RF_IQK, "path A IQK failed!!\n");

#endif

	if (is2T) {
		_phy_path_a_stand_by_92f(dm);
/* Turn ADDA on */
/*_phy_path_adda_on_92f(dm, ADDA_REG, false, is2T);*/
/* IQ calibration setting */
/*RF_DBG(dm,DBG_RF_IQK, ("IQK setting!\n"));*/

/* path B Tx IQK */
#if 1
		for (i = 0; i < retry_count; i++) {
			path_bok = phy_path_b_iqk_8192f(dm);
			/*		if(path_bok == 0x03){ */
			if (path_bok == 0x01) {
				RF_DBG(dm, DBG_RF_IQK,
				       "path B Tx IQK Success!!\n");
				result[t][4] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_B, MASKDWORD) & 0x3FF0000) >> 16;
				result[t][5] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_B, MASKDWORD) & 0x3FF0000) >> 16;
				break;
			}
#if 0
			else if (i == (retry_count - 1) && path_aok == 0x01) {	/*Tx IQK OK*/
				RT_DISP(FINIT, INIT_IQK, ("path B IQK Only  Tx Success!!\n"));

				result[t][0] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_B, MASKDWORD) & 0x3FF0000) >> 16;
				result[t][1] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_B, MASKDWORD) & 0x3FF0000) >> 16;
			}
#endif
		}
#endif

/* path B RX IQK */
#if 1

		for (i = 0; i < retry_count; i++) {
			path_bok = phy_path_b_rx_iqk_92f(dm, is2T);
			if (path_bok == 0x03) {
				RF_DBG(dm, DBG_RF_IQK,
				       "path B Rx IQK Success!!\n");
				/*				result[t][0] = (odm_get_bb_reg(dm, REG_TX_POWER_BEFORE_IQK_A, MASKDWORD)&0x3FF0000)>>16;
				 *				result[t][1] = (odm_get_bb_reg(dm, REG_TX_POWER_AFTER_IQK_A, MASKDWORD)&0x3FF0000)>>16; */
				result[t][6] = (odm_get_bb_reg(dm, REG_RX_POWER_BEFORE_IQK_B_2, MASKDWORD) & 0x3FF0000) >> 16;
				result[t][7] = (odm_get_bb_reg(dm, REG_RX_POWER_AFTER_IQK_B_2, MASKDWORD) & 0x3FF0000) >> 16;
				break;
			}
		}

		if (0x00 == path_bok)
			RF_DBG(dm, DBG_RF_IQK, "path B IQK failed!!\n");
#endif
	}

	/* Back to BB mode, load original value */
	RF_DBG(dm, DBG_RF_IQK, "IQK:Back to BB mode, load original value!\n");
	odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);

	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xcc0000c0);

	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44bbbb44); /*_phy_path_adda_on_92f*/
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x80408040);
	odm_set_bb_reg(dm, REG_OFDM_0_TRX_PATH_ENABLE, MASKDWORD, 0x6f005433);
	odm_set_bb_reg(dm, REG_OFDM_0_TR_MUX_PAR, MASKDWORD, 0x000004e4);
	odm_set_bb_reg(dm, REG_FPGA0_XCD_RF_INTERFACE_SW, MASKDWORD, 0x04003400);
	odm_set_bb_reg(dm, R_0x820, 0xffffffff, 0x01000100);

	/*if (t != 0) {*/
	/* Reload ADDA power saving parameters*/
	_phy_reload_adda_registers_92f(dm, ADDA_REG, cali_info->ADDA_backup, 2);
	/* Reload MAC parameters*/
	_phy_reload_mac_registers_92f(dm, IQK_MAC_REG, cali_info->IQK_MAC_backup);
	_phy_reload_adda_registers_92f(dm, IQK_BB_REG_92C, cali_info->IQK_BB_backup, IQK_BB_REG_NUM);
	_phy_path_adda_on_92f(dm, false);
	/*Allen initial gain 0xc50*/
	/* Restore RX initial gain*/
	odm_set_bb_reg(dm, R_0xc50, MASKBYTE0, 0x50);
	odm_set_bb_reg(dm, R_0xc50, MASKBYTE0, tmp0xc50);
	if (is2T) {
		odm_set_bb_reg(dm, R_0xc58, MASKBYTE0, 0x50);
		odm_set_bb_reg(dm, R_0xc58, MASKBYTE0, tmp0xc58);
	}
#if 0
		/* load 0xe30 IQC default value */
		odm_set_bb_reg(dm, REG_TX_IQK_TONE_A, MASKDWORD, 0x01008c00);
		odm_set_bb_reg(dm, REG_RX_IQK_TONE_A, MASKDWORD, 0x01008c00);
		odm_set_bb_reg(dm, REG_TX_IQK_TONE_B, MASKDWORD, 0x01008c00);
		odm_set_bb_reg(dm, REG_RX_IQK_TONE_B, MASKDWORD, 0x01008c00);
#endif
	/*}*/
	RF_DBG(dm, DBG_RF_IQK, "%s <==\n", __func__);
}

void _phy_lc_calibrate_8192f(
	struct dm_struct *dm,
	boolean is2T)
{
	struct _hal_rf_ *rf = &dm->rf_table;
	u8 tmp_reg, bb_clk;
	u32 rf_amode = 0, rf_bmode = 0, lc_cal, cnt;
	u32 reg_ce4 = 0;		/*Aries's NarrowBand*/

	RF_DBG(dm, DBG_RF_LCK, "LCK:Start!!!\n");

	/* Aries's NarrowBand*/
	reg_ce4 = odm_get_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)));
	odm_set_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)), 0x0);

	/* Check continuous TX and Packet TX */
	tmp_reg = odm_read_1byte(dm, 0xd03);

	if ((tmp_reg & 0x70) != 0) /*Deal with contisuous TX case*/
		odm_write_1byte(dm, 0xd03, tmp_reg & 0x8F); /*disable all continuous TX*/
	else /* Deal with Packet TX case*/
		odm_write_1byte(dm, REG_TXPAUSE, 0xFF); /* block all queues*/

	/*backup RF0x18*/
	lc_cal = odm_get_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK);

	/*Start LCK*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK, lc_cal | 0x08000);

	for (cnt = 0; cnt < 100; cnt++) {
		if (odm_get_rf_reg(dm, RF_PATH_A, RF_CHNLBW, 0x8000) != 0x1)
			break;

		ODM_delay_ms(10);
	}

	if (cnt == 100)
		RF_DBG(dm, DBG_RF_LCK, "LCK time out\n");

	/*Recover channel number*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK, lc_cal);

	/*Restore original situation*/
	if ((tmp_reg & 0x70) != 0) {
		/*Deal with contisuous TX case*/
		odm_write_1byte(dm, 0xd03, tmp_reg);
	} else {
		/* Deal with Packet TX case*/
		odm_write_1byte(dm, REG_TXPAUSE, 0x00);
	}

	/*Aries's NarrowBand*/
	odm_set_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)), reg_ce4);
	/*reset OFDM state*/
	odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x0);
	odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x1);

	RF_DBG(dm, DBG_RF_LCK, "LCK:Stop!!!\n");
}

#ifdef CONFIG_2G_BAND_SHIFT
void _phy_lck_2g_band_shift_8192f(struct dm_struct *dm,	u8 band_type)
{
	struct _hal_rf_ *rf = &dm->rf_table;
	u8 tmp_reg, bb_clk;
	u32 rf_amode = 0, rf_bmode = 0, lc_cal, cnt;
	u32 reg_ce4 = 0;		/*Aries's NarrowBand*/

	RF_DBG(dm, DBG_RF_LCK, "BAND_SHIFT LCK:Start!!!\n");

	/* Aries's NarrowBand*/
	reg_ce4 = odm_get_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)));
	odm_set_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)), 0x0);

	/* Check continuous TX and Packet TX */
	tmp_reg = odm_read_1byte(dm, 0xd03);

	/*Deal with contisuous TX case*/
	if ((tmp_reg & 0x70) != 0)
		/*disable all continuous TX*/
		odm_write_1byte(dm, 0xd03, tmp_reg & 0x8F);
	else /* Deal with Packet TX case*/
		odm_write_1byte(dm, REG_TXPAUSE, 0xFF); /* block all queues*/
	/*backup RF0x18*/
	lc_cal = odm_get_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK);

	/*Start LCK*/
	switch (band_type) {
	case HAL_RF_2P3:/*2.3GHz LCK*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, MASK12BITS, 0X626);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFF00, 0xE97);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, MASK12BITS, 0x1B4);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0xE800C);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0xB6F46);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x1);
		/*ODM_delay_ms(100);*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK,
			       lc_cal | 0x08000);
		for (cnt = 0; cnt < 100; cnt++) {
			if (odm_get_rf_reg(dm, RF_PATH_A, 0x18, 0x8000) != 0x1)
				break;
			ODM_delay_ms(10);
		}
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x0);
		/*SDM*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x000E7);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, 0xFFFFF, 0x33333);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFFFF, 0x33340);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, 0xFFFFF, 0x0);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0x1666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0x66666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x800E7);
		RF_DBG(dm, DBG_RF_LCK, "LCK:RF BAND = 2.3G.\n");
		break;
	case HAL_RF_2P5:  /*2.5GHz LCK*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, MASK12BITS, 0X6AF);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFF00, 0x720);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, MASK12BITS, 0x1B4);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0xE800D);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0xC8056);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x1);
		/*ODM_delay_ms(100);*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK,
			       lc_cal | 0x08000);
		for (cnt = 0; cnt < 100; cnt++) {
			if (odm_get_rf_reg(dm, RF_PATH_A, 0x18, 0x8000) != 0x1)
				break;
			ODM_delay_ms(10);
		}
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x0);
		/*SDM*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x000FB);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, 0xFFFFF, 0x33333);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFFFF, 0x33340);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, 0xFFFFF, 0x0);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0x1666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0x66666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x800FB);
		RF_DBG(dm, DBG_RF_LCK, "LCK:RF BAND = 2.5G.\n");
		break;
	default: /*2.4GHz LCK*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, MASK12BITS, 0X66B);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFF00, 0x2DB);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, MASK12BITS, 0x1B4);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0xE800D);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0x3F7CE);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x1);
		/*ODM_delay_ms(100);*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK,
			       lc_cal | 0x08000);
		for (cnt = 0; cnt < 100; cnt++) {
			if (odm_get_rf_reg(dm, RF_PATH_A, 0x18, 0x8000) != 0x1)
				break;
			ODM_delay_ms(10);
		}
		odm_set_rf_reg(dm, RF_PATH_A, 0xC9, 0x02000, 0x0);
		/*SDM*/
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x000F1);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBD, 0xFFFFF, 0x33333);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBE, 0xFFFFF, 0x33340);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBF, 0xFFFFF, 0x0);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC0, 0xFFFFF, 0x1666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xC1, 0xFFFFF, 0x66666);
		odm_set_rf_reg(dm, RF_PATH_A, 0xBC, 0xFFFFF, 0x800F1);
		RF_DBG(dm, DBG_RF_LCK, "LCK:RF BAND = 2.4G.\n");
		break;
	}

	/*Recover channel number*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_CHNLBW, RFREGOFFSETMASK, lc_cal);

	/*Restore original situation*/
	if ((tmp_reg & 0x70) != 0) {
		/*Deal with contisuous TX case*/
		odm_write_1byte(dm, 0xd03, tmp_reg);
	} else {
		/* Deal with Packet TX case*/
		odm_write_1byte(dm, REG_TXPAUSE, 0x00);
	}
	/*Aries's NarrowBand*/
	odm_set_bb_reg(dm, R_0xce4, (BIT(31) | BIT(30)), reg_ce4);
	/*reset OFDM state*/
	odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x0);
	odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x1);

	RF_DBG(dm, DBG_RF_LCK, "BAND_SHIFT LCK:Stop!!!\n");
}
#endif
void phy_iq_calibrate_8192f(
	void *dm_void,
	boolean is_recovery)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &(dm->rf_calibrate_info);
	s32 result[4][8]; /* last is final result */
	u32 RFregDFa, RFreg35a, RFregDFb, RFreg35b;
	u8 i, final_candidate, indexforchannel;
	boolean is_patha_ok, is_pathb_ok;
	s32 rege94, rege9c, regea4, regeac, regeb4, regebc, regec4, regecc;
	s32 reg940, reg90c;
	boolean is12simular, is13simular, is23simular;
	u32 IQK_BB_REG_92C[IQK_BB_REG_NUM] = {
		REG_OFDM_0_XA_RX_IQ_IMBALANCE, REG_OFDM_0_XB_RX_IQ_IMBALANCE,
		REG_OFDM_0_ECCA_THRESHOLD, REG_OFDM_0_AGC_RSSI_TABLE,
		REG_OFDM_0_XA_TX_IQ_IMBALANCE, REG_OFDM_0_XB_TX_IQ_IMBALANCE,
		REG_OFDM_0_XC_TX_AFE, REG_OFDM_0_XD_TX_AFE,
		REG_OFDM_0_RX_IQ_EXT_ANTA};

	/*for ODM_WIN*/
	if (is_recovery && !dm->is_in_hct_test) { /*YJ,add for PowerTest,120405*/
		RF_DBG(dm, DBG_RF_INIT,
		       "PHY_IQCalibrate_92E: Return due to is_recovery!\n");
		_phy_reload_adda_registers_92f(dm, IQK_BB_REG_92C, cali_info->IQK_BB_backup_recover, 9);
		return;
	}

	reg940 = odm_get_bb_reg(dm, R_0x940, MASKDWORD);
	reg90c = odm_get_bb_reg(dm, R_0x90c, MASKDWORD);

#if (DM_ODM_SUPPORT_TYPE == ODM_AP) && defined(CONFIG_RF_DPK_SETTING_SUPPORT)
	/*turn off DPK block for IQK conflict*/
	phy_dpkoff_8192f(dm);
#endif

	RF_DBG(dm, DBG_RF_IQK, "IQK:Start!!!\n");

	for (i = 0; i < 8; i++) {
		result[0][i] = 0;
		result[1][i] = 0;
		result[2][i] = 0;

		if (i == 0 || i == 2 || i == 4 || i == 6)
			result[3][i] = 0x100;
		else
			result[3][i] = 0;
	}

	final_candidate = 0xff;
	is_patha_ok = false;
	is_pathb_ok = false;
	is12simular = false;
	is23simular = false;
	is13simular = false;

	RFregDFa = odm_get_rf_reg(dm, RF_PATH_A, RF_0xdf, 0xfffff);
	RFreg35a = odm_get_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff);
	RFregDFb = odm_get_rf_reg(dm, RF_PATH_B, RF_0xdf, 0xfffff);
	RFreg35b = odm_get_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff);
	RF_DBG(dm, DBG_RF_IQK, "Default PATH A 0xdf = 0x%x, 0x35 = 0x%x\n",
	       RFregDFa, RFreg35a);
	RF_DBG(dm, DBG_RF_IQK, "Default PATH B 0xdf = 0x%x, 0x35 = 0x%x\n",
	       RFregDFb, RFreg35b);

	for (i = 0; i < 3; i++) {
		/*	 		_phy_iq_calibrate_8192f(dm, result, i, false); */
		odm_set_bb_reg(dm, REG_FPGA0_IQK, 0xffffff00, 0x000000);
		_phy_iq_calibrate_8192f(dm, result, i, true);
		if (i == 1) {
			is12simular = phy_simularity_compare_8192f(dm, result, 0, 1, true);
			if (is12simular) {
				final_candidate = 0;
				RF_DBG(dm, DBG_RF_IQK,
				       "IQK: is12simular final_candidate is %x\n",
				       final_candidate);
				break;
			}
		}

		if (i == 2) {
			is13simular = phy_simularity_compare_8192f(dm, result, 0, 2, true);
			if (is13simular) {
				final_candidate = 0;
				RF_DBG(dm, DBG_RF_IQK,
				       "IQK: is13simular final_candidate is %x\n",
				       final_candidate);

				break;
			}
			is23simular = phy_simularity_compare_8192f(dm, result, 1, 2, true);
			if (is23simular) {
				final_candidate = 1;
				RF_DBG(dm, DBG_RF_IQK,
				       "IQK: is23simular final_candidate is %x\n",
				       final_candidate);
			} else {
				/*
								for(i = 0; i < 4; i++)
									reg_tmp &= result[3][i*2];

								if(reg_tmp != 0)
									final_candidate = 3;
								else
									final_candidate = 0xFF;
				*/
				final_candidate = 3;
			}
		}
	}

	for (i = 0; i < 4; i++) {
		rege94 = result[i][0];
		rege9c = result[i][1];
		regea4 = result[i][2];
		regeac = result[i][3];
		regeb4 = result[i][4];
		regebc = result[i][5];
		regec4 = result[i][6];
		regecc = result[i][7];
		RF_DBG(dm, DBG_RF_IQK,
		       "IQK: rege94=%x rege9c=%x regea4=%x regeac=%x regeb4=%x regebc=%x regec4=%x regecc=%x\n ",
		       rege94, rege9c, regea4, regeac, regeb4, regebc, regec4,
		       regecc);
	}

	if (final_candidate != 0xff) {
		cali_info->rege94 = rege94 = result[final_candidate][0];
		cali_info->rege9c = rege9c = result[final_candidate][1];
		regea4 = result[final_candidate][2];
		regeac = result[final_candidate][3];
		cali_info->regeb4 = regeb4 = result[final_candidate][4];
		cali_info->regebc = regebc = result[final_candidate][5];
		regec4 = result[final_candidate][6];
		regecc = result[final_candidate][7];
		RF_DBG(dm, DBG_RF_IQK, "IQK: final_candidate is %x\n",
		       final_candidate);
		RF_DBG(dm, DBG_RF_IQK,
		       "IQK: TX0_X=%x TX0_Y=%x RX0_X=%x RX0_Y=%x TX1_X=%x TX1_Y=%x RX1_X=%x RX1_Y=%x\n ",
		       rege94, rege9c, regea4, regeac, regeb4, regebc, regec4,
		       regecc);
		is_patha_ok = is_pathb_ok = true;
	} else {
		RF_DBG(dm, DBG_RF_IQK, "IQK: FAIL use default value\n");

		cali_info->rege94 = cali_info->regeb4 = 0x100; /* X default value */
		cali_info->rege9c = cali_info->regebc = 0x0; /* Y default value */
	}

	odm_set_bb_reg(dm, R_0xe30, 0x3FF00000, 0x100);
	odm_set_bb_reg(dm, R_0xe20, 0x3FF, 0x000);
	odm_set_bb_reg(dm, R_0xe50, 0x3FF00000, 0x100);
	odm_set_bb_reg(dm, R_0xe24, 0x3FF, 0x000);

	if ((rege94 != 0) /*&&(regea4 != 0)*/)
		_phy_path_a_fill_iqk_matrix_92f(dm, is_patha_ok, result,
						final_candidate, (regea4 == 0));
	if (regeb4 != 0 /*&&(regec4 != 0)*/)
		_phy_path_b_fill_iqk_matrix_92f(dm, is_pathb_ok, result,
						final_candidate, (regec4 == 0));

	indexforchannel = odm_get_right_chnl_place_for_iqk(*dm->channel);

	/* To Fix BSOD when final_candidate is 0xff
	 * by sherry 20120321 */
	if (final_candidate < 4) {
		for (i = 0; i < iqk_matrix_reg_num; i++)
			cali_info->iqk_matrix_reg_setting[indexforchannel].value[0][i] = result[final_candidate][i];
		cali_info->iqk_matrix_reg_setting[indexforchannel].is_iqk_done = true;
	}
	/* RT_DISP(FINIT, INIT_IQK, ("\nIQK OK indexforchannel %d.\n", indexforchannel)); */
	RF_DBG(dm, DBG_RF_IQK, "\nIQK OK indexforchannel %d.\n",
	       indexforchannel);
	_phy_save_adda_registers_92f(dm, IQK_BB_REG_92C, cali_info->IQK_BB_backup_recover, IQK_BB_REG_NUM);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0xfffff, RFregDFa);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff, RFreg35a);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0xdf, 0xfffff, RFregDFb);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff, RFreg35b);

	RF_DBG(dm, DBG_RF_IQK, "Back to default 0xdf, 0x35 !\n");

	RFregDFa = odm_get_rf_reg(dm, RF_PATH_A, RF_0xdf, 0xfffff);
	RFreg35a = odm_get_rf_reg(dm, RF_PATH_A, RF_0x35, 0xfffff);
	RFregDFb = odm_get_rf_reg(dm, RF_PATH_B, RF_0xdf, 0xfffff);
	RFreg35b = odm_get_rf_reg(dm, RF_PATH_B, RF_0x35, 0xfffff);

	RF_DBG(dm, DBG_RF_IQK, "PATH A 0xdf = 0x%x, 0x35 = 0x%x\n", RFregDFa,
	       RFreg35a);
	RF_DBG(dm, DBG_RF_IQK, "PATH B 0xdf = 0x%x, 0x35 = 0x%x\n", RFregDFb,
	       RFreg35b);

	RF_DBG(dm, DBG_RF_IQK, "0xc80 = 0x%x, 0xc94 = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xc80, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xc94, MASKDWORD));
	RF_DBG(dm, DBG_RF_IQK, "0xc88 = 0x%x, 0xc9c = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xc88, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xc9c, MASKDWORD));
	RF_DBG(dm, DBG_RF_IQK, "0xc14 = 0x%x, 0xca0 = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xc14, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xca0, MASKDWORD));
	RF_DBG(dm, DBG_RF_IQK, "0xc1c = 0x%x, 0xc78 = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xc1c, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xc78, MASKDWORD));

	RF_DBG(dm, DBG_RF_IQK, "IQK finished\n");

	RF_DBG(dm, DBG_RF_IQK, "PATH A 0x0 = 0x%x, 0x56 = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x0, 0xfffff),
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x56, 0xfffff));

	RF_DBG(dm, DBG_RF_IQK, "PATH B 0x0 = 0x%x, 0x56 = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x0, 0xfffff),
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x56, 0xfffff));

	if (dm->rfe_type == 7) {
		odm_set_bb_reg(dm, R_0x103c, 0x70000, 0x7);
		odm_set_bb_reg(dm, R_0x4c, 0x6c00000, 0x0);
		odm_set_bb_reg(dm, R_0x64, BIT(29) | BIT(28), 0x3);
		odm_set_bb_reg(dm, R_0x1038, 0x600000 | BIT(4), 0x0);
		odm_set_bb_reg(dm, R_0x944, MASKLWORD, 0x081F);
		odm_set_bb_reg(dm, R_0x930, 0xFFFFF, 0x23200);
		odm_set_bb_reg(dm, R_0x938, 0xFFFFF, 0x23200);
		odm_set_bb_reg(dm, R_0x934, 0xF000, 0x3);
		odm_set_bb_reg(dm, R_0x93c, 0xF000, 0x3);
		odm_set_bb_reg(dm, R_0x968, BIT(2), 0x0);
		odm_set_bb_reg(dm, R_0x920, MASKDWORD, 0x03000003);
		odm_set_bb_reg(dm, R_0x940, MASKDWORD, reg940);
	
	} else if (dm->rfe_type == 8 || dm->rfe_type == 9 ||
		  dm->rfe_type == 12) {
		odm_set_bb_reg(dm, R_0x103c, 0x70000, 0x7);
		odm_set_bb_reg(dm, R_0x4c, 0x6c00000, 0x0);
		odm_set_bb_reg(dm, R_0x64, BIT(29) | BIT(28), 0x3);
		odm_set_bb_reg(dm, R_0x1038, 0x600000 | BIT(4), 0x0);
		odm_set_bb_reg(dm, R_0x944, MASKLWORD, 0x081F);
		odm_set_bb_reg(dm, R_0x930, 0xFFFFF, 0x22200);
		odm_set_bb_reg(dm, R_0x938, 0xFFFFF, 0x22200);
		odm_set_bb_reg(dm, R_0x934, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x93c, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x968, BIT(2), 0x0);
		odm_set_bb_reg(dm, R_0x940, MASKDWORD, reg940);
	}

#if (DM_ODM_SUPPORT_TYPE == ODM_AP) && defined(CONFIG_RF_DPK_SETTING_SUPPORT)
	/*turn on DPK block for TXIQC 0xe30, 0xe50*/
	phy_dpkon_8192f(dm);
#endif
}

void phy_lc_calibrate_8192f(
	void *dm_void)
{
	boolean is_single_tone = false, is_carrier_suppression = false;
	u32 timeout = 2000, timecount = 0;
	u8 band_type_2g_8192f;
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct _hal_rf_ *rf = &(dm->rf_table);

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN))
	if (odm_check_power_status(dm) == false)
		return;
#endif

#if (MP_DRIVER)
	if (*(dm->mp_mode) &&
	    ((*(rf->is_con_tx) || *(rf->is_single_tone) || *(rf->is_carrier_suppresion))))
		return;
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_CE)
	if (!(rf->rf_supportability & HAL_RF_IQK))
		return;
#endif

#if DISABLE_BB_RF
	return;
#endif

	while (*(dm->is_scan_in_process)) {
		RF_DBG(dm, DBG_RF_LCK, "[LCK]scan is in process, bypass LCK\n");
		return;
	}

	dm->rf_calibrate_info.is_lck_in_progress = true;

#ifndef CONFIG_2G_BAND_SHIFT
	RF_DBG(dm, DBG_RF_LCK, "LCK start!!!\n");
	_phy_lc_calibrate_8192f(dm, true);
	dm->rf_calibrate_info.is_lck_in_progress = false;
	RF_DBG(dm, DBG_RF_LCK, "LCK:Finish!!!\n");
#else
//#if (DM_ODM_SUPPORT_TYPE == ODM_CE)
	if (rf->rf_supportability & HAL_2GBAND_SHIFT) {
		band_type_2g_8192f = rf->rf_shift_band;
		RF_DBG(dm, DBG_RF_LCK, "LCK:rf_shift_band = %d\n",
		       band_type_2g_8192f);
		_phy_lck_2g_band_shift_8192f(dm, band_type_2g_8192f);
		dm->rf_calibrate_info.is_lck_in_progress = false;
	} else {
		_phy_lc_calibrate_8192f(dm, true);
		dm->rf_calibrate_info.is_lck_in_progress = false;
	}
//endif
#endif
}

void _phy_set_rf_path_switch_8192f(
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct dm_struct *dm,
#else
	struct _ADAPTER *adapter,
#endif
	boolean is_main,
	boolean is2T)
{
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
	HAL_DATA_TYPE *hal_data = GET_HAL_DATA(adapter);
#if (DM_ODM_SUPPORT_TYPE == ODM_CE)
	struct dm_struct *dm = &hal_data->odmpriv;
#elif (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	struct dm_struct *dm = &hal_data->DM_OutSrc;
#endif
#endif

	if (dm->rfe_type == 11 || dm->rfe_type == 13) {
		/*CUTB+RFE11(8725A SDIO QFN52 iLNA iPA with BT)*/
		odm_set_mac_reg(dm, R_0x40, BIT(8), 0x0);
		odm_set_mac_reg(dm, R_0x1038, BIT(0), 0x1);
		odm_set_mac_reg(dm, R_0x40, BIT(2), 0x1);
		odm_set_mac_reg(dm, R_0x1038, BIT(28) | BIT(27), 0x3);
		/*SW*/
		odm_set_mac_reg(dm, R_0x4c, BIT(27), 0x1);
	if (is_main)
		/*WIFI*/
		odm_set_mac_reg(dm, R_0x103c, BIT(15) | BIT(14), 0x2);
	else
		/*BT*/
		odm_set_mac_reg(dm, R_0x103c, BIT(15) | BIT(14), 0x1);
	}
}

void phy_set_rf_path_switch_8192f(
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct dm_struct *dm,
#else
	struct _ADAPTER *adapter,
#endif
	boolean is_main)
{
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
/* HAL_DATA_TYPE	*hal_data = GET_HAL_DATA(adapter); */
#endif

#if DISABLE_BB_RF
	return;
#endif

#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)

	_phy_set_rf_path_switch_8192f(adapter, is_main, true);
#endif
}

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
/* return value true => Main; false => Aux */
boolean _phy_query_rf_path_switch_8192f(
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct dm_struct *dm,
#else
	struct _ADAPTER *adapter,
#endif
	boolean is2T)
{
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
	HAL_DATA_TYPE *hal_data = GET_HAL_DATA(adapter);
#if (DM_ODM_SUPPORT_TYPE == ODM_CE)
	struct dm_struct *dm = &hal_data->odmpriv;
#endif
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	struct dm_struct *dm = &hal_data->DM_OutSrc;
#endif
#endif
	/* MAIN: WiFi */
	if (dm->rfe_type == 11 || dm->rfe_type == 13) {
		if (odm_get_mac_reg(dm, R_0x103c, BIT(15) | BIT(14)) == 0x2)
			return true;
		else
			return false;
	}
	return true;
}

/* return value true => Main; false => Aux */
boolean phy_query_rf_path_switch_8192f(
#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	struct dm_struct *dm
#else
	struct _ADAPTER *adapter
#endif
	)
{
	HAL_DATA_TYPE *hal_data = GET_HAL_DATA(adapter);

#if DISABLE_BB_RF
	return true;
#endif
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
	if (IS_2T2R(hal_data->VersionID))
		return _phy_query_rf_path_switch_8192f(adapter, true);
#endif
/* For 88C 1T1R */
#if !(DM_ODM_SUPPORT_TYPE & ODM_AP)
	return _phy_query_rf_path_switch_8192f(adapter, false);
#else
	return _phy_query_rf_path_switch_8192f(dm, false);
#endif
}
#endif

#else /* #if (RTL8192f_SUPPORT == 1)*/

void phy_iq_calibrate_8192f(
	void *dm_void,
	boolean is_recovery) {}
void phy_lc_calibrate_8192f(
	void *dm_void) {}

void odm_tx_pwr_track_set_pwr92_f(
	void *dm_void,
	enum pwrtrack_method method,
	u8 rf_path,
	u8 channel_mapped_index) {}

#endif
