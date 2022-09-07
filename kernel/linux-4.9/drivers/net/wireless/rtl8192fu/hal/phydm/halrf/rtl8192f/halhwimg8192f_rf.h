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
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/

/*Image2HeaderVersion: R3 1.5.6*/
#if (RTL8192F_SUPPORT == 1)
#ifndef __INC_MP_RF_HW_IMG_8192F_H
#define __INC_MP_RF_HW_IMG_8192F_H

/* Please add following compiler flags definition (#define CONFIG_XXX_DRV_DIS)
 * into driver source code to reduce code size if necessary.
 * #define CONFIG_8192F_DRV_DIS
 * #define CONFIG_8192F_TYPE0_DRV_DIS
 * #define CONFIG_8192F_TYPE1_DRV_DIS
 * #define CONFIG_8192F_TYPE10_DRV_DIS
 * #define CONFIG_8192F_TYPE11_DRV_DIS
 * #define CONFIG_8192F_TYPE12_DRV_DIS
 * #define CONFIG_8192F_TYPE13_DRV_DIS
 * #define CONFIG_8192F_TYPE14_DRV_DIS
 * #define CONFIG_8192F_TYPE15_DRV_DIS
 * #define CONFIG_8192F_TYPE16_DRV_DIS
 * #define CONFIG_8192F_TYPE17_DRV_DIS
 * #define CONFIG_8192F_TYPE18_DRV_DIS
 * #define CONFIG_8192F_TYPE19_DRV_DIS
 * #define CONFIG_8192F_TYPE2_DRV_DIS
 * #define CONFIG_8192F_TYPE20_DRV_DIS
 * #define CONFIG_8192F_TYPE21_DRV_DIS
 * #define CONFIG_8192F_TYPE22_DRV_DIS
 * #define CONFIG_8192F_TYPE23_DRV_DIS
 * #define CONFIG_8192F_TYPE24_DRV_DIS
 * #define CONFIG_8192F_TYPE25_DRV_DIS
 * #define CONFIG_8192F_TYPE26_DRV_DIS
 * #define CONFIG_8192F_TYPE27_DRV_DIS
 * #define CONFIG_8192F_TYPE28_DRV_DIS
 * #define CONFIG_8192F_TYPE29_DRV_DIS
 * #define CONFIG_8192F_TYPE3_DRV_DIS
 * #define CONFIG_8192F_TYPE30_DRV_DIS
 * #define CONFIG_8192F_TYPE31_DRV_DIS
 * #define CONFIG_8192F_TYPE4_DRV_DIS
 * #define CONFIG_8192F_TYPE5_DRV_DIS
 * #define CONFIG_8192F_TYPE6_DRV_DIS
 * #define CONFIG_8192F_TYPE7_DRV_DIS
 * #define CONFIG_8192F_TYPE8_DRV_DIS
 * #define CONFIG_8192F_TYPE9_DRV_DIS
 */

#define CONFIG_8192F
#ifdef CONFIG_8192F_DRV_DIS
    #undef CONFIG_8192F
#endif

#define CONFIG_8192F_TYPE0
#ifdef CONFIG_8192F_TYPE0_DRV_DIS
    #undef CONFIG_8192F_TYPE0
#endif

#define CONFIG_8192F_TYPE1
#ifdef CONFIG_8192F_TYPE1_DRV_DIS
    #undef CONFIG_8192F_TYPE1
#endif

#define CONFIG_8192F_TYPE10
#ifdef CONFIG_8192F_TYPE10_DRV_DIS
    #undef CONFIG_8192F_TYPE10
#endif

#define CONFIG_8192F_TYPE11
#ifdef CONFIG_8192F_TYPE11_DRV_DIS
    #undef CONFIG_8192F_TYPE11
#endif

#define CONFIG_8192F_TYPE12
#ifdef CONFIG_8192F_TYPE12_DRV_DIS
    #undef CONFIG_8192F_TYPE12
#endif

#define CONFIG_8192F_TYPE13
#ifdef CONFIG_8192F_TYPE13_DRV_DIS
    #undef CONFIG_8192F_TYPE13
#endif

#define CONFIG_8192F_TYPE14
#ifdef CONFIG_8192F_TYPE14_DRV_DIS
    #undef CONFIG_8192F_TYPE14
#endif

#define CONFIG_8192F_TYPE15
#ifdef CONFIG_8192F_TYPE15_DRV_DIS
    #undef CONFIG_8192F_TYPE15
#endif

#define CONFIG_8192F_TYPE16
#ifdef CONFIG_8192F_TYPE16_DRV_DIS
    #undef CONFIG_8192F_TYPE16
#endif

#define CONFIG_8192F_TYPE17
#ifdef CONFIG_8192F_TYPE17_DRV_DIS
    #undef CONFIG_8192F_TYPE17
#endif

#define CONFIG_8192F_TYPE18
#ifdef CONFIG_8192F_TYPE18_DRV_DIS
    #undef CONFIG_8192F_TYPE18
#endif

