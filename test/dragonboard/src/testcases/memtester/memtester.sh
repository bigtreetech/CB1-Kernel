#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="memtester"

#sdram config registe
platform=`script_fetch "dram" "platform"`

get_dram_size()
{
	local tmp
	tmp=$(cat /proc/meminfo |grep 'MemTotal' |awk -F : '{print $2}' |sed 's/^[ \t]*//g'| awk -Fk '{print $1}')
	let "tmp=$tmp/1048576"
	let "tmp=$tmp+1"
	let "dram_size=$tmp*1024"
	echo $dram_size

}


dram_size=`script_fetch "dram" "dram_size"`
test_size=`script_fetch "dram" "test_size"`

actual_size=`get_dram_size`

if [ "$platform" == "sun50iw1p1" -o "$platform" == "sun50iw3p1" ] ; then
	dram_freq=`cat /sys/class/devfreq/dramfreq/cur_freq | awk -F: '{print $1}'`
	let "dram_freq=$dram_freq/1000"
elif [ "$platform" == "sun8iw15p1" ] ; then
dram_freq=-1
for parm in $(cat /proc/cmdline) ; do
	case $parm in
		dram_clk=*)
			dram_freq=`echo $parm | awk -F\= '{print $2}'`
			;;
	esac
done
else
	dram_freq=`cat /sys/devices/platform/sunxi-ddrfreq/devfreq/sunxi-ddrfreq/cur_freq | awk -F: '{print $1}'`
	let "dram_freq=$dram_freq/1000"
fi

LOGD "dram_freq=$dram_freq"
LOGD "config dram_size=$dram_size"M""
LOGD "actual_size=$actual_size"M""
LOGD "test_size=$test_size"M""

if [ $actual_size -lt $dram_size ]; then
	SEND_CMD_PIPE_FAIL_EX $3 "size "$actual_size"M"" error"
	exit 0
fi
SEND_CMD_PIPE_MSG $3 "size:$actual_size"M" freq:$dram_freq"MHz""
memtester $test_size"M" 1 > /dev/null
if [ $? -ne 0 ]; then
	LOGE "memtest fail"
	SEND_CMD_PIPE_FAIL_EX $3 "size:$actual_size"M" freq:$dram_freq"MHz""
else
	LOGD "memtest success!"
	SEND_CMD_PIPE_OK_EX $3 "size:$actual_size"M" freq:$dram_freq"MHz""
fi
