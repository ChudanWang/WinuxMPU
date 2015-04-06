/* arch/arm/mach-mapu/include/mach/regs-serial.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __ASM_ARM_REGS_UART_H
#define __ASM_ARM_REGS_UART_H

#define MAPU_RBR  (0x00)
#define MAPU_THR  (0x00)
#define MAPU_IER  (0x04)
#define MAPU_FCR  (0x08)
#define MAPU_IIR  (0x08)
#define MAPU_LCR  (0x0c)
#define MAPU_MCR  (0x10)
#define MAPU_LSR  (0x14)
#define MAPU_MSR  (0x18)
#define MAPU_FAR  (0x70)
#define MAPU_TFR  (0x74)
#define MAPU_RFW  (0x78)
#define MAPU_DLL  (0x00)
#define MAPU_DLH  (0x04)

 /*
 * These are the definitions for the FIFO Control Register
 */
#define UART_FCR_FIFO_EN	0x01 /* Fifo enable */
#define UART_FCR_CLEAR_RCVR	0x02 /* Clear the RCVR FIFO */
#define UART_FCR_CLEAR_XMIT	0x04 /* Clear the XMIT FIFO */
#define UART_FCR_DMA_SELECT	0x08 /* For DMA applications */
#define UART_FCR_TRIGGER_MASK	0xC0 /* Mask for the FIFO trigger range */
#define UART_FCR_TRIGGER_1	0x00 /* Mask for trigger set at 1 */
#define UART_FCR_TRIGGER_4	0x40 /* Mask for trigger set at 4 */
#define UART_FCR_TRIGGER_8	0x80 /* Mask for trigger set at 8 */
#define UART_FCR_TRIGGER_14	0xC0 /* Mask for trigger set at 14 */
#define UART_FCR_TX_EMPTY_TRIGGER_MASK 0x30 /* TX Empty Trigger */
#define UART_FCR_TX_EMPTY_TRIGGER_EMPTY	0x00 /* Mask for trigger FIFO empty */
#define UART_FCR_TX_EMPTY_TRIGGER_TWOCHAR	0x10 /* Mask for trigger 2 char in FIFO */
#define UART_FCR_TX_EMPTY_TRIGGER_QUARTER	0x20 /* Mask for trigger FIFO 1/4 full */
#define UART_FCR_TX_EMPTY_TRIGGER_HALFFULL	0x30 /* Mask for trigger set at 14 */
#define UART_FCR_RX_TRIGGER_MASK	0xC0 /* Mask for the FIFO trigger range */
#define UART_FCR_RX_TRIGGER_ONECHAR	0x00 /* Mask for trigger set at 1 */
#define UART_FCR_RX_TRIGGER_QUARTERFULL	0x40 /* Mask for trigger set at 4 */
#define UART_FCR_RX_TRIGGER_HALFFULL	0x80 /* Mask for trigger set at 8 */
#define UART_FCR_RX_TRIGGER_ALMOSTFULL	0xC0 /* Mask for trigger set at 14 */

#define UART_FCR_RXSR		0x02 /* Receiver soft reset */
#define UART_FCR_TXSR		0x04 /* Transmitter soft reset */

/*
 * These are the definitions for the Modem Control Register
 */
#define UART_MCR_DTR	0x01		/* DTR   */
#define UART_MCR_RTS	0x02		/* RTS   */
#define UART_MCR_OUT1	0x04		/* Out 1 */
#define UART_MCR_OUT2	0x08		/* Out 2 */
#define UART_MCR_LOOP	0x10		/* Enable loopback test mode */

#define UART_MCR_DMA_EN	0x04
#define UART_MCR_TX_DFR	0x08

/*
 * These are the definitions for the Line Control Register
 *
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 */
#define UART_LCR_WLS_MSK 0x03		/* character length select mask */
#define UART_LCR_WLS_5	0x00		/* 5 bit character length */
#define UART_LCR_WLS_6	0x01		/* 6 bit character length */
#define UART_LCR_WLS_7	0x02		/* 7 bit character length */
#define UART_LCR_WLS_8	0x03		/* 8 bit character length */
#define UART_LCR_STB	0x04		/* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN	0x08		/* Parity eneble */
#define UART_LCR_EPS	0x10		/* Even Parity Select */
#define UART_LCR_STKP	0x20		/* Stick Parity */
#define UART_LCR_SBRK	0x40		/* Set Break */
#define UART_LCR_BKSE	0x80		/* Bank select enable */
#define UART_LCR_DLAB	0x80		/* Divisor latch access bit */

/*
 * These are the definitions for the Line Status Register
 */
#define UART_LSR_DR	0x01		/* Data ready */
#define UART_LSR_OE	0x02		/* Overrun */
#define UART_LSR_PE	0x04		/* Parity error */
#define UART_LSR_FE	0x08		/* Framing error */
#define UART_LSR_BI	0x10		/* Break */
#define UART_LSR_THRE	0x20		/* Xmit holding register empty */
#define UART_LSR_TEMT	0x40		/* Xmitter empty */
#define UART_LSR_ERR	0x80		/* Error */

#define UART_MSR_DCD	0x80		/* Data Carrier Detect */
#define UART_MSR_RI	0x40		/* Ring Indicator */
#define UART_MSR_DSR	0x20		/* Data Set Ready */
#define UART_MSR_CTS	0x10		/* Clear to Send */
#define UART_MSR_DDCD	0x08		/* Delta DCD */
#define UART_MSR_TERI	0x04		/* Trailing edge ring indicator */
#define UART_MSR_DDSR	0x02		/* Delta DSR */
#define UART_MSR_DCTS	0x01		/* Delta CTS */

/*
 * These are the definitions for the Interrupt Identification Register
 */
#define UART_IIR_NO_INT	0x01	/* No interrupts pending */
#define UART_IIR_ID	0x06	/* Mask for the interrupt ID */

#define UART_IIR_MSI	0x00	/* Modem status interrupt */
#define UART_IIR_THRI	0x02	/* Transmitter holding register empty */
#define UART_IIR_RDI	0x04	/* Receiver data interrupt */
#define UART_IIR_RLSI	0x06	/* Receiver line status interrupt */
#define UART_IIR_CTOUT	0x0c    /* character timeout */
#define UART_IIR_ENFIFO	0xC0	/*  enable fifo */
/*
 * These are the definitions for the Interrupt Enable Register
 */
#define UART_IER_MSI	0x08	/* Enable Modem status interrupt */
#define UART_IER_RLSI	0x04	/* Enable receiver line status interrupt */
#define UART_IER_THRI	0x02	/* Enable Transmitter holding register int. */
#define UART_IER_RDI	0x01	/* Enable receiver data interrupt */

/* useful defaults for LCR */
#define UART_LCR_8N1	0x03

 
struct mapu_uartcfg {
	unsigned char	   hwport;	 /* hardware port number */
	unsigned char	   unused;
	unsigned short	   flags;
	unsigned int	   uart_flags;	 /* default uart flags */
	unsigned int	   clk_sel;

	unsigned int	   has_fracval;

	unsigned long	   fcr;	 /* value of fifo control register for port */
	unsigned long	   lcr;	 /* value of line control register for port */
	unsigned long	   mcr;	 /* value of modem control register for port */
	unsigned long	   ier;	 /* value of Interrupt Enable Register register for port */
};

#endif /* __ASM_ARM_REGS_SERIAL_H */