#define CONFIG_8192F_TYPE19
#ifdef CONFIG_8192F_TYPE19_DRV_DIS
    #undef CONFIG_8192F_TYPE19
#endif

#define CONFIG_8192F_TYPE2
#ifdef CONFIG_8192F_TYPE2_DRV_DIS
    #undef CONFIG_8192F_TYPE2
#endif

#define CONFIG_8192F_TYPE20
#ifdef CONFIG_8192F_TYPE20_DRV_DIS
    #undef CONFIG_8192F_TYPE20
#endif

#define CONFIG_8192F_TYPE21
#ifdef CONFIG_8192F_TYPE21_DRV_DIS
    #undef CONFIG_8192F_TYPE21
#endif

#define CONFIG_8192F_TYPE22
#ifdef CONFIG_8192F_TYPE22_DRV_DIS
    #undef CONFIG_8192F_TYPE22
#endif

#define CONFIG_8192F_TYPE23
#ifdef CONFIG_8192F_TYPE23_DRV_DIS
    #undef CONFIG_8192F_TYPE23
#endif

#define CONFIG_8192F_TYPE24
#ifdef CONFIG_8192F_TYPE24_DRV_DIS
    #undef CONFIG_8192F_TYPE24
#endif

#define CONFIG_8192F_TYPE25
#ifdef CONFIG_8192F_TYPE25_DRV_DIS
    #undef CONFIG_8192F_TYPE25
#endif

#define CONFIG_8192F_TYPE26
#ifdef CONFIG_8192F_TYPE26_DRV_DIS
    #undef CONFIG_8192F_TYPE26
#endif

#define CONFIG_8192F_TYPE27
#ifdef CONFIG_8192F_TYPE27_DRV_DIS
    #undef CONFIG_8192F_TYPE27
#endif

#define CONFIG_8192F_TYPE28
#ifdef CONFIG_8192F_TYPE28_DRV_DIS
    #undef CONFIG_8192F_TYPE28
#endif

#define CONFIG_8192F_TYPE29
#ifdef CONFIG_8192F_TYPE29_DRV_DIS
    #undef CONFIG_8192F_TYPE29
#endif

#define CONFIG_8192F_TYPE3
#ifdef CONFIG_8192F_TYPE3_DRV_DIS
    #undef CONFIG_8192F_TYPE3
#endif

#define CONFIG_8192F_TYPE30
#ifdef CONFIG_8192F_TYPE30_DRV_DIS
    #undef CONFIG_8192F_TYPE30
#endif

#define CONFIG_8192F_TYPE31
#ifdef CONFIG_8192F_TYPE31_DRV_DIS
    #undef CONFIG_8192F_TYPE31
#endif

#define CONFIG_8192F_TYPE4
#ifdef CONFIG_8192F_TYPE4_DRV_DIS
    #undef CONFIG_8192F_TYPE4
#endif

#define CONFIG_8192F_TYPE5
#ifdef CONFIG_8192F_TYPE5_DRV_DIS
    #undef CONFIG_8192F_TYPE5
#endif

#define CONFIG_8192F_TYPE6
#ifdef CONFIG_8192F_TYPE6_DRV_DIS
    #undef CONFIG_8192F_TYPE6
#endif

#define CONFIG_8192F_TYPE7
#ifdef CONFIG_8192F_TYPE7_DRV_DIS
    #undef CONFIG_8192F_TYPE7
#endif

#define CONFIG_8192F_TYPE8
#ifdef CONFIG_8192F_TYPE8_DRV_DIS
    #undef CONFIG_8192F_TYPE8
#endif

#define CONFIG_8192F_TYPE9
#ifdef CONFIG_8192F_TYPE9_DRV_DIS
    #undef CONFIG_8192F_TYPE9
#endif

