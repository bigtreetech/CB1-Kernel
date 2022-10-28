# SPDX-License-Identifier: GPL-2.0+

dtb-$(CONFIG_MACH_SUN50I_H616) += \
	sun50i-h616-biqu.dtb

ifdef CONFIG_RCAR_GEN3
DTC_FLAGS += -R 4 -p 0x1000
endif

targets += $(dtb-y)

# Add any required device tree compiler flags here
DTC_FLAGS += -a 0x8

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb *.dtbo *_HS
