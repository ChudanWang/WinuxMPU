/* linux/arch/arm/mach-mapu/include/mach/regs-lcd.h
 *
 * mapu - GMAC register definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_REGS_GMAC_H
#define __ASM_ARCH_REGS_GMAC_H __FILE__

#include <mach/map.h>
/*¹ØÓÚPHYµÄ*/
enum GMACPhyBase
{
	PHY0  = 0,			//The device can support 32 phys, but we use first phy only
	PHY1  = 1,
	PHY31 = 31,
};
#define DEFAULT_PHY_BASE PHY0		//We use First Phy 
/*MACºÍDMA»ùµØÖ·*/
#define MACBASE 0x0000			// The Mac Base address offset is 0x0000
#define DMABASE 0x1000			// Dma base address starts with an offset 0x1000

#ifdef AVB_SUPPORT
#define DMABASE_CH0 DMABASE     // DMA base address for Channel 0
#define DMABASE_CH1 0x1100      // DMA base address for Channel 1
#define DMABASE_CH2 0x1200      // DMA base address for Channel 2
#define ETHERNET_HEADER_AVB     18	//6 byte Dest addr, 6 byte Src addr, 2 byte length/type
#endif
/*This is the IP's phy address. This is unique address for every MAC in the universe*/
/*MACµØÖ·*/
#define DEFAULT_MAC_ADDRESS {0x00, 0x55, 0x7B, 0xB5, 0x7D, 0xF7}

/* Below is "88E1011/88E1011S Integrated 10/100/1000 Gigabit Ethernet Transceiver" 
 * Register and their layouts. This Phy has been used in the Dot Aster GMAC Phy daughter.
 * Since the Phy register map is standard, this map hardly changes to a different Ppy
 */
 /*PHY¼Ä´æÆ÷Æ«ÒÆµØÖ·*/
	enum MiiRegisters
	{
	  PHY_CONTROL_REG			= 0x0000,		/*Control Register*/
	  PHY_STATUS_REG			= 0x0001,		/*Status Register */
	  PHY_ID_HI_REG 			= 0x0002,		/*PHY Identifier High Register*/
	  PHY_ID_LOW_REG			= 0x0003,		/*PHY Identifier High Register*/
	  PHY_AN_ADV_REG			= 0x0004,		/*Auto-Negotiation Advertisement Register*/
	  PHY_LNK_PART_ABl_REG		= 0x0005,		/*Link Partner Ability Register (Base Page)*/
	  PHY_AN_EXP_REG			= 0x0006,		/*Auto-Negotiation Expansion Register*/
	  PHY_AN_NXT_PAGE_TX_REG	= 0x0007,		/*Next Page Transmit Register*/
	  PHY_LNK_PART_NXT_PAGE_REG = 0x0008,		/*Link Partner Next Page Register*/
	  PHY_1000BT_CTRL_REG		= 0x0009,		/*1000BASE-T Control Register*/
	  PHY_1000BT_STATUS_REG 	= 0x000a,		/*1000BASE-T Status Register*/
	  PHY_SPECIFIC_CTRL_REG 	= 0x0010,		/*Phy specific control register*/
	  PHY_SPECIFIC_STATUS_REG	= 0x0011,		/*Phy specific status register*/
	  PHY_INTERRUPT_ENABLE_REG	= 0x0012,		/*Phy interrupt enable register*/
	  PHY_INTERRUPT_STATUS_REG	= 0x0013,		/*Phy interrupt status register*/
	  PHY_EXT_PHY_SPC_CTRL		= 0x0014,		/*Extended Phy specific control*/
	  PHY_RX_ERR_COUNTER		= 0x0015,		/*Receive Error Counter*/
	  PHY_EXT_ADDR_CBL_DIAG 	= 0x0016,		/*Extended address for cable diagnostic register*/
	  PHY_LED_CONTROL		= 0x0018,		/*LED Control*/ 		
	  PHY_MAN_LED_OVERIDE		= 0x0019,		/*Manual LED override register*/
	  PHY_EXT_PHY_SPC_CTRL2 	= 0x001a,		/*Extended Phy specific control 2*/
	  PHY_EXT_PHY_SPC_STATUS	= 0x001b,		/*Extended Phy specific status*/
	  PHY_CBL_DIAG_REG		= 0x001c,		/*Cable diagnostic registers*/
	};

/* This is Control register layout. Control register is of 16 bit wide.*/
/*»ù±¾Ä£Ê½¿ØÖÆ¼Ä´æÆ÷0x00*/  
enum Mii_GEN_CTRL
{		                      /*	Description	               bits	       R/W	default value  */
	Mii_reset		     = 0x8000, 
  	Mii_Speed_10   		 = 0x0000,   /* 10   Mbps                    6:13         	RW                      */
	Mii_Speed_100   	 = 0x2000,   /* 100  Mbps                    6:13         	RW                      */
	Mii_Speed_1000 		 = 0x0040,   /* 1000 Mbit/s                  6:13         	RW                      */

	Mii_Duplex     		 = 0x0100,   /* Full Duplex mode             8               	RW                      */
 	
	Mii_Manual_Master_Config = 0x0800,   /* Manual Master Config         11              	RW			*/
	
	Mii_Loopback   		 = 0x4000,   /* Enable Loop back             14              	RW                      */
	Mii_NoLoopback 		 = 0x0000,   /* Enable Loop back             14              	RW                      */
};
/*»ù±¾Ä£Ê½×´Ì¬¼Ä´æÆ÷0x01*/
enum Mii_Phy_Status
{
	Mii_phy_status_speed_10	 	= 0x0000,
	Mii_phy_status_speed_100  	= 0x4000,
	Mii_phy_status_speed_1000	= 0x8000,
	
	Mii_phy_status_full_duplex	= 0x2000,
	Mii_phy_status_half_duplex	= 0x0000,
	
	Mii_phy_status_link_up		= 0x0400,
};

/* This is Status register layout. Status register is of 16 bit wide.*/
/*»ù±¾Ä£Ê½×´Ì¬¼Ä´æÆ÷0x01*/
enum Mii_GEN_STATUS
{
	Mii_AutoNegCmplt     = 0x0020,   /* Autonegotiation completed      5              RW                   */
	Mii_Link             = 0x0004,   /* Link status                    2              RW                   */
};
enum Mii_Link_Status
{
	LINKDOWN	= 0,
	LINKUP		= 1,
};
enum Mii_Duplex_Mode
{
	HALFDUPLEX = 1,
	FULLDUPLEX = 2,
};
enum Mii_Link_Speed
{
	SPEED10     = 1,
	SPEED100    = 2,
	SPEED1000   = 3,
};
enum Mii_Loop_Back
{
	NOLOOPBACK  = 0,
	LOOPBACK    = 1,
};
/**********************************************************
 * GMAC registers Map
 * For Pci based system address is BARx + GmacRegisterBase
 * For any other system translation is done accordingly
 **********************************************************/
 /*GMAC¼Ä´æÆ÷Æ«ÒÆµØÖ·*/
	enum GmacRegisters				
	{
	  GmacConfig		  = 0x0000,    /* Mac config Register						*/
	  GmacFrameFilter	  = 0x0004,    /* Mac frame filtering controls				*/
	  GmacHashHigh		  = 0x0008,    /* Multi-cast hash table high				*/
	  GmacHashLow		  = 0x000C,    /* Multi-cast hash table low 				*/
	  GmacGmiiAddr		  = 0x0010,    /* GMII address Register(ext. Phy)			*/
	  GmacGmiiData		  = 0x0014,    /* GMII data Register(ext. Phy)				*/
	  GmacFlowControl	  = 0x0018,    /* Flow control Register 					*/
	  GmacVlan			  = 0x001C,    /* VLAN tag Register (IEEE 802.1Q)			*/
	  
	  GmacVersion		  = 0x0020,    /* GMAC Core Version Register				*/ 
	  GmacWakeupAddr	  = 0x0028,    /* GMAC wake-up frame filter adrress reg 	*/ 
	  GmacPmtCtrlStatus 	  = 0x002C,    /* PMT control and status register			*/ 
	 
  #ifdef LPI_SUPPORT
	  GmacLPICtrlSts	  = 0x0030,    /* LPI (low power idle) Control and Status Register			*/
	  GmacLPITimerCtrl	  = 0x0034,    /* LPI timer control register			   */
  #endif
	  
	  GmacInterruptStatus	  = 0x0038,    /* Mac Interrupt ststus register 	   */  
	  GmacInterruptMask 	  = 0x003C,    /* Mac Interrupt Mask register		   */  
	 
	  GmacAddr0High 	  = 0x0040,    /* Mac address0 high Register				*/
	  GmacAddr0Low		  = 0x0044,    /* Mac address0 low Register 				*/
	  GmacAddr1High 	  = 0x0048,    /* Mac address1 high Register				*/
	  GmacAddr1Low		  = 0x004C,    /* Mac address1 low Register 				*/
	  GmacAddr2High 	  = 0x0050,    /* Mac address2 high Register				*/
	  GmacAddr2Low		  = 0x0054,    /* Mac address2 low Register 				*/
	  GmacAddr3High 	  = 0x0058,    /* Mac address3 high Register				*/
	  GmacAddr3Low		  = 0x005C,    /* Mac address3 low Register 				*/
	  GmacAddr4High 	  = 0x0060,    /* Mac address4 high Register				*/
	  GmacAddr4Low		  = 0x0064,    /* Mac address4 low Register 				*/
	  GmacAddr5High 	  = 0x0068,    /* Mac address5 high Register				*/
	  GmacAddr5Low		  = 0x006C,    /* Mac address5 low Register 				*/
	  GmacAddr6High 	  = 0x0070,    /* Mac address6 high Register				*/
	  GmacAddr6Low		  = 0x0074,    /* Mac address6 low Register 				*/
	  GmacAddr7High 	  = 0x0078,    /* Mac address7 high Register				*/
	  GmacAddr7Low		  = 0x007C,    /* Mac address7 low Register 				*/
	  GmacAddr8High 	  = 0x0080,    /* Mac address8 high Register				*/
	  GmacAddr8Low		  = 0x0084,    /* Mac address8 low Register 				*/
	  GmacAddr9High 	  = 0x0088,    /* Mac address9 high Register				*/
	  GmacAddr9Low			  = 0x008C,    /* Mac address9 low Register 				*/
	  GmacAddr10High		  = 0x0090,    /* Mac address10 high Register				*/
	  GmacAddr10Low 	  = 0x0094,    /* Mac address10 low Register				*/
	  GmacAddr11High	  = 0x0098,    /* Mac address11 high Register				*/
	  GmacAddr11Low 	  = 0x009C,    /* Mac address11 low Register				*/
	  GmacAddr12High	  = 0x00A0,    /* Mac address12 high Register				*/
	  GmacAddr12Low 		  = 0x00A4,    /* Mac address12 low Register				*/
	  GmacAddr13High	  = 0x00A8,    /* Mac address13 high Register				*/
	  GmacAddr13Low 	  = 0x00AC,    /* Mac address13 low Register				*/
	  GmacAddr14High	  = 0x00B0,    /* Mac address14 high Register				*/
	  GmacAddr14Low 		  = 0x00B4,    /* Mac address14 low Register				*/
	  GmacAddr15High		  = 0x00B8,    /* Mac address15 high Register				*/
	  GmacAddr15Low 	  = 0x00BC,    /* Mac address15 low Register				*/
	
	  /*Time Stamp Register Map*/
	  GmacTSControl 		  = 0x0700,  /* Controls the Timestamp update logic 						: only when IEEE 1588 time stamping is enabled in corekit			 */
	
	  GmacTSSubSecIncr		  = 0x0704,  /* 8 bit value by which sub second register is incremented 	: only when IEEE 1588 time stamping without external timestamp input */
	
	  GmacTSHigh			  = 0x0708,  /* 32 bit seconds(MS)											: only when IEEE 1588 time stamping without external timestamp input */
	  GmacTSLow 			  = 0x070C,  /* 32 bit nano seconds(MS) 									: only when IEEE 1588 time stamping without external timestamp input */
	  
	  GmacTSHighUpdate		  = 0x0710,  /* 32 bit seconds(MS) to be written/added/subtracted			: only when IEEE 1588 time stamping without external timestamp input */
	  GmacTSLowUpdate		  = 0x0714,  /* 32 bit nano seconds(MS) to be writeen/added/subtracted		: only when IEEE 1588 time stamping without external timestamp input */
	  
	  GmacTSAddend			  = 0x0718,  /* Used by Software to readjust the clock frequency linearly	: only when IEEE 1588 time stamping without external timestamp input */
	  
	  GmacTSTargetTimeHigh	  = 0x071C,  /* 32 bit seconds(MS) to be compared with system time			: only when IEEE 1588 time stamping without external timestamp input */
	  GmacTSTargetTimeLow	  = 0x0720,  /* 32 bit nano seconds(MS) to be compared with system time 	: only when IEEE 1588 time stamping without external timestamp input */
	
	  GmacTSHighWord		  = 0x0724,  /* Time Stamp Higher Word Register (Version 2 only); only lower 16 bits are valid													 */
	  //GmacTSHighWordUpdate	= 0x072C,  /* Time Stamp Higher Word Update Register (Version 2 only); only lower 16 bits are valid 										   */
	  
	  GmacTSStatus			  = 0x0728,  /* Time Stamp Status Register																										 */
  #ifdef AVB_SUPPORT
	  GmacAvMacCtrl 		   = 0x0738,  /* AV mac control Register  */
  #endif
	  
	};