/******************************************************************************
 *                           radioa.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_radioa(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_radioa(void);

/******************************************************************************
 *                           radiob.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_radiob(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_radiob(void);

/******************************************************************************
 *                           txpowertrack.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack(void);

/******************************************************************************
 *                           txpowertrack_type0.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type0(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type0(void);

/******************************************************************************
 *                           txpowertrack_type1.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type1(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type1(void);

/******************************************************************************
 *                           txpowertrack_type10.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type10(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type10(void);

/******************************************************************************
 *                           txpowertrack_type11.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type11(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type11(void);

/******************************************************************************
 *                           txpowertrack_type12.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type12(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type12(void);

/******************************************************************************
 *                           txpowertrack_type13.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type13(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type13(void);

/******************************************************************************
 *                           txpowertrack_type14.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type14(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type14(void);

/******************************************************************************
 *                           txpowertrack_type15.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type15(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type15(void);

/******************************************************************************
 *                           txpowertrack_type16.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type16(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type16(void);

/******************************************************************************
 *                           txpowertrack_type17.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type17(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type17(void);

/******************************************************************************
 *                           txpowertrack_type18.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type18(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type18(void);

/******************************************************************************
 *                           txpowertrack_type19.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type19(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type19(void);

/******************************************************************************
 *                           txpowertrack_type2.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type2(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type2(void);

/******************************************************************************
 *                           txpowertrack_type20.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type20(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type20(void);

/******************************************************************************
 *                           txpowertrack_type21.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type21(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type21(void);

/******************************************************************************
 *                           txpowertrack_type22.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type22(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type22(void);

/******************************************************************************
 *                           txpowertrack_type23.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type23(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type23(void);

/******************************************************************************
 *                           txpowertrack_type24.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type24(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type24(void);

/******************************************************************************
 *                           txpowertrack_type25.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type25(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type25(void);

/******************************************************************************
 *                           txpowertrack_type26.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type26(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type26(void);

/******************************************************************************
 *                           txpowertrack_type27.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type27(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type27(void);

/******************************************************************************
 *                           txpowertrack_type28.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type28(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type28(void);

/******************************************************************************
 *                           txpowertrack_type29.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type29(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type29(void);

/******************************************************************************
 *                           txpowertrack_type3.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type3(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type3(void);

/******************************************************************************
 *                           txpowertrack_type30.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type30(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type30(void);

/******************************************************************************
 *                           txpowertrack_type31.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type31(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type31(void);

/******************************************************************************
 *                           txpowertrack_type4.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type4(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type4(void);

/******************************************************************************
 *                           txpowertrack_type5.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type5(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type5(void);

/******************************************************************************
 *                           txpowertrack_type6.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type6(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type6(void);

/******************************************************************************
 *                           txpowertrack_type7.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type7(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type7(void);

/******************************************************************************
 *                           txpowertrack_type8.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type8(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type8(void);

/******************************************************************************
 *                           txpowertrack_type9.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpowertrack_type9(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpowertrack_type9(void);

/******************************************************************************
 *                           txpwr_lmt.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt(void);

/******************************************************************************
 *                           txpwr_lmt_type0.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type0(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type0(void);

/******************************************************************************
 *                           txpwr_lmt_type1.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type1(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type1(void);

/******************************************************************************
 *                           txpwr_lmt_type10.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type10(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type10(void);

/******************************************************************************
 *                           txpwr_lmt_type11.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type11(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type11(void);

/******************************************************************************
 *                           txpwr_lmt_type12.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type12(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type12(void);

/******************************************************************************
 *                           txpwr_lmt_type13.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type13(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type13(void);

/******************************************************************************
 *                           txpwr_lmt_type14.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type14(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type14(void);

/******************************************************************************
 *                           txpwr_lmt_type15.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type15(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type15(void);

/******************************************************************************
 *                           txpwr_lmt_type16.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type16(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type16(void);

/******************************************************************************
 *                           txpwr_lmt_type17.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type17(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type17(void);

/******************************************************************************
 *                           txpwr_lmt_type18.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type18(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type18(void);

/******************************************************************************
 *                           txpwr_lmt_type19.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type19(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type19(void);

/******************************************************************************
 *                           txpwr_lmt_type2.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type2(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type2(void);

/******************************************************************************
 *                           txpwr_lmt_type20.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type20(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type20(void);

/******************************************************************************
 *                           txpwr_lmt_type21.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type21(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type21(void);

/******************************************************************************
 *                           txpwr_lmt_type22.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type22(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type22(void);

/******************************************************************************
 *                           txpwr_lmt_type23.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type23(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type23(void);

/******************************************************************************
 *                           txpwr_lmt_type24.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type24(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type24(void);

/******************************************************************************
 *                           txpwr_lmt_type25.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type25(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type25(void);

/******************************************************************************
 *                           txpwr_lmt_type26.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type26(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type26(void);

/******************************************************************************
 *                           txpwr_lmt_type27.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type27(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type27(void);

/******************************************************************************
 *                           txpwr_lmt_type28.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type28(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type28(void);

/******************************************************************************
 *                           txpwr_lmt_type29.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type29(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type29(void);

/******************************************************************************
 *                           txpwr_lmt_type3.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type3(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type3(void);

/******************************************************************************
 *                           txpwr_lmt_type30.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type30(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type30(void);

/******************************************************************************
 *                           txpwr_lmt_type31.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type31(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type31(void);

/******************************************************************************
 *                           txpwr_lmt_type4.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type4(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type4(void);

/******************************************************************************
 *                           txpwr_lmt_type5.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type5(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type5(void);

/******************************************************************************
 *                           txpwr_lmt_type6.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type6(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type6(void);

/******************************************************************************
 *                           txpwr_lmt_type7.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type7(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type7(void);

/******************************************************************************
 *                           txpwr_lmt_type8.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type8(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type8(void);

/******************************************************************************
 *                           txpwr_lmt_type9.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txpwr_lmt_type9(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txpwr_lmt_type9(void);

/******************************************************************************
 *                           txxtaltrack.TXT
 ******************************************************************************/

/* tc: Test Chip, mp: mp Chip*/
void
odm_read_and_config_mp_8192f_txxtaltrack(struct dm_struct *dm);
u32 odm_get_version_mp_8192f_txxtaltrack(void);

#endif
#endif /* end of HWIMG_SUPPORT*/

