/* @(#) File name: simload.c   Ver: 3.1   Cntl date: 1/20/89 14:41:04 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"

/**************************************************************************/
/*  maksim loads the M88000 simulator Ram                                 */
/*  The raw op code is loaded, along with an expanded version, fitting    */
/*  the IR_FIELDS structure (instruction register fields).  The expanded  */
/*  version is built using data from the raw op code along with data      */
/*  from the disassembler table.                                          */
/**************************************************************************/

INSTAB  simdata = {0,"word           ",{0,32,HEX},{0,0,0},{0,0,0},{0,0,0,0,0,0,0,0,0} ,NULL};

void makesim(unsigned int instr, struct IR_FIELDS *opcode)
{
     register INSTAB *tblptr;
/* Determine disassembler opword mask for the instruction. */
/* Search disassembler instruction table for the instruction. */

       if(!(tblptr = lookupdisasm(instr & classify(instr))))
	   tblptr = &simdata;

       opcode->op = tblptr->flgs.op;
       opcode->dest = uext(instr, tblptr->op1.offset, tblptr->op1.width);
       opcode->src1 = uext(instr, tblptr->op2.offset, tblptr->op2.width);

       if(!tblptr->flgs.imm_flags)
	    opcode->src2 = uext(instr, tblptr->op3.offset, tblptr->op3.width);

       opcode->p = tblptr;
}
