#!/bin/bash

OUT_PATH=""
export PATH=$PATH:${LICHEE_BUILD_DIR}/bin
# build adb
cd ${LICHEE_DRAGONBAORD_DIR}/common/adb;
make
make install
if [ $? -ne 0 ]; then
    exit 1
fi

# sysroot exist?
cd ${LICHEE_DRAGONBAORD_DIR}
if [ ! -d "./sysroot" ]; then
    echo "extract sysroot.tar.gz"
    tar zxf ./common/rootfs/sysroot.tar.gz
fi

if [ ! -d "./output/bin" ]; then
    mkdir -p ./output/bin
fi

if [ ! -d "./output/firmware" ]; then
    mkdir -p ./output/firmware
fi

cd src
make
if [ $? -ne 0 ]; then
    exit 1
fi
cd ..

if [ ! -d "rootfs/${LICHEE_LINUX_DEV}" ]; then
    mkdir -p rootfs/${LICHEE_LINUX_DEV}
fi
cp -rf ${LICHEE_DRAGONBAORD_DIR}/common/scripts/autorun.sh rootfs/
rm -rf rootfs/${LICHEE_LINUX_DEV}/*
cp -rf output/* rootfs/${LICHEE_LINUX_DEV}/

echo "generating rootfs..."

NR_SIZE=`du -sm rootfs | awk '{print $1}'`
NEW_NR_SIZE=$(((($NR_SIZE+32)/16)*16))
#NEW_NR_SIZE=360
TARGET_IMAGE=rootfs.ext4

echo "blocks: $NR_SIZE"M" -> $NEW_NR_SIZE"M""
make_ext4fs -l $NEW_NR_SIZE"M" $TARGET_IMAGE rootfs/
fsck.ext4 -y $TARGET_IMAGE > /dev/null
echo "success in generating rootfs"

if [ -n "$OUT_PATH" ]; then
    cp -v rootfs.ext4 $OUT_PATH/
fi

echo "Build at: `date`"
