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
#include "../phydm_precomp.h"

#if (RTL8192F_SUPPORT == 1)

/* ======================================================================== */
/* These following functions can be used for PHY DM only*/

__iram_odm_func__
void phydm_soml_switch_8192f(
	struct dm_struct *dm,
	boolean is_soml_en)
{
	u32 reg998;
	reg998 = odm_get_bb_reg(dm, R_0x998, MASKDWORD);
	if (is_soml_en == true)
		reg998 |= BIT(6);
	else
		reg998 &= (~BIT(6));
	odm_set_bb_reg(dm, R_0x998, MASKDWORD, reg998);
}

void phydm_rx_dfir_par_by_bw_8192f(
	struct dm_struct *dm,
	enum channel_width bandwidth)
{
	odm_set_bb_reg(dm, ODM_REG_TAP_UPD_97F, (BIT(21) | BIT(20)), 0x2);
	odm_set_bb_reg(dm, ODM_REG_DOWNSAM_FACTOR_11N, (BIT(29) | BIT(28)), 0x2);

	if (bandwidth == CHANNEL_WIDTH_40) {
		/* RX DFIR for BW40 */
		odm_set_bb_reg(dm, ODM_REG_RX_DFIR_MOD_97F, BIT(8), 0x0);
		odm_set_bb_reg(dm, ODM_REG_RX_DFIR_MOD_97F, MASKBYTE0, 0x3);
	} else {
		/* RX DFIR for BW20, BW10 and BW5*/
		odm_set_bb_reg(dm, ODM_REG_RX_DFIR_MOD_97F, BIT(8), 0x1);
		odm_set_bb_reg(dm, ODM_REG_RX_DFIR_MOD_97F, MASKBYTE0, 0xa3);
	}
}

void phydm_init_hw_info_by_rfe_type_8192f(
	struct dm_struct *dm, enum bb_path tx_path, enum bb_path rx_path
)
{
	if (dm->rfe_type == 7) {
		/*CUTB+RFE7(PCIE QFN46_SW2576L+SKY85201 wi RX bypass mode);*/
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
		if (tx_path == BB_PATH_A || rx_path == BB_PATH_A)
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x000007AE);
		else if (tx_path == BB_PATH_B || rx_path == BB_PATH_B)
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004007EE);
		else
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004007AE);
		odm_cmn_info_init(dm, ODM_CMNINFO_EXT_LNA, true);
		odm_cmn_info_init(dm, ODM_CMNINFO_EXT_PA, true);
	}
	else if (dm->rfe_type == 15) {
		/*CUTB+RFE8(PCIE QFN46_iPA+RXFEM(MAXSCEND MXD7220CT) wi RX bypass mode )*/
		odm_set_bb_reg(dm, R_0x103c, 0x70000, 0x7);
		odm_set_bb_reg(dm, R_0x4c, 0x6c00000, 0x0);
		odm_set_bb_reg(dm, R_0x64, BIT(29) | BIT(28), 0x3);
		odm_set_bb_reg(dm, R_0x1038, 0x600000 | BIT(4), 0x0);
		odm_set_bb_reg(dm, R_0x944, MASKLWORD, 0x081F);
		odm_set_bb_reg(dm, R_0x930, 0xFFFFF, 0x32322);
		odm_set_bb_reg(dm, R_0x938, 0xFFFFF, 0x32322);
		odm_set_bb_reg(dm, R_0x934, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x93c, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x92c, MASKDWORD, 0x00170017);
		odm_set_bb_reg(dm, R_0x968, BIT(2), 0x0);
		odm_set_bb_reg(dm, R_0x920, MASKDWORD, 0x03000003);
		if (tx_path == BB_PATH_A || rx_path == BB_PATH_A)
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004006AE);
		else if (tx_path == BB_PATH_B || rx_path == BB_PATH_B)
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004007BE);
		else
			odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004007AE);
		odm_cmn_info_init(dm, ODM_CMNINFO_EXT_LNA, true);
		
	}
	else if (dm->rfe_type == 8 || dm->rfe_type == 9 ||
			   dm->rfe_type == 12) {
		/*CUTB+RFE8(PCIE QFN46_RTL6691+BFP740+RTC6603 wo bypass)*/
		odm_set_bb_reg(dm, R_0x103c, 0x70000, 0x7);
		odm_set_bb_reg(dm, R_0x4c, 0x6c00000, 0x0);
		odm_set_bb_reg(dm, R_0x64, BIT(29) | BIT(28), 0x3);
		odm_set_bb_reg(dm, R_0x1038, 0x600000 | BIT(4), 0x0);
		odm_set_bb_reg(dm, R_0x944, MASKLWORD, 0x081F);
		odm_set_bb_reg(dm, R_0x930, 0xFFFFF, 0x22200);
		odm_set_bb_reg(dm, R_0x938, 0xFFFFF, 0x22200);
		odm_set_bb_reg(dm, R_0x934, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x93c, 0xF000, 0x2);
		odm_set_bb_reg(dm, R_0x92c, MASKDWORD, 0x08000008);
		odm_set_bb_reg(dm, R_0x968, BIT(2), 0x0);
		odm_set_bb_reg(dm, R_0x940, MASKDWORD, 0x004007AE);
	} else if (dm->rfe_type == 11 || dm->rfe_type == 13) {
		/*CUTB+RFE11(8725A SDIO QFN52 iLNA iPA with BT)*/
		odm_set_bb_reg(dm, R_0x40, BIT(8), 0x0);
		odm_set_bb_reg(dm, R_0x1038, BIT(0), 0x1);
		odm_set_bb_reg(dm, R_0x40, BIT(2), 0x1);
		odm_set_bb_reg(dm, R_0x1038, BIT(28) | BIT(27), 0x3);
		/*SW*/
		/*odm_set_bb_reg(dm, R_0x4c, BIT(27), 0x1);*/
		/*WIFI*/
		/*odm_set_bb_reg(dm, R_0x103c, BIT(15) | BIT(14), 0x2);*/
		/*BT*/
		/*odm_set_bb_reg(dm, R_0x103c, BIT(15) | BIT(14), 0x1);*/
		/*HW*/
		odm_set_bb_reg(dm, R_0x4c, BIT(27), 0x0);
		odm_set_bb_reg(dm, R_0x944, BIT(7) | BIT(6), 0x3);
		odm_set_bb_reg(dm, R_0x940, 0xF000, 0x0);
		odm_set_bb_reg(dm, R_0x944, BIT(31), 0x0);
		odm_set_bb_reg(dm, R_0x92c, BIT(7) | BIT(6), 0x1);
		odm_set_bb_reg(dm, R_0x930, 0xFF000000, 0x66);
	}

	/*1.1V -> 1.05V  for spur*/
	if (dm->rfe_type == 10 || dm->rfe_type == 11 ||
	    dm->rfe_type == 13) {
		if (odm_get_bb_reg(dm, R_0xf0, BIT(24)) == 0)/*SPS*/
			odm_set_bb_reg(dm, R_0x14, 0x00f00000, 0x6);
		else/*LDO*/
			odm_set_bb_reg(dm, R_0x14, 0x00f00000, 0xE);
	}

	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: RFE type (%d)\n",
		  __func__, dm->rfe_type);
}

