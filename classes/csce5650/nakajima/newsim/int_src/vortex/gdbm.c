/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - DBM0.c */
#define DBM_C
#define GDBM
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "gdbm.h"
#include "files.h"
#include "env0.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...GDBM... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[36] =
{
   "\n    DbmFileOutGrpHdr[%4u] ...\n",
   "    Grp ExEntryTbl      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp RgnStatusDirVbn : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp->GrpIndexDir    : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp RegionVbnDir     : Chunk[%4u] AT Vbn[%3u]\n",
   "    DbmFileOutGrpRgns    : Release= %s\n",
   "     CopyOUT RgnChunk[%3u] To SwapVbn[%4u] @%8x\n\n",
   "     RollOut Regions :: Rgns Swaped Out = %s.\n",
   "       Region [%4u] @Vbn[%3u] Not Loaded.\n",
   "       Chunk [%4u] @%x; SwapVbn[%4u];\n",
   "       Copy From      SwapVbn[%4u];  To @%x\n",
   "       Region[%4u]   Chunk  [%4u];  To Vbn[%4u]\n",
   "       CopyIN CurrRgnChunk[%4u] from SwapVbn[%4u] To @%x\n",
   "    DbmFileOutGrpRgns  : Status= %d; GrpHdr @%8x, GrpNum=%3u\n",
   "    Number Vchunk Regions Filed out = %u;\n",
   "       Region [%4u] @Vbn[%3u] Not Loaded.\n",
   "       Region [%4u] @Vbn[%3u] Not Loaded.\n",
   "       Chunk [%4u] @%x; SwapVbn[%4u];\n",
   "        Copy From  SwapVbn To Chunk[%4u]\n",
   "      GrpRgn[%4u]   Chunk  [%4u];  To Vbn[%4u]\n",
   " ***Error...GrpHdrNotExist for GrpNum=%3u MaxVbn=%u\n",
   " ***Error...GrpHdrNotExist for GrpNum=%3u Vbn=%u\n",
   "    LOAD GRPHDR[%3u:%2u] from FilePtr = %x\n" ,
   " DbmLoad CoreDb[%2u]::GrpHdr[%2u] has a NULL FilePtr" ,
   "    GRP HDR restored from FileVbn[%3u]\n\n",
   "    CoreDb[%2u] GrpHdr[%2u] Loaded @ %x\n\n",
   "    Grp ExEntryTbl     : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp RegionVbnDir    : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp RgnStatusDir    : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp->GrpIndexDir        : Chunk[%4u] AT Vbn[%3u]\n",
   "    Region StackPtr     : %4u\n" ,
   "    GRP RegionChunkDir  : Chunk[%4u] Transient. \n",
   "    GRP RegionSwapDir   : Chunk[%4u] Transient. \n",
   "  DbmFileInGrpRgn :: [%2u]  Chunk[%2u]  :  AT Vbn[%3u]\n",
   "    Db[%4u] FileIn Vchunks Region[%2u];Set to zero\n",
   " DbmDeleteGrpHdr:: NotImplemented; DbId=%2u, GrpNum=%3u\n"
};
#endif

/*+-----------------------------------------------------------------------+*
~DBM                          DBM-GRP
 *+-----------------------------------------------------------------------+*/
extern   idtype        DbmDbId;      /* from dbm.h                         */

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

 
#define    MemGetVbn          MemGetWord
#define    MemPutVbn          MemPutWord

#define    MemGetChunkNum     MemGetWord
#define    MemPutChunkNum     MemPutWord

#define    MemGetIndex        MemGetWord

