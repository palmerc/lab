/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - Env01.c */
/*+-----------------------------------------------------------------------+
  |                             VORTEx                                    |
  |                           ENVIORNMENT                                 |
  +-----------------------------------------------------------------------+*/
#define DBA_C
#include <time.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "env0.h"
#include "odbm.h"
#include "oa0.h"
#include "oa1.h"
#include "env1.h"
#include "dba.h"

/*+-----------------------------------------------------------------------+
~ENV                            DBA                                       !
  +-----------------------------------------------------------------------+*/
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        Primal_ObjNum          (objnumtype )25
#define        lSetHead_ObjNum        (objnumtype )63     
#define        Wschema_ObjNum         (objnumtype )132
#define        Db_ObjNum              (objnumtype )134

#define        DbId_AttrNum           (numtype )3
#define        CoreDbId_AttrNum       (numtype )4
#define        ObjNum_AttrNum         (numtype )5

#define        DbNum_AttrNum          (numtype )18
#define        Nam_AttrNum            (numtype )19
#define        EnvType_AttrNum        (numtype )20

#define        DbCount_AttrNum        (numtype )38

#define        AccessRights_AttrNum   (numtype )70

#define        DbAccessType_AttrNum       (numtype )156

#define        DbFileName_AttrNum         (numtype )158
#define        ObjsOfSchema_AttrNum       (numtype )159

#define        OwnerWenvDbs_AttrNum       (numtype )247

#define        DbSlotAttr_AttrNum         (numtype )263
#define        PrimalSlotAttr_AttrNum     (numtype )264
#define        RschemaSlotAttr_AttrNum    (numtype )265
#define        WschemaSlotAttr_AttrNum    (numtype )266


#define        RenvSchemas_Hndl       (handletype )684
#define        WenvDbs_Hndl           (handletype )692
#define        SchemaDbs_Hndl         (handletype )693

#endif

