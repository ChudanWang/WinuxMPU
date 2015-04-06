# arch/arm/mach-mapu/Makefile
#
# Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences) 
#
# Licensed under GPLv2

obj-y				:=
obj-m				:=
obj-n				:=
obj-				:=

# Core support for MAPU system

obj-$(CONFIG_CPU_MAPU)	+=   time.o  cpu.o irq.o  bootmem.o dev-uart.o


# machine support
 
obj-$(CONFIG_MACH_MAPU)	+= mach-mapu.o
 
# device support

obj-$(CONFIG_ICTL_MAPU)			+= dev-ictl.o
obj-$(CONFIG_FB_MAPU)			+= dev-fb.o
obj-$(CONFIG_MMC_SDHCI_MAPU)		+= dev-sdhci.o
obj-$(CONFIG_GPIO_MAPU)			+= gpio.o 
obj-$(CONFIG_SPI_MAPU)				+= dev-spi.o
obj-$(CONFIG_GPU_DEVICE)			+= dev-gpu.o 
obj-$(CONFIG_GMAC_MAPU)				+= dev-gmac.o 
obj-$(CONFIG_DW_DMAC)				+= dev-dmac.o
