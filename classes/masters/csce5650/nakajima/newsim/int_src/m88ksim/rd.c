/* @(#) File name: rd.c   Ver: 3.2   Cntl date: 3/6/89 12:15:02 */
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

extern	char	*cmdptrs[];
extern	int	cmdcount;
extern	int	cmdflags;

	/**********************************************************
	*
	*       Local Defines
	*
	*/

#define	NORMAL	(0)
#define	SINGLE	(1)
#define	DOUBLE	(2)


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
	*      rd()
	*/


int rd(void)
{
	int format = NORMAL;

	if(cmdflags & TYPE)
		format = ((trans.type & SF) ? SINGLE: DOUBLE);

	if(((cmdcount == 2) && !(cmdflags & TYPE)) || (cmdcount > 2))
	{
		if((cmdcount == 2) && (*(cmdptrs[1] + 1) == '\0'))
		{
			switch(*cmdptrs[1])
			{
				case '0':	/* SFU 0 Control reg. */
					dispSFU(m88000.SFU0_regs);
					return 0;

				case '1':	/* SFU 1 Control reg. */
					dispSFU(m88000.SFU1_regs);
					return 0;

				default:
					break;
			}
		}
		PPrintf("rd error: unknown command option\n");
		return(-1);
	}
	rdexec(format);
	return 0;
}


void rdexec(int format)
{
	int i;

	for (i = 0; i < 32; i++)
	{
		Eprintf("   r%02d:  ", i);
		if(format == NORMAL)
		{
			Eprintf("%08X", m88000.Regs[i]);
			if ((i % 4) == 3)
				Eprintf("\n");
		}
		else if(format == SINGLE)
		{
			PPrintf("%.7e", *(float *)(&m88000.Regs[i]));
			if ((i % 3) == 2)
				PPrintf("\n");
		}
		else
		{
			union {int i[2]; double d;}u;
			u.i[0] = m88000.Regs[i];
			u.i[1] = m88000.Regs[i+1];
			PPrintf("%.16e", u.d);
			if (i % 2)
				PPrintf("\n");
		}
	}

	if(format ==SINGLE)
		PPrintf("\n");

	Eprintf("   ip:   0x%08X   vbr:  0x%08X   psr:  0x%08X\n", IP, VBR, PSR);
	Eprintf("   sbd:  0x%08X\n", m88000.scoreboard);
	Eprintf("   fpsr: 0x%08X   fpcr: 0x%08X \n", FPSR, FPCR);

	return;
}

void dispSFU(int *p)
{
	int i;

	for (i = 0; i < 64; i++)
	{
		Eprintf("   cr%02d:  %08X", i, p[i]);
		if ((i % 4) == 3)
			Eprintf("\n");
	}
	return;
}
