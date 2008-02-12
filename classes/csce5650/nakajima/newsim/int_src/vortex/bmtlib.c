/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* BMT01.c */
#define BMTLIB

#include "bmt01.h"

typedef Part2  Part;

extern boolean DrawBug;
/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/
/* .......................................................................
                    ....       PARTLIB     ....
   .......................................................................*/

boolean PartLib_Init     (ft F,lt Z,zz *Status)
{
/* Initiaize the ClassType                                               */

 if (PartLib_Class == 0)
 {
/*
ClassBug = True;
*/
  if (Env_GetClassId      ("PartLib",            McStat,  &PartLib_Class))
  if (Env_GetAttrId       ("PartsListHead",      McStat,  &PartsListHead_Attr))
  if (Env_GetAttrId       ("VpartsDir",          McStat,  &VpartsDir_Attr))
  if (Env_GetAttrId       ("VlinksDir",          McStat,  &VlinksDir_Attr))
  if (Env_GetAttrId       ("OwnerTosFroms",      McStat,  &OwnerTosFroms_Attr))
      Env_GetMatrixId     ("TosFroms",           McStat,  &TosFroms_Mtrx);
/*
  ClassBug = False;
*/

  if (Normal(*Status))
  if (Env_InitClassMap    (PartLib_Class,        sizeof(PartLib),   
                           C_Apl,                McStat))
  if (Normal(*Status))
      Env_InvokeMap       (C_Apl,                McStat);

  if (Normal(*Status))
  if (DrawPartPair_InitClass                    (McStat))
      Part_InitClass                            (McStat);
 }

TRACK(TrackBak,"PartLib_Init\n");
return (STAT);
}


boolean PartLib_Create   (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,  boolean       UseVparts,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, PartLib     **PrimalObj)
{
addrtype      IntVarray    = NullPtr;
numtype       GrpNum       = 0;
grpheader    *GrpHdr       = NullPtr;
sizetype      EntrySize    = sizeof (parttkntype);
numtype       AllocRgns    = (RgnHndls / 125) + 27;
numtype       RgnEntrys    = 125;
numtype       AllocXs      = 1000;
numtype       BaseIndex    = 2;
accesstype    AccessType   = Read_Write;
boolean       DiskCache    = False;
numtype       CacheLimit   = AllocRgns;
numtype       CacheLoad    = AllocRgns;
numtype       NumBuffRgns  = AllocRgns;
numtype       BaseRegion   = 0;
parttkntype   PartToken;
indextype     Index        = 0;
boolean      TempClassBug  = ClassBug;

  /* ClassBug = True; */

  VlinksDir             = 0;
  if (ClassBug)
  if (sprintf(Msg, "  PartLib_Create:: Db= '%32s'; AllocRgns=%6u\n", 
              DbName,  AllocRgns))
    TraceMsg (0, Msg);

  Primal_CreateDb     ("PartSchema", DbName,     DbFileName,  DbAccess,     
                       HndlRgns,     RgnHndls,   VhndlRgns,   RgnVhndls,
                       XmemRgns,     RgnXmems,   McStat,      PrimalTkn);

  OaDumpObject( 0, PrimalTkn, McStat ) ;

  if (Normal(*Status))
  {
     if (Object_GetImage(PrimalTkn,    McStat,    (addrtype *)PrimalObj))
     if (Grp_NewPacket  (PrimalTkn, EntrySize,   AllocRgns,   RgnEntrys,
                         AllocXs,   BaseIndex,   AccessType,  DiskCache,
                         CacheLimit,CacheLoad,   NumBuffRgns,
                         McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))
     if (Grp_NewEntry   (PrimalTkn, GrpNum, 
                         McStat,    &Index,       (addrtype )&PartToken))
     {
        GrpHdr->EnvObjType    = EnvType_AttrNum;

        VlinksDir             = 0;
     }

#ifdef     __INCLUDE_MTRX__
     if (Normal(*Status))
     {
        MultiMate              = False;
        AllocMates             = 30000;
        AllocPoints            = 0;
        RgnEntrys              = 1000;
        AllocRgns              = 10;
        AllocXs                = 1000;
        BaseIndex              = 0;
        AccessType             = Read_Write;
        DiskCache              = False;
        CacheLimit             = 10;
        CacheLoad              = 5;
        NumBuffRgns            = 10;
        MaxBuckets             = 2000;
        Divisor                = 10;
        KeyType                = aInteger;
        KeySize                = sizeof (partidtype);

        Matrix_Create      (TosFroms_Mtrx,               PrimalTkn,      
                            AllocKeys,     MaxBuckets,   Divisor,
                            AllocKeys,     MaxBuckets,   Divisor,
                            AllocMates,    AllocPoints,
                            AllocRgns,     RgnEntrys,    AllocXs,
                            DiskCache,     CacheLimit,   CacheLoad,
                            NumBuffRgns,   McStat,      &MtrxTkn);
     }
#endif /* __INCLUDE_MTRX__ */

     if (Normal(*Status))
     if (UseVparts)
     {
        if (Attr_ArrayCreate (PrimalTkn,                  PartsListHead_Attr,
                              AllocParts,                 sizeof(int),
                              McStat,                    &IntVarray))
        {
           VlistTkn.DbId     = PrimalTkn->DbId;
           VlistTkn          = (*PrimalObj)->PartsListHead.VchunkTkn;
           NumVlists         = 0;
           VchunkPutStackPtr (&VlistTkn,                  0, 
                              McStat,        (addrtype *)&Vlists);
        }
     }
 
     if (Normal(*Status))
     if (UseVparts)
     {
       (*PrimalObj)->VpartsDir      = GrpNum;
         VpartsDir                  = GrpNum;
     } 

     if (sprintf(Msg, "   PartLibCreate:: Db[%3u]; VpartsDir=%4u\n", 
                PrimalTkn->DbId,  VpartsDir))
       TraceMsg (0, Msg);

     if (Normal(*Status))
     if (Object_CommitImage  (PrimalTkn,    False,                 McStat))
     {
        Print_Msg           = True;
        WriteHdr            = False;
     }

  } else if (*Status == Csh_DbAlreadyExists) {

    *Status = Env_Normal;
    if (Object_GetImage     (PrimalTkn,    McStat,    (addrtype *)PrimalObj))
    if (UseVparts)
    if ((*PrimalObj)->VpartsDir == 0)
    {
       if (Grp_NewPacket    (PrimalTkn, EntrySize,   AllocRgns,   RgnEntrys,
                             AllocXs,   BaseIndex,   AccessType,  DiskCache,
                             CacheLimit,CacheLoad,   NumBuffRgns,
                             McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))
       if (Grp_NewEntry     (PrimalTkn, GrpNum, 
                             McStat,   &Index,        (addrtype )&PartToken))
       {
          GrpHdr->EnvObjType         = EnvType_AttrNum;
          (*PrimalObj)->VpartsDir    = GrpNum;
          VpartsDir                  = GrpNum;

          Object_CommitImage(PrimalTkn,              False,       McStat);
       }
    }

    if (Normal(*Status))
    if ((*PrimalObj)->VpartsDir)
    if (Grp_GetPacket       (PrimalTkn,    (*PrimalObj)->VpartsDir,
                             McStat,                    &GrpHdr))
       VpartsDir           = (*PrimalObj)->VpartsDir;

    if (Normal(*Status))
    {
       VlistTkn            = (*PrimalObj)->PartsListHead.VchunkTkn;
       if (VlistTkn.Handle)
       if (Vchunk_GetStackPtr(&VlistTkn,    McStat,               &NumVlists))
           Vchunk_GetAddress (&VlistTkn,    McStat,   (addrtype *)&Vlists);
    }
  }

  if (Normal(*Status))
  if (ClassBug || DeBug >= 0)
     Object_Dump          (PrimalTkn);

  ClassBug = TempClassBug;

TRACK(TrackBak,"PartLib_Create\n");
return (STAT);
}


