/* @(#) File name: symbols.c   Ver: 3.5   Cntl date: 3/6/89 12:26:42 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "symbols.h"
#include <string.h>
#include <stdio.h>

#ifndef SYMNMLEN
#define SYMNMLEN	8
#endif


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

extern	int	interrupt;
extern	int	quit;
extern	int	cmdcount;
extern	char	*cmdptrs[];

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

static struct symbols	*st_dataptr;
static struct symbols	*st_instrptr;
static struct symbols	*cur_instrptr;
static struct symbols	*nxt_instrptr;

char	symname[256] = { 0 };
struct symbols *symtable = 0;
void	*nametable = 0;
int	symphyval = 1;		/* used by asm to get virtual symbol value */
				/* ALL OTHER get physical symbol value */


/*
*******************************************************************************
*
*       Module:     symbol()
*
*       File:       symbols.c
*
*******************************************************************************
*
*  Functional Description:
*	symbol() searches the "sect" section of the loaded symbol table and
*	looks for the symbolic address associated with the "value" address
*	passed in.  If the "value" is equal to a symbolic address, then the
*	symbol is returned, otherwise a symbol plus a hex offset is returned.
*
*  Globals:
*	symtable	Pointer to symbol table.
*
*  Parameters:
*	sect	M_DATA || M_INSTR.  Memory data segment or memory intruction
*		segment of symbol table.  Defined in sim.h
*	value	Address of instruction.
*
*  Output:
*	Returns a pointer to the symbolic address string.
*
*  Revision History:
*	Modified to provide a faster symbol table lookup.  A pointer was added
*	to M_DATA searches which points to the first symbol in the M_DATA
*	section.  All searches start at this point instead of the beginning
*	of the table.
*	For M_INSTR searches, two pointers were added.  The two pointers
*	try to encapsulate the current symbol, so that most searches do not
*	have to be made since the current address is most likely already
*	bounded by the two pointers.
*
*	The symbol table modifications did not work properly for the M_INSTR
*	section unless the current address range was bounded by two symbols.
*	(ie) if there is only one symbol in the M_INSTR section then bounding
*	is impossible and the technique is more complex then needed.  This
*	is corrected.
*
*	The function was cleaned up and dead code was eliminated.  A new
*	function find_next_symbol() was added which help out with the
*	flow of the function.  The initsymptrs() functionality was changed
*	somewhat.  See appropriate header.
*
*
*	When searching the symbol table of section type M_INSTR of M_DATA, the
*	function will check to see if that section of the symbol table is
*	defined (ie st_???ptr != 0).  If the section is not defined then that
*	portion of the symbol table is empty and a string containing the HEX
*	address is returned.
*
*
*	Searching the M_DATA section.  The while loop did not stop when
*	find_next_symbol() returned a nil pointer.  The pointer lastsectptr()
*	should be initialized to st_dataptr in case the while loop is not
*	executed.
*
*******************************************************************************/

char *symbol(int sect,unsigned int value)
{
	struct symbols	*ptr = symtable;
	struct symbols	*lastsectptr = 0;

			/* Find first entry in symbol table for desired
			 * section
			 */

	if ((sect == M_INSTR) && (st_instrptr != 0))
	{
		if (value < cur_instrptr->value)
		{
			initsymptrs(M_INSTR);

			while ((nxt_instrptr) && (value >= nxt_instrptr->value))
			{
				cur_instrptr = nxt_instrptr;
				nxt_instrptr = find_next_symbol(M_INSTR, cur_instrptr);
			}
		}
		else if ((nxt_instrptr) && (value >= nxt_instrptr->value))
		{
			while ((nxt_instrptr) && (value >= nxt_instrptr->value))
			{
				cur_instrptr = nxt_instrptr;
				nxt_instrptr = find_next_symbol(M_INSTR, cur_instrptr);
			}
		}

		if (value == cur_instrptr->value)
		{
			return(cur_instrptr->sym);
		}
		else
		{
			sprintf(symname, "%s+$%X", cur_instrptr->sym,
			        value - cur_instrptr->value);
			return(symname);
		}
	}
	else if ((sect == M_DATA) && (st_dataptr != 0))
	{
		ptr = lastsectptr = st_dataptr;

		while((ptr) && (value > ptr->value))
		{
			lastsectptr = ptr;
			ptr = find_next_symbol(M_DATA, lastsectptr);
		}

		if (value == lastsectptr->value)
		{
			return(lastsectptr->sym);
		}
		else
		{
			sprintf(symname, "%s+$%X", lastsectptr->sym,
			        value - lastsectptr->value);
			return(symname);
		}
	}

	sprintf(symname, "$%08X", value);
	return(symname);
}


	/**********************************************************
	*
	*      findsym()
	*/

