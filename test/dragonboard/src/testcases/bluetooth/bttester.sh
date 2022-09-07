#!/bin/bash
##############################################################################
# \version     1.0.0
# \date        2012年05月31日
# \author      James Deng <csjamesdeng@allwinnertech.com>
# \Descriptions:
#			create the inital version

# \version     1.1.0
# \date        2012年09月26日
# \author      Martin <zhengjiewen@allwinnertech.com>
# \Descriptions:
#			add some new features:
#			1.wifi hotpoint ssid and single strongth san
#			2.sort the hotpoint by single strongth quickly
##############################################################################
source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="bttester"

module_path=`script_fetch "bluetooth" "module_path"`
loop_time=`script_fetch "bluetooth" "test_time"`
destination_bt=`script_fetch "bluetooth" "dst_bt"`
device_node=`script_fetch "bluetooth" "device_node"`
baud_rate=`script_fetch "bluetooth" "baud_rate"`
bt_vnd=`script_fetch "bluetooth" "bt_vnd"`

LOGD "module_path   : "$module_path
LOGD "loop_time     : "$loop_time
LOGD "destination_bt: "$destination_bt
LOGD "device_node   : "$device_node
LOGD "baud_rate     : "$baud_rate
LOGD "bt_vnd        : "$bt_vnd

if [ -z "$bt_vnd" ]; then
	bt_vnd="broadcom"
	LOGW "Warning: bt_vnd in test_config.fex is not set, use defaut: broadcom"
fi

if [ -z "$baud_rate" ]; then
	baud_rate="115200"
	LOGW "Warning: baud_rate in test_config.fex is not set, use defaut: 115200"
fi

if [ "X_$bt_vnd" == "X_realtek" ]; then
	cfgpgm=hciattach
	cfgpara="-n -s $baud_rate $device_node rtk_h5"
	tsleep=10
elif [ "X_$bt_vnd" == "X_broadcom" ]; then
	cfgpgm=brcm_patchram_plus
	cfgpara="--tosleep=50000 --no2bytes --enable_hci --scopcm=0,2,0,0,0,0,0,0,0,0 --baudrate $baud_rate --patchram $module_path $device_node"
	tsleep=20
elif [ "x$bt_vnd" == "xxradio" ]; then
	[ ! -d /proc/bluetooth/sleep ] && insmod /vendor/modules/xradio_btlpm.ko
	echo 1 > /proc/bluetooth/sleep/btwake
	cfgpgm=hciattach
	cfgpara="-n -s $baud_rate $device_node xradio"
	tsleep=10
elif [ "x$bt_vnd" == "xsprd" ]; then
	for f in $(cd /vendor/etc/firmware && find -name "bt_configure_*.ini"); do
		ln -sf /vendor/etc/firmware/$(basename $f) /vendor/etc/$(basename $f)
	done
	insmod /vendor/modules/uwe5622_bsp_sdio.ko 2>/dev/null
	insmod /vendor/modules/sprdbt_tty.ko 2>/dev/null
	rfkillpath="/sys/devices/platform/mtty/rfkill"
	cfgpgm=hciattach
	cfgpara="-n -s $baud_rate $device_node sprd"
	tsleep=10
elif [ "x$bt_vnd" == "xaic" ]; then
	insmod /vendor/modules/aic8800_bsp.ko 2>/dev/null
	insmod /vendor/modules/aic8800_btsdio.ko 2>/dev/null
	rfkillpath="/sys/devices/platform/aic-bsp/rfkill"
	cfgpgm=hciattach
	LOGW "Force baudrate 1500000 for aic bluetooth"
	baud_rate=1500000
	cfgpara="-n -s $baud_rate $device_node aic"
	tsleep=10
else
	LOGE "Unsuport bt_vnd, exit."
	exit 1
fi

rfkillpath="/sys/class/rfkill"
for d in $(find $rfkillpath -maxdepth 1 -name "rfkill[0-9]*"); do
	if [ "$(cat $d/type)" == "bluetooth" ] && [ "$(cat $d/name)" != "hci0" ]; then
		power_node=$(basename $d)
	fi
done

for ((tryloop=0;tryloop<3;tryloop++)); do

	pid=`ps | awk '{print $1" "$3}' | grep $cfgpgm`
	for kp in `echo "$pid" | awk '{print $1}'`; do
		kill -9 $kp
	done

	echo 0 > $rfkillpath/$power_node/state
	echo 1 > $rfkillpath/$power_node/state
	usleep 500000

	$cfgpgm $cfgpara > /tmp/.btcfglog 2>&1 &
	success=0
	for ((wt=0;wt<$tsleep;wt++)); do
		if [ -n "$(grep "Done setting line discpline" /tmp/.btcfglog)" ] \
		|| [ -n "$(grep "Realtek Bluetooth post process" /tmp/.btcfglog)" ]; then
			success=1
			break
		fi
		sleep 1
	done

	if [ $success -eq 0 ]; then
		LOGE "Firmware load timeout, time: ${wt}s"
	else
		LOGD "Firmware load finish, time: ${wt}s"
		sleep 1
	fi

	success=0
	for node in `ls $rfkillpath`; do
		devtype="$(cat $rfkillpath/$node/type)"
		devname="$(cat $rfkillpath/$node/name)"
		if [ "X_$devtype" == "X_bluetooth" ] && [ "X_$devname" == "X_hci0" ]; then
			LOGD "hci0 state path: $rfkillpath/$node"
			echo 1 > $rfkillpath/$node/state
			success=1
			break 2
		fi
	done
done

if [ $success -eq 0 ]; then
	SEND_CMD_PIPE_FAIL_EX $3 "Bluetooth can't find hci port."
	LOGE "Bluetooth can't find hci port."
	exit 1
fi

hciconfig hci0 up

if [ $? -ne 0 ]; then
	SEND_CMD_PIPE_FAIL_EX $3 "Bluetooth can't bring up hci port."
	LOGE "Bluetooth can't bring up hci port."
	exit 1
fi

for((i=1;i<=${loop_time} ;i++));  do
	devlist=`hcitool scan | grep "${destination_bt}"`
	if [ ! -z "$devlist" ]; then
		LOGD "Bluetooth found devices list:"
		for((i=1;i<=$(echo "$devlist" | wc -l);i++)); do
			mac="$(echo  "$devlist" | sed -n "${i}p" | grep -o "^\s*[0-9a-fA-F:]\{17\}" | sed 's/^\s\+//g')"
			name="$(echo "$devlist" | sed -n "${i}p" | grep "^\s*[0-9a-fA-F:]\{17\}"    | sed 's/^\s*[0-9a-fA-F:]\{17\}\s\+//g')"
			line="MAC [ $mac ]    Name [ $name ]"
			[ -n "${mac}${name}" ] && LOGD "$line"
		done
		devlist=`echo "$devlist" | sed -e 's/^\s*[0-9a-fA-F:]\{17\}\s\+//g' -e 's/^/[/g' -e 's/$/]/g'`
		devlist=`echo $devlist`
		SEND_CMD_PIPE_OK_EX $3 "List: $devlist"
		hciconfig hci0 down
		exit 1
	fi
done

SEND_CMD_PIPE_FAIL_EX $3 "Bluetooth OK, but no device found."
LOGW "Bluetooth OK, but no device found."
