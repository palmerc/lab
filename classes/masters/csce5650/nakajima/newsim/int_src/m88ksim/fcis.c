/* @(#) File name: fcis.c   Ver: 3.1   Cntl date: 1/20/89 14:25:17 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"


ULONG fcis(int a,ULONG *dest,ULONG rnd)
{
   FPNUM         res;
   ULONG         g,r,s,flag;

/* Prepare for Normalization */

   s = 0;

   if (a < 0) {
      res.sign = 1;
      a = -a;
   }
   else res.sign = 0;

   res.exp = 0x98;
   res.mant = a;

/* Normalize */

   normalizes(&res.exp,&res.mant,&s);

/* Round */

   g = (res.mant>>1)&1;
   r = res.mant&1;
   res.mant >>= 2;

   flag = rounds(&res.exp,&res.sign,&res.mant,g,r,s,rnd);

/* Sign of zero result */

   if (res.mant==0)
      if (rnd == RM)
	 res.sign = 1;
      else
	 res.sign = 0;

/* Return result */

   flag |= return_single(res.sign,res.exp,res.mant,dest,0,0,0,0);
   return(flag);
}
