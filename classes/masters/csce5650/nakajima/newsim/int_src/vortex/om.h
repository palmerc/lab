/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OM.h  */
#ifndef OM_H
#define OM_H

/*+-----------------------------------------------------------------------+
~V                          VARIABLES                                     !
  +-----------------------------------------------------------------------+*/
#ifdef OM
#ifndef  ENV00
static boolean      Config_Class             = False;
static numtype      Class_RegionEntrys       = 1000;
static numtype      Class_AllocRegions       = 20;
static numtype      Class_ExtendRegions      = 10;
static numtype      Class_AllocExObjects     = 1000;
static numtype      Class_ExtendExObjects    = 100;
static accesstype   Class_AccessType         = Read_Write;
static boolean      Class_DiskCache          = False;
static indextype    Class_CacheLimit         = 20;
static indextype    Class_CacheLoad          = 20;
#endif
       tokentype    HdrSizeVchunkTkn         = {1, 1, 1};
       tokentype    AllocQtyVchunkTkn        = {2, 1, 1};
       tokentype    RegionQtyVchunkTkn       = {3, 1, 1};
       tokentype    SectorQtyVchunkTkn       = {4, 1, 1};

       bytessize   *ObjHdrSize               = NullPtr;
       indextype   *ObjAllocQty              = NullPtr;
       indextype   *ObjRegionQty             = NullPtr;
       indextype   *ObjSectorQty             = NullPtr;
#else


extern tokentype    HdrSizeVchunkTkn;
extern tokentype    AllocQtyVchunkTkn;
extern tokentype    RegionQtyVchunkTkn;
extern tokentype    SectorQtyVchunkTkn;

extern bytessize   *ObjHdrSize;
extern indextype   *ObjAllocQty;
extern indextype   *ObjRegionQty;
extern indextype   *ObjSectorQty;
#endif

#ifndef  ENV00
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
    void    OmSetConfig    (boolean       Config);

    void    OmConfigClass  (numtype       RgnEntrys,
                            numtype       AllocRgns, numtype       ExtendRgns,
                            numtype       AllocXs,   numtype       ExtendXs, 
                            accesstype    AccessType,boolean       DiskCache,
                            numtype       CacheLimit,numtype       CacheLoad);

    boolean OmNewObjHdr   (tokentype    *Anchor,    numtype       EnvObj,
                           indextype     AllocQty,
                           ft F,lt Z,zz *Status,    objheader   **ObjHdr);

    boolean OmMakeObjChunks
                          (tokentype    *Anchor,    objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,    indextype    *AlocObjects);

    boolean OmPutHandleOffset
                          (tokentype    *Anchor,    numtype       ObjNum,
                           bytessize     Offset,    ft F,lt Z,zz *Status);

    boolean OmGetObjHdr   (tokentype    *Anchor,    numtype       LocalObj,
                           ft F,lt Z,zz *Status,    objheader   **ObjHdr);


    boolean OmNewObject   (dbheader     *CoreDb,    objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,
                           indextype    *Index,     addrtype     *Object);

    boolean OmNextAvailObject
                          (objheader    *ObjHdr,    ft F,lt Z,zz *Status,
                           indextype    *Index,     indextype    *RgnIndex,
                           indextype    *LocalIndex);

    boolean OmNewObjRegion
                          (dbheader     *CoreDb,    objheader    *ObjHdr,
                           indextype     Index, 
                           ft F,lt Z,zz *Status,    numtype      *Region);

    boolean OmNewObjRgnChunks
                          (objheader    *ObjHdr, 
                           ft F,lt Z,zz *Status,    indextype    *RgnIndex);

    boolean OmGetRegion   (dbheader     *CoreDb,    objheader    *ObjHdr,
                           indextype     Region,
                           ft F,lt Z,zz *Status,    numtype      *Chunk);

    boolean OmPairRgnObjects
                          (dbheader     *CoreDb,    objheader    *ObjHdr,
                           numtype       RgnNum,    ft F,lt Z,zz *Status);

    boolean OmUnPairRgnObjects
                          (dbheader     *CoreDb,    objheader    *ObjHdr,
                           numtype       RgnNum,    ft F,lt Z,zz *Status);

    boolean OmGetObjHandles
                          (tokentype    *Anchor,    objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,  
                           indextype    *Count,     handletype  **Handles,
                           indextype    *RgnCount,  numtype     **RgnStatus);

    boolean OmPairDbObject
                          (objheader    *ObjHdr,    indextype     Index,
                           handletype    Handle,    ft F,lt Z,zz *Status);

    boolean OmGetObject   (tokentype    *Token,     ft F,lt Z,zz *Status,
                           indextype    *Index,     addrtype     *Object);

    boolean OmDeleteObject 
                          (tokentype    *Token,     objheader    *ObjHdr,
                           addrtype      Object,    ft F,lt Z,zz *Status);
 


    boolean OmChkImage    (tokentype    *Anchor,    numtype       ObjNum,
                           ft F,lt Z,zz *Status);


    boolean OmGetObjectHndls 
                          (tokentype    *Anchor,
                           objheader    *ObjHdr,    sizeinbytes   HndlOffset,
                           ft F,lt Z,zz *Status,    handletype   *ObjHndls);

#endif

/*+-----------------------------------------------------------------------+
~P                         END OF OM.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
