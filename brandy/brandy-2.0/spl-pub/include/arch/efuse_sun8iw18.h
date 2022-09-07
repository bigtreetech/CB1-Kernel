/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 */

#ifndef __EFUSE_H__
#define __EFUSE_H__

#include <arch/cpu.h>

#define SID_PRCTL				(SUNXI_SID_BASE + 0x40)
#define SID_PRKEY				(SUNXI_SID_BASE + 0x50)
#define SID_RDKEY				(SUNXI_SID_BASE + 0x60)
#define SJTAG_AT0				(SUNXI_SID_BASE + 0x80)
#define SJTAG_AT1				(SUNXI_SID_BASE + 0x84)
#define SJTAG_S					(SUNXI_SID_BASE + 0x88)

#define SID_EFUSE               (SUNXI_SID_BASE + 0x200)
#define SID_SECURE_MODE			(SUNXI_SID_BASE + 0xA0)
#define SID_OP_LOCK				(0xAC)

#define EFUSE_CHIPD			(0x00)/* 128 bits */
#define EFUSE_BROM_CONFIG		(0x10)/* 16 bits config, 16 bits try */
#define EFUSE_THERMAL_SENSOR		(0x14)/* 64 bits */
#define EFUSE_TF_ZONE			(0x1C)/* 128 bits */
#define EFUSE_OEM_PROGRAM		(0x2C)/* 128bits :tvout 32+reserv 96 */

#define EFUSE_WRITE_PROTECT		(0x30)/* 32 bits */
#define EFUSE_READ_PROTECT		(0x34)/* 32 bits */
#define EFUSE_LCJS			(0x38)/* 32 bits */
#define EFUSE_ATTR			(0x3C)/* 32 bits */
#define EFUSE_ROTPK			(0x40)/* 256 bits */
#define EFUSE_SSK			(0x60)/* 128 bits */
#define EFUSE_NV1			(0x70)/* 32 bits */
#define EFUSE_NV2			(0x74)/* 32 bits */
#define EFUSE_OEM_PROGRAM_SECURE	(0x78)/* 64 bits */

/* size (bit) */
#define SID_CHIPID_SIZE			(128)
#define SID_THERMAL_SIZE		(64)
#define SID_OEM_PROGRAM_SIZE		(32)
#define SID_NV1_SIZE			(32)
#define SID_NV2_SIZE			(32)
#define SID_ROTPK_SIZE			(256)
#define SID_SSK_SIZE			(128)
#define SID_OEM_PROGRAM_SECURE_SIZE	(64)

uint sid_read_key(uint key_index);
void sid_program_key(uint key_index, uint key_value);
void sid_set_security_mode(void);
int  sid_probe_security_mode(void);
int  sid_get_security_status(void);

#endif    /*  #ifndef __EFUSE_H__  */
