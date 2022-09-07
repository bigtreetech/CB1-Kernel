# scripts/mkcmd.sh
#
# (c) Copyright 2013
# Allwinner Technology Co., Ltd. <www.allwinnertech.com>
# James Deng <csjamesdeng@allwinnertech.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# Notice:
#   1. This script muse source at the top directory of lichee.

cpu_cores=`cat /proc/cpuinfo | grep "processor" | wc -l`
if [ ${cpu_cores} -le 8 ] ; then
	LICHEE_JLEVEL=${cpu_cores}
else
	LICHEE_JLEVEL=`expr ${cpu_cores} / 2`
fi

export LICHEE_JLEVEL

function mk_error()
{
	echo -e "\033[47;31mERROR: $*\033[0m"
}

function mk_warn()
{
	echo -e "\033[47;34mWARN: $*\033[0m"
}

function mk_info()
{
	echo -e "\033[47;30mINFO: $*\033[0m"
}

# define importance variable
export LICHEE_TOP_DIR=`pwd`
export LICHEE_BUILD_DIR=${LICHEE_TOP_DIR}/build
export LICHEE_DEVICE_DIR=${LICHEE_TOP_DIR}/device
export LICHEE_PLATFORM_DIR=${LICHEE_TOP_DIR}/platform
export LICHEE_SATA_DIR=${LICHEE_TOP_DIR}/test/SATA
export LICHEE_DRAGONBAORD_DIR=${LICHEE_TOP_DIR}/test/dragonboard
export LICHEE_TOOLS_DIR=${LICHEE_TOP_DIR}/tools
export LICHEE_COMMON_CONFIG_DIR=${LICHEE_DEVICE_DIR}/config/common
export LICHEE_OUT_DIR=$LICHEE_TOP_DIR/out
export LICHEE_PACK_OUT_DIR=${LICHEE_OUT_DIR}/pack_out
export LICHEE_TOOLCHAIN_PATH=${LICHEE_OUT_DIR}/external-toolchain/gcc-arm

# make surce at the top directory of lichee
if [ ! -d ${LICHEE_BUILD_DIR} -o \
	! -d ${LICHEE_DEVICE_DIR} ] ; then
	mk_error "You are not at the top directory of lichee."
	mk_error "Please changes to that directory."
	exit 1
fi

allconfig=(
LICHEE_PLATFORM
LICHEE_LINUX_DEV
LICHEE_IC
LICHEE_CHIP
LICHEE_ARCH
LICHEE_KERN_VER
LICHEE_KERN_DEFCONF
LICHEE_KERN_DEFCONF_RELATIVE
LICHEE_KERN_DEFCONF_ABSOLUTE
LICHEE_BUILDING_SYSTEM
LICHEE_BR_VER
LICHEE_BR_DEFCONF
LICHEE_BR_RAMFS_CONF
LICHEE_PRODUCT
LICHEE_BOARD
LICHEE_FLASH
LICHEE_BRANDY_VER
LICHEE_BRANDY_DEFCONF
LICHEE_CROSS_COMPILER
LICHEE_COMPILER_TAR
LICHEE_ROOTFS
LICHEE_BUSSINESS
CONFIG_SESSION_SEPARATE
LICHEE_TOP_DIR
LICHEE_BRANDY_DIR
LICHEE_BUILD_DIR
LICHEE_BR_DIR
LICHEE_DEVICE_DIR
LICHEE_KERN_DIR
LICHEE_PLATFORM_DIR
LICHEE_SATA_DIR
LICHEE_DRAGONBAORD_DIR
LICHEE_TOOLS_DIR
CONFIG_SESSION_SEPARATE
LICHEE_COMMON_CONFIG_DIR
LICHEE_CHIP_CONFIG_DIR
LICHEE_BOARD_CONFIG_DIR
LICHEE_PRODUCT_CONFIG_DIR
CONFIG_SESSION_SEPARATE
LICHEE_OUT_DIR
LICHEE_BRANDY_OUT_DIR
LICHEE_BR_OUT
LICHEE_PACK_OUT_DIR
LICHEE_TOOLCHAIN_PATH
LICHEE_PLAT_OUT
LICHEE_BOARDCONFIG_PATH
CONFIG_SESSION_SEPARATE
ANDROID_CLANG_PATH
ANDROID_TOOLCHAIN_PATH
LICHEE_COMPRESS)

# export importance variable
platforms=(
"android"
"linux"
)

linux_development=(
"bsp"
)

flash=(
"default"
"nor"
)

cross_compiler=(
'linux-3.4	arm		gcc-linaro.tar.bz2	target_arm.tar.bz2'
'linux-3.4	arm64	gcc-linaro-aarch64.tar.xz	target_arm64.tar.bz2'
'linux-3.10	arm		gcc-linaro-arm.tar.xz	target_arm.tar.bz2'
'linux-3.10	arm64	gcc-linaro-aarch64.tar.xz	target_arm64.tar.bz2'
'linux-4.4	arm		gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi.tar.xz	target-arm-linaro-5.3.tar.bz2'
'linux-4.4	arm64	gcc-linaro-5.3.1-2016.05-x86_64_aarch64-linux-gnu.tar.xz	target-arm64-linaro-5.3.tar.bz2'
'linux-4.9	arm		gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi.tar.xz	target-arm-linaro-5.3.tar.bz2'
'linux-4.9	arm64	gcc-linaro-5.3.1-2016.05-x86_64_aarch64-linux-gnu.tar.xz	target-arm64-linaro-5.3.tar.bz2'
)

#eg. save_config "LICHEE_PLATFORM" "$LICHEE_PLATFORM" $BUILD_CONFIG
function save_config()
{
	local cfgkey=$1
	local cfgval=$2
	local cfgfile=$3
	local dir=$(dirname $cfgfile)
	[ ! -d $dir ] && mkdir -p $dir
	cfgval=$(echo -e "$cfgval" | sed -e 's/^\s\+//g' -e 's/\s\+$//g')
	if [ -f $cfgfile ] && [ -n "$(sed -n "/^\s*export\s\+$cfgkey\s*=/p" $cfgfile)" ]; then
		sed -i "s|^\s*export\s\+$cfgkey\s*=\s*.*$|export $cfgkey=$cfgval|g" $cfgfile
	else
		echo "export $cfgkey=$cfgval" >> $cfgfile
	fi
}

function load_config()
{
	local cfgkey=$1
	local cfgfile=$2
	local defval=$3
	local val=""

	[ -f "$cfgfile" ] && val="$(sed -n "/^\s*export\s\+$cfgkey\s*=/h;\${x;p}" $cfgfile | sed -e 's/^[^=]\+=//g' -e 's/^\s\+//g' -e 's/\s\+$//g')"
	eval echo "${val:-"$defval"}"
}

