/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef HM00_H
#define HM00_H

/* ENV - HM.h  */
/*+-----------------------------------------------------------------------+
                    ....        HM       ....
  +-----------------------------------------------------------------------+*/
/*                       HANDLE MANAGER                                    */

#define HmDRIswi                 Hm_DRIswi
#define HmSetDefaults            Hm_SetDefaults
#define HmGetDefaults            Hm_GetDefaults 
#define HmMakeDbHdr              Hm_MakeDbHdr       

#define HmGetObjNum              Hm_GetObjNum       
#define HmIncrementMemRef        Hm_IncrementMemRef 
#define HmDecrementMemRef        Hm_DecrementMemRef 

#define HmNewHandle              Hm_NewHandle       
#define HmPairDbObject           Hm_PairDbObject    
#define HmFetchDbObject          Hm_FetchDbObject   
#define HmGetStrObjectIndex      Hm_GetStrObjectIndex
#define HmClearObject            Hm_ClearObject     
#define HmPointToInnerRealm      Hm_PointToInnerRealm
#define HmFreeHandle             Hm_FreeHandle      
#define HmNextFreeHandle         Hm_NextFreeHandle  
#define HmDumpDbHdr              Hm_DumpDbHdr

/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/

typedef struct TypeBlkHdr
{
    filetokentype BlkFile;
#ifdef STRUC_ALIGN_BY8
    filetokentype BlkFile_64;
#endif /* STRUC_ALIGN_BY8 */

    idtype        DbId;
    idtype        CoreDbId;

    numtype       HeadFreeVbns;
    numtype       BlkDirChunk;

    numtype       HeadFreeBlks;
    numtype       HeadNullBlks;

    numtype       BlkTknChunk;
    numtype       BlkSizeChunk;

    farlongtype   EofMark;
}                     blkhdrtype;
 
 
typedef struct TypeVchunkTkn
{
    numtype      InCore;
    vbntype      OnDisk;
}                     vchunktkntype;
 
typedef struct TypeTextTkn 
{
    numtype      InCore;
    vbntype      OnDisk;
}                     texttkntype;

typedef struct TypeMapRefTkn
{
    numtype      FieldNum;
    numtype      LocalObj;
}                     maptkntype;
 
 
typedef  numtype handlechunk;
typedef  num     regionflag[2];

/*+-----------------------------------------------------------------------+
~C                           GRP Header                                   !
  +-----------------------------------------------------------------------+*/
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

/*+-----------------------------------------------------------------------+
~C                            DB Header                                   !
  +-----------------------------------------------------------------------+*/
