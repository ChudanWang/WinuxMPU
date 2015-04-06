/*
 *  linux/drivers/mmc/host/sdhci-1.c  
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 */

#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>

#include <linux/leds.h>

#include <linux/mmc/host.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/mmc.h>

#include <mach/regs-sdhci.h>
#include "sdhci-1.h"

#define DRIVER_NAME "sdhci"

#ifdef  CONFIG_MAPU_SD_DEBUG
#define DBG(f, x...)  printk(DRIVER_NAME " [%s()]: " f, __func__,## x)
#else	    
#define DBG(f, x...)
#endif 
 
#define SDHCI_CLK_ON 1
#define SDHCI_CLK_OFF 0

 
static unsigned int debug_quirks = 0;

static void sdhci_prepare_data(struct sdhci_host *, struct mmc_data *);
static void sdhci_finish_data(struct sdhci_host *);

static void sdhci_send_command(struct sdhci_host *, struct mmc_command *);
static void sdhci_finish_command(struct sdhci_host *);
static void sdhci_set_clock(struct sdhci_host *host, unsigned int clock);

 
static void sdhci_dumpregs(struct sdhci_host *host)
{ /*
	printk( DRIVER_NAME ": ============== REGISTER DUMP ==============\n");
	printk( DRIVER_NAME ":  Blk cnt:  0x%08x\n",
		sdhci_readl(host, SDHCI_BLOCK_COUNT));
	printk( DRIVER_NAME ": Argument: 0x%08x  ",
		sdhci_readl(host, SDHCI_ARGUMENT));
	printk( DRIVER_NAME ": Present:  0x%08x  ",
		sdhci_readl(host, SDHCI_PRESENT_STATE));
	printk( DRIVER_NAME ":  Int stat: 0x%08x\n",
		sdhci_readl(host, SDHCI_INT_STATUS));
	printk( DRIVER_NAME ": ===========================================\n");
	*/
}

static void sdhci_reset(struct sdhci_host *host)
{	
	int flag=0;
	unsigned int timeout;
	if (host->quirks & SDHCI_QUIRK_NO_CARD_NO_RESET) {
		if (!(sdhci_readl(host, SDHCI_PRESENT_STATE) &
			SDHCI_CARD_PRESENT))
			return;
	}
	/* Send CMD0 to Reset the card and host */
while(1){ 
	sdhci_writel(host, 0x0, SDHCI_ARGUMENT); 
	sdhci_writel(host, 0x0, SDHCI_COMMAND);
	DBG(" CMD0 send complete...\n");
	mdelay(100); 
	DBG(" CMD0 Resp0 = 0x%08x\n",sdhci_readl(host, SDHCI_RESPONSE));
	if(!sdhci_readl(host, SDHCI_RESPONSE))
		flag=1; 
	// Reset the host 
	sdhci_writel(host, SDHCI_SOFTWARE_RESET, SDHCI_AHB_HPROT);
	DBG(" Hprot reg: Reset SD/MMC Host...\n");
	mdelay(100);
	if(flag)
	   break;
}
	host->clock = 0;

	timeout = 100;
	while (sdhci_readl(host, SDHCI_AHB_HPROT)& SDHCI_SOFTWARE_RESET) {
		if (timeout == 0) {
			printk("%s: timeout error\n", __func__);
			return;
		}
		timeout--;
		udelay(1000);
	}
	 
	sdhci_writel(host, 0x0, SDHCI_INT_STATUS); 
}

static void sdhci_init(struct sdhci_host *host)
{
	sdhci_reset(host);

}

/*****************************************************************************\
 *                                                                           *
 * Core functions                                                            *
 *                                                                           *
\*****************************************************************************/
 
static void sdhci_set_adma_desc(u32 desc,u32 direction ,u32 blocks ,u32 sd_data_addr,u32 ram_data_addr, u32 flag ,u32 next)
{
   *((u32 *)desc)=(direction<<31)+(flag<<30)+blocks;
   *((u32 *)desc+0x01)=sd_data_addr;
   *((u32 *)desc+0x02)=ram_data_addr;
   *((u32 *)desc+0x03)= flag ? next:0x0;
}