/* ======================================================================== */

/* ======================================================================== */
/* These following functions can be used by driver*/

u32 config_phydm_read_rf_reg_8192f(
	struct dm_struct *dm,
	enum rf_path rf_path,
	u32 reg_addr,
	u32 bit_mask)
{
	u32 readback_value, direct_addr;
	u32 offset_read_rf[2] = {0x2800, 0x2c00};

	/* Error handling.*/
	if (rf_path > RF_PATH_B) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: unsupported path (%d)\n",
			  __func__, rf_path);
		return INVALID_RF_DATA;
	}

	/* Calculate offset */
	reg_addr &= 0xff;
	direct_addr = offset_read_rf[rf_path] + (reg_addr << 2);

	/* RF register only has 20bits */
	bit_mask &= RFREGOFFSETMASK;

	/* Read RF register directly */
	readback_value = odm_get_bb_reg(dm, direct_addr, bit_mask);
	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: RF-%d 0x%x = 0x%x, bit mask = 0x%x\n", __func__, rf_path,
		  reg_addr, readback_value, bit_mask);
	return readback_value;
}

boolean
config_phydm_write_rf_reg_8192f(
	struct dm_struct *dm,
	enum rf_path rf_path,
	u32 reg_addr,
	u32 bit_mask,
	u32 data)
{
	u32 data_and_addr = 0, data_original = 0;
	u32 offset_write_rf[2] = {0x840, 0x844};
	u8 bit_shift;

	/* Error handling.*/
	if (rf_path > RF_PATH_B) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: unsupported path (%d)\n",
			  __func__, rf_path);
		return false;
	}

	/* Read RF register content first */
	reg_addr &= 0xff;
	bit_mask = bit_mask & RFREGOFFSETMASK;

	if (bit_mask != RFREGOFFSETMASK) {
		data_original = config_phydm_read_rf_reg_8192f(dm, rf_path, reg_addr, RFREGOFFSETMASK);

		/* Error handling. RF is disabled */
		if (config_phydm_read_rf_check_8192f(data_original) == false) {
			PHYDM_DBG(dm, ODM_PHY_CONFIG,
				  "%s: Write fail, RF is disable\n", __func__);
			return false;
		}

		/* check bit mask */
		if (bit_mask != 0xfffff) {
			for (bit_shift = 0; bit_shift <= 19; bit_shift++) {
				if (((bit_mask >> bit_shift) & 0x1) == 1)
					break;
			}
			data = ((data_original) & (~bit_mask)) | (data << bit_shift);
		}
	}

	/* Put write addr in [27:20]  and write data in [19:00] */
	data_and_addr = ((reg_addr << 20) | (data & 0x000fffff)) & 0x0fffffff;

	/* Write operation */
	odm_set_bb_reg(dm, offset_write_rf[rf_path], MASKDWORD, data_and_addr);
	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: RF-%d 0x%x = 0x%x (original: 0x%x), bit mask = 0x%x\n",
		  __func__, rf_path, reg_addr, data, data_original, bit_mask);
	return true;
}

boolean
config_phydm_write_txagc_8192f(
	struct dm_struct *dm,
	u32 power_index,
	enum rf_path path,
	u8 hw_rate)
{
	/*u8	read_back_data;	*/
	/*for 97F workaroud*/
	/* Input need to be HW rate index, not driver rate index!!!! */

	if (dm->is_disable_phy_api) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: disable PHY API for debug!!\n", __func__);
		return true;
	}

	/* Error handling */
	if (path > RF_PATH_B || hw_rate > ODM_RATEMCS15) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: unsupported path (%d)\n",
			  __func__, path);
		return false;
	}

	if (path == RF_PATH_A) {
		switch (hw_rate) {
		case ODM_RATE1M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_CCK_1_MCS32, 0x00007f00, power_index);
			break;
		case ODM_RATE2M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x00007f00, power_index);
			break;
		case ODM_RATE5_5M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x007f0000, power_index);
			break;
		case ODM_RATE11M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x7f000000, power_index);
			break;

		case ODM_RATE6M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x0000007f, power_index);
			break;
		case ODM_RATE9M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x00007f00, power_index);
			break;
		case ODM_RATE12M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x007f0000, power_index);
			break;
		case ODM_RATE18M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x7f000000, power_index);
			break;
		case ODM_RATE24M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x0000007f, power_index);
			break;
		case ODM_RATE36M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x00007f00, power_index);
			break;
		case ODM_RATE48M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x007f0000, power_index);
			break;
		case ODM_RATE54M:
			odm_set_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x7f000000, power_index);
			break;

		case ODM_RATEMCS0:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS1:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS2:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS3:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x7f000000, power_index);
			break;
		case ODM_RATEMCS4:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS5:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS6:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS7:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x7f000000, power_index);
			break;

		case ODM_RATEMCS8:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS9:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS10:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS11:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x7f000000, power_index);
			break;
		case ODM_RATEMCS12:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS13:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS14:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS15:
			odm_set_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x7f000000, power_index);
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else if (path == RF_PATH_B) {
		switch (hw_rate) {
		case ODM_RATE1M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x00007f00, power_index);
			break;
		case ODM_RATE2M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x007f0000, power_index);
			break;
		case ODM_RATE5_5M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x7f000000, power_index);
			break;
		case ODM_RATE11M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x0000007f, power_index);
			break;

		case ODM_RATE6M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x0000007f, power_index);
			break;
		case ODM_RATE9M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x00007f00, power_index);
			break;
		case ODM_RATE12M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x007f0000, power_index);
			break;
		case ODM_RATE18M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x7f000000, power_index);
			break;
		case ODM_RATE24M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x0000007f, power_index);
			break;
		case ODM_RATE36M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x00007f00, power_index);
			break;
		case ODM_RATE48M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x007f0000, power_index);
			break;
		case ODM_RATE54M:
			odm_set_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x7f000000, power_index);
			break;

		case ODM_RATEMCS0:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS1:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS2:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS3:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x7f000000, power_index);
			break;
		case ODM_RATEMCS4:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS5:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS6:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS7:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x7f000000, power_index);
			break;

		case ODM_RATEMCS8:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS9:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS10:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS11:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x7f000000, power_index);
			break;
		case ODM_RATEMCS12:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x0000007f, power_index);
			break;
		case ODM_RATEMCS13:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x00007f00, power_index);
			break;
		case ODM_RATEMCS14:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x007f0000, power_index);
			break;
		case ODM_RATEMCS15:
			odm_set_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x7f000000, power_index);
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid RF path!!\n");

	PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: path-%d rate index 0x%x = 0x%x\n",
		  __func__, path, hw_rate, power_index);
	return true;
}

