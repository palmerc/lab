/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env - ogrp.c */
#define  OGRP_C
#define  GRP_C

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "odbm.h"
#include "grp.h"
#include "ogrp.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...OGRP... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[7] =
{
   " ObjHdr[%3u:%3u] Not set as ObjectType.\n",
   "    NewRgn  Entry(%5u) to Region[%5u] Head=%3u, RgnBufRgn=%5u\n",
   "    Append Grp[%3u] Entry(%5u) to Region[%5u] (%5u)\n",
   "   GrpNewObject[%4u] (%4u) Rgn=%3u RgnChunk= %3u   @ %x\n",
   "  GrpGetObject DbId[%3u] ObjHdr[%2u] Index=%4u ...\n",
   "        Index =%3u; RgnNum =%3u, LocalIndex = %3u\n",
   "   GrpGetObject DbId=%3u Obj[%3u] Index=%4u Region[%2u:%2u]\n"
};
#endif

#define MemPopIndex          Mem_PopWord
#define MemGetChunkNum       Mem_GetWord

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

extern  idtype       PrimalId;

/*+-----------------------------------------------------------------------+
~GRP                    GENERIC REGION PACKET                             !
  +-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*
  !                 ....     Grp Entry      ....                          !
  !                 ....    for ObjDesc     ....                          !
  +-----------------------------------------------------------------------+*/

