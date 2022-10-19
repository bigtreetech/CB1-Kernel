#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/sunxi-gpio.h>

#include "gpio_i2c.h"

#define UDELAY udelay(10)
#define MDELAY mdelay(10)

#define SCL 74
#define SDA 76

#define ONE 1 
#define ZERO 0

#define USESOFTIIC

#define I2C_ADDR 0x48

static void i2c_start(void) 
{ //起始信号 SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据
    gpio_set_value(SDA, 1);
    gpio_set_value(SCL, 1);
    UDELAY;
    gpio_set_value(SDA, 0);
    UDELAY;
    gpio_set_value(SCL, 0);
}

static void i2c_stop (void)
{//停止信号 SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据
    gpio_set_value(SDA, 0);
    gpio_set_value(SCL, 1);
    UDELAY;
    gpio_set_value(SDA, 1);
    UDELAY;
}

static void i2c_sendack(u8 ack)
{ //发送应答信号 入口参数:ack (0x0:ACK 0x01:NAK)
    if(ack)
        gpio_set_value(SDA, 1);
    else
        gpio_set_value(SDA, 0);
    gpio_set_value(SCL, 1);
    UDELAY;
    gpio_set_value(SCL, 0);
    UDELAY;
}

static u8 i2c_recvack(void)
{//接收应答信号
    u8 cy;

    gpio_set_value(SDA, 1);
    UDELAY;
    gpio_direction_input(SDA);
    gpio_set_value(SCL, 1);
    UDELAY;
    cy=gpio_get_value(SDA);
    gpio_set_value(SCL, 0);
    UDELAY;
    gpio_direction_output(SDA,1);
    UDELAY;
    return cy;
}

static int i2c_sendbyte(u8 data)
{//向IIC总线发送一个字节数据
    u8 i;
    u8 ack;
    u8 cy[8];
    u8 bit;

    for (i=0; i<8; i++)
    {
        bit=data&0x80;
        if(bit)
            cy[i]=ONE;
        else
            cy[i]=ZERO;
        data<<=1;
    }

    for (i=0; i<8; i++)
    {
        gpio_set_value(SDA,cy[i]);
        gpio_set_value(SCL, 1);
        UDELAY;
        gpio_set_value(SCL, 0);
        UDELAY;
    }

    ack=i2c_recvack();
    return ack;
}

static u8 i2c_recvbyte(void)
{//从IIC总线接收一个字节数据
    u8 i;
    u8 dat = 0;
    gpio_set_value(SDA,1);
    gpio_direction_input(SDA);
    UDELAY;
    for (i=0; i<8; i++)
    {
        dat <<= 1;
        gpio_set_value(SCL, 1);
        UDELAY;
        dat |= gpio_get_value(SDA);
        gpio_set_value(SCL, 0);
        UDELAY;
    }
    gpio_direction_output(SDA,1);

    return dat;
}

void gpio_i2c_init(void) 
{
    int ret;
    ret = gpio_request(SDA, "I2C_SDA");
    ret = gpio_request(SCL, "I2C_SCL");
    gpio_direction_output(SCL, 1);
    gpio_direction_output(SDA, 1);
}
EXPORT_SYMBOL(gpio_i2c_init);

int i2c_write_reg(u8 reg,u8 value)
{
    int ret1,ret2,ret3;
    i2c_start();
    ret1=i2c_sendbyte(I2C_ADDR<<1);
    ret2=i2c_sendbyte(reg);
    ret3=i2c_sendbyte(value);
    i2c_stop();
    if(ret1||ret2||ret3)
    {
        pr_err("i2c_write error",ret1,ret2,ret3);
        return -ENXIO;
    }

    return 0;

}
EXPORT_SYMBOL(i2c_write_reg);

u8 i2c_read_reg(u8 reg)
{
    int ret1,ret2,ret3,ret4;
    i2c_start();
    ret1=i2c_sendbyte(I2C_ADDR<<1);
    ret2=i2c_sendbyte(reg);
    i2c_stop();
    i2c_start();
    ret3=i2c_sendbyte((I2C_ADDR<<1)+1);

    ret4=i2c_recvbyte();
    i2c_sendack(0x1);
    
    i2c_stop();
    if(ret1||ret2||ret3)
    {
        pr_err("i2c_write error",ret1,ret2,ret3,ret4);
        return -ENXIO;
    }

    return ret4;

}
EXPORT_SYMBOL(i2c_read_reg);

u8 i2c_read_multi_reg(u8 reg,u8 *data ,u8 size) 
{
    int ret1,ret2,ret3,ret4=0,i;
    
    i2c_start();
    ret1=i2c_sendbyte(I2C_ADDR<<1);
    ret2=i2c_sendbyte(reg);
    i2c_stop();
    i2c_start();
    ret3=i2c_sendbyte((I2C_ADDR<<1)+1);
    
    for(i=0;i<size;i++)
    {
       data[i]=i2c_recvbyte();
       i2c_sendack(0x0);        
    }
    i2c_sendack(0x1); 
    
    i2c_stop();
    if(ret1||ret2||ret3)
    {
        pr_err("i2c_write error",ret1,ret2,ret3,ret4);
        return -ENXIO;
    }

    return 0;

}
EXPORT_SYMBOL(i2c_read_multi_reg);

MODULE_LICENSE("GPL");
