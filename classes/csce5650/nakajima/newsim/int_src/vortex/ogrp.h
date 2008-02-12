/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OGRP.h  */

#ifndef OGRP01_H
#define OGRP01_H

/*+-----------------------------------------------------------------------+
                    ....      OGRP       ....
  +-----------------------------------------------------------------------+*/
/*                    GENERIC REGION PACKET                                */


#define GrpNewObject                Grp_NewObject

#define GrpGetObject                Grp_GetObject

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
       boolean Grp_NewObject
                         (dbheader     *CoreDb,    objheader    *ObjHdr,
                          ft F,lt Z,zz *Status,    indextype    *Index,
                          numtype      *RgnIndex,  addrtype     *RgnAddr);
       boolean Grp_GetObject
                         (dbheader     *CoreDb,    objheader    *ObjHdr,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *RgnAddr);

	boolean Grp_DeleteObject
                         (dbheader     *CoreDb,    objheader    *ObjHdr,
                          indextype     Index,     ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF OGRP.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
