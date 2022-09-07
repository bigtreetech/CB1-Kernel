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
#define _RTL8192F_PHYCFG_C_

#include <rtl8192f_hal.h>


/*---------------------------Define Local Constant---------------------------*/
/* Channel switch:The size of command tables for switch channel*/
#define MAX_PRECMD_CNT 16
#define MAX_RFDEPENDCMD_CNT 16
#define MAX_POSTCMD_CNT 16

#define MAX_DOZE_WAITING_TIMES_9x 64

/*---------------------------Define Local Constant---------------------------*/


/*------------------------Define global variable-----------------------------*/

/*------------------------Define local variable------------------------------*/


/*--------------------Define export function prototype-----------------------*/
/* Please refer to header file
 *--------------------Define export function prototype-----------------------*/

/*----------------------------Function Body----------------------------------*/
/*
 * 1. BB register R/W API
 *   */

/**
* Function:	phy_CalculateBitShift
*
* OverView:	Get shifted position of the BitMask
*
* Input:
*			u32		BitMask,
*
* Output:	none
* Return:		u32		Return the shift bit bit position of the mask
*/
static	u32
phy_CalculateBitShift(
	u32 BitMask
)
{
	u32 i;

	for (i = 0; i <= 31; i++) {
		if (((BitMask >> i) &  0x1) == 1)
			break;
	}

	return i;
}


/**
* Function:	PHY_QueryBBReg
*
* OverView:	Read "sepcific bits" from BB register
*
* Input:
*			PADAPTER		Adapter,
*			u32			RegAddr,
*			u32			BitMask
*
* Output:	None
* Return:		u32			Data
* Note:		This function is equal to "GetRegSetting" in PHY programming guide
*/
u32
PHY_QueryBBReg_8192F(
		PADAPTER	Adapter,
		u32		RegAddr,
		u32		BitMask
)
{
	u32	ReturnValue = 0, OriginalValue, BitShift;
	u16	BBWaitCounter = 0;

#if (DISABLE_BB_RF == 1)
	return 0;
#endif


	OriginalValue = rtw_read32(Adapter, RegAddr);
	BitShift = phy_CalculateBitShift(BitMask);
	ReturnValue = (OriginalValue & BitMask) >> BitShift;

	return ReturnValue;

}


/**
* Function:	PHY_SetBBReg
*
* OverView:	Write "Specific bits" to BB register (page 8~)
*
* Input:
*			PADAPTER		Adapter,
*			u32			RegAddr,
*			u32			BitMask
*
*			u32			Data
*
*
* Output:	None
* Return:		None
* Note:		This function is equal to "PutRegSetting" in PHY programming guide
*/

void
PHY_SetBBReg_8192F(
		PADAPTER	Adapter,
		u32		RegAddr,
		u32		BitMask,
		u32		Data
)
{
	HAL_DATA_TYPE	*pHalData		= GET_HAL_DATA(Adapter);
	/* u16			BBWaitCounter	= 0; */
	u32			OriginalValue, BitShift;

#if (DISABLE_BB_RF == 1)
	return;
#endif


	if (BitMask != bMaskDWord) { /* if not "double word" write */
		OriginalValue = rtw_read32(Adapter, RegAddr);
		BitShift = phy_CalculateBitShift(BitMask);
		Data = ((OriginalValue & (~BitMask)) | ((Data << BitShift) & BitMask));
	}

	rtw_write32(Adapter, RegAddr, Data);

}


/*
 * 2. RF register R/W API
 *   */