u8 config_phydm_read_txagc_8192f(
	struct dm_struct *dm,
	enum rf_path path,
	u8 hw_rate)
{
	u8 read_back_data;
	read_back_data = 0;

	/* Input need to be HW rate index, not driver rate index!!!! */

	/* Error handling */
	if (path > RF_PATH_B || hw_rate > ODM_RATEMCS15) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: unsupported path (%d)\n",
			  __func__, path);
		return INVALID_TXAGC_DATA;
	}

	if (path == RF_PATH_A) {
		switch (hw_rate) {
		case ODM_RATE1M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_CCK_1_MCS32, 0x00007f00);
			break;
		case ODM_RATE2M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x00007f00);
			break;
		case ODM_RATE5_5M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x007f0000);
			break;
		case ODM_RATE11M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x7f000000);
			break;

		case ODM_RATE6M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x0000007f);
			break;
		case ODM_RATE9M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x00007f00);
			break;
		case ODM_RATE12M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x007f0000);
			break;
		case ODM_RATE18M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE18_06, 0x7f000000);
			break;
		case ODM_RATE24M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x0000007f);
			break;
		case ODM_RATE36M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x00007f00);
			break;
		case ODM_RATE48M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x007f0000);
			break;
		case ODM_RATE54M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_RATE54_24, 0x7f000000);
			break;

		case ODM_RATEMCS0:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x0000007f);
			break;
		case ODM_RATEMCS1:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x00007f00);
			break;
		case ODM_RATEMCS2:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x007f0000);
			break;
		case ODM_RATEMCS3:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS03_MCS00, 0x7f000000);
			break;
		case ODM_RATEMCS4:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x0000007f);
			break;
		case ODM_RATEMCS5:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x00007f00);
			break;
		case ODM_RATEMCS6:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x007f0000);
			break;
		case ODM_RATEMCS7:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS07_MCS04, 0x7f000000);
			break;

		case ODM_RATEMCS8:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x0000007f);
			break;
		case ODM_RATEMCS9:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x00007f00);
			break;
		case ODM_RATEMCS10:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x007f0000);
			break;
		case ODM_RATEMCS11:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS11_MCS08, 0x7f000000);
			break;
		case ODM_RATEMCS12:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x0000007f);
			break;
		case ODM_RATEMCS13:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x00007f00);
			break;
		case ODM_RATEMCS14:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x007f0000);
			break;
		case ODM_RATEMCS15:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_A_MCS15_MCS12, 0x7f000000);
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else if (path == RF_PATH_B) {
		switch (hw_rate) {
		case ODM_RATE1M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x00007f00);
			break;
		case ODM_RATE2M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x007f0000);
			break;
		case ODM_RATE5_5M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_1_55_MCS32, 0x7f000000);
			break;
		case ODM_RATE11M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_CCK_11_A_CCK_2_11, 0x0000007f);
			break;

		case ODM_RATE6M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x0000007f);
			break;
		case ODM_RATE9M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x00007f00);
			break;
		case ODM_RATE12M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x007f0000);
			break;
		case ODM_RATE18M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE18_06, 0x7f000000);
			break;
		case ODM_RATE24M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x0000007f);
			break;
		case ODM_RATE36M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x00007f00);
			break;
		case ODM_RATE48M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x007f0000);
			break;
		case ODM_RATE54M:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_RATE54_24, 0x7f000000);
			break;

		case ODM_RATEMCS0:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x0000007f);
			break;
		case ODM_RATEMCS1:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x00007f00);
			break;
		case ODM_RATEMCS2:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x007f0000);
			break;
		case ODM_RATEMCS3:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS03_MCS00, 0x7f000000);
			break;
		case ODM_RATEMCS4:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x0000007f);
			break;
		case ODM_RATEMCS5:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x00007f00);
			break;
		case ODM_RATEMCS6:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x007f0000);
			break;
		case ODM_RATEMCS7:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS07_MCS04, 0x7f000000);
			break;

		case ODM_RATEMCS8:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x0000007f);
			break;
		case ODM_RATEMCS9:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x00007f00);
			break;
		case ODM_RATEMCS10:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x007f0000);
			break;
		case ODM_RATEMCS11:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS11_MCS08, 0x7f000000);
			break;
		case ODM_RATEMCS12:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x0000007f);
			break;
		case ODM_RATEMCS13:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x00007f00);
			break;
		case ODM_RATEMCS14:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x007f0000);
			break;
		case ODM_RATEMCS15:
			read_back_data = (u8)odm_get_bb_reg(dm, REG_TX_AGC_B_MCS15_MCS12, 0x7f000000);
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid RF path!!\n");

	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "config_phydm_read_txagc_8197f(): path-%d rate index 0x%x = 0x%x\n",
		  path, hw_rate, read_back_data);
	return read_back_data;
}

void phydm_fix_csi_mask_8725a_rfe10_patha(struct dm_struct *dm)
{
	if (*dm->band_width == CHANNEL_WIDTH_20) {
		switch (*dm->channel) {
		case 13:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x04000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 14:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00080000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	} else if (*dm->band_width == CHANNEL_WIDTH_40) {
		switch (*dm->channel) {
		case 11:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x04000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	}
}

void phydm_fix_csi_mask_8725a_rfe10_pathb(struct dm_struct *dm)
{
	if (*dm->band_width == CHANNEL_WIDTH_20) {
		switch (*dm->channel) {
		case 13:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 14:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00080000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	} else if (*dm->band_width == CHANNEL_WIDTH_40) {
		switch (*dm->channel) {
		case 3:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x04000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 4:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000400);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 5:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 8:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000400);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 10:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000600);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 11:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	}
}

void phydm_fix_csi_mask_8725a_rfe11_patha(struct dm_struct *dm)
{
	if (*dm->band_width == CHANNEL_WIDTH_40) {
		switch (*dm->channel) {
		case 5:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x02000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	}
}

void phydm_fix_csi_mask_8725a_rfe11_pathb(struct dm_struct *dm)
{
	if (*dm->band_width == CHANNEL_WIDTH_20) {
		switch (*dm->channel) {
		case 13:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 14:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00080000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	} else if (*dm->band_width == CHANNEL_WIDTH_40) {
		switch (*dm->channel) {
		case 3:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x04000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 4:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000600);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 5:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 10:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000600);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		case 11:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);
			break;
		}
	}
}