char *findsym(char *ptr,int *value)
{
	char symtolkup[100];
	char *ptrsave, *bufptr;
	register struct symbols *symptr = symtable;

	if(!symptr)
		return(ptr);

	ptrsave = ptr;
	bufptr = symtolkup;

	while((*ptr != '\n') && (*ptr != '+')  && (*ptr != '-')  &&
		(*ptr != '*')  && (*ptr != '/')  && *ptr)
	{
		*bufptr++ = *ptr++;
	}
	*bufptr = '\0';
	while(symptr->sym)
	{
		if(strcmp(symptr->sym, symtolkup) == 0)
		{
			*value = ((symphyval) ? symptr->phyval: symptr->value);
			return(ptr);
		}
		symptr++;
	}
	return(ptrsave);
}



/*
*******************************************************************************
*
*       Module:     symcreate()
*
*       File:       symbols.c
*
*******************************************************************************
*
*  Functional Description:
*	Create the global symbol table from the coff file sections passed
*	from lo().
*
*  Globals:
*
*  Parameters:
*
*  Output:
*
*  Revision History:
*	The passed variable was changed from a character pointer to a
*	pointer to a structure of type string_table defined in ld_coff.h
*	This changed coencides with the change offunction lo().
*
*
*******************************************************************************/

int symcreate(struct syment *symptr,int nsyms,struct string_table *flexname,
struct scnhdr *shdr)
{
	int cnt = (nsyms * (SYMNMLEN + 1));
	register struct symbols *ptr, *ptr1;
	struct symbols hold;
	char *nptr;

	symfree();

	if((symtable = (struct symbols *)malloc(nsyms * sizeof(struct symbols))) == 0)
	{
		perror("symcreate:");
		return(-1);
	}

					/* number of entries in string table */
	cnt += (flexname->number_of_entries);

	if((nametable = calloc(cnt, 1))== 0)
	{
		perror("symcreate:");
		symfree();
		return(-1);
	}

	ptr = symtable;
	nptr = nametable;
	cnt = 0;

	while(nsyms > cnt)
	{
		if(symptr->n_numaux)	/* if aux entries trash them */
		{
			cnt += symptr->n_numaux;
			symptr += symptr->n_numaux;
		}
		else
		{
					/* do not save local symbols "@L565" */
					/* do not save symbols with negative */
					/*   or 0 section numbers            */
			if((symptr->n_scnum > 0) && (*symptr->_n._n_name != '@'))
			{
				ptr->value = symptr->n_value;
				ptr->phyval = symptr->n_value - shdr[symptr->n_scnum-1].s_vaddr;
				ptr->phyval += shdr[symptr->n_scnum-1].s_paddr;
				ptr->sym = nptr;
				ptr->sect = shdr[symptr->n_scnum-1].s_flags;

				if(symptr->_n._n_n._n_zeroes == 0) {
					nptr = symcopy(nptr, &(flexname->string_ptr[(symptr->_n._n_n._n_offset) - 4]), -1);
				} else {
					nptr = symcopy(nptr, symptr->_n._n_name, SYMNMLEN);
				}
				ptr++;	
			}
		}
		symptr++;
		cnt++;
	}

	ptr->sym = 0;
	ptr = symtable;

	do
	{
		ptr1 = ptr;

		do
		{
			if(ptr->phyval > ptr1->phyval)
			{
				hold = *ptr;
				*ptr = *ptr1;
				*ptr1 = hold;
			}
			ptr1++;
		} while(ptr1->sym);

		ptr++;

	} while(ptr->sym);

	initsymptrs(M_INSTR);
	initsymptrs(M_DATA);

	return(0);
}


	/**********************************************************
	*
	*      symcopy()
	*/

char *symcopy(char *ptr1, char *ptr2, int cnt)
{
	do
	{
		*ptr1++ = *ptr2++;
	} while(--cnt && *ptr2);

	return(++ptr1);
}



	/**********************************************************
	*
	*      symfree()
	*/

void symfree(void)
{
	if(symtable)
		free(symtable);

	symtable = NULL;

	if(nametable)
		free(nametable);

	nametable = NULL;
}


	/*************************************************************
	*
	*
	*	prtsym();
	*/

