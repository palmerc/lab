/* @(#) File name: aligns.c   Ver: 3.1   Cntl date: 1/20/89 14:25:02 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void aligns(int aexp,ULONG *amant,int bexp,ULONG *bmant,int *resexp,ULONG *s)
{
   ULONG temp;
   int expdiff;

/* Calculate the exponent difference */

   expdiff = aexp - bexp;

/* Shift up to allow for guard and round */

   *amant <<= 2;
   *bmant <<= 2;

/* Down shift the number with the smaller exponent by the difference in  */
/* the exponents, keeping track of the sticky bits.                      */

   if (expdiff >= 0) {
      temp = *bmant;
      if (expdiff <= 32) {
	 *bmant >>= expdiff;
	 *s = (temp != (*bmant << expdiff));
      }
      else {
	 *s = (*bmant != 0);
	 *bmant = 0;
      }
      *resexp = aexp;
   }
   else {
      expdiff = -expdiff;
      if (expdiff <= 32) {
	 temp = *amant;
	 *amant >>= expdiff;
	 *s = (temp != (*amant << expdiff));
      }
      else {
	 *s = (*amant != 0);
	 *amant = 0;
      }
      *resexp = bexp;
   }
}