typedef struct TypeDbHeader
{
     idtype      DbId;            
     idtype      CoreDbId;

     indextype   NextHandle;      
     indextype   HandleExQty;     
     indextype   HandleUnits;     
 
     filename    DbFileName;      
     filename    ActiveFileName;  
     accesstype  LocalAccess;     
     boolean     DiskCache;       

     blkhdrtype  BlkHdr;          
                                  

     indextype   GrpCount;        
     numtype     GrpHdrDir;       
     vbntype     GrpHdrDirVbn;
     vbntype     GrpVbnDir;       
     vbntype     GrpVbnDirVbn;
     indextype   HeadFreeGrps;    

     numtype     VchunkCommitDir; 
     numtype     VchunkSwapDir;   

     vbntype     VbnDeleteDir;    
     vbntype     FreeVchunkVbn;   

     indextype   VchunkGrpNum;    
     indextype   VchunkBaseIndex; 

     indextype   HandleCount;     
     indextype   HandleBaseIndex; 

     numtype     ObjectAddrDir;   
     vbntype     ObjectAddrDirVbn;
     farlongtype ObjectAddrDirOffset;
     indextype   ObjectAddrGrpNum;

     numtype     ObjectMemLocDir; 

     numtype     ObjectMemRefDir; 

     numtype     ObjectIndexDir;  
     vbntype     ObjectIndexDirVbn; 
     farlongtype ObjectIndexDirOffset;
     indextype   ObjectIndexGrpNum;

     numtype     LocalObjNumDir;
     vbntype     LocalObjNumDirVbn;
     farlongtype LocalObjNumDirOffset;
     indextype   LocalObjNumGrpNum;

     indextype   LastEnvObjCount; 

     numtype     LocalObjNumMap;
     vbntype     LocalObjNumMapVbn;

     numtype     ObjHandleDir;    
     numtype     ObjHandleDirVbn; 

     indextype   LocalObjCount;   
     numtype     ObjNumMap;       
     vbntype     ObjNumMapDirVbn;
 
     numtype     ObjTimeStampDir; 
                                  
                                  
                                  
     numtype     ObjTimeStampVbn;

     numtype     ObjHdrDir;       
     vbntype     ObjHdrDirVbn;
     numtype     ObjVbnDir;       
     vbntype     ObjVbnDirVbn;

     indextype   LastEnvAttrCount;
     numtype     LocalAttrNumMap; 
     vbntype     LocalAttrMapVbn;

     numtype     AttrHandleDir;   
                                  
     vbntype     AttrHandleDirVbn;

     indextype   LocalAttrCount;  
     numtype     AttrNumMap;      
     vbntype     AttrNumMapVbn;
 
     numtype     LocalAttrSizeTbl;
     vbntype     LocalAttrSizeVbn;
 

     numtype     TextTokenDir;    
     numtype     TextTknDirVbn;   

     numtype     FreeTxtTknStack; 
     vbntype     FreeTxtTknVbn;   
 
     indextype   FutureHandles;   

     indextype   CurrentHandles;  

     indextype   AvailHandles;    
     numtype     NewHandles;      
     vbntype     NewHandlesVbn;
     indextype   NextFree;        
 

     numtype     StackHeadDir;    
     handletype  StackNames;      

     numtype     ListHeadDir;     
     handletype  ListNames;       

     numtype     TblHeadDir;      
     handletype  TblNames;        

     numtype     TreeHeadDir;     
     handletype  TreeNames;       

     numtype     lKeyHeadDir;     
     handletype  lKeyNames;       

     numtype     MtrxHeadDir;     
     handletype  MtrxNames;       


     sizetype    PrimalHdrSize;   
     vbntype     PrimalHdrVbn;    

     vbntype     EOFVbn;          

     indextype   BlkBlksBaseIndex;
     indextype   BlkBlksExtendQty;

     vbntype     BlkDirVbn;       
     indextype   BlkDirUnits;     
     farlongtype BlkDirOffset;    
     indextype   BlkDirGrpNum;    

     vbntype     BlkTknVbn;
     indextype   BlkTknUnits;
     farlongtype BlkTknOffset;
     indextype   BlkTknGrpNum;    

     vbntype     BlkSizeVbn;
     farlongtype BlkSizeOffset;
     indextype   BlkSizeGrpNum;   


     indextype   FreeRegionCount; 
     numtype     FreeRegionDir;   
     numtype     FreeRegionLengths;

     handletype  SchemaHndl;

     addrtype    PrimalHdr;       
     grpheader **GrpHdrs;         
     grpheader  *VchunkGrpHdr;    

#ifndef __RISC_64__
     addrtype    PrimalHdr_64;
     grpheader **GrpHdrs_64;
     grpheader  *VchunkGrpHdr_64;
#endif /* __RISC_64__ */

}                          dbheader;
typedef dbheader           coredb;
 
 
typedef struct CoreObjType
{
     num         ObjNum;
     boolean     (*CreateCode)(void);
}              coreobj;
typedef coreobj            coreobject;
 
typedef struct KernelCore
{
     coredb      *CoreDbs;
	  coreobject  *CoreObjs;
}                          corekernel;
 

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
#ifdef HM
        corekernel  *Kernel              = NullPtr;

        idtype       CoreDbCount         = 0;

        idtype       MaxCoreDbId         = 99;

        dbheader    *CoreDbs[100]        = {
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr,
             NullPtr,NullPtr,NullPtr,NullPtr,NullPtr
                                           };

        dbheader    *CoreDb0             = NullPtr;
        dbheader    *CoreDb1             = NullPtr;

        boolean      SwapHandles         = False;
        handletype   SwapHandleUnits     = 0;

        handletype   CoreHandles         = 0;

static  boolean      DRI_Env             = False;

static  handletype   VoidHandle          = 0;

static  indextype    HM_EnvObjs          = Env_Objs;
static  indextype    HM_EnvAttrs         = Env_Attrs;


