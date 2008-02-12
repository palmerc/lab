/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env - grp.c */
#define  GRP2_C
#define  GRP_C

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "env0.h" 
#include "core01.h"
#include "mem010.h"
#include "gdbm.h"
#include "vchunk.h"
 
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
#define MemPutHandle          Mem_PutWord
#define MemGetHandle          Mem_GetWord
 
#define MemPushLocalGrpNum    Mem_PushWord
#define MemPutLocalGrpNum     Mem_PutWord
#define MemGetLocalGrpNum     Mem_GetWord
 
#define MemPushEntryIndex     Mem_PushWord
#define MemPutEntryIndex      Mem_PutWord
#define MemGetEntryIndex      Mem_GetWord
 
#define MemPushEntryAddr      Mem_PushAddr
#define MemPutEntryAddr       Mem_PutAddr
#define MemGetEntryAddr       Mem_GetAddr
#define MemGetEntry           Mem_GetAddr
 
#define MemPutGrpNum          Mem_PutWord
#define MemGetGrpNum          Mem_GetWord
#define MemPushGrpNum         Mem_PushWord

#define MemPushAttrNum        Mem_PushWord
#define MemGetAttrNum         Mem_GetWord
#define MemPutAttrNum         Mem_PutWord
 
#define MemPushObjHdr         Mem_PushAddr
#define MemPutObjHdr          Mem_PutAddr
#define MemGetObjHdr          Mem_GetAddr
 
#define MemPushChunkNum       Mem_PushWord
#define MemGetChunkNum        Mem_GetWord
#define MemPutChunkNum        Mem_PutWord
 
#define MemPushIndex          Mem_PushWord
#define MemGetIndex           Mem_GetWord
#define MemPutIndex           Mem_PutWord
#define MemPopIndex           Mem_PopWord
 
#define MemPutRegionNum       Mem_PutWord
#define MemGetRegionNum       Mem_GetWord
 
#define MemPutOffset          Mem_PutWord
#define MemGetOffset          Mem_GetWord
 
#define MemPushLink           Mem_PushWord
#define MemPutLink            Mem_PutWord
#define MemGetLink            Mem_GetWord
 
#define MemPushExEntry        Mem_PushWord
#define MemPopExEntry         Mem_PopWord
 
 
#define MemGetVbn             Mem_GetWord
#define MemGetChunkNum        Mem_GetWord
 
#define MemGetSize            Mem_GetWord
#define MemPushSize           Mem_PushWord

/*+-----------------------------------------------------------------------+
~GRP                    GENERIC REGION PACKET                             !
  +-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*
  !                 ....     Grp Entry      ....                          !
  +-----------------------------------------------------------------------+*/
/* ~   12.2.0                                                              */
boolean Grp_EntryCount   (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Count)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;

 if (DeBug || GrpBug || DbmBug)
    TraceMsg (0, "   GrpEntryCount::  = *Count\n");

 if (TmGetCoreDb   (Anchor,          McStat, &CoreDb))

 if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
 {
    GrpGetPacket   (Anchor,  GrpNum, McStat, &GrpHdr);
 }
 if (Normal(*Status))
 if (GrpHdr != NullPtr)
   *Count    = GrpHdr->EntryCount;

 if (DeBug || GrpBug || DbmBug)
    TraceMsg (0, "   Grp_EntryCount::  = *Count\n");


TRACK(TrackBak,"Grp_EntryCount\n");
return(STAT);
}

