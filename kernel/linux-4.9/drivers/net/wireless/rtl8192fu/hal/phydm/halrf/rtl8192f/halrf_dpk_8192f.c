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
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
#if RT_PLATFORM == PLATFORM_MACOSX
#include "phydm_precomp.h"
#else
#include "../phydm_precomp.h"
#endif
#else
#include "../../phydm_precomp.h"
#endif

#if (RTL8192F_SUPPORT == 1)

/*8192F DPK ver:0xe 20200618*/

u32 _dpk_log2base(u32 val)
{
	u8 j;
	u32 tmp, tmp2, val_integerd_b = 0, tindex, shiftcount = 0;
	u32 result, val_fractiond_b = 0, table_fraction[21] = {0, 432, 332, 274, 232, 200,
							       174, 151, 132, 115, 100, 86, 74, 62, 51, 42,
							       32, 23, 15, 7, 0};

	if (val == 0)
		return 0;

	tmp = val;

	while (1) {
		if (tmp == 1)
			break;

		tmp = (tmp >> 1);
		shiftcount++;
	}

	val_integerd_b = shiftcount + 1;

	tmp2 = 1;
	for (j = 1; j <= val_integerd_b; j++)
		tmp2 = tmp2 * 2;

	tmp = (val * 100) / tmp2;
	tindex = tmp / 5;

	if (tindex > 20)
		tindex = 20;

	val_fractiond_b = table_fraction[tindex];

	result = val_integerd_b * 100 - val_fractiond_b;

	return result;
}

void _backup_mac_bb_registers_8192f(
	struct dm_struct *dm,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)
{
	u32 i;

	RF_DBG(dm, DBG_RF_DPK, "[DPK] Backup MAC/BB parameters !\n");
	for (i = 0; i < reg_num; i++) {
		reg_backup[i] = odm_read_4byte(dm, reg[i]);
		/*RF_DBG(dm, DBG_RF_DPK, "[DPK] Backup 0x%x = 0x%x\n", reg[i], reg_backup[i]);*/
	}
}

void _backup_rf_registers_8192f(
	struct dm_struct *dm,
	u32 *rf_reg,
	u32 rf_reg_backup[][DPK_RF_PATH_NUM_8192F])
{
	u32 i;

	for (i = 0; i < DPK_RF_REG_NUM_8192F; i++) {
		rf_reg_backup[i][RF_PATH_A] = odm_get_rf_reg(dm, RF_PATH_A,
			rf_reg[i], RFREG_MASK);
		rf_reg_backup[i][RF_PATH_B] = odm_get_rf_reg(dm, RF_PATH_B,
			rf_reg[i], RFREG_MASK);
#if 0
		RF_DBG(dm, DBG_RF_DPK, "[DPK] Backup RF_A 0x%x = 0x%x\n",
		       rf_reg[i], rf_reg_backup[i][RF_PATH_A]);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] Backup RF_B 0x%x = 0x%x\n",
		       rf_reg[i], rf_reg_backup[i][RF_PATH_B]);
#endif
	}
}

void _reload_mac_bb_registers_8192f(
	struct dm_struct *dm,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)
{
	u32 i;

	RF_DBG(dm, DBG_RF_DPK, "[DPK] Reload ADDA power saving parameters !\n");
	for (i = 0; i < reg_num; i++) {
		odm_write_4byte(dm, reg[i], reg_backup[i]);
		/*RF_DBG(dm, DBG_RF_DPK, "[DPK] Reload 0x%x = 0x%x\n", reg[i], reg_backup[i]);*/
	}
}

void _reload_rf_registers_8192f(
	struct dm_struct *dm,
	u32 *rf_reg,
	u32 rf_reg_backup[][DPK_RF_PATH_NUM_8192F])
{
	u32 i, rf_reg_8f[DPK_RF_PATH_NUM_8192F] = {0x0};;

	for (i = 0; i < DPK_RF_REG_NUM_8192F; i++) {
		odm_set_rf_reg(dm, RF_PATH_A, rf_reg[i], RFREG_MASK,
			       rf_reg_backup[i][RF_PATH_A]);
		odm_set_rf_reg(dm, RF_PATH_B, rf_reg[i], RFREG_MASK,
			       rf_reg_backup[i][RF_PATH_B]);
#if 0
		RF_DBG(dm, DBG_RF_DPK, "[DPK] Reload RF_A 0x%x = 0x%x\n",
		       rf_reg[i], rf_reg_backup[i][RF_PATH_A]);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] Reload RF_B 0x%x = 0x%x\n",
		       rf_reg[i], rf_reg_backup[i][RF_PATH_B]);
#endif
	}
	/*reload RF 0x8f for non-saving power mode*/
	for (i = 0; i < DPK_RF_PATH_NUM_8192F; i++) {
		rf_reg_8f[i] = odm_get_rf_reg(dm, (enum rf_path)i,
			RF_0x8f, 0x00fff);
		odm_set_rf_reg(dm, (enum rf_path)i, RF_0x8f, RFREG_MASK,
			       0xa8000 | rf_reg_8f[i]);
	}

}

void phy_dpkoff_8192f(
	struct dm_struct *dm)
{
	/*Path A*/
	odm_write_1byte(dm, R_0xb00, 0x18);
	/*odm_write_1byte(dm, R_0xb2b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb2b, 0x00);*/
	/*ODM_delay_ms(10);*/

	/*Path B*/
	odm_write_1byte(dm, R_0xb70, 0x18);
	/*odm_write_1byte(dm, R_0xb9b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb9b, 0x00);*/
	/*ODM_delay_ms(10);*/
}

void phy_dpkon_8192f(
	struct dm_struct *dm)
{
	/*Path A*/
	odm_write_1byte(dm, R_0xb00, 0x98);
	/*odm_write_1byte(dm, R_0xb07, 0xF7);*/
	/*odm_write_1byte(dm, R_0xb2b, 0x00);*/

	/*odm_write_1byte(dm, R_0xb2b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb2b, 0x00);*/
	/*ODM_delay_ms(10);*/

	odm_write_1byte(dm, R_0xb07, 0x77);
	odm_set_bb_reg(dm, R_0xb08, MASKDWORD, 0x4c104c10); /*1.5dB*/
	/*only need 0xe30, 0xe20 if TXIQC in 0xc80*/
	//odm_set_bb_reg(dm, R_0xe30, 0x3FF00000, 0x100);
	//odm_set_bb_reg(dm, R_0xe20, 0x000003FF, 0x0);

	/*Path B*/
	odm_write_1byte(dm, R_0xb70, 0x98);
	/*odm_write_1byte(dm, R_0xb77, 0xF7);*/
	/*odm_write_1byte(dm, R_0xb9b, 0x00);*/

	/*odm_write_1byte(dm, R_0xb9b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb9b, 0x00);*/
	/*ODM_delay_ms(10);*/

	odm_write_1byte(dm, R_0xb77, 0x77);
	odm_set_bb_reg(dm, R_0xb78, MASKDWORD, 0x4c104c10); /*1.5dB*/
	/*only need 0xe50, 0xe24 if TXIQC in 0xc88*/
	//odm_set_bb_reg(dm, R_0xe50, 0x3FF00000, 0x100);
	//odm_set_bb_reg(dm, R_0xe24, 0x000003FF, 0x0);
}

void phy_path_a_dpk_init_8192f(
	struct dm_struct *dm)
{
	odm_set_bb_reg(dm, R_0xb00, MASKDWORD, 0x0005e018);
	odm_set_bb_reg(dm, R_0xb04, MASKDWORD, 0xf76d9f84);
	odm_set_bb_reg(dm, R_0xb28, MASKDWORD, 0x000844aa);
	odm_set_bb_reg(dm, R_0xb68, MASKDWORD, 0x31160200);

	odm_set_bb_reg(dm, R_0xb30, 0x000fffff, 0x0007bdef);
	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x40000000);

	odm_set_bb_reg(dm, R_0xbc0, MASKDWORD, 0x0000a9bf);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xb08, MASKDWORD, 0x41382e21);
	odm_set_bb_reg(dm, R_0xb0c, MASKDWORD, 0x5b554f48);
	odm_set_bb_reg(dm, R_0xb10, MASKDWORD, 0x6f6b6661);
	odm_set_bb_reg(dm, R_0xb14, MASKDWORD, 0x817d7874);
	odm_set_bb_reg(dm, R_0xb18, MASKDWORD, 0x908c8884);
	odm_set_bb_reg(dm, R_0xb1c, MASKDWORD, 0x9d9a9793);
	odm_set_bb_reg(dm, R_0xb20, MASKDWORD, 0xaaa7a4a1);
	odm_set_bb_reg(dm, R_0xb24, MASKDWORD, 0xb6b3b0ad);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x40000000);

	odm_set_bb_reg(dm, R_0xb00, MASKDWORD, 0x02ce03e8);
	odm_set_bb_reg(dm, R_0xb04, MASKDWORD, 0x01fd024c);
	odm_set_bb_reg(dm, R_0xb08, MASKDWORD, 0x01a101c9);
	odm_set_bb_reg(dm, R_0xb0c, MASKDWORD, 0x016a0183);
	odm_set_bb_reg(dm, R_0xb10, MASKDWORD, 0x01430153);
	odm_set_bb_reg(dm, R_0xb14, MASKDWORD, 0x01280134);
	odm_set_bb_reg(dm, R_0xb18, MASKDWORD, 0x0112011c);
	odm_set_bb_reg(dm, R_0xb1c, MASKDWORD, 0x01000107);
	odm_set_bb_reg(dm, R_0xb20, MASKDWORD, 0x00f200f9);
	odm_set_bb_reg(dm, R_0xb24, MASKDWORD, 0x00e500eb);
	odm_set_bb_reg(dm, R_0xb28, MASKDWORD, 0x00da00e0);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00d200d6);
	odm_set_bb_reg(dm, R_0xb30, MASKDWORD, 0x00c900cd);
	odm_set_bb_reg(dm, R_0xb34, MASKDWORD, 0x00c200c5);
	odm_set_bb_reg(dm, R_0xb38, MASKDWORD, 0x00bb00be);
	odm_set_bb_reg(dm, R_0xb3c, MASKDWORD, 0x00b500b8);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10000304);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10010203);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10020102);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10030101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10040101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10050101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10060101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x10070101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x1008caff);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100980a1);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100a5165);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100b3340);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100c2028);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100d1419);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100e0810);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x100f0506);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x0000000d);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01007800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01017800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01027800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01037800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01047800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01057800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01067800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01077800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01087800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01097800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x010F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01107800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01117800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01127800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01137800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01147800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01157800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01167800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01177800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01187800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01197800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x011F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01207800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01217800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01227800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01237800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01247800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01257800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01267800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01277800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01287800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01297800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x012F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01307800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01317800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01327800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01337800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01347800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01357800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01367800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01377800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01387800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x01397800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x013F7800);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x0000000d);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02007800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02017800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02027800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02037800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02047800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02057800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02067800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02077800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02087800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02097800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x020F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02107800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02117800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02127800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02137800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02147800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02157800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02167800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02177800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02187800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02197800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x021F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02207800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02217800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02227800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02237800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02247800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02257800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02267800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02277800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02287800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02297800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x022F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02307800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02317800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02327800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02337800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02347800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02357800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02367800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02377800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02387800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x02397800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x023F7800);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
}

