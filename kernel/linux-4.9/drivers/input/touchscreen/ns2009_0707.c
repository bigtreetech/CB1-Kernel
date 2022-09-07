/*
 * Nsiway NS2009 touchscreen controller driver
 *
 * Copyright (C) 2017 Icenowy Zheng <icenowy@aosc.xyz>
 *
 * Some codes are from silead.c, which is
 *   Copyright (C) 2014-2015 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/input-polldev.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/sunxi-gpio.h>
#include <linux/delay.h>

/* polling interval in ms */
#define POLL_INTERVAL	30

/* this driver uses 12-bit readout */
#define MAX_12BIT	0xfff

#define NS2009_TS_NAME	"ns2009"

#define NS2009_READ_X_LOW_POWER_12BIT	0xc0
#define NS2009_READ_Y_LOW_POWER_12BIT	0xd0
#define NS2009_READ_Z1_LOW_POWER_12BIT	0xe0
#define NS2009_READ_Z2_LOW_POWER_12BIT	0xf0

#define NS2009_DEF_X_FUZZ	32
#define NS2009_DEF_Y_FUZZ	16


typedef u8 u8;

#define i2c_device_addr 0x31

static struct i2c_client *i2c_client;

#define RETRY_TIMES  1

#define GPIO_SCL  79
#define GPIO_SDA  70



#define I2C_SCL_PIN_NUM 79

#define I2C_SDA_PIN_NUM 70

#define I2C7_SDA_H do{ gpio_direction_output(I2C_SDA_PIN_NUM, 1);gpio_set_value(I2C_SDA_PIN_NUM, 1);} while(0);//gpio2 b0, sda

#define I2C7_SDA_L do{ gpio_direction_output(I2C_SDA_PIN_NUM, 0);gpio_set_value(I2C_SDA_PIN_NUM, 0);} while(0);//gpio2 b0, sda

#define I2C7_SCL_H do{ gpio_direction_output(I2C_SCL_PIN_NUM, 1);gpio_set_value(I2C_SCL_PIN_NUM, 1);} while(0);//gpio2 A7, scl

#define I2C7_SCL_L do{ gpio_direction_output(I2C_SCL_PIN_NUM, 0);gpio_set_value(I2C_SCL_PIN_NUM, 0);} while(0);//gpio2 A7, scl

int I2C7_IO_read_sda(void)
{

int ret = -1;

gpio_direction_input(I2C_SDA_PIN_NUM);

udelay(1);

ret = gpio_get_value(I2C_SDA_PIN_NUM);

return ret;

}

/**

* I2C_start - 启动I2C传输

* @void: none

*/

void I2C_start(void)

{

I2C7_SDA_H; //发送起始条件的数据信号

udelay(1);

I2C7_SCL_H; //发送起始条件的时钟信号

udelay(4);//起始条件建立时间大于4.7us，不能少于

I2C7_SDA_L; //发送起始信号，起始条件锁定时间大于4us，这里也必须大于4us

        udelay(5);

I2C7_SCL_L; //钳住I2C总线,准备发送或接收

        udelay(2);

}

/**

* I2C_stop - 停止I2C传输

* @void: none

*/

void I2C_stop(void) //释放I2C总线

{

I2C7_SDA_L;//发送停止条件的数据信号

udelay(2);

I2C7_SCL_H;

udelay(4);//起始条件建立时间大于4us

I2C7_SDA_H;//发送I2C总线停止信号

udelay(5);//停止条件锁定时间大于4us

}

/**

* I2C_stop - 停止I2C传输

* @ackBit: 主机对从机发送的应答信号，在数据显示，1 bit，应答为0，非应答为1

* 例如发送I2C_send_ack(0)表示不应答

*/

void I2C_send_ack(int ackBit)

{

//发送的应答或非应答信号

if(ackBit){

I2C7_SDA_H;

}else{

I2C7_SDA_L;

}

udelay(2);

I2C7_SCL_H;//置时钟线为高使应答位有效

udelay(4);//时钟高周期大于4us，不同于器件发送到主机的应答信号

I2C7_SCL_L;

udelay(2);

}

