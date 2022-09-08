#!/bin/bash

username=biqu

sudo chown $username:$username /home/$username/ -R

c=0
cd /dev
for file in `ls mmcblk*`
do
  filelist[$c]=$file
  ((c++))
done

cd /

ROOT_DEV=/dev/${filelist[0]}

ROOT_NUM=4
UDISK_TEMP=5
UDISK_NUM=6

PART_SIZE=`cat /sys/block/${filelist[0]}/size`

UDISK_START=`expr $PART_SIZE - 614400`

ROOTFS_START=270336

ROOTFS_END=`expr $UDISK_START - 1`

# 使用 fdisk 工具进行磁盘分区;
sudo fdisk "$ROOT_DEV" << EOF
p
n
p
$UDISK_NUM
$UDISK_START

t
$UDISK_NUM
11
w
EOF

sudo mkdir /udisk
sudo mkdir /udisk_temp

unset filelist

c=0
cd /dev
for file in `ls mmcblk*`
do
    filelist[$c]=$file
    ((c++))
done

if [[ ${filelist[6]} == mmcblk1p6 ]]; then
    sudo mkfs.vfat /dev/mmcblk1p6            # 格式化分区为 FAT 格式;
    sudo mount /dev/mmcblk1p6 /udisk/
    sudo mount /dev/mmcblk1p5 /udisk_temp/
fi

if [[ ${filelist[6]} == mmcblk0p6 ]]; then
    sudo mkfs.vfat /dev/mmcblk0p6            # 格式化分区为 FAT 格式;
    sudo mount /dev/mmcblk0p6 /udisk/
    sudo mount /dev/mmcblk0p5 /udisk_temp/
fi

#------------------------
sudo rm /udisk/* -fr
cd /udisk_temp
sudo cp ./* /udisk/ -fr

cd /udisk
if [ ! -d "gcode" ]; then
    sudo mkdir gcode
fi

cd /
sudo umount /udisk
sudo umount /udisk_temp
sudo rm /udisk /udisk_temp -fr
#------------------------

# 使用 fdisk 工具进行磁盘分区;
sudo fdisk "$ROOT_DEV" << EOF
p
d
$UDISK_TEMP
d
$ROOT_NUM
n
p
$ROOT_NUM
$ROOTFS_START
$ROOTFS_END
y

w
EOF

sudo resize2fs /dev/${filelist[4]}           # 扩展分区;

cd /home/$username/scripts

sudo rm init.sh -fr
touch init.sh
chmod +x init.sh

cat >> init.sh << EOF
#!/bin/bash

c=0
cd /dev
for file in \`ls mmcblk*\`
do
    filelist[\$c]=\$file
    ((c++))
done

cd /home/$username/scripts
if [ -e "expand_rootfs_4_9.sh" ];then
    sudo rm ./expand_rootfs_4_9.sh -fr
fi

/etc/NetManager.sh &

sudo ifconfig can0 down
sleep 5
sudo ifconfig can0 up

sudo chown $username:$username /home/$username/ -R

sudo ntpdate stdtime.gov.hk

sudo mkdir /udisk
if [[ \${filelist[5]} == mmcblk0p5 ]]; then
    sudo mount /dev/mmcblk0p5 /udisk/
fi

if [[ \${filelist[5]} == mmcblk1p5 ]]; then
    sudo mount /dev/mmcblk1p5 /udisk/
fi

if [[ \${filelist[5]} == mmcblk0p6 ]]; then
    sudo mount /dev/mmcblk0p6 /udisk/
fi

if [[ \${filelist[5]} == mmcblk1p6 ]]; then
    sudo mount /dev/mmcblk1p6 /udisk/
fi

cd /udisk/gcode
if ls *.gcode > /dev/null 2>&1;then
    sudo cp ./*.gcode /home/$username/gcode_files -fr
    sync
    sudo rm ./*.gcode -fr
fi

cd /udisk
if [ -e "system.cfg" ];then
    sudo cp ./system.cfg /etc/
fi

cd /
sudo umount /udisk
sudo rm /udisk -fr
sync

cd /home/$username/scripts

# sudo mount -t vfat /dev/mmcblk0p6 /mnt/tfcard

/etc/wifi_config.sh &

EOF

cd /home/$username/klipper_logs
rm ./* -fr
cd /home/$username/gcode_files
rm ./* -fr

git config --global --unset http.proxy      # 取消代理
git config --global --unset https.proxy

history -c          # 清除历史shell记录
history -w
cd /home/$username/
sudo rm ./.bash_history -fr

sudo sed -i 's/expand_rootfs_4_9.sh/init.sh \&/' /etc/rc.local

sync

sudo reboot
