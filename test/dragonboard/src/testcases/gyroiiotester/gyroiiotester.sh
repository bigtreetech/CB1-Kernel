#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="gyroiiotester"

module_count=`script_fetch "gyro" "module_count"`
if [ $module_count -gt 0 ]; then
	for i in $(seq $module_count); do
		key_name="module"$i"_path"
		module_path=`script_fetch "gyro" "$key_name"`
		if [ -n "$module_path" ]; then
			LOGD "insmod $module_path"
			insmod "$module_path"
			if [ $? -ne 0 ]; then
				SEND_CMD_PIPE_FAIL $3
				exit 1
			fi
		fi
	done
fi

sleep 3

device_name=`script_fetch "gyro" "device_name"`
for event in $(cd /sys/bus/iio/devices/; ls iio:device*); do
	name=`cat /sys/bus/iio/devices/$event/name`
	case $name in
		$device_name)
			#SEND_CMD_PIPE_OK $3
			gyroiiotester $* "$device_name" "$event" &
			exit 0
			;;
		*)
			;;
	esac
done

SEND_CMD_PIPE_FAIL $3
