/* @(#) File name: asm.c   Ver: 3.1   Cntl date: 1/20/89 14:20:03 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include <ctype.h>
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

extern	struct instruction mnemonics[];
extern	struct instruction memsuffixes[];
extern	struct instruction xfrsymbols[];
extern	char	cmdbuf[];
extern	char	*cmdptrs[];
extern	int	cmdcount;
extern	int	symphyval;



	/**********************************************************
	*
	*       Local Defines
	*
	*/

#define	MAXCMD		(7)
#define	MAXOPERS	(4)

struct oper {
	unsigned int	imm:MAXOPERS,
		symbol:MAXOPERS,
		reg:MAXOPERS,
		creg:MAXOPERS;
	int	value[MAXOPERS];
};

	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/

int	suffix;
int	Index;
int	opercnt;
int	curoper;
int	found;
unsigned int Asmaddr;
static struct oper operands = { 0 };

	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*
	*       This function will manage the instruction struct tree during
	*       the mnemonic searching.
	*/

int assembler( char *buffer, union opcode *ptr, unsigned int addr)
{
	register struct instruction *cmd;		/* pointer to cmd */

	Asmaddr = addr;

	ptr->opc = suffix = Index = 0;

	a_pname(buffer);		/* break command line in to buffers */

	if((cmd = a_choice(mnemonics, cmdptrs[0])) == NULL)
		return(reterr(cmdptrs[0]));

	symphyval = 0;
	if(procopers((suffix) ? &cmdptrs[2]: &cmdptrs[1]))
		return(reterr(cmdptrs[0]));

	if(cmd->funct != 0)
		if((cmd->funct)(cmd, ptr))
			return(reterr(cmdptrs[0]));
	
	symphyval = 1;
	return(0);
}

int reterr(char *ptr)
{
	PPrintf("Unknown mnemonic or operands for mnemonic '%s'\n", ptr);
	symphyval = 1;
	return(-1);				/* return error */
}

	/*********************************************************************
	*
	*       "a_pname()"                        Date Created: 08/21/85 - DLN
	*
	*       This function will break up the command line into a
	*       buffer pointer array.
	*/

void a_pname(char *ptr)
{
	int  count;                                             /* count of words on command line */
	int  caretfound = 0;
	int  bracketfound = 0;

	for(cmdcount = count = 0; count < MAXCMD; ++count)     /* loop until MAXCMD or <CR> */
	{
		while(isspace(*ptr) != 0)
			++ptr;

		cmdptrs[count] = ptr;                           /* store pointer to string */

		if(*ptr != '\0')
			cmdcount++;                             /* bump cmdcount */

		while((isprint(*ptr) != 0) && (*ptr != ' '))	/* look for next white space */
		{
			if(!suffix && (*ptr == '.'))
			{
				suffix = 1;
				break;
			}
			if(*ptr == '[')
			{
				Index = 1;
				bracketfound = 1;
				break;
			}
			if(*ptr == '<')
			{
				caretfound = 1;
				break;
			}
			if(*ptr == ',')
				break;

			if((*ptr == ']') & bracketfound)
			{
				bracketfound = 0;
				break;
			}
			if((*ptr == '>') & caretfound)
			{
				caretfound = 0;
				break;
			}
			if((*ptr >= 'A') && (*ptr <= 'Z'))	/* check bounds */
				*ptr |= 0x20;			/* make it lower case */
			++ptr;
		}
		if((*ptr != '\0') && (count != (MAXCMD-1)))
			*(ptr++) = '\0';                        /* null white space */
	}
}


	/*********************************************************************
	*
	*       "a_choice()"                       Date Created: 08/21/85 - DLN
	*
	*       This function is the work horse of the parser.  This
	*       function evaluate each command tree node for a match
	*       to the passed command ling buffer.
	*/

struct instruction *a_choice(struct instruction *cmdptr, char *cmdbuf)
{
/* 			*cmdptr;      total possible test/command selections */
/* 			cmdbuf;       pointer to which command buffer */
   while(cmdptr->name)               /* search test/command table for a match */
   {
      if (strcmp(cmdbuf, cmdptr->name) == 0)
	    return(cmdptr);
      cmdptr++;
   }
   return ((struct instruction *)NULL);                 /* return status  */
}