/**********************************************************
 * GMAC Network interface registers
 * This explains the Register's Layout
 
 * FES is Read only by default and is enabled only when Tx 
 * Config Parameter is enabled for RGMII/SGMII interface
 * during CoreKit Config.
 
 * DM is Read only with value 1'b1 in Full duplex only Config
 **********************************************************/
 /* GmacConfig              = 0x0000,    Mac config Register  Layout */
/*gmacÅäÖÃ¼Ä´æÆ÷*/
 enum GmacConfigReg      
{ 
                                           /* Bit description                      Bits         R/W   Reset value  */
  GmacWatchdog		   = 0x00800000,
  GmacWatchdogDisable      = 0x00800000,     /* (WD)Disable watchdog timer on Rx      23           RW                */
  GmacWatchdogEnable       = 0x00000000,     /* Enable watchdog timer                                        0       */

  GmacJabber		   = 0x00400000,
  GmacJabberDisable        = 0x00400000,     /* (JD)Disable jabber timer on Tx        22           RW                */
  GmacJabberEnable         = 0x00000000,     /* Enable jabber timer                                          0       */

  GmacFrameBurst           = 0x00200000,
  GmacFrameBurstEnable     = 0x00200000,     /* (BE)Enable frame bursting during Tx   21           RW                */
  GmacFrameBurstDisable    = 0x00000000,     /* Disable frame bursting                                       0       */
  
  GmacJumboFrame           = 0x00100000,
  GmacJumboFrameEnable     = 0x00100000,     /* (JE)Enable jumbo frame for Tx         20           RW                */
  GmacJumboFrameDisable    = 0x00000000,     /* Disable jumbo frame                                          0       */

  GmacInterFrameGap7       = 0x000E0000,     /* (IFG) Config7 - 40 bit times          19:17        RW                */
  GmacInterFrameGap6       = 0x000C0000,     /* (IFG) Config6 - 48 bit times                                         */
  GmacInterFrameGap5       = 0x000A0000,     /* (IFG) Config5 - 56 bit times                                         */
  GmacInterFrameGap4       = 0x00080000,     /* (IFG) Config4 - 64 bit times                                         */
  GmacInterFrameGap3       = 0x00040000,     /* (IFG) Config3 - 72 bit times                                         */
  GmacInterFrameGap2       = 0x00020000,     /* (IFG) Config2 - 80 bit times                                         */
  GmacInterFrameGap1       = 0x00010000,     /* (IFG) Config1 - 88 bit times                                         */
  GmacInterFrameGap0       = 0x00000000,     /* (IFG) Config0 - 96 bit times                                 000     */
 
  GmacDisableCrs	   = 0x00010000, 
  GmacMiiGmii		   = 0x00008000,
  GmacSelectMii            = 0x00008000,     /* (PS)Port Select-MII mode              15           RW                */
  GmacSelectGmii           = 0x00000000,     /* GMII mode                                                    0       */

  GmacFESpeed100           = 0x00004000,     /*(FES)Fast Ethernet speed 100Mbps       14           RW                */ 
  GmacFESpeed10            = 0x00000000,     /* 10Mbps                                                       0       */ 

  GmacRxOwn		   = 0x00002000, 
  GmacDisableRxOwn         = 0x00002000,     /* (DO)Disable receive own packets       13           RW                */
  GmacEnableRxOwn          = 0x00000000,     /* Enable receive own packets                                   0       */
  
  GmacLoopback		   = 0x00001000,
  GmacLoopbackOn           = 0x00001000,     /* (LM)Loopback mode for GMII/MII        12           RW                */
  GmacLoopbackOff          = 0x00000000,     /* Normal mode                                                  0       */

  GmacDuplex		   = 0x00000800,
  GmacFullDuplex           = 0x00000800,     /* (DM)Full duplex mode                  11           RW                */
  GmacHalfDuplex           = 0x00000000,     /* Half duplex mode                                             0       */

  GmacRxIpcOffload	   = 0x00000400,     /*IPC checksum offload		      10           RW        0       */

  GmacRetry		   = 0x00000200,
  GmacRetryDisable         = 0x00000200,     /* (DR)Disable Retry                      9           RW                */
  GmacRetryEnable          = 0x00000000,     /* Enable retransmission as per BL                              0       */

  GmacLinkUp               = 0x00000100,     /* (LUD)Link UP                           8           RW                */ 
  GmacLinkDown             = 0x00000100,     /* Link Down                                                    0       */ 
  
  GmacPadCrcStrip	   = 0x00000080,
  GmacPadCrcStripEnable    = 0x00000080,     /* (ACS) Automatic Pad/Crc strip enable   7           RW                */
  GmacPadCrcStripDisable   = 0x00000000,     /* Automatic Pad/Crc stripping disable                          0       */
  
  GmacBackoffLimit	   = 0x00000060,
  GmacBackoffLimit3        = 0x00000060,     /* (BL)Back-off limit in HD mode          6:5         RW                */
  GmacBackoffLimit2        = 0x00000040,     /*                                                                      */
  GmacBackoffLimit1        = 0x00000020,     /*                                                                      */
  GmacBackoffLimit0        = 0x00000000,     /*                                                              00      */

  GmacDeferralCheck	   = 0x00000010,
  GmacDeferralCheckEnable  = 0x00000010,     /* (DC)Deferral check enable in HD mode   4           RW                */
  GmacDeferralCheckDisable = 0x00000000,     /* Deferral check disable                                       0       */
   
  GmacTx		   = 0x00000008,
  GmacTxEnable             = 0x00000008,     /* (TE)Transmitter enable                 3           RW                */
  GmacTxDisable            = 0x00000000,     /* Transmitter disable                                          0       */

  GmacRx		   = 0x00000004,
  GmacRxEnable             = 0x00000004,     /* (RE)Receiver enable                    2           RW                */
  GmacRxDisable            = 0x00000000,     /* Receiver disable                                             0       */
};

/* GmacFrameFilter    = 0x0004,     Mac frame filtering controls Register Layout*/
/*gmacÖ¡ÂË²¨¼Ä´æÆ÷*/
enum GmacFrameFilterReg 
{
  GmacFilter		   = 0x80000000,
  GmacFilterOff            = 0x80000000,     /* (RA)Receive all incoming packets       31         RW                 */
  GmacFilterOn             = 0x00000000,     /* Receive filtered packets only                                0       */

  GmacHashPerfectFilter	   = 0x00000400,     /*Hash or Perfect Filter enable           10         RW         0       */

  GmacSrcAddrFilter	   = 0x00000200,
  GmacSrcAddrFilterEnable  = 0x00000200,     /* (SAF)Source Address Filter enable       9         RW                 */
  GmacSrcAddrFilterDisable = 0x00000000,     /*                                                              0       */

  GmacSrcInvaAddrFilter    = 0x00000100,
  GmacSrcInvAddrFilterEn   = 0x00000100,     /* (SAIF)Inv Src Addr Filter enable        8         RW                 */
  GmacSrcInvAddrFilterDis  = 0x00000000,     /*                                                              0       */

  GmacPassControl	   = 0x000000C0,
  GmacPassControl3         = 0x000000C0,     /* (PCS)Forwards ctrl frms that pass AF    7:6       RW                 */
  GmacPassControl2         = 0x00000080,     /* Forwards all control frames                                          */
  GmacPassControl1         = 0x00000040,     /* Does not pass control frames                                         */
  GmacPassControl0         = 0x00000000,     /* Does not pass control frames                                 00      */

  GmacBroadcast		   = 0x00000020,
  GmacBroadcastDisable     = 0x00000020,     /* (DBF)Disable Rx of broadcast frames     5         RW                 */
  GmacBroadcastEnable      = 0x00000000,     /* Enable broadcast frames                                      0       */

  GmacMulticastFilter      = 0x00000010,
  GmacMulticastFilterOff   = 0x00000010,     /* (PM) Pass all multicast packets         4         RW                 */
  GmacMulticastFilterOn    = 0x00000000,     /* Pass filtered multicast packets                              0       */