#
# This function can get the realpath between $SRC and $DST
#
function get_realpath()
{
	local src=$(cd $1; pwd);
	local dst=$(cd $2; pwd);
	local res="./";
	local tmp="$dst"

	while [ "${src##*$tmp}" == "${src}" ]; do
		tmp=${tmp%/*};
		res=$res"../"
	done
	res="$res${src#*$tmp/}"

	printf "%s" $res
}

function check_output_dir()
{
	#mkdir out directory:
	if [ "x" != "x${LICHEE_PLAT_OUT}" ]; then
		if [ ! -d ${LICHEE_PLAT_OUT} ]; then
			mkdir -p ${LICHEE_PLAT_OUT}
		fi
	fi

	if [ "x" != "x${LICHEE_PLATFORM}" ]; then
		if [ ${LICHEE_PLATFORM} = "linux" ] ; then
			if [ "x" != "x${LICHEE_BR_OUT}" ]; then
				if [ ! -d ${LICHEE_BR_OUT} ]; then
					mkdir -p ${LICHEE_BR_OUT}
				fi
			fi
		fi
	fi

}

function check_env()
{
	if [ -z "${LICHEE_IC}" -o \
		-z "${LICHEE_PLATFORM}" -o \
		-z "${LICHEE_BOARD}" ] ; then
		mk_error "run './build.sh config' setup env"
		exit 1
	fi

	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		if [ -z "${LICHEE_LINUX_DEV}" ] ; then
			mk_error "LICHEE_LINUX_DEV is invalid, run './build.sh config' setup env"
			exit 1
		fi
	fi

	cd ${LICHEE_DEVICE_DIR}
	ln -sfT $(get_realpath config/chips/ ./)/${LICHEE_IC} product
	cd - > /dev/null
}

function init_defconf()
{
	if [ "x${LICHEE_KERN_DIR}" != "x" ]; then
		local relative_path=""
		local absolute_path=""
		local KERNEL_DEFCONF_PATH=$LICHEE_KERN_DIR/arch/$LICHEE_ARCH/configs
		local search_tab
		if [ -n "$LICHEE_KERN_DEFCONF" ] && [ -f $KERNEL_DEFCONF_PATH/$LICHEE_KERN_DEFCONF ]; then
			search_tab+=($KERNEL_DEFCONF_PATH/${LICHEE_KERN_DEFCONF})
		else
			[ -n "${LICHEE_LINUX_DEV}" ] && \
			search_tab+=($LICHEE_BOARD_CONFIG_DIR/$LICHEE_LINUX_DEV/config-${LICHEE_KERN_VER:6})
			search_tab+=($LICHEE_BOARD_CONFIG_DIR/$LICHEE_PLATFORM/config-${LICHEE_KERN_VER:6})
			search_tab+=($LICHEE_CHIP_CONFIG_DIR/configs/default/config-${LICHEE_KERN_VER:6})
		fi

		for absolute_path in "${search_tab[@]}"; do
			if [ -f $absolute_path ]; then
				relative_path=$(python -c "import os.path; print os.path.relpath('$absolute_path', '$KERNEL_DEFCONF_PATH')")
				break
			fi
		done

		if [ -z "$relative_path" ]; then
			mk_error "Can't find kernel defconfig!"
			exit 1
		fi
		save_config "LICHEE_KERN_DEFCONF_RELATIVE" ${relative_path} $BUILD_CONFIG
		save_config "LICHEE_KERN_DEFCONF_ABSOLUTE" ${absolute_path} $BUILD_CONFIG
		mk_info "kernel defconfig: generate ${LICHEE_KERN_DIR}/.config by ${absolute_path}"
		(cd ${LICHEE_KERN_DIR};make ARCH=${LICHEE_ARCH} defconfig KBUILD_DEFCONFIG=${relative_path})
	fi
	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		if [ -d ${LICHEE_BR_DIR} -a -f ${LICHEE_BR_DIR}/configs/${LICHEE_BR_DEFCONF} ]; then
			rm -rf ${LICHEE_BR_OUT}/.config
			(cd ${LICHEE_BR_DIR};make O=${LICHEE_BR_OUT} -C ${LICHEE_BR_DIR} ${LICHEE_BR_DEFCONF})
			mk_info "buildroot defconfig is ${LICHEE_BR_DEFCONF} "
		else
			mk_info "skip buildroot"
		fi
	fi

	#mkdir out directory:
	check_output_dir
}

function list_subdir()
{
	echo "$(eval "$(echo "$(ls -d $1/*/)" | sed  "s/^/basename /g")")"
}

function init_key()
{
	local val_list=$1
	local cfg_key=$2
	local cfg_val=$3

	if [ -n "$(echo $val_list | grep -w $cfg_val)" ]; then
		export $cfg_key=$cfg_val
		return 0
	else
		return 1
	fi
}

function init_chips()
{
	local cfg_val=$1 # chip
	local cfg_key="LICHEE_CHIP"
	local val_list=$(list_subdir $LICHEE_DEVICE_DIR/config/chips)
	init_key "$val_list" "$cfg_key" "$cfg_val"
	return $?
}

function init_ic()
{
	local cfg_val=$1 # chip
	local cfg_key="LICHEE_IC"
	local val_list=$(list_subdir $LICHEE_DEVICE_DIR/config/chips)
	init_key "$val_list" "$cfg_key" "$cfg_val"
	return $?
}

function init_platforms()
{
	local cfg_val=$1 # platform
	local cfg_key="LICHEE_PLATFORM"
	local val_list=${platforms[@]}
	init_key "$val_list" "$cfg_key" "$cfg_val"
	return $?
}

function init_kern_ver()
{
	local cfg_val=$1 # kern_ver
	local cfg_key="LICHEE_KERN_VER"
	local val_list=$(list_subdir $LICHEE_TOP_DIR | grep "linux-")
	init_key "$val_list" "$cfg_key" "$cfg_val"
	return $?
}

function init_boards()
{
	local chip=$1
	local cfg_val=$2 # board
	local cfg_key="LICHEE_BOARD"
	local val_list=$(list_subdir $LICHEE_DEVICE_DIR/config/chips/$chip/configs | grep -v default)
	init_key "$val_list" "$cfg_key" "$cfg_val"
	return $?
}

function mk_select()
{
	local val_list=$1
	local cfg_key=$2
	local cnt=0
	local cfg_val=$(load_config $cfg_key $BUILD_CONFIG)
	local cfg_idx=0
	local banner=$(echo ${cfg_key:7} | tr '[:upper:]' '[:lower:]')

	printf "All available $banner:\n"
	for val in $val_list; do
		array[$cnt]=$val
		if [ "X_$cfg_val" == "X_${array[$cnt]}" ]; then
			cfg_idx=$cnt
		fi
		printf "%4d. %s\n" $cnt $val
		let "cnt++"
	done
	while true; do
		read -p "Choice [${array[$cfg_idx]}]: " choice
		if [ -z "${choice}" ]; then
			choice=$cfg_idx
		fi

		if [ -z "${choice//[0-9]/}" ] ; then
			if [ $choice -ge 0 -a $choice -lt $cnt ] ; then
				cfg_val="${array[$choice]}"
				break;
			fi
		fi
		 printf "Invalid input ...\n"
	done
	export $cfg_key=$cfg_val
	save_config "$cfg_key" "$cfg_val" $BUILD_CONFIG
}

