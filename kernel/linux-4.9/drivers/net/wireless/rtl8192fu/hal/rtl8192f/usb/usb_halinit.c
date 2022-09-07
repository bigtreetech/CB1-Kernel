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

#define _USB_HALINIT_C_

#include <rtl8192f_hal.h>
#ifdef CONFIG_WOWLAN
	#include "hal_com_h2c.h"
#endif


static void
_ConfigChipOutEP_8192F(
	PADAPTER padapter,
	u8 NumOutPipe
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	pHalData->OutEpQueueSel = 0;
	pHalData->OutEpNumber = 0;

	switch (NumOutPipe) {
	case 6:
		pHalData->OutEpQueueSel = TX_SELE_HQ | TX_SELE_LQ | TX_SELE_NQ;
		pHalData->OutEpNumber = 6;
		break;	
	case 5:
		pHalData->OutEpQueueSel = TX_SELE_HQ | TX_SELE_LQ | TX_SELE_NQ;
		pHalData->OutEpNumber = 5;
		break;		
	case 4:
		pHalData->OutEpQueueSel = TX_SELE_HQ | TX_SELE_LQ | TX_SELE_NQ;
		pHalData->OutEpNumber = 4;
		break;
	case 3:
		pHalData->OutEpQueueSel = TX_SELE_HQ | TX_SELE_LQ | TX_SELE_NQ;
		pHalData->OutEpNumber = 3;
		break;
	case 2:
		pHalData->OutEpQueueSel = TX_SELE_HQ | TX_SELE_NQ;
		pHalData->OutEpNumber = 2;
		break;
	case 1:
		pHalData->OutEpQueueSel = TX_SELE_HQ;
		pHalData->OutEpNumber = 1;
		break;
	default:
		break;
	}
}

static BOOLEAN HalUsbSetQueuePipeMapping8192FUsb(
	PADAPTER padapter,
	u8 NumInPipe,
	u8 NumOutPipe
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	BOOLEAN result = _FALSE;

	_ConfigChipOutEP_8192F(padapter, NumOutPipe);

	result = Hal_MappingOutPipe(padapter, NumOutPipe);

	RTW_INFO("USB NumInPipe(%u), NumOutPipe(%u/%u)\n"
		 , NumInPipe
		 , pHalData->OutEpNumber
		 , NumOutPipe);

	return result;
}

void rtl8192fu_interface_configure(
	PADAPTER padapter
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(padapter);

	if (IS_HIGH_SPEED_USB(padapter)) {
		/* HIGH SPEED */
		pHalData->UsbBulkOutSize = USB_HIGH_SPEED_BULK_SIZE; /* 512 bytes */
	} else {
		/* FULL SPEED */
		pHalData->UsbBulkOutSize = USB_FULL_SPEED_BULK_SIZE; /* 64 bytes */
	}

#ifdef CONFIG_USB_TX_AGGREGATION
	pHalData->UsbTxAggMode = 1;
	pHalData->UsbTxAggDescNum = 0x6; /* only 4 bits */
#endif

#ifdef CONFIG_USB_RX_AGGREGATION
	pHalData->rxagg_mode = RX_AGG_USB;
	pHalData->rxagg_usb_size = 0x5; /* unit: 4KB, for USB mode */
	pHalData->rxagg_usb_timeout = 0x20; /* unit: 32us, for USB mode */
	pHalData->rxagg_dma_size = 0xF; /* uint: 1KB, for DMA mode */
	pHalData->rxagg_dma_timeout = 0x20; /* unit: 32us, for DMA mode */
#endif

	HalUsbSetQueuePipeMapping8192FUsb(padapter,
			  pdvobjpriv->RtNumInPipes, pdvobjpriv->RtNumOutPipes);
}

#ifdef CONFIG_GPIO_WAKEUP
/*
 * we set it high under init and fw will
 * give us Low Pulse when host wake up
 */
void HostWakeUpGpioClear(PADAPTER padapter)
{
	u32 value32;

	value32 = rtw_read32(padapter, REG_GPIO_PIN_CTRL_2);

	/* set GPIO 12 1 */
	value32 |= BIT(12); /*4+8 */
	/* GPIO 12 out put */
	value32 |= BIT(20); /*4+16 */

	rtw_write32(padapter, REG_GPIO_PIN_CTRL_2, value32);
} /* HostWakeUpGpioClear */

void HalSetOutPutGPIO(PADAPTER padapter, u8 index, u8 OutPutValue)
{
	if (index <= 7) {
		/* config GPIO mode */
		rtw_write8(padapter, REG_GPIO_PIN_CTRL + 3, rtw_read8(padapter, REG_GPIO_PIN_CTRL + 3) & ~BIT(index));

		/* config GPIO Sel */
		/* 0: input */
		/* 1: output */
		rtw_write8(padapter, REG_GPIO_PIN_CTRL + 2, rtw_read8(padapter, REG_GPIO_PIN_CTRL + 2) | BIT(index));

		/* set output value */
		if (OutPutValue)
			rtw_write8(padapter, REG_GPIO_PIN_CTRL + 1, rtw_read8(padapter, REG_GPIO_PIN_CTRL + 1) | BIT(index));
		else
			rtw_write8(padapter, REG_GPIO_PIN_CTRL + 1, rtw_read8(padapter, REG_GPIO_PIN_CTRL + 1) & ~BIT(index));
	} else {
		/* 88C Series: */
		/* index: 11~8 transform to 3~0 */
		/* 8192 Series: */
		/* index: 12~8 transform to 4~0 */
		index -= 8;

		/* config GPIO mode */
		rtw_write8(padapter, REG_GPIO_PIN_CTRL_2 + 3, rtw_read8(padapter, REG_GPIO_PIN_CTRL_2 + 3) & ~BIT(index));

		/* config GPIO Sel */
		/* 0: input */
		/* 1: output */
		rtw_write8(padapter, REG_GPIO_PIN_CTRL_2 + 2, rtw_read8(padapter, REG_GPIO_PIN_CTRL_2 + 2) | BIT(index));

		/* set output value */
		if (OutPutValue)
			rtw_write8(padapter, REG_GPIO_PIN_CTRL_2 + 1, rtw_read8(padapter, REG_GPIO_PIN_CTRL_2 + 1) | BIT(index));
		else
			rtw_write8(padapter, REG_GPIO_PIN_CTRL_2 + 1, rtw_read8(padapter, REG_GPIO_PIN_CTRL_2 + 1) & ~BIT(index));
	}
}
#endif

static u32 _InitPowerOn_8192FU(PADAPTER padapter)
{
	u32 status = _SUCCESS;
	u32 value32 = 0;
	u16 value16 = 0;
	u8 value8 = 0;

	rtw_hal_get_hwreg(padapter, HW_VAR_APFM_ON_MAC, &value8);
	if (value8 == _TRUE)
		return _SUCCESS;
	/* HW Power on sequence */
	if (!HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, rtl8192F_card_enable_flow))
		return _FAIL;

	/* Enable MAC DMA/WMAC/SCHEDULE/SEC block */
	/* Set CR bit10 to enable 32k calibration. Suggested by SD1 Gimmy. Added by tynli. 2011.08.31. */
	rtw_write16(padapter, REG_CR, 0x00);  /* suggseted by zhouzhou, by page, 20111230 */

	value16 = rtw_read16(padapter, REG_CR);
	value16 |= (HCI_TXDMA_EN | HCI_RXDMA_EN | TXDMA_EN | RXDMA_EN
		    | PROTOCOL_EN | SCHEDULE_EN | ENSEC | CALTMR_EN);
	rtw_write16(padapter, REG_CR, value16);

	value8 = _TRUE;
	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &value8);

	return status;
}

/*
 * ---------------------------------------------------------------
 * MAC init functions
 * ---------------------------------------------------------------
 */

/*
 * USB has no hardware interrupt,
 * so no need to initialize HIMR.
 */
static void _InitInterrupt(PADAPTER padapter)
{
#ifdef CONFIG_SUPPORT_USB_INT
	/* clear interrupt, write 1 clear */
	rtw_write32(padapter, REG_HISR0_8192F, 0xFFFFFFFF);
	rtw_write32(padapter, REG_HISR1_8192F, 0xFFFFFFFF);
#endif /* CONFIG_SUPPORT_USB_INT */
}

static void _InitQueueReservedPage(PADAPTER padapter)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	u32 outEPNum = (u32)pHalData->OutEpNumber;
	u32 numHQ = 0;
	u32 numLQ = 0;
	u32 numNQ = 0;
	u32 numPubQ;
	u32 value32;
	u8 value8;
	BOOLEAN bWiFiConfig = pregistrypriv->wifi_spec;

	if (pHalData->OutEpQueueSel & TX_SELE_HQ)
		numHQ = bWiFiConfig ? WMM_NORMAL_PAGE_NUM_HPQ_8192F : NORMAL_PAGE_NUM_HPQ_8192F;

	if (pHalData->OutEpQueueSel & TX_SELE_LQ)
		numLQ = bWiFiConfig ? WMM_NORMAL_PAGE_NUM_LPQ_8192F : NORMAL_PAGE_NUM_LPQ_8192F;

	/* NOTE: This step shall be proceed before writing REG_RQPN. */
	if (pHalData->OutEpQueueSel & TX_SELE_NQ)
		numNQ = bWiFiConfig ? WMM_NORMAL_PAGE_NUM_NPQ_8192F : NORMAL_PAGE_NUM_NPQ_8192F;
	value8 = (u8)_NPQ(numNQ);
	rtw_write8(padapter, REG_RQPN_NPQ, value8);

	numPubQ = TX_TOTAL_PAGE_NUMBER_8192F - numHQ - numLQ - numNQ;

	/* TX DMA */
	value32 = _HPQ(numHQ) | _LPQ(numLQ) | _PUBQ(numPubQ) | LD_RQPN;
	rtw_write32(padapter, REG_RQPN, value32);

}

static void _InitTRxBufferBoundary(PADAPTER padapter)
{
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
#ifdef CONFIG_CONCURRENT_MODE
	u8 val8;
#endif /* CONFIG_CONCURRENT_MODE */

	/* u16	txdmactrl; */
	u8 txpktbuf_bndy;

	if (!pregistrypriv->wifi_spec)
		txpktbuf_bndy = TX_PAGE_BOUNDARY_8192F;
	else {
		/* for WMM */
		txpktbuf_bndy = WMM_NORMAL_TX_PAGE_BOUNDARY_8192F;
	}

	rtw_write8(padapter, REG_TXPKTBUF_BCNQ_BDNY_8192F, txpktbuf_bndy);
	rtw_write8(padapter, REG_TXPKTBUF_MGQ_BDNY_8192F, txpktbuf_bndy);
	rtw_write8(padapter, REG_TXPKTBUF_WMAC_LBK_BF_HD_8192F, txpktbuf_bndy);
	rtw_write8(padapter, REG_TRXFF_BNDY, txpktbuf_bndy);
	rtw_write8(padapter, REG_TDECTRL + 1, txpktbuf_bndy);

	/* RX Page Boundary */
	rtw_write16(padapter, REG_TRXFF_BNDY + 2, RX_DMA_BOUNDARY_8192F);

#ifdef CONFIG_CONCURRENT_MODE
	val8 = txpktbuf_bndy + 8;
	rtw_write8(padapter, REG_BCNQ1_BDNY, val8);
	rtw_write8(padapter, REG_DWBCN1_CTRL_8192F + 1, val8); /* BCN1_HEAD */

	val8 = rtw_read8(padapter, REG_DWBCN1_CTRL_8192F + 2);
	val8 |= BIT(1); /* BIT1- BIT_SW_BCN_SEL_EN */
	rtw_write8(padapter, REG_DWBCN1_CTRL_8192F + 2, val8);
#endif /* CONFIG_CONCURRENT_MODE */
}


