/* @(#) File name: divs.c   Ver: 3.1   Cntl date: 1/20/89 14:25:05 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "float.h"
#include "functions.h"

void divs(ULONG a,ULONG b,ULONG *res,ULONG *s)
{
   int i;

   a <<= 1;
   b <<= 1;

/* The operands are normalized so 26 iterations will get the required accuracy */
/* This is a restoring divide algorithm */

   for(i=0 , *res = 0 ; i < 26 ; i++) {
      if ((b <= a) && (b != 0)) {
	 *res |= 1;
	 a -= b;
      }
      *res <<= 1;
      a <<= 1;
   }

/* If remainder exists, set the sticky bit */

   if (a != 0)  *s = 1;
   else *s = 0;
}
