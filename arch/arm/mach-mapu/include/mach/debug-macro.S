
/* pull in the relevant register and map files. */

#include <mach/map.h>
/* #include <mach/regs-serial.h> */
#include <mach/map-va.h>

	.macro	addruart,rx
	mrc	p15, 0, \rx, c1, c0, 0
	tst	\rx, #1					@ MMU enabled?
	ldreq	\rx, =MAPU_PA_UART0			@ physical
	ldrne	\rx, =MAPU_VA_UART0	@ virtual
	.endm

#define UART_SHIFT	2
#include <asm/hardware/debug-8250.S>



	
