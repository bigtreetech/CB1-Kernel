#!/bin/bash    

cfg_file=/boot/system.cfg

WIFI_CFG=/home/biqu/control/wifi/conf/netinfo.txt
log_file=/etc/scripts/wifi.log

IFS=\"

IS_AP_MODE="no"
sta_mount=0

function Env_init() {
    source $cfg_file     # 加载配置文件

    exec 1> /dev/null
    # without check_interval set, we risk a 0 sleep = busy loop
    if [ ! "$check_interval" ]; then
        echo $(date)" ===> No check interval set!" >> $log_file
        exit 1
    fi

    sudo kill -9 `pidof wpa_supplicant`
    sleep 2
    sudo systemctl restart NetworkManager
    sleep 4
    [[ $(ifconfig | grep $wlan) == "" ]] && nmcli radio wifi on

    sudo nmcli dev wifi connect $WIFI_SSID password $WIFI_PASSWD ifname $wlan
    sleep 6
}

function is_network() {
    local Result=no
    local err_num=0

    for((i=1;i<=5;i++))
    do
        if [ $# -eq 0 ]; then
            ping -c 1 $router_ip >/dev/null 2>&1
            [[ $? != 0 ]] && err_num=`expr $err_num + 1`
        else
            ping -c 1 $router_ip -I $1 >/dev/null 2>&1
            [[ $? != 0 ]] && err_num=`expr $err_num + 1`
        fi
        sleep 1
        sync
    done
    [[ $err_num -lt 5 ]] && Result=yes || Result=no
    err_num=0
    unset err_num

    echo $Result
}

function Create_AP_ON() {
    if [[ $IS_AP_MODE == "no" && $sta_mount -gt 1 ]]; then
        nmcli device disconnect $wlan
        sudo systemctl start create_ap
        sleep 2
        IS_AP_MODE="yes"

        echo $(date)" xxxx $wlan Change to ap mode..." >> $log_file
        if inotifywait $WIFI_CFG --timefmt '%d/%m/%y %H:%M' --format "%T %f" -e MODIFY
        then
            echo -e $(date)" ==== $wlan modify cfg..." >> $log_file
            IS_AP_MODE="no"
            source $WIFI_CFG
            sudo sed -i "s/^WIFI_SSID=.*$/WIFI_SSID=$WIFI_SSID/" $cfg_file
            sudo sed -i "s/^WIFI_PASSWD=.*$/WIFI_PASSWD=$WIFI_PASSWD/" $cfg_file
            [[ $(is_network $eth) == no ]] && Create_AP_OFF
        fi
    fi
}

function Create_AP_OFF() {
    sudo systemctl stop create_ap
    sudo create_ap --fix-unmanaged
    sudo systemctl restart NetworkManager

    [[ $(ifconfig | grep $wlan) == "" ]] && nmcli radio wifi on  # 确保wlan连接启动了

    if [[ $(is_network $wlan) == no ]]; then
        source $cfg_file
        echo -e $(date)" ==== $wlan prepare connection... -WIFI_SSID:$WIFI_SSID " >> $log_file
        sudo nmcli dev wifi connect $WIFI_SSID password $WIFI_PASSWD ifname $wlan
        sleep 5
    fi
    sta_mount=0
    IS_AP_MODE="no"

    [[ $(is_network $wlan) == no ]] || echo -e $(date)" ==== $wlan network connection..." >> $log_file
}

function startWifi_sta() {
    source $cfg_file
    sta_mount=`expr $sta_mount + 1`
    echo $(date)" .... sta connecting...$sta_mount..." >> $log_file

    Create_AP_OFF
    sleep 2
}

function startWifi() {
    [[ $(ifconfig | grep $wlan) == "" ]] && nmcli radio wifi on  # 确保wlan连接启动了

    if [[ $sta_mount -le 2 ]]; then
        nmcli device connect $wlan      # 连接wifi
        echo $(date)" .... $wlan connecting..." >> $log_file
        sleep 2
        [[ $(is_network $wlan) == no ]] && startWifi_sta
        [[ $(is_network $wlan) == yes ]] && sta_mount=0 && IS_AP_MODE="no" && echo $(date)" [O.K.] $wlan connected!" >> $log_file
    else
        echo $(date)" xxxx $wlan connection failure... IS_AP_MODE=$IS_AP_MODE ..." >> $log_file
        Create_AP_ON
    fi
}

Env_init
sleep 20

while [ 1 ]; do

    if [[ $WIFI_AP == "false" ]]; then
        if [[ $(is_network) == no ]]; then      # 没有网络连接
            echo -e $(date)" ==== No network connection..." >> $log_file
            startWifi
            sleep 6    # 更改间隔时间，因为有些服务启动较慢，试验后，改的间隔长一点有用
        # else
        #     sleep 6
        #     [[ $(is_network $eth) == yes ]] && nmcli device disconnect $wlan && echo "==== Ethernet Connected, wlan disconnect! ====" >> $log_file
        fi
    elif [[ $WIFI_AP == "true" ]]; then
        if [[ $(is_network $eth) == yes ]]; then
            sta_mount=6
            [[ $(is_network $wlan) == yes ]] && IS_AP_MODE="no"
            echo -e $(date)" ==== $eth network connection..." >> $log_file
            startWifi
        elif [[ $(is_network $wlan) == no ]]; then
            [[ $sta_mount -eq 6 ]] && sta_mount=0
            echo -e $(date)" ==== No $wlan network connection..." >> $log_file
            startWifi
        fi
    fi

    sync
    sleep $check_interval
done
