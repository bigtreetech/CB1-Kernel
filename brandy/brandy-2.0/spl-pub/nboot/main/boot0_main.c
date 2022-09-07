/*
 * (C) Copyright 2018
 * wangwei <wangwei@allwinnertech.com>
 */

#include <common.h>
#include <private_boot0.h>
#include <private_uboot.h>
#include <private_toc.h>
#include <arch/clock.h>
#include <arch/uart.h>
#include <arch/dram.h>
#include <arch/rtc.h>

static void update_uboot_info(u32 uboot_base, u32 optee_base, u32 monitor_base, u32 dram_size);
static int boot0_clear_env(void);

void main(void)
{
	int dram_size;
	int status;
	u32 uboot_base = 0, optee_base = 0, monitor_base = 0;

	sunxi_serial_init(BT0_head.prvt_head.uart_port, (void *)BT0_head.prvt_head.uart_ctrl, 6);
	printf("HELLO! BOOT0 is starting!\n");
	printf("BOOT0 commit : %s\n", BT0_head.hash);

	status = sunxi_board_init();
	if(status)
		goto _BOOT_ERROR;

	if (rtc_probe_fel_flag()) {
		rtc_clear_fel_flag();
		goto _BOOT_ERROR;
	}

#ifdef FPGA_PLATFORM
	dram_size = mctl_init((void *)BT0_head.prvt_head.dram_para);
#else
	if (BT0_head.prvt_head.dram_para[30] & (1 << 11))
		neon_enable();
	dram_size = init_DRAM(0, (void *)BT0_head.prvt_head.dram_para);
#endif
	if(!dram_size)
		goto _BOOT_ERROR;
	else {
		printf("dram size =%d\n", dram_size);
	}
	char uart_input_value = get_uart_input();

	if (uart_input_value == '2') {
		printf("detected user input 2\n");
		goto _BOOT_ERROR;
	}

	mmu_enable(dram_size);

	status = sunxi_board_late_init();
	if (status)
		goto _BOOT_ERROR;

	status = load_package();
	if(status == 0 )
		load_image(&uboot_base, &optee_base, &monitor_base);
	else
		goto _BOOT_ERROR;

	update_uboot_info(uboot_base, optee_base, monitor_base, dram_size);
	mmu_disable( );

	printf("Jump to second Boot.\n");

	if (monitor_base)
		boot0_jmp_monitor(monitor_base);
	else if (optee_base)
		boot0_jmp_optee(optee_base, uboot_base);
	else
		boot0_jmp(uboot_base);

	while(1);
_BOOT_ERROR:
	boot0_clear_env();
	boot0_jmp(FEL_BASE);

}

static void update_uboot_info(u32 uboot_base, u32 optee_base, u32 monitor_base, u32 dram_size)
{
	uboot_head_t  *header = (uboot_head_t *) uboot_base;
	struct sbrom_toc1_head_info *toc1_head = (struct sbrom_toc1_head_info *)CONFIG_BOOTPKG_BASE;

	header->boot_data.boot_package_size = toc1_head->valid_len;
	header->boot_data.dram_scan_size = dram_size;
	memcpy((void *)header->boot_data.dram_para, &BT0_head.prvt_head.dram_para, 32 * sizeof(int));

	if (monitor_base)
		header->boot_data.monitor_exist = 1;
	if (optee_base)
		header->boot_data.secureos_exist = 1;

	header->boot_data.func_mask = get_uboot_func_mask(UBOOT_FUNC_MASK_ALL);
	update_flash_para(uboot_base);
}

static int boot0_clear_env(void)
{
	sunxi_board_exit();
	sunxi_board_clock_reset();
	mmu_disable();
	mdelay(10);

	return 0;
}
