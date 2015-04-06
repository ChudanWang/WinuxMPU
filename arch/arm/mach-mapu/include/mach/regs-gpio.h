/* linux/arch/arm/mach-MAPUv210/include/mach/regs-gpio.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * MAPUV210 - GPIO register definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_REGS_GPIO_H
#define __ASM_ARCH_REGS_GPIO_H __FILE__

#define		GPIOA_DR 0x00
#define		GPIOB_DR 0x0c
#define 	GPIOC_DR 0x18
#define		GPIOD_DR 0x24

#define		GPIO_DR	0X00      //port A data register
#define		GPIO_DDR	0X04		//port 	A data direction register	
#define		GPIO_CTL	0X08    	//port  A	data source register

#define		GPIO_INT_EN	0X30		//interrupt enable register
#define		GPIO_INT_MASK	0X34	//interrupt	mask register
#define		GPIO_INT_TYPE	0X38	//interrupt	level	register
#define		GPIO_INT_POL	0X3C	//interrupt	polarity register
#define		GPIO_INT_STA	0X40	//interrupt status of port A
#define		GPIO_INT_RAWST	0X44	//raw interrupt status of port 	A
#define		GPIO_DEBOUNCE	0X48 	//debounce	enable register
#define		GPIOA_EOI		0X4C	//port A clear interrupt register
#define		GPIOA_EXT		0X50	//port A external port register
#define		GPIOB_EXT		0X54	//port	B	external	port register
#define		GPIOC_EXT		0X58	//port c external port register
#define		GPIOD_EXT		0X5C	//port d external port register
#define		GPIO_LS_SYNC	0X60	//Level-sensitive synchronization	enable register
#define		GPIO_ID_CODE	0X64	//id code register



 
#define		MAPU_GPA_BASE		(MAPU_VA_GPIO+GPIOA_DR)
#define		MAPU_GPB_BASE		(MAPU_VA_GPIO+GPIOB_DR)	
#define		MAPU_GPC_BASE		(MAPU_VA_GPIO+GPIOC_DR)
#define		MAPU_GPD_BASE		(MAPU_VA_GPIO+GPIOD_DR)
/* values for MAPU_EXTINT0 */
#define MAPU_EXTINT_LOWLEV		(0x00)
#define MAPU_EXTINT_HILEV		(0x01)
#define MAPU_EXTINT_FALLEDGE		(0x02)
#define MAPU_EXTINT_RISEEDGE		(0x03)
#define MAPU_EXTINT_BOTHEDGE		(0x04)

#endif /* __ASM_ARCH_REGS_GPIO_H */
