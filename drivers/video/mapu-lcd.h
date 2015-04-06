/*
 * linux/drivers/video/MAPUfb.h
 *	Copyright (c) 2004 Arnaud Patard
 *
 *  MAPU LCD Framebuffer Driver
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
*/

#ifndef __MAPUFB_H
#define __MAPUFB_H

 

struct mapu_fb_info {
	struct device		*dev;
	struct clk		*clk;

	struct resource		*mem;
	void __iomem		*io;

	unsigned long		clk_rate;
	unsigned int		palette_ready;

#ifdef CONFIG_CPU_FREQ
	struct notifier_block	freq_transition;
#endif

	/* keep these registers in case we need to re-write palette */
	u32			palette_buffer[256];
	u32			pseudo_pal[16];
};

#define PALETTE_BUFF_CLEAR (0x80000000)	/* entry is clear/invalid */

int mapu_fb_init(void);

#endif
