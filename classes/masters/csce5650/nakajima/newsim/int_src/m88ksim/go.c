/* @(#) File name: go.c   Ver: 3.2   Cntl date: 3/6/89 12:12:11 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


# ifdef m88000
# undef m88000
# endif

#include "functions.h"
#include "trans.h"


	/**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/



	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	int	cmdcount;
extern	int	brkenabled;
extern	int	brkpend;
extern	unsigned int curbrkaddr;
extern	int	memerr;
extern	int	interrupt;
extern	int	quit;
extern	int	buserr;
extern	int	segviol;
extern	int	trace_flg;
extern	int	silent;

	/**********************************************************
	*
	*       Local Defines
	*
	*/


	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/



	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      go()
	*/


int go(void)
{
	int dpathret;

 	if(cmdcount != 1)
	{
		IP = trans.adr1;
	}

	PPrintf("Effective address: %08X\n", IP);

	if((brkpend) && (IP = curbrkaddr))
	{
		switch(dpathret = Data_path())
		{
			case 2:			/* sysV exit */
				if(silent)
					return(dpathret);
			case -1:		/* Major processing error */
				rdexec(0);
				return(dpathret);

			case 1:			/* Exception return */
			default:		/* Normal return */
				break;
		}
	}
	brkptenb();

	return(goexec());
}

int gd(void)
{
 	if(cmdcount != 1)
	{
		IP = trans.adr1;
	}
	PPrintf("Effective address: %08X\n", IP);

	brkenabled = 0;

	return(goexec());
}


int goexec(void)
{
	int dpathret;
	memerr = 0;
	trace_flg = 0;

	while(!interrupt && !quit && !buserr && !segviol)
	{
		switch(dpathret = Data_path())
		{
			case 2:			/* sysV exit */
				if(silent)
					return(dpathret);
			case -1:		/* Major processing error */
				brkenabled = 0;
				rdexec(0);
				return(dpathret);

			case 1:			/* Exception return */
				/*
				** force simulator to abort on exception
				*/
				quit = 0;
			default:		/* Normal return */
				break;
		}
	}
	brkenabled = 0;

	return(0);
}


	/**********************************************************
	*
	*      gn()
	*/


int gn(void)
{
	settmpbrk(IP + 4);
	go();
	rsttmpbrk();
	return 0;
}


	/**********************************************************
	*
	*      gt()
	*/


int gt(void)
{
	if(cmdcount != 2)
	{
		PPrintf("Temporary Break point address required\n");
		return(-1);
	}

				/*
				 * Set cmdcount = 1 so that "go" will not
				 * interpret the breakpoint address as a
				 * start address.
				 */
	cmdcount = 1;

	settmpbrk(trans.adr1);
	go();
	rsttmpbrk();
	return(0);
}


	/**********************************************************
	*
	*      tr()
	*/


int tr(void)
{
	trace_flg = 1;

	if(cmdcount != 0)
		trans.cnt = (cmdcount == 1) ? 1: trans.expr;

	return(trexec(trans.cnt));
}





	/**********************************************************
	*
	*      tv()
	*/


int tv(void)
{
	trace_flg = 0;

	if(cmdcount != 0)
		trans.cnt = (cmdcount == 1) ? 1: trans.expr;

	return(trexec(trans.cnt));
}



	/**********************************************************
	*
	*      tx()
	*/


int tx(void)
{
	trace_flg = 2;

	return(trexec(0xFFFFFFFF));
}



	/**********************************************************
	*
	*      tz()
	*/


int tz(void)
{
	trace_flg = 4;

	return(trexec(0xFFFFFFFF));
}






	/**********************************************************
	*
	*      tt()
	*/


int tt(void)
{
	trace_flg = 1;

	if(cmdcount != 2)
	{
		PPrintf("Temporary Break point address required\n");
		return(-1);
	}
	settmpbrk(trans.adr1);
	trexec(0xFFFFFFFF);
	rsttmpbrk();
	return(0);
}



	/**********************************************************
	*
	*      trexec()
	*/

int trexec(unsigned int cnt)
{

	unsigned int opcode;
        extern int     instr_cnt, Clock;
	extern int	show_regs, show_bus, show_instr, show_clock, sym_addr;
	memerr = 0;

	if((brkpend) && (IP = curbrkaddr))
	{
		switch(Data_path())
		{
			case 2:			/* sysV exit */
			case -1:		/* Major processing error */
				return(0);

			case 1:			/* Exception return */
			default:		/* Normal return */
				break;
		}
		cnt--;

		if(!trace_flg)
        	{
			if(show_regs)
				rdexec(0);

			if(show_bus)
			{
				trans.adr1 = IP;
				PPrintf ("                %08X %08X %08X\n", 
				        m88000.ALU, m88000.S1bus, m88000.S2bus);
			}

			if(show_instr)
			{
        			if(rdwr((M_INSTR | M_RD), IP, &opcode, 4) != -1)
				{
					if (sym_addr)
						PPrintf("%12s %08X %08X %s\n",
						       symbol(M_INSTR, IP), IP, 
						       opcode, dis(opcode, IP));
					else
						PPrintf("%08X %08X %s\n", IP, 
						       opcode, dis(opcode, IP));
				}
			}

			if(show_clock)
				PPrintf("Clock : %d, Instr : %d\n", Clock, 
				       instr_cnt);
		}

	}
	brkptenb();

	while(cnt-- && !interrupt && !quit && !buserr && ! segviol)
	{
		switch(Data_path())
		{
			case 2:			/* sysV exit */
				return(0);

			case -1:		/* Major processing error */
				brkenabled = 0;
				return(-1);

			case 1:			/* Exception return */
			default:		/* Normal return */
				break;
		}

		if(!trace_flg)
		{
                        if(show_regs)
                                rdexec(0);

                        if(show_bus)
                        {
                                trans.adr1 = IP;
                                PPrintf ("                %08X %08X %08X\n",
                                        m88000.ALU, m88000.S1bus, m88000.S2bus);
                        }

                        if(show_instr)
                        {
        			if(rdwr((M_INSTR | M_RD), IP, &opcode, 4) != -1)
				{
                                	if (sym_addr)
						PPrintf("%12s %08X %08X %s\n",
	                                               symbol(M_INSTR, IP), IP,
						       opcode, dis(opcode, IP));
					else
						PPrintf("%08X %08X %s\n", IP,
						       opcode, dis(opcode, IP));
				}
                        }

                        if(show_clock)
                                PPrintf("Clock : %d, Instr : %d\n", Clock,
                                       instr_cnt);
		}
	}
	brkenabled = 0;

	return(0);
}
