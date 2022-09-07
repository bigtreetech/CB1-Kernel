/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 */

#ifndef	__SBROM_HW__H__
#define	__SBROM_HW__H__

#include <private_toc.h>

typedef struct {
	struct sbrom_toc1_item_info *key_certif;
	struct sbrom_toc1_item_info *bin_certif;
	struct sbrom_toc1_item_info *binfile;
	struct sbrom_toc1_item_info *normal;
} sbrom_toc1_item_group;

int toc1_init(void);
uint toc1_item_read(struct sbrom_toc1_item_info *p_toc_item, void * p_dest, u32 buff_len);

uint toc1_item_read_rootcertif(void * p_dest, u32 buff_len);
int toc1_item_probe_start(void);
int toc1_item_probe_next(sbrom_toc1_item_group *item_group);
int toc1_verify_and_run(u32 dram_size);


#endif	//__SBROM_HW__H__
