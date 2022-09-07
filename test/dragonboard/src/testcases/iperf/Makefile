
# define sources root directory before everything
SRC_ROOT := ../..

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
#CFLAGS := $(CFLAGS) -g
CFLAGS = -I. -Iinclude -Isrc -Icompat -Wall
CFLAGS += -DHAVE_CONFIG_H -DHAVE_GETTIMEOFDAY
CFLAGS += -DIPERF_VERSION="\"2.0.13a\""
CFLAGS += -DIPERF_VERSION_DATE="\"17 Oct 2018\""
CFLAGS += -DIPERF_VERSION_MAJORHEX=0x00020000
CFLAGS += -DIPERF_VERSION_MINORHEX=0x000D0000
LDFLAGS := -lscript -lrt -lpthread -lm

# include rule.mk
include $(SRC_ROOT)/rule.mk

# define objects collection variable
compat_c_objs = \
	compat/Thread.o \
	compat/error.o \
	compat/delay.o \
	compat/gettimeofday.o \
	compat/inet_ntop.o \
	compat/inet_pton.o \
	compat/signal.o \
	compat/snprintf.o \
	compat/string.o

iperf_c_objs = \
	src/Extractor.o \
	src/Locale.o \
	src/ReportCSV.o \
	src/ReportDefault.o \
	src/Reporter.o \
	src/SocketAddr.o \
	src/gnu_getopt.o \
	src/gnu_getopt_long.o \
	src/histogram.o \
	src/service.o \
	src/sockets.o \
	src/stdio.o \
	src/tcp_window_size.o \
	src/checksums.o \
	src/pdfs.o

iperf_cpp_objs = \
	src/main.o \
	src/Settings.o \
	src/Server.o \
	src/Client.o \
	src/isochronous.o \
	src/Launch.o \
	src/List.o \
	src/Listener.o \
	src/PerfSocket.o

# add your target(s) to all
.PHONY: all
all: iperf

# define you target, the target will be output to dragonboard/output/bin
# directory
iperf: $(compat_c_objs) $(iperf_c_objs) $(iperf_cpp_objs)
	$(LINK_MSG)
	$(LINKCPPX)

# change objs to your objects collection variable
$(compat_c_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

$(iperf_c_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

$(iperf_cpp_objs): %.o: %.cpp
	$(COMPILE_MSG)
	$(COMPILECPPX)

# clean temp files
clean:
	-rm -rf $(compat_c_objs) $(compat_c_objs:.o=.d) $(compat_c_objs:.o=.d.*)
	-rm -rf $(iperf_c_objs) $(iperf_c_objs:.o=.d) $(iperf_c_objs:.o=.d.*)
	-rm -rf $(iperf_cpp_objs) $(iperf_cpp_objs:.o=.d) $(iperf_cpp_objs:.o=.d.*)

