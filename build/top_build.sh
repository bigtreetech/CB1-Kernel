#!/bin/bash
# Description:
# Allwinner compile tools usage
# We follow below step:
#
# To sun8i & linux-3.x developer:
#------------------------------------------------------------------------------------------------
# Using origin buildroot compile once, then not compile with reusable.
#	Arch			Kernel				Out Dir
#	sun8i			Linux-3.x			out/${chip}
#------------------------------------------------------------------------------------------------
#
# To mostly developer(sun50i|linux-4.x): using tools/build/mkcommon.sh
#------------------------------------------------------------------------------------------------
#	Kernel			Linux-3.x			Linux-4.x
#	Toolchain		origin				linaro-5.3
#	Toolchain Dir		external-toolchain		gcc-linaro-5.3.1-2016.05
#	Def Rootfs		target_${ARCH}.tar.xz		target-${ARCH}-linaro-5.3.tar.xz
#	Out Dir			out/${chip}			out/${chip}-linaro-5.3
#
# To self design rootfs developer: using ${buildroot}/build/mkcommon.sh
#------------------------------------------------------------------------------------------------
#	Kernel			Linux-3.x			Linux-4.x
#	buildroot		origin				buildroot-201611
#	Out Dir			out/${chip}			out/${chip}-linaro-5.3
#
# As self design rootfs developer, if not manager buildroot, please reference to buildroot manual
#

build/mkcommon.sh $@
