/*
 * hal_ethe.c
 *
 *  Created on: 2012-8-31
 *      Author: YangZhiyong
 */


#include "hal_ethe.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc17_emac.h"


void ETHE_Init(void)
{
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

    /* Configure ethernet pin muxing as follows:
       RMII_TXD0     -> P1.0    -> FUNC1
       RMII_TXD1     -> P1.1    -> FUNC1
       RMII_TXEN     -> P1.4    -> FUNC1
       RMII_RXD0     -> P1.9    -> FUNC1
       RMII_RXD1     -> P1.10   -> FUNC1
       RMII_CRS      -> P1.8    -> FUNC1
       RMII_RXER     -> P1.14   -> FUNC1
       RMII_MDIO     -> P1.17   -> FUNC1
       RMII_MCD      -> P1.16   -> FUNC1
       RMII_CLK      -> P1.15   -> FUNC1
    */
    PINSEL_ConfigPin(1, 0, 1);
    PINSEL_ConfigPin(1, 1, 1);
    PINSEL_ConfigPin(1, 4, 1);
    PINSEL_ConfigPin(1, 9, 1);
    PINSEL_ConfigPin(1, 10, 1);
    PINSEL_ConfigPin(1, 8, 1);
    PINSEL_ConfigPin(1, 14, 1);
    PINSEL_ConfigPin(1, 17, 1);
    PINSEL_ConfigPin(1, 16, 1);
    PINSEL_ConfigPin(1, 15, 1);

    PINSEL_ConfigPin(4, 15, 0);

    GPIO_SetDir(4, 15, 1);

    GPIO_SetValue(4, 1<<15);
    for( int i = 0; i < 10000; i++ );
    GPIO_ClearValue(4, 1<<15);
    for( int i = 0; i < 10000; i++ );
    GPIO_SetValue(4, 1<<15);

//    struct netif lpc_netif;
//    lpc_enetif_init(&lpc_netif);
}


