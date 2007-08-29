#include "includes.h"

#define TASK_STK_SIZE 512 /* Size of start task's stacks */
#define TASK_START_PRIO	0	/* Priority of your startup task */

OS_STK TaskStartStk[TASK_STK_SIZE];

void TaskStart(void *data);

int main (void)
{
   OSInit(); /* Initialize uC/OS-II */
   /* ToDo: Create semaphores, mailboxes etc. */
   OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], TASK_START_PRIO);	/* Create the startup task*/
   OSStart(); /* Start multitasking */
   return 0;
}


void TaskStart(void *pdata)
{   
   INT16S key;
   pdata = pdata; /* Prevent compiler warning */

#if OS_TASK_STAT_EN > 0
   OSStatInit(); /* Initialize uC/OS-II's statistics */
#endif
   /* Create all other application tasks here */

   while(1) /* Startup task's infinite loop */
   {
	/* ToDo: Place additional code for your startup task, which you want to run once or periodically here */
      printf("%6u - Hello, my name is Cameron Palmer - press ESC to stop\n", OSTime);
      if (PC_GetKey(&key) == TRUE) 
      {  /* See if key has been pressed */
         if (key == 0x1B) 
         { /* Yes, see if it's the ESCAPE key */
            exit(0); /* End program */
         }
      }
      
      /* ToDo: Don't forget to call the uCOS scheduler with OSTimeDly etc., to give other tasks a chance to run */
      OSTimeDly(100);                        		   /* Wait 100 ticks */
   }
}
