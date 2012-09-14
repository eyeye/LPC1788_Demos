/*
 * demo_apploader.c
 *
 *  Created on: 2012-9-12
 *      Author: YangZhiyong
 */

#include <os.h>
#include "launcher.h"


void App_Loader(void *p_arg)
{
    OS_ERR      err;

    (void)p_arg;


}






APP_TASK_DEFINE(App_Loader,      /*name*/
                App_Loader, /*p_task*/
                0,              /*p_arg*/
                4,              /*prio*/
                2048,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)