void phy_path_b_dpk_init_8192f(
	struct dm_struct *dm)
{
	odm_set_bb_reg(dm, R_0xb70, MASKDWORD, 0x0005e018);
	odm_set_bb_reg(dm, R_0xb74, MASKDWORD, 0xf76d9f84);
	odm_set_bb_reg(dm, R_0xb98, MASKDWORD, 0x000844aa);
	odm_set_bb_reg(dm, R_0xb6c, MASKDWORD, 0x31160200);

	odm_set_bb_reg(dm, R_0xba0, 0x000fffff, 0x0007bdef);
	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x40000000);

	odm_set_bb_reg(dm, R_0xbc4, MASKDWORD, 0x0000a9bf);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xb78, MASKDWORD, 0x41382e21);
	odm_set_bb_reg(dm, R_0xb7c, MASKDWORD, 0x5b554f48);
	odm_set_bb_reg(dm, R_0xb80, MASKDWORD, 0x6f6b6661);
	odm_set_bb_reg(dm, R_0xb84, MASKDWORD, 0x817d7874);
	odm_set_bb_reg(dm, R_0xb88, MASKDWORD, 0x908c8884);
	odm_set_bb_reg(dm, R_0xb8c, MASKDWORD, 0x9d9a9793);
	odm_set_bb_reg(dm, R_0xb90, MASKDWORD, 0xaaa7a4a1);
	odm_set_bb_reg(dm, R_0xb94, MASKDWORD, 0xb6b3b0ad);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x40000000);

	odm_set_bb_reg(dm, R_0xb60, MASKDWORD, 0x02ce03e8);
	odm_set_bb_reg(dm, R_0xb64, MASKDWORD, 0x01fd024c);
	odm_set_bb_reg(dm, R_0xb68, MASKDWORD, 0x01a101c9);
	odm_set_bb_reg(dm, R_0xb6c, MASKDWORD, 0x016a0183);
	odm_set_bb_reg(dm, R_0xb70, MASKDWORD, 0x01430153);
	odm_set_bb_reg(dm, R_0xb74, MASKDWORD, 0x01280134);
	odm_set_bb_reg(dm, R_0xb78, MASKDWORD, 0x0112011c);
	odm_set_bb_reg(dm, R_0xb7c, MASKDWORD, 0x01000107);
	odm_set_bb_reg(dm, R_0xb80, MASKDWORD, 0x00f200f9);
	odm_set_bb_reg(dm, R_0xb84, MASKDWORD, 0x00e500eb);
	odm_set_bb_reg(dm, R_0xb88, MASKDWORD, 0x00da00e0);
	odm_set_bb_reg(dm, R_0xb8c, MASKDWORD, 0x00d200d6);
	odm_set_bb_reg(dm, R_0xb90, MASKDWORD, 0x00c900cd);
	odm_set_bb_reg(dm, R_0xb94, MASKDWORD, 0x00c200c5);
	odm_set_bb_reg(dm, R_0xb98, MASKDWORD, 0x00bb00be);
	odm_set_bb_reg(dm, R_0xb9c, MASKDWORD, 0x00b500b8);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20000304);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20010203);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20020102);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20030101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20040101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20050101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20060101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x20070101);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x2008caff);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200980a1);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200a5165);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200b3340);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200c2028);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200d1419);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200e0810);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x200f0506);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x0000000d);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04007800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04017800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04027800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04037800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04047800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04057800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04067800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04077800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04087800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04097800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x040F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04107800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04117800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04127800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04137800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04147800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04157800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04167800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04177800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04187800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04197800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x041F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04207800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04217800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04227800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04237800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04247800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04257800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04267800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04277800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04287800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04297800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x042F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04307800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04317800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04327800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04337800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04347800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04357800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04367800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04377800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04387800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x04397800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x043F7800);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x0000000d);

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08007800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08017800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08027800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08037800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08047800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08057800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08067800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08077800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08087800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08097800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x080F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08107800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08117800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08127800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08137800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08147800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08157800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08167800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08177800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08187800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08197800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x081F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08207800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08217800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08227800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08237800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08247800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08257800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08267800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08277800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08287800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08297800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x082F7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08307800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08317800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08327800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08337800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08347800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08357800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08367800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08377800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08387800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x08397800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083A7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083B7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083C7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083D7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083E7800);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x083F7800);

	odm_set_bb_reg(dm, R_0xba8, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
}

void _dpk_mac_bb_setting_8192f(
	struct dm_struct *dm)
{
	odm_set_bb_reg(dm, R_0x520, MASKBYTE2, 0xff); /*Tx Pause*/
	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xccf008c0);
	odm_set_bb_reg(dm, R_0xc5c, MASKDWORD, 0x80708492);
	odm_set_bb_reg(dm, R_0xd94, MASKDWORD, 0x44FFBB44);
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x00400040);
	odm_set_bb_reg(dm, R_0x87c, MASKDWORD, 0x004f0201);
	odm_set_bb_reg(dm, R_0x884, MASKDWORD, 0xc0000120);
	odm_set_bb_reg(dm, R_0x880, MASKDWORD, 0xd8001402);
	odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x6f005433);
	odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000804e4);
	odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);
	odm_set_bb_reg(dm, R_0xc14, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xc1c, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0x800, BIT(24), 0x0); /*disable CCK block*/

	/*Set Tx Rate Path A*/
	odm_set_bb_reg(dm, R_0xe08, 0x00007F00, 0x30);
	odm_set_bb_reg(dm, R_0x86c, 0xFFFFFF00, 0x303030);
	odm_set_bb_reg(dm, R_0xe00, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0xe04, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0xe10, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0xe14, MASKDWORD, 0x30303030);

	/*Set Tx Rate Path B*/
	odm_set_bb_reg(dm, R_0x838, 0xFFFFFF00, 0x303030);
	odm_set_bb_reg(dm, R_0x86c, 0x000000FF, 0x30);
	odm_set_bb_reg(dm, R_0x830, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0x834, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0x83c, MASKDWORD, 0x30303030);
	odm_set_bb_reg(dm, R_0x848, MASKDWORD, 0x30303030);

	/*TRSW PA on, LNA off*/
	odm_set_bb_reg(dm, R_0x930, 0x0000000F, 0x7);
	odm_set_bb_reg(dm, R_0x930, 0x0000FF00, 0x77);
	odm_set_bb_reg(dm, R_0x938, 0x000000F0, 0x7);
	odm_set_bb_reg(dm, R_0x938, 0x000F0000, 0x7);
	odm_set_bb_reg(dm, R_0x93c, 0x0000F000, 0x7);
	odm_set_bb_reg(dm, R_0x92c, BIT(0), 0x1);
	odm_set_bb_reg(dm, R_0x92c, BIT(3), 0x1);
	odm_set_bb_reg(dm, R_0x92c, BIT(17), 0x1);
	odm_set_bb_reg(dm, R_0x92c, BIT(27), 0x1);

	/*set CCA TH to highest*/
	odm_set_bb_reg(dm, R_0xc3c, 0x000007F8, 0xff);
	odm_set_bb_reg(dm, R_0xc30, 0x0000000f, 0xf);
	odm_set_bb_reg(dm, R_0xc84, 0xf0000000, 0xf);
}

void _dpk_one_shot_8192f(
	struct dm_struct *dm,
	u8 path,
	u8 action)
{
	u16 temp_reg = 0x0;
	u8 dpk_cmd = 0x0;

	temp_reg = 0xb2b + path * 0x70;
	dpk_cmd = (action - 1) * 0x40;

	odm_write_1byte(dm, temp_reg, dpk_cmd + 0x80);
	odm_write_1byte(dm, temp_reg, dpk_cmd);
#if 0
	RF_DBG(dm, DBG_RF_DPK, "[DPK][one-shot] reg=0x%x, dpk_cmd=0x%x\n",
	       temp_reg, dpk_cmd);
#endif
	ODM_delay_ms(2);
}

u32 _dpk_pas_get_iq_8192f(
	struct dm_struct *dm,
	u8 path,
	boolean is_gain_chk)
{
	s32 i_val = 0, q_val = 0;

	switch (path) {

	case RF_PATH_A:
		odm_set_bb_reg(dm, R_0xb68, BIT(26), 0x0);

		if (is_gain_chk) {
			odm_set_bb_reg(dm, R_0xb00, MASKDWORD, 0x0101f03f);
			i_val = odm_get_bb_reg(dm, R_0xbe8, MASKHWORD);
			q_val = odm_get_bb_reg(dm, R_0xbe8, MASKLWORD);	
		} else {
			odm_set_bb_reg(dm, R_0xb00, MASKDWORD, 0x0101f038);
			i_val = odm_get_bb_reg(dm, R_0xbdc, MASKHWORD);
			q_val = odm_get_bb_reg(dm, R_0xbdc, MASKLWORD);	
		}
		odm_set_bb_reg(dm, R_0xb68, BIT(26), 0x1);
		break;

	case RF_PATH_B:
		odm_set_bb_reg(dm, R_0xb6c, BIT(26), 0x0);

		if (is_gain_chk) {
			odm_set_bb_reg(dm, R_0xb70, MASKDWORD, 0x0101f03f);
			i_val = odm_get_bb_reg(dm, R_0xbf8, MASKHWORD);
			q_val = odm_get_bb_reg(dm, R_0xbf8, MASKLWORD);	
		} else {
			odm_set_bb_reg(dm, R_0xb70, MASKDWORD, 0x0101f038);
			i_val = odm_get_bb_reg(dm, R_0xbec, MASKHWORD);
			q_val = odm_get_bb_reg(dm, R_0xbec, MASKLWORD);	
		}
		odm_set_bb_reg(dm, R_0xb6c, BIT(26), 0x1);
		break;

	default:
		break;
	}

	if (i_val >> 15 != 0)
		i_val = 0x10000 - i_val;
	if (q_val >> 15 != 0)
		q_val = 0x10000 - q_val;

#if (DPK_GAINLOSS_DBG_8192F)
	RF_DBG(dm, DBG_RF_DPK, "[DPK][%s] i=0x%x, q=0x%x, i^2+q^2=0x%x\n",
	       is_gain_chk ? "Gain" : "Loss",
	       i_val, q_val, i_val*i_val + q_val*q_val);
#endif
	return i_val*i_val + q_val*q_val;
}

