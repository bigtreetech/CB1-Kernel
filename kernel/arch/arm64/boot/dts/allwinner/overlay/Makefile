# SPDX-License-Identifier: GPL-2.0

# Overlays Instructions
# https://docs.armbian.com/User-Guide_Armbian_overlays/

dtbo-$(CONFIG_ARCH_SUNXI) += \
	sun50i-h616-spi-spidev.dtbo \
	sun50i-h616-spidev0_0.dtbo \
	sun50i-h616-spidev1_0.dtbo \
	sun50i-h616-spidev1_1.dtbo \
	sun50i-h616-spidev1_2.dtbo \
	sun50i-h616-ir.dtbo \
	sun50i-h616-tft35_spi.dtbo \
	sun50i-h616-tft35_spi25.dtbo \
	sun50i-h616-mcp2515.dtbo \
	sun50i-h616-pwm.dtbo \
	sun50i-h616-disable_uart0.dtbo \
	sun50i-h616-ws2812.dtbo \
	sun50i-h616-light.dtbo

scr-$(CONFIG_ARCH_SUNXI) += \
	sun50i-h616-fixup.scr
	
targets += $(dtbo-y) $(scr-y) $(dtbotxt-y)

always		:= $(dtbo-y) $(scr-y) $(dtbotxt-y)
clean-files	:= *.dtbo *.scr
