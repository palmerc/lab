/* @(#) File name: rm.c   Ver: 3.1   Cntl date: 1/20/89 14:20:38 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

#include "functions.h"
#include "trans.h"
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

struct regmap {
	char *name;
	unsigned int *reg;
	char sfunum, regnum;
};

	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/

static struct regmap regmap[] =
{
	{ "ip",		(unsigned int *)&IP,	0,	0 },
	{ "psr",	(unsigned int *)&PSR,	0,	1 },
	{ "vbr",	(unsigned int *)&VBR,	0,	7 },
	{ "fpsr",	(unsigned int *)&FPSR,	1,	62 },
	{ "fpcr",	(unsigned int *)&FPCR,	1,	63 },
	{ NULL,		NULL,			0,	0 }
};


	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      rm()
	*/


int rm(void)
{
	int regcnt = REGs + sizeof(regmap)/sizeof(struct regmap) - 1;
	int regnum = 0, direction = 1;
	char *errptr, inbuf[30];
	unsigned int *reg;
	unsigned int *reg1 = (unsigned int *)0;
	unsigned int new;
	union {
		unsigned int l[2];
		double d;
	} newd;
	int format = NORMAL;

	if(cmdflags & TYPE)
		format = ((trans.type & SF) ? SINGLE: DOUBLE);

	if(cmdcount != 1)
	{
		switch(*cmdptrs[1])
		{
			case 'r':
				*cmdptrs[1] = '&';
				if(getexpr(cmdptrs[1], &errptr, &new) != 0)
				{
					PPrintf("\nUnkown input \"%c\"\n", *errptr);
					return(-1);
				}
				if(((regnum = new) < 0) || (regnum > REGs))
					return(-1);
				break;

			case '0':
			case '1':
				if(strncmp("cr", cmdptrs[1] + 1, 2))
				{
					PPrintf("Unknown register name \"%s\"\n", cmdptrs[1]);
					return(-1);
				}
				*(cmdptrs[1] + 2) = '&';
				if(getexpr(cmdptrs[1]+ 2, &errptr, &new) != 0)
				{
					PPrintf("\nUnkown input \"%c\"\n", *errptr);
					return(-1);
				}
				if(((regnum = new) < 0) || (regnum > 63))
					return(-1);
				if(*cmdptrs[1] == '0')
					return(rmSFU(0, regnum));
				else
					return(rmSFU(1, regnum));

			default:
				while(regmap[regnum].name)
				{
					if(strcmp(cmdptrs[1], regmap[regnum].name) == 0)
					{
						regnum += REGs;
						break;
					}
					regnum++;
				}
				if(regnum < REGs)
				{
					PPrintf("Unknown register name \"%s\"\n", cmdptrs[1]);
					return(-1);
				}
				break;
		}
	}
	while(1)
	{
		if(regnum < REGs)
		{
			reg = (unsigned int *)&m88000.Regs[regnum];
			PPrintf("r%02d: ", regnum);
			if(format == NORMAL)
				PPrintf("%08X ", *reg);
			else if(format == SINGLE)
				PPrintf("%.7e ", *(float *)reg);
			else
			{
				if(regnum == 31)
					PPrintf("%.16e ", *reg, 0);
				else
				{
					reg1 = (unsigned int *)&m88000.Regs[regnum + 1];
					regnum += direction;
					PPrintf("%.16e ", *reg, *reg1);
				}
			}
		}
		else
		{
			reg = regmap[regnum - REGs].reg;
			PPrintf("%-4s: %08X ", regmap[regnum - REGs].name, *reg);
		}
		FFgets(inbuf, 30, stdin);

		switch(inbuf[0])
		{
			case	'v':
			case	'V':
				direction = 1;
				break;

			case	'^':
				direction = -1;
				break;

			case	'=':
				direction = 0;
				break;

			case	'.':
				return(0);

			case	'\n':
				break;

			default:
				switch(format)
				{
					case	DOUBLE:
						newd.d = atof(inbuf);
						break;

					case	SINGLE:
						new = atosf(inbuf);
						break;

					default:
						if(getexpr(inbuf, &errptr, &new) != 0)
						{
							PPrintf("\nUnkown input \"%c\"\n", *errptr);
							continue;
						}
						break;
				}
				if(regnum)
				{
					if(regnum > 32)
						wrctlregs(regmap[regnum - REGs].sfunum,
							regmap[regnum - REGs].regnum, new, 1);
						
					else if(format != DOUBLE) {
						*reg = new;
					}
					else
					{
						*reg = newd.l[0];
						if(regnum != 31)
							*reg1 = newd.l[1];
					}
				}
				break;
		}
		if((regnum += direction) >= regcnt)
			regnum = 0;
		else if(regnum < 0)
			regnum = regcnt - 1;
		
	}
}


int rmSFU(int num,int i)
{
	int direction = 1;
	char inbuf[30], *errptr;
	int new, *SFUptr;

	SFUptr = (num) ? m88000.SFU1_regs: m88000.SFU0_regs;

	while(1)
	{
		PPrintf("SFU%d:cr%02d:  %08X ", num, i, SFUptr[i]);
		FFgets(inbuf, 30, stdin);

		switch(inbuf[0])
		{
			case	'v':
			case	'V':
				direction = 1;
				break;

			case	'^':
				direction = -1;
				break;

			case	'=':
				direction = 0;
				break;

			case	'.':
				return(0);

			case	'\n':
				break;

			default:
				if(getexpr(inbuf, &errptr, (unsigned int *)&new) != 0)
				{
					PPrintf("\nUnkown input \"%c\"\n", *errptr);
					continue;
				}
				wrctlregs(num, i, new, 1);
				break;
		}
		if((i += direction) > 63)
			i = 0;
		else if(i == -1)
			i = 63;
	}
}
	
	
	/**********************************************************
	*
	*      getregval()
	*/

char * getregval(char *ptr,int *value)
{
	char reglookup[100];
	char *ptrsave, *bufptr, *errptr;
	int  regnum;

	ptrsave = ptr;
	bufptr = reglookup;

	while((*ptr != '\n') && (*ptr != '+')  && (*ptr != '-')  &&
		(*ptr != '*')  && (*ptr != '/')  && *ptr)
	{
		*bufptr++ = *ptr++;
	}
	*bufptr = '\0';
	bufptr = reglookup;
	if(*bufptr == 'r')
	{
		*bufptr = '&';
		if(getexpr(bufptr, &errptr,(unsigned int *) &regnum) == 0)
		{
			if((regnum >= 0) || (regnum <= REGs))
			{
				*value = m88000.Regs[regnum];
				return(ptr);
			}
		}
	}
	if(strcmp(bufptr, "vbr") == 0)
	{
		*value = VBR;
		return(ptr);
	}
	if(strcmp(bufptr, "ip") == 0)
	{
		*value = IP;
		return(ptr);
	}
	return(ptrsave);
}

