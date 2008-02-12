/* @(#) File name: fsub64.c   Ver: 3.1   Cntl date: 1/20/89 14:25:46 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"


ULONG fsub64(
ULONG ahi,
ULONG alo,
ULONG bhi,
ULONG blo,
ULONG *desthi,
ULONG *destlo,
ULONG rnd,
ULONG rndprec)
{
   FPLONG        a,b,dest;
   ULONG         g,r,s,flag;

/* Reserved operand check */


   flag = reserved(ahi,alo,bhi,blo,0);
   if (flag != 0) return(flag);
   	flag = (flag<<FLAGWIDTH) | reserved(ahi,alo,bhi,blo,1);
   if (flag != 0) return(flag);

   if (flag) {
      *desthi = 0;
      *destlo = 0;
      return(flag);
   }

/* Extract fields */

   a.sign = BFEXTU(ahi,31,1);
   b.sign = BFEXTU(bhi,31,1);
   a.exp  = BFEXTU(ahi,20,11);
   b.exp  = BFEXTU(bhi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   b.manthi = BFEXTU(bhi,0,20);
   a.mantlo = alo;
   b.mantlo = blo;

/* Negate the sign of the second operand */

   b.sign = !b.sign;

/* Insert Hidden Bits */

   if (a.exp !=0) a.manthi |= HBITMASKD;
   if (b.exp !=0) b.manthi |= HBITMASKD;

/* Align operands */

   alignd(a.exp,&a.manthi,&a.mantlo,b.exp,&b.manthi,&b.mantlo,&dest.exp,&s);

/* Perform Addition */

   addd(a.sign,a.manthi,a.mantlo,b.sign,b.manthi,b.mantlo,&dest.sign,
	&dest.manthi,&dest.mantlo,s);

/* Normalize */

   normalized(&dest.exp,&dest.manthi,&dest.mantlo,&s);

/* Round */

   if (rndprec != SINGLE) 
		{
      g = (dest.mantlo>>1) & 1;
      r = dest.mantlo & 1;
      dest.mantlo >>= 2;
      dest.mantlo |= dest.manthi<<30;
      dest.manthi >>= 2;
      flag |= roundd(dest.exp,&dest.sign,&dest.manthi,&dest.mantlo,g,r,s,rnd);
   		}
   else {
      g = (dest.mantlo>>30) & 1;
      r = (dest.mantlo>>29) & 1;
      s |= dest.mantlo & 0x1fffffff;
      dest.mantlo &= 0x80000000;
      dest.manthi <<= 1;
      dest.manthi |= dest.mantlo>>31;
      flag |= rounds(&dest.exp,&dest.sign,&dest.manthi,g,r,s,rnd);
      dest.mantlo = dest.manthi<<29;
      dest.manthi >>= 3;
   }

/* Sign of zero result */

   if (dest.manthi==0 && dest.mantlo==0 && (a.sign != b.sign))
      if (rnd == RM)
	 dest.sign = 1;
      else
	 dest.sign = 0;

/* Return result */

   flag |= return_double(dest.sign,dest.exp,dest.manthi,dest.mantlo,desthi,destlo,rnd,g,r,s);

   return(flag);
}
