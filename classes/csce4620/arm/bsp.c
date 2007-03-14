/*
*********************************************************************************************************
*                                                Logic PD
*                                         Board Support Package
*                                          LH79520 Card Engine
*
*                             (c) Copyright 2004, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*
* File         : BSP.C
* Originally by: Jean J. Labrosse
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                               CONSTANTS                                                
*********************************************************************************************************
*/

#define  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR   (*(INT32U *)        0x00000004L)
#define  BSP_SWI_VECTOR_ADDR                 (*(INT32U *)        0x00000008L)
#define  BSP_PREFETCH_ABORT_VECTOR_ADDR      (*(INT32U *)        0x0000000CL)
#define  BSP_DATA_ABORT_VECTOR_ADDR          (*(INT32U *)        0x00000010L)
#define  BSP_IRQ_VECTOR_ADDR                 (*(INT32U *)        0x00000018L)
#define  BSP_FIQ_VECTOR_ADDR                 (*(INT32U *)        0x0000001CL)

#define  BSP_IRQ_ISR_ADDR                    (*(INT32U *)        0x00000038L)
#define  BSP_FIQ_ISR_ADDR                    (*(INT32U *)        0x0000003CL)


#define  LED_PORT                            (*(volatile INT8U *)0x55600000L)

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

typedef  void (*PFNCT)(void);

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

static  INT8U  LED_Image;

/*
*********************************************************************************************************
*                                               PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_InitIntCtrl(void);
static  void  Tmr_TickInit(void);

/*
*********************************************************************************************************
*                                           BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    VIC->intenclear     = 0xFFFFFFFFL;                /* Disable ALL interrupts                        */
    RCPC->intclear      = 0x000000FFL;                /* Clear ALL external interrupts                 */                                            

#if 1
    RCPC->remap         = 0x00000002L;                /* Remap internal SRAM to 0x00000000             */
#endif

    RCPC->periphclkctrl = 0x00000000L;                /* Initialize the peripheral clock control       */
    RCPC->spareclkctrl  = 0x00000000L;

    BSP_InitIntCtrl();                                /* Initialize the interrupt controller           */

    Tmr_Init();                                       /* Initialize the timers                         */

    Tmr_TickInit();                                   /* Initialize uC/OS-II's tick ISR                */

    LED_Init();                                       /* Initialize LEDs                               */
}

/*
*********************************************************************************************************
*                                    INITIALIZE INTERRUPT CONTROLLER
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*
* Note(s)     : 1) 0xE59FF020 is opcode of:  ldr pc,[pc,#+0x20]
*********************************************************************************************************
*/

static  void  BSP_InitIntCtrl (void)
{
    VIC->intenclear                   = 0xFFFFFFFF;   /* Disable ALL interrupts                        */

    BSP_IRQ_VECTOR_ADDR               = 0xE59FF018;                /* LDR PC,[PC,#0x18] instruction    */
    BSP_IRQ_ISR_ADDR                  = (INT32U)OS_CPU_IRQ_ISR;    /* IRQ exception vector address     */

    BSP_FIQ_VECTOR_ADDR               = 0xE59FF018;                /* LDR PC,[PC,#0x18] instruction    */
    BSP_FIQ_ISR_ADDR                  = (INT32U)OS_CPU_FIQ_ISR;    /* FIQ exception vector address     */


    BSP_UNDEF_INSTRUCTION_VECTOR_ADDR = 0xEAFFFFFE;   /* Jump to itself                                */
    BSP_SWI_VECTOR_ADDR               = 0xEAFFFFFE;
    BSP_PREFETCH_ABORT_VECTOR_ADDR    = 0xEAFFFFFE;
    BSP_DATA_ABORT_VECTOR_ADDR        = 0xEAFFFFFE;
    BSP_FIQ_VECTOR_ADDR               = 0xEAFFFFFE;
}

/*
*********************************************************************************************************
*                                     DISABLE ALL INTERRUPTS
*
* Description : This function disables all interrupts from the interrupt controller.
*               
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    VIC->intenclear = 0xFFFFFFFFL;           /* Disable ALL interrupts                                 */
}