void
_InitTransferPageSize_8192fu(
	PADAPTER padapter
)
{

	u8 value8;

	value8 = _PSRX(PBP_256) | _PSTX(PBP_256);

	rtw_write8(padapter, REG_PBP, value8);
}


static void
_InitNormalChipRegPriority(
	PADAPTER padapter,
	u16 beQ,
	u16 bkQ,
	u16 viQ,
	u16 voQ,
	u16 mgtQ,
	u16 hiQ
)
{
	u32 value32 = (rtw_read32(padapter, REG_TRXDMA_CTRL) & 0x7);

	value32 |= _TXDMA_BEQ_MAP_8192F(beQ) | _TXDMA_BKQ_MAP_8192F(bkQ) |
			   _TXDMA_VIQ_MAP_8192F(viQ) | _TXDMA_VOQ_MAP_8192F(voQ) |
			   _TXDMA_MGQ_MAP_8192F(mgtQ) | _TXDMA_HIQ_MAP_8192F(hiQ);

	rtw_write32(padapter, REG_TRXDMA_CTRL, value32);
}


static void
_InitNormalChipTwoOutEpPriority(
	PADAPTER padapter
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	u16 beQ, bkQ, viQ, voQ, mgtQ, hiQ;

	u16 valueHi = 0;
	u16 valueLow = 0;

	switch (pHalData->OutEpQueueSel) {
	case (TX_SELE_HQ | TX_SELE_LQ):
		valueHi = QUEUE_HIGH;
		valueLow = QUEUE_LOW;
		break;
	case (TX_SELE_NQ | TX_SELE_LQ):
		valueHi = QUEUE_NORMAL;
		valueLow = QUEUE_LOW;
		break;
	case (TX_SELE_HQ | TX_SELE_NQ):
		valueHi = QUEUE_HIGH;
		valueLow = QUEUE_NORMAL;
		break;
	default:
		/* RT_ASSERT(FALSE,("Shall not reach here!\n")); */
		break;
	}

	if (!pregistrypriv->wifi_spec) {
		beQ = valueLow;
		bkQ = valueLow;
		viQ = valueHi;
		voQ = valueHi;
		mgtQ = valueHi;
		hiQ = valueHi;
	} else { /* for WMM ,CONFIG_OUT_EP_WIFI_MODE */
		beQ = valueLow;
		bkQ = valueHi;
		viQ = valueHi;
		voQ = valueLow;
		mgtQ = valueHi;
		hiQ = valueHi;
	}

	_InitNormalChipRegPriority(padapter, beQ, bkQ, viQ, voQ, mgtQ, hiQ);

}

static void
_InitNormalChipThreeOutEpPriority(
	PADAPTER padapter
)
{
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	u16 beQ, bkQ, viQ, voQ, mgtQ, hiQ;

	if (!pregistrypriv->wifi_spec) { /* typical setting */
		beQ = QUEUE_LOW;
		bkQ = QUEUE_LOW;
		viQ = QUEUE_NORMAL;
		voQ = QUEUE_HIGH;
		mgtQ = QUEUE_HIGH;
		hiQ = QUEUE_HIGH;
	} else { /* for WMM */
		beQ = QUEUE_LOW;
		bkQ = QUEUE_NORMAL;
		viQ = QUEUE_NORMAL;
		voQ = QUEUE_HIGH;
		mgtQ = QUEUE_HIGH;
		hiQ = QUEUE_HIGH;
	}
	_InitNormalChipRegPriority(padapter, beQ, bkQ, viQ, voQ, mgtQ, hiQ);
}

static void _InitQueuePriority(PADAPTER padapter)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	switch (pHalData->OutEpNumber) {
	case 2:
		_InitNormalChipTwoOutEpPriority(padapter);
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		_InitNormalChipThreeOutEpPriority(padapter);
		break;
	default:
		/* RT_ASSERT(FALSE,("Shall not reach here!\n")); */
		break;
	}

}

static void
_InitHardwareDropIncorrectBulkOut(
	PADAPTER padapter
)
{
	u32 value32 = rtw_read32(padapter, REG_TXDMA_OFFSET_CHK);

	value32 |= DROP_DATA_EN;

	rtw_write32(padapter, REG_TXDMA_OFFSET_CHK, value32);
}

static void
_InitNetworkType(
	PADAPTER padapter
)
{
	u32 value32;

	value32 = rtw_read32(padapter, REG_CR);

	/* TODO: use the other function to set network type */
#if 0 /* RTL8191C_FPGA_NETWORKTYPE_ADHOC */
	value32 = (value32 & ~MASK_NETTYPE) | _NETTYPE(NT_LINK_AD_HOC);
#else
	value32 = (value32 & ~MASK_NETTYPE) | _NETTYPE(NT_LINK_AP);
#endif
	rtw_write32(padapter, REG_CR, value32);
	/* RASSERT(pIoBase->rtw_read8(REG_CR + 2) == 0x2); */
}


static void
_InitDriverInfoSize(
	PADAPTER padapter,
	u8 drvInfoSize
)
{
	u8 value8;

	/* BIT_DRVINFO_SZ [3:0] */
	value8 = rtw_read8(padapter, REG_RX_DRVINFO_SZ) & 0xF8;
	value8 |= drvInfoSize;
	rtw_write8(padapter, REG_RX_DRVINFO_SZ, value8);
}

static void
_InitWMACSetting(
	PADAPTER padapter
)
{
	u16 value16;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	u32 rcr;

	rcr = RCR_APM | RCR_AM | RCR_AB | RCR_CBSSID_DATA | RCR_CBSSID_BCN | RCR_APP_ICV | RCR_AMF | RCR_HTC_LOC_CTRL | RCR_APP_MIC | RCR_APP_PHYST_RXFF;
	rtw_hal_set_hwreg(padapter, HW_VAR_RCR, (u8 *)&rcr);

	/* Accept all data frames */
	value16 = 0xFFFF;
	rtw_write16(padapter, REG_RXFLTMAP2_8192F, value16);

	/* 2010.09.08 hpfan */
	/* Since ADF is removed from RCR, ps-poll will not be indicate to driver, */
	/* RxFilterMap should mask ps-poll to gurantee AP mode can rx ps-poll. */

	value16 = 0x400;
	rtw_write16(padapter, REG_RXFLTMAP1_8192F, value16);

	/* Accept all management frames */
	value16 = 0xFFFF;
	rtw_write16(padapter, REG_RXFLTMAP0_8192F, value16);

}

static void
_InitAdaptiveCtrl(
	PADAPTER padapter
)
{
	u16 value16;
	u32 value32;

	/* Response Rate Set */
	value32 = rtw_read32(padapter, REG_RRSR);
	value32 &= ~RATE_BITMAP_ALL;
	value32 |= RATE_RRSR_CCK_ONLY_1M;
	rtw_write32(padapter, REG_RRSR, value32);

	/* CF-END Threshold */
	/* m_spIoBase->rtw_write8(REG_CFEND_TH, 0x1); */

	/* SIFS (used in NAV) */
	value16 = _SPEC_SIFS_CCK(0x10) | _SPEC_SIFS_OFDM(0x10);
	rtw_write16(padapter, REG_SPEC_SIFS, value16);

	/* Retry Limit */
	value16 = BIT_LRL(RL_VAL_STA) | BIT_SRL(RL_VAL_STA);
	rtw_write16(padapter, REG_RETRY_LIMIT, value16);

}

static void
_InitEDCA(
	PADAPTER padapter
)
{
	/* Set Spec SIFS (used in NAV) */
	rtw_write16(padapter, REG_SPEC_SIFS, 0x100a);
	rtw_write16(padapter, REG_MAC_SPEC_SIFS, 0x100a);

	/* Set SIFS for CCK */
	rtw_write16(padapter, REG_SIFS_CTX, 0x100a);

	/* Set SIFS for OFDM */
	rtw_write16(padapter, REG_SIFS_TRX, 0x100a);

	/* TXOP */
	rtw_write32(padapter, REG_EDCA_BE_PARAM, 0x005EA42B);
	rtw_write32(padapter, REG_EDCA_BK_PARAM, 0x0000A44F);
	rtw_write32(padapter, REG_EDCA_VI_PARAM, 0x005EA324);
	rtw_write32(padapter, REG_EDCA_VO_PARAM, 0x002FA226);

}

#ifdef CONFIG_RTW_LED
static void _InitHWLed(PADAPTER padapter)
{
	struct led_priv *pledpriv = adapter_to_led(padapter);

	if (pledpriv->LedStrategy != HW_LED)
		return;

	/* HW led control */
	/* to do .... */
	/* must consider cases of antenna diversity/ commbo card/solo card/mini card */

}
#endif /* CONFIG_RTW_LED */

static void
_InitRDGSetting_8192fu(
	PADAPTER padapter
)
{
	rtw_write8(padapter, REG_RD_CTRL_8192F, 0xFF);
	rtw_write16(padapter, REG_RD_NAV_NXT_8192F, 0x200);
	rtw_write8(padapter, REG_RD_RESP_PKT_TH_8192F, 0x05);
}

static void
_InitRetryFunction(
	PADAPTER padapter
)
{
	u8 value8;

	value8 = rtw_read8(padapter, REG_FWHW_TXQ_CTRL);
	value8 |= EN_AMPDU_RTY_NEW;
	rtw_write8(padapter, REG_FWHW_TXQ_CTRL, value8);

	/* Set ACK timeout */
	rtw_write8(padapter, REG_ACKTO, 0x40);
}

static void _InitBurstPktLen(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);
	u8 tmp8;


	tmp8 = rtw_read8(padapter, REG_RXDMA_MODE_CTRL_8192F);
	tmp8 &= ~(BIT(4) | BIT(5));
	switch (pHalData->UsbBulkOutSize) {
	case USB_HIGH_SPEED_BULK_SIZE:
		tmp8 |= BIT(4); /* set burst pkt len=512B */
		break;
	case USB_FULL_SPEED_BULK_SIZE:
	default:
		tmp8 |= BIT(5); /* set burst pkt len=64B */
		break;
	}
	tmp8 |= BIT(1) | BIT(2) | BIT(3);
	rtw_write8(padapter, REG_RXDMA_MODE_CTRL_8192F, tmp8);

	pHalData->bSupportUSB3 = _FALSE;

	tmp8 = rtw_read8(padapter, REG_HT_SINGLE_AMPDU_8192F);
	tmp8 |= BIT(7); /* enable single pkt ampdu */
	rtw_write8(padapter, REG_HT_SINGLE_AMPDU_8192F, tmp8);
	rtw_write8(padapter, REG_MAX_AGGR_NUM, 0x1F);
	rtw_write8(padapter, REG_RTS_MAX_AGGR_NUM, 0x1F);
	rtw_write8(padapter, REG_AMPDU_MAX_TIME_8192F, 0x5E);
	rtw_write32(padapter, REG_AMPDU_MAX_LENGTH_8192F, 0xffffffff);
	if (pHalData->AMPDUBurstMode)
		rtw_write8(padapter, REG_AMPDU_BURST_MODE_8192F, 0x5F);

	/* for VHT packet length 11K */
	rtw_write8(padapter, REG_RX_PKT_LIMIT, 0x18);

	rtw_write8(padapter, REG_PIFS, 0x00);
	rtw_write8(padapter, REG_FWHW_TXQ_CTRL, 0x80);
	rtw_write32(padapter, REG_FAST_EDCA_CTRL, 0x03086666);

	/* to prevent mac is reseted by bus. 20111208, by Page */
	tmp8 = rtw_read8(padapter, REG_RSV_CTRL);
	tmp8 |= BIT(5) | BIT(6);
	rtw_write8(padapter, REG_RSV_CTRL, tmp8);
}