static  indextype    HM_AllocDbObjs      = Alloc_DbObjs;
static  indextype    HM_ExtendDbObjs     = Extend_DbObjs;

static  indextype    HM_AllocDbAttrs     = Alloc_Attrs;
static  indextype    HM_ExtendDbAttrs    = Extend_DbAttrNums;

/*
static  indextype    HM_ExtendDbObjNums  = Extend_DbObjNums;
static  indextype    HM_ExtendDbAttrNums = Extend_DbAttrNums;
*/

static  indextype    HM_AllocHandles     = 0;  /*  Alloc_Handles         */
static  indextype    HM_ExtendHandles    = 0;  /*  Extend_Handles        */

static  indextype    HM_AllocVchunks     = 0;  /*  Alloc_Vchunks         */
static  indextype    HM_ExtendVchunks    = 0;  /*  Extend_Vchunks        */

static  indextype    HM_AllocGrps        = Alloc_Grps;
static  indextype    HM_ExtendGrps       = Extend_Grps;

static  indextype    HM_AllocTxtTkns     = Alloc_TxtTkns;
static  indextype    HM_ExtendTxtTkns    = Extend_TxtTkns;

static  indextype    HM_AllocDbXmemBlks  = 0;  /* DbXmemBlocks_AllocQty  */
static  indextype    HM_ExtendDbXmemBlks = 0;  /* DbXmemBlocks_ExtendQty */

extern  indextype    Alloc_Handles;
extern  indextype    Extend_Handles;

extern  indextype    Alloc_Vchunks;
extern  indextype    Extend_Vchunks;
extern  indextype    DbXmemBlocks_AllocQty;
extern  indextype    DbXmemBlocks_ExtendQty; 

#else
extern  indextype    Alloc_Handles;
extern  indextype    Extend_Handles;

extern  corekernel  *Kernel;
extern  idtype       CoreDbCount;
extern  idtype       MaxCoreDbId;
extern  dbheader    *CoreDbs[];
extern  dbheader    *CoreDb0;
extern  dbheader    *CoreDb1;
extern  boolean      SwapHandles;
extern  handletype   SwapHandleUnits;
extern  handletype   CoreHandles;


#endif
 
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/

   void    Hm_SetDRIswi  (boolean       DRI_Env);
   boolean HmDRIswi      (void);

   void    HmSetDefaults (indextype     EnvObjs,   indextype     EnvAttrs,  
                          indextype     AllocObjs, indextype     ExtendObjs,
                          indextype     AllocAttrs,indextype     ExtendAttrs,
                          indextype     AllocHndls,indextype     ExtendHndls,
                          indextype     AllocVchunks,indextype   ExtendVchunks, 
                          indextype     AllocGrps, indextype     ExtendGrps, 
                          indextype     AllocTexts,indextype     ExtendTexts, 
                          indextype     AllocXblks,indextype     ExtendXblks);

   void    HmGetDefaults (indextype    *EnvObjs,   indextype    *EnvAttrs,  
                          indextype    *AllocObjs, indextype    *ExtendObjs,
                          indextype    *AllocAttrs,indextype    *ExtendAttrs,
                          indextype    *AllocHndls,indextype    *ExtendHndls,
                          indextype    *AllocVchunks,indextype  *ExtendVchunks, 
                          indextype    *AllocGrps, indextype    *ExtendGrps, 
                          indextype    *AllocTexts,indextype    *ExtendTexts, 
                          indextype    *AllocXblks,indextype    *ExtendXblks);

   boolean HmMakeDbHdr       (indextype     EnvObjs,  indextype     EnvAttrs,  
                              idtype        CoreDbId,
                              ft F,lt Z,zz *Status,   dbheader    **CoreDb);

#ifdef __MACROIZE_HM__

#define HmPushLocalObjNum(CoreDb, LocalObj, Stat,   StackPtr)  \
   MemPushWord (CoreDb->LocalObjNumDir, LocalObj, Stat,   StackPtr)

#define HmGetLocalObjNum(CoreDb, Handle,   Stat,   LocalObj)  \
   MemGetWord (CoreDb->LocalObjNumDir, Handle,   Stat,   LocalObj)

#define HmPutLocalObjNum(CoreDb, Handle,   LocalObj, Stat)  \
   MemPutWord (CoreDb->LocalObjNumDir, Handle,   LocalObj, Stat)


