
/*
 * hal_sys.c
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */

#include <cpu.h>
#include <os.h>

void SYS_EnterInt(void)
{
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
}




void SYS_ExitInt(void)
{
    OSIntExit();
}




