/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - GRP.h  */

#ifndef GRP01_H
#define GRP01_H

/*+-----------------------------------------------------------------------+
                    ....       GRP       ....
  +-----------------------------------------------------------------------+*/
/*                    GENERIC REGION PACKET                                */

#ifndef HM00_H
typedef struct TypeGrpHdr  
{
     wordtype    EnvObjType;      

     accesstype  PacketAccess;    
     boolean     DiskCache;       

     indextype   GrpNum;          

     valueclass  UnitType;
     numtype     UnitObj;
     sizeinbytes EntrySize;       
     indextype   EntryCount;      

     numtype     RegionEntrys;    
     numtype     RegionCount;     
     numtype     RegionsAllocd;   

     indextype   RegionIndex;     
     boolean     NoRgnsInCore;    
     numtype     FreezePacket;    

                                  
     numtype     RegionChunkDir;  
     numtype     RegionSwapDir;   
     numtype     DirtyRegionDir;  
     numtype     RegionFreezeDir; 
     numtype     RgnBuffNumDir;   
     numtype     RgnAccessedDir;  

     numtype     RegionVbnDir;    
     vbntype     RegionVbnDirVbn;

     indextype   ExEntryCount;  
     numtype     ExEntryTbl;      
     vbntype     ExEntryTblVbn;  
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


}                         grpheader;
#endif


#define GrpMakeHdrChunks            Grp_MakeHdrChunks

#define GrpTestPacket               Grp_TestPacket
#define GrpNewPacket                Grp_NewPacket

#define GrpGetPacket                Grp_GetPacket

#define GrpSetPacketCache           Grp_SetPacketCache

#define GrpSetPacketAccess          Grp_SetPacketAccess


#define GrpFreezePacket             Grp_FreezePacket

#define GrpThawPacket               Grp_ThawPacket

#define GrpDefrostPacket            Grp_DefrostPacket

#define GrpFreePacket               Grp_FreePacket

#define GrpDeletePacket             Grp_DeletePacket

#define GrpGetRgnBuffer             Grp_GetRgnBuffer  

#define GrpSwapOutBuffRgn           Grp_SwapOutBuffRgn 

#define GrpNewRegion                Grp_NewRegion

#define GrpGetRegion                Grp_GetRegion

#define GrpCreateEntry              Grp_CreateEntry

#define GrpNewEntry                 Grp_NewEntry  

#define GrpPutNewEntry              Grp_PutNewEntry  

#define GrpPutEntry                 Grp_PutEntry

#define GrpGetEntry                 Grp_GetEntry

#define GrpDeleteEntry              Grp_DeleteEntry

#define GrpShowStats                Grp_ShowStats
#ifdef LARGE
#define GRP_EOL              -999999L
#else
#define GRP_EOL               6053
#endif

#ifdef GRP0_C
numtype   CacheAtRgnChunks  = 0; 
numtype   FileInRgnChunks   = 0; 
numtype   FileOutRgnChunks  = 0; 
numtype   CopyInRgnChunks   = 0; 
numtype   CopyOutRgnChunks  = 0; 
numtype   CleanRgnChunks    = 0; 
#else
#ifdef GRP_C
extern numtype   CacheAtRgnChunks; 
extern numtype   FileInRgnChunks; 
extern numtype   FileOutRgnChunks; 
extern numtype   CopyInRgnChunks; 
extern numtype   CopyOutRgnChunks; 
extern numtype   CleanRgnChunks; 
#endif
#endif
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....    protected       ....
   ....................................................................... */
/* ~   12.0.1                                                              */
       boolean Grp_MakeHdrChunks 
                         (tokentype    *Anchor,    grpheader    *GrpHdr,
                          numtype       AllocRgns, numtype       AllocXs,
                          ft F,lt Z,zz *Status);

/* ~   12.0.2                                                              */
       boolean Grp_GetPacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    grpheader   **GrpHdr);

#ifdef GRP_C

/* ~   12.0.5                                                              */
       boolean GrpNewRegion
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     Index,     indextype     RgnIndex,
                          ft F,lt Z,zz *Status,    
                          numtype      *RgnChunk,  addrtype     *RgnAddr);

