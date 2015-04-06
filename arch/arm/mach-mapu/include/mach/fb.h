/* arch/arm/mach-mapu/include/mach/fb.h
 *
 * Copyright (c) 2004 liudandan
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARM_FB_H
#define __ASM_ARM_FB_H

#include <mach/regs-lcd.h>

struct mapu_display_info {

	unsigned type;

	/* Screen size */
	unsigned short width;
	unsigned short height;

	/* Screen info */
	unsigned short xres;
	unsigned short yres;
	unsigned short bpp;
 
};

 

#endif /* __ASM_ARM_FB_H */
