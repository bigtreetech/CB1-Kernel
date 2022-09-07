
# define sources root directory before everything
SRC_ROOT = .

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
#CFLAGS := $(CFLAGS) -g
LDFLAGS := -lscript -ldirectfb -lpthread

# include rule.mk
include $(SRC_ROOT)/rule.mk

CFLAGS := $(CFLAGS) -I$(SYSROOT)/usr/include/directfb

CFLAGS_PLATFORM=$(shell echo _$(LICHEE_CHIP) | tr 'a-z' 'A-Z' | sed "s/P[0-9]\+\$$/P/")

CFLAGS += -D$(CFLAGS_PLATFORM)

# define objects collection
libscript_objs = core/script.o core/audio_id.o

# Do not warning unused label
CFLAGS += -Wno-unused-label

core_objs = core/core.o core/script_parser.o \
			view/view.o view/df_view_pad.o view/tp_track.o view/ion_alloc.o

obj_platform=$(shell echo camera_$(LICHEE_CHIP).o)

# Only obj_platform != camera_$(LICHEE_CHIP).o, need to redifined it.
ifeq ($(LICHEE_CHIP),sun50iw3p1)
ifeq ($(LICHEE_BUSINESS),vr)
obj_platform =
endif
else ifeq ($(LICHEE_CHIP),sun9iw1p1)
ifeq ($(LICHEE_BOARD),v1)
obj_platform =
endif
else ifeq ($(LICHEE_CHIP),sun50iw6p1)
obj_platform =
CFLAGS := $(CFLAGS) -I$(SYSROOT)/usr/include/directfb -D_SUN50IW6P1
tinyasa_dir = "tinyalsa-master-1.0"
else ifeq ($(LICHEE_CHIP),sun50iw9p1)
obj_platform =
CFLAGS := $(CFLAGS) -I$(SYSROOT)/usr/include/directfb -D_SUN50IW9P1
tinyasa_dir = "tinyalsa-master-2.0"
else ifeq ($(LICHEE_CHIP),sun8iw7p1)
obj_platform =
CFLAGS := $(CFLAGS) -I$(SYSROOT)/usr/include/directfb -D_SUN8IW7P1
tinyasa_dir = "tinyalsa-master-1.0"
else ifeq ($(LICHEE_CHIP),sun8iw6p1)
ifeq ($(LICHEE_BOARD),n106)
obj_platform = view/camera_sun8iw6p1_for_linux3_10.o
endif
else ifeq ($(wildcard view/camera_$(LICHEE_CHIP).c),)
# Universal implement
obj_platform =
endif

ifneq ($(obj_platform),)
core_objs += view/$(obj_platform)
endif

tinyasa_dir ?= "tinyalsa-master"

launcher_objs = core/launcher.o

# add targets to all
.PHONY: all
all: libscript.a core launcher
	make -C lib/${tinyasa_dir}
	make -C testcases
	make -C view

libscript.a: $(libscript_objs)
	@echo "  AR\t$^ -> $@"
	$(AR) $(ARFLAGS) $(LIBDIR)/$@ $^

$(libscript_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

core: $(core_objs)
	$(LINK_MSG)
	$(LINKX)

$(core_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

launcher: $(launcher_objs)
	$(LINK_MSG)
	$(LINKX)

$(launcher_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

.PHONY: clean
clean:
	@make -C lib/tinyalsa-master-1.0 clean
	@rm -rf $(BINDIR)/*
	@echo "  CLEAN *.o"
	@find . -name *.o -exec rm -f {} \;
	@echo "  CLEAN *.d"
	@find . -name *.d -exec rm -f {} \;
	@echo "  CLEAN *.d.*"
	@find . -name *.d.* -exec rm -f {} \;


# generate dependence
SRCDIRS = core/ view/
SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

-include $(SOURCES:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
