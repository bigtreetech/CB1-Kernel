
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
 
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>	
 
#define ADS7846_CNV_NBR  8  //ADC连续转换的次数
 
struct ads7846_ts_info {
 
    struct input_dev *dev;
 
    unsigned int xp; //x方向位置
    unsigned int yp; //y方向位置
    unsigned int count; //adc转换次数
    unsigned int cnv_nbr;
    unsigned int x_buf[ADS7846_CNV_NBR];  //ad转换buf 
    unsigned int y_buf[ADS7846_CNV_NBR];
    
};

static int ts_irq_num=0;
 
static struct ads7846_ts_info	*ts;
static struct input_dev *ts_dev;
int absX = 0;
int absY = 0;
#if 0
#define GPIO_TS_IRQ 74 //PC10 
#define GPIO_TS_CLK 72 //PC8
#define GPIO_TS_MISO 71 //PC7
#define GPIO_TS_MOSI 79 //PC15 
#else
#define GPIO_TS_IRQ  234 //PH10
#define GPIO_TS_CLK  230 //PH6
#define GPIO_TS_MISO 232 //PH8
#define GPIO_TS_MOSI 231 //PH7
#define GPIO_TS_CS   201 //PG9
#endif

 
// ADS7846 Control Byte bit defines
#define ADS7846_CMD_START	0x0080
#define ADS7846_ADDR_BIT	4
#define ADS7846_ADDR_MASK 	(0x7<<ADS7846_ADDR_BIT)
#define ADS7846_MEASURE_X  	(0x5<<ADS7846_ADDR_BIT)
#define ADS7846_MEASURE_Y  	(0x1<<ADS7846_ADDR_BIT)
#define ADS7846_MEASURE_Z1 	(0x3<<ADS7846_ADDR_BIT)
#define ADS7846_MEASURE_Z2 	(0x4<<ADS7846_ADDR_BIT)
#define ADS7846_8BITS     	(1<<3)
#define ADS7846_12BITS    	0
#define ADS7846_SER       	(1<<2)
#define ADS7846_DFR       	0
#define ADS7846_PWR_BIT   	0
#define ADS7846_PD      	0
#define ADS7846_ADC_ON  	(0x1<<ADS7846_PWR_BIT)
#define ADS7846_REF_ON  	(0x2<<ADS7846_PWR_BIT)
#define ADS7846_REF_ADC_ON 	(0x3<<ADS7846_PWR_BIT)
 
#define MEASURE_8BIT_X\
    (unsigned short)(ADS7846_CMD_START | ADS7846_MEASURE_X | ADS7846_8BITS | ADS7846_DFR | ADS7846_PD)
#define MEASURE_8BIT_Y\
    (unsigned short)(ADS7846_CMD_START | ADS7846_MEASURE_Y | ADS7846_8BITS | ADS7846_DFR | ADS7846_PD)
 
#define MEASURE_12BIT_X \
    (unsigned short)(ADS7846_CMD_START | ADS7846_MEASURE_X | ADS7846_12BITS | ADS7846_DFR | ADS7846_PD)
#define MEASURE_12BIT_Y \
    (unsigned short)(ADS7846_CMD_START | ADS7846_MEASURE_Y | ADS7846_12BITS | ADS7846_DFR | ADS7846_PD)
#define MEASURE_12BIT_Z1 \
    (unsigned char)(ADS7846_MEASURE_Z1 | ADS7846_12BITS | ADS7846_DFR | ADS7846_PD)
#define MEASURE_12BIT_Z2 \
    (unsigned char)(ADS7846_MEASURE_Z2 | ADS7846_12BITS | ADS7846_DFR | ADS7846_PD)
 
 
static inline void set_miso_as_input(void)
{
	gpio_direction_input(GPIO_TS_MISO);	
}
 
static inline void set_cs_mosi_clk_as_output(void)
{
	gpio_direction_output(GPIO_TS_MOSI,1);	//MOSI
	gpio_direction_output(GPIO_TS_CLK,1);	//clk
	gpio_direction_output(GPIO_TS_CS,1);	//cs
}
 