int procopers(char **opers)
{
	char *ptr;
	unsigned int value;
	char *errptr;
	struct instruction *cmd;		/* pointer to cmd */

	opercnt = 0;
	operands.imm = 0;
	operands.symbol = 0;
	operands.reg = 0;
	operands.creg = 0;
	for(; (opercnt < (MAXOPERS + 1)) && *opers[opercnt]; opercnt++)
	{
		ptr = opers[opercnt];
				/* ck for reserved symbols */
		if((cmd = a_choice(xfrsymbols, ptr)) != NULL)
		{
			if(opercnt)
			{
				PPrintf("Reserved symbol\n");
				return(-1);
			}
			value = (unsigned) cmd->opc.imm;
			operands.symbol |= 1 << opercnt;
		}
		else if(*ptr == 'r')
		{
			*ptr = '&';
			 if((getexpr(ptr, &errptr, &value) != 0) || (value > 31))
				return(-1);
			operands.reg |= 1 << opercnt;
		}
		else if((*ptr == 'c') && (*(ptr + 1) == 'r'))
		{
			*++ptr = '&';
			 if((getexpr(ptr, &errptr, &value) != 0) || (value > 63))
				return(-1);
			operands.creg |= 1 << opercnt;
		}
		else
		{
			if(getexpr(ptr, &errptr, &value) != 0)
				return(-1);
			operands.imm |= 1 << opercnt;
		}
		operands.value[opercnt] = value;
	}
	if(opercnt > MAXOPERS)
		return(-1);
	return(0);
}



int mkwrd(struct instruction *cmd, union opcode *ptr)
{
	ptr->opc = operands.value[0];
	return 0;
}


int a_sfu1(struct instruction *cmd, union opcode *ptr)
{
	ptr->gen.opc1 = cmd->opc.imm;
	ptr->gen.opc2 = cmd->opc.rrr;
	ptr->gen.dest = operands.value[0];

	if(!suffix || Index)
		return(-1);

	switch(ptr->sfu1.opc)
	{
		case	0x07:
			if(cksfu1(ptr, "s##"))
				return(-1);
		case	0x00:
		case	0x05:
		case	0x06:
		case	0x0E:
			if(!validreg(0x07000000) || cksfu1(ptr, "###"))
				return(-1);
			ptr->gen.src1 = operands.value[1];
			ptr->gen.src2 = operands.value[2];
			return(0);

		case	0x04:
			if(cksfu1(ptr, "#s"))
				return(-1);
			break;

		case	0x09:
		case	0x0A:
		case	0x0B:
			if(cksfu1(ptr, "s#"))
				return(-1);
			break;

		default:
			return(-1);
	}
	if(!validreg(0x03000000) || cksfu1(ptr, "##"))
		return(-1);
	ptr->gen.src2 = operands.value[1];
	return(0);
}

int cksfu1(union opcode *ptr, char *sufstr)
{
	int cnt, items;
	register char *sufptr = cmdptrs[1];
	int size[3];

	for(cnt = items = 0; cnt < 3; cnt++, sufstr++)
	{
		switch(*sufstr)
		{
			case	'#':
				if((*sufptr == '\0') || ((*sufptr != 'd') && (*sufptr != 's')))
					return(-1);

				size[cnt] = (*sufptr++ == 'd') ? 1: 0;
				items++;
				break;

			case	's':
			case	'd':
				if((*sufptr == '\0') || (*sufptr != *sufstr))
					return(-1);

				size[cnt] = (*sufptr++ == 'd') ? 1: 0;
				items++;
				break;

			case	'\0':
				if(*sufptr != '\0')
					return(-1);

				size[cnt] = 0;
				break;

			default:
				return(-1);
		}
	}
	ptr->sfu1.td = size[0];
	ptr->sfu1.t2 = size[items - 1];
	if(items == 3)
		ptr->sfu1.t1 = size[1];
	
	return(0);
}

int a_ctl(struct instruction *cmd, union opcode *ptr)
{
	if(suffix || Index)
		return(-1);

	ptr->gen.opc1 = 0x20;
	ptr->rrr.opc = cmd->opc.rrr;
	switch(ptr->ctl.opc)
	{
		case	1:	/* ?ldcr r,cr */
			if(!validreg(0x01000200))
				return(-1);
			ptr->ctl.dest = operands.value[0];
			ptr->ctl.crsd = operands.value[1];
			break;

		case	2:	/* ?stcr r,cr */
			if(!validreg(0x01000200))
				return(-1);
			ptr->ctl.src1 = operands.value[0];
			ptr->ctl.src2 = operands.value[0];
			ptr->ctl.crsd = operands.value[1];
			break;

		case	3:	/* ?xcr r,r,cr */
			if(!validreg(0x03000400))
				return(-1);
			ptr->ctl.dest = operands.value[0];
			ptr->ctl.src1 = operands.value[1];
			ptr->ctl.src2 = operands.value[1];
			ptr->ctl.crsd = operands.value[2];
			break;
	}
	return(0);
}


