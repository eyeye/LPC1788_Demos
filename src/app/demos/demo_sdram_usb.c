
/*
 * demo_sdram_usb.c
 *
 *  Created on: 2012-9-6
 *      Author: YangZhiyong
 */


#include "launcher.h"
#include "usb.h"
#include "lpc_types.h"
#include "usbhw.h"
#include "hal_led.h"
#include "hal_extbus.h"


static volatile uint32_t USB_PowerStatus;

void USB_SDRAM_Disk(void *p_arg);

//APP_TASK_DEFINE(USB_SDRAM,      /*name name##_TCB*/
//                USB_SDRAM_Disk, /*p_task*/
//                0,              /*p_arg*/
//                4,              /*prio*/
//                128,            /*stk_size_32*/
//                0,              /*q_size*/
//                0,              /*time_quanta*/
//                0,              /*p_ext*/
//                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
//)




void SDRAM_Test(void)
{
    uint32_t address;
    uint32_t index;

    address = SDRAM_BASE_ADDR;
//    index    = SDRAM_SIZE;

    for( index = 0; index != SDRAM_SIZE/4; index++ )
    {
        *(uint32_t*)address = index;
        address += 4;
    }

    address = SDRAM_BASE_ADDR;

    for( index = 0; index != SDRAM_SIZE/4; index++ )
    {
        if( *(uint32_t*)address != index )
        {
            while(1)
            {
                LED_On(0);
                LED_On(1);
                LED_On(2);
                LED_On(3);

                for( int i = 0; i < 1000000; i++);

                LED_Off(0);
                LED_Off(1);
                LED_Off(2);
                LED_Off(3);

                for( int i = 0; i < 1000000; i++);
            }
        }
        address += 4;
    }


    return;
}





void USB_SDRAM_Disk(void *p_arg)
{
    OS_ERR err;
    OS_SEM_CTR sem_cnt;

    (void)p_arg;

    ExtBus_Init();
    SDRAM_Test();

//    for (n = 0; n < MSC_ImageSize; n++) {     /* Copy Initial Disk Image */
//        Memory[n] = DiskImage[n];               /*   from Flash to RAM     */
//    }

//    BSP_IntVectSet((CPU_INT08U   )BSP_INT_ID_USB,
//                   (CPU_FNCT_VOID)USB_IRQHandler);
//    BSP_IntPrioSet (BSP_INT_ID_USB, 3);
//    BSP_IntEn (BSP_INT_ID_USB);

    USB_Init();                               /* USB Initialization */
    USB_Connect(TRUE);                        /* USB Connect */

    while (1)
    {
//        OSTimeDlyHMSM(0, 0, 0, 200,
//                      OS_OPT_TIME_HMSM_STRICT,
//                      &err);

        if( USB_PowerStatus )
        {

            LED_Off(2);
            sem_cnt = OSTaskSemPend( 300, OS_OPT_PEND_BLOCKING, 0, &err);
            LED_On(2);
            sem_cnt = OSTaskSemPend( 300, OS_OPT_PEND_BLOCKING, 0, &err);
        }
        else
        {
            LED_Off(2);
            sem_cnt = OSTaskSemPend( 0, OS_OPT_PEND_BLOCKING, 0, &err);
        }

    }

}



//void USB_Power_Event (uint32_t  power)
//{
//    OS_ERR error;
//
//    OSTaskSemPost (&USB_SDRAM_TCB, OS_OPT_POST_NO_SCHED, &error);
//    USB_PowerStatus = power;
//}






