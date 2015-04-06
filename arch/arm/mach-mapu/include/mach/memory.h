/* linux/arch/arm/mach-mach/include/mach/memory.h
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

 
#define PHYS_OFFSET		UL(0x80000000)
#define CONSISTENT_DMA_SIZE	(SZ_8M + SZ_4M + SZ_2M)
/*
 * Sparsemem support
 * Physical memory can be located from 0x20000000 to 0x7fffffff,
 * so MAX_PHYSMEM_BITS is 31.
 */

#define MAX_PHYSMEM_BITS	32   //31
#define SECTION_SIZE_BITS	28

#endif /* __ASM_ARCH_MEMORY_H */