u8 _dpk_pas_iq_check_8192f(
	struct dm_struct *dm,
	u8 path,
	u8 limited_pga)
{
	u8 result = 0;
	u32 i_val = 0, q_val = 0, loss = 0, gain = 0;
	s32 loss_db = 0, gain_db = 0;
	
	loss = _dpk_pas_get_iq_8192f(dm, path, LOSS_CHK);
	gain = _dpk_pas_get_iq_8192f(dm, path, GAIN_CHK);
	loss_db = 3 * _dpk_log2base(loss);
	gain_db = 3 * _dpk_log2base(gain);

#if (DPK_GAINLOSS_DBG_8192F)
	RF_DBG(dm, DBG_RF_DPK, "[DPK][GL_Chk] G=%d.%d, L=%d.%d, GL=%d.%02ddB\n",
		(gain_db - 6020) / 100, HALRF_ABS(6020, gain_db) % 100,
		(loss_db - 6020) / 100, HALRF_ABS(6020, loss_db) % 100,
		(gain_db - loss_db) / 100, (gain_db - loss_db) % 100);
#endif

	if ((gain > 0x100000) && !limited_pga) { /*Gain > 0dB happen*/
		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL_Chk] Gain > 0dB happen!!\n");
		result = 1;
		return result;
	} else if ((gain < 0x33142) && !limited_pga) { /*Gain < -7dB happen*/
		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL_Chk] Gain < -7dB happen!!\n");
		result = 2;
		return result;
	} else if ((gain_db - loss_db) > 500) { /*GL > 5dB*/
		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL_Chk] GL > 4dB happen!!\n");
		result = 3;
		return result;
	} else if ((gain_db - loss_db) < 250) { /*GL < 2.5dB*/
		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL_Chk] GL < 2.5dB happen!!\n");
		result = 4;
		return result;
	} else
		return result;

}

void phy_path_a_pas_read_8192f(
	struct dm_struct *dm,
	boolean is_gainloss)
{
	int k;
	u32 reg_b00, reg_b68, reg_bdc, reg_be0, reg_be4, reg_be8;

	reg_b00 = odm_get_bb_reg(dm, R_0xb00, MASKDWORD);
	reg_b68 = odm_get_bb_reg(dm, R_0xb68, MASKDWORD);

	if (is_gainloss) {
		odm_set_bb_reg(dm, R_0xb68, BIT(26), 0x0);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] 0xb68 = 0x%x\n",
		       odm_get_bb_reg(dm, R_0xb68, MASKDWORD));
	}

	for (k = 0; k < 8; k++) {
		odm_set_bb_reg(dm, R_0xb00, MASKDWORD, (0x0101f038 | k));
		/*RF_DBG(dm, DBG_RF_DPK, ("[DPK] 0xb00[%d] = 0x%x\n", k, odm_get_bb_reg(dm, R_0xb00, MASKDWORD)));*/
		reg_bdc = odm_get_bb_reg(dm, R_0xbdc, MASKDWORD);
		reg_be0 = odm_get_bb_reg(dm, R_0xbe0, MASKDWORD);
		reg_be4 = odm_get_bb_reg(dm, R_0xbe4, MASKDWORD);
		reg_be8 = odm_get_bb_reg(dm, R_0xbe8, MASKDWORD);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S0[%d] = 0x%x\n", k,
		       reg_bdc);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S0[%d] = 0x%x\n", k,
		       reg_be0);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S0[%d] = 0x%x\n", k,
		       reg_be4);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S0[%d] = 0x%x\n", k,
		       reg_be8);
	}

	odm_set_bb_reg(dm, R_0xb00, MASKDWORD, reg_b00);
	odm_set_bb_reg(dm, R_0xb68, MASKDWORD, reg_b68);
}

void phy_path_b_pas_read_8192f(
	struct dm_struct *dm,
	boolean is_gainloss)
{
	int k;
	u32 reg_b70, reg_b6c, reg_bec, reg_bf0, reg_bf4, reg_bf8;

	reg_b70 = odm_get_bb_reg(dm, R_0xb70, MASKDWORD);
	reg_b6c = odm_get_bb_reg(dm, R_0xb6c, MASKDWORD);

	if (is_gainloss) {
		odm_set_bb_reg(dm, R_0xb6c, BIT(26), 0x0);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] 0xb6c = 0x%x\n",
		       odm_get_bb_reg(dm, R_0xb6c, MASKDWORD));
	}

	for (k = 0; k < 8; k++) {
		odm_set_bb_reg(dm, R_0xb70, MASKDWORD, (0x0101f038 | k));
		//RF_DBG(dm, DBG_RF_DPK, ("[DPK] 0xb70[%d] = 0x%x\n", k, odm_get_bb_reg(dm, R_0xb70, MASKDWORD)));
		reg_bec = odm_get_bb_reg(dm, R_0xbec, MASKDWORD);
		reg_bf0 = odm_get_bb_reg(dm, R_0xbf0, MASKDWORD);
		reg_bf4 = odm_get_bb_reg(dm, R_0xbf4, MASKDWORD);
		reg_bf8 = odm_get_bb_reg(dm, R_0xbf8, MASKDWORD);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S1[%d] = 0x%x\n", k,
		       reg_bec);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S1[%d] = 0x%x\n", k,
		       reg_bf0);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S1[%d] = 0x%x\n", k,
		       reg_bf4);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] PA scan S1[%d] = 0x%x\n", k,
		       reg_bf8);
	}

	odm_set_bb_reg(dm, R_0xb70, MASKDWORD, reg_b70);
	odm_set_bb_reg(dm, R_0xb6c, MASKDWORD, reg_b6c);
}

void _dpk_pas_read_8192f(
	struct dm_struct *dm,
	u8 path,
	boolean is_gainloss)
{
	if (path == RF_PATH_A)
		phy_path_a_pas_read_8192f(dm, is_gainloss);
	else if (path == RF_PATH_B)
		phy_path_b_pas_read_8192f(dm, is_gainloss);
}

void _dpk_pas_agc_8192f(
	struct dm_struct *dm,
	u8 path)
{
	u8 tmp_txagc, tmp_pga, i = 0;
	u8 goout = 0, limited_pga = 0;

	do {
		switch (i) {
		case 0: /*one-shot*/
			tmp_txagc = odm_get_rf_reg(dm, (enum rf_path)path,
						   RF_0x00, 0x0001f);
			tmp_pga = odm_get_rf_reg(dm, (enum rf_path)path,
						 RF_0x8f, 0x00006000);

			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK][AGC] TXAGC=0x%x, PGA=0x%x\n",
			       tmp_txagc, tmp_pga);

			if (!limited_pga)
				_dpk_one_shot_8192f(dm, path, GAIN_LOSS);

			if (DPK_PAS_DBG_8192F)
				_dpk_pas_read_8192f(dm, path, TRUE);

			i = _dpk_pas_iq_check_8192f(dm, path, limited_pga);
			if (i == 0)
				goout = 1;
			break;
	
		case 1: /*Gain > criterion*/
			if (tmp_pga == 0x3) {
				odm_set_rf_reg(dm, (enum rf_path)path,
					       RF_0x8f, 0x00006000, 0x1);
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA(-1) = 1\n");
			} else if (tmp_pga == 0x1) {
				odm_set_rf_reg(dm, (enum rf_path)path,
			 		       RF_0x8f, 0x00006000, 0x0);
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA(-1) = 0\n");
			} else if ((tmp_pga == 0x0) || (tmp_pga == 0x2)) {
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA@ lower bound!!\n");
				limited_pga = 1;
			}
			i = 0;
			break;

		case 2: /*Gain < criterion*/
			if ((tmp_pga == 0x0) || (tmp_pga == 0x2)) {
				odm_set_rf_reg(dm, (enum rf_path)path,
					       RF_0x8f, 0x00006000, 0x1);
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA(+1) = 1\n");
			} else if (tmp_pga == 0x1) {
				odm_set_rf_reg(dm, (enum rf_path)path,
					       RF_0x8f, 0x00006000, 0x3);
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA(+1) = 3\n");
			} else if (tmp_pga == 0x3) {
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] PGA@ upper bound!!\n");
				limited_pga = 1;
			}
			i = 0;
			break;

		case 3: /*GL > criterion*/
			if (tmp_txagc == 0x0) {
				goout = 1;
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] TXAGC@ lower bound!!\n");
				break;
			}
			tmp_txagc--;
			odm_set_rf_reg(dm, (enum rf_path)path,
				       RF_0x00, 0x0001f, tmp_txagc);
			RF_DBG(dm, DBG_RF_DPK, "[DPK][AGC] txagc(-1) = 0x%x\n",
			       tmp_txagc);
			limited_pga = 0;
			i = 0;
			ODM_delay_ms(1);
			break;

		case 4:	/*GL < criterion*/
			if (tmp_txagc == 0x1f) {
				goout = 1;
				RF_DBG(dm, DBG_RF_DPK,
				       "[DPK][AGC] TXAGC@ upper bound!!\n");
				break;
			}
			tmp_txagc++;
			odm_set_rf_reg(dm, (enum rf_path)path,
				       RF_0x00, 0x0001f, tmp_txagc);
			RF_DBG(dm, DBG_RF_DPK, "[DPK][AGC] txagc(+1) = 0x%x\n",
			       tmp_txagc);
			limited_pga = 0;
			i = 0;
			ODM_delay_ms(1);
			break;

		default:
			goout = 1;
			break;
		}
	} while (!goout);

}

u8 phy_path_a_gainloss_8192f(
	struct dm_struct *dm)
{
	int k;
	u8 tx_agc_search = 0x0, result[5] = {0x0};

