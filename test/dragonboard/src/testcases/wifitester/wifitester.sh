#!/bin/sh
##############################################################################
# \version	 1.0.0
# \date		2012年05月31日
# \author	  James Deng <csjamesdeng@allwinnertech.com>
# \Descriptions:
#			create the inital version

# \version	 1.1.0
# \date		2012年09月26日
# \author	  Martin <zhengjiewen@allwinnertech.com>
# \Descriptions:
#			add some new features:
#			1.wifi hotpoint ssid and single strongth san
#			2.sort the hotpoint by single strongth quickly
##############################################################################
source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="wifitester"

WIFI_PIPE=/tmp/wifi_pipe
wlan_try=0

module_count=0

for f in $(cd /vendor/etc/firmware && find -name "wifi_*.ini"); do
	ln -sf /vendor/etc/firmware/$(basename $f) /vendor/etc/$(basename $f)
done

while true; do
	module_path=$(script_fetch "wifi" "module"${idx}"_path")
	module_args=$(script_fetch "wifi" "module"${idx}"_args")
	if [ -n "$module_path" ]; then
		module_count=$((module_count+1))
		insmod $module_path $module_args 2>/dev/null
		[ $? -ne 17 ] && \
		[ $? -ne 0 ] && SEND_CMD_PIPE_FAIL $3 && exit 1
	fi
	if [ -z "$idx" ]; then
		idx=0
	else
		idx=$((idx+1))
		[ $idx -ge 20 ] && break
	fi
done

[ "$module_count" -eq 0 ] && SEND_CMD_PIPE_FAIL $3 && exit 1

flag="######"

sleep 3

success=0
if ifconfig -a | grep wlan0; then
	for i in `seq 3`; do
		ifconfig wlan0 up > /dev/null
		[ $? -eq 0 ] && success=1 && break
		LOGW "ifconfig wlan0 up failed, try again 1s later"
		sleep 1
	done
fi

if [ $success -ne 1 ]; then
	LOGE "ifconfig wlan0 up failed, no more try"
	SEND_CMD_PIPE_FAIL $3
	exit 1
fi

ap_ssid=$(script_fetch "wifi" "test_ap_ssid")
ap_pass=$(script_fetch "wifi" "test_ap_pass")

state=0

while true ; do
	wifi=$(iw wlan0 scan | awk -F"[:|=]" '(NF&&$1~/^[[:space:]]*SSID/) \
									{printf "%s:",substr($2,2)} \
									(NF&&/[[:space:]]*signal/){printf "%d\n",$2 }' \
									| sort -n -r -k 2 -t :)

	for item in $wifi; do
		[ "${item:0:2}" == ":-" ] && continue
		[ "${item:0:1}" == "-"  ] && continue
		[ "${item:$((${#item}-1)):1}" == ":" ] && continue
		str="$str $item"
		if [ -n "$(echo "$str" | grep ":-[0-9]\{1,3\}$")" ]; then
			echo $str >> $WIFI_PIPE
			str=""
		fi
	done

	echo $flag >> $WIFI_PIPE
	if [ $state -eq 0 ]; then
		if [ -n "$ap_ssid" ]; then
			LOGD "Test AP SSID: $ap_ssid"
			LOGD "Test AP PASS: $ap_pass"
			pidlist=$(ps -ef | grep wpa_supplicant | grep -v grep | awk '{print $1}')
			for pid in $pidlist; do
				kill -9 $pid
			done
			if [ -n "$ap_pass" ]; then
				wpa_passphrase "$ap_ssid" "$ap_pass" > /etc/wpa_supplicant.conf
				LOGD "Connecting to ${ap_ssid} with password ${ap_pass}..."
			else
				echo -e "network={\n\tssid=\"$ap_ssid\"\n\tkey_mgmt=NONE\n\t}" > /etc/wpa_supplicant.conf
				LOGD "Connecting to ${ap_ssid} with no password..."
			fi
			wpa_supplicant -iwlan0 -Dnl80211 -c /etc/wpa_supplicant.conf -B
			udhcpc -i wlan0
			hwaddr=$(ifconfig wlan0 | grep -o HWaddr.* | awk '{print $2}')
			ipaddr=$(ifconfig wlan0 | grep -o "inet addr:\s*[0-9.]*" | grep -o "[0-9.]\{7,\}")
			mask=$(ifconfig wlan0 | grep -o "Mask:\s*[0-9.]*" | grep -o "[0-9.]\{7,\}")
			dns=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
			gateway=$(ip route | grep "default" | grep -o "[0-9.]\{7,\}")
			speed=$(iw dev wlan0 link | grep "bitrate" | awk '{print $3" "$4}')
			LOGD "Connected to $ap_ssid"
			LOGD "MAC     [ $hwaddr ]"
			LOGD "IPADDR  [ $ipaddr ]"
			LOGD "GATEWAY [ $gateway ]"
			LOGD "MASK    [ $mask ]"
			LOGD "DNS     [ $dns ]"
			LOGD "SPEED   [ $speed ]"
			SEND_CMD_PIPE_OK_EX $3 "Connected to $ap_ssid"
		else
			LOGD "No Test AP SSID configured"
			SEND_CMD_PIPE_OK_EX $3 "No Test AP SSID configured"
		fi
		state=1
	fi

	sleep 5
done

LOGE "wlan0 not found, no more try"
SEND_CMD_PIPE_FAIL $3
exit 1