static	u32
phy_RFSerialRead_8192F(
		PADAPTER			Adapter,
		enum rf_path			eRFPath,
		u32				Offset
)
{
	u32						retValue = 0;
	HAL_DATA_TYPE				*pHalData = GET_HAL_DATA(Adapter);
	BB_REGISTER_DEFINITION_T	*pPhyReg = &pHalData->PHYRegDef[eRFPath];
	u32						NewOffset;
	u32						tmplong, tmplong2;
	u8					RfPiEnable = 0;
	u32						MaskforPhySet = 0;
	int i = 0;

	_enter_critical_mutex(&(adapter_to_dvobj(Adapter)->rf_read_reg_mutex) , NULL);
	/* */
	/* Make sure RF register offset is correct */
	/* */
	Offset &= 0xff;

	NewOffset = Offset;

	if (eRFPath == RF_PATH_A) {
		tmplong2 = phy_query_bb_reg(Adapter, rFPGA0_XA_HSSIParameter2 | MaskforPhySet, bMaskDWord);
		tmplong2 = (tmplong2 & (~bLSSIReadAddress)) | (NewOffset << 23) | bLSSIReadEdge;	/* T65 RF */
		phy_set_bb_reg(Adapter, rFPGA0_XA_HSSIParameter2 | MaskforPhySet, bMaskDWord, tmplong2 & (~bLSSIReadEdge));
	} else {
		tmplong2 = phy_query_bb_reg(Adapter, rFPGA0_XB_HSSIParameter2 | MaskforPhySet, bMaskDWord);
		tmplong2 = (tmplong2 & (~bLSSIReadAddress)) | (NewOffset << 23) | bLSSIReadEdge;	/* T65 RF */
		phy_set_bb_reg(Adapter, rFPGA0_XB_HSSIParameter2 | MaskforPhySet, bMaskDWord, tmplong2 & (~bLSSIReadEdge));
	}

	tmplong2 = phy_query_bb_reg(Adapter, rFPGA0_XA_HSSIParameter2 | MaskforPhySet, bMaskDWord);
	phy_set_bb_reg(Adapter, rFPGA0_XA_HSSIParameter2 | MaskforPhySet, bMaskDWord, tmplong2 & (~bLSSIReadEdge));
	phy_set_bb_reg(Adapter, rFPGA0_XA_HSSIParameter2 | MaskforPhySet, bMaskDWord, tmplong2 | bLSSIReadEdge);

	rtw_udelay_os(10);

	for (i = 0; i < 2; i++)
		rtw_udelay_os(MAX_STALL_TIME);
	rtw_udelay_os(10);

	if (eRFPath == RF_PATH_A)
		RfPiEnable = (u8)phy_query_bb_reg(Adapter, rFPGA0_XA_HSSIParameter1 | MaskforPhySet, BIT(8));
	else if (eRFPath == RF_PATH_B)
		RfPiEnable = (u8)phy_query_bb_reg(Adapter, rFPGA0_XB_HSSIParameter1 | MaskforPhySet, BIT(8));

	if (RfPiEnable) {
		/* Read from BBreg8b8, 12 bits for 8190, 20bits for T65 RF */
		retValue = phy_query_bb_reg(Adapter, pPhyReg->rfLSSIReadBackPi | MaskforPhySet, bLSSIReadBackData);

		/* RT_DISP(FINIT, INIT_RF, ("Readback from RF-PI : 0x%x\n", retValue)); */
	} else {
		/* Read from BBreg8a0, 12 bits for 8190, 20 bits for T65 RF */
		retValue = phy_query_bb_reg(Adapter, pPhyReg->rfLSSIReadBack | MaskforPhySet, bLSSIReadBackData);

		/* RT_DISP(FINIT, INIT_RF,("Readback from RF-SI : 0x%x\n", retValue)); */
	}
	_exit_critical_mutex(&(adapter_to_dvobj(Adapter)->rf_read_reg_mutex) , NULL);
	return retValue;

}

/**
* Function:	phy_RFSerialWrite_8723D
*
* OverView:	Write data to RF register (page 8~)
*
* Input:
*			PADAPTER		Adapter,
			enum rf_path		eRFPath,
*			u32			Offset,
*			u32			Data
*
*
* Output:	None
* Return:		None
* Note:		Threre are three types of serial operations:
*			1. Software serial write
*			2. Hardware LSSI-Low Speed Serial Interface
*			3. Hardware HSSI-High speed
*			serial write. Driver need to implement (1) and (2).
*			This function is equal to the combination of RF_ReadReg() and  RFLSSIRead()
 *
 * Note:		  For RF8256 only
 *			 The total count of RTL8256(Zebra4) register is around 36 bit it only employs
 *			 4-bit RF address. RTL8256 uses "register mode control bit" (Reg00[12], Reg00[10])
 *			 to access register address bigger than 0xf. See "Appendix-4 in PHY Configuration
 *			 programming guide" for more details.
 *			 Thus, we define a sub-finction for RTL8526 register address conversion
 *		       ===========================================================
 *			 Register Mode		RegCTL[1]		RegCTL[0]		Note
 *								(Reg00[12])		(Reg00[10])
 *		       ===========================================================
 *			 Reg_Mode0				0				x			Reg 0 ~15(0x0 ~ 0xf)
 *		       ------------------------------------------------------------------
 *			 Reg_Mode1				1				0			Reg 16 ~30(0x1 ~ 0xf)
 *		       ------------------------------------------------------------------
 *			 Reg_Mode2				1				1			Reg 31 ~ 45(0x1 ~ 0xf)
 *		       ------------------------------------------------------------------
 *
 *	2008/09/02	MH	Add 92S RF definition
 *
 *
 *
*/
static	void
phy_RFSerialWrite_8192F(
		PADAPTER			Adapter,
		enum rf_path			eRFPath,
		u32				Offset,
		u32				Data
)
{
	u32						DataAndAddr = 0;
	HAL_DATA_TYPE				*pHalData = GET_HAL_DATA(Adapter);
	BB_REGISTER_DEFINITION_T	*pPhyReg = &pHalData->PHYRegDef[eRFPath];
	u32						NewOffset;

	Offset &= 0xff;

	/* */
	/* Shadow Update */
	/* */
	/* PHY_RFShadowWrite(Adapter, eRFPath, Offset, Data); */

	/* */
	/* Switch page for 8256 RF IC */
	/* */
	NewOffset = Offset;

	/* */
	/* Put write addr in [5:0]  and write data in [31:16] */
	/* */
	/* DataAndAddr = (Data<<16) | (NewOffset&0x3f); */
	DataAndAddr = ((NewOffset << 20) | (Data & 0x000fffff)) & 0x0fffffff;	/* T65 RF */

	/* */
	/* Write Operation */
	/* */
	phy_set_bb_reg(Adapter, pPhyReg->rf3wireOffset, bMaskDWord, DataAndAddr);
	/* RTPRINT(FPHY, PHY_RFW, ("RFW-%d Addr[0x%lx]=0x%lx\n", eRFPath, pPhyReg->rf3wireOffset, DataAndAddr)); */

}


