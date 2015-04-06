/* linux/arch/arm/mach-mapu/include/mach/regs-sdhci.h
 *
 * MAPU Platform - SDHCI (HSMMC) register definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __MAPU_SDHCI_REGS_H
#define __MAPU_SDHCI_REGS_H __FILE__

#define SDHCI_RCA	0x00

#define SDHCI_BLOCK_COUNT	0x04

#define SDHCI_ARGUMENT		0x08
 
#define SDHCI_COMMAND		0x0C
#define  SDHCI_CMD_RESP_MASK	0x03
#define  SDHCI_CMD_CRC		0x08
#define  SDHCI_CMD_INDEX	0x10
#define  SDHCI_CMD_DATA		0x20

#define  SDHCI_CMD_RESP_NONE	0x00
#define  SDHCI_CMD_RESP_LONG	0x01
#define  SDHCI_CMD_RESP_SHORT	0x02
#define  SDHCI_CMD_RESP_SHORT_BUSY 0x03

#define SDHCI_MAKE_CMD(c, f) (((c & 0xff) << 8) | (f & 0xff))

#define SDHCI_RESPONSE		0x10

#define SDHCI_PRESENT_STATE	0x24
#define  SDHCI_CMD_INHIBIT	0x00000001
#define  SDHCI_DATA_INHIBIT	0x00000002
#define  SDHCI_DOING_WRITE	0x00000100
#define  SDHCI_DOING_READ	0x00000200
#define  SDHCI_SPACE_AVAILABLE	0x00000400
#define  SDHCI_DATA_AVAILABLE	0x00000800
#define  SDHCI_CARD_PRESENT	0x00010000
#define  SDHCI_WRITE_PROTECT	0x00080000

#define SDHCI_INT_STATUS	0x30
 
#define  SDHCI_INT_ECC       		0x01<<0
#define  SDHCI_INT_TRAN_DONE      	0x01<<1
#define  SDHCI_INT_CARD_INSERT		0x01<<3
#define  SDHCI_INT_CARD_REMOVE		0x03<<2

#define  SDHCI_MAX_BLOCK_MASK	0x00030000
#define  SDHCI_MAX_BLOCK_SHIFT  16
/* 55-57 reserved */

#define SDHCI_ADMA_ADDRESS	0x58

#define SDHCI_ADMA_POINT_ADDRESS	0x5C

#define SDHCI_AHB_HPROT	0x60
#define SDHCI_SOFTWARE_RESET	(0x01 << 31)

struct platform_device;
struct mmc_host;
struct mmc_card;
struct mmc_ios;

struct mapu_sdhci_platdata{
	unsigned int	max_width;
	unsigned int	host_caps;
	
	char		**clocks;	/* set of clock sources */
	void	(*cfg_gpio)(struct platform_device *dev, int width);
	void	(*cfg_card)(struct platform_device *dev,
			    void __iomem *regbase,
			    struct mmc_ios *ios,
			    struct mmc_card *card);

	/* add to deal with EXT_IRQ as a card detect pin */
	void            (*cfg_ext_cd) (void);
	unsigned int    ext_cd;

	/* add to deal with GPIO as a card write protection pin */
	void            (*cfg_wp) (void);
	int             (*get_ro) (struct mmc_host *mmc);
};
 
 
#endif /* __MAPU_SDHCI_REGS_H */
