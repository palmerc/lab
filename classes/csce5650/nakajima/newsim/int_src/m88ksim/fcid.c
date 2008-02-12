/* @(#) File name: fcid.c   Ver: 3.1   Cntl date: 1/20/89 14:25:16 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"


ULONG fcid(int a,ULONG *desthi,ULONG *destlo)
{
   FPLONG        dest;
   ULONG         flag;

/* Calculate the sign and convert operand to unsigned */

   if (a < 0) {
      dest.sign = 1;
      a = -a;
   }
   else dest.sign = 0;

/* Calculate the exponent */

   for (dest.exp = 1023+31; a > 0 ; a<<=1, dest.exp--);

/* Calculate the mantissa */

   dest.manthi = ((ULONG)a)>>11;
   dest.mantlo = a<<21;

/* Return the conversion result */

   flag = return_double(dest.sign,dest.exp,dest.manthi,dest.mantlo,desthi,destlo,0,0,0,0);
   return(flag);
}
