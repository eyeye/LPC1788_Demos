/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/*  Porting by Michael Vysotsky <michaelvy@hotmail.com> August 2011   */

#define SYS_ARCH_GLOBALS



/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

#include "sys_arch.h"
#include "os_type.h"
#include "app_cfg.h"
#include "os_cfg_app.h"
#include <string.h>
#include "lwip/err.h"
/*----------------------------------------------------------------------------*/
/*                      DEFINITIONS                                           */
/*----------------------------------------------------------------------------*/
#define LWIP_ARCH_TICK_PER_MS       (1000/OS_CFG_TICK_RATE_HZ)


#if NO_SYS

#include "lpc_arch.h"

/* Returns the current time in mS. This is needed for the LWIP timers */
u32_t sys_now(void)
{
  return (u32_t) SysTick_GetMS();
}


#else
/*----------------------------------------------------------------------------*/
/*                      VARIABLES                                             */
/*----------------------------------------------------------------------------*/
static OS_MEM StackMem;

const void * const pvNullPointer = (mem_ptr_t*)0xffffffff;

#pragma data_alignment=8
CPU_STK       LwIP_Task_Stk[LWIP_TASK_MAX*LWIP_STK_SIZE];

#pragma data_alignment=8
//CPU_INT08U    LwIP_task_priopity_stask[LWIP_TASK_MAX];
OS_TCB        LwIP_Task_TCB[LWIP_TASK_MAX];


/*----------------------------------------------------------------------------*/
/*                      PROTOTYPES                                            */
/*----------------------------------------------------------------------------*/
/*--------------------Creates an empty mailbox.-------------------------------*/


err_t sys_mbox_new( sys_mbox_t *mbox, int size)
{
    OS_ERR       ucErr;

    OSQCreate(mbox, "LWIP queue", size, &ucErr);
    LWIP_ASSERT( "OSQCreate ", ucErr == OS_ERR_NONE );

    if( ucErr == OS_ERR_NONE)
    {
        return ERR_OK;
    }
    return ERR_MEM;
}



/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
 */
void
sys_mbox_free(sys_mbox_t * mbox)
{
    OS_ERR     ucErr;
    LWIP_ASSERT( "sys_mbox_free ", mbox != SYS_MBOX_NULL );

    OSQFlush(mbox,& ucErr);

    OSQDel(mbox, OS_OPT_DEL_ALWAYS, &ucErr);
    LWIP_ASSERT( "OSQDel ", ucErr == OS_ERR_NONE );
}



/*-----------------------------------------------------------------------------------
 *   Posts the "msg" to the mailbox.
 */
void
sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    OS_ERR     ucErr;
    CPU_INT08U  i=0;
    if( msg == NULL ) msg = (void*)&pvNullPointer;
    /* try 10 times */
    while(i<10){
        OSQPost(mbox, msg, 0, OS_OPT_POST_ALL, &ucErr);
        if(ucErr == OS_ERR_NONE)
            break;
        i++;
        OSTimeDly(5, OS_OPT_TIME_DLY, &ucErr);
    }
    LWIP_ASSERT( "sys_mbox_post error!\n", i !=10 );
}



/* Try to post the "msg" to the mailbox. */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    OS_ERR     ucErr;
    if(msg == NULL ) msg = (void*)&pvNullPointer;
    OSQPost(mbox, msg,0,OS_OPT_POST_ALL,&ucErr);
    if(ucErr != OS_ERR_NONE)
    {
        return ERR_MEM;
    }
    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP.
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    OS_ERR	ucErr;
    OS_MSG_SIZE   msg_size;
    CPU_TS        ucos_timeout;
    CPU_TS        in_timeout = timeout/LWIP_ARCH_TICK_PER_MS;

    if(timeout && in_timeout == 0)
        in_timeout = 1;
    *msg  = OSQPend (mbox,in_timeout,OS_OPT_PEND_BLOCKING,&msg_size,
                    &ucos_timeout,&ucErr);

    if ( ucErr == OS_ERR_TIMEOUT )
        ucos_timeout = SYS_ARCH_TIMEOUT;
    return ucos_timeout;
}



/**
 * Check if an mbox is valid/allocated:
 * @param sys_mbox_t *mbox pointer mail box
 * @return 1 for valid, 0 for invalid
 */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    if(mbox->NamePtr)
        return (strcmp(mbox->NamePtr,"?Q"))? 1:0;
    else
        return 0;
}



/**
 * Set an mbox invalid so that sys_mbox_valid returns 0
 */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    if(sys_mbox_valid(mbox))
        sys_mbox_free(mbox);
}
/*
 *  Creates and returns a new semaphore. The "count" argument specifies
 *  the initial state of the semaphore. TBD finish and test
 */

err_t sys_sem_new(sys_sem_t * sem, u8_t count)
{
    OS_ERR	ucErr;
    OSSemCreate (sem,"LWIP Sem",count,&ucErr);
    if(ucErr != OS_ERR_NONE ){
        LWIP_ASSERT("OSSemCreate ",ucErr == OS_ERR_NONE );
        return -1;
    }
    return 0;
}
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 */
u32_t
sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    OS_ERR	ucErr;
    CPU_TS        ucos_timeout;
    CPU_TS        in_timeout = timeout/LWIP_ARCH_TICK_PER_MS;
    if(timeout && in_timeout == 0)
        in_timeout = 1;
    OSSemPend (sem,in_timeout,OS_OPT_PEND_BLOCKING,&ucos_timeout,&ucErr);
    /*  only when timeout! */
    if(ucErr == OS_ERR_TIMEOUT)
        ucos_timeout = SYS_ARCH_TIMEOUT;
    return ucos_timeout;
}