  GmacDestAddrFilter       = 0x00000008,
  GmacDestAddrFilterInv    = 0x00000008,     /* (DAIF)Inverse filtering for DA          3         RW                 */
  GmacDestAddrFilterNor    = 0x00000000,     /* Normal filtering for DA                                      0       */

  GmacMcastHashFilter      = 0x00000004,
  GmacMcastHashFilterOn    = 0x00000004,     /* (HMC)perfom multicast hash filtering    2         RW                 */
  GmacMcastHashFilterOff   = 0x00000000,     /* perfect filtering only                                       0       */

  GmacUcastHashFilter      = 0x00000002,
  GmacUcastHashFilterOn    = 0x00000002,     /* (HUC)Unicast Hash filtering only        1         RW                 */
  GmacUcastHashFilterOff   = 0x00000000,     /* perfect filtering only                                       0       */

  GmacPromiscuousMode      = 0x00000001,
  GmacPromiscuousModeOn    = 0x00000001,     /* Receive all frames                      0         RW                 */
  GmacPromiscuousModeOff   = 0x00000000,     /* Receive filtered packets only                                0       */
};

/*GmacGmiiAddr             = 0x0010,    GMII address Register(ext. Phy) Layout          */
/*gmac GMIIµØÖ·¼Ä´æÆ÷*/
enum GmacGmiiAddrReg      
{
  GmiiDevMask              = 0x0000F800,     /* (PA)GMII device address                 15:11     RW         0x00    */
  GmiiDevShift             = 11,

  GmiiRegMask              = 0x000007C0,     /* (GR)GMII register in selected Phy       10:6      RW         0x00    */
  GmiiRegShift             = 6,
  
  GmiiCsrClkMask	   = 0x0000001C,     /*CSR Clock bit Mask			 4:2			     */
  GmiiCsrClk5              = 0x00000014,     /* (CR)CSR Clock Range     250-300 MHz      4:2      RW         000     */
  GmiiCsrClk4              = 0x00000010,     /*                         150-250 MHz                                  */
  GmiiCsrClk3              = 0x0000000C,     /*                         35-60 MHz                                    */
  GmiiCsrClk2              = 0x00000008,     /*                         20-35 MHz                                    */
  GmiiCsrClk1              = 0x00000004,     /*                         100-150 MHz                                  */
  GmiiCsrClk0              = 0x00000000,     /*                         60-100 MHz                                   */

  GmiiWrite                = 0x00000002,     /* (GW)Write to register                      1      RW                 */
  GmiiRead                 = 0x00000000,     /* Read from register                                            0      */

  GmiiBusy                 = 0x00000001,     /* (GB)GMII interface is busy                 0      RW          0      */
};

/* GmacGmiiData            = 0x0014,    GMII data Register(ext. Phy) Layout             */
/*gmac gmiiÊý¾Ý¼Ä´æÆ÷*/
enum GmacGmiiDataReg      
{
  GmiiDataMask             = 0x0000FFFF,     /* (GD)GMII Data                             15:0    RW         0x0000  */
};

/*GmacFlowControl    = 0x0018,    Flow control Register   Layout                  */
/*gmacÁ÷¿ØÖÆ¼Ä´æÆ÷*/
enum GmacFlowControlReg  
{                                          
  GmacPauseTimeMask        = 0xFFFF0000,     /* (PT) PAUSE TIME field in the control frame  31:16   RW       0x0000  */
  GmacPauseTimeShift       = 16,
  
  GmacPauseLowThresh	   = 0x00000030,
  GmacPauseLowThresh3      = 0x00000030,     /* (PLT)thresh for pause tmr 256 slot time      5:4    RW               */
  GmacPauseLowThresh2      = 0x00000020,     /*                           144 slot time                              */
  GmacPauseLowThresh1      = 0x00000010,     /*                            28 slot time                              */
  GmacPauseLowThresh0      = 0x00000000,     /*                             4 slot time                       000    */

  GmacUnicastPauseFrame    = 0x00000008,
  GmacUnicastPauseFrameOn  = 0x00000008,     /* (UP)Detect pause frame with unicast addr.     3    RW                */
  GmacUnicastPauseFrameOff = 0x00000000,     /* Detect only pause frame with multicast addr.                   0     */

  GmacRxFlowControl	   = 0x00000004,
  GmacRxFlowControlEnable  = 0x00000004,     /* (RFE)Enable Rx flow control                   2    RW                */
  GmacRxFlowControlDisable = 0x00000000,     /* Disable Rx flow control                                        0     */

  GmacTxFlowControl   	   = 0x00000002,
  GmacTxFlowControlEnable  = 0x00000002,     /* (TFE)Enable Tx flow control                   1    RW                */
  GmacTxFlowControlDisable = 0x00000000,     /* Disable flow control                                           0     */

  GmacFlowControlBackPressure= 0x00000001,
  GmacSendPauseFrame       = 0x00000001,     /* (FCB/PBA)send pause frm/Apply back pressure   0    RW          0     */
};

/*  GmacInterruptStatus	  = 0x0038,     Mac Interrupt ststus register	       */  
/*gmacÖÐ¶Ï×´Ì¬¼Ä´æÆ÷*/
enum GmacInterruptStatusBitDefinition
{
  GmacTSIntSts		   = 0x00000200,    /* set if int generated due to TS (Read Time Stamp Status Register to know details)*/
  GmacMmcRxChksumOffload   = 0x00000080,    /* set if int generated in MMC RX CHECKSUM OFFLOAD int register	                  */ 
  GmacMmcTxIntSts	   = 0x00000040,    /* set if int generated in MMC TX Int register			   */
  GmacMmcRxIntSts	   = 0x00000020,    /* set if int generated in MMC RX Int register 			   */
  GmacMmcIntSts		   = 0x00000010,    /* set if any of the above bit [7:5] is set			   */
  GmacPmtIntSts		   = 0x00000008,    /* set whenver magic pkt/wake-on-lan frame is received		   */
  GmacPcsAnComplete	   = 0x00000004,    /* set when AN is complete in TBI/RTBI/SGMIII phy interface        */
  GmacPcsLnkStsChange	   = 0x00000002,    /* set if any lnk status change in TBI/RTBI/SGMII interface        */
  GmacRgmiiIntSts	   = 0x00000001,    /* set if any change in lnk status of RGMII interface		   */

};

/*  GmacInterruptMask       = 0x003C,     Mac Interrupt Mask register	       */  
/*gmacÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷*/
enum GmacInterruptMaskBitDefinition
{
  GmacTSIntMask		   = 0x00000200,    /* when set disables the time stamp interrupt generation            */
  GmacPmtIntMask	   = 0x00000008,    /* when set Disables the assertion of PMT interrupt     	    	*/
  GmacPcsAnIntMask	   = 0x00000004,    /* When set disables the assertion of PCS AN complete interrupt	      	*/
  GmacPcsLnkStsIntMask	   = 0x00000002,    /* when set disables the assertion of PCS lnk status change interrupt	*/
  GmacRgmiiIntMask	   = 0x00000001,    /* when set disables the assertion of RGMII int 			*/
};

/**********************************************************
 * GMAC DMA registers
 * For Pci based system address is BARx + GmaDmaBase
 * For any other system translation is done accordingly
 **********************************************************/
 /*DMA¼Ä´æÆ÷Æ«ÒÆµØÖ·*/
	enum DmaRegisters			  
	{
	  DmaBusMode		= 0x0000,	 /* CSR0 - Bus Mode Register						  */
	  DmaTxPollDemand	= 0x0004,	 /* CSR1 - Transmit Poll Demand Register			  */
	  DmaRxPollDemand	= 0x0008,	 /* CSR2 - Receive Poll Demand Register 			  */
	  DmaRxBaseAddr 	= 0x000C,	 /* CSR3 - Receive Descriptor list base address 	  */
	  DmaTxBaseAddr 	= 0x0010,	 /* CSR4 - Transmit Descriptor list base address	  */
	  DmaStatus 		= 0x0014,	 /* CSR5 - Dma status Register						  */
	  DmaControl		= 0x0018,	 /* CSR6 - Dma Operation Mode Register				  */
	  DmaInterrupt		= 0x001C,	 /* CSR7 - Interrupt enable 						  */
	  DmaMissedFr		= 0x0020,	 /* CSR8 - Missed Frame & Buffer overflow Counter	  */
	  DmaTxCurrDesc 	= 0x0048,	 /* 	 - Current host Tx Desc Register			  */ 
	  DmaRxCurrDesc 	= 0x004C,	 /* 	 - Current host Rx Desc Register			  */ 
	  DmaTxCurrAddr 	= 0x0050,	 /* CSR20 - Current host transmit buffer address	  */
	  DmaRxCurrAddr 	= 0x0054,	 /* CSR21 - Current host receive buffer address 	  */
	
  #ifdef AVB_SUPPORT
	  HwFeature 		= 0x0058,	 /* Hardware Feature Register						  */
	  
	  DmaSlotFnCtrlSts	= 0x0030,	 /* Slot function control and status register		  */
	  
	  DmaChannelCtrl	= 0x0060,	 /* Channel Control register only for Channel1 and Channel2 */
	  DmaChannelAvSts	= 0x0064,	 /* Channel Status register only for Channel1 and  Channel2 */ 
	  IdleSlopeCredit	= 0x0068,	 /* Idle slope credit register								*/
	  SendSlopeCredit	= 0x006C,	 /* Send slope credit register								*/
	  HighCredit		= 0x0070,	 /* High Credit register									*/
	  LoCredit			= 0x0074,	 /* Lo Credit Register										*/	
  #endif
	  
	};

/**********************************************************
 * DMA Engine registers Layout
 **********************************************************/

/*DmaBusMode               = 0x0000,    CSR0 - Bus Mode */
/*dma×ÜÏßÄ£Ê½¼Ä´æÆ÷*/
enum DmaBusModeReg         
{           
                                          /* Bit description                                Bits     R/W   Reset value */
  #ifdef AVB_SUPPORT
  DmaChannelPrioWt       = 0x30000000,    /* Channel priority weight mask                     29:28    RW       0       */
  DmaChannelPrio1        = 0x00000000,    /* Channel priority weight 1                        29:28    RW       0       */
  DmaChannelPrio2        = 0x10000000,    /* Channel priority weight 2                        29:28    RW       0       */
  DmaChannelPrio3        = 0x20000000,    /* Channel priority weight 3                        29:28    RW       0       */
  DmaChannelPrio4        = 0x30000000,    /* Channel priority weight 4                        29:28    RW       0       */

  DmaTxRxPrio            = 0x08000000,    /* When set indicates Tx Dma has more priority       27      RW        0       */

