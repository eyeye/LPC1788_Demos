/**********************************************************************
* $Id$		lwipopts.h			2011-11-20
*//**
* @file		lwipopts.h
* @brief	LWIP option overrides
* @version	1.0
* @date		20. Nov. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#ifndef __LWIP_USER_OPT_H__
#define __LWIP_USER_OPT_H__

/** @defgroup hitex1850_tcpecho_sa_flash	LWIP configuration configuration
 * @ingroup hitex1850_tcpecho_sa
 *
 * @{
 */

/* Standalone build */
#define NO_SYS                          0

/* Use LWIP timers */
#define NO_SYS_NO_TIMERS				0

/* Need for memory protection */
#define SYS_LIGHTWEIGHT_PROT            0

/* 32-bit alignment */
#define MEM_ALIGNMENT                   4

/* pbuf buffers in pool. In zero-copy mode, these buffers are
   located in peripheral RAM. In copied mode, they are located in
   internal IRAM */
#define PBUF_POOL_SIZE                  8

 /* No padding needed */
#define ETH_PAD_SIZE                    0

#define IP_SOF_BROADCAST				1
#define IP_SOF_BROADCAST_RECV			1

/* The ethernet FCS is performed in hardware. The IP, TCP, and UDP
   CRCs still need to be done in hardware. */
#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_CHECK_IP               1
#define CHECKSUM_CHECK_UDP              1
#define CHECKSUM_CHECK_TCP              1
#define LWIP_CHECKSUM_ON_COPY           1

/* Use LWIP version of htonx() to allow generic functionality across
   all platforms. If you are using the Cortex Mx devices, you might
   be able to use the Cortex __rev instruction instead. */
#define LWIP_PLATFORM_BYTESWAP			0

/* Non-static memory, used with DMA pool */
#define MEM_SIZE						(10*1024)

/* Raw interface not needed */
#define LWIP_RAW                        0

/* DHCP is ok, UDP is required with DHCP */
#define LWIP_DHCP                       0
#define LWIP_UDP                        1

/* Hostname can be used */
#define LWIP_NETIF_HOSTNAME             1

#define LWIP_BROADCAST_PING				1

/* MSS should match the hardware packet size */
#define TCP_MSS                         1460
#define TCP_SND_BUF						(2 * TCP_MSS)

#if NO_SYS
#define LWIP_SOCKET                     0
#define LWIP_NETCONN                    0
#else
#define LWIP_SOCKET                     1
#define LWIP_NETCONN                    1
#endif

#define MEMP_NUM_SYS_TIMEOUT            300

#define LWIP_STATS                      0
#define LINK_STATS						0
#define LWIP_STATS_DISPLAY              0


/* There are more *_DEBUG options that can be selected.
   See opts.h. Make sure that LWIP_DEBUG is defined when
   building the code to use debug. */
#define LWIP_DEBUG

#define TCP_DEBUG                       LWIP_DBG_ON
#define ETHARP_DEBUG                    LWIP_DBG_ON
#define PBUF_DEBUG                      LWIP_DBG_ON
#define IP_DEBUG                        LWIP_DBG_ON
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_ON

/* This define is custom for the LPC EMAC driver. Enabled it to
   get debug messages for the driver. */
#define UDP_LPC_EMAC                    LWIP_DBG_OFF


#define DEFAULT_THREAD_PRIO             (6)
#define DEFAULT_THREAD_STACKSIZE        (1024)
#define DEFAULT_ACCEPTMBOX_SIZE         6
#define DEFAULT_ACCEPTMBOX_SIZE         6
#define DEFAULT_TCP_RECVMBOX_SIZE       6
#define DEFAULT_UDP_RECVMBOX_SIZE       6

/* TCPIP thread must run at higher priority than MAC threads! */
#define TCPIP_THREAD_PRIO               (5)
#define TCPIP_THREAD_STACKSIZE          (1024)
#define TCPIP_MBOX_SIZE                 6


/**
 * @}
 */

#endif /* __LWIP_USER_OPT_H__ */




