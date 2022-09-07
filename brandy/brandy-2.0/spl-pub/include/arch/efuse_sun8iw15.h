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
#define EFUSE_THERMAL_SENSOR    (0x14) /* 0x14-0x1b, 64bits */
#define EFUSE_TF_ZONE           (0x1C) /* 0x1c-0x2b, 128bits */
#define EFUSE_OEM_PROGRAM       (0x2C) /* 0x2c-0x3f, 160bits */

/* write protect */
#define EFUSE_WRITE_PROTECT     (0x40) /* 0x40-0x43, 32bits */
/* read  protect */
#define EFUSE_READ_PROTECT      (0x44) /* 0x44-0x47, 32bits */
/* jtag security */
#define EFUSE_LCJS              (0x48)
/* jtag attribute */
#define EFUSE_ATTR              (0x4C)

/*0x50-0x6F
0x50-0x67: 192bits HUK hardware Unique key
0x68-0x6b: 32bits ID of operator
0x6C-0x6F: 32bits ID
*/
#define EFUSE_IN			(0x50)/* 192 bits */
#define EFUSE_HUK			(0x50)/* 192 bits */
#define EFUSE_INDENTIFICATION		(0x68)/* 32 bits */
#define EFUSE_ID			(0x6C)/* 32 bits */
#define EFUSE_ROTPK			(0x70)/* 256 bits */
#define EFUSE_SSK			(0x90)/* 128 bits */
#define EFUSE_RSSK			(0xA0)/* 256 bits */
#define EFUSE_HDCP_HASH         (0xC0) /* 0xC0-0xCf, 128bits */
#define EFUSE_EK_HASH			(0xD0)/* 128 bits */
#define EFUSE_SN			(0xE0)/* 192 bits */
#define EFUSE_NV1			(0xF8)/* 32 bits */
#define EFUSE_NV2			(0xFC)/* 224 bits */
#define EFUSE_TCON_PRAM         (0x118) /* 0x118-0x11B,32bits */
#define EFUSE_BACKUP_KEY		(0x11C)/* 288 bits */

/* size (bit)*/
#define SID_CHIPID_SIZE			(128)
#define SID_OEM_PROGRAM_SIZE	(128)
#define SID_NV1_SIZE			(32)
#define SID_NV2_SIZE			(224)
#define SID_THERMAL_SIZE		(96)
#define SID_IN_SIZE			    (192)
#define SID_IDENTIFY_SIZE		(32)
#define SID_ID_SIZE			    (32)
#define SID_ROTPK_SIZE			(256)
#define SID_SSK_SIZE			(128)
#define SID_RSSK_SIZE			(256)
#define SID_EK_HASH_SIZE		(128)
#define SID_SN_SIZE			    (192)


#endif    /*  #ifndef __EFUSE_H__  */
