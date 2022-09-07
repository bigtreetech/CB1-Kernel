
#
# Makefile for sunxi bootloader
# wangwei@allwinnertech.com
#

TOPDIR=$(CURDIR)
SRCTREE=$(TOPDIR)

#Q: quit for compile
Q = @
ifeq ("$(origin V)", "command line")
VERBOSE=$(V)
endif
ifndef VERBOSE
VERBOSE=0
endif

ifeq ($(VERBOSE),1)
Q=
CMD_ECHO_SILENT := true
else
CMD_ECHO_SILENT := echo
endif

#P: platform: sun50iw3p1/sun8iw15p1 etc.
PLATFORM=NULL
ifeq ("$(origin p)", "command line")
PLATFORM=$(p)
endif


#M: compile module: nand mmc nor etc.
MODULE=NULL
ifeq ("$(origin m)", "command line")
MODULE=$(m)
endif

#ddr: compile module: dd3/lpddr3/ddr4/lpddr4
MODULE=NULL
ifeq ("$(origin ddr)", "command line")
DRAM_TYPE=$(ddr)
DRAM_TYPE_NAME=_$(DRAM_TYPE)
endif

#check input param
MK_FILE_CHANGE := $(shell if [ x$(p) != x ]; then echo yes; else echo no; fi;)
#$(info MK_FILE_CHANGE=$(MK_FILE_CHANGE))
ifeq (x$(MK_FILE_CHANGE),xyes)
	FILE_EXIST=$(shell if [ -f $(TOPDIR)/board/$(p)/common.mk ]; then echo yes; else echo no; fi;)
ifeq (x$(FILE_EXIST),xno)
$(info ***);
$(info ***configure plat or module not exist, Please run some configurator);
$(info ***e.g. make p=sun50iw3p1; make boot0);
$(info ***e.g. make p=sun50iw3p1; make sboot);
$(info ***e.g. make p=sun50iw3p1; make fes);
$(info ***e.g. make p=sun8iw15p1 ddr=ddr3|lpddr3|ddr4|lpddr4; make boot0);
$(info ***e.g. make p=sun8iw15p1 ddr=ddr3|lpddr3|ddr4|lpddr4; make sboot);
$(info ***e.g. make p=sun8iw15p1 ddr=ddr3|lpddr3|ddr4|lpddr4; make fes);
$(info ***);
$(error exit);
endif
$(shell cp -f $(TOPDIR)/board/$(p)/common.mk $(TOPDIR)/.module.common.mk)
$(shell sed -i '$$a\DRAM_TYPE_NAME=$(DRAM_TYPE_NAME)' $(TOPDIR)/.module.common.mk)
endif

#include config file
include $(TOPDIR)/mk/config.mk
include $(TOPDIR)/mk/checkconf.mk
sinclude $(TOPDIR)/.module.common.mk
arm_toolchain_check=$(shell if [ ! -d ../tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi ]; then echo yes; else echo no; fi;)
ifeq (x$(arm_toolchain_check), xyes)
$(info Prepare toolchain ...);
$(shell mkdir -p ../tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi || exit 1)
$(shell tar --strip-components=1 -xf ../tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz -C ../tools/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi || exit 1)
endif

export Q SOC TOPDIR SRCTREE CMD_ECHO_SILENT MODULE PLATFORM DRAM_TYPE_NAME

#$(info platform=$(PLATFORM) module=$(MODULE))

define build-boot0-module-if-exist
	@if [ x$(NAND_EXIST) = xyes ]; then  \
	$(MAKE)  -C  $(SRCTREE)/nboot nand; fi
	@if [ x$(MMC_EXIST) = xyes ]; then  \
	$(MAKE)  -C  $(SRCTREE)/nboot mmc; fi
	@if [ x$(SPINOR_EXIST) = xyes ]; then  \
	$(MAKE)  -C  $(SRCTREE)/nboot spinor; fi
endef


MAKEFLAGS +=  --no-print-directory

NAND_EXIST:=$(shell if [ -f $(TOPDIR)/board/$(PLATFORM)/nand.mk ]; then echo yes; else echo no; fi;)
MMC_EXIST:=$(shell if [ -f $(TOPDIR)/board/$(PLATFORM)/mmc.mk ]; then echo yes; else echo no; fi;)
SPINOR_EXIST:=$(shell if [ -f $(TOPDIR)/board/$(PLATFORM)/spinor.mk ]; then echo yes; else echo no; fi;)
SBOOT_EXIST:=$(shell if [ -f $(TOPDIR)/board/$(PLATFORM)/sboot.mk ]; then echo yes; else echo no; fi;)
ifeq (x$(MK_FILE_CHANGE),xyes)
#makefile is called to config platform, do nothing
all:
	@echo "platform set to $(PLATFORM)"
	@-ln -s $(TOPDIR)/board/$(PLATFORM)/commit_info.h include/commit_info.h
else
all: mkdepend
	$(MAKE) -C $(SRCTREE)/fes fes
	$(call build-boot0-module-if-exist)
ifeq (x$(SBOOT_EXIST),xyes)
	$(MAKE) -C $(SRCTREE)/sboot sboot
endif #SBOOT_EXIST
endif

ifeq (x$(NAND_EXIST),xyes)
nand: mkdepend
	$(MAKE)  -C  $(SRCTREE)/nboot nand
endif
ifeq (x$(MMC_EXIST),xyes)
mmc: mkdepend
	$(MAKE)  -C  $(SRCTREE)/nboot mmc
endif
ifeq (x$(SPINOR_EXIST),xyes)
spinor: mkdepend
	$(MAKE)  -C  $(SRCTREE)/nboot spinor
endif



fes: mkdepend
	$(MAKE) -C $(SRCTREE)/fes fes
boot0: mkdepend
	$(call build-boot0-module-if-exist)
ifeq (x$(SBOOT_EXIST),xyes)
sboot: mkdepend
	$(MAKE) -C $(SRCTREE)/sboot sboot
endif


clean:
	@find $(TOPDIR) -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'	-o -name '*.exe' -o -name '*.axf' \
		-o -name '*.elf' \
		-o -name '*.depend' \
		-o -name '*.bin' \
		-o -name '*.log' \
		-o -name '*.map' \) -print \
		| xargs rm -f

	@rm -f $(TOPDIR)/nboot/boot0.lds
	@rm -f $(TOPDIR)/fes/fes1.lds
	@rm -f $(TOPDIR)/sboot/sboot.lds
	@rm -f $(TOPDIR)/autoconf.mk

distclean: clean
	@rm -f $(TOPDIR)/include/config.h
	@rm -f $(TOPDIR)/include/commit_info.h
	@rm -f $(TOPDIR)/.module.common.mk
	@rm -f $(cleanfiles)

mkdepend : 
	@if [ ! -f $(TOPDIR)/.module.common.mk ]; then  \
	rm -rf autoconf.mk; \
	echo "***"; \
	echo "*** Configuration file \".module.common.mk\" not found!"; \
	echo "***"; \
	echo "*** Please run some configurator (e.g. make p=sun50iw3p1)"; \
	echo "***"; \
	exit 1; \
	fi;
	$(call update-commit-info)
depend:

PHONY +=FORCE
FORCE:
.PHONY:$(PHONY)