static inline void set_gpcx_value(int pinx ,int v)
{ 
	gpio_set_value(pinx,v);
}
 
static inline int get_gpcx_value(int pinx)
{
	return( gpio_get_value(pinx) );
}

void spi_delay(void)
{
  // ndelay(10); 
}
 
//读12bit
static unsigned int ads7846_Read_Data(void)
{
 unsigned int i,temp=0x00;
   for(i=0;i<12;i++)
   {
       temp <<=1;
       set_gpcx_value(GPIO_TS_CLK, 1); 
       spi_delay();
       set_gpcx_value(GPIO_TS_CLK, 0); 
       spi_delay();
       if(get_gpcx_value(GPIO_TS_MISO) != 0) temp++;
   }
   return (temp);
}
//写8bit
static void ads7846_Write_Data(unsigned int n)
{
   unsigned char i;
  set_gpcx_value(GPIO_TS_CLK, 0);
  for(i=0;i<8;i++)
   {
    if((n&0x80)==0x80)
      set_gpcx_value(GPIO_TS_MOSI, 1);
     else
      set_gpcx_value(GPIO_TS_MOSI, 0);
    
    n <<= 1;
    set_gpcx_value(GPIO_TS_CLK, 1); spi_delay();
    set_gpcx_value(GPIO_TS_CLK, 0); spi_delay();
    }
}
 
//ADS7846转换==//保存在ts->buf 当中//ADS7846_CNV_NBR是值为8的宏，含义是AD转换次数。
static void ads7846_conver_start(void)
{
	int i;
	unsigned int cmd[2];
	unsigned int data[2];
    
        //开读
	cmd[0] = MEASURE_12BIT_X;//宏，本程序内定义
	cmd[1] = MEASURE_12BIT_Y;//宏，本程序内定义
 
	/* CS# Low */
	set_gpcx_value(GPIO_TS_CS, 0);
	
	//连续转换
	for(ts->count=0; ts->count<ts->cnv_nbr;) 
	{
	    //分别读出x y坐标＝＝
	    for(i=0; i<2; i++){
	      ads7846_Write_Data(cmd[i]);
	     // udelay(5);
          spi_delay();
	      data[i] = ads7846_Read_Data();
          printk(" %d",data[i]);
	    }
        printk("\n");
	    //保存转换结果
	    ts->x_buf[ts->count]= data[0];
	    ts->y_buf[ts->count]= data[1];
	    ts->count++;
	}
	/* CS# High */
	set_gpcx_value(GPIO_TS_CS, 1);
 
}

//-----------------------------------------------------------------------------
//触摸屏数据滤波算法
//触摸屏AD连续转换N次后，按升序排列，再取中间几位值求平均
#define  TS_AD_NBR_PJ		4      	 //取中间4位求平均值 
#define  TS_AD_NBR_MAX_CZ	10       //最大与最小的差值

static inline bool touch_ad_data_filter(unsigned int *buf0, unsigned int *buf1)
{
	   unsigned int i,j,k,temp,temp1,nbr=(ADS7846_CNV_NBR);
	   //将转换结果升序排列
	   //buf0
	    for (j= 0; j< nbr; j++)
	    {
            for (i = 0; i < nbr; i++) 
	        {           
                if(buf0[i]>buf0[i+1])//升序排列
                {
                    temp=buf0[i+1];
                    buf0[i+1]=buf0[i];
                    buf0[i]=temp;
                }  
	        }
	    }
	    
	  
	    //buf1
	    for (j= 0; j< nbr; j++)
	    {
            for (i = 0; i < nbr; i++) 
            {           
                if(buf1[i]>buf1[i+1])//升序排列
                {
                    temp=buf1[i+1];
                    buf1[i+1]=buf1[i];
                    buf1[i]=temp;
                }  
            } 
	    }	 
	   //取中间值求平均==
	   k=((nbr-TS_AD_NBR_PJ)>>1);
	   temp = 0;temp1 = 0;
	   //检查值是否有效==
	   if((buf0[k+TS_AD_NBR_PJ-1]-buf0[k]>TS_AD_NBR_MAX_CZ)||(buf1[k+TS_AD_NBR_PJ-1]-buf1[k]>TS_AD_NBR_MAX_CZ)) //无效值
	    {
 
	      return 0; 
	    }
	   //--
	   //将中间指定位数累加
	   for(i=0;i<TS_AD_NBR_PJ;i++)
	   {  
	      temp += buf0[k+i];
	      temp1 += buf1[k+i];
	   }
	   //求平均值,将结果保存在最低位
	   buf0[0]=temp/TS_AD_NBR_PJ;   
	   buf1[0] = temp1/TS_AD_NBR_PJ; 
	   //--
	   return 1; 
}
 
