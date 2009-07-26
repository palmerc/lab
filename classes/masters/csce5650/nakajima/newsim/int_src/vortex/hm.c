/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - HM.c */
#define HM
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "core01.h"
#include "mem010.h"


#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...MEM00... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[24] =
{
   " NEW  Db Header  :: DbPrimal[xx:%4u] @ %x\n",
   "    Hm_Push LocalObjNum  for Handle[%u];  = %u\n",
   "    Hm_Put LocalObjNum   for Handle[%u];  = %u\n",
   "    Hm_Get LocalObjNum   for Handle[%u];  = %u\n",
   " HmPutObjNum DbId[%2u] LocalObj[%3u] = %3u\n",
   " HmGetObjNum for Token[%2u:%4u] = %3u\n",
   "    Hm_Push ObjectAddr for Handle[%u];  = %x\n",
   "    Hm_Put ObjectAddr for Handle[%u];  = %x\n",
   "    Hm_Get ObjectAddr(<=%5u) for Handle[%u]; = %x\n",
   "    Hm_Push ObjectMemLoc for Handle[%u];  = %1u\n",
   "    Hm_Put ObjectMemLoc for Handle[%u];  = %1u\n",
   "    Hm_Get ObjectMemLoc(<=%5u) for Handle[%u]; = %1u\n",
   "    Hm_Push ObjectIndex  for Handle[%u]; = %u\n",
   "    Hm_Put ObjectIndex  for Handle[%u]; = %u\n",
   "    Hm_Get ObjectIndex  for Handle[%u]; = %u\n",
   "    NEW Handle : %d\n",
   "  HmPairDbObject (CoreDb @%x, Hndl: %4u, i= %4u, Obj @%x, Stat= %d)\n",
   "  HmFetchDbObject (coreDb @%x, Handle=%4u, %2u, %x)\n",
   "   HmFetchDbObject BADHandle[%4u] Object @%x ->Handle= %4u; McStat=%d\n",
   "  HmClearObject [@%x:%4u]\n",
   "  HmInvokeObject [@%x:%4u] to CoreAddr @%x\n",
   "  HmPointToFreeStore [@%x:%4u] \n",
   "  HmPointToInnerRealm [@%x:%4u] = %2u\n",
   "  HmFreeHandle:: [@%x:%4u]\n",
};
#endif

extern boolean TmFetchCoreDb
                         (tokentype    *Anchor,   
                          ft F,lt Z,zz *Status,     dbheader   **CoreDb);

#define MemPutHandle         Mem_PutWord
#define MemGetHandle         Mem_GetWord
 
#define MemPushLocalObjNum   Mem_PushWord
#define MemPutLocalObjNum    Mem_PutWord
#define MemGetLocalObjNum    Mem_GetWord
 
#define MemPushObjectIndex   Mem_PushWord
#define MemPutObjectIndex    Mem_PutWord
#define MemGetObjectIndex    Mem_GetWord
 
#define MemPushObjectAddr    Mem_PushAddr
#define MemPutObjectAddr     Mem_PutAddr
#define MemGetObjectAddr     Mem_GetAddr
#define MemGetObject         Mem_GetAddr
 
#define MemPutObjNum         Mem_PutWord
#define MemGetObjNum         Mem_GetWord
#define MemPushObjNum        Mem_PushWord
 
#define MemPushChunkNum      Mem_PushWord
#define MemGetChunkNum       Mem_GetWord
#define MemPutChunkNum       Mem_PutWord
 
extern  handletype  SwapHandleUnits; 
extern  handletype  CoreHandles; 
extern  idtype      RenvId;
extern  idtype      WenvId;

/*+-----------------------------------------------------------------------+
~HM                           HANDLE                                      !
  +-----------------------------------------------------------------------+*/

void    Hm_SetDRIswi      (boolean       Dri_Env)
{
    DRI_Env = Dri_Env;
return;
}


boolean HmDRIswi         (void)
{
return (DRI_Env);
}