/*-----------------------------------------------------------------------------
 * Function:	usb_AggSettingTxUpdate()
 *
 * Overview:	Separate TX/RX parameters update independent for TP detection and
 *			dynamic TX/RX aggreagtion parameters update.
 *
 * Input:			PADAPTER
 *
 * Output/Return:	NONE
 *
 * Revised History:
 *	When		Who		Remark
 *	12/10/2010	MHC		Separate to smaller function.
 *
 *---------------------------------------------------------------------------*/
static void
usb_AggSettingTxUpdate(
	PADAPTER padapter
)
{
#ifdef CONFIG_USB_TX_AGGREGATION
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	u32 value32;

	if (padapter->registrypriv.wifi_spec)
		pHalData->UsbTxAggMode = _FALSE;

	if (pHalData->UsbTxAggMode) {
		value32 = rtw_read32(padapter, REG_DWBCN0_CTRL_8192F);
		value32 = value32 & ~(BLK_DESC_NUM_MASK << BLK_DESC_NUM_SHIFT);
		value32 |= ((pHalData->UsbTxAggDescNum & BLK_DESC_NUM_MASK) << BLK_DESC_NUM_SHIFT);

		rtw_write32(padapter, REG_DWBCN0_CTRL_8192F, value32);
		rtw_write8(padapter, REG_DWBCN1_CTRL_8192F, pHalData->UsbTxAggDescNum << 1);
	}
#endif
}   /* usb_AggSettingTxUpdate */


/*-----------------------------------------------------------------------------
 * Function:	usb_AggSettingRxUpdate()
 *
 * Overview:	Separate TX/RX parameters update independent for TP detection and
 *			dynamic TX/RX aggreagtion parameters update.
 *
 * Input:			PADAPTER
 *
 * Output/Return:	NONE
 *
 *---------------------------------------------------------------------------*/
static void
usb_AggSettingRxUpdate(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData;
	u8 aggctrl;
	u32 aggrx;
	u32 agg_size;

	pHalData = GET_HAL_DATA(padapter);

	aggctrl = rtw_read8(padapter, REG_TRXDMA_CTRL);
	aggctrl &= ~RXDMA_AGG_EN;

	aggrx = rtw_read32(padapter, REG_RXDMA_AGG_PG_TH);
	aggrx &= ~BIT_USB_RXDMA_AGG_EN;
	aggrx &= ~0xFF0F; /* reset agg size and timeout */

#ifdef CONFIG_USB_RX_AGGREGATION
	switch (pHalData->rxagg_mode) {
	case RX_AGG_DMA:
		agg_size = pHalData->rxagg_dma_size << 10;
		if (agg_size > RX_DMA_BOUNDARY_8192F)
			agg_size = RX_DMA_BOUNDARY_8192F >> 1;
		if ((agg_size + 2048) > MAX_RECVBUF_SZ)
			agg_size = MAX_RECVBUF_SZ - 2048;
		agg_size >>= 10; /* unit: 1K */
		if (agg_size > 0xF)
			agg_size = 0xF;

		aggctrl |= RXDMA_AGG_EN;
		aggrx |= BIT_USB_RXDMA_AGG_EN;
		aggrx |= agg_size;
		aggrx |= (pHalData->rxagg_dma_timeout << 8);
		RTW_INFO("%s: RX Agg-DMA mode, size=%dKB, timeout=%dus\n",
			__func__, agg_size, pHalData->rxagg_dma_timeout * 32);
		break;

	case RX_AGG_USB:
	case RX_AGG_MIX:
		agg_size = pHalData->rxagg_usb_size << 12;
		if ((agg_size + 2048) > MAX_RECVBUF_SZ)
			agg_size = MAX_RECVBUF_SZ - 2048;
		agg_size >>= 12; /* unit: 4K */
		if (agg_size > 0xF)
			agg_size = 0xF;

		aggctrl |= RXDMA_AGG_EN;
		aggrx &= ~BIT_USB_RXDMA_AGG_EN;
		aggrx |= agg_size;
		aggrx |= (pHalData->rxagg_usb_timeout << 8);
		RTW_INFO("%s: RX Agg-USB mode, size=%dKB, timeout=%dus\n",
				 __func__, agg_size * 4, pHalData->rxagg_usb_timeout * 32);
		break;

	case RX_AGG_DISABLE:
	default:
		RTW_INFO("%s: RX Aggregation Disable!\n", __func__);
		break;
	}
#endif /* CONFIG_USB_RX_AGGREGATION */

	rtw_write8(padapter, REG_TRXDMA_CTRL, aggctrl);
	rtw_write32(padapter, REG_RXDMA_AGG_PG_TH, aggrx);
}

static void
_initUsbAggregationSetting(
	PADAPTER padapter
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	/* Tx aggregation setting */
	usb_AggSettingTxUpdate(padapter);

	/* Rx aggregation setting */
	usb_AggSettingRxUpdate(padapter);

	/* 201/12/10 MH Add for USB agg mode dynamic switch. */
	pHalData->UsbRxHighSpeedMode = _FALSE;
}

static void
PHY_InitAntennaSelection8192F(
	PADAPTER padapter
)
{
}


static void
_InitRFType(
	PADAPTER Adapter
)
{
	struct registry_priv *pregpriv = &Adapter->registrypriv;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(Adapter);

#if DISABLE_BB_RF
	pHalData->rf_chip = RF_PSEUDO_11N;
	return;
#endif

	pHalData->rf_chip = RF_6052;

	RTW_INFO("Set RF Chip ID to RF_6052 and RF type to %d.\n", pHalData->rf_type);
}

/* Set CCK and OFDM Block "ON" */
static void _BBTurnOnBlock(
	PADAPTER padapter
)
{
#if (DISABLE_BB_RF)
	return;
#endif

	phy_set_bb_reg(padapter, rFPGA0_RFMOD, bCCKEn, 0x1);
	phy_set_bb_reg(padapter, rFPGA0_RFMOD, bOFDMEn, 0x1);
}

/* 2010/08/09 MH Add for power down check. */
static BOOLEAN
HalDetectPwrDownMode(
	PADAPTER padapter
)
{
	u8 tmpvalue;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(padapter);

	EFUSE_ShadowRead(padapter, 1, EEPROM_FEATURE_OPTION_8192F, (u32 *)&tmpvalue);

	if (tmpvalue & BIT(4) && pwrctrlpriv->reg_pdnmode)
		pHalData->pwrdown = _TRUE;
	else
		pHalData->pwrdown = _FALSE;

	RTW_INFO("%s(): PDN=%d\n", __func__, pHalData->pwrdown);
	return pHalData->pwrdown;
}   /* HalDetectPwrDownMode */


/*
 * 2010/08/26 MH Add for selective suspend mode check.
 * If Efuse 0x0e bit1 is not enabled, we can not support selective suspend for Minicard and
 * slim card.
 */
 #if 0   /* amyma */
static void
HalDetectSelectiveSuspendMode(
	PADAPTER padapter
)
{
	u8 tmpvalue;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(padapter);

	/* If support HW radio detect, we need to enable WOL ability, otherwise, we */
	/* can not use FW to notify host the power state switch. */

	EFUSE_ShadowRead(padapter, 1, EEPROM_USB_OPTIONAL1, (u32 *)&tmpvalue);

	RTW_INFO("HalDetectSelectiveSuspendMode(): SS ");
	if (tmpvalue & BIT(1))
		RTW_INFO("Enable\n");
	else {
		RTW_INFO("Disable\n");
		pdvobjpriv->RegUsbSS = _FALSE;
	}

	/* 2010/09/01 MH According to Dongle Selective Suspend INF. We can switch SS mode. */
	if (pdvobjpriv->RegUsbSS && !SUPPORT_HW_RADIO_DETECT(pHalData)) {
		/*PMGNT_INFO				pMgntInfo = &(padapter->MgntInfo); */

		/*if (!pMgntInfo->bRegDongleSS) */
		/*{ */
		pdvobjpriv->RegUsbSS = _FALSE;
		/*} */
	}
}   /* HalDetectSelectiveSuspendMode */
 #endif

/*-----------------------------------------------------------------------------
 * Function:	HwSuspendModeEnable()
 *
 * Overview:	HW suspend mode switch.
 *
 * Input:		NONE
 *
 * Output:	NONE
 *
 * Return:	NONE
 *
 * Revised History:
 *	When		Who		Remark
 *	08/23/2010	MHC		HW suspend mode switch test..
 *---------------------------------------------------------------------------*/
#if 0
static void
HwSuspendModeEnable(
	PADAPTER padapter,
	u8 Type
)
{
	/* PRT_USB_DEVICE pDevice = GET_RT_USB_DEVICE(padapter); */
	u16 reg = rtw_read16(padapter, REG_GPIO_MUXCFG);

	/* if (!pDevice->RegUsbSS) */
	{
		return;
	}

	/*
	 * 2010/08/23 MH According to Alfred's suggestion, we need to to prevent HW
	 * to enter suspend mode automatically. Otherwise, it will shut down major power
	 * domain and 8051 will stop. When we try to enter selective suspend mode, we
	 * need to prevent HW to enter D2 mode aumotmatically. Another way, Host will
	 * issue a S10 signal to power domain. Then it will cleat SIC setting(from Yngli).
	 * We need to enable HW suspend mode when enter S3/S4 or disable. We need
	 * to disable HW suspend mode for IPS/radio_off.
	 */
	if (Type == _FALSE) {
		reg |= BIT(14);
		rtw_write16(padapter, REG_GPIO_MUXCFG, reg);
		reg |= BIT(12);
		rtw_write16(padapter, REG_GPIO_MUXCFG, reg);
	} else {
		reg &= (~BIT(12));
		rtw_write16(padapter, REG_GPIO_MUXCFG, reg);
		reg &= (~BIT(14));
		rtw_write16(padapter, REG_GPIO_MUXCFG, reg);
	}

}   /* HwSuspendModeEnable */
#endif

rt_rf_power_state RfOnOffDetect(PADAPTER padapter)
{
	/* HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter); */
	u8 val8;
	rt_rf_power_state rfpowerstate = rf_off;

	if (adapter_to_pwrctl(padapter)->bHWPowerdown) {
		val8 = rtw_read8(padapter, REG_HSISR);
		RTW_INFO("pwrdown, 0x5c(BIT(7))=%02x\n", val8);
		rfpowerstate = (val8 & BIT(7)) ? rf_off : rf_on;
	} else { /* rf on/off */
		rtw_write8(padapter, REG_MAC_PINMUX_CFG, rtw_read8(padapter, REG_MAC_PINMUX_CFG) & ~(BIT(3)));
		val8 = rtw_read8(padapter, REG_GPIO_IO_SEL);
		RTW_INFO("GPIO_IN=%02x\n", val8);
		rfpowerstate = (val8 & BIT(3)) ? rf_on : rf_off;
	}
	return rfpowerstate;
}   /* HalDetectPwrDownMode */

