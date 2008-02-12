/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - OA0.c */
#define OA0
#include <time.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "oa0.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...MEM00... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[21] =
{
   "   OaInstallObjHdr(%3u) in DbId = %3u\n",
   "   InstallObjHdr Db[%2u] Obj[%3u] Local?[%3u] ObjHdr @%x McStat=%d\n",
   "  OaInstallObjHdr[%2u] as ObjNum=%2u In Db=%2u @%x\n",
   "   GET ObjClass from [%3u:%3u] ::=  %u \n",
   "   OaFreezeObjHdr(%3u) in DbId = %3u\n",
   "   ObjHdr[%3u:%4u] Frozen %2u times.\n",
   "   OaThawObjHdr(%3u) in DbId = %3u\n",
   "   OaCreateObject(%3u) in DbId = %3u\n",
   "   CreateObject Db[%2u] Obj[%3u] Local?[%3u] ObjHdr @ %x McStat= %d\n",
   "  OaCreateObject[%2u] Handle =%4u for Db=%2u @%x\n",
   "  CREATE Object (* %3u) ::  NewObjHdr[%u] [%2u:%4u]\n",
   "  OaCreateObject[type= %2u] :: [%2u:%4u]\n",
   "    OA New  Object(%4u) [%2u:%4u]  of Class Obj[%2u]\n",
   "  OaGetObject[%2u :%4u], McStat= %d\n",
   "  Oa GET Object[%2u :%4u] @%x, McStat= %d\n",
   "  OA Revoke C++ Object [%2u:%4u]\n",
   "   Oa Assign C++ Object @%x TO Image[%2u:%4u] \n",
   "   OaUpdateObject:: [%3u:%4u] ObjNum=%3u Index=%4u Region[%2u:%2u]\n",
   "   OaCHkObjHdr(%3u) in DbId = %3u\n",
   "  from DB_ObjHdr Time: %s   To RENV_ObjDesc Time %s\n",
   "   OaChange  [%2u:%4u] To Obj[%3u]\n"
};
#endif

/*+-----------------------------------------------------------------------+*
~OA0                         EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
extern boolean DbmCreateDb
                          (tokentype    *SchemaTkn, numtype       PrimalObj,
                           char         *DbName,    char         *NewFileName,
                           dbaccesstype  DbAccess,  indextype     AllocObjs,
                           indextype     AllocAttrs,
                           ft F,lt Z,zz *Status,    tokentype    *DbPrimal);

extern boolean EnvInstallObjHdr
                          (tokentype    *Anchor,    numtype       EnvObjNum,
                           ft F,lt Z,zz *Status,    numtype      *LocalObj);

extern boolean EnvFetchObjNum(tokentype    *ObjTkn,
                           ft F,lt Z,zz *Status,    numtype      *ObjNum);

extern boolean DbmLoadObjHdr
                          (tokentype    *DbToken,   numtype       LocalObj,
                           ft F,lt Z,zz *Status,    objheader   **ObjHdr);

#define MemPushObjHdr        Mem_PushAddr
#define MemGetObjHdr         Mem_GetAddr

#define MemGetLocalObjNum    Mem_GetWord
#define MemPutLocalObjNum    Mem_PutWord

#define MemGetObjNum         Mem_GetWord
#define MemPushObjNum        Mem_PushWord

#define MemGetVbn            Mem_GetWord


#ifdef __ZTC__
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#else

#ifdef MCSTAT
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (Anchor->DbId <= MaxCoreDbId \
 && ((*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr)) ? True : \
     TmFetchCoreDb(Anchor,Stat,DbInCore)
#else
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#endif
#endif

/*+-----------------------------------------------------------------------+*
~OA0                          OBJECT ACTOR                                !
 *+-----------------------------------------------------------------------+*/

