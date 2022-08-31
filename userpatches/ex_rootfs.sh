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

cd $shell_path

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
sudo ntpdate stdtime.gov.hk

cd /boot/gcode
if ls *.gcode > /dev/null 2>&1;then
    sudo cp ./*.gcode /home/$username/gcode_files -fr
    sudo rm ./*.gcode -fr
fi
sync

cd $shell_path
./reconnect_wifi.sh

EOF

[[ -d "/boot/gcode" ]] || sudo mkdir /boot/gcode -p
[[ -d "/home/${username}/klipper_logs" ]] && rm /home/${username}/klipper_logs/* -fr
[[ -d "/home/${username}/gcode_files" ]] && rm /home/${username}/gcode_files/* -fr

git config --global --unset http.proxy      # 取消代理
git config --global --unset https.proxy

history -c          # 清除历史shell记录
history -w
cd /home/$username/
sudo rm ./.bash_history -fr

sudo sed -i 's/ex_rootfs.sh/init.sh \&/' /etc/rc.local

sudo reboot
