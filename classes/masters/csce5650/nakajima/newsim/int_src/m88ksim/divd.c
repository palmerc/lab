/* @(#) File name: divd.c   Ver: 3.1   Cntl date: 1/20/89 14:25:03 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "float.h"
#include "functions.h"

void divd(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *reshi,ULONG *reslo,ULONG *s)
{
   int i;

   DLSH(ahi,alo,1);
   DLSH(bhi,blo,1);

/* The operands are normalized so 55 iterations will get the required accuracy */
/* This is a restoring divide algorithm */

   for(i=0 , *reshi = 0, *reslo = 0 ; i < 55 ; i++) {
      if (!DZERO(bhi,blo) && DGE(ahi,alo,bhi,blo)) {
	 *reslo |= 1;
	 DSUB(ahi,alo,ahi,alo,bhi,blo);
      }

      DLSH(*reshi,*reslo,1);

      DLSH(ahi,alo,1);
   }

/* If remainder exists, set the sticky bit */

   if (DZERO(ahi,alo))
      *s = 0;
   else *s = 1;

}
