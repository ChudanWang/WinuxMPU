/* linux/drivers/serial/mapu.c
 *
 * Driver core for MAPU SoC onboard UARTs.
 *
 * Ben Dooks, Copyright (c) 2003-2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* Hote on 2410 error handling
 *
 * The mapu manual has a love/hate affair with the contents of the
 * UERSTAT register in the UART blocks, and keeps marking some of the
 * error bits as reserved. Having checked with the mapux01,
 * it copes with BREAKs properly, so I am happy to ignore the RESERVED
 * feature from the latter versions of the manual.
 *
 * If it becomes aparrent that latter versions of the 2410 remove these
 * bits, then action will have to be taken to differentiate the versions
 * and change the policy on BREAK
 *
 * BJD, 04-Nov-2004
*/

#if defined(CONFIG_SERIAL_MAPU_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/sysrq.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/map.h>
#include <mach/devs.h>
#include <mach/regs-serial.h>
#include <mach/map-va.h>
#include <mach/ns16550.h>
#include "mapu.h"
/* UART name and device definitions */

#define MAPU_SERIAL_NAME	"ttySAC"
#define MAPU_SERIAL_MAJOR	204
#define MAPU_SERIAL_MINOR	64

/* macros to change one thing to another */

#define tx_enabled(port) ((port)->unused[0])
#define rx_enabled(port) ((port)->unused[1])

/* flag to ignore all characters comming in */
#define RXSTAT_DUMMY_READ (0x10000000)

static int mapu_serial_setsource(struct uart_port *port,
				     struct mapu_uart_clksrc *clk)
{
 return 0;
}	

static int mapu_serial_getsource(struct uart_port *port,
				    struct mapu_uart_clksrc *clk)
{
 return 0;
}	

static int mapu_serial_rstport(struct uart_port *port,
				    struct mapu_uartcfg *cfg)
{
  dbg("mapu_serial_resetport \n");
  wr_regl(port, MAPU_LCR, cfg->lcr);
  wr_regl(port, MAPU_FCR, cfg->fcr);
  wr_regl(port, MAPU_MCR, cfg->mcr);
 return 0;
}					
static struct mapu_uart_info mapu_uart_inf_0 = {
	.name		= "MAPU SOC UART",
	.type		= PORT_16550,
	.fifosize	= 16,
	.rx_fifomask	= 0,
	.rx_fifoshift	= 0,
	.rx_fifofull	= 0,
	.tx_fifofull	= 0,
	.tx_fifomask	= 0,
	.tx_fifoshift	= 0,
	.get_clksrc	= mapu_serial_getsource,
	.set_clksrc	= mapu_serial_setsource,
	.reset_port	= mapu_serial_rstport,
};

static struct mapu_uart_info mapu_uart_inf_1 = {
	.name		= "MAPU SOC UART",
	.type		= PORT_16550,
	.fifosize	= 16,
	.rx_fifomask	= 0,
	.rx_fifoshift	= 0,
	.rx_fifofull	= 0,
	.tx_fifofull	= 0,
	.tx_fifomask	= 0,
	.tx_fifoshift	= 0,
	.get_clksrc	= mapu_serial_getsource,
	.set_clksrc	= mapu_serial_setsource,
	.reset_port	= mapu_serial_rstport,
};

static struct mapu_uart_info *mapu_uart_inf[]={
[0]=&mapu_uart_inf_0,
[1]=&mapu_uart_inf_1,
};

static inline struct mapu_uart_port *to_ourport(struct uart_port *port)
{
	return container_of(port, struct mapu_uart_port, port);
}

/* translate a port to the device name */

static inline const char *mapu_serial_portname(struct uart_port *port)
{
	return to_platform_device(port->dev)->name;
}

static int mapu_serial_txempty_nofifo(struct uart_port *port)
{
	return (rd_regl(port,MAPU_LSR) & UART_LSR_THRE); // if empty ,return 1
}

static void mapu_serial_rx_enable(struct uart_port *port)
{
	unsigned long flags;
	unsigned int uier, ufcon;
	int count = 10000;

	spin_lock_irqsave(&port->lock, flags);
	while (--count && !mapu_serial_txempty_nofifo(port))
		udelay(100);
    uier = rd_regl(port, MAPU_IER);
	uier |= UART_IER_RDI;
	wr_regl(port, MAPU_IER,uier);
	rx_enabled(port) = 1;
	spin_unlock_irqrestore(&port->lock, flags);
}

