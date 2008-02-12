/* @(#) File name: normalizes.c   Ver: 3.1   Cntl date: 1/20/89 14:25:57 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void normalizes(int *resexp,ULONG *resmant,ULONG *s)
{

   if (*resmant == 0) return;

/* If VBIT is set downshift the mantissa and adjust the exponent else shift */
/* up and adjust the exponent until normalized. If zero do not adjust.      */

   while (*resmant & 0xfc000000) {
      *s = *s || (*resmant & 1);
      *resmant >>= 1;
      (*resexp)++;
   }

   while ((*resmant & (HBITMASK<<2)) == 0) {
      *resmant <<= 1;
      (*resexp)--;
   }
}
