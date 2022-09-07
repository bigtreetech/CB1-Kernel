#ifndef __LINUX_GPIO_H
#define __LINUX_GPIO_H

#include <linux/errno.h>

/* see Documentation/gpio/gpio-legacy.txt */

/* make these flag values available regardless of GPIO kconfig options */
#define GPIOF_DIR_OUT	(0 << 0)
#define GPIOF_DIR_IN	(1 << 0)

#define GPIOF_INIT_LOW	(0 << 1)
#define GPIOF_INIT_HIGH	(1 << 1)

#define GPIOF_IN		(GPIOF_DIR_IN)
#define GPIOF_OUT_INIT_LOW	(GPIOF_DIR_OUT | GPIOF_INIT_LOW)
#define GPIOF_OUT_INIT_HIGH	(GPIOF_DIR_OUT | GPIOF_INIT_HIGH)

/* Gpio pin is active-low */
#define GPIOF_ACTIVE_LOW        (1 << 2)

/* Gpio pin is open drain */
#define GPIOF_OPEN_DRAIN	(1 << 3)

/* Gpio pin is open source */
#define GPIOF_OPEN_SOURCE	(1 << 4)

#define GPIOF_EXPORT		(1 << 5)
#define GPIOF_EXPORT_CHANGEABLE	(1 << 6)
#define GPIOF_EXPORT_DIR_FIXED	(GPIOF_EXPORT)
#define GPIOF_EXPORT_DIR_CHANGEABLE (GPIOF_EXPORT | GPIOF_EXPORT_CHANGEABLE)

/**
 * struct gpio - a structure describing a GPIO with configuration
 * @gpio:	the GPIO number
 * @flags:	GPIO configuration as specified by GPIOF_*
 * @label:	a literal description string of this GPIO
 */
struct gpio {
	unsigned	gpio;
	unsigned long	flags;
	const char	*label;
};

#ifdef CONFIG_GPIOLIB

#ifdef CONFIG_ARCH_HAVE_CUSTOM_GPIO_H
#include <asm/gpio.h>
#else

#include <asm-generic/gpio.h>

static inline int gpio_get_value(unsigned int gpio)
{
	return __gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned int gpio, int value)
{
	__gpio_set_value(gpio, value);
}

static inline int gpio_cansleep(unsigned int gpio)
{
	return __gpio_cansleep(gpio);
}

static inline int gpio_to_irq(unsigned int gpio)
{
	return __gpio_to_irq(gpio);
}

#endif /* ! CONFIG_ARCH_HAVE_CUSTOM_GPIO_H */

/* CONFIG_GPIOLIB: bindings for managed devices that want to request gpios */

struct device;

int devm_gpio_request(struct device *dev, unsigned gpio, const char *label);
int devm_gpio_request_one(struct device *dev, unsigned gpio,
			  unsigned long flags, const char *label);
void devm_gpio_free(struct device *dev, unsigned int gpio);

#else /* ! CONFIG_GPIOLIB */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/bug.h>
#include <linux/pinctrl/pinctrl.h>

struct device;
struct gpio_chip;


#endif /* ! CONFIG_GPIOLIB */

#endif /* __LINUX_GPIO_H */