  DmaPriorityRatio11     = 0x00000000,   /* (PR)TX:RX DMA priority ratio 1:1                15:14   RW        00      */ 
  DmaPriorityRatio21     = 0x00004000,   /* (PR)TX:RX DMA priority ratio 2:1                                          */ 
  DmaPriorityRatio31     = 0x00008000,   /* (PR)TX:RX DMA priority ratio 3:1                                          */ 
  DmaPriorityRatio41     = 0x0000C000,   /* (PR)TX:RX DMA priority ratio 4:1                                          */ 
  
  DmaArbitration         = 0x00000002,    /* Dma Arbitration decides whether strict prio or RR  1      RW       0       */
  DmaArbitrationStrict   = 0x00000002,    /* Dma Arbitration decides whether strict prio or RR  1      RW       0       */
  DmaArbitrationRR       = 0x00000000,    /* Dma Arbitration decides whether strict prio or RR  0      RW       0       */        
  #endif


  DmaFixedBurstEnable     = 0x00010000,   /* (FB)Fixed Burst SINGLE, INCR4, INCR8 or INCR16   16     RW                */
  DmaFixedBurstDisable    = 0x00000000,   /*             SINGLE, INCR                                          0       */

  DmaTxPriorityRatio11    = 0x00000000,   /* (PR)TX:RX DMA priority ratio 1:1                15:14   RW        00      */ 
  DmaTxPriorityRatio21    = 0x00004000,   /* (PR)TX:RX DMA priority ratio 2:1                                          */ 
  DmaTxPriorityRatio31    = 0x00008000,   /* (PR)TX:RX DMA priority ratio 3:1                                          */ 
  DmaTxPriorityRatio41    = 0x0000C000,   /* (PR)TX:RX DMA priority ratio 4:1                                          */ 
  
  DmaBurstLengthx8        = 0x01000000,   /* When set mutiplies the PBL by 8                  24      RW        0      */ 
  
  DmaBurstLength256       = 0x01002000,   /*(DmaBurstLengthx8 | DmaBurstLength32) = 256      [24]:13:8                 */  
  DmaBurstLength128       = 0x01001000,   /*(DmaBurstLengthx8 | DmaBurstLength16) = 128      [24]:13:8                 */
  DmaBurstLength64        = 0x01000800,   /*(DmaBurstLengthx8 | DmaBurstLength8) = 64        [24]:13:8                 */
  DmaBurstLength32        = 0x00002000,   /* (PBL) programmable Dma burst length = 32        13:8    RW                */
  DmaBurstLength16        = 0x00001000,   /* Dma burst length = 16                                                     */
  DmaBurstLength8         = 0x00000800,   /* Dma burst length = 8                                                      */
  DmaBurstLength4         = 0x00000400,   /* Dma burst length = 4                                                      */
  DmaBurstLength2         = 0x00000200,   /* Dma burst length = 2                                                      */
  DmaBurstLength1         = 0x00000100,   /* Dma burst length = 1                                                      */
  DmaBurstLength0         = 0x00000000,   /* Dma burst length = 0                                               0x00   */

  DmaDescriptor8Words     = 0x00000080,   /* Enh Descriptor works  1=> 8 word descriptor      7                  0    */
  DmaDescriptor4Words     = 0x00000000,   /* Enh Descriptor works  0=> 4 word descriptor      7                  0    */

  DmaDescriptorSkip16     = 0x00000040,   /* (DSL)Descriptor skip length (no.of dwords)       6:2     RW               */
  DmaDescriptorSkip8      = 0x00000020,   /* between two unchained descriptors                                         */
  DmaDescriptorSkip4      = 0x00000010,   /*                                                                           */
  DmaDescriptorSkip2      = 0x00000008,   /*                                                                           */
  DmaDescriptorSkip1      = 0x00000004,   /*                                                                           */
  DmaDescriptorSkip0      = 0x00000000,   /*                                                                    0x00   */

  DmaArbitRr              = 0x00000000,   /* (DA) DMA RR arbitration                            1     RW         0     */ 
  DmaArbitPr              = 0x00000002,   /* Rx has priority over Tx                                                   */  
  
  DmaResetOn              = 0x00000001,   /* (SWR)Software Reset DMA engine                     0     RW               */
  DmaResetOff             = 0x00000000,   /*                                                                      0    */
};

/*DmaStatus         = 0x0014,    CSR5 - Dma status Register                        */
/*DMA×´Ì¬¼Ä´æÆ÷*/
enum DmaStatusReg         
{ 
  /*Bit 28 27 and 26 indicate whether the interrupt due to PMT GMACMMC or GMAC LINE Remaining bits are DMA interrupts*/                      
  #ifdef AVB_SUPPORT
  DmaSlotCounterIntr      = 0x40000000,   /* For Ch1 and Ch2 AVB slot interrupt status          31     RW       0       */
  #endif
  #ifdef LPI_SUPPORT
  GmacLPIIntr             = 0x40000000,   /* GMC LPI interrupt                                  31     RO       0       */ 
  #endif
  
  GmacPmtIntr             = 0x10000000,   /* (GPI)Gmac subsystem interrupt                      28     RO       0       */ 
  GmacMmcIntr             = 0x08000000,   /* (GMI)Gmac MMC subsystem interrupt                  27     RO       0       */ 
  GmacLineIntfIntr        = 0x04000000,   /* Line interface interrupt                           26     RO       0       */

  DmaErrorBit2            = 0x02000000,   /* (EB)Error bits 0-data buffer, 1-desc. access       25     RO       0       */
  DmaErrorBit1            = 0x01000000,   /* (EB)Error bits 0-write trnsf, 1-read transfr       24     RO       0       */
  DmaErrorBit0            = 0x00800000,   /* (EB)Error bits 0-Rx DMA, 1-Tx DMA                  23     RO       0       */

  DmaTxState              = 0x00700000,   /* (TS)Transmit process state                         22:20  RO               */
  DmaTxStopped            = 0x00000000,   /* Stopped - Reset or Stop Tx Command issued                         000      */
  DmaTxFetching           = 0x00100000,   /* Running - fetching the Tx descriptor                                       */
  DmaTxWaiting            = 0x00200000,   /* Running - waiting for status                                               */
  DmaTxReading            = 0x00300000,   /* Running - reading the data from host memory                                */
  DmaTxSuspended          = 0x00600000,   /* Suspended - Tx Descriptor unavailabe                                       */
  DmaTxClosing            = 0x00700000,   /* Running - closing Rx descriptor                                            */

  DmaRxState              = 0x000E0000,   /* (RS)Receive process state                         19:17  RO                */
  DmaRxStopped            = 0x00000000,   /* Stopped - Reset or Stop Rx Command issued                         000      */
  DmaRxFetching           = 0x00020000,   /* Running - fetching the Rx descriptor                                       */
  DmaRxWaiting            = 0x00060000,   /* Running - waiting for packet                                               */
  DmaRxSuspended          = 0x00080000,   /* Suspended - Rx Descriptor unavailable                                      */
  DmaRxClosing            = 0x000A0000,   /* Running - closing descriptor                                               */
  DmaRxQueuing            = 0x000E0000,   /* Running - queuing the recieve frame into host memory                       */

  DmaIntNormal            = 0x00010000,   /* (NIS)Normal interrupt summary                     16     RW        0       */
  DmaIntAbnormal          = 0x00008000,   /* (AIS)Abnormal interrupt summary                   15     RW        0       */

  DmaIntEarlyRx           = 0x00004000,   /* Early receive interrupt (Normal)       RW        0       */
  DmaIntBusError          = 0x00002000,   /* Fatal bus error (Abnormal)             RW        0       */
  DmaIntEarlyTx           = 0x00000400,   /* Early transmit interrupt (Abnormal)    RW        0       */
  DmaIntRxWdogTO          = 0x00000200,   /* Receive Watchdog Timeout (Abnormal)    RW        0       */
  DmaIntRxStopped         = 0x00000100,   /* Receive process stopped (Abnormal)     RW        0       */
  DmaIntRxNoBuffer        = 0x00000080,   /* Receive buffer unavailable (Abnormal)  RW        0       */
  DmaIntRxCompleted       = 0x00000040,   /* Completion of frame reception (Normal) RW        0       */
  DmaIntTxUnderflow       = 0x00000020,   /* Transmit underflow (Abnormal)          RW        0       */
  DmaIntRcvOverflow       = 0x00000010,   /* Receive Buffer overflow interrupt      RW        0       */
  DmaIntTxJabberTO        = 0x00000008,   /* Transmit Jabber Timeout (Abnormal)     RW        0       */
  DmaIntTxNoBuffer        = 0x00000004,   /* Transmit buffer unavailable (Normal)   RW        0       */
  DmaIntTxStopped         = 0x00000002,   /* Transmit process stopped (Abnormal)    RW        0       */
  DmaIntTxCompleted       = 0x00000001,   /* Transmit completed (Normal)            RW        0       */
};

/*DmaControl        = 0x0018,     CSR6 - Dma Operation Mode Register                */
/*DMA¿ØÖÆ¼Ä´æÆ÷*/
enum DmaControlReg        
{ 
  DmaDisableDropTcpCs	  = 0x04000000,   /* (DT) Dis. drop. of tcp/ip CS error frames        26      RW        0       */
                                        
  DmaStoreAndForward      = 0x00200000,   /* (SF)Store and forward                            21      RW        0       */
  DmaFlushTxFifo          = 0x00100000,   /* (FTF)Tx FIFO controller is reset to default      20      RW        0       */ 
  
  DmaTxThreshCtrl         = 0x0001C000,   /* (TTC)Controls thre Threh of MTL tx Fifo          16:14   RW                */ 
  DmaTxThreshCtrl16       = 0x0001C000,   /* (TTC)Controls thre Threh of MTL tx Fifo 16       16:14   RW                */ 
  DmaTxThreshCtrl24       = 0x00018000,   /* (TTC)Controls thre Threh of MTL tx Fifo 24       16:14   RW                */ 
  DmaTxThreshCtrl32       = 0x00014000,   /* (TTC)Controls thre Threh of MTL tx Fifo 32       16:14   RW                */ 
  DmaTxThreshCtrl40       = 0x00010000,   /* (TTC)Controls thre Threh of MTL tx Fifo 40       16:14   RW                */   
  DmaTxThreshCtrl256      = 0x0000c000,   /* (TTC)Controls thre Threh of MTL tx Fifo 256      16:14   RW                */   
  DmaTxThreshCtrl192      = 0x00008000,   /* (TTC)Controls thre Threh of MTL tx Fifo 192      16:14   RW                */   
  DmaTxThreshCtrl128      = 0x00004000,   /* (TTC)Controls thre Threh of MTL tx Fifo 128      16:14   RW                */   
  DmaTxThreshCtrl64       = 0x00000000,   /* (TTC)Controls thre Threh of MTL tx Fifo 64       16:14   RW        000     */ 
  