function mk_autoconfig()
{
	local IC=$1
	local platform=$2
	local board=$3
	local flash=$4
	local linux_ver=$5
	local linux_dev=""

	if [ "${platform}" != "android" ]; then
		linux_dev=${platform}
		platform="linux"
	fi

	export LICHEE_IC=${IC}
	export LICHEE_BOARD=${board}
	export LICHEE_PLATFORM=${platform}
	export LICHEE_FLASH=${flash}
	export LICHEE_LINUX_DEV=${linux_dev}

	if [ -n ${linux_ver} ]; then
		export LICHEE_KERN_VER=${linux_ver}
	fi

	#parse boardconfig
	parse_boardconfig

	init_global_variable

	#save config to buildconfig
	save_config_to_buildconfig

	#init defconfig
	init_defconf
}

function mk_config()
{
	#select config
	select_platform

	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		select_linux_development
		if [ ${LICHEE_LINUX_DEV} = "bsp" ] ; then
			select_kern_ver
		fi
		if [ ${LICHEE_LINUX_DEV} = "sata" ] ; then
			select_sata_module
		fi
	fi

	select_ic
	select_board

	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		select_flash
	fi

	#parse boardconfig
	parse_boardconfig

	init_global_variable
	#print_config

	#save config to buildconfig
	save_config_to_buildconfig
	#print_buildconfig

	#init defconfig
	init_defconf
}

function clean_old_env_var()
{
	local cfgkey
	for cfgkey in ${allconfig[@]}; do
		[ "x$cfgkey" == "xCONFIG_SESSION_SEPARATE" ] && continue
		export $cfgkey=""
	done
}

function init_global_variable()
{
	check_env

	export LICHEE_BRANDY_DIR=${LICHEE_TOP_DIR}/brandy/brandy-${LICHEE_BRANDY_VER}
	export  LICHEE_KERN_DIR=${LICHEE_TOP_DIR}/kernel/${LICHEE_KERN_VER}
	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		export LICHEE_BR_DIR=${LICHEE_TOP_DIR}/buildroot/buildroot-${LICHEE_BR_VER}
	fi

	export LICHEE_PRODUCT_CONFIG_DIR=${LICHEE_DEVICE_DIR}/target/${LICHEE_PRODUCT}

	export LICHEE_BRANDY_OUT_DIR=${LICHEE_DEVICE_DIR}/config/chips/${LICHEE_IC}/bin
	export LICHEE_BR_OUT=${LICHEE_OUT_DIR}/${LICHEE_IC}/${LICHEE_BOARD}/${LICHEE_LINUX_DEV}/buildroot

	if [ ${LICHEE_PLATFORM} = "android" ] ; then
		export LICHEE_PLAT_OUT=${LICHEE_OUT_DIR}/${LICHEE_IC}/${LICHEE_BOARD}/${LICHEE_PLATFORM}
	else
		export LICHEE_PLAT_OUT=${LICHEE_OUT_DIR}/${LICHEE_IC}/${LICHEE_BOARD}/${LICHEE_LINUX_DEV}
	fi
}

function parse_rootfs_tar()
{
	for i in "${cross_compiler[@]}" ; do
		local arr=($i)

		for j in "${arr[@]}" ; do
			if [ ${j} = $1 ] ; then
				local arch=`echo ${arr[@]} | awk '{print $2}'`

				if [ ${arch} = $2 ] ; then
					LICHEE_ROOTFS=`echo ${arr[@]} | awk '{print $4}'`
				fi
			fi
		done
	done

	if [ -z ${LICHEE_ROOTFS} ] ; then
		mk_error "can not match LICHEE_ROOTFS."
		exit 1
	fi
}


function parse_cross_compiler()
{
	for i in "${cross_compiler[@]}" ; do
		local arr=($i)

		for j in "${arr[@]}" ; do
			if [ ${j} = $1 ] ; then
				local arch=`echo ${arr[@]} | awk '{print $2}'`

				if [ ${arch} = $2 ] ; then
					LICHEE_COMPILER_TAR=`echo ${arr[@]} | awk '{print $3}'`
				fi
			fi
		done
	done

	if [ -z ${LICHEE_COMPILER_TAR} ] ; then
		mk_error "can not match LICHEE_COMPILER_TAR."
		exit 1
	fi
}

function substitute_inittab()
{

	declare console
	env_cfg_dir=${LICHEE_CHIP_CONFIG_DIR}/configs/default/env.cfg
	if [ "x${LICHEE_PLATFORM}" = "xlinux" ]; then
		if [ "x${LICHEE_LINUX_DEV}" != "xbsp" ] && [ "x${LICHEE_LINUX_DEV}" != "xdragonboard" ]; then
			if [ "x${LICHEE_LINUX_DEV}" = "xlongan" ];then
				env_cfg_dir=${LICHEE_BOARD_CONFIG_DIR}/${LICHEE_PLATFORM}/env.cfg
			else
				env_cfg_dir=${LICHEE_BOARD_CONFIG_DIR}/${LICHEE_LINUX_DEV}/env.cfg
			fi
			if [ ! -f ${env_cfg_dir} ];then
				env_cfg_dir=${LICHEE_CHIP_CONFIG_DIR}/configs/default/env.cfg
			fi
		fi
	fi
	if [ "x${LICHEE_PLATFORM}" = "xandroid" ]; then
		env_cfg_dir=${LICHEE_BOARD_CONFIG_DIR}/${LICHEE_PLATFORM}/env.cfg
		if [ ! -f ${env_cfg_dir} ];then
			env_cfg_dir=${LICHEE_CHIP_CONFIG_DIR}/configs/default/env.cfg
		fi
	fi

	if [ ! -f ${env_cfg_dir} ];then
		mk_info "not find env.cfg in ${env_cfg_dir}"
		return;
	fi
	console=$(grep -m1 -o ${env_cfg_dir} -e 'console=\w\+')
	console=$(sed -e 's/console=\(\w\+\).*/\1/g' <<< $console)

	if [ ${console} ]
	then
		sed -ie "s/ttyS[0-9]*/${console}/g" $1
	fi

}