/**
* Function:	PHY_QueryRFReg
*
* OverView:	Query "Specific bits" to RF register (page 8~)
*
* Input:
*			PADAPTER		Adapter,
			enum rf_path			eRFPath,
*			u32			RegAddr,
*			u32			BitMask
*
*
* Output:	None
* Return:		u32			Readback value
* Note:		This function is equal to "GetRFRegSetting" in PHY programming guide
*/
u32
PHY_QueryRFReg_8192F(
		PADAPTER			Adapter,
		enum rf_path			eRFPath,
		u32				RegAddr,
		u32				BitMask
)
{
	u32 Original_Value, Readback_Value, BitShift;

#if (DISABLE_BB_RF == 1)
	return 0;
#endif

	Original_Value = phy_RFSerialRead_8192F(Adapter, eRFPath, RegAddr);

	BitShift =  phy_CalculateBitShift(BitMask);
	Readback_Value = (Original_Value & BitMask) >> BitShift;

	return Readback_Value;
}

/**
* Function:	PHY_SetRFReg
*
* OverView:	Write "Specific bits" to RF register (page 8~)
*
* Input:
*			PADAPTER		Adapter,
*			RF_PATH			eRFPath,
*			u32			RegAddr,
*			u32			BitMask
*
*			u32			Data
*
*
* Output:	None
* Return:		None
* Note:		This function is equal to "PutRFRegSetting" in PHY programming guide
*/
void
PHY_SetRFReg_8192F(
		PADAPTER			Adapter,
		enum rf_path			eRFPath,
		u32				RegAddr,
		u32				BitMask,
		u32				Data
)
{
	u32		Original_Value, BitShift;

#if (DISABLE_BB_RF == 1)
	return;
#endif

	/* RF data is 12 bits only */
	if (BitMask != bRFRegOffsetMask) {
		Original_Value = phy_RFSerialRead_8192F(Adapter, eRFPath, RegAddr);
		BitShift =  phy_CalculateBitShift(BitMask);
		Data = ((Original_Value & (~BitMask)) | (Data << BitShift));
	}

	phy_RFSerialWrite_8192F(Adapter, eRFPath, RegAddr, Data);
}


/*
 * 3. Initial MAC/BB/RF config by reading MAC/BB/RF txt.
 *   */


/*-----------------------------------------------------------------------------
 * Function:    PHY_MACConfig8192C
 *
 * Overview:	Condig MAC by header file or parameter file.
 *
 * Input:       NONE
 *
 * Output:      NONE
 *
 * Return:      NONE
 *
 * Revised History:
 *  When		Who		Remark
 *  08/12/2008	MHC		Create Version 0.
 *
 *---------------------------------------------------------------------------*/
s32 PHY_MACConfig8192F(PADAPTER Adapter)
{
	int		rtStatus = _SUCCESS;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);

	/* */
	/* Config MAC */
	/* */
#ifdef CONFIG_LOAD_PHY_PARA_FROM_FILE
	rtStatus = phy_ConfigMACWithParaFile(Adapter, PHY_FILE_MAC_REG);
	if (rtStatus == _FAIL)
#endif
	{
#ifdef CONFIG_EMBEDDED_FWIMG
		odm_config_mac_with_header_file(&pHalData->odmpriv);
		rtStatus = _SUCCESS;
#endif/* CONFIG_EMBEDDED_FWIMG */
	}

	return rtStatus;
}

