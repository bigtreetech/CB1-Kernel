/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Berg Xing <bergxing@allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * Sunxi platform dram register definition.
 */

#ifndef _SUNXI_DRAM_H
#define _SUNXI_DRAM_H

#include <asm/io.h>
#include <linux/types.h>

/* dram regs definition */
#if defined(CONFIG_DRAM_PARA_V1)
#include <arch/dram_v1.h>
#elif defined(CONFIG_DRAM_PARA_V2)
#include <arch/dram_v2.h>
#else
#error "Unsupported plat"
#endif

unsigned long sunxi_dram_init(void);

#endif /* _SUNXI_DRAM_H */
