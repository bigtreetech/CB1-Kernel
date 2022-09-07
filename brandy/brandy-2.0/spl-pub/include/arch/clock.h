/*
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 */

#ifndef _SUNXI_CLOCK_H
#define _SUNXI_CLOCK_H

#include <linux/types.h>

#define CLK_GATE_OPEN			0x1
#define CLK_GATE_CLOSE			0x0

/* clock control module regs definition */
#if defined(CONFIG_ARCH_SUN50IW3)
#include <arch/clock_sun50iw3.h>
#elif defined(CONFIG_ARCH_SUN50IW9)
#include <arch/clock_sun50iw9.h>
#elif defined(CONFIG_ARCH_SUN8IW18)
#include <arch/clock_sun8iw18.h>
#elif defined(CONFIG_ARCH_SUN8IW16)
#include <arch/clock_sun8iw16.h>
#elif defined(CONFIG_ARCH_SUN8IW19)
#include <arch/clock_sun8iw19.h>
#elif defined(CONFIG_ARCH_SUN50IW10)
#include <arch/clock_sun50iw10.h>
#elif defined(CONFIG_ARCH_SUN8IW15)
#include <arch/clock_sun8iw15.h>
#elif defined(CONFIG_ARCH_SUN8IW7)
#include <arch/clock_sun8iw7.h>
#elif defined(CONFIG_ARCH_SUN50IW11)
#include <arch/clock_sun50iw11.h>
#else
#error "Unsupported plat"
#endif

