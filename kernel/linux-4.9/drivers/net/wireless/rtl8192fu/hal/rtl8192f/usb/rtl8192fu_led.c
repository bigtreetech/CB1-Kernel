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
#include "rtl8192f_hal.h"
#ifdef CONFIG_RTW_SW_LED

/*
 * ================================================================================
 * LED object.
 * ================================================================================
 */


/*
 * ================================================================================
 * Prototype of protected function.
 * ================================================================================
 */

/*
 * ================================================================================
 * LED_819xUsb routines.
 * ================================================================================
 */

/* LED0 is controlled by GPIOA_0 */
u8 rtl8192fu_CfgLed0Hw(PADAPTER padapter)
{
	struct led_priv *ledpriv = adapter_to_led(padapter);
	u32 reg_led_cfg = 0;
	u32 reg_gpio_muxcfg = 0;
	u32 reg_gpio_share_ctrl_0 = 0;
	u8 sw_ctrl_led = _FALSE;

	if (ledpriv->LedStrategy == HW_LED)
		goto led_ctrl_type;

	reg_led_cfg = rtw_read32(padapter, REG_LEDCFG0);
	reg_gpio_muxcfg = rtw_read32(padapter, REG_GPIO_MUXCFG);
	reg_gpio_share_ctrl_0 = rtw_read32(padapter, REG_SW_GPIO_SHARE_CTRL_0);

	/* set LED0 GPIO enable */
	reg_led_cfg |= LED0_GPIO_ENABLE_8192FU;
	/* set LED0 to software control */
	reg_led_cfg &= ~(BIT0|BIT1|BIT2);
	/* set LED0 to output mode*/
	reg_led_cfg &= ~(LED0_DISABLE_ANALOGSIGNAL_8192FU);
	/* set the usage of LED0 for Wi-Fi */
	reg_gpio_muxcfg |= ENABLE_LED0_AND_LED1_CTRL_BY_WIFI_8192FU;
	/* set the controlled location of LED0 */
	reg_gpio_share_ctrl_0 &= ~(LED_OUTPUT_PIN_LOCATION_8192FU);
		
	rtw_write32(padapter, REG_LEDCFG0, reg_led_cfg);
	rtw_write32(padapter, REG_GPIO_MUXCFG, reg_gpio_muxcfg);
	rtw_write32(padapter, REG_SW_GPIO_SHARE_CTRL_0, reg_gpio_share_ctrl_0);

	sw_ctrl_led = _TRUE;

led_ctrl_type:
	return sw_ctrl_led;
}

/*
 * Description:
 * Turn on LED according to LedPin specified.
 */
void
SwLedOn_8192FU(
	PADAPTER padapter,
	PLED_USB pLed
)
{
	u32 reg_led_cfg = 0;

	if (RTW_CANNOT_RUN(padapter))
		return;
	
	switch (pLed->LedPin) {
	case LED_PIN_LED0:

		/* configure HW GPIO for LED0 */
		if(rtl8192fu_CfgLed0Hw(padapter)) {
			/* set LED0 to turn on */
			reg_led_cfg = rtw_read32(padapter, REG_LEDCFG0);
			reg_led_cfg |= LED0_SW_VALUE_8192FU;
			rtw_write32(padapter, REG_LEDCFG0, reg_led_cfg);
		}
		break;

	case LED_PIN_LED1:
		break;

	default:
		break;
	}
	pLed->bLedOn = _TRUE;

}


/*
 * Description:
 * Turn off LED according to LedPin specified.
 */
void
SwLedOff_8192FU(
	PADAPTER padapter,
	PLED_USB pLed
)
{
	u32 reg_led_cfg = 0;

	if (RTW_CANNOT_RUN(padapter))
		goto exit;

	switch (pLed->LedPin) {
	case LED_PIN_LED0:

		/* configure HW GPIO for LED0 */
		if(rtl8192fu_CfgLed0Hw(padapter)) {
			/* set LED0 to turn off */
			reg_led_cfg = rtw_read32(padapter, REG_LEDCFG0);
			reg_led_cfg &= ~(LED0_SW_VALUE_8192FU);
			rtw_write32(padapter, REG_LEDCFG0, reg_led_cfg);
		}
		break;

	case LED_PIN_LED1:
		break;

	default:
		break;
	}

exit:
	pLed->bLedOn = _FALSE;

}

/*
 * ================================================================================
 * Interface to manipulate LED objects.
 * ================================================================================
 */

/*
 * ================================================================================
 * Default LED behavior.
 * ================================================================================
 */

/*
 * Description:
 * Initialize all LED_871x objects.
 */
void
rtl8192fu_InitSwLeds(
	PADAPTER padapter
)
{
	struct led_priv *pledpriv = adapter_to_led(padapter);

	pledpriv->LedControlHandler = LedControlUSB;

	pledpriv->SwLedOn = SwLedOn_8192FU;
	pledpriv->SwLedOff = SwLedOff_8192FU;

	InitLed(padapter, &(pledpriv->SwLed0), LED_PIN_LED0);
	InitLed(padapter, &(pledpriv->SwLed1), LED_PIN_LED1);
}


/*
 * Description:
 * DeInitialize all LED_819xUsb objects.
 */
void
rtl8192fu_DeInitSwLeds(
	PADAPTER padapter
)
{
	struct led_priv *ledpriv = adapter_to_led(padapter);

	DeInitLed(&(ledpriv->SwLed0));
	DeInitLed(&(ledpriv->SwLed1));
}
#endif