/*     12.2.1                                                              */
boolean Grp_CreateEntry  (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Index)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
indextype       RgnIndex     = 0;
boolean         ExEntry      = False;
numtype         BuffIndex    = 0;

 if (DeBug || GrpBug || DbmBug)
    TraceMsg (0, "   GrpNewEntry::\n");

 if (TmGetCoreDb   (Anchor,          McStat, &CoreDb))

 if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

 if (Normal(*Status))
 if (GrpHdr->ExEntryCount)
 {
    ExEntry      = True;
    if (MemPopIndex  (GrpHdr->ExEntryTbl, 
                      McStat,    Index,  &GrpHdr->ExEntryCount))
    if (*Index < GrpHdr->BaseIndex)
    {
       RgnIndex  = 0;
       Chunk              = GrpHdr->BaseRegion;
       MemGetChunkAddr    (Chunk,       McStat,    &ChunkAddr);

    } else {
       RgnIndex  = (*Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
       if (GrpHdr->BaseIndex)
          RgnIndex++;
       GrpGetRegion       (CoreDb,      GrpHdr,     RgnIndex,
                           McStat,     &Chunk,     &ChunkAddr);
    }
  --GrpHdr->RgnStats[RgnIndex];

 } else if (GrpHdr->EntryCount < GrpHdr->BaseIndex) { 
   *Index                   = GrpHdr->EntryCount++;
    RgnIndex                = 0;
    if (*Index             == 0
    &&  GrpHdr->BaseRegion == 0)
    {
       GrpNewRegion (CoreDb,  GrpHdr, *Index, RgnIndex, 
                     McStat, &Chunk,  &ChunkAddr);
       GrpHdr->BaseRegion = Chunk;

    } else {
       Chunk              = GrpHdr->BaseRegion;
       MemGetChunkAddr       (Chunk,       McStat,    &ChunkAddr);
    }

 } else { 
   /* Check if still some room for a new entry  in current region chunk    */
   *Index      =  GrpHdr->EntryCount++;
    RgnIndex   = (*Index  - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
    if (GrpHdr->BaseIndex)
      RgnIndex++;

    LocalIndex = (*Index  - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;

    if (LocalIndex == 0)
    {
      /* Make a New Region  and Swap out Current Chunk if not first Entry  */
       GrpNewRegion   (CoreDb,   GrpHdr,  *Index,    RgnIndex,
                       McStat,  &Chunk,   &ChunkAddr);

    } else {
       if (MemGetChunkNum   (GrpHdr->RegionChunkDir,  RgnIndex,
                             McStat,                 &Chunk))
       if (Chunk  == 0)
           GrpGetRegion     (CoreDb,      GrpHdr,     RgnIndex,
                             McStat,     &Chunk,     &ChunkAddr);
       else
           MemGetChunkAddr  (Chunk,  McStat,         &ChunkAddr);

       if (Normal(*Status))
       {
          BuffIndex                        = GrpHdr->RgnBuffNums[RgnIndex];
          GrpHdr->RgnBuffNodes[BuffIndex]  = 1;
       }

    }
 }
 
 /* Allocate space for Entry: Bytes in Region Chunk Space                  */
 if (Normal(*Status)) 
 if (RgnIndex)
    LocalIndex = (*Index - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;
 else
    LocalIndex = *Index;

 if (Normal(*Status)) 
 if (!ExEntry)
     MemPutStackPtr   (Chunk,  LocalIndex+1,    McStat);

 if (DbmBug)
    MemBug = False;

 if (Normal(*Status)) 
     MemPutBit        (GrpHdr->DirtyRegionDir,  RgnIndex, 1, McStat);

 if (DbmBug)
    MemBug = False;

TRACK(TrackBak,"Grp_CreateEntry\n");
return(STAT);
}

 
/*     12.2.2                                                              */
boolean Grp_NewEntry     (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    indextype    *Index,
                          addrtype      Entry)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
indextype       RgnIndex     = 0;
boolean         ExEntry      = False;
numtype         BuffIndex    = 0;

 if (DeBug || GrpBug || DbmBug)
    TraceMsg (0, "   GrpNewEntry::\n");

 if (TmGetCoreDb   (Anchor,          McStat, &CoreDb))

 if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

 if (Normal(*Status))
 if (GrpHdr->ExEntryCount)
 {
    ExEntry      = True;
    if (MemPopIndex  (GrpHdr->ExEntryTbl, 
                      McStat,    Index,  &GrpHdr->ExEntryCount))
    if (*Index < GrpHdr->BaseIndex)
    {
       RgnIndex  = 0;
       Chunk              = GrpHdr->BaseRegion;
       MemGetChunkAddr    (Chunk,       McStat,    &ChunkAddr);

    } else {
       RgnIndex  = (*Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
       if (GrpHdr->BaseIndex)
          RgnIndex++;
       GrpGetRegion       (CoreDb,      GrpHdr,     RgnIndex,
                           McStat,     &Chunk,     &ChunkAddr);
    }
  --GrpHdr->RgnStats[RgnIndex];

 } else if (GrpHdr->EntryCount < GrpHdr->BaseIndex) { 
   *Index        = GrpHdr->EntryCount++;
    RgnIndex     = 0;
    if (*Index             == 0
    &&  GrpHdr->BaseRegion == 0)
    {
       GrpNewRegion (CoreDb,  GrpHdr, *Index, RgnIndex, 
                     McStat, &Chunk,  &ChunkAddr);
       GrpHdr->BaseRegion = Chunk;

    } else {
       Chunk              = GrpHdr->BaseRegion;
       MemGetChunkAddr       (Chunk,       McStat,    &ChunkAddr);
    }

 } else { 
   /* Check if still some room for a new entry  in current region chunk    */
   *Index      =  GrpHdr->EntryCount++;
    RgnIndex   = (*Index  - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
    if (GrpHdr->BaseIndex)
      RgnIndex++;

    LocalIndex = (*Index  - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;

    if (LocalIndex == 0)
    {
      /* Make a New Region  and Swap out Current Chunk if not first Entry  */
       GrpNewRegion   (CoreDb,   GrpHdr,  *Index,    RgnIndex,
                       McStat,  &Chunk,   &ChunkAddr);

    } else {
       if (MemGetChunkNum   (GrpHdr->RegionChunkDir,  RgnIndex,
                             McStat,                 &Chunk))
       if (Chunk  == 0)
           GrpGetRegion     (CoreDb,      GrpHdr,     RgnIndex,
                             McStat,     &Chunk,     &ChunkAddr);
       else
           MemGetChunkAddr  (Chunk,  McStat,         &ChunkAddr);

       if (Normal(*Status))
       {
          BuffIndex                        = GrpHdr->RgnBuffNums[RgnIndex];
          GrpHdr->RgnBuffNodes[BuffIndex]  = 1;
       }

    }
 }
 
 /* Allocate space for Entry: Bytes in Region Chunk Space                  */
 if (Normal(*Status)) 
 if (RgnIndex)
    LocalIndex = (*Index - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;
 else
    LocalIndex = *Index;

 if (Normal(*Status)) 
 if (!ExEntry)
     MemPutStackPtr   (Chunk,  LocalIndex+1,    McStat);

 if (DbmBug)
    MemBug = False;

 if (Normal(*Status)) 
 if (MemPutBit        (GrpHdr->DirtyRegionDir,  RgnIndex, 1, McStat))
     MemGetSomeBytes  (Chunk,  LocalIndex,      GrpHdr->EntrySize,
                       McStat,                  Entry);

 if (DbmBug)
    MemBug = False;

TRACK(TrackBak,"GrpNewEntry\n");
return(STAT);
}

 
/*+-----------------------------------------------------------------------+*/
/*     12.2.3                                                              */
boolean Grp_PutNewEntry  (tokentype    *Anchor,    numtype       GrpNum,
                          addrtype      Entry,
                          ft F,lt Z,zz *Status,    indextype    *Index)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
indextype       RgnIndex     = 0;
boolean         ExEntry      = False;
numtype         BuffIndex    = 0;
vchunktkntype  *VchunkDir    = NullPtr;
handletype     *VhandleDir   = NullPtr;

 if (DeBug || GrpBug || DbmBug)
    TraceMsg (0, "   GrpPutNewEntry::\n");

 if (TmGetCoreDb   (Anchor,          McStat, &CoreDb))

 if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

 if (Normal(*Status))
 if (GrpHdr->ExEntryCount)
 {
    ExEntry      = True;
    if (MemPopIndex  (GrpHdr->ExEntryTbl, 
                      McStat,    Index,  &GrpHdr->ExEntryCount))
    if (*Index < GrpHdr->BaseIndex)
    {
       RgnIndex  = 0;
       Chunk              = GrpHdr->BaseRegion;
       MemGetChunkAddr    (Chunk,       McStat,    &ChunkAddr);

    } else {
       RgnIndex  = (*Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
       if (GrpHdr->BaseIndex)
          RgnIndex++;
       GrpGetRegion       (CoreDb,      GrpHdr,     RgnIndex,
                           McStat,     &Chunk,     &ChunkAddr);
    }
  --GrpHdr->RgnStats[RgnIndex];

 } else if (GrpHdr->EntryCount < GrpHdr->BaseIndex) { 
   *Index        = GrpHdr->EntryCount++;
    RgnIndex     = 0;
    if (*Index             == 0
    &&  GrpHdr->BaseRegion == 0)
    {
       GrpNewRegion (CoreDb,  GrpHdr, *Index, RgnIndex, 
                     McStat, &Chunk,  &ChunkAddr);
       GrpHdr->BaseRegion = Chunk;

    } else {
       Chunk              = GrpHdr->BaseRegion;
       if (Chunk == 0)
           GrpGetRegion     (CoreDb,     GrpHdr,     RgnIndex,
                             McStat,    &Chunk,     &ChunkAddr);
       else
           MemGetChunkAddr  (Chunk,      McStat,    &ChunkAddr);
    }

 } else { 
   /* Check if still some room for a new entry  in current region chunk    */
   *Index      =  GrpHdr->EntryCount++;
    RgnIndex   = (*Index  - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
    if (GrpHdr->BaseIndex)
      RgnIndex++;

    LocalIndex = (*Index  - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;

    if (LocalIndex == 0)
    {
      /* Make a New Region  and Swap out Current Chunk if not first Entry  */
       GrpNewRegion   (CoreDb,   GrpHdr,  *Index,    RgnIndex,
                       McStat,  &Chunk,   &ChunkAddr);

    } else {
       if (MemGetChunkNum   (GrpHdr->RegionChunkDir,  RgnIndex,
                             McStat,                 &Chunk))
       if (Chunk == 0)
           GrpGetRegion     (CoreDb,     GrpHdr,      RgnIndex,
                             McStat,    &Chunk,      &ChunkAddr);
       else
           MemGetChunkAddr  (Chunk,  McStat,         &ChunkAddr);

       if (Normal(*Status))
       {
          BuffIndex                        = GrpHdr->RgnBuffNums[RgnIndex];
          GrpHdr->RgnBuffNodes[BuffIndex]  = 1;
       }

    }
 }
 
 /* Allocate space for Entry: Bytes in Region Chunk Space                  */
 if (Normal(*Status)) 
 if (RgnIndex)
    LocalIndex = (*Index - GrpHdr->BaseIndex) % GrpHdr->RegionEntrys;
 else
    LocalIndex = *Index;

 if (Normal(*Status)) 
 if (!ExEntry)
     MemPutStackPtr   (Chunk,  LocalIndex+1,    McStat);

 if (DbmBug)
    MemBug = False;

 if (Normal(*Status)) 
 if (MemPutBit        (GrpHdr->DirtyRegionDir,  RgnIndex, 1, McStat))
 if (GrpHdr->EntrySize        == sizeof (vchunktkntype))
 {
      VchunkDir               =  (vchunktkntype *)ChunkAddr;
      VchunkDir[LocalIndex]   = *(vchunktkntype *)Entry;

 } else if (GrpHdr->EntrySize == sizeof (handletype)) {
      VhandleDir              =  (handletype *)ChunkAddr;
      VhandleDir[LocalIndex]  = *(handletype *)Entry;

 } else
     MemPutSomeBytes  (Chunk,  LocalIndex,      GrpHdr->EntrySize,
                       Entry,                   McStat);

 if (DbmBug)
    MemBug = False;

TRACK(TrackBak,"GrpPutNewEntry\n");
return(STAT);
}

 
/*+-----------------------------------------------------------------------+*/
/*     12.2.4                                                              */
boolean Grp_PutEntry     (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     addrtype      Entry,
                          ft F,lt Z,zz *Status)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
numtype         BuffIndex    = 0;
vchunktkntype  *VchunkDir    = NullPtr;
handletype     *VhandleDir   = NullPtr;
numtype         EntryCount   = 0;


  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == NullPtr)
  if (GrpGetPacket     (Anchor,  GrpNum, McStat, &GrpHdr))
      EntryCount = GrpHdr->EntryCount;

  if (Normal(*Status))
  if (Index     < GrpHdr->BaseIndex)
  {
     RgnNum     = 0;
     LocalIndex = Index;
     if (GrpHdr->BaseRegion == 0)
     {
        if (GrpHdr->EntryCount == 0)
        {
           GrpNewRegion     (CoreDb,   GrpHdr,      LocalIndex,  RgnNum, 
                             McStat,               &Chunk,      &ChunkAddr);
        } else
           GrpGetRegion     (CoreDb,   GrpHdr,      RgnNum,
                             McStat,  &Chunk,      &ChunkAddr);
        GrpHdr->BaseRegion = Chunk;

     } else {
       Chunk               = GrpHdr->BaseRegion;
       MemGetChunkAddr      (Chunk,    McStat,     &ChunkAddr);

     }

  } else {
     RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
     if (GrpHdr->BaseIndex)
       RgnNum++;

     LocalIndex = (Index - GrpHdr->BaseIndex)  % GrpHdr->RegionEntrys;

     if (Index < GrpHdr->EntryCount)
     {
        GrpGetRegion     (CoreDb,     GrpHdr,     RgnNum,
                          McStat,    &Chunk,     &ChunkAddr);

     } else if (Index == GrpHdr->EntryCount) {
        if (LocalIndex == 0)
        {
        /* Make a New Region  and Swap out Current Chunk if not first Entry */
           GrpNewRegion   (CoreDb,   GrpHdr,   Index,    RgnNum,
                           McStat,  &Chunk,   &ChunkAddr);

        } else {
           if (MemGetChunkNum (GrpHdr->RegionChunkDir,      RgnNum,
                               McStat,                     &Chunk))
           if (Chunk == 0)
              GrpGetRegion    (CoreDb,     GrpHdr,          RgnNum,
                               McStat,    &Chunk,          &ChunkAddr);
           else
              MemGetChunkAddr (Chunk,      McStat,         &ChunkAddr);

           if (Normal(*Status))
           {
              BuffIndex                        = GrpHdr->RgnBuffNums[RgnNum];
              GrpHdr->RgnBuffNodes[BuffIndex]  = 1;
           }

        }

     } else {
     }
  }

  if (Normal(*Status))
  if (Index == GrpHdr->EntryCount)
  if (MemPutStackPtr   (Chunk,  LocalIndex+1,    McStat))
  {
     GrpHdr->EntryCount++;
  }

  if (Normal(*Status))
  if (MemPutBit        (GrpHdr->DirtyRegionDir,     RgnNum, 1, McStat))
  if (GrpHdr->EntrySize        == sizeof (vchunktkntype))
  {
      VchunkDir                =  (vchunktkntype *)ChunkAddr;
      VchunkDir[LocalIndex]    = *(vchunktkntype *)Entry;

  } else if (GrpHdr->EntrySize == sizeof (handletype)) {
      VhandleDir               =  (handletype *)ChunkAddr;
      VhandleDir[LocalIndex]   = *(handletype *)Entry;

  } else
     MemPutSomeBytes  (Chunk,  LocalIndex,      GrpHdr->EntrySize,
                       Entry,                   McStat);

TRACK(TrackBak,"GrpPutEntry\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/*     12.2.5                                                              */
boolean Grp_GetBaseRegion
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    addrtype     *BaseRgn)
{
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;


  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr)
       if (GrpHdr->BaseRegion)
          MemGetChunkAddr    (GrpHdr->BaseRegion,    McStat,     BaseRgn);
       else
        *Status = Grp_NoBaseRegion;

     else
        *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"Grp_GetBaseRegion\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/*     12.2.6                                                              */
boolean GrpGetEntry      (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype      Entry)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
vchunktkntype  *VchunkDir    = NullPtr;
handletype     *VhandleDir   = NullPtr;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  {
     if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
        GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  } else {
    *Status = Grp_NullDb;

  }

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = Index;
        if (GrpHdr->BaseRegion == 0)
           GrpGetRegion     (CoreDb,     GrpHdr,               0,     
                             McStat,    &GrpHdr->BaseRegion,  &ChunkAddr);
        else
           MemGetChunkAddr  (GrpHdr->BaseRegion,  McStat,     &ChunkAddr);

        Chunk      = GrpHdr->BaseRegion;

     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (Index - GrpHdr->BaseIndex)  % GrpHdr->RegionEntrys;
        GrpGetRegion        (CoreDb,     GrpHdr,               RgnNum,     
                             McStat,    &Chunk,               &ChunkAddr);
     }
  }

  if (Normal(*Status))
  if (GrpHdr->EntrySize         == sizeof (vchunktkntype))
  {
      VchunkDir                  =  (vchunktkntype *)ChunkAddr;
     *(vchunktkntype *)Entry     = VchunkDir[LocalIndex];

  } else if (GrpHdr->EntrySize  == sizeof (handletype)) {
      VhandleDir                 =  (handletype *)ChunkAddr;
     *(handletype *)Entry        = VhandleDir[LocalIndex];
/*
  } else if (GrpHdr->EntrySize  == sizeof (nametype) *2) {
      StrAddr                    =  ((char *)ChunkAddr +
                                      (LocalIndex * sizeof(nametype)));
      strcpy ((char *)Entry, StrAddr);
*/
  } else
      MemGetSomeBytes  (Chunk,  LocalIndex,      GrpHdr->EntrySize,
                        McStat,                  Entry);

TRACK(TrackBak,"GrpGetEntry\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/*     12.2.6.1                                                            */
boolean Grp_GetInLineEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *Entry)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = Index;
        if (GrpHdr->BaseRegion == 0)
           GrpGetRegion     (CoreDb,     GrpHdr,               0,     
                             McStat,    &GrpHdr->BaseRegion,  &ChunkAddr);
        else
           MemGetChunkAddr  (GrpHdr->BaseRegion,  McStat,     &ChunkAddr);

     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (Index - GrpHdr->BaseIndex)  % GrpHdr->RegionEntrys;
        GrpGetRegion    (CoreDb,     GrpHdr,               RgnNum,     
                         McStat,    &Chunk,               &ChunkAddr);
     }

  }
  if (Normal(*Status))
     *Entry    =  (addrtype )((char *)ChunkAddr 
                            + (GrpHdr->EntrySize * LocalIndex));

TRACK(TrackBak,"Grp_GetInLineEntry\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/*     12.2.7                                                              */
boolean Grp_GetFrozenEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *Entry)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = Index;
        if (GrpHdr->BaseRegion == 0)
           GrpGetRegion     (CoreDb,     GrpHdr,               0,     
                             McStat,    &GrpHdr->BaseRegion,  &ChunkAddr);
        else
           MemGetChunkAddr  (GrpHdr->BaseRegion,  McStat,     &ChunkAddr);

     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (Index - GrpHdr->BaseIndex)  % GrpHdr->RegionEntrys;
        if (GrpGetRegion    (CoreDb,     GrpHdr,               RgnNum,     
                             McStat,    &Chunk,               &ChunkAddr))
         ++GrpHdr->FrozenRgns[RgnNum];
     }

  }
  if (Normal(*Status))
     *Entry    =  (addrtype )((char *)ChunkAddr 
                            + (GrpHdr->EntrySize * LocalIndex));

TRACK(TrackBak,"Grp_GetFrozenEntry\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/*     12.2.8.1                                                            */
boolean Grp_FreezeEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
     ++GrpHdr->FrozenRgns[RgnNum];
     }
  }

