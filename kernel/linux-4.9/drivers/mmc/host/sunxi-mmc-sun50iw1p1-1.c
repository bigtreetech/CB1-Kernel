/*
* Sunxi SD/MMC host driver
*
* Copyright (C) 2015 AllWinnertech Ltd.
* Author: lixiang <lixiang@allwinnertech>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed "as is" WITHOUT ANY WARRANTY of any
* kind, whether express or implied; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/


#ifdef CONFIG_ARCH_SUN50IW1P1

#include <linux/clk.h>
#include <linux/clk/sunxi.h>

#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/reset.h>

#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>

#include <linux/mmc/host.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
#include <linux/mmc/slot-gpio.h>

#include "sunxi-mmc.h"
#include "sunxi-mmc-sun50iw1p1-1.h"

/*reg*/
/*SMHC eMMC4.5 DDR Start Bit Detection Control Register */
/*SMHC CRC Status Detect Control Register */
/*SMHC Card Threshold Control Register */
/*SMHC Drive Delay Control Register */
/*SMHC Sample Delay Control Register */
/*SMHC Data Strobe Delay Control Register */
/*SMHC NewTiming Set Register */
#define SDXC_REG_EDSD		(0x010C)
#define SDXC_REG_CSDC		(0x0054)
#define SDXC_REG_THLD		(0x0100)
#define SDXC_REG_DRV_DL		(0x0140)
#define SDXC_REG_SAMP_DL	(0x0144)
#define SDXC_REG_DS_DL		(0x0148)
#define SDXC_REG_SD_NTSR	(0x005C)

/*bit*/
#define SDXC_HS400_MD_EN				(1U<<31)
#define SDXC_CARD_WR_THLD_ENB		(1U<<2)
#define SDXC_CARD_RD_THLD_ENB		(1U)

#define SDXC_DAT_DRV_PH_SEL			(1U<<17)
#define SDXC_CMD_DRV_PH_SEL			(1U<<16)
#define SDXC_SAMP_DL_SW_EN			(1u<<7)
#define SDXC_DS_DL_SW_EN			(1u<<7)

#define	SDXC_2X_TIMING_MODE			(1U<<31)

/*mask*/
#define SDXC_CRC_DET_PARA_MASK		(0xf)
#define SDXC_CARD_RD_THLD_MASK		(0x0FFF0000)
#define SDXC_TX_TL_MASK				(0xff)
#define SDXC_RX_TL_MASK				(0x00FF0000)

#define SDXC_SAMP_DL_SW_MASK		(0x0000003F)
#define SDXC_DS_DL_SW_MASK			(0x0000003F)

#define SDXC_SAM_TIMING_PH_MASK		(0x00000030)

/*value*/
#define SDXC_CRC_DET_PARA_HS400		(6)
#define SDXC_CRC_DET_PARA_OTHER		(3)
#define SDXC_FIFO_DETH					(1024>>2)

/*size*/
#define SDXC_CARD_RD_THLD_SIZE		(0x00000FFF)

/*shit*/
#define SDXC_CARD_RD_THLD_SIZE_SHIFT		(16)

#define SDXC_SAM_TIMING_PH_SHIFT			(4)

enum sunxi_mmc_clk_mode {
	mmc_clk_400k = 0,
	mmc_clk_26M,
	mmc_clk_52M,
	mmc_clk_52M_DDR4,
	mmc_clk_52M_DDR8,
	mmc_clk_104M,
	mmc_clk_208M,
	mmc_clk_104M_DDR,
	mmc_clk_208M_DDR,
	mmc_clk_mod_num,
};

struct sunxi_mmc_clk_dly {
	enum sunxi_mmc_clk_mode cmod;
	char *mod_str;
	u32 cmd_drv_ph;
	u32 dat_drv_ph;
	u32 sam_dly;
	u32 ds_dly;
	u32 sam_ph;
};

	/*sample delay and output deley setting */
