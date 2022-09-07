
#!/bin/sh
##############################################################################
# \version     1.0.0
# \date        2019.11.5
# \author
# \Descriptions:
#			test spdif
##############################################################################
source send_cmd_pipe.sh
source script_parser.sh
source log.sh

LOG_TAG="spdiftester"

spdif_id=`cat /proc/asound/cards | grep "spdif" | head -n 1 | cut -d "[" -f1`

while true; do
	result=`tinyplay /dragonboard/data/test48000.wav -D ${spdif_id}`

	LOGD $result

	sample=`echo ${result} | grep "sample"`
	state=`echo ${result} | grep -E "ERROR|errno"`

	if [ "x" != "xi${sample}" ] && [ "x" == "x${state}" ] ; then
		SEND_CMD_PIPE_OK_EX $3
		break
	fi
	sleep 2
done
