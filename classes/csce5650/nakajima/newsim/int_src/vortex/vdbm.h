/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef VDBM01_H
#define  VDBM01_H

/* ENV - VDBM.h  */
/*+-----------------------------------------------------------------------+
                    ....       VDBM      ....
  +-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+
~P                           PROCEDURES                                   !
  +-----------------------------------------------------------------------+*/

/* PUBLIC  */
       boolean DbmNewVchunk   (tokentype    *Anchor,   sizetype     UnitSize,
                               indextype     UnitCount,indextype    ExtendQty, 
                               ft F,lt Z,zz *Status,  
                               indextype    *Vchunk,   addrtype    *ChunkAddr);

       boolean DbmPutVchunkTkn
                              (tokentype    *Token,  vchunktkntype *VchunkTkn,
                               ft F,lt Z,zz *Status);

       boolean DbmFreeVchunk  (tokentype    *Vtoken,  ft F,lt Z,zz *Status);

       boolean DbmDeleteVchunk
                              (tokentype    *Vtoken,  ft F,lt Z,zz *Status);

       boolean DbmCommitVchunk
                              (tokentype    *VchunkTkn, boolean     Release,
                               ft F,lt Z,zz *Status);




       boolean DbmInvokeVchunk
                              (tokentype    *Token,
                               ft F,lt Z,zz *Status, vchunktkntype *VchunkTkn);

       boolean DbmGetVchunkTkn
                              (tokentype    *Token, 
                               ft F,lt Z,zz *Status, vchunktkntype *VchunkTkn);


       boolean DbmDumpVchunkVchunk
                              (tokentype    *Token,   ft F,lt Z,zz *Status);

       boolean DbmDumpVchunk  (tokentype    *Token,
                               numtype       Limit0, numtype        Limit1,
                               ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF VDBM.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