static struct sunxi_mmc_clk_dly mmc_clk_dly[mmc_clk_mod_num] = {
	[mmc_clk_400k] = {
			  .cmod = mmc_clk_400k,
			  .mod_str = "sunxi-dly-400k",
			  .cmd_drv_ph = 1,
			  .dat_drv_ph = 0,
			  .sam_dly = 0,
			  .ds_dly = 0,
			  .sam_ph = 0,
			  },
	[mmc_clk_26M] = {
			 .cmod = mmc_clk_26M,
			 .mod_str = "sunxi-dly-26M",
			 .cmd_drv_ph = 1,
			 .dat_drv_ph = 0,
			 .sam_dly = 0,
			 .ds_dly = 0,
			 .sam_ph = 0,
			 },
	[mmc_clk_52M] = {
			 .cmod = mmc_clk_52M,
			 .mod_str = "sunxi-dly-52M",
			 .cmd_drv_ph = 1,
			 .dat_drv_ph = 0,
			 .sam_dly = 0,
			 .ds_dly = 0,
			 .sam_ph = 0,
			 },
	[mmc_clk_52M_DDR4] = {
			      .cmod = mmc_clk_52M_DDR4,
			      .mod_str = "sunxi-dly-52M-ddr4",
			      .cmd_drv_ph = 1,
			      .dat_drv_ph = 0,
			      .sam_dly = 0,
			      .ds_dly = 0,
			      .sam_ph = 0,
			      },
	[mmc_clk_52M_DDR8] = {
			      .cmod = mmc_clk_52M_DDR8,
			      .mod_str = "sunxi-dly-52M-ddr8",
			      .cmd_drv_ph = 1,
			      .dat_drv_ph = 0,
			      .sam_dly = 0,
			      .ds_dly = 0,
			      .sam_ph = 0,
			      },
	[mmc_clk_104M] = {
			  .cmod = mmc_clk_104M,
			  .mod_str = "sunxi-dly-104M",
			  .cmd_drv_ph = 1,
			  .dat_drv_ph = 0,
			  .sam_dly = 0,
			  .ds_dly = 0,
			  .sam_ph = 0,

			  },
	[mmc_clk_208M] = {
			  .cmod = mmc_clk_208M,
			  .mod_str = "sunxi-dly-208M",
			  .cmd_drv_ph = 1,
			  .dat_drv_ph = 0,
			  .sam_dly = 0,
			  .ds_dly = 0,
			  .sam_ph = 0,
			  },
	[mmc_clk_104M_DDR] = {
			      .cmod = mmc_clk_104M_DDR,
			      .mod_str = "sunxi-dly-104M-ddr",
			      .cmd_drv_ph = 1,
			      .dat_drv_ph = 0,
			      .sam_dly = 0,
			      .ds_dly = 0,
			      .sam_ph = 0,
			      },
	[mmc_clk_208M_DDR] = {
			      .cmod = mmc_clk_208M_DDR,
			      .mod_str = "sunxi-dly-208M-ddr",
			      .cmd_drv_ph = 1,
			      .dat_drv_ph = 0,
			      .sam_dly = 0,
			      .ds_dly = 0,
			      .sam_ph = 0,
			      },
};

struct sunxi_mmc_spec_regs {
	u32 drv_dl;		/*REG_DRV_DL */
	u32 samp_dl;		/*REG_SAMP_DL */
	u32 ds_dl;		/*REG_DS_DL */
	u32 sd_ntsr;		/*REG_SD_NTSR */
};

static struct sunxi_mmc_spec_regs bak_spec_regs;

