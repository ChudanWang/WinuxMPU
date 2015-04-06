/* linux/arch/arm/mach-mapu/include/mach/system.h
 *
	* Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H __FILE__

//#include <plat/watchdog-reset.h>

static void arch_idle(void)
{
	/* nothing here yet */
}

static void arch_reset(char mode, const char *cmd)
{
//	if (mode != 's')
	//	arch_wdt_reset();

	/* if all else fails, or mode was for soft, jump to 0 */
	cpu_reset(0);
}

#endif /* __ASM_ARCH_SYSTEM_H */