/**
* Function:	phy_InitBBRFRegisterDefinition
*
* OverView:	Initialize Register definition offset for Radio Path A/B/C/D
*
* Input:
*			PADAPTER		Adapter,
*
* Output:	None
* Return:		None
* Note:		The initialization value is constant and it should never be changes
*/
static	void
phy_InitBBRFRegisterDefinition(
		PADAPTER		Adapter
)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(Adapter);

	/* RF Interface Sowrtware Control */
	pHalData->PHYRegDef[RF_PATH_A].rfintfs = rFPGA0_XAB_RFInterfaceSW; /* 16 LSBs if read 32-bit from 0x870 */
	pHalData->PHYRegDef[RF_PATH_B].rfintfs = rFPGA0_XAB_RFInterfaceSW; /* 16 MSBs if read 32-bit from 0x870 (16-bit for 0x872) */

	/* RF Interface Output (and Enable) */
	pHalData->PHYRegDef[RF_PATH_A].rfintfo = rFPGA0_XA_RFInterfaceOE; /* 16 LSBs if read 32-bit from 0x860 */
	pHalData->PHYRegDef[RF_PATH_B].rfintfo = rFPGA0_XB_RFInterfaceOE; /* 16 LSBs if read 32-bit from 0x864 */

	/* RF Interface (Output and)  Enable */
	pHalData->PHYRegDef[RF_PATH_A].rfintfe = rFPGA0_XA_RFInterfaceOE; /* 16 MSBs if read 32-bit from 0x860 (16-bit for 0x862) */
	pHalData->PHYRegDef[RF_PATH_B].rfintfe = rFPGA0_XB_RFInterfaceOE; /* 16 MSBs if read 32-bit from 0x864 (16-bit for 0x866) */

	pHalData->PHYRegDef[RF_PATH_A].rf3wireOffset = rFPGA0_XA_LSSIParameter; /* LSSI Parameter */
	pHalData->PHYRegDef[RF_PATH_B].rf3wireOffset = rFPGA0_XB_LSSIParameter;

	pHalData->PHYRegDef[RF_PATH_A].rfHSSIPara2 = rFPGA0_XA_HSSIParameter2;  /* wire control parameter2 */
	pHalData->PHYRegDef[RF_PATH_B].rfHSSIPara2 = rFPGA0_XB_HSSIParameter2;  /* wire control parameter2 */

	/* Tranceiver Readback LSSI/HSPI mode */
	pHalData->PHYRegDef[RF_PATH_A].rfLSSIReadBack = rFPGA0_XA_LSSIReadBack;
	pHalData->PHYRegDef[RF_PATH_B].rfLSSIReadBack = rFPGA0_XB_LSSIReadBack;
	pHalData->PHYRegDef[RF_PATH_A].rfLSSIReadBackPi = TransceiverA_HSPI_Readback;
	pHalData->PHYRegDef[RF_PATH_B].rfLSSIReadBackPi = TransceiverB_HSPI_Readback;

}

static	int
phy_BB8192f_Config_ParaFile(
		PADAPTER	Adapter
)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(Adapter);
	int			rtStatus = _SUCCESS;

	/* */
	/* 1. Read PHY_REG.TXT BB INIT!! */
	/* */
#ifdef CONFIG_LOAD_PHY_PARA_FROM_FILE
	if (phy_ConfigBBWithParaFile(Adapter, PHY_FILE_PHY_REG, CONFIG_BB_PHY_REG) == _FAIL)
#endif
	{
#ifdef CONFIG_EMBEDDED_FWIMG
		if (HAL_STATUS_SUCCESS != odm_config_bb_with_header_file(&pHalData->odmpriv, CONFIG_BB_PHY_REG))
			rtStatus = _FAIL;
#endif
	}

	if (rtStatus != _SUCCESS) {
		RTW_INFO("%s():Write BB Reg Fail!!", __func__);
		goto phy_BB8190_Config_ParaFile_Fail;
	}

	/* */
	/* 2. Read BB AGC table Initialization */
	/* */
#ifdef CONFIG_LOAD_PHY_PARA_FROM_FILE
	if (phy_ConfigBBWithParaFile(Adapter, PHY_FILE_AGC_TAB, CONFIG_BB_AGC_TAB) == _FAIL)
#endif
	{
#ifdef CONFIG_EMBEDDED_FWIMG
		if (HAL_STATUS_SUCCESS != odm_config_bb_with_header_file(&pHalData->odmpriv, CONFIG_BB_AGC_TAB))
			rtStatus = _FAIL;
#endif
	}

	if (rtStatus != _SUCCESS) {
		RTW_INFO("%s():AGC Table Fail\n", __func__);
		goto phy_BB8190_Config_ParaFile_Fail;
	}

phy_BB8190_Config_ParaFile_Fail:

	return rtStatus;
}


