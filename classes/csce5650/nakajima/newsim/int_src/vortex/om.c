/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OM.c                                                              */
#define  OM
#include <time.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "dbm.h"
#include "odbm.h"
#include "ogrp.h"
#include "env0.h" 
#include "om.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...MEM00... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[71] =
{
   "    OmNewObjHdr (CoreDb[%2u] @ %x)  for ObjNum = %3u\n",
   "        LocalObjNum = %3u\n",
   "   OmNewObjHdr intermediate Db Obj Query *Status= %d\n",
   "*** ERROR... Obj[%3u] NOT in Schema of CoreDb[%2u]\n",
   "   NEW ObjHdr[%3u](%3u) @ %x regions=%3u Sectors=%3u Alloc=%4u\n",
   "   NEW ObjHdr[%3u] @ %x Regions=%3u Objects/Region=%4u\n",
   "    Set ObjHdr[%3d]->HandleOffset = %2u\n",
   "    GET ObjHdr[%3d]  @ %x\n",
   " Error in OmGetObjHdr:: DbId= %3u; Object_Index=%6u\n",
   "     EXOBJECT Index = %4u Region = %2u Offset = %3u\n",
   "    APPEND Obj[%3u] Object(%u) to Region[%u] @ Offset=%u\n",
   "    OmNewObject[%4u] (%4u) RgnChunk %2d   @ %x\n",
   "     OM NEW Region Obj[%3u] Index[%4u] RgnIndex[%3u] Qty= %3u\n",
   "       Current Rgn[%2u] Chunk= %3u Vbn= %2u\n",
   "       New     Rgn[%2u] Chunk= XX  Vbn= XX\n",
   "  CurrentRegion %3u is EMPTY; swaps for NEW Region[%u]\n",
   "      NEW Region[%d] ObjectIndex[%4u] Chunk= %3d\n",
   "   Om Pair Object<--Handle :: Region[%2u](%2u)+%2u = %4u @%x Offset %u\n",
   "\n   UnPairRgnObjects<FALSE>::    ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   " BAD UNPAIR Handles for RegionChunk[%3u] @%x count %4u\n",
   "\n   UnPair Objects<-->Handles; ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   "    unpair Region[](%3u) @Offset= %4u;  Handle= %u @%x\n",
   " Outer Region Address Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   " Null Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   "   OmPairRgnObjects<FALSE>::    ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   " BAD PAIR Handles for RegionChunk[%3u] @%x count %4u\n",
   "   OmPairRgnObjects<-->Handles; ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   "    pair Region[](%3u) Object @%x;  Handle= %u @%x\n ",
   " Outer Region Address Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   " Region[%3u] Object[%3u] Handle= %4u\n",
   " Null Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   "  OmGetRegion for DB = %3u; ObjNum =%3u; Region = %3u\n",
   "       Current Rgn[%2u] Chunk= %3u Vbn= %2u\n",
   "       New     Rgn[%2u] Chunk= %3u Vbn= %2u\n",
   "  CurrentRegion %3u is EMPTY; swaps for Region[%u] vbn=%2u\n",
   "    Roll OUT Current Region[%4u] Chunk= %6u SwapVbn =%6u\n",
   "       Current Rgn[%2u] Chunk= %3u Vbn= %2u\n",
   "       New     Rgn[%2u] Chunk= %3u Vbn= %2u\n",
   "  CurrentRegion %3u is EMPTY; swaps for Region[%u] vbn=%2u\n",
   "    Roll OUT Current Region[%4u] Chunk= %6u SwapVbn =%6u\n",
   "    Roll IN  New     Region[%4u] Chunk= %6u SwapVbn =%6u\n",
   "      Region[%3u] Chunk[%6u] @%x OverLoaded!!!\n",
   "      Load Region[%3u] Chunk[%6u]\n",
   "      Current Region Set to %3u; As Chunk =%3u @%x\n",
   "  OmGetObject[%2u:%4u] ...\n",
   "        Index =%3u; RgnNum =%3u, LocalIndex = %3u\n",
   "   OmGetObject[%2u:%4u]:: @%x Class[%3u] of Region[%2u:%2u]\n",
   "   OmChkImage of ObjHdr(%3u) in DbId = %3u\n",
   "  from DB_ObjHdr Time: %s   To RENV_ObjDesc Time %s\n",
   "       DB_ObjHdr Size= %3u;  Renv ObjDesc Size=%3u\n",
   "   OmReBuild Objects of ObjHdr(%3u) in DbId = %3u\n",
   "    Field LocalAttr=%3u TransChunk[%3u]=%3u \n",
   "    Field LocalAttr=%3u TransChunk[%3u]=%3u \n",
   "   Move To Field[%2u]+ %3u @%x From ObjField[%2u]+ %2u @ %x = ",
   "\n    Reform Obj's in  Region[%2u]  Chunk[%2u]  :  @ %X\n",
   "    From Object[%2u] Handle[%4u] (+ %3u)... To (+ %3u)\n",
   " ObjHdr: Size=%3u Fields[%2u], LocalAttrMap=%2u, OffsetDir=%2u\n",
   "  REform Object @%x To NewRegion @%x ... \n",
   "   Move To Field[%2u]+ %3u @%x From ObjField[%2u]+ %3u @ %x = ",
   " OmGetObjHandles ::  ObjNum=%4u\n",
   " ObjectCount      :: %u\n",
   " RegionObjects    :: %u\n",
   " RegionCount      :: %u\n",
   " ExObjectCount    :: %u\n",
   "\n    Obj Region[%2u]  Chunk[%2u]  :  @ %X\n",
   "   %-7u\n",
   "   %-7u  ",
   "   RgnStatus[%3u] =%4u\n",
   "\n    Obj Region[%2u]  Chunk[%2u]  :  @ %X\n",
   "   %-7u\n",
   "   %-7u  "
};
#endif


