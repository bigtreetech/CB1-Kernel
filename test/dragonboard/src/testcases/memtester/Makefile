
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

# add your target(s) to all
.PHONY: all
all: 

# define you target, the target will be output to dragonboard/output/bin
# directory
	cp memtester.sh $(BINDIR)/