static void mapu_serial_rx_disable(struct uart_port *port)
{
	unsigned long flags;
	unsigned int uier;

	spin_lock_irqsave(&port->lock, flags);

	uier = rd_regl(port, MAPU_IER);
	uier &= ~UART_IER_RDI;
	wr_regl(port, MAPU_IER,uier);

	rx_enabled(port) = 0;
	spin_unlock_irqrestore(&port->lock, flags);
}

static void mapu_serial_stop_tx(struct uart_port *port)
{
	struct mapu_uart_port *ourport = to_ourport(port);
    dbg("\n mapu_serial_stop_tx \n");
	if (tx_enabled(port)) {
		disable_irq_nosync(ourport->tx_irq);
		tx_enabled(port) = 0;
		if (port->flags & UPF_CONS_FLOW)
			mapu_serial_rx_enable(port);
	}
}

static void mapu_serial_start_tx(struct uart_port *port)
{
	struct mapu_uart_port *ourport = to_ourport(port);
    dbg("\n mapu_serial_start_tx \n");
	unsigned int uier,uiir,irq_inten;
	 if (!tx_enabled(port)) {
	 	//if (port->flags & UPF_CONS_FLOW)
	 	//	mapu_serial_rx_disable(port);
     
		  enable_irq(ourport->tx_irq);
          tx_enabled(port) = 1;
	 }
	/*uier=rd_regl(port, MAPU_IER);
	uiir=rd_regl(port, MAPU_IIR);
	irq_inten=__raw_readl(MAPU_VA_IRQ);
	dbg("uier %08x, uiir %08x, irq_inten%08x \n",uier,uiir,irq_inten);*/
}


static void mapu_serial_stop_rx(struct uart_port *port)
{
	struct mapu_uart_port *ourport = to_ourport(port);
    dbg("mapu_serial_stop_rx: port=%p\n", port);
	if (rx_enabled(port)) {
		
		disable_irq_nosync(ourport->rx_irq);
		rx_enabled(port) = 0;
	}
}

static void mapu_serial_enable_ms(struct uart_port *port)
{
}

static inline struct mapu_uart_info *mapu_port_to_info(struct uart_port *port)
{
	return to_ourport(port)->info;
}

static inline struct mapu_uartcfg *mapu_port_to_cfg(struct uart_port *port)
{
	if (port->dev == NULL)
		return NULL;

	return (struct mapu_uartcfg *)port->dev->platform_data;
}

/*static int mapu_serial_rx_fifocnt(struct mapu_uart_port *ourport,
				     unsigned long ufstat)
{
	struct mapu_uart_info *info = ourport->info;

	if (ufstat & info->rx_fifofull)
		return info->fifosize;

	return (ufstat & info->rx_fifomask) >> info->rx_fifoshift;
}*/


 

static irqreturn_t
mapu_serial_rx_chars(int irq, void *dev_id)
{
	struct mapu_uart_port *ourport = dev_id;
	struct uart_port *port = &ourport->port;
	struct tty_struct *tty = port->state->port.tty;
	unsigned int ufcon, ch, flag, ulsr, uerstat;
	int max_count = 64;
    
	while (max_count-- > 0) {
		 
		 if(!((rd_regl(port, MAPU_IIR)&0x07)&UART_IIR_RDI))
		   break;
		ch = rd_regl(port, MAPU_RBR);

		/* insert the character into the buffer */

		flag = TTY_NORMAL;
		port->icount.rx++;

		if (uart_handle_sysrq_char(port, ch))
			goto ignore_char;

		tty_insert_flip_char(tty, ch, flag);

 ignore_char:
		continue;
	}
	tty_flip_buffer_push(tty);

	return IRQ_HANDLED;
}