	if (dm->cut_version == ODM_CUT_A) {
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK, 0xae000);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK, 0x5801c);
	} else {
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK, 0xaa000);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK, 0x58019);
	}

	/*RF_DBG(dm, DBG_RF_DPK, "[DPK] TXAGC for gainloss_a = 0x%x\n",
			odm_get_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK));*/

	odm_write_1byte(dm, R_0xb00, 0x38);
	odm_write_1byte(dm, R_0xb07, 0xf7);
	odm_write_1byte(dm, R_0xb2b, 0x00);
	odm_set_bb_reg(dm, R_0xb68, 0xffff0000, 0x159f);
	odm_set_bb_reg(dm, R_0xb08, MASKDWORD, 0x41382e21);
	odm_write_1byte(dm, R_0xbad, 0x2c);
	odm_set_bb_reg(dm, R_0xe34, MASKDWORD, 0x10000000);
	/*only need 0xe30, 0xe20 if TXIQC in 0xc80*/
	/*odm_set_bb_reg(dm, R_0xe30, 0x3FF00000, 0x100);*/
	/*odm_set_bb_reg(dm, R_0xe20, 0x000003FF, 0x0);*/

	ODM_delay_ms(1);
#if 1
	_dpk_pas_agc_8192f(dm, RF_PATH_A);
#else
	_dpk_one_shot_8192f(dm, RF_PATH_A, GAIN_LOSS);
#endif
	result[0] = (u8)odm_get_bb_reg(dm, R_0xbdc, 0x0000000f);
	RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] result[0] = 0x%x\n", result[0]);

	for (k = 1; k < 5; k++) {
#if 0
		odm_write_1byte(dm, R_0xb2b, 0x80);
		odm_write_1byte(dm, R_0xb2b, 0x00);
		ODM_delay_ms(2);
#endif
		_dpk_one_shot_8192f(dm, RF_PATH_A, GAIN_LOSS);

		result[k] = (u8)odm_get_bb_reg(dm, R_0xbdc, 0x0000000f);

		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] result[%d] = 0x%x\n", k,
		       result[k]);

		if (result[k] == result[k - 1])
			break;
	}

	if (k == 4)
		tx_agc_search = ((result[0] + result[1] + result[2] + result[3] + result[4]) / 5);
	else
		tx_agc_search = (u8)odm_get_bb_reg(dm, R_0xbdc, 0x0000000f);

#if 1
	RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] S0 RF_0x0=0x%x, 0x8f=0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK),
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK));
#endif

	return tx_agc_search;
}

u8 phy_path_b_gainloss_8192f(
	struct dm_struct *dm)
{
	int k;
	u8 tx_agc_search = 0x0, result[5] = {0x0};

	if (dm->cut_version == ODM_CUT_A) {
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK, 0xae000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK, 0x5801c);
	} else {
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK, 0xaa000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK, 0x58019);
	}

	/*RF_DBG(dm, DBG_RF_DPK, "[DPK] TXAGC for gainloss_b = 0x%x\n",
			odm_get_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK));*/

	odm_write_1byte(dm, R_0xb70, 0x38);
	odm_write_1byte(dm, R_0xb77, 0xf7);
	odm_write_1byte(dm, R_0xb9b, 0x00);
	odm_set_bb_reg(dm, R_0xb6c, 0xffff0000, 0x159f);
	odm_set_bb_reg(dm, R_0xb78, MASKDWORD, 0x41382e21);
	odm_write_1byte(dm, R_0xbad, 0x2c);
	odm_set_bb_reg(dm, R_0xe54, MASKDWORD, 0x10000000);
	/*only need 0xe50, 0xe24 if TXIQC in 0xc88*/
	/*odm_set_bb_reg(dm, R_0xe50, 0x3FF00000, 0x100);*/
	/*odm_set_bb_reg(dm, R_0xe24, 0x000003FF, 0x0);*/

	ODM_delay_ms(1);
#if 1
	_dpk_pas_agc_8192f(dm, RF_PATH_B);
#else
	_dpk_one_shot_8192f(dm, RF_PATH_B, GAIN_LOSS);
#endif
	result[0] = (u8)odm_get_bb_reg(dm, R_0xbec, 0x0000000f);
	RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] result[0] = 0x%x\n", result[0]);

	for (k = 1; k < 5; k++) {
#if 0
		odm_write_1byte(dm, R_0xb9b, 0x80);
		odm_write_1byte(dm, R_0xb9b, 0x00);
		ODM_delay_ms(2);
#endif
		_dpk_one_shot_8192f(dm, RF_PATH_B, GAIN_LOSS);

		result[k] = (u8)odm_get_bb_reg(dm, R_0xbec, 0x0000000f);

		RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] result[%d] = 0x%x\n", k,
		       result[k]);

		if (result[k] == result[k - 1])
			break;
	}

	if (k == 4)
		tx_agc_search = ((result[0] + result[1] + result[2] + result[3] + result[4]) / 5);
	else
		tx_agc_search = (u8)odm_get_bb_reg(dm, R_0xbec, 0x0000000f);

#if 1
	RF_DBG(dm, DBG_RF_DPK, "[DPK][GL] S1 RF_0x0=0x%x, 0x8f=0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK),
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK));
#endif

	return tx_agc_search;
}

u32 phy_path_a_dodpk_8192f(
	struct dm_struct *dm,
	u8 tx_agc_search,
	u8 k)
{
	u32 reg_b68, result;
	u8 tx_agc = 0x0;
	s8 pwsf_idx = 0;

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0xfcff0c00);
	odm_write_1byte(dm, R_0xb2b, 0x40);

	tx_agc = odm_get_rf_reg(dm, RF_PATH_A, RF_0x00, 0x0001f);

	tx_agc = tx_agc - (0xa - tx_agc_search);

	pwsf_idx = tx_agc - 0x19;

	if (dm->cut_version == ODM_CUT_A)
		reg_b68 = 0x11160200 | ((pwsf_idx & 0x1f) << 10);
	else
		reg_b68 = 0x11160200 | (((pwsf_idx + 1) & 0x1f) << 10);

	RF_DBG(dm, DBG_RF_DPK, "[DPK] pwsf_idx = 0x%x, tx_agc = 0x%x\n",
			pwsf_idx & 0x1f, tx_agc);

	odm_set_bb_reg(dm, R_0xb68, MASKDWORD, reg_b68);
#if 0
	if (dm->cut_version == ODM_CUT_A)
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK, 0xae000);
	else
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK, 0xa8000);
#endif
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK, (0x58000 | tx_agc));

	RF_DBG(dm, DBG_RF_DPK, "[DPK] S0 0x0 = 0x%x, 0x8f = 0x%x, 0xb68[%d] = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x00, RFREG_MASK),
	       odm_get_rf_reg(dm, RF_PATH_A, RF_0x8f, RFREG_MASK),
	       k, odm_get_bb_reg(dm, R_0xb68, MASKDWORD));

	ODM_delay_ms(1);
#if 0
	odm_write_1byte(dm, R_0xb2b, 0xc0);
	odm_write_1byte(dm, R_0xb2b, 0x40);
	ODM_delay_ms(2);
#endif
	_dpk_one_shot_8192f(dm, RF_PATH_A, DO_DPK);

	result = odm_get_bb_reg(dm, R_0xbd8, BIT(18));
	RF_DBG(dm, DBG_RF_DPK, "[DPK] fail bit = %x\n", result);
	return result;
}

u32 phy_path_b_dodpk_8192f(
	struct dm_struct *dm,
	u8 tx_agc_search,
	u8 k)
{
	u32 reg_b6c, result;
	u8 tx_agc = 0x0;
	s8 pwsf_idx = 0;

	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0xfcff0c00);
	odm_write_1byte(dm, R_0xb9b, 0x40);

	tx_agc = odm_get_rf_reg(dm, RF_PATH_B, RF_0x00, 0x0001f);

	tx_agc = tx_agc - (0xa - tx_agc_search);

	pwsf_idx = tx_agc - 0x19;
	
	if (dm->cut_version == ODM_CUT_A)
		reg_b6c = 0x11160200 | ((pwsf_idx & 0x1f) << 10);
	else
		reg_b6c = 0x11160200 | (((pwsf_idx + 1) & 0x1f) << 10);

	RF_DBG(dm, DBG_RF_DPK, "[DPK] pwsf_idx = 0x%x, tx_agc = 0x%x\n",
			pwsf_idx & 0x1f, tx_agc);

	odm_set_bb_reg(dm, R_0xb6c, MASKDWORD, reg_b6c);
#if 0
	if (dm->cut_version == ODM_CUT_A)
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK, 0xae000);
	else
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK, 0xa8000);
#endif
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK, (0x58000 | tx_agc));

	RF_DBG(dm, DBG_RF_DPK, "[DPK] S1 0x0 = 0x%x, 0x8f = 0x%x, 0xb6c[%d] = 0x%x\n",
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x00, RFREG_MASK),
	       odm_get_rf_reg(dm, RF_PATH_B, RF_0x8f, RFREG_MASK),
	       k, odm_get_bb_reg(dm, R_0xb6c, MASKDWORD));

	ODM_delay_ms(1);
#if 0
	odm_write_1byte(dm, R_0xb9b, 0xc0);
	odm_write_1byte(dm, R_0xb9b, 0x40);
	ODM_delay_ms(2);
#endif
	_dpk_one_shot_8192f(dm, RF_PATH_B, DO_DPK);

	result = odm_get_bb_reg(dm, R_0xbd8, BIT(19));
	RF_DBG(dm, DBG_RF_DPK, "[DPK] fail bit = %x\n", result);
	return result;
}

boolean
phy_path_a_iq_check_8192f(
	struct dm_struct *dm,
	u8 group,
	u8 addr,
	BOOLEAN	is_even)
{
	u32 i_val, q_val;

	if (is_even) {
		i_val = odm_get_bb_reg(dm, R_0xbc0, 0x003FF800);
		q_val = odm_get_bb_reg(dm, R_0xbc0, 0x000007FF);

	} else {
		i_val = odm_get_bb_reg(dm, R_0xbc4, 0x003FF800);
		q_val = odm_get_bb_reg(dm, R_0xbc4, 0x000007FF);
	}

	if (((q_val & 0x400)>>10) == 1)
		q_val = 0x800 - q_val;

	if ((addr == 0) && ((i_val*i_val + q_val*q_val) < 0x2851e)) {
		/* LMS (I^2 + Q^2) < -2dB happen*/
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] LUT < -2dB happen, I=0x%x, Q=0x%x\n",
		       i_val, q_val);
		return 1;
	} else if ((i_val*i_val + q_val*q_val) > 0x50924) {
		/* LMS (I^2 + Q^2) > 1dB happen*/
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] LUT > 1dB happen, I=0x%x, Q=0x%x\n",
		       i_val, q_val);
		return 1;
	} else {	
		return 0;
	}
}

