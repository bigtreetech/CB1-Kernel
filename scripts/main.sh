#!/bin/bash
#
# Main program
#
cleanup_list() {
	local varname="${1}"
	local list_to_clean="${!varname}"
	list_to_clean="${list_to_clean#"${list_to_clean%%[![:space:]]*}"}"
	list_to_clean="${list_to_clean%"${list_to_clean##*[![:space:]]}"}"
	echo ${list_to_clean}
}

# default umask for root is 022 so parent directories won't be group writeable without this
# this is used instead of making the chmod in prepare_host() recursive
umask 002

DEST="${SRC}"/output

REVISION="2.3.4"
NTP_SERVER="cn.pool.ntp.org"
titlestr="Choose an option"

[[ -n $COLUMNS ]] && stty cols $COLUMNS
[[ -n $LINES ]] && stty rows $LINES
TTY_X=$(($(stty size | awk '{print $2}')-6)) 			# determine terminal width
TTY_Y=$(($(stty size | awk '{print $1}')-6)) 			# determine terminal height

# Warnings mitigation
[[ -z $LANGUAGE ]] && export LANGUAGE="en_US:en"        # set to english if not set
[[ -z $CONSOLE_CHAR ]] && export CONSOLE_CHAR="UTF-8"   # set console to UTF-8 if not set

# Libraries include
source "${SRC}"/scripts/debootstrap.sh	    # system specific install
source "${SRC}"/scripts/image-helpers.sh	# helpers for OS image building
source "${SRC}"/scripts/distributions.sh	# system specific install
source "${SRC}"/scripts/compilation.sh	    # patching and compilation of kernel, uboot, ATF
source "${SRC}"/scripts/makeboarddeb.sh		# board support package
source "${SRC}"/scripts/general.sh		    # general functions
source "${SRC}"/scripts/chroot-buildpackages.sh	    # chroot packages building
source "${SRC}"/scripts/pack-uboot.sh

# set log path
LOG_SUBPATH=${LOG_SUBPATH:=debug}

# compress and remove old logs
mkdir -p "${DEST}"/${LOG_SUBPATH}
(cd "${DEST}"/${LOG_SUBPATH} && tar -czf logs-"$(<timestamp)".tgz ./*.log) > /dev/null 2>&1
rm -f "${DEST}"/${LOG_SUBPATH}/*.log > /dev/null 2>&1
date +"%d_%m_%Y-%H_%M_%S" > "${DEST}"/${LOG_SUBPATH}/timestamp

# delete compressed logs older than 7 days
(cd "${DEST}"/${LOG_SUBPATH} && find . -name '*.tgz' -mtime +7 -delete) > /dev/null

if [[ $PROGRESS_LOG_TO_FILE != yes ]]; then unset PROGRESS_LOG_TO_FILE; fi

SHOW_WARNING=yes

CCACHE=ccache
export PATH="/usr/lib/ccache:$PATH"

if [[ -z $BUILD_OPT ]]; then
	options+=("u-boot"	 "U-boot package")
	options+=("kernel"	 "Kernel package")
	options+=("rootfs"	 "Rootfs and all deb packages")
	options+=("image"	 "Full OS image for flashing")

	menustr="Compile image | rootfs | kernel | u-boot"
	BUILD_OPT=$(whiptail --title "${titlestr}" --backtitle "${backtitle}" --notags \
			  --menu "${menustr}" "${TTY_Y}" "${TTY_X}" $((TTY_Y - 8))  \
			  --cancel-button Exit --ok-button Select "${options[@]}" \
			  3>&1 1>&2 2>&3)

	unset options
	[[ -z $BUILD_OPT ]] && exit_with_error "No option selected"
	[[ $BUILD_OPT == rootfs ]] && ROOT_FS_CREATE_ONLY="yes"
fi

KERNEL_CONFIGURE="no"
[[ $BUILD_OPT == kernel ]] && KERNEL_CONFIGURE="yes"

[[ $MANUAL_KERNEL_CONFIGURE == yes ]] && KERNEL_CONFIGURE="yes"
[[ $MANUAL_KERNEL_CONFIGURE == no ]] && KERNEL_CONFIGURE="no"

BOOTCONFIG="h616_defconfig"
LINUXFAMILY="sun50iw9"

###################################################
BOARD="h616"
BOARD_NAME="BTT-CB1"

USER_NAME="biqu"
USER_PWD="biqu"
ROOT_PWD="root"             # Must be changed @first login

BRANCH="current"
RELEASE="bullseye"          # 发行版本 bookworm/bullseye/focal/jammy 可选   
SELECTED_CONFIGURATION="cli_standard"
###################################################

source "${SRC}"/scripts/configuration.sh

# optimize build time with 100% CPU usage
CPUS=$(grep -c 'processor' /proc/cpuinfo)
CTHREADS="-j$((CPUS + CPUS/2))"

branch2dir() {
	[[ "${1}" == "head" ]] && echo "HEAD" || echo "${1##*:}"
}

BOOTSOURCEDIR="${BOOTDIR}"
LINUXSOURCEDIR="${KERNELDIR}"

[[ -n $ATFSOURCE ]] && ATFSOURCEDIR="${ATFDIR}/$(branch2dir "${ATFBRANCH}")"

BSP_CLI_PACKAGE_NAME="bsp-cli-${BOARD}"
BSP_CLI_PACKAGE_FULLNAME="${BSP_CLI_PACKAGE_NAME}_${REVISION}_${ARCH}"

CHOSEN_UBOOT=linux-u-boot-${BRANCH}-${BOARD}
CHOSEN_KERNEL=linux-image-${BRANCH}-${LINUXFAMILY}
CHOSEN_ROOTFS=${BSP_CLI_PACKAGE_NAME}
CHOSEN_KSRC=linux-source-${BRANCH}-${LINUXFAMILY}

start=$(date +%s)

prepare_host

[[ $CLEAN_LEVEL == *sources* ]] && cleaning "sources"

for option in $(tr ',' ' ' <<< "$CLEAN_LEVEL"); do
	[[ $option != sources ]] && cleaning "$option"
done

# Compile u-boot if packed .deb does not exist or use the one from Armbian
if [[ $BUILD_OPT == u-boot || $BUILD_OPT == image ]]; then
	if [[ ! -f "${DEB_STORAGE}"/u-boot/${CHOSEN_UBOOT}_${REVISION}_${ARCH}.deb ]]; then
		[[ -n "${ATFSOURCE}" && "${REPOSITORY_INSTALL}" != *u-boot* ]] && compile_atf
		[[ ${REPOSITORY_INSTALL} != *u-boot* ]] && compile_uboot
	fi

	if [[ $BUILD_OPT == "u-boot" ]]; then
		display_alert "U-boot build done" "@host" "info"
		display_alert "Target directory" "${DEB_STORAGE}/u-boot" "info"
		display_alert "File name" "${CHOSEN_UBOOT}_${REVISION}_${ARCH}.deb" "info"
	fi
fi

# Compile kernel if packed .deb does not exist or use the one from Armbian
if [[ $BUILD_OPT == kernel || $BUILD_OPT == image ]]; then
	if [[ ! -f ${DEB_STORAGE}/${CHOSEN_KERNEL}_${REVISION}_${ARCH}.deb ]]; then 
		KDEB_CHANGELOG_DIST=$RELEASE
		[[ "${REPOSITORY_INSTALL}" != *kernel* ]] && compile_kernel
	fi

	if [[ $BUILD_OPT == "kernel" ]]; then
		display_alert "Kernel build done" "@host" "info"
		display_alert "Target directory" "${DEB_STORAGE}/" "info"
		display_alert "File name" "${CHOSEN_KERNEL}_${REVISION}_${ARCH}.deb" "info"
	fi
fi

if [[ $BUILD_OPT == rootfs || $BUILD_OPT == image ]]; then
	overlayfs_wrapper "cleanup"
	# create board support package
	[[ -n $RELEASE && ! -f ${DEB_STORAGE}/$RELEASE/${BSP_CLI_PACKAGE_FULLNAME}.deb ]] && create_board_package
	[[ $BSP_BUILD != yes ]] && debootstrap_ng
fi

# hook for function to run after build, i.e. to change owner of $SRC
# NOTE: this will run only if there were no errors during build process
[[ $(type -t run_after_build) == function ]] && run_after_build || true

end=$(date +%s)
runtime=$(((end-start)/60))
display_alert "Runtime" "$runtime min" "info"

display_alert "Repeat Build Options" "sudo ./build.sh ${BUILD_CONFIG} BOARD=${BOARD} BRANCH=${BRANCH} \
$([[ -n $BUILD_OPT ]] && echo "BUILD_OPT=${BUILD_OPT} ")\
$([[ -n $RELEASE ]] && echo "RELEASE=${RELEASE} ")\
$([[ -n $KERNEL_CONFIGURE ]] && echo "KERNEL_CONFIGURE=${KERNEL_CONFIGURE} ")\
" "ext"