/*+-----------------------------------------------------------------------+*
~OA                          EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
extern boolean OaDumpObjHdr   (int           TraceSwi, tokentype    *Anchor,  
                               num           ObjNum, 
                               ft F,lt Z,zz *Status);

extern boolean OaDumpObject   (int           TraceSwi, tokentype    *Token,
                               ft F,lt Z,zz *Status);
 
extern boolean KernelPutAttr  (tokentype    *Token,    numtype    EnvAttrNum,
                               addrtype      Value,    ft F,lt Z,zz *Status);

extern boolean KernelGetAttr  (tokentype    *Token,    numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype      Value);

extern boolean KernelFirstBit (tokentype    *BitFieldTkn,
                               ft F,lt Z,zz *Status,   indextype *FirstBitNum);

extern boolean KernelNextBit  (tokentype    *BitFieldTkn,
                               indextype     CurrBitNum,
                               ft F,lt Z,zz *Status,
                               indextype    *NextBitNum);
 
extern boolean SetFirstOf     (handletype    SetDesc,  tokentype    *OwnerTkn,
                               ft F,lt Z,zz *Status,   tokentype    *NodeTkn,
                               tokentype    *FirstTkn);

extern boolean SetNextOf      (handletype    SetDesc,  tokentype    *CurrNode,
                               ft F,lt Z,zz *Status,  
                               tokentype    *NodeTkn,  tokentype    *NextTkn);

extern boolean SetAddInto     (handletype    SetDesc,  tokentype    *OwnerTkn,
                               tokentype    *MemberTkn,ft F,lt Z,zz *Status);


#define MemPutObjNum        Mem_PutWord
#define MemGetObjNum        Mem_GetWord
#define MemPushObjNum       Mem_PushWord
#define MemPushObjHdr       Mem_PushAddr

/*+-----------------------------------------------------------------------+*/
/*+                        DATABASE ACTOR                                 +*/
/*+-----------------------------------------------------------------------+*/
boolean    DbmCreateDb   (tokentype    *SchemaTkn,   numtype      PrimalObjNum,
                          char         *DbName,
                          char         *NewFileName, dbaccesstype DbAccess,
                          indextype     AllocObjs,   indextype    AllocAttrs,
                          ft F,lt Z,zz *Status,      tokentype   *DbPrimalTkn)
{
indextype  StackPtr        = 0;
dbheader  *DbHdr           = NullPtr;
idtype     EnvId           = 0;
idtype     NewId           = 0;
statustype DumpStatus      = Env_Normal;
tokentype  Anchor;
tokentype  DbTkn;
envclass   EnvType         = aEnvDb;
int        TmpDeBug        = DeBug;
handletype SetHeadHndl     = 0;
tokentype  RschemaTkn;
tokentype  WschemaTkn;
tokentype  NodeTkn;
tokentype  CurrNodeTkn;
numtype    CurrObj         = 0;
numtype    NextObj         = 0;
tokentype  ObjTkn;
numtype    ObjNum          = 0;
numtype    LocalObjNum     = 0;
tokentype  RenvDbTkn;
time_t     Tod             = 0;
tokentype  BitFieldTkn;

 time (&Tod);

 NodeTkn.DbId            = RenvTkn.DbId;
 CurrNodeTkn.DbId        = RenvTkn.DbId;
 ObjTkn.DbId             = RenvTkn.DbId;
 
 RschemaTkn.DbId         = RenvTkn.DbId;
 
 RenvDbTkn.DbId          = WenvTkn.DbId;
 DbTkn.DbId              = WenvTkn.DbId;
 WschemaTkn.DbId         = WenvTkn.DbId;

 EnvId               = ++Env0->DbCount;
 if (!Create_Wenv)
 {
    if (KernelGetAttr   (&WenvTkn,       DbCount_AttrNum,
                         McStat,        (addrtype )&NewId))
    {
        NewId++;
        EnvId        = (idtype )NewId;
        CoreDbCount  = EnvId;
    }
 } else {
    CoreDbCount      = WenvId;
    NewId            = WenvId;
 }

 TraceMsg (0, " CREATE  Db Header and Db Primal  ... \n");
 if (Normal(*Status))
 if (HmMakeDbHdr (AllocObjs, AllocAttrs, NewId, McStat, &DbHdr)) 
 {
    DbHdr->DbId         = EnvId;
    DbHdr->LocalAccess  = DbAccess;
    DbHdr->DiskCache    = False;
    if (NewId == PrimalId)
       DbHdr->DiskCache = True;

    DbHdr->SchemaHndl   = SchemaTkn->Handle;
    CoreDbs[EnvId]      = DbHdr;
    Anchor.Handle       = NullHandle;
    Anchor.DbId         = EnvId;
    Anchor.CoreDbId     = EnvId;
    strcpy ((CoreDbs[EnvId])->DbFileName, NewFileName);

    sprintf (Msg, "  NEW DB [%2u] Created.\n",
             EnvId);
    TraceMsg (0, Msg);

    if (Normal(*Status))
    if (MemPushObjNum (CoreDbs[EnvId]->ObjNumMap,     NullObjNum,
                       McStat,                       &StackPtr))

    if (EnvId != RenvId)
    {
        if (ClassBug)
        if (sprintf (Msg, "  Time Stamp %x : %s\n", Tod, ctime(&Tod)))
           TraceMsg (0, Msg);
        MemPushLong   (CoreDbs[EnvId]->ObjTimeStampDir, 
                       (longwordtype )Tod,
                       McStat,                       &StackPtr);
    }

    if (Normal(*Status))
    if (MemPushObjHdr (CoreDbs[EnvId]->ObjHdrDir,     NullPtr,
                       McStat,                       &StackPtr))

        MemPushObjNum (CoreDbs[EnvId]->ObjVbnDir,     NullPtr,
                       McStat,                       &StackPtr);
 
    if (Normal(*Status))
    if (MemPushObjNum (CoreDbs[EnvId]->ObjNumMap,     PrimalObjNum,
                       McStat,                       &LocalObjNum))

    if (EnvId != RenvId)
        MemPushLong   (CoreDbs[EnvId]->ObjTimeStampDir, 
                       (longwordtype )Tod,
                       McStat,                       &StackPtr);

    if (Normal(*Status))
    if (MemPushObjHdr (CoreDbs[EnvId]->ObjHdrDir,     NullPtr,
                       McStat,                       &StackPtr))

    if (MemPushObjNum (CoreDbs[EnvId]->ObjVbnDir,     NullPtr,
                       McStat,                       &StackPtr))

    if (MemPutObjNum  (CoreDbs[EnvId]->LocalObjNumMap, PrimalObjNum,
                       LocalObjNum,                   McStat))
        CoreDbs[EnvId]->LocalObjCount++;
 
    if (Normal(*Status))
    if (TraceMsg (1, "  CREATE Primal Object OF This Db ...\n"))
    if (OaCreateObject  (&Anchor,         PrimalObjNum,
                         McStat,          DbPrimalTkn)) 
    {
       if (EnvId == WenvId)
       if (TraceMsg (1, "  CREATE the RENV DB Object ...\n"))
       if (OaCreateObject   (&WenvTkn,    Db_ObjNum,
                             McStat,     &RenvDbTkn))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         DbId_AttrNum,
                         (addrtype )&RenvId,                     McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         CoreDbId_AttrNum,   
                         (addrtype )&RenvId,                     McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         Nam_AttrNum,    (addrtype )RenvNam,     McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         DbSlotAttr_AttrNum,
                         (addrtype )&RenvTkn.Handle,         McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         DbFileName_AttrNum,
                         (addrtype )RenvFileName,                McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         DbAccessType_AttrNum, 
                         (addrtype )&RenvDbAccess,               McStat))
       if (OaPut        (False,          &RenvDbTkn,         PtrToVoid,
                         AccessRights_AttrNum,
                        (addrtype )&RenvDbAccess,                McStat))
       if (DeBug)
           OaDumpObject (0,              &RenvDbTkn,        Dump_Status);

       if (Normal(*Status))
       if (TraceMsg (1, "  CREATE a new DB Object in Wenv ... \n"))

       if (OaCreateObject
                        (&WenvTkn,        Db_ObjNum, 
                         McStat,         &DbTkn))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         DbId_AttrNum,   (addrtype )&EnvId,      McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         CoreDbId_AttrNum,   
                         (addrtype )&EnvId,                      McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         Nam_AttrNum,    
                         (addrtype )DbName,                      McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         DbSlotAttr_AttrNum,
                         (addrtype )DbPrimalTkn,             McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         DbFileName_AttrNum,
                         (addrtype )NewFileName,                 McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         DbAccessType_AttrNum,       (addrtype )&DbAccess,
                         McStat))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         AccessRights_AttrNum,       (addrtype )&DbAccess,
                         McStat))
       if (TraceMsg     (1,
        "  Pair the Db object in Wenv with  Primal object IN New Db... \n"))
       if (OaPut        (False,          &DbTkn,             PtrToVoid,
                         DbSlotAttr_AttrNum,
                         (addrtype )DbPrimalTkn,             McStat))
       if (TraceMsg     (1,
        "  Pair the new Primal object with the Db object IN Wenv... \n"))
       if (OaPut        (False,           DbPrimalTkn,       PtrToVoid,
                         PrimalSlotAttr_AttrNum,
                         (addrtype )&DbTkn.Handle,           McStat))  
       if (DeBug)
           OaDumpObject (0,              &DbTkn,            Dump_Status);
 
       if (Normal(*Status))
       if (WenvId == EnvId) 
       if (OaPut        (False,           DbPrimalTkn,       PtrToVoid,
                         DbNum_AttrNum,   (addrtype )&WenvId,    McStat))
       if (OaPut        (False,           DbPrimalTkn,       PtrToVoid,
                         Nam_AttrNum,     (addrtype )WenvNam,    McStat))
       if (OaPut        (False,           DbPrimalTkn,       PtrToVoid,
                         EnvType_AttrNum, (addrtype )&EnvType,   McStat))
       if (OaPut        (False,           DbPrimalTkn,       PtrToVoid,
                         PrimalSlotAttr_AttrNum,
                         (addrtype )&DbTkn.Handle,           McStat))  
 
       if (TraceMsg     (1, 
            "   Get the Rschema object and PAIR with Wschema object...\n"))
       if (SetFirstOf     (RenvSchemas_Hndl, &RenvTkn,        McStat,
                           &NodeTkn,         &RschemaTkn))
       if (OaCreateObject (&WenvTkn,          Wschema_ObjNum,
                           McStat,           &WschemaTkn))
       if (OaPut          (False,            &RschemaTkn,      PtrToVoid,
                           RschemaSlotAttr_AttrNum,    (addrtype )&WschemaTkn,
                           McStat))
       if (OaPut          (False,            &WschemaTkn,      PtrToVoid,
                           WschemaSlotAttr_AttrNum,    (addrtype )&RschemaTkn,
                           McStat))
       if (DeBug)
       if (OaDumpObject   (0,                &RschemaTkn,   Dump_Status))
           OaDumpObject   (0,                &WschemaTkn,   Dump_Status);
       CurrNodeTkn.Handle = NodeTkn.Handle;
 
       if (Normal(*Status) &&  WenvId == EnvId) 
       if (TraceMsg       (1, 
            "   Make a Schema Object and PAIR with RENV Schema...\n"))
       if (SetNextOf      (RenvSchemas_Hndl, &CurrNodeTkn,   McStat,
                           &NodeTkn,         &RschemaTkn))
       if (OaCreateObject (&WenvTkn,          Wschema_ObjNum,
                           McStat,           &WschemaTkn))
       if (OaPut          (False,            &RschemaTkn,      PtrToVoid,
                           RschemaSlotAttr_AttrNum,    (addrtype )&WschemaTkn,
                           McStat))
       if (OaPut          (False,            &WschemaTkn,      PtrToVoid,
                           WschemaSlotAttr_AttrNum,    (addrtype )&RschemaTkn,
                           McStat))
 
       if (SetAddInto     (WenvDbs_Hndl,     &WenvTkn,        &RenvDbTkn,
                           McStat))
       if (SetAddInto     (SchemaDbs_Hndl,   &WschemaTkn,     &RenvDbTkn,
                           McStat))
       if (DeBug)
       if (OaDumpObject   (0,                &RschemaTkn,     Dump_Status))
           OaDumpObject   (0,                &WschemaTkn,     Dump_Status);
       CurrNodeTkn.Handle = NodeTkn.Handle;
 
       if (Normal(*Status) &&  WenvId == EnvId) 
       if (TraceMsg       (1, 
            "   Make a Schema Object and PAIR with WENV Schema...\n"))
       if (SetNextOf      (RenvSchemas_Hndl, &CurrNodeTkn,     McStat,
                           &NodeTkn,         &RschemaTkn))
       if (OaCreateObject (&WenvTkn,          Wschema_ObjNum,
                           McStat,           &WschemaTkn))
       if (OaPut          (False,            &RschemaTkn,      PtrToVoid,
                           RschemaSlotAttr_AttrNum,    (addrtype )&WschemaTkn,
                           McStat))
       if (OaPut          (False,            &WschemaTkn,      PtrToVoid,
                           WschemaSlotAttr_AttrNum,    (addrtype )&RschemaTkn,
                           McStat))
 
       if (OaPut          (False,             DbPrimalTkn,    PtrToVoid,
                           OwnerWenvDbs_AttrNum,      (addrtype )&SetHeadHndl,
                           McStat))  
       if (DeBug)
       if (OaDumpObject   (0,                &RschemaTkn,   Dump_Status)) 
           OaDumpObject   (0,                &WschemaTkn,   Dump_Status);
 
       if (DeBug)
       if (OaDumpObject (0,  DbPrimalTkn,    Dump_Status))
           OaDumpObject (0, &DbTkn,          Dump_Status);

       if (DeBug)
       if (OaDumpObjHdr (0,  DbPrimalTkn,     PrimalObjNum,  Dump_Status)) 
           OaDumpObjHdr (0,  DbPrimalTkn,     Db_ObjNum,     Dump_Status); 
 
       if (Normal(*Status))
       if (TraceMsg     (1,
           "  Add The New Db Object Into the WenvDbs set...\n"))
       if (DeBug)
           OaDumpObject (0, &WenvTkn,        Dump_Status);

       if (Normal(*Status))
       if (SetAddInto     (WenvDbs_Hndl,     &WenvTkn,       &DbTkn,
                           McStat))
 
       if (TraceMsg     (1,
           "  Retrieve The WschemaTkn for this Schema Instance...\n"))
       if (OaGet          (False,             SchemaTkn,     PtrToVoid,
                           RschemaSlotAttr_AttrNum,
                           McStat,           (addrtype )&WschemaTkn))
       if (DeBug)
