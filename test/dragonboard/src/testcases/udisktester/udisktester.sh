#!/bin/bash

source send_cmd_pipe.sh
source log.sh

LOG_TAG="udisktester"

while true; do
	for nr in {a..z}; do
		udisk="/dev/sd$nr"
		part=$udisk

		while true; do
			while true; do
				if [ -b "$udisk" ]; then
					sleep 1
					if [ -b "$udisk" ]; then
						LOGD "udisk insert"
						break;
					fi
				else
					sleep 1
				fi
			done

			if [ ! -d "/tmp/udisk" ]; then
				mkdir /tmp/udisk
			fi

			mount $udisk /tmp/udisk
			if [ $? -ne 0 ]; then
				for partno in $(seq 1 1 15); do
					udiskp=$udisk$partno
					LOGD "$udiskp"
					if [ -b "$udiskp" ]; then
						mount $udiskp /tmp/udisk
						if [ $? -ne 0 ]; then
							SEND_CMD_PIPE_FAIL $3
							sleep 3
							# goto for nr in ...
							# detect next plugin, the devno will changed
							continue 2
						else
							part=$udiskp
							break
						fi
					fi
				done
			fi
			break
		done

		capacity=`df -h | grep $part | awk '{printf $2}'`
		LOGD "$part: $capacity"

		SEND_CMD_PIPE_OK_EX $3 $capacity

		while true; do
			if [ -b "$udisk" ]; then
				sleep 1
			else
				LOGD "udisk removed"
				break
			fi
		done
	done
done
