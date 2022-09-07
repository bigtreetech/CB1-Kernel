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
/* ************************************************************
 * Description:
 *
 * This file is for 92CE/92CU dynamic mechanism only
 *
 *
 * ************************************************************ */
#define _RTL8192F_DM_C_

/* ************************************************************
 * include files
 * ************************************************************ */
#include <rtl8192f_hal.h>

/* ************************************************************
 * Global var
 * ************************************************************ */
#ifdef CONFIG_SUPPORT_HW_WPS_PBC
static void dm_CheckPbcGPIO(_adapter *padapter)
{
	u8	tmp1byte;
	u8	bPbcPressed = _FALSE;

	if (!padapter->registrypriv.hw_wps_pbc)
		return;

#ifdef CONFIG_USB_HCI
	tmp1byte = rtw_read8(padapter, GPIO_IO_SEL);
	tmp1byte |= (HAL_8192C_HW_GPIO_WPS_BIT);
	rtw_write8(padapter, GPIO_IO_SEL, tmp1byte);	/* enable GPIO[2] as output mode */

	tmp1byte &= ~(HAL_8192C_HW_GPIO_WPS_BIT);
	rtw_write8(padapter,  GPIO_IN, tmp1byte);		/* reset the floating voltage level */

	tmp1byte = rtw_read8(padapter, GPIO_IO_SEL);
	tmp1byte &= ~(HAL_8192C_HW_GPIO_WPS_BIT);
	rtw_write8(padapter, GPIO_IO_SEL, tmp1byte);	/* enable GPIO[2] as input mode */

	tmp1byte = rtw_read8(padapter, GPIO_IN);

	if (tmp1byte == 0xff)
		return;

	if (tmp1byte & HAL_8192C_HW_GPIO_WPS_BIT)
		bPbcPressed = _TRUE;
#else
	tmp1byte = rtw_read8(padapter, GPIO_IN);

	if (tmp1byte == 0xff || padapter->init_adpt_in_progress)
		return;

	if ((tmp1byte & HAL_8192C_HW_GPIO_WPS_BIT) == 0)
		bPbcPressed = _TRUE;
#endif

	if (_TRUE == bPbcPressed) {
		/* Here we only set bPbcPressed to true */
		/* After trigger PBC, the variable will be set to false */
		RTW_INFO("CheckPbcGPIO - PBC is pressed\n");
		rtw_request_wps_pbc_event(padapter);
	}
}
#endif /* #ifdef CONFIG_SUPPORT_HW_WPS_PBC */


#ifdef CONFIG_PCI_HCI
/*
 *	Description:
 *		Perform interrupt migration dynamically to reduce CPU utilization.
 *
 *	Assumption:
 *		1. Do not enable migration under WIFI test.
 *
 *	Created by Roger, 2010.03.05.
 *   */
void
dm_InterruptMigration(
		PADAPTER	Adapter
)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	struct mlme_priv	*pmlmepriv = &(Adapter->mlmepriv);
	BOOLEAN			bCurrentIntMt, bCurrentACIntDisable;
	BOOLEAN			IntMtToSet = _FALSE;
	BOOLEAN			ACIntToSet = _FALSE;


	/* Retrieve current interrupt migration and Tx four ACs IMR settings first. */
	bCurrentIntMt = pHalData->bInterruptMigration;
	bCurrentACIntDisable = pHalData->bDisableTxInt;

	/* */
	/* <Roger_Notes> Currently we use busy traffic for reference instead of RxIntOK counts to prevent non-linear Rx statistics */
	/* when interrupt migration is set before. 2010.03.05. */
	/* */
	if (!Adapter->registrypriv.wifi_spec &&
	    (check_fwstate(pmlmepriv, WIFI_ASOC_STATE) == _TRUE) &&
	    pmlmepriv->LinkDetectInfo.bHigherBusyTraffic) {
		IntMtToSet = _TRUE;

		/* To check whether we should disable Tx interrupt or not. */
		if (pmlmepriv->LinkDetectInfo.bHigherBusyRxTraffic)
			ACIntToSet = _TRUE;
	}

	/* Update current settings. */
	if (bCurrentIntMt != IntMtToSet) {
		RTW_INFO("%s(): Update interrupt migration(%d)\n", __FUNCTION__, IntMtToSet);
		if (IntMtToSet) {
			/* */
			/* <Roger_Notes> Set interrupt migration timer and corresponging Tx/Rx counter. */
			/* timer 25ns*0xfa0=100us for 0xf packets. */
			/* 2010.03.05. */
			/* */
			rtw_write32(Adapter, REG_INT_MIG_8192F, 0xff000fa0);/* 0x306:Rx, 0x307:Tx */
			pHalData->bInterruptMigration = IntMtToSet;
		} else {
			/* Reset all interrupt migration settings. */
			rtw_write32(Adapter, REG_INT_MIG_8192F, 0);
			pHalData->bInterruptMigration = IntMtToSet;
		}
	}

	/*if( bCurrentACIntDisable != ACIntToSet ){
		RTW_INFO("%s(): Update AC interrupt(%d)\n",__FUNCTION__,ACIntToSet);
		if(ACIntToSet)
		{





			UpdateInterruptMask8192CE( Adapter, 0, RT_AC_INT_MASKS );
			pHalData->bDisableTxInt = ACIntToSet;
		}
		else
		{
			UpdateInterruptMask8192CE( Adapter, RT_AC_INT_MASKS, 0 );
			pHalData->bDisableTxInt = ACIntToSet;
		}
	}*/

}

