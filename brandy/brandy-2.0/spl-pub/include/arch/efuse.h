/*
 * (C) Copyright 2018
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 */

#ifndef _SUNXI_EFUSE_H
#define _SUNXI_EFUSE_H

#include <linux/types.h>
#include <config.h>
#include <arch/cpu.h>


/* clock control module regs definition */
#if defined(CONFIG_ARCH_SUN50IW3)
#include <arch/efuse_sun50iw3.h>
#elif defined(CONFIG_ARCH_SUN50IW9)
#include <arch/efuse_sun50iw9.h>
#elif defined(CONFIG_ARCH_SUN50IW10)
#include <arch/efuse_sun50iw10.h>
#elif defined(CONFIG_ARCH_SUN8IW15)
#include <arch/efuse_sun8iw15.h>
#elif defined(CONFIG_ARCH_SUN8IW18)
#include <arch/efuse_sun8iw18.h>
#elif defined(CONFIG_ARCH_SUN8IW7)
#include <arch/efuse_sun8iw7.h>
#else
#error "Unsupported plat"
#endif

#define JTAG_AT_SOURCE (1 << 0)
#define JTAG_DBGEN_CORE(x) ((1 << (x)) << 0)
#define JTAG_NIDEN_CORE(x) ((1 << (x)) << 4)
#define JTAG_SPIDEN_CORE(x) ((1 << (x)) << 8)
#define JTAG_SPNIDEN_CORE(x) ((1 << (x)) << 12)
#define JTAG_DEVICEEN (1 << 0)
#define JTAG_EFUSE_AT0_OFFSET (0x0)
#define JTAG_EFUSE_AT1_OFFSET (0x8)

#ifndef __ASSEMBLY__
uint sid_sram_read(uint key_index);
uint sid_read_key(uint key_index);
void sid_program_key(uint key_index, uint key_value);
void sid_set_security_mode(void);
int  sid_probe_security_mode(void);
int  sid_get_security_status(void);
void sid_read_rotpk(void *dst);
void sid_disable_jtag(void);
#endif

#endif /* _SUNXI_EFUSE_H */
