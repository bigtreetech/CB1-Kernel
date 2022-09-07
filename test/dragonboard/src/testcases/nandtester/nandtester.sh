#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="nandtester"

#get boot start flash type:0: nand, 1:card0, 2:emmc/tsd
BOOT_TYPE=-1
for parm in $(cat /proc/cmdline) ; do
	case $parm in
		boot_type=*)
			BOOT_TYPE=`echo $parm | awk -F\= '{print $2}'`
			;;
	esac
done

if [ ${BOOT_TYPE} -eq "0" ];then  #nand boot , sdcard device mmcblk0
	LOGD "nand boot"
	SEND_CMD_PIPE_OK_EX $3
	exit 1
elif [ ${BOOT_TYPE} -eq "1" ];then #card boot ,sdcard device mmcblk0
	LOGD "card boot,boot_type = $BOOT_TYPE"
else
	LOGE "no define boot_type"
	SEND_CMD_PIPE_FAIL $3
	exit 1
fi

#only in card boot mode,it will run here
LOGD "nand test ioctl start"
nandrw "/dev/nand0"

if [ $? -ne 0 ]; then
	LOGE "nand ioctl failed"
	SEND_CMD_PIPE_FAIL_EX $3 "nand ioctl failed"
	exit 1
else
	LOGD "nand ok"
	SEND_CMD_PIPE_OK_EX $3
fi
