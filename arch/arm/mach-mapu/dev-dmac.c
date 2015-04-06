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
#include <mach/regs-dmac.h>
#include <mach/devs.h>
  
 static struct mapu_dma_platform_data dmac_pdata={
	 //.nr_channels=2,
	 .nr_channels=1,
 };
 
 static struct resource mapu_dmac_resource[] = {
	 [0] ={
		   .start=MAPU_PA_DMAC,
		   .end=MAPU_PA_DMAC+DMAC_SIZE-1,
		   .flags=IORESOURCE_MEM,
	 },
	 [1] ={
		  .start=IRQ_DMA,
		  .end=IRQ_DMA,
		  .flags=IORESOURCE_IRQ,
	 }
 };
 
 static u64 mapu_device_dmac_dmamask=0xffffffffUL;
 
 /* lcd devices */ 
 extern struct platform_device mapu_dmac_dev= {
	  
		 .name="dw_dmac",
		 .id=-1,
		 .resource=mapu_dmac_resource,
		 .num_resources=ARRAY_SIZE(mapu_dmac_resource),
		 .dev= {
				
			   .dma_mask=&mapu_device_dmac_dmamask,
			   .coherent_dma_mask=0xffffffffUL,
			   .platform_data=&dmac_pdata,
		 }		 
	  
 };

