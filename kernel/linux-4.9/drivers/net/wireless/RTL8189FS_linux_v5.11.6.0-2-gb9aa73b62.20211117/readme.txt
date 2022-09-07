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

	android_ref_codes_O_8.0/
		linux-3.0.42_STATION_INFO_ASSOC_REQ_IES.diff
			Kernel patch file for cfg80211's STATION_INFO_ASSOC_REQ_IES event for kernel 3.0.
		realtek_wifi_SDK_for_android_O_8.0_20200505.tar.gz
			This tar ball includes our android wifi reference codes for Android 8.0
		supplicant_overlay_config.tar.gz
			Supplicant_overlay config files
		Realtek_Wi-Fi_SDK_for_Android_O_8.0.pdf
			Guide for porting Realtek wifi onto your Android 8.0 system

	android_ref_codes_P_9.x/
		linux-3.0.42_STATION_INFO_ASSOC_REQ_IES.diff
			Kernel patch file for cfg80211's STATION_INFO_ASSOC_REQ_IES event for kernel 3.0.
		realtek_wifi_SDK_for_android_P_9.x_20200505.tar.gz
			This tar ball includes our android wifi reference codes for Android 9.x
		supplicant_overlay_config.tar.gz
			Supplicant_overlay config files
		Realtek_Wi-Fi_SDK_for_Android_P_9.x.pdf
			Guide for porting Realtek wifi onto your Android 9.x system

	android_ref_codes_10.x/
		realtek_wifi_SDK_for_android_10_x_20200505.tar.gz
			This tar ball includes our android wifi reference codes for Android 10.x
		supplicant_overlay_config.tar.gz
			Supplicant_overlay config files
		wpa_supplicant_8_10.x_rtw_29226.20200409.tar.gz
			wpa_supplicant_8 from Android 10.x SDK and patched by Realtek
		Realtek_Wi-Fi_SDK_for_Android_10.pdf
			Guide for porting Realtek wifi onto your Android 10.x system

	document/
		Driver_Configuration_for_RF_Regulatory_Certification.pdf
		How_to_append_vendor_specific_ie_to_driver_management_frames.pdf
		How_to_set_driver_debug_log_level.pdf
		HowTo_enable_and_verify_TDLS_function_in_Wi-Fi_driver.pdf
		HowTo_enable_driver_to_support_80211K.pdf
		HowTo_enable_the_power_saving_functionality.pdf
		HowTo_support_WIFI_certification_test.pdf
		linux_dhcp_server_notes.txt
		Quick_Start_Guide_for_Adaptivity_and_Carrier_Sensing_Test.pdf
		Quick_Start_Guide_for_Bridge.pdf
		Quick_Start_Guide_for_Driver_Compilation_and_Installation.pdf
		Quick_Start_Guide_for_SoftAP.pdf
		Quick_Start_Guide_for_Station_Mode.pdf
		Quick_Start_Guide_for_WOW.pdf
		Quick_Start_Guide_for_wpa_supplicant_WiFi_P2P_test.pdf
		Quick_Start_Guide_for_WPA3.pdf
		Realtek_WiFi_concurrent_mode_Introduction.pdf
		SoftAP_Mode_features.pdf
		Wireless_tools_porting_guide.pdf
		wpa_cli_with_wpa_supplicant.pdf

	driver/
		rtl8189FS_linux_v5.11.6.0-2-gb9aa73b62.20211117.tar.gz
			Naming rule: rtlCHIPS_linux_vM.N.P[.H]-C_gSHA-1.yyyymmdd[_COEX_VER][_beta].tar.gz
			where:
				CHIPS: supported chips
				M: Major version
				N: miNor version
				P: Patch number
				H: Hotfix number
				C: numbers of commit
				SHA-1: sha-1 of this commit
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

		wpa_supplicant_8_O_8.x_rtw-17-g894b400ab.20210716.tar.gz
			wpa_supplicant_8 from Android 8.x SDK and patched by Realtek
			could be used for pure-linux and Android 8.x. Support only cfg80211/nl80211.

		wpa_supplicant_8_P_9.x_rtw_r29226.20180827.tar.gz
			wpa_supplicant_8 from Android 9.x SDK and patched by Realtek
			could be used for Android 9.x. Support only cfg80211/nl80211.

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
					wpa_supplicant_8_O_8.x_rtw-17-g894b400ab.20210716.tar.gz
			(*) Please refer to document/linux_dhcp_server_notes.txt
==================================================================================================================
		User Guide for Wi-Fi Direct
==================================================================================================================
		Wi-Fi Direct with nl80211
			(*) Please use:
					wpa_supplicant_8_kk_4.4_rtw_r25669.20171213.tar.gz
				or
					wpa_supplicant_8_O_8.x_rtw-17-g894b400ab.20210716.tar.gz
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
					wpa_supplicant_8_O_8.x_rtw-17-g894b400ab.20210716.tar.gz
			(*) For WPS instruction/command, please refert to:
					README-WPS inside the wpa_supplicant folder of the wpa_supplicant_8 you choose
==================================================================================================================
		User Guide for Power Saving Mode
==================================================================================================================
			(*) Please refer to document/HowTo_enable_the_power_saving_functionality.pdf
==================================================================================================================
		User Guide for Applying Wi-Fi solution onto Andriod System
==================================================================================================================
			(*) For Android 1.6 ~ 2.3, 4.0, 4.1, 4.2, 4.3, 5.x, 6.x, 7.x, please contact us for further information
			(*) For Android 4.4, please refer to android_ref_codes_KK_4.4/Realtek_Wi-Fi_SDK_for_Android_KK_4.4.pdf
			(*) For Android 8.0, please refer to android_ref_codes_O_8.0/Realtek_Wi-Fi_SDK_for_Android_O_8.0.pdf
			(*) For Android 9.x, please refer to android_ref_codes_P_9.x/Realtek_Wi-Fi_SDK_for_Android_P_9.x.pdf
			(*) For Android 10.x, please refer to android_ref_codes_10.x/Realtek_Wi-Fi_SDK_for_Android_10.pdf