boolean
phy_path_b_iq_check_8192f(
	struct dm_struct *dm,
	u8 group,
	u8 addr,
	BOOLEAN	is_even)
{
	u32 i_val, q_val;

	if (is_even) {
		i_val = odm_get_bb_reg(dm, R_0xbc8, 0x003FF800);
		q_val = odm_get_bb_reg(dm, R_0xbc8, 0x000007FF);

	} else {
		i_val = odm_get_bb_reg(dm, R_0xbcc, 0x003FF800);
		q_val = odm_get_bb_reg(dm, R_0xbcc, 0x000007FF);
	}

	if (((q_val & 0x400)>>10) == 1)
		q_val = 0x800 - q_val;

	if ((addr == 0) && ((i_val*i_val + q_val*q_val) < 0x2851e)) {
		/* LMS (I^2 + Q^2) < -2dB happen*/
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] LUT < -2dB happen, I=0x%x, Q=0x%x\n",
		       i_val, q_val);
		return 1;
	} else if ((i_val*i_val + q_val*q_val) > 0x50924) {
		/* LMS (I^2 + Q^2) > 1dB happen*/
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] LUT > 1dB happen, I=0x%x, Q=0x%x\n",
		       i_val, q_val);
		return 1;
	} else {	
		return 0;
	}

}

void phy_path_a_dpk_enable_8192f(
	struct dm_struct *dm)
{
	odm_write_1byte(dm, R_0xb00, 0x98);
	odm_write_1byte(dm, R_0xb2b, 0x00);
	odm_write_1byte(dm, R_0xb6a, 0x26);

	/*odm_write_1byte(dm, R_0xb2b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb2b, 0x00);*/
	/*ODM_delay_ms(10);*/

	odm_write_1byte(dm, R_0xb07, 0x77);
	odm_set_bb_reg(dm, R_0xb08, MASKDWORD, 0x4c104c10); /*1.5dB*/

	/*odm_set_bb_reg(dm, R_0xe30, MASKDWORD, 0x10000000);*/
	/*odm_set_bb_reg(dm, R_0xe20, MASKDWORD, 0x00000000);*/
}

void phy_path_b_dpk_enable_8192f(
	struct dm_struct *dm)
{
	odm_write_1byte(dm, R_0xb70, 0x98);
	odm_write_1byte(dm, R_0xb9b, 0x00);
	odm_write_1byte(dm, R_0xb6e, 0x26);

	/*odm_write_1byte(dm, R_0xb9b, 0x80);*/
	/*odm_write_1byte(dm, R_0xb9b, 0x00);*/
	/*ODM_delay_ms(10);*/

	odm_write_1byte(dm, R_0xb77, 0x77);
	odm_set_bb_reg(dm, R_0xb78, MASKDWORD, 0x4c104c10); /*1.5dB*/

	/*odm_set_bb_reg(dm, R_0xe30, MASKDWORD, 0x10000000);*/
	/*odm_set_bb_reg(dm, R_0xe20, MASKDWORD, 0x00000000);*/
}

u8 phy_dpk_channel_transfer_8192f(
	struct dm_struct *dm)
{
	u8 channel, bandwidth, i;

	channel = *dm->channel;
	bandwidth = *dm->band_width;

	if (channel <= 4)
		i = 0;
	else if (channel >= 5 && channel <= 8)
		i = 1;
	else if (channel >= 9)
		i = 2;

	RF_DBG(dm, DBG_RF_DPK,
	       "[DPK] channel = %d, bandwidth = %d, transfer idx = %d\n",
	       channel, bandwidth, i);

	return i;
}

u8 phy_lut_sram_read_8192f(
	struct dm_struct *dm,
	u8 k)
{
#if 0
	u8 addr, i;
	u32 regb2c;

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	for (addr = 0; addr < 64; addr++) {
		regb2c = (0x00801234 | (addr << 16));
		odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, regb2c);
		/*RF_DBG(dm, DBG_RF_DPK, ("[DPK] LUT_SRAM_read 0xb2c = 0x%x\n", regb2c));*/

		/*A even*/
		dm->rf_calibrate_info.lut_2g_even_a[k][addr] = odm_get_bb_reg(dm, R_0xbc0, 0x003FFFFF);
		if (dm->rf_calibrate_info.lut_2g_even_a[k][addr] > 0x100000) {
			/*printk("[DPK] 8192F LUT value > 0x100000, please check path A[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT value > 0x100000, please check path A[%d]\n",
			       k);
			return 0;
		}
		if (phy_path_a_iq_check_8192f(dm, k, addr, true)) {
			/*printk("[DPK] 8192F LUT IQ value < -1.9dB, please check path A[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT IQ value < -1.9dB, please check path A[%d]\n",
			       k);
			return 0;
		}

		/*A odd*/
		dm->rf_calibrate_info.lut_2g_odd_a[k][addr] = odm_get_bb_reg(dm, R_0xbc4, 0x003FFFFF);
		if (dm->rf_calibrate_info.lut_2g_odd_a[k][addr] > 0x100000) {
			/*printk("[DPK] 8192F LUT value > 0x100000, please check path A[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT value > 0x100000, please check path A[%d]\n",
			       k);
			return 0;
		}

		/*B even*/
		dm->rf_calibrate_info.lut_2g_even_b[k][addr] = odm_get_bb_reg(dm, R_0xbc8, 0x003FFFFF);
		if (dm->rf_calibrate_info.lut_2g_even_b[k][addr] > 0x100000) {
			/*printk("[DPK] 8192F LUT value > 0x100000, please check path B[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT value > 0x100000, please check path B[%d]\n",
			       k);
			return 0;
		}

		if (phy_path_b_iq_check_8192f(dm, k, addr, true)) {
			/*printk("[DPK] 8192F LUT IQ value < -1.9dB, please check path B[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT IQ value < -1.9dB, please check path B[%d]\n",
			       k);
			return 0;
		}

		/*B odd*/
		dm->rf_calibrate_info.lut_2g_odd_b[k][addr] = odm_get_bb_reg(dm, R_0xbcc, 0x003FFFFF);
		if (dm->rf_calibrate_info.lut_2g_odd_b[k][addr] > 0x100000) {
			/*printk("[DPK] 8192F LUT value > 0x100000, please check path B[%d]\n", k);*/
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] 8192F LUT value > 0x100000, please check path B[%d]\n",
			       k);
			return 0;
		}
	}

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	return 1;
#endif
}

void phy_lut_sram_write_8192f(
	struct dm_struct *dm)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 addr, group;
	u32 regba8_a_even[64] = {0}, regb2c_a_even[64] = {0};
	u32 regba8_a_odd[64] = {0}, regb2c_a_odd[64] = {0};
	u32 regba8_b_even[64] = {0}, regb2c_b_even[64] = {0};
	u32 regba8_b_odd[64] = {0}, regb2c_b_odd[64] = {0};

	group = phy_dpk_channel_transfer_8192f(dm);

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	for (addr = 0; addr < 64; addr++) {
		/*A even*/
		odm_set_bb_reg(dm, R_0xb2c, 0x0F000000, 0x1);
		/*regba8_a_even[addr] = (rfcal_info->lut_2g_even_a[group][addr] & 0x003F0000) >> 16;*/
		/*regb2c_a_even[addr] = rfcal_info->lut_2g_even_a[group][addr] & 0x0000FFFF;*/
		regba8_a_even[addr] = (dpk_info->lut_2g_even[0][group][addr] & 0x003F0000) >> 16;
		regb2c_a_even[addr] = dpk_info->lut_2g_even[0][group][addr] & 0x0000FFFF;

		odm_set_bb_reg(dm, R_0xba8, 0x0000003F, regba8_a_even[addr]);
		odm_set_bb_reg(dm, R_0xb2c, 0x003FFFFF, (regb2c_a_even[addr] | (addr << 16)));

		if (DPK_SRAM_write_DBG_8192F && (addr < 16)) {
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S0[%d] even 0xba8[%2d] = 0x%x\n", 
			       group, addr,
			       odm_get_bb_reg(dm, R_0xba8, MASKDWORD));
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S0[%d] even 0xb2c[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xb2c, MASKDWORD));
		}
	}

	for (addr = 0; addr < 64; addr++) {
		/*A odd*/
		odm_set_bb_reg(dm, R_0xb2c, 0x0F000000, 0x2);
		/*regba8_a_odd[addr] = (rfcal_info->lut_2g_odd_a[group][addr] & 0x003F0000) >> 16;*/
		/*regb2c_a_odd[addr] = rfcal_info->lut_2g_odd_a[group][addr] & 0x0000FFFF;*/
		regba8_a_odd[addr] = (dpk_info->lut_2g_odd[0][group][addr] & 0x003F0000) >> 16;
		regb2c_a_odd[addr] = dpk_info->lut_2g_odd[0][group][addr] & 0x0000FFFF;

		odm_set_bb_reg(dm, R_0xba8, 0x0000003F, regba8_a_odd[addr]);
		odm_set_bb_reg(dm, R_0xb2c, 0x003FFFFF, (regb2c_a_odd[addr] | (addr << 16)));

		if (DPK_SRAM_write_DBG_8192F && (addr < 16)) {
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S0[%d]  odd 0xba8[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xba8, MASKDWORD));
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S0[%d]  odd 0xb2c[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xb2c, MASKDWORD));
		}
	}

	for (addr = 0; addr < 64; addr++) {
		/*B even*/
		odm_set_bb_reg(dm, R_0xb2c, 0x0F000000, 0x4);
		/*regba8_b_even[addr] = (rfcal_info->lut_2g_even_b[group][addr] & 0x003F0000) >> 16;*/
		/*regb2c_b_even[addr] = rfcal_info->lut_2g_even_b[group][addr] & 0x0000FFFF;*/
		regba8_b_even[addr] = (dpk_info->lut_2g_even[1][group][addr] & 0x003F0000) >> 16;
		regb2c_b_even[addr] = dpk_info->lut_2g_even[1][group][addr] & 0x0000FFFF;

		odm_set_bb_reg(dm, R_0xba8, 0x0000003F, regba8_b_even[addr]);
		odm_set_bb_reg(dm, R_0xb2c, 0x003FFFFF, (regb2c_b_even[addr] | (addr << 16)));

		if (DPK_SRAM_write_DBG_8192F && (addr < 16)) {
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S1[%d] even 0xba8[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xba8, MASKDWORD));
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S1[%d] even 0xb2c[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xb2c, MASKDWORD));
		}
	}

	for (addr = 0; addr < 64; addr++) {
		/*B odd*/
		odm_set_bb_reg(dm, R_0xb2c, 0x0F000000, 0x8);
		/*regba8_b_odd[addr] = (rfcal_info->lut_2g_odd_b[group][addr] & 0x003F0000) >> 16;*/
		/*regb2c_b_odd[addr] = rfcal_info->lut_2g_odd_b[group][addr] & 0x0000FFFF;*/
		regba8_b_odd[addr] = (dpk_info->lut_2g_odd[1][group][addr] & 0x003F0000) >> 16;
		regb2c_b_odd[addr] = dpk_info->lut_2g_odd[1][group][addr] & 0x0000FFFF;

		odm_set_bb_reg(dm, R_0xba8, 0x0000003F, regba8_b_odd[addr]);
		odm_set_bb_reg(dm, R_0xb2c, 0x003FFFFF, (regb2c_b_odd[addr] | (addr << 16)));

		if (DPK_SRAM_write_DBG_8192F && (addr < 16)) {
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S1[%d]  odd 0xba8[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xba8, MASKDWORD));
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Write S1[%d]  odd 0xb2c[%2d] = 0x%x\n",
			       group, addr,
			       odm_get_bb_reg(dm, R_0xb2c, MASKDWORD));
		}
	}

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	/*odm_set_bb_reg(dm, R_0xb68, 0x00007c00, rfcal_info->pwsf_2g_a[group]);*/
	/*odm_set_bb_reg(dm, R_0xb68, 0x00007c00, dpk_info->pwsf_2g[0][group]);*/
	odm_set_bb_reg(dm, R_0xb68, MASKDWORD,
		       (0x11260200 | (dpk_info->pwsf_2g[0][group] << 10)));	
	RF_DBG(dm, DBG_RF_DPK, "[DPK] Write S0[%d] pwsf = 0x%x\n", group,
	       odm_get_bb_reg(dm, R_0xb68, 0x00007c00));

	/*odm_set_bb_reg(dm, R_0xb6c, 0x00007c00, rfcal_info->pwsf_2g_b[group]);*/
	/*odm_set_bb_reg(dm, R_0xb6c, 0x00007c00, dpk_info->pwsf_2g[1][group]);*/
	odm_set_bb_reg(dm, R_0xb6c, MASKDWORD,
		       (0x11260200 | (dpk_info->pwsf_2g[1][group] << 10)));
	RF_DBG(dm, DBG_RF_DPK, "[DPK] Write S1[%d] pwsf = 0x%x\n", group,
	       odm_get_bb_reg(dm, R_0xb6c, 0x00007c00));
}

