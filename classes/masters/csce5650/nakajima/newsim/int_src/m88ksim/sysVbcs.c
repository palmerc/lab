/* @(#) File name: sysV.c   Ver: 3.2   Cntl date: 4/18/89 06:32:45 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif
#include "functions.h"
#include "trans.h"
#ifdef NT
#include <io.h>
#elif VMS
#include <unix/io.h>
#else
/* presumed to be UNIX ! */
#include <unistd.h>
#endif

#define RDWRBUF	(2048)

static int	runtimes;

static unsigned char tty[] = {	0,0,5,0,
				0,0,0,5,
				0,13,4,0xb0,
				0,0,0x8a,0x3b,
				1,0x7f};

int sysVbcs(int sys_call_index)
{
	int	arg1, arg2, arg3;	/* passed args */
	int	i, j, k;		/* work vars. */
	char	rdwrbuf[RDWRBUF];
	unsigned int	TimeS;
	unsigned int	TimeS0;

	i = j = k = 0;
	arg1 = getarg(1);		/* get the first three args */
	arg2 = getarg(2);
	arg3 = getarg(3);

	switch(sys_call_index)
	{
		default:
			Eprintf("Undefined sysV BCS trap call: %d\n", sys_call_index);
			return -1;

		case	2:		/* brk */
			(void) getpmem(arg1);
			m88000.Regs[2] = 0;
			break;

		case	9:		/* exit */
			sysVclose();
			PPrintf("Program \"EXIT\", exit value %d\n", m88000.Regs[2]);
			return(2);

		case	23:		/* ioctl */
			/* Implement only a fudge for isatty for SPEC */

			if (arg2 == 0x402474c0) {
				rdwr(M_WR|M_DATA,(unsigned int)arg3,(void *)tty,18);
				m88000.Regs[2] = 0;
			}else{
				/* return -1 for all stermio commands */
				m88000.Regs[2] = -1;
			}
			break;
		case	60:		/* times */

			/*
			** the simulated time passed is original initial time plus
			** the time passed since start of simulation
			*/
			TimeS = runtimes + (readtime() / 200000);

			/*
			** caculate the number of 60'ths of second of time, and
			** fill all elements of simulated tms structure with 
			this value
			*/

			intswap(&TimeS0,&TimeS);
			{
				unsigned int timbuf[4];

				timbuf[0] = TimeS0;
				timbuf[1] = TimeS0;
				timbuf[2] = TimeS0;
				timbuf[3] = TimeS0;
				m88000.Regs[2] = TimeS0;
				(void) rdwr((M_DATA | M_WR), arg1, timbuf, 16);
			}
			break;

		case	68:		/* write */
			trans.adr1 = arg2;
			m88000.Regs[2] = 0;
			do
			{
				i = (arg3 < RDWRBUF) ? arg3 : RDWRBUF;	/* size for buffer */
 				if(rdwr((M_DATA | M_RD), trans.adr1, rdwrbuf, i) == -1) /* move */
					return(-1);
				if((j = write(arg1, rdwrbuf, i)) == -1)	/* write into buffer */
				{
					m88000.Regs[2] = -1;		/* set error */
					break;				/* if error return error */
				}
				m88000.Regs[2] += j;			/* update total read */
				trans.adr1 += j;			/* update memory pointer */
			}
			while((j == i) && ((arg3 -= j) > 0));		/* go until done */
			break;

	}

	if(m88000.Regs[2] == -1)
	{
		IP += 4;		/* Bump past the TRAP instruction */
	}
	else
		IP += 8;		/* bump past the TRAP instruction and the error handler */

	return(0);
}

#include "fildes.h"
int      openfildes[SZOPENFILDES] = { -1, -1, -1, -1, -1, -1, -1, -1 };


int sysVupfil(int fildes)
{
	int	i = 0;
	do
		if(!(openfildes[i] + 1))		/* find first open slot */
		{
			openfildes[i] = fildes;		/* save for exit close */
			break;
		}
	while(++i < SZOPENFILDES);			/* go for all */
	if(i == SZOPENFILDES)
	{
		close(fildes);
		Eprintf("\"OPEN:\" max files open\n");
		return(-1);
	}
	return(0);
}



void sysVclose(void) {
	int	numfildes = 0;

	do
		if(openfildes[numfildes] + 1) {
			close(openfildes[numfildes]);
			openfildes[numfildes] = -1;
		}
	while(++numfildes < SZOPENFILDES);
	return;
}