/**

* I2C_write_one_byte - 向I2C发送一个字节的数据

* @ucData: 无符号8位，要发送的数据

*

* return: bAck,成功写入返回1，否则，失败返回0

*/

int I2C_write_one_byte(unsigned char ucData)

{

static int bACK = 0;

unsigned char i;

i=8;

while(i--){//8 位没发送完继续发送

if((ucData & 0x80)==0x80){

I2C7_SDA_H;//I2C MSB高位先发

}else{

I2C7_SDA_L;

}

udelay(2);

I2C7_SCL_H;//置时钟线为高通知被控器开始接收数据位

udelay(2);

I2C7_SCL_L;

ucData=ucData<<1;

}

udelay(1);

I2C7_SDA_H; //8位数据发送完,释放I2C总线,准备接收应答位

udelay(2);

I2C7_SCL_H;//开始接收应答信号

udelay(4);

if(1 == I2C7_IO_read_sda()){//应答只需普通最小数据锁存的延时时间

bACK=0;//高电平说明无应答

}else{

bACK=1;//低电平有应答

}

I2C7_SCL_L;//发送结束钳住总线准备下一步发送或接收数据

gpio_direction_output(I2C_SDA_PIN_NUM, 1);

udelay(2);

return(bACK);//正确应答返回１

}

/**

* I2C_read_one_byte - 向I2C读取一个字节的数据

* @void: none

*

* return: 返回一个读到的数据，1个字节

*/

unsigned char I2C_read_one_byte(void)

{

static unsigned char i=0,byteData=0;

gpio_direction_input(I2C_SDA_PIN_NUM);//置数据线为输入方式

i=8;

while(i--){

udelay(1);

I2C7_SCL_L; //置钟线为零准备接收数据

udelay(2);//时钟低周期大于4.7us

I2C7_SCL_H;//置时钟线为高使数据线上数据有效

udelay(1);

byteData=byteData<<1;

if(1==I2C7_IO_read_sda()) {++byteData;}

udelay(1);

}

I2C7_SCL_L;

udelay(1);

return(byteData);

}

/**

* I2C_read_str - 从I2C设备读入一串数据

* @ucSla: slave，从器件地址

* @ucAddress: 器件里面，要读的寄存器地址

* @ucBuf: 要读入的buf，读到数据存在这里

* @ucCount: 计划读入的字节数

*

* return: 读入数成功返回读到字节数，否则返回0

*/

unsigned int I2C_read_str(unsigned char ucSla,unsigned char ucAddress,unsigned char *ucBuf,unsigned int ucCount)

{

int i=0;

// I2C_start();

///////////////这段代码用在i2c　eeprom上面//////////////////////

// if(1 != I2C_write_one_byte(ucSla)){//write one byte里包含应答

// I2C_stop();

// return 0;//选从器件的地址

// }

// printf("I2C_read_str, sla addr: %02x. \n", ucSla);

// if(1 != I2C_write_one_byte(ucAddress)){//选第一个寄存器地址

// I2C_stop();

// return 0;

// }

// printf("I2C_read_str, sla ucAddress: %02x. \n", ucAddress);

///////////////这段代码用在i2c　eeprom上面//////////////////////

I2C_start();

//printf("I2C_read_str, i2c start.\n");

if(1 != I2C_write_one_byte(ucSla+1)){//发送读器件命令，ucSla+1表示要读的器件是ucSla

I2C_stop();

return 0;

}

//printf("I2C_read_str, I2C_write_one_byte:%02x.\n", ucSla+1);

i=ucCount;

while(i--){//执行ucount次，最后还要执行一次递减，例如，ｉ＝３，那么ｉ=2,1,0, 跳出循环等于－１;

*ucBuf=I2C_read_one_byte();//读从器件寄存器

if(i)

I2C_send_ack(0);//未接收完所有字节,发送应答信号, 低电平应答，i2c飞利浦很低调

ucBuf++;

}

I2C_send_ack(1);//接收完所有字节,发送非应答信号, 1表示不应答

I2C_stop();

//printf("read str, finished, uCount:%d, i:%d. \n", ucCount, i);

return ucCount;

}

