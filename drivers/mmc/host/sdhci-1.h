/*
 *  linux/drivers/mmc/host/sdhci.h - Secure Digital Host Controller Interface driver
 *
 *  Copyright (C) 2005-2008 Pierre Ossman, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#include <linux/scatterlist.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/io.h>
 
 
struct sdhci_ops;

struct sdhci_host {
	/* Data set by hardware interface driver */
	const char		*hw_name;	/* Hardware bus name */

	unsigned int		quirks;		/* Deviations from spec. */

/* Controller doesn't honor resets unless we touch the clock register */
#define SDHCI_QUIRK_CLOCK_BEFORE_RESET			(1<<0)
/* Controller has bad caps bits, but really supports DMA */
#define SDHCI_QUIRK_FORCE_DMA				(1<<1)
/* Controller doesn't like to be reset when there is no card inserted. */
#define SDHCI_QUIRK_NO_CARD_NO_RESET			(1<<2)
/* Controller doesn't like clearing the power reg before a change */
#define SDHCI_QUIRK_SINGLE_POWER_WRITE			(1<<3)
/* Controller has flaky internal state so reset it on each ios change */
#define SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS		(1<<4)
/* Controller has an unusable DMA engine */
#define SDHCI_QUIRK_BROKEN_DMA				(1<<5)
/* Controller has an unusable ADMA engine */
#define SDHCI_QUIRK_BROKEN_ADMA				(1<<6)
/* Controller can only DMA from 32-bit aligned addresses */
#define SDHCI_QUIRK_32BIT_DMA_ADDR			(1<<7)
/* Controller can only DMA chunk sizes that are a multiple of 32 bits */
#define SDHCI_QUIRK_32BIT_DMA_SIZE			(1<<8)
/* Controller can only ADMA chunks that are a multiple of 32 bits */
#define SDHCI_QUIRK_32BIT_ADMA_SIZE			(1<<9)
/* Controller needs to be reset after each request to stay stable */
#define SDHCI_QUIRK_RESET_AFTER_REQUEST			(1<<10)
/* Controller needs voltage and power writes to happen separately */
#define SDHCI_QUIRK_NO_SIMULT_VDD_AND_POWER		(1<<11)
/* Controller provides an incorrect timeout value for transfers */
#define SDHCI_QUIRK_BROKEN_TIMEOUT_VAL			(1<<12)
/* Controller has an issue with buffer bits for small transfers */
#define SDHCI_QUIRK_BROKEN_SMALL_PIO			(1<<13)
/* Controller does not provide transfer-complete interrupt when not busy */
#define SDHCI_QUIRK_NO_BUSY_IRQ				(1<<14)
/* Controller has unreliable card detection */
#define SDHCI_QUIRK_BROKEN_CARD_DETECTION		(1<<15)
/* Controller reports inverted write-protect state */
#define SDHCI_QUIRK_INVERTED_WRITE_PROTECT		(1<<16)
/* Controller has nonstandard clock management */
#define SDHCI_QUIRK_NONSTANDARD_CLOCK			(1<<17)
/* Controller does not like fast PIO transfers */
#define SDHCI_QUIRK_PIO_NEEDS_DELAY			(1<<18)
/* Controller losing signal/interrupt enable states after reset */
#define SDHCI_QUIRK_RESTORE_IRQS_AFTER_RESET		(1<<19)
/* Controller has to be forced to use block size of 2048 bytes */
#define SDHCI_QUIRK_FORCE_BLK_SZ_2048			(1<<20)
/* Controller cannot do multi-block transfers */
#define SDHCI_QUIRK_NO_MULTIBLOCK			(1<<21)
/* Controller can only handle 1-bit data transfers */
#define SDHCI_QUIRK_FORCE_1_BIT_DATA			(1<<22)
/* Controller needs 10ms delay between applying power and clock */
#define SDHCI_QUIRK_DELAY_AFTER_POWER			(1<<23)
/* Controller uses SDCLK instead of TMCLK for data timeouts */
#define SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK		(1<<24)
/* Controller cannot support End Attribute in NOP ADMA descriptor */
#define SDHCI_QUIRK_NO_ENDATTR_IN_NOPDESC		(1<<25)
/* Controller does not use HISPD bit field in HI-SPEED SD cards */
#define SDHCI_QUIRK_NO_HISPD_BIT			(1<<26)
/* Controller has unreliable card present bit */
#define SDHCI_QUIRK_BROKEN_CARD_PRESENT_BIT		(1<<27)
/* Controller has hardware bugs */
#define SDHCI_QUIRK_INIT_ISSUE_CMD			(1<<29)
	int			irq;		/* Device IRQ */
	void __iomem *		ioaddr;		/* Mapped address */

	const struct sdhci_ops	*ops;		/* Low level hw interface */

	/* Internal data */
	struct mmc_host		*mmc;		/* MMC structure */
	u64			dma_mask;	/* custom DMA mask */

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
	struct led_classdev	led;		/* LED control */
	char   led_name[32];
#endif

	spinlock_t		lock;		/* Mutex */

	int			flags;		/* Host attributes */
