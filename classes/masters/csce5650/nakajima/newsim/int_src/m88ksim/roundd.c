/* @(#) File name: roundd.c   Ver: 3.2   Cntl date: 2/10/89 09:53:52 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern char inexact[];

ULONG addone;

ULONG roundd(int resexp,ULONG *resign,ULONG *resmanthi,ULONG *resmantlo,
ULONG g,ULONG r,ULONG s,ULONG rnd)
{
   ULONG l;

/* Calculate the l bit for rounding */

   l = *resmantlo & 1;

/* Determine rounding */

   addone = round(*resign,l,g,r,s,rnd);

   if (addone) {
      (*resmantlo)++;
      if (*resmantlo == 0)
       (*resmanthi)++;
   }

/* Set inexact flag if any of g, r, or s bits are on */

   if (g || r || s)
		if (FPCR & EFINX)
			{
			FPECR |= FINX;
			FPSR  |= AFINX;
			floaterr(inexact,*resign,0,resexp,0,*resmanthi,0,*resmantlo,0,0,r,g,rnd,s);
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
