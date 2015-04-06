/* ===================================================================================
 * Copyright (c) <2009> Synopsys, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software annotated with this license and associated documentation files 
 * (the "Software"), to deal in the Software without restriction, including without 
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * =================================================================================== */

/**\file
 *  The top most file which makes use of synopsys GMAC driver code.
 *
 *  This file can be treated as the example code for writing a application driver
 *  for synopsys GMAC device using the driver provided by Synopsys.
 *  This exmple is for Linux 2.6.xx kernel 
 *  - Uses 32 bit 33MHz PCI Interface as the host bus interface
 *  - Uses Linux network driver and the TCP/IP stack framework
 *  - Uses the Device Specific Synopsys GMAC Kernel APIs
 *  \internal
 * ---------------------------REVISION HISTORY--------------------------------
 * Synopsys 			01/Aug/2007			Created
 */
/*revised by clm at 2014/9/10*/
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
/*revised by clm at 2014/9/6*/
/*****************************************************************************/
#include <linux/platform_device.h>
/*****************************************************************************/
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include "synopGMAC_Host.h"
#include "synopGMAC_banner.h"
#include "synopGMAC_plat.h"
#include "synopGMAC_pci_bus_interface.h"
#include "synopGMAC_network_interface.h"
#include "synopGMAC_Dev.h"

/****************************************************/


/* Global declarations: these are required to handle 
   Os and Platform dependent functionalities        */

/*GMAC IP Base address and Size   */
u8 *synopGMACMappedAddr = NULL;
u32 synopGMACMappedAddrSize = 0;

/*global adapter gmacdev pcidev and netdev pointers */
/*revised by clm at 2014/9/18*/
/*************************************************************/
synopGMACNetworkAdapter *synopGMACadapter;
synopGMACdevice		   *synopGMACdev;
static const struct net_device_ops gmac_netdev_ops;
#ifdef AVB_SUPPORT
/*revised by clm at 2014/9/6*/
/************************************************************/
synopGMACdevice		   *synopGMACdev_ch1;
synopGMACdevice		   *synopGMACdev_ch2;
/************************************************************/
#endif

//struct pci_dev             *synopGMACpcidev;
//struct pci_dev             *synopGMACpcidev;
struct platform_device     *synopGMACplatdev;
struct net_device          *synopGMACnetdev;
//int irq_gmac;
/*************************************************************************************/
/*revised by clm at 2014/9/3*/
/*************************************************************************************/
int __init SynopGMAC_Host_Interface_init(void)
{

        int retval;

	TR0("**********************************************************\n");
	TR0("* Driver    :%s\n",synopGMAC_driver_string);
	TR0("* Version   :%s\n",synopGMAC_driver_version);
	TR0("* Copyright :%s\n",synopGMAC_copyright);
	TR0("**********************************************************\n");

        TR0("Initializing synopsys GMAC interfaces ..\n") ;
        /* Initialize the bus interface for the hostcontroller E.g PCI in our case */
        if ((retval = synopGMAC_init_platform_bus_interface())) {
	        TR0("Could not initiliase the bus interface. Is PCI device connected ?\n");
                return retval;
        }
	
	/*Now we have got pdev structure from pci interface. Lets populate it in our global data structure*/	

        /* Initialize the Network dependent services */
	
       if((retval = synopGMAC_init_network_interface())){
		TR0("Could not initialize the Network interface.\n");
		return retval;
	}
      return 0 ;
}
/*************************************************************************************/
/*revised by clm at 2014/9/3*/
/*************************************************************************************/
void __exit SynopGMAC_Host_Interface_exit(void)
{

        TR0("Exiting synopsys GMAC interfaces ..\n") ;

        /* De-Initialize the Network dependent services */
        synopGMAC_exit_network_interface();
	TR0("Exiting synopGMAC_exit_network_interface\n");

        /* Initialize the bus interface for the hostcontroller E.g PCI in our case */
	synopGMAC_exit_platform_bus_interface();
        TR0("Exiting synpGMAC_exit_platform_bus_interface\n");
}
/*************************************************************************************/

module_init(SynopGMAC_Host_Interface_init);
module_exit(SynopGMAC_Host_Interface_exit);
/*revised by clm at 2014/9/9*/
MODULE_AUTHOR("Cheng LM");
MODULE_DESCRIPTION("SYNOPSYS GMAC NETWORK DRIVER WITH PLATFORM INTERFACE");