static irqreturn_t mapu_serial_tx_chars(int irq, void *id)
{   
	struct mapu_uart_port *ourport = id;
	struct uart_port *port = &ourport->port;
	struct circ_buf *xmit = &port->state->xmit;
	int count = 256;
    unsigned int ch;
	unsigned long flags;
	spin_lock_irqsave(&port->lock, flags);
	if (port->x_char) {
	    ch=(unsigned int)port->x_char;
		wr_regl(port, MAPU_THR, ch);
		port->icount.tx++;
		port->x_char = 0;
		goto out;
	}

	/* if there isnt anything more to transmit, or the uart is now
	 * stopped, disable the uart and exit
	*/

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		mapu_serial_stop_tx(port);
		goto out;
	}

	/* try and drain the buffer... */

	while (!uart_circ_empty(xmit) && count-- > 0) {
		/*if (!(rd_regl(port,MAPU_IIR) & UART_IIR_THRI))
			break;*/
        ch=(unsigned int)xmit->buf[xmit->tail];
		NS16550_t port1=(NS16550_t)MAPU_VA_UART;
	    NS16550_putc(port1,ch);
		//wr_regl(port, MAPU_THR, ch);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		mapu_serial_stop_tx(port);

 out:
    spin_unlock_irqrestore(&port->lock, flags);
	return IRQ_HANDLED;
}

static unsigned int mapu_serial_tx_empty(struct uart_port *port)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);
	 
	unsigned long uiir = rd_regl(port, MAPU_IIR);

	if (uiir & UART_IIR_ENFIFO) {
		if ((uiir & UART_IIR_THRI) != 0)
			return 1;

		return 0;
	}

	return mapu_serial_txempty_nofifo(port);
}

/* no modem control lines */
static unsigned int mapu_serial_get_mctrl(struct uart_port *port)
{
	unsigned int umstat = rd_regl(port, MAPU_MCR);

	if (umstat & UART_MSR_CTS)
		return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
	else
		return TIOCM_CAR | TIOCM_DSR;
}

static void mapu_serial_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	/* todo - possibly remove AFC and do manual CTS */
}

static void mapu_serial_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;
	unsigned int ulcr;

	spin_lock_irqsave(&port->lock, flags);

	ulcr = rd_regl(port, MAPU_LCR);

	if (break_state)
		ulcr |= UART_LCR_SBRK;
	else
		ulcr &= ~UART_LCR_SBRK;

	wr_regl(port, MAPU_LCR,ulcr);
	
    if (break_state){
	       udelay(100);
		   ulcr &= ~UART_LCR_SBRK;
		   wr_regl(port,MAPU_LCR,ulcr);
	   }
	spin_unlock_irqrestore(&port->lock, flags);
}

static void mapu_serial_shutdown(struct uart_port *port)
{
	struct mapu_uart_port *ourport = to_ourport(port);

	if (ourport->tx_claimed) {
		free_irq(ourport->tx_irq, ourport);
		tx_enabled(port) = 0;
		ourport->tx_claimed = 0;
	}

	if (ourport->rx_claimed) {
		free_irq(ourport->rx_irq, ourport);
		ourport->rx_claimed = 0;
		rx_enabled(port) = 0;
	}
}


static int mapu_serial_startup(struct uart_port *port)
{
	struct mapu_uart_port *ourport = to_ourport(port);
	int ret=0;
    unsigned int uier;
	uier=rd_regl(port, MAPU_IER);
	dbg("uier here 1 %08x\n",uier);
	dbg("mapu_serial_startup: port=%p (%08lx,%p)\n",
	    port->mapbase, port->membase);

	rx_enabled(port) = 1;
     
	ret = request_irq(ourport->rx_irq, mapu_serial_rx_chars, 0,
			  mapu_serial_portname(port), ourport);
    dbg("request_irq rx_irq ok  %d\n ",ourport->rx_irq);
	uier=rd_regl(port, MAPU_IER);
	dbg("uier here 2 %08x\n",uier);
	if (ret != 0) {
		printk(KERN_ERR "cannot repuest_irq for serial in rx_irq %d\n", ourport->rx_irq);
		return ret;
	}

	ourport->rx_claimed = 1;
     
	dbg("requesting tx irq...\n");

	tx_enabled(port) = 1;

	ret = request_irq(ourport->tx_irq, mapu_serial_tx_chars, 0,
			  mapu_serial_portname(port), ourport);
    dbg("request_irq tx_irq ok  %d\n ",ourport->tx_irq);
	uier=rd_regl(port, MAPU_IER);
	dbg("uier here 3 %08x\n",uier);
	if (ret) {
		printk(KERN_ERR "cannot repuest_irq for serial in tx_irq %d\n", ourport->tx_irq);
		goto err;
	}

	ourport->tx_claimed = 1;

	dbg("mapu_serial_startup ok\n");

	/* the port reset code should have done the correct
	 * register setup for the port controls */

	return ret;

 err:
	mapu_serial_shutdown(port);
	return ret;
}