void _InitBBRegBackup_8192fu(PADAPTER padapter)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	/* For Channel 1~11 (Default Value)*/
	pHalData->RegForRecover[0].offset = rCCK0_TxFilter2;
	pHalData->RegForRecover[0].value =
		phy_query_bb_reg(padapter,
			       pHalData->RegForRecover[0].offset, bMaskDWord);

	pHalData->RegForRecover[1].offset = rCCK0_DebugPort;
	pHalData->RegForRecover[1].value =
		phy_query_bb_reg(padapter,
			       pHalData->RegForRecover[1].offset, bMaskDWord);

	pHalData->RegForRecover[2].offset = 0xAAC;
	pHalData->RegForRecover[2].value =
		phy_query_bb_reg(padapter,
			       pHalData->RegForRecover[2].offset, bMaskDWord);
#if 0
	/* For 20 MHz	(Default Value)*/
	pHalData->RegForRecover[2].offset = rBBrx_DFIR;
	pHalData->RegForRecover[2].value = phy_query_bb_reg(padapter, pHalData->RegForRecover[2].offset, bMaskDWord);

	pHalData->RegForRecover[3].offset = rOFDM0_XATxAFE;
	pHalData->RegForRecover[3].value = phy_query_bb_reg(padapter, pHalData->RegForRecover[3].offset, bMaskDWord);

	pHalData->RegForRecover[4].offset = 0x1E;
	pHalData->RegForRecover[4].value = phy_query_rf_reg(padapter, RF_PATH_A, pHalData->RegForRecover[4].offset, bRFRegOffsetMask);
#endif
}

u32 rtl8192fu_hal_init(PADAPTER padapter)
{
	u8 value8 = 0, u1bRegCR;
	u32 status = _SUCCESS;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(padapter);
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	rt_rf_power_state eRfPowerStateToSet;
	u32 NavUpper = WiFiNavUpperUs;
	u32 value32;
	systime init_start_time = rtw_get_current_time();


	/* if (rtw_is_surprise_removed(padapter)) */
	/*	return RT_STATUS_FAILURE; */

	/* Check if MAC has already power on. */
	/* set usb timeout to fix read mac register fail before power on */
	rtw_write8(padapter, 0xFE4C /*REG_USB_ACCESS_TIMEOUT*/, 0x80);
	value8 = rtw_read8(padapter, REG_SYS_CLKR_8192F + 1);
	u1bRegCR = rtw_read8(padapter, REG_CR);
	RTW_INFO(" power-on :REG_SYS_CLKR 0x09=0x%02x. REG_CR 0x100=0x%02x.\n", value8, u1bRegCR);
	if ((value8 & BIT(3)) && (u1bRegCR != 0 && u1bRegCR != 0xEA))
		RTW_INFO(" MAC has already power on.\n");
	else {
		/* Set FwPSState to ALL_ON mode to prevent from the I/O be return because of 32k */
		/* state which is set before sleep under wowlan mode. 2012.01.04. by tynli. */
		RTW_INFO(" MAC has not been powered on yet.\n");
	}

#ifdef CONFIG_WOWLAN
	if (rtw_read8(padapter, REG_MCUFWDL) & BIT(7) &&
	    (pwrctrlpriv->wowlan_wake_reason & FW_DECISION_DISCONNECT)) {
		u8 reg_val = 0;

		RTW_INFO("+Reset Entry+\n");
		rtw_write8(padapter, REG_MCUFWDL, 0x00);
		_8051Reset8192F(padapter);
		/* reset BB */
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN);
		reg_val &= ~(BIT(0) | BIT(1));
		rtw_write8(padapter, REG_SYS_FUNC_EN, reg_val);
		/* reset RF */
		rtw_write8(padapter, REG_RF_CTRL, 0);
		/* reset TRX path */
		rtw_write16(padapter, REG_CR, 0);
		/* reset MAC, Digital Core */
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN + 1);
		reg_val &= ~(BIT(4) | BIT(7));
		rtw_write8(padapter, REG_SYS_FUNC_EN + 1, reg_val);
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN + 1);
		reg_val |= BIT(4) | BIT(7);
		rtw_write8(padapter, REG_SYS_FUNC_EN + 1, reg_val);
		RTW_INFO("-Reset Entry-\n");
	}
#endif /* CONFIG_WOWLAN */

	status = rtw_hal_power_on(padapter);
	if (status == _FAIL) {
		goto exit;
	}

	status = rtl8192f_InitLLTTable(padapter);
	if (status == _FAIL) {
		goto exit;
	}

	if (pHalData->bRDGEnable)
		_InitRDGSetting_8192fu(padapter);


	/* Enable TX Report */
	/* Enable Tx Report Timer */
	value8 = rtw_read8(padapter, REG_TX_RPT_CTRL);
	rtw_write8(padapter, REG_TX_RPT_CTRL, value8 | BIT(1));
	/* Set MAX RPT MACID */
	rtw_write8(padapter, REG_TX_RPT_CTRL + 1, 2);
	/* Tx RPT Timer. Unit: 32us */
	rtw_write16(padapter, REG_TX_RPT_TIME, 0xCdf0);
#ifdef CONFIG_TX_EARLY_MODE
	if (pHalData->AMPDUBurstMode) {

		value8 = rtw_read8(padapter, REG_EARLY_MODE_CONTROL_8192F);
#if RTL8192F_EARLY_MODE_PKT_NUM_10 == 1
		value8 = value8 | 0x1f;
#else
		value8 = value8 | 0xf;
#endif
		rtw_write8(padapter, REG_EARLY_MODE_CONTROL_8192F, value8);

		rtw_write8(padapter, REG_EARLY_MODE_CONTROL_8192F + 3, 0x80);

		value8 = rtw_read8(padapter, REG_TCR_8192F + 1);
		value8 = value8 | 0x40;
		rtw_write8(padapter, REG_TCR_8192F + 1, value8);
	} else
#endif
		rtw_write8(padapter, REG_EARLY_MODE_CONTROL_8192F, 0);

	/* <Kordan> InitHalDm should be put ahead of FirmwareDownload. (HWConfig flow: FW->MAC->-BB->RF) */
	/* rtl8192f_InitHalDm(padapter); */

	if (padapter->registrypriv.mp_mode == 0
		#if defined(CONFIG_MP_INCLUDED) && defined(CONFIG_RTW_CUSTOMER_STR)
		|| padapter->registrypriv.mp_customer_str
		#endif
	) {
		status = rtl8192f_FirmwareDownload(padapter, _FALSE);
		if (status != _SUCCESS) {
			pHalData->bFWReady = _FALSE;
			pHalData->fw_ractrl = _FALSE;
			goto exit;
		} else {
			pHalData->bFWReady = _TRUE;
			pHalData->fw_ractrl = _TRUE;
		}
	}

	if (pwrctrlpriv->reg_rfoff == _TRUE)
		pwrctrlpriv->rf_pwrstate = rf_off;

	/* Set RF type for BB/RF configuration */
	_InitRFType(padapter);

	HalDetectPwrDownMode(padapter);

	/* We should call the function before MAC/BB configuration. */
	PHY_InitAntennaSelection8192F(padapter);


#if (DISABLE_BB_RF == 1)
	/* fpga verification to open phy */
	rtw_write8(padapter, REG_SYS_FUNC_EN_8192F, FEN_USBA | FEN_USBD | FEN_BB_GLB_RSTn | FEN_BBRSTB);
#endif

#if (HAL_MAC_ENABLE == 1)
	status = PHY_MACConfig8192F(padapter);
	if (status == _FAIL) {
		RTW_INFO("PHY_MACConfig8192F fault !!\n");
		goto exit;
	}
#endif

	/* d. Initialize BB related configurations. */
#if (HAL_BB_ENABLE == 1)
	status = PHY_BBConfig8192F(padapter);
	if (status == _FAIL) {
		RTW_INFO("PHY_BBConfig8192F fault !!\n");
		goto exit;
	}
#endif

#if (HAL_RF_ENABLE == 1)
	status = PHY_RFConfig8192F(padapter);

	if (status == _FAIL) {
		RTW_INFO("PHY_RFConfig8192F fault !!\n");
		goto exit;
	}
	/*---- Set CCK and OFDM Block "ON"----*/
	phy_set_bb_reg(padapter, rFPGA0_RFMOD, bCCKEn, 0x1);
	phy_set_bb_reg(padapter, rFPGA0_RFMOD, bOFDMEn, 0x1);
#endif

	_InitBBRegBackup_8192fu(padapter);

	_InitMacAPLLSetting_8192F(padapter);

	_InitQueueReservedPage(padapter);
	_InitTRxBufferBoundary(padapter);
	_InitQueuePriority(padapter);
	_InitTransferPageSize_8192fu(padapter);


	/* Get Rx PHY status in order to report RSSI and others. */
	_InitDriverInfoSize(padapter, DRVINFO_SZ);

	_InitInterrupt(padapter);
	_InitNetworkType(padapter); /* set msr */
	_InitWMACSetting(padapter);
	_InitAdaptiveCtrl(padapter);
	_InitEDCA(padapter);
	_InitRetryFunction(padapter);
	/* _InitOperationMode(padapter);*/ /*todo */
	rtl8192f_InitBeaconParameters(padapter);
	rtl8192f_InitBeaconMaxError(padapter, _TRUE);

	_InitBurstPktLen(padapter);
	_initUsbAggregationSetting(padapter);

#ifdef ENABLE_USB_DROP_INCORRECT_OUT
	_InitHardwareDropIncorrectBulkOut(padapter);
#endif

	/* Enable MACTXEN/MACRXEN block */
	u1bRegCR = rtw_read8(padapter, REG_CR);
	u1bRegCR |= (MACTXEN | MACRXEN);
	rtw_write8(padapter, REG_CR, u1bRegCR);

#ifdef CONFIG_RTW_LED
	_InitHWLed(padapter);
#endif /* CONFIG_RTW_LED */

	_BBTurnOnBlock(padapter);
	/* NicIFSetMacAddress(padapter, padapter->PermanentAddress); */


	rtw_hal_set_chnl_bw(padapter, padapter->registrypriv.channel,
		CHANNEL_WIDTH_20, HAL_PRIME_CHNL_OFFSET_DONT_CARE, HAL_PRIME_CHNL_OFFSET_DONT_CARE);

	invalidate_cam_all(padapter);

	/* 2010/12/17 MH We need to set TX power according to EFUSE content at first. */
	/* rtw_hal_set_tx_power_level(padapter, pHalData->current_channel); */
	rtl8192f_InitAntenna_Selection(padapter);

	/* HW SEQ CTRL */
	/* set 0x0 to 0xFF by tynli. Default enable HW SEQ NUM. */
	rtw_write8(padapter, REG_HWSEQ_CTRL, 0xFF);

	/*
	 * Disable BAR, suggested by Scott
	 * 2010.04.09 add by hpfan
	 */
	rtw_write32(padapter, REG_BAR_MODE_CTRL, 0x0201ffff);

	if (pregistrypriv->wifi_spec)
		rtw_write16(padapter, REG_FAST_EDCA_CTRL, 0);

	rtl8192f_InitHalDm(padapter);

