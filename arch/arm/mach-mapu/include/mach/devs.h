/* arch/arm/mach-mapu/include/mach/devs.h
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *	liu dandan <948144038@qq.com>
 *
 * Header file for mapu standard platform devices
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __MACH_DEVS_H
#define __MACH_DEVS_H __FILE__

#include <linux/platform_device.h>


extern struct platform_device mapu_uart_dev;

extern struct platform_device mapu_lcd_dev;

extern struct platform_device mapu_sdhci_dev;

extern struct platform_device mapu_spi_dev;

extern struct platform_device mapu_gpu_dev;

extern struct platform_device mapu_gmac_dev;

extern struct platform_device mapu_dmac_dev;

struct mapu_gpio_bank {
	unsigned short id;		/* peripheral ID */
	unsigned long offset;		/* offset from system peripheral base */
	struct clk *clock;		/* associated clock */
};
extern void __init mapu_gpio_init(struct mapu_gpio_bank *, int nr_banks);

 /* SPI */
extern void __init mapu_add_device_spi(struct spi_board_info *devices, int nr_devices);


#endif /* __PLAT_DEVS_H */
