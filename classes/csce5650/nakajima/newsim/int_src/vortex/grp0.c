/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env - grp.c */
#define  GRP0_C
#define  GRP_C

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "env0.h" 
#include "core01.h"
#include "mem010.h"
#include "dbm.h"
#include "gdbm.h"
#include "odbm.h"
#include "vchunk.h"
 
#define MemFreeFileBlk     Mem_FreeFileBlk
extern boolean MemFreeFileBlk (blkhdrtype   *BlkHdr,
                               ft F,lt Z,zz *Status,    vbntype      *Vbn);

/*+-----------------------------------------------------------------------+*
~GRP                           GRP0
 *+-----------------------------------------------------------------------+*/
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

#define MemPutHandle         Mem_PutWord
#define MemGetHandle         Mem_GetWord
 
#define MemPushLocalGrpNum   Mem_PushWord
#define MemPutLocalGrpNum    Mem_PutWord
#define MemGetLocalGrpNum    Mem_GetWord
 
#define MemPushEntryIndex     Mem_PushWord
#define MemPutEntryIndex      Mem_PutWord
#define MemGetEntryIndex      Mem_GetWord
 
#define MemPushEntryAddr      Mem_PushAddr
#define MemPutEntryAddr       Mem_PutAddr
#define MemGetEntryAddr       Mem_GetAddr
#define MemGetEntry           Mem_GetAddr
 
#define MemPutGrpNum         Mem_PutWord
#define MemGetGrpNum         Mem_GetWord
#define MemPushGrpNum        Mem_PushWord

#define MemPushAttrNum       Mem_PushWord
#define MemGetAttrNum        Mem_GetWord
#define MemPutAttrNum        Mem_PutWord
 
#define MemPushObjHdr        Mem_PushAddr
#define MemPutObjHdr         Mem_PutAddr
#define MemGetObjHdr         Mem_GetAddr
 
#define MemPushChunkNum      Mem_PushWord
#define MemPopChunkNum       Mem_PopWord
#define MemGetChunkNum       Mem_GetWord
#define MemPutChunkNum       Mem_PutWord
 
#define MemPushIndex         Mem_PushWord
#define MemGetIndex          Mem_GetWord
#define MemPutIndex          Mem_PutWord
#define MemPopIndex          Mem_PopWord
 
#define MemPutRegionNum      Mem_PutWord
#define MemGetRegionNum      Mem_GetWord
 
#define MemPutOffset         Mem_PutWord
#define MemGetOffset         Mem_GetWord
 
#define MemPushLink          Mem_PushWord
#define MemPutLink           Mem_PutWord
#define MemGetLink           Mem_GetWord
 
#define MemPushExEntry        Mem_PushWord
#define MemPopExEntry         Mem_PopWord
 
 
#define MemPopVbn            Mem_PopWord
#define MemGetVbn            Mem_GetWord
#define MemGetChunkNum       Mem_GetWord
 
#define MemGetSize           Mem_GetWord
#define MemPushSize          Mem_PushWord

/*+-----------------------------------------------------------------------+
~GRP                    GENERIC REGION PACKET                             !
  +-----------------------------------------------------------------------+*/