void phydm_dynamic_spur_det_eliminate_8192f(
	struct dm_struct *dm)
{
	u32	freq[11] = {0xFDCD, 0xFD4D, 0xFCCD, 0xFC4D, 0xFFCD,
	 0xFF4D, 0xFECD, 0xFE4D, 0xFDCD, 0xFCCD, 0xFF9A};
	/* {chnl 3, 4, 5, 6, 7, 8, 9, 10, 11,13,14}*/
	u8	idx = 0;
	BOOLEAN	b_donotch = FALSE;
	u8	initial_gain_a = 0;
	u8	initial_gain_b = 0;
	u32	wlan_channel = 0, cur_ch = 0;
	u32	wlan_bw = 0;
	u32 initial_c04 = 0;
	u32	initial_d04 = 0;
	u32	initial_800 = 0;
	u32 report = 0;
	u8  avg_num = 0;
	u32 report_total = 0;
	u32 report_avg = 0;
	u8  delay_idx = 0;

	PHYDM_DBG(dm, ODM_COMP_API, "phydm_dynamic_spur_det_eliminate:channel = %d, band_width = %d\n", *dm->channel, *dm->band_width);
	/*ADC clk 160M to 80M*/
	if (*dm->band_width == CHANNEL_WIDTH_20) {
		if (*dm->channel == 1) {
			/* ADC clock = 80M clock for BW20*/
		    odm_set_bb_reg(dm, R_0x800, (BIT(10) | BIT(9) | BIT(8)), 0x3);
			/* r_adc_upd0 0xca4[27:26] = 2'b01*/
			odm_set_bb_reg(dm, R_0xca4, (BIT(27) | BIT(26)), 0x1);
			/* r_tap_upd 0xe24[21:20] = 2'b01*/
			odm_set_bb_reg(dm, R_0xe24, (BIT(21) | BIT(20)), 0x1);
			/* Down_factor=2 0xc10[29:28]=0x1*/
			odm_set_bb_reg(dm, R_0xc10, (BIT(29) | BIT(28)), 0x1);
			/* Disable DFIR stage 1 0x948[8]=0*/
			odm_set_bb_reg(dm, R_0x948, BIT(8), 0x1);
			/* DFIR stage0=3 0x948[3:0]=0x3*/
			odm_set_bb_reg(dm, R_0x948, MASKBYTE0, 0x8a);
		} else {
			/* ADC clock = 160M clock for BW20*/
			odm_set_bb_reg(dm, R_0x800, (BIT(10) | BIT(9) | BIT(8)), 0x4);
			/* r_adc_upd0 0xca4[27:26] = 2'b10*/
			odm_set_bb_reg(dm, R_0xca4, (BIT(27) | BIT(26)), 0x2);
			/* r_tap_upd 0xe24[21:20] = 2'b10*/
			odm_set_bb_reg(dm, R_0xe24, (BIT(21) | BIT(20)), 0x2);
			/* Down_factor=4 0xc10[29:28]=0x2*/
			odm_set_bb_reg(dm, R_0xc10, (BIT(29) | BIT(28)), 0x2);
			/* enable DFIR stage 1 0x948[8]=1*/
			odm_set_bb_reg(dm, R_0x948, BIT(8), 0x1);
			/* DFIR stage0=3, stage1=10 0x948[3:0]=0x3 0x948[7:4]=0xa*/
			odm_set_bb_reg(dm, R_0x948, MASKBYTE0, 0xa3);
		}
	}

	if (*dm->channel == 3)
		idx = 0;
	else if (*dm->channel == 4)
		idx = 1;
	else if (*dm->channel == 5)
		idx = 2;
	else if (*dm->channel == 6)
		idx = 3;
	else if (*dm->channel == 7)
		idx = 4;
	else if (*dm->channel == 8)
		idx = 5;
	else if (*dm->channel == 9)
		idx = 6;
	else if (*dm->channel == 10)
		idx = 7;
	else
		idx = 15;

