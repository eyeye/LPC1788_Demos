
#define  BSP_INT_MODULE

#include <bsp.h>
#include <stdint.h>
#include "lpc177x_8x_nvic.h"
#include <intrinsics.h>


#define  BSP_INT_SRC_NBR                                 (BSP_INT_ID_MAX)


typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;


static  void  BSP_IntHandler     (void);
static  void  BSP_IntHandlerDummy(void);

#pragma language=extended
#pragma section="CSTACK"

static  void  BSP_NMI_ISR        (void);

static  void  BSP_Fault_ISR      (void);

static  void  BSP_BusFault_ISR   (void);

static  void  BSP_UsageFault_ISR (void);

static  void  BSP_MemFault_ISR   (void);

static  void  BSP_Spurious_ISR   (void);

static  void  BSP_Reserved_ISR   (void);

extern  void  __iar_program_start(void);

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

//#pragma location=".intvec"
__root  const  APP_INTVECT_ELEM  __cpu_vector_table[] =
{
                { .Ptr = (void *)__section_end( "CSTACK" )},                /*  0, SP start value.                                  */
                __iar_program_start,                                        /*  1, PC start value.                                  */
                BSP_NMI_ISR,                                                /*  2, NMI.                                             */
                BSP_Fault_ISR,                                              /*  3, Hard Fault.                                      */
                BSP_MemFault_ISR,                                           /*  4, Memory Management.                               */
                BSP_BusFault_ISR,                                           /*  5, Bus Fault.                                       */
                BSP_UsageFault_ISR,                                         /*  6, Usage Fault.                                     */
                BSP_Spurious_ISR,                                           /*  7, Reserved.                                        */
                BSP_Spurious_ISR,                                           /*  8, Reserved.                                        */
                BSP_Spurious_ISR,                                           /*  9, Reserved.                                        */
                BSP_Spurious_ISR,                                           /* 10, Reserved.                                        */
                BSP_Spurious_ISR,                                           /* 11, SVCall.                                          */
                BSP_Spurious_ISR,                                           /* 12, Debug Monitor.                                   */
                BSP_Spurious_ISR,                                           /* 13, Reserved.                                        */
                OS_CPU_PendSVHandler,                                       /* 14, PendSV Handler.                                  */
                OS_CPU_SysTickHandler,                                      /* 15, uC/OS-III Tick ISR Handler.                      */

                BSP_IntHandler             ,                         // 16:
                BSP_IntHandler             ,                         // 17:
                BSP_IntHandler             ,                         // 18:
                BSP_IntHandler             ,                         // 19:
                BSP_IntHandler             ,                         // 20:
                BSP_IntHandler             ,                         // 21:
                BSP_IntHandler             ,                         // 22:
                BSP_IntHandler             ,                         // 23:
                BSP_IntHandler             ,                         // 24:
                BSP_IntHandler             ,                         // 25:
                BSP_IntHandler             ,                         // 26:
                BSP_IntHandler             ,                         // 27:
                BSP_IntHandler             ,                         // 28:
                BSP_IntHandler             ,                         // 29:
                BSP_IntHandler             ,                         // 30:
                BSP_IntHandler             ,                         // 31:
                BSP_IntHandler             ,                         // 32:
                BSP_IntHandler             ,                         // 33:
                BSP_IntHandler             ,                         // 34:
                BSP_IntHandler             ,                         // 35:
                BSP_IntHandler             ,                         // 36:
                BSP_IntHandler             ,                         // 37:
                BSP_IntHandler             ,                         // 38:
                BSP_IntHandler             ,                         // 39:
                BSP_IntHandler             ,                         // 40:
                BSP_IntHandler             ,                         // 41:
                BSP_IntHandler             ,                         // 42:
                BSP_IntHandler             ,                         // 43:
                BSP_IntHandler             ,                         // 44:
                BSP_IntHandler             ,                         // 45:
                BSP_IntHandler             ,                         // 46:
                BSP_IntHandler             ,                         // 47:
                BSP_IntHandler             ,                         // 48:
                BSP_IntHandler             ,                         // 49:
                BSP_IntHandler             ,                         // 50:
                BSP_IntHandler             ,                         // 51:
                BSP_IntHandler             ,                         // 52:
                BSP_IntHandler             ,                         // 53:
                BSP_IntHandler             ,                         // 54:
                BSP_IntHandler             ,                         // 55:
                BSP_IntHandler             ,                         // 56:
                BSP_IntHandler             ,                         // 57:
                BSP_IntHandler             ,                         // 58:
                BSP_IntHandler             ,                         // 59:
                BSP_IntHandler             ,                         // 60:
                BSP_IntHandler             ,                         // 61:
                BSP_IntHandler             ,                         // 62:
                BSP_IntHandler             ,                         // 63:
                BSP_IntHandler             ,                         // 64:
                BSP_IntHandler             ,                         // 65:
                BSP_IntHandler             ,                         // 66:
                BSP_IntHandler             ,                         // 67:
                BSP_IntHandler             ,                         // 68:
};




static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_SRC_NBR];



/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              BSP_IntClr()
*
* Description : Clear interrupt.
*
* Argument(s) : int_id      Interrupt to clear.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) An interrupt does not need to be cleared within the interrupt controller.
*********************************************************************************************************
*/

void  BSP_IntClr (CPU_DATA  int_id)
{

}



void  BSP_IntDis (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcDis(int_id + 16);
    }
}



void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}


void  BSP_IntEn (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcEn(int_id + 16);
    }
}



void  BSP_IntVectSet (CPU_DATA       int_id,
                      CPU_FNCT_VOID  isr)
{
    CPU_SR_ALLOC();

    if (int_id < BSP_INT_SRC_NBR)
    {
        CPU_CRITICAL_ENTER();
        BSP_IntVectTbl[int_id] = isr;
        CPU_CRITICAL_EXIT();
    }
}



void  BSP_IntPrioSet (CPU_DATA    int_id,
                      CPU_INT08U  prio)
{
    CPU_SR_ALLOC();


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        CPU_IntSrcPrioSet(int_id + 16, prio);
        CPU_CRITICAL_EXIT();
    }
}



void  BSP_IntInit (void)
{
    CPU_DATA  int_id;

    for (int_id = 0; int_id < BSP_INT_SRC_NBR; int_id++)
    {
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }

}



static  void  BSP_IntHandler (void)
{
    CPU_FNCT_VOID  isr;
    CPU_DATA  int_id;

    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR            */
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    int_id = (SCB->ICSR)&0xFF;          // 获取当前中断号
//    int_id = __get_IPSR();
    int_id -= 16;

    if (int_id < BSP_INT_SRC_NBR)
    {
        isr = BSP_IntVectTbl[int_id];
        if ( isr != (CPU_FNCT_VOID)0 )
        {
            isr();
        }
    }

    OSIntExit();                                                /* Tell the OS that we are leaving the ISR            */
}



static  void  BSP_IntHandlerDummy (void)
{

}



static  void  BSP_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}



static  void  BSP_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}



static  void  BSP_BusFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}



static  void  BSP_UsageFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}



static  void  BSP_MemFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}



static  void  BSP_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


