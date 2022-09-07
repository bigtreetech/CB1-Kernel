/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 */


#ifndef __SPC_H__
#define __SPC_H__


#include <config.h>
#include <arch/cpu.h>

#if defined(CONFIG_ARCH_SUN50IW3)

#define SPC_DECPORT_STA_REG(n)       (SUNXI_SPC_BASE + (n) * 0x0C + 0x04)
#define SPC_DECPORT_SET_REG(n)       (SUNXI_SPC_BASE + (n) * 0x0C + 0x08)
#define SPC_DECPORT_CLR_REG(n)       (SUNXI_SPC_BASE + (n) * 0x0C + 0x0C)

#define SPC_STATUS_REG(n)      (SUNXI_SPC_BASE + (n) * 0x0C + 0x04)
#define SPC_SET_REG(n)         (SUNXI_SPC_BASE + (n) * 0x0C + 0x08)
#define SPC_CLEAR_REG(n)       (SUNXI_SPC_BASE + (n) * 0x0C + 0x0C)

#else
#error "Unsupported plat"
#endif

#ifndef __ASSEMBLY__
void sunxi_spc_set_to_ns(uint type);
#endif

#endif /* __SPC_H__ */
