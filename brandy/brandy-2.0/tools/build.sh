#!/bin/bash

#
#build.sh for uboot/spl
#wangwei@allwinnertech
#

TOP_DIR=$(cd `dirname $0`;pwd;cd - >/dev/null 2>&1)
BRANDY_SPL_DIR=$TOP_DIR/spl
BRANDY_SPL_PUB_DIR=$TOP_DIR/spl-pub
SPL_OLD=$(grep  ".module.common.mk"  ${BRANDY_SPL_DIR}/Makefile)
set -e

get_sys_config_name()
{
	[ -f $TOP_DIR/u-boot-2018/.config ] && \
	awk -F '=' '/CONFIG_SYS_CONFIG_NAME=/{print $2}' $TOP_DIR/u-boot-2018/.config | sed 's|"||g'
}

show_help()
{
	printf "\nbuild.sh - Top level build scritps\n"
	echo "Valid Options:"
	echo "  -h  Show help message"
	echo "  -t install gcc tools chain"
	echo "  -o build,e.g. uboot,spl,clean"
	echo "  -p <platform> platform, e.g. sun8iw18p1,  sun5i, sun6i, sun8iw1p1, sun8iw3p1, sun9iw1p1"
	echo "  -m mode,e.g. nand,mmc,nor"
	echo "  -c copy,e.g. any para"
	echo "example:"
	echo "./build.sh -o uboot -p sun8iw18p1"
	echo "./build.sh -o spl -p sun8iw18p1 -m nand"
	echo "./build.sh -o spl -p sun8iw18p1 -m nand -c copy"
	printf "\n\n"
}

prepare_toolchain()
{
	local ARCH="arm";
	local GCC="";
	local GCC_PREFIX="";
	local toolchain_archive_arm="./tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz";
	local tooldir_arm="./tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi";

	echo "Prepare toolchain ..."

	if [ ! -e "${tooldir_arm}/.prepared" ]; then
		rm -rf "${toolchain_arm}"
		mkdir -p ${tooldir_arm} || exit 1
		tar --strip-components=1 -xf ${toolchain_archive_arm} -C ${tooldir_arm} || exit 1
		touch "${tooldir_arm}/.prepared"
	fi
}

function build_clean()
{
	(cd $TOP_DIR/spl; make distclean)
	(cd $TOP_DIR/spl-pub; make distclean)
	(cd $TOP_DIR/u-boot-2018; make distclean)
}

build_uboot_once()
{
	local defconfig=$1
	local CONFIG_SYS_CONFIG_NAME=$(get_sys_config_name)
	if [ "x${defconfig}" = "x" ];then
		echo "please set defconfig"
		exit 1
	fi
	echo build for ${defconfig} ...
	(
	cd u-boot-2018/
	if [ -f .tmp_defcofig.o.md5sum ];then
		last_defconfig_md5sum=$(awk '{printf $1}' ".tmp_defcofig.o.md5sum")
	fi
	if [ -f .config ];then
		cur_defconfig_md5sum=$(md5sum "configs/${defconfig}" | awk '{printf $1}')
		if [ "x${CONFIG_SYS_CONFIG_NAME}" != "x${PLATFORM}" ];then
			make distclean
			make ${defconfig}
		elif [ "x${last_defconfig_md5sum}" != "x${cur_defconfig_md5sum}" ];then
			make ${defconfig}
		fi
	else
		make distclean
		make ${defconfig}
	fi
	make -j16
	)
}

function build_uboot()
{
	if [ "x${PLATFORM}" = "xall" ];then
		for defconfig in `ls ${TOP_DIR}/u-boot-2018/configs`;do
			if [[ $defconfig =~ .*_defconfig$ ]];then
				build_uboot_once $defconfig
			fi
		done
	else
		build_uboot_once ${PLATFORM}_defconfig
		if [ -e ${TOP_DIR}/u-boot-2018/configs/${PLATFORM}_nor_defconfig ]; then
			build_uboot_once ${PLATFORM}_nor_defconfig
		fi
	fi
}

