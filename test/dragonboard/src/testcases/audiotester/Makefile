
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

tinyalsa_objs += \
	./common/tinyalsa/pcm.o \
	./common/tinyalsa/mixer.o

CFLAGS := $(CFLAGS) -I./common/tinyalsa/include
# add stdc++ lib and math lib
CFLAGS := $(CFLAGS) -lstdc++ -lm
audiotester_objs += \
	./common/AudioFileContext.o \
	./common/AudioPlayer.o \
	./common/AudioRecorder.o \
	./common/AudioRouter.o \
	./common/FFT.o \
	./common/common.o \
	LoopbackTester.o \
	PlayRecTester.o \
	AudioTester.o

# chip header file
ifeq ($(LICHEE_CHIP),sun50iw1p1)
CFLAGS := $(CFLAGS) -I./platform/sun50iw1p
endif

ifeq ($(LICHEE_CHIP),sun9iw1p1)
CFLAGS := $(CFLAGS) -I./platform/sun9iw1p
endif

ifeq ($(LICHEE_CHIP),sun8iw6p1)
CFLAGS := $(CFLAGS) -I./platform/sun8iw6p
endif

ifeq ($(LICHEE_CHIP),sun8iw5p1)
CFLAGS := $(CFLAGS) -I./platform/sun8iw5p
endif

# add your target(s) to all
.PHONY: all
all: audiotester

# define you target, the target will be output to dragonboard/output/bin
# directory
audiotester: $(tinyalsa_objs) $(audiotester_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'example_objs' to your objects collection variable
$(tinyalsa_objs): %.o: %.c
	$(COMPILE_MSG)
	$(COMPILEX)

# change 'example_objs' to your objects collection variable
$(audiotester_objs): %.o: %.cpp
	$(COMPILE_MSG)
	$(COMPILEX)
