/* @(#) File name: fdiv64.c   Ver: 3.1   Cntl date: 1/20/89 14:25:29 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

extern char divzero[];

ULONG fdiv64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *desthi,ULONG *destlo,ULONG rnd)
{
   FPLONG        a,b,res;
   ULONG         g,r,s,flag;

/* Extract fields */

   a.sign = BFEXTU(ahi,31,1);
   b.sign = BFEXTU(bhi,31,1);
   a.exp  = BFEXTU(ahi,20,11);
   b.exp  = BFEXTU(bhi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   b.manthi = BFEXTU(bhi,0,20);
   a.mantlo = alo;
   b.mantlo = blo;

/* Reserved operand check */

   flag = reserved(ahi,alo,bhi,blo,0);
   if (flag != 0) return(flag);
   flag = (flag<<FLAGWIDTH) | reserved(ahi,alo,bhi,blo,1);
   if (flag != 0) return(flag);

   if (((bhi == 0) || (bhi == 0x80000000)) && (blo == 0)) {
      floaterr(divzero,a.sign,b.sign,a.exp,b.exp,a.manthi,b.manthi,a.mantlo,b.mantlo,0,0,0,0,0);
      flag |= DIVZERO;
   }

   if (flag) {
      *desthi = 0;
      *destlo = 0;
      return(flag);
   }

/* Insert Hidden Bits */

   if (a.exp != 0) a.manthi |= 0x00100000;
   if (b.exp != 0) b.manthi |= 0x00100000;

/* Calculate Result sign and exponent */

   res.sign = a.sign ^ b.sign;
   res.exp = a.exp - b.exp + 1022;

/* Perform Division */

   divd(a.manthi,a.mantlo,b.manthi,b.mantlo,&res.manthi,&res.mantlo,&s);

/* Normalize */

   if ((res.manthi & 0x00800000) != 0) {
      s |= res.mantlo & 1;
      res.mantlo >>= 1;
      res.mantlo |= res.manthi<<31;
      res.manthi >>= 1;
      res.exp++;
   }

/* Round */

   g = (res.mantlo>>1)&1;
   r = res.mantlo&1;
   res.mantlo >>= 2;
   res.mantlo |= res.manthi <<30;
   res.manthi >>= 2;

   flag |= roundd(res.exp,&res.sign,&res.manthi,&res.mantlo,g,r,s,rnd);

/* Return result */

   flag |= return_double(res.sign,res.exp,res.manthi,res.mantlo,desthi,destlo,rnd,g,r,s);

   return(flag);
}
