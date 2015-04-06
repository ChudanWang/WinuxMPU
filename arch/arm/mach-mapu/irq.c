#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/irq.h>

#include <asm/irq.h>
#include <asm/mach/irq.h>

#include <mach/regs-ictl.h>
#include <mach/cpu.h>
#include <mach/irqs.h>
#include <mach/regs-serial.h>
#include <mach/regs-timer.h>
#include <mach/regs-lcd.h>
#include <mach/map-va.h>

#ifdef CONFIG_SERIAL_MAPU
/*uart interrupt process*/ 
static void uart_irq_enable(unsigned int irq)
{
  unsigned int uier;
  switch(irq){ 
		case UART0_RX: 
				uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
				uier|= 0x01;
				__raw_writel(uier,MAPU_VA_UART0+MAPU_IER);
               break;
		case UART0_TX:
				uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
				uier|=0x02;
				__raw_writel(uier,MAPU_VA_UART0+MAPU_IER);
				break;
		case UART1_RX: 
				uier=__raw_readl(MAPU_VA_UART1+MAPU_IER);
				uier|=0x01;
				__raw_writel(uier,MAPU_VA_UART1+MAPU_IER);
				break;
		case UART1_TX: 
				uier=__raw_readl(MAPU_VA_UART1+MAPU_IER);
				uier|=0x02;
				__raw_writel(uier,MAPU_VA_UART1+MAPU_IER);
				break;
		default:
				printk("this is not a uart interrupt\n");	
				break;
  }
 // uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
 //printk("uier is %08x \n",uier);  
}

static void uart_irq_disable(unsigned int irq)
{
 unsigned int uier;
  switch(irq){
		case UART0_RX:
				uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
				uier&= ~0x01;
				__raw_writel(uier,MAPU_VA_UART0+MAPU_IER);
				break;
		case UART0_TX:
				uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
				uier&=~0x02;
				__raw_writel(uier,MAPU_VA_UART0+MAPU_IER);
				break;
		case UART1_RX:
				uier=__raw_readl(MAPU_VA_UART1+MAPU_IER);
				uier&=~0x01;
				__raw_writel(uier,MAPU_VA_UART1+MAPU_IER);
				break;
		case UART1_TX:
				uier=__raw_readl(MAPU_VA_UART1+MAPU_IER);
				uier&=~0x02;
				__raw_writel(uier,MAPU_VA_UART1+MAPU_IER);
				break;
		default:
				printk("this is not a uart interrupt\n");	
				break;
  }
}

static void mapu_irq_demux_uart0(unsigned int irq, struct irq_desc *desc)
{
 unsigned int iir,uier;
 
 spin_lock(&desc->lock);
 desc->chip->mask(irq);
 iir=__raw_readl(MAPU_VA_UART0+MAPU_IIR)&0x0f;
 //printk("uart iir status %08x\n",iir);
 switch(iir){
		case UART_IIR_RDI:
				generic_handle_irq(UART0_RX);
				uier=__raw_readl(MAPU_VA_UART0+MAPU_IER);
				//printk("uier is %08x\n",uier);
				uier|= 0x01;
				__raw_writel(uier,MAPU_VA_UART0+MAPU_IER);
				break;
		case UART_IIR_THRI:
				generic_handle_irq(UART0_TX);
				break;
		case UART_IIR_RLSI:
				break;
		case UART_IIR_CTOUT:
				break;		
	} 
 desc->chip->unmask(irq);
 spin_unlock(&desc->lock); 
}

static void mapu_irq_demux_uart1(unsigned int irq, struct irq_desc *desc)
{
   unsigned int iir,uier;
 
 spin_lock(&desc->lock);
 desc->chip->mask(irq);
 iir=__raw_readl(MAPU_VA_UART1+MAPU_IIR)&0x0f;
 //printk("uart iir status %08x\n",iir);
 switch(iir){
		case UART_IIR_RDI:
				generic_handle_irq(UART1_RX);
				uier=__raw_readl(MAPU_VA_UART1+MAPU_IER);
				//printk("uier is %08x\n",uier);
				uier|= 0x01;
				__raw_writel(uier,MAPU_VA_UART1+MAPU_IER);
				break;
		case UART_IIR_THRI:
				generic_handle_irq(UART1_TX);
				break;
		case UART_IIR_RLSI:
				break;
		case UART_IIR_CTOUT:
				break;		
	} 
 desc->chip->unmask(irq);
 spin_unlock(&desc->lock);
}