void    HmSetDefaults    (indextype     EnvObjs,   indextype     EnvAttrs,  
                          indextype     AllocObjs, indextype     ExtendObjs,
                          indextype     AllocAttrs,indextype     ExtendAttrs,
                          indextype     AllocHndls,indextype     ExtendHndls,
                          indextype     AllocVchunks,indextype   ExtendVchunks, 
                          indextype     AllocGrps, indextype     ExtendGrps, 
                          indextype     AllocTexts,indextype     ExtendTexts, 
                          indextype     AllocXblks,indextype     ExtendXblks)
{
  HM_EnvObjs          = EnvObjs;
  HM_EnvAttrs         = EnvAttrs;

  HM_AllocDbObjs      = AllocObjs;
  HM_ExtendDbObjs     = ExtendObjs;

  HM_AllocDbAttrs     = AllocAttrs;
  HM_ExtendDbAttrs    = ExtendAttrs;

  HM_AllocHandles     = AllocHndls;
  HM_ExtendHandles    = ExtendHndls;

  HM_AllocVchunks     = AllocVchunks;
  HM_ExtendVchunks    = ExtendVchunks;

  HM_AllocGrps        = AllocGrps;
  HM_ExtendGrps       = ExtendGrps;

  HM_AllocTxtTkns     = AllocTexts;
  HM_ExtendTxtTkns    = ExtendTexts;

  HM_AllocDbXmemBlks  = AllocXblks;
  HM_ExtendDbXmemBlks = ExtendXblks;

return;
}


void    HmGetDefaults    (indextype    *EnvObjs,   indextype    *EnvAttrs,  
                          indextype    *AllocObjs, indextype    *ExtendObjs,
                          indextype    *AllocAttrs,indextype    *ExtendAttrs,
                          indextype    *AllocHndls,indextype    *ExtendHndls,
                          indextype    *AllocVchunks,indextype  *ExtendVchunks, 
                          indextype    *AllocGrps, indextype    *ExtendGrps, 
                          indextype    *AllocTexts,indextype    *ExtendTexts, 
                          indextype    *AllocXblks,indextype    *ExtendXblks)
{
  *EnvObjs            =  HM_EnvObjs; 
  *EnvAttrs           =  HM_EnvAttrs; 

  *AllocObjs          =  HM_AllocDbObjs; 
  *ExtendObjs         =  HM_ExtendDbObjs; 

  *AllocAttrs         =  HM_AllocDbAttrs; 
  *ExtendAttrs        =  HM_ExtendDbAttrs; 

  *AllocHndls         =  Alloc_Handles; 
  *ExtendHndls        =  Extend_Handles; 

  *AllocVchunks       =  Alloc_Vchunks; 
  *ExtendVchunks      =  Extend_Vchunks; 

  *AllocGrps          =  HM_AllocGrps; 
  *ExtendGrps         =  HM_ExtendGrps; 

  *AllocTexts         =  HM_AllocTxtTkns; 
  *ExtendTexts        =  HM_ExtendTxtTkns; 

  *AllocXblks         =  DbXmemBlocks_AllocQty; 
  *ExtendXblks        =  DbXmemBlocks_ExtendQty; 

return;
}


boolean HmMakeDbHdr      (indextype     EnvObjs,   indextype     EnvAttrs,  
                          idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    dbheader    **CoreDb)
{
handletype    HandleToVoid = 0;
dbheader     *EnvDb        = NullPtr;
indextype     EnvObjs0     = 0;
indextype     EnvAttrs0    = 0;
indextype     AllocObjs    = 0;
indextype     ExtendObjs   = 0;
indextype     AllocAttrs   = 0;
indextype     ExtendAttrs  = 0;
indextype     AllocHndls   = 0;
indextype     ExtendHndls  = 0;
indextype     AllocVchunks = 0;
indextype     ExtendVchunks= 0;
indextype     AllocGrps    = 0;
indextype     ExtendGrps   = 0;
indextype     AllocTexts   = 0;
indextype     ExtendTexts  = 0;
indextype     AllocXblks   = 0;
indextype     ExtendXblks  = 0;


 HmGetDefaults (&EnvObjs0,      &EnvAttrs0,     &AllocObjs,    &ExtendObjs,
                &AllocAttrs,    &ExtendAttrs,   &AllocHndls,   &ExtendHndls,
                &AllocVchunks,  &ExtendVchunks, &AllocGrps,    &ExtendGrps, 
                &AllocTexts,    &ExtendTexts,   &AllocXblks,   &ExtendXblks);

 if (CoreMoreCore (sizeof(dbheader),    McStat,         (addrtype *)&EnvDb))

 if (AllocGrps)
 if (MemMakeChunk   (sizeof(addrtype),  AllocGrps,       ExtendGrps,
                     CoreDbId,          McStat,         &(EnvDb->GrpHdrDir)))
 if (MemPutStackPtr (EnvDb->GrpHdrDir,  1,               McStat))

 if (MemGetChunkAddr(EnvDb->GrpHdrDir,  McStat, (addrtype *)&EnvDb->GrpHdrs))


 if (MemMakeChunk   (sizeof(vbntype),   AllocGrps,       ExtendGrps,
                     CoreDbId,          McStat,         &(EnvDb->GrpVbnDir)))
     MemPutStackPtr (EnvDb->GrpVbnDir,  1,               McStat);
 

#ifdef   __FREEZE_VCHUNKS__
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(vbntype),   AllocVchunks,    ExtendVchunks,
                     CoreDbId,          McStat,   &(EnvDb->VbnDeleteDir)))
     MemPutStackPtr (EnvDb->VbnDeleteDir,  0,            McStat);
