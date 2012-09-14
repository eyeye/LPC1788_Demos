/*
 * demo0_spi.c
 *
 *  Created on: 2012-7-12
 *      Author: YangZhiyong
 */



#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"


// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;


void demo_spi(void)
{

    PINSEL_ConfigPin(0, 19, 0);     // I2C1_SDA     A0
    PINSEL_ConfigPin(0, 20, 0);     // I2C1_SCL     RST

    PINSEL_ConfigPin(2, 22, 2);     // SSP0_SCK

    PINSEL_ConfigPin(2, 23, 0);     // SSP0_SSEL

    PINSEL_ConfigPin(2, 26, 2);     // SSP0_MISO
    PINSEL_ConfigPin(2, 27, 2);     // SSP0_MOSI

    GPIO_SetDir(0, 1<<19, 1);
    GPIO_SetDir(0, 1<<20, 1);
    GPIO_SetDir(2, 1<<23, 1);

    // initialize SSP configuration structure to default
    SSP_ConfigStructInit(&SSP_ConfigStruct);
    // Initialize SSP peripheral with parameter given in structure above
    SSP_Init(LPC_SSP0, &SSP_ConfigStruct);

    // Enable SSP peripheral
    SSP_Cmd(LPC_SSP0, ENABLE);

    while(1)
    {


    }

}



