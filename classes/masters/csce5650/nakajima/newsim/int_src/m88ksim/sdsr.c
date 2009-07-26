/* @(#) File name: sdsr.c   Ver: 3.1   Cntl date: 1/20/89 14:20:01 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

/*************************************************************************/
/* statistics reset ()                                                   */
/*                                                                       */
/* Function calls:  statreset()                                          */
/*            clears all the instruction counts, clock and statistics    */
/*                                                                       */
/*************************************************************************/

int sr(void) /* statistics reset */
{
	statreset();
	return 0;
}
/*************************************************************************/
/* statistics display ()                                                 */
/*                                                                       */
/* Function calls:  readtime()                                           */
/*                   returns an unsigned int with the number of clocks  */
/*                   executed so far.                                    */
/*                  printstats()                                         */
/*                   prints out the instruction counts and statistics    */
/*                                                                       */
/*************************************************************************/

int sd(void) /* statistics  display */
{
	printstats();
	return 0;
}
