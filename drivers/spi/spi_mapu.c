/*
 * Driver for MaPU SPI Controllers
 *
 * Copyright (C) 2015 IACAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>

#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/cpu.h>
#include <mach/regs-spi.h>

/*
 * The core SPI transfer engine just talks to a register bank to set up
 * DMA transfers; transfer queue progress is driven by IRQs.  The clock
 * framework provides the base clock, subdivided for each spi_device.
 */
struct mapu_spi {
	spinlock_t		lock;

	void __iomem		*regs;
	int			irq;
	struct clk		*clk;
	struct platform_device	*pdev;
	struct spi_device	*stay;

	u8			stopping;
	struct list_head	queue;
	struct spi_transfer	*current_transfer;
	unsigned long		current_remaining_bytes;
	struct spi_transfer	*next_transfer;
	unsigned long		next_remaining_bytes;

	void			*buffer;
};

/* Controller-specific per-slave state */
struct mapu_spi_device {
	unsigned int		npcs_pin;
	u32			baudr;
};

#define BUFFER_SIZE		PAGE_SIZE
#define INVALID_DMA_ADDRESS	0xffffffff

static void cs_activate(struct mapu_spi *ms, struct spi_device *spi)
{
	struct mapu_spi_device *msd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;

	gpio_set_value(msd->npcs_pin, active);

	dev_dbg(&spi->dev, "activate %u%s baudr=%x\n",
			msd->npcs_pin, active ? " (high)" : "", msd->baudr);

        __raw_writel(0x0, ms->regs + MAPU_SPI_SSIENR);
        __raw_writel(msd->baudr, ms->regs + MAPU_SPI_BAUDR);
        __raw_writel(0x1, ms->regs + MAPU_SPI_SSIENR);
}

static void cs_deactivate(struct mapu_spi *as, struct spi_device *spi)
{
	struct mapu_spi_device *msd = spi->controller_state;
	unsigned active = spi->mode & SPI_CS_HIGH;

	dev_dbg(&spi->dev, "DEactivate %u%s\n",
			msd->npcs_pin, active ? " (low)" : "");

	gpio_set_value(msd->npcs_pin, !active);

}

static inline int mapu_spi_xfer_is_last(struct spi_message *msg,
					struct spi_transfer *xfer)
{
	return msg->transfers.prev == &xfer->transfer_list;
}

static inline int mapu_spi_xfer_can_be_chained(struct spi_transfer *xfer)
{
	return xfer->delay_usecs == 0 && !xfer->cs_change;
}

unsigned char spi_readwrite(void __iomem *regbase, char *buf, int flag)
{
	int i=0;
	u32 data=0, sr=0;

	data = *buf;
	sr = __raw_readl(regbase + MAPU_SPI_SR);

	while( SPI_TX_FIFO_EMPTY != ( __raw_readl(regbase+MAPU_SPI_SR) & SPI_TX_FIFO_EMPTY) ) ;

	__raw_writel(data, regbase + MAPU_SPI_DR);

	for (i=0; i<3; i++) ;
	
	sr = __raw_readl(regbase + MAPU_SPI_SR);

	while(( __raw_readl(regbase+MAPU_SPI_SR) & SPI_RX_FIFO_NOT_EMPTY) != SPI_RX_FIFO_NOT_EMPTY);
	data = __raw_readl(regbase + MAPU_SPI_DR);

	return data;
}

static void mapu_spi_xfer(struct spi_master *master,
				struct spi_message *msg)
{
	struct mapu_spi	*ms = spi_master_get_devdata(master);
	struct spi_transfer	*xfer;
	u32			len=0, loop=0, remaining=0;
	int			i=0, j=0;
	u32			ieval;
	char *			buf=NULL;

	if (!ms->current_transfer)
		xfer = list_entry(msg->transfers.next,
				struct spi_transfer, transfer_list);
	else if (!ms->next_transfer)
		xfer = list_entry(ms->current_transfer->transfer_list.next,
				struct spi_transfer, transfer_list);
	else
		xfer = NULL;

	if (NULL == xfer)
		return;

	if (xfer) {
		len = xfer->len;
                if (msg->spi->bits_per_word > 8)
                        len >>= 1;

		/* 32 bytes once transfer */
		loop = len >> 6;
		remaining = len - (loop << 6);

		dev_dbg(&msg->spi->dev,
			"  start xfer %p: len %u tx %p rx %p\n",
			xfer, xfer->len, xfer->tx_buf,
			xfer->rx_buf);
	} else {
		xfer = ms->next_transfer;
		remaining = ms->next_remaining_bytes;
	}

