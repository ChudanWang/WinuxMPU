 #ifndef _ASM_MAPU_CPU_H
 #define _ASM_MAPU_CPU_H
 
/* per-cpu initialisation function table. */

struct cpu_table {
	unsigned long	idcode;
	unsigned long	idmask;
	void		(*map_io)(void);
	int			(*init)(void);
	const char	*name;
};
extern void mapu_irq_mask(unsigned int irq) ;

extern void mapu_irq_unmask(unsigned int irq)  ;

extern void mapu_irq_enable(unsigned int irq)  ;

extern void mapu_irq_disable(unsigned int irq)  ;

extern void mapu_fiq_mask(unsigned int irq)  ;
extern void mapu_fiq_unmask(unsigned int irq) ;
extern void mapu_fiq_enable(unsigned int irq) ;
extern void mapu_fiq_disable(unsigned int irq) ;
extern void IRQ_Init() ;
extern void FIQ_Init() ;
 
extern void mapu_init_cpu(struct cpu_table *cpus, unsigned int cputab_size);
 
extern void  mapu_init_irq();//itcl register for system

extern struct sys_timer mapu_timer;//system timer 
 #endif
