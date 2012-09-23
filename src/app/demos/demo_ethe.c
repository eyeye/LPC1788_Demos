/*
 * demo_ethe.c
 *
 *  Created on: 2012-9-21
 *      Author: EYE
 */

#include "service/launcher.h"
#include "lwip/ip_addr.h"
#include "lwip/tcpip.h"
#include "lwip/debug.h"
#include "core_cm3.h"
#include "LPC177x_8x.h"
#include <string.h>
#include "lpc17_emac.h"
#include "tcpecho.h"
#include "lpc_phy.h"
#include "hal_led.h"

void ETHE_Task(void *p_arg);

APP_TASK_DEFINE(ETHE_Task,      /*name name##_TCB*/
                ETHE_Task, /*p_task*/
                0,              /*p_arg*/
                4,              /*prio*/
                128,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)

static struct netif lpc_netif;

/* Callback for TCPIP thread to indicate TCPIP init is done */
static void tcpip_init_done_signal(void *arg)
{
    /* Tell main thread TCP/IP init is done */
    *(s32_t *) arg = 1;
}

void ETHE_Task(void *p_arg)
{
    (void)p_arg;

    ip_addr_t ipaddr, netmask, gw;
    volatile s32_t tcpipdone = 0;

    /* Wait until the TCP/IP thread is finished before
       continuing or wierd things may happen */
    LWIP_DEBUGF(LWIP_DBG_ON, ("Waiting for TCPIP thread to initialize...\n"));
    tcpip_init(tcpip_init_done_signal, (void*)&tcpipdone);
    while (!tcpipdone)
    {
        sys_msleep(20);
    }

    LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP TCP echo server...\n"));

    /* Static IP assignment */
#if LWIP_DHCP
    IP4_ADDR(&gw, 0, 0, 0, 0);
    IP4_ADDR(&ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
    IP4_ADDR(&gw, 192, 168, 1, 1);
    IP4_ADDR(&ipaddr, 192, 168, 1, 234);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

    /* Add netif interface for lpc17xx_8x */
    memset((void*)&lpc_netif, 0, sizeof(lpc_netif));
    if (!netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init,
        tcpip_input))
        LWIP_ASSERT("Net interface failed to initialize\r\n", 0);

    netif_set_default(&lpc_netif);
    netif_set_up(&lpc_netif);

    /* Enable MAC interrupts */
    NVIC_SetPriority(ENET_IRQn, ((0x01 << 3) | 0x01));
    NVIC_EnableIRQ(ENET_IRQn);

#if LWIP_DHCP
    dhcp_start(&lpc_netif);
#endif

    /* Initialize and start application */
    tcpecho_init();

    /* This loop monitors the PHY link and will handle cable events
       via the PHY driver. */
    while (1)
    {
        /* Call the PHY status update state machine once in a while
           to keep the link status up-to-date */
        if (lpc_phy_sts_sm(&lpc_netif) != 0) {
            /* Set the state of the LED to on if the ethernet link is
               active or off is disconnected. */
            if (lpc_netif.flags & NETIF_FLAG_LINK_UP)
                LED_On(3);
            else
                LED_Off(3);
        }

        /* Non-blocking delay for link detection */
        sys_msleep(250);
    }
}



/* Populates the MAC address for the board */
void board_get_macaddr(u8_t *macaddr)
{
    macaddr[0] = 0x0A;
    macaddr[1] = 0x0A;
    macaddr[2] = 0x0A;
    macaddr[3] = 0x0A;
    macaddr[4] = 0x0A;
    macaddr[5] = 0x0A;
}



