/*
 * hal_led.c
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */


#include "hal_led.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"



void LED_Init(void)
{
    GPIO_Init();

    PINSEL_ConfigPin(0, 17, 0);
    PINSEL_ConfigPin(0, 19, 0);
    PINSEL_ConfigPin(0, 20, 0);
    PINSEL_ConfigPin(0, 22, 0);

    GPIO_SetDir(0, 1<<17, 1);
    GPIO_SetDir(0, 1<<19, 1);
    GPIO_SetDir(0, 1<<20, 1);
    GPIO_SetDir(0, 1<<22, 1);

    GPIO_ClearValue(0, 1<<17);
    GPIO_ClearValue(0, 1<<19);
    GPIO_ClearValue(0, 1<<20);
    GPIO_ClearValue(0, 1<<22);

}


void LED_On(uint8_t led)
{
    switch(led)
    {
    case 0:     GPIO_ClearValue(0, 1<<17);  break;
    case 1:     GPIO_ClearValue(0, 1<<19);  break;
    case 2:     GPIO_ClearValue(0, 1<<20);  break;
    case 3:     GPIO_ClearValue(0, 1<<22);  break;
    default:    break;
    }
}


void LED_Off(uint8_t led)
{
    switch(led)
    {
    case 0:     GPIO_SetValue(0, 1<<17);  break;
    case 1:     GPIO_SetValue(0, 1<<19);  break;
    case 2:     GPIO_SetValue(0, 1<<20);  break;
    case 3:     GPIO_SetValue(0, 1<<22);  break;
    default:    break;
    }
}