#endif /* __FREEZE_VCHUNKS__  */

 if (AllocHndls)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(chunkindex),AllocHndls,      ExtendHndls,
                     CoreDbId,          McStat,     &(EnvDb->ObjectIndexDir)))
 if (MemPutStackPtr (EnvDb->ObjectIndexDir,  (indextype )1,    McStat))
 
 if (MemMakeChunk   (sizeof(addr),      AllocHndls,      ExtendHndls,
                     CoreDbId,          McStat,      &(EnvDb->ObjectAddrDir)))
 if (MemPutStackPtr (EnvDb->ObjectAddrDir,  (indextype )1,McStat))
 
 if (MemNewBitChunk (AllocHndls,        CoreDbId,
                     McStat,                       &(EnvDb->ObjectMemLocDir)))

 if (MemMakeChunk   (sizeof(objnum),    AllocHndls,      ExtendHndls,
                     CoreDbId,          McStat,     &(EnvDb->LocalObjNumDir)))
 if (MemPutStackPtr (EnvDb->LocalObjNumDir,  (indextype )1,   McStat))

 if (CoreDbId > WenvId)
 if (MemMakeChunk   (sizeof(shorttype), AllocHndls,      ExtendHndls,
                     CoreDbId,          McStat,    &(EnvDb->ObjectMemRefDir)))
     MemPutStackPtr (EnvDb->ObjectMemRefDir,  1,         McStat);

 
 if (EnvObjs)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(localobjnum),EnvObjs+1,      EnvObjs/2,
                     CoreDbId,          McStat,     &(EnvDb->LocalObjNumMap)))

 if (MemMakeChunk   (sizeof(handletype),EnvObjs+1,       EnvObjs/2,
                     CoreDbId,          McStat,     &(EnvDb->ObjHandleDir)))

 if (MemPutStackPtr (EnvDb->LocalObjNumMap,
                    (EnvDb->LastEnvObjCount=EnvObjs), McStat)) 

 if (MemPutStackPtr (EnvDb->ObjHandleDir,
                     EnvDb->LastEnvObjCount,             McStat)) 

     MemPutObjNum   (EnvDb->LocalObjNumMap, 0,  0,       McStat);


 if (AllocObjs)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(objnum),    AllocObjs,       ExtendObjs,
                     CoreDbId,          McStat,        &(EnvDb->ObjNumMap)))
 
 if (MemMakeChunk   (sizeof (longwordtype),AllocObjs,    ExtendObjs,
                     CoreDbId,          McStat,    &(EnvDb->ObjTimeStampDir)))

 if (MemMakeChunk   (sizeof(addrtype),  AllocObjs,       ExtendObjs,
                     CoreDbId,          McStat,         &(EnvDb->ObjHdrDir)))

 if (MemMakeChunk   (sizeof(vbntype),   AllocObjs,       ExtendObjs,
                     CoreDbId,          McStat,         &(EnvDb->ObjVbnDir)))

 if (DRI_Env)
 if (MemPutStackPtr (EnvDb->ObjNumMap,  1,               McStat))
 if (MemPutStackPtr (EnvDb->ObjHdrDir,  1,               McStat))
 if (MemPutStackPtr (EnvDb->ObjVbnDir,  1,               McStat))
     MemPutStackPtr (EnvDb->ObjTimeStampDir, 1,          McStat);


 if (!DRI_Env)
 /*     set stackSize by  Num Env Obj's for RENV                           */
 if (Normal(*Status))
 if (Build_Env)
     MemPutStackPtr (EnvDb->ObjTimeStampDir,
                     EnvDb->LastEnvObjCount,             McStat);


 if (DeBug)
 if (Normal(*Status))
 if (EnvDb->LocalObjNumMap)
    DumpChunkChunk (2, EnvDb->LocalObjNumMap);


 if (EnvAttrs)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(localattrnum), EnvAttrs+1,      EnvAttrs / 2,
                     CoreDbId,             McStat, &(EnvDb->LocalAttrNumMap)))

 if (MemMakeChunk   (sizeof(handletype),   EnvAttrs+1,      EnvAttrs / 2,
                     CoreDbId,             McStat,   &(EnvDb->AttrHandleDir)))

 if (MemPutStackPtr (EnvDb->LocalAttrNumMap,
                    (EnvDb->LastEnvAttrCount= AllocAttrs),McStat)) 

     MemPutStackPtr (EnvDb->AttrHandleDir,    AllocAttrs, McStat);


 if (AllocAttrs)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(localattrnum), AllocAttrs+1,  AllocAttrs / 2,
                     CoreDbId,             McStat,      &(EnvDb->AttrNumMap)))

 if (MemMakeChunk   (sizeof(typesize),     AllocAttrs+1,  AllocAttrs / 2,
                     CoreDbId,             McStat,&(EnvDb->LocalAttrSizeTbl)))

 if (MemPutStackPtr (EnvDb->AttrNumMap,       1,          McStat))
     MemPutStackPtr (EnvDb->LocalAttrSizeTbl, 1,          McStat);
 

 if (AllocTexts)
 if (Normal(*Status))
 if (MemMakeChunk   (sizeof(texttkntype),  AllocTexts,    ExtendTexts,
                     CoreDbId,             McStat,  &(EnvDb->TextTokenDir)))

 if (MemPutStackPtr (EnvDb->TextTokenDir,  1, McStat))

     MemMakeChunk   (sizeof(chunkindex),   ExtendTexts,   ExtendTexts,
                     CoreDbId,             McStat,  &(EnvDb->FreeTxtTknStack));
 

 if (Normal(*Status))
 {
   if (MemMakeChunk   (sizeof(indextype),        DbXmemBlocks_AllocQty, 
                       DbXmemBlocks_ExtendQty,   CoreDbId,
                       McStat,                  &(EnvDb->BlkHdr.BlkDirChunk)))

   if (MemMakeChunk   (sizeof(blktkntype),       DbXmemBlocks_AllocQty, 
                       DbXmemBlocks_ExtendQty,   CoreDbId,
                       McStat,                  &(EnvDb->BlkHdr.BlkTknChunk)))

   if (MemMakeChunk   (sizeof(bytessize ),       DbXmemBlocks_AllocQty,
                       DbXmemBlocks_ExtendQty,   CoreDbId,
                       McStat,                  &(EnvDb->BlkHdr.BlkSizeChunk)))

   if (MemPutStackPtr (EnvDb->BlkHdr.BlkDirChunk,       1,      McStat))
   if (MemPutStackPtr (EnvDb->BlkHdr.BlkTknChunk,       1,      McStat))
   if (MemPutStackPtr (EnvDb->BlkHdr.BlkSizeChunk,      1,      McStat))
   {
      EnvDb->BlkHdr.DbId      = CoreDbId;
      EnvDb->BlkHdr.EofMark   = 0;
      EnvDb->BlkHdr.BlkFile   = NullPtr;
      EnvDb->SchemaHndl       = NullHandle;
      if (DeBug)
      {
         DumpChunkChunk (0,   EnvDb->BlkHdr.BlkDirChunk);
         DumpChunkChunk (0,   EnvDb->BlkHdr.BlkTknChunk);
         DumpChunkChunk (0,   EnvDb->BlkHdr.BlkSizeChunk);
      }
   }
 }
 
 /* DbHdr... initialize remainder of struc                                 */
 if (Normal(*Status))
 {
     /* the first handle is reserved for the DbPrimal object of ThisDb     */
     VoidHandle            = HandleToVoid++;
     EnvDb->NextHandle     = HandleToVoid;

     if (CoreDbId          > WenvId)
        EnvDb->DiskCache   = True;
     else
        EnvDb->DiskCache   = False;

     EnvDb->GrpCount       = 0;
     EnvDb->GrpHdrDirVbn   = 0;
     EnvDb->HeadFreeGrps   = 0;

     EnvDb->VchunkGrpNum   = 0;
     EnvDb->VchunkGrpHdr   = NullPtr;
     EnvDb->VchunkBaseIndex= 1;

     EnvDb->HandleCount    = HandleToVoid;
     EnvDb->HandleBaseIndex= Alloc_Handles;

     EnvDb->LocalAttrCount = 0;

     EnvDb->StackHeadDir   = 0;
     EnvDb->StackNames     = 0;
     EnvDb->ListHeadDir    = 0;
     EnvDb->ListNames      = 0;
     EnvDb->TblHeadDir     = 0;
     EnvDb->TblNames       = 0;
     EnvDb->TreeHeadDir    = 0;
     EnvDb->TreeNames      = 0;
     EnvDb->lKeyHeadDir    = 0;
     EnvDb->lKeyNames      = 0;
     EnvDb->MtrxHeadDir    = 0;
     EnvDb->MtrxNames      = 0;

     EnvDb->PrimalHdr      = NullPtr;
     EnvDb->PrimalHdrSize  = 0;
     EnvDb->PrimalHdrVbn   = 0;

    *CoreDb = EnvDb;

 }