/**

* I2C_write_str - 向I2C设备写入一串数据

* @ucSla: slave，从器件地址

* @ucAddress: 器件里面，要读的寄存器地址

* @ucData: 要写入的数据数组

* @ucNo: 期望写入的个数

*

* return: 正确返回写入字节数，否则返回０

*/

unsigned int I2C_write_str(unsigned char ucSla,unsigned char ucAddress,unsigned char *ucData,unsigned int ucNo)

{

int i;

I2C_start();

if(1 != I2C_write_one_byte(ucSla)){

I2C_stop();//写入失败，直接发停止命令

return 0;//往I2C写命令

}

///////////////这段代码用在i2c　eeprom上面//////////////////////

// if(1 != I2C_write_one_byte(ucAddress)){//

// I2C_stop();

// return 0;//写寄存器地址

// }

/////////////////这个加密芯片，不能用//////////////////////////

i=ucNo;

while(i--){

if(1 != I2C_write_one_byte(*ucData)){

I2C_stop();

return 0;//写数据

}

ucData++;

}

I2C_stop();//最后停止，返回1表示成功写入数组

//printf("read str, finished, uCount:%d, i:%d. \n", ucNo, i);

return ucNo;

}


static void i2c_init(void){

    int ret;
    
    printk("==> i2c_init \n"); 

    ret = gpio_request(GPIO_SDA, "I2C_SDA");
    if(ret)
       printk("==> request I2C_SDA failed\n");  
    
    ret = gpio_request(GPIO_SCL, "I2C_SCL");
    if(ret)
       printk("==> request I2C_SCL failed\n"); 


gpio_direction_output(GPIO_SCL, 1);

gpio_direction_output(GPIO_SDA, 1);

}


/*
 * The chip have some error in z1 value when pen is up, so the data read out
 * is sometimes not accurately 0.
 * This value is based on experiements.
 */
#define NS2009_PEN_UP_Z1_ERR	80

struct ns2009_data {
	struct i2c_client		*client;
	struct input_dev		*input;
	bool				pen_down;
};

static int ns2009_ts_read_data(struct ns2009_data *data, u8 cmd, u16 *val)
{
	u8 raw_data[2]; 
	int error;

	error = i2c_smbus_read_i2c_block_data(data->client, cmd, 2, raw_data);
	if (error < 0)
		return error;

	if (unlikely(raw_data[1] & 0xf))
		return -EINVAL;

	*val = (raw_data[0] << 4) | (raw_data[1] >> 4);

	return 0;
}

static int ns2009_ts_report(struct ns2009_data *data)
{
	u16 x=0, y=0, z1=0;
	int ret;
    u8 i=0;
    
#if 0
    u8 rd_data[10]={0}; 
 #if 0   
   // rd_data[0] = I2C_Byte_Read(0x63,0x52);
   // i2c_read_reg_block(0x52,9,rd_data);
     u8 value = 0x80;
     
     I2C_write_str(0x30,0x09,value,1);
     I2C_read_str(0x30,0x2f,rd_data,1);
  
	 printk("==> get magic data = ");
	 
	 for(i=0;i<1;i++)
	 {
		  printk(" 0x%02x",rd_data[i]);
	 }
	 printk("\n");
;
#else  
   // printk("==> ns2009 report: z1 = %d\n",z1);
    
   // i2c_smbus_read_i2c_block_data(data->client,0x52,9,rd_data);
    
    i2c_smbus_write_byte_data(data->client,0x09,0x80);
    rd_data[0] = i2c_smbus_read_byte_data(data->client,0x2f);
    
	 printk("==> get mg data = ");
	 
	 for(i=0;i<1;i++)
	 {
		  printk(" 0x%02x",rd_data[i]);
	 }
	 printk("\n");

#endif
    
    return;
#endif
    
	/*
	 * NS2009 chip supports pressure measurement, but currently it needs
	 * more investigation, so we only use z1 axis to detect pen down
	 * here.
	 */
    
	ret = ns2009_ts_read_data(data, NS2009_READ_Z1_LOW_POWER_12BIT, &z1);
    
   // printk("==> ns2009 report: z1 = %d\n",z1);
    
	if (ret)
		return ret;

	if (z1 >= NS2009_PEN_UP_Z1_ERR) {
		ret = ns2009_ts_read_data(data, NS2009_READ_X_LOW_POWER_12BIT,&x);
		if (ret)
			return ret;

		ret = ns2009_ts_read_data(data, NS2009_READ_Y_LOW_POWER_12BIT,
					  &y);
		if (ret)
			return ret;

		if (!data->pen_down) {
			input_report_key(data->input, BTN_TOUCH, 1);
			data->pen_down = true;
		}
		printk("z1:%d, x:%d, y:%d\n", z1, x, y);
        
		input_report_abs(data->input, ABS_X, x);
		input_report_abs(data->input, ABS_Y, y);
		input_sync(data->input);
	} else if (data->pen_down) {
		input_report_key(data->input, BTN_TOUCH, 0);
		input_sync(data->input);
		data->pen_down = false;
	}
	return 0;
}

