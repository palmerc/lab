/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - DBM2.c */

#define DBM_C
#define DBM2
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"

 
/*+-----------------------------------------------------------------------+*
~DBM                           DBM10
 *+-----------------------------------------------------------------------+*/

boolean Dbm_FreeDb      (tokentype    *DbToken,  ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+*
~DBM                           1->0  
 *+-----------------------------------------------------------------------+*/
boolean Dbm_FreeDb      (tokentype    *DbToken,  ft F,lt Z,zz *Status)
{
   *Status  = Err_NotImplemented;

TRACK(TrackBak,"DbmFreeDb\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+
~M                         END OF DBM2.C                                  !
  +-----------------------------------------------------------------------+*/