#ifdef CONFIG_SUNXI_VERSION1
struct sunxi_ccm_reg {
	u32 pll1_c0_cfg; /* 0x00 c1cpu# pll control */
	u32 pll1_c1_cfg; /* 0x04 c1cpu# pll control */
	u32 pll2_cfg; /* 0x08 pll2 audio control */
	u32 reserved1;
	u32 pll3_cfg; /* 0x10 pll3 video0 control */
	u32 reserved2;
	u32 pll4_cfg; /* 0x18 pll4 ve control */
	u32 reserved3;
	u32 pll5_cfg; /* 0x20 pll5 ddr control */
	u32 reserved4;
	u32 pll6_cfg; /* 0x28 pll6 peripheral control */
	u32 reserved5[3]; /* 0x2c */
	u32 pll7_cfg; /* 0x38 pll7 gpu control */
	u32 reserved6[2]; /* 0x3c */
	u32 pll8_cfg; /* 0x44 pll8 hsic control */
	u32 pll9_cfg; /* 0x48 pll9 de control */
	u32 pll10_cfg; /* 0x4c pll10 video1 control */
	u32 cpu_axi_cfg; /* 0x50 CPU/AXI divide ratio */
	u32 ahb1_apb1_div; /* 0x54 AHB1/APB1 divide ratio */
	u32 apb2_div; /* 0x58 APB2 divide ratio */
	u32 ahb2_div; /* 0x5c AHB2 divide ratio */
	u32 ahb_gate0; /* 0x60 ahb module clock gating 0 */
	u32 ahb_gate1; /* 0x64 ahb module clock gating 1 */
	u32 apb1_gate; /* 0x68 apb1 module clock gating 3 */
	u32 apb2_gate; /* 0x6c apb2 module clock gating 4 */
	u32 reserved7[2]; /* 0x70 */
	u32 cci400_cfg; /* 0x78 cci400 clock configuration A83T only */
	u32 reserved8; /* 0x7c */
	u32 nand0_clk_cfg; /* 0x80 nand clock control */
	u32 reserved9; /* 0x84 */
	u32 sd0_clk_cfg; /* 0x88 sd0 clock control */
	u32 sd1_clk_cfg; /* 0x8c sd1 clock control */
	u32 sd2_clk_cfg; /* 0x90 sd2 clock control */
	u32 sd3_clk_cfg; /* 0x94 sd3 clock control */
	u32 reserved10; /* 0x98 */
	u32 ss_clk_cfg; /* 0x9c security system clock control */
	u32 spi0_clk_cfg; /* 0xa0 spi0 clock control */
	u32 spi1_clk_cfg; /* 0xa4 spi1 clock control */
	u32 reserved11[2]; /* 0xa8 */
	u32 i2s0_clk_cfg; /* 0xb0 I2S0 clock control */
	u32 i2s1_clk_cfg; /* 0xb4 I2S1 clock control */
	u32 i2s2_clk_cfg; /* 0xb8 I2S2 clock control */
	u32 tdm_clk_cfg; /* 0xbc TDM clock control */
	u32 spdif_clk_cfg; /* 0xc0 SPDIF clock control */
	u32 reserved12[2]; /* 0xc4 */
	u32 usb_clk_cfg; /* 0xcc USB clock control */
	u32 reserved13[9]; /* 0xd0 */
	u32 dram_clk_cfg; /* 0xf4 DRAM configuration clock control */
	u32 dram_pll_cfg; /* 0xf8 PLL_DDR cfg register */
	u32 mbus_reset; /* 0xfc MBUS reset control */
	u32 dram_clk_gate; /* 0x100 DRAM module gating */
	u32 reserved14[5]; /* 0x104 BE0 */
	u32 lcd0_clk_cfg; /* 0x118 LCD0 module clock */
	u32 lcd1_clk_cfg; /* 0x11c LCD1 module clock */
	u32 reserved15[4]; /* 0x120 */
	u32 mipi_csi_clk_cfg; /* 0x130 MIPI CSI module clock */
	u32 csi_clk_cfg; /* 0x134 CSI module clock */
	u32 reserved16; /* 0x138 */
	u32 ve_clk_cfg; /* 0x13c VE module clock */
	u32 reserved17; /* 0x140 */
	u32 avs_clk_cfg; /* 0x144 AVS module clock */
	u32 reserved18[2]; /* 0x148 */
	u32 hdmi_clk_cfg; /* 0x150 HDMI module clock */
	u32 hdmi_slow_clk_cfg; /* 0x154 HDMI slow module clock */
	u32 reserved19; /* 0x158 */
	u32 mbus_clk_cfg; /* 0x15c MBUS module clock */
	u32 reserved20[2]; /* 0x160 */
	u32 mipi_dsi_clk_cfg; /* 0x168 MIPI DSI clock control */
	u32 reserved21[13]; /* 0x16c */
	u32 gpu_core_clk_cfg; /* 0x1a0 GPU core clock config */
	u32 gpu_mem_clk_cfg; /* 0x1a4 GPU memory clock config */
	u32 gpu_hyd_clk_cfg; /* 0x1a8 GPU HYD clock config */
	u32 reserved22[21]; /* 0x1ac */
	u32 pll_stable0; /* 0x200 PLL stable time 0 */
	u32 pll_stable1; /* 0x204 PLL stable time 1 */
	u32 reserved23; /* 0x208 */
	u32 pll_stable_status; /* 0x20c PLL stable status register */
	u32 reserved24[4]; /* 0x210 */
	u32 pll1_c0_bias_cfg; /* 0x220 PLL1 c0cpu# Bias config */
	u32 pll2_bias_cfg; /* 0x224 PLL2 audio Bias config */
	u32 pll3_bias_cfg; /* 0x228 PLL3 video Bias config */
	u32 pll4_bias_cfg; /* 0x22c PLL4 ve Bias config */
	u32 pll5_bias_cfg; /* 0x230 PLL5 ddr Bias config */
	u32 pll6_bias_cfg; /* 0x234 PLL6 periph Bias config */
	u32 pll1_c1_bias_cfg; /* 0x238 PLL1 c1cpu# Bias config */
	u32 pll8_bias_cfg; /* 0x23c PLL7 Bias config */
	u32 reserved25; /* 0x240 */
	u32 pll9_bias_cfg; /* 0x244 PLL9 hsic Bias config */
	u32 de_bias_cfg; /* 0x248 display engine Bias config */
	u32 video1_bias_cfg; /* 0x24c pll video1 bias register */
	u32 c0_tuning_cfg; /* 0x250 pll c0cpu# tuning register */
	u32 c1_tuning_cfg; /* 0x254 pll c1cpu# tuning register */
	u32 reserved26[11]; /* 0x258 */
	u32 pll2_pattern_cfg0; /* 0x284 PLL2 Pattern register 0 */
	u32 pll3_pattern_cfg0; /* 0x288 PLL3 Pattern register 0 */
	u32 reserved27; /* 0x28c */
	u32 pll5_pattern_cfg0; /* 0x290 PLL5 Pattern register 0*/
	u32 reserved28[4]; /* 0x294 */
	u32 pll2_pattern_cfg1; /* 0x2a4 PLL2 Pattern register 1 */
	u32 pll3_pattern_cfg1; /* 0x2a8 PLL3 Pattern register 1 */
	u32 reserved29; /* 0x2ac */
	u32 pll5_pattern_cfg1; /* 0x2b0 PLL5 Pattern register 1 */
	u32 reserved30[3]; /* 0x2b4 */
	u32 ahb_reset0_cfg; /* 0x2c0 AHB1 Reset 0 config */
	u32 ahb_reset1_cfg; /* 0x2c4 AHB1 Reset 1 config */
	u32 ahb_reset2_cfg; /* 0x2c8 AHB1 Reset 2 config */
	u32 reserved31;
	u32 ahb_reset3_cfg; /* 0x2d0 AHB1 Reset 3 config */
	u32 reserved32; /* 0x2d4 */
	u32 apb2_reset_cfg; /* 0x2d8 BUS Reset 4 config */
};

