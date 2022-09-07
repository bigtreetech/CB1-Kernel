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
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __PLATFORM_H
#define __PLATFORM_H

#define SUNXI_CE_BASE (0x01c15000L)
#define SUNXI_SS_BASE SUNXI_CE_BASE

//CPUX
#define SUNXI_CPUXCFG_BASE (0x01f01c00L)
//sys ctrl
#define SUNXI_SYSCRL_BASE (0x01c00000L)
#define SUNXI_CCM_BASE (0x01c20000L)
#define SUNXI_DMA_BASE (0x01c02000L)
#define SUNXI_MSGBOX_BASE (0X01C17000L)
#define SUNXI_SPINLOCK_BASE (0X01C18000L)
#define SUNXI_HSTMR_BASE (0x01c60000L)
#define SUNXI_SID_BASE (0x01c14000L)
#define SUNXI_SMC_BASE (0x01c1e000L)
//todo:#define SUNXI_SPC_BASE                   (0X01C21400L)

#define SUNXI_TIMER_BASE (0x01c20c00L)
//todo:#define SUNXI_CNT64_BASE                 (0x03009C00L)
#define SUNXI_PWM_BASE (0X01C21400L)
#define SUNXI_PIO_BASE (0x01c20800L)
//todo:#define SUNXI_PSI_BASE                   (0x0300C000L)
//todo:#define SUNXI_DCU_BASE                   (0x03010000L)
#define SUNXI_GIC_BASE (0x01c81000L)
//todo:#define SUNXI_IOMMU_BASE                 (0x030F0000L)

//storage
#define SUNXI_DRAMCTL0_BASE (0x01c63000L)
#define SUNXI_NFC_BASE (0x01c03000L)
#define SUNXI_SMHC0_BASE (0x01c0f000L)
#define SUNXI_SMHC1_BASE (0x01c10000L)
#define SUNXI_SMHC2_BASE (0x01c11000L)

//noraml
#define SUNXI_UART0_BASE (0x01c28000L)
#define SUNXI_UART1_BASE (0x01c28400L)
#define SUNXI_UART2_BASE (0x01c28800L)
#define SUNXI_UART3_BASE (0x01c28c00L)
#define SUNXI_UART4_BASE (0x01c29000L)

#define SUNXI_TWI0_BASE (0x01c2ac00L)
#define SUNXI_TWI1_BASE (0x01c2b000L)
#define SUNXI_TWI2_BASE (0x01c2b400L)
#define SUNXI_TWI3_BASE (0x01c2b800L)

//todo:#define SUNXI_SCR0_BASE                   (0x05005000L)

#define SUNXI_SPI0_BASE (0x01c68000L)
#define SUNXI_SPI1_BASE (0x01c69000L)
#define SUNXI_GMAC_BASE (0x01c50000L)

//todo:#define SUNXI_GPADC_BASE                  (0x05070000L)
#define SUNXI_LRADC_BASE (0X01C21800L)
#define SUNXI_KEYADC_BASE SUNXI_LRADC_BASE

#define SUNXI_USBOTG_BASE (0x01c19000L)
#define SUNXI_EHCI0_BASE (0x01c1a000L)
#define SUNXI_EHCI1_BASE (0x01c1b000L)
#define SUNXI_WDOG_BASE  (0x01C20CA0L)

//todo:#define ARMV7_GIC_BASE                    (SUNXI_GIC_BASE+0x1000L)
//todo:#define ARMV7_CPUIF_BASE                  (SUNXI_GIC_BASE+0x2000L)

//cpus
#define SUNXI_RTC_BASE (0x01f00000L)
#define SUNXI_CPUS_CFG_BASE (0x01f01C00L)
#define SUNXI_RCPUCFG_BASE (SUNXI_CPUS_CFG_BASE)
#define SUNXI_RPRCM_BASE (0x01f01400L)
//todo:#define SUNXI_RPWM_BASE                     (0x07020c00L)
#define SUNXI_RPIO_BASE (0X01F02C00L)
#define SUNXI_R_PIO_BASE (0X01F02C00L)
//todo:#define SUNXI_RTWI_BASE                     (0x07081400L)
//todo:#define SUNXI_RRSB_BASE                     (0x07083000L)
//todo:#define SUNXI_RSB_BASE                      (0x07083000L)

#define SUNXI_RTC_DATA_BASE (SUNXI_RTC_BASE + 0x100)

/* use for usb correct */
//todo:#ifdef CONFIG_ARCH_SUN8IW18P1
//todo:#define VDD_SYS_PWROFF_GATING_REG			(SUNXI_RTC_BASE + 0x220)
//todo:#define RES_CAL_CTRL_REG                    (SUNXI_RTC_BASE + 0X230)
//todo:#else
//todo:#define VDD_SYS_PWROFF_GATING_REG			(SUNXI_RPRCM_BASE + 0x250)
//todo:#define RES_CAL_CTRL_REG                    (SUNXI_RPRCM_BASE + 0X310)
//todo:#define VDD_ADDA_OFF_GATING					(9)
//todo:#define CAL_ANA_EN							(1)
//todo:#define CAL_EN								(0)
//todo:#endif

//todo:#define RVBARADDR0_L             (SUNXI_CPUXCFG_BASE+0x40)
//todo:#define RVBARADDR0_H             (SUNXI_CPUXCFG_BASE+0x44)

//todo:#define SRAM_CONTRL_REG0         (SUNXI_SYSCRL_BASE+0x0)
//todo:#define SRAM_CONTRL_REG1         (SUNXI_SYSCRL_BASE+0x4)

//todo:#define GPIO_BIAS_MAX_LEN (32)
//todo:#define GPIO_BIAS_MAIN_NAME "gpio_bias"
//todo:#define GPIO_POW_MODE_REG (0x0340)
//todo:#define GPIO_3_3V_MODE 0
//todo:#define GPIO_1_8V_MODE 1

#endif