#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)
 

/*+-----------------------------------------------------------------------+*
~OA0                         EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
extern boolean EnvBldObjImage 
                             (dbheader     *CoreDb,  objheader    *ObjHdr,
                              ft F,lt Z,zz *Status,
                              numtype      *FldChunk,numtype      *OffsetChnk,
                              indextype    *FldCount);

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

#define MemPushAttrNum       Mem_PushWord
#define MemGetAttrNum        Mem_GetWord
#define MemPutAttrNum        Mem_PutWord
 
#define MemPushObjHdr        Mem_PushAddr
#define MemPutObjHdr         Mem_PutAddr
#define MemGetObjHdr         Mem_GetAddr
 
#define MemPushChunkNum      Mem_PushWord
#define MemGetChunkNum       Mem_GetWord
#define MemPutChunkNum       Mem_PutWord
 
#define MemPutRegionNum      Mem_PutWord
#define MemGetRegionNum      Mem_GetWord
 
#define MemPutOffset         Mem_PutWord
#define MemGetOffset         Mem_GetWord
 
#define MemPushLink          Mem_PushWord
#define MemPutLink           Mem_PutWord
#define MemGetLink           Mem_GetWord
 
#define MemPushExObject      Mem_PushWord
#define MemPopExObject       Mem_PopWord
 
 
#define MemGetVbn            Mem_GetWord
#define MemGetChunkNum       Mem_GetWord
 
#define MemGetSize           Mem_GetWord
#define MemPushSize          Mem_PushWord
 

/*+-----------------------------------------------------------------------+*
~OM                         OBJECT MANAGER                                !
 *+-----------------------------------------------------------------------+*/
void       OmSetConfig   (boolean       Config)
{
   Config_Class   = Config;
return;
}

void       OmConfigClass (numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad)
{
  Config_Class          = True;

  Class_RegionEntrys    = RgnEntrys;
  Class_AllocRegions    = AllocRgns;
  Class_ExtendRegions   = ExtendRgns;
  Class_AllocExObjects  = AllocXs;
  Class_ExtendExObjects = ExtendXs;
  Class_AccessType      = AccessType;
  Class_DiskCache       = DiskCache;
  Class_CacheLimit      = CacheLimit;
  Class_CacheLoad       = CacheLoad;

return;
}


