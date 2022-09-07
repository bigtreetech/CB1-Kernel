/*
 * drivers/input/touchscreen/xpt2046_ts.c - driver for rk29 spi xpt2046 device and console
 *
 * Copyright (C) 2011 ROCKCHIP, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
 
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/input/mt.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include "calibration_ts.h"
#include <linux/types.h>
#include <linux/init.h>



#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include "xpt2046_ts.h"

extern volatile struct adc_point gADPoint;

/*
 * This code has been heavily tested on a Nokia 770, and lightly
 * tested on other xpt2046 devices (OSK/Mistral, Lubbock).
 * TSC2046 is just newer xpt2046 silicon.
 * Support for ads7843 tested on Atmel at91sam926x-EK.
 * Support for ads7845 has only been stubbed in.
 *
 * IRQ handling needs a workaround because of a shortcoming in handling
 * edge triggered IRQs on some platforms like the OMAP1/2. These
 * platforms don't handle the ARM lazy IRQ disabling properly, thus we
 * have to maintain our own SW IRQ disabled status. This should be
 * removed as soon as the affected platform's IRQ handling is fixed.
 *
 * app note sbaa036 talks in more detail about accurate sampling...
 * that ought to help in situations like LCDs inducing noise (which
 * can also be helped by using synch signals) and more generally.
 * This driver tries to utilize the measures described in the app
 * note. The strength of filtering can be set in the board-* specific
 * files.
 */
#define XPT2046_DEBUG			1
#if XPT2046_DEBUG
	#define xpt2046printInfo(msg...)	printk(msg);
	#define xpt2046printDebug(msg...)	printk(KERN_ERR msg);
#else
	#define xpt2046printInfo(msg...)
	#define xpt2046printDebug(msg...)
#endif

#define TS_POLL_DELAY	(10 * 1000000)	/* ns delay before the first sample */
#define TS_POLL_PERIOD	(20 * 1000000)	/* ns delay between samples */

/* this driver doesn't aim at the peak continuous sample rate */
#define	SAMPLE_BITS	(8 /*cmd*/ + 16 /*sample*/ + 2 /* before, after */)

//---------------------------zdx
#define TAG	 "tp:"

#define DEVICE_NAME "TPModule"
#define DEVICE_COUNT 1


#define SET_Calibration  _IO('S',2) 
#define SAVE_X  _IO('S',1) 
#define SAVE_Y  _IO('S',0) 

struct chr_dev
{
 struct cdev tp_cdev;
 dev_t dev_nr;
 struct class *tp_class;
struct device *tp_device;
 int TK_uncali_x[5];
 int TK_uncali_y[5];
 int TK_flag;
};
struct chr_dev *chr_devp;


static int my_open (struct inode *inode, struct file *filefp)
{
	chr_devp->TK_flag = 0;

	xpt2046printDebug("open the char device\n");
	return 0;//success
}
static int my_release(struct inode *inode, struct file *filefp)
{	
	xpt2046printDebug("close the char device\n");
	return 0;
}
static ssize_t my_read (struct file *filefp, char __user *buf, size_t count, loff_t *off)
{
	int ret;
	int msg[2];
	//xpt2046printDebug("=====enter my_read\n");
	msg[0] = gADPoint.x;
	msg[1] = gADPoint.y;
	//xpt2046printDebug("------ msg[0]=%d msg[1]=%d\n",msg[0],msg[1]);	
	ret = copy_to_user(buf, msg, count);
	if(ret != 0)
	{
		xpt2046printDebug("copy_to_user ERR\n");
		return -EFAULT;
	}
	//	xpt2046printDebug("enter read.ret=%d\n",ret);
	
	return 0;
}

static ssize_t my_write (struct file *filefp, const char __user *buf, size_t count, loff_t *off)
{
	//unsigned char ret;
	//xpt2046printDebug("=====enter my_write  chr_devp.TK_flag = %d\n",chr_devp->TK_flag);
  	if(chr_devp->TK_flag == 1)//save x
  	{
  		printk(KERN_ERR"write: enter save x.\n");
		if(copy_from_user(chr_devp->TK_uncali_x,buf,count))
		{
			xpt2046printDebug("copy_from_user  ERR\n");
			return -EFAULT;
		}
	}
    else if(chr_devp->TK_flag == 2)//save y
  	{
  		printk(KERN_ERR"write: enter save y.\n");
		if(copy_from_user(chr_devp->TK_uncali_y,buf,count))
		{
			xpt2046printDebug("copy_from_user  ERR\n");
			return -EFAULT;
		}
	}	
	else
		return -EFAULT;
	return 0;
}
int set_default_calib(void )
{
	unsigned char ret;
    int cali_num = 5;
    int screen_x[5], screen_y[5];
    int uncali_x[5], uncali_y[5];
    
    screen_x[0] = 15; screen_y[0] = 15;
    screen_x[1] = 15; screen_y[1] = 595;
    screen_x[2] = 785; screen_y[2] = 15;
    screen_x[3] = 785; screen_y[3] = 595;
    screen_x[4] = 400; screen_y[4] = 300;

    uncali_x[0] = 3780; uncali_y[0] = 3800;
    uncali_x[1] = 3780; uncali_y[1] = 360;
    uncali_x[2] = 250; uncali_y[2] = 3810;
    uncali_x[3] = 260; uncali_y[3] = 350;
    uncali_x[4] = 2020; uncali_y[4] = 2120;
	ret = TouchPanelSetCalibration(cali_num, screen_x, screen_y, uncali_x,uncali_y);
		 printk(KERN_ERR"tp_calib_iface_init---(%d,%d),(%d,%d),(%d,%d),(%d,%d),(%d,%d)\n",
				 uncali_x[0], uncali_y[0],
				 uncali_x[1], uncali_y[1],
				 uncali_x[2], uncali_y[2],
				 uncali_x[3], uncali_y[3],
				 uncali_x[4], uncali_y[4]);
	 if (ret == 1)
		 return 0;
	 else {
		 printk(KERN_ERR"Auto set calibraion failed!\n");	
		 return -1;
	 }
	
}
static long my_ioctl(struct file*filefp,unsigned int cmd,unsigned long arg)
 {
	 //xpt2046printDebug("%s:=====enter my_ioctl!!\n",__FUNCTION__);
	 if(cmd == SAVE_X)
	 {
		 chr_devp->TK_flag = 1;
		 return 0;
	 }
	 else if(cmd == SAVE_Y)
	 {
		 chr_devp->TK_flag = 2;
		 return 0;
	 }
	 else if(cmd == SET_Calibration)
	 {
		 unsigned char ret;
		 int cali_num = 5;
		 int screen_x[5], screen_y[5];
		 printk(KERN_ERR" to SET_Calibration!\n");
		 //user to set x/y value;
		 screen_x[0] = 25; screen_y[0] = 25;
		 screen_x[1] = 25; screen_y[1] = 575;
		 screen_x[2] = 775; screen_y[2] = 575;
		 screen_x[3] = 775; screen_y[3] = 25;
		 screen_x[4] = 400; screen_y[4] = 300;
		 ret = TouchPanelSetCalibration(cali_num, screen_x, screen_y, chr_devp->TK_uncali_x,chr_devp->TK_uncali_y);
		 printk(KERN_ERR"tp_calib_iface_init---(%d,%d),(%d,%d),(%d,%d),(%d,%d),(%d,%d)\n",
				 chr_devp->TK_uncali_x[0], chr_devp->TK_uncali_y[0],
				 chr_devp->TK_uncali_x[1], chr_devp->TK_uncali_y[1],
				 chr_devp->TK_uncali_x[2], chr_devp->TK_uncali_y[2],
				 chr_devp->TK_uncali_x[3], chr_devp->TK_uncali_y[3],
				 chr_devp->TK_uncali_x[4], chr_devp->TK_uncali_y[4]);
		 if (ret == 1){
			 printk(KERN_ERR"Auto set calibration successfully!!.\n");
			 return 0;
		 } else {
			 printk(KERN_ERR"touchpal calibration failed, use default value. !\n");
			 set_default_calib();	
			 return -1;
		 }
		 
	 }
	 else
		 return -1;  
 }



