/* linux/arch/arm/mach-mapu/bootmem.c
 *
 * Copyright (c) 2009 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Bootmem helper functions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/swap.h>
#include <asm/setup.h>
#include <linux/io.h>
#include <mach/memory.h>
#include <mach/media.h>
 

static struct mapu_media_device media_devs[] = {
 
#ifdef CONFIG_FB_MAPU
	{
		.id = MAPU_MDEV_LCD,
		.name = "lcd",
		.bank = 1,
		.memsize = 14336 * SZ_1K,
		.paddr = 0,
	},
 
#endif
#ifdef CONFIG_CAMERA_MAPU
	{
		.id = MAPU_MDEV_CAMERA,
		.name = "camera",
		.bank = 1,
		.memsize = CONFIG_VIDEO_MAPU_MEMSIZE_FIMC1 * SZ_1K,
		.paddr = 0,
	},
#endif

};

static struct mapu_media_device *mapu_get_media_device(int dev_id, int bank)
{
	struct mapu_media_device *mdev = NULL;
	int i = 0, found = 0, nr_devs;
	nr_devs = sizeof(media_devs) / sizeof(media_devs[0]);

	if (dev_id < 0)
		return NULL;

	while (!found && (i < nr_devs)) {
		mdev = &media_devs[i];
		if (mdev->id == dev_id && mdev->bank == bank)
			found = 1;
		else
			i++;
	}

	if (!found)
		mdev = NULL;

	return mdev;
}

dma_addr_t mapu_get_media_memory_bank(int dev_id, int bank)
{
	struct mapu_media_device *mdev;

	mdev = mapu_get_media_device(dev_id, bank);
	if (!mdev) {
		printk(KERN_ERR "invalid media device\n");
		return 0;
	}

	if (!mdev->paddr) {
		printk(KERN_ERR "no memory for %s\n", mdev->name);
		return 0;
	}

	return mdev->paddr;
}
EXPORT_SYMBOL(mapu_get_media_memory_bank);

size_t mapu_get_media_memsize_bank(int dev_id, int bank)
{
	struct mapu_media_device *mdev;

	mdev = mapu_get_media_device(dev_id, bank);
	if (!mdev) {
		printk(KERN_ERR "invalid media device\n");
		return 0;
	}

	return mdev->memsize;
}
EXPORT_SYMBOL(mapu_get_media_memsize_bank);

void mapu_reserve_bootmem(void)
{
	struct mapu_media_device *mdev;
	int i, nr_devs;

	nr_devs = sizeof(media_devs) / sizeof(media_devs[0]);
	for (i = 0; i < nr_devs; i++) {
		mdev = &media_devs[i];
		if (mdev->memsize <= 0)
			continue;

		mdev->paddr = virt_to_phys(__alloc_bootmem(mdev->memsize,
				PAGE_SIZE, meminfo.bank[mdev->bank].start));
		printk(KERN_INFO "MAPU: %lu bytes system memory reserved "
			"for %s at 0x%08x\n", (unsigned long) mdev->memsize,
			mdev->name, mdev->paddr);
	}
}

/* FIXME: temporary implementation to avoid compile error */
int dma_needs_bounce(struct device *dev, dma_addr_t addr, size_t size)
{
	return 0;
}
