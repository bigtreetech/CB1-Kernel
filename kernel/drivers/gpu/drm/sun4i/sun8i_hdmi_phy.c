// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2018 Jernej Skrabec <jernej.skrabec@siol.net>
 */

#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>

#include "sun8i_dw_hdmi.h"

/*
 * Address can be actually any value. Here is set to same value as
 * it is set in BSP driver.
 */
#define I2C_ADDR 0x69

static const struct dw_hdmi_mpll_config sun50i_h616_mpll_cfg[] = {
    {
        27000000,
        {
            {0x00b3, 0x0003},
            {0x2153, 0x0003},
            {0x40f3, 0x0003},
        },
    },
    {
        74250000,
        {
            {0x0072, 0x0003},
            {0x2145, 0x0003},
            {0x4061, 0x0003},
        },
    },
    {
        148500000,
        {
            {0x0051, 0x0003},
            {0x214c, 0x0003},
            {0x4064, 0x0003},
        },
    },
    {
        297000000,
        {
            {0x0040, 0x0003},
            {0x3b4c, 0x0003},
            {0x5a64, 0x0003},
        },
    },
    {
        594000000,
        {
            {0x1a40, 0x0003},
            {0x3b4c, 0x0003},
            {0x5a64, 0x0003},
        },
    },
    {
        ~0UL,
        {
            {0x0000, 0x0000},
            {0x0000, 0x0000},
            {0x0000, 0x0000},
        },
    }};

static const struct dw_hdmi_curr_ctrl sun50i_h616_cur_ctr[] = {
    /* pixelclk    bpp8    bpp10   bpp12 */
    {
        27000000,
        {0x0012, 0x0000, 0x0000},
    },
    {
        74250000,
        {0x0013, 0x0013, 0x0013},
    },
    {
        148500000,
        {0x0019, 0x0019, 0x0019},
    },
    {
        297000000,
        {0x0019, 0x001b, 0x0019},
    },
    {
        594000000,
        {0x0010, 0x0010, 0x0010},
    },
    {
        ~0UL,
        {0x0000, 0x0000, 0x0000},
    }};

static const struct dw_hdmi_phy_config sun50i_h616_phy_config[] = {
    /*pixelclk   symbol   term   vlev*/
    {27000000, 0x8009, 0x0007, 0x02b0},
    {74250000, 0x8019, 0x0004, 0x0290},
    {148500000, 0x8019, 0x0004, 0x0290},
    {297000000, 0x8039, 0x0004, 0x022b},
    {594000000, 0x8029, 0x0000, 0x008a},
    {~0UL, 0x0000, 0x0000, 0x0000}};

static int sun8i_hdmi_phy_config(struct dw_hdmi *hdmi, void *data,
                                 const struct drm_display_info *display,
                                 const struct drm_display_mode *mode)
{
    struct sun8i_hdmi_phy *phy = (struct sun8i_hdmi_phy *)data;
    u32 val = 0;

    if (mode->flags & DRM_MODE_FLAG_NHSYNC)
        val |= SUN8I_HDMI_PHY_DBG_CTRL_POL_NHSYNC;

    if (mode->flags & DRM_MODE_FLAG_NVSYNC)
        val |= SUN8I_HDMI_PHY_DBG_CTRL_POL_NVSYNC;

    regmap_update_bits(phy->regs, SUN8I_HDMI_PHY_DBG_CTRL_REG,
                       SUN8I_HDMI_PHY_DBG_CTRL_POL_MASK, val);

    if (phy->variant->has_phy_clk)
        clk_set_rate(phy->clk_phy, mode->crtc_clock * 1000);

    return phy->variant->phy_config(hdmi, phy, mode->crtc_clock * 1000);
};

static void sun8i_hdmi_phy_disable(struct dw_hdmi *hdmi, void *data)
{
    struct sun8i_hdmi_phy *phy = (struct sun8i_hdmi_phy *)data;

    phy->variant->phy_disable(hdmi, phy);
}

static const struct dw_hdmi_phy_ops sun8i_hdmi_phy_ops = {
    .init = &sun8i_hdmi_phy_config,
    .disable = &sun8i_hdmi_phy_disable,
    .read_hpd = &dw_hdmi_phy_read_hpd,
    .update_hpd = &dw_hdmi_phy_update_hpd,
    .setup_hpd = &dw_hdmi_phy_setup_hpd,
};

