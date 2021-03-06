/*
 * demo_ucos.c
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */

#include <os.h>
#include "launcher.h"
#include "hal_led.h"
#include "system_LPC177x_8x.h"

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/
static    OS_TCB       AppTaskStart_TCB;
static    CPU_STK      AppTaskStart_Stk[APP_TASK_START_STK_SIZE];
static    CPU_INT32U   AppCPU_ClkFreq_Hz;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


static  void         AppTaskStart       (void *p_arg);



void demo_ucos(void)
{
    OS_ERR  err;

//    SystemInit();
//    BSP_IntDisAll();                                            /* Disable all interrupts.                              */
    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStart_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR )AppTaskStart,
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStart_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}



/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart_()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

//    BSP_Init();                                                       /* Initialize BSP functions                          */
    CPU_Init();                                                       /* Initialize the uC/CPU services                    */

//    AppCPU_ClkFreq_Hz = BSP_CPU_ClkFreq();                            /* Determine SysTick reference freq.                 */
//    cnts          = AppCPU_ClkFreq_Hz / (CPU_INT32U)OSCfg_TickRate_Hz;/* Determine nbr SysTick increments                  */
    OS_CPU_SysTickInit(SystemCoreClock/OSCfg_TickRate_Hz);                                        /* Init uC/OS periodic time src (SysTick).           */

//#if OS_CFG_STAT_TASK_EN > 0u
//    OSStatTaskCPUUsageInit(&err);                                     /* Compute CPU capacity with no task running         */
//#endif
//
//#ifdef  CPU_CFG_INT_DIS_MEAS_EN
//    CPU_IntDisMeasMaxCurReset();
//#endif

    Launcher_Exec();

    while (DEF_TRUE)
    {                                                /* Task body, always written as an infinite loop.    */
        OSTimeDlyHMSM(0, 0, 0, 500,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        LED_On(0);

        OSTimeDlyHMSM(0, 0, 0, 500,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        LED_Off(0);
    }

}


////////////////////////////////////////////////////////////////

void LED_Task1(void *p_arg)
{
    OS_ERR      err;

    (void)p_arg;


    while(1)
    {
        OSTimeDlyHMSM(0, 0, 0, 700,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
        LED_On(1);

        OSTimeDlyHMSM(0, 0, 0, 700,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        LED_Off(1);
    }

}




APP_TASK_DEFINE(LED_Task1,          /*name*/
                LED_Task1,   /*p_task*/
                0,              /*p_arg*/
                4,              /*prio*/
                64,            /*stk_size*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)




////////////////////////////////////////////////////////////////

void LED_Task2(void *p_arg)
{
    OS_ERR      err;

    (void)p_arg;


    while(1)
    {
        OSTimeDlyHMSM(0, 0, 0, 200,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
        LED_On(2);

        OSTimeDlyHMSM(0, 0, 0, 200,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        LED_Off(2);
    }

}




//APP_TASK_DEFINE(LED_Task2,      /*name*/
//                LED_Task2,      /*p_task*/
//                0,              /*p_arg*/
//                4,              /*prio*/
//                64,           /*stk_size*/
//                0,              /*q_size*/
//                0,              /*time_quanta*/
//                0,              /*p_ext*/
//                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
//)


////////////////////////////////////////////////////////////////

void LED_Task3(void *p_arg)
{
    OS_ERR      err;

    (void)p_arg;


    while(1)
    {
        OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
        LED_On(3);

        OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        LED_Off(3);
    }

}




//APP_TASK_DEFINE(LED_Task3,      /*name*/
//                LED_Task3,      /*p_task*/
//                0,              /*p_arg*/
//                4,              /*prio*/
//                64,           /*stk_size_32*/
//                0,              /*q_size*/
//                0,              /*time_quanta*/
//                0,              /*p_ext*/
//                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
//)


/////////////////////////////////////////////////////////////////////

