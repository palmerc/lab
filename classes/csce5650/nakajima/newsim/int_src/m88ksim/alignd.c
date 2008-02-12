/* @(#) File name: alignd.c   Ver: 3.1   Cntl date: 1/20/89 14:26:17 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

void alignd(int aexp,ULONG *amanthi,ULONG *amantlo,int bexp,
ULONG *bmanthi,ULONG *bmantlo,int *resexp,ULONG *s)
{
   int expdiff;

/* Calculate the exponent difference */

   expdiff = aexp - bexp;

/* Shift up to allow for guard and round */

   *amanthi <<= 2;
   *amanthi |= *amantlo>>30;
   *amantlo <<= 2;
   *bmanthi <<= 2;
   *bmanthi |= *bmantlo>>30;
   *bmantlo <<= 2;

/* Down shift the number with the smaller exponent by the difference in  */
/* the exponents, keeping track of the sticky bits.                      */

   if (expdiff >= 0) {
      for (*s = 0 ; expdiff > 0 ; expdiff--) {
	 *s |= *bmantlo & 1;
	 *bmantlo >>= 1;
	 *bmantlo |= *bmanthi<<31;
	 *bmanthi >>= 1;
      }
      *resexp = aexp;
   }
   else {
      expdiff = -expdiff;
      for (*s = 0 ; expdiff > 0 ; expdiff--) {
	 *s |= *amantlo & 1;
	 *amantlo >>= 1;
	 *amantlo |= *amanthi<<31;
	 *amanthi >>= 1;
      }
      *resexp = bexp;
   }
}

