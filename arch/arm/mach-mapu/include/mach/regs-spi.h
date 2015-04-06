/*
 * spi_mapu.h - MAPU SOC SPI controller driver.
 *
 */

#ifndef _LINUX_SPI_MAPU_H
#define _LINUX_SPI_MAPU_H

#define MAPU_SPI_HZ  50000000  /* 50MHz */
#define MAPU_SPI_FIFO_LENGTH  32

#define MAPU_SPI_CTRLR0		(0x00)      //SPI control register 0
#define MAPU_SPI_CTRLR1		(0x04)      //spi control register 1
#define MAPU_SPI_SSIENR		(0x08)      //SPI enable register
#define MAPU_SPI_MWCR		(0x0C)      //microwire control  register
#define MAPU_SPI_SER		(0x10)      //SPI slave enable register
#define MAPU_SPI_BAUDR		(0x14)      //SPI baud rate select
#define MAPU_SPI_TXFTLR		(0x18)      //SPI transmit fifo threshold level
#define MAPU_SPI_RXFTLR		(0x1C)      //SPI receive  fifo threshold level
#define MAPU_SPI_TXFLR		(0x20)      //transmit fifo  level register
#define MAPU_SPI_RXFLR		(0x24)      //receive fifo  level register
#define MAPU_SPI_SR		(0x28)      //status  register
#define MAPU_SPI_IMR		(0x2c)      //interrupt mash register
#define MAPU_SPI_ISR		(0x30)      //interrupt status register
#define MAPU_SPI_RISR		(0x34)      //raw  interrupt status register
#define MAPU_SPI_TXOICR		(0x38)      //transmit fifo  overflow interrupt clear register
#define MAPU_SPI_RXOICR		(0x3C)      //receive  fifo overflow interrupt clear register
#define MAPU_SPI_RXUICR		(0x40)      //receive  fifo underflow interrupt clear register
#define MAPU_SPI_MSTICR		(0x44)      //multi-master  register
#define MAPU_SPI_ICR		(0x48)      //interrupt clear  register
#define MAPU_SPI_DMACR		(0x4C)      //DMA control register
#define MAPU_SPI_DMATDLR		(0x50)      //DMA transmit data level register
#define MAPU_SPI_DMARDLR		(0x54)      //DMA receive data  level register
#define MAPU_SPI_IDR			(0x58)      //identification register
#define MAPU_SPI_VERSION		(0x5C)      //version id register
#define MAPU_SPI_DR				(0x60)       //data  register
#define MAPU_SPI_RX_SAMPLE_DLY	(0xF0)      //RXD sample delay  register
#define MAPU_SPI_RSVD_0			(0xF4)      //reserved location for future use
#define MAPU_SPI_RSVD_1			(0xF8)      //reserved location for future use
#define MAPU_SPI_RSVD_2			(0xFC)      //reserved location for future use

#define DATA_FRAME_SIZE(size)     	((size&0x0f)<<0)
#define FRAME_FORMAT(n)         	((n&0x03)<<4)
#define SPI_CPHA_B		    		(1<<6)
#define SPI_CPOL_L		    		(1<<7)
#define SPI_TMOD(n)         		((n&0x03)<<8)
#define SPI_SLAVE_OE				(1<<10)
#define SPI_TEST_MODE				(1<<11)
#define SPI_CON_FRAME_SIZE(size)    ((size&0x0f)<<12)

#define SALVE_ENABLE(n)    			(1<<n)

#define SPI_BUSY         			(1<<0)
#define SPI_TX_FIFO_NOT_FULL        (1<<1)
#define SPI_TX_FIFO_EMPTY         	(1<<2)
#define SPI_RX_FIFO_NOT_EMPTY       (1<<3)
#define SPI_RX_FIFO_FULL         	(1<<4)
#define SPI_TX_ERR         			(1<<5)
#define SPI_DATA_COLL         		(1<<6)

#define TX_FIFO_EMPTY_NO_MASK  		(1<<0)
#define TX_FIFO_OVERFLOW_NO_MASK  	(1<<1)
#define RX_FIFO_UNDERFLOW_NO_MASK  	(1<<2)
#define RX_FIFO_OVERFLOW_NO_MASK  	(1<<3)
#define RX_FIFO_FULL_NO_MASK  		(1<<4)
#define MULTI_CONTENT_NO_MASH  		(1<<5)
 
#define TX_FIFO_EMPTY_INT    		(1<<0)
#define TX_FIFO_OVERFLOW_INT  		(1<<1)
#define RX_FIFO_UNDERFLOW_INT  		(1<<2)
#define RX_FIFO_OVERFLOW_INT  		(1<<3)
#define RX_FIFO_FULL_INT  			(1<<4)
#define MULTI_CONTENT_INT  			(1<<5)
  
#define MAPU_SPI_DMABUF_LEN			(16*1024)
 
#define MAPU_SPI_FIFO_DEPTH  		(8) 
#endif 	//_LINUX_SPI_MAPU_H