#define SDHCI_USE_SDMA		(1<<0)		/* Host is SDMA capable */
#define SDHCI_USE_ADMA		(1<<1)		/* Host is ADMA capable */
#define SDHCI_REQ_USE_DMA	(1<<2)		/* Use DMA for this req. */
#define SDHCI_DEVICE_DEAD	(1<<3)		/* Device unresponsive */

	unsigned int		version;	/* SDHCI spec. version */

	unsigned int		max_clk;	/* Max possible freq (MHz) */
	unsigned int		timeout_clk;	/* Timeout freq (KHz) */

	unsigned int		clock;		/* Current clock (MHz) */
	unsigned int		clock_to_restore;	/* Saved clock for dynamic clock gating (MHz) */
	u8			pwr;		/* Current voltage */

	struct mmc_request	*mrq;		/* Current request */
	struct mmc_command	*cmd;		/* Current command */
	struct mmc_data		*data;		/* Current data request */
	unsigned int		data_early:1;	/* Data finished before cmd */

	struct sg_mapping_iter	sg_miter;	/* SG state for PIO */
	unsigned int		blocks;		/* remaining PIO blocks */

	int			sg_count;	/* Mapped sg entries */

	u32			*adma_desc;	/* ADMA descriptor table */
	u32			*align_buffer;	/* Bounce buffer */

	dma_addr_t		adma_addr;	/* Mapped ADMA descr. table */
	dma_addr_t		align_addr;	/* Mapped bounce buffer */

	struct tasklet_struct	card_tasklet;	/* Tasklet structures */
	struct tasklet_struct	finish_tasklet;

	struct timer_list	timer;		/* Timer for timeouts */
	struct timer_list	clock_timer;	/* Timer for clock gating */

	unsigned long		private[0] ____cacheline_aligned;
};


struct sdhci_ops {
#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS
	u32		(*readl)(struct sdhci_host *host, int reg);
	u16		(*readw)(struct sdhci_host *host, int reg);
	u8		(*readb)(struct sdhci_host *host, int reg);
	void		(*writel)(struct sdhci_host *host, u32 val, int reg);
	void		(*writew)(struct sdhci_host *host, u16 val, int reg);
	void		(*writeb)(struct sdhci_host *host, u8 val, int reg);
#endif

	void	(*set_clock)(struct sdhci_host *host, unsigned int clock);

	int		(*enable_dma)(struct sdhci_host *host);
	unsigned int	(*get_max_clock)(struct sdhci_host *host);
	unsigned int	(*get_min_clock)(struct sdhci_host *host);
	unsigned int	(*get_timeout_clock)(struct sdhci_host *host);
	void            (*set_ios)(struct sdhci_host *host,
				   struct mmc_ios *ios);
	int             (*get_ro) (struct mmc_host *mmc);
	int		(*init_issue_cmd)(struct sdhci_host *host);
};

#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS

static inline void sdhci_writel(struct sdhci_host *host, u32 val, int reg)
{
	if (unlikely(host->ops->writel))
		host->ops->writel(host, val, reg);
	else
		writel(val, host->ioaddr + reg);
}

static inline void sdhci_writew(struct sdhci_host *host, u16 val, int reg)
{
	if (unlikely(host->ops->writew))
		host->ops->writew(host, val, reg);
	else
		writew(val, host->ioaddr + reg);
}

static inline void sdhci_writeb(struct sdhci_host *host, u8 val, int reg)
{
	if (unlikely(host->ops->writeb))
		host->ops->writeb(host, val, reg);
	else
		writeb(val, host->ioaddr + reg);
}

static inline u32 sdhci_readl(struct sdhci_host *host, int reg)
{
	if (unlikely(host->ops->readl))
		return host->ops->readl(host, reg);
	else
		return readl(host->ioaddr + reg);
}

static inline u16 sdhci_readw(struct sdhci_host *host, int reg)
{
	if (unlikely(host->ops->readw))
		return host->ops->readw(host, reg);
	else
		return readw(host->ioaddr + reg);
}

static inline u8 sdhci_readb(struct sdhci_host *host, int reg)
{
	if (unlikely(host->ops->readb))
		return host->ops->readb(host, reg);
	else
		return readb(host->ioaddr + reg);
}

#else

static inline void sdhci_writel(struct sdhci_host *host, u32 val, int reg)
{
	writel(val, host->ioaddr + reg);
}

static inline void sdhci_writew(struct sdhci_host *host, u16 val, int reg)
{
	writew(val, host->ioaddr + reg);
}

static inline void sdhci_writeb(struct sdhci_host *host, u8 val, int reg)
{
	writeb(val, host->ioaddr + reg);
}

static inline u32 sdhci_readl(struct sdhci_host *host, int reg)
{
	return readl(host->ioaddr + reg);
}

static inline u16 sdhci_readw(struct sdhci_host *host, int reg)
{
	return readw(host->ioaddr + reg);
}

static inline u8 sdhci_readb(struct sdhci_host *host, int reg)
{
	return readb(host->ioaddr + reg);
}

#endif /* CONFIG_MMC_SDHCI_IO_ACCESSORS */

extern struct sdhci_host *sdhci_alloc_host(struct device *dev,
	size_t priv_size);
extern void sdhci_free_host(struct sdhci_host *host);

static inline void *sdhci_priv(struct sdhci_host *host)
{
	return (void *)host->private;
}

extern int sdhci_add_host(struct sdhci_host *host);
extern void sdhci_remove_host(struct sdhci_host *host, int dead);

#ifdef CONFIG_PM
extern int sdhci_suspend_host(struct sdhci_host *host, pm_message_t state);
extern int sdhci_resume_host(struct sdhci_host *host);
#endif