TRACK(TrackBak,"HmMakeDbHdr\n");
return (STAT);
}
 

#ifndef __MACROIZE_HM__

boolean    HmPushLocalObjNum  (dbheader     *CoreDb, localnumtype LocalObj,
                               ft F,lt Z,zz *Status, indextype   *StackPtr)
{
     MemPushLocalObjNum (CoreDb->LocalObjNumDir,        LocalObj,
                         McStat,                        StackPtr);

TRACK(TrackBak,"HmPushLocalObjNum\n");
return (STAT);
}
 

boolean    HmPutLocalObjNum  (dbheader    *CoreDb,  handletype    Handle,
                             localnumtype  LocalObj, ft F,lt Z,zz *Status)
{
     MemPutLocalObjNum (CoreDb->LocalObjNumDir,        Handle,
                        LocalObj,                      McStat);
 
TRACK(TrackBak,"HmPutLocalObjNum\n");
return (STAT);
}
 

boolean    HmGetLocalObjNum  (dbheader    *CoreDb,  handletype    Handle,
                             ft F,lt Z,zz *Status,  localnumtype *LocalObj)
{
     MemGetLocalObjNum (CoreDb->LocalObjNumDir,        Handle,
                        McStat,                        LocalObj);

TRACK(TrackBak,"HmGetLocalObjNum\n");
return (STAT);
}
 

