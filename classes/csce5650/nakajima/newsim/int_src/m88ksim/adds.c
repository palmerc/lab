/* @(#) File name: adds.c   Ver: 3.1   Cntl date: 1/20/89 14:26:16 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include "float.h"

void adds(ULONG asign,ULONG amant,ULONG bsign,ULONG bmant,
ULONG *resign,ULONG *resmant,ULONG s)
{

/* If same sign, add the numbers and set the sign to one of the signs */

   if (asign == bsign) {
      *resmant = amant + bmant;
      *resign = asign;
   }

/* If signs differ, bit invert the smaller number and use not sticky as */
/* carry in.  Set the sign bit to the sign of the larger mantissa.			*/

	 else if (amant >= bmant) {
			*resign = asign;
			*resmant = amant + (~bmant);
			if (!s)
				(*resmant)++;
	 }
	 else {
			*resign = bsign;
			*resmant = (~amant) + bmant;
			if (!s)
				(*resmant)++;
	 }
}

