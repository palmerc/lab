/* @(#) File name: multd.c   Ver: 3.1   Cntl date: 1/20/89 14:25:51 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void multd(ULONG asign,int aexp,ULONG amanthi,ULONG amantlo,ULONG bsign,
int bexp,ULONG bmanthi,ULONG bmantlo,ULONG *resign,int *resexp,
ULONG *resmanthi,ULONG *resmantlo,ULONG *s)
{
   ULONG  prod,a[4],b[4],res[8];
   ULONG  i,j;

/* Determine result sign */

   *resign = asign ^ bsign;

/* Split the mantissa into 16 bit parts */

   a[0] = BFEXTU(amantlo,0,16);
   a[1] = BFEXTU(amantlo,16,16);
   a[2] = BFEXTU(amanthi,0,16);
   a[3] = BFEXTU(amanthi,16,16);
   b[0] = BFEXTU(bmantlo,0,16);
   b[1] = BFEXTU(bmantlo,16,16);
   b[2] = BFEXTU(bmanthi,0,16);
   b[3] = BFEXTU(bmanthi,16,16);

/* Multiply parts */

   for (i = 0 ; i < 8 ; i++) res[i] = 0;

   for (i = 0 ; i < 4 ; i++) {
      for (j = 0 ; j < 4 ; j++) {
	 prod = a[i] * b[j];
	 res[i+j] += prod & 0xffff;
	 res[i+j+1] += (prod & 0xffff0000)>>16;
      }
   }

/* Sum the parts */

   for (i = 0; i < 6 ; i++) {
      if (res[i] > 0xffff) {
	  res[i+1] += res[i]>>16;
	  res[i] &= 0xffff;
      }
   }

/* Form the result with guard and round bits */

   *resmanthi = res[6] << 14;
   *resmanthi |= res[5] >> 2;
   *resmantlo = res[5] << 30;
   *resmantlo |= res[4] <<14;
   *resmantlo |= res[3] >>2;
   *s = (res[3] & 3) | res[2] | res[1] | res[0];

/* If overflow, downshift by 1 */

   if (*resmanthi & 0x00800000) {
      *s |= *resmantlo & 1;
      *resmantlo >>= 1;
      *resmantlo |= (*resmanthi)<<31;
      *resmanthi >>= 1;
      *resexp = aexp + bexp - 1022;
   }
   else {
      *resexp = aexp + bexp - 1023;
   }
}