static void ns2009_ts_poll(struct input_polled_dev *dev)
{
	struct ns2009_data *data = dev->private;
	int ret;

	ret = ns2009_ts_report(data);
	if (ret)
		dev_err(&dev->input->dev, "Poll touch data failed: %d\n", ret);
}

static void ns2009_ts_config_input_dev(struct ns2009_data *data)
{
	struct input_dev *input = data->input;

	input_set_abs_params(input, ABS_X, 0, MAX_12BIT, NS2009_DEF_X_FUZZ, 0);
	input_set_abs_params(input, ABS_Y, 0, MAX_12BIT, NS2009_DEF_Y_FUZZ, 0);

	input->name = NS2009_TS_NAME;
	input->phys = "input/ts";
	input->id.bustype = BUS_I2C;
	input_set_capability(input, EV_ABS, ABS_X);
	input_set_capability(input, EV_ABS, ABS_Y);
	input_set_capability(input, EV_KEY, BTN_TOUCH);
}

static int ns2009_ts_request_polled_input_dev(struct ns2009_data *data)
{
	struct device *dev = &data->client->dev;
	struct input_polled_dev *polled_dev;
	int error;

	polled_dev = devm_input_allocate_polled_device(dev);
	if (!polled_dev) {
		dev_err(dev,
			"Failed to allocate polled input device\n");
		return -ENOMEM;
	}
	data->input = polled_dev->input;

	ns2009_ts_config_input_dev(data);
	polled_dev->private = data;
	polled_dev->poll = ns2009_ts_poll;
	polled_dev->poll_interval = POLL_INTERVAL;

	error = input_register_polled_device(polled_dev);
	if (error) {
		dev_err(dev, "Failed to register polled input device: %d\n",
			error);
		return error;
	}

	return 0;
}

static int ns2009_ts_probe(struct i2c_client *client,
			   const struct i2c_device_id *id)
{
	struct ns2009_data *data;
	struct device *dev = &client->dev;
	int error;
    
    i2c_client = client;
    
   // i2c_init();
    
    printk("===> ns2009_ts_probe \n");

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_I2C |
				     I2C_FUNC_SMBUS_READ_I2C_BLOCK |
				     I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)) {
		dev_err(dev, "I2C functionality check failed\n");
		return -ENXIO;
	}

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	data->client = client;
    
    printk("===> i2c address = %02x\n",i2c_client->addr);

	error = ns2009_ts_request_polled_input_dev(data);
	if (error)
		return error;
    
    printk("===> ns2009_ts_probe OK\n");

	return 0;
};

static const struct i2c_device_id ns2009_ts_id[] = {
	{ "ns2009", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ns2009_ts_id);

static struct i2c_driver ns2009_ts_driver = {
	.probe = ns2009_ts_probe,
	.id_table = ns2009_ts_id,
	.driver = {
		.name = NS2009_TS_NAME,
	},
};

module_i2c_driver(ns2009_ts_driver);
MODULE_LICENSE("GPL");