boolean    OaCreateDb     (tokentype    *SchemaTkn, numtype       PrimalObj,
                           char         *DbName,    char         *NewFileName,
                           dbaccesstype  DbAccess,
                           ft F,lt Z,zz *Status,    tokentype    *DbPrimal)
{
indextype   AllocObjs       = CoreDb0->LastEnvObjCount  + 1;
indextype   AllocAttrs      = CoreDb0->LastEnvAttrCount + 1;
int         TmpDeBug        = DeBug;

   if (SchemaTkn->DbId == PrimalId)
   {
      AllocObjs  = Env_Objs;
      AllocAttrs = Env_Attrs;
   } else {
      AllocObjs  = CoreDb0->LastEnvObjCount  + 1;
      AllocAttrs = CoreDb0->LastEnvAttrCount + 1;
   }

   DbmCreateDb   (SchemaTkn,         PrimalObj,
                  DbName,            NewFileName,       DbAccess,
                  AllocObjs,         AllocAttrs,
                  McStat,            DbPrimal);

   DeBug = TmpDeBug;

TRACK(TrackBak,"OaCreateDb\n");
return (STAT);
}


boolean    OaInstallObjHdr (tokentype    *Anchor,   numtype     EnvObj,
                            ft F,lt Z,zz *Status,   objheader **ObjHdr)
{
localnumtype LocalObj        = 0;
dbheader    *CoreDb          = NullPtr;
vbntype      Vbn             = 0;

 if (TmGetCoreDb       (Anchor,                 McStat,  &CoreDb))
 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap, EnvObj,   McStat, &LocalObj))
 if (LocalObj)
 {
    if (MemGetObjHdr   (CoreDb->ObjHdrDir,      LocalObj,
                        McStat,                 (addrtype *)ObjHdr))
    if (*ObjHdr == NullPtr)
    if (CoreDb->ObjVbnDirVbn)
    {
       if (MemGetVbn   (CoreDb->ObjVbnDir,      LocalObj,
                        McStat,                &Vbn))
       if (Vbn)
       {
          if (DbmLoadObjHdr (Anchor, LocalObj,      McStat,     ObjHdr))
              OmChkImage    (Anchor,     (*ObjHdr)->ObjNum,     McStat);
       } else
          *Status = Oa_NewObjHdr;
    } else
       *Status = Oa_NewObjHdr;
 }

 if ( (! LocalObj && EnvObj)
 ||   *Status == Oa_NewObjHdr)
 {
    *Status = Env_Normal;

    /* Get The New Copy of a ObjHdr and its allocated Chunks               */
    if (! Build_Env)
    {
       if (EnvInstallObjHdr (Anchor, EnvObj, McStat, &LocalObj))
           MemGetObjHdr     (CoreDb->ObjHdrDir,      LocalObj,
                             McStat,                 (addrtype *)ObjHdr);
    } else
       *Status = Om_ObjNotInSchema;
 }

TRACK(TrackBak,"OaInstallObjHdr\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean    OaGetObjClass  (tokentype    *Token,
                           ft F,lt Z,zz *Status,     numtype    *ObjClass)
{
dbheader     *CoreDb    = NullPtr;

  if (TmGetCoreDb      (Token,  McStat,         &CoreDb))
  if (HmGetObjNum      (CoreDb, Token->Handle,   McStat,     ObjClass))

  if (!Normal(*Status) || DeBug || OaBug)
  {
/* "   GET ObjClass from [%3u:%3u] ::=  %u \n" */
     sprintf (Msg, TraceStr(3),
              Token->DbId, Token->Handle,  *ObjClass);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"OaGetObjClass\n");
return (STAT);
}


