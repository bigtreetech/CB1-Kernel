#!/bin/bash
# Auto install for 8192cu
# September, 1 2010 v1.0.0, willisTang
# 
# Add make_drv to select chip type
# Novembor, 21 2011 v1.1.0, Jeff Hung
################################################################################

echo "##################################################"
echo "Realtek Wi-Fi driver Auto installation script"
echo "Novembor, 21 2011 v1.1.0"
echo "##################################################"

################################################################################
#			Decompress the driver source tal ball
################################################################################
cd driver
Drvfoulder=`ls |grep .tar.gz`
echo "Decompress the driver source tar ball:"
echo "	"$Drvfoulder
tar zxvf $Drvfoulder

Drvfoulder=`ls |grep -iv '.tar.gz'`
echo "$Drvfoulder"
cd  $Drvfoulder

################################################################################
#			If makd_drv exixt, execute it to select chip type
################################################################################
if [ -e ./make_drv ]; then
	./make_drv
fi

################################################################################
#                       make clean
################################################################################
echo "Authentication requested [root] for make clean:"
if [ "`uname -r |grep fc`" == " " ]; then
        sudo su -c "make clean"; Error=$?
else
        su -c "make clean"; Error=$?
fi

################################################################################
#			Compile the driver
################################################################################
echo "Authentication requested [root] for make driver:"
if [ "`uname -r |grep fc`" == " " ]; then
	sudo su -c make; Error=$?
else	
	su -c make; Error=$?
fi
################################################################################
#			Check whether or not the driver compilation is done
################################################################################
module=`ls |grep -i 'ko'`
echo "##################################################"
if [ "$Error" != 0 ];then
	echo "Compile make driver error: $Error"
	echo "Please check error Mesg"
	echo "##################################################"
	exit
else
	echo "Compile make driver ok!!"	
	echo "##################################################"
fi

if [ "`uname -r |grep fc`" == " " ]; then
	echo "Authentication requested [root] for install driver:"
	sudo su -c "make install"
	echo "Authentication requested [root] for remove driver:"
	sudo su -c "modprobe -r ${module%.*}"
	echo "Authentication requested [root] for insert driver:"
	sudo su -c "modprobe ${module%.*}"
else
	echo "Authentication requested [root] for install driver:"
	su -c "make install"
	echo "Authentication requested [root] for remove driver:"
	su -c "modprobe -r ${module%.*}"
	echo "Authentication requested [root] for insert driver:"
	su -c "modprobe ${module%.*}"
fi
echo "##################################################"
echo "The Setup Script is completed !"
echo "##################################################"
