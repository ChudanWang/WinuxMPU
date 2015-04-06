/*
* Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * SD resource and device definitions
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
#include <linux/mmc/host.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/irqs.h>
#include <mach/map.h>
#include <mach/regs-sdhci.h>
#include <mach/devs.h>
 

static struct mapu_sdhci_platdata mapu_sdhci_data={
		.max_width=4,
		.host_caps=MMC_CAP_4_BIT_DATA|MMC_CAP_MMC_HIGHSPEED|MMC_CAP_SD_HIGHSPEED,
 
};  

static struct resource mapu_sdhci_resource[] = {
	[0] ={
	      .start=MAPU_PA_SD,
		  .end=MAPU_PA_SD+SD_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=IRQ_SD,
		 .end=IRQ_SD,
		 .flags=IORESOURCE_IRQ,
	}
};

static u64 mapu_device_sdhci_dmamask=0xffffffffUL;

/* sdhci devices */ 
extern struct platform_device mapu_sdhci_dev= {
	 
        .name="mapu-sdhci",
		.id=-1,
        .resource=mapu_sdhci_resource,
        .num_resources=ARRAY_SIZE(mapu_sdhci_resource),
        .dev= {
		      .platform_data=&mapu_sdhci_data ,
			  .dma_mask=&mapu_device_sdhci_dmamask,
			  .coherent_dma_mask=0xffffffffUL,
		}		
	 
};

 