/* linux/drivers/mmc/host/sdhci-mapu.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <linux/mmc/host.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/mmc.h>
#include <mach/regs-sdhci.h>

#include "sdhci-1.h"

#define MAX_BUS_CLK	(4)
 
/**
 * struct sdhci_mapu - mapu SDHCI instance
 * @host: The SDHCI host created
 * @pdev: The platform device we where created from.
 * @ioarea: The resource created when we claimed the IO area.
 * @pdata: The platform data for this controller.
 * @cur_clk: The index of the current bus clock.
 * @clk_io: The clock for the internal bus interface.
 * @clk_bus: The clocks that are available for the SD/MMC bus clock.
 */
struct sdhci_mapu {
	struct sdhci_host	*host;
	struct platform_device	*pdev;
	struct resource		*ioarea;
	struct mapu_sdhci_platdata *pdata;
	unsigned int		cur_clk;

	struct clk		*clk_io;
	struct clk		*clk_bus[MAX_BUS_CLK];
};

static inline struct sdhci_mapu *to_mapu(struct sdhci_host *host)
{
	return sdhci_priv(host);
}

/**
 * sdhci_mapu_get_max_clk - callback to get maximum clock frequency.
 * @host: The SDHCI host instance.
 *
 * Callback to return the maximum clock rate acheivable by the controller.
*/
static unsigned int sdhci_mapu_get_max_clk(struct sdhci_host *host)
{
	unsigned int max=52000000; 

	return max;
}

static unsigned int sdhci_mapu_get_timeout_clk(struct sdhci_host *host)
{
	return sdhci_mapu_get_max_clk(host) / 1000000;
}

static void sdhci_mapu_set_ios(struct sdhci_host *host,struct mmc_ios *ios)
{
	 
}

 
/**
 * sdhci_mapu_set_clock - callback on clock change
 * @host: The SDHCI host being changed
 * @clock: The clock rate being requested.
 *
 * When the card's clock is going to be changed, look at the new frequency
 * and find the best clock source to go with it.
*/
static void sdhci_mapu_set_clock(struct sdhci_host *host, unsigned int clock)
{
	  
}

static int sdhci_mapu_get_ro(struct mmc_host *mmc)
{
	struct sdhci_host *host;
	struct sdhci_mapu *sc;

	host = mmc_priv(mmc);
	sc = sdhci_priv(host);

	if(sc->pdata->get_ro)
		return sc->pdata->get_ro(mmc);

	return 0;
}

/*
 * This function is to avoid abnormal command complete on issuing command.
 * Sometimes abnormal command would be occurred because of H/W glitch.
 */
static int sdhci_mapu_init_issue_cmd(struct sdhci_host *host)
{
	struct sdhci_mapu *ourhost = to_mapu(host);
	uint timeout;
	if(host->cmd->opcode==MMC_APP_CMD){
	host->cmd->error=0;
	return 2;
	}
	if((host->cmd->opcode==SD_APP_SET_BUS_WIDTH)||(host->cmd->opcode==SD_APP_SEND_NUM_WR_BLKS)||
	(host->cmd->opcode==SD_APP_OP_COND)||(host->cmd->opcode==SD_APP_SEND_SCR)){
	 return 1;
		}
	return 0;	
}

static struct sdhci_ops sdhci_mapu_ops = {
	.get_max_clock		= sdhci_mapu_get_max_clk,
	.get_timeout_clock	= sdhci_mapu_get_timeout_clk,
	.set_clock		= sdhci_mapu_set_clock,
	.set_ios		= sdhci_mapu_set_ios,
	.init_issue_cmd		= sdhci_mapu_init_issue_cmd,
};

irqreturn_t sdhci_irq_cd(int irq, void *dev_id)
{
	struct sdhci_mapu *sc = dev_id;
	tasklet_schedule(&sc->host->card_tasklet);

	return IRQ_HANDLED;
}