  DmaTxStart              = 0x00002000,   /* (ST)Start/Stop transmission                      13      RW        0       */

  DmaRxFlowCtrlDeact      = 0x00401800,   /* (RFD)Rx flow control deact. threhold             [22]:12:11   RW                 */ 
  DmaRxFlowCtrlDeact1K    = 0x00000000,   /* (RFD)Rx flow control deact. threhold (1kbytes)   [22]:12:11   RW        00       */ 
  DmaRxFlowCtrlDeact2K    = 0x00000800,   /* (RFD)Rx flow control deact. threhold (2kbytes)   [22]:12:11   RW                 */ 
  DmaRxFlowCtrlDeact3K    = 0x00001000,   /* (RFD)Rx flow control deact. threhold (3kbytes)   [22]:12:11   RW                 */ 
  DmaRxFlowCtrlDeact4K    = 0x00001800,   /* (RFD)Rx flow control deact. threhold (4kbytes)   [22]:12:11   RW                 */ 	
  DmaRxFlowCtrlDeact5K    = 0x00400000,   /* (RFD)Rx flow control deact. threhold (4kbytes)   [22]:12:11   RW                 */ 	
  DmaRxFlowCtrlDeact6K    = 0x00400800,   /* (RFD)Rx flow control deact. threhold (4kbytes)   [22]:12:11   RW                 */ 	
  DmaRxFlowCtrlDeact7K    = 0x00401000,   /* (RFD)Rx flow control deact. threhold (4kbytes)   [22]:12:11   RW                 */ 	
  
  DmaRxFlowCtrlAct        = 0x00800600,   /* (RFA)Rx flow control Act. threhold              [23]:10:09   RW                 */ 
  DmaRxFlowCtrlAct1K      = 0x00000000,   /* (RFA)Rx flow control Act. threhold (1kbytes)    [23]:10:09   RW        00       */ 
  DmaRxFlowCtrlAct2K      = 0x00000200,   /* (RFA)Rx flow control Act. threhold (2kbytes)    [23]:10:09   RW                 */ 
  DmaRxFlowCtrlAct3K      = 0x00000400,   /* (RFA)Rx flow control Act. threhold (3kbytes)    [23]:10:09   RW                 */ 
  DmaRxFlowCtrlAct4K      = 0x00000300,   /* (RFA)Rx flow control Act. threhold (4kbytes)    [23]:10:09   RW                 */ 	
  DmaRxFlowCtrlAct5K      = 0x00800000,   /* (RFA)Rx flow control Act. threhold (5kbytes)    [23]:10:09   RW                 */ 	
  DmaRxFlowCtrlAct6K      = 0x00800200,   /* (RFA)Rx flow control Act. threhold (6kbytes)    [23]:10:09   RW                 */ 	
  DmaRxFlowCtrlAct7K      = 0x00800400,   /* (RFA)Rx flow control Act. threhold (7kbytes)    [23]:10:09   RW                 */ 	
  
  DmaRxThreshCtrl         = 0x00000018,   /* (RTC)Controls thre Threh of MTL rx Fifo          4:3   RW                */ 
  DmaRxThreshCtrl64       = 0x00000000,   /* (RTC)Controls thre Threh of MTL tx Fifo 64       4:3   RW                */ 
  DmaRxThreshCtrl32       = 0x00000008,   /* (RTC)Controls thre Threh of MTL tx Fifo 32       4:3   RW                */ 
  DmaRxThreshCtrl96       = 0x00000010,   /* (RTC)Controls thre Threh of MTL tx Fifo 96       4:3   RW                */ 
  DmaRxThreshCtrl128      = 0x00000018,   /* (RTC)Controls thre Threh of MTL tx Fifo 128      4:3   RW                */ 

  DmaEnHwFlowCtrl         = 0x00000100,   /* (EFC)Enable HW flow control                      8       RW                 */ 
  DmaDisHwFlowCtrl        = 0x00000000,   /* Disable HW flow control                                            0        */ 
        
  DmaFwdErrorFrames       = 0x00000080,   /* (FEF)Forward error frames                        7       RW        0       */
  DmaFwdUnderSzFrames     = 0x00000040,   /* (FUF)Forward undersize frames                    6       RW        0       */
  DmaTxSecondFrame        = 0x00000004,   /* (OSF)Operate on second frame                     4       RW        0       */
  DmaRxStart              = 0x00000002,   /* (SR)Start/Stop reception                         1       RW        0       */
};

/*DmaInterrupt      = 0x001C,    CSR7 - Interrupt enable Register Layout     */
/*DMAÖÐ¶Ï¼Ä´æÆ÷*/
enum  DmaInterruptReg
{                     
  DmaIeNormal            = DmaIntNormal     ,   /* Normal interrupt enable                 RW        0       */
  DmaIeAbnormal          = DmaIntAbnormal   ,   /* Abnormal interrupt enable               RW        0       */

  DmaIeEarlyRx           = DmaIntEarlyRx    ,   /* Early receive interrupt enable          RW        0       */
  DmaIeBusError          = DmaIntBusError   ,   /* Fatal bus error enable                  RW        0       */
  DmaIeEarlyTx           = DmaIntEarlyTx    ,   /* Early transmit interrupt enable         RW        0       */
  DmaIeRxWdogTO          = DmaIntRxWdogTO   ,   /* Receive Watchdog Timeout enable         RW        0       */
  DmaIeRxStopped         = DmaIntRxStopped  ,   /* Receive process stopped enable          RW        0       */
  DmaIeRxNoBuffer        = DmaIntRxNoBuffer ,   /* Receive buffer unavailable enable       RW        0       */
  DmaIeRxCompleted       = DmaIntRxCompleted,   /* Completion of frame reception enable    RW        0       */
  DmaIeTxUnderflow       = DmaIntTxUnderflow,   /* Transmit underflow enable               RW        0       */

  DmaIeRxOverflow        = DmaIntRcvOverflow,   /* Receive Buffer overflow interrupt       RW        0       */
  DmaIeTxJabberTO        = DmaIntTxJabberTO ,   /* Transmit Jabber Timeout enable          RW        0       */
  DmaIeTxNoBuffer        = DmaIntTxNoBuffer ,   /* Transmit buffer unavailable enable      RW        0       */
  DmaIeTxStopped         = DmaIntTxStopped  ,   /* Transmit process stopped enable         RW        0       */
  DmaIeTxCompleted       = DmaIntTxCompleted,   /* Transmit completed enable               RW        0       */
};

#ifdef AVB_SUPPORT
/*DmaSlotFnCtrlSts  = 0x0030,     Slot function control and status register         */
enum DmaSlotFnCtrlStsReg
{
  SlotNum                = 0x000F0000,   /* Current Slot Number                            19:16     R0         0      */
  AdvSlotInt             = 0x00000002,   /* Advance the slot interval for data fetch       1         RW         0      */
  EnaSlot                = 0x00000001,   /* Enable checking of Slot number                 0         RW         0      */
};

/*  DmaChannelCtrl    = 0x0060,     Channel Control register only for Channel1 and Channel2 */
enum DmaChannelCtrlReg
{
  ChannelSlotIntEn       = 0x00020000,   /* Channel Slot Interrupt Enable                  16         RW         0      */ 
  ChannelSlotCount       = 0x00000070,   /* Channel Slot Count                             6:4        RW         0      */ 
  ChannelCreditCtrl      = 0x00000002,   /* Channel Credit Control                         1          RW         0      */ 
  ChannelCreditShDis     = 0x00000001,   /* Channel Credit based shaping disable           0          RW         0      */ 
};

/*  DmaChannelSts     = 0x0064,     Channel Status register only for Channel1 and  Channel2 */ 
enum DmaChannelStsReg     
{
  ChannelAvBitsPerSlot   = 0x0000FFFF,   /* Channel Average Bits per slot                  16:0       RO         0      */      
};

/*  IdleSlopeCredit   = 0x0068,     Idle slope credit register                              */
enum IdleSlopeCreditReg
{
   ChannelIdleSlCr       = 0x00003FFF,   /*Channel Idle Slope Credit                       13:0       RW         0     */      
};
  
/*SendSlopeCredit   = 0x006C,     Send slope credit register                              */
enum SendSlopeCreditReg
{
   ChannelSendSlCr       = 0x00003FFF,   /*Channel Send Slope Credit                       13:0       RW         0     */      
};

/*  HighCredit        = 0x0070,     High Credit register                                    */
enum HighCreditReg        
{
   ChannelHiCr           = 0x1FFFFFFF,   /*Channel Hi Credit                               28:0       RW         0     */
};

/*  LoCredit          = 0x0074,     Lo Credit Register                                      */  
enum LoCreditReg         
{
   ChannelLoCr           = 0x1FFFFFFF,   /* Channel Lo Credit                               28:0      RW         0    */     
};
/*DmaChannelAvSts   */
enum DmaChannelAvStsReg
{
   ChannelAvgBitsPerSlotMsk = 0x0001FFFF,
};
#endif

/**********************************************************
 * Initial register values  ³õÊ¼»¯gmac¼Ä´æÆ÷Öµ
 **********************************************************/
 enum InitialRegisters
{
   /* Full-duplex mode with perfect filter on */
  GmacConfigInitFdx1000   = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable | GmacJumboFrameDisable
                          | GmacSelectGmii     | GmacEnableRxOwn          | GmacLoopbackOff
                          | GmacFullDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable,
  
  /* Full-duplex mode with perfect filter on */
  GmacConfigInitFdx110    = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
                          | GmacSelectMii      | GmacEnableRxOwn          | GmacLoopbackOff
                          | GmacFullDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable,

   /* Full-duplex mode */
   // CHANGED: Pass control config, dest addr filter normal, added source address filter, multicast & unicast 
   // Hash filter. 
   /*                        = GmacFilterOff         | GmacPassControlOff | GmacBroadcastEnable */
   GmacFrameFilterInitFdx = GmacFilterOn          | GmacPassControl0   | GmacBroadcastEnable |  GmacSrcAddrFilterDisable
                           | GmacMulticastFilterOn | GmacDestAddrFilterNor | GmacMcastHashFilterOff
                          | GmacPromiscuousModeOff | GmacUcastHashFilterOff,
   
   /* Full-duplex mode */
   GmacFlowControlInitFdx = GmacUnicastPauseFrameOff | GmacRxFlowControlEnable | GmacTxFlowControlEnable,

   /* Full-duplex mode */
   GmacGmiiAddrInitFdx    = GmiiCsrClk2,


