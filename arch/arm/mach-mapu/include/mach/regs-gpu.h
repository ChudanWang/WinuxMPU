/* arch/arm/mach-mapu/include/mach/gpu.h
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * gpu definitions for mapu soc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/


#ifndef __MAPU_GPU_H__
#define __MAPU_GPU_H__

#define MAPU_GPU_NAME_UTGARD "mapu-gpu"

/* Configuration for the EB platform with ZBT memory enabled */
/*zepplin added 2010.08.17 for orion configuration*/
#define GPU_BASE_ADDR 		   0x13000000
#define GP_ADDR 	GPU_BASE_ADDR
#define L2_ADDR 	GPU_BASE_ADDR+0x1000
#define PMU_ADDR 	GPU_BASE_ADDR+0x2000
#define GP_MMU_ADDR 	GPU_BASE_ADDR+0x3000
#define PP0_MMU_ADDR 	GPU_BASE_ADDR+0x4000
#define PP1_MMU_ADDR 	GPU_BASE_ADDR+0x5000
#define PP2_MMU_ADDR 	GPU_BASE_ADDR+0x6000
#define PP3_MMU_ADDR 	GPU_BASE_ADDR+0x7000
#define PP0_ADDR 	GPU_BASE_ADDR+0x8000
#define PP1_ADDR 	GPU_BASE_ADDR+0xA000
#define PP2_ADDR 	GPU_BASE_ADDR+0xC000
#define PP3_ADDR 	GPU_BASE_ADDR+0xE000

/*for mmu and os memory*/
#define MEM_BASE_ADDR    0x40000000
#define MEM_TOTAL_SIZE   0x40000000
#define MEM_MAPU_OS_SIZE 0x18000000

/*for dedicated memory*/
#define MEM_MAPU_SIZE 	 CONFIG_MAPU_MEM_SIZE*1024*1024
#define MEM_MAPU_BASE    0x60000000 - MEM_MAPU_SIZE 

struct mapu_gpu_device_data
{
	/* Dedicated GPU memory range (physical). */
	unsigned long dedicated_mem_start;
	unsigned long dedicated_mem_size;

	/* Shared GPU memory */
	unsigned long shared_mem_size;

	/* Frame buffer memory to be accessible by Mali GPU (physical) */
	unsigned long fb_start;
	unsigned long fb_size;

	/* Report GPU utilization in this interval (specified in ms) */
	unsigned long utilization_interval;

};

#endif /* __ARCH_CONFIG_H__ */