#if (MP_DRIVER == 1)
	if (padapter->registrypriv.mp_mode == 1) {
		padapter->mppriv.channel = pHalData->current_channel;
		MPT_InitializeAdapter(padapter, padapter->mppriv.channel);
	} else
#endif
	{
		pwrctrlpriv->rf_pwrstate = rf_on;

		/*phy_lc_calibrate_8192f(&pHalData->odmpriv);*/
		halrf_lck_trigger(&pHalData->odmpriv);

		pHalData->neediqk_24g = _TRUE;

		odm_txpowertracking_check(&pHalData->odmpriv);
	}

#ifdef CONFIG_BT_COEXIST
	/* Init BT hw config. */
	if (_TRUE == pHalData->EEPROMBluetoothCoexist)
		rtw_btcoex_HAL_Initialize(padapter, _FALSE);
#endif /* CONFIG_BT_COEXIST */

	rtw_hal_set_hwreg(padapter, HW_VAR_NAV_UPPER, (u8 *)&NavUpper);

#ifdef CONFIG_XMIT_ACK
	/* ack for xmit mgmt frames. */
	rtw_write32(padapter, REG_FWHW_TXQ_CTRL, rtw_read32(padapter, REG_FWHW_TXQ_CTRL) | BIT(12));
#endif /*CONFIG_XMIT_ACK */

	phy_set_bb_reg(padapter, rOFDM0_XAAGCCore1, bMaskByte0, 0x50);
	phy_set_bb_reg(padapter, rOFDM0_XAAGCCore1, bMaskByte0, 0x20);

exit:

	RTW_INFO("%s in %dms\n", __func__, rtw_get_passing_time_ms(init_start_time));


	return status;
}
#if 0
static void
_ResetFWDownloadRegister(
	PADAPTER padapter
)
{
	u32 value32;

	value32 = rtw_read32(padapter, REG_MCUFWDL);
	value32 &= ~(MCUFWDL_EN | MCUFWDL_RDY);
	rtw_write32(padapter, REG_MCUFWDL, value32);
}

static void
_ResetBB(
	PADAPTER padapter
)
{
	u16 value16;

	/* reset BB */
	value16 = rtw_read16(padapter, REG_SYS_FUNC_EN);
	value16 &= ~(FEN_BBRSTB | FEN_BB_GLB_RSTn);
	rtw_write16(padapter, REG_SYS_FUNC_EN, value16);
}

static void
_ResetMCU(
	PADAPTER padapter
)
{
	u16 value16;

	/* reset MCU */
	value16 = rtw_read16(padapter, REG_SYS_FUNC_EN);
	value16 &= ~FEN_CPUEN;
	rtw_write16(padapter, REG_SYS_FUNC_EN, value16);
}

static void
_DisableMAC_AFE_PLL(
	PADAPTER padapter
)
{
	u32 value32;

	/* disable MAC/ AFE PLL */
	value32 = rtw_read32(padapter, REG_APS_FSMCO);
	value32 |= APDM_MAC;
	rtw_write32(padapter, REG_APS_FSMCO, value32);

	value32 |= APFM_OFF;
	rtw_write32(padapter, REG_APS_FSMCO, value32);
}

static void
_AutoPowerDownToHostOff(
	PADAPTER padapter
)
{
	u32 value32;

	rtw_write8(padapter, REG_SPS0_CTRL, 0x22);

	value32 = rtw_read32(padapter, REG_APS_FSMCO);

	value32 |= APDM_HOST; /* card disable */
	rtw_write32(padapter, REG_APS_FSMCO, value32);

	/* set USB suspend */
	value32 = rtw_read32(padapter, REG_APS_FSMCO);
	value32 &= ~AFSM_PCIE;
	rtw_write32(padapter, REG_APS_FSMCO, value32);

}

static void
_SetUsbSuspend(
	PADAPTER padapter
)
{
	u32 value32;

	value32 = rtw_read32(padapter, REG_APS_FSMCO);

	/* set USB suspend */
	value32 |= AFSM_HSUS;
	rtw_write32(padapter, REG_APS_FSMCO, value32);

	/* RT_ASSERT(0 == (rtw_read32(padapter, REG_APS_FSMCO) & BIT(12)),("")); */

}

static void
_DisableRFAFEAndResetBB(
	PADAPTER padapter
)
{
	/*
	 * a.	TXPAUSE 0x522[7:0] = 0xFF			Pause MAC TX queue
	 * b.	RF path 0 offset 0x00 = 0x00		disable RF
	 * c.	APSD_CTRL 0x600[7:0] = 0x40
	 * d.	SYS_FUNC_EN 0x02[7:0] = 0x16		reset BB state machine
	 * e.	SYS_FUNC_EN 0x02[7:0] = 0x14		reset BB state machine
	 */
	enum rf_path eRFPath = RF_PATH_A, value8 = 0;

	rtw_write8(padapter, REG_TXPAUSE, 0xFF);
	phy_set_rf_reg(padapter, eRFPath, 0x0, bMaskByte0, 0x0);

	value8 |= APSDOFF;
	rtw_write8(padapter, REG_APSD_CTRL, value8); /*0x40 */

	value8 = 0;
	value8 |= (FEN_USBD | FEN_USBA | FEN_BB_GLB_RSTn);
	rtw_write8(padapter, REG_SYS_FUNC_EN, value8); /*0x16 */

	value8 &= (~FEN_BB_GLB_RSTn);
	rtw_write8(padapter, REG_SYS_FUNC_EN, value8); /*0x14 */

}

static void
_ResetDigitalProcedure1(
	PADAPTER padapter,
	BOOLEAN bWithoutHWSM
)
{

	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	if (pHalData->firmware_version <= 0x20) {
#if 0
		/*
		 * f.	SYS_FUNC_EN 0x03[7:0]=0x54		reset MAC register, DCORE
		 * g.	MCUFWDL 0x80[7:0]=0				reset MCU ready status
		 */
		u32 value32 = 0;

		PlatformIOWrite1Byte(padapter, REG_SYS_FUNC_EN + 1, 0x54);
		PlatformIOWrite1Byte(padapter, REG_MCUFWDL, 0);
#else
		/*
		 * f.	MCUFWDL 0x80[7:0]=0				reset MCU ready status
		 * g.	SYS_FUNC_EN 0x02[10]= 0			reset MCU register, (8051 reset)
		 * h.	SYS_FUNC_EN 0x02[15-12]= 5		reset MAC register, DCORE
		 * i.     SYS_FUNC_EN 0x02[10]= 1		enable MCU register, (8051 enable)
		 */
		u16 valu16 = 0;

		rtw_write8(padapter, REG_MCUFWDL, 0);

		valu16 = rtw_read16(padapter, REG_SYS_FUNC_EN);
		rtw_write16(padapter, REG_SYS_FUNC_EN, (valu16 & (~FEN_CPUEN))); /*reset MCU ,8051 */

		valu16 = rtw_read16(padapter, REG_SYS_FUNC_EN) & 0x0FFF;
		rtw_write16(padapter, REG_SYS_FUNC_EN, (valu16 | (FEN_HWPDN | FEN_ELDR))); /*reset MAC */

#ifdef DBG_SHOW_MCUFWDL_BEFORE_51_ENABLE
		{
			u8 val;

			val = rtw_read8(padapter, REG_MCUFWDL)

			if (val) {
				RTW_INFO("DBG_SHOW_MCUFWDL_BEFORE_51_ENABLE %s:%d REG_MCUFWDL:0x%02x\n",
					 __func__, __LINE__, val);
			}
		}
#endif


		valu16 = rtw_read16(padapter, REG_SYS_FUNC_EN);
		rtw_write16(padapter, REG_SYS_FUNC_EN, (valu16 | FEN_CPUEN)); /*enable MCU ,8051 */


#endif
	} else {
		u8 retry_cnts = 0;

		if (rtw_read8(padapter, REG_MCUFWDL) & BIT(1)) {
			/* IF fw in RAM code, do reset */

			rtw_write8(padapter, REG_MCUFWDL, 0);
			if (GET_HAL_DATA(padapter)->bFWReady) {
				/* 2010/08/25 MH According to RD alfred's suggestion, we need to disable other */
				/* HRCV INT to influence 8051 reset. */
				rtw_write8(padapter, REG_FWIMR, 0x20);

				rtw_write8(padapter, REG_HMETFR + 3, 0x20); /*8051 reset by self */

				while ((retry_cnts++ < 100) && (FEN_CPUEN & rtw_read16(padapter, REG_SYS_FUNC_EN)))
					rtw_udelay_os(50); /* us */

				if (retry_cnts >= 100) {
					RTW_INFO("%s #####=> 8051 reset failed!.........................\n", __func__);
					/* if 8051 reset fail we trigger GPIO 0 for LA */
					/*PlatformEFIOWrite4Byte(	padapter, */
					/*						REG_GPIO_PIN_CTRL, */
					/*						0x00010100); */
					/* 2010/08/31 MH According to Filen's info, if 8051 reset fail, reset MAC directly. */
					rtw_write8(padapter, REG_SYS_FUNC_EN + 1, 0x50); /*Reset MAC and Enable 8051 */
					rtw_mdelay_os(10);
				} else {
					/*RTW_INFO("%s =====> 8051 reset success (%d) .\n", __func__, retry_cnts); */
				}
			} else
				RTW_INFO("%s =====> 8051 in RAM but !hal_data->bFWReady\n", __func__);
		} else {
			/*RTW_INFO("%s =====> 8051 in ROM.\n", __func__); */
		}

#ifdef DBG_SHOW_MCUFWDL_BEFORE_51_ENABLE
		{
			u8 val;

			val = rtw_read8(padapter, REG_MCUFWDL);

			if (val) {
				RTW_INFO("DBG_SHOW_MCUFWDL_BEFORE_51_ENABLE %s:%d REG_MCUFWDL:0x%02x\n",
					 __func__, __LINE__, val);
			}
		}
#endif

		rtw_write8(padapter, REG_SYS_FUNC_EN + 1, 0x54); /*Reset MAC and Enable 8051 */
	}

	/* Clear rpwm value for initial toggle bit trigger. */
	rtw_write8(padapter, REG_USB_HRPWM, 0x00);

	if (bWithoutHWSM) {
		/*
		 * Without HW auto state machine
		 * g.	SYS_CLKR 0x08[15:0] = 0x30A3			disable MAC clock
		 * h.	AFE_PLL_CTRL 0x28[7:0] = 0x80			disable AFE PLL
		 * i.	AFE_XTAL_CTRL 0x24[15:0] = 0x880F		gated AFE DIG_CLOCK
		 * j.	SYS_ISO_CTRL 0x00[7:0] = 0xF9			isolated digital to PON
		 */
		/*rtw_write16(padapter, REG_SYS_CLKR, 0x30A3); */
		rtw_write16(padapter, REG_SYS_CLKR, 0x70A3); /*modify to 0x70A3 by Scott. */
		rtw_write8(padapter, REG_AFE_PLL_CTRL, 0x80);
		rtw_write16(padapter, REG_AFE_XTAL_CTRL, 0x880F);
		rtw_write8(padapter, REG_SYS_ISO_CTRL, 0xF9);
	} else {
		/* Disable all RF/BB power */
		rtw_write8(padapter, REG_RF_CTRL, 0x00);
	}

}

