/* @(#) File name: bm.c   Ver: 3.1   Cntl date: 1/20/89 14:20:10 */
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

char	bmerror[] = { "bm error" };

	/**********************************************************
	*
	*      bm()
	*/


int bm(void)
{
	int totcnt, rdrequest;
	union {
		unsigned char c[BUFSIZE];
		unsigned short s[BUFSIZE/2];
		unsigned int l[BUFSIZE/4];
	} buffer;

	if((totcnt = trans.adr2 - trans.adr1) < 0)
		return(-1);

	if(transinit(bmerror, LONG) == -1)
		return(-1);
	
	while(totcnt)
	{
		rdrequest = (totcnt > BUFSIZE) ? BUFSIZE: totcnt;
		
		if(rdwr((M_SEGANY | M_RD), trans.adr1, (char *)&buffer, rdrequest) == -1)
			return(-1);

		if(rdwr((M_SEGANY | M_WR), trans.expr, (char *)&buffer, rdrequest) == -1)
			return(-1);

		totcnt -= rdrequest;
		trans.expr += rdrequest;
		trans.adr1 += rdrequest;
	}
	return(0);
}
