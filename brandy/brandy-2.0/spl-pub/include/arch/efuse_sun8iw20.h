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
#define SJTAG_AT1				(SUNXI_SID_BASE + 0x84)
#define SJTAG_S					(SUNXI_SID_BASE + 0x88)

#define SID_EFUSE               (SUNXI_SID_BASE + 0x200)
#define SID_SECURE_MODE         (SUNXI_SID_BASE + 0xA0)
#define SID_OP_LOCK  (0xAC)

#define SID_WORK_STATUS			(3)

/*efuse power ctl*/
#define EFUSE_HV_SWITCH			(SUNXI_RTC_BASE + 0x204)

#define EFUSE_THS				(0x14)
/* jtag security */
#define EFUSE_LCJS              (0x30)
#define EFUSE_ROTPK             (0x38) /* 0x38-0x57, 256bits */
#define EFUSE_NV1               (0x68) /* 0x68-0x6B, 32 bits */

/* size (bit)*/
#define SID_NV1_SIZE			(32)
#define SID_ROTPK_SIZE			(256)


#endif    /*  #ifndef __EFUSE_H__  */
