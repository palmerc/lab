/* @(#) File name: mults.c   Ver: 3.1   Cntl date: 1/20/89 14:25:53 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void mults(ULONG asign,int aexp,ULONG amant,ULONG bsign,int bexp,ULONG bmant,
ULONG *resign,int *resexp,ULONG *resmant,ULONG *s)
{
   ULONG  a_hi,a_lo,b_hi,b_lo,res_hi,res_lo,temp;

/* Determine result sign */

   *resign = asign ^ bsign;

/* Split the mantissa into 12 bit halves */

   a_hi = BFEXTU(amant,12,12);
   a_lo = BFEXTU(amant,0,12);
   b_hi = BFEXTU(bmant,12,12);
   b_lo = BFEXTU(bmant,0,12);

/* Multiply parts */

   res_lo = a_lo * b_lo;
   res_hi = a_hi * b_hi;
   temp   = a_hi * b_lo;
   temp  += a_lo * b_hi;

   res_lo += (temp & 0x00000fff)<<12;

   if (res_lo & 0x01000000) {
      res_hi++;
      res_lo &= 0x00ffffff;
   }

   res_hi += temp>>12;

/* Calculate sticky and Normalize */

   *s = res_lo & 0x001fffff;

   if (res_hi & 0x00800000) {
      *s |= res_lo & 0x00200000;
      *resmant = (res_hi<<2) | ((res_lo & 0x00ffffff)>>22);
      *resexp = aexp + bexp - 126;
   }
   else {
      *resmant = (res_hi<<3) | ((res_lo & 0x00ffffff)>>21);
      *resexp = aexp + bexp - 127;
   }
}