int integer(struct instruction *cmd, union opcode *ptr)
{
	if(Index)
		return(-1);

	ptr->gen.dest = operands.value[0];
	ptr->gen.src1 = operands.value[1];

	if(!validreg(0x07000000))
	{
		if(suffix || !validreg(0x03040000))
			return(-1);
		ptr->gen.opc1 = cmd->opc.imm;
		ptr->imm16.imm16 = operands.value[2];
	}
	else
	{
		ptr->gen.opc1 = 0x3d;
		ptr->rrr.opc = cmd->opc.rrr;
		if(suffix)
		{
			if(!strcmp(cmdptrs[1], "co"))
				ptr->opc |= 0x00000100;
			else if(!strcmp(cmdptrs[1], "ci"))
				ptr->opc |= 0x00000200;
			else if(!strcmp(cmdptrs[1], "cio"))
				ptr->opc |= 0x00000300;
			else
				return(-1);
		}
		ptr->rrr.src2 = operands.value[2];
	}
	return(0);
}

int trpbci(struct instruction *cmd, union opcode *ptr)
{
	if((!(validreg(0x02050000) || validreg(0x02040001))
	    && suffix) || Index)
		return(-1);
	ptr->gen.opc1 = 0x3c;
	ptr->trp.opc = cmd->opc.imm10;
	ptr->trp.dest = operands.value[0];
	ptr->trp.src1 = operands.value[1];
	ptr->trp.vec9 = operands.value[2];
	return(0);
}

int tbnd(struct instruction *cmd, union opcode *ptr)
{
	if(suffix || Index)
		return(-1);

	if(!validreg(0x03000000))
	{
		if(!validreg(0x01020000))
			return(-1);
		ptr->gen.opc1 = cmd->opc.imm;
		ptr->imm16.src1 = operands.value[0];
		ptr->imm16.imm16 = operands.value[1];
	}
	else
	{
		ptr->gen.opc1 = 0x3d;
		ptr->rrr.opc = cmd->opc.rrr;
		ptr->rrr.src1 = operands.value[0];
		ptr->rrr.src2 = operands.value[1];
	}
	return(0);
}

int mem(struct instruction *cmd, union opcode *ptr)
{
	ptr->gen.dest = operands.value[0];
	ptr->gen.src1 = operands.value[1];

	if(!validreg(0x07000000))
	{
		if(!validreg(0x03040000) || Index)
			return(-1);
		ptr->gen.opc1 = cmd->opc.imm;
		if(ckmem(ptr, 0))
			return(-1);
		ptr->imm16.imm16 = operands.value[2];
	}
	else
	{
		ptr->gen.opc1 = 0x3d;
		ptr->rrr.opc = cmd->opc.rrr;
		if(ckmem(ptr, 1))
			return(-1);
		if(Index)
			ptr->mem.idx = 1;
		ptr->rrr.src2 = operands.value[2];
	}
	return(0);
}

/*	Bit definitions for the imm10 field in memsuffixes */
/**/
/*		0 xmem okay	*/
/*		1 ld okay	*/
/*		2 st okay	*/
/*		3 lda okay	*/
/*		4 ld.xu okay	*/
/*		5 imm mode okay	*/
/**/


int ckmem(union opcode *ptr, int flag)
{
	struct instruction *type;
	int p, ty = 0x020;

	if(flag)
	{
		p = ptr->mem.p;
		ty = 0x020;
	}
	else
	{
		p = ptr->memi.p;
		ty = 0x01;
	}

	if((type = a_choice(memsuffixes, (suffix) ? cmdptrs[1]: "")) == NULL)
		return(-1);

	if(!flag && !(type->opc.imm10 & 0x20))
		return(-1);	/* user access not allowed in imm mode */

	switch(p)
	{
		case	0:
			if(type->opc.imm10 & 0x01)
			{
				ty = (flag) ? type->opc.rrr: type->opc.imm;
				if(((ty & 0x060) == 0x60) || ((ty & 0x3) == 0x3))
					ty &= 0x8;
				break;
			}
			return(-1);

		case	1:
			if(type->opc.imm10 & 0x02)
			{
				ty = (flag) ? type->opc.rrr: type->opc.imm;
				break;
			}
			else if(type->opc.imm10 & 0x10)
			{
				if(flag)
				{
					ptr->mem.p = 0;
					ty = type->opc.rrr;
				}
				else
				{
					ptr->memi.p = 0;
					ty = type->opc.imm;
				}
				break;
			}
			return(-1);

		case	2:
			if(type->opc.imm10 & 0x04)
			{
				ty = (flag) ? type->opc.rrr: type->opc.imm;
				break;
			}
			return(-1);

		case	3:
			if(type->opc.imm10 & 0x08)
			{
				if(flag && Index)
				{
					ty = type->opc.rrr;
					break;
				}
				if((type = a_choice(mnemonics, "addu")) != NULL)
				{
					suffix = 0;
					return(integer(type, ptr));
				}
			}
			return(-1);
	}
	if(flag)
	{
		ptr->rrr.opc |= ty;
	}
	else
	{
		ptr->gen.opc1 |= ty;
	}
	return(0);
}

