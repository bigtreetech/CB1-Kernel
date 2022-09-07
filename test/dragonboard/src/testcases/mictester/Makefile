
# define sources root directory before everything
SRC_ROOT := ../..

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
#CFLAGS := $(CFLAGS) -g
LDFLAGS := -lscript -lpthread -lsalsa

# include rule.mk
include $(SRC_ROOT)/rule.mk

CFLAGS := $(CFLAGS) -I$(LIBDIR)/salsa-lib-0.0.22/src

# define objects collection variable
platformfile=$(shell echo tiny_cap_play_$(LICHEE_CHIP).o | sed "s/p[0-9]\+\.o$$/p.c/g")
mictester_objs = mixer.o pcm.o $(patsubst %.c,%.o,$(platformfile))

# add your target(s) to all
.PHONY: all

ifeq ($(platformfile),$(wildcard $(platformfile)))
all: mictester

# define you target, the target will be output to dragonboard/output/bin
# directory
mictester: $(mictester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'example_objs' to your objects collection variable
$(mictester_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)
else
all:
	@echo -e '\033[0;31mCannot build platformfile $(platformfile), skip this build.\033[0m'
endif

