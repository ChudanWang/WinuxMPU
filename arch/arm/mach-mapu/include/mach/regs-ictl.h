/* 
******************************************************************************* 
* 			    regs  for interrupt controller in mapu  soc
******************************************************************************* 
*/ 

#ifndef __ASM_ARCH_ICTL_H
#define __ASM_ARCH_ITCL_H 


 
#define TRUE 1
#define FALSE 0
#define IRQ_NUM 64
#define FIQ_NUM 8
 
#define IRQ_EN_L        0x00 
#define IRQ_EN_H        0x04 
#define IRQ_MASK_L      0x08 
#define IRQ_MASK_H      0x0c 
#define IRQ_FORCE_L     0x10 
#define IRQ_FORCE_H     0x14 
#define IRQ_RAWSTS_L    0x18 
#define IRQ_RAMSTS_H    0x1c 
#define IRQ_STS_L       0x20 
#define IRQ_STS_H       0x24 
#define IRQ_MASKSTS_L   0x28 
#define IRQ_MASKSTS_H   0x2c 
#define IRQ_FINALSTS_L  0x30 
#define IRQ_FINALSTS_H  0x34 
#define IRQ_VECTOR      0x38 
 
#define IRQ_VCT0  0x40  
#define IRQ_VCT1  0x48 
#define IRQ_VCT2  0x50 
#define IRQ_VCT3  0x58 
#define IRQ_VCT4  0x60 
#define IRQ_VCT5  0x68 
#define IRQ_VCT6  0x70 
#define IRQ_VCT7  0x78 
#define IRQ_VCT8  0x80 
#define IRQ_VCT9  0x88 
#define IRQ_VCT10 0x90 
#define IRQ_VCT11 0x98
#define IRQ_VCT12 0xa0
#define IRQ_VCT13 0xa8
#define IRQ_VCT14 0xb0 
#define IRQ_VCT15 0xb8 

#define FIQ_EN       0xc0
#define FIQ_MASK     0xc4
#define FIQ_FORCE    0xc8
#define FIQ_RAMSTS   0xcc
#define FIQ_STS      0xd0
#define FIQ_FINALSTS 0xd4

/*IRQ SYSTEM PRIORITY LEVEL REGISTER GROUP*/
#define IRQ_PLEVEL       0xd8 
#define IRQ_INTERNAL_PLEVEL       0xdc 

#define IRQ_PL0   0xe8 
#define IRQ_PL1   0xec 
#define IRQ_PL2   0xf0 
#define IRQ_PL3   0xf4 
#define IRQ_PL4   0xf8 
#define IRQ_PL5   0xfc 
#define IRQ_PL6   0x100 
#define IRQ_PL7   0x104 
#define IRQ_PL8   0x108 
#define IRQ_PL9   0x10c 
#define IRQ_PL10  0x110 
#define IRQ_PL11  0x114 
#define IRQ_PL12  0x118 
#define IRQ_PL13  0x11c 
#define IRQ_PL14  0x120 
#define IRQ_PL15  0x124 
#define IRQ_PL16  0x128 
#define IRQ_PL17  0x12c 
#define IRQ_PL18  0x130 
#define IRQ_PL19  0x134 
#define IRQ_PL20  0x138 
#define IRQ_PL21  0x13c 
#define IRQ_PL22  0x140 
#define IRQ_PL23  0x144 
#define IRQ_PL24  0x148 
#define IRQ_PL25  0x14c 
#define IRQ_PL26  0x150 
#define IRQ_PL27  0x154 
#define IRQ_PL28  0x158 
#define IRQ_PL29  0x15c 
#define IRQ_PL30  0x160 
#define IRQ_PL31  0x164 
#define IRQ_PL32  0x168 
#define IRQ_PL33  0x16c 
#define IRQ_PL34  0x170 
#define IRQ_PL35  0x174 
#define IRQ_PL36  0x178 
#define IRQ_PL37  0x17c 
#define IRQ_PL38  0x180 
#define IRQ_PL39  0x184 
#define IRQ_PL40  0x188 
#define IRQ_PL41  0x18c 
#define IRQ_PL42  0x190 
#define IRQ_PL43  0x194 
#define IRQ_PL44  0x198 
#define IRQ_PL45  0x19c 
#define IRQ_PL46  0x1a0 
#define IRQ_PL47  0x1a4 
#define IRQ_PL48  0x1a8 
#define IRQ_PL49  0x1ac 
#define IRQ_PL50  0x1b0 
#define IRQ_PL51  0x1b4 
#define IRQ_PL52  0x1b8 
#define IRQ_PL53  0x1bc 
#define IRQ_PL54  0x1c0 
#define IRQ_PL55  0x1c4 
#define IRQ_PL56  0x1c8 
#define IRQ_PL57  0x1cc 
#define IRQ_PL58  0x1d0 
#define IRQ_PL59  0x1d4 
#define IRQ_PL60  0x1d8 
#define IRQ_PL61  0x1dc 
#define IRQ_PL62  0x1e0 
#define IRQ_PL63  0x1e4 
#define IRQ_VCT_DEF  0x1e8 
#define APB_ICTL_COMP_VERSION  0xe0



#endif
 