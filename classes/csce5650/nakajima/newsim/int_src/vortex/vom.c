/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - VOM.c                                                              */
#define  VOM
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "vdbm.h"
#include "env0.h" 
#include "vom.h"

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)
 
#define PrimalId  RenvId
/*+-----------------------------------------------------------------------+*
~VOM                         EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
#define MemFreeFileBlk     Mem_FreeFileBlk 
#define FileInChunk        File_InChunk    
extern boolean MemFreeFileBlk (blkhdrtype   *BlkHdr,
                               ft F,lt Z,zz *Status,   vbntype      *Vbn);

extern boolean FileInChunk    (chunknum     *Chunk,    blkhdrtype   *BlkHdr,
                               boolean       Release,
                               ft F,lt Z,zz *Status,   vbntype      *VbnBlk);

#define MemPopChunkIndex     Mem_PopWord
#define MemPushChunkIndex    Mem_PushWord


/*+-----------------------------------------------------------------------+*
~OM                     VCHUNK OBJECT MANAGER                             !
 *+-----------------------------------------------------------------------+*/
boolean OmNewVchunk       (tokentype    *Anchor,    sizetype      UnitSize,
                           indextype     UnitCount, indextype     ExtendQty,
                           ft F,lt Z,zz *Status, 
                           indextype    *Vchunk,    addrtype     *ChunkAddr)
{
  SendMsg (0, " Warning ... OmNewVchunk Accessed via OA \n");
 *Status = Oa_VchunkNoAccess;

TRACK(TrackBak,"OmNewVchunk\n");
return (STAT);
}
 


boolean OmGetVchunkToken  (tokentype    *Token,     ft F,lt Z,zz *Status, 
                           vchunktkntype           *VchunkToken)
{
dbheader     *CoreDb       = NullPtr;
booleantype   Release      = False;
boolean       TmpFileBug   = FileBug;

  if (!Token->Handle)
     *Status = Oa_VchunkNotExist;

  if (Normal(*Status))
  if (TmGetCoreDb      (Token,  McStat,         &CoreDb))
     DbmGetVchunkTkn   (Token,  McStat,          VchunkToken);

  if (Normal(*Status))
  if (!VchunkToken->InCore)
  {
     if (VchunkToken->OnDisk)
     {
        if (FileInChunk      (&VchunkToken->InCore,   &CoreDb->BlkHdr,
                              Release,
                              McStat,                 &VchunkToken->OnDisk))
        if (Token->DbId >= PrimalId)
            DbmPutVchunkTkn  (Token,      VchunkToken, McStat);
        else {
            SendMsg (0, "\n CoreDb->VchunkDir should NOT be Accessed!\n");
           *Status  = Err_BadDesign;
        }
     } else
        *Status = Oa_VchunkNotExist;
  }

  FileBug = TmpFileBug;

TRACK(TrackBak,"OmGetVchunkToken\n");
return (STAT);
}
 


boolean OmGetVchunkStruc  (tokentype    *Token,     ft F,lt Z,zz *Status, 
                           chunkstruc   *VchunkStruc)
{
vchunktkntype VchunkToken;
  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemGetChunkStruc (VchunkToken.InCore, McStat, VchunkStruc);

TRACK(TrackBak,"OmGetVchunkStruc\n");
return (STAT);
}
 


boolean OmDeleteVchunk    (tokentype    *Token,     ft F,lt Z,zz *Status)
{

  SendMsg (0, " Warning ... Vchunk Accessed via OA \n");
 *Status = Oa_VchunkNoAccess;

TRACK(TrackBak,"OmDeleteVchunk\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF VOM.C                                   !
  +-----------------------------------------------------------------------+*/

