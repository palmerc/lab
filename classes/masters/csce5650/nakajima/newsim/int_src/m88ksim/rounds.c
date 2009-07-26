/* @(#) File name: rounds.c   Ver: 3.1   Cntl date: 1/20/89 14:26:11 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern ULONG addone;

extern char inexact[];


ULONG rounds(int *resexp,ULONG *resign,ULONG *resmant,
ULONG g,ULONG r,ULONG s,ULONG rnd)
{
   ULONG l;

/* Calculate the l bit for rounding */

   l = *resmant & 1;

/* Determine rounding */

   addone = round(*resign,l,g,r,s,rnd);

   if (addone)
      (*resmant)++;

/* Set inexact flag if any of g, r, or s bits are on */

   if (g || r || s)
		if (FPCR & EFINX)
			{
			FPECR |= FINX;
			FPSR  |= AFINX;
			floaterr(inexact,*resign,0,*resexp,0,*resmant,0,0,0,1,r,g,rnd,s);
      		return(INEXACT);
			}
		else
			{
			FPSR  |= AFINX;
      		return(0);
			}
   else 
		return(0);

}
