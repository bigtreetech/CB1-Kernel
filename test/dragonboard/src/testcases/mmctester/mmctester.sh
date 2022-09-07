#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="mmctester"

sdcard_flash="/dev/null"
capacity=0
nr=-1

###step 1:get boot type########
#get boot start flash type:0: nand, 1:card0, 2:emmc/tsd
BOOT_TYPE=-1
for parm in $(cat /proc/cmdline) ; do
	case $parm in
		boot_type=*)
			BOOT_TYPE=`echo $parm | awk -F\= '{print $2}'`
			;;
	esac
done

###step 2:get sdcard devie name########
if [ ${BOOT_TYPE} -eq "1" ];then  #card boot ,sdcard device mmcblk0
	nr=0
elif [ ${BOOT_TYPE} -eq "2" ];then #emmc boot ,  sdcard device mmcblk1
	nr=1
elif [ ${BOOT_TYPE} -eq "0" ];then #nand boot , sdcard device mmcblk0
	nr=0
else
	LOGE "no define boot_type"
	SEND_CMD_PIPE_FAIL $3
	exit 1
fi
###step3:wait sdcard insert########
while true; do
	if [ -b "/dev/block/mmcblk$nr" -o -b "/dev/mmcblk$nr" ]; then
		LOGD "card0 insert"
		break
	else
		sleep 1
	fi
done

if [ -b "/dev/block/mmcblk$nr" ];then
	sdcard_flash="/dev/block/mmcblk$nr"
elif [ -b "/dev/mmcblk$nr" ];then
	sdcard_flash="/dev/mmcblk$nr"
else
	LOGE "can not find the sdcard device"
	SEND_CMD_PIPE_FAIL $3
	exit 1
fi

LOGD "sdcard_flash=$sdcard_flash"

###step 4: get the  sdcard capacity########
if [ $nr  -eq "1" ];then
	capacity=`cat /sys/block/mmcblk1/size|grep -o '\<[0-9]\{1,\}.[0-9]\{1,\}\>'|awk '{sum+=$0}END{printf "%.2f\n",sum/2097152}'`
elif [ $nr  -eq "0" ];then
	capacity=`cat /sys/block/mmcblk0/size|grep -o '\<[0-9]\{1,\}.[0-9]\{1,\}\>'|awk '{sum+=$0}END{printf "%.2f\n",sum/2097152}'`
fi

have_realnum=`echo $capacity | grep -E [1-9]`
if [ -n "$have_realnum" ];then
	SEND_CMD_PIPE_OK_EX $3 "$capacity"G""
	LOGD "get sdcard size:$capacity"
	exit 0
else
	LOGE "get sdcard capacity fail"
	SEND_CMD_PIPE_FAIL $3
	exit 1
fi
