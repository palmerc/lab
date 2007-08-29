#include "includes.h"

#define          UPPERCASE          0
#define          LOWERCASE          1
#define          TASK_STK_SIZE     512                /* Size of each task's stacks (# of WORDs)       */
#define          TASK_START_ID       0                /* Application tasks                             */
#define          TASK_CLK_ID         1
#define          TASK_A_ID           2
#define          TASK_B_ID           3
#define          TASK_C_ID           4             
#define          TASK_START_PRIO    10                /* Application tasks priorities                  */
#define          TASK_CLK_PRIO      11
#define          TASK_A_PRIO        12
#define          TASK_B_PRIO        13
#define          TASK_C_PRIO        14   
#define          BUF_SIZE           20

typedef struct
{
    char    TaskName[30];
    INT16U  TaskCtr;
    INT8U   TaskPrio;
    INT16U  TaskExecTime;
    INT32U  TaskTotExecTime;
} TASK_USER_DATA;


OS_STK          TaskStartStk[TASK_STK_SIZE];          /* Startup    task stack                         */
OS_STK          TaskClkStk[TASK_STK_SIZE];            /* Clock      task stack                         */
OS_STK          TaskAStk[TASK_STK_SIZE];              /* Task #1   producer #1  task stack             */
OS_STK          TaskBStk[TASK_STK_SIZE];              /* Task #2   Producer #2  task stack             */
OS_STK          TaskCStk[TASK_STK_SIZE];              /* Task #3   Consumer     task stack             */

TASK_USER_DATA  TaskUserData[5];

OS_EVENT *SemaCntFull;
OS_EVENT *SemaCntEmpty;
OS_EVENT *SemaBinary;

char buf[BUF_SIZE];
static int head, tail;

void  TaskStart(void *data);
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);
void  TaskClk(void *data);
void  TaskA(void *data);
void  TaskB(void *data);
void  TaskC(void *data);
void  DispTaskStat(INT8U id);
char  ProduceChar(int CASE);
char  ConsumeChar(); 

int main (void)
{
    head = tail = 0;

    PC_DispClrScr(DISP_BGND_BLACK);                        /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    PC_ElapsedInit();                                      /* Initialized elapsed time measurement     */

    SemaCntFull = OSSemCreate(BUF_SIZE);   // create a counting semaphore for full
    SemaCntEmpty = OSSemCreate(BUF_SIZE);
    SemaBinary = OSSemCreate(1); // create a binary semaphore

    strcpy(TaskUserData[TASK_START_ID].TaskName, "StartTask");
    OSTaskCreateExt(TaskStart,
                    (void *)0,
                    &TaskStartStk[TASK_STK_SIZE - 1],
                    TASK_START_PRIO,
                    TASK_START_ID,
                    &TaskStartStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_START_ID],
                    0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}


void  TaskStart (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT16S     key;

    INT16U value;
    value = 1;

    pdata = pdata;                                         /* Prevent compiler warning                 */
    TaskStartDispInit();                                   /* Setup the display                        */
    OSStatInit();                                         /* Initialize uC/OS-II's statistics         */
    while(value>0)
	value = OSSemAccept(SemaCntFull);                 /* decrement SemaCntFull to 0 */


    TaskStartCreateTasks();

    for (;;) {
        TaskStartDisp();                                  /* Update the display                       */

        if (PC_GetKey(&key)) {                             /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                exit(0);                                   /* Yes, return to DOS                       */
            }
        }

        OSCtxSwCtr = 0;                                    /* Clear the context switch counter         */
        OSTimeDly(OS_TICKS_PER_SEC);                       /* Wait one second                          */
    }
}

void  TaskStartCreateTasks (void)
{
    strcpy(TaskUserData[TASK_CLK_ID].TaskName, "Clock Task");
    OSTaskCreateExt(TaskClk,
                    (void *)0,
                    &TaskClkStk[TASK_STK_SIZE - 1],
                    TASK_CLK_PRIO,
                    TASK_CLK_ID,
                    &TaskClkStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_CLK_ID],
                    0);

    strcpy(TaskUserData[TASK_A_ID].TaskName, "Producer A");
    OSTaskCreateExt(TaskA,
                    (void *)0,
                    &TaskAStk[TASK_STK_SIZE - 1],
                    TASK_A_PRIO,
                    TASK_A_ID,
                    &TaskAStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_A_ID],
                    0);

    strcpy(TaskUserData[TASK_B_ID].TaskName, "Producer B");
    OSTaskCreateExt(TaskB,
                    (void *)0,
                    &TaskBStk[TASK_STK_SIZE - 1],
                    TASK_B_PRIO,
                    TASK_B_ID,
                    &TaskBStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_B_ID],
                    0);

	strcpy(TaskUserData[TASK_C_ID].TaskName, "Consumer");
    OSTaskCreateExt(TaskC,
                    (void *)0,
                    &TaskCStk[TASK_STK_SIZE - 1],
                    TASK_C_PRIO,
                    TASK_C_ID,
                    &TaskCStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_C_ID],
                    0);

}

