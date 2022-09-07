#!/bin/sh
###############################################################################
# \version     1.0.0
# \date        2012年09月26日
# \author      Martin <zhengjiewen@allwinnertech.com>
# \Descriptions:
#			create the inital version
###############################################################################

source send_cmd_pipe.sh
source log.sh

LOG_TAG="otgtester"

while true; do
	pid_vid=`lsusb | awk '(NF&&$2~/001/&&$6!~/^1d6b/) {print $6}'`
	pid=`echo $pid_vid | awk -F: '{print $1}'`
	vid=`echo $pid_vid | awk -F: '{print $2}'`

	if [ -n "$pid" ] && [ -n "$vid" ]; then
		LOGD "pid=$pid;vid=$vid"
		SEND_CMD_PIPE_OK_EX $3
	fi
	sleep 1
done