	// If wlan at S1 (both HW control & SW control) and current channel=5,6,7,8,13,14
	if (idx <= 10) {
		initial_gain_a = (u8)(odm_get_bb_reg(dm, R_0xc50, MASKBYTE0) & 0x7f);
		initial_gain_b = (u8)(odm_get_bb_reg(dm, R_0xc58, MASKBYTE0) & 0x7f);
		initial_c04 = odm_get_bb_reg(dm, R_0xc04, MASKBYTE0);
		initial_d04 = odm_get_bb_reg(dm, R_0xd04, MASKBYTE0);
		initial_800 = odm_get_bb_reg(dm, R_0x800, MASKBYTE3);

		odm_set_bb_reg(dm, R_0xc58, MASKBYTE0, 0x30);
		odm_set_bb_reg(dm, R_0xc50, MASKBYTE0, 0x7e);
		odm_set_bb_reg(dm, R_0x88c, (BIT(23) | BIT(22) | BIT(21) | BIT(20)), 0xF);/*disable 3-wire*/
		odm_set_bb_reg(dm, R_0x800, BIT(24), 0x0);
		odm_set_bb_reg(dm, R_0xc04, MASKBYTE0, 0x0);
		odm_set_bb_reg(dm, R_0xd04, MASKBYTE0, 0x0);
		odm_set_bb_reg(dm, R_0x80c, BIT(23), 0x0);

		odm_set_bb_reg(dm, R_0x804, MASKBYTE0, 0x13);/* PSD use PATHB*/
		for (avg_num = 0; avg_num < 16; avg_num += 1) {
			odm_set_bb_reg(dm, R_0x808, MASKDWORD, freq[idx]);/* Setup PSD*/
			odm_set_bb_reg(dm, R_0x808, MASKDWORD, 0x400000 | freq[idx]);/* Start PSD*/
			for (delay_idx = 0; delay_idx < 10; delay_idx += 1)
				ODM_delay_us(50);
			report = odm_get_bb_reg(dm, R_0x8b4, MASKDWORD);
			/*DbgPrint("channel = %d, avg_num = %d, report = %x\n",*dm->channel, avg_num, report);*/
			report_total = report_total + report;
		}
		report_avg = report_total >> 4;

		/*PHYDM_DBG(dm, ODM_COMP_API, "report %x\n", report);	*/
		/*DbgPrint("channel = %d, report_avg = %x\n",*dm->channel, report_avg);*/
		if (report_avg > 0x6)
			b_donotch = TRUE;

		odm_set_bb_reg(dm, R_0x808, MASKDWORD, freq[idx]);/* turn off PSD*/
		odm_set_bb_reg(dm, R_0x88c, (BIT(23) | BIT(22) | BIT(21) | BIT(20)), 0x0);/* enable 3-wire*/
		odm_set_bb_reg(dm, R_0xc58, MASKBYTE0, initial_gain_a);
		odm_set_bb_reg(dm, R_0xc50, MASKBYTE0, initial_gain_b);
		odm_set_bb_reg(dm, R_0x800, MASKBYTE3, initial_800);
		odm_set_bb_reg(dm, R_0xc04, MASKBYTE0, initial_c04);
		odm_set_bb_reg(dm, R_0xd04, MASKBYTE0, initial_d04);

	}
	if (b_donotch) {
		/*DbgPrint("b_donotch = %x\n",b_donotch);*/
		if ((dm->rfe_type == 5) && (*dm->band_width == CHANNEL_WIDTH_20)) {/*USB QFN40 iLNA iPA STA,BW 20M*/
			switch (*dm->channel) {
			case 5:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 6:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 7:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 8:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			default:
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
				break;
			}
		} else if ((dm->rfe_type == 5) && (*dm->band_width == CHANNEL_WIDTH_40)) {/*USB QFN40 iLNA iPA STA,BW 40M*/
			switch (*dm->channel) {
			case 3:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 4:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 5:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 6:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 7:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 8:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 9:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 10:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			default:
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
				break;
			}
		} else if ((dm->rfe_type == 0 || dm->rfe_type == 1 ||
			  dm->rfe_type == 3 || dm->rfe_type == 7 ||
			  dm->rfe_type == 8 || dm->rfe_type == 9 ||
			  dm->rfe_type == 12) &&
			  (*dm->band_width == CHANNEL_WIDTH_20)) {
			switch (*dm->channel) {
			case 5:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
				break;
			case 6:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000400);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
				break;
			case 7:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x06000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);
				break;
			case 8:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			default:
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
				break;
			}
		} else if ((dm->rfe_type == 0 || dm->rfe_type == 1 ||
			dm->rfe_type == 3 || dm->rfe_type == 7 ||
			dm->rfe_type == 8 || dm->rfe_type == 9 ||
			dm->rfe_type == 12) &&
			(*dm->band_width == CHANNEL_WIDTH_40)) {
			switch (*dm->channel) {
			case 3:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 4:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000400);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 5:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 6:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000400);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 7:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 8:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 9:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x04000000);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			case 10:
				odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000600);
				odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
				break;
			default:
				odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
				break;
			}
		}
		return;
	}
	if ((dm->rfe_type == 5) && (*dm->band_width == CHANNEL_WIDTH_20)) {/*USB QFN40 iLNA iPA STA,BW 20M*/
		switch (*dm->channel) {
		case 13:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		case 14:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00080000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
			break;
		}
	} else if ((dm->rfe_type == 5) && (*dm->band_width == CHANNEL_WIDTH_40)) {/*USB QFN40 iLNA iPA STA,BW 40M*/
		switch (*dm->channel) {
		case 11:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
			break;
		}
	} else if ((dm->rfe_type == 0 || dm->rfe_type == 1 ||
		  dm->rfe_type == 3 || dm->rfe_type == 7 ||
		  dm->rfe_type == 8 || dm->rfe_type == 9 ||
		  dm->rfe_type == 12) &&
		  (*dm->band_width == CHANNEL_WIDTH_20)) {
		switch (*dm->channel) {
		case 13:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x06000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		case 14:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00080000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
			break;
		}
	} else if ((dm->rfe_type == 0 || dm->rfe_type == 1 ||
		dm->rfe_type == 3 || dm->rfe_type == 7 ||
		dm->rfe_type == 8 || dm->rfe_type == 9 ||
		dm->rfe_type == 12) &&
		(*dm->band_width == CHANNEL_WIDTH_40)) {
		switch (*dm->channel) {
		case 11:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x04000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x1);/*enable CSI mask*/
			break;
		default:
			odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0x00000000);
			odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
			break;
		}
	}
}

void phydm_spur_calibration_8192f(
	struct dm_struct *dm)
{
	u32 path_value;
	if (*dm->is_scan_in_process)
		return;
	if (*dm->band_width == CHANNEL_WIDTH_5)
		return;
	if (*dm->band_width == CHANNEL_WIDTH_10)
		return;
	path_value = odm_get_bb_reg(dm, ODM_REG_BB_RX_PATH_11N, MASKBYTE0);

	if (odm_get_bb_reg(dm, ODM_REG_BB_RX_PATH_11N, MASKBYTE0) != 0x11) {
		phydm_dynamic_spur_det_eliminate_8192f(dm);
	} else {
		odm_set_bb_reg(dm, R_0xd2c, BIT(28), 0x0);/*disable CSI mask	function*/
		/* ADC clock = 160M clock for BW20*/
		odm_set_bb_reg(dm, R_0x800, (BIT(10) | BIT(9) | BIT(8)), 0x4);
		/* r_adc_upd0 0xca4[27:26] = 2'b10*/
		odm_set_bb_reg(dm, R_0xca4, (BIT(27) | BIT(26)), 0x2);
		/* r_tap_upd 0xe24[21:20] = 2'b10*/
		odm_set_bb_reg(dm, R_0xe24, (BIT(21) | BIT(20)), 0x2);
		/* Down_factor=4 0xc10[29:28]=0x2*/
		odm_set_bb_reg(dm, R_0xc10, (BIT(29) | BIT(28)), 0x2);
		if (*dm->band_width == CHANNEL_WIDTH_40) {
			/* disable DFIR stage 1 0x948[8]=0*/
			odm_set_bb_reg(dm, R_0x948, BIT(8), 0x0);
			/* DFIR stage0=3, 0x948[3:0]=0x3*/
			odm_set_bb_reg(dm, R_0x948, MASKBYTE0, 0x3);
		} else {
			/* enable DFIR stage 1 0x948[8]=1*/
			odm_set_bb_reg(dm, R_0x948, BIT(8), 0x1);
			/* DFIR stage0=3, stage1=10*/
			odm_set_bb_reg(dm, R_0x948, MASKBYTE0, 0xa3);
		}
	}
	if (dm->rfe_type == 10) {
		if (path_value == 0x11)
			phydm_fix_csi_mask_8725a_rfe10_patha(dm);
		else
			phydm_fix_csi_mask_8725a_rfe10_pathb(dm);
	} else if (dm->rfe_type == 11) {
		if (path_value == 0x11)
			phydm_fix_csi_mask_8725a_rfe11_patha(dm);
		else
			phydm_fix_csi_mask_8725a_rfe11_pathb(dm);
	}
}