boolean    OmNewObjHdr (tokentype    *Anchor,   numtype     EnvObj,
                        indextype     AllocQty, 
                        ft F,lt Z,zz *Status,   objheader **ObjHdr)
{
dbheader    *CoreDb          = NullPtr;
localnumtype LocalObj        = 0;
indextype    ObjIndex        = 0;
indextype    StackPtr        = 0;
indextype    AllocObjects    = AllocQty;
time_t       Tod             = 0;

 time (&Tod);

*ObjHdr     = NullPtr;

 if (TmGetCoreDb       (Anchor,   McStat,   &CoreDb))
 if (Normal(*Status))
 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap,  EnvObj,  McStat, &LocalObj))
 if (LocalObj          == 0
 &&  EnvObj            != 0)
    *Status             = Om_ObjNotInSchema;

 if (Normal(*Status))
 {
    ObjIndex            = LocalObj;
    if (MemGetStackPtr (CoreDb->ObjHdrDir,    McStat,   &StackPtr))
    if (ObjIndex
    &&  ObjIndex        < StackPtr)
    {
      *Status           = Env_Normal;
      if (Build_Env 
      &&  Anchor->DbId == RenvId)
      {
         TraceMsg (0, "    OmNewObjHdr ... from Void to Core \n");
         OmGetObjHdr (Anchor,     ObjIndex,      McStat,  ObjHdr);
      } else
         *ObjHdr = NullPtr;
    } else {
       /* This is for RENV only                                           */
       if (EnvObj      == 0)
          MemGetObjHdr (CoreDb->ObjHdrDir, ObjIndex,
                        McStat,                      (addrtype *)ObjHdr);
       else
         *Status        = Om_IndexOutOfRange;
    }
 }

 if (*ObjHdr 
 && (*Status           != Om_IndexOutOfRange))
 {
   *Status              = Om_ObjHdrExists;

 } else { 
   if (*Status      == Om_IndexOutOfRange
   ||  *Status      == Om_ObjNotInSchema)
   {
      if (Anchor->DbId <= PrimalId
      ||  Anchor->DbId >= PrimalId)
      {
         *Status = Env_Normal;
      } else {
         if (sprintf (Msg,
/* "*** ERROR... Obj[%3u] NOT in Schema of CoreDb[%2u]\n" */
               TraceStr(3),
               EnvObj, Anchor->DbId))
         if (SendMsg (0, Msg))
            *Status = Om_ObjNotInSchema;
      }

      if (Normal(*Status))
      if (MemPushObjNum (CoreDb->ObjNumMap,  EnvObj,  McStat, &LocalObj))

      if (Anchor->DbId != RenvId)
          MemPushLong   (CoreDb->ObjTimeStampDir,  (longwordtype )Tod,
                         McStat,                              &LocalObj);

      if (Normal(*Status))
      if (MemPushObjHdr (CoreDb->ObjHdrDir,  NullPtr, McStat, &ObjIndex))
      if (MemPushWord   (CoreDb->ObjVbnDir,  0,       McStat, &StackPtr))
         ++(CoreDb->LocalObjCount);

      if (Normal(*Status))
          MemPutLocalObjNum (CoreDb->LocalObjNumMap,  EnvObj,
                             LocalObj,                McStat);
   }
 
   if (Normal(*Status))   
   if ((EnvObj        && Anchor->DbId == RenvId)
   ||  (LocalObj > 1  && Anchor->DbId != RenvId))
   { 
      if (LocalObj == 0)
      {
         if (MemPushObjNum (CoreDb->ObjNumMap, EnvObj,   McStat, &LocalObj))
         if (Anchor->DbId != RenvId)
             MemPushLong   (CoreDb->ObjTimeStampDir, 
                            (longwordtype )Tod,
                            McStat,                              &LocalObj); 
      } else  {
         MemPutObjNum  (CoreDb->ObjNumMap, LocalObj, EnvObj,  McStat);
         if (Anchor->DbId != RenvId)
            MemPutLong (CoreDb->ObjTimeStampDir, LocalObj, 
                        (longwordtype )Tod,  McStat);
      }
   }
 
   /* Make new Obj header and link into Dbheader                           */
   if (Normal(*Status))   
   if (CoreMoreCore      (sizeof(objheader), McStat,    (addrtype *)ObjHdr))
   if (MemPutObjHdr      (CoreDb->ObjHdrDir, LocalObj, *(addrtype *)ObjHdr, 
                          McStat))
   if (MemPutWord        (CoreDb->ObjVbnDir, LocalObj,  0,          McStat))
   if (MemPutLocalObjNum (CoreDb->LocalObjNumMap,      (indextype  )EnvObj,
                          LocalObj,                                 McStat))
   {
      (**ObjHdr).EnvObjType    = 0;
      (**ObjHdr).LocalAccess   = CoreDb->LocalAccess;
      (**ObjHdr).DiskCache     = CoreDb->DiskCache;

      (**ObjHdr).ObjSize       = ObjHdrSize[EnvObj];
      (**ObjHdr).LocalObjNum   = LocalObj;
      (**ObjHdr).ObjNum        = EnvObj;
   }
 
   /* Install the generic Obj header Chunks                                */
   if (Normal(*Status))
   { 
      OmMakeObjChunks (Anchor, *ObjHdr, McStat, &AllocObjects);

   }

   if (ObjSectorQty[EnvObj] == 0 
   ||  ObjRegionQty[EnvObj] == 0)
   {
      *Status                = Env0_NullRegionQty;
       sprintf (Msg,
/* "   NEW ObjHdr[%3u](%3u) @ %x regions=%3u Sectors=%3u Alloc=%4u\n" */
          TraceStr(4), 
                EnvObj, LocalObj, *ObjHdr,  ObjRegionQty[EnvObj], 
                ObjSectorQty[EnvObj],       AllocObjects);
       TraceMsg (0, Msg);
   }
 }

