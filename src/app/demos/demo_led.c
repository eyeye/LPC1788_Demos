/*
 * demo_led.c
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */

#include "hal/hal_led.h"


void demo_led(void)
{
    LED_Init();

    while(1)
    {
        LED_On(0);
        LED_On(1);
        LED_On(2);
        LED_On(3);

        LED_Off(0);
        LED_Off(1);
        LED_Off(2);
        LED_Off(3);
    }
}







