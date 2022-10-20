// SPDX-License-Identifier: GPL-2.0+
/*
 * FB driver for the ILI9341 LCD display controller
 *
 * This display uses 9-bit SPI: Data/Command bit + 8 data bits
 * For platforms that doesn't support 9-bit, the driver is capable
 * of emulating this using 8-bit transfer.
 * This is done by transferring eight 9-bit words in 9 bytes.
 *
 * Copyright (C) 2013 Christian Vogelgsang
 * Based on adafruit22fb.c by Noralf Tronnes
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <video/mipi_display.h>

#include "fbtft.h"

#define DRVNAME "fb_st7796"
#define WIDTH 320
#define HEIGHT 480
#define TXBUFLEN (8 * PAGE_SIZE)

static int init_display(struct fbtft_par *par)
{
    par->fbtftops.reset(par);

    /* startup sequence for MI0283QT-9A */
    write_reg(par, MIPI_DCS_SOFT_RESET);
    mdelay(10);
    write_reg(par, MIPI_DCS_SET_DISPLAY_OFF);

    /* ------------power control-------------------------------- */
    write_reg(par, 0xC0, 0x0c, 0x02);
    write_reg(par, 0xC1, 0x44);
    /* ------------VCOM --------- */
    write_reg(par, 0xC5, 0x00, 0x16, 0x80);
    write_reg(par, 0x36, 0x28);

    write_reg(par, 0x3a, 0x55); // Interface Mode Control
    write_reg(par, 0xB0, 0x00); // Interface Mode Control

    /* ------------frame rate----------------------------------- */
    write_reg(par, 0xB1, 0xB0); // 70HZ

    write_reg(par, 0xB4, 0x02);
    write_reg(par, 0xB6, 0x02, 0x02); // RGB/MCU Interface Control
    write_reg(par, 0xE9, 0x00);
    write_reg(par, 0xF7, 0xA9, 0x51, 0x2C, 0x82);

    write_reg(par, 0x11);
    mdelay(120);
    write_reg(par, 0x29);
    mdelay(20);

    printk("===> BQ-TFT: st7796 init complete!\n");

    return 0;
}

static void set_addr_win(struct fbtft_par *par, int xs, int ys, int xe, int ye)
{
    write_reg(par, 0x2a, (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF);
    write_reg(par, 0x2b, (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF);

    write_reg(par, 0x2c);
}

static int set_var(struct fbtft_par *par)
{
    switch (par->info->var.rotate)
    {
    case 0:
        write_reg(par, 0x36, 0x28);
        break;
    case 270:
        write_reg(par, 0x36, 0x48);
        break;
    case 180:
        write_reg(par, 0x36, 0xE8);
        break;
    case 90:
        write_reg(par, 0x36, 0x88);
        break;
    }

    return 0;
}

static struct fbtft_display display = {
    .regwidth = 8,
    .width = WIDTH,
    .height = HEIGHT,
    .txbuflen = TXBUFLEN,
    .gamma_num = 2,
    .gamma_len = 14,
    .fbtftops = {
        .init_display = init_display,
        .set_addr_win = set_addr_win,
        .set_var = set_var,
    },
};

FBTFT_REGISTER_DRIVER(DRVNAME, "sitronix,st7796", &display);

MODULE_ALIAS("spi:" DRVNAME);
MODULE_ALIAS("platform:" DRVNAME);
MODULE_ALIAS("spi:st7796");
MODULE_ALIAS("platform:st7796");

MODULE_DESCRIPTION("FB driver for the st7796 LCD display controller");
MODULE_AUTHOR("lodge");
MODULE_LICENSE("GPL");