static void sun8i_hdmi_phy_unlock(struct sun8i_hdmi_phy *phy)
{
    /* enable read access to HDMI controller */
    regmap_write(phy->regs, SUN8I_HDMI_PHY_READ_EN_REG,
                 SUN8I_HDMI_PHY_READ_EN_MAGIC);

    /* unscramble register offsets */
    regmap_write(phy->regs, SUN8I_HDMI_PHY_UNSCRAMBLE_REG,
                 SUN8I_HDMI_PHY_UNSCRAMBLE_MAGIC);
}

static void sun50i_hdmi_phy_init_h616(struct sun8i_hdmi_phy *phy)
{
    regmap_update_bits(phy->regs, SUN8I_HDMI_PHY_REXT_CTRL_REG,
                       SUN8I_HDMI_PHY_REXT_CTRL_REXT_EN,
                       SUN8I_HDMI_PHY_REXT_CTRL_REXT_EN);

    regmap_update_bits(phy->regs, SUN8I_HDMI_PHY_REXT_CTRL_REG,
                       0xffff0000, 0x80c00000);
}

int sun8i_hdmi_phy_init(struct sun8i_hdmi_phy *phy)
{
    int ret;

    ret = reset_control_deassert(phy->rst_phy);
    if (ret)
    {
        dev_err(phy->dev, "Cannot deassert phy reset control: %d\n", ret);
        return ret;
    }

    ret = clk_prepare_enable(phy->clk_bus);
    if (ret)
    {
        dev_err(phy->dev, "Cannot enable bus clock: %d\n", ret);
        goto err_assert_rst_phy;
    }

    ret = clk_prepare_enable(phy->clk_mod);
    if (ret)
    {
        dev_err(phy->dev, "Cannot enable mod clock: %d\n", ret);
        goto err_disable_clk_bus;
    }

    if (phy->variant->has_phy_clk)
    {
        ret = sun8i_phy_clk_create(phy, phy->dev, phy->variant->has_second_pll);
        if (ret)
        {
            dev_err(phy->dev, "Couldn't create the PHY clock\n");
            goto err_disable_clk_mod;
        }

        clk_prepare_enable(phy->clk_phy);
    }

    phy->variant->phy_init(phy);

    return 0;

err_disable_clk_mod:
    clk_disable_unprepare(phy->clk_mod);
err_disable_clk_bus:
    clk_disable_unprepare(phy->clk_bus);
err_assert_rst_phy:
    reset_control_assert(phy->rst_phy);

    return ret;
}

void sun8i_hdmi_phy_deinit(struct sun8i_hdmi_phy *phy)
{
    clk_disable_unprepare(phy->clk_mod);
    clk_disable_unprepare(phy->clk_bus);
    clk_disable_unprepare(phy->clk_phy);

    reset_control_assert(phy->rst_phy);
}

void sun8i_hdmi_phy_set_ops(struct sun8i_hdmi_phy *phy,
                            struct dw_hdmi_plat_data *plat_data)
{
    struct sun8i_hdmi_phy_variant *variant = phy->variant;

    if (variant->is_custom_phy)
    {
        plat_data->phy_ops = &sun8i_hdmi_phy_ops;
        plat_data->phy_name = "sun8i_dw_hdmi_phy";
        plat_data->phy_data = phy;
    }
    else
    {
        plat_data->mpll_cfg = variant->mpll_cfg;
        plat_data->cur_ctr = variant->cur_ctr;
        plat_data->phy_config = variant->phy_cfg;
    }
}

static const struct regmap_config sun8i_hdmi_phy_regmap_config = {
    .reg_bits = 32,
    .val_bits = 32,
    .reg_stride = 4,
    .max_register = SUN8I_HDMI_PHY_CEC_REG,
    .name = "phy"};

static const struct sun8i_hdmi_phy_variant sun50i_h616_hdmi_phy = {
    .cur_ctr = sun50i_h616_cur_ctr,
    .mpll_cfg = sun50i_h616_mpll_cfg,
    .phy_cfg = sun50i_h616_phy_config,
    .phy_init = &sun50i_hdmi_phy_init_h616,
};

static const struct of_device_id sun8i_hdmi_phy_of_table[] = {
    {
        .compatible = "allwinner,sun50i-h616-hdmi-phy",
        .data = &sun50i_h616_hdmi_phy,
    },
    {/* sentinel */}};

