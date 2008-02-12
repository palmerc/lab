/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef ODBM01_H
#define  ODBM01_H

/*+-----------------------------------------------------------------------+
                    ....       ODBM      ....
  +-----------------------------------------------------------------------+*/
#ifdef ODBM
        booleantype   PairObjects    = True;
        booleantype   UnPairObjects  = True;
#else
extern  booleantype   PairObjects;
extern  booleantype   UnPairObjects;
#endif


/* ENV - ODBM.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
typedef struct TypeObjHeader
{ 
     /* This portion of struct is common to both GRP and ObjHdr           */

     wordtype    EnvObjType;      
                                  
     accesstype  LocalAccess;     
     boolean     DiskCache;       

     indextype   LocalObjNum;     

     valueclass  UnitType;
     numtype     UnitObj;
     sizeinbytes ObjSize;         
     indextype   ObjectCount;     

     numtype     RegionObjects;   
     numtype     RegionCount;     
     numtype     RegionsAllocd;   

     indextype   RegionIndex;     
     boolean     NoRegionsInCore; 
     numtype     FreezeClass;     

                                  
     numtype     RegionChunkDir;  
     numtype     RegionSwapDir;   
     numtype     DirtyRegionDir;  
     numtype     RegionFreezeDir; 
     numtype     RgnBuffNumDir;   
     numtype     RgnAccessedDir;  

     numtype     RegionVbnDir;    
     vbntype     RegionVbnDirVbn;

     indextype   ExObjectCount;
     numtype     ExObjectTbl;     
     vbntype     ExObjectTblVbn;
     numtype     NextEx;          

     indextype   NextEntry;       
     indextype   BaseIndex;       
                                  
     numtype     BaseRegion;      

     indextype   CacheLimit;      
     indextype   CacheLoad;       

     numtype     NumBuffRgns;     
     indextype   RgnBuffCount;    

     numtype     BuffRgnsDir;     
     numtype     BuffNodesDir;    

     indextype   HeadRgnBuffs;    
     indextype   LastRgnBuff;     

     indextype   RgnStatusDir;    
     indextype   RgnStatusDirVbn; 

     numtype     RgnBuffIdsDir;

     indextype   GrpIndexDir;     
     indextype   GrpIndexDirVbn;  
     indextype   GrpDirCount;     
     lt64        HeadExIndexs;    

     numtype    *FrozenRgns;      
     numtype    *RgnBuffNums;     
     numtype    *RgnAccessArray;  

     indextype  *RgnBuffRgns;     
     indextype  *RgnBuffNodes;    

     indextype  *RgnStats;        

     indextype  *RgnBuffIds;

     lt64       *GrpDirAddr;      

#ifndef __RISC_64__
     numtype    *FrozenRgns_64;
     numtype    *RgnBuffNums_64;
     numtype    *RgnAccessArray_64;

     indextype  *RgnBuffRgns_64;
     indextype  *RgnBuffNodes_64;

     indextype  *RgnStats_64;

     indextype  *RgnBuffIds_64;

     lt64       *GrpDirAddr_64;
#endif /* __RISC_64__ */

     indextype   ObjectLockDir;   
     indextype   ObjectLockDirVbn;

     sizeinbytes PsudoSize;       

     numtype     ObjNum;          

     indextype   NextObject;      
                                  

     SWordType   HandleOffset;    

     numtype     FieldNumMap;     
     vbntype     FieldNumMapVbn;
     numtype     LocalAttrNumMap; 
     vbntype     LocalAttrNumMapVbn;
 
     indextype   FieldCount;      
     numtype     FieldOffsetDir;  
     vbntype     FieldOffsetDirVbn;
     numtype     ClassOffsetDir;  
     vbntype     ClassOffsetDirVbn;

     indextype   FieldRefCount;   
     numtype     FieldRefDir;     
     vbntype     FieldRefDirVbn;

     indextype   FieldMapCount;   
     numtype     FieldMapDir;     
     vbntype     FieldMapDirVbn;

     indextype   FieldArrayCount; 
     numtype     FieldArrayDir;   
     vbntype     FieldArrayDirVbn;
 
     indextype   StringCount;     
     indextype   TextChunk;       
     vbntype     TextChunkVbn;

#ifdef STRUC_ALIGN_BY8
     wordtype    FieldExtender08;
#endif /* STRUC_ALIGN_BY8  */

}                         objheader;

/*+-----------------------------------------------------------------------+
~P                            PROCEDURES                                  !
  +-----------------------------------------------------------------------+*/

/* PUBLIC  */

/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/

       boolean DbmLoadObjHdr  (tokentype    *DbToken, numtype       LocalObj,
                               ft F,lt Z,zz *Status,  objheader   **ObjHdr);

       boolean DbmFileInObjHdr(objheader    *ObjHdr,  idtype        DbId,
                               blkhdrtype   *BlkHdr,  boolean       Release,
                               ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+*
 ~L                          REGIONS  
 *+-----------------------------------------------------------------------+*/
       boolean DbmFileInRegionChunk
                              (dbheader     *CoreDb,  objheader    *ObjHdr,
                               indextype     RgnIndex,boolean       Release,
                               ft F,lt Z,zz *Status,  numtype      *RgnChunk);


       boolean DbmPairRgnObjects
                              (dbheader     *CoreDb,  objheader    *ObjHdr,
                               numtype  CurrRgnChunk, ft F,lt Z,zz *Status);


       boolean DbmUnPairRgnObjects
                              (dbheader     *CoreDb,  objheader    *ObjHdr,
                               numtype  CurrRgnChunk, ft F,lt Z,zz *Status);


       void    DbmDumpObjHdr  (int          TraceSwi,
                               objheader   *ObjHdr,   ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF ODBM.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