static void
_ResetDigitalProcedure2(
	PADAPTER padapter
)
{
	/*
	 * k.	SYS_FUNC_EN 0x03[7:0] = 0x44			disable ELDR runction
	 * l.	SYS_CLKR 0x08[15:0] = 0x3083			disable ELDR clock
	 * m.	SYS_ISO_CTRL 0x01[7:0] = 0x83			isolated ELDR to PON
	 ******************************/
	/*rtw_write8(padapter, REG_SYS_FUNC_EN+1, 0x44); */ /* marked by Scott. */
	/*rtw_write16(padapter, REG_SYS_CLKR, 0x3083); */
	/*rtw_write8(padapter, REG_SYS_ISO_CTRL+1, 0x83); */

	rtw_write16(padapter, REG_SYS_CLKR, 0x70a3); /* modify to 0x70a3 by Scott. */
	rtw_write8(padapter, REG_SYS_ISO_CTRL + 1, 0x82); /* modify to 0x82 by Scott. */
}

static void
_DisableAnalog(
	PADAPTER padapter,
	BOOLEAN bWithoutHWSM
)
{
	u16 value16 = 0;
	u8 value8 = 0;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	if (bWithoutHWSM) {
		/*
		 * n.	LDOA15_CTRL 0x20[7:0] = 0x04		disable A15 power
		 * o.	LDOV12D_CTRL 0x21[7:0] = 0x54		disable digital core power
		 * r.	When driver call disable, the ASIC will turn off remaining clock automatically
		 */

		rtw_write8(padapter, REG_LDOA15_CTRL, 0x04);
		/* PlatformIOWrite1Byte(padapter, REG_LDOV12D_CTRL, 0x54); */

		value8 = rtw_read8(padapter, REG_LDOV12D_CTRL);
		value8 &= (~LDV12_EN);
		rtw_write8(padapter, REG_LDOV12D_CTRL, value8);
	}

	/*
	 * h.	SPS0_CTRL 0x11[7:0] = 0x23			enter PFM mode
	 * i.	APS_FSMCO 0x04[15:0] = 0x4802		set USB suspend
	 */


	value8 = 0x23;

	rtw_write8(padapter, REG_SPS0_CTRL, value8);


	if (bWithoutHWSM) {
		/* 2010/08/31 According to Filen description, we need to use HW to shut down 8051 automatically. */
		/* Because suspend operation need the asistance of 8051 to wait for 3ms. */
		value16 |= (APDM_HOST | AFSM_HSUS | PFM_ALDN);
	} else
		value16 |= (APDM_HOST | AFSM_HSUS | PFM_ALDN);

	rtw_write16(padapter, REG_APS_FSMCO, value16); /* 0x4802 */

	rtw_write8(padapter, REG_RSV_CTRL, 0x0e);

#if 0
	/* tynli_test for suspend mode. */
	if (!bWithoutHWSM)
		rtw_write8(padapter, 0xfe10, 0x19);
#endif

}
#endif

static void rtl8192fu_hw_power_down(PADAPTER padapter)
{
	u8 u1bTmp;

	RTW_INFO("PowerDownRTL8192FU\n");


	/* 1. Run Card Disable Flow */
	/* Done before this function call. */

	/* 2. 0x04[16] = 0 */   /* reset WLON */
	u1bTmp = rtw_read8(padapter, REG_APS_FSMCO + 2);
	rtw_write8(padapter, REG_APS_FSMCO + 2, (u1bTmp & (~BIT(0))));

	/* 3. 0x04[12:11] = 2b'11 */ /* enable suspend */
	/* Done before this function call. */

	/* 4. 0x04[15] = 1 */ /* enable PDN */
	u1bTmp = rtw_read8(padapter, REG_APS_FSMCO + 1);
	rtw_write8(padapter, REG_APS_FSMCO + 1, (u1bTmp | BIT(7)));
}

/*
 * Description: RTL8723e card disable power sequence v003 which suggested by Scott.
 * First created by tynli. 2011.01.28.
 */
void
CardDisableRTL8192fu(
	PADAPTER padapter
)
{
	u8 u1bTmp;

	rtw_hal_get_hwreg(padapter, HW_VAR_APFM_ON_MAC, &u1bTmp);
	RTW_INFO(FUNC_ADPT_FMT ": bMacPwrCtrlOn=%d\n", FUNC_ADPT_ARG(padapter), u1bTmp);
	if (u1bTmp == _FALSE)
		return;
	u1bTmp = _FALSE;
	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &u1bTmp);

	/* Stop Tx Report Timer. 0x4EC[Bit1]=b'0 */
	u1bTmp = rtw_read8(padapter, REG_TX_RPT_CTRL);
	rtw_write8(padapter, REG_TX_RPT_CTRL, u1bTmp & (~BIT(1)));

	/* stop rx */
	rtw_write8(padapter, REG_CR, 0x0);

	/* 1. Run LPS WL RFOFF flow */
	HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, rtl8192F_enter_lps_flow);

	if ((rtw_read8(padapter, REG_MCUFWDL_8192F) & BIT(7)) &&
	    GET_HAL_DATA(padapter)->bFWReady) /*8051 RAM code */
		rtl8192f_FirmwareSelfReset(padapter);

	/* Reset MCU. Suggested by Filen. 2011.01.26. by tynli. */
	u1bTmp = rtw_read8(padapter, REG_SYS_FUNC_EN_8192F + 1);
	rtw_write8(padapter, REG_SYS_FUNC_EN_8192F + 1, (u1bTmp & (~BIT(2))));

	/* MCUFWDL 0x80[1:0]=0 */   /* reset MCU ready status */
	rtw_write8(padapter, REG_MCUFWDL_8192F, 0x00);

	/* Card disable power action flow */
	HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, rtl8192F_card_disable_flow);

	GET_HAL_DATA(padapter)->bFWReady = _FALSE;
}


u32 rtl8192fu_hal_deinit(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(padapter);

	RTW_INFO("==> %s\n", __func__);

#ifdef CONFIG_RTW_LED
#ifdef CONFIG_RTW_SW_LED
	/* configure HW GPIO for LED0 */
	if(rtl8192fu_CfgLed0Hw(padapter)) {
		/* set LED0 to turn off */
		rtw_write32(padapter, REG_LEDCFG0, (rtw_read32(padapter, REG_LEDCFG0) &
				~(LED0_SW_VALUE_8192FU)));
	}
#endif /* CONFIG_RTW_SW_LED */
#endif /* CONFIG_RTW_LED */

	rtw_write16(padapter, REG_GPIO_MUXCFG, rtw_read16(padapter, REG_GPIO_MUXCFG) & (~BIT(12)));

	rtw_write32(padapter, REG_HISR0_8192F, 0xFFFFFFFF);
	rtw_write32(padapter, REG_HISR1_8192F, 0xFFFFFFFF);
#ifdef CONFIG_MP_INCLUDED
	if (padapter->registrypriv.mp_mode == 1)
		MPT_DeInitAdapter(padapter);
#endif

#ifdef SUPPORT_HW_RFOFF_DETECTED
	RTW_INFO("%s: bkeepfwalive(%x)\n", __func__, pwrctl->bkeepfwalive);

	if (pwrctl->bkeepfwalive) {
		_ps_close_RF(padapter);
		if ((pwrctl->bHWPwrPindetect) && (pwrctl->bHWPowerdown))
			rtl8192fu_hw_power_down(padapter);
	} else
#endif
	{
		if (rtw_is_hw_init_completed(padapter)) {
			rtw_hal_power_off(padapter);

			if ((pwrctl->bHWPwrPindetect) && (pwrctl->bHWPowerdown))
				rtl8192fu_hw_power_down(padapter);
		}
		pHalData->bMacPwrCtrlOn = _FALSE;
	}
	return _SUCCESS;
}


unsigned int rtl8192fu_inirp_init(PADAPTER padapter)
{
	struct registry_priv *regsty = adapter_to_regsty(padapter);
	u8 i;
	struct recv_buf *precvbuf;
	uint status;
	struct dvobj_priv *pdev = adapter_to_dvobj(padapter);
	struct intf_hdl *pintfhdl = &padapter->iopriv.intf;
	struct recv_priv *precvpriv = &(padapter->recvpriv);

	u32(*_read_port)(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pmem);
#ifdef CONFIG_USB_INTERRUPT_IN_PIPE
	u32(*_read_interrupt)(struct intf_hdl *pintfhdl, u32 addr);
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
#endif /*CONFIG_USB_INTERRUPT_IN_PIPE */


	_read_port = pintfhdl->io_ops._read_port;

	status = _SUCCESS;


	precvpriv->ff_hwaddr = RECV_BULK_IN_ADDR;

	/* issue Rx irp to receive data */
	precvbuf = (struct recv_buf *)precvpriv->precv_buf;
	for (i = 0; i < regsty->recvbuf_nr; i++) {
		if (_read_port(pintfhdl, precvpriv->ff_hwaddr, 0, (unsigned char *)precvbuf) == _FALSE) {
			status = _FAIL;
			goto exit;
		}

		precvbuf++;
		precvpriv->free_recv_buf_queue_cnt--;
	}

#ifdef CONFIG_USB_INTERRUPT_IN_PIPE
	_read_interrupt = pintfhdl->io_ops._read_interrupt;
	if (_read_interrupt(pintfhdl, RECV_INT_IN_ADDR) == _FALSE) {
		status = _FAIL;
	}
	pHalData->IntrMask[0] = rtw_read32(padapter, REG_USB_HIMR);
	RTW_INFO("pHalData->IntrMask = 0x%04x\n", pHalData->IntrMask[0]);
	pHalData->IntrMask[0] |= UHIMR_C2HCMD | UHIMR_CPWM;
	rtw_write32(padapter, REG_USB_HIMR, pHalData->IntrMask[0]);
#endif /*CONFIG_USB_INTERRUPT_IN_PIPE */

exit:



	return status;

}

unsigned int rtl8192fu_inirp_deinit(PADAPTER padapter)
{
#ifdef CONFIG_USB_INTERRUPT_IN_PIPE
	u32(*_read_interrupt)(struct intf_hdl *pintfhdl, u32 addr);
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
#endif /*CONFIG_USB_INTERRUPT_IN_PIPE */

	rtw_read_port_cancel(padapter);
#ifdef CONFIG_USB_INTERRUPT_IN_PIPE
	pHalData->IntrMask[0] = rtw_read32(padapter, REG_USB_HIMR);
	RTW_INFO("%s pHalData->IntrMask = 0x%04x\n", __func__, pHalData->IntrMask[0]);
	pHalData->IntrMask[0] = 0x0;
	rtw_write32(padapter, REG_USB_HIMR, pHalData->IntrMask[0]);
#endif /*CONFIG_USB_INTERRUPT_IN_PIPE */
	return _SUCCESS;
}


/*
 *-------------------------------------------------------------------
 *	EEPROM/EFUSE Content Parsing
 *-------------------------------------------------------------------
 */

static void
ReadLEDSetting_8192FU(
		PADAPTER	Adapter,
		u8		*PROMContent,
		BOOLEAN		AutoloadFail
)
{
#ifdef CONFIG_RTW_LED
	struct led_priv *pledpriv = adapter_to_led(Adapter);
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
#ifdef CONFIG_RTW_SW_LED
	pledpriv->bRegUseLed = _TRUE;

	switch (pHalData->CustomerID) {
	default:
		pledpriv->LedStrategy = SW_LED_MODE1;
		break;
	}
	pHalData->bLedOpenDrain = _TRUE;/* Support Open-drain arrangement for controlling the LED. Added by Roger, 2009.10.16. */
#else /* HW LED */
	pledpriv->LedStrategy = HW_LED;
#endif /* CONFIG_RTW_SW_LED */
#endif
}