static void sdhci_bread(struct sdhci_host *host,u32 desc,u32 direction ,u32 blocks ,
							u32 sd_data_addr,u32 ram_data_addr, u32 flag ,u32 next)
{
	
	sdhci_set_adma_desc(desc,direction,blocks,sd_data_addr,ram_data_addr,flag,next);
 
}							
static int sdhci_adma_table_pre(struct sdhci_host *host,
	struct mmc_data *data)
{	static m;
	m++;
    DBG(" %d \n",m);
	int direction;
	u32 direction_1;

	u32 *desc;
 
	dma_addr_t addr;
 
	int len, offset;
    u32 start=host->cmd->arg;
 
	int retry=0x15;
	struct scatterlist *sg;
	int i;
	char *buffer;
	unsigned long flags;

	/*
	 * The spec does not specify endianness of descriptor table.
	 * We currently guess that it is LE.
	 */

	if (data->flags & MMC_DATA_READ)
		{
		direction = DMA_FROM_DEVICE;
		direction_1=1;
		}
	else
		{
		direction = DMA_TO_DEVICE;
		direction_1=0;
		}

	/*
	 * The ADMA descriptor table is mapped further down as we
	 * need to fill it with data first.
	 */

	host->sg_count = dma_map_sg(mmc_dev(host->mmc),data->sg, data->sg_len, direction);
	if (host->sg_count == 0)
		goto fail;
 
	desc = host->adma_desc;
		
	for_each_sg(data->sg, sg, host->sg_count, i) {
		addr = sg_dma_address(sg);
		len = sg_dma_len(sg);
		DBG("i=0x%08x,sg_len=0x%08x\n",i,len);
		 
		sdhci_bread(host,(u32)desc,direction_1,(len>>9),start,addr,0x0,0x0);
		start=start+(len>>9);
		desc += 4;
 
		WARN_ON((desc - host->adma_desc) > 16);
	}
	 
	host->adma_addr = dma_map_single(mmc_dev(host->mmc),
		host->adma_desc, 16, DMA_TO_DEVICE);	
	if (dma_mapping_error(mmc_dev(host->mmc), host->adma_addr))
		goto fail;
	
	sdhci_writel(host, host->adma_addr,SDHCI_ADMA_ADDRESS);
	for(i=0;i<retry;i++){ 
		if(sdhci_readl(host, SDHCI_PRESENT_STATE)&(1<<1))
			DBG("ADMA BUSY \n");
		else {
		if(direction)
			DBG("Read by ADMA success\n");
		else
			DBG("Write by ADMA success\n");
			break ;
			}
		}	 
	return 0;
fail:
	printk("has dma map error \n");
	return -EINVAL;
}

static void sdhci_adma_table_post(struct sdhci_host *host,
	struct mmc_data *data)
{
	int direction;

	if (data->flags & MMC_DATA_READ)
		direction = DMA_FROM_DEVICE;
	else
		direction = DMA_TO_DEVICE;
	mdelay(20);
	dma_unmap_single(mmc_dev(host->mmc), host->adma_addr,
		16, DMA_TO_DEVICE);
 
	dma_unmap_sg(mmc_dev(host->mmc), data->sg,
		data->sg_len, direction);
}

 

static void sdhci_prepare_data(struct sdhci_host *host, struct mmc_data *data)
{  static m;
	m++;
    DBG(" %d \n",m);
	u8 count;
	u8 ctrl;
	int ret;

	WARN_ON(host->data);

	if (data == NULL)
		return;

	/* Sanity checks */
	BUG_ON(data->blksz * data->blocks > 512*65535);
	BUG_ON(data->blksz > host->mmc->max_blk_size);
	BUG_ON(data->blocks > 65535);
	DBG("data->blksz=%08x, data->blocks=%08x\n",data->blksz,data->blocks);
	host->data = data;

	ret = sdhci_adma_table_pre(host, data);
	if (ret) {
				/* This only happens when someone fed us an invalid request.*/
		WARN_ON(1);
		printk("someone fed us an invalid request/n");
	} 
}

 

