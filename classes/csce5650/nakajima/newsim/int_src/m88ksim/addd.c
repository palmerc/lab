/* @(#) File name: addd.c   Ver: 3.1   Cntl date: 1/20/89 14:26:12 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include "float.h"

void addd(ULONG asign,ULONG amanthi,ULONG amantlo,ULONG bsign,ULONG bmanthi,
ULONG bmantlo,ULONG *resign,ULONG *resmanthi,ULONG *resmantlo,ULONG s)
{

/* If same sign, add the numbers and set the sign to one of the signs */

   if (asign == bsign) {
      *resmanthi = amanthi + bmanthi;
      *resmantlo = amantlo + bmantlo;
      if ((*resmantlo < amantlo) || (*resmantlo < bmantlo))
	 (*resmanthi)++;
      *resign = asign;
   }

/* If signs differ, bit invert the negative number and use not sticky as */
/* carry in.  Set the sign bit depending on the add result.              */

   else if ((amanthi > bmanthi) || ((amanthi==bmanthi) && (amantlo >= bmantlo)))
   {
      *resign = asign;
      *resmantlo = amantlo + (~bmantlo);
      *resmanthi = amanthi + (~bmanthi);

      if ((*resmantlo < (~bmantlo)) || (*resmantlo < amantlo))
	 (*resmanthi)++;

      if (!s) {
	 (*resmantlo)++;
	 if (*resmantlo == 0)
	    (*resmanthi)++;
      }
   }
   else {
      *resign = bsign;
      *resmantlo = (~amantlo) + bmantlo;
      *resmanthi = (~amanthi) + bmanthi;

      if ((*resmantlo < (~amantlo)) || (*resmantlo < bmantlo))
	 (*resmanthi)++;

      if (!s) {
	 (*resmantlo)++;
	 if (*resmantlo == 0)
	    (*resmanthi)++;
      }
   }
}
