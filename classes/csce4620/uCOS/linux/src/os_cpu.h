/*
*********************************************************************************************************
*                                                uC/OS-II
*                                The Real-Time Kernel (by Jean J. Labrosse)
*
*                                              LINUX PORT
*
*                          (c) Copyright 2004-... Werner.Zimmermann@fht-esslingen.de
*                                           All Rights Reserved
*
* File : OS_CPU.H
* By   : Werner Zimmermann
*********************************************************************************************************
*/

#ifndef OS_CPU_H
#define OS_CPU_H

#define _GNU_SOURCE
#define _THREAD_SAFE

/*
*********************************************************************************************************
   uCOS-II standard definitions and declarations.
*********************************************************************************************************
*/

typedef unsigned char         	BOOLEAN;
typedef unsigned char           INT8U;
typedef char                    INT8S;
typedef unsigned short          INT16U;
typedef short                   INT16S;
typedef unsigned long           INT32U;
typedef long                    INT32S;
typedef unsigned long long      INT64U;
typedef long long               INT64S;
typedef float                   FP32;
typedef double                  FP64;

typedef INT32U                  OS_STK;

#define  OS_ENTER_CRITICAL()    OSDisableInterruptFlag()
#define  OS_EXIT_CRITICAL()     OSEnableInterruptFlag()

#define  OS_STK_GROWTH          1
#define  OS_TASK_SW()           OSCtxSw()

#define  OS_CRITICAL_METHOD     1

void OSCtxSw(void);
void OSIntCtxSw(void);
void OSStartHighRdy(void);

/*
*********************************************************************************************************
   Port-specific definitions and declarations
*********************************************************************************************************
*/

INT16U OSPortVersion(void);

void OSEnableInterruptFlag(void);
void OSDisableInterruptFlag(void);

/* DEBUGLEVEL	These values can be logically ored to set the debug level for uCOS-II LINUX port debugging
                Please note, that debugging will create a lot of screen messages and thus may affect
                the real-time performance of your application
   0x00000001   Scheduler
   0x00000002   Task switch
   0x00000004   Task creation
   0x00000008   Timer
   0x00000010   Initialization
   0x00000020   Idle and stat task
   0x00000040   Scheduler and Time Tick Interrupt Timeouts
   0x00000080   Interrupt-Enable/Disable
 */
#ifndef DEBUGLEVEL
#define DEBUGLEVEL 0	//0x07F
#endif

/* Timeout value in milliseconds for the scheduler - used to detect deadlocks. Set to INFINITE for "slow" applications*/
#define OS_SCHEDULER_TIMEOUT    INFINITE                        //10000

/* Timeout value in milliseconds for the time tick interrupt */
#define OS_INTERRUPT_TIMEOUT    INFINITE                        //10000

/* If this define is set, uCOS-II runs with elevated priority to ensure better (soft)-real time behaviour.
   This may decrease the performace of other applications and reduce the responsiveness to user inputs,
   if your uCOS-II generates a high CPU load.
*/
#define OS_BOOST_LINUX_PRIORITY TRUE

#define TASKSTACKSIZE	8192

#define gettid()   syscall(__NR_gettid)
#define getpid()   syscall(__NR_gettid)
#define kill(tid,sig)  syscall(__NR_tkill, tid, sig)

/* Call in OSTaskIdleHook() to give non-uCOS threads a change. Otherwise the CPU load may go up to 100% even when uCOS is idling.
*/
#define  OS_SLEEP()		{ struct timespec req, rem; req.tv_sec=0; req.tv_nsec=10000000; nanosleep(&req, &rem);}	//sched_yield();

#ifndef TRUE 
#define TRUE  1
#define FALSE 0
#endif

#endif
