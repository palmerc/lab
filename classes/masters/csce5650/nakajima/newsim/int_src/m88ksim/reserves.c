/* @(#) File name: reserves.c   Ver: 3.1   Cntl date: 1/20/89 14:26:00 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "float.h"
#include "functions.h"

extern char naninf[];
extern char denorm[];


ULONG reserves(ULONG src1,ULONG src2,int opchoice)
{
   ULONG a;
   ULONG flag;
   FPNUM err1,err2;

/* select proper operand */

   if (opchoice)
	   a = src2;
   else
       a = src1;

/* Extract fields */
	
	err1.sign = BFEXTU(src1,31,1);
	err2.sign = BFEXTU(src2,31,1);
	err1.exp  = BFEXTU(src1,23,8);
	err2.exp  = BFEXTU(src2,23,8);
	err1.mant = BFEXTU(src1,0,23);
	err2.mant = BFEXTU(src2,0,23);

/* Check for Nan and Infinity and Denorm */

   if ((a & 0x7f800000) == 0x7f800000) 
		{
      	if ((a & 0x007fffff) == 0)
	 		flag = INF;
      	else 
			flag = NAN;
      	floaterr(naninf,err1.sign,err2.sign,err1.exp,err2.exp,err1.mant,err2.mant,0,0,1,0,0,0,0);
   		}
   else 
		if (((a & 0x7f800000) == 0) && ((a & 0x007fffff) != 0)) 
			{
      		flag = DENORM;
      		floaterr(denorm,err1.sign,err2.sign,err1.exp,err2.exp,err1.mant,err2.mant,0,0,1,0,0,0,0);
   			}
   		else flag = 0;

   return(flag);
}
