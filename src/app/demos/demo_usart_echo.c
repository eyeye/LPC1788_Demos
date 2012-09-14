



#include "launcher.h"
#include "hal/hal_usart.h"



void USART_Echo(void *p_arg);

APP_TASK_DEFINE(USART_Echo,      /*name name##_TCB*/
                USART_Echo, /*p_task*/
                0,              /*p_arg*/
                4,              /*prio*/
                128,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)






void onRecvByteDone(uint8_t byte)
{

}


void onRecvTimeout(void)
{
    OS_ERR  err;
    OSTaskSemPost(&USART_Echo_TCB,
                  OS_OPT_POST_NONE,
                  &err);
}


void onSendDone(void)
{

}



void USART_Echo(void *p_arg)
{
    OS_ERR  err;

    USART_SetRecvByteDoneISR(onRecvByteDone);
    USART_SetRecvTimeoutISR(onRecvTimeout);
    USART_SetSendDoneISR(onSendDone);

    while(1)
    {
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, (void*)0, &err);

    }

}














