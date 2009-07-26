/* @(#) File name: normalized.c   Ver: 3.1   Cntl date: 1/20/89 14:25:55 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void normalized(int *resexp,ULONG *resmanthi,ULONG *resmantlo,ULONG *s)
{

   if ((*resmanthi == 0) && (*resmantlo == 0)) return;

/* If VBIT is set downshift the mantissa and adjust the exponent else shift */
/* up and adjust the exponent until normalized. If zero do not adjust.      */

   while (*resmanthi & 0xff800000) {
      *s = *s || (*resmantlo & 1);
      *resmantlo >>= 1;
      *resmantlo |= (*resmanthi)<<31;
      *resmanthi >>= 1;
      (*resexp)++;
   }

   while ((*resmanthi & (HBITMASKD<<2)) == 0) {
      *resmanthi <<= 1;
      *resmanthi |= (*resmantlo)>>31;
      *resmantlo <<= 1;
      (*resexp)--;
   }
}
