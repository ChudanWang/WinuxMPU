   /*mapu timers for system*/
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include <asm/mach-types.h>
#include <asm/mach/time.h>

#include <asm/irq.h>
#include <mach/map.h>
#include <mach/regs-timer.h>//note
#include <mach/irqs.h>
#include <mach/map-va.h>
#include <mach/cpu.h>

#ifndef TICK_MAX
#define TICK_MAX 0xffffffff
#endif

 
#define TIMER_CLK  50000000 
   /*
 * IRQ handler for the timer
 */
static irqreturn_t
mapu_timer_interrupt(int irq, void *dev_id)
{   
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction mapu_timer_irq = {
	.name		= "mapu Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= mapu_timer_interrupt,
};


/*
 * Set up timer interrupt, and return the current time in seconds.
 *
 * Currently we only use timer8, as it is the only timer which has no
 * other function that can be exploited externally
 */
static void mapu_timer_setup (void)
{ /*chose timer8 as the system timer*/
     unsigned long cnt;
	 unsigned long tmp;
	 
	 cnt=TIMER_CLK/HZ;
	 /*disable timer, unmask ,usr_defined mode*/
	 __raw_writel(Timer_Mode_Usr,MAPU_VA_TIMER+Timer8CRE);
	 /*load the value*/
	 __raw_writel(cnt,MAPU_VA_TIMER+Timer8LCT);
	 /*enable timer*/
	 __raw_writel(Timer_EN|Timer_Mode_Usr,MAPU_VA_TIMER+Timer8CRE);
	printk("init system timer setup\n") ;
}

static void __init mapu_timer_init(void)
{   int ret;
	mapu_timer_setup();
	ret=setup_irq(SYS_TIMER_IRQ, &mapu_timer_irq);
	if(ret)
	  printk("system timer: could not request IRQ \n");
}

struct sys_timer mapu_timer = {
	.init		= mapu_timer_init,
	.resume		= mapu_timer_setup
};
