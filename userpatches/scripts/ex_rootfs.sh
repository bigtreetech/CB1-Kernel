#!/bin/bash

username=biqu
shell_path=/etc/scripts

#===================================#

c=0
cd /dev
for file in `ls mmcblk*`
do
    filelist[$c]=$file
    ((c++))
done

ROOT_DEV=/dev/${filelist[0]}

BOOT_NUM=1
ROOT_NUM=2

ROOTFS_START=532480

# 使用 fdisk 工具进行磁盘分区;
sudo fdisk "$ROOT_DEV" << EOF
p
d
$ROOT_NUM
n
p
$ROOT_NUM
$ROOTFS_START

y
w
EOF

sudo resize2fs /dev/${filelist[$c-1]}          # 扩展分区;
unset filelist

#-----------------
# sudo usermod -a -G root $username         # 将biqu加入root用户组
# sudo gpasswd -d biqu root     # 将biqu移出root用户组

# sudo chmod 777 /var/run/wpa_supplicant/wlan0

cd $shell_path

#######################################################
#---------------------- init.sh ----------------------#
#######################################################
[[ -e "init.sh" ]] && rm init.sh -fr

touch init.sh
chmod +x init.sh
cat >> init.sh << EOF
#!/bin/bash

cd $shell_path
./pwr_status.sh &
./system_cfg.sh &

[[ -e "ex_rootfs.sh" ]] && sudo rm ./ex_rootfs.sh -fr

sudo chown $username:$username /home/$username/ -R
# sudo ntpdate stdtime.gov.hk