static struct file_operations tp_flops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.compat_ioctl = my_ioctl,
};



//--------------------zdx


struct ts_event {
	/* For portability, we can't read 12 bit values using SPI (which
	 * would make the controller deliver them as native byteorder u16
	 * with msbs zeroed).  Instead, we read them as two 8-bit values,
	 * *** WHICH NEED BYTESWAPPING *** and range adjustment.
	 */
	u16	x;
	u16	y;
	int	ignore;
};

/*
 * We allocate this separately to avoid cache line sharing issues when
 * driver is used with DMA-based SPI controllers (like atmel_spi) on
 * systems where main memory is not DMA-coherent (most non-x86 boards).
 */
struct xpt2046_packet {
	u8			read_x, read_y, pwrdown;
	u16			dummy;		/* for the pwrdown read */
	struct ts_event		tc;
};

struct xpt2046 {
	struct input_dev	*input;
	char	phys[32];
	char	name[32];
	char	pendown_iomux_name[IOMUX_NAME_SIZE];	
	struct spi_device	*spi;

	u16		model;
	u16		x_min, x_max;	
	u16		y_min, y_max; 
	u16		debounce_max;
	u16		debounce_tol;
	u16		debounce_rep;
	u16		penirq_recheck_delay_usecs;
	bool	swap_xy;

	struct xpt2046_packet	*packet;

	struct spi_transfer	xfer[18];
	struct spi_message	msg[5];
	struct spi_message	*last_msg;
	int		msg_idx;
	int		read_cnt;
	int		read_rep;
	int		last_read;
	int		pendown_iomux_mode;	
	int 	touch_virtualkey_length;
	
	spinlock_t		lock;
	struct hrtimer	timer;
	unsigned		pendown:1;	/* P: lock */
	unsigned		pending:1;	/* P: lock */
// FIXME remove "irq_disabled"
	unsigned		irq_disabled:1;	/* P: lock */
	unsigned		disabled:1;
	unsigned		is_suspended:1;

	int			(*filter)(void *data, int data_idx, int *val);
	void		*filter_data;
	void		(*filter_cleanup)(void *data);
	int			(*get_pendown_state)(void);
	int			gpio_pendown;

	void		(*wait_for_sync)(void);
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif
};

/* leave chip selected when we're done, for quicker re-select? */
#if	0
#define	CS_CHANGE(xfer)	((xfer).cs_change = 1)
#else
#define	CS_CHANGE(xfer)	((xfer).cs_change = 0)
#endif

/*--------------------------------------------------------------------------*/

/* The xpt2046 has touchscreen and other sensors.
 * Earlier xpt2046 chips are somewhat compatible.
 */
#define	XPT2046_START			(1 << 7)
#define	XPT2046_A2A1A0_d_y		(1 << 4)	/* differential */
#define	XPT2046_A2A1A0_d_z1		(3 << 4)	/* differential */
#define	XPT2046_A2A1A0_d_z2		(4 << 4)	/* differential */
#define	XPT2046_A2A1A0_d_x		(5 << 4)	/* differential */
#define	XPT2046_A2A1A0_temp0	(0 << 4)	/* non-differential */
#define	XPT2046_A2A1A0_vbatt	(2 << 4)	/* non-differential */
#define	XPT2046_A2A1A0_vaux		(6 << 4)	/* non-differential */
#define	XPT2046_A2A1A0_temp1	(7 << 4)	/* non-differential */
#define	XPT2046_8_BIT			(1 << 3)
#define	XPT2046_12_BIT			(0 << 3)
#define	XPT2046_SER				(1 << 2)	/* non-differential */
#define	XPT2046_DFR				(0 << 2)	/* differential */
#define	XPT2046_PD10_PDOWN		(0 << 0)	/* lowpower mode + penirq */
#define	XPT2046_PD10_ADC_ON		(1 << 0)	/* ADC on */
#define	XPT2046_PD10_REF_ON		(2 << 0)	/* vREF on + penirq */
#define	XPT2046_PD10_ALL_ON		(3 << 0)	/* ADC + vREF on */

#define	MAX_12BIT	((1<<12)-1)

/* leave ADC powered up (disables penirq) between differential samples */
#define	READ_12BIT_DFR(x, adc, vref) (XPT2046_START | XPT2046_A2A1A0_d_ ## x \
	| XPT2046_12_BIT | XPT2046_DFR | \
	(adc ? XPT2046_PD10_ADC_ON : 0) | (vref ? XPT2046_PD10_REF_ON : 0))

#define	READ_Y(vref)	(READ_12BIT_DFR(y,  1, vref))
//#define	READ_Y(vref)	(READ_12BIT_DFR(y,  0, 0))

#define	READ_Z1(vref)	(READ_12BIT_DFR(z1, 1, vref))
#define	READ_Z2(vref)	(READ_12BIT_DFR(z2, 1, vref))

