/* linux/drivers/video/mapu-lcd.c
 *
 * MAPU LCD Framebuffer Driver
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Driver based on skeletonfb.c, sa1100fb.c and others.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>

#include <asm/io.h>
#include <asm/div64.h>

#include <asm/mach/map.h>
#include <mach/regs-lcd.h>
#include <mach/fb.h>
#include <mach/media.h>

#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

#include "mapu-lcd.h"

/* Debugging stuff */
#ifdef CONFIG_FB_MAPU_DEBUG
static int debug	= 1;
#else
static int debug	= 0;
#endif

#define dprintk(msg...)	if (debug) { printk("mapu-fb: " msg); }

 #ifndef CONFIG_FRAMEBUFFER_CONSOLE
static int mapu_draw_logo(struct fb_info *fb)
{
 
	struct fb_fix_screeninfo *fix = &fb->fix;
	struct fb_var_screeninfo *var = &fb->var;
 
	u32 height = 720 / 3;
	u32 line = 1280*4;
	u32 i, j;
	//u32 base=fb->fix.smem_start;
	for (i = 0; i < height; i++) {
		for (j = 0; j < 1280; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0xff, 1);
		}
	}

	for (i = height; i < height * 2; i++) {
		for (j = 0; j < 1280; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0xff, 1);
		}
	}

	for (i = height * 2; i < height * 3; i++) {
		for (j = 0; j < 1280; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0xff, 1);
		}
	}
	dprintk("%s: fb->screen_base 1 = %x\n", __func__, fb->screen_base);
	return 0;
}
#else
static int mapu_draw_logo(struct fb_info *fb)
{
return 0;
}
#endif
/* mapu_fb_set_lcdaddr
 *
 * initialise lcd controller address pointers
 */
static void mapu_fb_set_lcdaddr(struct fb_info *info)
{
	unsigned long saddr ;
	struct mapu_fb_info *fbi = info->par;
	void __iomem *regs = fbi->io;

	saddr  = info->fix.smem_start;
    writel(saddr, regs + LCD_LAY0_BASE); 
	 
}
 

/*
 *	mapu_fb_check_var():
 *	Get the video params out of 'var'. If a value doesn't fit, round it up,
 *	if it's too big, return -EINVAL.
 *
 */
static int mapu_fb_check_var(struct fb_var_screeninfo *var,
			       struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;
	struct mapu_display_info *mach_info = fbi->dev->platform_data;
	 
	unsigned i;

	dprintk("check_var(var=%p, info=%p)\n", var, info);

	/* validate x/y resolution */
	/* choose default mode if possible */
	if (!(var->yres == mach_info->yres &&
	    var->xres == mach_info->xres &&
	    var->bits_per_pixel == mach_info->bpp))
	{
		dprintk("wrong resolution or depth %dx%d at %d bpp\n",
			var->xres, var->yres, var->bits_per_pixel);
		return -EINVAL;
	}

	/* it is always the size as the display */
	var->xres_virtual = mach_info->xres;
	var->yres_virtual = mach_info->yres;
	var->height = mach_info->height;
	var->width = mach_info->width;

	 
	/* set r/g/b positions  24 bpp 888 and 8 alpha */
	
        var->transp.length		= 8;
		var->transp.offset		= 24;
		var->red.length		= 8;
		var->red.offset		= 16;
		var->green.length	= 8;
		var->green.offset	= 8;
		var->blue.length	= 8;
		var->blue.offset	= 0;
 
	return 0;
}

static unsigned int mapu_fb_calculate_size(unsigned int xres,unsigned int yres)
{
  unsigned int size;
  size=(xres&0xffff)|((yres<<16)&0xffff0000);
  return size;
}
/* mapu_fb_activate_var
 *
 * activate (set) the controller from the given framebuffer
 * information
 */
static void mapu_fb_activate_var(struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;
	void __iomem *regs = fbi->io;
	struct fb_var_screeninfo *var = &info->var;
	unsigned int size; 
	dprintk("%s: var->xres  = %x\n", __func__, var->xres);
	dprintk("%s: var->yres  = %x\n", __func__, var->yres);
	dprintk("%s: var->bpp   = %d\n", __func__, var->bits_per_pixel);
    size=mapu_fb_calculate_size(var->xres,var->yres);
	dprintk("%s: size is  = %x\n", __func__, size);
 
 
	writel(size, regs + LCD_LAY0_SIZE);
    writel(size, regs + LCD_LAY0_WIN_SIZE);

	/* set lcd address pointers */
	mapu_fb_set_lcdaddr(info);

}