/*     || Align_Bytes)                                                     */
       if (OaDumpObject   (0,                &DbTkn,        Dump_Status))
           OaDumpObject   (0,                &WschemaTkn,   Dump_Status);
 
       if (Normal(*Status))
       if (TraceMsg     (1,
           "  Add Into the SchemaDbs set this Rschema Object... \n"))
           SetAddInto     (SchemaDbs_Hndl,   &WschemaTkn,   &DbTkn,
                           McStat);
    }
/*  *********************************************************  */
    if (Normal(*Status)) 
    {
       BitFieldTkn.DbId  = RenvId;
       if (sprintf   (Msg,
                  "  Assign SchemaObjs to LocalObjNumMap of Db[%2u]...\n\n",
                  EnvId))
       if (TraceMsg  (1,  Msg))

       if (OaGet          (False,            SchemaTkn,        PtrToVoid,
                           ObjsOfSchema_AttrNum,
                           McStat,          (addrtype )&BitFieldTkn.Handle))
       if (KernelFirstBit (&BitFieldTkn,     McStat,          &CurrObj))

       while (Normal(*Status) && CurrObj) 
       {
          if (EnvFetchObjHandle 
                          (CurrObj,           McStat,       &ObjTkn.Handle))
          if (OaGet       (False,            &ObjTkn,        PtrToVoid,
                           ObjNum_AttrNum,
                           McStat,           (addrtype )&ObjNum))

          if (ObjNum)
          if (MemGetObjNum  (CoreDbs[EnvId]->LocalObjNumMap,   ObjNum,
                             McStat,                     &LocalObjNum))

          if (ObjNum        >= lSetHead_ObjNum)
          if (ObjNum && !LocalObjNum)  
          if (PrimalObjNum  >= Primal_ObjNum)

          if (MemPushObjNum (CoreDbs[EnvId]->ObjNumMap,      ObjNum,
                             McStat,                        &LocalObjNum))

          if (MemPushLong   (CoreDbs[EnvId]->ObjTimeStampDir, 
                             (longwordtype )0,
                             McStat,                        &LocalObjNum)) 

          if (MemPushObjHdr (CoreDbs[EnvId]->ObjHdrDir,      NullPtr,
                             McStat,                        &StackPtr))
          if (MemPushObjNum (CoreDbs[EnvId]->ObjVbnDir,      NullPtr,
                             McStat,                        &StackPtr))
          if (MemPutObjNum  (CoreDbs[EnvId]->LocalObjNumMap, ObjNum,
                             LocalObjNum,                    McStat))
              CoreDbs[EnvId]->LocalObjCount++;

          if (Normal(*Status))
          if (KernelNextBit (&BitFieldTkn,      CurrObj, 
                             McStat,           &NextObj))
              CurrObj  = NextObj;

          if (DeBug)
          if (sprintf (Msg, " Add Obj[%3u] to DbObjs as LocalObj[%3u]\n",
                       ObjNum, (CoreDbs[EnvId]->LocalObjCount)))
              TraceMsg (1, Msg);
       }
       if (*Status == Set_EndOfSet)
           *Status = Env_Normal;
    }
