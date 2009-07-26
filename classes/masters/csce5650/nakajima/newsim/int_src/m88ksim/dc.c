/* @(#) File name: dc.c   Ver: 3.1   Cntl date: 1/20/89 14:20:20 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
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



int dc(void)
{
	if(trans.expr & 0x80000000)
	{
		PPrintf("SIGNED   : %08X = -$%X = -&%d\n", trans.expr, 0-trans.expr, 0-trans.expr);
		PPrintf("UNSIGENED: ");
	}
	PPrintf("%08X = %X = &%u\n", trans.expr, trans.expr, trans.expr);

	return 0;
}