//#define	READ_X(vref)	(READ_12BIT_DFR(x,  0, 0))
#define	READ_X(vref)	(READ_12BIT_DFR(x,  1, vref))

#define	PWRDOWN		(READ_12BIT_DFR(y,  0, 0))	/* LAST */

/* single-ended samples need to first power up reference voltage;
 * we leave both ADC and VREF powered
 */
#define	READ_12BIT_SER(x) (XPT2046_START | XPT2046_A2A1A0_ ## x \
	| XPT2046_12_BIT | XPT2046_SER)

#define	REF_ON	(READ_12BIT_DFR(x, 1, 1))
#define	REF_OFF	(READ_12BIT_DFR(y, 0, 0))



#define DEBOUNCE_MAX 10
#define DEBOUNCE_REP 5
#define DEBOUNCE_TOL 10
#define PENIRQ_RECHECK_DELAY_USECS 1
#define PRESS_MAX 255


/*--------------------------------------------------------------------------*/
/*
 * touchscreen sensors  use differential conversions.
 */

struct dfr_req {
	u8			command;
	u8			pwrdown;
	u16			dummy;		/* for the pwrdown read */
	__be16			sample;
	struct spi_message	msg;
	struct spi_transfer	xfer[4];
};

volatile struct adc_point gADPoint;
volatile int gFirstIrq;

//static void xpt2046_enable(struct xpt2046 *ts);
//static void xpt2046_disable(struct xpt2046 *ts);
static int xpt2046_verifyAndConvert(struct xpt2046 *ts,int adx, int ady,int *x, int *y)
{

	xpt2046printInfo("%s:(%d,%d)(%d/%d)\n",__FUNCTION__, adx, ady, *x, *y);
	
	if((*x< ts->x_min) || (*x > ts->x_max))
		return 1;

	if((*y< ts->y_min) || (*y > ts->y_max + ts->touch_virtualkey_length))
		return 1;


	return 0;
}
static int device_suspended(struct device *dev)
{
	struct xpt2046 *ts = dev_get_drvdata(dev);
	return ts->is_suspended || ts->disabled;
}

static int xpt2046_read12_dfr(struct device *dev, unsigned command)
{
	struct spi_device	*spi = to_spi_device(dev);
	struct xpt2046		*ts = dev_get_drvdata(dev);
	struct dfr_req		*req = kzalloc(sizeof *req, GFP_KERNEL);
	int			status;

	if (!req)
		return -ENOMEM;

	spi_message_init(&req->msg);

	/* take sample */
	req->command = (u8) command;
	req->xfer[0].tx_buf = &req->command;
	req->xfer[0].len = 1;
	spi_message_add_tail(&req->xfer[0], &req->msg);

	req->xfer[1].rx_buf = &req->sample;
	req->xfer[1].len = 2;
	spi_message_add_tail(&req->xfer[1], &req->msg);

	/* converter in low power mode & enable PENIRQ */
	req->pwrdown= PWRDOWN;
	req->xfer[2].tx_buf = &req->pwrdown;
	req->xfer[2].len = 1;
	spi_message_add_tail(&req->xfer[2], &req->msg);

	req->xfer[3].rx_buf = &req->dummy;
	req->xfer[3].len = 2;
	CS_CHANGE(req->xfer[3]);
	spi_message_add_tail(&req->xfer[3], &req->msg);

	ts->irq_disabled = 1;
	disable_irq(spi->irq);
	status = spi_sync(spi, &req->msg);
	ts->irq_disabled = 0;
	enable_irq(spi->irq);
	
	if (status == 0) {
		/* on-wire is a must-ignore bit, a BE12 value, then padding */
		status = be16_to_cpu(req->sample);
		status = status >> 3;
		status &= 0x0fff;
		xpt2046printInfo("***>%s:status=%d\n",__FUNCTION__,status);
	}

	kfree(req);
	return status;
}



/*--------------------------------------------------------------------------*/

static int get_pendown_state(struct xpt2046 *ts)
{
	if (ts->get_pendown_state)
		return ts->get_pendown_state();

	return !gpio_get_value(ts->gpio_pendown);
}

static void null_wait_for_sync(void)
{
	
}

/*
 * PENIRQ only kicks the timer.  The timer only reissues the SPI transfer,
 * to retrieve touchscreen status.
 *
 * The SPI transfer completion callback does the real work.  It reports
 * touchscreen events and reactivates the timer (or IRQ) as appropriate.
 */
