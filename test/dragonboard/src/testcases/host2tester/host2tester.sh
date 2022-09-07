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

LOG_TAG="host2tester"

while true; do
	pid_vid=`lsusb | awk '(NF&&$2~/^(004|005)/&&$6!~/^1d6b/) {print $6}'`
	pid=`echo $pid_vid | awk -F: '{print $1}'`
	vid=`echo $pid_vid | awk -F: '{print $2}'`

	if [ -n "$pid" ] && [ -n "$vid" ]; then
		LOGD "pid=$pid;vid=$vid"
		SEND_CMD_PIPE_OK_EX $3 "pid=0x$pid;vid=0x$vid"
	fi
	sleep 1
done

