/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - VDBM.c */
#define DBM_C
#define VDBM
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "vdbm.h"
#include "gdbm.h"
#include "files.h"
#include "env0.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...VDBM... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[37] =
{
   "   DbmNewVchunkDir Db[%3u] VchunkDir=%4u\n",
   "   DbmNewVchunk[%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "   DbmPutVchunkTkn [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "   DbmFreeVchunk [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "   DbmDeleteVchunk[%3u:%4u] \n",
   "   DbmDeleteVchunk [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   " Warming... DbmCommitChunk [%3u:%4u] Null Handle\n",
   "    DbmFileOutVchunk[%3u:%4u] : InCore[%4u]; OnDisk[%3u]\n",
   "    DB Vchunk[%3u]          : InCore[%4u]; OnDisk[%3u]\n",
   " ***ERROR...DbmFileOutVchunk[%3u:%4u] > Index=%4u\n",
   "    DbmFileOutVchunks    : Release= %s\n",
   "    DB Vchunk[%3u]          : InCore[%4u]; OnDisk[%3u]\n",
   "    DB Vchunk[%3u]          : InCore[%4u]; OnDisk[%3u]\n",
   "    Number Vchunk_DIR Chunks Filed out = %u;\n",
   "    DbmFileOutGrpRgns    : Release= %s\n",
   "    DB Vchunk[%3u] from Chunk[%4u]:: InCore[%4u]; OnDisk[%3u]\n",
   "    DB Vchunk[%3u] NULL Addr : InCore[%4u]; OnDisk[%3u]\n",
   "    Number Vchunk_GRP Chunks[*%6u]... Filed out = %u;\n",
   "   Invoke Block Chunk. DiskBlk=%3u, Chunks=%3u\n",
   "   InvokeVchunk:: Db=%3u BlkTkn[%4u].Owner=%3u; InCore=%4u, OnDisk=%4u\n",
   "   Free Block Chunk=%3u\n",
   "   DbmInvokeVchunk[%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "   DbmGetVchunkTkn [%3u:%4u]...\n",
   "   DbmGetVchunkTkn [%3u:%4u] InCore = ZERO(%u) OnDisk= %4u\n",
   "               Rgn [%3u:%4u]; Chunk[%5u] @%8x Local @%8x\n",
   "   DbmGetVchunkTkn [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "  *** DbId > EnvDb's Count(%4u).\n",
   "      DbmDumpVchunkVchunk [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "  *** DbId > EnvDb's Count(%4u).\n",
   "      DbmDumpVchunk       [%3u:%4u] InCore = %4u, OnDisk= %4u\n",
   "      Limit0 = %4u, Limit1 = %4u\n",
   "   Invoke Block Chunk. DiskBlk=%3u, Chunks=%3u\n",
   "   Db=%3u BlkTkn[%4u].Owner=%3u; InCore=%4u, OnDisk=%4u\n",
   "   Free Block Chunk=%3u\n",
   "    DbmFreeVchunks    : Release= %s\n",
   "    Number Vchunk_DIR Chunks Freeed = %u;\n",
   "    Number Vchunk_GRP Chunks Filed out = %u;\n"
};
#endif

/*+-----------------------------------------------------------------------+*
~DBM                            VDBM
 *+-----------------------------------------------------------------------+*/
extern   idtype        DbmDbId;      /* from dbm.h                         */
extern   booleantype   NewDbFile;
 
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)
 
#define    MemGetVbn          Mem_GetWord
#define    MemPushVbn         Mem_PushWord
#define    MemPutVbn          Mem_PutWord

#define    MemGetChunkNum     Mem_GetWord
#define    MemPutChunkNum     Mem_PutWord

#define    MemGetIndex        Mem_GetWord

#define    MemFreeFileBlk     Mem_FreeFileBlk

extern boolean MemFreeFileBlk (blkhdrtype   *BlkHdr,
                               ft F,lt Z,zz *Status,   vbntype      *Vbn);

extern boolean Grp_GetRegion
                         (dbheader     *CoreDb,    grpheader    *GrpHdr,
                          indextype     RgnIndex,  ft F,lt Z,zz *Status,
                          numtype      *RgnChunk,  addrtype     *RgnAddr);

