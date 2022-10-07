# SPDX-License-Identifier: GPL-2.0
# Makefile for Linux PHY drivers

libphy-y			:= phy.o phy-c45.o phy-core.o phy_device.o \
				   linkmode.o sunxi-ephy.o
mdio-bus-y			+= mdio_bus.o mdio_device.o

ifdef CONFIG_MDIO_DEVICE
obj-y				+= mdio-boardinfo.o
endif

# PHYLIB implies MDIO_DEVICE, in that case, we have a bunch of circular
# dependencies that does not make it possible to split mdio-bus objects into a
# dedicated loadable module, so we bundle them all together into libphy.ko
ifdef CONFIG_PHYLIB
libphy-y			+= $(mdio-bus-y) 
else
obj-$(CONFIG_MDIO_DEVICE)	+= mdio-bus.o
endif
obj-$(CONFIG_MDIO_DEVRES)	+= mdio_devres.o
libphy-$(CONFIG_SWPHY)		+= swphy.o
libphy-$(CONFIG_LED_TRIGGER_PHY)	+= phy_led_triggers.o

obj-$(CONFIG_PHYLINK)		+= phylink.o
obj-$(CONFIG_PHYLIB)		+= libphy.o

obj-$(CONFIG_NETWORK_PHY_TIMESTAMPING) += mii_timestamper.o

obj-$(CONFIG_SFP)		+= sfp.o
sfp-obj-$(CONFIG_SFP)		+= sfp-bus.o
obj-y				+= $(sfp-obj-y) $(sfp-obj-m)

obj-$(CONFIG_MOTORCOMM_PHY)	+= motorcomm.o
obj-$(CONFIG_QSEMI_PHY)		+= qsemi.o
obj-$(CONFIG_TERANETICS_PHY)	+= teranetics.o
obj-$(CONFIG_VITESSE_PHY)	+= vitesse.o

