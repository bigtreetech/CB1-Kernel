TARGET    = libcutils.a
INCLUDES += \
	-I. \
	-I./include \
	-I$(KERNEL_DIR)/include

commonSources := \
	array.c \
	hashmap.c \
	native_handle.c \
	buffer.c \
	socket_inaddr_any_server.c \
	socket_local_client.c \
	socket_local_server.c \
	socket_loopback_client.c \
	socket_loopback_server.c \
	socket_network_client.c \
	sockets.c \
	config_utils.c \
	cpu_info.c \
	load_file.c \
	list.c \
	open_memstream.c \
	strdup16to8.c \
	strdup8to16.c \
	record_stream.c \
	process_name.c \
	qsort_r_compat.c \
	threads.c \
	sched_policy.c \
	iosched_policy.c \
	str_parms.c \
	properties.c \

#delete source file
#commonSources += \
	atomic.c.arm \

commonSources += \
	abort_socket.c \
	fs.c \
	selector.c \
	tztime.c \
	multiuser.c \
	zygote.c

SRCS := $(commonSources) \
	android_reboot.c \
	ashmem-dev.c \
	debugger.c \
	klog.c \
	mq.c \
	partition_utils.c \
	qtaguid.c \
	uevent.c \
	misc_rw.c \
	memory.c

myCFLAGS += -DHAVE_PTHREADS -DANDROID_SMP=1 -DHAVE_LIBC_SYSTEM_PROPERTIES=1 -DHAVE_SYS_SOCKET_H=1 -D_GNU_SOURCE
myCFLAGS += -Wno-implicit-function-declaration
myCFLAGS += -Wno-discarded-qualifiers
myCFLAGS += -Wno-int-conversion

OBJS    = $(SRCS:.c=.o)
CC := arm-linux-gnueabi-gcc
%.o: %.c
	@echo "  CC\t$<"
	@$(CC) $(CFLAGS) $(myCFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo "  AR\t$<"
	@ar r $@ $(OBJS)

all:$(TARGET)

clean:
	rm -rf $(TARGET) *.o *.a *~