function build_spl_once()
{
	platform=$1
	mode=$2
	(
	cd $3
	if [ "x${mode}" = "xall" ];then
		echo --------build for platform:${platform}-------------------
		make distclean
		make p=${platform} ${CP}
		make -j ${CP}
	else
		echo --------build for platform:${platform} mode:${mode}-------------------
		make distclean
		make p=${platform} m=${mode} ${CP}
		case ${mode} in
			nand | mmc | spinor)
				make boot0 -j ${CP}
				;;
			sboot_nor)
				echo "Neednot build sboot_nor ..."
				;;
			*)
				make ${mode} -j ${CP}
				;;
		esac
	fi
	)
}


function build_spl()
{
	local CONFIG_SYS_CONFIG_NAME=$(get_sys_config_name)
	if [ ! -d $TOP_DIR/$1/board/${PLATFORM} ] && [ "x${PLATFORM}" != "xall" ];then
		PLATFORM=${CONFIG_SYS_CONFIG_NAME}
	fi

	if [ "x${PLATFORM}" = "xall" ];then
		for platform in `ls $TOP_DIR/$1/board`;do
			if [ "x${MODE}" = "xall" ];then
				build_spl_once ${platform} all $1
			else
				build_spl_once $platform ${MODE} $1
			fi
		done
	elif [ "x${MODE}" = "xall" ];then
		build_spl_once ${PLATFORM} all $1
	else
		build_spl_once ${PLATFORM} ${MODE} $1
	fi
}

function build_spl_old()
{
	if [ "x${PLATFORM}" = "xall" ];then
		for platform in `ls $TOP_DIR/spl/board`;do
			if [ "x${MODE}" = "xall" ];then
				for mode in `ls ${TOP_DIR}/spl/board/${platform}`;do
					if [[ $mode =~ .*\.mk$ ]]  \
					&& [ "x$mode" != "xcommon.mk" ];then
						mode=${mode%%.mk*}
						build_spl_once ${platform} ${mode} $1
					fi
				done
			else
				build_spl_once $platform ${MODE} $1
			fi
		done
	elif [ "x${MODE}" = "xall" ];then
		for mode in `ls ${TOP_DIR}/spl/board/${PLATFORM}`;do
			if [[ $mode =~ .*\.mk$ ]]  \
			&& [ "x$mode" != "xcommon.mk" ];then
				mode=${mode%%.mk*}
				build_spl_once ${PLATFORM} ${mode} $1
			fi
		done
	else
		build_spl_once ${PLATFORM} ${MODE} $1
	fi
}

function build_all()
{
	build_uboot

	if [ -d ${BRANDY_SPL_DIR} ] && [ "x${SPL_OLD}" != "x" ] ; then
		build_spl spl
	elif [ -d ${BRANDY_SPL_DIR} ] && [ "x${SPL_OLD}" = "x" ] ; then
		build_spl_old spl
	elif [ -d ${BRANDY_SPL_PUB_DIR} ];then
		build_spl spl-pub
	fi
}

while getopts to:p:m:c: OPTION; do
	case $OPTION in
	t)
		prepare_toolchain
		exit $?
		;;
	o)
		prepare_toolchain
		if [ "x$OPTARG" == "xspl-pub" ]; then
			command="build_spl spl-pub"
		elif [ "x${SPL_OLD}" = "x" ] && [ "x$OPTARG" = "xspl" ]; then
			command="build_spl_old $OPTARG"
		else
			command="build_$OPTARG $OPTARG"
		fi
		;;

	p)
		PLATFORM=$OPTARG
		;;
	m)
		MODE=$OPTARG
		;;
	c)
		CP=C\=$OPTION
		;;
	*)
		show_help
		exit $?
		;;
	esac
done

if [ "x${PLATFORM}" = "x" ];then
	PLATFORM=all
fi
if [ "x${MODE}" = "x" ];then
	MODE=all
fi
if [ "x$command" != "x" ];then
	$command
else
	build_all
fi
exit $?