/*     12.0.1                                                              */
boolean GrpMakeHdrChunks (tokentype    *Anchor,    grpheader    *GrpHdr,
                          numtype       AllocRgns, numtype       AllocXs,
                          ft F,lt Z,zz *Status)
{
idtype       CoreDbId        = Anchor->DbId;
numtype      AllocEntrys     = (AllocRgns * GrpHdr->RegionEntrys)
                             + GrpHdr->BaseIndex;
numtype      ExtendRegions   = AllocRgns;

   if (GrpHdr->BaseIndex)
      AllocRgns++;
  
   if (MemMakeChunk (sizeof(chunknum),  AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RegionChunkDir)))

   if (MemMakeChunk (sizeof(vbntype),   AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RegionSwapDir)))

   if (MemNewBitChunk
                    (AllocEntrys,       CoreDbId,
                     McStat,                       &(GrpHdr->DirtyRegionDir)))

   if (MemMakeChunk (sizeof(numtype),   AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RegionFreezeDir)))
   if (MemGetChunkAddr    
                    (GrpHdr->RegionFreezeDir,
                     McStat,            (addrtype *)&GrpHdr->FrozenRgns))

   if (MemMakeChunk (sizeof(numtype),   AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RgnBuffNumDir)))
   if (MemGetChunkAddr    
                    (GrpHdr->RgnBuffNumDir,
                     McStat,            (addrtype *)&GrpHdr->RgnBuffNums))

   if (MemMakeChunk (sizeof(numtype),   AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RgnAccessedDir)))
   if (MemGetChunkAddr    
                    (GrpHdr->RgnAccessedDir,
                     McStat,            (addrtype *)&GrpHdr->RgnAccessArray))


   if (MemMakeChunk (sizeof(vbntype),   AllocRgns,   ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->RegionVbnDir)))

   if (MemMakeChunk (sizeof(chunkindex),AllocXs,      AllocXs,
                     CoreDbId,          McStat,      &(GrpHdr->ExEntryTbl)))


   if (MemMakeChunk (sizeof(indextype), (GrpHdr->NumBuffRgns+1),ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->BuffRgnsDir)))
   if (MemPutStackPtr
                    (GrpHdr->BuffRgnsDir,           (GrpHdr->NumBuffRgns+1),
                     McStat))
   if (MemGetChunkAddr    
                    (GrpHdr->BuffRgnsDir,
                     McStat,           (addrtype *)&GrpHdr->RgnBuffRgns))

   if (MemMakeChunk (sizeof(indextype), (GrpHdr->NumBuffRgns+1),ExtendRegions,
                     CoreDbId,          McStat,    &(GrpHdr->BuffNodesDir)))
   if (MemPutStackPtr
                    (GrpHdr->BuffNodesDir,          (GrpHdr->NumBuffRgns+1),
                     McStat))
   if (MemGetChunkAddr    
                    (GrpHdr->BuffNodesDir,
                     McStat,           (addrtype *)&GrpHdr->RgnBuffNodes))


   /* Create an array that indicates how many Entrys have been deleted */
   if (MemMakeChunk (sizeof (indextype),       AllocRgns,   AllocRgns,
                     Anchor->DbId,    McStat,      &GrpHdr->RgnStatusDir))
   if (MemPutStackPtr
                    (GrpHdr->RgnStatusDir,     AllocRgns,   McStat))
   if (MemGetChunkAddr 
                    (GrpHdr->RgnStatusDir, 
                     McStat,      (addrtype *)&GrpHdr->RgnStats))

   {
      GrpHdr->RegionVbnDirVbn   = 0;
      GrpHdr->ExEntryTblVbn     = 0;

      GrpHdr->BaseRegion        = 0;
      GrpHdr->RegionsAllocd     = AllocRgns;

      GrpHdr->HeadRgnBuffs      = 0;
      GrpHdr->LastRgnBuff       = 0;
      GrpHdr->RgnBuffCount      = 0;
   }

