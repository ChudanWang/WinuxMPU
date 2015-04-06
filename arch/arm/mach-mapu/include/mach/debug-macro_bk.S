
/* pull in the relevant register and map files. */

#include <mach/map.h>
#include <mach/regs-serial.h>
#include <mach/map-va.h>
	
    .macro addruart, rx, tmp
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1
		ldreq	\rx, = MAPU_PA_UART0
		ldrne	\rx, = MAPU_VA_UART
    .endm
	
    .macro	senduart,rd,rx
		str 	\rd, [\rx, # MAPU_THR]
	.endm
	
    .macro	waituart,rd,rx
1001:		
            ldr	\rd, [\rx, # MAPU_LSR]
		    tst	\rd, #UART_LSR_THRE
		    beq	1001b				 
		     
 		     @ exit busyuart
	 .endm	
     
	.macro  busyuart,rd,rx
	
	.endm