/*
 *      mapu_fb_set_par - Alters the hardware state.
 *      @info: frame buffer structure that represents a single frame buffer
 *
 */
static int mapu_fb_set_par(struct fb_info *info)
{
	struct fb_var_screeninfo *var = &info->var;

	switch (var->bits_per_pixel) {
	case 32:
	case 16:
	case 12:
		info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 1:
		info->fix.visual = FB_VISUAL_MONO01;
		break;
	default:
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		break;
	}

	info->fix.line_length = (var->xres_virtual * var->bits_per_pixel) / 8;

	/* activate this new configuration */

	mapu_fb_activate_var(info);
	return 0;
}

static void schedule_palette_update(struct mapu_fb_info *fbi,
				    unsigned int regno, unsigned int val)
{
 
}

/* from pxafb.c */
static inline unsigned int __chan_to_field(unsigned int chan,
					 struct fb_bitfield bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf.length;
	return chan << bf.offset;
}

static int mapu_fb_setcolreg(unsigned regno,
			       unsigned red, unsigned green, unsigned blue,
			       unsigned transp, struct fb_info *info)
{
	unsigned int *pal = (unsigned int *)info->pseudo_palette;
	unsigned int val = 0;
	/* dprintk("setcol: regno=%d, rgb=%d,%d,%d\n",
		   regno, red, green, blue); */
	if (regno < 16) {
		/* fake palette of 16 colors */
		val |= __chan_to_field(red, info->var.red);
		val |= __chan_to_field(green, info->var.green);
		val |= __chan_to_field(blue, info->var.blue);
		val |= __chan_to_field(transp, info->var.transp);

		pal[regno] = val;
	}


	return 0;
}

/* mapu_fb_lcd_enable
 *
 * shutdown the lcd controller
 */
static void mapu_fb_lcd_enable(struct mapu_fb_info *fbi, int enable)
{
	unsigned long flags;
    unsigned int val;
	local_irq_save(flags);
	
    val = readl(fbi->io + LCD_LAYER_EN);
	if (enable)
		val|= ENABLE_DIS;
	else
		val&= ~ENABLE_DIS;
 
	writel(val, fbi->io +LCD_LAYER_EN);

	local_irq_restore(flags);
}


/*
 *      mapu_fb_blank
 *	@blank_mode: the blank mode we want.
 *	@info: frame buffer structure that represents a single frame buffer
 *
 *	Blank the screen if blank_mode != 0, else unblank. Return 0 if
 *	blanking succeeded, != 0 if un-/blanking failed due to e.g. a
 *	video mode which doesn't support it. Implements VESA suspend
 *	and powerdown modes on hardware that supports disabling hsync/vsync:
 *
 *	Returns negative errno on error, or zero on success.
 *
 */
static int mapu_fb_blank(int blank_mode, struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;
	void __iomem *regs = fbi->io;
    unsigned int val;
	val=readl(fbi->io +LCD_CON);
	dprintk("blank(mode=%d, info=%p)\n", blank_mode, info);


	if (blank_mode == FB_BLANK_POWERDOWN) {
		mapu_fb_lcd_enable(fbi, 0);
	} else {
		mapu_fb_lcd_enable(fbi, 1);
	}

	if (blank_mode == FB_BLANK_UNBLANK)
		 {
		 val|=TEST_MODE2;
		 val|=0x00ffffff;
//		 writel(val, fbi->io +LCD_CON);
		 }
	return 0;
}

static int mapu_fb_debug_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", debug ? "on" : "off");
}

static int mapu_fb_debug_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t len)
{
	if (len < 1)
		return -EINVAL;

	if (strnicmp(buf, "on", 2) == 0 ||
	    strnicmp(buf, "1", 1) == 0) {
		debug = 1;
		printk(KERN_DEBUG "mapu_fb: Debug On");
	} else if (strnicmp(buf, "off", 3) == 0 ||
		   strnicmp(buf, "0", 1) == 0) {
		debug = 0;
		printk(KERN_DEBUG "mapu_fb: Debug Off");
	} else {
		return -EINVAL;
	}

	return len;
}

static DEVICE_ATTR(debug, 0666, mapu_fb_debug_show, mapu_fb_debug_store);

static struct fb_ops mapu_fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= mapu_fb_check_var,
	.fb_set_par	= mapu_fb_set_par,
	.fb_blank	= mapu_fb_blank,
	.fb_setcolreg	= mapu_fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};

