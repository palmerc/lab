/* @(#) File name: fcmps.c   Ver: 3.1   Cntl date: 1/20/89 14:25:23 */

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


ULONG fcmps(ULONG aa,ULONG bb,ULONG *dest)
{
   FPNUM         a,b,diff;
   ULONG         s,flag;

   *dest = CP;

/* Reserved operand check */

   flag = reserves(aa,bb,0);
   if (flag != 0) return(flag);
   flag = (flag<<FLAGWIDTH) | reserves(aa,bb,1);
   if (flag != 0) return(flag);

   if (flag) {
      *dest = 0;
      return(flag);
   }

/* Extract fields, inverting the sign of the b operand */

   a.sign = BFEXTU(aa,31,1);
   b.sign = ! BFEXTU(bb,31,1);
   a.exp  = BFEXTU(aa,23,8);
   b.exp  = BFEXTU(bb,23,8);
   a.mant = BFEXTU(aa,0,23);
   b.mant = BFEXTU(bb,0,23);

/* Insert Hidden Bits */

   if (a.exp != 0) a.mant |= HBITMASK;
   if (b.exp != 0) b.mant |= HBITMASK;

/* Align operands */

   aligns(a.exp,&a.mant,b.exp,&b.mant,&diff.exp,&s);

/* Calculate the difference */

   adds(a.sign,a.mant,b.sign,b.mant,&diff.sign,&diff.mant,s);
   b.sign = !b.sign;	/* Make b.sign its original value for comparisons */

/* Calculate the comparison result register value */

	if (b.sign == 0 || b.exp == 0)
		/* (s1 > s2) || (s1 < 0) */
		if ((a.exp > b.exp) || ((a.exp == b.exp) && (((!diff.sign) && diff.mant) > 0)) || (a.sign && a.exp))
			*dest |= HI | HS;
		/* (s2 > s1) && (s1 != 0) */
		else if (((b.exp > a.exp) || ((a.exp == b.exp) && (b.mant > a.mant))) && a.exp)
			*dest |= LO | LS;
		/* s1 == s2 */
		else
			*dest |= LS | HS;


   if ((diff.mant == 0) && (s == 0))
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
