#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="2gtester"

LOGD "#######test for 2g module#####"
single_level=0

sleep_time=`script_fetch "2g" "delay"`
device_node=`script_fetch "2g" "device_node"`
call_number=`script_fetch "2g" "call_number"`
call_time=`script_fetch "2g" "call_time"`

if [ -z $device_node ];then
	LOGE "input device_node not correct"
	SEND_CMD_PIPE_FAIL_EX $3 "no spec device"
	exit 1
fi

if [ -z $call_number ];then
	LOGE "no call number"
	SEND_CMD_PIPE_FAIL_EX $3 "no call number"
	exit 1
fi

if [ $sleep_time -le 0 ];then
	LOGD "use default delay time"
	sleep_time=5
fi


LOGD "wait $sleep_time S before contiune"
sleep $sleep_time
sleep 10
if [ -c $device_node ]; then
	LOGD "find device"
else
	LOGE "no device find !"
	SEND_CMD_PIPE_FAIL_EX $3 "no device"
	exit 1
fi
LOGD "query the single level"
echo "AT+CSQ" > $device_node
echo "AT+CSQ" > $device_node
sleep 1
cat $device_node | while read cmd_back
do
	cmd_back=`echo $cmd_back |grep +CSQ: | cut -d: -f 2 |cut -d, -f 1`

	if [ -n $cmd_back -a $cmd_back -ge 0 ];then
		single_level=$cmd_back
		let "single_level=$single_level-113"
		LOGD "single_level=$single_level"
		LOGD "single break"
		echo "AT+CSQ" > $device_node
		break 2;
	fi
	let "loop=$loop+1"
	if [ $loop -gt 3 ];then
		LOGD "gt break"
		break 2;
		echo "AT+CSQ" > $device_node
	fi
	LOGD "loop=$loop"
done
LOGD "call the telephone number:$call_number"
LOGD "single_level=$single_level"
string=${single_level}" dB ,calling"
SEND_CMD_PIPE_MSG $3 $string

echo "AT+CLVL=7" > $device_node
echo ATD$call_number\; > $device_node

sleep 20

LOGD "hang up"
SEND_CMD_PIPE_MSG $3 "hang up"

echo "AT+CHUP" > $device_node
sleep 1

SEND_CMD_PIPE_OK_EX $3 "test done"
