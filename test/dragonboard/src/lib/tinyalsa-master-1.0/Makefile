# define sources root directory before everything
SRC_ROOT = ./../../

# include rule.mk
include $(SRC_ROOT)/rule.mk

#CFLAGS = -c -fPIC -Wall
#CFLAGS := -c -fPIC -Wall -O3 -Os -pipe \
#		 -mtune=cortex-a8 -march=armv7-a -mabi=aapcs-linux -mfloat-abi=softfp -mfpu=neon \
#		 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
#		 -D_GNU_SOURCE -D_REENTRANT
CFLAGS := -c -fPIC $(CFLAGS)
INC = include
OBJECTS = mixer.o pcm.o
LIB = libtinyalsa.so
#CROSS_COMPILE = arm-linux-gnueabihf-gcc
#tinypcminfo
all: $(LIB) tinyplay tinycap tinymix
	 cp tinyplay tinycap tinymix $(BINDIR)/
	 cp libtinyalsa.so $(SRC_ROOT)../sysroot/lib/
	 cp libtinyalsa.so $(SRC_ROOT)../rootfs/lib/

tinyplay: $(LIB) tinyplay.o
	$(CC) tinyplay.o -L. -ltinyalsa -o tinyplay

tinycap: $(LIB) tinycap.o
	$(CC) tinycap.o -L. -ltinyalsa -o tinycap

tinymix: $(LIB) tinymix.o
	$(CC) tinymix.o -L. -ltinyalsa -o tinymix

tinypcminfo: $(LIB) tinypcminfo.o
	$(CC) tinypcminfo.o -L. -ltinyalsa -o tinypcminfo

$(LIB): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $(LIB)

.c.o:
	$(CC) $(CFLAGS) $< -I$(INC)

clean:
	-rm -f $(LIB) $(OBJECTS) tinyplay.o tinyplay tinycap.o tinycap \
	tinymix.o tinymix tinypcminfo.o tinypcminfo