#define HmPutObjNum(CoreDb, LocalObj, EnvObjNum, Stat)  \
   MemPutWord (CoreDb->ObjNumMap, LocalObj, EnvObjNum, Stat)


#define HmPushObjectAddr(CoreDb, ObjAddr,  Stat,   StackPtr)  \
   MemPushAddr (CoreDb->ObjectAddrDir, ObjAddr,  Stat,   StackPtr)

#define HmPutObjectAddr(CoreDb,  Handle,   ObjAddr,  Stat)  \
   MemPutAddr (CoreDb->ObjectAddrDir,  Handle,   ObjAddr,  Stat)

#define HmGetObjectAddr(CoreDb,  Handle,   Stat,   ObjAddr)  \
   MemGetAddr  (CoreDb->ObjectAddrDir,  Handle,   Stat,   ObjAddr)


#define HmPushObjectMemLoc(CoreDb, MemLoc,   Stat,   StackPtr)  \
   MemPushBit (CoreDb->ObjectMemLocDir, MemLoc,   Stat,   StackPtr)

#define HmPutObjectMemLoc(CoreDb, Handle,    MemLoc,   Stat)  \
   MemPutBit (CoreDb->ObjectMemLocDir, Handle,    MemLoc,   Stat)

#define HmGetObjectMemLoc(CoreDb, Handle,   Stat,   MemLoc)  \
   MemGetBit (CoreDb->ObjectMemLocDir, Handle,   Stat,   MemLoc)
                                                                  
#define HmPushObjectMemRef(CoreDb, MemRef,  Stat,  StackPtr)  \
   MemPushShort(CoreDb->ObjectMemRefDir, MemRef,  Stat,  StackPtr)


#define HmPutObjectMemRef(CoreDb, Handle, MemRef,   Stat)  \
   MemPutShort(CoreDb->ObjectMemRefDir, Handle, MemRef,   Stat)

#define HmGetObjectMemRef(CoreDb, Handle,  Stat,   MemRef)  \
   MemGetShort(CoreDb->ObjectMemRefDir, Handle,  Stat,   MemRef)
                                                                  

#define HmPushObjectIndex(CoreDb, ObjIndex, Stat,   StackPtr)  \
   MemPushWord(CoreDb->ObjectIndexDir, ObjIndex, Stat,   StackPtr)

#define HmPutObjectIndex(CoreDb, Handle, ObjIndex, Stat)  \
   MemPutWord (CoreDb->ObjectIndexDir, Handle, ObjIndex, Stat)

#define HmGetObjectIndex(CoreDb, Handle,  Stat,   ObjIndex)  \
   MemGetWord (CoreDb->ObjectIndexDir, Handle,  Stat,   ObjIndex)

#define HmInvokeObject(CoreDb, Handle,  Object,   Stat)  \
   MemPutAddr (CoreDb->ObjectAddrDir, Handle,  Object,   Stat)

#define HmPointToOuterRegion(CoreDb, Handle,  Stat)  \
   MemPutBit(CoreDb->ObjectMemLocDir, Handle,  1, Stat)


#else

#define HmPushLocalObjNum        Hm_PushLocalObjNum 
#define HmGetLocalObjNum         Hm_GetLocalObjNum  
#define HmPutLocalObjNum         Hm_PutLocalObjNum  

#define HmPutObjNum              Hm_PutObjNum       

#define HmPushObjectAddr         Hm_PushObjectAddr  
#define HmPutObjectAddr          Hm_PutObjectAddr  
#define HmGetObjectAddr          Hm_GetObjectAddr  

#define HmPushObjectMemLoc       Hm_PushObjectMemLoc
#define HmPutObjectMemLoc        Hm_PutObjectMemLoc 
#define HmGetObjectMemLoc        Hm_GetObjectMemLoc 

#define HmPushObjectMemRef       Hm_PushObjectMemRef
#define HmPutObjectMemRef        Hm_PutObjectMemRef 
#define HmGetObjectMemRef        Hm_GetObjectMemRef 

#define HmPushObjectIndex        Hm_PushObjectIndex 
#define HmPutObjectIndex         Hm_PutObjectIndex  
#define HmGetObjectIndex         Hm_GetObjectIndex  

