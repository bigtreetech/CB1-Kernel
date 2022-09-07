/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 */

#ifndef _CONFIG_SBOOT_FLASH_H
#define _CONFIG_SBOOT_FLASH_H

#include <common.h>

int sunxi_flash_init(int boot_type);
int sunxi_flash_read(u32 start_sector, u32 blkcnt, void *buff);
int load_toc1_from_mmc( int boot_type );
int load_toc1_from_nand( void );
int load_toc1_from_spinand(void);
int load_toc1_from_spinor(void);


extern sbrom_toc0_config_t *toc0_config;

#endif