static void
ParseLEDEfuseSetting_8192F(
	PADAPTER padapter,
	u8 *PROMContent,
	BOOLEAN AutoloadFail
)
{
#if 0
	struct led_priv *pledpriv = adapter_to_led(padapter);
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

#ifdef CONFIG_RTW_SW_LED
	pledpriv->bRegUseLed = _TRUE;

	/* Led mode */
	switch (pHalData->CustomerID) {
	case RT_CID_DEFAULT:
		pledpriv->LedStrategy = SW_LED_MODE1;
		pledpriv->bRegUseLed = _TRUE;
		break;

	case RT_CID_819x_HP:
		pledpriv->LedStrategy = SW_LED_MODE6;
		break;

	default:
		pledpriv->LedStrategy = SW_LED_MODE1;
		break;
	}

	pHalData->bLedOpenDrain = _TRUE; /* Support Open-drain arrangement for controlling the LED. Added by Roger, 2009.10.16. */
#else /* HW LED */
	pledpriv->LedStrategy = HW_LED;
#endif /*CONFIG_RTW_SW_LED */
#endif
}


/* Read HW power down mode selection */
static void
hal_EfuseParsePowerSavingSetting(
	PADAPTER padapter,
	u8 *PROMContent,
	u8 AutoloadFail
)
{
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(padapter);

	if (AutoloadFail) {
		pwrctl->bHWPowerdown = _FALSE;
		pwrctl->bSupportRemoteWakeup = _FALSE;
	} else {
		/*if(SUPPORT_HW_RADIO_DETECT(padapter)) */
		pwrctl->bHWPwrPindetect = padapter->registrypriv.hwpwrp_detect;
		/*else */
		/*pwrctl->bHWPwrPindetect = _FALSE; */ /*dongle not support new */


		/*hw power down mode selection , 0:rf-off / 1:power down */

		if (padapter->registrypriv.hwpdn_mode == 2)
			pwrctl->bHWPowerdown = (PROMContent[EEPROM_FEATURE_OPTION_8192F] & BIT(4));
		else
			pwrctl->bHWPowerdown = padapter->registrypriv.hwpdn_mode;

		/* decide hw if support remote wakeup function */
		/* if hw supported, 8051 (SIE) will generate WeakUP signal( D+/D- toggle) when autoresume */
		pwrctl->bSupportRemoteWakeup = (PROMContent[EEPROM_USB_OPTIONAL_FUNCTION0] & BIT(1)) ? _TRUE : _FALSE;

		/*if(SUPPORT_HW_RADIO_DETECT(padapter)) */
		/*padapter->registrypriv.usbss_enable = pwrctl->bSupportRemoteWakeup ; */

		RTW_INFO("%s...bHWPwrPindetect(%x)-bHWPowerdown(%x) ,bSupportRemoteWakeup(%x)\n", __func__,
			pwrctl->bHWPwrPindetect, pwrctl->bHWPowerdown, pwrctl->bSupportRemoteWakeup);

		RTW_INFO("### PS params=>  power_mgnt(%x),usbss_enable(%x) ###\n", padapter->registrypriv.power_mgnt, padapter->registrypriv.usbss_enable);

	}
}

static void
hal_EfuseParseIDs(
	PADAPTER padapter,
	u8 *hwinfo,
	BOOLEAN AutoLoadFail
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

	if (!AutoLoadFail) {
		/* VID, PID */
		pHalData->EEPROMVID = ReadLE2Byte(&hwinfo[EEPROM_VID_8192FU]);
		pHalData->EEPROMPID = ReadLE2Byte(&hwinfo[EEPROM_PID_8192FU]);

		/* Customer ID, 0x00 and 0xff are reserved for Realtek. */
		pHalData->EEPROMCustomerID = *(u8 *)&hwinfo[EEPROM_CustomID_8192F];
		pHalData->EEPROMSubCustomerID = EEPROM_Default_SubCustomerID;
	} else {
		pHalData->EEPROMVID = EEPROM_Default_VID;
		pHalData->EEPROMPID = EEPROM_Default_PID;

		/* Customer ID, 0x00 and 0xff are reserved for Realtek. */
		pHalData->EEPROMCustomerID = EEPROM_Default_CustomerID;
		pHalData->EEPROMSubCustomerID = EEPROM_Default_SubCustomerID;
	}

	if ((pHalData->EEPROMVID == EEPROM_Default_VID)
	    && (pHalData->EEPROMPID == EEPROM_Default_PID)) {
		pHalData->CustomerID = EEPROM_Default_CustomerID;
		pHalData->EEPROMSubCustomerID = EEPROM_Default_SubCustomerID;
	}

	RTW_INFO("VID = 0x%04X, PID = 0x%04X\n", pHalData->EEPROMVID, pHalData->EEPROMPID);
	RTW_INFO("Customer ID: 0x%02X, SubCustomer ID: 0x%02X\n", pHalData->EEPROMCustomerID, pHalData->EEPROMSubCustomerID);
}

static u8
InitAdapterVariablesByPROM_8192FU(
	PADAPTER padapter
)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);
	u8 *hwinfo = NULL;
	u8 ret = _FAIL;

	if (sizeof(pHalData->efuse_eeprom_data) < HWSET_MAX_SIZE_8192F)
		RTW_INFO("[WARNING] size of efuse_eeprom_data is less than HWSET_MAX_SIZE_8192F!\n");

	hwinfo = pHalData->efuse_eeprom_data;

	Hal_InitPGData(padapter, hwinfo);
	Hal_EfuseParseIDCode(padapter, hwinfo);
	Hal_EfuseParseEEPROMVer_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);

	hal_EfuseParseIDs(padapter, hwinfo, pHalData->bautoload_fail_flag);
	hal_config_macaddr(padapter, pHalData->bautoload_fail_flag);
	hal_EfuseParsePowerSavingSetting(padapter, hwinfo, pHalData->bautoload_fail_flag);

	Hal_EfuseParseTxPowerInfo_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	Hal_EfuseParseBoardType_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	if (Hal_ReadRFEType_8192F(padapter, pHalData->efuse_eeprom_data, 
		 pHalData->bautoload_fail_flag) != _SUCCESS)
		goto exit;

	Hal_EfuseParseChnlPlan_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	Hal_EfuseParseXtal_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	Hal_EfuseParseThermalMeter_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	Hal_EfuseParseAntennaDiversity_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);
	Hal_EfuseParseCustomerID_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);

	ReadLEDSetting_8192FU(padapter, hwinfo, pHalData->bautoload_fail_flag);
	ParseLEDEfuseSetting_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);

#ifdef CONFIG_BT_COEXIST
	Hal_EfuseParseBTCoexistInfo_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag);

	/* set coex. ant info once efuse parsing is done */
	rtw_btcoex_set_ant_info(padapter);
#endif /* CONFIG_BT_COEXIST */

	/* Hal_EfuseParseKFreeData_8192F(padapter, hwinfo, pHalData->bautoload_fail_flag); */

	if (hal_read_mac_hidden_rpt(padapter) != _SUCCESS) {
		goto exit;
	} 
#ifdef CONFIG_BT_COEXIST
	else {
		struct hal_spec_t *hal_spec = GET_HAL_SPEC(padapter);

		if (hal_spec->hci_type <= 3 && hal_spec->hci_type >= 1) {
			pHalData->EEPROMBluetoothCoexist = _FALSE;
			RTW_INFO("EEPROM Disable BT-coex by hal_spec\n");
		}
	}
#endif /* CONFIG_BT_COEXIST */

	rtw_phydm_read_efuse(padapter);
	ret = _SUCCESS;

exit:
	return ret;
}

static u8
Hal_ReadPROMContent_8192FU(
	PADAPTER padapter
)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);

	u8 eeValue;
	u32 i;
	u16 value16;
	u8 ret = _FAIL;

	eeValue = rtw_read8(padapter, REG_9346CR);
	/* To check system boot selection. */
	pHalData->EepromOrEfuse = (eeValue & BOOT_FROM_EEPROM) ? _TRUE : _FALSE;
	pHalData->bautoload_fail_flag = (eeValue & EEPROM_EN) ? _FALSE : _TRUE;

	RTW_INFO("Boot from %s, Autoload %s !\n", (pHalData->EepromOrEfuse ? "EEPROM" : "EFUSE"),
		 (pHalData->bautoload_fail_flag ? "Fail" : "OK"));

	if (InitAdapterVariablesByPROM_8192FU(padapter) != _SUCCESS)
		goto exit;

	ret = _SUCCESS;

exit:
	return ret;
}

static void
_ReadRFType(PADAPTER padapter)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);

#if DISABLE_BB_RF
	pHalData->rf_chip = RF_PSEUDO_11N;
#else
	pHalData->rf_chip = RF_6052;
#endif
}

/*
 * Description:
 *    We should set Efuse cell selection to WiFi cell in default.
 * Assumption:
 *    PASSIVE_LEVEL
 */
void
hal_EfuseCellSel(PADAPTER padapter)
{
	u32 value32;

	value32 = rtw_read32(padapter, EFUSE_TEST);
	value32 = (value32 & ~EFUSE_SEL_MASK) | EFUSE_SEL(EFUSE_WIFI_SEL_0);
	rtw_write32(padapter, EFUSE_TEST, value32);
}

static u8 ReadAdapterInfo8192FU(PADAPTER padapter)
{
	u8 ret = _FAIL;

	/* Read EEPROM size before call any EEPROM function */
	padapter->EepromAddressSize = GetEEPROMSize8192F(padapter);

	hal_EfuseCellSel(padapter);

	_ReadRFType(padapter);
	if (Hal_ReadPROMContent_8192FU(padapter) != _SUCCESS)
		goto exit;

	ret = _SUCCESS;

exit:
	return ret;
}

/*
 * If variable not handled here,
 * some variables will be processed in SetHwReg8192F()
 */