   /* Half-duplex mode with perfect filter on */
   // CHANGED: Removed Endian configuration, added single bit config for PAD/CRC strip,   			  
   /*| GmacSelectMii      | GmacLittleEndian         | GmacDisableRxOwn      | GmacLoopbackOff*/
   GmacConfigInitHdx1000  = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
                          | GmacSelectGmii     | GmacDisableRxOwn         | GmacLoopbackOff
                          | GmacHalfDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable   
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable,

   /* Half-duplex mode with perfect filter on */
   GmacConfigInitHdx110   = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
                          | GmacSelectMii      | GmacDisableRxOwn         | GmacLoopbackOff
                          | GmacHalfDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable 
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable,

   /* Half-duplex mode */
   GmacFrameFilterInitHdx = GmacFilterOn          | GmacPassControl0        | GmacBroadcastEnable | GmacSrcAddrFilterDisable
                          | GmacMulticastFilterOn | GmacDestAddrFilterNor   | GmacMcastHashFilterOff
                          | GmacUcastHashFilterOff| GmacPromiscuousModeOff,

   /* Half-duplex mode */
   GmacFlowControlInitHdx = GmacUnicastPauseFrameOff | GmacRxFlowControlDisable | GmacTxFlowControlDisable,

   /* Half-duplex mode */
   GmacGmiiAddrInitHdx    = GmiiCsrClk2,



   /**********************************************
   *DMA configurations   DMAÅäÖÃ³õÊ¼»¯
   **********************************************/

  DmaBusModeInit         = DmaFixedBurstEnable |   DmaBurstLength8   | DmaDescriptorSkip2       | DmaResetOff,
//   DmaBusModeInit         = DmaFixedBurstEnable |   DmaBurstLength8   | DmaDescriptorSkip4       | DmaResetOff,
   
   /* 1000 Mb/s mode */
   DmaControlInit1000     = DmaStoreAndForward,//       | DmaTxSecondFrame ,

   /* 100 Mb/s mode */
   DmaControlInit100      = DmaStoreAndForward,
   
   /* 10 Mb/s mode */
   DmaControlInit10       = DmaStoreAndForward,

  /* Interrupt groups */
  DmaIntErrorMask         = DmaIntBusError,           /* Error */
  DmaIntRxAbnMask         = DmaIntRxNoBuffer,         /* receiver abnormal interrupt */
  DmaIntRxNormMask        = DmaIntRxCompleted,        /* receiver normal interrupt   */
  DmaIntRxStoppedMask     = DmaIntRxStopped,          /* receiver stopped */
  DmaIntTxAbnMask         = DmaIntTxUnderflow,        /* transmitter abnormal interrupt */
  DmaIntTxNormMask        = DmaIntTxCompleted,        /* transmitter normal interrupt */
  DmaIntTxStoppedMask     = DmaIntTxStopped,          /* transmitter stopped */

  DmaIntEnable            = DmaIeNormal     | DmaIeAbnormal    | DmaIntErrorMask
                          | DmaIntRxAbnMask | DmaIntRxNormMask | DmaIntRxStoppedMask
                          | DmaIntTxAbnMask | DmaIntTxNormMask | DmaIntTxStoppedMask,
  DmaIntDisable           = 0,
};

/**********************************************************
 * Mac Management Counters (MMC)      MAC¹ÜÀí¼ÆÊýÆ÷Æ«ÒÆµØÖ·
 **********************************************************/
 enum MMC_ENABLE
{
	GmacMmcCntrl			= 0x0100,	/* mmc control for operating mode of MMC						*/
	GmacMmcIntrRx			= 0x0104,	/* maintains interrupts generated by rx counters					*/
	GmacMmcIntrTx			= 0x0108,	/* maintains interrupts generated by tx counters					*/
	GmacMmcIntrMaskRx		= 0x010C,	/* mask for interrupts generated from rx counters					*/
	GmacMmcIntrMaskTx		= 0x0110,	/* mask for interrupts generated from tx counters					*/
};

enum MMC_TX
{
	GmacMmcTxOctetCountGb		= 0x0114,	/*Bytes Tx excl. of preamble and retried bytes     (Good or Bad)			*/
	GmacMmcTxFrameCountGb		= 0x0118,	/*Frames Tx excl. of retried frames	           (Good or Bad)			*/
	GmacMmcTxBcFramesG		= 0x011C,	/*Broadcast Frames Tx 				   (Good)				*/
	GmacMmcTxMcFramesG		= 0x0120,	/*Multicast Frames Tx				   (Good)				*/
	
	GmacMmcTx64OctetsGb		= 0x0124,	/*Tx with len 64 bytes excl. of pre and retried    (Good or Bad)			*/
	GmacMmcTx65To127OctetsGb	= 0x0128,	/*Tx with len >64 bytes <=127 excl. of pre and retried    (Good or Bad)			*/
	GmacMmcTx128To255OctetsGb	= 0x012C,	/*Tx with len >128 bytes <=255 excl. of pre and retried   (Good or Bad)			*/
	GmacMmcTx256To511OctetsGb	= 0x0130,	/*Tx with len >256 bytes <=511 excl. of pre and retried   (Good or Bad)			*/
	GmacMmcTx512To1023OctetsGb	= 0x0134,	/*Tx with len >512 bytes <=1023 excl. of pre and retried  (Good or Bad)			*/
	GmacMmcTx1024ToMaxOctetsGb	= 0x0138,	/*Tx with len >1024 bytes <=MaxSize excl. of pre and retried (Good or Bad)		*/
	
	GmacMmcTxUcFramesGb		= 0x013C,	/*Unicast Frames Tx 					 (Good or Bad)			*/
	GmacMmcTxMcFramesGb		= 0x0140,	/*Multicast Frames Tx				   (Good and Bad)			*/
	GmacMmcTxBcFramesGb		= 0x0144,	/*Broadcast Frames Tx 				   (Good and Bad)			*/
	GmacMmcTxUnderFlowError		= 0x0148,	/*Frames aborted due to Underflow error							*/
	GmacMmcTxSingleColG		= 0x014C,	/*Successfully Tx Frames after singel collision in Half duplex mode			*/
	GmacMmcTxMultiColG		= 0x0150,	/*Successfully Tx Frames after more than singel collision in Half duplex mode		*/
	GmacMmcTxDeferred		= 0x0154,	/*Successfully Tx Frames after a deferral in Half duplex mode				*/
	GmacMmcTxLateCol		= 0x0158,	/*Frames aborted due to late collision error						*/
	GmacMmcTxExessCol		= 0x015C,	/*Frames aborted due to excessive (16) collision errors					*/
	GmacMmcTxCarrierError		= 0x0160,	/*Frames aborted due to carrier sense error (No carrier or Loss of carrier)		*/
	GmacMmcTxOctetCountG		= 0x0164,	/*Bytes Tx excl. of preamble and retried bytes     (Good) 				*/
	GmacMmcTxFrameCountG		= 0x0168,	/*Frames Tx 				           (Good)				*/
	GmacMmcTxExessDef		= 0x016C,	/*Frames aborted due to excessive deferral errors (deferred for more than 2 max-sized frame times)*/
	
	GmacMmcTxPauseFrames		= 0x0170,	/*Number of good pause frames Tx.							*/
	GmacMmcTxVlanFramesG		= 0x0174,	/*Number of good Vlan frames Tx excl. retried frames					*/
};

enum MMC_RX
{
	GmacMmcRxFrameCountGb		= 0x0180,	/*Frames Rx 				           (Good or Bad)			*/
	GmacMmcRxOctetCountGb		= 0x0184,	/*Bytes Rx excl. of preamble and retried bytes     (Good or Bad)			*/
	GmacMmcRxOctetCountG		= 0x0188,	/*Bytes Rx excl. of preamble and retried bytes     (Good) 				*/
	GmacMmcRxBcFramesG		= 0x018C,	/*Broadcast Frames Rx 				   (Good)				*/
	GmacMmcRxMcFramesG		= 0x0190,	/*Multicast Frames Rx				   (Good)				*/
	
	GmacMmcRxCrcError		= 0x0194,	/*Number of frames received with CRC error						*/
	GmacMmcRxAlignError		= 0x0198,	/*Number of frames received with alignment (dribble) error. Only in 10/100mode		*/
	GmacMmcRxRuntError		= 0x019C,	/*Number of frames received with runt (<64 bytes and CRC error) error			*/
	GmacMmcRxJabberError		= 0x01A0,	/*Number of frames rx with jabber (>1518/1522 or >9018/9022 and CRC) 			*/
	GmacMmcRxUnderSizeG		= 0x01A4,	/*Number of frames received with <64 bytes without any error				*/
	GmacMmcRxOverSizeG		= 0x01A8,	/*Number of frames received with >1518/1522 bytes without any error			*/
	
	GmacMmcRx64OctetsGb		= 0x01AC,	/*Rx with len 64 bytes excl. of pre and retried    (Good or Bad)			*/
	GmacMmcRx65To127OctetsGb	= 0x01B0,	/*Rx with len >64 bytes <=127 excl. of pre and retried    (Good or Bad)			*/
	GmacMmcRx128To255OctetsGb	= 0x01B4,	/*Rx with len >128 bytes <=255 excl. of pre and retried   (Good or Bad)			*/
	GmacMmcRx256To511OctetsGb	= 0x01B8,	/*Rx with len >256 bytes <=511 excl. of pre and retried   (Good or Bad)			*/
	GmacMmcRx512To1023OctetsGb	= 0x01BC,	/*Rx with len >512 bytes <=1023 excl. of pre and retried  (Good or Bad)			*/
	GmacMmcRx1024ToMaxOctetsGb	= 0x01C0,	/*Rx with len >1024 bytes <=MaxSize excl. of pre and retried (Good or Bad)		*/
	
	GmacMmcRxUcFramesG		= 0x01C4,	/*Unicast Frames Rx 					 (Good)				*/
	GmacMmcRxLengthError		= 0x01C8,	/*Number of frames received with Length type field != frame size			*/
	GmacMmcRxOutOfRangeType		= 0x01CC,	/*Number of frames received with length field != valid frame size			*/
	
	GmacMmcRxPauseFrames		= 0x01D0,	/*Number of good pause frames Rx.							*/
	GmacMmcRxFifoOverFlow		= 0x01D4,	/*Number of missed rx frames due to FIFO overflow					*/
	GmacMmcRxVlanFramesGb		= 0x01D8,	/*Number of good Vlan frames Rx 							*/
	
	GmacMmcRxWatchdobError		= 0x01DC,	/*Number of frames rx with error due to watchdog timeout error				*/
};

enum MMC_IP_RELATED
{
	GmacMmcRxIpcIntrMask		= 0x0200,	/*Maintains the mask for interrupt generated from rx IPC statistic counters 		*/
	GmacMmcRxIpcIntr		= 0x0208,	/*Maintains the interrupt that rx IPC statistic counters generate			*/
	
