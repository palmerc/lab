/* @(#) File name: signals.c   Ver: 3.2   Cntl date: 3/6/89 12:23:05 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include <signal.h>


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

int	interrupt;
int	quit;
int	buserr;
int	segviol;
/* Don't seem to need this. MJP
int	(*sigaddr)();
*/



	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      sig_handler()
	*/


void sig_handler(int signo)
{
	sig_set();

	switch(signo)
	{
		case SIGINT:
			fprintf(stderr, "\nInterrupt caught\n");
			fflush (stderr);
			interrupt = 1;
			break;
		default:
			fprintf(stderr, "\nUnknown signal caught\n");
			fflush (stderr);
			interrupt = 1;
			break;
	}
	return;
}



	/**********************************************************
	*
	*      sig_set()
	*/


void sig_set(void)
{
	if(signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, sig_handler);
	return;
}