#define HmInvokeObject           Hm_InvokeObject    
#define HmPointToOuterRegion     Hm_PointToOuterRegion

   boolean HmPushLocalObjNum (dbheader     *CoreDb,   localnumtype  LocalObj,
                              ft F,lt Z,zz *Status,   indextype    *StackPtr);

   boolean HmGetLocalObjNum  (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   localnumtype *LocalObj);

   boolean HmPutLocalObjNum  (dbheader     *CoreDb,   handletype    Handle,
                              localnumtype  LocalObj, ft F,lt Z,zz *Status);

   boolean HmPutObjNum       (dbheader     *CoreDb,   indextype     LocalObj,
                              numtype       EnvObjNum,ft F,lt Z,zz *Status);

   boolean HmPushObjectAddr  (dbheader     *CoreDb,   addrtype      ObjAddr,
                              ft F,lt Z,zz *Status,   indextype    *StackPtr);

   boolean HmPutObjectAddr  (dbheader      *CoreDb,   handletype    Handle,
                              addrtype      ObjAddr,  ft F,lt Z,zz *Status);

   boolean HmGetObjectAddr  (dbheader      *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   addrtype     *ObjAddr);

   boolean HmPushObjectMemLoc(dbheader     *CoreDb,   numtype       MemLoc,
                              ft F,lt Z,zz *Status,   indextype    *StackPtr);

   boolean HmPutObjectMemLoc (dbheader     *CoreDb,   handletype    Handle,
                              numtype       MemLoc,   ft F,lt Z,zz *Status);

   boolean HmGetObjectMemLoc (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   numtype      *MemLoc);
                                                                  
   boolean HmPushObjectMemRef(dbheader     *CoreDb,   shorttype     MemRef,
                              ft F,lt Z,zz *Status,   indextype    *StackPtr);

   boolean HmPutObjectMemRef (dbheader     *CoreDb,   handletype    Handle,
                              shorttype     MemRef,   ft F,lt Z,zz *Status);

   boolean HmGetObjectMemRef (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   shorttype    *MemRef);
                                                                  

   boolean HmPushObjectIndex (dbheader    *CoreDb,    indextype     ObjIndex,
                              ft F,lt Z,zz *Status,   indextype    *StackPtr);

   boolean HmPutObjectIndex  (dbheader     *CoreDb,   handletype    Handle,
                              indextype     ObjIndex, ft F,lt Z,zz *Status);

   boolean HmGetObjectIndex  (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   indextype    *ObjIndex);

   boolean HmInvokeObject    (dbheader     *CoreDb,   handletype    Handle,
                              addrtype      Object,   ft F,lt Z,zz *Status);

   boolean HmPointToOuterRegion
                             (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status);
#endif /* __MACROIZE_HM__ */

   boolean HmGetObjNum       (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   numtype      *EnvObjNum);

   boolean HmIncrementMemRef (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status);

   boolean HmDecrementMemRef (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   shorttype    *MemRef);


   boolean HmNewHandle       (dbheader     *CoreDb,   localnumtype  LocalObj,
                              ft F,lt Z,zz *Status,   handletype   *Handle);

   boolean HmPairDbObject    (dbheader     *CoreDb,   handletype    Handle,
                              indextype     Index,    addrtype      Object,
                              ft F,lt Z,zz *Status);

   boolean HmFetchDbObject   (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status,   addrtype     *Object);

   boolean HmGetStrObjectIndex(dbheader    *CoreDb,   strhandle     StrHandle,
                              ft F,lt Z,zz *Status,   indextype    *StrIndex);

   boolean HmClearObject     (dbheader     *CoreDb,   handletype    Handle,
                              ft F,lt Z,zz *Status);

   boolean HmPointToInnerRealm
                             (dbheader     *CoreDb,   handletype   Handle,
                              ft F,lt Z,zz *Status);

   boolean HmFreeHandle      (dbheader     *CoreDb,   handletype   Handle,
                              ft F,lt Z,zz *Status);

   boolean HmNextFreeHandle  (dbheader     *CoreDb, 
                              ft F,lt Z,zz *Status,   handletype  *Handle);

   boolean HmReclaimHandles  (dbheader     *CoreDb, ft F,lt Z,zz *Status);

   boolean HmDumpDbHdr       (int           TraceSwi, tokentype   *Anchor,
                              ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                          END OF HM.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