boolean    OaConfigClass   (tokentype    *PrimalTkn, numtype       ClassId,
                            numtype       RgnEntrys,
                            numtype       AllocRgns, numtype       ExtendRgns,
                            numtype       AllocXs,   numtype       ExtendXs,
                            accesstype    AccessType,boolean       DiskCache,
                            numtype       CacheLimit,numtype       CacheLoad,
                            ft F,lt Z,zz *Status)
{
objheader    *ObjHdr    = NullPtr;

/* does autmatic set of Config_Class := True;                              */
  OmConfigClass        (RgnEntrys,    AllocRgns,  ExtendRgns,
                        AllocXs,      ExtendXs,   AccessType,
                        DiskCache,    CacheLimit, CacheLoad);

  OaInstallObjHdr      (PrimalTkn,    ClassId,    McStat,     &ObjHdr);

/* reset                Config_Class := False;                             */
  OmSetConfig          (False);

TRACK(TrackBak,"OaConfigClass\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
boolean    OaFreezeObjClass(tokentype    *Anchor,   numtype     EnvObj,
                            ft F,lt Z,zz *Status)
{
objheader   *ObjHdr          = NullPtr;
dbheader    *CoreDb          = NullPtr;
localnumtype LocalObj        = 0;

 if (OaInstallObjHdr   (Anchor,   EnvObj,  McStat,  &ObjHdr))
     ObjHdr->FreezeClass++;

TRACK(TrackBak,"OaFreezeObjClass\n");
return (STAT);
}


boolean    OaThawObjClass  (tokentype    *Anchor,   numtype     EnvObj,
                            ft F,lt Z,zz *Status)
{
objheader   *ObjHdr          = NullPtr;
localnumtype LocalObj        = 0;
dbheader    *CoreDb          = NullPtr;

 if (TmGetCoreDb       (Anchor,                 McStat,           &CoreDb))
 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap, EnvObj,   McStat, &LocalObj))
 if (LocalObj)
 if (MemGetObjHdr      (CoreDb->ObjHdrDir,                         LocalObj,
                        McStat,                       (addrtype *)&ObjHdr))
 if (ObjHdr == NullPtr)
   *Status    = Oa_ObjClassNotLoaded;

 else if (ObjHdr->FreezeClass)
    ObjHdr->FreezeClass--;

TRACK(TrackBak,"OaThawObjClass\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
boolean    OaCreateObject (tokentype    *Anchor, numtype     EnvObj,
                           ft F,lt Z,zz *Status, tokentype  *Token )
{
handletype  Handle          = 0;
indextype   Index           = 0;
localnumtype LocalObj        = 0;
addrtype    Object          = NullPtr;
dbheader   *CoreDb          = NullPtr;
objheader  *ObjHdr          = NullPtr;
count       AllocQty        = 0;
indextype   StackPtr        = 0;
vbntype     Vbn             = 0;
boolean     TempOaBug       = OaBug;

 if (TmGetCoreDb       (Anchor,                 McStat,  &CoreDb))
 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap, EnvObj,   McStat, &LocalObj))
 if (LocalObj)
 {
    if (MemGetObjHdr   (CoreDb->ObjHdrDir,      LocalObj,
                        McStat,                 (addrtype *)&ObjHdr))
    if (ObjHdr == NullPtr)
    if (CoreDb->ObjVbnDirVbn)
    {
       if (MemGetVbn   (CoreDb->ObjVbnDir,      LocalObj,
                        McStat,                &Vbn))
       if (Vbn)
       {
          if (DbmLoadObjHdr (Anchor, LocalObj,  McStat,     &ObjHdr))
              OmChkImage    (Anchor,    ObjHdr->ObjNum,      McStat);
       } else
          *Status = Oa_NewObjHdr;
    } else
       *Status = Oa_NewObjHdr;
 }

 if ( (! LocalObj && EnvObj)
 ||   *Status == Oa_NewObjHdr)
 {
    *Status = Env_Normal;
    if (ObjAllocQty[EnvObj] > 0)
       AllocQty = ObjAllocQty[EnvObj];
    else
       AllocQty = Alloc_ObjectObjs;

    if (!LocalObj)
    if (Build_Env
    ||  (Admin_Run && Anchor->DbId == RenvId))
    {
       if (MemPushObjNum (CoreDb->ObjNumMap,  EnvObj,  McStat, &LocalObj))

       if (MemPushObjHdr (CoreDb->ObjHdrDir,  NullPtr, McStat, &StackPtr))
       if (MemPushWord   (CoreDb->ObjVbnDir,  0,       McStat, &StackPtr))
          ++(CoreDb->LocalObjCount);
       if (Normal(*Status))
           MemPutLocalObjNum (CoreDb->LocalObjNumMap,  EnvObj,
                              LocalObj,                McStat);
    }

    if (! Build_Env)
    {
       if (EnvInstallObjHdr (Anchor, EnvObj, McStat, &LocalObj))
          HmNewHandle (CoreDb,  LocalObj, McStat,  &Handle);
    } else if (OmNewObjHdr (Anchor,  EnvObj, AllocQty, McStat, &ObjHdr)) {
       ObjHdr->ObjSize = ObjHdrSize[EnvObj];
       if (ObjHdr->ObjSize == 0)
          *Status = Om_ObjNotInSchema;
    } else
       *Status = Om_ObjNotInSchema;
 } else {
    HmNewHandle (CoreDb,  LocalObj, McStat,  &Handle);
 }

 if (Normal(*Status))
 if (TmMakeToken    (Anchor,  Handle,   McStat,   Token))
 if (OaNewObject    (Token,   LocalObj, McStat,  &Index,  &Object))
     HmPairDbObject (CoreDb,  Handle,   Index,    Object,  McStat);

TRACK(TrackBak,"OaCreateObject\n");
return (STAT);
}