/*
*********************************************************************************************************
*                                           BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void  LED_Init (void)
{
    LED_On(0);                                  /* Turn ON all the LEDs                                */
}

/*
*********************************************************************************************************
*                                                 LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON User LED 0 on the board
*                      .
*                      .
*                      3    turns ON User LED 2 on the board
*********************************************************************************************************
*/

void  LED_On (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             LED_Image  =  0x00;
             LED_PORT   = LED_Image;
             GPIOB->dr &= ~0x02;
             break;

        case 1:
             LED_Image &= ~0x01;
             LED_PORT   = LED_Image;
             break;

        case 2:
             LED_Image &= ~0x02;
             LED_PORT   = LED_Image;
             break;

        case 3:
             GPIOB->dr &= ~0x02;
             break;

        case 4:
             LED_Image &= ~0x08;
             LED_PORT   = LED_Image;
             break;
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
*                                                LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF User LED0 on the board
*                      .
*                      .
*                      3    turns OFF User LED2 on the board
*********************************************************************************************************
*/

void  LED_Off (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             LED_Image  = 0xFF;
             LED_PORT   = LED_Image;
             GPIOB->dr |= 0x02;
             break;

        case 1:
             LED_Image |= 0x01;
             LED_PORT   = LED_Image;
             break;

        case 2:
             LED_Image |= 0x02;
             LED_PORT   = LED_Image;
             break;

        case 3:
             GPIOB->dr |= 0x02;
             break;

        case 4:
             LED_Image |= 0x08;
             LED_PORT   = LED_Image;
             break;
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
*                                              LED TOGGLE
*
* Description : This function is used to alternate the state of an LED
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    toggle User LED 0 on the board
*                      .
*                      .
*                      3    toggle User LED 2 on the board
*********************************************************************************************************
*/

void  LED_Toggle (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             LED_Image ^= 0xFF;
             LED_PORT   = LED_Image;
             break;

        case 1:
             LED_Image ^= 0x01;
             LED_PORT   = LED_Image;
             break;

        case 2:
             LED_Image ^= 0x02;
             LED_PORT   = LED_Image;
             break;

        case 3:
             GPIOB->dr ^= 0x02;
             break;
    }
    OS_EXIT_CRITICAL();
}


/*
*********************************************************************************************************
*                                           IRQ ISR HANDLER
*
* Description : This function is called by OS_CPU_IRQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_IRQ_ISR_Handler (void)
{
    PFNCT  pfnct;


#if 1   
    pfnct = (PFNCT)VIC->vectoraddr;             /* Read the interrupt vector from the VIC               */
    if (pfnct != (PFNCT)0) {                    /* Make sure we don't have a NULL pointer               */
        (*pfnct)();                             /* Execute the ISR for the interrupting device          */
    }
#else    
    pfnct = (PFNCT)VIC->vectoraddr;             /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct = (PFNCT)VIC->vectoraddr;         /* Read the interrupt vector from the VIC               */
    }
#endif    
}

/*
*********************************************************************************************************
*                                           FIQ ISR HANDLER
*
* Description : This function is called by OS_CPU_FIQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_FIQ_ISR_Handler (void)
{
    PFNCT  pfnct;


#if 1   
    pfnct = (PFNCT)VIC->vectoraddr;             /* Read the interrupt vector from the VIC               */
    if (pfnct != (PFNCT)0) {                    /* Make sure we don't have a NULL pointer               */
        (*pfnct)();                             /* Execute the ISR for the interrupting device          */
    }
#else    
    pfnct = (PFNCT)VIC->vectoraddr;             /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct = (PFNCT)VIC->vectoraddr;         /* Read the interrupt vector from the VIC               */
    }
#endif    
}

/*
*********************************************************************************************************
*                          READ TIMER USED TO MEASURE INTERRUPT DISABLE TIME
*
* Description : This function is called to read the current counts of the interrupt disable time 
*               measurement timer.  It is assumed that the timer used for this measurement is a free-running
*               16 bit timer.  
*
*               Timer #1 of the LH79520 is used.  This is a down-timer and the counts are inverted to make
*               the timer look as an up counter.
*               
* Arguments   : none
*
* Returns     ; The 16 bit counts of the timer assuming the timer is an UP counter.
*********************************************************************************************************
*/