static struct irq_chip mapu_irq_uart = {
	.name		= "mapu-uart",
	.mask	= uart_irq_enable,
	.unmask	= uart_irq_disable,
	//.ack	= uart_irq_ack,
	.enable=uart_irq_enable,
	.disable=uart_irq_disable,
};

#endif //end of CONFIG_SERIAL_MAPU

/*timer interrupt process*/ 
static void timers_irq_enable(unsigned int irq)
{
  unsigned int val;
  switch(irq){
	case    TIMER1_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer1CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer1CRE);
				break;
	case	TIMER2_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer2CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer2CRE);
				break;
	case	TIMER3_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer3CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer3CRE);
				break;
	case	TIMER4_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer4CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer4CRE);
				break;
	case	TIMER5_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer5CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer5CRE);
				break;
	case	TIMER6_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer6CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer6CRE);
				break;
	case	TIMER7_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer7CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer7CRE);
				break;
	case	TIMER8_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer8CRE);
				val&=~Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer8CRE);
				break;
		default:
		       printk("this is not a timer interrupt\n");
			   break;
  }
}

static void timers_irq_disable(unsigned int irq)
{
  unsigned int val;
  switch(irq){
	case	TIMER1_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer1CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer1CRE);
				break;
	case	TIMER2_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer2CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer2CRE);
				break;
	case	TIMER3_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer3CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer3CRE);
				break;
	case	TIMER4_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer4CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer4CRE);
				break;
	case	TIMER5_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer5CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer5CRE);
				break;
	case	TIMER6_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer6CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer6CRE);
				break;
	case	TIMER7_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer7CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer7CRE);
				break;
	case	TIMER8_IRQ:
				val=__raw_readl(MAPU_VA_TIMER+Timer8CRE);
				val|=Timer_Mask;
				__raw_writel(val,MAPU_VA_TIMER+Timer8CRE);
				break;
		default:
		       printk("this is not a timer interrupt\n");
			   break;
  }
}

static void timers_irq_ack(unsigned int irq)
{
  switch(irq){
	case	TIMER1_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer1EOI); 
				break;
	case	TIMER2_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer2EOI); 
				break;
	case	TIMER3_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer3EOI); 
				break;
	case	TIMER4_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer4EOI); 
				break;
	case	TIMER5_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer5EOI); 
				break;
	case	TIMER6_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer6EOI); 
				break;
	case	TIMER7_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer7EOI); 
				break;
	case	TIMER8_IRQ:
				__raw_readl(MAPU_VA_TIMER+Timer8EOI); 
				break;
		default:
		       printk("this is not a timer interrupt\n");
			   break;
  }
}

static void mapu_irq_demux_timers(unsigned int irq, struct irq_desc *desc)
{
  unsigned int timer_intsts;
  spin_lock(&desc->lock);
  desc->chip->mask(irq);
  timer_intsts=__raw_readl(MAPU_VA_TIMER+TimersINS)&0xff;
  switch(timer_intsts){
			case 0x01:
				generic_handle_irq(TIMER1_IRQ);
				break;
			case 0x02:
				generic_handle_irq(TIMER2_IRQ);
				break;
			case 0x04:
				generic_handle_irq(TIMER3_IRQ);
				break;
			case 0x08:
				generic_handle_irq(TIMER4_IRQ);
				break;
            case 0x10:
				generic_handle_irq(TIMER5_IRQ);
				break;				
            case 0x20:
				generic_handle_irq(TIMER6_IRQ);
				break;
			case 0x40:
				generic_handle_irq(TIMER7_IRQ);
				break;
			case 0x80:
				generic_handle_irq(TIMER8_IRQ);
				break;
  }
  desc->chip->unmask(irq);
  spin_unlock(&desc->lock); 
}

