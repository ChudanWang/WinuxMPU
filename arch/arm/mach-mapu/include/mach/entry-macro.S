/* linux/arch/arm/mach-mapu/include/mach/entry-macro.S
 *
 *
 * Low-level IRQ helper macros for the MAPU
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <asm/hardware/vic.h>
#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/map-va.h>
#include <mach/regs-ictl.h>
	.macro	disable_fiq
	.endm

	.macro	get_irqnr_preamble, base, tmp
	 
	.endm

	.macro	arch_ret_to_user, tmp1, tmp2
	.endm

	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
       mov	\base, #MAPU_VA_IRQ
	   ldr	\irqstat, [\base, #IRQ_FINALSTS_L]
	   teq	\irqstat, #0
	   beq	1001f
	   mov  \irqnr,#0
1003:   
	   tst  \irqstat,#1
	   bne 1002f
	   add \irqnr,\irqnr,#1
	   mov \irqstat,\irqstat,lsr #1
	   cmp \irqnr,#32
       bcc 1003b
1001:	
	    ldr	\irqstat, [\base, #IRQ_FINALSTS_H]
	    teq	\irqstat, #0
	    beq	1002f
		mov  \irqnr,#32
1004:
       tst  \irqstat,#1
	   bne 1002f
	   add \irqnr,\irqnr,#1
	   mov \irqstat,\irqstat,lsr #1
	   cmp \irqnr,#64
       bcc 1004b
1005:	
	    ldr	\irqstat, [\base, #FIQ_FINALSTS]
	    teq	\irqstat, #0
	    beq	1002f
		mov  \irqnr,#0
1006:
       tst  \irqstat,#1
	   bne 1002f
	   add \irqnr,\irqnr,#1
	   mov \irqstat,\irqstat,lsr #1
	   cmp \irqnr,#32
       bcc 1006b	   
1002:
   	
	.endm