/*
vbntype       BlkDir_Vbn      = 0;
vbntype       BlkTkn_Vbn      = 0;
vbntype       BlkSize_Vbn     = 0;
farlongtype   BlkDir_Offset   = 0;
farlongtype   BlkTkn_Offset   = 0;
farlongtype   BlkSize_Offset  = 0;
indextype     BlkDir_Units    = 0;
indextype     BlkTkn_Units    = 0;
*/

/*+-----------------------------------------------------------------------+*
                         Vchunk Activities
 *+-----------------------------------------------------------------------+*/

boolean DbmNewVchunk     (tokentype    *Anchor,    sizetype     UnitSize,
                          indextype     UnitCount, indextype    ExtendQty, 
                          ft F,lt Z,zz *Status,  
                          indextype    *Vchunk,    addrtype    *ChunkAddr)
{
       dbheader     *CoreDb       = NullPtr;
       vchunktkntype VchunkToken;
       grpheader    *GrpHdr       = NullPtr;
static sizetype      EntrySize    = sizeof (vchunktkntype);
static numtype       AllocRgns    = 0;
static numtype       RgnEntrys    = 125;
static numtype       AllocXs      = 1000;
static numtype       BaseIndex    = 2;
static accesstype    AccessType   = Read_Write;
static boolean       DiskCache    = False;
static numtype       CacheLimit   = 0;
static numtype       CacheLoad    = 0;
static numtype       NumBuffRgns  = 0;
       numtype       GrpNum       = 0;
       numtype       BaseRegion   = 0;

  DiskCache    = False;
  AllocRgns    = (Alloc_Handles/125) + 22;
  NumBuffRgns  = AllocRgns;
  CacheLimit   = AllocRgns;
  CacheLoad    = 100;
  VchunkToken.OnDisk = 0;
  VchunkToken.InCore = 0;

  if (TmGetCoreDb      (Anchor,  McStat,           &CoreDb))
  if (CoreDb->VchunkGrpNum == 0)
  {
     if (GrpNewPacket   (Anchor,    EntrySize,   AllocRgns,   RgnEntrys,
                         AllocXs,   BaseIndex,   AccessType,  DiskCache,
                         CacheLimit,CacheLoad,   NumBuffRgns,
                         McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))
     if (GrpNewEntry    (Anchor,    GrpNum, 
                         McStat,    Vchunk,       (addrtype )&VchunkToken))
        GrpHdr->EnvObjType    = UnitChunk_AttrNum;

     CoreDb->VchunkGrpNum  = GrpNum;
     CoreDb->VchunkGrpHdr  = GrpHdr;

     if (Normal(*Status))
     if (Anchor->DbId > WenvId)
     if (CoreDb->VchunkCommitDir  == 0)
     if (MemNewBitChunk (10000,      Anchor->DbId,
                         McStat,                  &(CoreDb->VchunkCommitDir)))
     if (MemMakeChunk   (sizeof(vbntype),   10000,    2000,
                         Anchor->DbId,      McStat, &(CoreDb->VchunkSwapDir)))
         MemPutStackPtr (CoreDb->VchunkSwapDir,  1,           McStat);
  }

  if (Normal(*Status))
  if (MemMakeChunk    (UnitSize,  UnitCount,  ExtendQty,  Anchor->DbId,
                       McStat,                           &VchunkToken.InCore))
      GrpPutNewEntry  (Anchor,    CoreDb->VchunkGrpNum, 
                       (addrtype )&VchunkToken,
                       McStat,                            Vchunk);
  if (Normal(*Status))
      MemGetChunkAddr (VchunkToken.InCore,    McStat,     ChunkAddr);

#ifdef   __FREEZE_VCHUNKS__
  if (Normal(*Status))
  if (Anchor->DbId > WenvId)
  if (CoreDb->VchunkCommitDir)
  if (MemPutBit       (CoreDb->VchunkCommitDir,  *Vchunk, 1, 
                       McStat))
      MemPutVbn       (CoreDb->VchunkSwapDir,    *Vchunk, 0, 
                       McStat);
#endif /* __FREEZE_VCHUNKS__  */

TRACK(TrackBak,"DbmNewVchunk\n");
return (STAT);
}
 

