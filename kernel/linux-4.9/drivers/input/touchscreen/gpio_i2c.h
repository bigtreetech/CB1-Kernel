#ifndef __GPIO_I2C_H
#define __GPIO_I2C_H


void gpio_i2c_init(void);
int i2c_write_reg(u8 reg,u8 value);
u8 i2c_read_reg(u8 reg);
u8 i2c_read_multi_reg(u8 reg,u8 *data ,u8 size);

#endif
