/* @(#) File name: map.c   Ver: 3.1   Cntl date: 1/20/89 14:20:28 */
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

extern	int cmdflags;
extern	int cmdcount;
extern	int sID;

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
	*      map()
	*/


int map(void)
{
	int segtype = M_SEGANY;
	int paddr;

	if(cmdcount != 1)
	{
		if(cmdflags & TYPE)
			segtype = trans.type & 0x3E0;

		if(segtype == 0x200)
			releaseseg(trans.adr1);
		else
		{
			if(trans.adr1 >= trans.adr2)
			{
				PPrintf("Starting address is greater than Ending address\n");
				return(-1);
			}
			paddr = trans.adr1;
			if(sID && (segtype & M_DATA))
				paddr += 0x40000;
			if(getmem(trans.adr1, trans.adr2 - trans.adr1, segtype, paddr) == -1)
			{
				PPrintf("Unable to allocate requested memory\n");
				return(-1);
			}
		}
	}
	dm();
	return(0);
}
