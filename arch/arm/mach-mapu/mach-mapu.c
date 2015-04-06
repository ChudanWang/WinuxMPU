/* linux/arch/arm/mach-mapu/mach-mapu.c
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *	liu dandan <948144038@qq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/pwm_backlight.h>
#include <linux/amba/bus.h>

#include <asm/hardware/vic.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/mach-types.h> 
 
#include <mach/cpu.h>
#include <mach/map.h>
#include <mach/map-va.h>
#include <mach/devs.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <mach/media.h>

/*initial IO mappings*/
static struct map_desc mapu_iodesc[] __initdata = {

	{
		.virtual	= (u32)MAPU_VA_IRQ,
		.pfn		= __phys_to_pfn(MAPU_PA_INTC),
		.length		=SZ_256K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)MAPU_VA_SYS,
		.pfn		= __phys_to_pfn(MAPU_PA_SYSCON),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)MAPU_VA_GPIO,
		.pfn		= __phys_to_pfn(MAPU_PA_GPIO),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	},{
		.virtual	= (u32)MAPU_VA_TIMER,
		.pfn		= __phys_to_pfn(MAPU_PA_TIMER),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	},  {
		.virtual	= (u32)MAPU_VA_WATCHDOG,
		.pfn		= __phys_to_pfn(MAPU_PA_WATCHDOG),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= (u32)MAPU_VA_LCD,
		.pfn		= __phys_to_pfn(MAPU_PA_LCD),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	},{
		.virtual	= (u32)MAPU_VA_GMAC,
		.pfn		= __phys_to_pfn(MAPU_PA_GMAC),
		.length		= SZ_256K,
		.type		= MT_DEVICE,
	},{
		.virtual	= (u32)MAPU_VA_UART0,
		.pfn		= __phys_to_pfn(MAPU_PA_UART0),
		.length		= SZ_128K,
		.type		= MT_DEVICE,
	},{
                .virtual        = (u32)MAPU_VA_UART1,
                .pfn            = __phys_to_pfn(MAPU_PA_UART1),
                .length         = SZ_128K,
                .type           = MT_DEVICE,
	},
};

/*CPU information*/
static const char name_mapu[] = "MAPU SOC";

static struct cpu_table cpu_ids[] __initdata = {
	{
		.map_io		= NULL,
		.init		= NULL,
		.name		= name_mapu,
	},
};

/* --------------------------------------------------------------------
 *  GPIO
 * -------------------------------------------------------------------- */

static struct mapu_gpio_bank mapu_gpio[] = {
	{ /* GPIO A*/
		.id		= 0,
		.offset		= GPIOA_DR,
		.clock		= NULL,
	}, { /* GPIO B */
		.id		= 0,
		.offset		= GPIOB_DR,
		.clock		= NULL,
	}, { /* GPIO C */
		.id		= 0,
		.offset		= GPIOC_DR,
		.clock		= NULL,
	}, { /* GPIO D */
                .id             = 0,
                .offset         = GPIOD_DR,
                .clock          = NULL,
        }
};

#ifdef CONFIG_SPI_MAPU

/*
 * VS10XX SPI Devices (Audio)
 */

struct vs10xx_board_info {
	int device_id;
	int gpio_reset;
	int gpio_dreq;
};

struct vs10xx_board_info vs10xx_device_0 = {
	.device_id = 0, /* =/dev/vs10xx-0 */
	.gpio_reset = MAPU_PIN_PA3,
	.gpio_dreq  = MAPU_PIN_PA2,
};

static struct spi_board_info ek_spi_devices[] = {
	{	/* device 0 on spi bus 0, cs 0 */
		.modalias	= "vs10xx-ctrl",
		.bus_num	= 0,
		.chip_select	= 0,
		.platform_data	= &vs10xx_device_0,
		.max_speed_hz	= 1 * 1000 * 1000,
	},
	{	/* device 0 on SPI bus 0, cs 1 */
		.modalias	= "vs10xx-data",
		.bus_num	= 0,
		.chip_select	= 1,
		.platform_data	= &vs10xx_device_0,
		.max_speed_hz	= 12 * 1000 * 1000,
	
	},
};
#endif /* CONFIG_SPI_MAPU */


static void __init mapu_map_io(void)
{
	iotable_init(mapu_iodesc, ARRAY_SIZE(mapu_iodesc));//remap  iotable in static 
	mapu_init_cpu(cpu_ids, ARRAY_SIZE(cpu_ids));
	mapu_reserve_bootmem();//reserve room for some device needs continuous physics address	
	mapu_gpio_init(mapu_gpio, 4);
}
 
static void __init mapu_machine_init(void)
{ 
  int ret;

#ifdef  CONFIG_SERIAL_MAPU
  ret = platform_add_devices(mapu_uart_devs,ARRAY_SIZE(mapu_uart_devs));	
  if(ret)
	    printk(KERN_ERR "register uart device fail\n");   
  else
        printk(KERN_NOTICE "register uart device success\n");
#endif

#ifdef  CONFIG_SERIAL_8250
  ret=platform_device_register(&mapu_uart_dev);
  if(ret)
  {
        /* printk(KERN_ERR"register uart device fail\n"); */
	printk(KERN_ERR "register uart device fail\n");
        platform_device_unregister(&mapu_uart_dev);
  }
  else {
        printk(KERN_NOTICE "register uart device success\n");
  }

#endif

#ifdef	CONFIG_FB_MAPU	
  ret=platform_device_register(&mapu_lcd_dev);
  if(ret)
		{
		printk(KERN_ERR"register lcd device fail\n");
		platform_device_unregister(&mapu_lcd_dev);
		}
  else
        printk("register lcd device success\n");
#endif

#ifdef  CONFIG_MMC_SDHCI_MAPU
  ret=platform_device_register(&mapu_sdhci_dev);
  if(ret)
		{
		printk(KERN_ERR"register sdhci device fail\n");
		platform_device_unregister(&mapu_sdhci_dev);
		}
  else
        printk("register sdhci device success\n");	
#endif
		
#ifdef CONFIG_SPI_MAPU
	mapu_add_device_spi(ek_spi_devices, ARRAY_SIZE(ek_spi_devices));
#endif

#ifdef CONFIG_GPU_DEVICE
  ret=platform_device_register(&mapu_gpu_dev);
  if(ret)
        {
            printk("register gpu device fail\n");
            platform_device_unregister(&mapu_gpu_dev);
        }
  else
        printk("register gpu device success\n");
#endif		

#ifdef CONFIG_DW_DMAC
ret=platform_device_register(&mapu_dmac_dev);
  if(ret)
		{
		printk("register dmac device fail\n");
		platform_device_unregister(&mapu_dmac_dev);
		}
  else
        printk("register dmac device success\n");
#endif		
}
 

MACHINE_START(MAPU, "mapu")
	/* Maintainer: liudandan <948144038@qq.com> */
	.phys_io	= MAPU_PA_UART0 & 0xfff00000,
	.io_pg_offst	= (((u32)MAPU_VA_UART) >> 18) & 0xfffc,
	.boot_params	=0x80000100,
	.init_irq	= mapu_init_irq,
	.map_io		= mapu_map_io,
	.init_machine	= mapu_machine_init,
	.timer		= &mapu_timer,
MACHINE_END
