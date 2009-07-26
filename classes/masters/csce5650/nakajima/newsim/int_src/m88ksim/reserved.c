/* @(#) File name: reserved.c   Ver: 3.1   Cntl date: 1/20/89 14:25:58 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

extern char naninf[];
extern char denorm[];


ULONG reserved(ULONG src1hi,ULONG src1lo,ULONG src2hi,ULONG src2lo,int opchoice)
{
   FPLONG err1,err2;
   ULONG ahi,alo;
   ULONG flag;

   flag = 0;

/* select proper operand */

	if (opchoice)
		{
		ahi = src2hi;
		alo = src2lo;
		}
	else
		{
		ahi = src1hi;
		alo = src1lo;
		}
/* Extract fields */

	err1.sign = BFEXTU(src1hi,31,1);
	err2.sign = BFEXTU(src2hi,31,1);
	err1.exp  = BFEXTU(src1hi,20,11);
	err2.exp  = BFEXTU(src2hi,20,11);
	err1.manthi  = BFEXTU(src1hi,0,20);
	err2.manthi  = BFEXTU(src2hi,0,20);
	err1.mantlo  = src1lo;
	err2.mantlo  = src2lo;

/* Check for Nan and Infinity and Denorm */

	if ((ahi & 0x7ff00000) == 0x7ff00000) 
		{
		if (((ahi & 0x000fffff) != 0) || (alo != 0))
			flag = NAN;
		else 
			flag = INF;
		floaterr(naninf,err1.sign,err2.sign,err1.exp,err2.exp,err1.manthi,err2.manthi,err1.mantlo,err2.mantlo,0,0,0,0,0);
		}
	else 
		if ((ahi & 0x7ff00000) == 0) 
			{
			if(((ahi & 0x000fffff) != 0) || (alo != 0)) 
				{
				flag = DENORM;
				floaterr(denorm,err1.sign,err2.sign,err1.exp,err2.exp,err1.manthi,err2.manthi,err1.mantlo,err2.mantlo,0,0,0,0,0);
				}
			}
   return(flag);
}
