/* @(#) File name: md.c   Ver: 3.1   Cntl date: 1/20/89 14:20:30 */
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
extern	int	cmdcount;


	/**********************************************************
	*
	*       Local Defines
	*
	*/

#define	BUFSIZE	(16)


	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/


	/**********************************************************
	*
	*      Messages
	*/

char	mderror[] = { "md error" };

	/**********************************************************
	*
	*      mds()
	*/

int mds(void)
{
	if(!(cmdflags & COUNT))
		if((cmdcount <= 2) || ((cmdcount == 3) && (cmdflags & TYPE)))
			trans.adr2 = trans.adr1 + (trans.size * 128);

	return(md());
}


	/**********************************************************
	*
	*      md()
	*/


int md(void)
{
	int totcnt, Index, rdrequest;
	union {
		unsigned char c[BUFSIZE];
		unsigned short s[BUFSIZE/2];
		unsigned int l[BUFSIZE/4];
		float f[BUFSIZE/4];
		double d[BUFSIZE/8];
	} buffer;
	unsigned int revint;
	unsigned short revshort;

	if(cmdcount == 0)
		trans.adr2 = trans.adr1 + trans.cnt;

	if((totcnt = trans.adr2 - trans.adr1) < 0)
		return(-1);

	trans.cnt = totcnt;

	if(transinit(mderror, LONG*2) == -1)
		return(-1);
	
	while(totcnt)
	{
		if(trans.type & (DI | SF | DF))
			rdrequest = trans.size;
		else
			rdrequest = (totcnt > BUFSIZE) ? BUFSIZE: totcnt;
		
		if(rdwr((M_SEGANY | M_RD), trans.adr1, (char *)&buffer, rdrequest) == -1)
			return(-1);

		totcnt -= rdrequest;

		if(trans.type & DI)
			PPrintf("\n%12s ", symbol(M_INSTR, trans.adr1));
		else
			PPrintf("\n%08X ", trans.adr1);

		for(Index = 0; Index < (rdrequest / trans.size); Index++)
		{
			switch(trans.size)
			{
				case	BYTE:
					PPrintf("%02X ", buffer.c[Index]);
					break;

				case	WRD:
					shortswap(&revshort,&buffer.s[Index]);
					PPrintf("%04X ", revshort);
					break;

				case	LONG:
					if(trans.type & SF)
					{
						PPrintf("%.7e", buffer.f[Index]);
						break;
					}
					if(trans.type & DI)
					{
						PPrintf("%08X ", buffer.l[Index]);
						PPrintf(dis(buffer.l[Index], trans.adr1));
					} else {
/* MJP */					intswap(&revint,&buffer.l[Index]);
						PPrintf("%08X ", revint);
					}
					break;

				case	LONG*2:
					PPrintf("%.16e", buffer.d[Index]);
					break;
			}
		}
		if(!(trans.type & (DI | SF | DF)))
		{
			PPrintf("\t");
			if(rdrequest < 16)
				PPrintf("\t");

			for(Index = 0; Index < rdrequest; Index++)
			{
				PPrintf("%c", ((buffer.c[Index] >= 0x20) && (buffer.c[Index] <= 0x7e)) ? buffer.c[Index]: '.');
			}
		}
		trans.adr1 += rdrequest;
	}
	return(0);
}