boolean PartLib_ActivateDb
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          ft F,lt Z,zz *Status,    PartLib     **PrimalObj)
{
grpheader    *GrpHdr       = NullPtr;
dbaccesstype  AccessType   = Read_Write;

  if (ClassBug)
  if (sprintf(Msg, "  PartLibActivate:: [%3u:%8u]  DbFile= '%32s'\n",
              PrimalTkn->DbId, PrimalTkn->Handle, DbFileName))
     TraceMsg (0, Msg);

  if (Primal_ActivateDbByToken
                          (PrimalTkn,    DbFileName,  AccessType,  McStat))
  if (ClassBug || DeBug >= 0)
     Object_Dump          (PrimalTkn);

  if (Normal(*Status))
  if (Object_GetImage     (PrimalTkn,    McStat,    (addrtype *)PrimalObj))
  if (ClassBug || DeBug >= 0)
     Object_Dump          (PrimalTkn);

  if (Normal(*Status))
  if ((*PrimalObj)->VpartsDir)
  if (Grp_GetPacket       (PrimalTkn,    (*PrimalObj)->VpartsDir,
                           McStat,      &GrpHdr))
  {
     VpartsDir          = (*PrimalObj)->VpartsDir;
     VlinksDir          = 0;
     VlistTkn           = (*PrimalObj)->PartsListHead.VchunkTkn;
     if (VlistTkn.Handle)
     if (Vchunk_GetStackPtr(&VlistTkn,    McStat,               &NumVlists))
         Vchunk_GetAddress (&VlistTkn,    McStat,   (addrtype *)&Vlists);
  }

TRACK(TrackBak,"PartLib_ActivateDb\n");
return (STAT);
}

void PartLib_Topology       (void)
{
         longaddr    StartAddr = 0;
static   PartLib     That;
static   PartLib    *This      = &That;

  TraceMsg (0, "\n\n  <PartLib2> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof PartLib            : =%3u\n",   sizeof(PartLib));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->DbNum               : +%3ld\n",
           (longaddr )&This->DbNum                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->EnvType             : +%3ld\n",
           (longaddr )&This->EnvType             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->DescNum             : +%3ld\n",
           (longaddr )&This->DescNum              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->PrimalSlotAttr      : +%3ld\n",
           (longaddr )&This->PrimalSlotAttr       - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->PartsListHead       : +%3ld\n",
           (longaddr )&This->PartsListHead        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->VpartsDir           : +%3ld\n",
           (longaddr )&This->VpartsDir            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->VlinksDir           : +%3ld\n",
           (longaddr )&This->VlinksDir            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->OwnerTosFroms       : +%3ld\n\n",
           (longaddr )&This->OwnerTosFroms        - StartAddr);
  TraceMsg (0, Msg);

return;
}

/*+-----------------------------------------------------------------------+
~P                         END OF BMTLIB.C                                !
  +-----------------------------------------------------------------------+*/

