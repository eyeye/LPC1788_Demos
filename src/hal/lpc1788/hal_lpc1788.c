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
#include "debug_frmwrk.h"
#include "hal_ethe.h"


void HAL_Init(void)
{
    PIN_Init();
    LED_Init();
//    USART_Init();
    debug_frmwrk_init();

    ETHE_Init();
}