static  void  TaskStartDispInit (void)
{
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
    PC_DispStr( 0,  0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED);
    PC_DispStr( 0,  1, "  Original version by Jean J. Labrosse, 80x86-LINUX port by Werner Zimmermann   ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  3, "                                   Project #3                                   ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  9, "Task Name   Prio   Counter  Exec.Time(uS)   Tot.Exec.Time(uS)  %Tot.   Char     ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 10, "---------- ------  -------  -------------   -----------------  -----  ------    ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 20, "#Tasks          :        CPU Usage:     %                                       ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 21, "#Task switch/sec:                                                               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 22, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

static  void  TaskStartDisp (void)
{
    char   s[80];

    sprintf(s, "%5d", OSTaskCtr);                                  /* Display #tasks running               */
    PC_DispStr(18, 20, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

#if OS_TASK_STAT_EN > 0
    sprintf(s, "%3d", OSCPUUsage);                                 /* Display CPU usage in %               */
    PC_DispStr(36, 20, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
#endif

    sprintf(s, "%5d", OSCtxSwCtr);                                 /* Display #context switches per second */
    PC_DispStr(18, 21, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

    sprintf(s, "uCOS-II V%1d.%02d  LINUX V%1d.%02d", OSVersion() / 100, OSVersion() % 100, OSPortVersion() / 100, OSPortVersion() % 100); /* Display uC/OS-II's version number    */ 
    PC_DispStr(52, 21, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

}

void  TaskA (void *pdata)
{
    char msg;
    INT8U err;
    OS_TCB task_data;

    pdata = pdata;

    for (;;)
    { 
	OSSemPend(SemaCntEmpty, 0, &err);
	OSSemPend(SemaBinary, 0, &err);
	msg=ProduceChar(UPPERCASE);
	OSSemPost(SemaBinary);        
        OSSemPost(SemaCntFull);
	PC_DispChar(73, 13, msg, DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDly(OS_TICKS_PER_SEC/2);
    }
}

void  TaskB (void *pdata)
{
    INT8U err;
	char  msg;
    OS_TCB task_data;

    pdata = pdata;

    for(;;)
    {
	OSSemPend(SemaCntEmpty, 0, &err );
	OSSemPend(SemaBinary, 0, &err);
	msg=ProduceChar(LOWERCASE);
        OSSemPost(SemaBinary); 
        OSSemPost(SemaCntFull);
	PC_DispChar(73, 14, msg, DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDly(OS_TICKS_PER_SEC/3);
    }
}

void TaskC(void *pdata)
{
    INT8U err;
    char msg;

    pdata = pdata;

    for(;;)
    {
	OSSemPend(SemaCntFull, 0, &err);
	OSSemPend(SemaBinary, 0, &err);
	msg= ConsumeChar();
	OSSemPost(SemaBinary); 
	OSSemPost(SemaCntEmpty);
	PC_DispChar(73, 15, msg, DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDly(OS_TICKS_PER_SEC/5);
    }
}

void  TaskClk (void *pdata)
{
    char s[40];
    pdata = pdata;
    for (;;) {
        PC_GetDateTime(s);
        PC_DispStr(56, 20, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDly(OS_TICKS_PER_SEC);
    }
}

void  DispTaskStat(INT8U id)
{
    char  s[80];
    sprintf(s, "%-10s  %3u     %05u      %5u          %10ld",
	    TaskUserData[id].TaskName,
	    TaskUserData[id].TaskPrio,
	    TaskUserData[id].TaskCtr,
	    TaskUserData[id].TaskExecTime,
	    TaskUserData[id].TaskTotExecTime);
    PC_DispStr(0, (INT8U) (id + 11), s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
}

char  ProduceChar(int CASE)
{
   char temp;

   if(CASE==UPPERCASE)
	temp = (rand()%26)+65;
   else
	temp = (rand()%26)+97;

   buf[head] = temp; 
   head = (head+1)%BUF_SIZE; 
   return temp;
}

char  ConsumeChar()
{
	char temp;
	temp=buf[tail];
	tail=(tail+1)%BUF_SIZE;
	return temp;
}
