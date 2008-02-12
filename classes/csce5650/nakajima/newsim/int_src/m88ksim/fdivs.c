/* @(#) File name: fdivs.c   Ver: 3.1   Cntl date: 1/20/89 14:25:31 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern char divzero[];

ULONG fdivs(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd,int singleflag)
{
   FPNUM         a,b,res;
   ULONG         g=0,r=0,s=0,flag;

	flag = 0;

/* Extract fields */

   a.sign = BFEXTU(aa,31,1);
   b.sign = BFEXTU(bb,31,1);
   a.exp  = BFEXTU(aa,23,8);
   b.exp  = BFEXTU(bb,23,8);
   a.mant = BFEXTU(aa,0,23);
   b.mant = BFEXTU(bb,0,23);


/* Reserved operand check */

   flag = reserves(aa,bb,0);
   if (flag != 0) return(flag);
   flag = (flag<<FLAGWIDTH) | reserves(aa,bb,1);
   if (flag != 0) return(flag);

   if ((bb == 0) || (bb == 0x80000000)) {
      floaterr(divzero,a.sign,b.sign,a.exp,b.exp,a.mant,b.mant,0,0,singleflag,rnd,g,r,s);
      flag |= DIVZERO;
   }

   if (flag) {
      *dest = 0;
      return(flag);
   }

/* Insert Hidden Bits */

   if (a.exp != 0) a.mant |= 0x00800000;
   if (b.exp != 0) b.mant |= 0x00800000;

/* Calculate Result sign and exponent */

   res.sign = a.sign ^ b.sign;
   res.exp = a.exp - b.exp + 126;

/* Perform Division */

   divs(a.mant,b.mant,&res.mant,&s);

/* Normalize */

   if ((res.mant & 0x04000000) != 0) {
      s |= res.mant & 1;
      res.mant >>= 1;
      res.exp++;
   }

/* Round */

   g = (res.mant>>1)&1;
   r = res.mant&1;
   res.mant >>= 2;

   flag |= rounds(&res.exp,&res.sign,&res.mant,g,r,s,rnd);

/* Return result */

   flag |= return_single(res.sign,res.exp,res.mant,dest,rnd,g,r,s);

   return(flag);
}