	ms->current_transfer = xfer;
	ms->current_remaining_bytes = remaining;

	if (NULL != xfer->tx_buf) {
		buf = xfer->tx_buf;
		for (i=0; i<loop; i++) {
			for (j=0; j<32; j++)
				spi_readwrite(ms->regs, buf+j, 0);
			buf += 32;
		}
		for (j=0; j<remaining; j++)
			spi_readwrite(ms->regs, buf+j, 0);
	}

	if (NULL != xfer->rx_buf) {
		buf = xfer->rx_buf;
		for (i=0; i<loop; i++) {
			for (j=0; j<32; j++)
				*(buf+j) = spi_readwrite(ms->regs, buf+j, 1);
			buf += 32;
		}
		for (j=0; j<remaining; j++)
			*(buf+j) = spi_readwrite(ms->regs, buf+j, 1);
	}

	ms->current_remaining_bytes = 0;
}



/*
 * lock is held, spi irq is blocked
 */
static void mapu_spi_next_xfer(struct spi_master *master,
				struct spi_message *msg)
{
	struct mapu_spi	*ms = spi_master_get_devdata(master);
	struct spi_transfer	*xfer;
	u32			len, remaining;
	u32			ieval;

	if (!ms->current_transfer)
		xfer = list_entry(msg->transfers.next,
				struct spi_transfer, transfer_list);
	else if (!ms->next_transfer)
		xfer = list_entry(ms->current_transfer->transfer_list.next,
				struct spi_transfer, transfer_list);
	else
		xfer = NULL;

	if (xfer) {
		len = xfer->len;
		/* 32 data once transfer */
		len = 32;
		remaining = xfer->len - len;

		if (msg->spi->bits_per_word > 8)
			len >>= 1;

		dev_dbg(&msg->spi->dev,
			"  start xfer %p: len %u tx %p rx %p\n",
			xfer, xfer->len, xfer->tx_buf,
			xfer->rx_buf);
	} else {
		xfer = ms->next_transfer;
		remaining = ms->next_remaining_bytes;
	}

	ms->current_transfer = xfer;
	ms->current_remaining_bytes = remaining;

	if (remaining > 0)
		len = remaining;
	else if (!mapu_spi_xfer_is_last(msg, xfer)
			&& mapu_spi_xfer_can_be_chained(xfer)) {
		xfer = list_entry(xfer->transfer_list.next,
				struct spi_transfer, transfer_list);
		len = xfer->len;
	} else
		xfer = NULL;

	ms->next_transfer = xfer;

	if (xfer) {
		u32	total;

		total = len;
		len = 32;
		ms->next_remaining_bytes = total - len;

		if (msg->spi->bits_per_word > 8)
			len >>= 1;

		dev_dbg(&msg->spi->dev,
			"  next xfer %p: len %u tx %p rx %p\n",
			xfer, xfer->len, xfer->tx_buf,
			xfer->rx_buf);
	} else {
	}

	/*
	spi_writel(as, IER, ieval);
	spi_writel(as, PTCR, SPI_BIT(TXTEN) | SPI_BIT(RXTEN));
	*/
}

static void mapu_spi_next_message(struct spi_master *master)
{
	struct mapu_spi	*ms = spi_master_get_devdata(master);
	struct spi_message	*msg;
	struct spi_device	*spi;
	struct spi_transfer	*xfer;

	BUG_ON(ms->current_transfer);

	msg = list_entry(ms->queue.next, struct spi_message, queue);
	spi = msg->spi;


	dev_dbg(master->dev.parent, "start message %p for %s\n",
			msg, dev_name(&spi->dev));

	/* select chip if it's not still active */
	if (ms->stay) {
		if (ms->stay != spi) {
			cs_deactivate(ms, ms->stay);
			cs_activate(ms, spi);
		}
		ms->stay = NULL;
	} else
		cs_activate(ms, spi);

	while(1) {
		xfer = ms->current_transfer;
		if (xfer != NULL && mapu_spi_xfer_is_last(msg, xfer)) {
			break;
		} else {
			if (xfer !=NULL && xfer->cs_change) {
				cs_deactivate(ms, msg->spi);
				udelay(1);
				cs_activate(ms, msg->spi);
			}

			mapu_spi_xfer(master, msg);
		}
	}

}

static void
mapu_spi_msg_done(struct spi_master *master, struct mapu_spi *ms,
		struct spi_message *msg, int status, int stay)
{
	if (!stay || status < 0)
		cs_deactivate(ms, msg->spi);
	else
		ms->stay = msg->spi;

	list_del(&msg->queue);
	msg->status = status;