/* power power management control */

static void mapu_serial_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
	 
}

/* baud rate calculation
 *
 * The UARTs on the S3C2410/S3C2440 can take their clocks from a number
 * of different sources, including the peripheral clock ("pclk") and an
 * external clock ("uclk"). The S3C2440 also adds the core clock ("fclk")
 * with a programmable extra divisor.
 *
 * The following code goes through the clock sources, and calculates the
 * baud clocks (and the resultant actual baud rates) and then tries to
 * pick the closest one and select that.
 *
*/


#define MAX_CLKS (8)

static struct mapu_uart_clksrc tmp_clksrc = {
	.name		= "pclk",
	.min_baud	= 0,
	.max_baud	= 0,
	.divisor	= 1,
};

/*static inline int
mapu_serial_getsource(struct uart_port *port, struct mapu_uart_clksrc *c)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);

	return (info->get_clksrc)(port, c);
}

static inline int
mapu_serial_setsource(struct uart_port *port, struct mapu_uart_clksrc *c)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);

	return (info->set_clksrc)(port, c);
}
*/
#define SERIAL_CLK 20000000
struct baud_calc {
	struct mapu_uart_clksrc	*clksrc;
	unsigned int			 calc;
	unsigned int			 divslot;
	unsigned int			 quot;//buad_Divisor
	struct clk			*src;
};

static unsigned int mapu_serial_calcbaud( unsigned int baud)
{
	 
	unsigned long rate;
    unsigned int divisor;
	
	rate = SERIAL_CLK+(baud * (16/ 2))  ;

	divisor = rate / (baud*16);
	 
	 
	return divisor;
}

/*static unsigned int mapu_serial_getclk(struct uart_port *port,
					  struct mapu_uart_clksrc **clksrc,
					  struct clk **clk,
					  unsigned int baud)
{
	 
	return best->quot;
}
*/
/* udivslot_table[]
 *
 * This table takes the fractional value of the baud divisor and gives
 * the recommended setting for the UDIVSLOT register.
 */
static u16 udivslot_table[16] = {
	[0] = 0x0000,
	[1] = 0x0080,
	[2] = 0x0808,
	[3] = 0x0888,
	[4] = 0x2222,
	[5] = 0x4924,
	[6] = 0x4A52,
	[7] = 0x54AA,
	[8] = 0x5555,
	[9] = 0xD555,
	[10] = 0xD5D5,
	[11] = 0xDDD5,
	[12] = 0xDDDD,
	[13] = 0xDFDD,
	[14] = 0xDFDF,
	[15] = 0xFFDF,
};

static void mapu_serial_set_termios(struct uart_port *port,
				       struct ktermios *termios,
				       struct ktermios *old)
{
	struct mapu_uartcfg *cfg = mapu_port_to_cfg(port);
	struct mapu_uart_port *ourport = to_ourport(port);
	struct mapu_uart_clksrc *clksrc = NULL;
	struct clk *clk = NULL;
	unsigned long flags;
	unsigned int baud, quot;
	unsigned int ulcr;
	unsigned int umcr;
	unsigned int ufcr;
	unsigned int uier;
	unsigned int udivslot = 0;

	/*
	 * We don't support modem control lines.
	 */
	termios->c_cflag &= ~(HUPCL | CMSPAR);
	termios->c_cflag |= CLOCAL;

	/*
	 * Ask the core to calculate the divisor for us.
	 */

	baud = uart_get_baud_rate(port, termios, old, 0, 115200*8);
	quot = mapu_serial_calcbaud(baud);

	switch (termios->c_cflag & CSIZE) {
	case CS5:
		dbg("config: 5bits/char\n");
		ulcr = UART_LCR_WLS_5;
		break;
	case CS6:
		dbg("config: 6bits/char\n");
		ulcr = UART_LCR_WLS_6;
		break;
	case CS7:
		dbg("config: 7bits/char\n");
		ulcr = UART_LCR_WLS_7;
		break;
	case CS8:
	default:
		dbg("config: 8bits/char\n");
		ulcr =UART_LCR_WLS_8;
		break;
	}

