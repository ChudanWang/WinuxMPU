/* linux/arch/arm/mach-mapu/include/mach/uncompress.h
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences)
 *
 * MAPU - uncompress code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <linux/serial_reg.h>
#include <mach/map.h>

static volatile unsigned long *UART = (unsigned long *)MAPU_PA_UART0;
 
static void putc(const char c)
{	 
	while (!(UART[UART_LSR] & UART_LSR_THRE))
		barrier();
	UART[UART_TX] = c;

}

static void arch_detect_cpu(void)
{
	/* we do not need to do any cpu detection here at the moment. */
}
static inline void flush(void)
{
}

static inline void arch_decomp_wdog(void)
{
}
static void arch_decomp_setup(void)
{
}
#endif /* __ASM_ARCH_UNCOMPRESS_H */