TRACK(TrackBak,"Grp_FreezeEntry\n");
return (STAT);
}
 
 
/*     12.2.8.2                                                           */
boolean Grp_DefrostEntry
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
     --GrpHdr->FrozenRgns[RgnNum];
     }
  }
  if ((int  )GrpHdr->FrozenRgns[RgnNum] < 0)
     *Status  = Grp_PermaFrostReached;

TRACK(TrackBak,"Grp_DefrostEntry\n");
return (STAT);
}
 
 
/*     12.2.8.2                                                           */
boolean Grp_EntryIsFrosted
                         (tokentype    *Anchor,    numtype       GrpNum,
                          indextype     Index,     ft F,lt Z,zz *Status)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  {
     if (Index   < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
     } else {
        RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
     }
  }
  if ((int  )GrpHdr->FrozenRgns[RgnNum] > 0)
     return (True);

  if (Normal(*Status))
     return (False);

TRACK(TrackBak,"Grp_EntryIsFrosted\n");
return (STAT);
}
 
 
/*     12.2.8.3                                                            */
boolean Grp_FrostStatus
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
dbheader       *CoreDb       = NullPtr;
grpheader      *GrpHdr       = NullPtr;
numtype         RgnNum       = 0;
boolean         FrostStatus  = True;

  if (TmGetCoreDb      (Anchor, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == 0)
     GrpGetPacket  (Anchor,  GrpNum, McStat, &GrpHdr);

  if (Normal(*Status))
  for (RgnNum = 0;
       RgnNum < GrpHdr->RegionCount;  
     ++RgnNum)
  {
     if (GrpHdr->FrozenRgns[RgnNum] > 0)
     {
        FrostStatus    = False;
     }
  }

  if (Normal(*Status))
     return (FrostStatus);

TRACK(TrackBak,"Grp_FrostStatus\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/*     12.2.9                                                              */
boolean Grp_DeleteEntry   (tokentype    *Anchor,    numtype       GrpNum,
                           indextype     Index,     ft F,lt Z,zz *Status)
{  /* error free                                                           */
dbheader       *CoreDb          = NullPtr;
grpheader      *GrpHdr          = NullPtr;
bytetype       *ByteAddr        = NullPtr;
numtype         Region          = 0;
indextype       RgnNum          = 0;
indextype       LocalIndex      = 0;
numtype         RegionEntrys    = 0;
numtype         DeletedEntrys   = 0;
indextype       StackIndex      = 0;

 if (TmGetCoreDb       (Anchor,             McStat,       &CoreDb))
 if ((GrpHdr = CoreDb->GrpHdrs[GrpNum]) == NullPtr)
     GrpGetPacket      (Anchor, GrpNum,     McStat,       &GrpHdr);


 if (Normal(*Status))
 if (Index     < GrpHdr->BaseIndex)
 {
    RgnNum     = 0;
    LocalIndex = Index;
    Region     = GrpHdr->BaseRegion;
    MemGetChunkAddr (Region,  McStat, (addrtype *)&ByteAddr);

 } else {
    RgnNum     = (Index - GrpHdr->BaseIndex) / GrpHdr->RegionEntrys;
    if (GrpHdr->BaseIndex)
       RgnNum++;
    LocalIndex = (Index - GrpHdr->BaseIndex)  % GrpHdr->RegionEntrys;
    GrpGetRegion    (CoreDb,     GrpHdr,   RgnNum,     
                     McStat,    &Region,  (addrtype *)&ByteAddr);
 }

 if (Normal(*Status))
 if (MemPutBit       (GrpHdr->DirtyRegionDir,     RgnNum, 1, McStat))
 {
     ByteAddr      +=  (LocalIndex  *        GrpHdr->EntrySize);
     memset            (ByteAddr,   0,       GrpHdr->EntrySize);
     DeletedEntrys  = ++GrpHdr->RgnStats[RgnNum];
     MemPushExEntry    (GrpHdr->ExEntryTbl,  Index,
                        McStat,             &StackIndex);
   ++GrpHdr->ExEntryCount;
 }

 if (Normal(*Status))
 if (RgnNum         ==   GrpHdr->RegionCount - 1)
 {
     if (MemGetStackPtr    (Region,           McStat, &RegionEntrys))
     if (DeletedEntrys     == RegionEntrys)
     if (GrpHdr->BaseIndex == 0)
        Grp_DeleteRgn      (CoreDb,  GrpHdr,  RgnNum,  Region, McStat);
     else if (RgnNum        > 0)
        Grp_DeleteRgn      (CoreDb,  GrpHdr,  RgnNum,  Region, McStat);

 } else if (GrpHdr->GrpIndexDir       != 0
        &&  GrpHdr->RgnStats[RgnNum]  == GrpHdr->RegionEntrys
        &&  RgnNum                    != 0)  {

     Grp_RgnSwapTopAndPop  (CoreDb,  GrpHdr,  RgnNum,  Region, McStat);

 }

TRACK(TrackBak,"Grp_DeleteEntry\n");
return(STAT);
}
/*+-----------------------------------------------------------------------+
~M                         END OF GRP2.C                                  !
  +-----------------------------------------------------------------------+*/