cd /boot/gcode
if ls *.gcode > /dev/null 2>&1;then
    sudo cp ./*.gcode /home/$username/printer_data/gcodes -fr
    sudo rm ./*.gcode -fr
fi
sync

cd $shell_path
./reconnect_wifi.sh

EOF

#######################################################
#------------------- pwr_status.sh -------------------#
#######################################################
[[ -e "pwr_status.sh" ]] && rm pwr_status.sh -fr
touch pwr_status.sh
chmod +x pwr_status.sh

cat >> pwr_status.sh << EOF
#!/bin/bash

echo 229 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio229/direction

while [ 1 ]
do
    echo 1 > /sys/class/gpio/gpio229/value
    sleep 0.5
    echo 0 > /sys/class/gpio/gpio229/value
    sleep 0.5
done

EOF

#######################################################
#------------------- system_cfg.sh -------------------#
#######################################################
[[ -e "system_cfg.sh" ]] && rm system_cfg.sh -fr
touch system_cfg.sh
chmod +x system_cfg.sh

cat >> system_cfg.sh << EOF
#!/bin/bash

SYSTEM_CFG_PATH="/boot/system.cfg"
source \${SYSTEM_CFG_PATH}

grep -e "^hostname" \${SYSTEM_CFG_PATH} > /dev/null
STATUS=\$?
if [ \${STATUS} -eq 0 ]; then
    cur_name=$(nmcli general hostname)
    if [[ \${cur_name} != \${hostname} ]]; then
        sudo nmcli general hostname \${hostname}
        sudo systemctl restart systemd-hostnamed
        sudo systemctl restart avahi-daemon.service
    fi
fi

grep -e "^TimeZone" \${SYSTEM_CFG_PATH} > /dev/null
STATUS=\$?
if [ \${STATUS} -eq 0 ]; then
    sudo timedatectl set-timezone \${TimeZone}
fi

#######################################################
grep -e "^ks_angle" \${SYSTEM_CFG_PATH} > /dev/null
STATUS=\$?
if [ \${STATUS} -eq 0 ]; then
    if [[ \${ks_angle} == "normal" ]]; then
        i=0
    elif [[ \${ks_angle} == "left" ]]; then
        i=1
    elif [[ \${ks_angle} == "inverted" ]]; then
        i=2
    elif [[ \${ks_angle} == "right" ]]; then
        i=3
    else
        i=0
    fi

    ks_restart=0

    DISPLAY_CONFIG_OPTION="Option \"Rotate\" "
    DISPLAY_DIR_OPTIONS=(
        "\"normal\""
        "\"left\""
        "\"inverted\""
        "\"right\""
    )
    DISPLAY_CONFIG_PATH="/usr/share/X11/xorg.conf.d"
    DISPLAY_CONFIG="/usr/share/X11/xorg.conf.d/90-monitor.conf"
    DISPLAY_MONITOR="Identifier \"HDMI-1\""
    DISPLAY_DIR_LINE="\${DISPLAY_DIR_OPTIONS[\$i]}"
    if [ -e "\${DISPLAY_CONFIG}" ]; then
        grep -e "^\    \${DISPLAY_CONFIG_OPTION}\${DISPLAY_DIR_LINE}" \${DISPLAY_CONFIG} > /dev/null
        STATUS=\$?
        if [ \$STATUS -eq 1 ]; then
            sudo sed -i "/\${DISPLAY_CONFIG_OPTION}/d" \${DISPLAY_CONFIG}
            sudo sed -i "/\${DISPLAY_MONITOR}/a\    \${DISPLAY_CONFIG_OPTION}\${DISPLAY_DIR_LINE}" \${DISPLAY_CONFIG}
            ks_restart=1
        fi
    else
        if [ -d "\${DISPLAY_CONFIG_PATH}" ]; then
            sudo touch \${DISPLAY_CONFIG}
            sudo bash -c "echo 'Section \"Monitor\"' > \${DISPLAY_CONFIG} "
            sudo bash -c "echo '    Identifier \"HDMI-1\"' >> \${DISPLAY_CONFIG} "
            sudo bash -c "echo 'EndSection' >> \${DISPLAY_CONFIG} "
            sudo sed -i "/\${DISPLAY_MONITOR}/a\    \${DISPLAY_CONFIG_OPTION}\${DISPLAY_DIR_LINE}" \${DISPLAY_CONFIG}
            ks_restart=1
        fi
    fi

    CONFIG_OPTION="Option \"CalibrationMatrix\" "
    CALIB_OPTIONS=(
        "\"1 0 0 0 1 0 0 0 1\""
        "\"0 -1 1 1 0 0 0 0 1\""
        "\"-1 0 1 0 -1 1 0 0 1\""
        "\"0 1 0 -1 0 1 0 0 1\""
    )
    CONFIG="/usr/share/X11/xorg.conf.d/40-libinput.conf"
    INPUT_CLASS="Identifier \"libinput touchscreen catchall\""
    CONFIG_LINE="\${CALIB_OPTIONS[\$i]}"
    if [ -e "\${CONFIG}" ]; then
        grep -e "^\        \${CONFIG_OPTION}\${CONFIG_LINE}" \${CONFIG} > /dev/null
        STATUS=\$?
        if [ \$STATUS -eq 1 ]; then
            sudo sed -i "/\${CONFIG_OPTION}/d" \${CONFIG}
            sudo sed -i "/\${INPUT_CLASS}/a\        \${CONFIG_OPTION}\${CONFIG_LINE}" \${CONFIG}
            ks_restart=1
        fi
    fi

    if [ \${ks_restart} -eq 1 ];then
        sudo service KlipperScreen restart
    fi
fi

#######################################################
if [[ \${BTT_PAD7} == "ON" ]]; then
    # Toggle status light color
    sudo /etc/scripts/set_rgb 0x000001 0x000001
    sudo /etc/scripts/set_rgb 0x000001 0x000001

    # Automatic brightness adjustment
    [[ \${AUTO_BRIGHTNESS} == "ON" ]] && /etc/scripts/auto_brightness &

    SRC_FILE=/etc/scripts/ks_click.sh

    [[ -e "\${SRC_FILE}" ]] && sudo rm \${SRC_FILE} -fr

    touch \${SRC_FILE} && chmod +x \${SRC_FILE}
    echo "#!/bin/bash" >> \${SRC_FILE}

    if [[ \${TOUCH_VIBRATION} == "ON" ]]; then
        RUN_FILE="\${RUN_FILE}vibration"
    fi
    if [[ \${TOUCH_SOUND} == "ON" ]]; then
        RUN_FILE="\${RUN_FILE}sound"
    fi

    if [ -n "\${RUN_FILE}" ]; then
        if [[ \${RUN_FILE} =~ "vibration" ]]; then
            sudo chmod 666 /sys/class/gpio/export
            echo 79 > /sys/class/gpio/export
            cd /sys/class/gpio/gpio79
            sudo chmod 666 direction value
            echo out > /sys/class/gpio/gpio79/direction
        fi

        if [[ \${RUN_FILE} =~ "sound" ]]; then
            export AUDIODRIVER=alsa
        fi

        [[ -z "\${RUN_FILE}" ]] || echo "/etc/scripts/\${RUN_FILE}.sh &" >> \${SRC_FILE}
    fi
fi

#######################################################

EOF

# ================================================ #

# ================================================ #

[[ -d "/boot/gcode" ]] || sudo mkdir /boot/gcode -p
[[ -d "/home/${username}/klipper_logs" ]] && rm /home/${username}/klipper_logs/* -fr
[[ -d "/home/${username}/printer_data/gcodes" ]] && rm /home/${username}/printer_data/gcodes/* -fr

sudo sed -i 's/ex_rootfs.sh/init.sh \&/' /etc/rc.local

sudo reboot