	/* preserve original lcon IR settings */
	ulcr |= cfg->lcr  ;

	if (termios->c_cflag & CSTOPB)
		ulcr|= UART_LCR_STB;

	umcr = cfg->mcr;
    ufcr =  cfg->fcr;
	spin_lock_irqsave(&port->lock, flags);

	dbg("setting ulcr to %08x, ufcr to %08x,brddiv to %d \n",
	    ulcr,ufcr, quot );

	wr_regl(port, MAPU_LCR,UART_LCR_DLAB);
	wr_regl(port, MAPU_DLL, quot&0xff);
	wr_regl(port, MAPU_DLH, (quot >>8)&0xff);
	wr_regl(port, MAPU_LCR,ulcr);
	wr_regl(port, MAPU_MCR, umcr);
    wr_regl(port, MAPU_FCR, ufcr);
	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);
    
	spin_unlock_irqrestore(&port->lock, flags);
}


static const char *mapu_serial_type(struct uart_port *port)
{
 
	return port->type==PORT_16550 ? "mapu -uart":NULL;
}

#define MAP_SIZE (0x100)

static void mapu_serial_release_port(struct uart_port *port)
{
	release_mem_region(port->mapbase, MAP_SIZE);
}

static int mapu_serial_request_port(struct uart_port *port)
{
	const char *name = mapu_serial_portname(port);
	return request_mem_region(port->mapbase, MAP_SIZE, name) ? 0 : -EBUSY;
}

static void mapu_serial_config_port(struct uart_port *port, int flags)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);

	if (flags & UART_CONFIG_TYPE &&
	    mapu_serial_request_port(port) == 0)
		port->type = info->type;
}

/*
 * verify the new serial_struct (for TIOCSSERIAL).
 */
static int
mapu_serial_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);

	if (ser->type != PORT_UNKNOWN && ser->type != info->type)
		return -EINVAL;

	return 0;
}


#ifdef CONFIG_SERIAL_MAPU_CONSOLE

static struct console mapu_serial_console;

#define MAPU_SERIAL_CONSOLE &mapu_serial_console
#else
#define MAPU_SERIAL_CONSOLE NULL
#endif

static struct uart_ops mapu_serial_ops = {
	.pm		= mapu_serial_pm,
	.tx_empty	= mapu_serial_tx_empty,
	.get_mctrl	= mapu_serial_get_mctrl,
	.set_mctrl	= mapu_serial_set_mctrl,
	.stop_tx	= mapu_serial_stop_tx,
	.start_tx	= mapu_serial_start_tx,
	.stop_rx	= mapu_serial_stop_rx,
	.enable_ms	= mapu_serial_enable_ms,
	.break_ctl	= mapu_serial_break_ctl,
	.startup	= mapu_serial_startup,
	.shutdown	= mapu_serial_shutdown,
	.set_termios	= mapu_serial_set_termios,
	.type		= mapu_serial_type,
	.release_port	= mapu_serial_release_port,
	.request_port	= mapu_serial_request_port,
	.config_port	= mapu_serial_config_port,
	.verify_port	= mapu_serial_verify_port,
};


static struct uart_driver mapu_uart_drv = {
	.owner		= THIS_MODULE,
	.dev_name	= "mapu_serial",
	.nr		= CONFIG_SERIAL_MAPU_UARTS,
	.cons		= MAPU_SERIAL_CONSOLE,
	.driver_name	= MAPU_SERIAL_NAME,
	.major		= MAPU_SERIAL_MAJOR,
	.minor		= MAPU_SERIAL_MINOR,
};

static struct mapu_uart_port mapu_serial_ports[CONFIG_SERIAL_MAPU_UARTS] = {
	[0] = {
		.port = {
			.lock		= __SPIN_LOCK_UNLOCKED(mapu_serial_ports[0].port.lock),
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART0,
			.uartclk	= 0,
			.fifosize	= 16,
			.ops		= &mapu_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 0,
		}
	},
	[1] = {
		.port = {
			.lock		= __SPIN_LOCK_UNLOCKED(mapu_serial_ports[1].port.lock),
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART1,
			.uartclk	= 0,
			.fifosize	= 16,
			.ops		= &mapu_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 1,
		}
	},
 
};

