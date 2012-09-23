
/*
 * demos.c
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */


#include <intrinsics.h>
#include "system_LPC177x_8x.h"
#include "hal.h"

extern void demo_led(void);
extern void demo_ucos(void);

void main(void)
{
    SystemInit();
    HAL_Init();
    demo_ucos();
}


