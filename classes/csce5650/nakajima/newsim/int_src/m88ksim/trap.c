/* @(#) File name: trap.c   Ver: 3.4   Cntl date: 4/4/89 14:57:05 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


/*
 * Trap and Exception Handling logic for M88000 Simulator
 */

# ifdef m88000
# undef m88000
# endif
#include "functions.h"

#define tmode 31
#define texm  18
#define ttrm  17

#define TRAPREG	(30)

#define pswmask (1<<psrmode) | (1<<exm) | (1<<trm)

extern struct mem_wrd  *getmemptr ();

extern int	unixvec, vecyes, trace_flg;
extern int 	debugflag;
extern char	null[];
extern int	stdio_en;


/*********************************************************************
*	vector
*/
int vector(struct IR_FIELDS *ir,int vec)
{
	struct cmmu	*cmmu_ptr;
#ifdef NOT_SPEC95
	int 		retstat;
#endif
	int		syscall_num;

	if(vec > 0x1ff)
	{
		Eprintf("Vector to large\n");
		return(-1);
	}

	if((vecyes) && (ir->op == RTE))
	{
		if(!(PSR & 0x80000000))
			return(exception(E_TPRV, "Supervisor Privilege Violation"));

		PSR = TPSR;			/* get saved psr */
		m88000.scoreboard = TSB;	/* get scoreboard */

		if(TNIP & 2)
			IP = TNIP;		/* get saved ip */
		else if(TFIP & 2)
			IP = TFIP;
		else
			IP = TFIP + 4;

		IP &= 0xFFFFFFFC;
		return(0);
	}

	if(ir->op == TBND)
		return(exception(E_TBND, "Array Bounds Violation"));

/*
 * Process system call vectors.
 * Entry to this section is done if the trap vector is 80 which
 * indicates old scheme sysV system call interface or if the vector
 * is >= 0x100 which indicates Berkeley traps or new scheme sysV
 * system call interface vectors between 0x81 and 0x99 are used for
 * other purposes

   The BCS Standard corrected of July 1988 uses trap number 450 for all
   required POSIX system calls.
 */

	if(((vec == 0x80) || (vec >= 0x100)) && (ir->op >= (unsigned)TB0) && (ir->op <= (unsigned)TCND))
	{
		if(unixvec)
		{
			if ((!stdio_en) && (vec == 450))
			{
			/* Flush the data cache for sys call */
				cmmu_ptr = &Dcmmu;
				cmmu_ptr->control.SCMR = 0x0000001b;
				cmmu_ctrl_func(cmmu_ptr);
				/*
				** OCS_BCS system call interface
				*/
				syscall_num = (int) m88000.Regs[9];
				return(sysVbcs(syscall_num));
			}

#ifdef NOT_SPEC95
			/*
			** stdio IO interface
			*/
			if ((stdio_en) && (((retstat = stdio_io(vec)) == -1) || !retstat))
				return(retstat);
#endif

		}
#ifdef NOT_SPEC95
					/*
					** basic IO alway in
					*/
		if(((retstat = fake_io(vec)) == -1) || !retstat)
			return(retstat);
#endif


	}
	else if((vecyes) && (vec >= 0x00) && (vec <= 0x7F))
	{
		if(!(PSR & 0x80000000))
			return(exception(E_TPRV, "Supervisor Privilege Violation"));
	}

	return(exception(vec, null));
}


/*
*******************************************************************************
*
*       Module:     exception()
*
*       File:       trap.c
*
*******************************************************************************
*
*  Functional Description:
*	exception() is called by the opcode execution function (where runtine
*	exceptions are recognized) and when traps are requested that are not
*	covered by the System V, Simulated IO or Fake IO calls.
*
*  Globals:
*
*  Parameters:
*
*  Output:
*	returns	 1	If an exception is taken.
*	returns -1	If an exception vector is not found or the (-v)
*			command line option is not specified (vecyes is false).
*
*  Revision History:
*	Returns a value of (1) if an exception is taken.  A (1) must be
*	returned so that the function calling Data_path() knows that an
*	exception occured.
*
*******************************************************************************/

int exception(int vec, char *message)
{
	int tempip;

	if(trace_flg || !vecyes)
		Eprintf("\n%s\n", message);

	if(!vecyes)
	{
		Eprintf("No vector table support or undefined vector for trap handlers\n");
		Eprintf("vector number: %d\n", vec);
		Eprintf("Use -v option on invocation line for vector table support\n");
		return(-1);
	}

	TPSR = PSR;					/* save psr */
	TCIP = IP | 2;					/* save ip */

	if(vec == E_TCACC)				/* code bus error ? */
		TCIP |= 0x1;				/* set exception bits */

	/* set up NIP */
	if(getmemptr(TNIP = (IP + 4) | 2, M_INSTR) == 0)
		TNIP |= 0x1;				/* set exception bits */

	/* set up FIP */
	if(getmemptr(TFIP = (IP + 8) | 2, M_INSTR) == 0)
		TFIP |= 0x1;				/* set exception bits */

	TSB = m88000.scoreboard;			/* save scoreboard */
	PSR |= 0x800001FB;				/* set supervisor state */
	tempip = VBR + (vec << 3);			/* get new ip */

	if(debugflag)
		PPrintf("VBR = %08X \n", VBR);

	if(debugflag)
		PPrintf("tempip = %08X\n", tempip);

	if (getmemptr (tempip, M_INSTR) == 0)
	{
		Eprintf("No vector table found\n");
		Eprintf("Instruction Address %08X\n", IP);
		Eprintf("Processor Halted\n");
		return (-1);
	}

	IP = tempip;
	return(1);		/* A return value of 1 indicates an exception */
}

