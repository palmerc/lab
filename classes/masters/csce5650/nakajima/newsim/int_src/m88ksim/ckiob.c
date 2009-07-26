/* @(#) File name: ckiob.c   Ver: 3.1   Cntl date: 1/20/89 14:41:24 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"

FILE *ckiob(TARGETFILE *fp)
{
	TARGETFILE *addr;
	char *eptr;

	if(getexpr("__iob", &eptr, (unsigned int *)&addr) == -1)
		return((FILE *)fp);

	if(addr == fp)
		return(stdin);

	if(++addr == fp)
		return(stdout);

	if(++addr == fp)
		return(stderr);

	return((FILE *)fp);
}
