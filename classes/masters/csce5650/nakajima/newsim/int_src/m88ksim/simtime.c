/* @(#) File name: simtime.c   Ver: 3.3   Cntl date: 2/19/89 15:50:39 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
# ifdef m88000
# undef m88000
# endif

#include "functions.h"

#define CLASSBITS	5
#define BUSYBITS	16
#define MAX_BUSY	((1 << BUSYBITS) - 1)
#define MIN( x, y )	((x)<(y)?(x):(y))

typedef struct {
	char *			name;
	unsigned int	class;
	unsigned int	busy;
} funct_unit;

static funct_unit funct_units[] = {
	{"Integer ALU",			INT_CLASS},
	{"Branch Unit",			BR_CLASS},
	{"Load/Store Unit",		LDST_CLASS},
	{"Multiplier",			FMUL_CLASS},
	{"FP Adder",			FADD_CLASS},
	{"No Such Unit",		ILLEGAL_CLASS}
};


extern int debugflag;
extern int trace_flg;


/*
*******************************************************************************
*
*       Module:     test_issue()
*
*       File:       issue.c
*
*******************************************************************************
*
*  Functional Description:
*	This procedure determines if the given instruction can be issued.
*	If there is a data dependency, the appropriate number of clocks are
*	are calculated.  Then the state of the functional units is checked.
*	If there is a busy hold on the functional unit, the appropriate number
*	of clocks are added to the clocks-to-be-expired total.  This total
*	number of clocks is returned.  A value of (-1) is returned on error.
*
*  Globals:
*
*  Parameters:
*
*  Output:
*
*  Revision History:
*
*
*******************************************************************************/

unsigned int	j;
unsigned int	latency;

int test_issue(struct IR_FIELDS *ir, struct SIM_FLAGS *f)
{
	unsigned int	i=0;
	unsigned int	retval=0;
	unsigned int	minclk=MAX_BUSY;

	j = 0;

	/* see if the scoreboard will allow the issue */

	retval = check_scoreboard ( ir, f );
/*		there used to be a call to KILLTIME here */
	latency = f->fu_busy;

	/* calculate the minimum clocks needed until an FU is available */

	while ( funct_units[i].class != ILLEGAL_CLASS )
	{
		if( f->class == funct_units[i].class )
		{
			if ( minclk > funct_units[i].busy )
			{
				minclk = funct_units[i].busy;
				j = i;

				/* if minclk is zero, break to terminate search */
				if ( minclk == 0 ) break;
			}
		}

		i++;
	}

	if ( minclk == MAX_BUSY )
	{
		/* an illegal instruction class has been used */
		Eprintf("issue(): illegal instruction class %d at IP = 0x%x\n",
					f->class, IP );
		return(-1);
	}

/*
	retval = minclk;
*/
	retval += minclk;

	return ( retval );
}

/**************************************************************************
 *
 *	do_issue()
 *
 *	This procedure actually issues an instruction.  Data dependencies are
 *	NOT checked (test_issue() is assumed to have been called before
 *	and killtime() called with the result).
 *
 *************************************************************************/

void do_issue(void)
{

	/* set up the functional unit busy counter */

	funct_units[j].busy += latency;
	return;
}



/* This module computes the instruction execution time for the M88000
 * simulator.
 */

unsigned int   Clock;		/* The counter of clock cycles consumed */
extern int usecmmu;
extern int Dcmmutime;
extern int mbus_latency;

#define MIN(x,y)	((x)<(y)?(x):(y))

/* This routine is called whenever an instruction is executed, and when
 * an instruction has to stall because an operand is not ready.
 */

void killtime (unsigned int   time_to_kill)
{
	register int     i;

	/* 
	* increment the clock and decrement the time_left field of each register
	*/

	Clock += time_to_kill;

	/* decrement all registers with time remaining */


	for (i = 0; i < 32; i++)
	{
		m88000.time_left[i] -= MIN(m88000.time_left[i], time_to_kill);
	}

	if(usecmmu)
	{
		Dcmmutime -= MIN(Dcmmutime, time_to_kill);
		mbus_latency -= MIN(mbus_latency, time_to_kill);
	}

        /* kill the indicated number of clocks off the busy timers in the FUs */ 
        for(i=0;i<=4;i++) 
        {
                funct_units[i].busy -= MIN( funct_units[i].busy, time_to_kill );
        }
 

}


int readtime (void)
{
    return (Clock);
}


int check_scoreboard (struct IR_FIELDS *ir, struct SIM_FLAGS *f)
{
    int     time;
    int     i;

	/*
	 * Read the scoreboard , check for registers waiting on results, if
	 * any are waiting, kill time until they are ready for use.
	 *
	 * The rules:
	 *	Check all source registers and destination if the
	 *	instruction uses them.
	 *
	 *	Check r1 for call types of instructions.
	 *
	 *	Check both registers when a double result is returned.
	 *
	 *	Check all registers before doing a trap instruction.
	 *
	 *	Check all registers if serialized mode (not implemented)
	 */

    time = 0;

    if ((f -> rs2_used)				/* if source 2 register */
	    && (ir->src2)							/* and it is not r0 */
	    && (m88000.time_left[ir -> src2] != 0))	/* and if not ready */
		time = m88000.time_left[ir -> src2];

    if ((f -> rs1_used)
	    && (ir->src1)								/* and it is not r0 */
	    && (m88000.time_left[ir -> src1] > time))	/* if src1 is not ready */
		time = m88000.time_left[ir -> src1];

    if ((f -> rsd_used)
	    && (ir->dest)				/* and it is not r0 */
	    && (m88000.time_left[ir -> dest] > time))
	{
		time = m88000.time_left[ir -> dest];
	}

/* also check next consecutive register for LOAD DOUBLE */

	if ((ir->op == LDD)
		&& (f -> rsd_used)
		&& ((ir->dest) != 31)) /* and it is not r0 because of wrap around */
		if (m88000.time_left[(ir -> dest)+1] > time)
			time = m88000.time_left[(ir -> dest)+1];

/* for stores */
    if (!(f -> rsd_used)
	    && (ir->dest)				/* and it is not r0 */
	    && (m88000.time_left[ir -> dest] > time)
		&& ((ir->op >= (unsigned)STB) && (ir->op <= (unsigned)STD)))
	{
		time = m88000.time_left[ir -> dest];
	}

/* also check next consecutive register for STORE DOUBLE */

	if ((ir->op == STD)
		&& !(f -> rsd_used)
		&& ((ir->dest) != 31)) /* and it is not r0 because of wrap around */
		if (m88000.time_left[(ir -> dest)+1] > time)
			time = m88000.time_left[(ir -> dest)+1];


    if ((ir -> op >= (unsigned)TB0) && (ir -> op <= (unsigned)RTE))
		for (i = 1; i < 32; i++)
			if (m88000.time_left[i] > time)
				time += m88000.time_left[i];

    if ((ir -> op == JSR) || (ir -> op == BSR))
		if (m88000.time_left[1] > time)
	    	time = m88000.time_left[1];


	return(time);
}