static struct irq_chip mapu_irq_timer = {
	.name		= "mapu-timers",
	.mask	= timers_irq_disable,
	.unmask	= timers_irq_enable,
	.ack	= timers_irq_ack,
	.enable=timers_irq_enable,
	.disable=timers_irq_disable,
};

#ifdef CONFIG_SERIAL_8250
static struct irq_chip mapu_irq_uart0 = {
        .name           = "mapu-uart0",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        //.ack  = uart_irq_ack,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};

static struct irq_chip mapu_irq_uart1 = {
        .name           = "mapu-uart1",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        //.ack  = uart_irq_ack,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};
#endif

/*lcd interrupt process*/
static void lcd_irq_ack(unsigned int irq)
{
	unsigned int val;
	val=__raw_readl(MAPU_VA_LCD+LCD_INTC)&LCD_INTC_FLAG;
	if(val)
		__raw_writel(LCD_INTC_CLEAR,MAPU_VA_LCD+LCD_INTC);
}

static struct irq_chip mapu_irq_lcd = {
	.name		= "mapu-lcd",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
	.ack	= lcd_irq_ack,
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};

/*sdhci interrupt process*/
static struct irq_chip mapu_irq_sdhci = {
	.name		= "mapu-sdhci",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
	 
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};

/*spi interrupt process*/
static struct irq_chip mapu_irq_spi = {
	.name		= "mapu-spi",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
	 
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};

/*gpu interrupt process*/
static struct irq_chip mapu_irq_gpu_gp = {
        .name           = "mapu-gpu-gp",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};

static struct irq_chip mapu_irq_gpu_gpmmu = {
        .name           = "mapu-gpu-gpmmu",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};  

static struct irq_chip mapu_irq_gpu_pp0 = {
        .name           = "mapu-gpu-pp0",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};

static struct irq_chip mapu_irq_gpu_pp0mmu = {
        .name           = "mapu-gpu-pp0mmu",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};

static struct irq_chip mapu_irq_gpu_pmu = {
        .name           = "mapu-gpu-pmu",
        .mask   = mapu_irq_mask,
        .unmask = mapu_irq_unmask,
        .enable=mapu_irq_enable,
        .disable=mapu_irq_disable,
};

/*GMAC interrupt process*/
static struct irq_chip mapu_irq_gmac = {
	.name		= "mapu-gmac",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
//	.ack	= gmac_irq_ack,
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};

static struct irq_chip mapu_irq_ictl = {
	.name		= "mapu-ictl",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
	//.ack	= mapu_irq_ack,
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};


/*DMAC interrupt process*/
static struct irq_chip mapu_irq_dmac={
	.name		= "mapu-dmac",
	.mask	= mapu_irq_mask,
	.unmask	= mapu_irq_unmask,
//	.ack	= gmac_irq_ack,
	.enable=mapu_irq_enable,
	.disable=mapu_irq_disable,
};

void __init mapu_init_irq(void)
{
  int irqno;
  IRQ_Init(); 
  printk("mapu interrupt system init\n");
#ifdef CONFIG_FIQ
	FIQ_Init();
	printk("frq init\n");
#endif
 
/* register the main interrupts such as uart ,timer ,rtc ,watchdog,gmac ,sdhci ,lcd,camera*/
 /* register the timers interrupts */ 
	 for(irqno=TIMER1_IRQ;irqno<=TIMER8_IRQ;irqno++){
		set_irq_chip(irqno, &mapu_irq_timer);
		set_irq_handler(irqno,handle_level_irq);
		set_irq_flags(irqno, IRQF_VALID);
		}
	set_irq_chip(IRQ_TIMER, &mapu_irq_ictl);
    set_irq_chained_handler(IRQ_TIMER,mapu_irq_demux_timers);
	
#ifdef  CONFIG_SERIAL_MAPU
/* register the uart interrupts */
     for(irqno=UART0_RX;irqno<=UART0_ER;irqno++){
	        set_irq_chip(irqno, &mapu_irq_uart);
	        set_irq_handler(irqno,handle_level_irq);
	        set_irq_flags(irqno, IRQF_VALID);   
        }
		set_irq_chip(IRQ_UART0, &mapu_irq_ictl);
		set_irq_chained_handler(IRQ_UART0,mapu_irq_demux_uart0);
	 for(irqno=UART1_RX;irqno<=UART1_ER;irqno++){
	        set_irq_chip(irqno, &mapu_irq_uart);
	        set_irq_handler(irqno,handle_level_irq);
	        set_irq_flags(irqno, IRQF_VALID);    
        }		
	  set_irq_chip(IRQ_UART1, &mapu_irq_ictl);
      set_irq_chained_handler(IRQ_UART1,mapu_irq_demux_uart1);
#endif
#ifdef CONFIG_SERIAL_8250
        set_irq_chip(IRQ_UART0, &mapu_irq_uart0);
        set_irq_handler(IRQ_UART0,handle_level_irq);
        set_irq_flags(IRQ_UART0, IRQF_VALID);
        set_irq_chip(IRQ_UART1, &mapu_irq_uart1);
        set_irq_handler(IRQ_UART1,handle_level_irq);
        set_irq_flags(IRQ_UART1, IRQF_VALID);

#endif	  	

#ifdef	CONFIG_FB_MAPU	
	/* register the LCD display interrupts */
	set_irq_chip(IRQ_LCD, &mapu_irq_lcd);
	set_irq_handler(IRQ_LCD,handle_level_irq);
	set_irq_flags(IRQ_LCD, IRQF_VALID);
#endif

#ifdef  CONFIG_MMC_SDHCI_MAPU	
	/* register the sdhci/mmc interrupts */
	set_irq_chip(IRQ_SD, &mapu_irq_sdhci);
	set_irq_handler(IRQ_SD,handle_level_irq);
	set_irq_flags(IRQ_SD, IRQF_VALID);
#endif	

#ifdef CONFIG_SPI_MAPU
	/* register the spi interrupts */
	set_irq_chip(IRQ_SPI0, &mapu_irq_spi);
	set_irq_handler(IRQ_SPI0,handle_level_irq);
	set_irq_flags(IRQ_SPI0, IRQF_VALID);
#endif	

#ifdef CONFIG_GPU_DEVICE
	/*register the gpu interrupts*/
		set_irq_chip(IRQ_GP_3D, &mapu_irq_gpu_gp);
        set_irq_handler(IRQ_GP_3D,handle_level_irq);
        set_irq_flags(IRQ_GP_3D, IRQF_VALID);

        set_irq_chip(IRQ_GPMMU_3D, &mapu_irq_gpu_gpmmu);
        set_irq_handler(IRQ_GPMMU_3D,handle_level_irq);
        set_irq_flags(IRQ_GPMMU_3D, IRQF_VALID);

        set_irq_chip(IRQ_PP0_3D, &mapu_irq_gpu_pp0);
        set_irq_handler(IRQ_PP0_3D,handle_level_irq);
        set_irq_flags(IRQ_PP0_3D, IRQF_VALID);

        set_irq_chip(IRQ_PPMMU0_3D, &mapu_irq_gpu_pp0mmu);
        set_irq_handler(IRQ_PPMMU0_3D,handle_level_irq);
        set_irq_flags(IRQ_PPMMU0_3D, IRQF_VALID);

        set_irq_chip(IRQ_PMU_3D, &mapu_irq_gpu_pmu);
        set_irq_handler(IRQ_PMU_3D,handle_level_irq);
        set_irq_flags(IRQ_PMU_3D, IRQF_VALID);
#endif		

#ifdef CONFIG_GMAC_MAPU
	/* register the gmac interrupts */
		set_irq_chip(IRQ_GMAC, &mapu_irq_gmac);
		set_irq_handler(IRQ_GMAC,handle_level_irq);
		set_irq_flags(IRQ_GMAC, IRQF_VALID);
#endif

#ifdef CONFIG_DW_DMAC
	/* register the dmac interrupts */
		set_irq_chip(IRQ_DMA, &mapu_irq_dmac);
		set_irq_handler(IRQ_DMA,handle_level_irq);
		set_irq_flags(IRQ_DMA, IRQF_VALID);
#endif
}
