TARGET    = adbd
INCLUDES += \
	-I. \
	-I./libs/libcutils \
	-I./libs/libmincrypt

SRCS := \
	adb.c \
	backup_service.c \
	fdevent.c \
	transport.c \
	transport_local.c \
	transport_usb.c \
	adb_auth_client.c \
	sockets.c \
	services.c \
	file_sync_service.c \
	jdwp_service.c \
	framebuffer_service.c \
	remount_service.c \
	usb_linux_client.c \
	log_service.c \
	utils.c

SUB_LIB := \
	libmincrypt.a \
	libcutils.a \
	-lpthread

OBJS    = $(SRCS:.c=.o)
CC := arm-linux-gnueabi-gcc
LOCAL_CFLAGS := -O2 -g -DADB_HOST=0 -Wall -DALLOW_ADBD_ROOT=1
LOCAL_CFLAGS += -D_XOPEN_SOURCE -D_GNU_SOURCE
LOCAL_CFLAGS += -DHAVE_FORKEXEC=1
LOCAL_CFLAGS += -Wno-unused-parameter
LOCAL_CFLAGS += -Wno-implicit-function-declaration
LOCAL_CFLAGS += -Wno-deprecated-declarations
LOCAL_CFLAGS += -Wno-unused-but-set-variable
LOCAL_CFLAGS += -Wno-unused-variable
LOCAL_CFLAGS += -Wno-strict-aliasing

%.o: %.c
	@echo "  CC\t$<"
	@$(CC) $(CFLAGS) $(LOCAL_CFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS) libcutils libmincrypt
	@echo "  LN\t$^ -> $@"
	@$(CC) $(LDFLAGS) $(OBJS) $(SUB_LIB) -o $@

libmincrypt:
	make -C libs/libmincrypt
	cp -f libs/libmincrypt/libmincrypt.a .

libcutils:
	make -C libs/libcutils
	cp -f libs/libcutils/libcutils.a .

all:$(TARGET)

install:
	install adbd ${LICHEE_DRAGONBAORD_DIR}/rootfs/bin
	install scripts/adb_shell ${LICHEE_DRAGONBAORD_DIR}/rootfs/bin
	install scripts/adbd.init ${LICHEE_DRAGONBAORD_DIR}/rootfs/etc/init.d
	install scripts/adb_profile ${LICHEE_DRAGONBAORD_DIR}/rootfs/etc
	install scripts/start_adb.sh ${LICHEE_DRAGONBAORD_DIR}/rootfs/bin

clean: libmincrypt_clean libcutils_clean
	rm -rf $(TARGET) *.o *.a *~

libmincrypt_clean:
	make -C libs/libmincrypt clean

libcutils_clean:
	make -C libs/libcutils clean

