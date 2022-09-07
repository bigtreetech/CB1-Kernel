#ifndef __ASM_ARM_BYTEORDER_H
#define __ASM_ARM_BYTEORDER_H


#include <asm/types.h>

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#if defined(__ARMEB__) || defined(__AARCH64EB__)
#include <linux/byteorder/big_endian.h>
#else
#include <linux/byteorder/little_endian.h>
#endif

#endif