boolean
config_phydm_switch_channel_8192f(
	struct dm_struct *dm,
	u8 central_ch)
{
	struct phydm_dig_struct *dig_tab = &dm->dm_dig_table;
	u32 rf_reg18;
	boolean rf_reg_status = true;

	PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s====================>\n", __func__);

	if (dm->is_disable_phy_api) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: disable PHY API for debug!!\n", __func__);
		return true;
	}

	rf_reg18 = config_phydm_read_rf_reg_8192f(dm, RF_PATH_A, ODM_REG_CHNBW_11N, RFREGOFFSETMASK);
	rf_reg_status = rf_reg_status & config_phydm_read_rf_check_8192f(rf_reg18);

	/* Switch band and channel */
	if (central_ch <= 14) {
		/* 2.4G */
		/*table selection*/
		dig_tab->agc_table_idx = (u8)odm_get_bb_reg(dm, ODM_REG_BB_PWR_SAV2_11N, BIT(12) | BIT(11) | BIT(10) | BIT(9));
		/* 1. RF band and channel*/
		rf_reg18 = (rf_reg18 & (~(BIT(18) | BIT(17) | MASKBYTE0)));
		rf_reg18 = (rf_reg18 | central_ch);

		/* CCK TX filter parameters */
		if (central_ch == 13) {
			odm_set_bb_reg(dm, R_0xa20, MASKDWORD, 0xf8fe0001);
			odm_set_bb_reg(dm, R_0xa24, MASKDWORD, 0x64b80c1c);
			odm_set_bb_reg(dm, R_0xa28, MASKLWORD, 0x8810);
			odm_set_bb_reg(dm, R_0xaac, MASKDWORD, 0x01235667);
		} else if (central_ch == 14) {
			odm_set_bb_reg(dm, R_0xa20, MASKDWORD, 0xe82c0001);
			odm_set_bb_reg(dm, R_0xa24, MASKDWORD, 0x0000b81c);
			odm_set_bb_reg(dm, R_0xa28, MASKLWORD, 0x0000);
			odm_set_bb_reg(dm, R_0xaac, MASKDWORD, 0x00003667);
		} else {
			odm_set_bb_reg(dm, R_0xa20, MASKDWORD, 0xe82c0001);
			odm_set_bb_reg(dm, R_0xa24, MASKDWORD, 0x64b80c1c);
			odm_set_bb_reg(dm, R_0xa28, MASKLWORD, 0x8810);
			odm_set_bb_reg(dm, R_0xaac, MASKDWORD, 0x01235667);
		}
	} else {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Fail to switch band (ch: %d)\n", __func__,
			  central_ch);
		return false;
	}

	odm_set_rf_reg(dm, RF_PATH_A, ODM_REG_CHNBW_11N, RFREGOFFSETMASK, rf_reg18);
	if (dm->rf_type > RF_1T1R)
		odm_set_rf_reg(dm, RF_PATH_B, ODM_REG_CHNBW_11N, RFREGOFFSETMASK, rf_reg18);

	if (rf_reg_status == false) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Fail to switch channel (ch: %d), because writing RF register is fail\n",
			  __func__, central_ch);
		return false;
	}
	/*2400M,2440M,2480M CSI mask for PATHB & PATHAB*/
	if (*dm->mp_mode) {
		phydm_spur_calibration_8192f(dm);
	} else {
#ifdef CONFIG_8912F_SPUR_CALIBRATION
		phydm_spur_calibration_8192f(dm);
#else
		if ((*dm->channel == 1) || (*dm->channel == 11) || (*dm->channel == 13) || (*dm->channel == 14))
			phydm_spur_calibration_8192f(dm);
#endif
	}

	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: Success to switch channel (ch: %d)\n", __func__,
		  central_ch);
	return true;
}

boolean
config_phydm_switch_bandwidth_8192f(
	struct dm_struct *dm,
	u8 primary_ch_idx,
	enum channel_width bandwidth)
{
	u32 rf_reg18;
	boolean rf_reg_status = true;

	PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s===================>\n", __func__);