void phy_dpk_enable_disable_8192f(
	struct dm_struct *dm)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	if (dpk_info->is_dpk_enable) { /*use dpk result*/
#if 0
		odm_set_bb_reg(dm, R_0xb68, BIT(29), 0x0);
		odm_set_bb_reg(dm, R_0xb6c, BIT(29), 0x0);
		rfcal_info->is_dp_path_aok = 1;
		rfcal_info->is_dp_path_bok = 1;
#endif
		if ((dpk_info->dpk_path_ok & BIT(0))) {
			odm_set_bb_reg(dm, R_0xb68, BIT(29), 0x0);
			RF_DBG(dm, DBG_RF_DPK, "[DPK] S0 DPD enable!!!\n");
		}
		if ((dpk_info->dpk_path_ok & BIT(1)) >> 1) {
			odm_set_bb_reg(dm, R_0xb6c, BIT(29), 0x0);
			RF_DBG(dm, DBG_RF_DPK, "[DPK] S1 DPD enable!!!\n");
		}
	} else { /*bypass dpk result*/
#if 0
		odm_set_bb_reg(dm, R_0xb68, BIT(29), 0x1);
		odm_set_bb_reg(dm, R_0xb6c, BIT(29), 0x1);
		rfcal_info->is_dp_path_aok = 0;
		rfcal_info->is_dp_path_bok = 0;
#endif
		if ((dpk_info->dpk_path_ok & BIT(0))) {
			odm_set_bb_reg(dm, R_0xb68, BIT(29), 0x1);
			RF_DBG(dm, DBG_RF_DPK, "[DPK] S0 DPD disable!!!\n");
		}
		if ((dpk_info->dpk_path_ok & BIT(1)) >> 1) {
			odm_set_bb_reg(dm, R_0xb6c, BIT(29), 0x1);
			RF_DBG(dm, DBG_RF_DPK, "[DPK] S1 DPD disable!!!\n");
		}
	}
}

void _dpk_rf_setting_8192f(
	struct dm_struct *dm)
{
	/*DPK loopback switch on*/
	if (dm->cut_version != ODM_CUT_A) {
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x86, 0x0000c, 0x3);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x86, 0x0000c, 0x3);
		RF_DBG(dm, DBG_RF_DPK, "[DPK] 0x86@ S0 = 0x%x, S1 = 0x%x\n",
		       odm_get_rf_reg(dm, RF_PATH_A, RF_0x86, RFREG_MASK),
		       odm_get_rf_reg(dm, RF_PATH_B, RF_0x86, RFREG_MASK));	
	}

	/*disable CCK_IND*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x0, BIT(14), 0x0);
	odm_set_rf_reg(dm, RF_PATH_B, RF_0x0, BIT(14), 0x0);
}

void _dpk_result_reset_8192f(
	struct dm_struct *dm)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 path, group;

	dpk_info->dpk_path_ok = 0;

	for (path = 0; path < DPK_RF_PATH_NUM_8192F; path++) {
		for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
			dpk_info->pwsf_2g[path][group] = 0;
			dpk_info->dpk_result[path][group] = 0;

#if 0
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK][reset] S%d pwsf[%d]=0x%x, dpk_result[%d]=%d\n",
		       path, group, dpk_info->pwsf_2g[path][group],
		       group, dpk_info->dpk_result[path][group],
#endif
		}
	}

#if 0
	rfcal_info->is_dp_path_aok = 0;
	rfcal_info->is_dp_path_bok = 0;

	for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
		rfcal_info->pwsf_2g_a[group] = 0;
		rfcal_info->pwsf_2g_b[group] = 0;
		rfcal_info->dp_path_a_result[group] = 0;
		rfcal_info->dp_path_b_result[group] = 0;
#if 0
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK][reset] S0 pwsf[%d]=0x%x, dpk_result[%d]=%d\n",
		       group, rfcal_info->pwsf_2g_a[group],
		       group, rfcal_info->dp_path_a_result[group],
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK][reset] S1 pwsf[%d]=0x%x, dpk_result[%d]=%d\n",
		       group, rfcal_info->pwsf_2g_b[group],
		       group, rfcal_info->dp_path_b_result[group],
#endif
	}
#endif

}

void _dpk_set_group_8192f(
	struct dm_struct *dm,
	u8 group)
{
	switch (group) {
	case 0: /*channel 3*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x18, 0x00fff, 0xc03);
		break;

	case 1: /*channel 7*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x18, 0x00fff, 0xc07);
		break;

	case 2: /*channel 11*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x18, 0x00fff, 0xc0b);
		break;

	default:
		break;
	}
#if 0
	RF_DBG(dm, DBG_RF_DPK, "[DPK] switch to group%d, RF0x18 = 0x%x\n",
	       group, odm_get_rf_reg(dm, RF_PATH_A, RF_0x18, RFREG_MASK));
#endif
}

u8 _dpk_gainloss_8192f(
	struct dm_struct *dm,
	u8 path)
{
	u8 tx_agc_search = 0x0;

	switch (path) {

	case RF_PATH_A:

		tx_agc_search = phy_path_a_gainloss_8192f(dm);
		if (DPK_PAS_DBG_8192F)
			phy_path_a_pas_read_8192f(dm, TRUE);
		break;

	case RF_PATH_B:

		tx_agc_search = phy_path_b_gainloss_8192f(dm);
		if (DPK_PAS_DBG_8192F)
			phy_path_b_pas_read_8192f(dm, TRUE);
		break;

	default:
		break;
	}

	RF_DBG(dm, DBG_RF_DPK, "[DPK] TXAGC_search = 0x%x\n", tx_agc_search);
	return tx_agc_search;
}

u8 _dpk_by_path_8192f(
	struct dm_struct *dm,
	u8 tx_agc_search,
	u8 path,
	u8 group)
{
	u32 result;

	switch (path) {

	case RF_PATH_A:

		result = phy_path_a_dodpk_8192f(dm, tx_agc_search, group);
		if (DPK_PAS_DBG_8192F)
			phy_path_a_pas_read_8192f(dm, false);
		break;

	case RF_PATH_B:

		result = phy_path_b_dodpk_8192f(dm, tx_agc_search, group);
		if (DPK_PAS_DBG_8192F)
			phy_path_b_pas_read_8192f(dm, false);
		break;

	default:
		break;
	}

	return result;
}

void dpk_sram_read_8192f(
	void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 path, group, addr;

	RF_DBG(dm, DBG_RF_DPK, "[DPK] ========= SRAM Read Start =========\n");

	for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
		for (addr = 0; addr < 16; addr++)
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Read S0[%d] even[%2d] = 0x%x\n",
			       group, addr,
			       /*rfcal_info->lut_2g_even_a[group][addr]);*/
			       dpk_info->lut_2g_even[0][group][addr]);

		for (addr = 0; addr < 16; addr++)
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Read S0[%d]  odd[%2d] = 0x%x\n",
				group, addr,
				/*rfcal_info->lut_2g_odd_a[group][addr]);*/
			       dpk_info->lut_2g_odd[0][group][addr]);

		for (addr = 0; addr < 16; addr++)
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Read S1[%d] even[%2d] = 0x%x\n",
			       group, addr,
			       /*rfcal_info->lut_2g_even_b[group][addr]);*/
			       dpk_info->lut_2g_even[1][group][addr]);

		for (addr = 0; addr < 16; addr++)
			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] Read S1[%d]  odd[%2d] = 0x%x\n",
			       group, addr,
			       /*rfcal_info->lut_2g_odd_b[group][addr]);*/
			       dpk_info->lut_2g_odd[1][group][addr]);

	}

	RF_DBG(dm, DBG_RF_DPK, "[DPK] ========= SRAM Read Finish =========\n");
}

