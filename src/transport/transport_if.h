/*
 * transport_if.h
 *
 *  Created on: 2012-9-4
 *      Author: YangZhiyong
 */

#ifndef TRANSPORT_IF_H_
#define TRANSPORT_IF_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef void (*TRANSPORT_EventHandler)(void);



/**
 * ͨ��״̬
 */
enum TRANSPORT_Status
{
    TRANSPORT_Idle      = 0x00,//!< ����
    TRANSPORT_RecvBusy  = 0x01,//!< ����æ
    TRANSPORT_RecvDone  = 0x02,//!< �������
    TRANSPORT_SendBusy  = 0x04,//!< ����æ
    TRANSPORT_SendDone  = 0x08,//!< �������
};


/**
 * ͨ���¼�
 */
enum TRANSPORT_Event
{
    TRANSPORT_Event_RecvDone,//!< ��������¼�
    TRANSPORT_Event_SendDone,//!< ��������¼�
    TRANSPORT_Event_Error,   //!< �����¼�
};



/**
 * ͨ�Žӿ�(IF = Interface)
 */
struct TRANSPORT_IF
{
    /**
     * �ӿ�
     */
    char* name;


    /**
     * ��ʼ���ӿ�
     * @param config �ӿ����ò������ɽӿ�ʵ���߾��嶨��
     * @return
     */
    bool init(void* config);


    /**
     * ���ͺ���
     * @param buffer    ���ͻ����ַ
     * @param length    ���ͳ���
     * @return
     */
    uint32_t send(void* buffer, uint32_t length);


    /**
     * ���պ���
     * @param buffer    ���ջ����ַ
     * @param length    ���ճ���
     * @return
     */
    uint32_t recv(void* buffer, uint32_t length);


    /**
     * �����¼�����ص�����
     * @param event     ��Ҫ������¼�
     * @param handler   �¼�����ص�����
     */
    void setEventHandler(enum TRANSPORT_Event event,
                         TRANSPORT_EventHandler handler);


    /**
     * ��ȡ�ӿ�״̬
     * @return
     */
    enum TRANSPORT_Status   status(void);

};




#ifdef __cplusplus
}
#endif

#endif /* TRANSPORT_IF_H_ */



