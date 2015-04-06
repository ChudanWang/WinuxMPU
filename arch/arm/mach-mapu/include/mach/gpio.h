/* linux/arch/arm/mach-mapu/include/mach/gpio.h
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_GPIO_H__
#define __ASM_ARCH_GPIO_H__

#define PIN_BASE   0

#define MAX_GPIO_BANKS 4

#define	MAPU_PIN_PA0	(PIN_BASE + 0x00 + 0)
#define	MAPU_PIN_PA1	(PIN_BASE + 0x00 + 1)
#define	MAPU_PIN_PA2	(PIN_BASE + 0x00 + 2)
#define	MAPU_PIN_PA3	(PIN_BASE + 0x00 + 3)
#define	MAPU_PIN_PA4	(PIN_BASE + 0x00 + 4)
#define	MAPU_PIN_PA5	(PIN_BASE + 0x00 + 5)
#define	MAPU_PIN_PA6	(PIN_BASE + 0x00 + 6)
#define	MAPU_PIN_PA7	(PIN_BASE + 0x00 + 7)
#define	MAPU_PIN_PA8	(PIN_BASE + 0x00 + 8)
#define	MAPU_PIN_PA9	(PIN_BASE + 0x00 + 9)
#define	MAPU_PIN_PA10	(PIN_BASE + 0x00 + 10)
#define	MAPU_PIN_PA11	(PIN_BASE + 0x00 + 11)
#define	MAPU_PIN_PA12	(PIN_BASE + 0x00 + 12)
#define	MAPU_PIN_PA13	(PIN_BASE + 0x00 + 13)
#define	MAPU_PIN_PA14	(PIN_BASE + 0x00 + 14)
#define	MAPU_PIN_PA15	(PIN_BASE + 0x00 + 15)
#define	MAPU_PIN_PA16	(PIN_BASE + 0x00 + 16)
#define	MAPU_PIN_PA17	(PIN_BASE + 0x00 + 17)
#define	MAPU_PIN_PA18	(PIN_BASE + 0x00 + 18)
#define	MAPU_PIN_PA19	(PIN_BASE + 0x00 + 19)
#define	MAPU_PIN_PA20	(PIN_BASE + 0x00 + 20)
#define	MAPU_PIN_PA21	(PIN_BASE + 0x00 + 21)
#define	MAPU_PIN_PA22	(PIN_BASE + 0x00 + 22)
#define	MAPU_PIN_PA23	(PIN_BASE + 0x00 + 23)
#define	MAPU_PIN_PA24	(PIN_BASE + 0x00 + 24)
#define	MAPU_PIN_PA25	(PIN_BASE + 0x00 + 25)
#define	MAPU_PIN_PA26	(PIN_BASE + 0x00 + 26)
#define	MAPU_PIN_PA27	(PIN_BASE + 0x00 + 27)
#define	MAPU_PIN_PA28	(PIN_BASE + 0x00 + 28)
#define	MAPU_PIN_PA29	(PIN_BASE + 0x00 + 29)
#define	MAPU_PIN_PA30	(PIN_BASE + 0x00 + 30)
#define	MAPU_PIN_PA31	(PIN_BASE + 0x00 + 31)

#define	MAPU_PIN_PB0	(PIN_BASE + 0x20 + 0)
#define	MAPU_PIN_PB1	(PIN_BASE + 0x20 + 1)
#define	MAPU_PIN_PB2	(PIN_BASE + 0x20 + 2)
#define	MAPU_PIN_PB3	(PIN_BASE + 0x20 + 3)
#define	MAPU_PIN_PB4	(PIN_BASE + 0x20 + 4)
#define	MAPU_PIN_PB5	(PIN_BASE + 0x20 + 5)
#define	MAPU_PIN_PB6	(PIN_BASE + 0x20 + 6)
#define	MAPU_PIN_PB7	(PIN_BASE + 0x20 + 7)
#define	MAPU_PIN_PB8	(PIN_BASE + 0x20 + 8)
#define	MAPU_PIN_PB9	(PIN_BASE + 0x20 + 9)
#define	MAPU_PIN_PB10	(PIN_BASE + 0x20 + 10)
#define	MAPU_PIN_PB11	(PIN_BASE + 0x20 + 11)
#define	MAPU_PIN_PB12	(PIN_BASE + 0x20 + 12)
#define	MAPU_PIN_PB13	(PIN_BASE + 0x20 + 13)
#define	MAPU_PIN_PB14	(PIN_BASE + 0x20 + 14)
#define	MAPU_PIN_PB15	(PIN_BASE + 0x20 + 15)
#define	MAPU_PIN_PB16	(PIN_BASE + 0x20 + 16)
#define	MAPU_PIN_PB17	(PIN_BASE + 0x20 + 17)
#define	MAPU_PIN_PB18	(PIN_BASE + 0x20 + 18)
#define	MAPU_PIN_PB19	(PIN_BASE + 0x20 + 19)
#define	MAPU_PIN_PB20	(PIN_BASE + 0x20 + 20)
#define	MAPU_PIN_PB21	(PIN_BASE + 0x20 + 21)
#define	MAPU_PIN_PB22	(PIN_BASE + 0x20 + 22)
#define	MAPU_PIN_PB23	(PIN_BASE + 0x20 + 23)
#define	MAPU_PIN_PB24	(PIN_BASE + 0x20 + 24)
#define	MAPU_PIN_PB25	(PIN_BASE + 0x20 + 25)
#define	MAPU_PIN_PB26	(PIN_BASE + 0x20 + 26)
#define	MAPU_PIN_PB27	(PIN_BASE + 0x20 + 27)
#define	MAPU_PIN_PB28	(PIN_BASE + 0x20 + 28)
#define	MAPU_PIN_PB29	(PIN_BASE + 0x20 + 29)
#define	MAPU_PIN_PB30	(PIN_BASE + 0x20 + 30)
#define	MAPU_PIN_PB31	(PIN_BASE + 0x20 + 31)

#define	MAPU_PIN_PC0	(PIN_BASE + 0x40 + 0)
#define	MAPU_PIN_PC1	(PIN_BASE + 0x40 + 1)
#define	MAPU_PIN_PC2	(PIN_BASE + 0x40 + 2)
#define	MAPU_PIN_PC3	(PIN_BASE + 0x40 + 3)
#define	MAPU_PIN_PC4	(PIN_BASE + 0x40 + 4)
#define	MAPU_PIN_PC5	(PIN_BASE + 0x40 + 5)
#define	MAPU_PIN_PC6	(PIN_BASE + 0x40 + 6)
#define	MAPU_PIN_PC7	(PIN_BASE + 0x40 + 7)
#define	MAPU_PIN_PC8	(PIN_BASE + 0x40 + 8)
#define	MAPU_PIN_PC9	(PIN_BASE + 0x40 + 9)
#define	MAPU_PIN_PC10	(PIN_BASE + 0x40 + 10)
#define	MAPU_PIN_PC11	(PIN_BASE + 0x40 + 11)
#define	MAPU_PIN_PC12	(PIN_BASE + 0x40 + 12)
#define	MAPU_PIN_PC13	(PIN_BASE + 0x40 + 13)
#define	MAPU_PIN_PC14	(PIN_BASE + 0x40 + 14)
#define	MAPU_PIN_PC15	(PIN_BASE + 0x40 + 15)
#define	MAPU_PIN_PC16	(PIN_BASE + 0x40 + 16)
#define	MAPU_PIN_PC17	(PIN_BASE + 0x40 + 17)
#define	MAPU_PIN_PC18	(PIN_BASE + 0x40 + 18)
#define	MAPU_PIN_PC19	(PIN_BASE + 0x40 + 19)
#define	MAPU_PIN_PC20	(PIN_BASE + 0x40 + 20)
#define	MAPU_PIN_PC21	(PIN_BASE + 0x40 + 21)
#define	MAPU_PIN_PC22	(PIN_BASE + 0x40 + 22)
#define	MAPU_PIN_PC23	(PIN_BASE + 0x40 + 23)
#define	MAPU_PIN_PC24	(PIN_BASE + 0x40 + 24)
#define	MAPU_PIN_PC25	(PIN_BASE + 0x40 + 25)
#define	MAPU_PIN_PC26	(PIN_BASE + 0x40 + 26)
#define	MAPU_PIN_PC27	(PIN_BASE + 0x40 + 27)
#define	MAPU_PIN_PC28	(PIN_BASE + 0x40 + 28)
#define	MAPU_PIN_PC29	(PIN_BASE + 0x40 + 29)
#define	MAPU_PIN_PC30	(PIN_BASE + 0x40 + 30)
#define	MAPU_PIN_PC31	(PIN_BASE + 0x40 + 31)

#define	MAPU_PIN_PD0	(PIN_BASE + 0x60 + 0)
#define	MAPU_PIN_PD1	(PIN_BASE + 0x60 + 1)
#define	MAPU_PIN_PD2	(PIN_BASE + 0x60 + 2)
#define	MAPU_PIN_PD3	(PIN_BASE + 0x60 + 3)
#define	MAPU_PIN_PD4	(PIN_BASE + 0x60 + 4)
#define	MAPU_PIN_PD5	(PIN_BASE + 0x60 + 5)
#define	MAPU_PIN_PD6	(PIN_BASE + 0x60 + 6)
#define	MAPU_PIN_PD7	(PIN_BASE + 0x60 + 7)
#define	MAPU_PIN_PD8	(PIN_BASE + 0x60 + 8)
#define	MAPU_PIN_PD9	(PIN_BASE + 0x60 + 9)
#define	MAPU_PIN_PD10	(PIN_BASE + 0x60 + 10)
#define	MAPU_PIN_PD11	(PIN_BASE + 0x60 + 11)
#define	MAPU_PIN_PD12	(PIN_BASE + 0x60 + 12)
#define	MAPU_PIN_PD13	(PIN_BASE + 0x60 + 13)
#define	MAPU_PIN_PD14	(PIN_BASE + 0x60 + 14)
#define	MAPU_PIN_PD15	(PIN_BASE + 0x60 + 15)
#define	MAPU_PIN_PD16	(PIN_BASE + 0x60 + 16)
#define	MAPU_PIN_PD17	(PIN_BASE + 0x60 + 17)
#define	MAPU_PIN_PD18	(PIN_BASE + 0x60 + 18)
#define	MAPU_PIN_PD19	(PIN_BASE + 0x60 + 19)
#define	MAPU_PIN_PD20	(PIN_BASE + 0x60 + 20)
#define	MAPU_PIN_PD21	(PIN_BASE + 0x60 + 21)
#define	MAPU_PIN_PD22	(PIN_BASE + 0x60 + 22)
#define	MAPU_PIN_PD23	(PIN_BASE + 0x60 + 23)
#define	MAPU_PIN_PD24	(PIN_BASE + 0x60 + 24)
#define	MAPU_PIN_PD25	(PIN_BASE + 0x60 + 25)
#define	MAPU_PIN_PD26	(PIN_BASE + 0x60 + 26)
#define	MAPU_PIN_PD27	(PIN_BASE + 0x60 + 27)
#define	MAPU_PIN_PD28	(PIN_BASE + 0x60 + 28)
#define	MAPU_PIN_PD29	(PIN_BASE + 0x60 + 29)
#define	MAPU_PIN_PD30	(PIN_BASE + 0x60 + 30)
#define	MAPU_PIN_PD31	(PIN_BASE + 0x60 + 31)

extern int __init_or_module mapu_set_gpio_input(unsigned pin, int use_pullup);
extern int __init_or_module mapu_set_gpio_output(unsigned pin, int value);
extern int mapu_set_gpio_value(unsigned pin, int value);
extern int mapu_get_gpio_value(unsigned pin);

#define gpio_get_value  __gpio_get_value
#define gpio_set_value	__gpio_set_value
#define gpio_cansleep	__gpio_cansleep
#define gpio_to_irq(gpio) NULL

#include <asm-generic/gpio.h>

#endif /* __ASM_ARCH_GPIO_H__ */
