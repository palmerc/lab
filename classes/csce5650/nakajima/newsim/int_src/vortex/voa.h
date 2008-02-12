/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - VOA.h  */
#ifndef VOA_H
#define VOA_H
 
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
       boolean OaCreateVchunk
                          (tokentype    *Anchor,    sizetype      UnitSize,
                           indextype     UnitCount, indextype     ExtendQty,
                           ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean OaFreezeVchunk
                          (tokentype    *Token,     ft F,lt Z,zz *Status);


       boolean OaDumpVchunkVchunk
                          (tokentype    *Token,     ft F,lt Z,zz *Status);

       boolean OaGetVchunkAddr
                          (tokentype    *Token,    
                           ft F,lt Z,zz *Status,    addrtype     *Chunk);

       boolean OaPutVchunkAddr
                          (tokentype    *Token,     addrtype      ChunkAddr,    
                           ft F,lt Z,zz *Status);

       boolean OaGetVchunkAllocQty 
                          (tokentype    *Token,
                           ft F,lt Z,zz *Status,    indextype    *AllocQty);

       boolean OaGetVchunkStackPtr
                          (tokentype    *Token,
                           ft F,lt Z,zz *Status,    indextype    *StackPtr);

       boolean OaPutVchunkStackPtr
                          (tokentype    *Token,     indextype     StackPtr,    
                           ft F,lt Z,zz *Status);

       boolean OaPushUnit
                          (tokentype    *Token,     addrtype      SomeBytes,
                           ft F,lt Z,zz *Status,    indextype    *StackPtr);

       boolean OaPutUnit
                          (tokentype    *Token,     indextype     Index,    
                           addrtype      SomeBytes, ft F,lt Z,zz *Status);

       boolean OaGetUnit
                          (tokentype    *Token,     indextype     Index,    
                           ft F,lt Z,zz *Status,    addrtype      SomeBytes);


       boolean OaDumpVchunk
                          (tokentype    *Token,     ft F,lt Z,zz *Status);

       boolean OaDeleteVchunk
                          (tokentype    *Token,     ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF VOA.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
