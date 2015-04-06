 /* 
******************************************************************************* 
* 			    regs  for timer in mapu  soc
******************************************************************************* 
*/ 
 
 #ifndef _ASM_ARCH_MAPU_TIMER
 #define  _ASM_ARCH_MAPU_TIMER
 
 /*  Timer1  */
 #define Timer1LCT     0x00 
 #define Timer1CVL     0x04 
 #define Timer1CRE    0x08 
 #define Timer1EOI    0x0C
 #define Timer1INS     0x10
 /*  Timer2  */
 #define Timer2LCT    0x14
 #define Timer2CVL    0x18
 #define Timer2CRE    0x1C
 #define Timer2EOI    0x20
 #define Timer2INS    0x24
 /*  Timer3 */
 #define Timer3LCT    0x28
 #define Timer3CVL    0x2C
 #define Timer3CRE    0x30
 #define Timer3EOI    0x34
 #define Timer3INS    0x38
 /*  Timer4  */
 #define Timer4LCT    0x3C
 #define Timer4CVL    0x40
 #define Timer4CRE    0x44
 #define Timer4EOI    0x48
 #define Timer4INS    0x4C
/*  Timer5*/ 
 #define Timer5LCT    0x50
 #define Timer5CVL    0x54
 #define Timer5CRE    0x58
 #define Timer5EOI    0x5C
 #define Timer5INS    0x60
 /*  Timer6  */
 #define Timer6LCT    0x64
 #define Timer6CVL    0x68
 #define Timer6CRE    0x6C
 #define Timer6EOI    0x70
 #define Timer6INS    0x74
 /*  Timer7  */
 #define Timer7LCT    0x78
 #define Timer7CVL    0x7C
 #define Timer7CRE    0x80
 #define Timer7EOI    0x84
 #define Timer7INS    0x88
 /*  Timer8  */
 #define Timer8LCT    0x8C
 #define Timer8CVL    0x90
 #define Timer8CRE    0x94
 #define Timer8EOI    0x98
 #define Timer8INS    0x9C
  
 #define  TimersINS    0xa0
 #define  TimersEOT   0xa4
 #define  TimersRIS   0xa8
 #define  TimersVER   0xa8
  
 #define  Timer_Mask    0x04
 #define  Timer_Mode_Usr  0x02
 #define  Timer_EN       0x01
 #endif