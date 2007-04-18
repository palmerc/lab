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
OS_EVENT *Mbox3, *Q2, *Q3a;
void *MessageQ2[100];
void *MessageQ3a[100];
int msg1, msg2;

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
   Mbox3 = OSMboxCreate((void *)0); 
   
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

   OSStatInit(); /* Initialize uC/OS-II's statistics */
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

void Task1(void *pdata)
{
   INT8U err;
   int counter;
	
   for (counter = 0; counter < 1000; ++counter)
   {  
      printf("Entering Task 1\n");
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
   OSTaskDel(OS_PRIO_SELF);
}

void  Task2 (void *pdata)
{
   INT8U err;
   int i, history[10];
   for (i = 0; i < 10; ++i)
   {
      history[i] = 0;
   }
   
   for(;;) 
   {
      printf("Entering Task 2\n");
      // Wait for message to arrive
      msg2 = *(int *) OSQPend(Q2, 0, &err);
      history[msg2 % 10]++;
      if (err)
         printf("Received error %d\n", err);
      
      if (msg2 != -1)
      {
        printf("T2 Received %d\n", msg2);
        // Pass message to T3 using mailbox
        err = OSMboxPost(Mbox3, (void *)&msg2);
        // Calculate Histogram and printf
	for (i = 0; i < 10; ++i)
	{
	   printf("%d -> %d\n", i, history[i]);
	}
      }
      else
      {
	// if message was negative print histogram and die
	for (i = 0; i < 10; ++i)
	{
	   printf("%d -> %d\n", i, history[i]);
	}
	printf("T2 terminating\n");
	OSTaskDel(OS_PRIO_SELF);       
      }
   }
}

void  Task3 (void *pdata) // highest priority
{
   INT8U err;
   int msg3a, msg3b, msg_count = 0;
	
   for(;;) 
   {
      printf("Entering Task 3\n");
      // Wait for message from T1
      msg3a = *(int *) OSQPend(Q3a, 0, &err);
      // Wait for message from T2
      msg3b = *(int *) OSMboxPend(Mbox3, 0, &err);
      msg_count++;
      // Compare messages and print if not equal, mesg num, numbers, and not equal
      if (msg3a != msg3b)
      {
         printf("Not Equal, msg number %d, value1 %d, value2 %d\n", msg_count, msg3a, msg3b);
      }
      // Delay one tick
      OSTimeDly(1);
      // If message from T1 is negative, print a line stating finished, and terminate
      if (msg3a == -1)
      {
         // if message was negative print histogram and die
         printf("T3 terminating\n");
         OSTaskDel(OS_PRIO_SELF);        
      }
   }
}
