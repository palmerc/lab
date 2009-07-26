/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OA0.h  */
#ifndef OA0_H
#define OA0_H

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
       boolean OaCreateDb (tokentype    *SchemaTkn, numtype       PrimalObj,
                           char         *DbName,    char         *NewFileName,
                           dbaccesstype  DbAccess,
                           ft F,lt Z,zz *Status,    tokentype    *DbPrimal);

       boolean OaInstallObjHdr
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status,    objheader   **ObjHdr);

       boolean OaGetObjClass
                          (tokentype    *Token,
                           ft F,lt Z,zz *Status,    numtype      *ObjNum);

       boolean OaConfigClass  
                          (tokentype    *PrimalTkn, numtype       ClassId,
                           numtype       RgnEntrys,
                           numtype       AllocRgns, numtype       ExtendRgns,
                           numtype       AllocXs,   numtype       ExtendXs, 
                           accesstype    AccessType,boolean       DiskCache,
                           numtype       CacheLimit,numtype       CacheLoad,
                           ft F,lt Z,zz *Status);

       boolean OaFreezeObjClass
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status);
       boolean OaThawObjClass
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status);

       boolean OaCreateObject
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean OaNewObject
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status,    indextype    *Index,
                           addrtype     *Object);

       boolean OaGetObject
                          (tokentype    *Token,
                           ft F,lt Z,zz *Status,    addrtype     *Object);

       boolean OaRevokeCppObject
                          (tokentype    *Token,     ft F,lt Z,zz *Status);

       boolean OaInvokeCppObject 
                          (tokentype    *Token,     addrtype      Object,
                           ft F,lt Z,zz *Status);

       boolean OaUpdateObject
                          (tokentype    *Token,     ft F,lt Z,zz *Status);

       boolean OaChkImage (tokentype    *Token,     ft F,lt Z,zz *Status);



       boolean OaFreeObject
                          (tokentype    *Token,     ft F,lt Z,zz *Status);


       boolean OaDeleteObject
                          (tokentype    *Token,     ft F,lt Z,zz *Status);


       boolean OaGetObjHandles
                          (tokentype    *Anchor,    numtype       ObjNum,
                           ft F,lt Z,zz *Status,  
                           indextype    *Count,     handletype  **Handles);

       boolean OaReclaimHandles
                          (tokentype    *Anchor,    numtype       ObjNum,
                           ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF OA0.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