/*
 * mapu_fb_map_video_memory():
 *	Allocates the DRAM memory for the frame buffer.  This buffer is
 *	remapped into a non-cached, non-buffered, memory region to
 *	allow palette and pixel writes to occur without flushing the
 *	cache.  Once this area is remapped, all virtual memory
 *	access to the video memory should occur at the new region.
 */
static int __init mapu_fb_map_video_memory(struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;
	dma_addr_t map_dma;
	unsigned map_size = PAGE_ALIGN(info->fix.smem_len+PAGE_SIZE);

	dprintk("map_video_memory(fbi=%p) map_size %08x\n", fbi, map_size);

	info->screen_base = dma_alloc_coherent(fbi->dev, map_size,
						   &map_dma, GFP_KERNEL);
	//info->screen_base=kmalloc(map_size, GFP_KERNEL|GFP_DMA);

	if (info->screen_base) {
		/* prevent initial garbage on screen */
		dprintk("map_video_memory: clear %p:%08x\n",
			info->screen_base, map_size);
		memset(info->screen_base, 0x00, map_size);
		//map_dma=virt_to_dma(fbi->dev, info->screen_base);
		info->fix.smem_start = map_dma;

		dprintk("map_video_memory: dma=%08lx cpu=%p size=%08x\n",
			info->fix.smem_start, info->screen_base, map_size);
	}

	return info->screen_base ? 0 : -ENOMEM;
}

static int __init mapu_fb_map_default_video_memory(struct fb_info *fb)
{
	struct fb_fix_screeninfo *fix = &fb->fix;
	int reserved_size = 0;

	 

	fix->smem_start = mapu_get_media_memory_bank(MAPU_MDEV_LCD, 1);
	reserved_size = mapu_get_media_memsize_bank(MAPU_MDEV_LCD, 1);
	fb->screen_base = ioremap_wc(fix->smem_start, reserved_size);

	if (!fb->screen_base)
		return -ENOMEM;
	else
		dprintk("map_video_memory: dma: 0x%08x, cpu: 0x%08x, "
			"size: 0x%08x\n", 
			(unsigned int)fix->smem_start,
			(unsigned int)fb->screen_base, fix->smem_len);

	memset(fb->screen_base, 0, fix->smem_len);
	 

	return 0;
}

static inline void mapu_fb_unmap_video_memory(struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;

	dma_free_writecombine(fbi->dev, PAGE_ALIGN(info->fix.smem_len),
			      info->screen_base, info->fix.smem_start);
}

static int mapu_fb_unmap_default_video_memory(struct fb_info *fb)
{
	struct fb_fix_screeninfo *fix = &fb->fix;
	 

	if (fix->smem_start) {
		iounmap(fb->screen_base);
		fix->smem_start = 0;
		fix->smem_len = 0;
		dprintk( " video memory released\n");
	}

	return 0;
}
/*
 * mapu_fb_init_registers - Initialise all LCD-related registers
 */
static int mapu_fb_init_registers(struct fb_info *info)
{
	struct mapu_fb_info *fbi = info->par;
	struct mapu_display_info *mach_info = fbi->dev->platform_data;
	unsigned long flags;
	void __iomem *regs = fbi->io;
	 
	writel(0x001e000a, regs + LCD_VER_SYN); 
	writel(0x00040438, regs + LCD_VER_ACT);
	writel(0x0094002c, regs + LCD_HOR_SYN);
	writel(0x00580780, regs + LCD_HOR_ACT);
	
	writel(0x70000000, regs + LCD_CON);
	
	writel(0x80000007, regs + LCD_LAYER_SEQ);
	writel(0x11011111, regs + LCD_LAYER_EN);
	
	writel(0x04380780, regs + LCD_SCREEN_SIZE);
	
	writel(0x100, regs + LCD_AXI);
 
	return 0;
}

static void mapu_fb_write_palette(struct mapu_fb_info *fbi)
{
	unsigned int i;
	void __iomem *regs = fbi->io;

	fbi->palette_ready = 0;

 
}

static irqreturn_t mapu_fb_irq(int irq, void *dev_id)
{
	struct mapu_fb_info *fbi = dev_id;
	void __iomem *regs = fbi->io;
  //  saddr  = info->fix.smem_start ;
   // writel(saddr, regs + LCD_LAY0_BASE); 
	return IRQ_HANDLED;
}

#ifdef CONFIG_CPU_FREQ