TRACK(TrackBak,"OmNewObjHdr\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmMakeObjChunks (tokentype    *Anchor,     objheader  *ObjHdr,
                            ft F,lt Z,zz *Status,     indextype  *AllocObjects)
{
indextype    AllocExObjects  = Alloc_ExObjects;
indextype    ExtendExObjects = Extend_ExObjects;
indextype    AllocRegions    = 10; /*     Alloc_Regions;                  */
indextype    ExtendRegions   = 10; /*     Extend_Regions;                 */
numtype      CacheLimit      = 3;
numtype      CacheLoad       = 3;
idtype       CoreDbId        = Anchor->DbId;

 if (ObjHdr->ObjNum      == (numtype )0 && Anchor->DbId == RenvId
 ||  ObjHdr->ObjNum      == (numtype )1 && Anchor->DbId != RenvId)
 {
    /* Then This IS the Primal Object of this Db                         */
    AllocExObjects        = 1;
    ExtendExObjects       = 0;

    AllocRegions          = 1;
    ExtendRegions         = 0;

   *AllocObjects          = 1;
    CacheLimit            = 1;

 } else if (Config_Class) {
   *AllocObjects          = Class_RegionEntrys;
    AllocRegions          = Class_AllocRegions;
    ExtendRegions         = Class_ExtendRegions;
    AllocExObjects        = Class_AllocExObjects;
    ExtendExObjects       = Class_ExtendExObjects;
    CacheLimit            = Class_CacheLimit;
    CacheLoad             = Class_CacheLoad;

    ObjHdr->LocalAccess   = Class_AccessType;
    ObjHdr->DiskCache     = Class_DiskCache;

 } else {
   *AllocObjects          = ObjSectorQty[ObjHdr->ObjNum];
    AllocRegions          = ObjRegionQty[ObjHdr->ObjNum];
 }

 ObjHdr->RegionObjects    = *AllocObjects;
 ObjHdr->NoRegionsInCore  = True;

 if (FREEZE_GRP_PACKETS  == True)
 {
   ObjHdr->FreezeClass    = 1;
   ObjHdr->DiskCache      = False;
 } else {
   ObjHdr->FreezeClass    = 0;
   ObjHdr->DiskCache      = True;
 }

 ObjHdr->RegionIndex      = 0;
 ObjHdr->RegionCount      = 0;
 ObjHdr->ExObjectCount    = 0;
 ObjHdr->NextEx           = 0;
 ObjHdr->ObjectCount      = 0;
 ObjHdr->NextObject       = 0;

 if (Normal(*Status))
 {
   ObjHdr->NextEntry      = 0;
   ObjHdr->BaseIndex      = 0;
   ObjHdr->BaseRegion     = 0;

   ObjHdr->CacheLimit     = CacheLimit;
   ObjHdr->CacheLoad      = CacheLoad;
   ObjHdr->NumBuffRgns    = CacheLimit;

   if (Anchor->DbId      == RenvId)
      ObjHdr->FreezeClass = 1;

   GrpMakeHdrChunks   (Anchor,              (grpheader *)ObjHdr,  
                       AllocRegions,  AllocExObjects,    McStat);
 }

 if (Normal(*Status))
 {
   if (MemNewBitChunk
                    (*AllocObjects,        CoreDbId,
                     McStat,                     &(ObjHdr->ObjectLockDir)))
   if (MemMakeChunk (sizeof(localattrnum),        Alloc_DbAttrs,
                     Extend_DbAttrNums,           CoreDbId,
                     McStat,                     &(ObjHdr->FieldNumMap)))
   if (MemPutStackPtr
                    (ObjHdr->FieldNumMap,         Alloc_DbAttrs,  McStat)) 


   if (MemMakeChunk (sizeof(localattrnum),        Alloc_DbAttrs,
                     Extend_DbAttrNums,           CoreDbId,
                     McStat,                     &(ObjHdr->LocalAttrNumMap)))

   if (MemMakeChunk (sizeof(typesize),            Alloc_Fields, 
                     Extend_Fields,               CoreDbId,
                     McStat,                     &(ObjHdr->FieldOffsetDir)))

   if (MemMakeChunk (sizeof(typesize),            Alloc_Fields, 
                     Extend_Fields,               CoreDbId,
                     McStat,                     &(ObjHdr->ClassOffsetDir)))

   if (MemMakeChunk (sizeof(reftkntype),          Alloc_FieldRefs, 
                     Extend_FieldRefs,            CoreDbId,
                     McStat,                     &(ObjHdr->FieldRefDir)))

   if (MemPutStackPtr (ObjHdr->LocalAttrNumMap, (indextype  )1, McStat)) 
   if (MemPutStackPtr (ObjHdr->FieldOffsetDir,  (indextype  )1, McStat)) 
   if (MemPutStackPtr (ObjHdr->ClassOffsetDir,  (indextype  )1, McStat)) 
   if (MemPutStackPtr (ObjHdr->FieldRefDir,     (indextype  )1, McStat)) 
   {
      ObjHdr->HandleOffset  = 0;
      ObjHdr->StringCount   = 0;
      ObjHdr->ExObjectCount = 0;
      ObjHdr->NextEx        = 0;
   }
 }

TRACK(TrackBak,"OmMakeObjChunks\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmPutHandleOffset (tokentype *Token,  numtype   EnvObj,
                              bytessize  Offset, ft F,lt Z,zz *Status)
{ 
/* this is a ADMIN ONLY procedure call from Build/Make_ObjDesc             */ 
/* Define position of Attr "Handle"  within OBJ struct template            */

dbheader    *CoreDb   = NullPtr;
objheader   *ObjHdr   = NullPtr;
indextype    LocalObj = 0;

  if (TmGetCoreDb  (Token,                  McStat,     &CoreDb))
  if (MemGetObjNum (CoreDb->LocalObjNumMap, EnvObj,      McStat, &LocalObj))
  if (MemGetObjHdr (CoreDb->ObjHdrDir,      LocalObj,
                    McStat,                 (addrtype *)&ObjHdr))
     ObjHdr->HandleOffset = Offset;

  if (DeBug >= 2)
/* "    Set ObjHdr[%3d]->HandleOffset = %2u\n" */
  if (sprintf (Msg, TraceStr(6),
               EnvObj, Offset))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OmPutHandleOffset\n");
return(STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
boolean    OmGetObjHdr (tokentype    *Anchor,   indextype     ObjIndex,
                        ft F,lt Z,zz *Status,   objheader   **ObjHdr)
{
/* ObjIndex is the Local ObjNum of an ObjectClass in a Db Schema           */

dbheader    *CoreDb   = NullPtr;
vbntype      Vbn      = 0;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetObjHdr  (CoreDb->ObjHdrDir, ObjIndex, McStat, (addrtype *)ObjHdr))
  {
     if (*ObjHdr == NullPtr)
     {
        if(CoreDb->ObjVbnDirVbn)
        if (MemGetVbn     (CoreDb->ObjVbnDir,               ObjIndex,
                           McStat,                         &Vbn))          
        if (DbmLoadObjHdr (Anchor, ObjIndex,   McStat,      ObjHdr))
        if (CoreDb->DbId == RenvId)
/*
//        &&  Admin_Run    == True)    
*/
           (*ObjHdr)->FreezeClass  = 1;
        else
          OmChkImage      (Anchor,  (*ObjHdr)->ObjNum,    McStat);
     }

  } else {
    *Status = Om_ObjHdrNotExist;
/* " Error in OmGetObjHdr:: DbId= %3u; Object_Index=%6u\n" */
     sprintf  (Msg, TraceStr(8), 
               Anchor->DbId, ObjIndex);
     TraceMsg (0,   Msg);
  }

TRACK(TrackBak,"OmGetObjHdr\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmNextAvailObject (objheader *ObjHdr,   ft F,lt Z,zz *Status,
                             indextype  *ObjIndex, indextype    *RgnNum,
                             indextype  *RgnIndex)
{
indextype    StackPtr = 0;
 if (ObjHdr->ExObjectCount)
 {
    if (MemPopWord (ObjHdr->ExObjectTbl, 
                    McStat,             &StackPtr,      ObjIndex))
    {
        ObjHdr->ExObjectCount--;
       *RgnNum   =  *ObjIndex / ObjHdr->RegionObjects;
       *RgnIndex =  *ObjIndex % ObjHdr->RegionObjects;
    }

 } else {
   *Status     =  Om_NewIndex;
   *ObjIndex   =  ObjHdr->ObjectCount++;
   *RgnNum     = *ObjIndex / ObjHdr->RegionObjects;
   *RgnIndex   = *ObjIndex % ObjHdr->RegionObjects;

 }

TRACK(TrackBak,"OmNextAvailObject\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmNewObject (dbheader     *CoreDb,  objheader  *ObjHdr,
                        ft F,lt Z,zz *Status,
                        indextype    *Index,   addrtype   *Object)
{
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
indextype       RgnIndex     = 0;
char           *Base         = 0;
indextype       Inset        = 0;
addrtype        Ptr          = 0;
 
 if (GrpNewObject      (CoreDb,  ObjHdr,
                        McStat,  Index,  &RgnIndex,  &ChunkAddr))
 {
    LocalIndex       = (*Index  - ObjHdr->BaseIndex) % ObjHdr->RegionObjects;

    Base             = (char *)ChunkAddr;
    Inset            = LocalIndex           * ObjHdr->ObjSize;
    Ptr              = (addrtype )((char *)Base     + Inset);
    *(byte **)Object = ((byte *)ChunkAddr)  + (LocalIndex * ObjHdr->ObjSize);

    if ((addrtype )Ptr != *Object)
       *Status = Om_BadAddr;

    if (!Normal(*Status) || DeBug || OaBug || DbmBug)
    if (sprintf (Msg,
/* "    OmNewObject[%4u] (%4u) RgnChunk %2d   @ %x\n" */
             TraceStr(11),
             *Index, LocalIndex, Chunk, *Object))
       TraceMsg (0, Msg);
 } 

TRACK(TrackBak,"OmNewObject\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmNewObjRegion (dbheader     *CoreDb,  objheader   *ObjHdr,
                           indextype     Index, 
                           ft F,lt Z,zz *Status,  numtype     *Region)
{
indextype   SwapRgn    = 0;
indextype   SwapVbn    = 0;
numtype     Chunk      = 0;
indextype   StackPtr   = 0;
indextype   RgnObjects = 0;
indextype   RgnIndex   = Index  / ObjHdr->RegionObjects;

 if (RgnIndex != ObjHdr->RegionIndex)
 if (ObjHdr->FreezeClass == 0)
 {
 /* then have swap regions                                                 */
    if (MemGetWord (ObjHdr->RegionChunkDir,  ObjHdr->RegionIndex,
                    McStat,                 &SwapRgn))
    if (MemGetWord (ObjHdr->RegionSwapDir,   ObjHdr->RegionIndex,
                    McStat,                 &SwapVbn))

    if (Normal(*Status)) 
    if (SwapRgn)
    if (MemGetStackPtr     (SwapRgn, McStat, &RgnObjects))
    if (RgnObjects)
    {
       OmUnPairRgnObjects (CoreDb,  ObjHdr,  SwapRgn,    McStat);
    } else {
       sprintf (Msg, 
/* "  CurrentRegion %3u is EMPTY; swaps for NEW Region[%u]\n" */
               TraceStr(15),
               ObjHdr->RegionIndex, Region);
       TraceMsg (0, Msg);
      *Status = Om_BadSwapChunks;
    }

    if (Normal(*Status)) 
    if (SwapRgn)
    if (MemSwapOutChunk (SwapRgn,                McStat, &SwapVbn))
        MemPutChunkNum  (ObjHdr->RegionSwapDir,  ObjHdr->RegionIndex,
                         SwapVbn,                        McStat);
 }

 /* Create the Region Space for ALLOC objects                         */
 if (Normal(*Status)) 
 if (MemMakeChunk    (ObjHdr->ObjSize,   ObjHdr->RegionObjects, 
                      Extend_Objects,    CoreDb->DbId,
                      McStat,           &Chunk))
 if (MemPushChunkNum (ObjHdr->RegionChunkDir, Chunk, McStat, &StackPtr))
 if (MemPushChunkNum (ObjHdr->RegionSwapDir,  (vbntype )0,
                      McStat,                                &StackPtr))
 {
   *Region                  = (regionnum )StackPtr;
    ObjHdr->NoRegionsInCore = False;
    ObjHdr->RegionIndex     = *Region;
    ObjHdr->RegionCount++;
    if (RgnIndex           != *Region) 
       *Status              = Om_BadSwapChunks;

    if (!Normal(*Status)   ||  SwapBug || DeBug)
/* "      NEW Region[%d] ObjectIndex[%4u] Chunk= %3d\n" */
    if (sprintf (Msg, TraceStr(16),
                 *Region,  Index, Chunk))
    if (TraceMsg (0, Msg))
       DumpChunkChunk (0, Chunk);
 }

TRACK(TrackBak,"OmNewObjRegion\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmPairDbObject (objheader *ObjHdr,   indextype     Index,
                           handletype Handle,   ft F,lt Z,zz *Status)
{
indextype   RegionNum   = Index / ObjHdr->RegionObjects;
indextype   RegionIndex = Index % ObjHdr->RegionObjects;
numtype    RegionChunk = 0;
byte       *RegionAddr  = NullPtr;
handletype *HandlePtr   = NullPtr;
indextype   Offset      = 0;

 if (MemGetChunkNum  (ObjHdr->RegionChunkDir,                 RegionNum, 
                      McStat,                                &RegionChunk))
 if (MemGetChunkAddr (RegionChunk,     McStat,   (addrtype *)&RegionAddr))
 {
    Offset     =  (ObjHdr->ObjSize * RegionIndex) + ObjHdr->HandleOffset;
    RegionAddr =  RegionAddr + Offset;
    HandlePtr  =  (handletype *)RegionAddr;
   *HandlePtr  =  Handle;
 }

 if (! HandlePtr)
    *Status = Om_BadHandleObjPair;

 if (!Normal(*Status) || SwapBug || SwapHandles)
 if (sprintf (Msg,
/* "   Om Pair Object<--Handle :: Region[%2u](%2u)+%2u = %4u @%x Offset %u\n" */
    TraceStr(17),
        RegionNum, RegionIndex, ObjHdr->HandleOffset,
       *HandlePtr, HandlePtr,   Offset))
    TraceMsg (1, Msg);

TRACK(TrackBak,"OmPairDbObject\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmUnPairRgnObjects (dbheader  *CoreDb,       objheader    *ObjHdr,
                               numtype    CurrRgnChunk, ft F,lt Z,zz *Status)
{ 
indextype   Index         = 0;
indextype   Offset        = 0;
indextype   Objects       = 0;
byte       *RegionAddr    = NullPtr;
handletype *HandlePtr     = NullPtr;
addrtype    NullAddrValue = NullPtr;
int         TempDeBug     = DeBug;
booleantype TempMemBug    = MemBug;
numtype     MemLoc        = 0;

 if (!UnPairObjects)
 {

   TRACK(TrackBak,"OmUnPairRgnObjects\n");
   return (STAT);
 }

 if (MemGetChunkAddr (CurrRgnChunk,          McStat, (addrtype *)&RegionAddr))
     MemGetStackPtr  (CurrRgnChunk,          McStat, &Objects);

 if (!RegionAddr || !Objects)
 {
     *Status = Om_BadSwapChunks;
/* " BAD UNPAIR Handles for RegionChunk[%3u] @%x count %4u\n" */
     sprintf (Msg, TraceStr(19),
              CurrRgnChunk, RegionAddr, Objects);
     TraceMsg (0, Msg);
 }


 if (SWAP)
 {
   DeBug = 0;
   MemBug = False;
   while (Normal(*Status) && Index < Objects)
   {
     Offset     = (ObjHdr->ObjSize * Index) + ObjHdr->HandleOffset;
     HandlePtr  = (handletype *)(RegionAddr + Offset);

     if (*HandlePtr && *HandlePtr < CoreDb->HandleCount) 
     {
        if (HmGetObjectMemLoc (CoreDb,  *HandlePtr, McStat, &MemLoc))
        if (MemLoc == 0)
            HmPutObjectAddr (CoreDb, *HandlePtr, NullAddrValue, McStat); 
     }
     if (SwapBug)
        *Status = Env_Normal;

     ++Index;
   }

   DeBug  = TempDeBug;
   MemBug = TempMemBug;
 }

TRACK(TrackBak,"OmUnPairRgnObjects\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmPairRgnObjects   (dbheader  *CoreDb,       objheader    *ObjHdr,
                               numtype    CurrRgnChunk, ft F,lt Z,zz *Status)
{ 
indextype   Index         = 0;
indextype   Offset        = 0;
indextype   Objects       = 0;
byte       *RegionAddr    = NullPtr;
handletype *HandlePtr     = NullPtr;
addrtype    ObjectAddr    = NullPtr;
int         TempDeBug     = DeBug;
booleantype TempMemBug    = MemBug;
numtype     MemLoc        = 0;

 if (!PairObjects)
 {
    TRACK(TrackBak,"OmPairRgnObjects\n");
    return (STAT);
 }

 if (MemGetChunkAddr (CurrRgnChunk,          McStat, (addrtype *)&RegionAddr))
     MemGetStackPtr  (CurrRgnChunk,          McStat, &Objects);

 if (!RegionAddr || !Objects)
 {
    *Status = Om_BadSwapChunks;
/* " BAD PAIR Handles for RegionChunk[%3u] @%x count %4u\n" */
     sprintf (Msg, TraceStr(25),
              CurrRgnChunk, RegionAddr, Objects);
     TraceMsg (0, Msg);
 }


 if (SWAP)
 {
   DeBug = 0;
   MemBug = False;
   while (Normal(*Status) 
   &&     Index < Objects)
   {
     Offset     = (ObjHdr->ObjSize * Index) + ObjHdr->HandleOffset;
     ObjectAddr =  (addrtype )(RegionAddr + (ObjHdr->ObjSize * Index));
     HandlePtr  = (handletype *)(RegionAddr + Offset);

     if (*HandlePtr)
     {
        if (HmGetObjectMemLoc (CoreDb,  *HandlePtr, McStat, &MemLoc))
        if (MemLoc == 0)
            HmPutObjectAddr   (CoreDb,  *HandlePtr, ObjectAddr, McStat); 
        if (!Normal(*Status))
        if (sprintf (Msg, 
/* " Region[%3u] Object[%3u] Handle= %4u\n" */
              TraceStr(29),
                CurrRgnChunk,   Index, *HandlePtr))
           TraceMsg (0, Msg);

     }
     ++Index;
   }
   MemBug = TempMemBug;
   DeBug = TempDeBug;
 }

TRACK(TrackBak,"OmPairRgnObjects\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmGetRegion (dbheader     *CoreDb,  objheader *ObjHdr,
                        indextype     Region,
                        ft F,lt Z,zz *Status,  numtype   *Chunk)
{
*Status = Err_BadDesign;
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    OmGetObject  (tokentype   *Token,  ft F,lt Z,zz *Status,
                         indextype   *Index,  addrtype     *Object)
{
dbheader     *CoreDb    = NullPtr;
localnumtype  LocalObj  = 0;
objheader    *ObjHdr    = NullPtr;
numtype      RgnNum    = 0;
addrtype      ChunkAddr = NullPtr;
handletype    Handle    = Token->Handle;
indextype     LocalIndex= 0;
numtype       ObjNum    = 0;

  if (TmGetCoreDb      (Token,  McStat,    &CoreDb))
  if (HmGetLocalObjNum (CoreDb, Handle,     McStat,   &LocalObj))
  if (LocalObj     == Null_ObjNum)
     *Status = Om_ObjectHasBeenDeleted;

  if (Normal(*Status))
  if (OmGetObjHdr      (Token,  LocalObj,   McStat,   &ObjHdr))
  if (HmGetObjectIndex (CoreDb, Handle,     McStat,    Index))
  {
     ObjNum     = ObjHdr->ObjNum;
     RgnNum     = *Index  / ObjHdr->RegionObjects;
     LocalIndex = *Index  % ObjHdr->RegionObjects;
  }

  if (Normal(*Status))
     GrpGetObject         (CoreDb,          ObjHdr,    *Index, 
                           McStat,                     &ChunkAddr);

  if (Normal(*Status))
  if (ChunkAddr)
     *(byte **)Object = ((byte *)ChunkAddr) + (LocalIndex * ObjHdr->ObjSize);
  else
     *Status = Om_BadRegion;

  if (!Normal(*Status) || OaBug || DeBug)
  if (sprintf (Msg, 
/* "   OmGetObject[%2u:%4u]:: @%x Class[%3u] of Region[%2u:%2u]\n" */
              TraceStr(46),
              Token->DbId, Token->Handle,
             *Object, ObjNum,  RgnNum, LocalIndex))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OmGetObject\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
boolean    OmDeleteObject   (tokentype    *Token,   objheader    *ObjHdr,
                             addrtype      Object,  ft F,lt Z,zz *Status)
{  /* error free                                                           */
dbheader       *CoreDb          = NullPtr;
indextype       ObjIndex        = 0;

 if (TmGetCoreDb       (Token,         McStat,            &CoreDb))
 if (HmGetObjectIndex  (CoreDb,        Token->Handle,
                        McStat,       &ObjIndex))
#ifdef   __SWAP_GRP_POS__
 if (Grp_DeleteObject  (CoreDb,    ObjHdr,    ObjIndex,    McStat))
     TmFreeToken       (McStat,                            Token);

#else
 if (MemPushWord       (ObjHdr->ExObjectTbl,               ObjIndex,
                        McStat,                           &StackPtr))
 if (TmFreeToken       (McStat,                            Token))
     ObjHdr->ExObjectCount++;

/* ZERO the core space within the region                                   */
 if (Normal(*Status))
    memset (Object,  0, (size_t)ObjHdr->ObjSize);

#endif /* __SWAP_GRP_POS__  */

TRACK(TrackBak,"OmDeleteObject\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
boolean OmChkImage       (tokentype     *Anchor,   numtype    ObjNum,
                          ft F,lt Z,zz  *Status)
{
dbheader     *CoreDb          = NullPtr;
numtype       LocalObj        = 0;
objheader    *ObjHdr          = NullPtr;
time_t        ObjTod          = 0;
time_t        TknTod          = 0;

 if (TmGetCoreDb       (Anchor,  McStat,          &CoreDb))

 if (OaBug || DeBug)
/* "   OmChkImage of ObjHdr(%3u) in DbId = %3u\n" */
 if (sprintf  (Msg, TraceStr(47), 
               ObjNum, Anchor->DbId))
     TraceMsg (0,  Msg);

 if (MemGetLong        (CoreDbs[RenvId]->ObjTimeStampDir,
                        (indextype )ObjNum,  
                        McStat,                  (lt64     *)&ObjTod))

 if (MemGetObjNum      (CoreDb->LocalObjNumMap,   ObjNum,  
                        McStat,                  &LocalObj))
 if (MemGetLong        (CoreDb->ObjTimeStampDir,  LocalObj,
                        McStat,                  (lt64     *)&TknTod))

 if (OmGetObjHdr       (Anchor,      LocalObj,     McStat,  &ObjHdr))

 if (Normal(*Status))
 if (ObjHdr->ObjSize != ObjHdrSize[ObjNum])
/* "  from DB_ObjHdr Time: %s   To RENV_ObjDesc Time %s\n" */
 if (sprintf  (Msg, TraceStr(48), 
               ctime (&TknTod), ctime (&ObjTod) ))
 if (TraceMsg (0,  Msg))
/* "       DB_ObjHdr Size= %3u;  Renv ObjDesc Size=%3u\n" */
 if (sprintf  (Msg, TraceStr(49), 
               ObjHdr->ObjSize, ObjHdrSize[ObjNum]))
 if (TraceMsg (0,  Msg))
 {

    *Status   = Err_NotImplemented;

 }

TRACK(TrackBak,"OmChkImage\n");
return (STAT);
}




boolean    OmGetObjHandles
                          (tokentype    *Anchor,  objheader   *ObjHdr,
                           ft F,lt Z,zz *Status,  
                           indextype    *Count,   handletype **Handles,
                           indextype    *RgnCount,numtype    **RgnStatus)
{
dbheader     *CoreDb        = NullPtr;
bytessize     CoreBlkSize   = 0;
bytessize     RgnBlkSize    = 0;
numtype       RgnChunk      = 0;
indextype     RgnIndex      = 0;
indextype     Index         = 0;
indextype     HndlCount     = 0;
indextype     RgnOffset     = 0;
addrtype      RgnAddr       = NullPtr;
int           Items         = 0;
addrtype      ObjectAddr    = NullPtr;
handletype   *ObjHndls      = NullPtr;
numtype      *Rgn_Status    = NullPtr;

 *Handles       = NullPtr;
 *RgnStatus     = NullPtr;

  if (TmFetchCoreDb     (Anchor,           McStat,    &CoreDb))
    
  if (ObjHdr->ObjectCount)
  if (Normal(*Status))
  {
     CoreBlkSize  = ObjHdr->ObjectCount * sizeof (handletype);

     if (CoreMoreCore      (CoreBlkSize,             McStat,
                            (addrtype *)Handles))
     if (MemGetStackPtr    (ObjHdr->RegionSwapDir,   McStat,   RgnCount))
     {
         ObjHndls     = *Handles;
         RgnBlkSize   = *RgnCount       * sizeof(numtype);
         if (CoreMoreCore (RgnBlkSize,   McStat,  (addrtype *)RgnStatus))
             Rgn_Status   = *RgnStatus;
     }

     while (Normal(*Status)
     &&     RgnIndex       < *RgnCount)
     {
        Rgn_Status[RgnIndex]  = 0;

        if (OmGetRegion     (CoreDb,      ObjHdr,        RgnIndex,
                             McStat,     &RgnChunk))
        if (MemGetChunkAddr (RgnChunk,    McStat,       &RgnAddr))
        if (ClassBug || OaBug || DeBug)
        if (sprintf (Msg, 
/* "\n    Obj Region[%2u]  Chunk[%2u]  :  @ %X\n" */
                    TraceStr(64),
                    RgnIndex, RgnChunk,  RgnAddr))
           TraceMsg (0, Msg);

        Index       = 0;
        RgnOffset   = 0;

        while (Normal(*Status)
        && ++Index     <= ObjHdr->RegionObjects
        && ++HndlCount <= ObjHdr->ObjectCount)
        {
          ObjectAddr            = (char *)RgnAddr    + RgnOffset;
          ObjectAddr            = (char *)ObjectAddr + ObjHdr->HandleOffset;
          ObjHndls[HndlCount]   = *(handletype *)ObjectAddr;
          RgnOffset            += ObjHdr->ObjSize;
          if (*(handletype *)ObjectAddr)
             Rgn_Status[RgnIndex]++;

          if (OaBug || DeBug)
          {
             if (++Items % 6 == 0)
/* "   %-7u\n" */
                sprintf  (Msg,  TraceStr(65), ObjHndls[HndlCount]);
             else 
/* "   %-7u  " */
                sprintf  (Msg,  TraceStr(66), ObjHndls[HndlCount]);
             TraceMsg (0, Msg);
          }
        }
        if (ClassBug || OaBug || DeBug)
/* "   RgnStatus[%3u] =%4u\n" */
        if (sprintf  (Msg,  TraceStr(67), Rgn_Status[RgnIndex]))
            TraceMsg (0, Msg);

      ++RgnIndex;
     }

  } else
     TraceMsg (0, " No Current Objects Of this OBJ type in this DB\n");

 *Count = HndlCount;

TRACK(TrackBak,"OmGetObjHandles\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF OM.C                                    !
  +-----------------------------------------------------------------------+*/