static int   sdhci_mapu_probe(struct platform_device *pdev)
{
	struct mapu_sdhci_platdata *pdata = pdev->dev.platform_data;
	struct device *dev = &pdev->dev;
	struct sdhci_host *host;
	struct sdhci_mapu *sc;
	struct resource *res;
	int ret, irq, ptr, clks;

	if (!pdata) {
		dev_err(dev, "no device data specified\n");
		return -ENOENT;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(dev, "no irq specified\n");
		return irq;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "no memory specified\n");
		return -ENOENT;
	}

	host = sdhci_alloc_host(dev, sizeof(struct sdhci_mapu));
	if (IS_ERR(host)) {
		dev_err(dev, "sdhci_alloc_host() failed\n");
		return PTR_ERR(host);
	}

	sc = sdhci_priv(host);

	sc->host = host;
	sc->pdev = pdev;
	sc->pdata = pdata;

	platform_set_drvdata(pdev, host);

	sc->ioarea = request_mem_region(res->start, resource_size(res),mmc_hostname(host->mmc));
	if (!sc->ioarea) {
		dev_err(dev, "failed to reserve register area\n");
		ret = -ENXIO;
		goto err_add_host;
	}  

	host->ioaddr = ioremap_nocache(res->start, resource_size(res));
	//printk("host->ioaddr is %p \n",host->ioaddr);
	if (!host->ioaddr) {
		dev_err(dev, "failed to map registers\n");
		ret = -ENXIO;
		goto err_add_host;
	}

	/* Ensure we have minimal gpio selected CMD/CLK/Detect */
	if (pdata->cfg_gpio)
		pdata->cfg_gpio(pdev, pdata->max_width);

	if (pdata->get_ro)
		sdhci_mapu_ops.get_ro = sdhci_mapu_get_ro;

	host->hw_name = "mapu-hsmmc";
	host->ops = &sdhci_mapu_ops;
	host->quirks = 0;
	host->irq = irq;

	/* Setup quirks for the controller */
	 
	host->quirks |= SDHCI_QUIRK_BROKEN_CARD_PRESENT_BIT;
	 
	/* It seems we do not get an DATA transfer complete on non-busy
	 * transfers, not sure if this is a problem with this specific
	 * SDHCI block, or a missing configuration that needs to be set. */
	host->quirks |= SDHCI_QUIRK_NO_BUSY_IRQ;

	host->quirks |= (SDHCI_QUIRK_32BIT_DMA_ADDR | SDHCI_QUIRK_32BIT_DMA_SIZE);

	host->quirks |= SDHCI_QUIRK_INIT_ISSUE_CMD;

	if (pdata->host_caps)
		host->mmc->caps = pdata->host_caps;
	else
		host->mmc->caps = 0;

	/* to add external irq as a card detect signal */
	if (pdata->cfg_ext_cd)
		pdata->cfg_ext_cd();

	/* to configure gpio pin as a card write protection signal */
	if (pdata->cfg_wp)
		pdata->cfg_wp();

	ret = sdhci_add_host(host);
	
	if (ret) {
		dev_err(dev, "sdhci_add_host() failed\n");
		goto err_add_host;
	}

	/* register external irq here (after all init is done) */
	if (pdata->cfg_ext_cd) {
		ret = request_irq(pdata->ext_cd, sdhci_irq_cd,
				IRQF_SHARED, mmc_hostname(host->mmc), sc);
	}

	return 0;

 err_add_host:
	release_resource(sc->ioarea);
	kfree(sc->ioarea); 
 err_io_clk:
	sdhci_free_host(host);

	return ret;
}

static int  sdhci_mapu_remove(struct platform_device *pdev)
{
	return 0;
}

#ifdef CONFIG_PM
static int sdhci_mapu_suspend(struct platform_device *dev, pm_message_t pm)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	sdhci_suspend_host(host, pm);
	return 0;
}

static int sdhci_mapu_resume(struct platform_device *dev)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	sdhci_resume_host(host);
	return 0;
}
#else
#define sdhci_mapu_suspend NULL
#define sdhci_mapu_resume NULL
#endif

static struct platform_driver sdhci_mapu_driver = {
	.probe		= sdhci_mapu_probe,
	.remove		=  sdhci_mapu_remove,
	.suspend	= sdhci_mapu_suspend,
	.resume	        = sdhci_mapu_resume,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "mapu-sdhci",
	},
};

static int __init sdhci_mapu_init(void)
{
	return platform_driver_register(&sdhci_mapu_driver);
}

static void __exit sdhci_mapu_exit(void)
{
	platform_driver_unregister(&sdhci_mapu_driver);
}

module_init(sdhci_mapu_init);
module_exit(sdhci_mapu_exit);

MODULE_DESCRIPTION("MAPU SDHCI (HSMMC) glue");
MODULE_AUTHOR("Ben Dooks, <ben@simtec.co.uk>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:mapu-sdhci");
