/* @(#) File name: fmul64.c   Ver: 3.1   Cntl date: 1/20/89 14:25:39 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"


ULONG fmul64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *desthi,ULONG *destlo,ULONG rnd,ULONG prec)
{
   ULONG         g,r,s,flag;
   FPLONG        a,b,res;

/* Reserved operand check */

   flag = reserved(ahi,alo,bhi,blo,0);
   if (flag != 0 ) return(flag);
   flag = (flag<<FLAGWIDTH) | reserved(ahi,alo,bhi,blo,1);
   if (flag != 0 ) return(flag);

   if (flag) {
      *desthi = 0;
      *destlo = 0;
      return(flag);
   }

/* Extract fields */

   a.sign   = BFEXTU(ahi,31,1);
   b.sign   = BFEXTU(bhi,31,1);
   a.exp    = BFEXTU(ahi,20,11);
   b.exp    = BFEXTU(bhi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   b.manthi = BFEXTU(bhi,0,20);
   a.mantlo = alo;
   b.mantlo = blo;

/* Insert Hidden Bits */

   if (a.exp != 0) a.manthi |= 0x00100000;
   if (b.exp != 0) b.manthi |= 0x00100000;

/* Perform Multiplication */

   multd(a.sign,a.exp,a.manthi,a.mantlo,b.sign,b.exp,b.manthi,b.mantlo,
      &res.sign,&res.exp,&res.manthi,&res.mantlo,&s);

/* Round */

   if (prec != SINGLE) {
      g = (res.mantlo>>1)&1;
      r = res.mantlo&1;
      res.mantlo >>= 2;
      res.mantlo |= res.manthi<<30;
      res.manthi >>= 2;

      flag |= roundd(res.exp,&res.sign,&res.manthi,&res.mantlo,g,r,s,rnd);
   }
   else {
      g = (res.mantlo>>30)&1;
      r = (res.mantlo>>29)&1;
      s = res.mantlo & 0x1fffffff;
      res.mantlo &= 0x80000000;
      res.manthi <<= 1;
      res.manthi |= res.mantlo>>31;

      flag |= rounds(&res.exp,&res.sign,&res.manthi,g,r,s,rnd);

      res.mantlo = res.manthi<<29;
      res.manthi >>= 3;
   }

/*   if (res.manthi & 0xffe00000)        */
/*      res.exp++;                       */

/* Return result */


   flag |= return_double(res.sign,res.exp,res.manthi,res.mantlo,desthi,destlo,rnd,g,r,s);
   return(flag);
}
