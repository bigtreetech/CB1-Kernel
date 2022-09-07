
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
compasstester_objs = compasstester.o

.PHONY: all
all: compasstester
	cp compasstester.sh $(BINDIR)/
	cp compasstester $(BINDIR)/

# define you target, the target will be output to dragonboard/output/bin
# directory
compasstester: $(compasstester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'compasstester_objs' to your objects collection variable
$(compasstester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

