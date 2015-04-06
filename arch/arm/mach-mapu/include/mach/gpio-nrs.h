/* arch/arm/mach-MAPU/include/mach/gpio-nrs.h
 *
 * Copyright (c) 2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * MAPU - GPIO bank numbering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __MACH_GPIONRS_H
#define __MACH_GPIONRS_H

#define MAPU_GPIONO(bank,offset) ((bank) + (offset))

#define MAPU_GPIO_BANKA   (32*0)
#define MAPU_GPIO_BANKB   (32*1)
#define MAPU_GPIO_BANKC   (32*2)
#define MAPU_GPIO_BANKD   (32*3)
 

/* GPIO bank sizes */
#define MAPU_GPIO_A_NR	(8)
#define MAPU_GPIO_B_NR	(32)
#define MAPU_GPIO_C_NR	(32)
#define MAPU_GPIO_D_NR	(32)
 


#define MAPU_GPIO_NEXT(__gpio) \
	((__gpio##_START) + (__gpio##_NR) )

#ifndef __ASSEMBLY__

enum mapu_gpio_number {
	MAPU_GPIO_A_START = 0,
	MAPU_GPIO_B_START = MAPU_GPIO_NEXT(MAPU_GPIO_A),
	MAPU_GPIO_C_START = MAPU_GPIO_NEXT(MAPU_GPIO_B),
	MAPU_GPIO_D_START = MAPU_GPIO_NEXT(MAPU_GPIO_C),
	MAPU_GPIO_E_START = MAPU_GPIO_NEXT(MAPU_GPIO_D),
 
};

#endif /* __ASSEMBLY__ */

/* MAPU GPIO number definitions. */

#define MAPU_GPA(_nr)	(MAPU_GPIO_A_START + (_nr))
#define MAPU_GPB(_nr)	(MAPU_GPIO_B_START + (_nr))
#define MAPU_GPC(_nr)	(MAPU_GPIO_C_START + (_nr))
#define MAPU_GPD(_nr)	(MAPU_GPIO_D_START + (_nr))
 

/* compatibility until drivers can be modified */

#define MAPU_GPA0	MAPU_GPA(0)
#define MAPU_GPA1	MAPU_GPA(1)
#define MAPU_GPA3	MAPU_GPA(3)
#define MAPU_GPA7	MAPU_GPA(7)

 

 

#endif /* __MACH_GPIONRS_H */

