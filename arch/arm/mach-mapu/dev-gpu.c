/*
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * GPU resource and device definitions
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
#include <mach/regs-gpu.h>
#include <mach/devs.h>

static struct mapu_gpu_device_data mapu_gpu_data = {
	.shared_mem_size = 1024*1024*1024,
};

static struct resource mapu_gpu_resource[] = {
	[0] ={
	      .start=MAPU_PA_GPU,
		  .end=MAPU_PA_GPU+GPU_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=IRQ_GP_3D,
		 .end=IRQ_GP_3D,
		 .flags=IORESOURCE_IRQ,
	}
};


struct platform_device mapu_gpu_dev = {
	.name		= MAPU_GPU_NAME_UTGARD,
	.id		= 0,
	.num_resources	= ARRAY_SIZE(mapu_gpu_resource),
	.resource	= mapu_gpu_resource,
	.dev.platform_data = &mapu_gpu_data,
};

 