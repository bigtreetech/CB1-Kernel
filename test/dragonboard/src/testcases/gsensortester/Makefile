
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
gsensortester_objs = gsensortester.o

.PHONY: all
all: gsensortester
	cp gsensortester.sh $(BINDIR)/

# define you target, the target will be output to dragonboard/output/bin
# directory
gsensortester: $(gsensortester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'gsensortester_objs' to your objects collection variable
$(gsensortester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

