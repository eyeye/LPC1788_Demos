/*
 * hal.c
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */

#include "hal.h"
#include "hal_led.h"
#include "hal_usart.h"
#include "hal_pin.h"


void HAL_Init(void)
{
    PIN_Init();
    LED_Init();
    USART_Init();
}