boolean    DbmPutVchunkTkn (tokentype    *Token,  vchunktkntype *VchunkTkn,
                            ft F,lt Z,zz *Status)
{
dbheader      *CoreDb       = NullPtr;

  if (!Token->Handle)
     *Status = Oa_VchunkNotExist;

  if (TmGetCoreDb     (Token, McStat,    &CoreDb))
  if (CoreDb->VchunkGrpHdr == 0)
      GrpGetPacket    (Token,      CoreDb->VchunkGrpNum,     
                       McStat,    &CoreDb->VchunkGrpHdr);

  if (Normal(*Status))
     GrpPutEntry            (Token,     CoreDb->VchunkGrpNum,Token->Handle,
                             (addrtype )VchunkTkn,           McStat);

TRACK(TrackBak,"DbmPutVchunkTkn\n");
return (STAT);
}
 

boolean    DbmFreeVchunk   (tokentype    *Vtoken,  ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;
numtype       IsCommitted  = 0;

#ifdef   __FREEZE_VCHUNKS__
dbheader     *DbHdr        = NullPtr;
#endif /* __FREEZE_VCHUNKS__  */

  if (!Vtoken->Handle)
     *Status = Oa_VchunkNotExist;

  if (Normal(*Status))
  if (DbmGetVchunkTkn    (Vtoken,          McStat,          &Vchunk))

  if (Normal(*Status))
  if (Vchunk.InCore) 
#ifdef   __FREEZE_VCHUNKS__
  if (TmGetCoreDb       (Vtoken,          McStat,       &DbHdr))
  if (DbHdr->VchunkCommitDir)
      MemGetBit         (DbHdr->VchunkCommitDir,  Vtoken->Handle, 
                         McStat,                 &IsCommitted);
#endif /* __FREEZE_VCHUNKS__  */

  if (Normal(*Status))
  if (IsCommitted == 0)
  if (MemFreeChunk      (&Vchunk.InCore,   McStat))
      DbmPutVchunkTkn   (Vtoken,          &Vchunk,           McStat);


TRACK(TrackBak,"DbmFreeVchunk\n");
return (STAT);
}
 

boolean    DbmDeleteVchunk (tokentype    *Vtoken,  ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
vchunktkntype Vchunk;

#ifdef   __FREEZE_VCHUNKS__
#endif

  if (!Vtoken->Handle)
     *Status = Oa_VchunkNotExist;

  if (DbmInvokeVchunk    (Vtoken,          McStat,          &Vchunk))
  if (Normal(*Status))
  if (Vchunk.InCore) 
      MemFreeChunk       (&Vchunk.InCore,  McStat);

  if (Normal(*Status))
  if (TmGetCoreDb        (Vtoken,          McStat,       &CoreDb))
  if (Vchunk.OnDisk)
  {

     *Status  = Err_NotImplemented;


  }

  if (Normal(*Status))
  if (CoreDb->VchunkSwapDir)
  if (MemPutVbn       (CoreDb->VchunkSwapDir,   Vtoken->Handle,  0, 
                       McStat))
      MemPutBit       (CoreDb->VchunkCommitDir, Vtoken->Handle,  0, 
                       McStat);

  if (Normal(*Status))
      GrpDeleteEntry    (Vtoken,           CoreDb->VchunkGrpNum,
                         Vtoken->Handle,   McStat);


TRACK(TrackBak,"DbmDeleteVchunk\n");
return (STAT);
}


