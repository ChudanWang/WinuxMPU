/* linux/arch/arm/mapu/gpio.c
 *
 * Copyright 2015 IACAS
 *
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sysdev.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/seq_file.h>
#include <asm/gpio.h>

#include <mach/devs.h>
#include <mach/map-va.h>
#include <mach/gpio.h> 
#include <mach/regs-gpio.h> 

 
struct mapu_gpio_chip {
	struct gpio_chip	chip;
	struct mapu_gpio_chip	*next;		/* Bank sharing same clock */
	struct mapu_gpio_bank	*bank;		/* Bank definition */
	void __iomem		*regbase;	/* Base of register bank */
};

#define to_mapu_gpio_chip(c) container_of(c, struct mapu_gpio_chip, chip)

static void mapu_gpiolib_dbg_show(struct seq_file *s, struct gpio_chip *chip);
static void mapu_gpiolib_set(struct gpio_chip *chip, unsigned offset, int val);
static int mapu_gpiolib_get(struct gpio_chip *chip, unsigned offset);
static int mapu_gpiolib_direction_output(struct gpio_chip *chip,
					 unsigned offset, int val);
static int mapu_gpiolib_direction_input(struct gpio_chip *chip,
					unsigned offset);
#define MAPU_GPIO_CHIP(name, base_gpio, nr_gpio)			\
	{								\
		.chip = {						\
			.label		  = name,			\
			.direction_input  = mapu_gpiolib_direction_input, \
			.direction_output = mapu_gpiolib_direction_output, \
			.get		  = mapu_gpiolib_get,		\
			.set		  = mapu_gpiolib_set,		\
			.dbg_show	  = mapu_gpiolib_dbg_show,	\
			.base		  = base_gpio,			\
			.ngpio		  = nr_gpio,			\
		},							\
	}

static struct mapu_gpio_chip gpio_chip[] = {
	MAPU_GPIO_CHIP("A", 0x00 + PIN_BASE, 32),
	MAPU_GPIO_CHIP("B", 0x20 + PIN_BASE, 32),
	MAPU_GPIO_CHIP("C", 0x40 + PIN_BASE, 32),
	MAPU_GPIO_CHIP("D", 0x60 + PIN_BASE, 32),
};

static int gpio_banks;

static inline void __iomem *pin_to_controller(unsigned pin)
{
	pin -= PIN_BASE;
	pin /= 32;
	if (likely(pin < gpio_banks))
		return gpio_chip[pin].regbase;

	return NULL;
}

static inline unsigned pin_to_mask(unsigned pin)
{
	pin -= PIN_BASE;
	return 1 << (pin % 32);
}

int __init_or_module mapu_set_gpio_input(unsigned pin, int use_pullup)
{
	void __iomem	*pio = pin_to_controller(pin);
	unsigned	mask = pin_to_mask(pin);
	unsigned long data = 0;

	if (!pio)
		return -EINVAL;

        data = __raw_readl(pio + GPIO_DDR);

        data &= ~(mask);

        __raw_writel(data, pio + GPIO_DDR);

	return 0;
}
EXPORT_SYMBOL(mapu_set_gpio_input);

int __init_or_module mapu_set_gpio_output(unsigned pin, int value)
{
	void __iomem	*pio = pin_to_controller(pin);
	unsigned	mask = pin_to_mask(pin);
	unsigned long	data = 0;

	if (!pio)
		return -EINVAL;

        data = __raw_readl(pio + GPIO_DDR);

        data |= mask;

        __raw_writel(data, pio + GPIO_DDR);

	return 0;
}
EXPORT_SYMBOL(mapu_set_gpio_output);

int mapu_set_gpio_value(unsigned pin, int value)
{
	void __iomem	*pio = pin_to_controller(pin);
	unsigned	mask = pin_to_mask(pin);
	unsigned long  data = 0;

	if (!pio)
		return -EINVAL;
	
        data = __raw_readl(pio + GPIO_DR);

        if (value == 1)
                data |= mask;
        else
                data &= ~(mask);

        __raw_writel(data, pio + GPIO_DR);

	return 0;
}
EXPORT_SYMBOL(mapu_set_gpio_value);