/* ~   12.0.6                                                              */
       boolean Grp_GetRegion
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  ft F,lt Z,zz *Status,
                          numtype      *RgnChunk,  addrtype     *RgnAddr);


/* ~   12.0.9                                                              */
       boolean Grp_CleanRgnChunks
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status);

/* ~   12.0.10                                                             */
       boolean Grp_DeleteRgn    
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status);

/* ~   12.0.11                                                             */
       boolean Grp_RgnSwapTopAndPop
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status);
#endif /* GRP_C  */

/* .......................................................................
                    ....     General        ....
   ....................................................................... */
/* ~   12.1.1                                                              */
       boolean Grp_NewPacket
                         (tokentype    *Anchor,    sizetype      EntrySize,
                          numtype       AllocRgns, numtype       RgnEntrys,
                          numtype       AllocXs,   numtype       BaseIndex, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          numtype       BuffRgns,
                          ft F,lt Z,zz *Status,    numtype      *GrpNum,
                          grpheader   **GrpHdr,    numtype      *BaseRegion);

/* ~   12.1.2                                                              */
       boolean Grp_SetPacketCache
                         (tokentype    *Anchor,    numtype       GrpNum,
                          boolean       DiskCache, ft F,lt Z,zz *Status);

/* ~   12.1.3                                                              */
       boolean Grp_SetPacketAccess
                         (tokentype    *Anchor,    numtype       GrpNum,
                          accesstype    AccessType,ft F,lt Z,zz *Status);

/* ~   12.1.4                                                              */
       boolean Grp_FreezePacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.1.5                                                              */
       boolean Grp_ThawPacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.1.6                                                              */
       boolean Grp_DefrostPacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.1.7                                                              */
       boolean Grp_FreePacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.1.8                                                              */
       boolean Grp_CommitPacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          boolean       Release,   ft F,lt Z,zz *Status);

/* ~   12.1.9                                                              */
       boolean Grp_DeletePacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.1.10                                                             */
       boolean Grp_ShowStats
                         (tokentype    *Token,     numtype       GrpNum,
                          ft F,lt Z,zz *Status);
/* ~   12.1.11                                                             */
       boolean Grp_DumpStruct
                         (tokentype    *Token,     numtype       GrpNum,
                          ft F,lt Z,zz *Status);
/* ~   12.1.12                                                             */
       boolean Grp_DumpEntrys
                         (tokentype    *Token,     numtype       GrpNum,
                          indextype     Low,       indextype     High,
                          indextype     Limit,     dumpentryptr  DumpEntryPtr,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....     Grp Entry      ....
   ....................................................................... */

/* ~   12.2.0                                                              */
       boolean Grp_EntryCount  
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Count);

/* ~   12.2.1                                                              */
       boolean Grp_CreateEntry  
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Index);

/* ~   12.2.2                                                              */
       boolean Grp_NewEntry  
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Index,
                          addrtype      Entry);

/* ~   12.2.3                                                              */
       boolean Grp_PutNewEntry  
                         (tokentype    *Anchor,    numtype       GrpNum,
                          addrtype      Entry,
                          ft F,lt Z,zz *Status,    indextype    *Index);

/* ~   12.2.4                                                              */
       boolean Grp_PutEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     addrtype      Entry,
                          ft F,lt Z,zz *Status);

/* ~   12.2.5                                                              */
       boolean Grp_GetBaseRegion
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    addrtype     *BaseRgn);

/* ~   12.2.6                                                              */
       boolean Grp_GetEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype      Entry);

/* ~   12.2.6.1                                                            */
       boolean Grp_GetInLineEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *Entry);

/* ~   12.2.7                                                              */
       boolean Grp_GetFrozenEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *Entry);
/* ~   12.2.8.1                                                            */
       boolean Grp_FreezeEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status);

/* ~   12.2.8.2                                                            */
       boolean Grp_DefrostEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status);
/* ~   12.2.8.2                                                            */
       boolean Grp_EntryIsFrosted
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status);
/* ~   12.2.8.3                                                            */
       boolean Grp_FrostStatus
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status);

/* ~   12.2.9                                                              */
       boolean Grp_DeleteEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF GRP.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