int sun8i_hdmi_phy_get(struct sun8i_dw_hdmi *hdmi, struct device_node *node)
{
    struct platform_device *pdev = of_find_device_by_node(node);
    struct sun8i_hdmi_phy *phy;

    if (!pdev)
        return -EPROBE_DEFER;

    phy = platform_get_drvdata(pdev);
    if (!phy)
    {
        put_device(&pdev->dev);
        return -EPROBE_DEFER;
    }

    hdmi->phy = phy;

    put_device(&pdev->dev);

    return 0;
}

static int sun8i_hdmi_phy_probe(struct platform_device *pdev)
{
    const struct of_device_id *match;
    struct device *dev = &pdev->dev;
    struct device_node *node = dev->of_node;
    struct sun8i_hdmi_phy *phy;
    struct resource res;
    void __iomem *regs;
    int ret;

    match = of_match_node(sun8i_hdmi_phy_of_table, node);
    if (!match)
    {
        dev_err(dev, "Incompatible HDMI PHY\n");
        return -EINVAL;
    }

    phy = devm_kzalloc(dev, sizeof(*phy), GFP_KERNEL);
    if (!phy)
        return -ENOMEM;

    phy->variant = (struct sun8i_hdmi_phy_variant *)match->data;
    phy->dev = dev;

    ret = of_address_to_resource(node, 0, &res);
    if (ret)
    {
        dev_err(dev, "phy: Couldn't get our resources\n");
        return ret;
    }

    regs = devm_ioremap_resource(dev, &res);
    if (IS_ERR(regs))
    {
        dev_err(dev, "Couldn't map the HDMI PHY registers\n");
        return PTR_ERR(regs);
    }

    phy->regs = devm_regmap_init_mmio(dev, regs, &sun8i_hdmi_phy_regmap_config);
    if (IS_ERR(phy->regs))
    {
        dev_err(dev, "Couldn't create the HDMI PHY regmap\n");
        return PTR_ERR(phy->regs);
    }

    phy->clk_bus = of_clk_get_by_name(node, "bus");
    if (IS_ERR(phy->clk_bus))
    {
        dev_err(dev, "Could not get bus clock\n");
        return PTR_ERR(phy->clk_bus);
    }

    phy->clk_mod = of_clk_get_by_name(node, "mod");
    if (IS_ERR(phy->clk_mod))
    {
        dev_err(dev, "Could not get mod clock\n");
        ret = PTR_ERR(phy->clk_mod);
        goto err_put_clk_bus;
    }

    if (phy->variant->has_phy_clk)
    {
        phy->clk_pll0 = of_clk_get_by_name(node, "pll-0");
        if (IS_ERR(phy->clk_pll0))
        {
            dev_err(dev, "Could not get pll-0 clock\n");
            ret = PTR_ERR(phy->clk_pll0);
            goto err_put_clk_mod;
        }

        if (phy->variant->has_second_pll)
        {
            phy->clk_pll1 = of_clk_get_by_name(node, "pll-1");
            if (IS_ERR(phy->clk_pll1))
            {
                dev_err(dev, "Could not get pll-1 clock\n");
                ret = PTR_ERR(phy->clk_pll1);
                goto err_put_clk_pll0;
            }
        }
    }

    phy->rst_phy = of_reset_control_get_shared(node, "phy");
    if (IS_ERR(phy->rst_phy))
    {
        dev_err(dev, "Could not get phy reset control\n");
        ret = PTR_ERR(phy->rst_phy);
        goto err_put_clk_pll1;
    }

    platform_set_drvdata(pdev, phy);

    return 0;

err_put_clk_pll1:
    clk_put(phy->clk_pll1);
err_put_clk_pll0:
    clk_put(phy->clk_pll0);
err_put_clk_mod:
    clk_put(phy->clk_mod);
err_put_clk_bus:
    clk_put(phy->clk_bus);

    return ret;
}

static int sun8i_hdmi_phy_remove(struct platform_device *pdev)
{
    struct sun8i_hdmi_phy *phy = platform_get_drvdata(pdev);

    reset_control_put(phy->rst_phy);

    clk_put(phy->clk_pll0);
    clk_put(phy->clk_pll1);
    clk_put(phy->clk_mod);
    clk_put(phy->clk_bus);
    return 0;
}

struct platform_driver sun8i_hdmi_phy_driver = {
    .probe = sun8i_hdmi_phy_probe,
    .remove = sun8i_hdmi_phy_remove,
    .driver = {
        .name = "sun8i-hdmi-phy",
        .of_match_table = sun8i_hdmi_phy_of_table,
    },
};
