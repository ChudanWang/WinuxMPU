/* linux/arch/arm/plat-mapu/include/plat/spi.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __PLAT_SPI_H
#define __PLAT_SPI_H __FILE__

struct mapu_spi_info {
	unsigned int		 num_cs;	/* total chipselects */
	int			 bus_num;       /* bus number to use. */
	unsigned int   txftl; //transmit fifo threshold
	unsigned int   rxftl;//receive fifo threshold
	unsigned int   imr;//  interrupt mask
	unsigned int   dmatdl;//dma  transmit data level
	unsigned int   dmardl;//dma   receive  data level
	
};

#endif /* __PLAT_SPI_H */
