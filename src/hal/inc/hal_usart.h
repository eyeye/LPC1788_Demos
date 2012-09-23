/*
 * hal_usart.h
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

#ifdef __cplusplus
extern "C" {
#endif



#include <stdint.h>


/**
 * ��������жϷ�������
 */
typedef void (*USART_SendDoneISR)(void);

/**
 * �����ֽ�����жϷ�������
 * @param byte ���յ����ֽ�����
 */
typedef void (*USART_RecvByteDoneISR)(uint8_t byte);

/**
 * �������ݳ�ʱ( �������Ͽ�����һ��ʱ�� )�жϷ�������
 */
typedef void (*USART_RecvTimeoutISR)(void);


/**
 * ��ʼ������
 */
void USART_Init(void);

/**
 * �ֽڷ���
 * @param byte  �����ֽ�
 */
void USART_SendByte(uint8_t byte);

/**
 * ���巢��
 * @param buffer    ����ָ��
 * @param length    ���峤��
 */
void USART_Send(void* buffer, uint32_t length);

/**
 * ���ý����ֽ�����жϷ���
 * @param isr   �жϷ�����
 */
void USART_SetRecvByteDoneISR   (USART_RecvByteDoneISR isr);

/**
 *
 * @param isr
 */
void USART_SetRecvTimeoutISR    (USART_RecvTimeoutISR isr);

/**
 *
 * @param isr
 */
void USART_SetSendDoneISR       (USART_SendDoneISR isr);


#ifdef __cplusplus
}
#endif

#endif /* HAL_USART_H_ */



