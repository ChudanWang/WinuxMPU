/* 
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 *MAPU - Memory map definitions (virtual addresses)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MAPVA_H
#define __ASM_ARCH_MAPVA_H __FILE__

/* Fit all our registers in at 0xF6000000 upwards, trying to use as
 * little of the VA space as possible so vmalloc and friends have a
 * better chance of getting memory.
 *
 * we try to ensure stuff like the IRQ registers are available for
 * an single MOVS instruction (ie, only 8 bits of set data)
 */

#define MAPU_ADDR_BASE	0xF6000000

#ifndef __ASSEMBLY__
#define MAPU_ADDR(x)	((void __iomem __force *)MAPU_ADDR_BASE + (x))
#else
#define MAPU_ADDR(x)	(MAPU_ADDR_BASE + (x))
#endif

#define MAPU_VA_IRQ	        	MAPU_ADDR(0x00000000)	/* irq controller(s) */
#define MAPU_VA_SYS	        	MAPU_ADDR(0x00100000)	/* system control */
#define MAPU_VA_GPIO	        MAPU_ADDR(0x00200000)	/* memory control */
#define MAPU_VA_TIMER	    	MAPU_ADDR(0x00300000)	/* timer block */
#define MAPU_VA_WATCHDOG		MAPU_ADDR(0x00400000)	/* watchdog */
#define MAPU_VA_LCD				MAPU_ADDR(0x00500000)	/* lcd */
#define MAPU_VA_GMAC			MAPU_ADDR(0x00600000)	/* gmac */
#define MAPU_VA_GPU         	MAPU_ADDR(0x00700000)   /*gpu*/

#define MAPU_VA_UART	    	MAPU_ADDR(0x01000000)	/* UART */
#define MAPU_VA_UART0	    	MAPU_ADDR(0x01000000)	/* UART0 */
#define MAPU_VA_UART1	    	MAPU_ADDR(0x01010000)	/* UART1 */

#endif /* __ASM_PLAT_MAP_H */
