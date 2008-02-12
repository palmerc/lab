/* @(#) File name: returns.c   Ver: 3.1   Cntl date: 1/20/89 14:26:03 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

extern char overflow[];
extern char underflow[];


ULONG return_single(ULONG sign,int exp,ULONG mant,ULONG *dest,ULONG rnd,
ULONG grd,ULONG rd,ULONG stky)
{

/* If zero, return signed zero */

   if (mant == 0) {
      *dest = sign<<31;
      return(0);
   }

/* If round caused overflow, increment the exponent */

   else if ((mant & VBITMASK) != 0)
      exp++;

/* Check for overflow or underflow in the result */

   if (exp >= MAXEXP) 
		{
      floaterr(overflow,sign,0,exp,0,mant,0,0,0,1,rnd,grd,rd,stky);
      *dest = 0;
      return(OVERFLOW);
   }
   else if (exp <= MINEXP) 
   		{
      floaterr(underflow,sign,0,exp,0,mant,0,0,0,1,rnd,grd,rd,stky);
      *dest = 0;
      return(UNDERFLOW);
   }

   *dest = ((sign<<31) | ((exp & MAXEXP)<<23) | (mant & MANTMASK));
   return(0);
}