boolean    HmPutObjNum   (dbheader     *CoreDb,    indextype     LocalObjNum,
                          numtype       EnvObjNum, ft F,lt Z,zz *Status)
{

  MemPutObjNum     (CoreDb->ObjNumMap, LocalObjNum,  EnvObjNum, McStat);

 
TRACK(TrackBak,"HmPutObjNum\n");
return (STAT);
}
 

boolean    HmPushObjectAddr  
                         (dbheader     *CoreDb, addrtype     ObjectAddr,
                          ft F,lt Z,zz *Status, indextype   *StackPtr)
{
     MemPushObjectAddr    (CoreDb->ObjectAddrDir,        ObjectAddr,
                           McStat,                       StackPtr);

TRACK(TrackBak,"HmPushObjectAddr\n");
return (STAT);
}
 

boolean    HmPutObjectAddr  (dbheader  *CoreDb,      handletype Handle,
                             addrtype   ObjectAddr, ft F,lt Z,zz *Status)
{
     MemPutObjectAddr (CoreDb->ObjectAddrDir,        Handle,
                       ObjectAddr,                   McStat);

TRACK(TrackBak,"HmPutObjectAddr\n");
return (STAT);
}
 

boolean    HmGetObjectAddr  (dbheader  *CoreDb,  handletype   Handle,
                            ft F,lt Z,zz *Status,  addrtype    *ObjectAddr)
{
     MemGetObjectAddr (CoreDb->ObjectAddrDir,        Handle,
                        McStat,                      ObjectAddr);

TRACK(TrackBak,"HmGetObjectAddr\n");
return (STAT);
}
 

