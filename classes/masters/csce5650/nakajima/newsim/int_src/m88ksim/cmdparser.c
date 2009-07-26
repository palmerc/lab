/* @(#) File name: cmdparser.c   Ver: 3.1   Cntl date: 1/20/89 14:20:16 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*
*       Functions contain in this module are:
*
*               parse()         Converts the command line into a structure
*                               pointer used to execute the function.
*
*               pname()         converts the command line into an array
*                               of pointers.
*
*               choice()        manages the structures used in parsing the
*                               the command line.
*
*
*                                          Date Last Modified: 
*/

#include "functions.h"
#include <ctype.h>
#include <string.h>
#include "defines.h"
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

extern	char	cmdbuf[BSIZE];


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

char	*cmdptrs[MAXCMD] = {'\0'};
struct cmdstruct oldcmd = {NULL};
int	cmdcount = 0;
int	cmdflags = NONE;

	/**********************************************************
	*
	*      Messages
	*/

char	bang[] = { "!" };


	/*********************************************************************
	*
	*       "parse()"                        Date Created: 08/21/85 - DLN
	*
	*       This function will manage the command struct tree during
	*       the command search.
	*/

struct cmdstruct *parse(char *ptr, struct dirtree *list)
{
	struct cmdstruct *cmd;			/* pointer to cmd */
	struct options   *work;			/* scratch pointer */
	char		 *errptr;

	cmdflags = NONE;
	pname(ptr);                             /* break command line in to buffers */

	if(cmdcount == 0)
	{
		if(oldcmd.cmd != NULL)
			return(&oldcmd);
		else
			return(0);
	}

	do
	{
		if(list->flags == VALID)	/* if vaild directory */
		{
			if((cmd = choice(list->dir, strtolower(cmdptrs[0]))) != NULL)
			{
				trans.adr1 = trans.adr2 = trans.expr = 0;
				trans.size = trans.type = DEFWRDSZE;
				if(!cmd->flags)
				{
					if(cmdcount > 1)
						break;
				}
				if(cmdflags & TYPE)
				{
					if(cmd->flags & TYPE)
					{
						for(work = cmd->opts; work && work->name; work++)
						{
							if(strcmp(work->name, strtolower(cmdptrs[cmdcount-1])) == 0)
							{
								trans.size = work->flags & 0xff;
								trans.type = work->flags;
								break;
							}
						}
					}
					else
						break;
				}
				if(cmd->flags & COMMAND)
				{
					if(cmdcount > 1)
					{
						cmdflags |= COMMAND;
						return(cmd);
					}
				}
				if(cmd->flags & RANGE)
				{
					if(getrange() != 0)
						break;

					if(cmd->flags & (DATA | TEXT))
					{
						if(cmdflags & COUNT)
						{
							if(getdata(cmdptrs[2], &errptr, &trans.expr) != 0)
								break;
						}
						else
						{
							if(getdata(cmdptrs[3], &errptr, &trans.expr) != 0)
								break;
						}
					}
				}
				if(cmd->flags & (EXPR | ADDR))
				{
					if(getexpr(cmdptrs[1], &errptr, &trans.expr) != 0)
						break;

					if(cmd->flags & ADDR)
					{
						trans.adr1 = trans.expr;
						if(cmd->flags & DATA) 
							if(getdata(cmdptrs[2], &errptr, &trans.expr) != 0)
								break;
					}
				}
				if(cmd->flags & RERUN)
					oldcmd = *cmd;
				else
					oldcmd.cmd = NULL;

				return(cmd);
			}
		}
	}
	while((++list)->flags); 		/* assign next tree node */
						/* if we got here we have a command that is unknown to us */
	PPrintf("Unknown command or option for command '%s'\n",  cmdptrs[0]);
	return(NULL);                           /* return error */
}

	/*********************************************************************
	*
	*       "pname()"                        Date Created: 08/21/85 - DLN
	*
	*       This function will break up the command line into a
	*       buffer pointer array.
	*/

void pname(char *ptr)
{
	int  count;                                             /* count of words on command line */

	for(cmdcount = count = 0; count != MAXCMD; ++count)     /* loop until MAXCMD or <CR> */
	{
		while(isspace(*ptr) != 0)
			++ptr;

		if(*(cmdptrs[count] = ptr) == '\0')		/* store pointer to string */
			continue;

		cmdcount++;					/* bump cmdcount */

		if((count == 0) && (*ptr == '!'))
		{
			cmdptrs[0] = bang;
			ptr++;
			continue;
		}

		if((*ptr == '\'') || (*ptr == '"'))		/* check for text */
		{
			while((*++ptr != '\'') && (*ptr != '"') && (*ptr != '\0'));
			if(*ptr != '\0')
				ptr++;
		}
		else
		{
			while((isprint(*ptr) != 0) && (*ptr != ',')
				&& (*ptr != ';') && (*ptr != ' '))	/* look for next white space */
				++ptr;

			if(*ptr == ';')
				cmdflags = TYPE;
		}
		if((*ptr != '\0') && (count != (MAXCMD-1)))
			*(ptr++) = '\0';                        /* null white space */
	}
}


char *strtolower(char *p)
{
	char *oldp = p;

	while(*p)
	{
		if((*p >= 'A') && (*p <= 'Z'))		/* check bounds */
			*p |= 0x20;			/* make it lower case */
		p++;
	}
	return(oldp);
}


	/*********************************************************************
	*
	*       "choice()"                       Date Created: 08/21/85 - DLN
	*
	*       This function is the work horse of the parser.  This
	*       function evaluate each command tree node for a match
	*       to the passed command ling buffer.
	*/

struct cmdstruct *choice(struct cmdstruct *cmdptr,      char *cmdbuf)
{
   while(cmdptr->cmd)                   /* search test/command table for a match */
   {
      if(strcmp(cmdbuf, cmdptr->cmd) == 0)
	    return(cmdptr);
      cmdptr++;
   }

   return (NULL);                                       /* return status  */
}

