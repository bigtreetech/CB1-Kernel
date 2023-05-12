/***************************************************************************
 * WS2812 linux5.16内核驱动
 *
 * 2022-12-19
 *
 ***************************************************************************/
#include <linux/init.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h> //for various structures related to files(e.g.fops)
#include <linux/device.h>
#include <asm/uaccess.h> //for copy_from_user and copy_to user functions
#include <linux/moduleparam.h>
#include <linux/ioctl.h>   //for ioctl
#include <linux/version.h> //for LINUX_VERSION_CODE and KERNEL_VERSION Macros
#include <linux/errno.h>   //Linux system errors
#include <linux/rbtree.h>
#include <linux/ktime.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ws2812-led" // 设备名称
#define DRIVER_NAME "ws2812_ctl"
#define CLASS_NAME "ws2812"

//=====================================
#define GPIO_BASE 0x0300B000
#define GPIO_DAT_OFFSET(n) ((n)*0x0024 + 0x10)

static uint32_t ws2812_pin = 0; // 定义ws2812的管脚
static volatile uint32_t *ws2812_gpio_port; // 直接用指针形式，用此定义
static volatile uint32_t ws2812_gpio_bit;
static volatile uint32_t ws2812_set_val = 0;
static volatile uint32_t ws2812_reset_val = 0;

//=====================================

DEFINE_SPINLOCK(lock);

// ws2812 reset
static void ws2812_rst(void)
{
    *ws2812_gpio_port &= ~ws2812_gpio_bit;
    udelay(200);// RES low voltage time, Above 50µs
}

static void ws2812_Write_24Bits(uint32_t grb)
{
    uint8_t i;
    for (i = 0; i < 24; i++)
    {
        if (grb & 0x800000)
        {
            // loop for delay about 700ns
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            // loop for delay about 600ns
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
        }
        else
        {
            // loop for delay about 200ns
            *ws2812_gpio_port = ws2812_set_val;
            *ws2812_gpio_port = ws2812_set_val;
            // loop for delay about 800ns
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
            *ws2812_gpio_port = ws2812_reset_val;
        }
        grb <<= 1;
    }
}

static void ws2812_write_array(uint32_t *rgb, uint32_t cnt)
{
    uint32_t i = 0;
    unsigned long flags;

    for (i = 0; i < cnt; i++)
    {
        // rgb -> grb
        rgb[i] = (((rgb[i] >> 16) & 0xff) << 8) | (((rgb[i] >> 8) & 0xff) << 16) | ((rgb[i]) & 0xff);
    }

    spin_lock_irqsave(&lock, flags);
    ws2812_set_val = *ws2812_gpio_port | ws2812_gpio_bit;
    ws2812_reset_val = *ws2812_gpio_port & (~ws2812_gpio_bit);
    ws2812_rst();
    for (i = 0; i < cnt; i++)
    {
        ws2812_Write_24Bits(rgb[i]);
    }
    spin_unlock_irqrestore(&lock, flags);
}

/*ws2812读函数*/
ssize_t ws2812_read(struct file *file, char __user *user, size_t bytesize, loff_t *this_loff_t)
{
    return 0;
}

ssize_t ws2812_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    uint32_t rgb[255];
    unsigned long ret = 0;

    if (count > 255 * 4) count = 255 * 4;
    ret = copy_from_user(&rgb[0], user_buf, count);
    if (ret < 0)
    {
        printk("copy_from_user fail!!!\n");
        return -1;
    }

    ws2812_write_array((uint32_t *)rgb, count / 4);

    return 0;
}

/*ws2812设备节点打开函数*/
int ws2812_open(struct inode *inode, struct file *file)
{
    // printk("--------------%s--------------\n", __FUNCTION__);
    return 0;
}

/*设备节点关闭*/
int ws2812_close(struct inode *inode, struct file *file)
{
    return 0; // 返回0
}

/*ws2812文件结构体*/
static struct file_operations ws2812_ops = {
    .owner = THIS_MODULE,
    .open = ws2812_open,
    .release = ws2812_close,
    .write = ws2812_write,
};

/*将ws2812注册为杂项*/
static struct miscdevice ws2812_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &ws2812_ops,
};

/*probe 函数*/
static int ws2812_probe(struct platform_device *pdev)
{
    int ret;
    enum of_gpio_flags flag; //(flag == OF_GPIO_ACTIVE_LOW) ?
    struct device_node *ws2812_gpio_node = pdev->dev.of_node;
    uint32_t rgb_cnt = 0;
    uint32_t rgb[255];

    of_property_read_u32(ws2812_gpio_node, "rgb_cnt", &rgb_cnt);
    if (rgb_cnt > 255)
        rgb_cnt = 255;
    of_property_read_u32_array(ws2812_gpio_node, "rgb_value", rgb, rgb_cnt);
    ws2812_pin = of_get_named_gpio_flags(ws2812_gpio_node->child, "gpios", 0, &flag);
    if (!gpio_is_valid(ws2812_pin))
    {
        printk("===> ws2812-gpio: %d is invalid\n", ws2812_pin);
        return -ENODEV;
    }

    ws2812_gpio_port = ioremap(GPIO_BASE + GPIO_DAT_OFFSET((ws2812_pin >> 5)), 4);
    ws2812_gpio_bit = 1 << (ws2812_pin & 0x001F);

    if (gpio_request(ws2812_pin, "ws2812-gpio"))
    {
        printk("===> gpio %d request failed!\n", ws2812_pin);
        gpio_free(ws2812_pin);
        return -ENODEV;
    }
    gpio_direction_output(ws2812_pin, 0); // output low-level

    ret = misc_register(&ws2812_misc_dev); // 注册设备为杂项设备
    msleep(50);                            // 延时100毫秒

    ws2812_write_array(rgb, rgb_cnt);

    return 0;
}

// void gpiod_set_raw_value(struct gpio_desc *desc, int value)
static int ws2812_remove(struct platform_device *pdev)
{
    misc_deregister(&ws2812_misc_dev);
    gpio_free(ws2812_pin);

    return 0;
}

/*匹配设备树*/
static const struct of_device_id ws2812_of_match[] = {
    {.compatible = "rgb-ws2812"},
    {/* sentinel */}};

MODULE_DEVICE_TABLE(of, ws2812_of_match);

/*注册平台驱动*/
static struct platform_driver ws2812_driver = {
    .probe = ws2812_probe,
    .remove = ws2812_remove,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(ws2812_of_match),
    },
};

/*设备驱动挂载函数*/
static int __init ws2812_dev_init(void)
{
    return platform_driver_register(&ws2812_driver);
}

/*设备驱动退出函数*/
static void __exit ws2812_dev_exit(void)
{
    platform_driver_unregister(&ws2812_driver);
}

module_init(ws2812_dev_init);
module_exit(ws2812_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MacLodge");
MODULE_DESCRIPTION("WS2811 leds driver for Allwinner");
MODULE_VERSION("1.0");