boolean    HmPushObjectMemLoc  (dbheader  *CoreDb, numtype      MemLoc,
                             ft F,lt Z,zz *Status, indextype   *StackPtr)
{
if (MemLoc)
   MemLoc = 1;

  MemPushBit         (CoreDb->ObjectMemLocDir,   MemLoc,
                      McStat,                    StackPtr);

TRACK(TrackBak,"HmPushObjectMemLoc\n");
return (STAT);
}
 

boolean    HmPutObjectMemLoc  (dbheader  *CoreDb,      handletype Handle,
                             numtype    MemLoc, ft F,lt Z,zz *Status)
{
if (MemLoc)
   MemLoc = 1;

     MemPutBit        (CoreDb->ObjectMemLocDir,  Handle,
                       MemLoc,                   McStat);

TRACK(TrackBak,"HmPutObjectMemLoc\n");
return (STAT);
}
 

boolean    HmGetObjectMemLoc  (dbheader     *CoreDb,  handletype   Handle,
                               ft F,lt Z,zz *Status,  numtype     *MemLoc)
{
  if (Handle == 0)
    *Status   = Hm_BadHandle;
  else
     MemGetBit         (CoreDb->ObjectMemLocDir,   Handle,
                        McStat,                    MemLoc);

TRACK(TrackBak,"HmGetObjectMemLoc\n");
return (STAT);
}
 

boolean    HmPushObjectMemRef  (dbheader     *CoreDb, shorttype    MemRef,
                                ft F,lt Z,zz *Status, indextype   *StackPtr)
{
  MemPushShort          (CoreDb->ObjectMemRefDir,        MemRef,
                         McStat,                         StackPtr);

TRACK(TrackBak,"HmPushObjectMemRef\n");
return (STAT);
}
 

boolean    HmPutObjectMemRef  (dbheader  *CoreDb,   handletype    Handle,
                               shorttype  MemRef,   ft F,lt Z,zz *Status)
{
  if (Handle == 0)
    *Status   = Hm_BadHandle;
  else
     MemPutShort   (CoreDb->ObjectMemRefDir,  Handle,
                    MemRef,                   McStat);

TRACK(TrackBak,"HmPutObjectMemRef\n");
return (STAT);
}
 


boolean    HmGetObjectMemRef  (dbheader     *CoreDb,  handletype   Handle,
                               ft F,lt Z,zz *Status,  shorttype   *MemRef)
{
  if (Handle == 0)
    *Status   = Hm_BadHandle;
  else
     MemGetShort       (CoreDb->ObjectMemRefDir,   Handle,
                        McStat,                    MemRef);

TRACK(TrackBak,"HmGetObjectMemRef\n");
return (STAT);
}
 

boolean    HmPushObjectIndex  (dbheader  *CoreDb, indextype   ObjectIndex,
                              ft F,lt Z,zz *Status, indextype   *StackPtr)
{
     MemPushObjectIndex (CoreDb->ObjectIndexDir,        ObjectIndex,
                         McStat,                        StackPtr);

TRACK(TrackBak,"HmPushObjectIndex\n");
return (STAT);
}
 

boolean    HmPutObjectIndex (dbheader   *CoreDb,      handletype Handle,
                             indextype   ObjectIndex,ft F,lt Z,zz *Status)
{
     MemPutObjectIndex (CoreDb->ObjectIndexDir,        Handle,
                        ObjectIndex,                   McStat);

TRACK(TrackBak,"HmPutObjectIndex\n");
return (STAT);
}
 

