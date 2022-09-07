#!/bin/sh
###############################################################################
# \version     1.0.0
# \date        2012年09月26日
# \author      luoweijian@allwinnertech.com
# \Descriptions:
#			create the inital version
###############################################################################

source send_cmd_pipe.sh
source log.sh

LOG_TAG="host1tester"

vid=`cat /sys/bus/usb/devices/usb2/idVendor`
if [ -n "$vid" ];then
	 LOGD "vid=$vid"
	 SEND_CMD_PIPE_OK_EX $3 "vid=0x$vid"
fi
