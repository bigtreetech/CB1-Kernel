/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 */

#ifndef __EFUSE_H__
#define __EFUSE_H__

#include <arch/cpu.h>

#define SID_PRCTL               (SUNXI_SID_BASE + 0x40)
#define SID_PRKEY               (SUNXI_SID_BASE + 0x50)
#define SID_RDKEY               (SUNXI_SID_BASE + 0x60)
#define SJTAG_AT0               (SUNXI_SID_BASE + 0x80)
#define SJTAG_AT1               (SUNXI_SID_BASE + 0x84)
#define SJTAG_S                 (SUNXI_SID_BASE + 0x88)

#define SID_EFUSE               (SUNXI_SID_BASE + 0x200)
#define SID_SECURE_MODE         (SUNXI_SID_BASE + 0xA0)
#define SID_OP_LOCK  (0xAC)

#define EFUSE_CHIPD             (0x00) /* 0x0-0xf, 128bits */
#define EFUSE_BROM_CONFIG       (0x10) /* 16 bit config, 16 bits try */
#define EFUSE_THERMAL_SENSOR    (0x14) /* 0x14-0x18, 32bits */
#define EFUSE_TF_ZONE           (0x18) /* 0x18-0x23, 96bits */
#define EFUSE_OEM_PROGRAM       (0x24) /* 0x24-0x27, 32bits */

/* write protect */
#define EFUSE_WRITE_PROTECT     (0x28) /* 0x28-0x2b, 32bits */
/* read  protect */
#define EFUSE_READ_PROTECT      (0x2c) /* 0x2c-0x2f, 32bits */
/* jtag security */
#define EFUSE_LCJS              (0x30)
/* jtag attribute */
#define EFUSE_ATTR              (0x34)

#define EFUSE_ROTPK             (0x38) /* 0x38-0x57, 256bits */
#define EFUSE_SSK               (0x58) /* 0x58-0x67, 128bits */
#define EFUSE_NV1               (0x68) /* 0x68-0x6B, 32 bits */
#define EFUSE_NV2               (0x6C) /* 0x6C-0x6F, 32 bits */
#define EFUSE_HUK		(0x70) /* 0x70-0x7F, 128bits */

/* size (bit)*/
#define SID_CHIPID_SIZE			(128)
#define SID_OEM_PROGRAM_SIZE	(32)
#define SID_NV1_SIZE			(32)
#define SID_NV2_SIZE			(32)
#define SID_THERMAL_SIZE		(32)
#define SID_ROTPK_SIZE			(256)
#define SID_SSK_SIZE			(128)
#define SID_HUK_SIZE			(128)


#endif    /*  #ifndef __EFUSE_H__  */