static void sunxi_mmc_set_clk_dly(struct sunxi_mmc_host *host, int clk,
				  int bus_width, int timing)
{
	struct mmc_host *mhost = host->mmc;
	u32 rval = 0;
	enum sunxi_mmc_clk_mode cmod = mmc_clk_400k;
	u32 in_clk_dly[5] = { 0 };
	int ret = 0;
	struct device_node *np = NULL;

	if (!mhost->parent || !mhost->parent->of_node) {
		dev_err(mmc_dev(host->mmc),
			"no dts to parse clk dly,use default\n");
		return;
	}

	np = mhost->parent->of_node;

	if (clk <= 400 * 1000) {
		cmod = mmc_clk_400k;
	} else if (clk <= 26 * 1000 * 1000) {
		cmod = mmc_clk_26M;
	} else if (clk <= 52 * 1000 * 1000) {
		if ((bus_width == MMC_BUS_WIDTH_4)
		    && sunxi_mmc_ddr_timing(timing)) {
			cmod = mmc_clk_52M_DDR4;
		} else if ((bus_width == MMC_BUS_WIDTH_8)
			   && (timing == MMC_TIMING_MMC_DDR52)) {
			cmod = mmc_clk_52M_DDR8;
		} else {
			cmod = mmc_clk_52M;
		}
	} else if (clk <= 104 * 1000 * 1000) {
		if ((bus_width == MMC_BUS_WIDTH_8)
		    && (timing == MMC_TIMING_MMC_HS400)) {
			cmod = mmc_clk_104M_DDR;
		} else {
			cmod = mmc_clk_104M;
		}
	} else if (clk <= 208 * 1000 * 1000) {
		if ((bus_width == MMC_BUS_WIDTH_8)
		    && (timing == MMC_TIMING_MMC_HS400)) {
			cmod = mmc_clk_208M_DDR;
		} else {
			cmod = mmc_clk_208M;
		}
	} else {
		dev_err(mmc_dev(mhost), "clk %d is out of range\n", clk);
		return;
	}

	ret = of_property_read_u32_array(np, mmc_clk_dly[cmod].mod_str,
					 in_clk_dly, ARRAY_SIZE(in_clk_dly));
	if (ret) {
		dev_dbg(mmc_dev(host->mmc), "failded to get %s used default\n",
			mmc_clk_dly[cmod].mod_str);
	} else {
		mmc_clk_dly[cmod].cmd_drv_ph = in_clk_dly[0];
		mmc_clk_dly[cmod].dat_drv_ph = in_clk_dly[1];
		/*mmc_clk_dly[cmod].sam_dly             = in_clk_dly[2]; */
		/*mmc_clk_dly[cmod].ds_dly              = in_clk_dly[3]; */
		mmc_clk_dly[cmod].sam_ph = in_clk_dly[4];
		dev_dbg(mmc_dev(host->mmc), "Get %s clk dly ok\n",
			mmc_clk_dly[cmod].mod_str);

	}

	dev_dbg(mmc_dev(host->mmc), "Try set %s clk dly       ok\n",
		mmc_clk_dly[cmod].mod_str);
	dev_dbg(mmc_dev(host->mmc), "cmd_drv_ph	%d\n",
		mmc_clk_dly[cmod].cmd_drv_ph);
	dev_dbg(mmc_dev(host->mmc), "dat_drv_ph	%d\n",
		mmc_clk_dly[cmod].dat_drv_ph);
	dev_dbg(mmc_dev(host->mmc), "sam_ph	%d\n",
		mmc_clk_dly[cmod].sam_ph);

	rval = mmc_readl(host, REG_DRV_DL);
	if (mmc_clk_dly[cmod].cmd_drv_ph)
		rval |= SDXC_CMD_DRV_PH_SEL;	/*180 phase */
	else
		rval &= ~SDXC_CMD_DRV_PH_SEL;	/*90 phase */


	if (mmc_clk_dly[cmod].dat_drv_ph)
		rval |= SDXC_DAT_DRV_PH_SEL;	/*180 phase */
	 else
		rval &= ~SDXC_DAT_DRV_PH_SEL;	/*90 phase */

	mmc_writel(host, REG_DRV_DL, rval);

/*
*	rval = mmc_readl(host,REG_SAMP_DL);
*	rval &= ~SDXC_SAMP_DL_SW_MASK;
*	rval |= mmc_clk_dly[cmod].sam_dly & SDXC_SAMP_DL_SW_MASK;
*	rval |= SDXC_SAMP_DL_SW_EN;
*	mmc_writel(host,REG_SAMP_DL,rval);
*
*	rval = mmc_readl(host,REG_DS_DL);
*	rval &= ~SDXC_DS_DL_SW_MASK;
*	rval |= mmc_clk_dly[cmod].ds_dly & SDXC_DS_DL_SW_MASK;
*	rval |= SDXC_DS_DL_SW_EN;
*	mmc_writel(host,REG_DS_DL,rval);
*/
	rval = mmc_readl(host, REG_SD_NTSR);
	rval &= ~SDXC_SAM_TIMING_PH_MASK;
	rval |=
	    (mmc_clk_dly[cmod].
	     sam_ph << SDXC_SAM_TIMING_PH_SHIFT) & SDXC_SAM_TIMING_PH_MASK;
	mmc_writel(host, REG_SD_NTSR, rval);

}

