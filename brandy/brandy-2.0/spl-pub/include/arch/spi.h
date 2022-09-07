/*
 * (C) Copyright 20018-2019
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: spinor driver for General spinor operations
 * Author: wangwei <wangwei@allwinnertech.com>
 * Date: 2018-11-15 14:18:18
 */

#ifndef __SUNXI_SPI_H
#define __SUNXI_SPI_H

#include <common.h>
#include <asm/io.h>

int spi_init(void);
void spi_exit(void);
int  spi_xfer( unsigned int tx_len, const void *dout, unsigned int rx_len,  void *din);


#endif
