// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 *
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * Some init for sunxi platform.
 */

#include <common.h>
#include <cpu_func.h>
#include <init.h>
#include <log.h>
#include <mmc.h>
#include <i2c.h>
#include <serial.h>
#include <spl.h>
#include <asm/cache.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/spl.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/timer.h>
#include <asm/arch/tzpc.h>
#include <asm/arch/mmc.h>
#include <asm/arch/prcm.h>

#include <linux/compiler.h>
#include <linux/delay.h>

struct fel_stash
{
    uint32_t sp;
    uint32_t lr;
    uint32_t cpsr;
    uint32_t sctlr;
    uint32_t vbar;
    uint32_t cr;
};

struct fel_stash fel_stash __section(".data");

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>

static struct mm_region sunxi_mem_map[] = {
    {/* SRAM, MMIO regions */
     .virt = 0x0UL,
     .phys = 0x0UL,
     .size = 0x40000000UL,
     .attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) | PTE_BLOCK_NON_SHARE},
    {/* RAM */
     .virt = 0x40000000UL,
     .phys = 0x40000000UL,
     .size = CONFIG_SUNXI_DRAM_MAX_SIZE,
     .attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_INNER_SHARE},
    {
        /* List terminator */
        0,
    }};
struct mm_region *mem_map = sunxi_mem_map;

ulong board_get_usable_ram_top(ulong total_size)
{
    /* Some devices (like the EMAC) have a 32-bit DMA limit. */
    if (gd->ram_top > (1ULL << 32))
        return 1ULL << 32;

    return gd->ram_top;
}
#endif

static int gpio_init(void)
{
    __maybe_unused uint val;
#if CONFIG_CONS_INDEX == 1 && defined(CONFIG_UART0_PORT_F)
#if defined(CONFIG_MACH_SUN8I) && !defined(CONFIG_MACH_SUN8I_R40)
    sunxi_gpio_set_cfgpin(SUNXI_GPF(2), SUN8I_GPF_UART0);
    sunxi_gpio_set_cfgpin(SUNXI_GPF(4), SUN8I_GPF_UART0);
#else
    sunxi_gpio_set_cfgpin(SUNXI_GPF(2), SUNXI_GPF_UART0);
    sunxi_gpio_set_cfgpin(SUNXI_GPF(4), SUNXI_GPF_UART0);
#endif
    sunxi_gpio_set_pull(SUNXI_GPF(4), 1);
#elif CONFIG_CONS_INDEX == 1 && defined(CONFIG_MACH_SUN50I_H616)
    sunxi_gpio_set_cfgpin(SUNXI_GPH(0), SUN50I_H616_GPH_UART0);
    sunxi_gpio_set_cfgpin(SUNXI_GPH(1), SUN50I_H616_GPH_UART0);
    sunxi_gpio_set_pull(SUNXI_GPH(1), SUNXI_GPIO_PULL_UP);
#else
#error Unsupported console port number. Please fix pin mux settings in board.c
#endif

#ifdef CONFIG_SUN50I_GEN_H6
    /* Update PIO power bias configuration by copy hardware detected value */
    val = readl(SUNXI_PIO_BASE + SUN50I_H6_GPIO_POW_MOD_VAL);
    writel(val, SUNXI_PIO_BASE + SUN50I_H6_GPIO_POW_MOD_SEL);
    val = readl(SUNXI_R_PIO_BASE + SUN50I_H6_GPIO_POW_MOD_VAL);
    writel(val, SUNXI_R_PIO_BASE + SUN50I_H6_GPIO_POW_MOD_SEL);
#endif

    return 0;
}

#if defined(CONFIG_SPL_BOARD_LOAD_IMAGE) && defined(CONFIG_SPL_BUILD)
static int spl_board_load_image(struct spl_image_info *spl_image,
                                struct spl_boot_device *bootdev)
{
    debug("Returning to FEL sp=%x, lr=%x\n", fel_stash.sp, fel_stash.lr);
    return_to_fel(fel_stash.sp, fel_stash.lr);

    return 0;
}
SPL_LOAD_IMAGE_METHOD("FEL", 0, BOOT_DEVICE_BOARD, spl_board_load_image);
#endif

void s_init(void)
{
    /*
     * Undocumented magic taken from boot0, without this DRAM
     * access gets messed up (seems cache related).
     * The boot0 sources describe this as: "config ema for cache sram"
     */

#if !defined(CONFIG_ARM_CORTEX_CPU_IS_UP) && !defined(CONFIG_ARM64)
    /* Enable SMP mode for CPU0, by setting bit 6 of Auxiliary Ctl reg */
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 1\n"
        "orr r0, r0, #1 << 6\n"
        "mcr p15, 0, r0, c1, c0, 1\n" ::
            : "r0");
#endif

    clock_init();
    timer_init();
    gpio_init();