/*     12.0.3                                                              */
boolean GrpNewObject     (dbheader     *CoreDb,    objheader    *ObjHdr,
                          ft F,lt Z,zz *Status,    indextype    *Index,
                          numtype      *RgnIndex,  addrtype     *RgnAddr)
{
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
boolean         ExObject     = False;
numtype         BuffIndex    = 0;


 if (ObjHdr->EnvObjType > That_AttrNum)
 if (sprintf (Msg, TraceStr(0),
/* " ObjHdr[%3u:%3u] Not set as ObjectType.\n" */
              CoreDb->DbId, ObjHdr->ObjNum))
 if (SendMsg (0, Msg))
    ObjHdr->EnvObjType = 0;


 if (ObjHdr->ExObjectCount)
 {
    ExObject     = True;
    if (MemPopIndex  (ObjHdr->ExObjectTbl,
                      McStat,    Index,  &ObjHdr->ExObjectCount))
    if (*Index < ObjHdr->BaseIndex)
    {
      *RgnIndex  = 0;
       Chunk              = ObjHdr->BaseRegion;
       MemGetChunkAddr    (Chunk,       McStat,    &ChunkAddr);

    } else {
      *RgnIndex  = (*Index - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
       if (ObjHdr->BaseIndex)
          ++*RgnIndex;

       GrpGetRegion       (CoreDb,     (grpheader *)ObjHdr,    *RgnIndex,
                           McStat,     &Chunk,     &ChunkAddr);
    }
/*  ObjHdr->ExObjectCount--;
*/
 } else if (ObjHdr->ObjectCount < ObjHdr->BaseIndex) {
   *Index        = ObjHdr->ObjectCount++;
   *RgnIndex     = 0;
    if (*Index  == 0)
    {
       GrpNewRegion (CoreDb,  (grpheader *)ObjHdr, *Index,     *RgnIndex,
                     McStat,              &Chunk,  &ChunkAddr);
       ObjHdr->BaseRegion = Chunk;

    } else {
       Chunk              = ObjHdr->BaseRegion;
       MemGetChunkAddr       (Chunk,       McStat,    &ChunkAddr);
    }

 } else {
   *Index      =  ObjHdr->ObjectCount++;
   *RgnIndex   = (*Index  - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
    if (ObjHdr->BaseIndex)
       ++*RgnIndex;

    LocalIndex = (*Index  - ObjHdr->BaseIndex) % ObjHdr->RegionObjects;

    if (LocalIndex == 0)
    {
       GrpNewRegion   (CoreDb,   (grpheader *)ObjHdr,
                       *Index,               *RgnIndex,
                       McStat,               &Chunk,   &ChunkAddr);

    } else {
       if (MemGetChunkNum   (ObjHdr->RegionChunkDir, *RgnIndex,
                             McStat,                 &Chunk))
       if (Chunk  != 0)
           MemGetChunkAddr  (Chunk,  McStat,         &ChunkAddr);

       if (Chunk     == 0
       ||  ChunkAddr == NullPtr)
           GrpGetRegion     (CoreDb,     (grpheader *)ObjHdr,    *RgnIndex,
                             McStat,     &Chunk,     &ChunkAddr);

       if (Normal(*Status))
       {
          BuffIndex                        = ObjHdr->RgnBuffNums[*RgnIndex];
          ObjHdr->RgnBuffNodes[BuffIndex]  = 1;
       }

    }
 }

 /* Allocate space for Entry: Bytes in Region Chunk Space                  */
 if (Normal(*Status))
 if (*RgnIndex)
    LocalIndex = (*Index - ObjHdr->BaseIndex) % ObjHdr->RegionObjects;
 else
    LocalIndex = *Index;

 if (Normal(*Status))
 if (!ExObject)
     MemPutStackPtr   (Chunk,  LocalIndex+1,    McStat);

 if (Normal(*Status))
 if (MemPutBit        (ObjHdr->DirtyRegionDir,  *RgnIndex, 1, McStat))
    *RgnAddr   = ChunkAddr;

 if (DbmBug)
     MemBug = False;

 if (!Normal(*Status) || DeBug || GrpBug || DbmBug)
 if (sprintf (Msg, TraceStr(3),
/* "   GrpNewObject[%4u] (%4u) Rgn=%3u RgnChunk= %3u   @ %x\n" */
             *Index, LocalIndex, *RgnIndex, Chunk, *RgnAddr))
       TraceMsg (0, Msg);

TRACK(TrackBak,"GrpNewObject\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+*/
/*     12.0.4                                                              */
boolean GrpGetObject     (dbheader     *CoreDb,    objheader    *ObjHdr,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *RgnAddr)
{
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;

  if (Normal(*Status))
  {
     if (Index   < ObjHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = Index;
        if (ObjHdr->BaseRegion == 0)
           GrpGetRegion     (CoreDb,     (grpheader *)ObjHdr,  0,
                             McStat,    &ObjHdr->BaseRegion,  &ChunkAddr);
        else
           MemGetChunkAddr  (ObjHdr->BaseRegion,  McStat,     &ChunkAddr);

        Chunk      = ObjHdr->BaseRegion;

     } else {
        RgnNum     = (Index - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
        if (ObjHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (Index - ObjHdr->BaseIndex)  % ObjHdr->RegionObjects;
        GrpGetRegion        (CoreDb,     (grpheader *)ObjHdr,  RgnNum,
                             McStat,    &Chunk,               &ChunkAddr);
     }
    *RgnAddr    = ChunkAddr;

  }
  if (Normal(*Status))
  if (Admin_Run)
  if (CoreDb->DbId < PrimalId)
     MemPutBit        (ObjHdr->DirtyRegionDir,  RgnNum, 1, McStat);

  if (!Normal(*Status) || GrpBug || DbmBug || DeBug)
  if (sprintf (Msg, TraceStr(6),
/* "   GrpGetObject DbId=%3u Obj[%3u] Index=%4u Region[%2u:%2u]\n" */
               CoreDb->DbId, ObjHdr->LocalObjNum,  Index,
               RgnNum, LocalIndex))
     TraceMsg (0, Msg);

TRACK(TrackBak,"GrpGetObject\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/*     12.0.4                                                              */
boolean Grp_DeleteObject (dbheader     *CoreDb,    objheader    *ObjHdr,
                          indextype     Index,     ft F,lt Z,zz *Status)
{
numtype         DeleteRgn    = 0;
numtype         RgnNum       = 0;
numtype         Chunk        = 0;
addrtype        ChunkAddr    = NullPtr;
indextype       LocalIndex   = 0;
addrtype        DeleteEntry  = NullPtr;
indextype       LastIndex    = 0;
addrtype        LastEntry    = NullPtr;
numtype         MemLoc       = 0;
handletype      Handle       = 0;
boolean         TempGrpBug   = GrpBug;

  if (Index   < ObjHdr->BaseIndex)
  {
     DeleteRgn               = 0;
     LocalIndex              = Index;
     if (ObjHdr->BaseRegion == 0)
        GrpGetRegion     (CoreDb,     (grpheader *)ObjHdr,  0,     
                          McStat,    &ObjHdr->BaseRegion,  &ChunkAddr);
     else
        MemGetChunkAddr  (ObjHdr->BaseRegion,  McStat,     &ChunkAddr);

     Chunk      = ObjHdr->BaseRegion;

  } else {
     DeleteRgn      = (Index - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
     if (ObjHdr->BaseIndex)
        RgnNum++;
     LocalIndex     = (Index - ObjHdr->BaseIndex) % ObjHdr->RegionObjects;
     if (GrpGetRegion    (CoreDb,     (grpheader *)ObjHdr,  DeleteRgn,     
                          McStat,    &Chunk,               &ChunkAddr))
       ++ObjHdr->FrozenRgns[DeleteRgn];
  }
  DeleteEntry   =  (addrtype )((char *)ChunkAddr 
                                     + (ObjHdr->ObjSize * LocalIndex));

--ObjHdr->ObjectCount;

  LastIndex             = ObjHdr->ObjectCount;

  if (GrpBug || DbmBug || DeBug)
  if (sprintf (Msg, 
               " Rgn =%4u; Chunk=%4u;                  LocalIndex=%4u;\n",
               DeleteRgn, Chunk, LocalIndex))
  if (TraceMsg (0, Msg))
  if (sprintf (Msg, 
               "                                         LastIndex=%4u\n", 
               ObjHdr->ObjectCount))
     TraceMsg (0, Msg);

  if (Normal(*Status)) 
  if (ObjHdr->ObjectCount == 0)
  {
     memset     (DeleteEntry,    0,            ObjHdr->ObjSize);
     Grp_CleanRgnChunks (CoreDb,  (grpheader *)ObjHdr,  DeleteRgn,  Chunk, 
                         McStat);

  } else if (LastIndex    == Index) {

     memset     (DeleteEntry,    0,            ObjHdr->ObjSize);
     if (DeleteRgn)
      --ObjHdr->FrozenRgns[DeleteRgn];

   ++ObjHdr->RgnStats[DeleteRgn];

     if (LocalIndex  == 0)
         Grp_CleanRgnChunks (CoreDb,  (grpheader *)ObjHdr,  DeleteRgn, Chunk,
                             McStat);
     else
         MemPutStackPtr     (Chunk,        LocalIndex+1,    McStat);

  } else {
     /*  Get Position of Last Object Entry of Last Grp Rgn.                */
     if (LastIndex         < ObjHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = LastIndex;
        if (ObjHdr->BaseRegion == 0)
           GrpGetRegion     (CoreDb,     (grpheader *)ObjHdr,  0,     
                             McStat,    &ObjHdr->BaseRegion,  &ChunkAddr);
        else
           MemGetChunkAddr  (ObjHdr->BaseRegion,  McStat,     &ChunkAddr);

        Chunk      = ObjHdr->BaseRegion;

     } else {
        RgnNum     = (LastIndex - ObjHdr->BaseIndex) / ObjHdr->RegionObjects;
        if (ObjHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (LastIndex - ObjHdr->BaseIndex) % ObjHdr->RegionObjects;
        GrpGetRegion    (CoreDb,     (grpheader *)ObjHdr,  RgnNum,     
                         McStat,    &Chunk,               &ChunkAddr);
     }
     LastEntry     =  (addrtype )((char *)ChunkAddr 
                                        + (ObjHdr->ObjSize * LocalIndex));

     Handle        = *(handletype *)((char *)LastEntry 
                                        + ObjHdr->HandleOffset);

     if (GrpBug || DbmBug || DeBug)
     if (sprintf (Msg,
         " LAST Rgn =%4u; Chunk=%4u; Local=%4u;      Index=%4u; Handle=%6u\n",
                  RgnNum, Chunk, LocalIndex, LastIndex,  Handle))
        TraceMsg (0, Msg);

     /*  Copy Last Object Entry  to position of deleted.                  */
     if (Normal(*Status)) 
     if (HmPutObjectIndex   (CoreDb,  Handle,  Index,         McStat))
     if (HmGetObjectMemLoc  (CoreDb,  Handle,  McStat,       &MemLoc))
     if (MemLoc  ==0)
         HmPutObjectAddr    (CoreDb,  Handle,  DeleteEntry,   McStat);

     if (Normal(*Status)) 
     {
      ++ObjHdr->RgnStats[RgnNum];
      ++ObjHdr->RgnStats[DeleteRgn];

        MoveBytes  (LastEntry,  DeleteEntry,  ObjHdr->ObjSize);
        memset     (LastEntry,  0,            ObjHdr->ObjSize);

        if (LocalIndex  == 0)
           Grp_CleanRgnChunks (CoreDb,  (grpheader *)ObjHdr,  RgnNum,  Chunk,
                               McStat);
        else
           MemPutStackPtr     (Chunk,        LocalIndex+1,    McStat);
     }

     if (DeleteRgn)
       --ObjHdr->FrozenRgns[DeleteRgn];
  }
  GrpBug  = TempGrpBug;

TRACK(TrackBak,"Grp_DeleteObject\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF OGRP.C                                  !
  +-----------------------------------------------------------------------+*/
