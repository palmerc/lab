/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef GDBM01_H
#define  GDBM01_H

/* ENV - GDBM.h  */
/*+-----------------------------------------------------------------------+
                    ....       GDBM      ....
  +-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+
~P                           PROCEDURES                                   !
  +-----------------------------------------------------------------------+*/

/* PUBLIC  */
/*+-----------------------------------------------------------------------+*
 ~L                           UP LOAD
 *+-----------------------------------------------------------------------+*/



/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/
       boolean DbmLoadGrpHdr  (tokentype    *DbToken, numtype       GrpNum,
                               ft F,lt Z,zz *Status,  grpheader   **GrpHdr);

       boolean DbmFileInGrpHdr(grpheader    *GrpHdr,  idtype        DbId,
                               blkhdrtype   *BlkHdr,  boolean       Release,
                               ft F,lt Z,zz *Status);

       boolean DbmFileInGrpRgnChunk 
                              (dbheader     *CoreDb,  grpheader    *GrpHdr,
                               indextype     RgnIndex,boolean       Release,
                               ft F,lt Z,zz *Status,  numtype      *RgnChunk);

       boolean DbmDeleteGrpHdr
                              (tokentype    *DbToken, numtype       GrpNum,
                               ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF GDBM.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