static int __sunxi_mmc_do_oclk_onoff(struct sunxi_mmc_host *host, u32 oclk_en,
				     u32 pwr_save, u32 ignore_dat0)
{
	unsigned long expire = jiffies + msecs_to_jiffies(250);
	u32 rval;

	rval = mmc_readl(host, REG_CLKCR);
	rval &= ~(SDXC_CARD_CLOCK_ON | SDXC_LOW_POWER_ON | SDXC_MASK_DATA0);

	if (oclk_en)
		rval |= SDXC_CARD_CLOCK_ON;
	if (pwr_save)
		rval |= SDXC_LOW_POWER_ON;
	if (ignore_dat0)
		rval |= SDXC_MASK_DATA0;

	mmc_writel(host, REG_CLKCR, rval);

	dev_dbg(mmc_dev(host->mmc), "%s REG_CLKCR:%x\n", __func__,
		mmc_readl(host, REG_CLKCR));

	rval = SDXC_START | SDXC_UPCLK_ONLY | SDXC_WAIT_PRE_OVER;
	mmc_writel(host, REG_CMDR, rval);

	do {
		rval = mmc_readl(host, REG_CMDR);
	} while (time_before(jiffies, expire) && (rval & SDXC_START));

	/* clear irq status bits set by the command */
	/*? */
	mmc_writel(host, REG_RINTR,
			mmc_readl(host, REG_RINTR) & ~SDXC_SDIO_INTERRUPT);

	if (rval & SDXC_START) {
		dev_err(mmc_dev(host->mmc), "fatal err update clk timeout\n");
		return -EIO;
	}

	/*only use mask data0 when update clk,clear it when not update clk */
	if (ignore_dat0)
		mmc_writel(host, REG_CLKCR,
			   mmc_readl(host, REG_CLKCR) & ~SDXC_MASK_DATA0);

	return 0;
}

static int sunxi_mmc_oclk_onoff(struct sunxi_mmc_host *host, u32 oclk_en)
{
	struct device_node *np = NULL;
	struct mmc_host *mmc = host->mmc;
	int pwr_save = 0;
	int len = 0;

	if (!mmc->parent || !mmc->parent->of_node) {
		dev_err(mmc_dev(host->mmc),
			"no dts to parse power save mode\n");
		return -EIO;
	}

	np = mmc->parent->of_node;
	if (of_find_property(np, "sunxi-power-save-mode", &len))
		pwr_save = 1;
	return __sunxi_mmc_do_oclk_onoff(host, oclk_en, pwr_save, 1);
}

int sunxi_mmc_oclk_onoff_sdmmc1(struct sunxi_mmc_host *host, u32 oclk_en)
{
	return sunxi_mmc_oclk_onoff(host, oclk_en);
}

static void sunxi_mmc_2xmod_onoff(struct sunxi_mmc_host *host, u32 newmode_en)
{
	u32 rval = mmc_readl(host, REG_SD_NTSR);

	if (newmode_en)
		rval |= SDXC_2X_TIMING_MODE;
	else
		rval &= ~SDXC_2X_TIMING_MODE;

	mmc_writel(host, REG_SD_NTSR, rval);
}

int sunxi_mmc_clk_set_rate_for_sdmmc1(struct sunxi_mmc_host *host,
				      struct mmc_ios *ios)
{
	u32 mod_clk = 0;
	u32 src_clk = 0;
	u32 rval = 0;
	s32 err = 0;
	u32 rate = 0;
	char *sclk_name = NULL;
	struct clk *mclk = host->clk_mmc;
	struct clk *sclk = NULL;
	struct device *dev = mmc_dev(host->mmc);
	int div = 0;

	if (ios->clock == 0) {
		__sunxi_mmc_do_oclk_onoff(host, 0, 0, 1);
		return 0;
	}

	if (sunxi_mmc_ddr_timing(ios->timing)) {
		mod_clk = ios->clock << 2;
		div = 1;
	} else {
		mod_clk = ios->clock << 1;
		div = 0;
	}

	if (ios->clock <= 400000) {
		sclk = clk_get(dev, "osc24m");
		sclk_name = "osc24m";
	} else {
		sclk = clk_get(dev, "pll_periph");
		sclk_name = "pll_periph";
	}

	if (IS_ERR(sclk)) {
		dev_err(mmc_dev(host->mmc), "Error to get source clock %s\n",
			sclk_name);
		return -1;
	}

	sunxi_mmc_oclk_onoff(host, 0);

	err = clk_set_parent(mclk, sclk);
	if (err) {
		dev_err(mmc_dev(host->mmc), "set parent failed\n");
		clk_put(sclk);
		return -1;
	}

	rate = clk_round_rate(mclk, mod_clk);

	dev_dbg(mmc_dev(host->mmc), "get round rate %d\n", rate);

	clk_disable_unprepare(host->clk_mmc);

	err = clk_set_rate(mclk, rate);
	if (err) {
		dev_err(mmc_dev(host->mmc), "set mclk rate error, rate %dHz\n",
			rate);
		clk_put(sclk);
		return -1;
	}
	