int mapu_get_gpio_value(unsigned pin)
{
	void __iomem	*pio = pin_to_controller(pin);
	unsigned	mask = pin_to_mask(pin);
	u32		pdsr=0;

	if (!pio)
		return -EINVAL;
	pdsr = __raw_readl(pio + GPIO_DR);

	return (pdsr & mask) != 0;
}
EXPORT_SYMBOL(mapu_get_gpio_value);

/* gpiolib support */
static int mapu_gpiolib_direction_input(struct gpio_chip *chip,
					unsigned offset)
{
	struct mapu_gpio_chip *mapu_gpio = to_mapu_gpio_chip(chip);
	void __iomem *pio = mapu_gpio->regbase;
	unsigned mask = 1 << offset;
	unsigned long data = 0;

	data = __raw_readl(pio + GPIO_DDR);

	data &= ~(mask);

	__raw_writel(data, pio + GPIO_DDR);

	return 0;
}

static int mapu_gpiolib_direction_output(struct gpio_chip *chip,
					 unsigned offset, int val)
{
	struct mapu_gpio_chip *mapu_gpio = to_mapu_gpio_chip(chip);
	void __iomem *pio = mapu_gpio->regbase;
	unsigned mask = 1 << offset;
	unsigned long data = 0;
	
	data = __raw_readl(pio + GPIO_DDR);

	data |= mask;

	__raw_writel(data, pio + GPIO_DDR);

	return 0;
}

static int mapu_gpiolib_get(struct gpio_chip *chip, unsigned offset)
{
	struct mapu_gpio_chip *mapu_gpio = to_mapu_gpio_chip(chip);
	void __iomem *pio = mapu_gpio->regbase;
	unsigned mask = 1 << offset;
	u32 pdsr;

	pdsr = __raw_readl(pio + GPIO_DR);

	return (pdsr & mask) != 0;
}

static void mapu_gpiolib_set(struct gpio_chip *chip, unsigned offset, int val)
{
	struct mapu_gpio_chip *mapu_gpio = to_mapu_gpio_chip(chip);
	void __iomem *pio = mapu_gpio->regbase;
	unsigned mask = 1 << offset;
	unsigned long data = 0;

	data = __raw_readl(pio + GPIO_DR);

	if (val == 1)
		data |= mask;
	else
		data &= ~(mask);

	__raw_writel(data, pio + GPIO_DR);

}

static void mapu_gpiolib_dbg_show(struct seq_file *s, struct gpio_chip *chip)
{
	int i;

	for (i = 0; i < chip->ngpio; i++) {
		unsigned pin = chip->base + i;
		const char *gpio_label;

		gpio_label = gpiochip_is_requested(chip, i);
		if (gpio_label) {
			seq_printf(s, "[%s] GPIO%s%d: ",
				   gpio_label, chip->label, i);
			seq_printf(s, "[gpio] %s\n",
				   mapu_get_gpio_value(pin) ?
				   "set" : "clear");
		}
	}
}

/*
 * Called from the processor-specific init to enable GPIO pin support.
 */
void __init mapu_gpio_init(struct mapu_gpio_bank *data, int nr_banks)
{
	unsigned		i;
	struct mapu_gpio_chip *mapu_gpio, *last = NULL;
	int	status = 0;

	BUG_ON(nr_banks > MAX_GPIO_BANKS);

	gpio_banks = nr_banks;

	for (i = 0; i < nr_banks; i++) {
		mapu_gpio = &gpio_chip[i];

		mapu_gpio->bank = &data[i];
		mapu_gpio->chip.base = i * 32;
		mapu_gpio->regbase = mapu_gpio->bank->offset +
			(void __iomem *)MAPU_VA_GPIO;

		/* enable PIO controller's clock */
		/* clk_enable(mapu_gpio->bank->clock); */

		if (last && last->bank->id == mapu_gpio->bank->id)
			last->next = mapu_gpio;
		last = mapu_gpio;

		status = gpiochip_add(&mapu_gpio->chip);
	}
}

