/* @(#) File name: fcsi.c   Ver: 3.1   Cntl date: 1/20/89 14:25:26 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern char intover[];

ULONG fcsi(ULONG aa,int *dest,ULONG rnd)
{
   FPNUM         a,b,res;
   ULONG         g,r,s,flag;

/* Extract fields */

   a.sign = BFEXTU(aa,31,1);
   a.exp  = BFEXTU(aa,23,8);
   a.mant = BFEXTU(aa,0,23);
   b.sign = 0;
   b.exp  = 127+23; /* it used to be 127+23 */
   b.mant = 0;
   s = 0;

/* Before Error Checks */

   flag = reserves(0,aa,1);
   if (flag != 0) return(flag);

   if (a.sign) {
      if ((a.exp > (127+31)) || ((a.exp == (127+31)) && (a.mant != 0))) {
	 floaterr(intover,0,a.sign,0,a.exp,0,a.mant,0,0,1,0,0,0,0); /* source 2 info sent*/
	 flag |= CONVOVF;
      }
   }
   else if (a.exp >= (127+31)) {
	  floaterr(intover,0,a.sign,0,a.exp,0,a.mant,0,0,1,0,0,0,0); /* source 2 info sent*/
      flag |= CONVOVF;
   }

   if (flag) {
      *dest = 0;
      return(flag);
   }

/* Insert Hidden Bits */

   if (a.exp != 0) a.mant |= HBITMASK;

/* Align operand */

   if (a.exp < (127+24)) {
      aligns(a.exp,&a.mant,b.exp,&b.mant,&res.exp,&s);
      g = (a.mant>>1)&1;
      r = a.mant&1;
      a.mant >>= 2;
      flag |= rounds(&a.exp,&a.sign,&a.mant,g,r,s,rnd);
   }
   else {
      a.mant <<= a.exp - (127+23);
   }

   if (a.sign)
      *dest = -a.mant;
   else *dest = a.mant;

   return(flag);
}
