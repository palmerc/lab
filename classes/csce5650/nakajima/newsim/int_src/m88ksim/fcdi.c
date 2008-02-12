/* @(#) File name: fcdi.c   Ver: 3.1   Cntl date: 1/20/89 14:25:12 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

extern char intover[];

ULONG fcdi(ULONG ahi,ULONG alo,int *dest,ULONG rnd)
{
   FPLONG        a;
   ULONG         i,g,r,s,flag;

/* Extract fields */

   a.sign = BFEXTU(ahi,31,1);
   a.exp  = BFEXTU(ahi,20,11);
   a.manthi = BFEXTU(ahi,0,20);
   a.mantlo = alo;
   s = 0;

/* Before Error Checks */

   flag = reserved(0,0,ahi,alo,1); /* check src2 */
   if (flag != 0) return(flag);

   if (a.sign) 
		{
		if ((a.exp == (1023+31)) && (a.manthi == 0) && ((a.mantlo&0xffe00000) == 0)) 
			{
			if (round(a.sign,a.mantlo&0x200000,a.mantlo&0x100000,a.mantlo&0x080000,a.mantlo&0x7ffff,rnd)) 
				{
				floaterr(intover,0,0,0,0,0,0,0,0,0,0,0,0,0);
				flag |= CONVOVF;
				}
			}
		else 
			if (a.exp >= (1023+31)) 
				{
	 			floaterr(intover,0,0,0,0,0,0,0,0,0,0,0,0,0);
	 			flag |= CONVOVF;
				}
		}
   else 
		{
		if ((a.exp == (1023+30)) && (a.manthi == 0xfffff) && ((a.mantlo&0xffc00000) == 0xffc00000)) 
			{
			if (round(a.sign,a.mantlo&0x400000,a.mantlo&0x200000,a.mantlo&0x100000,a.mantlo&0xfffff,rnd)) 
				{
	 			floaterr(intover,0,0,0,0,0,0,0,0,0,0,0,0,0);
	    		flag |= CONVOVF;
				}
			}
		else 
			if (a.exp >= (1023+31)) 
				{
	 			floaterr(intover,0,0,0,0,0,0,0,0,0,0,0,0,0);
	 			flag |= CONVOVF;
      			}
   		}

   if (flag) 
		{
      	*dest = 0;
      	return(flag);
		}

/* Insert Hidden Bit */

   if (a.exp != 0) a.manthi |= 0x100000;

/* Align operand */

   if (a.exp < (1023+20)) {
      for (i=a.exp, s=0 ; i < (1023+20) ; i++) {
	 s |= a.mantlo & 1;
	 a.mantlo >>= 1;
	 a.mantlo |= a.manthi<<31;
	 a.manthi >>= 1;
      }
   }
   else {
      for (i = a.exp ; i > (1023+20) ; i--) {
	 a.manthi <<= 1;
	 a.manthi |= a.mantlo>>31;
	 a.mantlo <<= 1;
      }
   }

/* Round to the integer using the rounding mode */

   g = (a.mantlo>>31) & 1;
   r = (a.mantlo>>30) & 1;
   s |= a.mantlo & 0x3fffffff;

   flag |= rounds(&a.exp,&a.sign,&a.manthi,g,r,s,rnd);

/* If number was negative, negate the integer */

   if (a.sign)
      *dest = -a.manthi;
   else *dest = a.manthi;

return(flag);
}