#else
struct sunxi_ccm_reg {
	u32 pll_cpux_ctrl;      /*0x0*/
	u8 reserved_0x4[12];
	u32 pll_ddr0_ctrl;      /*0x10*/
	u8 reserved_0x14[4];
	u32 pll_ddr1_ctrl;      /*0x18*/
	u8 reserved_0x1C[4];
	u32 pll_peri0_ctrl;     /*0x20*/
	u8 reserved_0x24[4];
	u32 pll_peri1_ctrl;     /*0x28*/
	u8 reserved_0x2C[68];
	u32 pll_hsic_ctrl;      /*0x70*/
	u8 reserved_0x74[1164];
	u32 cpux_axi_cfg;       /*0x500*/
	u8 reserved_0x504[12];
	u32 psi_ahb1_ahb2_cfg;  /*0x510*/
	u8 reserved_0x514[8];
	u32 ahb3_cfg;           /*0x51c*/
	u32 apb1_cfg;           /*0x520*/
	u32 apb2_cfg;           /*0x524*/
	u8 reserved_0x528[24];
	u32 mbus_cfg;           /*0x540*/
	u8 reserved_0x544[316];
	u32 ce_clk;             /*0x680*/
	u8 reserved_0x684[8];
	u32 ce_bgr;             /*0x68c*/
	u32 ve_clk;             /*0x690*/
	u8 reserved_0x694[8];
	u32 ve_bgr;             /*0x69c*/
	u8 reserved_0x6A0[108];
	u32 dma_gate_reset;     /*0x70c*/
	u8 reserved_0x710[48];
	u32 avs_clk;            /*0x740*/
	u8 reserved_0x744[8];
	u32 avs_bgr;            /*0x74c*/
	u8 reserved_0x750[108];
	u32 iommu_bgr;          /*0x7bc*/
	u8 reserved_0x7C0[64];
	u32 dram_clk;           /*0x800*/
	u32 mbus_gate;/*0x804*/
	u32 pll_ddr_aux;        /*0x808*/
	u32 dram_bgr;           /*0x80c*/
	u32 nand_clk;           /*0x810*/
	u8 reserved_0x814[24];
	u32 nand_bgr;           /*0x82c*/
	u32 sdmmc0_clk;         /*0x830*/
	u32 sdmmc1_clk;         /*0x834*/
	u32 sdmmc2_clk;         /*0x838*/
	u8 reserved_0x83C[16];
	u32 smhc_bgr;           /*0x84c*/
	u8 reserved_0x850[188];
	u32 uart_bgr;           /*0x90c*/
	u8 reserved_0x910[12];
	u32 twi_bgr;            /*0x91c*/
	u8 reserved_0x920[28];
	u32 scr_bgr;            /*0x93c*/
	u32 spi0_clk_cfg;       /*0x940*/
	u32 spi1_clk_cfg;       /*0x944*/
	u8 reserved_0x948[36];
	u32 spi_gate_reset;     /*0x96c*/
	u8 reserved_0x970[256];
	u32 usb0_clk;           /*0xa70*/
	u8 reserved_0xA74[24];
	u32 usb_bgr;            /*0xa8c*/
};
#endif

/* Module gate/reset shift*/
#define RESET_SHIFT                     (16)
#define GATING_SHIFT                    (0)


#ifndef __ASSEMBLY__
void sunxi_board_pll_init(void);
void sunxi_board_clock_reset(void);
void sunxi_clock_init_uart(int port);
#endif
/*key clock*/
int sunxi_clock_init_key(void);
int sunxi_clock_exit_key(void);

#endif /* _SUNXI_CLOCK_H */