boolean DbmCommitVchunk  (tokentype    *VchunkTkn, boolean     Release,
                          ft F,lt Z,zz *Status)
{
dbheader   *DbHdr        = NullPtr;

  if (VchunkTkn->Handle == 0)
    return (STAT);

#ifndef   __FREEZE_VCHUNKS__
  if (TmGetCoreDb       (VchunkTkn,          McStat,       &DbHdr))
  {
     CoreBug     = False;
     PageBug     = False;
     Index       = VchunkTkn->Handle;
     if (DbmGetVchunkTkn  (VchunkTkn,    McStat,  &Vchunk))
     {
        if (Vchunk.InCore)
        if (FileOutChunk   (&Vchunk.InCore,           &(DbHdr->BlkHdr),
                            Release,       McStat,    &Vchunk.OnDisk))
            Vchunk.InCore = 0;

        if (Normal(*Status))
            GrpPutEntry    (&DbToken,      DbHdr->VchunkGrpNum,
                            Index,
                            (addrtype )&Vchunk,        McStat);

     }
     CoreBug      = TempCoreBug;
     PageBug      = TempPageBug;

  } else {
  }
#else
  if (Normal(*Status))
  if (TmGetCoreDb       (VchunkTkn,          McStat,       &DbHdr))
  if (DbHdr->VchunkCommitDir)
  if (MemPutBit         (DbHdr->VchunkCommitDir,  VchunkTkn->Handle, 1, 
                         McStat))
  if (Release)
      MemPutVbn         (DbHdr->VchunkSwapDir,    VchunkTkn->Handle, 0, 
                         McStat);

#endif /* __FREEZE_VCHUNKS__  */

TRACK(TrackBak,"DbmCommitVchunk\n");
return(STAT);
}
 



/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/

boolean    DbmInvokeVchunk (tokentype    *Token,
                            ft F,lt Z,zz *Status, vchunktkntype *VchunkToken)
{
dbheader     *CoreDb       = NullPtr;
booleantype   Release      = False;
boolean       TmpFileBug   = FileBug;
chunkstruc *ChunkStrucPtr = NullPtr;
numtype     NumBlkArrays   = 0;
numtype     ChunkBlk       = 0;
char       *BlkAddr        = NullPtr;
chunkstruc *ChunkStrucRef  = NullPtr;  
blktkntype *BlkTkns        = NullPtr;
numtype     Chunk          = 0;
addrtype    ChunkAddr      = NullPtr;
int         i              = 0;
indextype   Index          = 0;
numtype     InCore         = 0;

  if (!Token->Handle)
     *Status = Oa_VchunkNotExist;

  if (Normal(*Status))
  if (TmGetCoreDb      (Token,     McStat,            &CoreDb))
  if (DbmGetVchunkTkn  (Token,     McStat,             VchunkToken))
  if (VchunkToken->InCore == 0)
  {
     if (VchunkToken->OnDisk)
     {
        if (FileInChunk      (&VchunkToken->InCore,   &CoreDb->BlkHdr,
                              Release,
                              McStat,                 &VchunkToken->OnDisk))
        if (MemGetChunkAddr  (VchunkToken->InCore,
                              McStat,                 (addrtype *)&BlkAddr))
        {
            ChunkBlk      = VchunkToken->InCore;
            ChunkStrucRef = (chunkstruc *)(BlkAddr - sizeof(chunkstruc));
        }

        if (Normal(*Status))
        if (ChunkStrucRef->MaxUnitLength != MAX_UNIT_LENGTH)
        {
           GrpPutEntry       (Token,     CoreDb->VchunkGrpNum,  Token->Handle,
                              (addrtype )VchunkToken,           McStat);

        } else {
           NumBlkArrays  = ChunkStrucRef->StackPtr;
           BlkTkns       = (blktkntype *)BlkAddr;
           InCore        = 0;
           i             = 0;
           while (Normal(*Status)
           &&     ++i <= NumBlkArrays)
           {
            Index              = BlkTkns[i].Owner;
            Chunk              = 0;
            if (Index)
            {
              ChunkAddr        = (addrtype )(BlkAddr
                                 + (BlkTkns[i].Offset + sizeof (chunkstruc)));
              if (NewChunkChunk      (McStat,    &Chunk))
              if (MemPutChunkAddr    (Chunk,      ChunkAddr,    McStat))
                  ChunkStrucPtr = (chunkstruc *)((char *)ChunkAddr 
                                                 - sizeof (chunkstruc));
              VchunkToken->InCore = Chunk;
              if (Normal(*Status))
              if (MemPutChunkStruc (Chunk,     ChunkStrucPtr,   Token->DbId,
                                    McStat))
                  GrpPutEntry      (Token,     CoreDb->VchunkGrpNum,  
                                    Index,     (addrtype )VchunkToken,
                                    McStat);

            }
            if (Chunk)
                 MemDumpChunk (0, Chunk, McStat);

            if (Index == Token->Handle)
               InCore  = VchunkToken->InCore;
           }

           if (Normal(*Status))
           {
              VchunkToken->InCore      = InCore;
              ChunkStrucRef->UnitCount = BlkTkns[1].Offset;
              MemFreeChunk (&ChunkBlk, McStat);
           }
        }


     } else
        *Status = Oa_VchunkNotExist;
  }

  FileBug = TmpFileBug;

TRACK(TrackBak,"DbmInvokeVchunk\n");
return (STAT);
}
 

