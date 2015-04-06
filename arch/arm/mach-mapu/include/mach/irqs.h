/* linux/arch/arm/mach-mapu/include/mach/irqs.h
 *
 * Copyright (c) 2014 CASIA(Institute of Automation,Chinese Academy of Sciences)
 *
 * MAPU - IRQ definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H __FILE__

 #define IRQ_UART0  0
 #define IRQ_UART1  20
 #define IRQ_TIMER  1
 #define IRQ_RTC  5
 #define IRQ_WATCHDOG  3
 #define IRQ_I2C  2
 #define IRQ_SD  6
 #define IRQ_GMAC  7
 #define IRQ_LCD  11
 #define IRQ_GPIO  8
 #define IRQ_SPI0  9
 
 /*IRQ INT FOR GPU*/
 #define IRQ_PPMMU0_3D           17
 #define IRQ_GPMMU_3D            18
 #define IRQ_PP0_3D              10
 #define IRQ_GP_3D               14
 #define IRQ_PMU_3D              16
 
 #define FIQ_DMA  0
 
 #define IRQ_DMA  12
 #define IRQ_EXTEND(x) ((x)+65)
 #define UART0_RX      IRQ_EXTEND(1)
 #define UART0_TX      IRQ_EXTEND(2)
 #define UART0_ER      IRQ_EXTEND(3)
 #define UART1_RX      IRQ_EXTEND(4)
 #define UART1_TX      IRQ_EXTEND(5)
 #define UART1_ER      IRQ_EXTEND(6)
 #define TIMER1_IRQ      IRQ_EXTEND(7)
 #define TIMER2_IRQ      IRQ_EXTEND(8)
 #define TIMER3_IRQ      IRQ_EXTEND(9)
 #define TIMER4_IRQ      IRQ_EXTEND(10)
 #define TIMER5_IRQ      IRQ_EXTEND(11)
 #define TIMER6_IRQ      IRQ_EXTEND(12)
 #define TIMER7_IRQ      IRQ_EXTEND(14)
 #define TIMER8_IRQ      IRQ_EXTEND(15)
 #define SYS_TIMER_IRQ    TIMER8_IRQ
 #define IRQ_EINT			IRQ_EXTEND(16)
 #define IRQ_EINT_(X)		(IRQ_EINT+X)
 
 
 #define   NR_IRQS    128

 
#endif /* ASM_ARCH_IRQS_H */