boolean    OaNewObject (tokentype    *Anchor,    localnumtype LocalObj,
                        ft F,lt Z,zz *Status,    indextype   *Index,
                        addrtype     *Object)
{
objheader    *ObjHdr         = NullPtr;
handletype   *HandlePtr      = NullPtr;
cppobjtype   *CppObject      = NullPtr;
dbheader     *CoreDb         = NullPtr;

 if (TmGetCoreDb    (Anchor,    McStat,   &CoreDb))
 if (OmGetObjHdr    (Anchor,    LocalObj,  McStat,        &ObjHdr))
 if (ObjHdr == NullPtr)
    *Status = Oa_NullObjHdr;

 if (Normal(*Status))
 if (OmNewObject    (CoreDb,   ObjHdr,    McStat,          Index,     Object))
 if (OmPairDbObject (ObjHdr,  *Index,     Anchor->Handle,  McStat))
 {
   if (ObjHdr->EnvObjType != That_AttrNum)
   {
      HandlePtr = *(handletype **)Object;
     *HandlePtr = Anchor->Handle;

   } else {
     CppObject           = *(cppobjtype **)Object;

#ifdef IMAGE_WITH_THAT
     CppObject->That     = *Object;
#endif /* IMAGE_WITH_THAT  */

     CppObject->Handle   = Anchor->Handle;
     CppObject->DbId     = Anchor->DbId;
     CppObject->CoreDbId = Anchor->CoreDbId;
   }
 }

 if (! Normal(*Status))
 {
    sprintf (Msg,
             "\n *** ERROR *** at OaNewObject::LocalObj[%3u] in Db %2u\n",
             LocalObj, Anchor->DbId);
    TraceMsg (0, Msg);

 } else {
    if (DeBug || OaBug)
    if (sprintf (Msg,
/* "    OA New  Object(%4u) [%2u:%4u]  of Class Obj[%2u]\n" */
                 TraceStr(12),
                 LocalObj, Anchor->DbId, Anchor->Handle, ObjHdr->ObjNum))
       TraceMsg (1, Msg);
 }

 if (Normal(*Status))
 if (Anchor->DbId != RenvId)
   *Status = Env_Normal;

TRACK(TrackBak,"OaNewObject\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/


boolean    OaGetObject (tokentype    *Token,
                        ft F,lt Z,zz *Status,     addrtype  *Object )
{
indextype   Index           = 0;
dbheader   *CoreDb          = NullPtr;


  TmGetObject          (Token,    McStat,         Object);

  if (*Status == Hm_BadHandle)
  {
   *Status = Env0_Corrupted;

  } else if (*Status == Tm_ObjectNotPaired) {
   *Status = Env_Normal;
    if (TmGetCoreDb       (Token,   McStat,        &CoreDb))
    if (OmGetObject       (Token,   McStat,        &Index,        Object))
    if (HmPutObjectMemLoc (CoreDb,  Token->Handle, 0,             McStat))
        HmPutObjectAddr   (CoreDb,  Token->Handle, *Object,       McStat);
  }

  if (!Normal(*Status) || DeBug || OaBug)
/* "  Oa GET Object[%2u :%4u] @%x, McStat= %d\n" */
  if (sprintf  (Msg, TraceStr(14),
                Token->DbId, Token->Handle, *Object, *Status))
      TraceMsg (0, Msg);

TRACK(TrackBak,"OaGetObject\n");
return (STAT);
}


boolean    OaRevokeCppObject (tokentype    *Token,  ft F,lt Z,zz *Status )
{
dbheader     *CoreDb    = NullPtr;

  if (TmGetCoreDb         (Token,  McStat,       &CoreDb))
  if (HmInvokeObject      (CoreDb, Token->Handle, (addrtype )NullPtr,  McStat))
      HmPointToInnerRealm (CoreDb, Token->Handle, McStat);

TRACK(TrackBak,"OaRevokeCppObject\n");
return (STAT);
}


boolean    OaInvokeCppObject (tokentype    *Token,  addrtype  Object,
                              ft F,lt Z,zz *Status )
{
dbheader     *CoreDb    = NullPtr;

  if (TmGetCoreDb          (Token,   McStat,         &CoreDb))
  if (HmInvokeObject       (CoreDb,  Token->Handle,   Object,  McStat))
      HmPointToOuterRegion (CoreDb,  Token->Handle,   McStat);

TRACK(TrackBak,"OaInvokeCppObject\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
boolean   OaUpdateObject (tokentype    *Token,   ft F,lt Z,zz *Status)
{
dbheader     *CoreDb    = NullPtr;
indextype     LocalObj  = 0;
objheader    *ObjHdr    = NullPtr;
indextype     Index     = 0;
numtype       RgnNum    = 0;
indextype     LocalIndex= 0;

  if (TmGetCoreDb       (Token,    McStat,         &CoreDb))
  if (HmGetObjectIndex  (CoreDb,   Token->Handle,   McStat,   &Index))
  if (HmGetLocalObjNum  (CoreDb,   Token->Handle,   McStat,   &LocalObj))
  if (LocalObj     == Null_ObjNum)
     *Status = Oa_ObjectHasBeenDeleted;


  if (Normal(*Status))
  if (OmGetObjHdr       (Token,    LocalObj,        McStat,   &ObjHdr))
  if (Index   < ObjHdr->BaseIndex)
  {
      RgnNum     = 0;
      LocalIndex = Index;
  } else {
      RgnNum     = (Index - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
      if (ObjHdr->BaseIndex)
         RgnNum++;
      LocalIndex = (Index - ObjHdr->BaseIndex)  % ObjHdr->RegionObjects;
  }

  if (Normal(*Status))
     MemPutBit        (ObjHdr->DirtyRegionDir,  RgnNum, 1, McStat);

  if (!Normal(*Status) || GrpBug || DbmBug || DeBug)
  if (sprintf (Msg,
/* "   OaUpdateObject:: [%3u:%4u] ObjNum=%3u Index=%4u Region[%2u:%2u]\n" */
        TraceStr(17),
               Token->DbId, Token->Handle, ObjHdr->LocalObjNum,  Index,
               RgnNum, LocalIndex))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaUpdateObject\n");
return(STAT);
}


boolean OaChkImage       (tokentype     *Token,   ft F,lt Z,zz  *Status)
{
dbheader     *CoreDb          = NullPtr;
numtype       ObjNum          = 0;
numtype       LocalObj        = 0;
time_t        ObjTod          = 0;
time_t        TknTod          = 0;

 if (Renv_Modified)
 if (TmGetCoreDb       (Token,  McStat,          &CoreDb))

 if (EnvFetchObjNum    (Token,  McStat,          &ObjNum))
 if (OaBug || DeBug)
/* "   OaCHkObjHdr(%3u) in DbId = %3u\n" */
 if (sprintf  (Msg, TraceStr(18),
               ObjNum, Token->DbId))
     TraceMsg (0,  Msg);

 if (MemGetLong        (CoreDbs[RenvId]->ObjTimeStampDir,
                        (indextype )ObjNum,
                        McStat,                  (lt64     *)&ObjTod))

 if (MemGetObjNum      (CoreDb->LocalObjNumMap,   ObjNum,
                        McStat,                  &LocalObj))
 if (MemGetLong        (CoreDb->ObjTimeStampDir,  LocalObj,
                        McStat,                  (lt64     *)&TknTod))

 if (TknTod < ObjTod)
 if (OaBug || DeBug)
/* "  from DB_ObjHdr Time: %s   To RENV_ObjDesc Time %s\n" */
 if (sprintf  (Msg, TraceStr(19),
               ctime (&TknTod), ctime (&ObjTod)))
 if (TraceMsg (0,  Msg))
   *Status   = Err_NotImplemented;


TRACK(TrackBak,"OaChkImage\n");
return (STAT);
}




/*+-----------------------------------------------------------------------+*/
boolean   OaFreeObject   (tokentype    *Token,   ft F,lt Z,zz *Status)
{  /* error free */
dbheader     *CoreDb    = NullPtr;
numtype       MemLoc    = 0;
addrtype      Object    = NullPtr;
indextype     LocalObj  = 0;
objheader    *ObjHdr    = NullPtr;

  if (TmGetCoreDb       (Token,    McStat,         &CoreDb))
  if (HmGetObjectMemLoc (CoreDb,   Token->Handle,   McStat,   &MemLoc))
  if (MemLoc)
  {
     if (HmGetLocalObjNum  (CoreDb,   Token->Handle,   McStat,   &LocalObj))
     if (OmGetObjHdr       (Token,    LocalObj,        McStat,   &ObjHdr))
     if (HmGetObjectAddr   (CoreDb,   Token->Handle,   McStat,   &Object))
     if (CoreFreeCoreSpace (&Object,  ObjHdr->ObjSize, McStat))
     if (HmPutObjectAddr   (CoreDb,   Token->Handle,   NullPtr,   McStat))
         HmPutObjectMemLoc (CoreDb,   Token->Handle,   0,         McStat);
  } else {
     SendMsg (0, "***ERROR... OaFreeObject:: Object NOT a Image\n");
    *Status  = Oa_ObjNotInImageForm;

     if (Normal(*Status))
        HmClearObject (CoreDb, Token->Handle, McStat);
  }

TRACK(TrackBak,"OaFreeObject\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
boolean   OaDeleteObject  (tokentype   *Token,   ft F,lt Z,zz *Status)
{
dbheader       *CoreDb          = NullPtr;
addrtype        Object          = NullPtr;
numtype         ObjNum          = 0;
numtype         LocalObj        = 0;
objheader      *ObjHdr          = NullPtr;
indextype       ObjIndex        = 0;
indextype       StackPtr        = 0;

 if (TmGetCoreDb      (Token,              McStat,          &CoreDb))

 if (EnvFetchObjNum   (Token,              McStat,          &ObjNum))
 if (MemGetLocalObjNum
                      (CoreDb->LocalObjNumMap,               ObjNum,
                       McStat,                              &LocalObj))
 if (OmGetObjHdr      (Token,                                LocalObj,
                       McStat,                              &ObjHdr))

 if (OaGetObject       (Token,              McStat,         &Object))

 if (OaUpdateObject    (Token,              McStat))

 if (Normal(*Status))
#ifdef   __SWAP_GRP_POS__
   OmDeleteObject      (Token,            ObjHdr,      Object,   McStat);

#else

 if (HmGetObjectIndex  (CoreDb,                              Token->Handle,
                        McStat,                             &ObjIndex))

 if (MemPushWord       (ObjHdr->ExObjectTbl,                 ObjIndex,
                        McStat,                             &StackPtr))

 if (TmFreeToken       (McStat,                              Token))
     ObjHdr->ExObjectCount++;

 /* ZERO the objsize slot within the region                                */

 if (Normal(*Status))
    memset (Object, 0, (size_t )ObjHdr->ObjSize);
#endif /* __SWAP_GRP_POS__  */

TRACK(TrackBak,"OaDeleteObject\n");
return(STAT);
}

boolean    OaGetObjHandles
                          (tokentype    *Anchor,  numtype      ObjNum,
                           ft F,lt Z,zz *Status,
                           indextype    *Count,   handletype **Handles)
{
dbheader     *CoreDb        = NullPtr;
localnumtype  ObjIndex      = 0;
objheader    *ObjHdr        = NullPtr;
bytessize     CoreBlkSize   = 0;
numtype      *RgnStatus     = NullPtr;
indextype     RgnCount      = 0;

  if (TmFetchCoreDb     (Anchor,                  McStat,    &CoreDb))
  if (MemGetLocalObjNum (CoreDb->LocalObjNumMap,  ObjNum,
                         McStat,                 &ObjIndex))
  if (OmGetObjHdr       (Anchor,                  ObjIndex,
                         McStat,                 &ObjHdr))
  if (OmGetObjHandles   (Anchor,    ObjHdr,       McStat,
                         Count,     Handles,     &RgnCount,  &RgnStatus))
  {
      CoreBlkSize  = RgnCount * sizeof (numtype);
      CoreFreeCoreSpace ((addrtype *)RgnStatus,   CoreBlkSize,   McStat);
  }

TRACK(TrackBak,"OaGetObjHandles\n");
return (STAT);
}


boolean    OaReclaimHandles
                          (tokentype    *Anchor,  numtype      ObjNum,
                           ft F,lt Z,zz *Status)
{
indextype     Count         = 0;
handletype   *Handles       = NullPtr;
dbheader     *CoreDb        = NullPtr;
localnumtype  ObjIndex      = 0;
objheader    *ObjHdr        = NullPtr;
bytessize     CoreBlkSize   = 0;
numtype      *RgnStatus     = NullPtr;
indextype     RgnCount      = 0;
indextype     NullRgnCount  = 0;
int           i             = -1;
int           j             = -1;
indextype    *ExObjects     = NullPtr;
indextype     ExObjectCount = 0;
indextype     HighIndex     = 0;
indextype     LowIndex      = 0;

  if (TmFetchCoreDb     (Anchor,                  McStat,    &CoreDb))
  if (MemGetLocalObjNum (CoreDb->LocalObjNumMap,  ObjNum,
                         McStat,                 &ObjIndex))
  if (OmGetObjHdr       (Anchor,                  ObjIndex,
                         McStat,                 &ObjHdr))
  if (OmGetObjHandles   (Anchor,    ObjHdr,       McStat,
                        &Count,    &Handles,     &RgnCount,  &RgnStatus))
  if (MemGetChunkAddr   (ObjHdr->ExObjectTbl, McStat, (addrtype *)&ExObjects))
  {
     ExObjectCount    = ObjHdr->ExObjectCount;
     NullRgnCount     = 0;

     while (++i < RgnCount)
     {
        if (RgnStatus[i] == 0)
        {
           for (j = -1; ++j < ObjHdr->ExObjectCount; )
           {
             if (ExObjects[j] <= HighIndex
             &&  ExObjects[j] >= LowIndex)
             {
                 ExObjects[j] =  0;
                 ExObjectCount--;
             }

           }
           RgnStatus[NullRgnCount++] = i;
        }
     }
     for (i = -1, j= -1; ++i < ObjHdr->ExObjectCount; )
     {
        if (ExObjects[i]  == 0)
        {
           ExObjects[++j]  = 0;

        }
     }
     if ((ObjHdr->ExObjectCount = ExObjectCount) != ++j)
     if (sprintf (Msg,  "***ERROR... Mismatched Stacks; j=%4u, ExCount=%4u\n",
                 j, ExObjectCount))
     if (TraceMsg (0,  Msg))
        *Status  = Oa_BadObjStacks;

  }

  CoreBlkSize  = Count * sizeof (handletype);
  if (Handles)
      CoreFreeCoreSpace ((addrtype *)Handles,   CoreBlkSize,   McStat);

  CoreBlkSize  = RgnCount * sizeof (numtype);
  if (RgnStatus)
      CoreFreeCoreSpace ((addrtype *)RgnStatus,   CoreBlkSize,   McStat);

TRACK(TrackBak,"OaReclaimHandles\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF OA0.C                                   !
  +-----------------------------------------------------------------------+*/