/* ****************************************                               */
 }

 if (Normal(*Status)) 
     KernelPutAttr   (&WenvTkn,             DbCount_AttrNum,
                      (addrtype )&NewId,    McStat);
 if (DeBug || OaBug)
 if (sprintf (Msg,  " EnvNewDbHdr::   DbPrimal = [%3u:%4u];\n",
              DbPrimalTkn->DbId, DbPrimalTkn->Handle))
    TraceMsg (0, Msg);

 DeBug = TmpDeBug;
 TRACK(TrackBak,"DbmCreateDb\n");
return(STAT);
}


#ifndef __DECACHE_DBS__ 
boolean    Dbm_DeCacheDbs (ft F,lt Z,zz *Status)
{
tokentype    DbToken         = NullToken;
dbheader    *DbHdr           = NullPtr;
int          i               = 0;
addrtype     FreeAddr        = NullPtr;
addrtype    *FreeAddrs       = NullPtr;
sizeinbytes *FreeSizes       = NullPtr;
bytessize    BlkSize1        = Env0->DbCount * sizeof (addrtype)    * 6;
bytessize    BlkSize2        = Env0->DbCount * sizeof (sizeinbytes) * 6;
numtype      Count           = 0;
statustype   DumpStatus      = Env_Normal;

  DbToken.Handle        = 1;

  if (Core0_MoreCore     (BlkSize1,    McStat,   (addrtype *)&FreeAddrs))
  if (Core0_MoreCore     (BlkSize2,    McStat,   (addrtype *)&FreeSizes))
  while (Normal(*Status)
  &&   ++i             <= (int) Env0->DbCount)
  {
     DbHdr              = CoreDbs[i];
     DbToken.DbId       = i;
     if (DbmBug)
     {
        sprintf (Msg, "\n Dba_Cache movement:: DbId[%4u] ...\n",
                 i);
        TraceMsg (0, Msg);
        HmDumpDbHdr      (0,                        &DbToken,  Dump_Status);
     }

     if (DbHdr   != NullPtr)
     if (Mem_MoveChunk   (DbHdr->ObjectIndexDir,     McStat,
                         &FreeAddrs[Count+0],       &FreeSizes[Count+0]))
     if (Mem_MoveChunk   (DbHdr->ObjectAddrDir,      McStat,
                         &FreeAddrs[Count+1],       &FreeSizes[Count+1]))
     if (Mem_MoveChunk   (DbHdr->ObjectMemLocDir,    McStat,
                         &FreeAddrs[Count+2],       &FreeSizes[Count+2]))
     if (Mem_MoveChunk   (DbHdr->LocalObjNumDir,     McStat,
                         &FreeAddrs[Count+3],       &FreeSizes[Count+3]))
     if (Mem_MoveChunk   (DbHdr->ObjNumMap,          McStat,
                         &FreeAddrs[Count+4],       &FreeSizes[Count+4]))
        Count += 5;

     if (Normal(*Status))
     if (DbHdr->ObjectMemRefDir)
     if (Mem_MoveChunk   (DbHdr->ObjectMemRefDir,    McStat,
                          &FreeAddrs[Count],        &FreeSizes[Count]))
        Count += 1;
  }

  i     = -1;
  while (Normal(*Status)
  &&   ++i              < Count)
  {
     FreeAddr          = FreeAddrs[i];
     if (DbmBug)
     {
     sprintf (Msg, " Free CoreBlk[%2u] :: FreeAddr:= @%8x, FreeSize :=%6u\n",
              i, FreeAddr,  FreeSizes[i]);
     TraceMsg (0, Msg);
     }
     CoreFreeCoreSpace   (&FreeAddr,     FreeSizes[i],  McStat);
  }

  if (Normal(*Status))
  if (Core0FreeCoreSpace ((addrtype *)&FreeAddrs,    BlkSize1,      McStat))
      Core0FreeCoreSpace ((addrtype *)&FreeSizes,    BlkSize2,      McStat);

TRACK(TrackBak,"Dbm_DeCacheHdr\n");
return (STAT);
}
#endif /* __DECACHE_DBS__  */

/*+-----------------------------------------------------------------------+
~M                         END OF DBA.C                                   !
  +-----------------------------------------------------------------------+*/
