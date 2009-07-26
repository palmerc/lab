/* @(#) File name: classify.c   Ver: 3.1   Cntl date: 1/20/89 14:41:22 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1985";

/*
*****************************************************************************
*                    Classify the Type of the Instruction
*****************************************************************************
*
*       Module:     classify()
*
*       File:       classify.c
*
*************************** Functional Description **************************
*
*       Determine the mask necessary to mask the non-opcode bits of the
*       instruction.  This is determined based in the first 6 bits of the
*       instruction.
*
********************************* Parameters ********************************
*
*       UINT inst;      The 32 bit instruction that is to be classified.
*
*********************************** Output **********************************
*
*       Returns a mask that when bitwise anded to the instruction zeroes all
*       all non-opcode bits of the instruction.
*
****************************** Revision History *****************************
*
*       Revision 1.0    11/08/85        Creation date
*
****************************************************************************/

#include "functions.h"

UINT classify(UINT inst)
{
   UINT i;

   i = inst & DEFMASK;

   if ( i >= SFU0 && i <= SFU7) {
      if ((inst) < SFU1)
	 return(CTRLMASK);
      else
	 return(SFUMASK);
   }

   if (i==RRR)
      return(RRRMASK);

   if (i==RRI10)
      return(RRI10MASK);

   return(DEFMASK);
}