int prtsym(void)
{
#ifdef NOT_SPEC95
	FILE	*popen();
	FILE	*st;
#endif
	struct symbols *symptr = symtable;
	int value;
	int cnt = 1;
	char *eptr, *mess;

	
	if(cmdcount == 1)
	{
#ifdef NOT_SPEC95
#ifndef MORE_EXISTS
		char	*pager = "more";
#else
		char	*pager = "pg";
#endif
		if((st = popen(pager, "w")) == NULL)
		{
                	perror("prtsym: popen");
	                exit(0);
		}
#endif

		while(symptr && symptr->sym)
		{
			if(symptr->sect & STYP_TEXT)
				mess = "text";
			else if(symptr->sect & STYP_DATA)
				mess = "data";
			else if(symptr->sect & STYP_BSS)
				mess = "bss ";
			else
				mess = "unkn";

			if(interrupt)
				break;

#ifdef NOT_SPEC95
			fprintf(st, "%s %08X - %s\n", mess, symptr->value, symptr->sym);
#else
			PPrintf("%s %08X - %s\n", mess, symptr->value, symptr->sym);
#endif
			symptr++;
		}
#ifdef NOT_SPEC95
       		pclose(st);
#endif
	}
	else
	{
		do
		{
			if(getexpr(cmdptrs[cnt], &eptr,(unsigned int *) &value) == -1)
				PPrintf("Unknown symbol \"%s\"\n", cmdptrs[cnt]);
			else
				PPrintf("%s = %08X\n", cmdptrs[cnt], value);
		} while(cmdcount > ++cnt);
	}
	return 0;
}



/*
*******************************************************************************
*
*       Module:     initsymptrs()
*
*       File:       symbols.c
*
*******************************************************************************
*
*  Functional Description:
*	Called from symcreate().  Initializes symbol table pointers used to
*	implement a primitive cache of the M_INSTR and M_DATA sections of the
*	loaded symbol table.
*
*  Globals:
*	st_dataptr	(locally global) Points to the first symbol in the
*			M_DATA section of the symbol table.
*	st_instrptr	(locally global) Points to the first symbol in the
*			M_INSTR section of the symbol table.
*	cur_instrptr	(locally global) Points to the symbol that was last
*			used by symbol().  It is initialized to st_instrptr.
*	nxt_instrptr	(locally global) Points to the symbol following
*			cur_instrptr or NIL if one is not available.
*
*  Parameters:
*	sect	The section of the symbol table whose pointers should be
*		initializes.  Acceptable values are M_DATA and M_INSTR
*
*  Output:
*
*  Revision History:
*	The function was changed to allow the M_INSTR and M_DATA section
*	pointer to be initialized independently.  In the case of the M_INSTR
*	section, the next pointer is computed.
*
*******************************************************************************/

void initsymptrs(int sect)
{
	struct symbols		*find_next_symbol();
	register struct symbols *ptr = symtable;

	/* Find the first symbol in the desired section */
	while ((ptr->sym) && !(ptr->sect & sect))
		ptr++;


	/* Initialize the appropriate section pointers */
	if((ptr->sym) && (ptr->sect & sect))
	{
		if (sect == M_INSTR)
		{
			st_instrptr = ptr;
			cur_instrptr = ptr;
			nxt_instrptr = find_next_symbol(M_INSTR, cur_instrptr);
		}
		else if (sect == M_DATA)
		{
			st_dataptr = ptr;
		}
	}
}



/*
*******************************************************************************
*
*       Module:     find_next_symbol()
*
*       File:       symbols.c
*
*******************************************************************************
*
*  Functional Description:
*	Given a symbol table section name and a starting symbol, this function
*	searches for the next symbol in the table and returns a pointer to it.
*	A value if (0) is returns if the search is unssucessfull.
*
*  Globals:
*
*  Parameters:
*	sect	The section to search in the symbol table.
*	cptr	A pointer to the symbol to begin the search from.  This
*		pointer should point to a valid symbol.
*
*  Output:
*	Returns a pointer to the next symbol in the symbol table of the
*	specifies section.  A value of NILL (0) is returned if the search
*	was unsuccessful.
*
*  Revision History:
*
*******************************************************************************/

struct symbols *find_next_symbol(int sect, struct symbols *cptr)
{
	cptr++;					/* Point to next symbol */

	while(cptr->sym && !(cptr->sect & sect))
		cptr++;

	if ((cptr->sym) && (cptr->sect & sect))
		return(cptr);
	else
		return(0);
}