#if !CONFIG_IS_ENABLED(DM_I2C)
    i2c_init_board();
#endif
    eth_init_board();
}

#define SUNXI_INVALID_BOOT_SOURCE -1

static int sunxi_get_boot_source(void)
{
    if (!is_boot0_magic(SPL_ADDR + 4)) /* eGON.BT0 */
        return SUNXI_INVALID_BOOT_SOURCE;

    return readb(SPL_ADDR + 0x28);
}

/* The sunxi internal brom will try to loader external bootloader
 * from mmc0, nand flash, mmc2.
 */
uint32_t sunxi_get_boot_device(void)
{
    int boot_source = sunxi_get_boot_source();

    /*
     * When booting from the SD card or NAND memory, the "eGON.BT0"
     * signature is expected to be found in memory at the address 0x0004
     * (see the "mksunxiboot" tool, which generates this header).
     *
     * When booting in the FEL mode over USB, this signature is patched in
     * memory and replaced with something else by the 'fel' tool. This other
     * signature is selected in such a way, that it can't be present in a
     * valid bootable SD card image (because the BROM would refuse to
     * execute the SPL in this case).
     *
     * This checks for the signature and if it is not found returns to
     * the FEL code in the BROM to wait and receive the main u-boot
     * binary over USB. If it is found, it determines where SPL was
     * read from.
     */
    switch (boot_source)
    {
    case SUNXI_INVALID_BOOT_SOURCE:
        return BOOT_DEVICE_BOARD;
    case SUNXI_BOOTED_FROM_MMC0:
    case SUNXI_BOOTED_FROM_MMC0_HIGH:
        return BOOT_DEVICE_MMC1;
    case SUNXI_BOOTED_FROM_NAND:
        return BOOT_DEVICE_NAND;
    case SUNXI_BOOTED_FROM_MMC2:
    case SUNXI_BOOTED_FROM_MMC2_HIGH:
        return BOOT_DEVICE_MMC2;
    case SUNXI_BOOTED_FROM_SPI:
        return BOOT_DEVICE_SPI;
    }

    panic("Unknown boot source %d\n", boot_source);
    return -1; /* Never reached */
}

#ifdef CONFIG_SPL_BUILD
static u32 sunxi_get_spl_size(void)
{
    if (!is_boot0_magic(SPL_ADDR + 4)) /* eGON.BT0 */
        return 0;

    return readl(SPL_ADDR + 0x10);
}

/*
 * The eGON SPL image can be located at 8KB or at 128KB into an SD card or
 * an eMMC device. The boot source has bit 4 set in the latter case.
 * By adding 120KB to the normal offset when booting from a "high" location
 * we can support both cases.
 * Also U-Boot proper is located at least 32KB after the SPL, but will
 * immediately follow the SPL if that is bigger than that.
 */
unsigned long spl_mmc_get_uboot_raw_sector(struct mmc *mmc, unsigned long raw_sect)
{
    unsigned long spl_size = sunxi_get_spl_size();
    unsigned long sector;

    sector = max(raw_sect, spl_size / 512);

    switch (sunxi_get_boot_source())
    {
    case SUNXI_BOOTED_FROM_MMC0_HIGH:
    case SUNXI_BOOTED_FROM_MMC2_HIGH:
        sector += (128 - 8) * 2;
        break;
    }

    return sector;
}

u32 spl_boot_device(void)
{
    return sunxi_get_boot_device();
}

void board_init_f(ulong dummy)
{
    spl_init();
    preloader_console_init();

#ifdef CONFIG_SPL_I2C
    /* Needed early by sunxi_board_init if PMU is enabled */
    i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
#endif
    sunxi_board_init();
}
#endif

void reset_cpu(void)
{
#if defined(CONFIG_SUNXI_GEN_SUN6I) || defined(CONFIG_SUN50I_GEN_H6)
#if defined(CONFIG_MACH_SUN50I_H6)
    /* WDOG is broken for some H6 rev. use the R_WDOG instead */
    static const struct sunxi_wdog *wdog =
        (struct sunxi_wdog *)SUNXI_R_WDOG_BASE;
#else
    static const struct sunxi_wdog *wdog =
        ((struct sunxi_timer_reg *)SUNXI_TIMER_BASE)->wdog;
#endif
    /* Set the watchdog for its shortest interval (.5s) and wait */
    writel(WDT_CFG_RESET, &wdog->cfg);
    writel(WDT_MODE_EN, &wdog->mode);
    writel(WDT_CTRL_KEY | WDT_CTRL_RESTART, &wdog->ctl);
    while (1)
    {
    }
#endif
}

#if !CONFIG_IS_ENABLED(SYS_DCACHE_OFF) && !defined(CONFIG_ARM64)
void enable_caches(void)
{
    /* Enable D-cache. I-cache is already enabled in start.S */
    dcache_enable();
}
#endif