	GmacMmcRxIpV4FramesG		= 0x0210,	/*Good IPV4 datagrams received								*/
	GmacMmcRxIpV4HdrErrFrames	= 0x0214,	/*Number of IPV4 datagrams received with header errors					*/
	GmacMmcRxIpV4NoPayFrames	= 0x0218,	/*Number of IPV4 datagrams received which didnot have TCP/UDP/ICMP payload		*/
	GmacMmcRxIpV4FragFrames		= 0x021C,	/*Number of IPV4 datagrams received with fragmentation					*/
	GmacMmcRxIpV4UdpChkDsblFrames	= 0x0220,	/*Number of IPV4 datagrams received that had a UDP payload checksum disabled		*/
	
	GmacMmcRxIpV6FramesG		= 0x0224,	/*Good IPV6 datagrams received								*/
	GmacMmcRxIpV6HdrErrFrames	= 0x0228,	/*Number of IPV6 datagrams received with header errors					*/
	GmacMmcRxIpV6NoPayFrames	= 0x022C,	/*Number of IPV6 datagrams received which didnot have TCP/UDP/ICMP payload		*/
	
	GmacMmcRxUdpFramesG		= 0x0230,	/*Number of good IP datagrams with good UDP payload					*/
	GmacMmcRxUdpErrorFrames		= 0x0234,	/*Number of good IP datagrams with UDP payload having checksum error			*/
	
	GmacMmcRxTcpFramesG		= 0x0238,	/*Number of good IP datagrams with good TDP payload					*/
	GmacMmcRxTcpErrorFrames		= 0x023C,	/*Number of good IP datagrams with TCP payload having checksum error			*/

	GmacMmcRxIcmpFramesG		= 0x0240,	/*Number of good IP datagrams with good Icmp payload					*/
	GmacMmcRxIcmpErrorFrames	= 0x0244,	/*Number of good IP datagrams with Icmp payload having checksum error			*/
	
	GmacMmcRxIpV4OctetsG		= 0x0250,	/*Good IPV4 datagrams received excl. Ethernet hdr,FCS,Pad,Ip Pad bytes			*/
	GmacMmcRxIpV4HdrErrorOctets	= 0x0254,	/*Number of bytes in IPV4 datagram with header errors					*/
	GmacMmcRxIpV4NoPayOctets	= 0x0258,	/*Number of bytes in IPV4 datagram with no TCP/UDP/ICMP payload				*/
	GmacMmcRxIpV4FragOctets		= 0x025C,	/*Number of bytes received in fragmented IPV4 datagrams 				*/
	GmacMmcRxIpV4UdpChkDsblOctets	= 0x0260,	/*Number of bytes received in UDP segment that had UDP checksum disabled		*/
	
	GmacMmcRxIpV6OctetsG		= 0x0264,	/*Good IPV6 datagrams received excl. Ethernet hdr,FCS,Pad,Ip Pad bytes			*/
	GmacMmcRxIpV6HdrErrorOctets	= 0x0268,	/*Number of bytes in IPV6 datagram with header errors					*/
	GmacMmcRxIpV6NoPayOctets	= 0x026C,	/*Number of bytes in IPV6 datagram with no TCP/UDP/ICMP payload				*/
	
	GmacMmcRxUdpOctetsG		= 0x0270,	/*Number of bytes in IP datagrams with good UDP payload					*/
	GmacMmcRxUdpErrorOctets		= 0x0274,	/*Number of bytes in IP datagrams with UDP payload having checksum error		*/
	
	GmacMmcRxTcpOctetsG		= 0x0278,	/*Number of bytes in IP datagrams with good TDP payload					*/
	GmacMmcRxTcpErrorOctets		= 0x027C,	/*Number of bytes in IP datagrams with TCP payload having checksum error		*/
	
	GmacMmcRxIcmpOctetsG		= 0x0280,	/*Number of bytes in IP datagrams with good Icmp payload				*/
	GmacMmcRxIcmpErrorOctets	= 0x0284,	/*Number of bytes in IP datagrams with Icmp payload having checksum error		*/
};

enum MMC_CNTRL_REG_BIT_DESCRIPTIONS
{
	GmacMmcCounterFreeze		= 0x00000008,		/* when set MMC counters freeze to current value				*/
	GmacMmcCounterResetOnRead	= 0x00000004,		/* when set MMC counters will be reset to 0 after read				*/
	GmacMmcCounterStopRollover	= 0x00000002,		/* when set counters will not rollover after max value				*/
	GmacMmcCounterReset		= 0x00000001,		/* when set all counters wil be reset (automatically cleared after 1 clk)	*/
	
};

enum MMC_RX_INTR_MASK_AND_STATUS_BIT_DESCRIPTIONS
{
	GmacMmcRxWDInt			= 0x00800000,		/* set when rxwatchdog error reaches half of max value				*/
	GmacMmcRxVlanInt		= 0x00400000,		/* set when GmacMmcRxVlanFramesGb counter reaches half of max value		*/
	GmacMmcRxFifoOverFlowInt	= 0x00200000,		/* set when GmacMmcRxFifoOverFlow counter reaches half of max value		*/
	GmacMmcRxPauseFrameInt		= 0x00100000,		/* set when GmacMmcRxPauseFrames counter reaches half of max value		*/
	GmacMmcRxOutOfRangeInt		= 0x00080000,		/* set when GmacMmcRxOutOfRangeType counter reaches half of max value		*/
	GmacMmcRxLengthErrorInt		= 0x00040000,		/* set when GmacMmcRxLengthError counter reaches half of max value		*/
	GmacMmcRxUcFramesInt		= 0x00020000,		/* set when GmacMmcRxUcFramesG counter reaches half of max value		*/
	GmacMmcRx1024OctInt		= 0x00010000,		/* set when GmacMmcRx1024ToMaxOctetsGb counter reaches half of max value	*/
	GmacMmcRx512OctInt		= 0x00008000,		/* set when GmacMmcRx512To1023OctetsGb counter reaches half of max value	*/
	GmacMmcRx256OctInt		= 0x00004000,		/* set when GmacMmcRx256To511OctetsGb counter reaches half of max value		*/
	GmacMmcRx128OctInt		= 0x00002000,		/* set when GmacMmcRx128To255OctetsGb counter reaches half of max value		*/
	GmacMmcRx65OctInt		= 0x00001000,		/* set when GmacMmcRx65To127OctetsG counter reaches half of max value		*/
	GmacMmcRx64OctInt		= 0x00000800,		/* set when GmacMmcRx64OctetsGb counter reaches half of max value		*/
	GmacMmcRxOverSizeInt		= 0x00000400,		/* set when GmacMmcRxOverSizeG counter reaches half of max value		*/
	GmacMmcRxUnderSizeInt		= 0x00000200,		/* set when GmacMmcRxUnderSizeG counter reaches half of max value		*/
	GmacMmcRxJabberErrorInt		= 0x00000100,		/* set when GmacMmcRxJabberError counter reaches half of max value		*/
	GmacMmcRxRuntErrorInt		= 0x00000080,		/* set when GmacMmcRxRuntError counter reaches half of max value		*/
	GmacMmcRxAlignErrorInt		= 0x00000040,		/* set when GmacMmcRxAlignError counter reaches half of max value		*/
	GmacMmcRxCrcErrorInt		= 0x00000020,		/* set when GmacMmcRxCrcError counter reaches half of max value			*/
	GmacMmcRxMcFramesInt		= 0x00000010,		/* set when GmacMmcRxMcFramesG counter reaches half of max value		*/
	GmacMmcRxBcFramesInt		= 0x00000008,		/* set when GmacMmcRxBcFramesG counter reaches half of max value		*/
	GmacMmcRxOctetGInt		= 0x00000004,		/* set when GmacMmcRxOctetCountG counter reaches half of max value		*/
	GmacMmcRxOctetGbInt		= 0x00000002,		/* set when GmacMmcRxOctetCountGb counter reaches half of max value		*/
	GmacMmcRxFrameInt		= 0x00000001,		/* set when GmacMmcRxFrameCountGb counter reaches half of max value		*/
};

enum MMC_TX_INTR_MASK_AND_STATUS_BIT_DESCRIPTIONS
{

	GmacMmcTxVlanInt		= 0x01000000,		/* set when GmacMmcTxVlanFramesG counter reaches half of max value		*/
	GmacMmcTxPauseFrameInt		= 0x00800000,		/* set when GmacMmcTxPauseFrames counter reaches half of max value		*/
	GmacMmcTxExessDefInt		= 0x00400000,		/* set when GmacMmcTxExessDef counter reaches half of max value			*/
	GmacMmcTxFrameInt		= 0x00200000,		/* set when GmacMmcTxFrameCount counter reaches half of max value		*/
	GmacMmcTxOctetInt		= 0x00100000,		/* set when GmacMmcTxOctetCountG counter reaches half of max value		*/
	GmacMmcTxCarrierErrorInt	= 0x00080000,		/* set when GmacMmcTxCarrierError counter reaches half of max value		*/
	GmacMmcTxExessColInt		= 0x00040000,		/* set when GmacMmcTxExessCol counter reaches half of max value			*/
	GmacMmcTxLateColInt		= 0x00020000,		/* set when GmacMmcTxLateCol counter reaches half of max value			*/
	GmacMmcTxDeferredInt		= 0x00010000,		/* set when GmacMmcTxDeferred counter reaches half of max value			*/
	GmacMmcTxMultiColInt		= 0x00008000,		/* set when GmacMmcTxMultiColG counter reaches half of max value		*/
	GmacMmcTxSingleCol		= 0x00004000,		/* set when GmacMmcTxSingleColG	counter reaches half of max value		*/
	GmacMmcTxUnderFlowErrorInt	= 0x00002000,		/* set when GmacMmcTxUnderFlowError counter reaches half of max value		*/
	GmacMmcTxBcFramesGbInt 		= 0x00001000,		/* set when GmacMmcTxBcFramesGb	counter reaches half of max value		*/
	GmacMmcTxMcFramesGbInt 		= 0x00000800,		/* set when GmacMmcTxMcFramesGb	counter reaches half of max value		*/
	GmacMmcTxUcFramesInt 		= 0x00000400,		/* set when GmacMmcTxUcFramesGb counter reaches half of max value		*/
	GmacMmcTx1024OctInt 		= 0x00000200,		/* set when GmacMmcTx1024ToMaxOctetsGb counter reaches half of max value	*/
	GmacMmcTx512OctInt 		= 0x00000100,		/* set when GmacMmcTx512To1023OctetsGb counter reaches half of max value	*/
	GmacMmcTx256OctInt 		= 0x00000080,		/* set when GmacMmcTx256To511OctetsGb counter reaches half of max value		*/
	GmacMmcTx128OctInt 		= 0x00000040,		/* set when GmacMmcTx128To255OctetsGb counter reaches half of max value		*/
	GmacMmcTx65OctInt 		= 0x00000020,		/* set when GmacMmcTx65To127OctetsGb counter reaches half of max value		*/
	GmacMmcTx64OctInt 		= 0x00000010,		/* set when GmacMmcTx64OctetsGb	counter reaches half of max value		*/
	GmacMmcTxMcFramesInt 		= 0x00000008,		/* set when GmacMmcTxMcFramesG counter reaches half of max value		*/
	GmacMmcTxBcFramesInt 		= 0x00000004,		/* set when GmacMmcTxBcFramesG counter reaches half of max value		*/
	GmacMmcTxFrameGbInt 		= 0x00000002,		/* set when GmacMmcTxFrameCountGb counter reaches half of max value		*/
	GmacMmcTxOctetGbInt 		= 0x00000001,		/* set when GmacMmcTxOctetCountGb counter reaches half of max value		*/
	
};

