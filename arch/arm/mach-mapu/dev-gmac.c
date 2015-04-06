/*
 * Copyright (c) 2009,2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Base S5P UART resource and device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/irqs.h>
#include <mach/map.h>
#include <mach/regs-gmac.h>
#include <mach/devs.h>

static struct resource mapu_gmac_resource[] = {
	[0] ={
	      .start=MAPU_PA_GMAC,
		  .end=MAPU_PA_GMAC+GMAC_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=IRQ_GMAC,
		 .end=IRQ_GMAC,
		 .flags=IORESOURCE_IRQ,
	}
};

//static u64 mapu_device_lcd_dmamask=0xffffffffUL;

/* mapu devices */ 
extern struct platform_device mapu_gmac_dev= {
	 
        .name="mapu-gmac",
		.id=-1,
        .resource=mapu_gmac_resource,
        .num_resources=ARRAY_SIZE(mapu_gmac_resource),
     .dev= {
//		      .platform_data=&mapu_fb_info ,
//			  .dma_mask=&mapu_device_lcd_dmamask,
			  .coherent_dma_mask=0xffffffffUL,
}		
	 
};

 