#!/bin/bash    

cfg_file=/boot/system.cfg
log_file=/etc/scripts/wifi.log

IFS=:

source $cfg_file     # 加载配置文件

IS_AP_MODE="no"
sta_mount=0
sudo kill -9 `pidof wpa_supplicant`
sleep 2
systemctl restart NetworkManager
sleep 4

exec 1> /dev/null
# without check_interval set, we risk a 0 sleep = busy loop
if [ ! "$check_interval" ]; then
    echo $(date)" ===> No check interval set!" >> $log_file
    exit 1
fi

function is_network() {
    local Result=no
    if [ $# -eq 0 ]; then
        ping -c 1 $router_ip >/dev/null 2>&1
        [[ $? != 0 ]] && Result=no || Result=yes
    else
        ping -c 1 $router_ip -I $1 >/dev/null 2>&1
        [[ $? != 0 ]] && Result=no || Result=yes
    fi
    echo $Result
}

function startWifi_ap() {
    if [[ $IS_AP_MODE == "no" && $sta_mount -gt 1 ]]; then
        echo $(date)" ---- change to ap mode..." >> $log_file
        IS_AP_MODE="yes"
        nmcli device disconnect $wlan
        sudo kill -9 $(pidof hostapd)
        sudo kill -9 $(pidof udhcpd)
        sudo kill -9 $(pidof udhcpc)
        
        # hostapd启动中常用到的参数说明
            # -h   显示帮助信息
            # -d   显示更多的debug信息 (-dd 获取更多)
            # -B   将hostapd程序运行在后台
            # -g   全局控制接口路径，这个工hostapd_cli使用，一般为/var/run/hostapd
            # -G   控制接口组
            # -P   PID 文件
            # -K   调试信息中包含关键数据
            # -t   调试信息中包含时间戳
            # -v   显示hostapd的版本
        
        sudo hostapd /etc/soft_ap/hostapd.conf -B   # 指定配置文件，-B将程序放到后台运行
        sudo ifconfig wlan0 192.168.2.1
        sudo udhcpd /etc/soft_ap/udhcpd.conf &
        sleep 4
    fi
}

function startWifi_sta() {
    source $cfg_file
    sta_mount=`expr $sta_mount + 1`
    echo $(date)" .... sta connecting...$sta_mount..." >> $log_file
    sudo kill -9 $(pidof hostapd)
    sudo kill -9 $(pidof udhcpd)
    sudo kill -9 $(pidof udhcpc)
    
    sudo nmcli dev wifi connect $WIFI_SSID password $WIFI_PASSWD ifname $wlan
    # sudo nmcli dev wifi connect $WIFI_SSID password $WIFI_PASSWD wep-key-type key ifname $wlan
    
    sleep 2
    sudo udhcpc -i $wlan
    sudo udhcpc -i $wlan
    sleep 2
    [[ $(is_network $wlan) == yes ]] && sta_mount=0
    [[ $(is_network $wlan) == yes ]] && IS_AP_MODE="no"
}

function startWifi() {
    if ! ifconfig | grep $wlan;then     # 确保wlan连接启动了
        sudo ifconfig $wlan up
    fi

    if [[ $sta_mount -le 1 ]]; then
        nmcli device connect $wlan      # 连接wifi
        echo $(date)" .... $wlan connecting..." >> $log_file
        [[ $(is_network $wlan) == yes ]] && sudo udhcpc -i $wlan
        sleep 4
        [[ $(is_network $wlan) == no ]] && startWifi_sta
        [[ $(is_network $wlan) == yes ]] && sta_mount=0
        [[ $(is_network $wlan) == yes ]] && IS_AP_MODE="no"
    else
        echo $(date)" xxxx WIFI connecting... IS_AP_MODE=$IS_AP_MODE " >> $log_file
        startWifi_ap
    fi
}

sudo nmcli dev wifi connect $WIFI_SSID password $WIFI_PASSWD ifname $wlan

while [ 1 ]; do

    if [[ $WIFI_AP == "false" ]]; then
        if [[ $(is_network) == no ]]; then      # 没有网络连接
            echo -e $(date)" ==== No network connection..." >> $log_file
            startWifi
            sleep 6    # 更改间隔时间，因为有些服务启动较慢，试验后，改的间隔长一点有用
        else
            if [[ $(is_network $eth) == yes ]]; then    # 以太网连接
                nmcli device disconnect $wlan
                echo "==== Ethernet Connected, wlan disconnect! ====" >> $log_file
            fi
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
        elif [[ $(is_network $wlan) == yes ]]; then
            echo -e $(date)" ==== $wlan network connection..." >> $log_file
            sudo udhcpc -i $wlan
            IS_AP_MODE="no"
        fi
    fi

    sync
    sleep $check_interval
done
