/*
 * (C) Copyright 2007-2015
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */

#ifndef __PLATFORM_H
#define __PLATFORM_H

#define SUNXI_SRAM_A2_BASE               (0x00100000L)

/*CPUX*/
#define SUNXI_CPUXCFG_BASE               (0x08100000L)

/*sys ctrl*/
#define SUNXI_TIMER_BASE				(0x02000000L)
#define SUNXI_CCM_BASE					(0x02001000L)
#define SUNXI_PIO_BASE					(0x02000400L)
#define SUNXI_SPC_BASE					(0x02000800L)
#define SUNXI_SYSCRL_BASE				(0x03000000L)
#define SUNXI_DMA_BASE					(0x03002000L)
#define SUNXI_SID_BASE					(0x03006000L)

#define SUNXI_CE_BASE					(0x03040000L)
#define SUNXI_SS_BASE					SUNXI_CE_BASE

#define SUNXI_SMC_BASE					(0x03007000L)

/*storage*/
#define SUNXI_NFC_BASE					(0x04011000L)
#define SUNXI_SMHC0_BASE				(0x04020000L)
#define SUNXI_SMHC1_BASE				(0x04021000L)


/*noraml*/
#define SUNXI_UART0_BASE				(0x02500000L)
#define SUNXI_UART1_BASE				(0x02500400L)
#define SUNXI_UART2_BASE				(0x02500800L)
#define SUNXI_UART3_BASE				(0x02500c00L)

#define SUNXI_TWI0_BASE					(0x025002000L)
#define SUNXI_TWI1_BASE					(0x025002400L)

#define SUNXI_SPI0_BASE					(0x04025000L)
#define SUNXI_SPI1_BASE					(0x04026000L)

#define SUNXI_GPADC_BASE				(0x07030000L)
#define SUNXI_LRADC_BASE				(0x07030800L)
#define SUNXI_KEYADC_BASE				SUNXI_LRADC_BASE

/*cpus*/
#define SUNXI_RTC_BASE					(0x07090000L)
#define SUNXI_AUDIO_CODEC				(0x07032000L)
#define SUNXI_CPUS_CFG_BASE				(0x07000400L)
#define SUNXI_RCPUCFG_BASE				(SUNXI_CPUS_CFG_BASE)
#define SUNXI_RPRCM_BASE				(0x07010000L)
#define SUNXI_RPWM_BASE					(0x07020c00L)
#define SUNXI_RPIO_BASE					(0x07022000L)
#define SUNXI_R_PIO_BASE				SUNXI_RPIO_BASE
#define SUNXI_RTWI_BASE					(0x07081400L)
#define SUNXI_RRSB_BASE					(0x07083000L)
#define SUNXI_RSB_BASE					SUNXI_RRSB_BASE
#define SUNXI_RTWI_BRG_REG				(SUNXI_RPRCM_BASE + 0x019c)
#define SUNXI_RTWI0_RST_BIT				(16)
#define SUNXI_RTWI0_GATING_BIT			(0)

#define SUNXI_RTC_DATA_BASE                 (SUNXI_RTC_BASE + 0x100)
#define AUDIO_CODEC_BIAS_REG				(SUNXI_AUDIO_CODEC + 0x320)

/* use for usb correct */
#define VDD_SYS_PWROFF_GATING_REG			(SUNXI_RPRCM_BASE + 0x250)
#define RES_CAL_CTRL_REG                    (SUNXI_RPRCM_BASE + 0X310)
#define VDD_ADDA_OFF_GATING					(9)
#define CAL_ANA_EN							(1)
#define CAL_EN								(0)

#define RVBARADDR0_L						(SUNXI_CPUXCFG_BASE + 0x40)
#define RVBARADDR0_H						(SUNXI_CPUXCFG_BASE + 0x44)

#define SRAM_CONTRL_REG0					(SUNXI_SYSCRL_BASE + 0x0)
#define SRAM_CONTRL_REG1					(SUNXI_SYSCRL_BASE + 0x4)

#define GPIO_BIAS_MAX_LEN (32)
#define GPIO_BIAS_MAIN_NAME "gpio_bias"
#define GPIO_POW_MODE_REG (0x0340)
#define GPIO_3_3V_MODE 0
#define GPIO_1_8V_MODE 1

#endif