static int mapu_fb_cpufreq_transition(struct notifier_block *nb,
					unsigned long val, void *data)
{
	struct cpufreq_freqs *freqs = data;
	struct mapu_fb_info *info;
	struct fb_info *fbinfo;
	long delta_f;

	info = container_of(nb, struct mapu_fb_info, freq_transition);
	fbinfo = platform_get_drvdata(to_platform_device(info->dev));

	/* work out change, <0 for speed-up */
	delta_f = info->clk_rate - clk_get_rate(info->clk);

	if ((val == CPUFREQ_POSTCHANGE && delta_f > 0) ||
	    (val == CPUFREQ_PRECHANGE && delta_f < 0)) {
		info->clk_rate = clk_get_rate(info->clk);
		mapu_fb_activate_var(fbinfo);
	}

	return 0;
}

static inline int mapu_fb_cpufreq_register(struct mapu_fb_info *info)
{
	info->freq_transition.notifier_call = mapu_fb_cpufreq_transition;

	return cpufreq_register_notifier(&info->freq_transition,
					 CPUFREQ_TRANSITION_NOTIFIER);
}

static inline void mapu_fb_cpufreq_deregister(struct mapu_fb_info *info)
{
	cpufreq_unregister_notifier(&info->freq_transition,
				    CPUFREQ_TRANSITION_NOTIFIER);
}

#else
static inline int mapu_fb_cpufreq_register(struct mapu_fb_info *info)
{
	return 0;
}

static inline void mapu_fb_cpufreq_deregister(struct mapu_fb_info *info)
{
}
#endif


static char driver_name[] = "mapu_fb";

static int __init mapu_fb_probe(struct platform_device *pdev)
{
	struct mapu_fb_info *info;
	struct mapu_fb_display *display;
	
	struct fb_info *fbinfo;
	struct mapu_display_info *mach_info;
	struct resource *res;
	int ret;
	int irq;
	int i;
	int size;
	u32 val;

	mach_info = pdev->dev.platform_data;
	if (mach_info == NULL) {
		dev_err(&pdev->dev,
			"no platform data for lcd, cannot attach\n");
		return -EINVAL;
	}


	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "no irq for device\n");
		return -ENOENT;
	}

	fbinfo = framebuffer_alloc(sizeof(struct mapu_fb_info), &pdev->dev);
	if (!fbinfo)
		return -ENOMEM;

	platform_set_drvdata(pdev, fbinfo);

	info = fbinfo->par;
	info->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "failed to get memory registers\n");
		ret = -ENXIO;
		goto dealloc_fb;
	}

	size = (res->end - res->start) + 1;
	info->mem = request_mem_region(res->start, size, pdev->name);
	if (info->mem == NULL) {
		dev_err(&pdev->dev, "failed to get memory region\n");
		ret = -ENOENT;
		goto dealloc_fb;
	}

	info->io = ioremap(res->start, size);
	if (info->io == NULL) {
		dev_err(&pdev->dev, "ioremap() of registers failed\n");
		ret = -ENXIO;
		goto release_mem;
	}

	dprintk("devinit\n");

	strcpy(fbinfo->fix.id, driver_name);

	/* Stop the video */
	val = readl(info->io + LCD_LAYER_EN);
	writel(val&(~ENABLE_DIS), info->io +LCD_LAYER_EN);

	fbinfo->fix.type	    = FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.type_aux	    = 0;
	fbinfo->fix.xpanstep	    = 0;
	fbinfo->fix.ypanstep	    = 0;
	fbinfo->fix.ywrapstep	    = 0;
	fbinfo->fix.accel	    = FB_ACCEL_NONE;

	fbinfo->var.nonstd	    = 0;
	fbinfo->var.activate	    = FB_ACTIVATE_NOW;
	fbinfo->var.accel_flags     = 0;
	fbinfo->var.vmode	    = FB_VMODE_NONINTERLACED;

	fbinfo->fbops		    = &mapu_fb_ops;
	fbinfo->flags		    = FBINFO_FLAG_DEFAULT;
	fbinfo->pseudo_palette      = &info->pseudo_pal;

	//for (i = 0; i < 256; i++)
	//	info->palette_buffer[i] = PALETTE_BUFF_CLEAR;

	ret = request_irq(irq, mapu_fb_irq, IRQF_DISABLED, pdev->name, info);
	if (ret) {
		dev_err(&pdev->dev, "cannot get irq %d - err %d\n", irq, ret);
		ret = -EBUSY;
		goto release_regs;
	}


	/* find maximum required memory size for display */
	 
		unsigned long smem_len = mach_info->xres;

		smem_len *= mach_info->yres;
		smem_len *= mach_info->bpp;
		smem_len >>= 3;
		//if (fbinfo->fix.smem_len < smem_len)
			fbinfo->fix.smem_len = smem_len;
	 

	/* Initialize video memory */
	ret = mapu_fb_map_default_video_memory(fbinfo);
	if (ret) {
		printk(KERN_ERR "Failed to allocate video RAM: %d\n", ret);
		ret = -ENOMEM;
		goto release_clock;
	}

	dprintk("got video memory\n");

	fbinfo->var.xres = mach_info->xres;
	fbinfo->var.yres = mach_info->yres;
	fbinfo->var.bits_per_pixel = mach_info->bpp;

	

	mapu_fb_check_var(&fbinfo->var, fbinfo);

	ret = mapu_fb_cpufreq_register(info);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to register cpufreq\n");
		goto free_video_memory;
	}

	ret = register_framebuffer(fbinfo);
	if (ret < 0) {
		printk(KERN_ERR "Failed to register framebuffer device: %d\n",
			ret);
		goto  free_cpufreq;
	}

	/* create device files */
	ret = device_create_file(&pdev->dev, &dev_attr_debug);
	if (ret) {
		printk(KERN_ERR "failed to add debug attribute\n");
	}
	mapu_draw_logo(fbinfo);
    /* start the video */
	mapu_fb_init_registers(fbinfo);
	//mapu_fb_activate_var(fbinfo);
	val = readl(info->io + LCD_LAYER_EN);
	writel(val|ENABLE_DIS, info->io +LCD_LAYER_EN);
	
	printk(KERN_INFO "fb%d: %s frame buffer device\n",
		fbinfo->node, fbinfo->fix.id);

	return 0;

 free_cpufreq:
	mapu_fb_cpufreq_deregister(info);
