/* @(#) File name: fmuls.c   Ver: 3.1   Cntl date: 1/20/89 14:25:40 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"


ULONG fmuls(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd)
{
   ULONG         g,r,s,flag;
   FPNUM         a,b,res;

/* Reserved operand check */

   flag = reserves(aa,bb,0);
   if (flag != 0) return(flag);
   flag = (flag<<FLAGWIDTH) | reserves(aa,bb,1);
   if (flag != 0) return(flag);

   if (flag) {
      *dest = 0;
      return(flag);
   }

/* Extract fields */

   a.sign = BFEXTU(aa,31,1);
   b.sign = BFEXTU(bb,31,1);
   a.exp  = BFEXTU(aa,23,8);
   b.exp  = BFEXTU(bb,23,8);
   a.mant = BFEXTU(aa,0,23);
   b.mant = BFEXTU(bb,0,23);

/* Insert Hidden Bits */

   if (a.exp != 0) a.mant |= HBITMASK;
   if (b.exp != 0) b.mant |= HBITMASK;

/* Perform Multiplication */

   mults(a.sign,a.exp,a.mant,b.sign,b.exp,b.mant,&res.sign,&res.exp,&res.mant,&s);

/* Round */

   g = (res.mant>>1)&1;
   r = res.mant&1;
   res.mant >>= 2;

   flag |= rounds(&res.exp,&res.sign,&res.mant,g,r,s,rnd);

/* Return result */

   flag |= return_single(res.sign,res.exp,res.mant,dest,rnd,g,r,s);
   return(flag);
}
