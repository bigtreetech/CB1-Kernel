PLATFORM=PLATFORM1
#PLATFORM=PLATFORM2

##make -C KERNEL_OBJ M=driver_path ARCH=arm64 CROSS_COMPILE=PATH modules

echo "make clean"
make -f Makefile.x86 clean

if [ $? -eq 0 ]; then
	echo "compile clean success"
else 
	echo "compile clean fail"
fi

if [ $PLATFORM == 'PLATFORM1' ] 
then
	echo "=======Compile driver at PLATFORM1=========="
	LINUX_SRC=
	CROSS_COMPILE=
	make -C ${LINUX_SRC} M=$PWD CROSS_COMPILE=${CROSS_COMPILE} ARCH=arm modules
elif [ $PLATFORM == 'PLATFORM2' ]
then
	echo "=======Compile driver at PLATFORM2=========="
	LINUX_SRC=
	CROSS_COMPILE=
	make -C ${LINUX_SRC} M=$PWD CROSS_COMPILE=${CROSS_COMPILE} ARCH=arm64 modules
fi
