#!/bin/bash

cd ~

version=2.2.1

uboot_img="linux-u-boot-current-h616_${version}_arm64.deb"
dtb_img="linux-dtb-current-sun50iw9_${version}_arm64.deb"
kernel_img="linux-image-current-sun50iw9_${version}_arm64.deb"

if [ -e $uboot_img ];then
    echo -e "\n **** remove uboot ****\n"
    sudo apt purge -y linux-u-boot-h616-current
    echo -e "\n **** install uboot ****\n"
    sudo dpkg -i $uboot_img

    sudo nand-sata-install
fi

# sudo mv regulatory.db* /lib/firmware/

# sudo rm /lib/modules/5.16.17-sun50iw9 -fr
# sudo mv ./5.16.17-sun50iw9 /lib/modules/ -f

if [ -e $dtb_img ];then
    echo -e "\n **** remove dtb ****\n"
    sudo apt purge -y linux-dtb-current-sun50iw9
    echo -e "\n **** install dtb ****\n"
    sudo dpkg -i $dtb_img
fi

if [ -e $kernel_img ];then
    echo -e "\n **** remove kernel ****\n"
    sudo apt purge -y linux-image-current-sun50iw9
    echo -e "\n **** install kernel ****\n"
    sudo dpkg -i $kernel_img
fi

sudo rm ./linux-*.deb ./5.16.17-sun50iw9 -fr

sudo reboot