static void sdhci_finish_data(struct sdhci_host *host)
{	
	struct mmc_data *data;

	if(host->data){
	 
		data = host->data;
	
		sdhci_adma_table_post(host, data);
 	}
	/*
	 * The specification states that the block count register must
	 * be updated, but it does not specify at what point in the
	 * data flow. That makes the register entirely useless to read
	 * back so we have to assume that nothing made it to the card
	 * in the event of an error.
	 */
	if (data->error)
		data->bytes_xfered = 0;
	else
		data->bytes_xfered = data->blksz * data->blocks;

	tasklet_schedule(&host->finish_tasklet);
}

 
static int simple_send_cmd(struct sdhci_host *host,u32 arg,u32 cmd)
{
    int retry=5;
	int i,ret;
	DBG("cmd is 0x%08x \n",cmd); 
	sdhci_writel(host, arg, SDHCI_ARGUMENT);
	mdelay(10);
	sdhci_writel(host, cmd, SDHCI_COMMAND);
	for(i=0;i<retry;i++){
		if(!(sdhci_readl(host, SDHCI_PRESENT_STATE)&(1<<0)))
				break;
		mdelay(10);
	}
	if(i==retry)
		 ret= 1;
	else
		 ret= 0;
	return ret;
}
static int sdhci_restart_controller(struct sdhci_host *host)
{
int i=0;
_restart:
	sdhci_reset(host);
	 i++;
	 if(i==3)
		return -1;
	switch(host->cmd->opcode){
						case SD_SEND_IF_COND:
									if(simple_send_cmd(host,0x1ef,0x80a))
														goto _restart;
										break;
						case SD_APP_OP_COND:
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906))
														goto _restart;
										break;
						case MMC_ALL_SEND_CID:
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906)
										||simple_send_cmd(host,0x0,0x209))
														goto _restart;
										break;
						case MMC_SET_RELATIVE_ADDR:
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906)
										||simple_send_cmd(host,0x0,0x209)||simple_send_cmd(host,0x0,0x31a))
														goto _restart;
										break;
						case MMC_SEND_CSD:
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906)
										||simple_send_cmd(host,0x0,0x209)||simple_send_cmd(host,0x0,0x31a)||simple_send_cmd(host,0xe6240000,0x909))
														goto _restart;
										break;
						case MMC_SELECT_CARD:
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906)
										||simple_send_cmd(host,0x0,0x209)||simple_send_cmd(host,0x0,0x31a)||simple_send_cmd(host,0xe6240000,0x909)
										||simple_send_cmd(host,0xe6240000,0x70b))
														goto _restart;
										break;
						case SD_APP_SET_BUS_WIDTH :
									if(simple_send_cmd(host,0x1ef,0x80a)||simple_send_cmd(host,0x50000000,0x2906)
										||simple_send_cmd(host,0x50200000,0x2906)||simple_send_cmd(host,0x50200000,0x2906)
										||simple_send_cmd(host,0x0,0x209)||simple_send_cmd(host,0x0,0x31a)||simple_send_cmd(host,0xe6240000,0x909)
										||simple_send_cmd(host,0xe6240000,0x70b)||simple_send_cmd(host,0x02,0x60e))
														goto _restart;
										break;
						 				
	}
 return 0;
}
static void sdhci_send_command(struct sdhci_host *host, struct mmc_command *cmd)
{	
	static m;
	m++;
    DBG(" %d \n",m);
	int flags,i,flag2;
	u32 mask;
	unsigned long timeout;
	unsigned int acmd_flag;

	WARN_ON(host->cmd);
	timeout = 10;	/* Wait max 10 ms */
	
	if(cmd->opcode==MMC_SEND_CSD)
		sdhci_writel(host,0xe6240000, SDHCI_RCA);	
	
	mask = SDHCI_CMD_INHIBIT;
	if ((cmd->data != NULL) || (cmd->flags & MMC_RSP_BUSY))
		mask |= SDHCI_DATA_INHIBIT;

	/* We shouldn't wait for data inihibit for stop commands, even
	   though they might use busy signaling */
	if (host->mrq->data)
		mask &= ~SDHCI_DATA_INHIBIT;

	while (sdhci_readl(host, SDHCI_PRESENT_STATE) & mask) {
		if (timeout == 0) {
			printk(KERN_ERR "%s: Controller never released "
				"inhibit bit(s).\n", mmc_hostname(host->mmc));
			sdhci_dumpregs(host);
			cmd->error = -EIO;
			tasklet_schedule(&host->finish_tasklet);
			return;
		}
		timeout--;
		mdelay(1);
	}

	//mod_timer(&host->timer, msecs_to_jiffies(500));

	host->cmd = cmd;
	
	if (host->quirks & SDHCI_QUIRK_INIT_ISSUE_CMD) {
		flag2=host->ops->init_issue_cmd(host);
		if(flag2==2)
		return ;
	}
	
	if(cmd->data){
		sdhci_prepare_data(host, cmd->data);
		return ;
	}
	sdhci_writel(host, cmd->arg, SDHCI_ARGUMENT);
	DBG("cmd->arg is 0x%08x \n",cmd->arg);

	if ((cmd->flags & MMC_RSP_136) && (cmd->flags & MMC_RSP_BUSY)) {
		printk(KERN_ERR "%s: Unsupported response type!\n",
			mmc_hostname(host->mmc));
		cmd->error = -EINVAL;
		tasklet_schedule(&host->finish_tasklet);
		return;
	}

	if (!(cmd->flags & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->flags & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->flags & MMC_RSP_BUSY)
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
	else
		flags = SDHCI_CMD_RESP_SHORT;

	if (cmd->flags & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->flags & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;
	if (cmd->data)
		flags |= SDHCI_CMD_DATA;
	mdelay(10);
	acmd_flag = (flag2&0x01)?(1<<2):(0<<2);
	sdhci_writel(host, SDHCI_MAKE_CMD(cmd->opcode, flags)|(acmd_flag&0xff), SDHCI_COMMAND);
	DBG("cmd->opcode is 0x%08x \n",SDHCI_MAKE_CMD(cmd->opcode, flags)|(acmd_flag&0xff));
	if(cmd->opcode==SD_SEND_IF_COND)
		mdelay(100);
	else 
		mdelay(50);
	sdhci_finish_command(host);
}

static void sdhci_finish_command(struct sdhci_host *host)
{
	int i;
	unsigned int retry=0x03;
	BUG_ON(host->cmd == NULL);
	
	for(i=0;i<retry;i++){
		 
		if(sdhci_readl(host, SDHCI_PRESENT_STATE)&(1<<1)){
			DBG("ADMA BUSY \n");
		}
		else if(sdhci_readl(host, SDHCI_PRESENT_STATE)&(1<<0)){
			DBG("CMD BUSY\n");
		} 
		else{
		    DBG("NO BUST STAT\n");
			break;
		}
		mdelay(10);
	}
	
	if(i==retry){
		DBG("card is still busy and restart the controller\n");
		if(sdhci_restart_controller(host)){
				host->cmd->error=-ETIMEDOUT;
				goto e_out;
			}		
	}
	host->cmd->resp[0]=0xaaaaaaaa;
	host->cmd->resp[1]=0xaaaaaaaa;
	host->cmd->resp[2]=0xaaaaaaaa;
	host->cmd->resp[3]=0xaaaaaaaa;
	
	if (host->cmd->flags & MMC_RSP_PRESENT) {
		if (host->cmd->flags & MMC_RSP_136) {
			/* CRC is stripped so we need to do some shifting. */
			for (i = 0;i < 4;i++) {
				host->cmd->resp[i] = sdhci_readl(host,
					SDHCI_RESPONSE + (3-i)*4) << 8;
				if (i != 3)
					host->cmd->resp[i] |=
						(sdhci_readl(host,SDHCI_RESPONSE + (3-i-1)*4)&0xFF000000)>> 24;
			}
		} else {
			host->cmd->resp[0] = sdhci_readl(host, SDHCI_RESPONSE);
		}
	}
	for (i = 0;i < 4;i++)
		DBG("host->cmd->resp[%d] is 0x%08x \n",i,host->cmd->resp[i]);
	
	if(!host->cmd->resp[0])
		host->cmd->error = EBUSY;
	else
		host->cmd->error = 0;
e_out: 
	tasklet_schedule(&host->finish_tasklet);
	 
	host->cmd = NULL;
	return ;
 
}

 

/*****************************************************************************\
 *                                                                           *
 * MMC callbacks                                                             *
 *                                                                           *
\*****************************************************************************/

static void sdhci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{	
	static m;
	m++;
    DBG(" %d \n",m);
	struct sdhci_host *host;
	bool present;
	unsigned long flags;

	host = mmc_priv(mmc);

	spin_lock_irqsave(&host->lock, flags);

	WARN_ON(host->mrq != NULL);
	host->mrq = mrq;

	/* If polling, assume that the card is always present. */
	if ((host->quirks & SDHCI_QUIRK_BROKEN_CARD_DETECTION) ||
	    (host->quirks & SDHCI_QUIRK_BROKEN_CARD_PRESENT_BIT))
		present = true;
	else
		present = sdhci_readl(host, SDHCI_PRESENT_STATE) &
				SDHCI_CARD_PRESENT;

	if (!present || host->flags & SDHCI_DEVICE_DEAD) {
		host->mrq->cmd->error = -ENOMEDIUM;
		tasklet_schedule(&host->finish_tasklet);
	} else
		sdhci_send_command(host, mrq->cmd);
		
	spin_unlock_irqrestore(&host->lock, flags);
}

static void sdhci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{	
 
}

static int sdhci_get_ro(struct mmc_host *mmc)
{
	 
}

static void sdhci_enable_sdio_irq(struct mmc_host *mmc, int enable)
{

}

static struct mmc_host_ops sdhci_ops = {
	.request	= sdhci_request,
	.set_ios	= sdhci_set_ios,
	//.get_ro		= sdhci_get_ro,
	//.enable_sdio_irq = sdhci_enable_sdio_irq,
};

/*****************************************************************************\
 *                                                                           *
 * Tasklets                                                                  *
 *                                                                           *
\*****************************************************************************/

static void sdhci_tasklet_card(unsigned long param)
{	
	static m;
	m++;
    DBG(" %d \n",m);
	struct sdhci_host *host;
	unsigned long flags;

	host = (struct sdhci_host*)param;

	spin_lock_irqsave(&host->lock, flags);

    sdhci_reset(host);
	spin_unlock_irqrestore(&host->lock, flags);

	mmc_detect_change(host->mmc, 0);
}

static void sdhci_tasklet_finish(unsigned long param)
{	
	static m;
	m++;
    DBG(" %d \n",m);
	struct sdhci_host *host;
	unsigned long flags;
	struct mmc_request *mrq;

	host = (struct sdhci_host*)param;

	spin_lock_irqsave(&host->lock, flags);

	//del_timer(&host->timer);

	mrq = host->mrq;

	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;

	spin_unlock_irqrestore(&host->lock, flags);

	mmc_request_done(host->mmc, mrq);
}

static void sdhci_timeout_timer(unsigned long data)
{
	static m;
	m++;
    DBG(" %d \n",m);
	struct sdhci_host *host;
	unsigned long flags;
     
	host = (struct sdhci_host*)data;

	spin_lock_irqsave(&host->lock, flags);

	if (host->mrq) {
		printk(KERN_ERR "%s: Timeout interrupt.\n", mmc_hostname(host->mmc));
		sdhci_dumpregs(host);

		if (host->data) {
			host->data->error = -ETIMEDOUT;
			sdhci_finish_data(host);
		} else {
			if (host->cmd)
				host->cmd->error = -ETIMEDOUT;
			else
				host->mrq->cmd->error = -ETIMEDOUT;

			tasklet_schedule(&host->finish_tasklet);
		}
	}


	spin_unlock_irqrestore(&host->lock, flags);
}

 

/*****************************************************************************\
 *                                                                           *
 * Interrupt handling                                                        *
 *                                                                           *
\*****************************************************************************/

static irqreturn_t sdhci_irq(int irq, void *dev_id)
{	static m;
	m++;
    DBG(" %d \n",m);
	irqreturn_t result;
	struct sdhci_host* host = dev_id;
	u32 intmask;

	spin_lock(&host->lock);

	intmask = sdhci_readl(host, SDHCI_INT_STATUS);

	if (!intmask || intmask == 0xffffffff) {
		result = IRQ_NONE;
		goto out;
	}

	DBG("*** %s got interrupt: 0x%08x\n",
		mmc_hostname(host->mmc), intmask);

		if (( intmask&SDHCI_INT_CARD_REMOVE)==SDHCI_INT_CARD_REMOVE) {
		printk("found card remove \n");
		intmask &= ~(SDHCI_INT_CARD_REMOVE);
		sdhci_writel(host, intmask , SDHCI_INT_STATUS);
		tasklet_schedule(&host->card_tasklet);
 
	}
	
	if ((intmask & SDHCI_INT_CARD_INSERT)== SDHCI_INT_CARD_INSERT ) {
		printk("found card insert\n");
		intmask &= ~(SDHCI_INT_CARD_INSERT);
		sdhci_writel(host, intmask, SDHCI_INT_STATUS);
		tasklet_schedule(&host->card_tasklet);
 
	}
	
	if (intmask & SDHCI_INT_ECC) {
	    printk("found ecc error during transport\n");
		intmask &= ~(SDHCI_INT_ECC);
		sdhci_writel(host, intmask ,SDHCI_INT_STATUS);
 
	}

	if (intmask & SDHCI_INT_TRAN_DONE) {
	    DBG("transport done \n");
		intmask &= ~(SDHCI_INT_TRAN_DONE);
		sdhci_writel(host, intmask ,SDHCI_INT_STATUS);
		sdhci_finish_data(host);
	}	 
 
	if (intmask) {
		DBG("%s: Unexpected interrupt 0x%08x.\n",
			mmc_hostname(host->mmc), intmask);
		sdhci_dumpregs(host);
		sdhci_writel(host,0x00, SDHCI_INT_STATUS);
	}

	result = IRQ_HANDLED;
out:
	spin_unlock(&host->lock);
	return result;
}

/*****************************************************************************\
 *                                                                           *
 * Suspend/resume                                                            *
 *                                                                           *
\*****************************************************************************/

#ifdef CONFIG_PM

int sdhci_suspend_host(struct sdhci_host *host, pm_message_t state)
{
	return 0;
}

EXPORT_SYMBOL_GPL(sdhci_suspend_host);

int sdhci_resume_host(struct sdhci_host *host)
{
	return 0;
}

EXPORT_SYMBOL_GPL(sdhci_resume_host);

#endif /* CONFIG_PM */

/*****************************************************************************\
 *                                                                           *
 * Device allocation/registration                                            *
 *                                                                           *
\*****************************************************************************/

struct sdhci_host *sdhci_alloc_host(struct device *dev,
	size_t priv_size)
{
	struct mmc_host *mmc;
	struct sdhci_host *host;

	WARN_ON(dev == NULL);

	mmc = mmc_alloc_host(sizeof(struct sdhci_host) + priv_size, dev);
	if (!mmc)
		return ERR_PTR(-ENOMEM);

	host = mmc_priv(mmc);
	host->mmc = mmc;

	return host;
}

EXPORT_SYMBOL_GPL(sdhci_alloc_host);

int sdhci_add_host(struct sdhci_host *host)
{
	struct mmc_host *mmc;
	unsigned int caps;
	int ret;

	WARN_ON(host == NULL);
	if (host == NULL)
		return -EINVAL;

	mmc = host->mmc;

	sdhci_reset(host);
 
    host->flags |= SDHCI_USE_ADMA;
	if (host->flags & SDHCI_USE_ADMA) {
		/*
		 * We need to allocate descriptors for all sg entries
		 * (12) and potentially one alignment transfer for
		 * each of those entries.
		 */
		host->adma_desc =(u32 *) kmalloc(16, GFP_KERNEL);
		 
		if (!host->adma_desc ) {
			kfree(host->adma_desc);
			printk(KERN_WARNING "%s: Unable to allocate ADMA "
				"buffers. Falling back to standard DMA.\n",
				mmc_hostname(mmc));
			host->flags &= ~SDHCI_USE_ADMA;
		}
	}

	/*
	 * If we use DMA, then it's up to the caller to set the DMA
	 * mask, but PIO does not need the hw shim so we set a new
	 * mask here in that case.
	 */
	if (!(host->flags & (SDHCI_USE_SDMA | SDHCI_USE_ADMA))) {
		host->dma_mask = DMA_BIT_MASK(64);
		mmc_dev(host->mmc)->dma_mask = &host->dma_mask;
	}
		 
	host->max_clk = 0;
	if (host->max_clk == 0) {
		if (!host->ops->get_max_clock) {
			printk(KERN_ERR
			       "%s: Hardware doesn't specify base clock "
			       "frequency.\n", mmc_hostname(mmc));
			return -ENODEV;
		}
		host->max_clk = host->ops->get_max_clock(host);
	}

	host->timeout_clk =0	; 
	if (host->timeout_clk == 0) {
		if (host->ops->get_timeout_clock) {
			host->timeout_clk = host->ops->get_timeout_clock(host);
		} else if (!(host->quirks &
				SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK)) {
			printk(KERN_ERR
			       "%s: Hardware doesn't specify timeout clock "
			       "frequency.\n", mmc_hostname(mmc));
			return -ENODEV;
		}
	}

	/*
	 * Set host parameters.
	 */
	if(host->ops->get_ro)
		sdhci_ops.get_ro = host->ops->get_ro;

	mmc->ops = &sdhci_ops;
	 
	mmc->f_min = 400000;
	mmc->f_max = 52000000;

	mmc->caps |= MMC_CAP_4_BIT_DATA;
	mmc->caps |= MMC_CAP_SD_HIGHSPEED;
 
	mmc->ocr_avail=MMC_VDD_32_33|MMC_VDD_33_34|MMC_VDD_165_195;
 
	spin_lock_init(&host->lock);

	mmc->max_hw_segs = 1;
	 
	mmc->max_phys_segs = 1 ;

	mmc->max_blk_size = 512  ;
	mmc->max_blk_count =  65534;
	
	mmc->max_req_size = 65534*512;

	mmc->max_seg_size = mmc->max_req_size;
	/* Init tasklets.*/
	tasklet_init(&host->card_tasklet,sdhci_tasklet_card, (unsigned long)host);
	tasklet_init(&host->finish_tasklet,sdhci_tasklet_finish, (unsigned long)host);

	setup_timer(&host->timer, sdhci_timeout_timer, (unsigned long)host);
	 
	ret = request_irq(host->irq, sdhci_irq, IRQF_SHARED,mmc_hostname(mmc), host);
	if (ret){
		printk("register irq failed \n");
		goto untasklet;
	}	

	mmc_add_host(mmc);
	 
	printk(KERN_INFO "%s: SDHCI controller on %s [%s] using %s\n",
		mmc_hostname(mmc), host->hw_name, dev_name(mmc_dev(mmc)),
		(host->flags & SDHCI_USE_ADMA) ? "ADMA" :
		(host->flags & SDHCI_USE_SDMA) ? "DMA" : "PIO");

	return 0;
untasklet:
	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);
	return ret;
}

EXPORT_SYMBOL_GPL(sdhci_add_host);

void sdhci_remove_host(struct sdhci_host *host, int dead)
{
	unsigned long flags;

	if (dead) {
		spin_lock_irqsave(&host->lock, flags);
		host->flags |= SDHCI_DEVICE_DEAD;
		if (host->mrq) {
			printk(KERN_ERR "%s: Controller removed during "
					" transfer!\n", mmc_hostname(host->mmc));
			host->mrq->cmd->error = -ENOMEDIUM;
			tasklet_schedule(&host->finish_tasklet);
		}
		spin_unlock_irqrestore(&host->lock, flags);
	}

	mmc_remove_host(host->mmc);
	if (!dead)
		sdhci_reset(host);

	free_irq(host->irq, host);
	del_timer_sync(&host->timer);

	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);

	kfree(host->adma_desc);

	host->adma_desc = NULL;
}

EXPORT_SYMBOL_GPL(sdhci_remove_host);

void sdhci_free_host(struct sdhci_host *host)
{
	mmc_free_host(host->mmc);
}

EXPORT_SYMBOL_GPL(sdhci_free_host);

/*****************************************************************************\
 *                                                                           *
 * Driver init/exit                                                          *
 *                                                                           *
\*****************************************************************************/

static int __init sdhci_drv_init(void)
{
	printk(KERN_INFO DRIVER_NAME
		": Secure Digital Host Controller Interface driver\n");
	printk(KERN_INFO DRIVER_NAME ": Copyright(c) liu dan dan \n");

	return 0;
}

static void __exit sdhci_drv_exit(void)
{
}

module_init(sdhci_drv_init);
module_exit(sdhci_drv_exit);

module_param(debug_quirks, uint, 0404);

MODULE_AUTHOR("Pierre Ossman <pierre@ossman.eu>");
MODULE_DESCRIPTION("Secure Digital Host Controller Interface core driver");
MODULE_LICENSE("GPL");

MODULE_PARM_DESC(debug_quirks, "Force certain quirks.");