static void xpt2046_rx(void *xpt)
{
	struct xpt2046		*ts = xpt;
	struct xpt2046_packet	*packet = ts->packet;
	unsigned		Rt = 1;
	u16			x, y;
	int 		cal_x,cal_y;
	/* xpt2046_rx_val() did in-place conversion (including byteswap) from
	 * on-the-wire format as part of debouncing to get stable readings.
	 */
	x = packet->tc.x;
	y = packet->tc.y;

	//printk(KERN_ERR"***>%s:x=%d,y=%d\n",__FUNCTION__,x,y);

	/* range filtering */
	if (x == MAX_12BIT)
		x = 0;

	/* Sample found inconsistent by debouncing or pressure is beyond
	 * the maximum. Don't report it to user space, repeat at least
	 * once more the measurement
	 */
	if (packet->tc.ignore) {

		xpt2046printInfo("%s:ignored=%d\n",__FUNCTION__,packet->tc.ignore);
	
		hrtimer_start(&ts->timer, ktime_set(0, TS_POLL_PERIOD),
			      HRTIMER_MODE_REL);
		return;
	}

	/* Maybe check the pendown state before reporting. This discards
	 * false readings when the pen is lifted.
	 */
	if (ts->penirq_recheck_delay_usecs) {
		udelay(ts->penirq_recheck_delay_usecs);
		if (!get_pendown_state(ts))
		{
			xpt2046printInfo("%s:get_pendown_state(ts)==0,discard false reading\n",__FUNCTION__);
			Rt = 0;
		}
	}

	/* NOTE: We can't rely on the pressure to determine the pen down
	 * state, even this controller has a pressure sensor.  The pressure
	 * value can fluctuate for quite a while after lifting the pen and
	 * in some cases may not even settle at the expected value.
	 *
	 * The only safe way to check for the pen up condition is in the
	 * timer by reading the pen signal state (it's a GPIO _and_ IRQ).
	 */
	if (Rt) {
		struct input_dev *input = ts->input;
		
		TouchPanelCalibrateAPoint(x, y, &cal_x, &cal_y);
    
    	cal_x = cal_x / 4;
    	cal_y = cal_y / 4;
		gADPoint.x = x;
        gADPoint.y = y;
		
		if (ts->swap_xy)
			swap(cal_x, cal_y);	
		
		if(xpt2046_verifyAndConvert(ts,cal_x,cal_y,&cal_x,&cal_y))
		{
			xpt2046printInfo("%s:xpt2046_verifyAndConvert fail\n",__FUNCTION__);
			goto out;
		}
		
		if (!ts->pendown) {
			input_report_key(input, BTN_TOUCH, 1);
			ts->pendown = 1;
			xpt2046printDebug("%s:input_report_key(pen down)\n",__FUNCTION__);
		}	
#if	1	
		input_report_abs(input, ABS_X, cal_x);
		input_report_abs(input, ABS_Y, cal_y);

		input_sync(input);
#else
		input_report_abs(input, ABS_MT_PRESSURE, 10);
		input_report_abs(input, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(input, ABS_MT_POSITION_X, cal_x);
		input_report_abs(input, ABS_MT_POSITION_Y, cal_x);
		input_mt_sync(input);
		
		input_sync(ts->input);
#endif
		xpt2046printDebug("%s:input_report_abs(%4d/%4d)\n",__FUNCTION__,cal_x, cal_y);
	}
out:
	hrtimer_start(&ts->timer, ktime_set(0, TS_POLL_PERIOD),
			HRTIMER_MODE_REL);
}
#if 1
static int xpt2046_debounce(void *xpt, int data_idx, int *val)
{
	struct xpt2046		*ts = xpt;
	static int average_val[2];
	

	xpt2046printInfo("%s:%d,%d,%d,%d,%d,%d,%d,%d\n",__FUNCTION__,
		data_idx,ts->last_read,
	  ts->read_cnt,ts->debounce_max,
		abs(ts->last_read - *val),ts->debounce_tol,
		ts->read_rep,ts->debounce_rep);
	
	/* discard the first sample. */
	 //on info_it50, the top-left area(1cmx1cm top-left square ) is not responding cause the first sample is invalid, @sep 17th
	if(!ts->read_cnt)
	{
		//udelay(100);
		ts->read_cnt++;
		return XPT2046_FILTER_REPEAT;
	}
	if(*val == 4095 || *val == 0)
	{
		ts->read_cnt = 0;
		ts->last_read = 0;
		memset(average_val,0,sizeof(average_val));
		xpt2046printDebug("%s:*val == 4095 || *val == 0\n",__FUNCTION__);
		return XPT2046_FILTER_IGNORE;
	}


	if (ts->read_cnt==1 || (abs(ts->last_read - *val) > ts->debounce_tol)) {
		/* Start over collecting consistent readings. */
		ts->read_rep = 1;
		average_val[data_idx] = *val;
		/* Repeat it, if this was the first read or the read
		 * wasn't consistent enough. */
		if (ts->read_cnt < ts->debounce_max) {
			ts->last_read = *val;
			ts->read_cnt++;
			return XPT2046_FILTER_REPEAT;
		} else {
			/* Maximum number of debouncing reached and still
			 * not enough number of consistent readings. Abort
			 * the whole sample, repeat it in the next sampling
			 * period.
			 */
			ts->read_cnt = 0;
			ts->last_read = 0;
			memset(average_val,0,sizeof(average_val));
			xpt2046printDebug("%s:XPT2046_FILTER_IGNORE\n",__FUNCTION__);
			return XPT2046_FILTER_IGNORE;
		}
	} 
	else {
		average_val[data_idx] += *val;
		
		if (++ts->read_rep >= ts->debounce_rep) {
			/* Got a good reading for this coordinate,
			 * go for the next one. */
			ts->read_cnt = 0;
			ts->read_rep = 0;
			ts->last_read = 0;
			*val = average_val[data_idx]/(ts->debounce_rep);
			return XPT2046_FILTER_OK;
		} else {
			/* Read more values that are consistent. */
			ts->read_cnt++;
			
			return XPT2046_FILTER_REPEAT;
		}
	}
}
#else
static int xpt2046_no_filter(void *xpt, int data_idx, int *val)
{
	return XPT2046_FILTER_OK;
}
#endif
//#define spi_async(a,b)
static void xpt2046_rx_val(void *xpt)
{
	struct xpt2046 *ts = xpt;
	struct xpt2046_packet *packet = ts->packet;
	struct spi_message *m;
	struct spi_transfer *t;
	int val;
	int action;
	int status;
	
	m = &ts->msg[ts->msg_idx];
	t = list_entry(m->transfers.prev, struct spi_transfer, transfer_list);

	/* adjust:  on-wire is a must-ignore bit, a BE12 value, then padding;
	 * built from two 8 bit values written msb-first.
	 */
	val = (be16_to_cpup((__be16 *)t->rx_buf) >> 3) & 0x0fff;

	xpt2046printInfo("%s:value=%d\n",__FUNCTION__,val);
	
	action = ts->filter(ts->filter_data, ts->msg_idx, &val);
	switch (action) {
	case XPT2046_FILTER_REPEAT:
		xpt2046printInfo("%s:XPT2046_FILTER_REPEAT:value=%d\n",__FUNCTION__,val);
		break;
	case XPT2046_FILTER_IGNORE:
		packet->tc.ignore = 1;
		/* Last message will contain xpt2046_rx() as the
		 * completion function.
		 */
		 xpt2046printInfo("%s:XPT2046_FILTER_IGNORE:value=%d\n",__FUNCTION__,val);
		m = ts->last_msg;
		break;
	case XPT2046_FILTER_OK:
		*(u16 *)t->rx_buf = val;
		packet->tc.ignore = 0;
		m = &ts->msg[++ts->msg_idx];
		xpt2046printInfo("%s:XPT2046_FILTER_OK:value=%d\n",__FUNCTION__,val);
		break;
	default:
		BUG();
	}
	ts->wait_for_sync();
	status = spi_async(ts->spi, m);
	if (status)
		dev_err(&ts->spi->dev, "spi_async --> %d\n",
				status);
}

static enum hrtimer_restart xpt2046_timer(struct hrtimer *handle)
{
	struct xpt2046	*ts = container_of(handle, struct xpt2046, timer);
	int		status = 0;
	
	spin_lock(&ts->lock);

	if (unlikely(!get_pendown_state(ts) ||
		     device_suspended(&ts->spi->dev))) {
		if (ts->pendown) {
			struct input_dev *input = ts->input;
			input_report_key(input, BTN_TOUCH, 0);
			input_sync(input);
			//input_report_abs(input, ABS_MT_TOUCH_MAJOR, 0);
			//input_mt_sync(input);
			//input_sync(ts->input);

			ts->pendown = 0;
			gFirstIrq = 1;
			
			xpt2046printDebug("%s:input_report_key(The touchscreen up)\n",__FUNCTION__);
		}

		/* measurement cycle ended */
		if (!device_suspended(&ts->spi->dev)) {
			xpt2046printInfo("%s:device_suspended==0\n",__FUNCTION__);
			ts->irq_disabled = 0;
			enable_irq(ts->spi->irq);
		}
		ts->pending = 0;
	} else {
		if(gFirstIrq)
		{
			xpt2046printDebug("%s:delay 15 ms!!!\n",__FUNCTION__);
			gFirstIrq = 0;
			mdelay(15);
			xpt2046printDebug("%s:end delay!!!\n",__FUNCTION__);
		}
		/* pen is still down, continue with the measurement */
		xpt2046printInfo("%s:pen is still down, continue with the measurement\n",__FUNCTION__);
		ts->msg_idx = 0;
		ts->wait_for_sync();
		status = spi_async(ts->spi, &ts->msg[0]);
		if (status)
			dev_err(&ts->spi->dev, "spi_async --> %d\n", status);
	}

	spin_unlock(&ts->lock);
	return HRTIMER_NORESTART;
}

static irqreturn_t xpt2046_irq(int irq, void *handle)
{
	struct xpt2046 *ts = handle;
	unsigned long flags;
	
	xpt2046printInfo("%s.....%s.....%d\n",__FILE__,__FUNCTION__,__LINE__);
	
	spin_lock_irqsave(&ts->lock, flags);

	if (likely(get_pendown_state(ts))) {
		if (!ts->irq_disabled) {
			/* The ARM do_simple_IRQ() dispatcher doesn't act
			 * like the other dispatchers:  it will report IRQs
			 * even after they've been disabled.  We work around
			 * that here.  (The "generic irq" framework may help...)
			 */
			ts->irq_disabled = 1;
			disable_irq_nosync(ts->spi->irq);
			ts->pending = 1;
			hrtimer_start(&ts->timer, ktime_set(0, TS_POLL_DELAY),
					HRTIMER_MODE_REL);
		}
	}
	spin_unlock_irqrestore(&ts->lock, flags);

	return IRQ_HANDLED;
}

/*--------------------------------------------------------------------------*/
#if 0
/* Must be called with ts->lock held */
static void xpt2046_disable(struct xpt2046 *ts)
{
	if (ts->disabled)
		return;

	ts->disabled = 1;

	/* are we waiting for IRQ, or polling? */
	if (!ts->pending) {
		ts->irq_disabled = 1;
		disable_irq(ts->spi->irq);
	} else {
		/* the timer will run at least once more, and
		 * leave everything in a clean state, IRQ disabled
		 */
		while (ts->pending) {
			spin_unlock_irq(&ts->lock);
			msleep(1);
			spin_lock_irq(&ts->lock);
		}
	}

	/* we know the chip's in lowpower mode since we always
	 * leave it that way after every request
	 */
}

/* Must be called with ts->lock held */
static void xpt2046_enable(struct xpt2046 *ts)
{
	if (!ts->disabled)
		return;

	ts->disabled = 0;
	ts->irq_disabled = 0;
	enable_irq(ts->spi->irq);
}

static int xpt2046_pSuspend(struct xpt2046 *ts)
{
	spin_lock_irq(&ts->lock);

	ts->is_suspended = 1;
	xpt2046_disable(ts);

	spin_unlock_irq(&ts->lock);

	return 0;
}

static int xpt2046_pResume(struct xpt2046 *ts)
{
	spin_lock_irq(&ts->lock);

	ts->is_suspended = 0;
	xpt2046_enable(ts);

	spin_unlock_irq(&ts->lock);

	return 0;
}
#endif

#if !defined(CONFIG_HAS_EARLYSUSPEND)
/*static int xpt2046_suspend(struct spi_device *spi, pm_message_t message)
{
	struct xpt2046 *ts = dev_get_drvdata(&spi->dev);
	
	printk("xpt2046_suspend\n");
	
	xpt2046_pSuspend(ts);
	
	return 0;
}

static int xpt2046_resume(struct spi_device *spi)
{
	struct xpt2046 *ts = dev_get_drvdata(&spi->dev);
	
	printk("xpt2046_resume\n");
	
	xpt2046_pResume(ts);

	return 0;
}*/

#elif defined(CONFIG_HAS_EARLYSUSPEND)
static void xpt2046_early_suspend(struct early_suspend *h)
{
	struct xpt2046	*ts;
    ts = container_of(h, struct xpt2046, early_suspend);
	
	printk("xpt2046_suspend early\n");
	
	xpt2046_pSuspend(ts);
	
	return;
}

static void xpt2046_late_resume(struct early_suspend *h)
{
	struct xpt2046	*ts;
    ts = container_of(h, struct xpt2046, early_suspend);
	
	printk("xpt2046_resume late\n");
	
	xpt2046_pResume(ts);

	return;
}
#endif

static int setup_pendown(struct spi_device *spi, struct xpt2046 *ts)
{
	//struct xpt2046_platform_data *pdata = spi->dev.platform_data;
	int err;

	/* REVISIT when the irq can be triggered active-low, or if for some
	 * reason the touchscreen isn't hooked up, we don't need to access
	 * the pendown state.
	 */
	if (!gpio_is_valid(ts->gpio_pendown/*pdata->gpio_pendown*/)) {
		dev_err(&spi->dev, "no get_pendown_state nor gpio_pendown?\n");
		return -EINVAL;
	}

	/*if (pdata->get_pendown_state) {
		ts->get_pendown_state = pdata->get_pendown_state;
		return 0;
	}*/

	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);

	err = gpio_request(ts->gpio_pendown, "xpt2046_pendown");
	if (err) {
		dev_err(&spi->dev, "failed to request pendown GPIO%d\n",
				ts->gpio_pendown);
		return err;
	}


	err = gpio_direction_input(ts->gpio_pendown);
	if (err) {
		dev_err(&spi->dev, "failed to set input pendown GPIO%d\n",
				ts->gpio_pendown);
		return err;
	}	
	
	//ts->gpio_pendown = pdata->gpio_pendown;
	//strcpy(ts->pendown_iomux_name,pdata->pendown_iomux_name);
	//ts->pendown_iomux_mode = pdata->pendown_iomux_mode;
	
        /*rk29_mux_api_set(ts->pendown_iomux_name,pdata->pendown_iomux_mode);
	err = gpio_request(pdata->gpio_pendown, "xpt2046_pendown");
	if (err) {
		dev_err(&spi->dev, "failed to request pendown GPIO%d\n",
				pdata->gpio_pendown);
		return err;
	}
	
	err = gpio_pull_updown(pdata->gpio_pendown, GPIOPullUp);
	if (err) {
		dev_err(&spi->dev, "failed to pullup pendown GPIO%d\n",
				pdata->gpio_pendown);
		return err;
	}
	ts->gpio_pendown = pdata->gpio_pendown;*/
	
	return 0;
}

