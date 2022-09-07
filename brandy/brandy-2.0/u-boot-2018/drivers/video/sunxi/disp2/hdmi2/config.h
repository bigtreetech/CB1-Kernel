/*
 * drivers/video/sunxi/disp2/hdmi2/config.h
 *
 * Copyright (c) 2007-2019 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _CONFIG_H_
#define  _CONFIG_H_

//#define __LINUX_PLAT__ 

#ifdef CONFIG_MACH_SUN50IW9
#define CCMU_PLL_VIDEO2_BIAS
#define CCMU_PLL_VIDEO2_BIAS_ADDR 0x03001350
#endif

#endif
