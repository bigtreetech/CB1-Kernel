/*
 * Copyright 2014 - Hans de Goede <hdegoede@redhat.com>
 */
#ifndef _SUNXI_I2C_H_
#define _SUNXI_I2C_H_

#include <common.h>
#include <asm/io.h>

#ifdef CONFIG_I2C0_ENABLE
#define CONFIG_I2C_MVTWSI_BASE0	SUNXI_TWI0_BASE
#endif
#ifdef CONFIG_I2C1_ENABLE
#define CONFIG_I2C_MVTWSI_BASE1	SUNXI_TWI1_BASE
#endif
#ifdef CONFIG_I2C2_ENABLE
#define CONFIG_I2C_MVTWSI_BASE2	SUNXI_TWI2_BASE
#endif
#ifdef CONFIG_I2C3_ENABLE
#define CONFIG_I2C_MVTWSI_BASE3	SUNXI_TWI3_BASE
#endif
#ifdef CONFIG_I2C4_ENABLE
#define CONFIG_I2C_MVTWSI_BASE4	SUNXI_TWI4_BASE
#endif
#ifdef CONFIG_R_I2C_ENABLE
#define CONFIG_I2C_MVTWSI_BASE5 SUNXI_R_TWI_BASE
#endif

/* This is abp0-clk on sun4i/5i/7i / abp1-clk on sun6i/sun8i which is 24MHz */
#define CONFIG_SYS_TCLK		24000000
#define TWI_CONTROL_OFFSET             0x400
#define SUNXI_I2C_CONTROLLER             3

struct sunxi_twi_reg {
	volatile unsigned int addr;        /* slave address     */
	volatile unsigned int xaddr;       /* extend address    */
	volatile unsigned int data;        /* data              */
	volatile unsigned int ctl;         /* control           */
	volatile unsigned int status;      /* status            */
	volatile unsigned int clk;         /* clock             */
	volatile unsigned int srst;        /* soft reset        */
	volatile unsigned int eft;         /* enhanced future   */
	volatile unsigned int lcr;         /* line control      */
	volatile unsigned int dvfs;        /* dvfs control      */
};

void i2c_init_cpus(int speed, int slaveaddr);
int i2c_read(u8 chip, uint addr, int alen, u8 *buffer, int len);
int i2c_write(u8 chip, uint addr, int alen, u8 *buffer, int len);



#endif
