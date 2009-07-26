/* @(#) File name: br.c   Ver: 3.1   Cntl date: 1/20/89 14:20:12 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*
*/

#include "functions.h"
#include "trans.h"
#include "br.h"
#include <string.h>


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
extern	int	cmdflags;
extern	char	*cmdptrs[];
extern	int	brkpend;
extern	int	brkenabled;


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

struct	brkpoints brktable[TMPBRK] = { {0} };
unsigned int curbrkaddr = 0;


	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      br()
	*/


int br(void)
{
	struct brkpoints *bp, *target;
	int Index, cnt;
	unsigned int value1, value2;
	char *ptr, *ptr1;
	char *errptr;

	for(Index = 1; Index < cmdcount; Index++)
	{	
		if(ptr1 = strchr((ptr = cmdptrs[Index]), ':'))
			*ptr1++ = '\0';

		if((getexpr(ptr, &errptr, &value1) != 0)
		  || (ptr1 && (getexpr(ptr1, &errptr, &value2) != 0)))
		{
			PPrintf("Illegal value entered %s\n", ptr);
			continue;
		}
		for(cnt = 0, bp = brktable, target = 0; cnt < MAXBRK; cnt++, bp++)
		{
			if(!target && !bp->code)
				target = bp;
			if(bp->code && (bp->adr == value1))
			{
				target = bp;		/* assign new break */
				PPrintf("Changing break point at address %08X\n", value1);
				break;
			}
		}
		if(!target)
		{
			PPrintf("Break table full\n");
			break;
		}
		target->code = BRK_EXEC;
		target->adr = value1;
		if(ptr1)
		{
			target->limit = value2;
			target->code |= BRK_CNT;
		}
		if(cmdflags & TYPE)
		{
			switch(*cmdptrs[Index + 1])
			{
				case 'w':
					target->code |= BRK_WR;
					break;

				case 'r':
					target->code |= BRK_RD;
					break;

				default:
					continue;
			}
			switch(*(cmdptrs[Index + 1] + 1))
			{
				case 'w':
					target->code |= BRK_WR;
					break;

				case 'r':
					target->code |= BRK_RD;
				case '\0':
					break;

				default:
					continue;
			}
			Index++;
		}
	}
	return(dbrks());
}

int nobr(void)
{
	struct brkpoints *bp = brktable;
	int Index, cnt;
	unsigned int value1;
	char *errptr;

	if(cmdcount == 1)
		for(cnt = 0; cnt < MAXBRK; cnt++, bp++)
			bp->code = 0;

	for(Index = 1; Index < cmdcount; Index++)
	{	
		if(getexpr(cmdptrs[Index], &errptr, &value1) != 0)
		{
			PPrintf("Illegal value entered %s\n", cmdptrs[Index]);
			continue;
		}
		for(cnt = 0; cnt < MAXBRK; cnt++, bp++)
		{
			if(bp->code && (bp->adr == value1))
			{
				bp->code = 0;
				break;
			}
		}
	}
	return(dbrks());
}

int dbrks(void)
{
	struct brkpoints *bp = brktable;
	int Index, cnt;

	PPrintf("BREAKPOINTS\n");
	for(Index = cnt = 0; Index < MAXBRK; Index++, bp++)
	{
		if(bp->code != 0)
		{
			cnt++;
			PPrintf("%08X", bp->adr);
			if(bp->code & BRK_CNT)
				PPrintf(":%X", bp->limit);
			if(bp->code & (BRK_WR | BRK_RD))
			{
				putchar(';');
				if(bp->code & BRK_WR)
					putchar('w');
				if(bp->code & BRK_RD)
					putchar('r');
			}
			putchar('\t');
			if((cnt & 0x3) == 0)
				putchar('\n');
		}
	}
	return(0);
}


int ckbrkpts(unsigned int addr, int brktype)
{
	struct brkpoints *bp = brktable;
	int cnt;

	if(!brkenabled)
		return(0);

	for(cnt = 0; cnt < TMPBRK; cnt++, bp++)
		if(bp->code && ((bp->adr & ~0x3) == addr))
			break;

	if(cnt == TMPBRK)
		return(0);

	if(!(bp->code & brktype))
		return(0);

	if(bp->code & BRK_CNT)
		if(++bp->cnt != bp->limit)
			return(0);

	if((curbrkaddr = addr) != IP)
		curbrkaddr = IP;
	PPrintf("At Breakpoint\n");
	brkpend = 1;
	return(-1);
}



int brkptenb(void)
{
	struct brkpoints *bp = brktable;
	int cnt;

	for(cnt = 0; cnt < TMPBRK; cnt++, bp++)
		bp->cnt = 0;

	brkenabled = 1;
	brkpend = 0;

	return(0);
}


void settmpbrk(int addr)
{
	brktable[MAXBRK].code = BRK_EXEC;
	brktable[MAXBRK].adr = addr;
}

void rsttmpbrk(void)
{
	brktable[MAXBRK].code = 0;
}