boolean    HmGetObjectIndex (dbheader  *CoreDb,  handletype   Handle,
                            ft F,lt Z,zz *Status,  indextype   *ObjectIndex)
{
     MemGetObjectIndex (CoreDb->ObjectIndexDir,      Handle,
                        McStat,                      ObjectIndex);
 
TRACK(TrackBak,"HmGetObjectIndex\n");
return (STAT);
}
 

boolean HmInvokeObject (dbheader *CoreDb,     handletype   Handle,
                        addrtype  Object,     ft F,lt Z,zz *Status)
{
  HmPutObjectAddr (CoreDb, Handle, Object,  McStat);

TRACK(TrackBak,"HmInvokeObject\n");
return (STAT);
}


boolean HmPointToOuterRegion (dbheader     *CoreDb,     handletype   Handle,
                              ft F,lt Z,zz *Status)
{
  HmPutObjectMemLoc (CoreDb, Handle, 1,  McStat);

TRACK(TrackBak,"HmPointToOuterRegion\n");
return (STAT);
}
 

#endif /* __MACROIZE_HM__ */


boolean    HmGetObjNum   (dbheader     *CoreDb,  handletype    Handle,
                          ft F,lt Z,zz *Status,  numtype      *EnvObjNum)
{
numtype  LocalObjNum = 0;

  if (HmGetLocalObjNum (CoreDb, Handle,    McStat,      &LocalObjNum))
      MemGetObjNum     (CoreDb->ObjNumMap, LocalObjNum,  McStat,  EnvObjNum);

 
TRACK(TrackBak,"HmGetObjNum\n");
return (STAT);
}



boolean  HmIncrementMemRef  (dbheader     *CoreDb, handletype   Handle,
                             ft F,lt Z,zz *Status)
{
shorttype   MemRef   = 0;

  if (MemGetShort      (CoreDb->ObjectMemRefDir,   Handle,
                        McStat,                   &MemRef))
      MemPutShort      (CoreDb->ObjectMemRefDir,   Handle,    ++MemRef,
                        McStat);

TRACK(TrackBak,"HmIncrementMemRef\n");
return (STAT);
}
 

boolean  HmDecrementMemRef  (dbheader     *CoreDb, handletype   Handle,
                             ft F,lt Z,zz *Status, shorttype   *MemRef)
{
 *MemRef   = -1;
  if (MemGetShort      (CoreDb->ObjectMemRefDir,   Handle,
                        McStat,                    MemRef))
  {
     *MemRef          = *MemRef - (shorttype )1;
      MemPutShort      (CoreDb->ObjectMemRefDir,   Handle,    *MemRef,
                        McStat);
  }


TRACK(TrackBak,"HmDecrementMemRef\n");
return (STAT);
}
 

boolean    HmNewHandle  (dbheader     *CoreDb, localnumtype LocalObj,
                         ft F,lt Z,zz *Status, handletype  *Handle)
{
indextype    StackPtr = 0;
 HmNextFreeHandle    (CoreDb,  McStat,   Handle);
 if (*Status == Hm_NoFreeHandles)
 {
   *Status = Env_Normal;
    if (HmPushObjectIndex   (CoreDb,  0,        McStat, &StackPtr))
    if (HmPushObjectAddr    (CoreDb,  0,        McStat, &StackPtr))
    if (HmPushLocalObjNum   (CoreDb,  LocalObj, McStat, &StackPtr))
    if (CoreDb->DbId  > WenvId)
        HmPushObjectMemRef  (CoreDb,  0,        McStat, &StackPtr);

    if (Normal(*Status))
    if ((*Handle = StackPtr) == CoreDb->NextHandle++)
        ++CoreDb->HandleCount;
    else {
       *Status = Hm_BadHandlesStack;
    }
 }
 else
 {
    if (HmPutObjectIndex   (CoreDb, *Handle,  0,        McStat))
    if (HmPutObjectAddr    (CoreDb,  *Handle, 0,        McStat))
    if (HmPutLocalObjNum   (CoreDb,  *Handle, LocalObj, McStat))
    if (CoreDb->DbId  > WenvId)
        HmPutObjectMemRef  (CoreDb,  *Handle, 0,        McStat);
 }

TRACK(TrackBak,"HmNewHandle\n");
return (STAT);
}
 

