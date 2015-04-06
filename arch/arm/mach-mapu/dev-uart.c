/*
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
 *
 * Base  UART resource and device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#ifdef CONFIG_SERIAL_8250
#include <linux/serial_8250.h>
#endif

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/irqs.h>
#include <mach/map.h>
#include <mach/map-va.h>
#ifdef CONFIG_SERIAL_MAPU
#include <mach/regs-serial.h>
#include <mach/devs.h>
#endif

#ifdef CONFIG_SERIAL_8250

#define UART_CLK 20000000

static struct plat_serial8250_port serial_platform_data[] = {
	{
		.membase   = MAPU_VA_UART0,
		.mapbase   = MAPU_PA_UART0,
		.irq       = IRQ_UART0,
		.uartclk   = UART_CLK,
		.regshift  = 2,
		.iotype    = UPIO_MEM32,
		.type      = PORT_16550,
		.flags     = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_FIXED_TYPE,
	},
	{
                .membase   = MAPU_VA_UART1,
                .mapbase   = MAPU_PA_UART1,
                .irq       = IRQ_UART1,
                .uartclk   = UART_CLK,
                .regshift  = 2,
                .iotype    = UPIO_MEM32,
                .type      = PORT_16550,
                .flags     = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_FIXED_TYPE,

	},
};

/* uart devices */ 
extern struct platform_device mapu_uart_dev = {
        .name="serial8250",
	.id=PLAT8250_DEV_PLATFORM,
        .dev= {
	      .platform_data=serial_platform_data,
	 },		
};

#endif  //end of CONFIG_SERIAL_8250

/***************************************************/

#ifdef CONFIG_SERIAL_MAPU

/* Following are default values for FCR, MCR and LCR UART registers */
#define MAPU_FCR_DEFAULT	 0		/* Clear & enable FIFOs */

#define MAPU_MCR_DEFAULT	0     /* NO RTS/DTR*/

#define MAPU_LCR_DEFAULT	UART_LCR_8N1		/* 8 data, 1 stop, no parity */

#define MAPU_IER_DEFAULT	UART_IER_THRI |UART_IER_RDI  /*Enable Transmitter holding register and receiver data int*/
static struct mapu_uartcfg mapu_uartcfgs[]   = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.fcr		= MAPU_FCR_DEFAULT,
		.lcr		= MAPU_LCR_DEFAULT,
		.mcr		= MAPU_MCR_DEFAULT,
		.ier        = MAPU_IER_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.fcr		= MAPU_FCR_DEFAULT,
		.lcr		= MAPU_LCR_DEFAULT,
		.mcr		= MAPU_MCR_DEFAULT,
		.ier        = MAPU_IER_DEFAULT, 
	},
	 
};
 /* Serial port registrations */

static struct resource uart0_resource[] = {
	[0] ={
	      .start=MAPU_PA_UART0,
		  .end=MAPU_PA_UART0+UART_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=UART0_RX,
		 .end=UART0_RX+3,
		 .flags=IORESOURCE_IRQ,
	}
};

static struct resource uart1_resource[] = {
	[0] = {
	      .start=MAPU_PA_UART1,
		  .end=MAPU_PA_UART1+UART_SIZE-1,
		  .flags=IORESOURCE_MEM,
	},
	[1] ={
	     .start=UART1_RX,
		 .end=UART1_RX+3,
		 .flags=IORESOURCE_IRQ,
	}
};

/* uart devices */ 
static struct platform_device mapu_uart_dev[] = {
	[0]={
        .name="mapu-uart",
		.id=0,
        .resource=uart0_resource,
        .num_resources=ARRAY_SIZE(uart0_resource),
        .dev= {
		      .platform_data=&mapu_uartcfgs[0],
		}		
	},
	[1]={
        .name="mapu-uart",
		.id=1,
        .resource=uart1_resource,
        .num_resources=ARRAY_SIZE(uart1_resource),
        .dev= {
		      .platform_data=&mapu_uartcfgs[1],
		}		
	},
};

struct platform_device *mapu_uart_devs[CONFIG_SERIAL_MAPU_UARTS]={
		[0]=&mapu_uart_dev[0],
		[1]=&mapu_uart_dev[1],
};

#endif //end of CONFIG_SERIAL_MAPU