u8 _dpk_a_lut_sram_read_8192f(
	struct dm_struct *dm,
	u8 group)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 addr, i;
	u32 regb2c = 0x0, regbc0 = 0x0, regbc4 = 0x0;

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	for (addr = 0; addr < 64; addr++) { /*A even*/
		regb2c = (0x00801234 | (addr << 16));
		odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, regb2c);
		regbc0 = odm_get_bb_reg(dm, R_0xbc0, 0x003FFFFF);

		if (DPK_SRAM_IQ_DBG_8192F && (addr < 16))
			RF_DBG(dm, DBG_RF_DPK,
		               "[DPK] S0 LUT even[%2d] = 0x%x\n", addr, regbc0);	

		if (phy_path_a_iq_check_8192f(dm, group, addr, true)) {
			odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
			return 0;
		}
		/*rfcal_info->lut_2g_even_a[group][addr] = regbc0;*/
		dpk_info->lut_2g_even[0][group][addr] = regbc0;
	}

	for (addr = 0; addr < 64; addr++) { /*A odd*/
		regb2c = (0x00801234 | (addr << 16));
		odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, regb2c);
		regbc4 = odm_get_bb_reg(dm, R_0xbc4, 0x003FFFFF);

		if (DPK_SRAM_IQ_DBG_8192F && (addr < 16))
			RF_DBG(dm, DBG_RF_DPK,
		               "[DPK] S0 LUT  odd[%2d] = 0x%x\n", addr, regbc4);

		if (phy_path_a_iq_check_8192f(dm, group, addr, false)) {
			odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
			return 0;
		}
		/*rfcal_info->lut_2g_odd_a[group][addr] = regbc4;*/
		dpk_info->lut_2g_odd[0][group][addr] = regbc4;
	}

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	return 1;
}

u8 _dpk_b_lut_sram_read_8192f(
	struct dm_struct *dm,
	u8 group)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 addr, i;
	u32 regb2c = 0, regbc8 = 0x0, regbcc = 0x0;

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0xbac, MASKDWORD, 0x00000c00);

	for (addr = 0; addr < 64; addr++) { /*B even*/
		regb2c = (0x00801234 | (addr << 16));
		odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, regb2c);
		regbc8 = odm_get_bb_reg(dm, R_0xbc8, 0x003FFFFF);

		if (DPK_SRAM_IQ_DBG_8192F && (addr < 16))
			RF_DBG(dm, DBG_RF_DPK,
		               "[DPK] S1 LUT even[%2d] = 0x%x\n", addr, regbc8);	
			
		if (phy_path_b_iq_check_8192f(dm, group, addr, true)) {
			odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
			return 0;
		}
		/*rfcal_info->lut_2g_even_b[group][addr] = regbc8;*/
		dpk_info->lut_2g_even[1][group][addr] = regbc8;
	}

	for (addr = 0; addr < 64; addr++) { /*B odd*/
		regb2c = (0x00801234 | (addr << 16));
		odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, regb2c);
		regbcc = odm_get_bb_reg(dm, R_0xbcc, 0x003FFFFF);

		if (DPK_SRAM_IQ_DBG_8192F && (addr < 16))
			RF_DBG(dm, DBG_RF_DPK,
		               "[DPK] S1 LUT  odd[%2d] = 0x%x\n", addr, regbcc);	

		if (phy_path_b_iq_check_8192f(dm, group, addr, false)) {
			odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);
			return 0;
		}
		/*rfcal_info->lut_2g_odd_b[group][addr] = regbcc;*/
		dpk_info->lut_2g_odd[1][group][addr] = regbcc;
	}

	odm_set_bb_reg(dm, R_0xb2c, MASKDWORD, 0x00000000);

	return 1;
}

u8 _dpk_check_fail_8192f(
	struct dm_struct *dm,
	boolean is_fail,
	u8 path,
	u8 group)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 result = 0;
	
	switch (path) {

	case RF_PATH_A:

		if (!is_fail) {	
			if (_dpk_a_lut_sram_read_8192f(dm, group)) {
				/*rfcal_info->pwsf_2g_a[group] = (u8)odm_get_bb_reg(dm, R_0xb68, 0x00007C00);*/
				/*rfcal_info->dp_path_a_result[group] = 1;*/
				dpk_info->pwsf_2g[path][group] = (u8)odm_get_bb_reg(dm, R_0xb68, 0x00007C00);
				dpk_info->dpk_result[path][group] = 1;
				result = 1;
			} else {
				/*rfcal_info->pwsf_2g_a[group] = 0;*/
				/*rfcal_info->dp_path_a_result[group] = 0;*/
				dpk_info->pwsf_2g[path][group] = 0;
				dpk_info->dpk_result[path][group] = 0;
				result = 0;
			}
		} else {
			/*rfcal_info->pwsf_2g_a[group] = 0;*/
			/*rfcal_info->dp_path_a_result[group] = 0;*/
			dpk_info->pwsf_2g[path][group] = 0;
			dpk_info->dpk_result[path][group] = 0;
			result = 0;	

		}
		break;

	case RF_PATH_B:

		if (!is_fail) {	
			if (_dpk_b_lut_sram_read_8192f(dm, group)) {
				/*rfcal_info->pwsf_2g_b[group] = (u8)odm_get_bb_reg(dm, R_0xb6c, 0x00007C00);*/
				/*rfcal_info->dp_path_b_result[group] = 1;*/
				dpk_info->pwsf_2g[path][group] = (u8)odm_get_bb_reg(dm, R_0xb6c, 0x00007C00);
				dpk_info->dpk_result[path][group] = 1;
				result = 1;
			} else {
				/*rfcal_info->pwsf_2g_b[group] = 0;*/
				/*rfcal_info->dp_path_b_result[group] = 0;*/
				dpk_info->pwsf_2g[path][group] = 0;
				dpk_info->dpk_result[path][group] = 0;
				result = 0;
			}
		} else {
			/*rfcal_info->pwsf_2g_b[group] = 0;*/
			/*rfcal_info->dp_path_b_result[group] = 0;*/
			dpk_info->pwsf_2g[path][group] = 0;
			dpk_info->dpk_result[path][group] = 0;
			result = 0;
		}
		break;

	default:
		break;
	}

	if (result == 1)
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] ========= S%d[%d] DPK Success ========\n",
		       path, group);
	else
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] ======= S%d[%d] DPK need check =======\n",
		       path, group);
	return result;
}

void _dpk_calibrate_8192f(
	struct dm_struct *dm,
	u8 path)
{

	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 dpk_fail = 1, tx_agc_search = 0;
	u8 group, retry_cnt;

	for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
		_dpk_set_group_8192f(dm, group);

		for (retry_cnt = 0; retry_cnt < 6; retry_cnt++) {
			RF_DBG(dm, DBG_RF_DPK, "[DPK] S%d[%d] retry =%d\n",
			       path, group, retry_cnt);

			RF_DBG(dm, DBG_RF_DPK,
			       "[DPK] ========= S%d[%d] DPK Start =========\n",
			       path, group);

			tx_agc_search = _dpk_gainloss_8192f(dm, path);

			ODM_delay_ms(1);

			dpk_fail = _dpk_by_path_8192f(dm, tx_agc_search,
						      path, group);

			if (_dpk_check_fail_8192f(dm, dpk_fail, path, group))
				break;
		}
	}

	if (dpk_info->dpk_result[path][0] &&
	    dpk_info->dpk_result[path][1] &&
	    dpk_info->dpk_result[path][2]) {
		dpk_info->dpk_path_ok = dpk_info->dpk_path_ok | BIT(path);
	}

	if (dpk_info->dpk_path_ok > 0)
		dpk_info->is_dpk_enable = 1;
#if 0
	if (rfcal_info->dp_path_a_result[0] &&
	    rfcal_info->dp_path_a_result[1] &&
	    rfcal_info->dp_path_a_result[2]) {
		rfcal_info->is_dp_path_aok = 1;
	}

	if (rfcal_info->dp_path_b_result[0] &&
	    rfcal_info->dp_path_b_result[1] &&
	    rfcal_info->dp_path_b_result[2]) {
		rfcal_info->is_dp_path_bok = 1;
	}
#endif
}

void _dpk_on_8192f(
	struct dm_struct *dm,
	u8 path)
{

	switch (path) {

	case RF_PATH_A:
		phy_path_a_dpk_enable_8192f(dm);
		break;

	case RF_PATH_B:
		phy_path_b_dpk_enable_8192f(dm);
		break;

	default:
		break;
	}	
}

void _dpk_path_select_8192f(
	struct dm_struct *dm)
{
#if (DPK_DO_PATH_A)
	_dpk_calibrate_8192f(dm, RF_PATH_A);
	_dpk_on_8192f(dm, RF_PATH_A);
#endif

#if (DPK_DO_PATH_B)
	_dpk_calibrate_8192f(dm, RF_PATH_B);
	_dpk_on_8192f(dm, RF_PATH_B);
#endif
}

void _dpk_result_summary_8192f(
	struct dm_struct *dm)
{
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u8 path, group;

	RF_DBG(dm, DBG_RF_DPK, "[DPK] ======== DPK Result Summary =======\n");


	for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
#if 0
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] S0[%d] pwsf = 0x%x, dpk_result = %d\n",
		       group, rfcal_info->pwsf_2g_a[group],
		       rfcal_info->dp_path_a_result[group]);
#endif
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] S0[%d] pwsf = 0x%x, dpk_result = %d\n",
		       group, dpk_info->pwsf_2g[0][group],
		       dpk_info->dpk_result[0][group]);
	}
#if 0
	RF_DBG(dm, DBG_RF_DPK,
	       "[DPK] S0 DPK is %s\n", rfcal_info->is_dp_path_aok ?
	       "Success" : "Fail");
#endif

	RF_DBG(dm, DBG_RF_DPK,
	       "[DPK] S0 DPK is %s\n",((dpk_info->dpk_path_ok & BIT(0)) >> 0) ?
	       "Success" : "Fail");

	for (group = 0; group < DPK_GROUP_NUM_8192F; group++) {
		RF_DBG(dm, DBG_RF_DPK,
		       "[DPK] S1[%d] pwsf = 0x%x, dpk_result = %d\n",
		       group, dpk_info->pwsf_2g[1][group],
		       dpk_info->dpk_result[1][group]);
	}
