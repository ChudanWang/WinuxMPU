/* linux/arch/arm/mach-mapu/include/mach/map.h
 *
 *   Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H __FILE__

 //#define DEBUG

#define MAPU_PA_SDRAM		    0x80000000

#define MAPU_PA_SYSCON		    0x50300000
#define MAPU_PA_GPIO			0x50800000
#define MAPU_PA_IIC0			0x50a00000
#define MAPU_PA_IIC1			0x50a10000
#define MAPU_PA_IIC2			0x50a20000
#define MAPU_PA_IIC3			0x50a30000
#define MAPU_PA_TIMER		    0x50400000
#define MAPU_PA_WATCHDOG		0x50440000
#define MAPU_PA_RTC			    0x50480000
#define MAPU_PA_UART0			0x50900000
#define MAPU_PA_UART1			0x50910000 
#define MAPU_PA_IIS0			0x50b00000 
#define MAPU_PA_IIS1			0x50b10000
#define MAPU_PA_INTC			0x503C0000
#define MAPU_PA_DMAC            0x50000000
#define MAPU_PA_CAM             0x50500000
#define MAPU_PA_LCD             0x50504000
#define MAPU_PA_SD           	0x50d00000
#define MAPU_PA_SPI0		0x50ff0000
#define MAPU_PA_SPI1			    0x51000000 
#define MAPU_PA_GMAC			0x50C00000 
#define MAPU_PA_GPU				0x50540000

#define UART_SIZE               SZ_64K
#define LCD_SIZE               	SZ_64K
#define GMAC_SIZE               SZ_128K
#define SD_SIZE  			SZ_64K
#define SPI_SIZE                SZ_64K
#define GPU_SIZE				SZ_64K
#define DMAC_SIZE				SZ_64K
#endif /* __ASM_ARCH_MAP_H */
