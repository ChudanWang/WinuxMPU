/* linux/arch/arm/mach-mapu/cpu.c
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *	liu dandan <948144038@qq.com>
 *
 *  CPU initialisation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <mach/cpu.h>

void __init mapu_init_cpu(struct cpu_table *cputab, unsigned int cputab_size)
{

struct cpu_table *cpu = cputab;

	if (cpu == NULL) {
		printk(KERN_ERR "Unknown CPU type \n");
		panic("Unknown  CPU");
	}

	printk("CPU :%s \n", cpu->name);
	
	if(cpu->init)
		cpu->init();
	
	if(cpu->map_io)
		cpu->map_io();
 
}
 
