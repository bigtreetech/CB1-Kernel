/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 * wangwei <wangwei@allwinnertech.com>
 */

#include <common.h>
#include <private_boot0.h>
#include <private_uboot.h>
#include <private_toc.h>
#include <arch/clock.h>
#include <arch/uart.h>
#include <arch/dram.h>
#include <arch/gpio.h>
#include <arch/rtc.h>
#include <arch/efuse.h>
#include "sboot_flash.h"
#include "sboot_toc.h"

sbrom_toc0_config_t *toc0_config = (sbrom_toc0_config_t *)(CONFIG_TOC0_CFG_ADDR);
int mmc_config_addr = (int)(((sbrom_toc0_config_t *)CONFIG_TOC0_CFG_ADDR)->storage_data + 160);

static int sboot_clear_env(void);

static void print_commit_log(void)
{
	printf("HELLO! SBOOT is starting!\n");
	printf("sboot commit : %s \n",sboot_head.hash);
}

void sboot_main(void)
{
	toc0_private_head_t *toc0 = (toc0_private_head_t *)CONFIG_TOC0_HEAD_BASE;
	uint dram_size;
	int  ret;

	sunxi_serial_init(toc0_config->uart_port, toc0_config->uart_ctrl, 2);
	print_commit_log();

	ret = sunxi_board_init();
	if(ret)
		goto _BOOT_ERROR;

	if (rtc_probe_fel_flag()) {
		rtc_clear_fel_flag();
		goto _BOOT_ERROR;
	}

	if (toc0_config->enable_jtag) {
		boot_set_gpio((normal_gpio_cfg *)toc0_config->jtag_gpio, 5, 1);
	}
#if CFG_SUNXI_JTAG_DISABLE
	else {
		sid_disable_jtag();
	}
#endif

	printf("try to probe rtc region\n");

	//dram init
#ifdef FPGA_PLATFORM
	dram_size = mctl_init((void *)toc0_config->dram_para);
#else
	if (toc0_config->dram_para[30] & (1 << 11))
		neon_enable();
	dram_size = init_DRAM(0, (void *)toc0_config->dram_para);
#endif
	if (!dram_size) {
		printf("init dram fail\n");
		goto _BOOT_ERROR;
	}

	char uart_input_value = get_uart_input();

	if (uart_input_value == '2') {
		printf("detected user input 2\n");
		goto _BOOT_ERROR;
	}

	mmu_enable(dram_size);
	malloc_init(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE);

	ret = sunxi_board_late_init();
	if (ret)
		goto _BOOT_ERROR;

	if (toc0->platform[0] & 0xf0)
		printf("read toc0 from emmc backup\n");

	ret = sunxi_flash_init(toc0->platform[0] & 0x0f);
	if(ret)
		goto _BOOT_ERROR;

	ret = toc1_init();
	if(ret)
		goto _BOOT_ERROR;

	ret = toc1_verify_and_run(dram_size);
	if(ret)
		goto _BOOT_ERROR;

_BOOT_ERROR:
	sboot_clear_env();
	boot0_jmp(SECURE_FEL_BASE);
}

static int sboot_clear_env(void)
{
	sunxi_board_exit();
	sunxi_board_clock_reset();
	mmu_disable();
	mdelay(10);
	return 0;
}