#if OS_CPU_INT_DIS_MEAS_EN > 0
INT16U  OS_CPU_IntDisMeasTmrRd (void)
{
    INT16U  cnts;


    cnts = (INT16U)(~TIMER1->value & 0xFFFF);
    return (cnts);
}
#endif

/*
*********************************************************************************************************
*                                     INITIALIZE TIMER FOR uC/OS-View
*
* Description : This function is called to by uC/OS-View to initialize the free running timer that is
*               used to make time measurements.
*
* Arguments   : none
*
* Returns     ; none
*
* Note(s)     : This function is EMPTY because the timer is initialized elsewhere.
*********************************************************************************************************
*/

#if OS_VIEW_MODULE > 0
void  OSView_TmrInit (void)
{
}
#endif

/*
*********************************************************************************************************
*                                     READ TIMER FOR uC/OS-View
*
* Description : This function is called to read the current counts of a 16 bit free running timer.
*
*               Timer #1 of the LH79520 is used.  This is a down-timer and the counts are inverted to make
*               the timer look as an up counter.
*               
* Arguments   : none
*
* Returns     ; The 16 bit counts of the timer assuming the timer is an UP counter.
*********************************************************************************************************
*/

#if OS_VIEW_MODULE > 0
INT32U  OSView_TmrRd (void)
{
    INT32U  cnts;


    cnts = (INT32U)(~TIMER1->value & 0xFFFF);
    return (cnts);
}
#endif

/*
*********************************************************************************************************
*                                         TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
*               We decided to use Timer #0 as the tick interrupt source.
*
* Arguments   : none
*
* Notes       :
*********************************************************************************************************
*/

void  Tmr_Init (void)
{
    TIMER0->clear       = 0;                 /* Disable Timer #0                                       */
    TIMER0->load        = BSP_LH79520_CLK / 16;
    TIMER0->control     = TMRCTRL_DISABLE | TMRCTRL_MODE_PERIODIC | TMRCTRL_PRESCALE16;

    TIMER1->clear       = 0;                 /* Enable Timer #1 for interrupt disable time measurement */
    TIMER1->load        = 0xFFFF;
    TIMER1->control     = TMRCTRL_ENABLE  | TMRCTRL_MODE_FREERUN  | TMRCTRL_PRESCALE1;

    TIMER2->clear       = 0;                 /* DISABLE Timer #2                                       */
    TIMER2->load        = BSP_LH79520_CLK / 16 / 100;
    TIMER2->control     = TMRCTRL_DISABLE | TMRCTRL_MODE_PERIODIC | TMRCTRL_PRESCALE16;
                                             
    TIMER3->clear       = 0;                 /* DISABLE Timer #3                                       */
    TIMER3->load        = BSP_LH79520_CLK / 16 / 100;
    TIMER3->control     = TMRCTRL_DISABLE | TMRCTRL_MODE_PERIODIC | TMRCTRL_PRESCALE16;
}

/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*               
* Arguments   : none
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    TIMER0->clear       = 0;                          /* Enable Timer #0 for uC/OS-II Tick Interrupts  */
    TIMER0->load        = BSP_LH79520_CLK / 16 / OS_TICKS_PER_SEC;
    TIMER0->control     = TMRCTRL_ENABLE  | TMRCTRL_MODE_PERIODIC | TMRCTRL_PRESCALE16;

                                                      /* Setup the interrupt vector for the tick ISR   */
                                                      /*    Timer interrupt is a medium priority       */
    VIC->vectcntl[VIC_VECT_OS_TICK] = VIC_VECTCNTL_ENABLE | VIC_TIMER0;
    VIC->vectaddr[VIC_VECT_OS_TICK] = (INT32U)Tmr_TickISR_Handler;
    VIC->intenable                  = _BIT(VIC_TIMER0);
}

/*
*********************************************************************************************************
*                                          TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Arguments   : none
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
    TIMER0->clear   = 0x00000000L;              /* Clear the tick interrupt source                     */
    VIC->vectoraddr = 0x00000000L;              /* Clear the vector address register                   */

    OSTimeTick();                               /* Call uC/OS-II's OSTimeTick() to signal a tick       */
}
