/* linux/arch/arm/mach-mapu/include/mach/regs-lcd.h
 *
 * mapu - LCD DISPLAY CONTROLLER register definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_REGS_LCD_H
#define __ASM_ARCH_REGS_LCD_H __FILE__

#include <mach/map.h>

#define LCD_VER_SYN    0x00
#define LCD_VER_ACT    0x04
#define LCD_HOR_SYN    0x08
#define LCD_HOR_ACT    0x0C

#define LCD_CON        0x10
#define  TEST_MODE1     1<<24
#define  TEST_MODE2     1<<25
#define  DUAL_MODE     1<<26
#define  DUAL_SWAP     1<<27
#define  DE_POL_SET     1<<28
#define  HSYN_POL_SET     1<<29
#define  VSYN_POL_SET     1<<30
#define  BASE_MODE     1<<31

#define LCD_LAYER_SEQ    0x14

#define LCD_LAYER_EN   0x18
#define ENABLE_LAY0		1<<0
#define ENABLE_LAY1		1<<4
#define ENABLE_LAY2		1<<8
#define ENABLE_LAY3		1<<12
#define ENABLE_LAY4		1<<16
#define ENABLE_INT		1<<24
#define ENABLE_DIS		1<<28

#define LCD_SCREEN_SIZE    0x1c

#define LCD_LAY0_BASE    0x20
#define LCD_LAY0_LOCAL    0x24
#define LCD_LAY0_SIZE    0x28
#define LCD_LAY0_WIN_LOCAL    0x2c
#define LCD_LAY0_WIN_SIZE    0x30
#define LCD_LAY0_PACITY    0x34

#define LCD_LAY1_BASE    0x40
#define LCD_LAY1_LOCAL    0x44
#define LCD_LAY1_SIZE    0x48
#define LCD_LAY1_WIN_LOCAL    0x4c
#define LCD_LAY1_WIN_SIZE    0x50
#define LCD_LAY1_PACITY    0x54

#define LCD_LAY2_BASE    0x60
#define LCD_LAY2_LOCAL    0x64
#define LCD_LAY2_SIZE    0x68
#define LCD_LAY2_WIN_LOCAL    0x6c
#define LCD_LAY2_WIN_SIZE    0x70
#define LCD_LAY2_PACITY    0x74

#define LCD_LAY3_BASE    0x80
#define LCD_LAY3_LOCAL    0x84
#define LCD_LAY3_SIZE    0x88
#define LCD_LAY3_WIN_LOCAL    0x8c
#define LCD_LAY3_WIN_SIZE    0x90
#define LCD_LAY3_PACITY    0x94

#define LCD_LAY4_BASE    0xa0
#define LCD_LAY4_LOCAL    0xa4
#define LCD_LAY4_SIZE    0xa8
#define LCD_LAY4_WIN_LOCAL    0xac
#define LCD_LAY4_WIN_SIZE    0xb0
#define LCD_LAY4_PACITY    0xb4

#define LCD_BASIC_MODE    0xc0
#define LCD_INTC    0xc4
#define LCD_INTC_CLEAR	1<<0
#define LCD_INTC_FLAG	1<<4  
#define LCD_FRAME_COUNTER    0xc8
#define LCD_AXI    0xcc


 

#endif /* __ASM_ARCH_REGS_LCD_H */