boolean    HmPairDbObject  (dbheader *CoreDb,   handletype Handle,
                            indextype Index,    addrtype   Object,
                            ft F,lt Z,zz *Status)
{
 
 if (HmPutObjectIndex (CoreDb,   Handle,  Index,   McStat))
 if (HmPutObjectMemLoc(CoreDb,   Handle,  0,       McStat))
 if (HmPutObjectAddr  (CoreDb,   Handle,  Object,  McStat))
 if (CoreDb->DbId > WenvId)
     HmPutObjectMemRef(CoreDb,   Handle,  0,       McStat);

TRACK(TrackBak,"HmPairDbObject\n");
return (STAT);
}
 
 
boolean    HmFetchDbObject  (dbheader     *CoreDb,   handletype  Handle,
                             ft F,lt Z,zz *Status,   addrtype   *Object)
{
*Object    = NullPtr;

 if (Normal(*Status))
 if (HmGetObjectAddr  (CoreDb,   Handle,  McStat,  Object))
 if (*Object == NullPtr)
 {
    *Status = Hm_ObjectNotPaired;
 }

TRACK(TrackBak,"HmFetchDbObject\n");
return (STAT);
}
 

boolean HmClearObject  (dbheader *CoreDb, handletype  Handle, 
                        ft F,lt Z,zz *Status)
{
  if (HmPutObjectAddr   (CoreDb, Handle, (addrtype )NullPtr,  McStat))
  if (HmPutObjectMemLoc (CoreDb, Handle, (numtype  )0,        McStat))
  if (CoreDb->DbId > WenvId)
      HmPutObjectMemRef (CoreDb, Handle, (numtype  )0,        McStat);
 
TRACK(TrackBak,"HmClearObject\n");
return (STAT);
}
 

boolean HmPointToInnerRealm (dbheader     *CoreDb,     handletype   Handle,
                             ft F,lt Z,zz *Status)
{
numtype  MemLoc = 0;

  if (HmPutObjectMemLoc (CoreDb, Handle, 0,  McStat))
      HmGetObjectMemLoc (CoreDb, Handle, McStat, &MemLoc);

TRACK(TrackBak,"HmPointToInnerRealm\n");
return (STAT);
}
 


boolean  HmFreeHandle    (dbheader     *CoreDb,    handletype     Handle,
                          ft F,lt Z,zz *Status)
{
   if (CoreDb->FutureHandles   == NullHandle)
      HmPutObjectIndex (CoreDb,   Handle,  0,                      McStat);
   else
      HmPutObjectIndex (CoreDb,   Handle,  CoreDb->FutureHandles,  McStat);

   CoreDb->FutureHandles     = Handle;

   if (Normal(*Status))
      HmPutLocalObjNum (CoreDb,   Handle,  Null_ObjNum,            McStat);

TRACK(TrackBak,"HmFreeHandle\n");
return(STAT);
}

boolean  HmNextFreeHandle  (dbheader     *CoreDb, 
                            ft F,lt Z,zz *Status,    handletype    *Handle)
{
handletype  NxtHndl  = NullHandle;

   if (CoreDb->CurrentHandles == NullHandle)
   {
    *Status = Hm_NoFreeHandles;
   } else {
    *Handle = CoreDb->CurrentHandles;
     if (HmGetObjectIndex (CoreDb,  CoreDb->CurrentHandles, McStat, &NxtHndl))
     if (HmPutObjectIndex (CoreDb, *Handle,         0,      McStat))
        CoreDb->CurrentHandles  = NxtHndl;
   }

TRACK(TrackBak,"HmNextFreeHandle\n");
return(STAT);
}

boolean  HmReclaimHandles  (dbheader     *CoreDb, ft F,lt Z,zz *Status )
{
   CoreDb->CurrentHandles = CoreDb->FutureHandles ;
   CoreDb->FutureHandles = 0 ;
   TRACK(TrackBak,"HmReclaimHandles\n");
   return(STAT);
}


boolean    HmDumpDbHdr   (int           TraceSwi,  tokentype     *Anchor,
                          ft F,lt Z,zz *Status)
{
 
TRACK(TrackBak,"HmDumpDbHdr\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF HM.C                                    !
  +-----------------------------------------------------------------------+*/
