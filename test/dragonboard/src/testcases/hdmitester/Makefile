
# define sources root directory before everything
SRC_ROOT := ../..

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
#CFLAGS := $(CFLAGS) -g
LDFLAGS := -lscript -lsalsa -lpthread

# include rule.mk
include $(SRC_ROOT)/rule.mk

CFLAGS := $(CFLAGS) -I$(LIBDIR)/salsa-lib-0.0.22/src

# define objects collection variable
hdmitester_objs = hdmitester.o

# add your target(s) to all
.PHONY: all
all: hdmitester
	cp hdmitester.sh $(BINDIR)/

# define you target, the target will be output to dragonboard/output/bin
# directory
hdmitester: $(hdmitester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'example_objs' to your objects collection variable
$(hdmitester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

