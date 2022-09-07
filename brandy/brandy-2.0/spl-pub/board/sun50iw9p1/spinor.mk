#
#config file for sun50iw9
#

include $(TOPDIR)/board/sun50iw9p1/common.mk

MODULE=spinor
CFG_SUNXI_SPINOR =y
CFG_SUNXI_SPI =y
CFG_SPINOR_UBOOT_OFFSET=128
