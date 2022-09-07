/*
 * (C) Copyright 2015 Hans de Goede <hdegoede@redhat.com>
 *
 * Sunxi PMIC bus access helpers header
 */

#ifndef _SUNXI_PMIC_BUS_H
#define _SUNXI_PMIS_BUS_H

int pmic_bus_init(u32 device_addr, u32 runtime_addr);
int pmic_bus_read(u32 runtime_addr, u8 reg, u8 *data);
int pmic_bus_write(u32 runtime_addr, u8 reg, u8 data);
int pmic_bus_setbits(u32 runtime_addr, u8 reg, u8 bits);
int pmic_bus_clrbits(u32 runtime_addr, u8 reg, u8 bits);

#endif
