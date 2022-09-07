/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 */

#ifndef _CONFIG_SBOOT_CERITF_H
#define _CONFIG_SBOOT_CERITF_H

#include <common.h>

#define RSA_BIT_WITDH 2048
int sunxi_certif_pubkey_check( sunxi_key_t  *pubkey );
int sunxi_pubkey_hash_cal(char *out_buf, sunxi_key_t *pubkey);
int sunxi_root_certif_pk_verify(sunxi_certif_info_t *sunxi_certif, u8 *buf, u32 len);

#endif
