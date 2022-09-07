/*
 * (C) Copyright 20018-2019
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: spinor driver for General spinor operations
 * Author: wangwei <wangwei@allwinnertech.com>
 * Date: 2018-11-15 14:18:18
 */

#ifndef __SUNXI_SPINOR_H
#define __SUNXI_SPINOR_H

#include <common.h>
#include <asm/io.h>

typedef struct {
	__s32 readcmd;
	__s32 read_mode;
	__s32 write_mode;
	__s32 flash_size;
	__s32 erase_size;
	__s32 delay_cycle;    //When the frequency is greater than 60MHZ configured as 1;when the frequency is less than 24MHZ configured as 2;and other 3
	__s32 lock_flag;
	__s32  frequency;
} boot_spinor_info_t;

/*delay cycle*/
#define DELAY_ONE_CYCLE			1
#define DELAY_NORMAL_SAMPLE		2
#define DELAY_HALF_CYCLE		3

/* Erase commands */
#define CMD_ERASE_4K			0x20
#define CMD_ERASE_CHIP			0xc7
#define CMD_ERASE_64K			0xd8

/* Write commands */
#define CMD_WRITE_STATUS		0x01
#define CMD_WRITE_STATUS1		0x31
#define CMD_WRITE_CONFIG1		0xb1
#define CMD_PAGE_PROGRAM		0x02
#define CMD_WRITE_DISABLE		0x04
#define CMD_WRITE_ENABLE		0x06
#define CMD_QUAD_PAGE_PROGRAM		0x32

/* Used for Macronix and Winbond flashes. */
#define SPINOR_OP_EN4B			0xb7	/* Enter 4-byte mode */
#define SPINOR_OP_EX4B			0xe9	/* Exit 4-byte mode */

/* Used for Spansion flashes only. */
#define SPINOR_OP_BRWR			0x17	/* Bank register write */
#define SPINOR_OP_BRRD			0x16	/* Bank register read */
#define SPINOR_OP_CLSR			0x30	/* Clear status register 1 */

/* Read commands */
#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_DUAL_OUTPUT_FAST	0x3b
#define CMD_READ_DUAL_IO_FAST		0xbb
#define CMD_READ_QUAD_OUTPUT_FAST	0x6b
#define CMD_READ_QUAD_IO_FAST		0xeb
#define CMD_READ_ID			0x9f
#define CMD_READ_STATUS			0x05
#define CMD_READ_STATUS1		0x35
#define CMD_READ_CONFIG			0x35
#define CMD_READ_CONFIG1		0xb5
#define CMD_FLAG_STATUS			0x70

/*work mode*/
#define SPINOR_QUAD_MODE		4
#define SPINOR_DUAL_MODE		2
#define SPINOR_SINGLE_MODE		1

#define SPINOR_OP_READ_1_1_2    	0x3b    /* Read data bytes (Dual SPI) */
#define SPINOR_OP_READ_1_1_4    	0x6b    /* Read data bytes (Quad SPI) */
/* 4-byte address opcodes - used on Spansion and some Macronix flashes. */
#define SPINOR_OP_READ4_1_1_2   	0x3c    /* Read data bytes (Dual SPI) */
#define SPINOR_OP_READ4_1_1_4   	0x6c    /* Read data bytes (Quad SPI) */

/* CFI Manufacture ID's */
#define SPI_FLASH_CFI_MFR_SPANSION      0x01
//#define SPI_FLASH_CFI_MFR_STMICRO       0x20
#define SPI_FLASH_CFI_MFR_XMC	        0x20
#define SPI_FLASH_CFI_MFR_MACRONIX      0xc2
#define SPI_FLASH_CFI_MFR_SST           0xbf
#define SPI_FLASH_CFI_MFR_WINBOND       0xef
#define SPI_FLASH_CFI_MFR_ATMEL         0x1f
#define SPI_FLASH_CFI_MFR_GIGADEVICE    0xc8
#define SPI_FLASH_CFI_MFR_ADESTO        0xa1

#define STATUS_WIP			(1 << 0)
#define STATUS_QEB_WINSPAN		(1 << 1)
#define STATUS_QEB_MXIC			(1 << 6)
#define STATUS_QEB_GIGA			(1 << 1)
//#define STATUS_QEB_STMICRO		(1 << 3)


int spinor_init(int stage);
int spinor_exit(int force);
int spinor_read(uint start, uint nblock, void *buffer);


#endif