/*
static ssize_t xpt2046_set_calibration_value(struct class *class,
		struct class_attribute *attr, const char *buf, size_t count)
{
	int i=0;
	char orientation[20];
	char *tmp;

	struct sensor_private_data *sensor = g_sensor[SENSOR_TYPE_ACCEL];
	struct sensor_platform_data *pdata = sensor->pdata;


  	char *p = strstr(buf,"gsensor_class");
	int start = strcspn(p,"{");
	int end = strcspn(p,"}");

	strncpy(orientation,p+start,end-start+1);
	tmp = orientation;


    	while(strncmp(tmp,"}",1)!=0)
   	 {
    		if((strncmp(tmp,",",1)==0)||(strncmp(tmp,"{",1)==0))
		{

			 tmp++;
			 continue;
		}
		else if(strncmp(tmp,"-",1)==0)
		{
			pdata->orientation[i++]=-1;
			DBG("i=%d,data=%d\n",i,pdata->orientation[i]);
			 tmp++;
		}
		else
		{
			pdata->orientation[i++]=tmp[0]-48;
			DBG("----i=%d,data=%d\n",i,pdata->orientation[i]);
		}
		tmp++;


   	 }

	for(i=0;i<9;i++)
		DBG("i=%d gsensor_info=%d\n",i,pdata->orientation[i]);
	return 0;

}


static CLASS_ATTR(calibration, 0660, NULL, xpt2046_set_calibration_value);

static int  xpt2046_class_init(void)
{
	int ret ;
	struct sensor_private_data *sensor = g_sensor[SENSOR_TYPE_ACCEL];
	g_sensor_class[SENSOR_TYPE_ACCEL] = class_create(THIS_MODULE, "gsensor_class");
	ret =  class_create_file(g_sensor_class[SENSOR_TYPE_ACCEL], &class_attr_orientation);
	if (ret)
	{
		printk("%s:Fail to creat class\n",__func__);
		return ret;
	}
	printk("%s:%s\n",__func__,sensor->i2c_id->name);
	return 0;
}
*/

