/*******************************************************************************
* Gary Goodman
* CSCE 5620 - RTOS
*
* Bilateral Rendezvous #1
*******************************************************************************/

#include "includes.h"

/*******************************************************************************
* CONSTANTS
*******************************************************************************/

#define TASK_STK_SIZE 1024 /* Size of each task's stacks (# of WORDs) */
#define N_TASKS 10 /* Number of identical tasks */

/*******************************************************************************
* VARIABLES
*******************************************************************************/

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE]; /* Tasks stacks */
OS_STK TaskStartStk[TASK_STK_SIZE];
char TaskData[N_TASKS]; /* Parameters to pass to each task */
OS_EVENT *SemA, *SemB;

/*******************************************************************************
*                                           FUNCTION PROTOTYPES
*******************************************************************************/

void Task(void *data); /* Function prototypes of tasks */
void TaskStart(void *data); /* Function prototypes of Startup task */
static void TaskStartCreateTasks(void);
static void TaskStartDispInit(void);
static void TaskStartDisp(void);
void Task1(void *data);
void Task2(void *data);

/*******************************************************************************
* MAIN
*******************************************************************************/

int main(void)
{
   int ret;
   unsigned long mask = 1;
   unsigned int len = sizeof(mask);
   if (sched_setaffinity(0, len, &mask) < 0) 
   { 
      perror("sched_setaffinity");
   }
	
   PC_DispClrScr(DISP_FGND_BLACK + DISP_BGND_WHITE); /* Clear the screen */
   OSInit(); /* Initialize uC/OS-II */
   //PC_DOSSaveReturn(); /* Save environment to return to DOS */
   //PC_VectSet(uCOS, OSCtxSw); /* Install uC/OS-II's context switch vector */

   SemA  = OSSemCreate(0); /* Task1 Signals Task2 using this semaphore */
   SemB  = OSSemCreate(1); /* Task2 Signals Task1 using this semaphore */

   ret = OSTaskCreate(TaskStart, (void *)9, &TaskStartStk[TASK_STK_SIZE - 1], 9);
	
   PC_DispStr( 0,  3, " Start Multitasking                                                             ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   
   OSStart(); /* Start multitasking */
   return 0;
}


/*******************************************************************************
* STARTUP TASK
*******************************************************************************/
void TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
   OS_CPU_SR  cpu_sr;
#endif
   char       s[100];
   INT16S     key;
   
   pdata = pdata; /* Prevent compiler warning */

   TaskStartDispInit(); /* Initialize the display */

   //OSStatInit(); /* Initialize uC/OS-II's statistics */
   PC_DispStr( 0,  5, " Statistics Initialized                                                         ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   TaskStartCreateTasks(); /* Create all the application tasks */

   for (;;)
   {
      TaskStartDisp(); /* Update the display */

      if (PC_GetKey(&key) == TRUE) /* See if key has been pressed */
      {
         if (key == 0x1B) /* Yes, see if it's the ESCAPE key */
         { 
            exit(0); /* Exit program */
         }
      }
      OSCtxSwCtr = 0; /* Clear context switch counter */
      OSTimeDlyHMSM(0, 0, 1, 0); /* Wait one second */
   }
}

/*******************************************************************************
* INITIALIZE THE DISPLAY
*******************************************************************************/

static void TaskStartDispInit(void)
{
   /* This numeric junk is meant to help you layout the 80x24 screen */
                              /* 1111111111222222222233333333334444444444555555555566666666667777777777 */
                    /* 01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
   PC_DispStr( 0,  0, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  1, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED);
   PC_DispStr( 0,  2, "        Cameron Palmer's excursion on Gary Goodman's Bilateral Rendezvous       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0,  8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
	PC_DispStr( 0,  9, " TASK1:                                                                         ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
	PC_DispStr( 0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
	PC_DispStr( 0, 14, " TASK2:                                                                         ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 20, " #Tasks         :        CPU Usage:     %                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 21, " #Task switch/sec:                                                              ", DISP_FGND_BLACK + DISP_BGND_WHITE);
   PC_DispStr( 0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);   
   PC_DispStr( 0, 23, " <-PRESS 'ESC' TO QUIT->                                                        ", DISP_FGND_BLACK + DISP_BGND_WHITE);
                              /* 1111111111222222222233333333334444444444555555555566666666667777777777 */
                    /* 01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*******************************************************************************
* UPDATE THE DISPLAY
*******************************************************************************/

static void TaskStartDisp(void)
{
   char s[80];

   sprintf(s, "%5d", OSTaskCtr); /* Display #tasks running */
   PC_DispStr(18, 20, s, DISP_FGND_YELLOW + DISP_BGND_WHITE);

#if OS_TASK_STAT_EN > 0
   sprintf(s, "%3d", OSCPUUsage); /* Display CPU usage in % */
   PC_DispStr(36, 20, s, DISP_FGND_YELLOW + DISP_BGND_WHITE);
#endif

   sprintf(s, "%5d", OSCtxSwCtr); /* Display # context switches per second */
   PC_DispStr(18, 21, s, DISP_FGND_YELLOW + DISP_BGND_WHITE);
#ifdef __WIN32__
   sprintf(s, "uCOS-II V%1d.%02d  WIN32 V%1d.%02d", OSVersion() / 100, OSVersion() % 100, OSPortVersion() / 100, OSPortVersion() % 100); /* Display uC/OS-II's version number */
#endif
#ifdef __LINUX__
   sprintf(s, "uCOS-II V%1d.%02d  LINUX V%1d.%02d", OSVersion() / 100, OSVersion() % 100, OSPortVersion() / 100, OSPortVersion() % 100); /* Display uC/OS-II's version number */
#endif

   PC_DispStr(52, 23, s, DISP_FGND_BLACK + DISP_BGND_WHITE);
}

/*******************************************************************************
* CREATE TASKS
*******************************************************************************/

static void TaskStartCreateTasks(void)
{
	int ret;
   char s[80];
	PC_DispStr( 0,  6, " Creating Task1 and Task 2                                                      ", DISP_FGND_BLACK + DISP_BGND_WHITE);

	ret = OSTaskCreate(Task1, (void *)10, &TaskStk[1][TASK_STK_SIZE - 1], 10);
   sprintf(s, "Task 1 create return value: %i", ret);
   PC_DispStr( 0,  7, s, DISP_FGND_BLACK + DISP_BGND_WHITE);
   
   ret = OSTaskCreate(Task2, (void *)11, &TaskStk[2][TASK_STK_SIZE - 1], 11);
   sprintf(s, "Task 2 create return value: %i", ret);
   PC_DispStr( 0,  8, s, DISP_FGND_BLACK + DISP_BGND_WHITE);
   
   
   PC_DispStr( 0,  6, " Task1 and Task 2 Created                                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
}

/*******************************************************************************
* TASKS
*******************************************************************************/

void Task1(void *pdata)
{
   INT8U err;

   for (;;)
   {
      PC_DispStr(10, 10, " Task 1 start                                                                   ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      
      OSSemPend(SemA, 0, &err); /* Pend on Signal from Task 2 */
      PC_DispStr(10, 11, "  Signal Task 2 (POST Semaphore A)                                              ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      OSTimeDly(500); /* Wait one second */
      PC_DispStr(10, 12, "  Waiting on signal from Task 2 (PEND)                                          ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      OSSemPost(SemB); /* Signal Task2 using semaphore */
   }
}

void  Task2 (void *pdata)
{
   INT8U err;

   for(;;) 
   {
      PC_DispStr(10, 15, " Task 2 start                                                                   ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      PC_DispStr(10, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);

      OSSemPend(SemB, 0, &err); /* Pend on Signal from Task 1 */      
      PC_DispStr(10, 16, "  Signal Task 1 (POST Semaphore B)                                              ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      OSTimeDly(500); /* Wait one second */
      PC_DispStr(10, 17, "  Waiting on Task1 (PEND Semaphore A)                                           ", DISP_FGND_BLACK + DISP_BGND_WHITE);
      OSSemPost(SemA); /* Signal Task1 using semaphore */
   }
}
