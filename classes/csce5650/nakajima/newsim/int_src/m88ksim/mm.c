/* @(#) File name: mm.c   Ver: 3.1   Cntl date: 1/20/89 14:20:32 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

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

extern	int	cmdflags;


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

char	mmerror[] = { "mm error" };

	/**********************************************************
	*
	*      mm()
	*/


int mm(void)
{
	char mmbuf[90], *errptr;
	int  direction = trans.size;
	unsigned int new;
	double newd = 0.0;
	union {
		unsigned char c;
		unsigned short s;
		unsigned int l;
		float f;
		double d;
	} buffer;

	if(transinit(mmerror, LONG*2) == -1)
		return(-1);

	while(1)
	{
		if(rdwr((M_SEGANY | M_RD), trans.adr1, (char *)&buffer, trans.size) == -1)
			return(-1);

		if(trans.type & DI)
			PPrintf("%12s ", symbol(M_INSTR, trans.adr1));
		else
			PPrintf("%08X ", trans.adr1);
		switch(trans.size)
		{
			case	BYTE:
				PPrintf("%02X ", buffer.c);
				break;

			case	WRD:
				PPrintf("%04X ", buffer.s);
				break;

			case	LONG:
				if(trans.type & SF)
				{
					PPrintf("%.7e ", buffer.f);
					break;
				}
				PPrintf("%08X ", buffer.l);
				if(trans.type & DI)
					PPrintf(dis(buffer.l, trans.adr1));
				break;

			case	LONG*2:
				PPrintf("%.16e ", buffer.d);
				break;
		}

		FFgets(mmbuf, sizeof(mmbuf), stdin);

		switch(mmbuf[0])
		{
			case	'v':
			case	'V':
				direction = trans.size;
				break;

			case	'^':
				direction = 0 - trans.size;
				break;

			case	'=':
				direction = 0;
				break;

			case	'.':
				return(0);

			case	'\n':
				break;

			default:
				switch(trans.type & 0xffffff00)
				{
					case	DI:
						if(assembler(mmbuf, (union opcode *)&new, trans.adr1))
							continue;
						PPrintf("%08X %08X %s\n", trans.adr1, new, dis(new, trans.adr1));
						break;

					case	DF:
						newd = atof(mmbuf);
						break;

					case	SF:
						new = atosf(mmbuf);
						break;

					default:
						if(getexpr(mmbuf, &errptr, &new) != 0)
						{
							PPrintf("\nUnkown input \"%c\"\n", *errptr);
							continue;
						}
						break;
				}
				switch(trans.size)
				{
					case	BYTE:
						buffer.c = (unsigned char)new;
						break;

					case	WRD:
						buffer.s = (unsigned short)new;
						break;

					case	LONG:
						buffer.l = (unsigned int)new;
						break;

					case	LONG*2:
						buffer.d = newd;
						break;
				}
				if(rdwr((M_SEGANY | M_WR), trans.adr1, (char *)&buffer, trans.size) == -1)
					return(-1);
				break;
		}
		trans.adr1 += direction;
	}
}
