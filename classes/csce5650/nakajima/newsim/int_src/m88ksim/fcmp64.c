/* @(#) File name: fcmp64.c   Ver: 3.1   Cntl date: 1/20/89 14:25:21 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

#define HS    0x800
#define LO    0x400
#define LS    0x200
#define HI    0x100
#define GE    0x80
#define LT    0x40
#define LE    0x20
#define GT    0x10
#define NE    0x08
#define EQ    0x04
#define CP    0x02
#define NC    0x01


ULONG fcmp64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,ULONG *dest)
{
   FPLONG        a,b,diff;
   ULONG         s,flag;

   *dest = CP;

/* Reserved operand check */

   flag = reserved(ahi,alo,bhi,blo,0);
   if (flag != 0) return(flag);
   flag = (flag<<FLAGWIDTH) | reserved(ahi,alo,bhi,blo,1);
   if (flag != 0) return(flag);

   if (flag) {
      *dest = 0;
      return(flag);
   }

/* Extract fields, inverting the sign of the b operand */

   a.sign = BFEXTU(ahi,31,1);
   b.sign = ! BFEXTU(bhi,31,1);
   a.exp  = BFEXTU(ahi,20,11);
   b.exp  = BFEXTU(bhi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   b.manthi = BFEXTU(bhi,0,20);
   a.mantlo = alo;
   b.mantlo = blo;

/* Insert Hidden Bits */

   if (a.exp != 0) a.manthi |= 0x00100000;
   if (b.exp != 0) b.manthi |= 0x00100000;

/* Align operands */

   alignd(a.exp,&a.manthi,&a.mantlo,b.exp,&b.manthi,&b.mantlo,&diff.exp,&s);

/* Calculate the difference */

   addd(a.sign,a.manthi,a.mantlo,b.sign,b.manthi,b.mantlo,&diff.sign,&diff.manthi,&diff.mantlo,s);
   b.sign = !b.sign;	/* Give b.sign its original value for comparison */
/* Calculate the comparison result register value */

	if (b.sign == 0 || b.exp == 0)
		/* (s1 > s2) || (s1 < 0) */
		if ((a.exp > b.exp) || ((a.exp == b.exp) && ((!diff.sign) && ((diff.manthi > 0) || ((diff.manthi == 0) && (diff.mantlo > 0))))) || (a.sign && a.exp))
			*dest |= HI | HS;
		/* (s2 > s1) && (s1 != 0) */
		else if (((b.exp > a.exp) || ((b.exp == a.exp) && ((b.manthi > a.manthi) || ((b.manthi == a.manthi) && (b.mantlo > a.mantlo))))) && a.exp)
			*dest |= LO | LS;
				
		/* s1 == s2 */
		else
			*dest |= LS | HS;

   if ((diff.manthi == 0) && (diff.mantlo == 0) && (s == 0))
      *dest |= EQ | LE | GE;
   else {
      *dest |= NE;

      if (diff.sign)
	 *dest |= LT | LE;
      else
	 *dest |= GT | GE;
   }

   return(flag);
}