/**********************************************************
 * Power Management (PMT) Block    µçÔ´¹ÜÀíÄ£¿é
 **********************************************************/

/**
  * PMT supports the reception of network (remote) wake-up frames and Magic packet frames.
  * It generates interrupts for wake-up frames and Magic packets received by GMAC.
  * PMT sits in Rx path and is enabled with remote wake-up frame enable and Magic packet enable.
  * These enable are in PMT control and Status register and are programmed by apllication.
  *
  * When power down mode is enabled in PMT, all rx frames are dropped by the core. Core comes
  * out of power down mode only when either Magic packe tor a Remote wake-up frame is received 
  * and the corresponding detection is enabled.
  *
  * Driver need not be modified to support this feature. Only Api to put the device in to power 
  * down mode is sufficient
  */
  #define WAKEUP_REG_LENGTH	8 				/*This is the reg length for wake up register configuration*/
  enum GmacPmtCtrlStatusBitDefinition
{
	GmacPmtFrmFilterPtrReset	= 0x80000000,		/* when set remote wake-up frame filter register pointer to 3'b000 */
	GmacPmtGlobalUnicast		= 0x00000200,		/* When set enables any unicast packet to be a wake-up frame       */
	GmacPmtWakeupFrameReceived	= 0x00000040,		/* Wake up frame received					   */
	GmacPmtMagicPktReceived		= 0x00000020,		/* Magic Packet received					   */
	GmacPmtWakeupFrameEnable	= 0x00000004,		/* Wake-up frame enable						   */
	GmacPmtMagicPktEnable		= 0x00000002,		/* Magic packet enable						   */
	GmacPmtPowerDown		    = 0x00000001,		/* Power Down							   */
};

 /**********************************************************
 * IEEE 1588-2008 Precision Time Protocol (PTP) Support   IEEE1588-2008¾«È·Ê±¼äÐ­ÒéÖ§³Ö
 **********************************************************/
enum PTPMessageType
{
    SYNC        	       = 0x0,
    Delay_Req    	       = 0x1,
    Pdelay_Req             = 0x2,
    Pdelay_Resp            = 0x3,
    Follow_up              = 0x8,
    Delay_Resp             = 0x9,
    Pdelay_Resp_Follow_Up  = 0xA,
    Announce               = 0xB,
    Signaling              = 0xC,
    Management             = 0xD,
}; 
 
 typedef struct TimeStampStruct    
{ 
  u32   TSversion;      /* PTP Version 1 or PTP version2                                                                          */
  u32   TSmessagetype;  /* Message type associated with this time stamp                                                           */                              

  u16   TShighest16;    /* Highest 16 bit time stamp value, Valid onley when ADV_TIME_HIGH_WORD configured in corekit		  */
  u32   TSupper32;      /* Most significant 32 bit time stamp value								  */
  u32   TSlower32;      /* Least Significat 32 bit time stamp value								  */

} TimeStamp;

 /**
  * IEEE 1588-2008 is the optional module to support Ethernet frame time stamping.
  * Sixty four (+16) bit time stamps are given in each frames transmit and receive status.
  * The driver assumes the following
  *  1. "IEEE 1588 Time Stamping" "TIME_STAMPING"is ENABLED in corekit
  *  2. "IEEE 1588 External Time Stamp Input Enable" "EXT_TIME_STAMPING" is DISABLED in corekit
  *  3. "IEEE 1588 Advanced Time Stamp support" "ADV_TIME_STAMPING" is ENABLED in corekit
  *  4. "IEEE 1588 Higher Word Register Enable" "ADV_TIME_HIGH_WORD" is ENABLED in corekit
  */
	 
 /* GmacTSControl  = 0x0700,   Controls the Timestamp update logic	: only when IEEE 1588 time stamping is enabled in corekit		  */
 enum GmacTSControlReg
 {
   GmacTSENMACADDR	   = 0x00040000,	 /* Enable Mac Addr for PTP filtering	  18			RW		   0	 */
   
   GmacTSCLKTYPE	   = 0x00030000,	 /* Select the type of clock node		  17:16 		RW		   00	 */
   /*
	   TSCLKTYPE		TSMSTRENA	   TSEVNTENA		 Messages for wihich TS snapshot is taken
		00/01				 X			   0			  SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP
		00/01				 1			   0			  DELAY_REQ
		00/01				 0			   1			  SYNC
		 10 				 NA 		   0			  SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP
		 10 				 NA 		   1			  SYNC, FOLLOW_UP
		 11 				 NA 		   0			  SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP, PDELAY_REQ, PDELAY_RESP
		 11 				 NA 		   1			  SYNC, PDELAY_REQ, PDELAY_RESP 	   
   */
   GmacTSOrdClk 	   = 0x00000000,	 /* 00=> Ordinary clock*/
   GmacTSBouClk 	   = 0x00010000,	 /* 01=> Boundary clock*/
   GmacTSEtoEClk	   = 0x00020000,	 /* 10=> End-to-End transparent clock*/
   GmacTSPtoPClk	   = 0x00030000,	 /* 11=> P-to-P transparent clock*/
 
   GmacTSMSTRENA	   = 0x00008000,	 /* Ena TS Snapshot for Master Messages   15			RW		   0	 */
   GmacTSEVNTENA	   = 0x00004000,	 /* Ena TS Snapshot for Event Messages	  14			RW		   0	 */
   GmacTSIPV4ENA	   = 0x00002000,	 /* Ena TS snapshot for IPv4			  13			RW		   1	 */
   GmacTSIPV6ENA	   = 0x00001000,	 /* Ena TS snapshot for IPv6			  12			RW		   0	 */
   GmacTSIPENA		   = 0x00000800,	 /* Ena TS snapshot for PTP over E'net	  11			RW		   0	 */
   GmacTSVER2ENA	   = 0x00000400,	 /* Ena PTP snooping for version 2		  10			RW		   0	 */
 
   GmacTSCTRLSSR		   = 0x00000200,	  /* Digital or Binary Rollover 		  9 			RW		   0	 */
 
   GmacTSENALL			   = 0x00000100,	  /* Enable TS fro all frames (Ver2 only) 8 			RW		   0	 */
 
   GmacTSADDREG 	   = 0x00000020,	  /* Addend Register Update 		  5 			RW_SC	   0	 */
   GmacTSUPDT		   = 0x00000008,	  /* Time Stamp Update			  3 			RW_SC	   0	 */
   GmacTSINT	   = 0x00000004,	  /* Time Atamp Initialize			  2 			RW_SC	   0	 */
  
   GmacTSTRIG		   = 0x00000010,	  /* Time stamp interrupt Trigger Enable  4 			RW_SC	   0	 */
 
   GmacTSCFUPDT 	   = 0x00000002,	  /* Time Stamp Fine/Coarse 		  1 			RW		   0	 */
   GmacTSCUPDTCoarse   = 0x00000000,	  /* 0=> Time Stamp update method is coarse 					 */
   GmacTSCUPDTFine	   = 0x00000002,	  /* 1=> Time Stamp update method is fine					 */
 
   GmacTSENA	   = 0x00000001,	  /* Time Stamp Enable					  0 			RW		   0	 */
 };
 /*  GmacTSSubSecIncr     	  = 0x0704,   8 bit value by which sub second register is incremented     : only when IEEE 1588 time stamping without external timestamp input */
enum GmacTSSubSecIncrReg
{
  GmacSSINCMsk            = 0x000000FF,       /* Only Lower 8 bits are valid bits     7:0           RW         00    */
};
 /*  GmacTSLow   	  = 0x070C,   Indicates whether the timestamp low count is positive or negative; for Adv timestamp it is always zero */
enum GmacTSSign
{
  GmacTSSign              = 0x80000000,      /* PSNT                                  31            RW          0    */
  GmacTSPositive          = 0x00000000,
  GmacTSNegative          = 0x80000000,
};
 
 /*GmacTargetTimeLow	   = 0x0718,   32 bit nano seconds(MS) to be compared with system time	   : only when IEEE 1588 time stamping without external timestamp input */
 enum GmacTSLowReg
 {
   GmacTSDecThr 		   = 0x3B9AC9FF,	  /*when TSCTRLSSR is set the max value for GmacTargetTimeLowReg and GmacTimeStampLow register is 0x3B9AC9FF at 1ns precision		*/
 };
 /* GmacTSHighWord			= 0x0724,	Time Stamp Higher Word Register (Version 2 only); only lower 16 bits are valid													 */
 enum GmacTSHighWordReg 		 
 {
   GmacTSHighWordMask	   = 0x0000FFFF,	 /* Time Stamp Higher work register has only lower 16 bits valid		 */
 };
 /*GmacTSStatus            = 0x0728,   Time Stamp Status Register                                                                                                       */
enum GmacTSStatusReg
{
  GmacTSTargTimeReached   = 0x00000002,     /* Time Stamp Target Time Reached          1             RO          0    */
  GmacTSSecondsOverflow   = 0x00000001,     /* Time Stamp Seconds Overflow             0             RO          0    */
};  
 
 /* GmacAvMacCtrl			 = 0x0738,	 AV mac control Register  */
#ifdef AVB_SUPPORT
 enum GmacAvMacCtrlReg
 {
   GmacAvCtrlCh 		   = 0x03000000,	  /* Channel on which AV control packets to be received 				  25:24  RW   0    */
   GmacPtpCh			   = 0x00180000,	  /* Channel on which PTP packets to be received						  20:19  RW   0    */
   GmacAvPrio			   = 0x00070000,	  /* Priority tag value for AV Packets									  18:16  RW   4    */
   GmacAvTypeMask		   = 0x0000FFFF,	  /* Ethernet Type value to be used for comparing and detecting AV packet 15:0	 RW   0    */
 };
#endif

#endif /* __ASM_ARCH_REGS_GMAC_H */
