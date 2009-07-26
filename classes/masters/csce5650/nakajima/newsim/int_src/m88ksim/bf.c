/* @(#) File name: bf.c   Ver: 3.1   Cntl date: 1/20/89 14:20:08 */
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

char	bferror[] = { "bf error" };

	/**********************************************************
	*
	*      bf()
	*/


int bf(void)
{
	int totcnt, Index, rdrequest;
	union {
		unsigned char c[BUFSIZE];
		unsigned short s[BUFSIZE/2];
		unsigned int l[BUFSIZE/4];
	} buffer;

	if((totcnt = trans.adr2 - trans.adr1) < 0)
		return(-1);

	if(transinit(bferror, LONG) == -1)
		return(-1);
	
	while(totcnt)
	{
		rdrequest = (totcnt > BUFSIZE) ? BUFSIZE: totcnt;
		
		totcnt -= rdrequest;

		for(Index = 0; Index < (rdrequest / trans.size); Index++)
		{
			switch(trans.size)
			{
				case	BYTE:
					buffer.c[Index] = trans.expr;
					break;

				case	WRD:
					buffer.s[Index] = trans.expr;
					break;

				case	LONG:
					buffer.l[Index] = trans.expr;
					break;
			}
		}
		if(rdwr((M_SEGANY | M_WR), trans.adr1, (char *)&buffer, rdrequest) == -1)
			return(-1);

		trans.adr1 += rdrequest;
	}
	return(0);
}