/* mapu_serial_resetport
 *
 * wrapper to call the specific reset for this port (reset the fifos
 * and the settings)
*/

static inline int mapu_serial_resetport(struct uart_port *port,
					   struct mapu_uartcfg *cfg)
{
	struct mapu_uart_info *info = mapu_port_to_info(port);

	return (info->reset_port)(port, cfg);
}


#ifdef CONFIG_CPU_FREQ

static int mapu_serial_cpufreq_transition(struct notifier_block *nb,
					     unsigned long val, void *data)
{
	struct mapu_uart_port *port;
	struct uart_port *uport;

	port = container_of(nb, struct mapu_uart_port, freq_transition);
	uport = &port->port;

	/* check to see if port is enabled */

	if (port->pm_level != 0)
		return 0;

	/* try and work out if the baudrate is changing, we can detect
	 * a change in rate, but we do not have support for detecting
	 * a disturbance in the clock-rate over the change.
	 */

	if (IS_ERR(port->clk))
		goto exit;

	if (port->baudclk_rate == clk_get_rate(port->clk))
		goto exit;

	if (val == CPUFREQ_PRECHANGE) {
		/* we should really shut the port down whilst the
		 * frequency change is in progress. */

	} else if (val == CPUFREQ_POSTCHANGE) {
		struct ktermios *termios;
		struct tty_struct *tty;

		if (uport->state == NULL)
			goto exit;

		tty = uport->state->port.tty;

		if (tty == NULL)
			goto exit;

		termios = tty->termios;

		if (termios == NULL) {
			printk(KERN_WARNING "%s: no termios?\n", __func__);
			goto exit;
		}

		mapu_serial_set_termios(uport, termios, NULL);
	}

 exit:
	return 0;
}

static inline int mapu_serial_cpufreq_register(struct mapu_uart_port *port)
{
	port->freq_transition.notifier_call = mapu_serial_cpufreq_transition;

	return cpufreq_register_notifier(&port->freq_transition,
					 CPUFREQ_TRANSITION_NOTIFIER);
}

static inline void mapu_serial_cpufreq_deregister(struct mapu_uart_port *port)
{
	cpufreq_unregister_notifier(&port->freq_transition,
				    CPUFREQ_TRANSITION_NOTIFIER);
}

#else
static inline int mapu_serial_cpufreq_register(struct mapu_uart_port *port)
{
	return 0;
}

static inline void mapu_serial_cpufreq_deregister(struct mapu_uart_port *port)
{
}
#endif

/* mapu_serial_init_port
 *
 * initialise a single serial port from the platform device given
 */

static int mapu_serial_init_port(struct mapu_uart_port *ourport,
				    struct mapu_uart_info *info,
				    struct platform_device *platdev)
{
	struct uart_port *port = &ourport->port;
	struct mapu_uartcfg *cfg;
	struct resource *res;
	int ret;

	dbg("mapu_serial_init_port: port=%p, platdev=%p\n", port, platdev);

	if (platdev == NULL)
		return -ENODEV;

	cfg = mapu_dev_to_cfg(&platdev->dev);

	/*if (port->mapbase != 0)
		return 0;*/

	if (cfg->hwport > CONFIG_SERIAL_MAPU_UARTS) {
		printk(KERN_ERR "%s: port %d bigger than %d\n", __func__,
		       cfg->hwport, CONFIG_SERIAL_MAPU_UARTS);
		return -ERANGE;
	}

	/* setup info for port */
	port->dev	= &platdev->dev;
	ourport->info	= info;

	/* copy the info in from provided structure */
	ourport->port.fifosize = info->fifosize;

	dbg("mapu_serial_init_port: %p (hw %d)...\n", port, cfg->hwport);

	port->uartclk = 1;

	if (cfg->uart_flags & UPF_CONS_FLOW) {
		dbg("mapu_serial_init_port: enabling flow control\n");
		port->flags |= UPF_CONS_FLOW;
	}

	/* sort our the physical and virtual addresses for each UART */

	res = platform_get_resource(platdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		printk(KERN_ERR "failed to find memory resource for uart\n");
		return -EINVAL;
	}

	dbg("resource %p (%lx..%lx)\n", res, res->start, res->end);