	dev_dbg(master->dev.parent,
		"xfer complete: %u bytes transferred\n",
		msg->actual_length);

	spin_unlock(&ms->lock);
	msg->complete(msg->context);
	spin_lock(&ms->lock);

	ms->current_transfer = NULL;
	ms->next_transfer = NULL;

	/* continue if needed */
	if (list_empty(&ms->queue) || ms->stopping)
		; /*spi_writel(as, PTCR, SPI_BIT(RXTDIS) | SPI_BIT(TXTDIS)); */
	else
		mapu_spi_next_message(master);
}

static irqreturn_t
mapu_spi_interrupt(int irq, void *dev_id)
{
	struct spi_master	*master = dev_id;
	struct mapu_spi		*ms = spi_master_get_devdata(master);
	struct spi_message	*msg;
	struct spi_transfer	*xfer;
	u32			status, pending, imr;
	int			ret = IRQ_NONE;

	spin_lock(&ms->lock);
	/*
	xfer = ms->current_transfer;
	msg = list_entry(ms->queue.next, struct spi_message, queue);
	*/

	imr = __raw_readl(ms->regs + MAPU_SPI_IMR);
	status = __raw_readl(ms->regs + MAPU_SPI_ISR);
	pending = status & imr;

	/*
	if (pending & 1) {
		int timeout;

		ret = IRQ_HANDLED;

		if (xfer->delay_usecs)
			udelay(xfer->delay_usecs);
		dev_warn(master->dev.parent, "overrun (%u/%u remaining)\n",
			 spi_readl(as, TCR), spi_readl(as, RCR));

		
		for (timeout = 1000; timeout; timeout--)
			if (spi_readl(as, SR) & SPI_BIT(TXEMPTY))
				break;
		if (!timeout)
			dev_warn(master->dev.parent,
				 "timeout waiting for TXEMPTY");
		while (spi_readl(ms, SR) & SPI_BIT(RDRF))
			spi_readl(ms, RDR);

		spi_readl(ms, SR);

		mapu_spi_msg_done(master, ms, msg, -EIO, 0);
	} else if (pending & 0x10) {
		ret = IRQ_HANDLED;

		if (ms->current_remaining_bytes == 0) {
			msg->actual_length += xfer->len;

			if (xfer->delay_usecs)
				udelay(xfer->delay_usecs);

			if (mapu_spi_xfer_is_last(msg, xfer)) {
				mapu_spi_msg_done(master, ms, msg, 0,
						xfer->cs_change);
			} else {
				if (xfer->cs_change) {
					cs_deactivate(ms, msg->spi);
					udelay(1);
					cs_activate(ms, msg->spi);
				}

				mapu_spi_next_xfer(master, msg);
			}
		} else {
			mapu_spi_next_xfer(master, msg);
		}
	}
	*/

	spin_unlock(&ms->lock);

	return ret;
}

static int mapu_spi_setup(struct spi_device *spi)
{
	struct mapu_spi	*ms;
	struct mapu_spi_device	*msd;
	u32			baudr;
	unsigned int		bits = spi->bits_per_word;
	unsigned long		bus_hz;
	unsigned int		npcs_pin;
	int			ret;

	ms = spi_master_get_devdata(spi->master);

	if (ms->stopping)
		return -ESHUTDOWN;

	if (spi->chip_select > spi->master->num_chipselect) {
		dev_dbg(&spi->dev,
				"setup: invalid chipselect %u (%u defined)\n",
				spi->chip_select, spi->master->num_chipselect);
		return -EINVAL;
	}

	if (bits < 8 || bits > 16) {
		dev_dbg(&spi->dev,
				"setup: invalid bits_per_word %u (8 to 16)\n",
				bits);
		return -EINVAL;
	}

	/* spi start out at half the bus speed. */
	bus_hz = MAPU_SPI_HZ;

	if (spi->max_speed_hz) {
		/*
		 * Calculate the lowest divider that satisfies the
		 * constraint, assuming div32/fdiv/mbz == 0.
		 */
		baudr = bus_hz / spi->max_speed_hz;

	} else
		/* speed zero means "as slow as possible" */
		baudr = 0x0;

	/* chipselect must have been muxed as GPIO (e.g. in board setup) */
	npcs_pin = (unsigned int)spi->controller_data;
	msd = spi->controller_state;
	if (!msd) {
		msd = kzalloc(sizeof(struct mapu_spi_device), GFP_KERNEL);
		if (!msd)
			return -ENOMEM;

		ret = gpio_request(npcs_pin, dev_name(&spi->dev));
		if (ret) {
			kfree(msd);
			return ret;
		}

		msd->npcs_pin = npcs_pin;
		spi->controller_state = msd;
		gpio_direction_output(npcs_pin, !(spi->mode & SPI_CS_HIGH));
	} else {
		unsigned long		flags;

		spin_lock_irqsave(&ms->lock, flags);
		if (ms->stay == spi)
			ms->stay = NULL;
		cs_deactivate(ms, spi);
		spin_unlock_irqrestore(&ms->lock, flags);
	}

	msd->baudr = baudr;
/*	
	__raw_writel(0x0, ms->regs + MAPU_SPI_SSIENR);
	__raw_writel(baudr, ms->regs + MAPU_SPI_BAUDR);
	__raw_writel(0x1, ms->regs + MAPU_SPI_SSIENR);
*/	

	printk("nisp: mapu_spi_setup, baudr=%d\n", baudr);
	dev_dbg(&spi->dev,
		"setup: %lu Hz bpw %u mode 0x%x -> csr%d %08x\n",
		bus_hz / baudr, bits, spi->mode, spi->chip_select, baudr);

	return 0;
}

