/*
 * Copyright (C) 2016 Allwinner.
 * wangwei <wangwei@allwinnertech.com>
 *
 * SUNXI AXP806  Driver
 *
 */

#ifndef __AXP806_REGS_H__
#define __AXP806_REGS_H__

/*PMIC chip id reg03:bit7-6  bit3-0 */
#define AXP806_CHIP_ID (0x40)

#define AXP806_DEVICE_ADDR              (0x745)
#ifdef CFG_SUNXI_TWI
#define AXP806_RUNTIME_ADDR             (0x36)
#else
#define AXP806_RUNTIME_ADDR		(0x2d)
#endif
/*define AXP806 REGISTER*/
#define   AXP806_STARUP_SRC             			(0x00)
#define   AXP806_IC_TYPE         	   			(0x03)
#define   AXP806_DATA_BUFFER0        			(0x04)
#define   AXP806_DATA_BUFFER1        			(0x05)
#define   AXP806_DATA_BUFFER2        			(0x06)
#define   AXP806_DATA_BUFFER3        			(0x07)

#define   AXP806_OUTPUT_CTL1     	   			(0x10)
#define   AXP806_OUTPUT_CTL2     	   			(0x11)

#define   AXP806_DCAOUT_VOL                  	(0x12)
#define   AXP806_DCBOUT_VOL          			(0x13)
#define   AXP806_DCCOUT_VOL          			(0x14)
#define   AXP806_DCDOUT_VOL          			(0x15)
#define   AXP806_DCEOUT_VOL          			(0x16)
#define   AXP806_ALDO1OUT_VOL                    (0x17)
#define   AXP806_ALDO2OUT_VOL                    (0x18)
#define   AXP806_ALDO3OUT_VOL                    (0x19)
#define   AXP806_DCMOD_CTL1                  	(0x1A)
#define   AXP806_DCMOD_CTL2                  	(0x1B)
#define   AXP806_DCFREQ_SET                  	(0x1C)
#define   AXP806_DCMONITOR_CTL                  	(0x1D)

#define   AXP806_BLDO1OUT_VOL                    (0x20)
#define   AXP806_BLDO2OUT_VOL                    (0x21)
#define   AXP806_BLDO3OUT_VOL                    (0x22)
#define   AXP806_BLDO4OUT_VOL                    (0x23)

#define   AXP806_CLDO1OUT_VOL                    (0x24)
#define   AXP806_CLDO2OUT_VOL                    (0x25)
#define   AXP806_CLDO3OUT_VOL                    (0x26)

#define   AXP806_VOFF_SETTING                    (0x31)
#define   AXP806_DIASBLE				(0x32)
#define   AXP806_POK_SETTING                     (0x36)

#define   AXP806_INTEN1              			(0x40)
#define   AXP806_INTEN2              			(0x41)
#define   AXP806_INTSTS1             			(0x48)
#define   AXP806_INTSTS2             			(0x49)


int axp806_set_ddr_voltage(int set_vol);
int axp806_set_ddr4_2v5_voltage(int set_vol);
int axp806_set_pll_voltage(int set_vol);
int axp806_set_sys_voltage(int set_vol, int onoff);
int axp806_axp_init(u8 power_mode);
int axp806_probe_power_key(void);

#endif /* __AXP806_REGS_H__ */

