/*
 * (C) Copyright 2015 Hans de Goede <hdegoede@redhat.com>
 */

#ifndef _SUNXI_CPU_H
#define _SUNXI_CPU_H

#include <config.h>

#if defined(CONFIG_SUNXI_NCAT)
#include <arch/cpu_ncat.h>
#elif defined(CONFIG_SUNXI_VERSION1)
#include <arch/cpu_version1.h>
#else
#error "Unsupported plat"
#endif

#endif /* _SUNXI_CPU_H */
