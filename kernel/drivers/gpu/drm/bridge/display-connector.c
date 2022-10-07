// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2019 Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 */

#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <drm/drm_bridge.h>
#include <drm/drm_edid.h>

struct display_connector
{
    struct drm_bridge bridge;

    struct gpio_desc *hpd_gpio;
    int hpd_irq;

    struct regulator *dp_pwr;
};

static inline struct display_connector *to_display_connector(struct drm_bridge *bridge)
{
    return container_of(bridge, struct display_connector, bridge);
}

static int display_connector_attach(struct drm_bridge *bridge, enum drm_bridge_attach_flags flags)
{
    return flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR ? 0 : -EINVAL;
}

static enum drm_connector_status display_connector_detect(struct drm_bridge *bridge)
{
    struct display_connector *conn = to_display_connector(bridge);

    if (conn->hpd_gpio)
    {
        if (gpiod_get_value_cansleep(conn->hpd_gpio))
            return connector_status_connected;
        else
            return connector_status_disconnected;
    }

    if (conn->bridge.ddc && drm_probe_ddc(conn->bridge.ddc))
        return connector_status_connected;

    return connector_status_disconnected;
}

static struct edid *display_connector_get_edid(struct drm_bridge *bridge, struct drm_connector *connector)
{
    struct display_connector *conn = to_display_connector(bridge);
    return drm_get_edid(connector, conn->bridge.ddc);
}

static const struct drm_bridge_funcs display_connector_bridge_funcs = {
    .attach = display_connector_attach,
    .detect = display_connector_detect,
    .get_edid = display_connector_get_edid,
};

static irqreturn_t display_connector_hpd_irq(int irq, void *arg)
{
    struct display_connector *conn = arg;
    struct drm_bridge *bridge = &conn->bridge;

    drm_bridge_hpd_notify(bridge, display_connector_detect(bridge));

    return IRQ_HANDLED;
}

static int display_connector_probe(struct platform_device *pdev)
{
    struct display_connector *conn;
    unsigned int type;
    const char *label = NULL;
    int ret;

    conn = devm_kzalloc(&pdev->dev, sizeof(*conn), GFP_KERNEL);
    if (!conn)
        return -ENOMEM;

    platform_set_drvdata(pdev, conn);

    type = (uintptr_t)of_device_get_match_data(&pdev->dev);

    const char *hdmi_type;

    ret = of_property_read_string(pdev->dev.of_node, "type", &hdmi_type);
    if (ret < 0)
    {
        dev_err(&pdev->dev, "HDMI connector with no type\n");
        return -EINVAL;
    }

    conn->bridge.type = DRM_MODE_CONNECTOR_HDMIA;

    /* All the supported connector types support interlaced modes. */
    conn->bridge.interlace_allowed = true;

    /* Get the optional connector label. */
    of_property_read_string(pdev->dev.of_node, "label", &label);

    /*
     * Get the HPD GPIO for DVI, HDMI and DP connectors. If the GPIO can provide
     * edge interrupts, register an interrupt handler.
     */
    conn->hpd_gpio = devm_gpiod_get_optional(&pdev->dev, "hpd", GPIOD_IN);
    if (IS_ERR(conn->hpd_gpio))
    {
        if (PTR_ERR(conn->hpd_gpio) != -EPROBE_DEFER)
            dev_err(&pdev->dev, "Unable to retrieve HPD GPIO\n");
        return PTR_ERR(conn->hpd_gpio);
    }

    conn->bridge.funcs = &display_connector_bridge_funcs;
    conn->bridge.of_node = pdev->dev.of_node;

    dev_dbg(&pdev->dev,
            "Found %s display connector '%s' %s DDC bus and %s HPD GPIO (ops 0x%x)\n",
            drm_get_connector_type_name(conn->bridge.type),
            label ? label : "<unlabelled>",
            conn->bridge.ddc ? "with" : "without",
            conn->hpd_gpio ? "with" : "without",
            conn->bridge.ops);

    drm_bridge_add(&conn->bridge);

    return 0;
}

static int display_connector_remove(struct platform_device *pdev)
{
    struct display_connector *conn = platform_get_drvdata(pdev);

    if (conn->dp_pwr)
        regulator_disable(conn->dp_pwr);

    drm_bridge_remove(&conn->bridge);

    if (!IS_ERR(conn->bridge.ddc))
        i2c_put_adapter(conn->bridge.ddc);

    return 0;
}

static const struct of_device_id display_connector_match[] = {
    {
        .compatible = "hdmi-connector",
        .data = (void *)DRM_MODE_CONNECTOR_HDMIA,
    },
    {},
};
MODULE_DEVICE_TABLE(of, display_connector_match);

static struct platform_driver display_connector_driver = {
    .probe = display_connector_probe,
    .remove = display_connector_remove,
    .driver = {
        .name = "display-connector",
        .of_match_table = display_connector_match,
    },
};
module_platform_driver(display_connector_driver);

MODULE_AUTHOR("Laurent Pinchart <laurent.pinchart@ideasonboard.com>");
MODULE_DESCRIPTION("Display connector driver");
MODULE_LICENSE("GPL");
