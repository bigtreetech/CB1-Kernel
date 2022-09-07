#!/bin/sh

##############################################################################
# \version     1.0.0
# \date        2014年08月28日
# \author      hesimin <hesimin@allwinnertech.com>
# \Descriptions:
#			create the inital version
##############################################################################

source send_cmd_pipe.sh
source script_parser.sh

#batterypath="/sys/class/power_supply/battery/"
#if cat $batterypath"/present" | grep 1; then
while true ; do
	batterypath="/sys/class/power_supply/battery/present"
	if cat $batterypath | grep 0; then
		SEND_CMD_PIPE_OK_EX $3 "NOT PMU"
	else
		pmu_status=`cat /sys/class/power_supply/battery/status`
		SEND_CMD_PIPE_OK_EX $3 "${pmu_status}"
	fi
	sleep 1
done
#else
#	SEND_CMD_PIPE_FAIL $3
#fi

