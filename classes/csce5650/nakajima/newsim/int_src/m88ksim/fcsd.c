/* @(#) File name: fcsd.c   Ver: 3.1   Cntl date: 1/20/89 14:25:24 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"


ULONG fcsd(ULONG a,ULONG *desthi,ULONG *destlo)
{
   ULONG flag,exp;

/* Reserved operand check */

   flag = reserves(a,0,0);
   if (flag != 0) return(flag);

   if (flag) {
      *desthi =  0;
      *destlo = 0;
      return(flag);
   }

/* Do the conversion */

   *desthi = a & SIGNMASK;

   exp = BFEXTU(a,23,8);
   if(exp != 0)
      exp += 1023 - 127;
   *desthi |= exp<<20;

   *desthi |= (a & MANTMASK)>>3;

   *destlo = (a & MANTMASK)<<29;

/* Return the no error flag */

   return(0);
}