function parse_boardconfig()
{
	check_env

	export LICHEE_CHIP_CONFIG_DIR=${LICHEE_DEVICE_DIR}/config/chips/${LICHEE_IC}
	export LICHEE_BOARD_CONFIG_DIR=${LICHEE_DEVICE_DIR}/config/chips/${LICHEE_IC}/configs/${LICHEE_BOARD}

	local default_config_android="${LICHEE_CHIP_CONFIG_DIR}/configs/default/BoardConfig_android.mk"
	local default_config_nor="${LICHEE_CHIP_CONFIG_DIR}/configs/default/BoardConfig_nor.mk"
	local default_config="${LICHEE_CHIP_CONFIG_DIR}/configs/default/BoardConfig.mk"
	local special_config_android="${LICHEE_BOARD_CONFIG_DIR}/android/BoardConfig.mk"
	local special_config_linux_nor="${LICHEE_BOARD_CONFIG_DIR}/${LICHEE_LINUX_DEV}/BoardConfig_nor.mk"
	local special_config_linux="${LICHEE_BOARD_CONFIG_DIR}/${LICHEE_LINUX_DEV}/BoardConfig.mk"
	local special_config="${LICHEE_BOARD_CONFIG_DIR}/BoardConfig.mk"
	local config_list=""
	# find BoardConfig.mk path
	# Note: Order is important in config_list
	if [ "x${LICHEE_PLATFORM}" = "xandroid" ]; then
		config_list=($default_config $default_config_android $special_config $special_config_android)
	elif [ "x${LICHEE_PLATFORM}" = "xlinux" ]; then
		config_list=($default_config $default_config_nor $special_config $special_config_linux $special_config_linux_nor)
		[ ${LICHEE_FLASH} != "nor" ] && config_list=($default_config $special_config $special_config_linux)
	else
		mk_error "Unsupport LICHEE_PLATFORM!"
	fi

	local fetch_list=""
	local fpare_list=""
	for f in ${config_list[@]}; do
		if [ -f $f ]; then
			fetch_list=(${fetch_list[@]} $f)
			fpare_list="$fpare_list -f $f"
		fi
	done

	if [ -z "${fetch_list[0]}" ]; then
		mk_error "BoardConfig not found!"
		exit 1
	fi

	export LICHEE_BOARDCONFIG_PATH="\"${fetch_list[@]}\""

	#parse BoardConfig.mk
	local lichee_kern_ver=$LICHEE_KERN_VER
	local cfgkeylist=(
			LICHEE_KERN_DEFCONF  LICHEE_BUILDING_SYSTEM  LICHEE_BR_VER        LICHEE_BR_DEFCONF
			LICHEE_PRODUCT       LICHEE_ARCH             LICHEE_BRANDY_VER    LICHEE_BRANDY_DEFCONF
			LICHEE_COMPILER_TAR  LICHEE_ROOTFS           LICHEE_KERN_VER      LICHEE_BUSSINESS
			ANDROID_CLANG_PATH   ANDROID_TOOLCHAIN_PATH  LICHEE_BR_RAMFS_CONF LICHEE_CHIP
			LICHEE_COMPRESS)

	local cfgkey=""
	local cfgval=""
	for cfgkey in ${cfgkeylist[@]}; do
		cfgval="$(echo '__unique:;@echo ${'"$cfgkey"'}' | make -f - $fpare_list --no-print-directory __unique)"
		eval "$cfgkey='$cfgval'"
	done

	LICHEE_KERN_VER="linux-$LICHEE_KERN_VER"
	[ -n "$lichee_kern_ver" ] && LICHEE_KERN_VER="$lichee_kern_ver"

	if [ -z ${LICHEE_COMPILER_TAR} ] ; then
		parse_cross_compiler ${LICHEE_KERN_VER} ${LICHEE_ARCH}
	fi
	if [ -z ${LICHEE_ROOTFS} ]; then
		parse_rootfs_tar ${LICHEE_KERN_VER} ${LICHEE_ARCH}
	fi
}

function save_config_to_buildconfig()
{
	local cfgkey=""
	local cfgval=""

	for cfgkey in ${allconfig[@]}; do
		[ "x$cfgkey" == "xCONFIG_SESSION_SEPARATE" ] && continue
		cfgval="$(eval echo '$'${cfgkey})"
		save_config "$cfgkey" "$cfgval" $BUILD_CONFIG
	done
}

function print_buildconfig()
{
	printf "printf .buildconfig:\n"
	cat ./.buildconfig
}

