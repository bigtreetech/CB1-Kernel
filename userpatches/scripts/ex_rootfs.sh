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

[[ $c -gt 3 ]] && exit 0

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

sudo resize2fs /dev/${filelist[2]}          # 扩展分区;
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

source /boot/system.cfg

# Automatic brightness adjustment
[[ \${AUTO_BRIGHTNESS} == "ON" ]] && /etc/scripts/auto_brightness &

sudo timedatectl set-timezone \${TimeZone}
# [[ -e "/etc/localtime" ]] && sudo rm /etc/localtime -fr
# sudo ln -s /usr/share/zoneinfo/\${TimeZone} /etc/localtime

#######################################################
# if [[ \`lsusb\` == *"BTT-HDMI"* ]]
# then
#     sed -i "s/self.blanking_time = 0*$/self.blanking_time = abs(int(time))/" /home/$username/KlipperScreen/screen.py
# else
#     sed -i "s/self.blanking_time = abs(int(time))*$/self.blanking_time = 0/" /home/$username/KlipperScreen/screen.py
# fi

#######################################################
if [[ -d "/home/${username}/KlipperScreen" ]]
then
    SRC_FILE=/home/${username}/KlipperScreen/scripts/BTT-PAD7/click_effect.sh

    [[ -e "\${SRC_FILE}" ]] && sudo rm \${SRC_FILE} -fr

    touch \${SRC_FILE} && chmod +x \${SRC_FILE}
    echo "#!/bin/bash" >> \${SRC_FILE}

    if [[ \${TOUCH_VIBRATION} == "ON" ]] && [[ \${TOUCH_SOUND} == "ON" ]]; then
        RUN_FILE="motor_sound"
    elif [[ \${TOUCH_VIBRATION} == "ON" ]] && [[ \${TOUCH_SOUND} == "OFF" ]]; then
        RUN_FILE="motor"
    elif [[ \${TOUCH_VIBRATION} == "OFF" ]] && [[ \${TOUCH_SOUND} == "ON" ]]; then
        RUN_FILE="sound"
    fi

    if [[ \${RUN_FILE} != "sound" ]]; then
        sudo chmod 666 /sys/class/gpio/export
        echo 79 > /sys/class/gpio/export
        cd /sys/class/gpio/gpio79
        sudo chmod 666 direction value
        echo out > /sys/class/gpio/gpio79/direction
    fi

    [[ -z "\${RUN_FILE}" ]] || echo "/home/${username}/KlipperScreen/scripts/BTT-PAD7/\${RUN_FILE}.sh &" >> \${SRC_FILE}
fi

# Toggle status light color
sudo /etc/scripts/set_rgb 0xff 0xff00

#######################################################
echo 229 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio229/direction

while [ 1 ]
do
#######################################################
    string=\`DISPLAY=:0 xinput --list | grep \${HDMI_Vendor}\`
    string=\${string#*id=}
    input_id=\${string%[*}

    [[ \${dis_angle} != "normal" ]] && DISPLAY=:0 xrandr --output HDMI-1 --rotate \${dis_angle}

    if [[ \${dis_angle} == "left" ]]; then
        DISPLAY=:0 xinput --set-prop \${input_id} 'Coordinate Transformation Matrix' 0 -1 1 1 0 0 0 0 1
    elif [[ \${dis_angle} == "right" ]]; then
        DISPLAY=:0 xinput --set-prop \${input_id} 'Coordinate Transformation Matrix' 0 1 0 -1 0 1 0 0 1
    elif [[ \${dis_angle} == "inverted" ]]; then
        DISPLAY=:0 xinput --set-prop \${input_id} 'Coordinate Transformation Matrix' -1 0 1 0 -1 1 0 0 1
    fi

#######################################################
    echo 1 > /sys/class/gpio/gpio229/value
    sleep 0.5
    echo 0 > /sys/class/gpio/gpio229/value
    sleep 0.5
done

EOF

# ================================================ #

[[ -d "/boot/gcode" ]] || sudo mkdir /boot/gcode -p
[[ -d "/home/${username}/klipper_logs" ]] && rm /home/${username}/klipper_logs/* -fr
[[ -d "/home/${username}/printer_data/gcodes" ]] && rm /home/${username}/printer_data/gcodes/* -fr

sudo sed -i 's/ex_rootfs.sh/init.sh \&/' /etc/rc.local

sudo reboot

#######################################################
#------------------- disp_chose.sh -------------------#
#######################################################
function disp_chose() {

[[ -e "disp_chose.sh" ]] && rm disp_chose.sh -fr
touch disp_chose.sh
chmod +x disp_chose.sh

cat >> disp_chose.sh << EOF
#!/bin/bash

tft_dtb=/boot/h616_tft.deb
hdmi_dtb=/boot/h616_hdmi.deb

CFG_FILE="/boot/system.cfg"

if [ \`grep -c "DISP_OUT=\"TFT\"" \$CFG_FILE\` -ne '0' ];then
    if [[ \`dmesg\` =~ "BQ-TFT" ]]
    then
        echo "TFT is installed"
    else
        sudo apt purge -y linux-dtb-current-sun50iw9
        echo "change to TFT"
        sudo dpkg -i \$tft_dtb
        reboot
    fi
fi

if [ \`grep -c "DISP_OUT=\"HDMI\"" \$CFG_FILE\` -ne '0' ];then
    if [[ \`dmesg\` =~ "BQ-HDMI" ]]
    then
        echo "HDMI is installed"
    else
        sudo apt purge -y linux-dtb-current-sun50iw9
        echo "change to HDMI"
        sudo dpkg -i \$hdmi_dtb
        reboot
    fi
fi

EOF

}
