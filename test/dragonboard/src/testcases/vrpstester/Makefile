
# define sources root directory before everything
SRC_ROOT := ../..

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
#CFLAGS := $(CFLAGS) -g
LDFLAGS := -lscript

# include rule.mk
include $(SRC_ROOT)/rule.mk

# define objects collection variable
vrpstester_objs = vrpstester.o

.PHONY: all
all: vrpstester
	cp vrpstester.sh $(BINDIR)/

# define you target, the target will be output to dragonboard/output/bin
# directory
vrpstester: $(vrpstester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'vrpstester_objs' to your objects collection variable
$(vrpstester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

