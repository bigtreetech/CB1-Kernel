
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
brcm_patchram_plus_objs = brcm_patchram_plus.o

# add your target(s) to all
.PHONY: all
all: brcm_patchram_plus

# define you target, the target will be output to dragonboard/output/bin
# directory
brcm_patchram_plus: $(brcm_patchram_plus_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'brcm_patchram_plus_objs' to your objects collection variable
$(brcm_patchram_plus_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

