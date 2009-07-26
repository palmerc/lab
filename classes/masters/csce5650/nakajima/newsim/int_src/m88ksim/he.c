/* @(#) File name: he.c   Ver: 3.2   Cntl date: 3/10/89 10:38:53 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include <string.h>
#include <stdio.h>


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

extern	struct dirtree dir[];


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
/*
*	Modifications:
*	The help command now outputs using the system paging device.
*	(ie more on BSD machines and pg on System V machines)
*/


int he(void)
{

	extern int	interrupt;
	register struct dirtree *list = dir;
	struct cmdstruct *cmd;			/* pointer to cmd */
	struct options *opts;			/* pointer to options */
	char buf[40];

#ifdef NOT_SPEC95
        FILE    *popen();
        FILE    *st;
#ifndef MORE_EXISTS
        char    *pager = "more";
#else
        char    *pager = "pg";
#endif
        if((st = popen(pager, "w")) == NULL)
        {
                perror("he: popen");
                exit(0);
        }
#endif

	do
	{
		if(list->flags == VALID)	/* if vaild directory */
		{
			cmd = list->dir;
			while((cmd)->cmd)
			{
				if(cmd->opts)
				{
					strcpy(buf, "[;");
					for(opts = cmd->opts; opts->name; opts++)
					{
						if(opts != cmd->opts)
							strcat(buf, "|");
						strcat(buf, opts->name);
					}
					strcat(buf, "]");
				}
				else
					buf[0] = '\0';
#ifdef NOT_SPEC95
				fprintf(st, "%10s %-15s %s\n", cmd->cmd, buf, cmd->help);
#else
				PPrintf("%10s %-15s %s\n", cmd->cmd, buf, cmd->help);
#endif
				cmd++;
			}
		}

	        if(interrupt)
			break;

	}
	while((++list)->flags); 		/* assign next tree node */

#ifdef NOT_SPEC95
        pclose(st);
#endif

	return 0;
}