TRACK(TrackBak,"GrpMakeHdrChunks\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.0.2                                                              */
boolean GrpGetPacket     (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status,    grpheader   **GrpHdr)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat, (addrtype *)GrpHdr))
  {
     if (*GrpHdr == NullPtr)
        DbmLoadGrpHdr (Anchor, GrpNum,     McStat,      GrpHdr);

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpGetPacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         REGION BLOCK MOVES                             !
  +-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*/

/*     12.0.5                                                              */
boolean GrpNewRegion     (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     Index,     indextype     RgnIndex,
                          ft F,lt Z,zz *Status,    
                          numtype      *RgnChunk,  addrtype     *RgnAddr)
{
indextype   StackPtr   = 0;
indextype   RgnEntrys  = 0;
indextype   Region     = 0;
bytessize   UnitSize   = 0;
sizetype    RegionSize = 0;
boolean     TempMemBug = MemBug;

*RgnAddr  = NullPtr;
 if (GrpBug == True)
     MemBug = True;

 if (RgnIndex          == 0 
 &&  GrpHdr->BaseIndex  > 0)
    RgnEntrys  = GrpHdr->BaseIndex;
 else
    RgnEntrys  = GrpHdr->RegionEntrys;


 /* Create the Region Space for ALLOC RegionEntrys                        */
 if (Normal(*Status)) 
 if (*RgnAddr)
 {
    RegionSize    = GrpHdr->RegionEntrys  * GrpHdr->EntrySize;
    if (Normal(*Status)) 
    if (MemPutStackPtr   (*RgnChunk,     0,          McStat))
        memset           (*RgnAddr,      NULL,       RegionSize);

 } else {
    if (MemMakeChunk     (GrpHdr->EntrySize,         RgnEntrys, 
                          0,                         CoreDb->DbId,
                          McStat,                    RgnChunk))
        MemGetChunkAddr  (*RgnChunk,     McStat,     RgnAddr);
 }
 
 if (Normal(*Status)) 
 if (GrpHdr->RegionVbnDir)
 if (MemGetStackPtr  (GrpHdr->RegionVbnDir,   McStat,  &StackPtr))
 if (RgnIndex      == StackPtr)   
     MemPushChunkNum (GrpHdr->RegionVbnDir,   0,        McStat, &StackPtr);

 if (Normal(*Status)) 
 if (MemPutIndex     (GrpHdr->RgnStatusDir,   RgnIndex, 0,       McStat))
 if (MemGetChunkAddr (GrpHdr->RgnStatusDir,             McStat, 
                                   (addrtype *)&GrpHdr->RgnStats))

 if (MemPushChunkNum (GrpHdr->RegionChunkDir,*RgnChunk, McStat, &StackPtr))
 if (MemPushChunkNum (GrpHdr->RegionSwapDir,  0,        McStat, &StackPtr))
 if (MemPushChunkNum (GrpHdr->RegionFreezeDir,0,        McStat, &StackPtr))
 if (MemPushChunkNum (GrpHdr->RgnBuffNumDir,  0,        McStat, &StackPtr))
 if (MemPushChunkNum (GrpHdr->RgnAccessedDir, 1,        McStat, &StackPtr))
 {
    Region             = (regionnum )StackPtr;
    if (MemPutBit       (GrpHdr->DirtyRegionDir, Region,   1,       McStat))


    if (MemGetChunkAddr (GrpHdr->RgnStatusDir, 
                         McStat,     (addrtype *)&GrpHdr->RgnStats))
    if (MemGetChunkAddr (GrpHdr->RegionFreezeDir, 
                         McStat,     (addrtype *)&GrpHdr->FrozenRgns))
    if (MemGetChunkAddr (GrpHdr->RgnBuffNumDir, 
                         McStat,     (addrtype *)&GrpHdr->RgnBuffNums))
    if (MemGetChunkAddr (GrpHdr->RgnAccessedDir, 
                         McStat,     (addrtype *)&GrpHdr->RgnAccessArray))
        MemGetChunkSize (GrpHdr->RegionChunkDir,               McStat, 
                         (indextype *)&GrpHdr->RegionsAllocd, &UnitSize);

    if (Normal(*Status))
    {
       GrpHdr->NoRgnsInCore    = False;
       GrpHdr->RegionCount++;

       if (Region != 0)
         GrpHdr->RegionIndex   = Region;
       else {
         if (GrpHdr->BaseIndex == 0)
           GrpHdr->RegionIndex = Region;
       }  

       if (RgnIndex == 0)
         GrpHdr->BaseRegion    = *RgnChunk;

       if (RgnIndex           != Region) 
          *Status              = Grp_BadSwapChunks;
    }
 }

 if (Normal(*Status)) 
 if (RgnIndex              > 0
 ||  GrpHdr->BaseIndex    == 0)
 if (GrpHdr->RgnBuffCount < GrpHdr->NumBuffRgns)
 {
    GrpHdr->RgnBuffCount++;
    GrpHdr->HeadRgnBuffs                          = GrpHdr->RgnBuffCount;
    GrpHdr->RgnBuffRgns[GrpHdr->RgnBuffCount]     = RgnIndex;
    MemPutWord        (GrpHdr->RgnBuffNumDir,       RgnIndex, 
                       GrpHdr->RgnBuffCount,        McStat);

 }
 MemBug = TempMemBug;

 if (Normal(*Status))
 if (DbmBug || SwapBug || DeBug)
 if (*RgnChunk)
    DumpChunkChunk (0, *RgnChunk);

TRACK(TrackBak,"GrpNewRegion\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.0.6                                                              */
boolean Grp_GetRegion    (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  ft F,lt Z,zz *Status,
                          numtype      *RgnChunk,  addrtype     *RgnAddr)
{
vbn           Vbn          = 0;
vbntype       RgnVbn       = 0;
boolean       CopyIn       = False;
boolean       FileIn       = False;
numtype       BuffIndex    = 0;
statustype    ShellStatus  = Env_Normal;
tokentype     Anchor       = NullToken;

 Anchor.DbId   = CoreDb->DbId;
 Anchor.Handle = 1;

*RgnAddr  = NullPtr;

 if (MemGetChunkNum  (GrpHdr->RegionChunkDir,  RgnIndex,  McStat,  RgnChunk))
 if (*RgnChunk)
 if (MemGetChunkAddr (*RgnChunk,               McStat,    RgnAddr))
 if (*RgnAddr != NullPtr)
 {
    if (GrpHdr->FreezePacket == False)
    if (MemGetWord   (GrpHdr->RgnBuffNumDir,   RgnIndex,  McStat, &BuffIndex))
      GrpHdr->RgnBuffNodes[BuffIndex] = 1;

    if (!Normal(*Status))
      Grp_DumpStruct (&Anchor,  GrpHdr->GrpNum, ShellMcStat);

    CacheAtRgnChunks++;

    TRACK(TrackBak,"GrpGetRegion\n");
    return(STAT);
 }

 if (MemGetVbn           (GrpHdr->RegionSwapDir,    RgnIndex,
                          McStat,                  &RgnVbn))
 if (RgnVbn   > 0)
 {
     CopyIn   = True;
 } else
     FileIn   = True;


 if (Normal(*Status))
 if (FileIn)
 {

   if (MemGetVbn      (GrpHdr->RegionVbnDir,    RgnIndex,  McStat, &Vbn))
   if (Vbn)
   {
      if (DbmFileInGrpRgnChunk (CoreDb,    GrpHdr,     RgnIndex,  False, 
                                McStat,    RgnChunk))
          MemGetChunkAddr      (*RgnChunk, McStat,     RgnAddr);

   } else {
      if (MemGetChunkNum (GrpHdr->RegionChunkDir,  RgnIndex,
                          McStat,                  RgnChunk))
      if (RgnChunk == 0)
         *Status = Grp_NullRegionChunk;
   }

   FileInRgnChunks++;

 } else if (CopyIn) {
  *Status  = Err_NotImplemented;


 }


 if (RgnIndex != 0)
    GrpHdr->RegionIndex = RgnIndex;
 else {
    if (GrpHdr->BaseIndex == 0)
       GrpHdr->RegionIndex = RgnIndex;
 }

 if (Normal(*Status))
 if (GrpHdr->EnvObjType  <= That_AttrNum)
    DbmPairRgnObjects  (CoreDb, (objheader *)GrpHdr,  *RgnChunk,  McStat);

TRACK(TrackBak,"GrpGetRegion\n");
return(STAT);
}
 


/*     12.0.9                                                              */
boolean Grp_CleanRgnChunks
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status)
{
numtype    OldValue   = 0;
indextype  StackPtr   = 0;
numtype    RgnVbn     = 0;
numtype    ChunkRgn   = 0;
numtype    SwapVbn    = 0;
indextype  BuffIndex  = 0;

 if (Normal(*Status)) 
 if (MemPopChunkNum  (GrpHdr->RegionSwapDir,  McStat, &SwapVbn,  &StackPtr))
 if (MemPopChunkNum  (GrpHdr->RgnStatusDir,  McStat, &OldValue,  &StackPtr))
 if (MemPopVbn       (GrpHdr->RegionVbnDir,   McStat, &RgnVbn,   &StackPtr))
 if (MemPopChunkNum  (GrpHdr->RegionChunkDir, McStat, &ChunkRgn, &StackPtr))
 if (MemPopChunkNum  (GrpHdr->RegionFreezeDir,McStat, &OldValue, &StackPtr))
 if (MemPopChunkNum  (GrpHdr->RgnBuffNumDir,  McStat, &OldValue, &StackPtr))
 if (MemPopChunkNum  (GrpHdr->RgnAccessedDir, McStat, &OldValue, &StackPtr))
 {
     GrpHdr->RgnStats[RgnIndex] = 0;
     GrpHdr->RegionCount--;
     if (RgnIndex != 0)
       GrpHdr->RegionIndex      = RgnIndex;
     else
       GrpHdr->BaseRegion       = 0;
     if (RgnVbn)
       MemFreeFileBlk     (&CoreDb->BlkHdr, McStat, &RgnVbn);
 }
 if (Normal(*Status)) 
 if (RgnChunk == ChunkRgn)
 {
     MemFreeChunk         (&ChunkRgn,   McStat);
 } else {
    *Status   = Grp_RgnNotMatched;
 }

 if (Normal(*Status)) 
 if (RgnIndex              > 0
 ||  GrpHdr->BaseIndex    == 0)
 if (GrpHdr->RgnBuffCount  > 0)
 if (GrpHdr->RgnBuffCount  < GrpHdr->NumBuffRgns)
 {
    GrpHdr->HeadRgnBuffs              = --GrpHdr->RgnBuffCount;
    BuffIndex                         =   GrpHdr->RgnBuffNums[RgnIndex];
    GrpHdr->RgnBuffNums[RgnIndex]     = 0;

    while (BuffIndex <= GrpHdr->RgnBuffCount)
    {
       RgnIndex                       = GrpHdr->RgnBuffRgns[BuffIndex+1];

       GrpHdr->RgnBuffNodes[BuffIndex]= GrpHdr->RgnBuffNodes[BuffIndex+1];

       GrpHdr->RgnBuffRgns[BuffIndex] = GrpHdr->RgnBuffRgns[BuffIndex+1];

       GrpHdr->RgnBuffNums[RgnIndex]  = BuffIndex;

       BuffIndex++;
    }

 } else if (GrpHdr->FreezePacket == False) {

    GrpHdr->HeadRgnBuffs              = --GrpHdr->RgnBuffCount;
    BuffIndex                         =   GrpHdr->RgnBuffNums[RgnIndex];
    GrpHdr->RgnBuffNums[RgnIndex]     = 0;

    while (BuffIndex <= GrpHdr->RgnBuffCount)
    {
       RgnIndex                       = GrpHdr->RgnBuffRgns[BuffIndex+1];

       GrpHdr->RgnBuffNodes[BuffIndex]= GrpHdr->RgnBuffNodes[BuffIndex+1];

       GrpHdr->RgnBuffRgns[BuffIndex] = GrpHdr->RgnBuffRgns[BuffIndex+1];

       GrpHdr->RgnBuffNums[RgnIndex]  = BuffIndex;

       BuffIndex++;
    }

 }


TRACK(TrackBak,"GrpCleanRgnChunks\n");
return(STAT);
}


/*     12.0.10                                                             */
boolean Grp_DeleteRgn    
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status)
{
int           i            = -1;
indextype     Ilow         = 0;
indextype     Ihigh        = 0;
indextype     Ilimit       = 0;
numtype      *ExEntrys     = NullPtr;
numtype       Hits         = 0;
indextype     CurrIndex    = 0;
indextype     RgnNum       = 0;
numtype       Region       = 0;
bytetype     *ByteAddr     = NullPtr;
numtype       RegionEntrys = 0;
indextype     ExEntryCount = 0;

   if (MemGetStackPtr   (RgnChunk,             McStat,         &RegionEntrys))
   if (MemGetChunkAddr  (GrpHdr->ExEntryTbl,   McStat, (addrtype *)&ExEntrys))
   if (MemGetStackPtr   (GrpHdr->ExEntryTbl,   McStat,         &ExEntryCount))
   if (GrpHdr->BaseIndex > 0)
   {
      if (RgnIndex == 0)
      {
         Ilow    = 0;
         Ihigh   = GrpHdr->BaseIndex - 1;

      } else {
         Ilow    = GrpHdr->BaseIndex + (GrpHdr->RegionEntrys * (RgnIndex -1));
         Ihigh   = Ilow              +  GrpHdr->RegionEntrys - 1;
      }

   } else {
      Ilow       = GrpHdr->RegionEntrys *  RgnIndex;
      Ihigh      = Ilow                 +  GrpHdr->RegionEntrys - 1;
   }

   if (GrpHdr->ExEntryCount != ExEntryCount)
   {
     *Status            = Grp_BadExEntryTable;
   }

   Ilimit        = RegionEntrys;
   CurrIndex     = 0;
   if (Normal(*Status))
   for (i        = 0,     Hits         = 0; 
        i        < GrpHdr->ExEntryCount;
      ++i)
/*
//     || Hits     < Ilimit; ++i)
*/
   {
      if (Hits    >= Ilimit)
          ExEntrys[CurrIndex++]     = ExEntrys[i];
    
      else if (ExEntrys[i] >= Ilow
           &&  ExEntrys[i] <= Ihigh)
        ++Hits;
      else 
          ExEntrys[CurrIndex++]     = ExEntrys[i];
   }

   if (Normal(*Status))
   if (MemPutStackPtr  (GrpHdr->ExEntryTbl, CurrIndex,   McStat))
   {
      GrpHdr->ExEntryCount          = CurrIndex;
      GrpHdr->EntryCount           -= Hits;
      if (Hits            != Ilimit)
        *Status            = Grp_BadExEntryTable;

   }
   GrpHdr->RgnStats[RgnIndex]     = 0;

   if (Normal(*Status))
       Grp_CleanRgnChunks (CoreDb,  GrpHdr,  RgnIndex,  RgnChunk,  McStat);

   if (Normal(*Status))
   if (GrpHdr->RegionCount)
   if (GrpHdr->BaseIndex    == 0
   ||  GrpHdr->RegionCount   > 1)
   if (GrpHdr->RgnStats[GrpHdr->RegionCount-1] 
                            == GrpHdr->RegionEntrys)
   {
      RgnNum     =  GrpHdr->RegionCount - 1;
      if (GrpGetRegion    (CoreDb,     GrpHdr,   RgnNum,     
                           McStat,    &Region,  (addrtype *)&ByteAddr))
      if (MemPutBit       (GrpHdr->DirtyRegionDir,      RgnNum, 1, McStat))
         Grp_DeleteRgn    (CoreDb,     GrpHdr,   RgnNum,  Region,  McStat);
   }

TRACK(TrackBak,"GrpDeleteRgn\n");
return(STAT);
}

/*     12.0.11                                                             */
boolean Grp_RgnSwapTopAndPop
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  numtype       RgnChunk,
                          ft F,lt Z,zz *Status)
{
int           i            = -1;
indextype     Ilow         = 0;
indextype     Ihigh        = 0;
indextype     Ibase        = 0;
indextype     Idiff        = 0;
indextype     Ilimit       = 0;
numtype      *ExEntrys     = NullPtr;
numtype       Hits         = 0;
indextype     CurrIndex    = 0;
numtype       RegionEntrys = 0;
indextype     vIndex       = 0;
numtype       ExRegion     = 0;
numtype       ExRgnEntrys  = 0;
size_t        BytesSize    = 0;
addrtype      SourceAddr   = NullPtr;
addrtype      TargetAddr   = NullPtr;

 if (GrpGetRegion     (CoreDb,     GrpHdr,      GrpHdr->RegionCount -1,     
                       McStat,    &ExRegion,   &SourceAddr))
 if (MemGetStackPtr   (ExRegion,   McStat,     &ExRgnEntrys))
 if (ExRgnEntrys == GrpHdr->RegionEntrys)
 {
   if (MemGetStackPtr   (RgnChunk,             McStat,         &RegionEntrys))
   if (MemGetChunkAddr  (GrpHdr->ExEntryTbl,   McStat, (addrtype *)&ExEntrys))
   if (RgnIndex          == 0 
   &&  GrpHdr->BaseIndex != 0)
   {
      *Status    = Grp_CanNotSwapBaseRgn;

   } else {
      Ilow       = GrpHdr->BaseIndex + (GrpHdr->RegionEntrys * (RgnIndex -1));
      Ihigh      = Ilow              +  GrpHdr->RegionEntrys - 1;
   }
   BytesSize     = GrpHdr->RegionEntrys *  GrpHdr->EntrySize;
   Ilimit        = RegionEntrys;
   CurrIndex     = 0;

   /* Delete Index's of Region from the ExEntrys array                    */
   if (Normal(*Status))
   for (i = 0,         Hits         = 0;
        i    < GrpHdr->ExEntryCount
   ||   Hits < Ilimit; ++i)
   {
      if (ExEntrys[i] >= Ilow
      &&  ExEntrys[i] <= Ihigh)
        ++Hits;
      else 
          ExEntrys[CurrIndex++]     = ExEntrys[i];
   }

   if (Normal(*Status))
   {
      GrpHdr->ExEntryCount          = CurrIndex;
      GrpHdr->EntryCount           -= Hits;
      if (Hits                     != Ilimit)
        *Status                     = Grp_BadExEntryTable;

   }

   /* ReNumber LocalIndex's in GrpIndexDir to Swaped position index       */
   if (Normal(*Status))
   {
      Ibase            = Ilow;
      Idiff            = GrpHdr->EntryCount   - Ilow;
      Ilow             = GrpHdr->EntryCount   - GrpHdr->RegionEntrys; 
      Ihigh            = GrpHdr->EntryCount   - 1; 
      for (i = 0;
           i < GrpHdr->GrpDirCount &&   Hits < RegionEntrys; ++i)
      {
         if (GrpHdr->GrpDirAddr[i] > 0)
         {
            vIndex                 = abs((int )GrpHdr->GrpDirAddr[i]);
            if (vIndex            >= Ilow
            &&  vIndex            <= Ihigh)
            {
               ++Hits;
                 GrpHdr->GrpDirAddr[i]  =  vIndex - Idiff;
            }
         }
      }
   }

   /* Copy Top-most 'RegionEntrys' entrys into swap-chunk                 */
   if (Normal(*Status))
   if (GrpGetRegion     (CoreDb,     GrpHdr,      GrpHdr->RegionCount -1,     
                         McStat,    &ExRegion,   &SourceAddr))
   if (MemGetStackPtr   (ExRegion,   McStat,     &ExRgnEntrys))
   if (MemGetChunkAddr  (RgnChunk,   McStat,     &TargetAddr))
   {
      memcpy            (TargetAddr, SourceAddr,  BytesSize); 
      Grp_CleanRgnChunks(CoreDb, GrpHdr, GrpHdr->RegionCount - 1, ExRegion,
                         McStat);
   }

   if (*Status     == Grp_CanNotSwapBaseRgn)
       *Status      = Env_Normal;

 }

TRACK(TrackBak,"Grp_RgnSwapTopAndPop\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF GRP0.C                                  !
  +-----------------------------------------------------------------------+*/
