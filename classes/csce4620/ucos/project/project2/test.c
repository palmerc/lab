/*******************************************************************************
* Cameron Palmer
* CSCE 4620 - RTOS
*
* Project 2
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
OS_EVENT *Mbox1, *Sem2, *Mutex3, *Q2, *Q3a, *Q3b;
void *MessageQ2[100];
void *MessageQ3a[100];
void *MessageQ3b[100];
int msg1;
int msg2;

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
   INT8U err;
	
   OSInit(); /* Initialize uC/OS-II */
   //PC_DOSSaveReturn(); /* Save environment to return to DOS */
   //PC_VectSet(uCOS, OSCtxSw); /* Install uC/OS-II's context switch vector */

   Q2 = OSQCreate((void *) MessageQ2, 100);
   Q3a = OSQCreate((void *) MessageQ3a, 100);
   Q3b = OSQCreate((void *) MessageQ3b, 100);
   Mbox1 = OSMboxCreate((void *)1); /* Task1 */
   Sem2 = OSSemCreate(0); /* Task3 */
   Mutex3 = OSMutexCreate(9, &err); /* Task2 */
   
   ret = OSTaskCreate(TaskStart, (void *)7, &TaskStartStk[TASK_STK_SIZE - 1], 7);	
  
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
      OSTimeDly(500); /* Wait one second */
   }
}

/*******************************************************************************
* CREATE TASKS
*******************************************************************************/

static void TaskStartCreateTasks(void)
{
	int ret;
    ret = OSTaskCreate(Task1, (void *)12, &TaskStk[1][TASK_STK_SIZE - 1], 12);
    ret = OSTaskCreate(Task2, (void *)11, &TaskStk[2][TASK_STK_SIZE - 1], 11);
    ret = OSTaskCreate(Task3, (void *)10, &TaskStk[3][TASK_STK_SIZE - 1], 10); 
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
   int r;
   int counter;
   for (counter = 0; counter < 1000; ++counter)
   {  
      msg1 = rand() % 100;
      // Post Random Integer to T2 and T3 via MessageQ
      err = OSQPost(Q2, (void *)&msg1);
      err = OSQPost(Q3a, (void *)&msg1);
      
      // Sleep one tick
      OSTimeDly(1);
   }
   msg1 = -1;
   printf("Sending Term to T2\n");
   err = OSQPost(Q2, (void *)&msg1);
   // Send T2 a -1
}

void  Task2 (void *pdata)
{
   INT8U err;
   
   
   for(;;) 
   {
      // Wait for message to arrive
      msg2 = *(int *) OSQPend(Q2, 0, &err);
      if (err)
         printf("Received error %d\n", err);
      
      if (msg2 != -1)
      {
         printf("T2 Received %d\n", msg2);
         // Pass message to T3 using mailbox
         err = OSQPost(Q3b, (void *)&msg2);
         // Calculate Histogram and printf
      }
      else
      {
         // if message was negative print histogram and die
         printf("T2 terminating\n");
         OSTaskDel(OS_PRIO_SELF);        
      }
   }
}

void  Task3 (void *pdata) // highest priority
{
   INT8U err;
   int msg3a, msg3b, msg_count;

   for(;;) 
   {
      // Wait for message from T1
      msg3a = *(int *) OSQPend(Q3a, 0, &err);
      // Wait for message from T2
      msg3b = *(int *) OSQPend(Q3b, 0, &err);
      msg_count++;
      // Compare messages and print if not equal, mesg num, numbers, and not equal
      if (msg3a != msg3b)
      {
         printf("Not Equal, msg number %d, value1 %d, value2 %d\n", msg_count, msg3a, msg3b);
      }
      // Delay one tick
      // If message from T1 is negative, print a line stating finished, and terminate
   }
}