static int mapu_spi_transfer(struct spi_device *spi, struct spi_message *msg)
{
	struct mapu_spi		*ms;
	struct spi_transfer	*xfer;
	unsigned long		flags;
	struct device		*controller = spi->master->dev.parent;

	ms = spi_master_get_devdata(spi->master);

	dev_dbg(controller, "new message %p submitted for %s\n",
			msg, dev_name(&spi->dev));

	if (unlikely(list_empty(&msg->transfers)))
		return -EINVAL;

	if (ms->stopping)
		return -ESHUTDOWN;

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (!(xfer->tx_buf || xfer->rx_buf) && xfer->len) {
			dev_dbg(&spi->dev, "missing rx or tx buf\n");
			return -EINVAL;
		}

		/* FIXME implement these protocol options!! */
		if (xfer->bits_per_word || xfer->speed_hz) {
			dev_dbg(&spi->dev, "no protocol options yet\n");
			return -ENOPROTOOPT;
		}

	}

#ifdef VERBOSE
	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		dev_dbg(controller,
			"  xfer %p: len %u tx %p rx %p\n",
			xfer, xfer->len,
			xfer->tx_buf,
			xfer->rx_buf);
	}
#endif

	msg->status = -EINPROGRESS;
	msg->actual_length = 0;

	spin_lock_irqsave(&ms->lock, flags);
	list_add_tail(&msg->queue, &ms->queue);
	if (!ms->current_transfer)
		mapu_spi_next_message(spi->master);
	spin_unlock_irqrestore(&ms->lock, flags);

	spin_lock(&ms->lock);
	if (ms->current_remaining_bytes == 0) {
		msg->actual_length = ms->current_transfer->len;
	
		if (xfer->delay_usecs)
			udelay(xfer->delay_usecs);

		mapu_spi_msg_done(spi->master, ms, msg, 0, xfer->cs_change);
	}

	spin_unlock(&ms->lock);
	return 0;
}

static void mapu_spi_cleanup(struct spi_device *spi)
{
	struct mapu_spi	*ms = spi_master_get_devdata(spi->master);
	struct mapu_spi_device	*msd = spi->controller_state;
	unsigned		gpio = (unsigned) spi->controller_data;
	unsigned long		flags;

	if (!msd)
		return;

	spin_lock_irqsave(&ms->lock, flags);
	if (ms->stay == spi) {
		ms->stay = NULL;
		cs_deactivate(ms, spi);
	}
	spin_unlock_irqrestore(&ms->lock, flags);

	spi->controller_state = NULL;
	gpio_free(gpio);
	kfree(msd);
}

void mapu_spi_hw_init(void __iomem *regbase)
{
	__raw_writel(0x0, regbase + MAPU_SPI_SSIENR);
	/*
	 * TMOD:Transmit&Receive
	 * SCPOL=0,上升沿触发
	 * SCPH=0,在时钟信号的一个跳变沿捕获数据
	 * FRF=0,SPI模式
	 */
	__raw_writel(0xC7, regbase + MAPU_SPI_CTRLR0);

	/*
	 * DFS=0xf ,数据帧长度为16bits
	 */
	/* 波特率设置,Fssi_clk=50MHZ, Fclk_out = Fssi_clk/2*/
	__raw_writel(0x32, regbase + MAPU_SPI_BAUDR);

	/* 设置Tx FIFO和Rx FIFO的阈值 */
	//TXFTLR = 0;
	//RXFTLR = 0;
	/* 掩蔽所有中断 */
	__raw_writel(0x0, regbase + MAPU_SPI_IMR); 

	/*清寄存器状态*/
	__raw_writel(0x0, regbase + MAPU_SPI_SR);

	/* 选中从机设备 */
	__raw_writel(0x1, regbase + MAPU_SPI_SER);

	/* 使能DW_apb_ssi */
	__raw_writel(0x1, regbase + MAPU_SPI_SSIENR);
}