static inline bool get_down(void)	//获取中断口电平状态
{
	return (gpio_get_value (GPIO_TS_IRQ) );  	// 中断管脚电平状态
}
/*===========================================================================================
    touch_timer_get_value这个函数的调用：
    
    1、  触摸笔开始点击的时候， 在中断函数touch_down里面被调用，不是直接调用，而是设置定时器超时后调用
         这样是为了去抖动
         
    2、  touch_timer_get_value被调用一次后，如果pendown信号有效，则touch_timer_get_value会被持续调用
         也是通过定时器实现的
         
    touch_timer_get_value这个函数的功能：
      启动7846转换，直到连续转换8次后，再滤波处理，获得有效值，并向上报告触摸屏事件
============================================================================================*/
static void touch_timer_get_value(unsigned long data); 
 
static DEFINE_TIMER(touch_timer, touch_timer_get_value, 0, 0);
 
static void touch_timer_get_value(unsigned long data) {

	//printk("==>timer get value\n");   
 
	if(!get_down() )	//	检测按下状态
	{	
	        disable_irq(ts_irq_num);
	        ads7846_conver_start();
	        enable_irq(ts_irq_num);
 
			if(touch_ad_data_filter(ts->x_buf,ts->y_buf)) // 滤波函数处理
            {
		        
                ts->xp = ts->x_buf[0];
                ts->yp = ts->y_buf[0];
			
                printk("ts [%d %d]\n",ts->xp,ts->yp);
 
			//--------------------------------------------------------------------
				absX = ts->xp ;		
				absY = ts->yp ;				
 
				input_report_abs(ts_dev, ABS_X, absX);		//X轴坐标
				input_report_abs(ts_dev, ABS_Y, absY);		//Y轴坐标
				input_report_key(ts_dev, BTN_TOUCH, 1);	//按下 按键
				input_report_abs(ts_dev, ABS_PRESSURE, 1);	//按下 压力	
				input_sync(ts_dev);	//事件同步
				input_report_key(ts_dev, BTN_TOUCH, 0);	//抬起 按键值	
				input_report_abs(ts_dev, ABS_PRESSURE, 0);	//抬起 压力值
				input_sync(ts_dev);
			//--------------------------------------------------------------------
 
		    }
               
            mod_timer(&touch_timer, jiffies + 2);   //重新设置系统定时器，超时后，又会调用touch_timer_get_value
		                                         //jiffies变量记录了系统启动以来，系统定时器已经触发的次数。内核每秒钟将jiffies变量增加HZ次。
		                                        //因此，对于HZ值为100的系统，1个jiffy等于10ms，而对于HZ为1000的系统，1个jiffy仅为1ms
		                                        //这里系统配置提HZ是100
		
	} 
	else 
	{
 
	/*	ads7846_conver_start();
		if(touch_ad_data_filter(ts->x_buf,ts->y_buf)) // 滤波函数处理
		{
			ts->xp = ts->x_buf[0];
			ts->yp = ts->y_buf[0];
			printk("ts->xp = %d,ts->yp = %d\n",ts->xp,ts->yp);
			//--------------------------------------------------------------------
			absX = ts->xp ;		
			absY = ts->yp ;				
				input_report_abs(ts_dev, ABS_X, absX);		//X轴坐标
				input_report_abs(ts_dev, ABS_Y, absY);		//Y轴坐标
		
				input_report_key(ts_dev, BTN_TOUCH, 0);	//抬起 按键值	
				input_report_abs(ts_dev, ABS_PRESSURE, 0);	//抬起 压力值
				input_sync(ts_dev);
			//--------------------------------------------------------------------
		}
	*/
	}
	
}
 