	if (dm->is_disable_phy_api) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: disable PHY API for debug!!\n", __func__);
		return true;
	}

	/* Error handling */
	if (bandwidth >= CHANNEL_WIDTH_MAX || (bandwidth == CHANNEL_WIDTH_40 && primary_ch_idx > 2)) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Fail to switch bandwidth (bw: %d, primary ch: %d)\n",
			  __func__, bandwidth, primary_ch_idx);
		return false;
	}

	rf_reg18 = config_phydm_read_rf_reg_8192f(dm, RF_PATH_A, ODM_REG_CHNBW_11N, RFREGOFFSETMASK);
	rf_reg_status = rf_reg_status & config_phydm_read_rf_check_8192f(rf_reg18);

	/* Switch bandwidth */
	switch (bandwidth) {
	case CHANNEL_WIDTH_20: {
		/* MAC clk*/
		odm_set_bb_reg(dm, 0x55c, MASKBYTE0, 0x50);
		odm_set_bb_reg(dm, 0x638, MASKBYTE0, 0x50);

		/* Small BW([31:30]) = 0, rf mode(800[0], 900[0]) = 0 for 20M */
		odm_set_bb_reg(dm, ODM_REG_SMALL_BANDWIDTH_11N, (BIT(31) | BIT(30)), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(0), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_PAGE9_11N, BIT(0), 0x0);

		/* ADC clock = 160M clock for BW20*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(10) | BIT(9) | BIT(8)), 0x4);

		/* DAC clock = 80M clock for BW20 */
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(13) | BIT(12)), 0x2);

		/* ADC buffer clock 0xca4[27:26] = 2'b10*/
		odm_set_bb_reg(dm, ODM_REG_ANTDIV_PARA1_11N, (BIT(27) | BIT(26)), 0x2);

		/* enable CCK 0x800[24] = 0x1*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(24), 0x1);

		/* RF bandwidth */
		rf_reg18 = (rf_reg18 | BIT(11) | BIT(10));
		break;
	}
	case CHANNEL_WIDTH_40: {
		/* MAC clk*/
		odm_set_bb_reg(dm, 0x55c, MASKBYTE0, 0x50);
		odm_set_bb_reg(dm, 0x638, MASKBYTE0, 0x50);

		/* Small BW([31:30]) = 0, rf mode(800[0], 900[0]) = 1 for 40M */
		odm_set_bb_reg(dm, ODM_REG_SMALL_BANDWIDTH_11N, (BIT(31) | BIT(30)), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(0), 0x1);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_PAGE9_11N, BIT(0), 0x1);

		/* ADC clock = 160M clock for BW40 no need to setting, it will be setting in PHY_REG */
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(10) | BIT(9) | BIT(8)), 0x4);

		/* DAC clock = 80M clock for BW20 = 3'b10*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(13) | BIT(12)), 0x2);

		/* ADC buffer clock 0xca4[27:26] = 2'b10*/
		odm_set_bb_reg(dm, ODM_REG_ANTDIV_PARA1_11N, (BIT(27) | BIT(26)), 0x2);

		/* CCK primary channel: 1: upper subchannel  0: lower subchannel */
		if (primary_ch_idx == 1)
			odm_set_bb_reg(dm, ODM_REG_CCK_ANTDIV_PARA1_11N, BIT(4), primary_ch_idx);
		else
			odm_set_bb_reg(dm, ODM_REG_CCK_ANTDIV_PARA1_11N, BIT(4), 0);

		/* enable CCK 0x800[24] = 0x1*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(24), 0x1);

		/* RF bandwidth */
		rf_reg18 = (rf_reg18 & (~(BIT(11) | BIT(10))));
		rf_reg18 = (rf_reg18 | BIT(11));
		break;
	}
	case CHANNEL_WIDTH_5: {
		/* Small BW([31:30]) = 0, rf mode(800[0], 900[0]) = 0 for 5M */
		odm_set_bb_reg(dm, ODM_REG_SMALL_BANDWIDTH_11N, (BIT(31) | BIT(30)), 0x1);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(0), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_PAGE9_11N, BIT(0), 0x0);

		/* ADC clock = 40M clock for BW5 */
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(10) | BIT(9) | BIT(8)), 0x2);

		/* DAC clock = 20M clock for BW5 */
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(13) | BIT(12)), 0x0);

		/* ADC buffer clock 0xca4[27:26] = 2'b10*/
		odm_set_bb_reg(dm, ODM_REG_ANTDIV_PARA1_11N, (BIT(27) | BIT(26)), 0x2);

		/* disable CCK 0x800[24] = 0x0*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(24), 0x0);

		/* reset OFDM state*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x1);

		/* RF bandwidth */
		rf_reg18 = (rf_reg18 | BIT(11) | BIT(10));

		break;
	}
	case CHANNEL_WIDTH_10: {
		/* Small BW([31:30]) = 0, rf mode(800[0], 900[0]) = 0 for 10M */
		odm_set_bb_reg(dm, ODM_REG_SMALL_BANDWIDTH_11N, (BIT(31) | BIT(30)), 0x2);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(0), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_PAGE9_11N, BIT(0), 0x0);

		/* ADC clock = 80M clock for BW10*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(10) | BIT(9) | BIT(8)), 0x3);

		/* DAC clock = 40M clock for BW10*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(13) | BIT(12)), 0x1);

		/* ADC buffer clock 0xca4[27:26] = 2'b10*/
		odm_set_bb_reg(dm, ODM_REG_ANTDIV_PARA1_11N, (BIT(27) | BIT(26)), 0x2);

		/* disable CCK 0x800[24] = 0x0*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(24), 0x0);

		/* reset OFDM state*/
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x0);
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, BIT(25), 0x1);

		/* RF bandwidth */
		rf_reg18 = (rf_reg18 | BIT(11) | BIT(10));

		break;
	}
	default:
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Fail to switch bandwidth (bw: %d, primary ch: %d)\n",
			  __func__, bandwidth, primary_ch_idx);
	}

	/* Write RF register */
	odm_set_rf_reg(dm, RF_PATH_A, ODM_REG_CHNBW_11N, RFREGOFFSETMASK, rf_reg18);

	if (dm->rf_type > RF_1T1R)
		odm_set_rf_reg(dm, RF_PATH_B, ODM_REG_CHNBW_11N, RFREGOFFSETMASK, rf_reg18);

	if (rf_reg_status == false) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Fail to switch bandwidth (bw: %d, primary ch: %d), because writing RF register is fail\n",
			  __func__, bandwidth, primary_ch_idx);
		return false;
	}

	/* Modify RX DFIR parameters */
	phydm_rx_dfir_par_by_bw_8192f(dm, bandwidth);

	/* Modify CCA parameters */
	/*phydm_cca_par_by_bw_8192f(p_dm, bandwidth);*/

	#ifdef PHYDM_PRIMARY_CCA
	/* Dynamic Primary CCA */
	phydm_primary_cca(dm);
	#endif

	/*2400M,2440M,2480M CSI mask for PATHB & PATHAB*/
	if (*dm->mp_mode) {
		phydm_spur_calibration_8192f(dm);
	} else {
#ifdef CONFIG_8912F_SPUR_CALIBRATION
		phydm_spur_calibration_8192f(dm);
#else
		if ((*dm->channel == 1) || (*dm->channel == 11) || (*dm->channel == 13) || (*dm->channel == 14))
			phydm_spur_calibration_8192f(dm);
#endif
	}

	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: Success to switch bandwidth (bw: %d, primary ch: %d)\n",
		  __func__, bandwidth, primary_ch_idx);
	return true;
}

boolean
config_phydm_switch_channel_bw_8192f(
	struct dm_struct *dm,
	u8 central_ch,
	u8 primary_ch_idx,
	enum channel_width bandwidth)
{
	u8 e_rf_path = 0;
	u32 rf_val_to_wr, rf_tmp_val, bit_shift, bit_mask;

	/* Switch band */
	/*97F no need*/

	/* Switch channel */
	if (config_phydm_switch_channel_8192f(dm, central_ch) == false)
		return false;

	/* Switch bandwidth */
	if (config_phydm_switch_bandwidth_8192f(dm, primary_ch_idx, bandwidth) == false)
		return false;

	return true;
}

void phydm_dyn_energy_th_8192f(void *dm_void, u32 rx_path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (rx_path == BB_PATH_AB) {
		/* enable auto detection*/
		odm_set_bb_reg(dm, R_0xd08, BIT(17), 0);
		/*dynamic energy TH*/
		odm_set_bb_reg(dm, R_0xcd0, MASKDWORD, 0x593659ad);
	} else if (rx_path == BB_PATH_A) {
		/* enable auto detection*/
		odm_set_bb_reg(dm, R_0xd08, BIT(17), 0);
		/*dynamic energy TH*/
		odm_set_bb_reg(dm, R_0xcd0, MASKDWORD, 0x79b6e3ad);
	} else {
		/* disable auto detection*/
		odm_set_bb_reg(dm, R_0xd08, BIT(17), 1);
		/*dynamic energy TH*/
		odm_set_bb_reg(dm, R_0xcd0, MASKDWORD, 0x79b6e3ad);
	}
}

