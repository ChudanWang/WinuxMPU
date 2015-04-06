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
#include <mach/map-va.h>
#include <mach/irqs.h>
#include <mach/cpu.h>

static int IRQ_Enable(unsigned int intNum) // set the corresponding bit to 1  to enable the interrupt
{   
    unsigned int val;     
	if(intNum>=IRQ_NUM) 
	   return FALSE;
	if(intNum<32) {
	  val=__raw_readl(MAPU_VA_IRQ+IRQ_EN_L);
	  val|= (0x01<<intNum);	
      __raw_writel(val,MAPU_VA_IRQ+IRQ_EN_L) ;
      return TRUE ;	  
	  }
	else {	
	  val=__raw_readl(MAPU_VA_IRQ+IRQ_EN_H);
	  val|= (0x01<<(intNum-32));
	  __raw_writel(val,MAPU_VA_IRQ+IRQ_EN_H) ;
	  return TRUE ;
      }	  
     
} 

static int FIQ_Enable(unsigned int intNum) 
{  
    unsigned int val;
	if(intNum>= FIQ_NUM)
	  return FALSE;
	val=__raw_readl(MAPU_VA_IRQ+FIQ_EN); 
    val|= (0x01<<intNum);
	__raw_writel(val,MAPU_VA_IRQ+FIQ_EN) ;
	return TRUE; 
} 

static int IRQ_Disable(unsigned int intNum)  // clear the corresponding bit to disable the interrupt
{  
   unsigned int val;
   if(intNum>=IRQ_NUM)  
      return FALSE;
   if(intNum<32) {
        val=__raw_readl(MAPU_VA_IRQ+IRQ_EN_L);
        val&=~(0x01<<intNum);
		__raw_writel(val,MAPU_VA_IRQ+IRQ_EN_L) ;
        return TRUE ;	  
		}		
   else {
        val=__raw_readl(MAPU_VA_IRQ+IRQ_EN_H);
        val&=~(0x01<<(intNum-32));
		__raw_writel(val,MAPU_VA_IRQ+IRQ_EN_H);
        return TRUE; 
      }
} 

static int FIQ_Disable(unsigned int intNum) 
{  
   unsigned int val;
   if(intNum>=FIQ_NUM) 
      return FALSE;
	val=__raw_readl(MAPU_VA_IRQ+FIQ_EN);   
    val&=~(0x01<<intNum);
   __raw_writel(val,MAPU_VA_IRQ+FIQ_EN) ;
   return TRUE; 
} 

static int IRQ_Mask(unsigned int intNum) // set the corresponding bit to 1  to enable the interrupt
{    
	unsigned int val; 
    if(intNum>=IRQ_NUM) 
	      return FALSE;
    if(intNum<32) { 
	   val=__raw_readl(MAPU_VA_IRQ+IRQ_MASK_L); 
	   val |= (0x01<<intNum); 
	   __raw_writel(val,MAPU_VA_IRQ+IRQ_MASK_L) ;
	   return TRUE;
      }	   
    else {
	   val=__raw_readl(MAPU_VA_IRQ+IRQ_MASK_H); 
       val |= (0x01<<(intNum-32)); 
	   __raw_writel(val,MAPU_VA_IRQ+IRQ_MASK_H) ;
	   return TRUE;
     }	   
    
} 

 int FIQ_Mask(unsigned int intNum) 
{  
    unsigned int val; 
	if(intNum>=FIQ_NUM) 
	  return FALSE;
	 val=__raw_readl(MAPU_VA_IRQ+FIQ_MASK); 
	 val|= (0x01<<intNum);
	 __raw_writel(val,MAPU_VA_IRQ+FIQ_MASK) ;
    return TRUE; 
} 

static int IRQ_Unmask(unsigned int intNum) // clear the corresponding bit to disable the interrupt
{ 
	unsigned int val; 
    if(intNum>=IRQ_NUM)
		return FALSE;
    if(intNum<32) {
	    val=__raw_readl(MAPU_VA_IRQ+IRQ_MASK_L);
        val&=~(0x01<<intNum); 
        __raw_writel(val,MAPU_VA_IRQ+IRQ_MASK_L) ;		
     }		
    else {
	    val=__raw_readl(MAPU_VA_IRQ+IRQ_MASK_H);
        val&=~(0x01<<(intNum-32));
		__raw_writel(val,MAPU_VA_IRQ+IRQ_MASK_H) ;
		return TRUE;
    }	
}

static int  FIQ_Unmask(unsigned int intNum) 
{ 
    unsigned int val;
	if(intNum>=FIQ_NUM) 
		return FALSE;
	val=__raw_readl(MAPU_VA_IRQ+FIQ_MASK);	
	val&=~(0x01<<intNum);
	__raw_writel(val,MAPU_VA_IRQ+FIQ_MASK) ;
    return TRUE; 
}
/*IRQ SYMBOL EXPORT*/
void mapu_irq_mask(unsigned int irq)
{
	IRQ_Mask(irq) ;
}
EXPORT_SYMBOL(mapu_irq_mask);

void mapu_irq_unmask(unsigned int irq)
{
	IRQ_Unmask(irq);
}
EXPORT_SYMBOL(mapu_irq_unmask);

void mapu_irq_enable(unsigned int irq)
{
	IRQ_Enable(irq);
}
EXPORT_SYMBOL(mapu_irq_enable);

void mapu_irq_disable(unsigned int irq)
{
	IRQ_Disable(irq);
}
EXPORT_SYMBOL(mapu_irq_disable); 

void IRQ_Init() 
{ 
    __raw_writel(0x0,MAPU_VA_IRQ+IRQ_EN_L) ;  // disable all IRQ
    __raw_writel(0x0,MAPU_VA_IRQ+IRQ_EN_H)  ; 
	__raw_writel(0x0,MAPU_VA_IRQ+IRQ_MASK_L);  // unmask all IRQ
	__raw_writel(0x0,MAPU_VA_IRQ+IRQ_MASK_H);
    __raw_writel(0x0,MAPU_VA_IRQ+IRQ_FORCE_L); // clear all force IRQ 
    __raw_writel(0x0,MAPU_VA_IRQ+IRQ_FORCE_H);  
	 
} 
EXPORT_SYMBOL(IRQ_Init);
 
/*FRQ SYMBOL EXPORT*/ 
void mapu_fiq_mask(unsigned int irq)
{
	FIQ_Mask(irq) ;
}
EXPORT_SYMBOL(mapu_fiq_mask);

void mapu_fiq_unmask(unsigned int irq)
{
	FIQ_Unmask(irq);
}
EXPORT_SYMBOL(mapu_fiq_unmask);

void mapu_fiq_enable(unsigned int irq)
{
	FIQ_Enable(irq);
}
EXPORT_SYMBOL(mapu_fiq_enable); 

void mapu_fiq_disable(unsigned int irq)
{
	FIQ_Disable(irq);
}
EXPORT_SYMBOL(mapu_fiq_disable);  

void FIQ_Init()
{
    __raw_writel(0x0,MAPU_VA_IRQ+FIQ_EN);            // disable all FIQ
    __raw_writel(0x0,MAPU_VA_IRQ+FIQ_MASK);          // unmask all FIQ
    __raw_writel(0x0,MAPU_VA_IRQ+FIQ_FORCE);         // clear all force FIQ
     
}
EXPORT_SYMBOL(FIQ_Init);