	port->mapbase = res->start;
	if(!cfg->hwport)
	     port->membase = MAPU_VA_UART0;
	else
		 port->membase = MAPU_VA_UART1;	
	ret = platform_get_irq(platdev, 0);
	if (ret < 0)
		port->irq = 0;
	else {
		port->irq = ret;
		ourport->rx_irq = ret;
		ourport->tx_irq = ret + 1;
	}
	
	/*ret = platform_get_irq(platdev, 1);
	if (ret > 0)
		ourport->tx_irq = ret;*/

	ourport->clk	= SERIAL_CLK;

	dbg("port: map=%08x, mem=%08x, irq=%d (%d,%d), clock=%ld\n",
	    port->mapbase, port->membase, port->irq,
	    ourport->rx_irq, ourport->tx_irq, port->uartclk);

	/* reset the fifos (and setup the uart) */
	//mapu_serial_resetport(port, cfg);
	return 0;
}
/*
static ssize_t mapu_serial_show_clksrc(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct uart_port *port = mapu_dev_to_port(dev);
	struct mapu_uart_port *ourport = to_ourport(port);

	return snprintf(buf, PAGE_SIZE, "* %s\n", ourport->clksrc->name);
}

static DEVICE_ATTR(clock_source, S_IRUGO, mapu_serial_show_clksrc, NULL);
*/
/* Device driver serial port probe */

static int probe_index;

int mapu_serial_probe(struct platform_device *dev)
{
	struct mapu_uart_port *ourport;
	int ret;
    struct mapu_uart_info *info= mapu_uart_inf[probe_index];
	dbg("mapu_serial_probe(%p, %p) %d\n", dev, info, probe_index);

	ourport = &mapu_serial_ports[probe_index];
	probe_index++;

	dbg("%s: initialising port %p...\n", __func__, ourport);

	ret = mapu_serial_init_port(ourport, info, dev);
	if (ret < 0)
		goto probe_err;

	dbg("%s: adding port\n", __func__);
	ret=uart_add_one_port(&mapu_uart_drv, &ourport->port);
	if(ret<0)
	  {
	   printk(KERN_ERR "uart_add_one_port fail \n");
	   goto probe_err;
	  }
	platform_set_drvdata(dev, &ourport->port);

	return 0;

 probe_err:
	return ret;
}

EXPORT_SYMBOL_GPL(mapu_serial_probe);

int  mapu_serial_remove(struct platform_device *dev)
{
	struct uart_port *port = mapu_dev_to_port(&dev->dev);

	if (port) {
		mapu_serial_cpufreq_deregister(to_ourport(port));
		 
		uart_remove_one_port(&mapu_uart_drv, port);
	}

	return 0;
}

EXPORT_SYMBOL_GPL(mapu_serial_remove);

static struct platform_driver mapu_serial_driver={
	.probe= mapu_serial_probe,
	.remove=mapu_serial_remove,
	.driver ={
			.name="mapu-uart",
			.owner=THIS_MODULE,
	},
};
/* UART power management code */

#ifdef CONFIG_PM

static int mapu_serial_suspend(struct platform_device *dev, pm_message_t state)
{
	struct uart_port *port = mapu_dev_to_port(&dev->dev);

	if (port)
		uart_suspend_port(&mapu_uart_drv, port);

	return 0;
}

static int mapu_serial_resume(struct platform_device *dev)
{
	struct uart_port *port = mapu_dev_to_port(&dev->dev);
	struct mapu_uart_port *ourport = to_ourport(port);

	if (port) {
		clk_enable(ourport->clk);
		mapu_serial_resetport(port, mapu_port_to_cfg(port));
		clk_disable(ourport->clk);

		uart_resume_port(&mapu_uart_drv, port);
	}

	return 0;
}
#endif

int mapu_serial_init(struct platform_driver *drv,
			struct mapu_uart_info **info)
{
	dbg("mapu_serial_init(%p,%p)\n", drv, info);

#ifdef CONFIG_PM
	drv->suspend = mapu_serial_suspend;
	drv->resume = mapu_serial_resume;
#endif

	return platform_driver_register(drv);
}

EXPORT_SYMBOL_GPL(mapu_serial_init);

static int __init __mapu_serial_init(void)
{
	return mapu_serial_init(&mapu_serial_driver, mapu_uart_inf);
}

