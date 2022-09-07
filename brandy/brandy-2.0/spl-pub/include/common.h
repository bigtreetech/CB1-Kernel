/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom wangwei <wangwei@allwinnertech.com>
 */

#ifndef _SYS_COMMON_H_
#define _SYS_COMMON_H_

#include <linux/types.h>
#include <linux/sizes.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <config.h>
#include <arch/cpu.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifdef BOOT_DEBUG
#define _DEBUG	1
#define boot_info(fmt, args...)  printf(fmt, ##args)
#else
#define _DEBUG	0
#define boot_info(fmt, args...)
#endif



void udelay(unsigned long);
void mdelay(unsigned long);
void sdelay(unsigned long loops);
u32 timer_get_us(void);
u32 get_sys_ticks(void);

void print_sys_tick(void);
int printf(const char *fmt, ...);
void ndump(u8 *buf, int count);
void __assert_fail(const char *assertion, const char *file, unsigned line,
		   const char *function);
#define assert(x) \
	({ if (!(x) && _DEBUG) \
		__assert_fail(#x, __FILE__, __LINE__, __func__); })



void mmu_enable(u32 dram_size);
void mmu_disable(void);
void v7_flush_dcache_all(void);
void dcache_enable(void);
void dcache_disable(void);

int sunxi_board_init(void);
int sunxi_board_exit(void);

void boot0_jmp_monitor(u32 addr);
void boot0_jmp_optee(u32 optee, u32 uboot);
void boot0_jmp(u32 addr);

int axp_init(u8 power_mode);
int set_ddr_voltage(int set_vol);
int set_sys_voltage(int set_vol);
int set_pll_voltage(int set_vol);
int probe_power_key(void);
int get_power_mode(void);

void neon_enable(void);
void __iomem *sunxi_get_iobase (unsigned int base);

char* strcpy(char *dest,const char *src);
char* strncpy(char *dest,const char *src,size_t count);
char* strcat(char *dest, const char *src);
char* strncat(char *dest, const char *src, size_t count);
int strcmp(const char *cs,const char *ct);
int strncmp(const char *cs,const char *ct,size_t count);
char* strchr(const char *s, int c);
size_t strlen(const char *s);
char* strrchr(const char *s, int c);
size_t strnlen(const char *s, size_t count);
size_t strspn(const char *s, const char *accept);
extern void* memset0(void *s, int c, size_t count);
void* memset(void *s, int c, size_t count);
void* memcpy0(void *dest, const void *src, size_t n);
void* memcpy(void *dest, const void *src, size_t count);
int memcmp(const void *cs,const void *ct,size_t count);
void* memscan(void *addr, int c, size_t size);
char* strstr(const char *s1,const char *s2);
void* memchr(const void *s, int c, size_t n);

int malloc_init(u32 start, u32 size);
void* malloc(u32 size);
void* realloc(void *p, u32 size);
void  free(void *p);



int load_package(void);
int load_image(u32 *uboot_base, u32 *optee_base, u32 *monitor_base);
void update_flash_para(u32 uboot_base);
int verify_addsum(void *mem_base, u32 size);
u32 g_mod( u32 dividend, u32 divisor, u32 *quot_p);
char get_uart_input(void);

int sunxi_deassert_arisc(void);
void handler_super_standby(void);
int super_standby_mode(void);
void set_uboot_func_mask(uint8_t mask);
uint8_t get_uboot_func_mask(uint8_t mask);
uint8_t sunxi_board_late_init(void);

#endif
