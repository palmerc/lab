/* @(#) File name: fcds.c   Ver: 3.1   Cntl date: 1/20/89 14:25:14 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern char convovf[];
extern char convunf[];

ULONG fcds(ULONG ahi,ULONG alo,ULONG *dest,ULONG rnd)
{
   FPLONG a;
   ULONG g,r,s,flag;

/* Extract fields */

   a.sign = BFEXTU(ahi,31,1);
   a.exp  = BFEXTU(ahi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   a.mantlo = alo;

/* Insert Hidden Bit */

   if (a.exp != 0) a.manthi |= 0x00100000;

/* Reserved operand check */

   flag = reserved(ahi,alo,0,0,0);
   if (flag != 0) return(flag);

   if (flag) {
      *dest = 0;
      return(flag);
   }

   a.exp = a.exp - 1023 + 127;

   if (a.exp >= 0xff)
   {
	floaterr(convovf,0,0,0,0,0,0,0,0,0,0,0,0,0);
	*dest = 0;
	return(CONVOVF);
   }
   if((a.exp <= 0) && ((a.manthi != 0) || (a.mantlo != 0)))
   {
      floaterr(convunf,0,0,0,0,0,0,0,0,0,0,0,0,0);
      *dest = 0;
      return(CONVOVF);
   }

/* Shift double to force high part of mantissa to single format */

   a.manthi <<= 3;
   a.manthi |= a.mantlo >> 29;
   a.mantlo <<= 3;
   s = (a.mantlo & 0x3fffffff);

/* Round */

   g = a.mantlo & 0x80000000;
   r = a.mantlo & 0x40000000;

   flag = rounds(&a.exp,&a.sign,&a.manthi,g,r,s,rnd);

/* Return result */

   flag = return_single(a.sign,a.exp,a.manthi,dest,0,0,0,0);
   return(flag);
}