void phydm_config_rx_path_8192f(struct dm_struct *dm, enum bb_path rx_path)
{
	dm->rx_ant_status = (u8)rx_path;
	/* OFDM Rx path setting*/
	phydm_config_ofdm_rx_path(dm, rx_path);
	phydm_dyn_energy_th_8192f(dm, rx_path);

	/* CCK Rx path setting*/
	phydm_config_cck_rx_path(dm, rx_path);
}

void
phydm_config_ofdm_tx_path_8192f(struct dm_struct *dm, enum bb_path tx_path_en,
				enum bb_path tx_path_sel_1ss)
{
	if (tx_path_en == BB_PATH_AB)
		phydm_config_ofdm_tx_path(dm, tx_path_sel_1ss);
	else if (tx_path_en == BB_PATH_A)
		odm_set_bb_reg(dm, R_0x90c, MASKDWORD, 0x81121311);
	else
		odm_set_bb_reg(dm, R_0x90c, MASKDWORD, 0x82221322);
}

void phydm_config_tx_path_8192f(struct dm_struct *dm, enum bb_path tx_path_en,
				enum bb_path tx_path_sel_1ss,
				enum bb_path tx_path_sel_cck)
{
	dm->tx_ant_status = tx_path_en;
	dm->tx_2ss_status = tx_path_en;
	dm->tx_1ss_status = tx_path_sel_1ss;

	/* @CCK TX antenna mapping */
	phydm_config_cck_tx_path(dm, tx_path_sel_cck);

	/* OFDM tx path setting*/
	phydm_config_ofdm_tx_path_8192f(dm, tx_path_en, tx_path_sel_1ss);

	PHYDM_DBG(dm, DBG_PATH_DIV, "tx_path_sel_1ss=%d, tx_path_sel_cck=%d\n",
		  tx_path_sel_1ss, tx_path_sel_cck);
}

boolean
config_phydm_trx_mode_8192f(
	struct dm_struct *dm,
	enum bb_path tx_path_en,
	enum bb_path rx_path,
	enum bb_path tx_path_sel_1ss)
{
#ifdef CONFIG_PATH_DIVERSITY
	struct _ODM_PATH_DIVERSITY_ *p_div = &dm->dm_path_div;
#endif

	PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s=====================>\n", __func__);

	if (dm->is_disable_phy_api) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: disable PHY API for debug!!\n", __func__);
		return true;
	}

	if ((tx_path_en & ~BB_PATH_AB) != 0) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Wrong TX setting (TX: 0x%x)\n", __func__,
			  tx_path_en);
		return false;
	}

	if ((rx_path & ~BB_PATH_AB) != 0) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Wrong RX setting (RX: 0x%x)\n", __func__,
			  rx_path);
		return false;
	}

	/* ==== [RF Mode Table] =============================================*/
	if (tx_path_en == BB_PATH_AB) {
		odm_set_bb_reg(dm, R_0x824, 0xe, 2);
		odm_set_bb_reg(dm, R_0x82c, 0xe, 2);
	} else if (tx_path_en == BB_PATH_A) {
		odm_set_bb_reg(dm, R_0x824, 0xe, 2);
		odm_set_bb_reg(dm, R_0x82c, 0xe, 1);
	} else {
		odm_set_bb_reg(dm, R_0x824, 0xe, 1);
		odm_set_bb_reg(dm, R_0x82c, 0xe, 2);
	}

	odm_set_bb_reg(dm, R_0x804, 0xf, 0x3);
	/*CCK TX Path control by REG*/
	odm_set_bb_reg(dm, R_0x80c, BIT(31), 0x0);

	/* ==== [RX Path] ===================================================*/
	phydm_config_rx_path_8192f(dm, rx_path);

	/* ==== [TX Path] ===================================================*/
#ifdef CONFIG_PATH_DIVERSITY
	if (tx_path_en == BB_PATH_A || tx_path_en == BB_PATH_B) {
		p_div->stop_path_div = true;
		tx_path_sel_1ss = tx_path_en;
	} else if (tx_path_en == BB_PATH_AB) {
		if (tx_path_sel_1ss == BB_PATH_AUTO) {
			p_div->stop_path_div = false;
			tx_path_sel_1ss = p_div->default_tx_path;
		} else { /* @BB_PATH_AB, BB_PATH_A, BB_PATH_B*/
			p_div->stop_path_div = true;
		}
	}
#else
	tx_path_sel_1ss = tx_path_en;
#endif
	phydm_config_tx_path_8192f(dm, tx_path_en,
				   tx_path_sel_1ss, tx_path_sel_1ss);

	/*2400M,2440M,2480M CSI mask for PATHB & PATHAB*/
	if (*dm->mp_mode) {
		phydm_spur_calibration_8192f(dm);
	} else {
		#ifdef CONFIG_8912F_SPUR_CALIBRATION
		phydm_spur_calibration_8192f(dm);
		#else
		if ((*dm->channel == 1) || (*dm->channel == 11) ||
		    (*dm->channel == 13) || (*dm->channel == 14))
			phydm_spur_calibration_8192f(dm);
		#endif
	}
	/* HW Setting depending on RFE type*/
	phydm_init_hw_info_by_rfe_type_8192f(dm, tx_path_en, rx_path);

	PHYDM_DBG(dm, ODM_PHY_CONFIG,
		  "%s: Success to set TRx mode setting (TX: 0x%x, RX: 0x%x)\n",
		  __func__, tx_path_en, rx_path);
	return true;
}

boolean
config_phydm_parameter_8192f_init(
	struct dm_struct *dm,
	enum odm_parameter_init type)
{
	if (type == ODM_PRE_SETTING) {
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(25) | BIT(24)), 0x0);
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Pre setting: disable OFDM and CCK block\n",
			  __func__);
	} else if (type == ODM_POST_SETTING) {
		odm_set_bb_reg(dm, ODM_REG_BB_CTRL_11N, (BIT(25) | BIT(24)), 0x3);
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: Post setting: enable OFDM and CCK block\n",
			  __func__);
	} else {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: Wrong type!!\n", __func__);
		return false;
	}

	return true;
}

/* ======================================================================== */
#endif /* RTL8192F_SUPPORT == 1 */