function print_config()
{
	echo "boardconfig:"
	for ((i=0;i<${#allconfig[@]};i++)); do
		[ "x${allconfig[$i]}" == "xCONFIG_SESSION_SEPARATE" ] && break
		echo "	${allconfig[$i]}=$(eval echo '$'"${allconfig[$i]}")"
	done

	echo "top directory:"
	for ((i++;i<${#allconfig[@]};i++)); do
		[ "x${allconfig[$i]}" == "xCONFIG_SESSION_SEPARATE" ] && break
		echo "	${allconfig[$i]}=$(eval echo '$'"${allconfig[$i]}")"
	done

	echo "config:"
	for ((i++;i<${#allconfig[@]};i++)); do
		[ "x${allconfig[$i]}" == "xCONFIG_SESSION_SEPARATE" ] && break
		echo "	${allconfig[$i]}=$(eval echo '$'"${allconfig[$i]}")"
	done

	echo "out directory:"
	for((i++;i<${#allconfig[@]};i++)); do
		echo "	${allconfig[$i]}=$(eval echo '$'"${allconfig[$i]}")"
	done
}

function select_ic()
{
	local val_list=$(list_subdir $LICHEE_DEVICE_DIR/config/chips)
	local cfg_key="LICHEE_IC"
	mk_select "$val_list" "$cfg_key"
}

function select_platform()
{
	local val_list="${platforms[@]}"
	local cfg_key="LICHEE_PLATFORM"
	mk_select "$val_list" "$cfg_key"
}

function select_linux_development()
{
	local val_list="${linux_development[@]}"
	local cfg_key="LICHEE_LINUX_DEV"

	[ -d $LICHEE_DRAGONBAORD_DIR ] && \
	val_list="$val_list dragonboard"

	[ -d $LICHEE_SATA_DIR ] && \
	val_list="$val_list sata"

	[ -n "$(find $LICHEE_TOP_DIR/buildroot -maxdepth 1 -type d -name "buildroot-*" 2>/dev/null)" ] && \
	val_list="$val_list longan tinyos"

	mk_select "$val_list" "$cfg_key"
}

function select_sata_module()
{
	local val_list_all=$(list_subdir $LICHEE_SATA_DIR/linux/bsptest)
	local val_list="all"
	for val in $val_list_all;do
		if [ "x$val" != "xscript" -a "x$val" != "xconfigs" ];then
			val_list="$val_list $val"
		fi
	done
	local cfg_key="LICHEE_BTEST_MODULE"
	mk_select "$val_list" "$cfg_key"
}

function select_flash()
{
	local val_list="${flash[@]}"
	local cfg_key="LICHEE_FLASH"
	mk_select "$val_list" "$cfg_key"
}

function select_kern_ver()
{
	local val_list=$(list_subdir $LICHEE_TOP_DIR/kernel | grep "linux-")
	local cfg_key="LICHEE_KERN_VER"
	mk_select "$val_list" "$cfg_key"
}

function select_board()
{
	local val_list=$(list_subdir $LICHEE_DEVICE_DIR/config/chips/$LICHEE_IC/configs | grep -v default)
	local cfg_key="LICHEE_BOARD"
	mk_select "$val_list" "$cfg_key"
}

function get_file_create_time()
{
	local file=$1;
	local date=""
	local time=""
	local create_time=""

	if [ -f ${file} ]; then
		date="`ls --full-time ${file} | cut -d ' ' -f 6`"
		time="`ls --full-time ${file} | cut -d ' ' -f 7`"
		create_time="${date} ${time}"

		printf "%s" "${create_time}"
	fi
}

function modify_longan_config()
{
	local LONGAN_CONFIG="${LICHEE_BR_OUT}/target/etc/longan.conf"
	local KERNEL_IMG="${LICHEE_KERN_DIR}/output/uImage"
	local BUILDROOT_ROOTFS="${LICHEE_BR_OUT}/images/rootfs.ext4"
	local kernel_time=""
	local rootfs_time=""

	if [ -f ${LONGAN_CONFIG} ]; then
		kernel_time=$(get_file_create_time ${KERNEL_IMG})
		rootfs_time=$(get_file_create_time ${BUILDROOT_ROOTFS})

		#rootfs_time
		sed -i '/SYSTEM_VERSION/ s/$/ '"${rootfs_time}"'/g' ${LONGAN_CONFIG}
	fi
}

function mkramfs()
{
	mk_info "build ramfs ..."

	local build_script="build.sh"
	(cd ${LICHEE_BR_DIR} && [ -x ${build_script} ] && ./${build_script} ramfs ${LICHEE_BR_RAMFS_CONF})
	[ $? -ne 0 ] && mk_error "build ramfs Failed" && return 1
}

function mkbr()
{
	mk_info "build buildroot ..."

	#build buildroot
	local build_script="build.sh"

	[ $? -ne 0 ] && mk_error "prepare toolchain Failed!" && return 1

	(cd ${LICHEE_BR_DIR} && [ -x ${build_script} ] && ./${build_script})
	[ $? -ne 0 ] && mk_error "build buildroot Failed" && return 1

	#copy files to rootfs
	if [ -d ${LICHEE_BR_OUT}/target ]; then
		mk_info "copy the config files form device ..."
		if [ ! -e ${LICHEE_PLATFORM_DIR}/Makefile ]; then
			ln -fs ${LICHEE_BUILD_DIR}/Makefile  ${LICHEE_PLATFORM_DIR}/Makefile
		fi
		make -C ${LICHEE_PLATFORM_DIR}/ INSTALL_FILES
		[ $? -ne 0 ] && mk_error "copy the config files from device Failed" && return 1
	else
		mk_error "you nend build buildroot first!" && return 1
	fi

	modify_longan_config

	if [ ${LICHEE_LINUX_DEV} != "longan" ] ; then
		local rootfs_name=""

		if [ -n "`echo $LICHEE_KERN_VER | grep "linux-4.[49]"`" ]; then
			rootfs_name=target-${LICHEE_ARCH}-linaro-5.3.tar.bz2
		else
			rootfs_name=target_${LICHEE_ARCH}.tar.bz2
		fi

		mk_info "create rootfs tar ..."
		(cd ${LICHEE_BR_OUT}/target && tar -jcf ${rootfs_name} ./* && mv ${rootfs_name} ${LICHEE_DEVICE_DIR}/config/rootfs_tar)
	fi

	mk_info "build buildroot OK."
}

function clbr()
{
	mk_info "clean buildroot ..."

	local build_script="build.sh"
	(cd ${LICHEE_BR_DIR} && [ -x ${build_script} ] && ./${build_script} "clean")

	mk_info "clean buildroot OK."
}

function prepare_buildserver()
{
	cd $LICHEE_TOP_DIR/tools/build
	if [ -f buildserver ]; then
		./buildserver --path $LICHEE_TOP_DIR &
	fi
}

function clbuildserver()
{
	mk_info "clean buildserver"
	lsof $LICHEE_TOP_DIR/tools/build/buildserver | grep buildserver | awk '{ print $2}' | xargs kill -9
}

function prepare_toolchain()
{
	local ARCH=""
	local GCC=""
	local GCC_PREFIX=""
	local toolchain_archive=""
	local toolchain_archive_tmp=""
	local toolchain_archivedir=""
	local tooldir=""
	local boardconfig="${LICHEE_BOARDCONFIG_PATH}"

	mk_info "Prepare toolchain ..."

	if [ -n "$ANDROID_CLANG_PATH" ]; then
		if  [ ! -f "$LICHEE_TOP_DIR/../$ANDROID_CLANG_PATH/clang" ]; then
			mk_error "Cannot find android clang!"
			exit 1
		fi
		if [ -n "$ANDROID_TOOLCHAIN_PATH" ]; then
			if [ ! -d "$LICHEE_TOP_DIR/../$ANDROID_TOOLCHAIN_PATH" ]; then
				mk_error "Cannot find android toolchain!"
				exit 1
			fi
			return 0
		fi
	fi

	toolchain_archive=${LICHEE_COMPILER_TAR}
	toolchain_archivedir=${LICHEE_BUILD_DIR}/toolchain/${toolchain_archive}
	if [ ! -f ${toolchain_archivedir} ]; then
		mk_error "Prepare toolchain error!"
		exit 1
	fi

	toolchain_archive_tmp=${toolchain_archive%.*}
	tooldir=${LICHEE_OUT_DIR}/${toolchain_archive_tmp%.*}

	if [ ! -d "${tooldir}" ]; then
		mkdir -p ${tooldir} || exit 1
		tar --strip-components=1 -xf ${toolchain_archivedir} -C ${tooldir} || exit 1
	fi

	GCC=$(find ${tooldir} -perm /a+x -a -regex '.*-gcc');
	if [ -z "${GCC}" ]; then
		tar --strip-components=1 -xf ${toolchain_archivedir} -C ${tooldir} || exit 1
		GCC=$(find ${tooldir} -perm /a+x -a -regex '.*-gcc');
	fi
	GCC_PREFIX=${GCC##*/};

	if [ "${tooldir}" == "${LICHEE_TOOLCHAIN_PATH}" \
		-a "${LICHEE_CROSS_COMPILER}-gcc" == "${GCC_PREFIX}" \
		-a -x "${GCC}" ]; then
		return
	fi

	if ! echo $PATH | grep -q "${tooldir}" ; then
		export PATH=${tooldir}/bin:$PATH
	fi

	LICHEE_CROSS_COMPILER="${GCC_PREFIX%-*}";

	if [ -n ${LICHEE_CROSS_COMPILER} ]; then
		export LICHEE_CROSS_COMPILER=${LICHEE_CROSS_COMPILER}
		export LICHEE_TOOLCHAIN_PATH=${tooldir}
		save_config "LICHEE_CROSS_COMPILER" "$LICHEE_CROSS_COMPILER" ${BUILD_CONFIG}
		save_config "LICHEE_TOOLCHAIN_PATH" "$tooldir" ${BUILD_CONFIG}
	fi
}

function prepare_dragonboard_toolchain()
{
	local ARCH="arm";
	local GCC="";
	local GCC_PREFIX="";
	local toolchain_archive="${LICHEE_BUILD_DIR}/toolchain/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi.tar.xz";
	local tooldir="";

	mk_info "Prepare dragonboard toolchain ..."
	tooldir=${LICHEE_OUT_DIR}/gcc-linaro-5.3.1-2016.05/dragonboard/gcc-arm

	if [ ! -d "${tooldir}" ]; then
		mkdir -p ${tooldir} || exit 1
		tar --strip-components=1 -xf ${toolchain_archive} -C ${tooldir} || exit 1
	fi


	GCC=$(find ${tooldir} -perm /a+x -a -regex '.*-gcc');
	if [ -z "${GCC}" ]; then
		tar --strip-components=1 -xf ${toolchain_archive} -C ${tooldir} || exit 1
		GCC=$(find ${tooldir} -perm /a+x -a -regex '.*-gcc');
	fi
	GCC_PREFIX=${GCC##*/};

	if [ "${tooldir}" == "${LICHEE_TOOLCHAIN_PATH}" \
		-a "${LICHEE_CROSS_COMPILER}-gcc" == "${GCC_PREFIX}" \
		-a -x "${GCC}" ]; then
		return
	fi

	if ! echo $PATH | grep -q "${tooldir}" ; then
		export PATH=${tooldir}/bin:$PATH
	fi


	LICHEE_CROSS_COMPILER="${GCC_PREFIX%-*}";

	if [ -n ${LICHEE_CROSS_COMPILER} ]; then
		export LICHEE_CROSS_COMPILER=${LICHEE_CROSS_COMPILER}
		export LICHEE_TOOLCHAIN_PATH=${tooldir}
	fi
}

function prepare_mkkernel()
{
	# mark kernel .config belong to which platform
	local config_mark="${LICHEE_KERN_DIR}/.config.mark"
	local board_dts="$LICHEE_BOARD_CONFIG_DIR/board.dts"
	if [ -f ${board_dts} ]; then
		if [ "x${LICHEE_ARCH}" == "xarm64" ]; then
			cp $board_dts ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/sunxi/
		else
			cp $board_dts ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/
		fi
	fi
	if [ -f ${config_mark} ] ; then
		local tmp=`cat ${config_mark}`
		local tmp1="${LICHEE_CHIP}_${LICHEE_BOARD}_${LICHEE_PLATFORM}"
		if [ ${tmp} != ${tmp1} ] ; then
			mk_info "clean last time build for different platform"
			if [ "x${LICHEE_KERN_DIR}" != "x" -a -d ${LICHEE_KERN_DIR} ]; then
				(cd ${LICHEE_KERN_DIR} && [ -x ${build_script} ] && ./${build_script} "clean")
				rm -rf ${LICHEE_KERN_DIR}/.config
				echo "${LICHEE_CHIP}_${LICHEE_BOARD}_${LICHEE_PLATFORM}" > ${config_mark}
			fi
		fi
	else
		echo "${LICHEE_CHIP}_${LICHEE_BOARD}_${LICHEE_PLATFORM}" > ${config_mark}
	fi
}

function mkdts(){
	local build_script="scripts/build.sh"
	prepare_toolchain
	prepare_mkkernel
	(cd ${LICHEE_KERN_DIR} && [ -x ${build_script} ] && ./${build_script} dts)

	if [ "x$LICHEE_KERN_VER" != "xlinux-3.4" ]; then
		cp ${LICHEE_KERN_DIR}/scripts/dtc/dtc ${LICHEE_PLAT_OUT}
		local dts_path=$LICHEE_KERN_DIR/arch/${LICHEE_ARCH}/boot/dts
		[ "x${LICHEE_ARCH}" == "xarm64" ] && \
		dts_path=$dts_path/sunxi
		rm -rf ${LICHEE_PLAT_OUT}/.sun*.dtb.*.tmp
		cp $dts_path/.${LICHEE_CHIP}-*.dtb.d.dtc.tmp ${LICHEE_PLAT_OUT}
		cp $dts_path/.${LICHEE_CHIP}-*.dtb.dts.tmp ${LICHEE_PLAT_OUT}

		cp $dts_path/.board.dtb.d.dtc.tmp ${LICHEE_PLAT_OUT} 2>/dev/null
		cp $dts_path/.board.dtb.dts.tmp ${LICHEE_PLAT_OUT} 2>/dev/null
	fi
}

function mkkernel()
{
	mk_info "build kernel ..."

	local build_script="scripts/build.sh"

	prepare_buildserver
	prepare_toolchain

	prepare_mkkernel

	(cd ${LICHEE_KERN_DIR} && [ -x ${build_script} ] && ./${build_script})
	[ $? -ne 0 ] && mk_error "build kernel Failed" && return 1

	# copy files related to pack to platform out
	cp $BUILD_CONFIG $LICHEE_PLAT_OUT
	cp ${LICHEE_KERN_DIR}/vmlinux ${LICHEE_PLAT_OUT}
	if [ "x$LICHEE_KERN_VER" != "xlinux-3.4" ]; then
		cp ${LICHEE_KERN_DIR}/scripts/dtc/dtc ${LICHEE_PLAT_OUT}
		local dts_path=$LICHEE_KERN_DIR/arch/${LICHEE_ARCH}/boot/dts
		[ "x${LICHEE_ARCH}" == "xarm64" ] && \
		dts_path=$dts_path/sunxi
		rm -rf ${LICHEE_PLAT_OUT}/.sun*.dtb.*.tmp
		cp $dts_path/.${LICHEE_CHIP}-*.dtb.d.dtc.tmp ${LICHEE_PLAT_OUT} 2>/dev/null
		cp $dts_path/.${LICHEE_CHIP}-*.dtb.dts.tmp ${LICHEE_PLAT_OUT} 2>/dev/null

		cp $dts_path/.board.dtb.d.dtc.tmp ${LICHEE_PLAT_OUT} 2>/dev/null
		cp $dts_path/.board.dtb.dts.tmp ${LICHEE_PLAT_OUT} 2>/dev/null
	fi

	mk_info "build kernel OK."

	#delete board.dts
	if [ "x${LICHEE_ARCH}" == "xarm64" ]; then
		if [ -f ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/sunxi/board.dts ]; then
			rm ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/sunxi/board.dts
		fi
	else
		if [ -f ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/board.dts ];then
			rm ${LICHEE_KERN_DIR}/arch/${LICHEE_ARCH}/boot/dts/board.dts
		fi
	fi
}

function clkernel()
{
	local clarg="clean"

	if [ "x$1" == "xdistclean" ]; then
		clarg="distclean"
	fi

	mk_info "clean kernel ..."

	local build_script="scripts/build.sh"

	prepare_toolchain

	(cd ${LICHEE_KERN_DIR} && [ -x ${build_script} ] && ./${build_script} "$clarg")

	mk_info "clean kernel OK."
}

function cldragonboard()
{
	if  [ "x$LICHEE_PLATFORM" == "xlinux" ] && \
		[ "x$LICHEE_LINUX_DEV" == "xdragonboard" ] && \
		[ -n "$(ls $LICHEE_PLAT_OUT)" ]; then
		mk_info "clean dragonboard ..."

		local script_dir="${LICHEE_DRAGONBAORD_DIR}/common/scripts/"

		local clean_script="clean.sh"
		(cd ${script_dir} && [ -x ${clean_script} ] && ./${clean_script})

		mk_info "clean dragonboard OK."
	fi
}

function mkboot()
{
	mk_info "build boot ..."
	mk_info "build boot OK."
}

function mkbootloader()
{
	local CURDIR=$PWD
	local brandy_path=""
	local build_script="build.sh"

	if [ ${LICHEE_BRANDY_VER} = "1.0" ] ; then
		brandy_path=${LICHEE_BRANDY_DIR}/brandy
	elif [ ${LICHEE_BRANDY_VER} = "2.0" ] ; then
		brandy_path=${LICHEE_BRANDY_DIR}
	else
		echo "unkown brandy version, version=${LICHEE_BRANDY_VER}"
		exit 1;
	fi

	mk_info "mkbootloader: brandy_path= ${brandy_path}"
	(cd ${brandy_path} && [ -x ${build_script} ] && ./${build_script} -p ${LICHEE_BRANDY_DEFCONF%%_def*})
	[ $? -ne 0 ] && mk_error "build brandy Failed" && return 1

	mk_info "build brandy OK."

	cd $CURDIR
}

function mkbrandy()
{
	mkbootloader
}

function mksata()
{
	if [ "x$LICHEE_LINUX_DEV" = "xsata" ];then
		clsata
		mk_info "build sata ..."

		local build_script="linux/bsptest/script/bsptest.sh"
		local sata_config="${LICHEE_SATA_DIR}/linux/bsptest/script/Config"
		. ${sata_config}
		[ "x$LICHEE_BTEST_MODULE" = "x" ] && LICHEE_BTEST_MODULE="all"

		(cd ${LICHEE_SATA_DIR} && [ -x ${build_script} ] && ./${build_script} -b $LICHEE_BTEST_MODULE)

		[ $? -ne 0 ] && mk_error "build sata Failed" && return 1
		mk_info "build sata OK."

		(cd ${LICHEE_SATA_DIR} && [ -x ${build_script} ] && ./${build_script} -s $LICHEE_BTEST_MODULE)
	fi
}

function clsata()
{
	mk_info "clear sata ..."

	local build_script="linux/bsptest/script/bsptest.sh"
	(cd ${LICHEE_SATA_DIR} && [ -x ${build_script} ] && ./${build_script} -b clean)

	mk_info "clean sata OK."
}

function mk_tinyandroid()
{
	local ROOTFS=${LICHEE_PLAT_OUT}/rootfs_tinyandroid

	mk_info "Build tinyandroid rootfs ..."
	if [ "$1" = "f" ]; then
		rm -fr ${ROOTFS}
	fi

	if [ ! -f ${ROOTFS} ]; then
		mkdir -p ${ROOTFS}
		tar -jxf ${LICHEE_DEVICE_DIR}/config/rootfs_tar/tinyandroid_${LICHEE_ARCH}.tar.bz2 -C ${ROOTFS}
	fi

	mkdir -p ${ROOTFS}/lib/modules
	cp -rf ${LICHEE_KERN_DIR}/output/lib/modules/* \
		${ROOTFS}/lib/modules/

	if [ "x$PACK_BSPTEST" != "x" ];then
		if [ -d ${ROOTFS}/target ];then
 			rm -rf ${ROOTFS}/target/*
		fi
		if [ -d ${LICHEE_SATA_DIR}/linux/target ]; then
			mk_info "copy SATA rootfs_def"
			cp -a ${LICHEE_SATA_DIR}/linux/target  ${ROOTFS}/
		fi
	fi

	NR_SIZE=`du -sm ${ROOTFS} | awk '{print $1}'`
	NEW_NR_SIZE=$(((($NR_SIZE+32)/16)*16))

	echo "blocks: $NR_SIZE"M" -> $NEW_NR_SIZE"M""
	${LICHEE_BUILD_DIR}/bin/make_ext4fs -l \
		$NEW_NR_SIZE"M" ${LICHEE_PLAT_OUT}/rootfs.ext4 ${ROOTFS}
	fsck.ext4 -y ${LICHEE_PLAT_OUT}/rootfs.ext4 > /dev/null
}

function mk_defroot()
{
	local ROOTFS=$LICHEE_TOP_DIR/rootfs_def
	local INODES=""
	local BLOCKS=""
	local rootfs_archive=""
	local rootfs_archivedir=""
	local boardconfig="${LICHEE_BOARDCONFIG_PATH}"

	#rootfs_archive=`cat ${boardconfig} | grep -w "LICHEE_ROOTFS" | awk -F= '{printf $2}'`
	rootfs_archive=${LICHEE_ROOTFS}
	rootfs_archivedir=${LICHEE_DEVICE_DIR}/config/rootfs_tar/${rootfs_archive}
	mk_info "-----> Build ubuntu rootfs ..."
	
	echo "------> rootfs =  ${ROOTFS} "

	export PATH=$PATH:${LICHEE_BUILD_DIR}/bin
	fakeroot chown	 -h -R 0:0	${ROOTFS}
	fakeroot mke2img -d ${ROOTFS} -G 4 -R 1 -B 0 -I 256000 -o ${LICHEE_PLAT_OUT}/rootfs.ext4
}

function pack_rootfs()
{
	mk_info "pack rootfs ..."
	local ROOTFS=${LICHEE_BR_OUT}/target
	local INODES=""
	local BLOCKS=""

	mkdir -p ${ROOTFS}/lib/modules
	cp -rf ${LICHEE_KERN_DIR}/output/lib/modules/* \
		${ROOTFS}/lib/modules/

	(cd ${ROOTFS}; ln -fs bin/busybox init)

	export PATH=$PATH:${LICHEE_BUILD_DIR}/bin

	fakeroot chown -h -R 0:0 ${ROOTFS}
	fakeroot ${LICHEE_BUILD_DIR}/bin/mke2img -d \
	${ROOTFS} -G 4 -R 1 -B 0 -I 0 -o ${LICHEE_PLAT_OUT}/rootfs.ext4
	fakeroot ${LICHEE_BUILD_DIR}/bin/mksquashfs \
	${ROOTFS} ${LICHEE_PLAT_OUT}/rootfs.squashfs -root-owned -no-progress -comp xz -noappend

	fakeroot ${LICHEE_BUILD_DIR}/bin/mkfs.ubifs \
		-m 4096 -e 258048 -c 145 -F -x zlib -r ${ROOTFS} -o ${LICHEE_PLAT_OUT}/rootfs.ubifs

	mk_info "pack rootfs ok ..."
}

function mkrootfs()
{
	mk_info "build rootfs ..."
	if [ ${LICHEE_PLATFORM} = "linux" ] ; then
		case ${LICHEE_LINUX_DEV} in
			bsp)
				[ ${SKIP_BR} -ne 0 ] && mk_defroot $1
				;;
			sata)
				local ROOTFS=${LICHEE_PLAT_OUT}/rootfs_def
				if [ "x$PACK_TINY_ANDROID" = "xtrue" ]; then
					mk_tinyandroid $1
				else
					[ ${SKIP_BR} -ne 0 ] && mk_defroot $1
				fi
				;;
			longan)
				pack_rootfs $1
				;;
			dragonboard)
				prepare_dragonboard_toolchain
				if [ -d ${LICHEE_DRAGONBAORD_DIR} ]; then
					echo "Regenerating dragonboard Rootfs..."
					(
					cd ${LICHEE_DRAGONBAORD_DIR}; \
						if [ ! -d "./rootfs" ]; then \
							echo "extract dragonboard rootfs.tar.gz"; \
							tar zxf ./common/rootfs/rootfs.tar.gz; \
						fi
					)
					mkdir -p ${LICHEE_DRAGONBAORD_DIR}/rootfs/lib/modules
					rm -rf ${LICHEE_DRAGONBAORD_DIR}/rootfs/lib/modules/*
					cp -rf ${LICHEE_KERN_DIR}/output/lib/modules/* \
						${LICHEE_DRAGONBAORD_DIR}/rootfs/lib/modules/
					(cd ${LICHEE_DRAGONBAORD_DIR}/common/scripts; ./build.sh)
					[  $? -ne 0 ] && mk_error "build rootfs Failed" && return 1
					cp ${LICHEE_DRAGONBAORD_DIR}/rootfs.ext4 ${LICHEE_PLAT_OUT}
				else
					mk_error "no ${LICHEE_PLATFORM} in lichee,please git clone it under lichee"
					exit 1
				fi
				;;
			*)
				mk_info "skip make rootfs for LICHEE_LINUX_DEV=${LICHEE_LINUX_DEV}"
				;;
		esac
	else
		mk_info "skip make rootfs for ${LICHEE_PLATFORM}"
	fi

}

function cldtbo()
{
	if [ -d ${LICHEE_CHIP_CONFIG_DIR}/dtbo ];  then
		mk_info "clean dtbo ..."
		rm -rf ${LICHEE_CHIP_CONFIG_DIR}/dtbo/*.dtbo
	fi
}

function mkdtbo()
{
	local DTBO_DIR=${LICHEE_BOARD_CONFIG_DIR}/dtbo

	[ ! -d "${DTBO_DIR}" ] && DTBO_DIR=${LICHEE_CHIP_CONFIG_DIR}/dtbo

	if [ -d $DTBO_DIR ];  then
		mk_info "build dtbo ..."
		local DTC_FLAGS="-W no-unit_address_vs_reg"
		local DTS_DIR=${DTBO_DIR}
		local DTBO_OUT_DIR=${LICHEE_PLAT_OUT}
		local DTO_COMPILER=${DTBO_DIR}/dtco

		if [ ! -f $DTO_COMPILER ]; then
			mk_info "mkdtbo: Can not find dtco compiler."
			exit 1
		fi

		local out_file_name=0
		for dts_file in ${DTS_DIR}/*.dts; do
			out_file_name=${dts_file%.*}
			$DTO_COMPILER ${DTC_FLAGS} -a 4 -@ -O dtb -o ${out_file_name}.dtbo ${dts_file}
			if [ $? -ne 0 ]; then
				mk_info "mkdtbo:create dtbo file failed"
				exit 1
			fi
		done

		local MKDTIMG=${LICHEE_BUILD_DIR}/bin/mkdtimg
		local DTBOIMG_CFG_FILE=${DTBO_DIR}/dtboimg.cfg
		local DTBOIMG_OUT_DIR=${LICHEE_PLAT_OUT}
		if [ -f ${MKDTIMG} ]; then
			if [ -f ${DTBOIMG_CFG_FILE} ]; then
				mk_info "mkdtbo: make  dtboimg start."
				cd ${DTBO_DIR}/
				${MKDTIMG} cfg_create ${DTBOIMG_OUT_DIR}/dtbo.img ${DTBOIMG_CFG_FILE}
				${MKDTIMG} dump ${DTBOIMG_OUT_DIR}/dtbo.img
				cd ${LICHEE_BUILD_DIR}
			else
				mk_info "mkdtbo: Can not find dtboimg.cfg\n"
				exit 1
			fi
		else
			mk_info "mkdtbo: Can not find mkdtimg\n"
			exit 1
		fi

	else
		mk_info "don't build dtbo ..."
	fi
}

function mklichee()
{

	mk_info "----------------------------------------"
	mk_info "build lichee ..."
	mk_info "chip: $LICHEE_CHIP"
	mk_info "platform: $LICHEE_PLATFORM"
	mk_info "kernel: $LICHEE_KERN_VER"
	mk_info "board: $LICHEE_BOARD"
	mk_info "output: $LICHEE_PLAT_OUT"
	mk_info "----------------------------------------"

	check_env

	if [ ${SKIP_BR} -eq 0 ]; then
		mkbr
		if [ $? -ne 0 ]; then
			mk_info "mkbr failed"
			exit 1
		fi
	fi

	mkdtbo
	if [ $? -ne 0 ]; then
		mk_info "mkdtbo failed"
		exit 1
	fi

	mkbootloader
	if [ $? -ne 0 ]; then
		mk_info "mkbootloader failed"
		exit 1
	fi

	mkkernel
	if [ $? -ne 0 ]; then
		mk_info "mkkernel failed"
		exit 1
	fi

	if [ -d ${LICHEE_SATA_DIR} ]; then
		mksata
		if [ $? -ne 0 ]; then
			mk_info "mksata failed"
			exit 1
		fi
	fi

	mkrootfs $1

	[ $? -ne 0 ] && return 1

	mk_info "----------------------------------------"
	mk_info "build lichee OK."
	mk_info "----------------------------------------"
}

function mkclean()
{
	clkernel
	cldragonboard
	cldtbo
	clbuildserver
	if [ ${SKIP_BR} -eq 0 ]; then
		clbr
	fi

	mk_info "clean product output in ${LICHEE_PLAT_OUT} ..."
	if [ "x${LICHEE_PLAT_OUT}" != "x" -a -d ${LICHEE_PLAT_OUT} ];then
		cd ${LICHEE_PLAT_OUT}
		ls | grep -v "buildroot" | xargs rm -rf
		cd - > /dev/null
	fi
}

function mkdistclean()
{
	clkernel "distclean"
	if [ ${SKIP_BR} -eq 0 ]; then
		clbr
	fi
	cldtbo
	cldragonboard
	mk_info "clean entires output dir ..."
	if [ "x${LICHEE_PLAT_OUT}" != "x" ]; then
		rm -rf ${LICHEE_PLAT_OUT}/*
	fi
}

function mkpack()
{
	mk_info "packing firmware ..."

	check_env

	local PACK_PLATFORM=$LICHEE_PLATFORM
	[ "x${LICHEE_PLATFORM}" != "xandroid" ] && PACK_PLATFORM=$LICHEE_LINUX_DEV

	(cd ${LICHEE_TOP_DIR}/build && \
	./pack -i ${LICHEE_IC} -c ${LICHEE_CHIP} -p ${PACK_PLATFORM} -b ${LICHEE_BOARD} -k ${LICHEE_KERN_VER} -n ${LICHEE_FLASH} $@)
}

function mkhelp()
{
	printf "
	mkscript - lichee build script

	<version>: 1.0.0
	<author >: james

	<command>:
	mkboot      build boot
	mkbr        build buildroot
	mkkernel    build kernel
	mkrootfs    build rootfs for linux, dragonboard
	mklichee    build total lichee

	mkclean     clean current board output
	mkdistclean clean entires output

	mkpack      pack firmware for lichee

	mkhelp      show this message

	"
}

