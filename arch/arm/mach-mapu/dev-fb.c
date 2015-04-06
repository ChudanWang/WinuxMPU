/*
* Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * fb resource and device definitions
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
#include <mach/regs-lcd.h>
#include <mach/devs.h>
#include <mach/fb.h>

static struct mapu_display_info mapu_fb_info={
		.width=1920,
		.height=1080,
		.xres=1920,
		.yres=1080,
		.bpp=32,
		
};  

static struct resource mapu_lcd_resource[] = {
	[0] ={
	      .start=MAPU_PA_LCD,
		  .end=MAPU_PA_LCD+LCD_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=IRQ_LCD,
		 .end=IRQ_LCD,
		 .flags=IORESOURCE_IRQ,
	}
};

static u64 mapu_device_lcd_dmamask=0xffffffffUL;

/* lcd devices */ 
extern struct platform_device mapu_lcd_dev= {
	 
        .name="mapu-lcd",
		.id=-1,
        .resource=mapu_lcd_resource,
        .num_resources=ARRAY_SIZE(mapu_lcd_resource),
        .dev= {
		      .platform_data=&mapu_fb_info ,
			  .dma_mask=&mapu_device_lcd_dmamask,
			  .coherent_dma_mask=0xffffffffUL,
		}		
	 
};

 