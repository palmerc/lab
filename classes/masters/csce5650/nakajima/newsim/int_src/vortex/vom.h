/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - VOM.c                                                              */

#ifndef VOM_H
#define VOM_H
/*+-----------------------------------------------------------------------+*
~OM                    VIRTUAL OBJECT MANAGER                             !
 *+-----------------------------------------------------------------------+*/



/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
       boolean OmNewVchunk
                          (tokentype    *Anchor,    sizetype      UnitSize,
                           indextype     UnitCount, indextype     ExtendQty,
                           ft F,lt Z,zz *Status,
                           indextype    *Vchunk,    addrtype     *ChunkAddr);

       boolean OmGetVchunkToken
                          (tokentype    *Token,     ft F,lt Z,zz *Status,
                           vchunktkntype           *VchunkToken);

       boolean OmGetVchunkStruc
                          (tokentype    *Token,     ft F,lt Z,zz *Status,
                           chunkstruc   *VchunkStruc);

       boolean OmDeleteVchunk
                          (tokentype    *Token,     ft F,lt Z,zz *Status);



/*+-----------------------------------------------------------------------+
~P                         END OF VOM.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
