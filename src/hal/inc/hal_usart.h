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
 * 发送完成中断服务类型
 */
typedef void (*USART_SendDoneISR)(void);

/**
 * 接收字节完成中断服务类型
 * @param byte 接收到的字节数据
 */
typedef void (*USART_RecvByteDoneISR)(uint8_t byte);

/**
 * 接收数据超时( 数据流断开超过一定时间 )中断服务类型
 */
typedef void (*USART_RecvTimeoutISR)(void);


/**
 * 初始化配置
 */
void USART_Init(void);

/**
 * 字节发送
 * @param byte  发送字节
 */
void USART_SendByte(uint8_t byte);

/**
 * 缓冲发送
 * @param buffer    缓冲指针
 * @param length    缓冲长度
 */
void USART_Send(void* buffer, uint32_t length);

/**
 * 设置接收字节完成中断服务
 * @param isr   中断服务函数
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



