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
OS_EVENT *Sem1, *Sem2, *Sem3;

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
void Task3(void *data);

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
	
   OSInit(); /* Initialize uC/OS-II */
   //PC_DOSSaveReturn(); /* Save environment to return to DOS */
   //PC_VectSet(uCOS, OSCtxSw); /* Install uC/OS-II's context switch vector */

   Sem1  = OSSemCreate(1); /* Task1 */
   Sem2  = OSSemCreate(0); /* Task2 */
   Sem3  = OSSemCreate(0); /* Task3 */
   ret = OSTaskCreate(TaskStart, (void *)9, &TaskStartStk[TASK_STK_SIZE - 1], 9);	
  
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
   INT16S     key;
   
   pdata = pdata; /* Prevent compiler warning */

   //OSStatInit(); /* Initialize uC/OS-II's statistics */
   TaskStartCreateTasks(); /* Create all the application tasks */

   for (;;)
   {
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
* CREATE TASKS
*******************************************************************************/

static void TaskStartCreateTasks(void)
{
	int ret;
    ret = OSTaskCreate(Task1, (void *)10, &TaskStk[1][TASK_STK_SIZE - 1], 10);
    ret = OSTaskCreate(Task2, (void *)11, &TaskStk[2][TASK_STK_SIZE - 1], 11);
    ret = OSTaskCreate(Task3, (void *)12, &TaskStk[3][TASK_STK_SIZE - 1], 12); 
}

/*******************************************************************************
* TASKS
*******************************************************************************/

int isPrime(int x)
{
   int i;
   int max = sqrt(x); 

   if (x <= 1) { return 0; }

   for (i=2; i < max; ++i)
   {
      if (i % x == 0) { return 0; } 
   }
   return 1;
}

void Task1(void *pdata)
{
   INT8U err;

   for (;;)
   {    
      printf("BEGIN: task 1\n");
      OSSemPend(Sem1, 0, &err); /* Pend on Signal from Task 2 */
      OSTimeDly(500);
      printf("MIDDLE: task 1\n");
      //int i;
      //for (i = 0; i < 100000; ++i)
      //{
      //   isPrime(i); 
      //}
      OSSemPost(Sem2); /* Signal Task2 using semaphore */
      printf("END: task 1\n");
   }
}

void  Task2 (void *pdata)
{
   INT8U err;

   for(;;) 
   {
      printf("BEGIN: task 2\n");
      OSSemPend(Sem2, 0, &err); /* Pend on Signal from Task 1 */
      OSTimeDly(500);
      printf("MIDDLE: task 2\n");      
      //int k;
      //for (k = 0; k < 100000; ++k)
      //{
      //    isPrime(k);
      //}
      OSSemPost(Sem3); /* Signal Task1 using semaphore */
      printf("END: task 2\n");
   }
}

void  Task3 (void *pdata)
{
   INT8U err;

   for(;;) 
   {
      printf("BEGIN: task 3\n");
      OSSemPend(Sem3, 0, &err); /* Pend on Signal from Task 1 */
      OSTimeDly(500);
      printf("MIDDLE: task 3\n");      
      //int l;
      //for (l = 0; l < 100000; ++l)
      //{
      //    isPrime(l);
      //}
      OSSemPost(Sem1); /* Signal Task1 using semaphore */
      printf("END: task 3\n");
   }
}
