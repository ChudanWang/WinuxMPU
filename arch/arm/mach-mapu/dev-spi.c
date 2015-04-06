/* linux/arch/arm/mach-mapu/dev-spi.c
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/spi/spi.h>
#include <linux/dma-mapping.h>

#include <mach/map.h>
#include <mach/regs-spi.h>
#include <mach/spi.h>
#include <mach/devs.h>
#include <mach/irqs.h>
#include <mach/gpio.h>

/* SPI (0) */
static u64 spi_dmamask = DMA_BIT_MASK(32);

static struct resource mapu_spi0_resource[] = {
	[0] = {
		.start = MAPU_PA_SPI0,
		.end   = MAPU_PA_SPI0 + SPI_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SPI0,
		.end   = IRQ_SPI0,
		.flags = IORESOURCE_IRQ,
	}
};

static struct platform_device mapu_spi0_dev = {
	.name		= "mapu-spi",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(mapu_spi0_resource),
	.resource	= mapu_spi0_resource,
	.dev		= {
		.dma_mask = &spi_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};

static const unsigned spi0_standard_cs[2] = { MAPU_PIN_PA0, MAPU_PIN_PA1 };

void __init mapu_add_device_spi(struct spi_board_info *devices, int nr_devices)
{
	int i;
	unsigned long cs_pin;
	short enable_spi0 = 0;
	short enable_spi1 = 0;

	/* Choose SPI chip-selets */
	for (i=0; i<nr_devices; i++) {
		if (devices[i].controller_data)
			cs_pin = (unsigned long) devices[i].controller_data;
		else if (devices[i].bus_num == 0)
			cs_pin = spi0_standard_cs[devices[i].chip_select];
		/* else
			cs_pin = spi1_standard_cs[devices[i].chip_select];
		*/

		if (devices[i].bus_num == 0)
			enable_spi0 = 1;
		else
			enable_spi1 = 1;

		mapu_set_gpio_output(cs_pin, 1);

		devices[i].controller_data = (void *) cs_pin;
	}

	spi_register_board_info(devices, nr_devices);

	if (enable_spi0) {
		/* printk("nisp: mapu_add_device_spi\n"); */
		platform_device_register(&mapu_spi0_dev);
	}
}
