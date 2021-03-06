/*
 * hal_usart.c
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */



#include "hal_usart.h"
#include "lpc177x_8x_uart.h"
#include "LPC177x_8x.h"
#include "lpc177x_8x_pinsel.h"

#define USART_STRUCT        LPC_UART0
#define USART_IRQ           UART0_IRQn
#define USART_ID            UART_0
#define USART_FIFO_SIZE     16
#define USART_FIFO_LEVEL    8           /// 1, 4, 8, 14


static USART_RecvByteDoneISR    RecvByteDoneISR = (void*)0;
static USART_RecvTimeoutISR     RecvTimeoutISR  = (void*)0;
static USART_SendDoneISR        SendDoneISR     = (void*)0;

volatile static uint8_t txFifoRemain  = USART_FIFO_SIZE;        // ����FIFO������


volatile static uint8_t* sendPtr;
volatile static uint32_t sendCnt;



static void fillTxFifo(void)
{
    while( (sendCnt > 0) && (txFifoRemain > 0) )
    {
        USART_STRUCT->THR = (*sendPtr) & UART_THR_MASKBIT;
        sendPtr ++;
        sendCnt --;
        txFifoRemain --;
    }

}



static void dumpRxFifo(void)
{
    uint8_t recv_byte;
    uint8_t lsr;

    if( RecvByteDoneISR == 0 )
    {
        return ;
    }


    lsr = (USART_STRUCT->LSR) & UART_LSR_BITMASK;

    while( lsr & UART_LSR_RDR )
    {
        recv_byte = USART_STRUCT->RBR & UART_RBR_MASKBIT;
        RecvByteDoneISR(recv_byte);

        lsr = (USART_STRUCT->LSR) & UART_LSR_BITMASK;
    }
}



static void TxFifoEmpty_ISR(void)
{
    txFifoRemain = USART_FIFO_SIZE;

    fillTxFifo();

    if( txFifoRemain == USART_FIFO_SIZE )
    {
        if( SendDoneISR != 0 )
        {
            SendDoneISR();
        }
    }

}



static void RxTimeout_ISR(void)
{
    dumpRxFifo();
    if( RecvTimeoutISR != 0 )
    {
        RecvTimeoutISR();
    }
}



static void RxFifoReady_ISR(void)
{
    dumpRxFifo();
}



/****************************************************************
 *
 */



void UART0_IRQHandler(void)
{
    uint32_t iir;
    uint8_t  intId;

    iir = (USART_STRUCT->IIR) & UART_IIR_BITMASK;

    intId = iir & UART_IIR_INTID_MASK;

    if( intId == UART_IIR_INTID_RLS )
    {//
        uint8_t lsr;
        lsr = (USART_STRUCT->LSR) & UART_LSR_BITMASK;
    }

    if( intId == UART_IIR_INTID_RDA )
    {
        RxFifoReady_ISR();
    }

    if( intId == UART_IIR_INTID_CTI )
    {
        RxTimeout_ISR();
    }

    if( intId == UART_IIR_INTID_THRE )
    {
        TxFifoEmpty_ISR();
    }

}





void USART_Init(void)
{
    UART_CFG_Type UARTConfigStruct;
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;

    PINSEL_ConfigPin(0,2,1);
    PINSEL_ConfigPin(0,3,1);

    UART_ConfigStructInit(&UARTConfigStruct);

    UART_Init(USART_ID, &UARTConfigStruct);

    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
#if     USART_FIFO_LEVEL == 1
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV0;
#elif   USART_FIFO_LEVEL == 4
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV1;
#elif   USART_FIFO_LEVEL == 8
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV2;
#elif   USART_FIFO_LEVEL == 14
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV3;
#endif

    UART_FIFOConfig(USART_ID, &UARTFIFOConfigStruct);


    UART_TxCmd(USART_ID, ENABLE);
    UART_IntConfig(USART_ID, UART_INTCFG_RBR, ENABLE);
    UART_IntConfig(USART_ID, UART_INTCFG_RLS, ENABLE);
    UART_IntConfig(USART_ID, UART_INTCFG_THRE, ENABLE);

    NVIC_SetPriority(USART_IRQ, ((0x01<<3)|0x01));

    NVIC_EnableIRQ(USART_IRQ);
}



void USART_SendByte(uint8_t byte)
{
    while( txFifoRemain > 0 );

    USART_STRUCT->THR = byte & UART_THR_MASKBIT;

    txFifoRemain --;
}



void USART_Send(void* buffer, uint32_t length)
{
    sendPtr = buffer;
    sendCnt = length;
    fillTxFifo();
}



void USART_SetSendDoneISR   (USART_SendDoneISR isr)
{
    SendDoneISR = isr;
}



void USART_SetRecvByteDoneISR   (USART_RecvByteDoneISR isr)
{
    RecvByteDoneISR = isr;
}



void USART_SetRecvTimeoutISR    (USART_RecvTimeoutISR isr)
{
    RecvTimeoutISR = isr;
}