/*-------------------------------------------------------------------------*/

static int __init mapu_spi_probe(struct platform_device *pdev)
{
	struct resource		*regs;
	int			irq;
	struct clk		*clk;
	int			ret;
	struct spi_master	*master;
	struct mapu_spi		*ms;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	/* setup spi core then atmel-specific driver state */
	ret = -ENOMEM;
	master = spi_alloc_master(&pdev->dev, sizeof *ms);
	if (!master)
		goto out_free;

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;

	master->bus_num = pdev->id;
	master->num_chipselect = 2;
	master->setup = mapu_spi_setup;
	master->transfer = mapu_spi_transfer;
	master->cleanup = mapu_spi_cleanup;
	platform_set_drvdata(pdev, master);

	ms = spi_master_get_devdata(master);

	/*
	 * Scratch buffer is used for throwaway rx and tx data.
	 * It's coherent to minimize dcache pollution.
	 */
	/*ms->buffer = dma_alloc_coherent(&pdev->dev, BUFFER_SIZE,
					&ms->buffer_dma, GFP_KERNEL);
	if (!ms->buffer)
		goto out_free;
	*/

	spin_lock_init(&ms->lock);
	INIT_LIST_HEAD(&ms->queue);
	ms->pdev = pdev;
	ms->regs = ioremap(regs->start, (regs->end - regs->start) + 1);
	if (!ms->regs)
		goto out_free_buffer;
	ms->irq = irq;

        /* Initialize the hardware */
        mapu_spi_hw_init(ms->regs);

	ret = request_irq(irq, mapu_spi_interrupt, 0,
			dev_name(&pdev->dev), master);
	if (ret)
		goto out_unmap_regs;

	/* Initialize the hardware */
	/* mapu_spi_hw_init(ms->regs); */

	/* go! */
	dev_info(&pdev->dev, "MaPU SPI Controller at 0x%08lx (irq %d)\n",
			(unsigned long)regs->start, irq);

	ret = spi_register_master(master);
	if (ret)
		goto out_reset_hw;

	return 0;

out_reset_hw:
	__raw_writel(0x0, MAPU_SPI_SSIENR);
	free_irq(irq, master);
out_unmap_regs:
	iounmap(ms->regs);
out_free_buffer:
/*
	dma_free_coherent(&pdev->dev, BUFFER_SIZE, ms->buffer,
			ms->buffer_dma);
*/
out_free:
	spi_master_put(master);
	return ret;
}

static int __exit mapu_spi_remove(struct platform_device *pdev)
{
	struct spi_master	*master = platform_get_drvdata(pdev);
	struct mapu_spi		*ms = spi_master_get_devdata(master);
	struct spi_message	*msg;
	
	/* reset the hardware and block queue progress */
	spin_lock_irq(&ms->lock);
	ms->stopping = 1;
	/* spi software reset */
	/* spi_writel(ms, CR, SPI_BIT(SWRST));
	spi_writel(ms, CR, SPI_BIT(SWRST));
	spi_readl(ms, SR); */
	spin_unlock_irq(&ms->lock);

	/* Terminate remaining queued transfers */
	list_for_each_entry(msg, &ms->queue, queue) {
		/* REVISIT unmapping the dma is a NOP on ARM and AVR32
		 * but we shouldn't depend on that...
		 */
		msg->status = -ESHUTDOWN;
		msg->complete(msg->context);
	}

/*
	dma_free_coherent(&pdev->dev, BUFFER_SIZE, ms->buffer,
			ms->buffer_dma);
*/

	free_irq(ms->irq, master);
	iounmap(ms->regs);

	spi_unregister_master(master);

	return 0;
}

static struct platform_driver mapu_spi_driver = {
	.driver		= {
		.name	= "mapu-spi",
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(mapu_spi_remove),
};

static int __init mapu_spi_init(void)
{
	return platform_driver_probe(&mapu_spi_driver, mapu_spi_probe);
}
module_init(mapu_spi_init);

static void __exit mapu_spi_exit(void)
{
	platform_driver_unregister(&mapu_spi_driver);
}
module_exit(mapu_spi_exit);

MODULE_DESCRIPTION("MaPU SPI Controller driver");
MODULE_AUTHOR("NISP");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:spi_mapu");
