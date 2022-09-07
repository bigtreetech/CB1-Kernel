#!/bin/bash
# Copyright (C) 2006-2011

function switch_adb()
{
	echo "-------swith to adb...."
	local cfgpath=/sys/kernel/config
	local adbd_proc=$(ps -ef | grep adbd | grep -v grep)

	if [ -z "$adbd_proc" ]; then
		mkdir -p /system/bin 2>/dev/null
		ln -s /bin/sh /system/bin/sh 2>/dev/null

		# config adb function
		mount -t configfs none $cfgpath
		mkdir $cfgpath/usb_gadget/g1 2>/dev/null
		echo "0x1f3a" > $cfgpath/usb_gadget/g1/idVendor
		echo "0x1007" > $cfgpath/usb_gadget/g1/idProduct
		mkdir $cfgpath/usb_gadget/g1/strings/0x409 2>/dev/null
		echo "0402101560" > $cfgpath/usb_gadget/g1/strings/0x409/serialnumber
		echo "Google.Inc" > $cfgpath/usb_gadget/g1/strings/0x409/manufacturer
		echo "Configfs ffs gadget" > $cfgpath/usb_gadget/g1/strings/0x409/product
		mkdir $cfgpath/usb_gadget/g1/functions/ffs.adb 2>/dev/null
		mkdir $cfgpath/usb_gadget/g1/configs/c.1 2>/dev/null
		mkdir $cfgpath/usb_gadget/g1/configs/c.1/strings/0x409 2>/dev/null
		echo 0xc0 > $cfgpath/usb_gadget/g1/configs/c.1/bmAttributes
		echo 500 > $cfgpath/usb_gadget/g1/configs/c.1/MaxPower
		ln -s $cfgpath/usb_gadget/g1/functions/ffs.adb/ $cfgpath/usb_gadget/g1/configs/c.1/ffs.adb 2>/dev/null
		mkdir /dev/usb-ffs 2>/dev/null
		mkdir /dev/usb-ffs/adb 2>/dev/null
		mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb/

		# start adbd daemon
		adbd &
		sleep 1
	fi

	# enable udc
	udc=$(ls /sys/class/udc)
	echo $udc > $cfgpath/usb_gadget/g1/UDC

	echo "-------swith to adb done!"
}

role0="null"

while true; do
	role1=$(cat /sys/devices/platform/soc/usbc0/otg_role 2>&1)
	if [ "x$role1" != "x$role0" ]; then
		if [ "x$role1" == "xusb_device" ] && [ "x$role0" == "xnull" ]; then
			switch_adb
		fi
		role0=$role1
	fi
	sleep 2
done
