/******************************************************************************
 *
 * Copyright(c) 2013 Realtek Corporation. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
/*
 * Description:
 *	This file can be applied to following platforms:
 *	CONFIG_PLATFORM_ARM_SUNXI Series platform
 *
 */
#include <drv_types.h>

extern int sunxi_usb_disable_hcd(__u32 usbc_no);
extern int sunxi_usb_enable_hcd(__u32 usbc_no);
extern int sunxi_wlan_get_bus_index(void);
extern void sunxi_wlan_set_power(int on);

int platform_wifi_power_on(void)
{
	int ret = 0;
	int wlan_bus_index = sunxi_wlan_get_bus_index();
	if (wlan_bus_index < 0)
		return wlan_bus_index;
	sunxi_wlan_set_power(1);
	mdelay(100);
	sunxi_usb_enable_hcd(wlan_bus_index);
	return ret;
}

int platform_wifi_power_off(void)
{
	int ret = 0;
	int wlan_bus_index = sunxi_wlan_get_bus_index();
	if (wlan_bus_index < 0)
		return wlan_bus_index;
	sunxi_usb_disable_hcd(wlan_bus_index);
	sunxi_wlan_set_power(0);
	return ret;
}

