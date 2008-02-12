/* @(#) File name: show.c   Ver: 3.2   Cntl date: 3/6/89 12:21:23 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include <string.h>


/*
*******************************************************************************
*
*       Module:     show()
*
*       File:       show.c
*
*******************************************************************************
*
*  Functional Description:
*	This is a user executable instruction that allows the user to specify
*	what data is to be displayed when a "tv" trace is executed.  Entering
*	the command with-out arguments will display what data displays are
*	enabled
*
*  Globals:
*	show_clock	True if display clock enabled
*	show_regs	True if dusplay CPU registers enabled
*	show_bus	True if display CPU bus (distination, source1, source2)
*	show_instr	True if display disassembled instruction enabled
*	sym_addr	True if display address symbolicly enabled
*
*  Parameters:
*
*  Output:
*
*  Revision History:
*
*******************************************************************************/

int		show_clock = 0;
int		show_regs = 1;
int		show_bus = 1;
int		show_instr = 1;
extern int	sym_addr;


int show(void)
{
	extern	int	cmdcount;
	extern	char	*cmdptrs[];

	char	add;
	char	*ptr;
	int	i;

	if(cmdcount == 1)
	{
		PPrintf("Show: ");

		if(show_bus)
			PPrintf("Bus ");

		if(show_clock)
			PPrintf("Clock ");

		if(show_instr)
			PPrintf("Instructions ");

		if(show_regs)
			PPrintf("Registers ");

		if(sym_addr)
			PPrintf("Symbols ");

		PPrintf("\n");
	}
	else
	{
		for(i=1; i< cmdcount; i++)
		{
			str_toupper(cmdptrs[i]);

			switch(*cmdptrs[i])
			{
				case '-':
					add=0;
					ptr = cmdptrs[i]+1;
					break;
				case '+':
					add=1;
					ptr = cmdptrs[i]+1;
					break;
				default:
					add=1;
					ptr = cmdptrs[i];
					break;
			}

			if(strncmp("BUS", ptr, 3) == 0)
			{
				if(add)
					show_bus = 1;
				else
					show_bus = 0;
			}
			else if(strncmp("CLOCK", ptr, 5) == 0)
			{
				if(add)
					show_clock = 1;
				else
					show_clock = 0;
			}
			else if(strncmp("INST", ptr, 4) == 0)
			{
				if(add)
					show_instr = 1;
				else
					show_instr = 0;
			}
			else if(strncmp("REG", ptr, 3) == 0)
			{
				if(add)
					show_regs = 1;
				else
					show_regs = 0;
			}
			else if(strncmp("SYM", ptr, 3) == 0)
			{
				if(add)
					sym_addr = 1;
				else
					sym_addr = 0;
			}
		}
	}
	return 0;
}