int
PHY_BBConfig8192F(
		PADAPTER Adapter
)
{
	int	rtStatus = _SUCCESS;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	u32	RegVal;
	u8	TmpU1B = 0;
	u8	value8;

	phy_InitBBRFRegisterDefinition(Adapter);

	/* Enable BB and RF */
	RegVal = rtw_read16(Adapter, REG_SYS_FUNC_EN);
	rtw_write16(Adapter, REG_SYS_FUNC_EN, (u16)(RegVal | FEN_BBRSTB | FEN_BB_GLB_RSTn));

	rtw_write8(Adapter, REG_RF_CTRL, RF_EN | RF_RSTB | RF_SDMRSTB);

#ifdef CONFIG_USB_HCI
	/* To Fix MAC loopback mode fail. Suggested by SD4 Johnny. 2010.03.23. */
	PlatformEFIOWrite1Byte(Adapter, REG_LDOHCI12_CTRL, 0x0f);
	PlatformEFIOWrite1Byte(Adapter, 0x15, 0xe9);
#endif

	/*rtw_write8(Adapter, REG_AFE_XTAL_CTRL + 1, 0x80);*//*92F NO NEED*/

	/*
	 * Config BB and AGC
	 */
	rtStatus = phy_BB8192f_Config_ParaFile(Adapter);

	if (rtw_phydm_set_crystal_cap(Adapter, pHalData->crystal_cap) == _FALSE) {
		RTW_ERR("Init crystal_cap failed\n");
		rtw_warn_on(1);
		rtStatus = _FAIL;
	}

	return rtStatus;
}
#if 0
/* Block & Path enable */
#define		rOFDMCCKEN_Jaguar		0x808 /* OFDM/CCK block enable */
#define		bOFDMEN_Jaguar			0x20000000
#define		bCCKEN_Jaguar			0x10000000
#define		rRxPath_Jaguar			0x808	/* Rx antenna */
#define		bRxPath_Jaguar			0xff
#define		rTxPath_Jaguar			0x80c	/* Tx antenna */
#define		bTxPath_Jaguar			0x0fffffff
#define		rCCK_RX_Jaguar			0xa04	/* for cck rx path selection */
#define		bCCK_RX_Jaguar			0x0c000000
#define		rVhtlen_Use_Lsig_Jaguar	0x8c3	/* Use LSIG for VHT length */
void
PHY_BB8723D_Config_1T(
		PADAPTER Adapter
)
{
	/* BB OFDM RX Path_A */
	phy_set_bb_reg(Adapter, rRxPath_Jaguar, bRxPath_Jaguar, 0x11);
	/* BB OFDM TX Path_A */
	phy_set_bb_reg(Adapter, rTxPath_Jaguar, bMaskLWord, 0x1111);
	/* BB CCK R/Rx Path_A */
	phy_set_bb_reg(Adapter, rCCK_RX_Jaguar, bCCK_RX_Jaguar, 0x0);
	/* MCS support */
	phy_set_bb_reg(Adapter, 0x8bc, 0xc0000060, 0x4);
	/* RF Path_B HSSI OFF */
	phy_set_bb_reg(Adapter, 0xe00, 0xf, 0x4);
	/* RF Path_B Power Down */
	phy_set_bb_reg(Adapter, 0xe90, bMaskDWord, 0);
	/* ADDA Path_B OFF */
	phy_set_bb_reg(Adapter, 0xe60, bMaskDWord, 0);
	phy_set_bb_reg(Adapter, 0xe64, bMaskDWord, 0);
}
#endif

int
PHY_RFConfig8192F(
		PADAPTER	Adapter
)
{
	int		rtStatus = _SUCCESS;

	/* */
	/* RF config */
	/* */
	rtStatus = PHY_RF6052_Config8192F(Adapter);
	/* 20151207 LCK done at RadioA table */
	/* PHY_BB8723D_Config_1T(Adapter); */

	return rtStatus;
}

/*-----------------------------------------------------------------------------
 * Function:    PHY_ConfigRFWithParaFile()
 *
 * Overview:    This function read RF parameters from general file format, and do RF 3-wire
 *
 * Input:	PADAPTER			Adapter
 *			ps1Byte				pFileName
 *			enum rf_path				eRFPath
 *
 * Output:      NONE
 *
 * Return:      RT_STATUS_SUCCESS: configuration file exist
 *
 * Note:		Delay may be required for RF configuration
 *---------------------------------------------------------------------------*/
int
PHY_ConfigRFWithParaFile_8192F(
		PADAPTER			Adapter,
		u8					*pFileName,
	enum rf_path				eRFPath
)
{
	return _SUCCESS;
}

/**************************************************************************************************************
 *   Description:
 *       The low-level interface to set TxAGC , called by both MP and Normal Driver.
 *
 *                                                                                    <20120830, Kordan>
 **************************************************************************************************************/

void
PHY_SetTxPowerIndex_8192F(
		PADAPTER			Adapter,
		u32					PowerIndex,
		enum rf_path			RFPath,
		u8					Rate
)
{
	u8 HwRate =  MRateToHwRate(Rate);

	if ((RFPath == RF_PATH_A) || (RFPath == RF_PATH_B)) {
			if (config_phydm_write_txagc_8192f(GET_PDM_ODM(Adapter), PowerIndex, (enum rf_path) RFPath, HwRate) == FALSE)
						RTW_INFO ("%s(), Fail to set TX AGC\n", __func__);
	} else
			RTW_INFO("%s, Invalid RFPath!!\n", __func__);

}

void
PHY_SetTxPowerLevel8192F(
		PADAPTER		Adapter,
		u8				Channel
)
{
	struct hal_spec_t *hal_spec = GET_HAL_SPEC(Adapter);
	enum rf_path		RFPath = RF_PATH_A;

	for (RFPath = RF_PATH_A; RFPath < hal_spec->rf_reg_path_num; ++RFPath)
		if (GET_HAL_TX_PATH_BMP(Adapter) & BIT(RFPath))
			phy_set_tx_power_level_by_path(Adapter, Channel, RFPath);
}

