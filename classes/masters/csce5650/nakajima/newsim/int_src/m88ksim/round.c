/* @(#) File name: round.c   Ver: 3.1   Cntl date: 1/20/89 14:26:08 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

ULONG round(ULONG sign,ULONG l,ULONG g,ULONG r,ULONG s,ULONG rnd)
{

/* Determine rounding */

   if (sign) {
      if ((rnd == RN) && ((g && (r || s)) || (l && g))) {
	 return(1);
     }
      else if ((rnd == RM) && (g || r || s)) {
	 return(1);
     }
   }
   else {
      if ((rnd == RN) && ((g && (r || s)) || (l && g))) {
	 return(1);
      }
      else if ((rnd == RP) && (g || r || s)) {
	 return(1);
      }
   }
   return(0);
}