boolean    DbmGetVchunkTkn (tokentype    *Token,
                            ft F,lt Z,zz *Status,  vchunktkntype *VchunkTkn)
{
dbheader      *CoreDb       = NullPtr;
grpheader     *GrpHdr       = NullPtr;
vchunktkntype *VchunkDir    = NullPtr;
numtype        RgnNum       = 0;
indextype      LocalIndex   = 0;
numtype        Chunk        = 0;
indextype      StackPtr     = 0;

  if (!Token->Handle)
     *Status = Oa_VchunkNotExist;

  if (TmGetCoreDb     (Token, McStat,    &CoreDb))
  if ((GrpHdr = CoreDb->VchunkGrpHdr) == NullPtr)
  {
     if (GrpGetPacket    (Token,      CoreDb->VchunkGrpNum,     
                          McStat,    &CoreDb->VchunkGrpHdr))
     {
        GrpHdr   = CoreDb->VchunkGrpHdr;
        StackPtr = GrpHdr->EntryCount + 1;
     }

     if (Normal(*Status))
     if (DbmBug)
        Grp_DumpStruct   (Token, CoreDb->VchunkGrpNum,  McStat);

#ifdef   __FREEZE_VCHUNKS__
     if (Normal(*Status)) 
     if (CoreDb->DbId   > WenvId)
     if (MemNewBitChunk  (StackPtr,             CoreDb->DbId,
                          McStat,              &(CoreDb->VchunkCommitDir)))

     if (MemMakeChunk    (sizeof(vbntype),   StackPtr,      2000,
                          CoreDb->DbId,   McStat, &(CoreDb->VchunkSwapDir)))
         MemPutStackPtr  (CoreDb->VchunkSwapDir,  StackPtr,       McStat);

#endif /* __FREEZE_VCHUNKS__  */
  }

  if (Normal(*Status))
  {
     if (Token->Handle < GrpHdr->BaseIndex)
     {
        RgnNum     = 0;
        LocalIndex = Token->Handle;
     } else {
        RgnNum     = (Token->Handle - GrpHdr->BaseIndex)
                    / GrpHdr->RegionEntrys;
        if (GrpHdr->BaseIndex)
           RgnNum++;
        LocalIndex = (Token->Handle - GrpHdr->BaseIndex)
                    % GrpHdr->RegionEntrys;
     }

     if (GrpGetRegion (CoreDb,     GrpHdr,                         RgnNum,     
                       McStat,    &Chunk,          (addrtype *)&VchunkDir))
       *VchunkTkn    = VchunkDir[LocalIndex];
     if ( RgnNum == 0 )
	GrpHdr->BaseRegion = Chunk ;


     if (VchunkTkn->InCore == 0
     &&  VchunkTkn->OnDisk == 0)
     if (MemGetChunkNum  (GrpHdr->RegionChunkDir, RgnNum, McStat, &Chunk))
     if (Chunk)
         DumpChunkChunk  (0, Chunk);
     
  }

TRACK(TrackBak,"DbmGetVchunkTkn\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*
 ~L                           DEBUG    
 *+-----------------------------------------------------------------------+*/
boolean    DbmDumpVchunkVchunk
                           (tokentype    *Token,   ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;
dbheader     *CoreDb       = NullPtr;

  if (Token->Handle == 0)
  {
     TraceMsg (0,  "  *** NULL Handle to Virtual Chunk.\n");
    *Status = Oa_VchunkNotExist;
  }

  if (Token->DbId    > CoreDbCount)
  {
    *Status = Oa_VchunkNotExist;
  }

  if (Normal(*Status))
  if (TmGetCoreDb      (Token,     McStat,            &CoreDb))
  if (DbmGetVchunkTkn  (Token,     McStat,            &VchunkToken))

  if (Normal(*Status))
  if (VchunkToken.InCore)
     DumpChunkChunk    (0, VchunkToken.InCore);

TRACK(TrackBak,"DbmDumpVchunkVchunk\n");
return (STAT);
}


boolean    DbmDumpVchunk   (tokentype    *Token,  
                            numtype       Limit0, numtype        Limit1,
                            ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;
dbheader   *CoreDb         = NullPtr;
booleantype Release        = False;
chunkstruc *ChunkStrucPtr  = NullPtr;
numtype     NumBlkArrays   = 0;
numtype     ChunkBlk       = 0;
char       *BlkAddr        = NullPtr;
chunkstruc *ChunkStrucRef  = NullPtr;  
blktkntype *BlkTkns        = NullPtr;
numtype     Chunk          = 0;
addrtype    ChunkAddr      = NullPtr;
int         i              = 0;
indextype   Index          = 0;
boolean     NotFound       = True;

  if (Token->Handle == 0)
  {
     TraceMsg (0,  "  *** NULL Handle to Virtual Chunk.\n");
    *Status = Oa_VchunkNotExist;
  }

  if (Token->DbId    > CoreDbCount)
  {
    *Status = Oa_VchunkNotExist;
  }

  if (Normal(*Status))
  if (TmGetCoreDb      (Token,     McStat,            &CoreDb))
  if (DbmGetVchunkTkn  (Token,     McStat,            &VchunkToken))

  if (Normal(*Status))
  if (VchunkToken.InCore  == 0)
  {
     if (VchunkToken.OnDisk)
     {
        if (FileInChunk      (&VchunkToken.InCore,    &CoreDb->BlkHdr,
                              Release,
                              McStat,                 &VchunkToken.OnDisk))
        if (MemGetChunkAddr  (VchunkToken.InCore,
                              McStat,                 (addrtype *)&BlkAddr))
        {
            ChunkBlk      = VchunkToken.InCore;
            ChunkStrucRef = (chunkstruc *)(BlkAddr - sizeof(chunkstruc));
        }

        if (Normal(*Status))
        if (ChunkStrucRef->MaxUnitLength 
        !=  MAX_UNIT_LENGTH)
        {
           if (Mem_DumpChunkPart (0, VchunkToken.InCore, 
                                  Limit0,  Limit1,    McStat))
               MemFreeChunk (&VchunkToken.InCore,     McStat);

        } else {
           NumBlkArrays  = ChunkStrucRef->StackPtr;
           BlkTkns       = (blktkntype *)BlkAddr;

           i             = 0;
           while (Normal(*Status)
           &&     ++i <= NumBlkArrays
           &&     NotFound)
           {
            Index              = BlkTkns[i].Owner;
            Chunk              = 0;
            if (Index  == Token->Handle)
            {
              NotFound         = False;
              ChunkAddr        = (addrtype )(BlkAddr
                                 + (BlkTkns[i].Offset + sizeof (chunkstruc)));
              if (NewChunkChunk      (McStat,    &Chunk))
              if (MemPutChunkAddr    (Chunk,      ChunkAddr,    McStat))
                  ChunkStrucPtr = (chunkstruc *)((char *)ChunkAddr 
                                                 - sizeof (chunkstruc));
              VchunkToken.InCore = Chunk;
              if (Normal(*Status))
                  MemPutChunkStruc (Chunk,     ChunkStrucPtr,   Token->DbId,
                                    McStat);
            }
            if (Normal(*Status))
            if (Chunk)
            if (Mem_DumpChunkPart   (0, Chunk,  Limit0,  Limit1, McStat))
                MemFreeChunkNum     (&Chunk,    McStat);
           }

           if (Normal(*Status))
              MemFreeChunk      (&ChunkBlk, McStat);
        }


     } else
        TraceMsg (0, " *** DbmVchunkDump:: NULL virtual chunk\n");

  } else
     Mem_DumpChunkPart (0, VchunkToken.InCore, Limit0, Limit1,  McStat);

  if (*Status == Oa_VchunkNotExist)
      *Status  = Env_Normal;


TRACK(TrackBak,"DbmDumpVchunk\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF VDBM.C                                  !
  +-----------------------------------------------------------------------+*/