#if 0
	RF_DBG(dm, DBG_RF_DPK,
	       "[DPK] S1 DPK is %s\n", rfcal_info->is_dp_path_bok ?
	       "Success" : "Fail");
#endif

	RF_DBG(dm, DBG_RF_DPK,
	       "[DPK] S1 DPK is %s\n",((dpk_info->dpk_path_ok & BIT(1)) >> 1) ?
	       "Success" : "Fail");

	RF_DBG(dm, DBG_RF_DPK, "[DPK] dpk_path_ok = 0x%x, dpk_enable = %d\n",
	       dpk_info->dpk_path_ok, dpk_info->is_dpk_enable);	

	RF_DBG(dm, DBG_RF_DPK, "[DPK] ======== DPK Result Summary =======\n");

#if (DPK_SRAM_read_DBG_8192F)
	dpk_sram_read_8192f(dm);
#endif	

}

void dpk_reload_8192f(
	void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	phy_lut_sram_write_8192f(dm);
	/*_dpk_on_8192f(dm, RF_PATH_A);*/
	/*_dpk_on_8192f(dm, RF_PATH_B);*/
	phy_dpk_enable_disable_8192f(dm);
}


void do_dpk_8192f(
	void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;

	u32 mac_reg_backup[DPK_MAC_REG_NUM_8192F];
	u32 bb_reg_backup[DPK_BB_REG_NUM_8192F];
	u32 rf_reg_backup[DPK_RF_REG_NUM_8192F][DPK_RF_PATH_NUM_8192F];

	u32 mac_reg[DPK_MAC_REG_NUM_8192F] = {
		R_0xd94, REG_RX_WAIT_CCA, R_0x520,
		REG_BCN_CTRL, REG_GPIO_MUXCFG};

	u32 bb_reg[DPK_BB_REG_NUM_8192F] = {
		REG_OFDM_0_TRX_PATH_ENABLE, REG_OFDM_0_TR_MUX_PAR,
		REG_FPGA0_XCD_RF_INTERFACE_SW, R_0x800, R_0x88c, R_0xc5c,
		R_0xc14, R_0xc1c, R_0x880, R_0x884, R_0xe08, R_0x86c, R_0xe00,
		R_0xe04, R_0xe10, R_0xe14, R_0x838, R_0x830, R_0x834, R_0x83c,
		R_0x848, R_0x930, R_0x938, R_0x93c, R_0x92c, R_0xc30, R_0xc3c,
		R_0xc84};

	u32 rf_reg[DPK_RF_REG_NUM_8192F] = {RF_0x0, RF_0x18, RF_0x86, RF_0x8f};

	if (!((dm->rfe_type == 3) || (dm->rfe_type == 9) || (dm->rfe_type == 15))) {
		printk("[DPK] Skip DPK due to RFE type != 3 or 9 or 15\n");
		return;
	}
	RF_DBG(dm, DBG_RF_DPK, "[DPK] DPK Start (Ver: %s), Cv is %d\n",
	       DPK_VER_8192F, dm->cut_version);

	/*phy_path_a_dpk_init_8192f(dm);*/
	/*phy_path_b_dpk_init_8192f(dm);*/

	_backup_mac_bb_registers_8192f(dm, mac_reg, mac_reg_backup, DPK_MAC_REG_NUM_8192F);
	_backup_mac_bb_registers_8192f(dm, bb_reg, bb_reg_backup, DPK_BB_REG_NUM_8192F);
	_backup_rf_registers_8192f(dm, rf_reg, rf_reg_backup);

	_dpk_mac_bb_setting_8192f(dm);
	_dpk_rf_setting_8192f(dm);

	_dpk_result_reset_8192f(dm);
	_dpk_path_select_8192f(dm);
	_dpk_result_summary_8192f(dm);

	_reload_rf_registers_8192f(dm, rf_reg, rf_reg_backup);
	_reload_mac_bb_registers_8192f(dm, bb_reg, bb_reg_backup, DPK_BB_REG_NUM_8192F);
	_reload_mac_bb_registers_8192f(dm, mac_reg, mac_reg_backup, DPK_MAC_REG_NUM_8192F);
	
	dpk_reload_8192f(dm);

#if 0
	if (rfcal_info->is_dp_path_aok == 1 && rfcal_info->is_dp_path_bok == 1)
		phy_lut_sram_write_8192f(dm);
	else
		phy_dpk_enable_disable_8192f(dm, false);

	if (dpk_info->dpk_path_ok == 0x3)
		phy_lut_sram_write_8192f(dm);
	else
		phy_dpk_enable_disable_8192f(dm, false);
#endif

		
}

void phy_dpk_track_8192f(
	struct dm_struct *dm)
{
	struct rtl8192cd_priv *priv = dm->priv;
	/*struct dm_rf_calibration_struct *rfcal_info = &dm->rf_calibrate_info;*/
	struct dm_dpk_info *dpk_info = &dm->dpk_info;
	struct _hal_rf_ *rf = &dm->rf_table;

	s8 pwsf_a, pwsf_b;
	u8 offset, delta_dpk, is_increase, thermal_value = 0, thermal_dpk_avg_count = 0, i = 0, k = 0;
	u32 thermal_dpk_avg = 0;

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP))
#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific) {
		if (priv->pshare->mp_tx_dpk_tracking == false)
			return;
	}
#endif
#endif

#if 0
	if (!rfcal_info->thermal_value_dpk)
		rfcal_info->thermal_value_dpk = priv->pmib->dot11RFEntry.ther;
#endif

	if (!dpk_info->thermal_dpk[0])
		dpk_info->thermal_dpk[0] = rf->eeprom_thermal;

	/* calculate average thermal meter */

	odm_set_rf_reg(dm, RF_PATH_A, RF_T_METER_8192F, BIT(17) | BIT(16), 0x3); /*thermal meter trigger*/
	ODM_delay_ms(1);
	thermal_value = (u8)odm_get_rf_reg(dm, RF_PATH_A, RF_T_METER_8192F, 0xfc00); /*get thermal meter*/

#if 0
	rfcal_info->thermal_value_dpk_avg[rfcal_info->thermal_value_dpk_avg_index] = thermal_value;
	rfcal_info->thermal_value_dpk_avg_index++;

	if (rfcal_info->thermal_value_dpk_avg_index == THERMAL_DPK_AVG_NUM) /*Average times */
		rfcal_info->thermal_value_dpk_avg_index = 0;

	for (i = 0; i < THERMAL_DPK_AVG_NUM; i++) {
		if (rfcal_info->thermal_value_dpk_avg[i]) {
			thermal_value_dpk_avg += rfcal_info->thermal_value_dpk_avg[i];
			thermal_value_dpk_avg_count++;
		}
	}
#endif
	dpk_info->thermal_dpk_avg[0][dpk_info->thermal_dpk_avg_index] = thermal_value;
	dpk_info->thermal_dpk_avg_index++;

	if (dpk_info->thermal_dpk_avg_index == THERMAL_DPK_AVG_NUM) /*Average times */
		dpk_info->thermal_dpk_avg_index = 0;

	for (i = 0; i < THERMAL_DPK_AVG_NUM; i++) {
		if (dpk_info->thermal_dpk_avg[0][i]) {
			thermal_dpk_avg += dpk_info->thermal_dpk_avg[0][i];
			thermal_dpk_avg_count++;
		}
	}


	if (thermal_dpk_avg_count) { /*Calculate Average ThermalValue after average enough times*/
		RF_DBG(dm, DBG_RF_DPK_TRACK,
		       "[DPK_track] ThermalValue_DPK_AVG = %d  ThermalValue_DPK_AVG_count = %d\n",
		       thermal_dpk_avg, thermal_dpk_avg_count);

		thermal_value = (u8)(thermal_dpk_avg / thermal_dpk_avg_count);

		RF_DBG(dm, DBG_RF_DPK_TRACK,
		       "[DPK_track] AVG Thermal Meter = %d, PG Thermal Meter = %d\n",
		       thermal_value, rf->eeprom_thermal);
	}

	delta_dpk = RTL_ABS(thermal_value, rf->eeprom_thermal);
	is_increase = ((thermal_value < rf->eeprom_thermal) ? 0 : 1);

	offset = delta_dpk / DPK_THRESHOLD_8192F;

	k = phy_dpk_channel_transfer_8192f(dm);

#if 0
	pwsf_a = rfcal_info->pwsf_2g_a[k];
	pwsf_b = rfcal_info->pwsf_2g_b[k];
#endif
	pwsf_a = dpk_info->pwsf_2g[0][k];
	pwsf_b = dpk_info->pwsf_2g[1][k];

	RF_DBG(dm, DBG_RF_DPK_TRACK,
	       "[DPK track] delta_DPK = %d, offset = %d, track direction is %s\n",
	       delta_dpk, offset, (is_increase ? "Plus" : "Minus"));

	RF_DBG(dm, DBG_RF_DPK_TRACK,
	       "[DPK track] pwsf_a default is 0x%x, pwsf_b default is 0x%x\n",
	       pwsf_a, pwsf_b);

	if ((pwsf_a >> 4) != 0)
		pwsf_a = (pwsf_a | 0xe0);

	if ((pwsf_b >> 4) != 0)
		pwsf_b = (pwsf_b | 0xe0);

	if (is_increase) {
		pwsf_a = pwsf_a + offset;
		pwsf_b = pwsf_b + offset;
	} else {
		pwsf_a = pwsf_a - offset;
		pwsf_b = pwsf_b - offset;
	}

	odm_set_bb_reg(dm, R_0xb68, 0x00007C00, pwsf_a);
	odm_set_bb_reg(dm, R_0xb6c, 0x00007C00, pwsf_b);
	RF_DBG(dm, DBG_RF_DPK_TRACK,
	       "[DPK track] pwsf_a after tracking is %d (0x%x), 0xb68 = 0x%x\n",
	       pwsf_a, (pwsf_a & 0x1f), odm_get_bb_reg(dm, R_0xb68, MASKDWORD));
	RF_DBG(dm, DBG_RF_DPK_TRACK,
	       "[DPK track] pwsf_b after tracking is %d (0x%x), 0xb6c = 0x%x\n",
	       pwsf_b, (pwsf_b & 0x1f), odm_get_bb_reg(dm, R_0xb6c, MASKDWORD));
}

#endif