	printk("==> %s: clk rate = %dHZ\n",__func__,rate);

	rval = clk_prepare_enable(host->clk_mmc);
	if (rval) {
		dev_err(mmc_dev(host->mmc), "Enable mmc clk err %d\n", rval);
		return -1;
	}

	src_clk = clk_get_rate(sclk);
	clk_put(sclk);

	dev_dbg(mmc_dev(host->mmc), "set round clock %d, soure clk is %d\n",
		rate, src_clk);

#ifdef MMC_FPGA
	if (sunxi_mmc_ddr_timing(ios->timing)) {
		/* clear internal divider */
		rval = mmc_readl(host, REG_CLKCR);
		rval &= ~0xff;
		rval |= 1;
	} else {
		/* support internal divide clock under fpga environment  */
		rval = mmc_readl(host, REG_CLKCR);
		rval &= ~0xff;
		rval |= 24000000 / mod_clk / 2;	/* =24M/400K/2=0x1E*/
	}
	mmc_writel(host, REG_CLKCR, rval);
	dev_info(mmc_dev(host->mmc), "FPGA REG_CLKCR: 0x%08x\n",
		mmc_readl(host, REG_CLKCR));
#else
	/* clear internal divider */
	rval = mmc_readl(host, REG_CLKCR);
	rval &= ~0xff;
	rval |= div;
	mmc_writel(host, REG_CLKCR, rval);
#endif

	/*sunxi_of_parse_clk_dly(host);*/
	sunxi_mmc_2xmod_onoff(host, 1);

	if (sunxi_mmc_ddr_timing(ios->timing))
		ios->clock = rate >> 2;
	else
		ios->clock = rate >> 1;


	sunxi_mmc_set_clk_dly(host, ios->clock, ios->bus_width, ios->timing);

	return sunxi_mmc_oclk_onoff(host, 1);
}

void sunxi_mmc_thld_ctl_for_sdmmc1(struct sunxi_mmc_host *host,
				   struct mmc_ios *ios, struct mmc_data *data)
{
	u32 bsz = data->blksz;
	/*unit:byte */
	u32 rdtl = ((host->dma_tl & SDXC_RX_TL_MASK) >> 16) << 2;
	u32 rval = 0;

	if ((data->flags & MMC_DATA_READ)
	    && (bsz <= SDXC_CARD_RD_THLD_SIZE)
	    /*((SDXC_FIFO_DETH<<2)-bsz) >= (rdtl)*/
	    && ((SDXC_FIFO_DETH << 2) >= (rdtl + bsz))
	    && ((ios->timing == MMC_TIMING_MMC_HS200)
		|| (ios->timing == MMC_TIMING_UHS_SDR50)
		|| (ios->timing == MMC_TIMING_UHS_SDR104))) {
		rval = mmc_readl(host, REG_THLD);
		rval &= ~SDXC_CARD_RD_THLD_MASK;
		rval |= data->blksz << SDXC_CARD_RD_THLD_SIZE_SHIFT;
		rval |= SDXC_CARD_RD_THLD_ENB;
		mmc_writel(host, REG_THLD, rval);
	} else {
		rval = mmc_readl(host, REG_THLD);
		rval &= ~SDXC_CARD_RD_THLD_ENB;
		mmc_writel(host, REG_THLD, rval);
	}

	dev_dbg(mmc_dev(host->mmc), "SDXC_REG_THLD: 0x%08x\n",
		mmc_readl(host, REG_THLD));

}

void sunxi_mmc_save_spec_reg1(struct sunxi_mmc_host *host)
{
	bak_spec_regs.drv_dl = mmc_readl(host, REG_DRV_DL);
	bak_spec_regs.samp_dl = mmc_readl(host, REG_SAMP_DL);
	bak_spec_regs.ds_dl = mmc_readl(host, REG_DS_DL);
	bak_spec_regs.sd_ntsr = mmc_readl(host, REG_SD_NTSR);
}

void sunxi_mmc_restore_spec_reg1(struct sunxi_mmc_host *host)
{
	mmc_writel(host, REG_DRV_DL, bak_spec_regs.drv_dl);
	mmc_writel(host, REG_SAMP_DL, bak_spec_regs.samp_dl);
	mmc_writel(host, REG_DS_DL, bak_spec_regs.ds_dl);
	mmc_writel(host, REG_SD_NTSR, bak_spec_regs.sd_ntsr);
}

#endif