/* <20160217, Jessica> A workaround to eliminate the 2472MHz & 2484MHz spur of 8723D. */
void
phy_SpurCalibration_8192F(
		PADAPTER					pAdapter,
		u8						ToChannel,
		u8						threshold
)
{
	u32		freq[2] = {0xFCCD, 0xFF9A}; /* {chnl 13, 14} */
	u8		idx = 0xFF;
	u8		b_doNotch = FALSE;
	u8		initial_gain;

	/* add for notch */
	u32				wlan_channel, CurrentChannel;
	HAL_DATA_TYPE		*pHalData	= GET_HAL_DATA(pAdapter);
	struct dm_struct		*pDM_Odm = &(pHalData->odmpriv);

	/* check threshold */
	if (threshold <= 0x0)
		threshold = 0x16;

	RTW_DBG("===>phy_SpurCalibration_8192F: Channel = %d\n", ToChannel);

	if (ToChannel == 13)
		idx = 0;
	else if (ToChannel == 14)
		idx = 1;

	/* If current channel=13,14 */
	if (idx < 0xFF) {
		initial_gain = (u8)(odm_get_bb_reg(pDM_Odm, rOFDM0_XAAGCCore1, bMaskByte0) & 0x7f);
		odm_pause_dig(pDM_Odm, PHYDM_PAUSE, PHYDM_PAUSE_LEVEL_1, 0x30);
		phy_set_bb_reg(pAdapter, rFPGA0_AnalogParameter4, bMaskDWord, 0xccf000c0);		/* disable 3-wire */

		phy_set_bb_reg(pAdapter, rFPGA0_PSDFunction, bMaskDWord, freq[idx]);				/* Setup PSD */
		phy_set_bb_reg(pAdapter, rFPGA0_PSDFunction, bMaskDWord, 0x400000 | freq[idx]); /* Start PSD	 */

		rtw_msleep_os(30);

		if (phy_query_bb_reg(pAdapter, rFPGA0_PSDReport, bMaskDWord) >= threshold)
			b_doNotch = TRUE;

		phy_set_bb_reg(pAdapter, rFPGA0_PSDFunction, bMaskDWord, freq[idx]); /* turn off PSD */
		phy_set_bb_reg(pAdapter, rFPGA0_AnalogParameter4, bMaskDWord, 0xccc000c0);	/* enable 3-wire */
		odm_pause_dig(pDM_Odm, PHYDM_RESUME, PHYDM_PAUSE_LEVEL_1, NONE);
	}

	/* --- Notch Filter --- Asked by Rock	 */
	if (b_doNotch) {
		CurrentChannel = odm_get_rf_reg(pDM_Odm, RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask);
		wlan_channel   = CurrentChannel & 0x0f;						    /* Get center frequency */

		switch (wlan_channel) {											/* Set notch filter				 */
		case 13:
			odm_set_bb_reg(pDM_Odm, 0xC40, BIT(28) | BIT(27) | BIT(26) | BIT(25) | BIT(24), 0xB);
			odm_set_bb_reg(pDM_Odm, 0xC40, BIT(9), 0x1);                    /* enable notch filter */
			odm_set_bb_reg(pDM_Odm, 0xD40, bMaskDWord, 0x04000000);
			odm_set_bb_reg(pDM_Odm, 0xD44, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD48, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD4C, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD2C, BIT(28), 0x1);                   /* enable CSI mask */
			break;
		case 14:
			odm_set_bb_reg(pDM_Odm, 0xC40, BIT(28) | BIT(27) | BIT(26) | BIT(25) | BIT(24), 0x5);
			odm_set_bb_reg(pDM_Odm, 0xC40, BIT(9), 0x1);                   /* enable notch filter */
			odm_set_bb_reg(pDM_Odm, 0xD40, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD44, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD48, bMaskDWord, 0x00000000);
			odm_set_bb_reg(pDM_Odm, 0xD4C, bMaskDWord, 0x00080000);
			odm_set_bb_reg(pDM_Odm, 0xD2C, BIT(28), 0x1);                   /* enable CSI mask */
			break;
		default:
			odm_set_bb_reg(pDM_Odm, 0xC40, BIT(9), 0x0);						/* disable notch filter */
			odm_set_bb_reg(pDM_Odm, 0xD2C, BIT(28), 0x0);                   /* disable CSI mask	function */
			break;
		} /* switch(wlan_channel)	 */
		return;
	}

	odm_set_bb_reg(pDM_Odm, 0xC40, BIT(28) | BIT(27) | BIT(26) | BIT(25) | BIT(24), 0x1f);
	odm_set_bb_reg(pDM_Odm, 0xC40, BIT(9), 0x0);                     /* disable notch filter */
	odm_set_bb_reg(pDM_Odm, 0xD40, bMaskDWord, 0x00000000);
	odm_set_bb_reg(pDM_Odm, 0xD44, bMaskDWord, 0x00000000);
	odm_set_bb_reg(pDM_Odm, 0xD48, bMaskDWord, 0x00000000);
	odm_set_bb_reg(pDM_Odm, 0xD4C, bMaskDWord, 0x00000000);
	odm_set_bb_reg(pDM_Odm, 0xD2C, BIT(28), 0x0);                    /* disable CSI mask */
}


