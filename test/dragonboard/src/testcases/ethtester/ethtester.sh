#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="ethtester"

module_path=`script_fetch "ethernet" "module_path"`
module_args=`script_fetch "ethernet" "module_args"`

eth_try=0

if [ -z "$module_path" ]; then
	LOGD "ethernet driver buildin"
else
	LOGD "insmod $module_path $module_args"
	insmod "$module_path" "$module_args"
	if [ $? -ne 0 ]; then
		SEND_CMD_PIPE_FAIL $3
		exit 1
	fi
fi

for j in `seq 3`;do
	if ifconfig -a | grep eth0; then
		# enable eth0
		for i in `seq 3`; do
			ifconfig eth0 up > /dev/null
			if [ $? -ne 0 -a $i -eq 3 ]; then
				LOGW "ifconfig eth0 up failed, no more try"
				SEND_CMD_PIPE_FAIL $3
				exit 1
			fi
			if [ $? -ne 0 ]; then
				LOGD "ifconfig eth0 up failed, try again 1s later"
				sleep 1
			else
				LOGD "ifconfig eth0 up done"
				break
			fi
		done

		eth_run=$(ifconfig eth0 | grep -c "RUNNING")
		[ $eth_run -eq 0 ] && LOGD "eth0 not running"

		while true ; do
			eth_run=$(ifconfig eth0 | grep -c "RUNNING")
			if [ $eth_run -eq 1 ] ; then
				LOGD "eth0 running...."
				break
			fi
			sleep 1
		done

		for i in `seq 3`; do
			udhcpc -i eth0 > /dev/null
			if [ $? -ne 0 -a $i -eq 3 ]; then
				LOGE "dhcp eth0 failed, no more try"
				SEND_CMD_PIPE_FAIL $3
				exit 1
			fi
			if [ $? -ne 0 ]; then
				LOGD "dhcp eth0 failed, try again 1s later"
				sleep 1
			else
				LOGD "dhcp eth0 done"
				break
			fi
		done

			ping -w 3 www.baidu.com > /dev/null
			if [ $? -ne 0] ; then
				SEND_CMD_PIPE_FAIL $3
				exit 1
			fi

			# disable eth0
			ifconfig eth0 down
			if [ $? -ne 0 ]; then
				SEND_CMD_PIPE_FAIL $3
				exit 1
			fi

			# test done
			SEND_CMD_PIPE_OK $3
			exit 0

	else
		LOGW "eth0 not found, try it again later"
		sleep 1
	fi
done

# test failed
LOGE "eth0 not found, no more try"
SEND_CMD_PIPE_FAIL $3
exit 1