int tp_calib_init(void)
{
    unsigned char ret;
    int cali_num = 5;
    int screen_x[5], screen_y[5];
    int uncali_x[5], uncali_y[5];
    int tst_uncali_x, tst_uncali_y, tst_cali_x, tst_cali_y;
    
    screen_x[0] = 15; screen_y[0] = 15;
    screen_x[1] = 15; screen_y[1] = 595;
    screen_x[2] = 785; screen_y[2] = 15;
    screen_x[3] = 785; screen_y[3] = 595;
    screen_x[4] = 400; screen_y[4] = 300;
  #if 0
    uncali_x[0] = 3810; uncali_y[0] = 3830;
    uncali_x[1] = 3810; uncali_y[1] = 340;
    uncali_x[2] = 240; uncali_y[2] = 3830;
    uncali_x[3] = 240; uncali_y[3] = 335;
    uncali_x[4] = 2028; uncali_y[4] = 2130;

  uncali_x[0] = 3180; uncali_y[0] = 3495;
 uncali_x[1] = 3795; uncali_y[1] = 630;
 uncali_x[2] = 220; uncali_y[2] = 3410;
 uncali_x[3] = 940; uncali_y[3] = 768;
 uncali_x[4] = 1950; uncali_y[4] = 2100;

 #else
     uncali_x[0] = 3780; uncali_y[0] = 3800;
    uncali_x[1] = 3780; uncali_y[1] = 360;
    uncali_x[2] = 250; uncali_y[2] = 3810;
    uncali_x[3] = 260; uncali_y[3] = 350;
    uncali_x[4] = 2020; uncali_y[4] = 2120;
	
 #endif
    ret = tp_calib_iface_init(cali_num, screen_x, screen_y, uncali_x, uncali_y);
	if(ret ==0)
	{
		 xpt2046printDebug("TouchPanelSetCalibration OK.\n");
	}
	else
	{
		 xpt2046printDebug("TouchPanelSetCalibration FAIL.\n");
	}

    tst_uncali_x = 2028;
    tst_uncali_y = 2130;
    
    TouchPanelCalibrateAPoint(tst_uncali_x, tst_uncali_y,
                              &tst_cali_x, &tst_cali_y);
    
    xpt2046printDebug("(%d, %d) >> (%d, %d)\n", tst_uncali_x, tst_uncali_y,
                                     tst_cali_x/4, tst_cali_y/4);
    
    tst_uncali_x = 400;
    tst_uncali_y = 400;
    
    TouchPanelCalibrateAPoint(tst_uncali_x, tst_uncali_y,
                              &tst_cali_x, &tst_cali_y);
    
    xpt2046printDebug("(%d, %d) >> (%d, %d)\n", tst_uncali_x, tst_uncali_y,
                                     tst_cali_x/4, tst_cali_y/4);

    tst_uncali_x = 3800;
    tst_uncali_y = 3800;
    
    TouchPanelCalibrateAPoint(tst_uncali_x, tst_uncali_y,
                              &tst_cali_x, &tst_cali_y);
    
    xpt2046printDebug("(%d, %d) >> (%d, %d)\n", tst_uncali_x, tst_uncali_y,
                                     tst_cali_x/4, tst_cali_y/4);

    tst_uncali_x = 400;
    tst_uncali_y = 3800;
    
    TouchPanelCalibrateAPoint(tst_uncali_x, tst_uncali_y,
                              &tst_cali_x, &tst_cali_y);
    
    xpt2046printDebug("(%d, %d) >> (%d, %d)\n", tst_uncali_x, tst_uncali_y,
                                     tst_cali_x/4, tst_cali_y/4);

    tst_uncali_x = 3800;
    tst_uncali_y = 400;
    
    TouchPanelCalibrateAPoint(tst_uncali_x, tst_uncali_y,
                              &tst_cali_x, &tst_cali_y);
    
    xpt2046printDebug("(%d, %d) >> (%d, %d)\n", tst_uncali_x, tst_uncali_y,
                                     tst_cali_x/4, tst_cali_y/4);

	return 0;
									 
}
static int tp_init(void )
{
	int res;
	chr_devp = (struct chr_dev *)kmalloc(sizeof(struct chr_dev ), GFP_KERNEL);

	//xpt2046printDebug("==>tp_init!\n");
	res = alloc_chrdev_region(&chr_devp->dev_nr, 0, 1, "tp_chrdev");
	if(res){
    	xpt2046printDebug("==>alloc chrdev region failed!\n");
        goto chrdev_err;
	} 
	//xpt2046printDebug("==>alloc_chrdev_region!\n");
	cdev_init(&chr_devp->tp_cdev, &tp_flops);
	//xpt2046printDebug("==>cdev_init!\n");
	res = cdev_add(&chr_devp->tp_cdev, chr_devp->dev_nr, DEVICE_COUNT);
	if(res){
		xpt2046printDebug("==>cdev add failed!\n");
        goto cdev_err;
	}
	//xpt2046printDebug("==>cdev_add!\n");

	
	chr_devp->tp_class = class_create(THIS_MODULE,"tp_class");
	if(IS_ERR(&chr_devp->tp_class)){
	 	 res =  PTR_ERR(chr_devp->tp_class);
    goto class_err;
	}
	//xpt2046printDebug("==>class_create!\n");

	
	chr_devp->tp_device = device_create(chr_devp->tp_class,NULL, chr_devp->dev_nr, NULL,"tp_device");
	if(IS_ERR(&chr_devp->tp_device)){
   	   	res = PTR_ERR(&chr_devp->tp_device);
       	goto device_err;
    }
	//xpt2046printDebug("==>device_create!\n");


    //xpt2046printDebug("==>tp begin.\n");
	xpt2046printDebug(DEVICE_NAME " initialized.\n");
	
	return 0;

device_err:
	device_destroy(chr_devp->tp_class, chr_devp->dev_nr);
	class_destroy(chr_devp->tp_class);

class_err:
	cdev_del(&chr_devp->tp_cdev); 

cdev_err:
	unregister_chrdev_region(chr_devp->dev_nr, DEVICE_COUNT);

chrdev_err:

	return res;

}
static int xpt2046_probe(struct spi_device *spi)
{
	struct xpt2046			*ts;
	struct xpt2046_packet		*packet;
	struct input_dev		*input_dev;
	//struct xpt2046_platform_data	*pdata = spi->dev.platform_data;
	struct spi_message		*m;
	struct spi_transfer		*x;
	int				vref;
	int				err;
	struct device_node *np = spi->dev.of_node;
	unsigned long irq_flags;

	spi->irq = of_get_named_gpio_flags(np, "touch-gpio", 0, (enum of_gpio_flags *)&irq_flags);

	printk(KERN_ERR "===============%s:%d:spi->irq=%d==============\n", __func__, __LINE__,spi->irq);
	
    
	/* don't exceed max specified sample rate */
	if (spi->max_speed_hz > (125000 * SAMPLE_BITS)) {
		xpt2046printDebug("f(sample) %d KHz?\n",
				(spi->max_speed_hz/SAMPLE_BITS)/1000);
		return -EINVAL;
	}

	/* We'd set TX wordsize 8 bits and RX wordsize to 13 bits ... except
	 * that even if the hardware can do that, the SPI controller driver
	 * may not.  So we stick to very-portable 8 bit words, both RX and TX.
	 */
	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	err = spi_setup(spi);
	if (err < 0)
		return err;

	ts = kzalloc(sizeof(struct xpt2046), GFP_KERNEL);
	packet = kzalloc(sizeof(struct xpt2046_packet), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!ts || !packet || !input_dev) {
		err = -ENOMEM;
		goto err_free_mem;
	}

	dev_set_drvdata(&spi->dev, ts);
	
	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);

	ts->packet = packet;
	ts->spi = spi;
	ts->input = input_dev;
	ts->swap_xy =0;// pdata->swap_xy;

	
	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);

	gFirstIrq = 1;
	ts->gpio_pendown = spi->irq;
	err = setup_pendown(spi, ts);
	if (err)
		goto err_free_mem;

	if (!spi->irq) {
		dev_err(&spi->dev, "%s:no IRQ?\n", __func__);
		goto err_free_mem;
	}
	else{
		spi->irq = gpio_to_irq(spi->irq);
		dev_err(&spi->dev, "%s:IRQ=%d\n", __func__, spi->irq);
	}
	
	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);

	hrtimer_init(&ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ts->timer.function = xpt2046_timer;

	spin_lock_init(&ts->lock);

	ts->model = 2046;
	
	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);
	
	/*if (pdata->filter != NULL) {
		if (pdata->filter_init != NULL) {
			err = pdata->filter_init(pdata, &ts->filter_data);
			if (err < 0)
				goto err_free_mem;
		}
		ts->filter = pdata->filter;
		ts->filter_cleanup = pdata->filter_cleanup;
	} else if (pdata->debounce_max) {
		ts->debounce_max = pdata->debounce_max;
		if (ts->debounce_max < pdata->debounce_rep)
			ts->debounce_max = pdata->debounce_rep;
		ts->debounce_tol = pdata->debounce_tol;
		ts->debounce_rep = pdata->debounce_rep;
		ts->filter = xpt2046_debounce;
		ts->filter_data = ts;
	} else
		ts->filter = xpt2046_no_filter;*/

	ts->debounce_max = DEBOUNCE_MAX;
	if (ts->debounce_max < DEBOUNCE_REP)
		ts->debounce_max = DEBOUNCE_REP;
	ts->debounce_tol = DEBOUNCE_TOL;
	ts->debounce_rep = DEBOUNCE_REP;
	ts->filter = xpt2046_debounce;
	ts->filter_data = ts;	
	ts->penirq_recheck_delay_usecs = PENIRQ_RECHECK_DELAY_USECS;


	/*if (pdata->penirq_recheck_delay_usecs)
		ts->penirq_recheck_delay_usecs =
				pdata->penirq_recheck_delay_usecs;*/

	ts->wait_for_sync = null_wait_for_sync;
	ts->x_min = 0;//pdata->x_min;
	ts->x_max =SCREEN_MAX_X;// pdata->x_max;
	ts->y_min =0;// pdata->y_min;
	ts->y_max =SCREEN_MAX_Y;// pdata->y_max;
	

	ts->touch_virtualkey_length = 0;
	
	snprintf(ts->phys, sizeof(ts->phys), "%s/input0", dev_name(&spi->dev));
	snprintf(ts->name, sizeof(ts->name), "xpt%d-touchscreen", ts->model);

	input_dev->name = ts->name;
	input_dev->phys = ts->phys;
	input_dev->dev.parent = &spi->dev;