static irqreturn_t touch_down(int irqno, void *param)			//中断处理函数
{
     	//printk("Touch INT!\n");   
 
        //稍后调用touch_timer_get_value，去抖动
        mod_timer(&touch_timer, jiffies + 15);  
        //等ADS7846转换完成了再读
	    //同时还可以防抖动，如果定时器没有超时的这段时间里，
		//发生了抬起和按下中断，则定时器的值会被重设，不会超时
	    //内核配置时HZ值设为100，即1个jiffy等于10ms，
        //touch_timer_get_value(1);  //直接调用会有抖动
 
        return IRQ_RETVAL(IRQ_HANDLED);
}
 
//-------------------------------------------
static int __init ads7846_ts_init(void)
{
	    printk( KERN_INFO "\n------------xpt2046 Touchscreen driver -------\n\n" );
 
	    int ret = 0;
        
        gpio_direction_input(GPIO_TS_IRQ);
        
        ts_irq_num = gpio_to_irq(GPIO_TS_IRQ);
        
        printk("===> ts_irq_num = %d\n");
        
        if(!ts_irq_num)
        {
           printk("===> ts_irq_num error,ts driver init failed\n");
           return 1;
        }

	    //给ads7846_ts_info指针分配内存==
	    ts = kzalloc(sizeof(struct ads7846_ts_info), GFP_KERNEL);
	    ts->cnv_nbr = ADS7846_CNV_NBR;
	    ts->xp = 0;
	    ts->yp = 0;
	    ts->count = 0;

	    irq_set_irq_type(ts_irq_num, IRQ_TYPE_EDGE_BOTH);	
//-------------------------------------------------------------------------
	ts_dev = input_allocate_device();				// 申请一个输入设备
	if (!ts_dev) return -ENOMEM;
	ts_dev->name = "h616_ts";				// 触摸屏设备属性信息
	ts_dev->phys = "input";
	ts_dev->id.bustype = BUS_RS232;
	ts_dev->id.vendor  = 0xDEAD;
	ts_dev->id.product = 0xBEEF;
	ts_dev->id.version = 0x0101;
	
	input_set_abs_params(ts_dev, ABS_X, 0, 4000, 0, 0);			// 
	input_set_abs_params(ts_dev, ABS_Y, 0, 4000, 0, 0);			// 
	input_set_abs_params(ts_dev, ABS_PRESSURE, 0, 1, 0, 0);		// 压力
	
	ts_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	ts_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
//---------------------------------------------------------------------------
	ret = request_irq(ts_irq_num, touch_down,IRQ_TYPE_EDGE_BOTH, "xpt2046_touch", ts);
	if (ret != 0) {
		printk("==> request_irq failed!\n");
		ret = -EIO;
		goto fail;
	}
//--------------------------------------------------------------------------------	
	ret = input_register_device(ts_dev);
	if(ret)
	{	
		free_irq(ts_irq_num, ts_dev);
		input_free_device(ts_dev);
		return -1;
	}
//--------------------------------------------------------------------------------

	set_miso_as_input(); 
	set_cs_mosi_clk_as_output(); 
	set_gpcx_value(GPIO_TS_MOSI ,1);	
	set_gpcx_value(GPIO_TS_CS ,1);
	set_gpcx_value(GPIO_TS_CLK ,1);

	printk("ads7846_ts_probe OK!\n");
    printk("===> MEASURE_12BIT_X MEASURE_12BIT_Y = %02X %02X\n",MEASURE_12BIT_X,MEASURE_12BIT_Y);
	
   	return 0;
 
fail:
	disable_irq(ts_irq_num);
    free_irq(ts_irq_num, ts);
 
	return ret;	   
}
 
static void __exit ads7846_ts_exit(void)
{
	printk(KERN_INFO "ads7846_ts_remove() of TS called !\n");
        
	disable_irq(ts_irq_num);
    free_irq(ts_irq_num, ts);
}
 
module_init(ads7846_ts_init);
module_exit(ads7846_ts_exit);
 
MODULE_AUTHOR("LIGHT");
MODULE_LICENSE("GPL");