static u8 phy_GetSecondaryChnl_8192F(PADAPTER adapter)
{
	u8 pri_ch_idx = 0;
	PHAL_DATA_TYPE hal = GET_HAL_DATA(adapter);

	if (hal->current_channel_bw == CHANNEL_WIDTH_80) {
		/* primary channel is at lower subband of 80MHz & 40MHz */
		if ((hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER) && (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER))
			pri_ch_idx = VHT_DATA_SC_20_LOWEST_OF_80MHZ;
		/* primary channel is at lower subband of 80MHz & upper subband of 40MHz */
		else if ((hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER) && (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER))
			pri_ch_idx = VHT_DATA_SC_20_LOWER_OF_80MHZ;
		/* primary channel is at upper subband of 80MHz & lower subband of 40MHz */
		else if ((hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER) && (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER))
			pri_ch_idx = VHT_DATA_SC_20_UPPER_OF_80MHZ;
		/* primary channel is at upper subband of 80MHz & upper subband of 40MHz */
		else if ((hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER) && (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER))
			pri_ch_idx = VHT_DATA_SC_20_UPPERST_OF_80MHZ;
		else {
			if (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER)
				pri_ch_idx = VHT_DATA_SC_40_LOWER_OF_80MHZ;
			else if (hal->nCur80MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER)
				pri_ch_idx = VHT_DATA_SC_40_UPPER_OF_80MHZ;
			else
				RTW_INFO("SCMapping: DONOT CARE Mode Setting\n");
		}
	} else if (hal->current_channel_bw == CHANNEL_WIDTH_40) {
		/* primary channel is at upper subband of 40MHz */
		if (hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_UPPER)
			pri_ch_idx = VHT_DATA_SC_20_UPPER_OF_80MHZ;
		/* primary channel is at lower subband of 40MHz */
		else if (hal->nCur40MhzPrimeSC == HAL_PRIME_CHNL_OFFSET_LOWER)
			pri_ch_idx = VHT_DATA_SC_20_LOWER_OF_80MHZ;
		else
			RTW_INFO("SCMapping: DONOT CARE Mode Setting\n");
	}

	return  pri_ch_idx;
}


void
phy_SwChnlAndSetBwMode8192F(
		PADAPTER		Adapter
)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(Adapter);
		struct dm_struct *p_dm_odm = &pHalData->odmpriv;
	u8 center_ch = 0, ret = 0;

	if (Adapter->bNotifyChannelChange) {
		RTW_INFO("[%s] bSwChnl=%d, ch=%d, bSetChnlBW=%d, bw=%d\n",
			 __func__,
			 pHalData->bSwChnl,
			 pHalData->current_channel,
			 pHalData->bSetChnlBW,
			 pHalData->current_channel_bw);
	}

	if (RTW_CANNOT_RUN(Adapter))
		return;

	/* set channel & Bandwidth register */
	/* 1. set channel register */
	if (pHalData->bSwChnl) {
		ret = config_phydm_switch_channel_8192f(p_dm_odm, pHalData->current_channel);
		pHalData->bSwChnl = _FALSE;

		if (!ret) {
			RTW_INFO("%s: config_phydm_switch_channel_8192f fail\n", __FUNCTION__);
			rtw_warn_on(1);
			return;
		}
	}
	phydm_config_kfree(p_dm_odm, pHalData->current_channel);

	if (pHalData->bSetChnlBW) {
		/* get primary channel index */
		u8 pri_ch_idx = phy_GetSecondaryChnl_8192F(Adapter);
		rtw_write8(Adapter, REG_DATA_SC_8192F, pri_ch_idx);

#ifdef CONFIG_NARROWBAND_SUPPORTING
		if (Adapter->registrypriv.rtw_nb_config == RTW_NB_CONFIG_WIDTH_10) {
			rtw_write8(Adapter, REG_CCK_CHECK_8192F,
				(rtw_read8(Adapter, REG_CCK_CHECK_8192F) | BIT(7)));
			ret = config_phydm_switch_bandwidth_8192f(p_dm_odm, pri_ch_idx, CHANNEL_WIDTH_10);
		} else if (Adapter->registrypriv.rtw_nb_config == RTW_NB_CONFIG_WIDTH_5) {
			rtw_write8(Adapter, REG_CCK_CHECK_8192F,
				(rtw_read8(Adapter, REG_CCK_CHECK_8192F) | BIT(7)));
			ret = config_phydm_switch_bandwidth_8192f(p_dm_odm, pri_ch_idx, CHANNEL_WIDTH_5);
		} else
#endif
		{
		ret = config_phydm_switch_bandwidth_8192f(p_dm_odm, pri_ch_idx, pHalData->current_channel_bw);
		}
		pHalData->bSetChnlBW = _FALSE;
		if (!ret) {
			RTW_INFO("%s: config_phydm_switch_bandwidth_8192f fail\n", __FUNCTION__);
			rtw_warn_on(1);
			return;
		}
	}
	/* TX Power Setting */
	odm_clear_txpowertracking_state(p_dm_odm);
	rtw_hal_set_tx_power_level(Adapter, pHalData->current_channel);
	/* IQK */
	if (pHalData->bNeedIQK == _TRUE) {
		if (pHalData->neediqk_24g == _TRUE) {

			halrf_iqk_trigger(&pHalData->odmpriv, _FALSE);
			pHalData->bIQKInitialized = _TRUE;
			pHalData->neediqk_24g = _FALSE;
		}
		pHalData->bNeedIQK = _FALSE;
	}
}

