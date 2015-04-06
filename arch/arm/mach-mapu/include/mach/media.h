/* linux/arch/arm/mach-mapu/include/mach/media.h
 *
 *
 * MAPU Media device descriptions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _MAPU_MEDIA_H
#define _MAPU_MEDIA_H

#include <linux/types.h>
#include <asm/setup.h>

/* media  device id */
#define MAPU_MDEV_LCD      0
#define MAPU_MDEV_CAMERA      1
 


struct mapu_media_device {
	u32		id;
	const char 	*name;
	u32		bank;
	size_t		memsize;
	dma_addr_t	paddr;
};

extern struct meminfo meminfo;
extern dma_addr_t mapu_get_media_memory_bank(int dev_id, int bank);
extern size_t mapu_get_media_memsize_bank(int dev_id, int bank);
extern void mapu_reserve_bootmem(void);
#endif

