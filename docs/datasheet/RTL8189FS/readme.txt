===============================================================================
		Software Package - Component
===============================================================================
	android_ref_codes_KK_4.4/
		linux-3.0.42_STATION_INFO_ASSOC_REQ_IES.diff
			Kernel patch file for cfg80211's STATION_INFO_ASSOC_REQ_IES event for kernel 3.0.
		realtek_wifi_SDK_for_android_KK_4.4_20140117.tar.gz
			This tar ball includes our android wifi reference codes for Android 4.4
		Realtek_Wi-Fi_SDK_for_Android_KK_4.4.pdf
			Guide for porting Realtek wifi onto your Android 4.4 system

	android_ref_codes_L_5.x/
		linux-3.0.42_STATION_INFO_ASSOC_REQ_IES.diff
			Kernel patch file for cfg80211's STATION_INFO_ASSOC_REQ_IES event for kernel 3.0.
		realtek_wifi_SDK_for_android_L_5.x_20150811.tgz
			This tar ball includes our android wifi reference codes for Android 5.x
		Realtek_Wi-Fi_SDK_for_Android_L_5.x.pdf
			Guide for porting Realtek wifi onto your Android 5.x system

	document/
		Driver_Configuration_for_RF_Regulatory_Certification_V1.pdf
		HowTo_enable_driver_to_support_80211d.pdf
		HowTo_enable_driver_to_support_WIFI_certification_test.pdf
		HowTo_enable_the_power_saving_functionality.pdf
		linux_dhcp_server_notes.txt
		Quick_Start_Guide_for_Adaptivity_and_Carrier_Sensing_Test.pdf
		Quick_Start_Guide_for_Bridge.pdf
		Quick_Start_Guide_for_Driver_Compilation_and_Installation.pdf
		Quick_Start_Guide_for_SoftAP.pdf
		Quick_Start_Guide_for_Station_Mode.pdf
		Quick_Start_Guide_for_WOW.pdf
		Realtek_WiFi_concurrent_mode_Introduction.pdf
		SoftAP_Mode_features.pdf
		Wireless_tools_porting_guide.pdf
		wpa_cli_with_wpa_supplicant.pdf

	driver/
		rtl8189FS_linux_v5.3.12_28613.20180703.tar.gz
			Naming rule: rtlCHIPS_linux_vM.N.P[.H]_sssss.yyyymmdd[_COEX_VER][_beta].tar.gz
			where:
				CHIPS: supported chips
				M: Major version
				N: miNor version
				P: Patch number
				H: Hotfix number
				s: SVN number
				y: package year
				m: package month
				d: package day
				COEX_VER: Coext version
				_beta: beta driver

	wireless_tools/
		wireless_tools.30.rtl.tar.gz

	wpa_supplicant_hostapd/
		wpa_supplicant_hostapd-0.8_rtw_r24647.20171025.tar.gz
			wpa_supplicant
				The tool help the wlan network to communicate under the
				protection of WPAPSK mechanism (WPA/WPA2) and add WPS patch
			hostapd
		wpa_0_8.conf
			Configure file sample for wpa_supplicant-0.8
		rtl_hostapd_2G.conf
		rtl_hostapd_5G.conf
			Configure files for Soft-AP mode 2.4G/5G
		p2p_hostapd.conf
			Configure file for hostapd for Wi-Fi Direct (P2P)

		wpa_supplicant_8_kk_4.4_rtw_r25669.20171213.tar.gz
			wpa_supplicant_8 from Android 4.4 SDK and patched by Realtek
			could be used for pure-linux and Android 4.4. Support only cfg80211/nl80211.

		wpa_supplicant_8_L_5.x_rtw_r24600.20171025.tar.gz
			wpa_supplicant_8 from Android 5.x SDK and patched by Realtek
			could be used for pure-linux and Android 5.x. Support only cfg80211/nl80211.
	
	install.sh
		Script to compile and install WiFi driver easily in PC-Linux

	ReleaseNotes.pdf

==================================================================================================================
		User Guide for Driver compilation and installation
==================================================================================================================
			(*) Please refer to document/Quick_Start_Guide_for_Driver_Compilation_and_Installation.pdf
==================================================================================================================
		User Guide for Station mode
==================================================================================================================
			(*) Please refer to document/Quick_Start_Guide_for_Station_Mode.pdf
==================================================================================================================
		User Guide for Soft-AP mode
==================================================================================================================
			(*) Please refer to document/Quick_Start_Guide_for_SoftAP.pdf
			Soft-AP mode with Realtek's rtl871xdrv
				(*) Please use wpa_supplicant_hostapd-0.8_rtw_r24647.20171025.tar.gz
			Soft-AP mode with nl80211
				(*) Please use:
					wpa_supplicant_8_kk_4.4_rtw_r25669.20171213.tar.gz
				or
					wpa_supplicant_8_L_5.x_rtw_r24600.20171025.tar.gz
			(*) Please refer to document/linux_dhcp_server_notes.txt
==================================================================================================================
		User Guide for Wi-Fi Direct
==================================================================================================================
		Wi-Fi Direct with nl80211
			(*) Please use:
					wpa_supplicant_8_kk_4.4_rtw_r25669.20171213.tar.gz
				or
					wpa_supplicant_8_L_5.x_rtw_r24600.20171025.tar.gz
			(*) For P2P instruction/command, please refer to:
					README-P2P inside the wpa_supplicant folder of the wpa_supplicant_8 you choose
			(*) For DHCP server, please refer to:
					document/linux_dhcp_server_notes.txt
==================================================================================================================
		User Guide for WPS2.0
==================================================================================================================
			(*) Please use:
					wpa_supplicant_hostapd-0.8_rtw_r24647.20171025.tar.gz
				or
					wpa_supplicant_8_kk_4.4_rtw_r25669.20171213.tar.gz
				or
					wpa_supplicant_8_L_5.x_rtw_r24600.20171025.tar.gz
			(*) For WPS instruction/command, please refert to:
					README-WPS inside the wpa_supplicant folder of the wpa_supplicant_8 you choose
==================================================================================================================
		User Guide for Power Saving Mode
==================================================================================================================
			(*) Please refer to document/HowTo_enable_the_power_saving_functionality.pdf
==================================================================================================================
		User Guide for Applying Wi-Fi solution onto Andriod System
==================================================================================================================
			(*) For Android 1.6 ~ 2.3, 4.0, 4.1, 4.2, 4.3, please contact us for further information
			(*) For Android 4.4, please refer to android_ref_codes_KK_4.4/Realtek_Wi-Fi_SDK_for_Android_KK_4.4.pdf
			(*) For Android 5.x, please refer to android_ref_codes_L_5.x/Realtek_Wi-Fi_SDK_for_Android_L_5.x.pdf