extern boolean Dbm_CommitDb
                          (tokentype    *DbToken,   boolean       Release,
                           ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+*
                         Packet Activities
 *+-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*
                         LOAD GRP HEADER
 *+-----------------------------------------------------------------------+*/
boolean DbmLoadGrpHdr         (tokentype    *DbToken, numtype       GrpNum,
                               ft F,lt Z,zz *Status,  grpheader   **GrpHdr)
{
boolean     TmpWriteHdr  = WriteHdr;
boolean     TmpSwapBug   = SwapBug;
int         TmpDeBug     = DeBug;
vbntype     MaxVbn       = 0;
vbntype     GrpHdrVbn    = 0;
boolean     Release      = False;    
dbheader   *DbHdr        = NullPtr;
statustype  DumpStatus   = Env_Normal;

  if (!DbmBug && !SwapBug)
     WriteHdr    = True;

  SwapBug        = SwapBug;
  DeBug          = DeBug;
  DbmDbId        = DbToken->DbId;

  if (TmFetchCoreDb  (DbToken,           McStat, &DbHdr))
#ifdef __BAD_CODE__
  if (DbHdr->GrpHdrDirVbn == 0)
  if (TraceMsg  (0, "  GrpHdrDirVbn == ZERO\n"))
  if (HmDumpDbHdr    (0,       DbToken,  McStat))
     *Status     = Dbm_NullGrpHdrDirVbn;
#endif /* __BAD_CODE__ */

  if (Normal(*Status))
  if (MemGetStackPtr (DbHdr->GrpVbnDir,  McStat, &MaxVbn))
  if (GrpNum >= MaxVbn)
  if (sprintf (Msg, TraceStr(20),
/* " ***Error...GrpHdrNotExist for GrpNum=%3u MaxVbn=%u\n" */
               GrpNum, MaxVbn))
  if (TraceMsg (0, Msg))
  {
      MemDumpChunk   (0,  DbHdr->GrpVbnDir,  Dump_Status);
     *Status = Dbm_GrpHdrNotExist;
  }

  if (Normal(*Status))
  if (MemGetVbn (DbHdr->GrpVbnDir,      GrpNum, McStat,      &GrpHdrVbn))

  if (!GrpHdrVbn)
  if (sprintf (Msg, TraceStr(21),
/* " ***Error...GrpHdrNotExist for GrpNum=%3u Vbn=%u\n" */
               GrpNum, GrpHdrVbn))
  if (TraceMsg (0, Msg))
  {
      MemDumpChunk   (0,  DbHdr->GrpVbnDir,  Dump_Status);
     *Status = Dbm_GrpHdrNotExist;
  }

  if (Normal(*Status))
  if (DbHdr->BlkHdr.BlkFile)
  {
  } else {
     *Status = File_NotFound;
  } 

  /* Reserve space for GrpHeader */
  if (Normal(*Status))
  if (CoreMoreCore (sizeof(grpheader),  McStat,     (addrtype *)GrpHdr))

  if (FileInBlk    ((addrtype *)GrpHdr,     (bytessize )sizeof (grpheader),
                    &DbHdr->BlkHdr,     Release,  McStat,      &GrpHdrVbn)) 

  if (Normal(*Status))
  {
     (*GrpHdr)->NoRgnsInCore       = True;

     if (FREEZE_GRP_PACKETS       == True)
     {
        (*GrpHdr)->FreezePacket    = True;
        (*GrpHdr)->DiskCache       = False;
     } else {
        (*GrpHdr)->FreezePacket    = False;
        (*GrpHdr)->DiskCache       = True;
     }

     (*GrpHdr)->RegionChunkDir     = 0;
     (*GrpHdr)->DirtyRegionDir     = 0;
     (*GrpHdr)->RegionFreezeDir    = 0;
     (*GrpHdr)->RgnBuffNumDir      = 0;
     (*GrpHdr)->RegionSwapDir      = 0;
     (*GrpHdr)->RegionVbnDir       = 0;
     (*GrpHdr)->RgnStatusDir       = 0;
     (*GrpHdr)->ExEntryTbl         = 0;
  }

  if (Normal(*Status)) 
  if (DbmFileInGrpHdr    (*GrpHdr,  DbmDbId, &DbHdr->BlkHdr, Release, McStat))
      MemPutAddr         (DbHdr->GrpHdrDir,       GrpNum,
                          *(addrtype *)GrpHdr,    McStat);

  SwapBug        = TmpSwapBug;
  DeBug          = TmpDeBug;
  WriteHdr       = TmpWriteHdr;

TRACK(TrackBak,"DbmLoadGrpHdr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean DbmFileInGrpHdr  (grpheader    *GrpHdr,    idtype        DbId,
                          blkhdrtype   *BlkHdr,    boolean       Release,
                          ft F,lt Z,zz *Status )
{
chunkstruc   RgnChunkStruc;
numtype      AllocEntrys   = 0;

  if (FileInChunk (&GrpHdr->ExEntryTbl,            BlkHdr,        Release,  
                    McStat,                       &GrpHdr->ExEntryTblVbn))

  /* the RgnFileVbn                                                       */
  if (Normal(*Status))
  if (FileInChunk (&GrpHdr->RegionVbnDir,          BlkHdr,        Release,  
                    McStat,                       &GrpHdr->RegionVbnDirVbn)) 
  if (DeBug || DbmBug)
  {
     Mem_DumpChunkChunk (0, GrpHdr->RegionVbnDir);
  }

  /* the RgnStatusDir                                                     */
  if (Normal(*Status))
  if (GrpHdr->RgnStatusDirVbn)
  if (FileInChunk        (&GrpHdr->RgnStatusDir,   BlkHdr,        Release,  
                          McStat,                 &GrpHdr->RgnStatusDirVbn)) 
      MemGetChunkAddr    (GrpHdr->RgnStatusDir,
                          McStat,     (addrtype *)&GrpHdr->RgnStats);


  /* the GrpIndexDir                                                      */
  /* RollIn array of virtual entry indexs IF a VGRP                       */
  if (Normal(*Status))
  if (GrpHdr->GrpIndexDirVbn)
  if (FileInChunk        (&GrpHdr->GrpIndexDir,    BlkHdr,        Release,  
                          McStat,                 &GrpHdr->GrpIndexDirVbn)) 

      MemGetChunkAddr    (GrpHdr->GrpIndexDir,
                          McStat,     (addrtype *)&GrpHdr->GrpDirAddr);


  /* the RgnChunk; Swap Region by RegionIndex                             */
  if (Normal(*Status))
  if (MemGetChunkStruc   (GrpHdr->RegionVbnDir,    McStat, &RgnChunkStruc))
     AllocEntrys       = (RgnChunkStruc.UnitCount * GrpHdr->RegionEntrys)
                       + GrpHdr->BaseIndex;

  if (Normal(*Status))
  if (MemMakeChunk       (sizeof(numtype),         RgnChunkStruc.UnitCount,
                          RgnChunkStruc.ExtendQty,           DbId, 
                          McStat,                 &GrpHdr->RegionChunkDir)) 

  if (MemPutStackPtr     (GrpHdr->RegionChunkDir,  RgnChunkStruc.StackPtr,
                          McStat))


  if (MemMakeChunk       (sizeof(numtype),         RgnChunkStruc.UnitCount,
                          RgnChunkStruc.ExtendQty,           DbId, 
                          McStat,                 &GrpHdr->RegionSwapDir)) 

  if (MemPutStackPtr     (GrpHdr->RegionSwapDir,   RgnChunkStruc.StackPtr,
                          McStat))

  if (MemNewBitChunk     (AllocEntrys,             DbId,
                          McStat,                 &(GrpHdr->DirtyRegionDir)))


  if (MemMakeChunk       (sizeof(numtype),         RgnChunkStruc.UnitCount,
                          RgnChunkStruc.ExtendQty,           DbId, 
                          McStat,                 &GrpHdr->RegionFreezeDir)) 

  if (MemPutStackPtr     (GrpHdr->RegionFreezeDir, RgnChunkStruc.StackPtr,
                          McStat))

  if (MemGetChunkAddr    (GrpHdr->RegionFreezeDir,
                          McStat,     (addrtype *)&GrpHdr->FrozenRgns))


  if (MemMakeChunk       (sizeof(numtype),         RgnChunkStruc.UnitCount,
                          RgnChunkStruc.ExtendQty,           DbId, 
                          McStat,                 &GrpHdr->RgnBuffNumDir)) 

  if (MemPutStackPtr     (GrpHdr->RgnBuffNumDir,   RgnChunkStruc.StackPtr,
                          McStat))
  if (MemGetChunkAddr    (GrpHdr->RgnBuffNumDir,
                          McStat,     (addrtype *)&GrpHdr->RgnBuffNums))

  if (MemMakeChunk       (sizeof(numtype),         RgnChunkStruc.UnitCount,
                          RgnChunkStruc.ExtendQty,           DbId, 
                          McStat,                 &GrpHdr->RgnAccessedDir)) 

  if (MemPutStackPtr     (GrpHdr->RgnAccessedDir,  RgnChunkStruc.StackPtr,
                          McStat))

  if (MemGetChunkAddr    (GrpHdr->RgnAccessedDir,
                          McStat,     (addrtype *)&GrpHdr->RgnAccessArray))

  if (MemMakeChunk       (sizeof(indextype),      (GrpHdr->NumBuffRgns+1),
                          1,                       DbId,         
                          McStat,                &(GrpHdr->BuffRgnsDir)))
  if (MemPutStackPtr     (GrpHdr->BuffRgnsDir,    (GrpHdr->NumBuffRgns+1),
                          McStat))
  if (MemGetChunkAddr    (GrpHdr->BuffRgnsDir,
                          McStat,     (addrtype *)&GrpHdr->RgnBuffRgns))

  if (MemMakeChunk       (sizeof(indextype),      (GrpHdr->NumBuffRgns+1),
                          1,                       DbId,
                          McStat,                &(GrpHdr->BuffNodesDir)))
  if (MemPutStackPtr     (GrpHdr->BuffNodesDir,   (GrpHdr->NumBuffRgns+1),
                          McStat))
  if (MemGetChunkAddr    (GrpHdr->BuffNodesDir,
                          McStat,     (addrtype *)&GrpHdr->RgnBuffNodes))
  {
     GrpHdr->BaseRegion        = 0;

     GrpHdr->HeadRgnBuffs      = 0;
     GrpHdr->LastRgnBuff       = 0;
     GrpHdr->RgnBuffCount      = 0;
  }

  if (Normal(*Status))
  if (GrpHdr->BaseIndex)
     DbmFileInGrpRgnChunk  (CoreDbs[DbId], GrpHdr,  0, Release,
                            McStat,       &GrpHdr->BaseRegion); 

TRACK(TrackBak,"DbmFileInGrpHdr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean DbmFileInGrpRgnChunk (dbheader     *CoreDb,    grpheader *GrpHdr,
                              indextype     RgnIndex,  boolean    Release,
                              ft F,lt Z,zz *Status,    numtype   *RgnChunk )
{
blkhdrtype      BlkHdr;
indextype       MaxRgnVbn    = 0;
indextype       RgnVbn       = 0;
numtype         IsActive     = 0;
int             Index        = -1;
addrtype        RgnAddr      = NullPtr;
indextype       RgnEntrys    = 0;
vchunktkntype  *Vchunk       = (vchunktkntype *)NULL;

  BlkHdr = CoreDb->BlkHdr;  

  if (MemGetStackPtr (GrpHdr->RegionVbnDir,           McStat, &MaxRgnVbn))
  if (RgnIndex < MaxRgnVbn)
      MemGetVbn      (GrpHdr->RegionVbnDir, RgnIndex, McStat, &RgnVbn);
  else {
     sprintf (Msg, " DbmFileInGrpRgnChunk:: Grp[%3u]  Index=%3u > Max=%3u\n",
              GrpHdr->GrpNum, RgnIndex, MaxRgnVbn);
        TraceMsg  (0, Msg);
     if (MaxRgnVbn == 0)
       return (STAT);
/*
     else
       DbToken.DbId   = CoreDb->DbId;
       DbToken.Handle = 1;
       Grp_DumpStruct (&DbToken,   GrpHdr->GrpNum,   McStat);
      *Status = Err_BadDesign;
*/
  }

  if (Normal(*Status))
  if (FileInChunk (RgnChunk, &BlkHdr, Release, McStat, &RgnVbn))
  {
     GrpHdr->NoRgnsInCore     = False;
     GrpHdr->RegionIndex      = RgnIndex;
  }

  if (Normal(*Status))
  if (MemPutChunkNum     (GrpHdr->RegionChunkDir, RgnIndex,
                          *RgnChunk,               McStat))
  if (MemPutBit          (GrpHdr->DirtyRegionDir, RgnIndex, 0, McStat))
      MemPutVbn          (GrpHdr->RegionSwapDir,  RgnIndex,
                          0,                      McStat);

  if (Normal(*Status))
  if (CoreDb->VchunkGrpNum == GrpHdr->GrpNum)
  if (GrpHdr->EnvObjType    > That_AttrNum) 
  if (MemGetChunkNum     (GrpHdr->RgnAccessedDir, RgnIndex, 
                          McStat,                &IsActive))
  if (IsActive    == 0)
  {
  /* Clean the InCore chunk num to zero                                  */

      if (MemGetChunkAddr    (*RgnChunk,     McStat,     &RgnAddr))
      if (MemGetStackPtr     (*RgnChunk,     McStat,     &RgnEntrys))
      {
         Vchunk  = (vchunktkntype *)RgnAddr;
        
         for (Index = RgnIndex ? -1  : 0;  ++Index < RgnEntrys;) 
            Vchunk[Index].InCore = 0;
      }
      IsActive     = 1;
      if (Normal(*Status))
         MemPutChunkNum      (GrpHdr->RgnAccessedDir,     RgnIndex,
                              IsActive,                   McStat);
  }

 
TRACK(TrackBak,"DbmFileInGrpRgnChunk\n");
return(STAT);
}

boolean DbmDeleteGrpHdr
                              (tokentype    *DbToken, numtype       GrpNum,
                               ft F,lt Z,zz *Status)
{
  sprintf (Msg, TraceStr(35),
/* " DbmDeleteGrpHdr:: NotImplemented; DbId=%2u, GrpNum=%3u\n" */
           DbToken->DbId, GrpNum);
  TraceMsg (0, Msg);

TRACK(TrackBak,"DbmDeleteGrpHdr\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+
~M                         END OF GDBM.C                                  !
  +-----------------------------------------------------------------------+*/
