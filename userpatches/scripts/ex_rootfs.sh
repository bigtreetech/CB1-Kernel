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

sudo resize2fs /dev/${filelist[2]}           # 扩展分区;
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
if [ -e "ex_rootfs.sh" ];then
    sudo rm ./ex_rootfs.sh -fr
fi

sudo chown $username:$username /home/$username/ -R
# sudo ntpdate stdtime.gov.hk

cd /boot/gcode
if ls *.gcode > /dev/null 2>&1;then
    sudo cp ./*.gcode /home/$username/gcode_files -fr
    sudo rm ./*.gcode -fr
fi
sync

cd $shell_path
./disp_chose.sh
./pwr_status.sh &
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

# if [[ \`lsusb\` == *"BTT-HDMI"* ]]
# then
#     sed -i "s/self.blanking_time = 0*$/self.blanking_time = abs(int(time))/" /home/$username/KlipperScreen/screen.py
# else
#     sed -i "s/self.blanking_time = abs(int(time))*$/self.blanking_time = 0/" /home/$username/KlipperScreen/screen.py
# fi

cd /sys/class/gpio
echo 229 > export
cd /sys/class/gpio/gpio229
echo out > direction

while [ 1 ]
do
    echo 1 > value
    sleep 0.5
    echo 0 > value
    sleep 0.5
done

EOF

#######################################################
#------------------- disp_chose.sh -------------------#
#######################################################
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

# ================================================ #

[[ -d "/boot/gcode" ]] || sudo mkdir /boot/gcode -p
[[ -d "/home/${username}/klipper_logs" ]] && rm /home/${username}/klipper_logs/* -fr
[[ -d "/home/${username}/gcode_files" ]] && rm /home/${username}/gcode_files/* -fr

sudo sed -i 's/ex_rootfs.sh/init.sh \&/' /etc/rc.local

sudo reboot