u8 SetHwReg8192FU(PADAPTER padapter, u8 variable, u8 *val)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);
	u8 ret = _SUCCESS;

	switch (variable) {
	case HW_VAR_RXDMA_AGG_PG_TH:
#ifdef CONFIG_USB_RX_AGGREGATION
		{
			/* threshold == 1 , Disable Rx-agg when AP is B/G mode or wifi_spec=1 to prevent bad TP. */
		
			u8	threshold = *((u8 *)val);
			u32 agg_size;

			if (threshold == 0) {
				switch (pHalData->rxagg_mode) {
					case RX_AGG_DMA:
						agg_size = pHalData->rxagg_dma_size << 10;
						if (agg_size > RX_DMA_BOUNDARY_8192F)
							agg_size = RX_DMA_BOUNDARY_8192F >> 1;
						if ((agg_size + 2048) > MAX_RECVBUF_SZ)
							agg_size = MAX_RECVBUF_SZ - 2048;
						agg_size >>= 10; /* unit: 1K */
						if (agg_size > 0xF)
							agg_size = 0xF;

						threshold = (agg_size & 0xF);
						break;
					case RX_AGG_USB:
					case RX_AGG_MIX:
						agg_size = pHalData->rxagg_usb_size << 12;
						if ((agg_size + 2048) > MAX_RECVBUF_SZ)
							agg_size = MAX_RECVBUF_SZ - 2048;
						agg_size >>= 12; /* unit: 4K */
						if (agg_size > 0xF)
							agg_size = 0xF;

						threshold = (agg_size & 0xF);
						break;
					case RX_AGG_DISABLE:
					default:
						break;
				}
			}

			rtw_write8(padapter, REG_RXDMA_AGG_PG_TH, threshold);
			
#ifdef CONFIG_80211N_HT	
			{
				/* 2014-07-24 Fix WIFI Logo -5.2.4/5.2.9 - DT3 low TP issue */
				/* Adjust RxAggrTimeout to close to zero disable RxAggr for RxAgg-USB mode, suggested by designer */
				/* Timeout value is calculated by 34 / (2^n) */
				struct mlme_priv	*pmlmepriv = &padapter->mlmepriv;
				struct ht_priv		*phtpriv = &pmlmepriv->htpriv;

				if (pHalData->rxagg_mode == RX_AGG_USB) {
					/* BG mode || (wifi_spec=1 && BG mode Testbed)	 */
					if ((threshold == 1) && (phtpriv->ht_option == _FALSE))
						rtw_write8(padapter, REG_RXDMA_AGG_PG_TH + 1, 0);
					else
						rtw_write8(padapter, REG_RXDMA_AGG_PG_TH + 1, pHalData->rxagg_usb_timeout);
				}
			}
#endif/* CONFIG_80211N_HT */ 
		}
#endif/* CONFIG_USB_RX_AGGREGATION */
		break;

	case HW_VAR_SET_RPWM:
#ifdef CONFIG_LPS_LCLK
		{
			u8	ps_state = *((u8 *)val);
			/* rpwm value only use BIT0(clock bit) ,BIT6(Ack bit), and BIT7(Toggle bit) for 88e. */
			/* BIT0 value - 1: 32k, 0:40MHz. */
			/* BIT6 value - 1: report cpwm value after success set, 0:do not report. */
			/* BIT7 value - Toggle bit change. */
			/* modify by Thomas. 2012/4/2. */
			
			ps_state = ps_state & 0xC1;
			/* RTW_INFO("##### Change RPWM value to = %x for switch clk #####\n",ps_state); */
			rtw_write8(padapter, REG_USB_HRPWM, ps_state);
		}
#endif /* CONFIG_LPS_LCLK */
		break;

	case HW_VAR_TRIGGER_GPIO_0:
		break;
#ifdef CONFIG_GPIO_WAKEUP
	case HW_SET_GPIO_WL_CTRL: {
		struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(padapter);
		u8 enable = *((u8 *)val);

		if (pHalData->EEPROMBluetoothCoexist == _FALSE) {
			/* GPIOA_5 for 8192fu case */
			if (pwrpriv->wowlan_gpio_index != 5) {
				RTW_INFO("%s [HW_SET_GPIO_WL_CTRL]: switch gpio ctrl fail, index:%d \n"
					, __func__, pwrpriv->wowlan_gpio_index);
				break;
			}
				
			/* 0x4C[22]=0 for disable Use SW select SPDT for WL and BT */
			if (enable)
				rtw_write32(padapter, REG_LEDCFG0,
					rtw_read32(padapter, REG_LEDCFG0) & ~BIT_SW_SPDT_SEL);
			else
				rtw_write32(padapter, REG_LEDCFG0,
					rtw_read32(padapter, REG_LEDCFG0) | BIT_SW_SPDT_SEL);

			/* 0x48[16]=0 for disable External WOL function */
			rtw_write32(padapter, REG_GPIO_INTM,
				rtw_read32(padapter, REG_GPIO_INTM) & ~BIT_EXTWOL_EN);
		} else {
			/* GPIOB_4 for 8725au case */
			if (pwrpriv->wowlan_gpio_index != 36) {
				RTW_INFO("%s [HW_SET_GPIO_WL_CTRL]: switch gpio ctrl fail, index:%d \n"
					, __func__, WAKEUP_GPIO_IDX);
				break;
			}
		
			if (enable) {
				/* 0x103C[10:9]=3 for disable BIT_WLMAC_DBG_LOC */
				rtw_write32(padapter, REG_SW_GPIO_SHARE_CTRL_1,
					rtw_read32(padapter, REG_SW_GPIO_SHARE_CTRL_1) | BIT_WLMAC_DBG_LOC);

				/* 0x103C[30:31]=3 for selecting GPIO_B[7:0],GPIO_A[19:12] */
				rtw_write32(padapter, REG_SW_GPIO_SHARE_CTRL_1,
					rtw_read32(padapter, REG_SW_GPIO_SHARE_CTRL_1) | BIT_WL_GPIO_SEL);
			} else {
				rtw_write32(padapter, REG_SW_GPIO_SHARE_CTRL_1,
					rtw_read32(padapter, REG_SW_GPIO_SHARE_CTRL_1) & ~BIT_WLMAC_DBG_LOC);

				rtw_write32(padapter, REG_SW_GPIO_SHARE_CTRL_1,
					rtw_read32(padapter, REG_SW_GPIO_SHARE_CTRL_1) & (~BIT_WL_GPIO_SEL | BIT(30)));
			}
		}

		/* 0x1038[0]=1 for enable Software Control GPIO Enable */
		if (enable)
			rtw_write8(padapter, REG_SW_GPIO_SHARE_CTRL_0,
				rtw_read8(padapter, REG_SW_GPIO_SHARE_CTRL_0) | BIT_SW_GPIO_FUNC);
		else
			rtw_write8(padapter, REG_SW_GPIO_SHARE_CTRL_0,
				rtw_read8(padapter, REG_SW_GPIO_SHARE_CTRL_0) & ~BIT_SW_GPIO_FUNC);

		RTW_INFO("%s [HW_SET_GPIO_WL_CTRL] done.\n", __func__);	
		break;
	}
#endif
	default:
		ret = SetHwReg8192F(padapter, variable, val);
		break;
	}

	return ret;
}

/*
 * If variable not handled here,
 * some variables will be processed in GetHwReg8192F()
 */
void GetHwReg8192FU(PADAPTER padapter, u8 variable, u8 *val)
{
	PHAL_DATA_TYPE pHalData = GET_HAL_DATA(padapter);


	switch (variable) {
#ifdef CONFIG_LPS_LCLK
	case HW_VAR_CPWM:
		*val = rtw_read8(padapter, REG_USB_HCPWM);
		break;

	case HW_VAR_RPWM_TOG:
		*val = rtw_read8(padapter, REG_USB_HRPWM) & PS_TOGGLE;
		break;
#endif /* CONFIG_LPS_LCLK */
	default:
		GetHwReg8192F(padapter, variable, val);
		break;
	}

}

/*
 * Description:
 * Query setting of specified variable.
 */
u8
GetHalDefVar8192FUsb(
	PADAPTER padapter,
	HAL_DEF_VARIABLE eVariable,
	void *pValue
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	u8 bResult = _SUCCESS;

	switch (eVariable) {
	case HAL_DEF_IS_SUPPORT_ANT_DIV:
#ifdef CONFIG_ANTENNA_DIVERSITY
		*((u8 *)pValue) = _FALSE;
#endif
		break;

	case HAL_DEF_DRVINFO_SZ:
		*((u32 *)pValue) = DRVINFO_SZ;
		break;
	case HAL_DEF_MAX_RECVBUF_SZ:
		*((u32 *)pValue) = MAX_RECVBUF_SZ;
		break;
	case HAL_DEF_RX_PACKET_OFFSET:
		*((u32 *)pValue) = RXDESC_SIZE + DRVINFO_SZ * 8;
		break;
	case HW_VAR_MAX_RX_AMPDU_FACTOR:
		*((HT_CAP_AMPDU_FACTOR *)pValue) = MAX_AMPDU_FACTOR_64K;
		break;
	default:
		bResult = GetHalDefVar8192F(padapter, eVariable, pValue);
		break;
	}

	return bResult;
}




/*
 * Description:
 * Change default setting of specified variable.
 */
u8
SetHalDefVar8192FUsb(
	PADAPTER padapter,
	HAL_DEF_VARIABLE eVariable,
	void *pValue
)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	u8 bResult = _SUCCESS;

	switch (eVariable) {
	default:
		bResult = SetHalDefVar8192F(padapter, eVariable, pValue);
		break;
	}

	return bResult;
}

static u8 rtl8192fu_ps_func(PADAPTER padapter, HAL_INTF_PS_FUNC efunc_id, u8 *val)
{
	u8 bResult = _TRUE;

	switch (efunc_id) {

	default:
		break;
	}
	return bResult;
}

void rtl8192fu_set_hal_ops(PADAPTER padapter)
{
	struct hal_ops *pHalFunc = &padapter->hal_func;


	rtl8192f_set_hal_ops(pHalFunc);

	pHalFunc->hal_power_on = &_InitPowerOn_8192FU;
	pHalFunc->hal_power_off = &CardDisableRTL8192fu;

	pHalFunc->hal_init = &rtl8192fu_hal_init;
	pHalFunc->hal_deinit = &rtl8192fu_hal_deinit;

	pHalFunc->inirp_init = &rtl8192fu_inirp_init;
	pHalFunc->inirp_deinit = &rtl8192fu_inirp_deinit;

	pHalFunc->init_xmit_priv = &rtl8192fu_init_xmit_priv;
	pHalFunc->free_xmit_priv = &rtl8192fu_free_xmit_priv;

	pHalFunc->init_recv_priv = &rtl8192fu_init_recv_priv;
	pHalFunc->free_recv_priv = &rtl8192fu_free_recv_priv;

#ifdef CONFIG_RTW_SW_LED
	pHalFunc->InitSwLeds = &rtl8192fu_InitSwLeds;
	pHalFunc->DeInitSwLeds = &rtl8192fu_DeInitSwLeds;
#endif/*CONFIG_RTW_SW_LED */

	pHalFunc->init_default_value = &rtl8192f_init_default_value;
	pHalFunc->intf_chip_configure = &rtl8192fu_interface_configure;
	pHalFunc->read_adapter_info = &ReadAdapterInfo8192FU;

	pHalFunc->set_hw_reg_handler = &SetHwReg8192FU;
	pHalFunc->GetHwRegHandler = &GetHwReg8192FU;
	pHalFunc->get_hal_def_var_handler = &GetHalDefVar8192FUsb;
	pHalFunc->SetHalDefVarHandler = &SetHalDefVar8192FUsb;

	pHalFunc->hal_xmit = &rtl8192fu_hal_xmit;
	pHalFunc->mgnt_xmit = &rtl8192fu_mgnt_xmit;
#ifdef CONFIG_RTW_MGMT_QUEUE
	pHalFunc->hal_mgmt_xmitframe_enqueue = &rtl8192fu_hal_mgmt_xmitframe_enqueue;
#endif
	pHalFunc->hal_xmitframe_enqueue = &rtl8192fu_hal_xmitframe_enqueue;

#ifdef CONFIG_HOSTAPD_MLME
	pHalFunc->hostap_mgnt_xmit_entry = &rtl8192fu_hostap_mgnt_xmit_entry;
#endif
	pHalFunc->interface_ps_func = &rtl8192fu_ps_func;

#ifdef CONFIG_XMIT_THREAD_MODE
	pHalFunc->xmit_thread_handler = &rtl8192fu_xmit_buf_handler;
#endif
#ifdef CONFIG_SUPPORT_USB_INT
	pHalFunc->interrupt_handler = &rtl8192fu_interrupt_handler;
#endif

}
