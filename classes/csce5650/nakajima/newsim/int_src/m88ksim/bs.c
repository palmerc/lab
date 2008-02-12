/* @(#) File name: bs.c   Ver: 3.1   Cntl date: 1/20/89 14:20:14 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*
*/

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

#define BUFSIZE	(1024)


	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/


	/**********************************************************
	*
	*      Messages
	*/

char	bserror[] = { "bs error" };

	/**********************************************************
	*
	*      bs()
	*/


int bs(void)
{
	int totcnt, Index, rdrequest, found;
	union {
		unsigned char c[BUFSIZE];
		unsigned short s[BUFSIZE/2];
		unsigned int l[BUFSIZE/4];
	} buffer;

	if((totcnt = trans.adr2 - trans.adr1) < 0)
		return(-1);

	if(transinit(bserror, LONG) == -1)
		return(-1);
	
	while(totcnt)
	{
		rdrequest = (totcnt > BUFSIZE) ? BUFSIZE: totcnt;
		
		if(rdwr((M_SEGANY | M_RD), trans.adr1, (char *)&buffer, rdrequest) == -1)
			return(-1);

		totcnt -= rdrequest;

		for(Index = found = 0; Index < (rdrequest / trans.size); Index++)
		{
			switch(trans.size)
			{
				case	BYTE:
					if(buffer.c[Index] == trans.expr)
					{
						found++;
						PPrintf("%08x | %02x\t", trans.adr1, trans.expr);
					}
					break;

				case	WRD:
					if(buffer.s[Index] == trans.expr)
					{
						found++;
						PPrintf("%08x | %04x\t", trans.adr1, trans.expr);
					}
					break;

				case	LONG:
					if(buffer.l[Index] == trans.expr)
					{
						found++;
						PPrintf("%08x | %08x\t", trans.adr1, trans.expr);
					}
					break;
			}
			if(found == 4)
			{
				found = 0;
				PPrintf("\n");
			}
			trans.adr1 += trans.size;
		}
	}
	return(0);
}
