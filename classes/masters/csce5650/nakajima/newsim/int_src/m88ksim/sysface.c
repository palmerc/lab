/* @(#) File name: sysface.c   Ver: 3.1   Cntl date: 1/20/89 14:19:40 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include <errno.h>
#include <stdarg.h>
#include <string.h>


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

extern	char	*cmdptrs[];
extern	int	cmdcount;
extern	int	trace_flg;


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

int	paflag = 0;
FILE	*pa_out;
char	pafile[50] = { 0 };

int	silent = 0;

	/**********************************************************
	*
	*      Messages
	*/


int pa(void)
{
	if(cmdcount < 2)
	{
		if(*pafile == '\0')
			printf("\"pa\" file not defined\n");
		else
			PPrintf("\nCurrent \"pa\" file is \"%s\"\n", pafile);
		return(0);
	}
	if(paflag)
	{
		PPrintf("File \"%s\" already open\n", pafile);
		return(0);
	}
	if ((pa_out = fopen(cmdptrs[1], "a")) == NULL)
	{
		fprintf(stderr,"Can not open file \"%s\"\n", cmdptrs[1]);
		perror("2");
		return(1);
	}
	strncpy(pafile, cmdptrs[1], 50);
	paflag = 1;
	return 0;
}

int nopa(void)
{
	if(paflag)
	{
		fclose(pa_out);
		paflag = 0;
	}
	return 0;
}


int PPrintf(char *format,...)
{
	int	count;
	va_list ap;

	va_start(ap,format);

		/*
		 * trace_flg = 4 indicates a special trace.  Since you will
		 * always want to see a special trace, PPrintf will always
		 * print if trace_flg = 4
		 */
	if((silent) && (trace_flg != 4))
		return(0);

	if(paflag)
		(void) vfprintf(pa_out, format, ap);

	count = vprintf(format, ap);

	va_end(ap);
	return(count);
}

int Eprintf(char *format,...)
{
	int	count;
	va_list ap;

	va_start(ap,format);

	if(paflag)
		(void) vfprintf(pa_out, format, ap);
	count = vprintf(format, ap);
	va_end(ap);
	return(count);
}

char * FFgets(char *buf,int size, FILE *iob)
{
		return(fgets(buf, size, iob));
}


int cwd(void)
{
#ifdef NOT_SPEC95
	if(chdir(cmdptrs[1]) != 0)
		perror("cwd");
	(void) pwd();
#else
	PPrintf("cwd command not required for SPEC95\n");
#endif
	return 0;
}

int pwd(void)
{
#ifdef NOT_SPEC95
	char buf[200];
	extern char *getcwd();

/*
** On a SYS V system then use getcwd() to
** get current working directory.
**
*/

	strcpy(buf, getcwd((char *) NULL, 200));

	PPrintf("%s", buf);
#else
	PPrintf("pwd command not required for SPEC95\n");
#endif

	return 0;
}
