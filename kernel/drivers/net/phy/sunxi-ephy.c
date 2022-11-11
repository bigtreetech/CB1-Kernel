/*
 * Copyright © 2015-2016, Shuge
 *		Author: Sugar <shugeLinux@gmail.com>
 *
 * This file is provided under a dual BSD/GPL license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <linux/sunxi-sid.h>

struct ephy_res
{
    struct phy_device *ac300;
    atomic_t ephy_en;
};

static struct ephy_res ac300_ephy;

static void disable_intelligent_ieee(struct phy_device *phydev)
{
    unsigned int value;

    phy_write(phydev, 0x1f, 0x0100); /* switch to page 1 */
    value = phy_read(phydev, 0x17);  /* read address 0 0x17 register */
    value &= ~(1 << 3);              /* reg 0x17 bit 3, set 0 to disable IEEE */
    phy_write(phydev, 0x17, value);
    phy_write(phydev, 0x1f, 0x0000); /* switch to page 0 */
}

static void disable_802_3az_ieee(struct phy_device *phydev)
{
    unsigned int value;

    phy_write(phydev, 0xd, 0x7);
    phy_write(phydev, 0xe, 0x3c);
    phy_write(phydev, 0xd, 0x1 << 14 | 0x7);
    value = phy_read(phydev, 0xe);
    value &= ~(0x1 << 1);
    phy_write(phydev, 0xd, 0x7);
    phy_write(phydev, 0xe, 0x3c);
    phy_write(phydev, 0xd, 0x1 << 14 | 0x7);
    phy_write(phydev, 0xe, value);

    phy_write(phydev, 0x1f, 0x0200); /* switch to page 2 */
    phy_write(phydev, 0x18, 0x0000);
}

int ephy_config_init(struct phy_device *phydev)
{
    int value;

    /* init ephy */
    phy_write(phydev, 0x1f, 0x0100); /* Switch to Page 1 */
    phy_write(phydev, 0x12, 0x4824); /* Disable APS */

    phy_write(phydev, 0x1f, 0x0200); /* Switch to Page 2 */
    phy_write(phydev, 0x18, 0x0000); /* PHYAFE TRX optimization */

    phy_write(phydev, 0x1f, 0x0600); /* Switch to Page 6 */
    phy_write(phydev, 0x14, 0x708b); /* PHYAFE TX optimization */
    phy_write(phydev, 0x13, 0xF000); /* PHYAFE RX optimization */
    phy_write(phydev, 0x15, 0x1530);

    phy_write(phydev, 0x1f, 0x0800); /* Switch to Page 8 */
    phy_write(phydev, 0x18, 0x00bc); /* PHYAFE TRX optimization */

    disable_intelligent_ieee(phydev); /* Disable Intelligent IEEE */
    disable_802_3az_ieee(phydev);     /* Disable 802.3az IEEE */

    phy_write(phydev, 0x1f, 0x0000); /* Switch to Page 0 */
    phy_write(phydev, 0x04, 0x0de1);
    phy_write(phydev, 0x06, 0x860); /* RMII */
    phy_write(phydev, 0x07, 0x2001);
    phy_write(phydev, 25, 0x0800);

    phy_write(ac300_ephy.ac300, 0x06, 0x0802); /* RMII */

    return 0;
}

static int ephy_probe(struct phy_device *phydev)
{
    return 0;
}

static struct phy_driver sunxi_phy_driver = {
    .phy_id = 0x00441400,
    .name = "ephy",
    .phy_id_mask = 0x0ffffff0,
    .features = PHY_BASIC_FEATURES, //| SUPPORTED_Pause |SUPPORTED_Asym_Pause,

    .config_init = &ephy_config_init,
    .config_aneg = &genphy_config_aneg,
    .read_status = &genphy_read_status,
    .suspend = genphy_suspend,
    .resume = genphy_resume,
    .probe = ephy_probe,
};

static void ac300_ephy_enable(struct ephy_res *priv)
{
    phy_write(priv->ac300, 0x00, 0x1f83); /* release reset */
    phy_write(priv->ac300, 0x00, 0x1f37); /* clk gating (25MHz clock)*/

    phy_write(priv->ac300, 0x05, 0xa81f);
    phy_write(priv->ac300, 0x06, 0x02);

    msleep(1000); /* FIXME: fix some board compatible issues. */
    atomic_set(&ac300_ephy.ephy_en, 1);
}

int ac300_ephy_probe(struct phy_device *phydev)
{
    ac300_ephy.ac300 = phydev;

    atomic_set(&ac300_ephy.ephy_en, 0);
    ac300_ephy_enable(&ac300_ephy);

    return 0;
}

static int ac300_ephy_suspend(struct phy_device *phydev)
{
    ac300_ephy.ac300 = phydev;

    phy_write(ac300_ephy.ac300, 0x00, 0x1f40);
    phy_write(ac300_ephy.ac300, 0x05, 0xa800);
    phy_write(ac300_ephy.ac300, 0x06, 0x01);

    return 0;
}

static int ac300_ephy_resume(struct phy_device *phydev)
{
    ac300_ephy_enable(&ac300_ephy);
    return 0;
}

static void ac300_ephy_shutdown(struct phy_device *phydev)
{
    ac300_ephy.ac300 = phydev;
    phy_write(ac300_ephy.ac300, 0x00, 0x1f40);
}

static struct phy_driver ac300_ephy_driver = {
    .phy_id = 0xc0000000,
    .name = "ac300",
    .phy_id_mask = 0xffffffff,
    .suspend = ac300_ephy_suspend,
    .resume = ac300_ephy_resume,
    .probe = ac300_ephy_probe,
    //.shutdown	= ac300_ephy_shutdown,
};

static int ephy_init(void)
{
    int ret = 0;

    ret = phy_driver_register(&ac300_ephy_driver, THIS_MODULE);
    if (ret)
    {
        phy_driver_unregister(&ac300_ephy_driver);
    }

    ret = phy_driver_register(&sunxi_phy_driver, THIS_MODULE);
    if (ret)
    {
        phy_driver_unregister(&sunxi_phy_driver);
    }

    return ret;
}

static void ephy_exit(void)
{
    phy_driver_unregister(&sunxi_phy_driver);
    phy_driver_unregister(&ac300_ephy_driver);
}

module_init(ephy_init);
module_exit(ephy_exit);

static struct mdio_device_id __maybe_unused ephy_tbl[] = {
    {0x00441400, 0x0ffffff0},
    {}};

MODULE_DEVICE_TABLE(mdio, ephy_tbl);

MODULE_DESCRIPTION("Allwinner EPHY drivers");
MODULE_AUTHOR("Sugar <shugeLinux@gmail.com>");
MODULE_LICENSE("GPL");
