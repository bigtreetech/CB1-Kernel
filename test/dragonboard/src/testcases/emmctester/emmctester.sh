#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="emmctester"

ROOT_DEVICE=/dev/nandd
for parm in $(cat /proc/cmdline); do
	case $parm in
		root=*)
			ROOT_DEVICE=`echo $parm | awk -F\= '{print $2}'`
			;;
	esac
done

BOOT_TYPE=-1
for parm in $(cat /proc/cmdline); do
	case $parm in
		boot_type=*)
			BOOT_TYPE=`echo $parm | awk -F\= '{print $2}'`
			;;
	esac
done

case $ROOT_DEVICE in
	/dev/nand*)
		LOGD "nand boot"
		mount /dev/nanda /boot
		;;
	/dev/mmc*)
		case $BOOT_TYPE in
			2)
				LOGD "emmc boot,boot_type = $BOOT_TYPE"
				mount /dev/mmcblk0p2 /boot
				SEND_CMD_PIPE_OK_EX $3
				exit 1
				;;
			*)
				LOGD "card boot,boot_type = $BOOT_TYPE"
				;;
		esac
		;;
	*)
		LOGD "default boot type"
		mount /dev/nanda /boot
		;;
esac

#only in card boot mode,it will run here
if [ ! -b "/dev/mmcblk1" ]; then
	#/dev/mmcblk1 is not exist,maybe emmc driver hasn't been insmod
	SEND_CMD_PIPE_FAIL_EX $3 "11"
	exit 1
fi

flashdev="/dev/mmcblk1"
LOGD "flashdev=$flashdev"

mkfs.vfat $flashdev
if [ $? -ne 0 ]; then
	SEND_CMD_PIPE_FAIL_EX $3 "22"
	exit 1
fi

test_size=`script_fetch "emmc" "test_size"`
if [ -z "$test_size" -o $test_size -le 0 -o $test_size -gt $total_size ]; then
	test_size=64
fi

LOGD "test_size=$test_size"
LOGD "emmc test read and write"

emmcrw "$flashdev" "$test_size"
if [ $? -ne 0 ]; then
	SEND_CMD_PIPE_FAIL_EX $3 "33"
	LOGE "emmc test fail!!"
else
	SEND_CMD_PIPE_OK_EX $3
	LOGD "emmc test ok!!"
fi
