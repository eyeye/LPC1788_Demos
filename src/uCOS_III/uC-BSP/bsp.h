

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_ascii.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#include  <os_cpu.h>
#include  <os.h>
#include  <app_cfg.h>



/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/

#define BSP_INT_ID_MAX                          40

#define  BSP_INT_ID_WDT                            0     // 16: Watchdog Timer
#define  BSP_INT_ID_TIMER0                         1     // 17: Timer0
#define  BSP_INT_ID_TIMER1                         2     // 18: Timer1
#define  BSP_INT_ID_TIMER2                         3     // 19: Timer2
#define  BSP_INT_ID_TIMER3                         4     // 20: Timer3
#define  BSP_INT_ID_UART0                          5     // 21: UART0
#define  BSP_INT_ID_UART1                          6     // 22: UART1
#define  BSP_INT_ID_UART2                          7     // 23: UART2
#define  BSP_INT_ID_UART3                          8     // 24: UART3
#define  BSP_INT_ID_PWM1                           9     // 25: PWM1
#define  BSP_INT_ID_I2C0                          10     // 26: I2C0
#define  BSP_INT_ID_I2C1                          11     // 27: I2C1
#define  BSP_INT_ID_I2C2                          12     // 28: I2C2
#define  BSP_INT_ID_SPIFI                         13     // 29: SPIFI
#define  BSP_INT_ID_SSP0                          14     // 30: SSP0
#define  BSP_INT_ID_SSP1                          15     // 31: SSP1
#define  BSP_INT_ID_PLL0                          16     // 32: PLL0 Lock (Main PLL)
#define  BSP_INT_ID_RTC                           17     // 33: Real Time Clock
#define  BSP_INT_ID_EINT0                         18     // 34: External Interrupt 0
#define  BSP_INT_ID_EINT1                         19     // 35: External Interrupt 1
#define  BSP_INT_ID_EINT2                         20     // 36: External Interrupt 2
#define  BSP_INT_ID_EINT3                         21     // 37: External Interrupt 3
#define  BSP_INT_ID_ADC                           22     // 38: A/D Converter
#define  BSP_INT_ID_BOD                           23     // 39: Brown-Out Detect
#define  BSP_INT_ID_USB                           24     // 40: USB
#define  BSP_INT_ID_CAN                           25     // 41: CAN
#define  BSP_INT_ID_DMA                           26     // 42: General Purpose DMA
#define  BSP_INT_ID_I2S                           27     // 43: I2S
#define  BSP_INT_ID_ENET                          28     // 44: Ethernet
#define  BSP_INT_ID_MCI		  	                  29     // 45: MCI Card
#define  BSP_INT_ID_MCPWM                         30     // 46: Motor Control PWM
#define  BSP_INT_ID_QEI                           31     // 47: Quadrature Encoder Interface
#define  BSP_INT_ID_PLL1                          32     // 48: PLL1 Lock (USB PLL)
#define  BSP_INT_ID_USBActivity	                  33     // 49: USB Activity Interrupt
#define  BSP_INT_ID_CANActivity	                  34     // 50: CAN Activity Interrupt
#define  BSP_INT_ID_UART4                         35     // 51: UART4
#define  BSP_INT_ID_SSP2		                  36     // 52: SSP2
#define  BSP_INT_ID_LCD		  	                  37     // 53: LCD
#define  BSP_INT_ID_GPIO		  	              38     // 54: GPIO
#define  BSP_INT_ID_PWM0		  	              39     // 55: PWM0
#define  BSP_INT_ID_EEPROM	  	                  40     // 56: EEPROM




void         BSP_Init                    (void);

void         BSP_IntDisAll               (void);

CPU_INT32U   BSP_CPU_ClkFreq             (void);



void         BSP_IntInit                 (void);

void         BSP_IntEn                   (CPU_DATA       int_id);

void         BSP_IntDis                  (CPU_DATA       int_id);

void         BSP_IntClr                  (CPU_DATA       int_id);

void         BSP_IntVectSet              (CPU_DATA       int_id,
                                          CPU_FNCT_VOID  isr);

void         BSP_IntPrioSet              (CPU_DATA       int_id,
                                          CPU_INT08U     prio);


#endif