free_video_memory:
	mapu_fb_unmap_default_video_memory(fbinfo);
release_clock:
 
release_irq:
	free_irq(irq, info);
release_regs:
	iounmap(info->io);
release_mem:
	release_resource(info->mem);
	kfree(info->mem);
dealloc_fb:
	platform_set_drvdata(pdev, NULL);
	framebuffer_release(fbinfo);
	return ret;
}


/*
 *  Cleanup
 */
static int mapu_fb_remove(struct platform_device *pdev)
{
	struct fb_info *fbinfo = platform_get_drvdata(pdev);
	struct mapu_fb_info *info = fbinfo->par;
	int irq;

	unregister_framebuffer(fbinfo);
	mapu_fb_cpufreq_deregister(info);

	mapu_fb_lcd_enable(info, 0);
	msleep(1);

	mapu_fb_unmap_default_video_memory(fbinfo);

 
	irq = platform_get_irq(pdev, 0);
	free_irq(irq, info);

	iounmap(info->io);

	release_resource(info->mem);
	kfree(info->mem);

	platform_set_drvdata(pdev, NULL);
	framebuffer_release(fbinfo);

	return 0;
}

#ifdef CONFIG_PM

/* suspend and resume support for the lcd controller */
static int mapu_fb_suspend(struct platform_device *dev, pm_message_t state)
{
	struct fb_info	   *fbinfo = platform_get_drvdata(dev);
	return 0;
}

static int mapu_fb_resume(struct platform_device *dev)
{
 struct fb_info	   *fbinfo = platform_get_drvdata(dev);
	return 0;
}

#else
#define mapu_fb_suspend NULL
#define mapu_fb_resume  NULL
#endif

static struct platform_driver mapu_fb_driver = {
	.probe		= mapu_fb_probe,
	.remove		= mapu_fb_remove,
	.suspend	= mapu_fb_suspend,
	.resume		= mapu_fb_resume,
	.driver		= {
		.name	= "mapu-lcd",
		.owner	= THIS_MODULE,
	},
};


int __init mapu_fb_init(void)
{
	int ret = platform_driver_register(&mapu_fb_driver);
	return ret;
}

static void __exit mapu_fb_cleanup(void)
{
	platform_driver_unregister(&mapu_fb_driver);	 
}

module_init(mapu_fb_init);
module_exit(mapu_fb_cleanup);

MODULE_AUTHOR("liu dan dan  ");
MODULE_DESCRIPTION("Framebuffer driver for the mapu");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mapu-lcd");
 
