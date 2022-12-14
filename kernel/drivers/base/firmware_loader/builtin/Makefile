# SPDX-License-Identifier: GPL-2.0
obj-y  += main.o

# Create $(fwdir) from $(CONFIG_EXTRA_FIRMWARE_DIR) -- if it doesn't have a
# leading /, it's relative to $(srctree).
fwdir := $(subst $(quote),,$(CONFIG_EXTRA_FIRMWARE_DIR))
fwdir := $(addprefix $(srctree)/,$(filter-out /%,$(fwdir)))$(filter /%,$(fwdir))

firmware  := $(addsuffix .gen.o, $(subst $(quote),,$(CONFIG_EXTRA_FIRMWARE)))
obj-y += $(firmware)

FWNAME    = $(patsubst $(obj)/%.gen.S,%,$@)
FWSTR     = $(subst $(comma),_,$(subst /,_,$(subst .,_,$(subst -,_,$(FWNAME)))))
ASM_WORD  = $(if $(CONFIG_64BIT),.quad,.long)
ASM_ALIGN = $(if $(CONFIG_64BIT),3,2)
PROGBITS  = $(if $(CONFIG_ARM),%,@)progbits

filechk_fwbin = \
	echo "/* Generated by $(src)/Makefile */"		;\
	echo "    .section .rodata"				;\
	echo "    .p2align 4"					;\
	echo "_fw_$(FWSTR)_bin:"				;\
	echo "    .incbin \"$(fwdir)/$(FWNAME)\""		;\
	echo "_fw_end:"						;\
	echo "    .section .rodata.str,\"aMS\",$(PROGBITS),1"	;\
	echo "    .p2align $(ASM_ALIGN)"			;\
	echo "_fw_$(FWSTR)_name:"				;\
	echo "    .string \"$(FWNAME)\""			;\
	echo "    .section .builtin_fw,\"a\",$(PROGBITS)"	;\
	echo "    .p2align $(ASM_ALIGN)"			;\
	echo "    $(ASM_WORD) _fw_$(FWSTR)_name"		;\
	echo "    $(ASM_WORD) _fw_$(FWSTR)_bin"			;\
	echo "    $(ASM_WORD) _fw_end - _fw_$(FWSTR)_bin"

$(obj)/%.gen.S: FORCE
	$(call filechk,fwbin)

# The .o files depend on the binaries directly; the .S files don't.
$(addprefix $(obj)/, $(firmware)): $(obj)/%.gen.o: $(fwdir)/%

targets := $(patsubst $(obj)/%,%, \
                                $(shell find $(obj) -name \*.gen.S 2>/dev/null))