int xfrbci(struct instruction *cmd, union opcode *ptr)
{
	if(!(validreg(0x02050000) || validreg(0x02040001)) || Index)
		return(-1);
	ptr->gen.opc1 = cmd->opc.imm;
	if((suffix) && !strcmp(cmdptrs[1], "n"))
		ptr->opc |= 0x04000000;
	ptr->imm16.dest = operands.value[0];
	ptr->imm16.src1 = operands.value[1];
	ptr->imm16.imm16 = (operands.value[2] - Asmaddr) >> 2;
	return(0);
}

int logical(struct instruction *cmd, union opcode *ptr)
{
	ptr->gen.dest = operands.value[0];
	ptr->gen.src1 = operands.value[1];

	if(!validreg(0x07000000) || Index)
	{
		if(!validreg(0x03040000) || Index)
			return(-1);
		ptr->gen.opc1 = cmd->opc.imm;
		if((suffix) && !strcmp(cmdptrs[1], "u"))
			ptr->opc |= 0x04000000;
		ptr->imm16.imm16 = operands.value[2];
	}
	else
	{
		ptr->gen.opc1 = 0x3d;
		if((ptr->rrr.opc = cmd->opc.rrr) == 0)
			return(-1);
		if((suffix) && !strcmp(cmdptrs[1], "c"))
			ptr->opc |= 0x00000400;
		ptr->rrr.src2 = operands.value[2];
	}
	return(0);
}

int xfrri(struct instruction *cmd, union opcode *ptr)
{
	if(!validreg(0x00010000) || Index)
		return(-1);
	ptr->gen.opc1 = cmd->opc.imm;
	if((suffix) && !strcmp(cmdptrs[1], "n"))
		ptr->opc |= 0x04000000;
	ptr->imm26.imm26 = (operands.value[0] - Asmaddr) >> 2;
	return(0);
}

int ffirst(struct instruction *cmd, union opcode *ptr)
{
	if(suffix || Index)
		return(-1);
	if(!validreg(0x03000000))
		return(-1);
	ptr->gen.opc1 = 0x3d;
	ptr->gen.dest = operands.value[0];
	ptr->gen.opc2 = cmd->opc.rrr;
	ptr->gen.src2 = operands.value[1];
	return(0);
}

int rte(struct instruction *cmd, union opcode *ptr)
{
	if(!validreg(0))
		return(-1);
	ptr->opc = 0xf400fc00;
	return(0);
}

int xfra(struct instruction *cmd, union opcode *ptr)
{
	if(!validreg(0x01000000) || Index)
		return(-1);
	ptr->gen.opc1 = 0x3d;
	ptr->gen.opc2 = cmd->opc.rrr;
	if((suffix) && !strcmp(cmdptrs[1], "n"))
		ptr->opc |= 0x00000400;
	ptr->gen.src2 = operands.value[0];
	return(0);
}

int bits(struct instruction *cmd, union opcode *ptr)
{
	if(suffix || Index)
		return(-1);

	ptr->gen.dest = operands.value[0];
	ptr->gen.src1 = operands.value[1];

	if(!validreg(0x07000000))
	{
		if(!validreg(0x030c0000))
			return(-1);
		ptr->gen.opc1 = 0x3c;
		ptr->imm10.opc = cmd->opc.imm10;
		ptr->imm10.imm10 = (operands.value[2] << 5) | (operands.value[3] & 0x1f);
	}
	else
	{
		ptr->gen.opc1 = 0x3d;
		ptr->rrr.opc = cmd->opc.rrr;
		ptr->rrr.src2 = operands.value[2];
	}
	return(0);
}

int validreg(int code1)
{
struct code_struct{
	char reg,
	     imm,
	     creg,
	     symbol;
      } code;

	union { int i;
		struct code_struct j;
	} k;

	k.i = code1;
	code = k.j;

	return((operands.reg == code.reg) && (operands.imm == code.imm)
		&& (operands.creg == code.creg) && (operands.symbol == code.symbol));
}