/*
 *       Signals a semaphore
 */

void
sys_sem_signal(sys_sem_t *sem)
{
    OS_ERR	ucErr;
    OSSemPost(sem,OS_OPT_POST_ALL,&ucErr);
    LWIP_ASSERT("OSSemPost ",ucErr == OS_ERR_NONE );
}



/*
 *      Deallocates a semaphore
 */
void
sys_sem_free(sys_sem_t *sem)
{
    OS_ERR     ucErr;
    OSSemDel(sem, OS_OPT_DEL_ALWAYS, &ucErr );
    LWIP_ASSERT( "OSSemDel ", ucErr == OS_ERR_NONE );
}



int sys_sem_valid(sys_sem_t *sem)
{
    if( sem->Type == OS_OBJ_TYPE_SEM )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/** Set a semaphore invalid so that sys_sem_valid returns 0 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    sys_sem_free(sem);
}


/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new( sys_mutex_t *pxMutex )
{
    OS_ERR error;
    OSMutexCreate(pxMutex, "sys_mutex_new", &error);

    if( error == OS_ERR_NONE)
        return ERR_OK;
    else
        return ERR_MEM;
}



/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock( sys_mutex_t *pxMutex )
{
    OS_ERR error;
    OSMutexPend(pxMutex, 0, OS_OPT_PEND_BLOCKING, 0, &error);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock( sys_mutex_t *pxMutex )
{
    OS_ERR error;
    OSMutexPost(pxMutex, OS_OPT_PEND_BLOCKING, &error);
}


/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free( sys_mutex_t *pxMutex )
{
    OS_ERR error;
    OSMutexDel(pxMutex, OS_OPT_DEL_ALWAYS, &error);
}




/*-----------------------------------------------------------------------------------*/
/*            memory interface                                                       */
/*-----------------------------------------------------------------------------------*/


/*
 * Initialize sys arch
 * sys_init() must be called before anthing else.
 */
void sys_init(void)
{
    OS_ERR ucErr;

    for( int i = 0; i < LWIP_TASK_MAX; ++i)
    {
        OS_TaskInitTCB(&LwIP_Task_TCB[i]);
    }

__WFI();
    /* init mem used by sys_mbox_t, use ucosII functions */
    OSMemCreate(&StackMem,"LWIP TASK STK",(void*)LwIP_Task_Stk, LWIP_TASK_MAX, LWIP_STK_SIZE*sizeof(CPU_STK), &ucErr);
    LWIP_ASSERT( "sys_init: failed OSMemCreate STK", ucErr == OS_ERR_NONE );
}



/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
// TBD
/*-----------------------------------------------------------------------------------*/
/*
  Starts a new thread with priority "prio" that will begin its execution in the
  function "thread()". The "arg" argument will be passed as an argument to the
  thread() function. The id of the new thread is returned. Both the id and
  the priority are system dependent.

 * The only thread function:
 * Creates a new thread
 * @param name human-readable name for the thread (used for debugging purposes)
 * @param thread thread-function
 * @param arg parameter passed to 'thread'
 * @param stacksize stack size in bytes for the new thread (may be ignored by ports)
 * @param prio priority of the new thread (may be ignored by ports) */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{

    OS_ERR      error;

    /// get Stack
    if(stacksize > LWIP_STK_SIZE || !stacksize)
        stacksize = LWIP_STK_SIZE;

    CPU_STK * task_stk = OSMemGet( &StackMem, &error );
    if(error != OS_ERR_NONE)
    {
        return (sys_thread_t)0;
    }

    /// get TCB
    int idx;
    for( idx = 0; idx < LWIP_TASK_MAX; ++idx)
    {
        if( LwIP_Task_TCB[idx].StkPtr == (CPU_STK*)0 )
        {
            break;
        }
    }

    if( idx >= LWIP_TASK_MAX )
    {
        return (sys_thread_t)0;
    }

    /// create thread
    OSTaskCreate(&LwIP_Task_TCB[idx],
                 (CPU_CHAR  *)name,
                 (OS_TASK_PTR)thread,
                 (void      *)arg,
                 (OS_PRIO    )prio,
                 (CPU_STK   *)&task_stk[0],
                 (OS_STK_SIZE)stacksize/10,
                 (OS_STK_SIZE)stacksize,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&error);

    if( error == OS_ERR_NONE)
        return &LwIP_Task_TCB[idx];
    else
        return (sys_thread_t)0;
}



/**
 * Sleep for some ms. Timeouts are NOT processed while sleeping.
 *
 * @param ms number of milliseconds to sleep
 */
void sys_msleep(u32_t ms)
{
    OS_ERR      ucErr;
//    OSTimeDly(ms, OS_OPT_TIME_DLY, &ucErr);

    OSTimeDlyHMSM(0, 0, 0, ms, OS_OPT_TIME_DLY|OS_OPT_TIME_HMSM_NON_STRICT, &ucErr);
}



void msDelay(uint32_t ms)
{
//    sys_msleep(ms);
    volatile uint32_t cnt;

    cnt = 30000*ms;

    while(cnt--);
}


#endif



