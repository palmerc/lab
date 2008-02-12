/* @(#) File name: returnd.c   Ver: 3.1   Cntl date: 1/20/89 14:26:02 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

extern char overflow[];
extern char underflow[];


ULONG return_double(ULONG sign,int exp,ULONG manthi,ULONG mantlo,ULONG *desthi,
ULONG *destlo,ULONG rnd,ULONG grd,ULONG rd,ULONG stky)
{

/* If zero, return signed zero */

   if ((manthi == 0) && (mantlo == 0)) 
		{
      	*desthi = sign<<31;
      	*destlo = 0;
      	return(0);
   		}

/* If round caused overflow, increment the exponent */

   else if ((manthi & VBITMASKD) != 0) {
      exp++;
   }

/* Check for overflow or underflow in the result */

   if (exp >= MAXEXPD) 
		{
      	floaterr(overflow,sign,0,exp,0,manthi,0,mantlo,0,0,rnd,grd,rd,stky);
      	*desthi = 0;
      	*destlo = 0;
      	return(OVERFLOW);
   		}
   else 
		if (exp <= MINEXPD) 
			{
      		floaterr(underflow,sign,0,exp,0,manthi,0,mantlo,0,0,rnd,grd,rd,stky);
      		*desthi = 0;
      		*destlo = 0;
      		return(UNDERFLOW);
   			}

   *desthi = ((sign<<31) | ((exp & MAXEXPD)<<20) | (manthi & MANTMASKD));
   *destlo = mantlo;
   return(0);
}


