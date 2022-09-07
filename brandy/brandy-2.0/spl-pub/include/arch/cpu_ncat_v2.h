/*
 * (C) Copyright 2018 allwinnertech  <wangwei@allwinnertech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SUNXI_CPU_NCAT_V2_H
#define _SUNXI_CPU_NCAT_V2_H

/*CPUX*/
#define SUNXI_CPUXCFG_BASE               (0x08100000U)

/*sys ctrl*/
#define SUNXI_TIMER_BASE				(0x02050000U)
#define SUNXI_CCM_BASE					(0x02001000U)
#define SUNXI_PIO_BASE					(0x02000000U)
#define SUNXI_SPC_BASE					(0x02000800U)
#define SUNXI_SYSCRL_BASE				(0x03000000U)
#define SUNXI_DMA_BASE					(0x03002000U)
#define SUNXI_SID_BASE					(0x03006000U)

#define SUNXI_CE_BASE					(0x03040000U)
#define SUNXI_SS_BASE					SUNXI_CE_BASE

#define SUNXI_SMC_BASE					(0x03007000U)

/*storage*/
#define SUNXI_SMHC0_BASE				(0x04020000U)
#define SUNXI_SMHC1_BASE				(0x04021000U)

/*noraml*/
#define SUNXI_UART0_BASE		(0x02500000U)
#define SUNXI_UART1_BASE		(0x02500400U)
#define SUNXI_UART2_BASE		(0x02500800U)
#define SUNXI_UART3_BASE		(0x02500C00U)

#define SUNXI_TWI0_BASE			(0x02502000U)
#define SUNXI_TWI1_BASE			(0x02502400U)

#define SUNXI_SPI0_BASE			(0x04025000U)
#define SUNXI_SPI1_BASE			(0x04026000U)


/*physical key*/
#define SUNXI_GPADC_BASE                  (0x02009000U)
#define SUNXI_LRADC_BASE                  (0x02009800U)
#define SUNXI_KEYADC_BASE                 SUNXI_LRADC_BASE

/*cpus*/
#define SUNXI_RTC_BASE					(0x07090000U)
#define SUNXI_AUDIO_CODEC				(0x02030000U)
#define SUNXI_CPUS_CFG_BASE				(0x07000400U)
#define SUNXI_RCPUCFG_BASE				(SUNXI_CPUS_CFG_BASE)
#define SUNXI_RPRCM_BASE				(0x07010000U)
#define SUNXI_RPWM_BASE					(0x07020c00U)
#define SUNXI_RPIO_BASE					(0x07022000U)
#define SUNXI_R_PIO_BASE				SUNXI_RPIO_BASE
#define SUNXI_RTWI_BASE					(0x07020800U)
#define SUNXI_RRSB_BASE					(0x07083000U)
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

#endif /* _SUNXI_CPU_SCAT_V2H */
