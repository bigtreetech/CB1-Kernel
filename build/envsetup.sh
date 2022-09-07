#!/bin/bash

function build_usage()
{
    printf "Usage: build [args]
    build               - default build all
    build bootloader    - only build bootloader
    build buildroot     - only build buildroot
    build kernel        - only build kernel
    build clean         - clean all
    build distclean     - distclean all
"
    return 0
}

function pack_usage()
{
    printf "Usage: pack [args]
    pack                - pack firmware
    pack -d             - pack firmware with debug info output to card0
    pack -s             - pack firmware with secureboot
    pack -sd            - pack firmware with secureboot and debug info output to card0
"
    return 0
}

function build_help()
{
    printf "Invoke . build/envsetup.sh from your shell to add the following functions to your environment:
    croot               - Changes directory to the top of the tree.
    cbrandy             - Changes directory to the brandy
    cbr                 - Changes directory to the buildroot
    cconfigs            - Changes directory to the board's config
    cdevice             - Changes directory to the product's config
    cdts                - Changes directory to the dts.
    ckernel             - Changes directory to the kernel
    cout                - Changes directory to the board's output
"
    build_usage
    pack_usage

    return 0
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

function croot()
{
	cd $LICHEE_TOP_DIR
}

function ckernel()
{
	local dkey="LICHEE_KERN_DIR"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function saveconfig()
{
	local dkey1="LICHEE_KERN_DIR"
	local dkey2="LICHEE_ARCH"
	local dkey3="LICHEE_KERN_DEFCONF_ABSOLUTE"

	local dval1=$(load_config $dkey1 $LICHEE_TOP_DIR/.buildconfig)
	local dval2=$(load_config $dkey2 $LICHEE_TOP_DIR/.buildconfig)
	local dval3=$(load_config $dkey3 $LICHEE_TOP_DIR/.buildconfig)

	[ -z "$dval1" ] && echo "ERROR: $dkey1 not set in .buildconfig" && return 1
	[ -z "$dval2" ] && echo "ERROR: $dkey2 not set in .buildconfig" && return 1
	[ -z "$dval3" ] && echo "ERROR: $dkey3 not set in .buildconfig" && return 1

	(cd $dval1 && make ARCH=$dval2 savedefconfig && mv defconfig $dval3)
}

function cdts()
{
	local dkey1="LICHEE_KERN_DIR"
	local dkey2="LICHEE_ARCH"

	local dval1=$(load_config $dkey1 $LICHEE_TOP_DIR/.buildconfig)
	local dval2=$(load_config $dkey2 $LICHEE_TOP_DIR/.buildconfig)

	[ -z "$dval1" ] && echo "ERROR: $dkey1 not set in .buildconfig" && return 1
	[ -z "$dval2" ] && echo "ERROR: $dkey2 not set in .buildconfig" && return 1

	local dval=$dval1/arch/$dval2/boot/dts
	[ "$dval2" == "arm64" ] && dval=$dval/sunxi
	cd $dval
}

function cdevice()
{
	local dkey="LICHEE_PRODUCT_CONFIG_DIR"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function cconfigs()
{
	local dkey="LICHEE_BOARD_CONFIG_DIR"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function cout()
{
	local dkey="LICHEE_PLAT_OUT"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function cbrandy()
{
	local dkey="LICHEE_BRANDY_DIR"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function cbr()
{
	local dkey="LICHEE_BR_DIR"
	local dval=$(load_config $dkey $LICHEE_TOP_DIR/.buildconfig)
	[ -z "$dval" ] && echo "ERROR: $dkey not set in .buildconfig" && return 1
	cd $dval
}

function printconfig()
{
	cat $LICHEE_TOP_DIR/.buildconfig
}

function pack()
{
	local mode=$@
	if [ $# -gt 1 ]; then
		echo "too much args"
		return 1
	fi

	if [ "x$mode" == "x" ]; then
		./build.sh pack
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "x-d" ]; then
		./build.sh pack_debug
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "x-s" ]; then
		./build.sh pack_secure
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "x-sd" ]; then
		./build.sh pack_debug_secure
		[ $? -ne 0 ] && return 1
	else
		echo "unkwon commad."
		pack_usage
	fi
}

function build()
{
	local mode=$@
	if [ $# -gt 1 ]; then
		echo "too much args"
		return 1
	fi

	if [ "x$mode" == "x" ]; then
		./build.sh
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xbrandy" ]; then
		./build.sh brandy
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xkernel" ]; then
		./build.sh kernel
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xbuildroot" ]; then
		./build.sh buildroot
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xdistclean" ]; then
		./build.sh distclean
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xclean" ]; then
		./build.sh clean
		[ $? -ne 0 ] && return 1
	elif [ "x$mode" == "xhelp" ]; then
		build_help
		[ $? -ne 0 ] && return 1
	else
		echo "unkwon commad."
		build_usage
	fi
}

export LICHEE_TOP_DIR=$(cd $(dirname ${BASH_SOURCE[0]})/.. && pwd)

if [ ! -f $LICHEE_TOP_DIR/.buildconfig ]; then
	./build.sh config
fi

. $LICHEE_TOP_DIR/.buildconfig
build_help
