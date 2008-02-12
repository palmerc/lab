/* @(#) File name: table.c   Ver: 3.1   Cntl date: 1/20/89 14:41:36 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1985";

/*
*****************************************************************************
*          Lookup an Instruction in the Disassembler Instruction Table
*****************************************************************************
*
*       Module:   lookupdisasm()
*
*       File:     table.c
*
*************************** Functional Description **************************
*
*       Search the instruction table for the opcode specified. If the opcode
*       is not in the table a NULL pointer is returned, else a pointer to
*       the instruction in the table is returned.
*
********************************* Parameters ********************************
*
*       UINT    key             The opcode to be searched for in the table
*
*********************************** Output **********************************
*
*       Returns a pointer to the entry found.  If the key is not in the table
*       a NULL is returned.
*
****************************** Revision History *****************************
*
*       Revision 1.0    11/08/85        Creation date
*
****************************************************************************/

#include "functions.h"

INSTAB  *hashtab[HASHVAL] = {0};

INSTAB *lookupdisasm(UINT key)
{
   INSTAB *ptr;

   ptr = hashtab[key % HASHVAL];

   while (ptr != NULL && ptr->opcode != key)
      ptr = ptr->next;

   if (ptr == NULL)
      return(NULL);
   else
      return(ptr);
}

/*
*****************************************************************************
*                 Initialize the Disassembler Instruction Table
*****************************************************************************
*
*       Module:   init_disasm()
*
*       File:     table.c
*
*************************** Functional Description **************************
*
*       Initialize the hash table and instruction table for the disassembler.
*       This should be called once before the first call to disasm().
*
********************************* Parameters ********************************
*
*********************************** Output **********************************
*
*       If the debug option is selected, certain statistics about the hashing
*       distribution are written to stdout.
*
****************************** Revision History *****************************
*
*       Revision 1.0    11/08/85        Creation date
*
****************************************************************************/

extern INSTAB instructions[];


void init_disasm(void)
{
   int i;
   INSTAB *p;

   for (i = 0; i < HASHVAL; i++)
      hashtab[i] = NULL;

   for(p = instructions; p->mnemonic; install(p++));
}

/*
*****************************************************************************
*        Install an Instruction into the Disassembler Instruction Table
*****************************************************************************
*
*       Module:   install()
*
*       File:     table.c
*
*************************** Functional Description **************************
*
*       Insert an instruction into the disassembler table by hashing the
*       opcode and inserting it into the linked list for that hash value.
*
********************************* Parameters ********************************
*
*       INSTAB *instptr         Pointer to the entry in the instruction table
*                               to be installed
*
*********************************** Output **********************************
*
*       If any non-opcode bits are on in the instruction, a system error
*       message is written to stdout and the program is exited. This indicates
*       an error in the instruction table.
*
****************************** Revision History *****************************
*
*       Revision 1.0    11/08/85        Creation date
*
****************************************************************************/

void install(INSTAB *instptr)
{
   UINT i,mask;

/* Determine if any non-opcode bits are on in the instruction being placed */
/* in the instruction table.                                               */

   mask = classify(instptr->opcode);
   if (instptr->opcode & (~mask)) {
      Eprintf("\nSYSTEM ERROR - Invalid opcode specified in instruction table.\n");
      Eprintf("\tOpcode is %08x  Mnemonic is %s\n",instptr->opcode,
	 instptr->mnemonic);
   }

   i = (instptr->opcode) % HASHVAL;
   instptr->next = hashtab[i];
   hashtab[i] = instptr;
}