#if 1
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	input_set_abs_params(input_dev, ABS_X,
			ts->x_min ? : 0,
			ts->x_max ? : MAX_12BIT,
			0, 0);
	input_set_abs_params(input_dev, ABS_Y,
			ts->y_min ? : 0,
			ts->y_max ? : MAX_12BIT,
			0, 0);
#else
		set_bit(EV_ABS, input_dev->evbit);

		input_set_abs_params(input_dev,ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
		input_set_abs_params(input_dev,ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
		input_set_abs_params(input_dev,ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_PRESSURE, 0, 255, 0, 0);

#endif
	vref = 1;
	
	//printk(KERN_ERR "===============%s:%d==============\n", __func__, __LINE__);

	/* set up the transfers to read touchscreen state; this assumes we
	 * use formula #2 for pressure, not #3.
	 */
	m = &ts->msg[0];
	x = ts->xfer;

	spi_message_init(m);

	/* y- still on; turn on only y+ (and ADC) */
	packet->read_y = READ_Y(vref);
	x->tx_buf = &packet->read_y;
	x->len = 1;
	spi_message_add_tail(x, m);

	x++;
	x->rx_buf = &packet->tc.y;
	x->len = 2;
	spi_message_add_tail(x, m);

	m->complete = xpt2046_rx_val;
	m->context = ts;

	m++;
	spi_message_init(m);

	/* turn y- off, x+ on, then leave in lowpower */
	x++;
	packet->read_x = READ_X(vref);
	x->tx_buf = &packet->read_x;
	x->len = 1;
	spi_message_add_tail(x, m);

	x++;
	x->rx_buf = &packet->tc.x;
	x->len = 2;
	spi_message_add_tail(x, m);

	m->complete = xpt2046_rx_val;
	m->context = ts;

	/* power down */
	m++;
	spi_message_init(m);

	x++;
	packet->pwrdown = PWRDOWN;
	x->tx_buf = &packet->pwrdown;
	x->len = 1;
	spi_message_add_tail(x, m);

	x++;
	x->rx_buf = &packet->dummy;
	x->len = 2;
	CS_CHANGE(*x);
	spi_message_add_tail(x, m);

	m->complete = xpt2046_rx;
	m->context = ts;

	ts->last_msg = m;

	if (request_irq(spi->irq, xpt2046_irq, IRQF_TRIGGER_FALLING,
			spi->dev.driver->name, ts)) {
		xpt2046printDebug("%s:trying pin change workaround on irq %d\n",__FUNCTION__,spi->irq);
		err = request_irq(spi->irq, xpt2046_irq,
				  IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				  spi->dev.driver->name, ts);
		if (err) {
			dev_dbg(&spi->dev, "irq %d busy?\n", spi->irq);
			goto err_free_gpio;
		}
	}
	xpt2046printInfo("%s:touchscreen irq %d\n",__FUNCTION__,spi->irq);
	
	/* take a first sample, leaving nPENIRQ active and vREF off; avoid
	 * the touchscreen, in case it's not connected.
	 */
	xpt2046_read12_dfr(&spi->dev,READ_X(1));

	err = input_register_device(input_dev);
	if (err)
		goto err_remove_attr_group;
	
#ifdef CONFIG_HAS_EARLYSUSPEND
	ts->early_suspend.suspend = xpt2046_early_suspend;
	ts->early_suspend.resume = xpt2046_late_resume;
	ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	register_early_suspend(&ts->early_suspend);
#endif

	tp_calib_init();
	//zdx add
	tp_init();
	xpt2046printDebug("======xpt2046_tp initerface: driver initialized======\n");
    xpt2046printDebug("======xpt2046_ts: driver initialized======\n");
	return 0;

 err_remove_attr_group:
	free_irq(spi->irq, ts);
 err_free_gpio:
	if (ts->gpio_pendown != -1)
		gpio_free(ts->gpio_pendown);
 //err_cleanup_filter:
//	if (ts->filter_cleanup)
//		ts->filter_cleanup(ts->filter_data);
 err_free_mem:
	input_free_device(input_dev);
	kfree(packet);
	kfree(ts);
	return err;
}


static int xpt2046_remove(struct spi_device *spi)
{
	struct xpt2046		*ts = dev_get_drvdata(&spi->dev);

	input_unregister_device(ts->input);
	
#if !defined(CONFIG_HAS_EARLYSUSPEND)
	//xpt2046_suspend(spi, PMSG_SUSPEND);
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	xpt2046_early_suspend(&ts->early_suspend);
	unregister_early_suspend(&ts->early_suspend);
#endif

	free_irq(ts->spi->irq, ts);
	/* suspend left the IRQ disabled */
	enable_irq(ts->spi->irq);

	if (ts->gpio_pendown != -1)
		gpio_free(ts->gpio_pendown);

	if (ts->filter_cleanup)
		ts->filter_cleanup(ts->filter_data);

	kfree(ts->packet);
	kfree(ts);
	
	//tp_calib_iface_exit();
	
	dev_dbg(&spi->dev, "unregistered touchscreen\n");
	return 0;
}

static struct of_device_id xpt2046_dt_ids[] = {
	{ .compatible = "tecom,xpt2046" },
	{ }
};

static struct spi_driver xpt2046_driver = {
	.driver = {
		.name	= "xpt2046_ts",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.of_match_table = xpt2046_dt_ids,
	},
	.probe		= xpt2046_probe,
	.remove		= xpt2046_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	//.suspend	= xpt2046_suspend,
	//.resume		= xpt2046_resume,
#endif
};

static int __init xpt2046_init(void)
{
	int ret;
	gADPoint.x = 0;
	gADPoint.y = 0;

	ret = spi_register_driver(&xpt2046_driver);
    if (ret)
    {
 		printk(KERN_ERR"Register XPT2046 driver failed.\n");
	    return ret;
	}
    else
	xpt2046printDebug("Touch panel drive XPT2046 driver init...\n");

	return 0;

}


static void __exit xpt2046_exit(void)
{
	xpt2046printDebug("Touch panel drive XPT2046 driver exit...\n");
	spi_unregister_driver(&xpt2046_driver);
}
module_init(xpt2046_init);
module_exit(xpt2046_exit);

MODULE_DESCRIPTION("spi xpt2046 TouchScreen Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:xpt2046");