static void __exit __mapu_serial_exit(void)
{
	platform_driver_unregister(&mapu_serial_driver);
}

module_init(__mapu_serial_init);
module_exit(__mapu_serial_exit);
/* module initialisation code */

static int __init mapu_serial_modinit(void)
{
	int ret;

	ret = uart_register_driver(&mapu_uart_drv);
	if (ret < 0) {
		printk(KERN_ERR "failed to register UART driver\n");
		return -1;
	}

	return 0;
}

static void __exit mapu_serial_modexit(void)
{
	uart_unregister_driver(&mapu_uart_drv);
}

module_init(mapu_serial_modinit);
module_exit(mapu_serial_modexit);

/* Console code */

#ifdef CONFIG_SERIAL_MAPU_CONSOLE

static struct uart_port *cons_uart;

 

static void
mapu_serial_console_putchar(struct uart_port *port, int ch)
{
	 NS16550_t port1=(NS16550_t)MAPU_VA_UART;
	 NS16550_putc(port1,ch);
}

static void
mapu_serial_console_write(struct console *co, const char *s,
			     unsigned int count)
{    
	uart_console_write(cons_uart, s, count, mapu_serial_console_putchar);
}

static void __init
mapu_serial_get_options(struct uart_port *port, int *baud,
			   int *parity, int *bits)
{
	 
}

/* mapu_serial_init_ports
 *
 * initialise the serial ports from the machine provided initialisation
 * data.
*/

static int mapu_serial_init_ports(struct mapu_uart_info **info)
{
	struct mapu_uart_port *ptr = mapu_serial_ports;
	struct platform_device **platdev_ptr;
	int i;

	dbg("mapu_serial_init_ports: initialising ports...\n");

	platdev_ptr = mapu_uart_devs;

	for (i = 0; i < CONFIG_SERIAL_MAPU_UARTS; i++, ptr++, platdev_ptr++) {
		mapu_serial_init_port(ptr, info[i], *platdev_ptr);
	}

	return 0;
}

static int __init
mapu_serial_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 9600;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	dbg("mapu_serial_console_setup: co=%p (%d), %s\n",
	    co, co->index, options);

	/* is this a valid port */

	if (co->index == -1 || co->index >= CONFIG_SERIAL_MAPU_UARTS)
		co->index = 0;

	port = &mapu_serial_ports[co->index].port;

	/* is the port configured? */

	if (port->mapbase == 0x0) {
		co->index = 0;
		port = &mapu_serial_ports[co->index].port;
	}

	cons_uart = port;

	dbg("mapu_serial_console_setup: port=%p (%d)\n", port, co->index);

	/*
	 * Check whether an invalid uart number has been specified, and
	 * if so, search for the first available port that does have
	 * console support.
	 */
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	 

	dbg("mapu_serial_console_setup: baud %d\n", baud);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

/* mapu_serial_initconsole
 *
 * initialise the console from one of the uart drivers
*/

static struct console mapu_serial_console = {
	.name		= MAPU_SERIAL_NAME,
	.device		= uart_console_device,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.write		= mapu_serial_console_write,
	.setup		= mapu_serial_console_setup
};

int mapu_serial_initconsole(struct platform_driver *drv,
			       struct mapu_uart_info **info)

{
	struct platform_device *dev = mapu_uart_devs[0];

	dbg("mapu_serial_initconsole\n");

	/* select driver based on the cpu */

	if (dev == NULL) {
		printk(KERN_ERR "mapu: no devices for console init\n");
		return 0;
	}

	if (strcmp(dev->name, drv->driver.name) != 0){
	    printk(KERN_ERR "mapu:device and driver name is differernt\n");
		return 0;
    }
	mapu_serial_console.data = &mapu_uart_drv;
	mapu_serial_init_ports(info);
    
	register_console(&mapu_serial_console);
	return 0;
}

static int __init mapu_serial_console_init(void)
{
  return mapu_serial_initconsole(&mapu_serial_driver, mapu_uart_inf);
}

console_initcall(mapu_serial_console_init);
#endif /* CONFIG_SERIAL_MAPU_CONSOLE */

MODULE_DESCRIPTION("MAPU SoC Serial port driver");
MODULE_AUTHOR("liu dan dan <ben@simtec.co.uk>");
MODULE_LICENSE("GPL v2");
