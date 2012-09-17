/*
 * demo_usart_cli.c
 *
 *  Created on: 2012-9-17
 *      Author: YangZhiyong
 */
#include "launcher.h"

void USART_CLI(void *p_arg);

APP_TASK_DEFINE(USART_CLI,     /*name name##_TCB*/
                USART_CLI,     /*p_task*/
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
//    FIFO_Put(&USART_FIFO, &byte, 1);
    FIFO_PutByte(&USART_FIFO, byte);
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




void USART_CLI(void *p_arg)
{
    (void)p_arg;

    USART_SetRecvByteDoneISR(onRecvByteDone);
    USART_SetRecvTimeoutISR(onRecvTimeout);
    USART_SetSendDoneISR(onSendDone);



}




