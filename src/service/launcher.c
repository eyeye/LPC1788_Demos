/*
 * launcher.c
 *
 *  Created on: 2012-3-23
 *      Author: YangZhiyong
 */


#include "launcher.h"


#pragma section="OS_4K_MEM_POOL"
#pragma section="OS_2M_MEM_POOL"

OS_MEM  AppMem4KB;
OS_MEM  AppMem2MB;



static  void  AppMemCreate(void)
{
    OS_ERR  error = OS_ERR_NONE;

    void        *p_addr;
    void        *p_addr_end;
    OS_MEM_QTY  n_blks;
    OS_MEM_SIZE blk_size;

    p_addr = __section_begin("OS_4K_MEM_POOL");
    p_addr_end = __section_end("OS_4K_MEM_POOL");
    n_blks = ((OS_MEM_SIZE)p_addr_end - (OS_MEM_SIZE)p_addr)/(4*1024);
    blk_size = (4*1024);


    OSMemCreate(&AppMem4KB,
                "4KB memory for app",
                p_addr,
                n_blks,
                blk_size,
                &error );


    p_addr = __section_begin("OS_2M_MEM_POOL");
    p_addr_end = __section_end("OS_2M_MEM_POOL");
    n_blks = ((OS_MEM_SIZE)p_addr_end - (OS_MEM_SIZE)p_addr) / (2*1024*1024);
    blk_size = (2*1024*1024);

    OSMemCreate(&AppMem2MB,
                "2MB memory for app",
                p_addr,
                n_blks,
                blk_size,
                &error );

}










#pragma section="APP_TASK_DEFINE"

static  void  AppTaskCreate (void)
{
    OS_ERR  error = OS_ERR_NONE;

    struct OSTaskCreate_Arg* task_find;
    struct OSTaskCreate_Arg* task_end;

    task_find = __segment_begin("APP_TASK_DEFINE");
    task_end = __segment_end("APP_TASK_DEFINE");


    for(; task_find < task_end; task_find ++)
    {

        OSTaskCreate((OS_TCB     *)task_find->p_tcb,
                        (CPU_CHAR   *)task_find->p_name,
                        (OS_TASK_PTR )task_find->p_task,
                        (void       *)task_find->p_arg,
                        (OS_PRIO     )task_find->prio,
                        (CPU_STK    *)task_find->p_stk_base,
                        (CPU_STK_SIZE)task_find->stk_limit,
                        (CPU_STK_SIZE)task_find->stk_size,
                        (OS_MSG_QTY  )task_find->q_size,
                        (OS_TICK     )task_find->time_quanta,
                        (void       *)task_find->p_ext,
                        (OS_OPT      )task_find->opt,
                        (OS_ERR     *)&error );


        if(error != OS_ERR_NONE)
        {
            goto ERROR_HANDLE;
        }

    }


    return;


    ERROR_HANDLE:

    switch(error)
    {
    case OS_ERR_NONE:
        break;

    default:
        while(1);
        break;

    }
}



#pragma section="APP_SEM_DEFINE"

static  void  AppSemCreate (void)
{
    OS_ERR  error = OS_ERR_NONE;

    struct OSSemCreate_Arg* sem_find;
    struct OSSemCreate_Arg* sem_end;

    sem_find = __segment_begin("APP_SEM_DEFINE");
    sem_end = __segment_end("APP_SEM_DEFINE");

    for(; sem_find < sem_end; sem_find ++)
    {
        OSSemCreate  ((OS_SEM      *)sem_find->p_sem,
                      (CPU_CHAR    *)sem_find->p_name,
                      (OS_SEM_CTR   )sem_find->cnt,
                      (OS_ERR      *)&error);

        if(error != OS_ERR_NONE)
        {
            goto ERROR_HANDLE;
        }

    }

    return;


    ERROR_HANDLE:

    switch(error)
    {
    case OS_ERR_NONE:
        break;

    default:
        while(1);
        break;

    }
}



void Launcher_Exec(void)
{

    AppMemCreate();

    // �������
//    AppFlagCreate();
//    AppQCreate();
//    AppMutexCreate();

    AppSemCreate();

    AppTaskCreate();
}