#endif

/*
 * Initialize GPIO setting registers
 *   */
#ifdef CONFIG_USB_HCI
static void
dm_InitGPIOSetting(
		PADAPTER	Adapter
)
{
	PHAL_DATA_TYPE		pHalData = GET_HAL_DATA(Adapter);

	u8	tmp1byte;

	tmp1byte = rtw_read8(Adapter, REG_GPIO_MUXCFG);
	tmp1byte &= (GPIOSEL_GPIO | ~GPIOSEL_ENBT);

	rtw_write8(Adapter, REG_GPIO_MUXCFG, tmp1byte);
}
#endif
/* ************************************************************
 * functions
 * ************************************************************ */
static void Init_ODM_ComInfo_8192f(PADAPTER	Adapter)
{
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(Adapter);
	struct dm_struct		*pDM_Odm = &(pHalData->odmpriv);
	u8	cut_ver, fab_ver;

	Init_ODM_ComInfo(Adapter);

	odm_cmn_info_init(pDM_Odm, ODM_CMNINFO_PACKAGE_TYPE, pHalData->PackageType);

	fab_ver = ODM_TSMC;
	cut_ver = GET_CVID_CUT_VERSION(pHalData->version_id);

	RTW_INFO("%s(): Fv=%d Cv=%d\n", __func__, fab_ver, cut_ver);
	odm_cmn_info_init(pDM_Odm, ODM_CMNINFO_FAB_VER, fab_ver);
	odm_cmn_info_init(pDM_Odm, ODM_CMNINFO_CUT_VER, cut_ver);
}

#if defined (CONFIG_CONCURRENT_MODE)  && defined (CONFIG_TSF_SYNC)
/*suggest by RS yx_qi to resolve  NVR TBTT conflict of AP mode while concurrent is defined */
void rtl8192f_sync_tsfr(_adapter *Adapter) {

	struct mlme_priv *pmlmepriv;
	struct dvobj_priv *dvobj = adapter_to_dvobj(Adapter);
	boolean need_sync = _FALSE, ap_mode = _FALSE, sta_linked = _FALSE;
	u8 value8;
	enum _hw_port hwport = 0xFF;
	u16 sync_period = 600;
	u16 watchdog_period = 2;
	int i;
	_adapter *iface = NULL;
	pmlmepriv = &Adapter->mlmepriv;

	if(dvobj->sync_tsfr_counter == 0)
		dvobj->sync_tsfr_counter++;
	else if(dvobj->sync_tsfr_counter > (sync_period/watchdog_period)) {
		dvobj->sync_tsfr_counter = 0;
		goto exit;
	} else {
		dvobj->sync_tsfr_counter++;
		goto exit;
	}

	/*check any iface is ap or not*/
	for (i = 0; i < dvobj->iface_nums; i++) {
		iface = dvobj->padapters[i];
		if (MLME_IS_AP(iface) || MLME_IS_MESH(iface)) {
			ap_mode = _TRUE;
			hwport = get_hw_port(iface);
			/*RTW_INFO("%s "ADPT_FMT"- hw port(%d)  AP mode\n", __func__, ADPT_ARG(iface), hwport);*/
		} else if (MLME_IS_STA(iface) && (check_fwstate(&iface->mlmepriv, WIFI_ASOC_STATE) == _TRUE))
			sta_linked = _TRUE;
	}

	if(ap_mode == _FALSE){
		dvobj->sync_tsfr_counter = 0;
		goto exit;
	} else if((ap_mode == _TRUE) && (sta_linked ==  _TRUE))
		need_sync = _TRUE;

	/*RTW_DBG("%s - hw port(%d) dvobj->sync_tsfr1_counter=%u ap_mode=%d sta_linked=%d\n", 
		__func__,
		hwport,
		dvobj->sync_tsfr_counter,
		ap_mode,
		sta_linked);*/
	/*one of iface is ap mode ,counter =0 ,sync setting*/
	if(need_sync == _TRUE) {
		if(Adapter == NULL) {
			RTW_INFO("%s -Adapter = NULL, need_sync=_TRUE!!! \n", __func__);
			goto exit;
		}

		rtw_write16(Adapter, REG_TSFTR_SYN_OFFSET, 0x640);
	}
	value8 = rtw_read8(Adapter, REG_DUAL_TSF_RST) ;
	if(hwport == HW_PORT0)
		value8 = value8 | BIT2 ;
	else if(hwport==HW_PORT1)
		value8 = value8 | BIT3;
	rtw_write8(Adapter, REG_DUAL_TSF_RST, value8);
	/*RTW_INFO("%s  sync tsf 553=0x%x\n",  __func__,rtw_read8(iface, REG_DUAL_TSF_RST) );*/


exit:
	return;
}
#endif