void
PHY_HandleSwChnlAndSetBW8192F(
	PADAPTER Adapter, u8 bSwitchChannel, u8 bSetBandWidth,
	u8 ChannelNum, enum channel_width ChnlWidth, u8 ChnlOffsetOf40MHz,
	u8 ChnlOffsetOf80MHz, u8 CenterFrequencyIndex1)
{
	PADAPTER pDefAdapter = GetDefaultAdapter(Adapter);
	PHAL_DATA_TYPE hal = GET_HAL_DATA(pDefAdapter);
	u8 tmpChannel = hal->current_channel;
	enum channel_width tmpBW = hal->current_channel_bw;
	u8 tmpnCur40MhzPrimeSC = hal->nCur40MhzPrimeSC;
	u8 tmpnCur80MhzPrimeSC = hal->nCur80MhzPrimeSC;
	u8 tmpCenterFrequencyIndex1 = hal->CurrentCenterFrequencyIndex1;
	struct mlme_ext_priv *pmlmeext = &Adapter->mlmeextpriv;


	/* check swchnl or setbw */
	if (!bSwitchChannel && !bSetBandWidth) {
		RTW_INFO("%s: not switch channel and not set bandwidth\n", __FUNCTION__);
		return;
	}

	/* skip switch channel operation for current channel & ChannelNum(will be switch) are the same */
	if (bSwitchChannel) {
		if (hal->current_channel != ChannelNum) {
			if (HAL_IsLegalChannel(Adapter, ChannelNum))
				hal->bSwChnl = _TRUE;
			else
				return;
		}
	}

	/* check set BandWidth */
	if (bSetBandWidth) {
		/* initial channel bw setting */
		if (hal->bChnlBWInitialized == _FALSE) {
			hal->bChnlBWInitialized = _TRUE;
			hal->bSetChnlBW = _TRUE;
		} else if ((hal->current_channel_bw != ChnlWidth) || /* check whether need set band or not */
			   (hal->nCur40MhzPrimeSC != ChnlOffsetOf40MHz) ||
			(hal->CurrentCenterFrequencyIndex1 != CenterFrequencyIndex1))
			hal->bSetChnlBW = _TRUE;
	}

	/* return if not need set bandwidth nor channel after check*/
	if (!hal->bSetChnlBW && !hal->bSwChnl && hal->bNeedIQK != _TRUE)
		return;

	/* set channel number to hal data */
	if (hal->bSwChnl) {
		hal->current_channel = ChannelNum;
		hal->CurrentCenterFrequencyIndex1 = ChannelNum;
	}

	/* set bandwidth info to hal data */
	if (hal->bSetChnlBW) {
		hal->current_channel_bw = ChnlWidth;
		hal->nCur40MhzPrimeSC = ChnlOffsetOf40MHz;
		hal->CurrentCenterFrequencyIndex1 = CenterFrequencyIndex1;
	}

	/* switch channel & bandwidth */
	if (!RTW_CANNOT_RUN(Adapter))
		phy_SwChnlAndSetBwMode8192F(Adapter);
	else {
		if (hal->bSwChnl) {
			hal->current_channel = tmpChannel;
			hal->CurrentCenterFrequencyIndex1 = tmpChannel;
		}

		if (hal->bSetChnlBW) {
			hal->current_channel_bw = tmpBW;
			hal->nCur40MhzPrimeSC = tmpnCur40MhzPrimeSC;
			hal->CurrentCenterFrequencyIndex1 = tmpCenterFrequencyIndex1;
		}
	}
}

void
PHY_SetSwChnlBWMode8192F(PADAPTER adapter, u8 center_ch, enum channel_width bw, u8 offset40, u8 offset80)
{
	/* RTW_INFO("%s()===>\n",__FUNCTION__); */

		PHY_HandleSwChnlAndSetBW8192F(adapter, _TRUE, _TRUE, center_ch, bw, offset40, offset80, center_ch);

	/* RTW_INFO("<==%s()\n",__FUNCTION__); */
}
