
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
keytester_objs = keytester.o

# add your target(s) to all
.PHONY: all
all: keytester

# define you target, the target will be output to dragonboard/output/bin
# directory
keytester: $(keytester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'example_objs' to your objects collection variable
$(keytester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