void
rtl8192f_InitHalDm(
		PADAPTER	Adapter
)
{
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(Adapter);
	struct dm_struct		*pDM_Odm = &(pHalData->odmpriv);

#ifdef CONFIG_USB_HCI
	dm_InitGPIOSetting(Adapter);
#endif
	rtw_phydm_init(Adapter);

}

void
rtl8192f_HalDmWatchDog(
		PADAPTER	Adapter
)
{
	BOOLEAN		bFwCurrentInPSMode = _FALSE;
	u8 bFwPSAwake = _TRUE;
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(Adapter);
	struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(Adapter);
	u8 in_lps = _FALSE;

#ifdef CONFIG_MP_INCLUDED
	/* #if MP_DRIVER */
	if (Adapter->registrypriv.mp_mode == 1 && Adapter->mppriv.mp_dm == 0) /* for MP power tracking */
		return;
	/* #endif */
#endif

	if (!rtw_is_hw_init_completed(Adapter))
		goto skip_dm;

#ifdef CONFIG_LPS
	bFwCurrentInPSMode = pwrpriv->bFwCurrentInPSMode;
	rtw_hal_get_hwreg(Adapter, HW_VAR_FWLPS_RF_ON, &bFwPSAwake);
#endif

#ifdef CONFIG_P2P
	/* Fw is under p2p powersaving mode, driver should stop dynamic mechanism. */
	/* modifed by thomas. 2011.06.11. */
	if (Adapter->wdinfo.p2p_ps_mode)
		bFwPSAwake = _FALSE;
#endif /* CONFIG_P2P */


	if ((rtw_is_hw_init_completed(Adapter))
	    && ((!bFwCurrentInPSMode) && bFwPSAwake)) {

		rtw_hal_check_rxfifo_full(Adapter);
		/* */
		/* Dynamically switch RTS/CTS protection. */
		/* */

#ifdef CONFIG_PCI_HCI
		/* 20100630 Joseph: Disable Interrupt Migration mechanism temporarily because it degrades Rx throughput. */
		/* Tx Migration settings. */
		/* dm_InterruptMigration(Adapter); */

		/* if(Adapter->HalFunc.TxCheckStuckHandler(Adapter)) */
		/*	PlatformScheduleWorkItem(&(GET_HAL_DATA(Adapter)->HalResetWorkItem)); */
#endif
	}

#ifdef CONFIG_DISABLE_ODM
	goto skip_dm;
#endif

#ifdef CONFIG_LPS
	if (pwrpriv->bLeisurePs && bFwCurrentInPSMode && pwrpriv->pwr_mode != PS_MODE_ACTIVE)
		in_lps = _TRUE;
#endif

	rtw_phydm_watchdog(Adapter, in_lps);

skip_dm:

	/* Check GPIO to determine current RF on/off and Pbc status. */
	/* Check Hardware Radio ON/OFF or not */
	/* if(Adapter->MgntInfo.PowerSaveControl.bGpioRfSw) */
	/* { */
	/* RTPRINT(FPWR, PWRHW, ("dm_CheckRfCtrlGPIO\n")); */
	/*	dm_CheckRfCtrlGPIO(Adapter); */
	/* } */
#ifdef CONFIG_SUPPORT_HW_WPS_PBC
	dm_CheckPbcGPIO(Adapter);
#endif

#if defined (CONFIG_CONCURRENT_MODE)  && defined (CONFIG_TSF_SYNC)
	rtl8192f_sync_tsfr(Adapter);
#endif/*defined (CONFIG_CONCURRENT_MODE)  && defined (CONFIG_TSF_SYNC)*/

	return;
}

void rtl8192f_init_dm_priv(PADAPTER Adapter)
{
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(Adapter);
	struct dm_struct		*podmpriv = &pHalData->odmpriv;

	Init_ODM_ComInfo_8192f(Adapter);
	odm_init_all_timers(podmpriv);

}

void rtl8192f_deinit_dm_priv(PADAPTER Adapter)
{
	PHAL_DATA_TYPE	pHalData = GET_HAL_DATA(Adapter);
	struct dm_struct		*podmpriv = &pHalData->odmpriv;

	odm_cancel_all_timers(podmpriv);

}
