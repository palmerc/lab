/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - DBM1.c */
#define DBM_C
#define DBM1
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "dbm.h"
#include "vdbm.h"
#include "gdbm.h"
#include "odbm.h"
#include "files.h"
#include "env0.h"

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

 
#define    MemGetVbn          MemGetWord
#define    MemPutVbn          MemPutWord

#define    MemGetChunkNum     MemGetWord
#define    MemPutChunkNum     MemPutWord
#define    MemPutSize         MemPutWord
#define    MemGetIndex        MemGetWord

#ifdef ENV01
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
   #define        DbFileName_AttrNum     (numtype    )158

   #define        WenvDbs_Hndl           (handletype )692
#endif

extern boolean KernelGetAttr  (tokentype    *Token,    numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype   Value);
extern boolean KernelFirstOf  (handletype    SetHndl,  tokentype *Owner,
                               ft F,lt Z,zz *Status,   tokentype *FirstNode,
                               tokentype    *FirstMember);
extern boolean KernelNextOf   (handletype    SetHndl,  tokentype *CurrentNode,
                               ft F,lt Z,zz *Status,   tokentype *NextNode,
                               tokentype    *NextMember);
#endif


/*+-----------------------------------------------------------------------+*
~DBM                             DBM
 *+-----------------------------------------------------------------------+*/
 
/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/
boolean DbmLoadDb       (tokentype  *Anchor,   ft F,lt Z,zz *Status,
                         char       *DbName,   dbheader    **CoreDb)
{
char       LoadFileName[MAXPATHNAME];
tokentype  Token        = NullToken;
idtype     DbId         = 0;

#ifdef ENV01
tokentype  DbToken;
tokentype  NodeTkn;
tokentype  NextNodeTkn;
#endif


 DbmDbId        = Anchor->DbId;
 if (DbmDbId <= WenvId)
 {
    Token.Handle  = 1;
    Token.DbId    = DbmDbId;
    DbId          = DbmDbId;
#ifndef ENV01
    if (DbId     == RenvId)
       strcpy (LoadFileName, RenvFileName);
    else if (DbId     == WenvId)
       strcpy (LoadFileName, WenvFileName);
    else
    {
      if (strlen (DbName) == 0)
         strcpy (LoadFileName, RenvFileName);
      else
         strcpy (DbFileName, DbName);
    }
#else
    if (DbId     == RenvId)
       strcpy (LoadFileName, RenvFileName);

    else
       strcpy (LoadFileName, WenvFileName);

 } else {
    if (KernelFirstOf    (WenvDbs_Hndl,  &WenvTkn,
                          McStat,        &NodeTkn,            &DbToken))
    while (Normal(*Status)
    &&     NodeTkn.Handle)
    {
       if (KernelGetAttr (&DbToken,      DbId_AttrNum, 
                          McStat,        (addrtype )&DbId))
       if (DbId != Anchor->DbId)
       {
          if (KernelNextOf  (WenvDbs_Hndl,  &NodeTkn,
                             McStat,        &NextNodeTkn,        &DbToken))
          {
              NodeTkn.DbId   = NextNodeTkn.DbId;
              NodeTkn.Handle = NextNodeTkn.Handle;
          }
       } else {
           NodeTkn.Handle = 0;
           Token.DbId     = DbId;
           Token.Handle   = 1;
       }
    }
#endif

 }

#ifdef ENV01
    if (Normal(*Status)
    &&  DbId == Anchor->DbId)
    if (KernelGetAttr  (&DbToken,      DbFileName_AttrNum, 
                        McStat,        (addrtype )LoadFileName))
    {
       sprintf (Msg, "\n DbLoadDb LoadFileFile := <%-32s>\n",
                LoadFileName);
       SendMsg (0, Msg);
       sprintf (Msg, "            DbName       := <%-32s>\n",
                DbName);
       SendMsg (0, Msg);
#endif
       DbmLoadDbHdr   (&Token,        LoadFileName,
                       McStat,        CoreDb);
#ifdef ENV01
    } else {
       sprintf (Msg, " Can Not open DbFile ::  %s\n", LoadFileName);
       SendMsg (0, Msg);
      *Status = Err_NotImplemented;
    }
#endif

    if (*Status == Set_NotFound
    ||  !Normal(*Status))
    if (sprintf  (Msg,  " Not a registered DbId [=%3u]\n", Anchor->DbId))
        TraceMsg (0,    Msg);

TRACK(TrackBak,"DbmLoadDb\n");
return(STAT);
}
 


boolean DbmLoadDbHdr    (tokentype    *DbToken,   char      *LoadFileName,
                         ft F,lt Z,zz *Status,    dbheader **CoreDb)
{
boolean     TmpWriteHdr  = WriteHdr;
boolean     TmpSwapBug   = SwapBug;
int         TmpDeBug     = DeBug;
boolean     TmpDbmBug    = DbmBug;
vbntype     DbHdrVbn     = 0;
boolean     Release      = False;    
dbheader   *DbHdr        = *CoreDb;
indextype   HndlUnits    = 0;
numtype     AllocGroups  = 0;

  sprintf (Msg, "\n OPEN File %s \n", LoadFileName);
  TraceMsg (0, Msg);
  if (!DbmBug  && !SwapBug)
     WriteHdr    = True;

/*DbmBug         = True; */

  /* Reserve space for the NEW DbHeader */
  if (*CoreDb)
  {
     WriteHdr               = TmpWriteHdr;

     sprintf (Msg, "\n CoreDb ALREADY LOADED @%x\n\n", *CoreDb);
     TraceMsg (0, Msg);

     return (STAT);
  }

  if (LoadFileName == NullPtr)
     *Status = File_NoName;

  else  if (!strlen (LoadFileName))
     *Status = File_NoName;

  else
     DiskFilePtr = fopen (LoadFileName, "r+b");

  if (DiskFilePtr == NULL)
  {
     SendMsg (0, " DbmLoadDbHdr:: Null File Name\n");
    *Status = File_WontOpen;
  } 
#ifdef NO_DISKCACHE
    else
      setvbuf (DiskFilePtr, NULL, _IONBF, 1024); 
/*    setvbuf (DiskFilePtr, NULL, _IOFBF, 1024);  */
#endif

#ifdef __DISPLAY_MESSAGE__
  if (Normal(*Status) || DbmBug)
  if (sprintf (Msg, " LOAD DBHDR FilePtr = %x; CoreDb @%8x\n",
               DiskFilePtr, *CoreDb))
     TraceMsg (0, Msg);
#endif /* __DISPLAY_MESSAGE__ */

/*  LastBlkOffset[DbHdr->DbId]  = 0;
    fseek (DbHdr->BlkHdr.BlkFile, LastBlkOffset[DbHdr->DbId], SEEK_SET);
*/
  if (Normal(*Status))
  {
     CoreMoreCore (sizeof (dbheader),  McStat,   (addrtype *)&DbHdr);
#ifdef __DISPLAY_MESSAGE__
     sprintf (Msg, " CoreDb[%4u] Located @%x\n", DbToken->DbId, DbHdr);
     TraceMsg (0, Msg);
#endif /* __DISPLAY_MESSAGE__ */
  }

  if (Normal(*Status))
  {
     *Status = Env_Normal;
      if (sprintf (Msg, "    *Status            =%4d\n", *Status))
         TraceMsg (0, Msg);
      DbHdr->BlkHdr.BlkFile     = DiskFilePtr;
      if (FileGetSomeBytes  (&DbHdr->BlkHdr,              (farlongtype )0,
                             (unsigned )sizeof (dbheader),
                             McStat,                      (addrtype )DbHdr))
      {
/*
         BlkDir_Vbn            = DbHdr->BlkDirVbn;
         BlkDir_Units          = DbHdr->BlkDirUnits;
         BlkDir_Offset         = DbHdr->BlkDirOffset;

         BlkTkn_Vbn            = DbHdr->BlkTknVbn;
         BlkTkn_Units          = DbHdr->BlkTknUnits;
         BlkTkn_Offset         = DbHdr->BlkTknOffset;

         BlkSize_Vbn           = DbHdr->BlkSizeVbn;
         BlkSize_Offset        = DbHdr->BlkSizeOffset;
*/
         sprintf (Msg, "   DB HDR restored from FileVbn[%3u]\n", DbHdrVbn);
         TraceMsg (0, Msg);
      }
  } 

  if (Normal(*Status))
  {
     DbHdr->GrpHdrDir          = 0;
     DbHdr->GrpVbnDir          = 0;
     DbHdr->ObjectIndexDir     = 0;
     DbHdr->ObjectAddrDir      = 0;
     DbHdr->ObjectMemLocDir    = 0;
     DbHdr->ObjectMemRefDir    = 0;
     DbHdr->LocalObjNumDir     = 0;

     DbHdr->LocalObjNumMap     = 0;
     DbHdr->ObjHandleDir       = 0;
     DbHdr->ObjNumMap          = 0;
     DbHdr->ObjTimeStampDir    = 0;

     DbHdr->ObjHdrDir          = 0;
     DbHdr->ObjVbnDir          = 0;

     DbHdr->VchunkCommitDir    = 0;
     DbHdr->VchunkSwapDir      = 0;

     DbHdr->VbnDeleteDir       = 0;

     DbHdr->VchunkGrpHdr       = NullPtr;
     DbHdr->NewHandles         = 0;
     DbHdr->FreeRegionDir      = 0;
     DbHdr->FreeRegionLengths  = 0;

     DbHdr->BlkHdr.BlkFile     = DiskFilePtr;
     DbHdr->BlkHdr.BlkDirChunk = 0;
     DbHdr->BlkHdr.BlkTknChunk = 0;
     DbHdr->BlkHdr.BlkSizeChunk= 0;

     DbHdr->LocalAttrNumMap    = 0;
     DbHdr->AttrHandleDir      = 0;
     DbHdr->AttrNumMap         = 0;
     DbHdr->LocalAttrSizeTbl   = 0;
     DbHdr->TextTokenDir       = 0;
     DbHdr->FreeTxtTknStack    = 0;

     if (DbHdr->DbId == RenvId && SwapHandles)
     {
        CoreHandles               = DbHdr->HandleCount;
        if (sprintf (Msg, "  1st %u Handles Of Renv are NonPutable\n",
                       CoreHandles))
          TraceMsg (0, Msg);
     }
  }


#ifdef   __FREEZE_VCHUNKS__
     if (Normal(*Status)) 
     if (DbHdr->DbId  > WenvId)
     if (MemMakeChunk      (sizeof(vbntype), 5000,      2000,
                            DbHdr->DbId,    McStat, &(DbHdr->VbnDeleteDir)))
        MemPutStackPtr     (DbHdr->VbnDeleteDir,   0,            McStat);

#endif /* __FREEZE_VCHUNKS__  */

  if (Normal(*Status)) 
  if (DbHdr->ObjectAddrDir == 0)
  if (DbHdr->HandleUnits)
  {
     if (MemMakeChunk   (sizeof(addr),  
                         DbHdr->HandleUnits,          Extend_Handles,
                         DbHdr->DbId,    McStat,    &(DbHdr->ObjectAddrDir)))
         MemPutStackPtr (DbHdr->ObjectAddrDir, DbHdr->HandleCount, McStat);
     HndlUnits = DbHdr->HandleUnits;
  }

  if (Normal(*Status)) 
  if (DbHdr->HandleUnits)
      MemNewBitChunk    (HndlUnits,                  DbHdr->DbId,
                         McStat,                    &(DbHdr->ObjectMemLocDir));

  if (Normal(*Status)) 
  if (DbHdr->HandleUnits)
  if (DbHdr->DbId      > WenvId)
  if (MemMakeChunk      (sizeof(shorttype),  
                         DbHdr->HandleUnits,         Extend_Handles,
                         DbHdr->DbId,   McStat,    &(DbHdr->ObjectMemRefDir)))
      MemPutStackPtr    (DbHdr->ObjectMemRefDir, DbHdr->HandleCount, McStat);

  if (Normal(*Status)) 
  if (DbmFileInBlkHdr    (DbHdr,    McStat))

  if (DbmFileInDbHdr     (DbHdr,    Release,     McStat))
  if (DbHdr->GrpCount == 0 )  
     AllocGroups = Alloc_Grps ;
  else
     AllocGroups = DbHdr->GrpCount + Extend_Grps ;
  if (Normal(*Status)) 
  if (MemMakeChunk      (sizeof(addrtype),  
                         AllocGroups ,         Extend_Grps,
                         DbHdr->DbId,   McStat,    &(DbHdr->GrpHdrDir)))
  if (MemPutStackPtr    (DbHdr->GrpHdrDir, DbHdr->GrpCount+1, McStat))
  if (MemGetChunkAddr(DbHdr->GrpHdrDir,McStat,(addrtype *) &DbHdr->GrpHdrs))
  if (MemMakeChunk      (sizeof(addrtype),  
                         DbHdr->LastEnvObjCount+1 ,         Extend_DbObjs,
                         DbHdr->DbId,   McStat,    &(DbHdr->ObjHdrDir)))
  if (MemPutStackPtr    (DbHdr->ObjHdrDir, DbHdr->LocalObjCount+2, McStat))

  if (DeBug || DbmBug)
  if (sprintf (Msg, "    DB ObjHdrDir         : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->ObjHdrDir,  DbHdr->ObjHdrDirVbn))
     TraceMsg (0, Msg);
 
  if (Normal(*Status))
  {
     *CoreDb = DbHdr;
      sprintf (Msg, "\n  DB[%2u] LOADED;  Handles= %u\n",
               DbHdr->DbId, DbHdr->HandleCount);
      TraceMsg (0, Msg);
/*
      sprintf (Msg, "  DB[%2u] Header LOADED @ %x Handles= %u\n",
               DbHdr->DbId, DbHdr, DbHdr->HandleCount);
      TraceMsg (0, Msg);
*/
  }

  if (Normal(*Status))
  if (DbHdr->DbId  == WenvId)
  {
        CoreDbCount  = 100;
/*      CoreDbs[WenvId] = DbHdr;
        KernelGetAttr   (&WenvTkn,       DbCount_AttrNum,
                         McStat,        (addrtype )&CoreDbCount);
*/
  }
  SwapBug                = TmpSwapBug;
  DbmBug                 = TmpDbmBug;
  DeBug                  = TmpDeBug;
  WriteHdr               = TmpWriteHdr;


TRACK(TrackBak,"DbmLoadDbHdr\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean DbmFileInBlkHdr  (dbheader     *DbHdr,    ft F,lt Z,zz *Status)
{
chunkstruc    CS;
farlongtype   Offset      = sizeof (chunkstruc);
addrtype      ChunkAddr   = NullPtr;

/* Create The DbFile Blk Chunks                                            */

  if (DeBug >= 0 || DbmBug)
  if (sprintf (Msg, "    DB BlkDirOffset      : @%8x\n", DbHdr->BlkDirOffset))
     TraceMsg (0, Msg);

  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkDirOffset,
                        (unsigned )sizeof (chunkstruc),
                        McStat,                 (addrtype )&CS))
  if (MemMakeChunk     (CS.UnitSize,            CS.UnitCount, 
                        CS.ExtendQty,           DbHdr->DbId,
                        McStat,                &DbHdr->BlkHdr.BlkDirChunk))

  if (MemGetChunkAddr  (DbHdr->BlkHdr.BlkDirChunk,   McStat, &ChunkAddr))

  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkDirOffset + Offset,
                        (unsigned )(CS.UnitSize * CS.UnitCount),
                        McStat,                  ChunkAddr))
      MemPutStackPtr   (DbHdr->BlkHdr.BlkDirChunk, CS.StackPtr,  McStat);

  if (DeBug >= 0 || DbmBug)
  if (sprintf (Msg, "    DB BlkDirChunk       : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->BlkHdr.BlkDirChunk,   DbHdr->BlkDirVbn))
  if (TraceMsg (0, Msg))
      MemDumpChunkStruc(&CS,                     McStat);


  if (Normal(*Status))
  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkTknOffset,
                        (unsigned )sizeof (chunkstruc),
                        McStat,                 (addrtype )&CS))
  if (MemMakeChunk     (CS.UnitSize,            CS.UnitCount, 
                        CS.ExtendQty,           0,
                        McStat,                &DbHdr->BlkHdr.BlkTknChunk))
  if (MemGetChunkAddr  (DbHdr->BlkHdr.BlkTknChunk,   McStat, &ChunkAddr))

  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkTknOffset + Offset,
                        (unsigned )(CS.UnitSize * CS.UnitCount),
                        McStat,                  ChunkAddr))
  if (MemPutStackPtr   (DbHdr->BlkHdr.BlkTknChunk, CS.StackPtr,  McStat))

  if (DeBug >= 0 || DbmBug)
  if (sprintf (Msg, "    DB BlkTknChunk       : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->BlkHdr.BlkTknChunk,   DbHdr->BlkTknVbn))
  if (TraceMsg (0, Msg))
      MemDumpChunkStruc(&CS,                     McStat);

  if (Normal(*Status))
  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkSizeOffset,
                        (unsigned )sizeof (chunkstruc),
                        McStat,                 (addrtype )&CS))
  if (MemMakeChunk     (CS.UnitSize,            CS.UnitCount, 
                        CS.ExtendQty,           0,
                        McStat,                &DbHdr->BlkHdr.BlkSizeChunk))
  if (MemGetChunkAddr  (DbHdr->BlkHdr.BlkSizeChunk,  McStat, &ChunkAddr))

  if (FileGetSomeBytes (&DbHdr->BlkHdr,          DbHdr->BlkSizeOffset+ Offset,
                        (unsigned )(CS.UnitSize * CS.UnitCount),
                        McStat,                  ChunkAddr))
  if (MemPutStackPtr   (DbHdr->BlkHdr.BlkSizeChunk, CS.StackPtr,  McStat))

  if (DeBug >= 0 || DbmBug)
  if (sprintf (Msg, "    DB BlkSizeChunk      : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->BlkHdr.BlkSizeChunk,   DbHdr->BlkSizeVbn))
  if (TraceMsg (0, Msg))
      MemDumpChunkStruc(&CS,                     McStat);

TRACK(TrackBak,"DbmFileInBlkHdr\n");
return(STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
boolean DbmFileInDbHdr  (dbheader   *DbHdr,    boolean   Release,
                         ft F,lt Z,zz *Status)
{
indextype       StackPtr = 0;

  if (SwapHandles && DbHdr->DbId == RenvId)
  {
    *Status = Env_Normal;

  } else {

    if (DbHdr->ObjectIndexDirVbn)
    if (FileInChunk (&DbHdr->ObjectIndexDir, 
                     &(DbHdr->BlkHdr),        Release,  
                     McStat,                      &DbHdr->ObjectIndexDirVbn)) 
    if (MemGetStackPtr (DbHdr->ObjectIndexDir, McStat,     &StackPtr))
    if (sprintf (Msg, " DB Handle Chunk's StackPtr = %u\n", StackPtr))
    if (TraceMsg (0, Msg))

    if (DeBug || DbmBug)
    if (sprintf (Msg, "    DB ObjectIndexDir    : Chunk[%4u] AT Vbn[%3u]\n",
                DbHdr->ObjectIndexDir,  DbHdr->ObjectIndexDirVbn))
    if (TraceMsg (0, Msg))
        DumpChunkChunk (0, DbHdr->ObjectIndexDir);


    if (Normal(*Status))
    if (DbHdr->LocalObjNumDirVbn)
    if (FileInChunk (&DbHdr->LocalObjNumDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                      &DbHdr->LocalObjNumDirVbn)) 
    if (DeBug || DbmBug)
    if (sprintf (Msg, "    DB LocalObjNumDir    : Chunk[%4u] AT Vbn[%3u]\n",
                DbHdr->LocalObjNumDir,  DbHdr->LocalObjNumDirVbn))
    if (TraceMsg (0, Msg))
        DumpChunkChunk (0, DbHdr->LocalObjNumDir);
  }

  if (Normal(*Status))
  if (DbHdr->LocalObjNumMapVbn)
  if (FileInChunk (&DbHdr->LocalObjNumMap,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                      &DbHdr->LocalObjNumMapVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB LocalObjNumMap    : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->LocalObjNumMap,  DbHdr->LocalObjNumMapVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->ObjHandleDirVbn)
  if (FileInChunk (&DbHdr->ObjHandleDir,  
                   &(DbHdr->BlkHdr),               Release,  
                    McStat,                        &DbHdr->ObjHandleDirVbn)) 
  if (DeBug || DbmBug) 
  {
     sprintf (Msg, "    DB ObjHandleDir      : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->ObjHandleDir,  DbHdr->ObjHandleDirVbn);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (DbHdr->ObjNumMapDirVbn)
  if (FileInChunk (&DbHdr->ObjNumMap,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                        &DbHdr->ObjNumMapDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB ObjNumMap         : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->ObjNumMap,  DbHdr->ObjNumMapDirVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->ObjTimeStampVbn)
  if (FileInChunk (&DbHdr->ObjTimeStampDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                        &DbHdr->ObjTimeStampVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB ObjTimeStampDir   : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->ObjTimeStampDir,  DbHdr->ObjTimeStampVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->LocalAttrMapVbn)
  if (FileInChunk (&DbHdr->LocalAttrNumMap,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                        &DbHdr->LocalAttrMapVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB LocalAttrNumMap   : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->LocalAttrNumMap,  DbHdr->LocalAttrMapVbn);
     TraceMsg (0, Msg);
     DumpChunkChunk (0, DbHdr->LocalAttrNumMap);
  }

  if (Normal(*Status))
  if (DbHdr->AttrHandleDirVbn)
  if (FileInChunk (&DbHdr->AttrHandleDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                       &DbHdr->AttrHandleDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB AttrHandleDir     : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->AttrHandleDir,  DbHdr->AttrHandleDirVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->AttrNumMapVbn)
  if (FileInChunk (&DbHdr->AttrNumMap,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                          &DbHdr->AttrNumMapVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB AttrNumMap        : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->AttrNumMap,  DbHdr->AttrNumMapVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->LocalAttrSizeVbn)
  if (FileInChunk (&DbHdr->LocalAttrSizeTbl,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                       &DbHdr->LocalAttrSizeVbn)) 
  {
      if (DeBug || DbmBug)
      {
         sprintf (Msg, "    DB LocalAttrSizeTbl  : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->LocalAttrSizeTbl,  DbHdr->LocalAttrSizeVbn);
         TraceMsg (0, Msg);
      }
      if ( DbHdr->DbId == RenvId )
	 MemPutSize( DbHdr->LocalAttrSizeTbl, That_AttrNum,
		     sizeof( addrtype ) , McStat ) ;
  }


  if (Normal(*Status))
  if (DbHdr->TextTknDirVbn)
  if (FileInChunk (&DbHdr->TextTokenDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                       &DbHdr->TextTknDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB TextTokenDir      : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->TextTokenDir, DbHdr->TextTknDirVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (DbHdr->FreeTxtTknVbn)
  if (FileInChunk (&DbHdr->FreeTxtTknStack,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                       &DbHdr->FreeTxtTknVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB FreeTxtTknStack   : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->FreeTxtTknStack, DbHdr->FreeTxtTknVbn);
     TraceMsg (0, Msg);
  }


  if (Normal(*Status))
  if (DbHdr->GrpVbnDirVbn)
  if (FileInChunk (&DbHdr->GrpVbnDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                           &DbHdr->GrpVbnDirVbn)) 
     if (DeBug || DbmBug)
     {
        sprintf (Msg, "    DB GrpVbnDir         : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->GrpVbnDir,  DbHdr->GrpVbnDirVbn);
        TraceMsg (0, Msg);
     }


  if (Normal(*Status))
  if (DbHdr->ObjVbnDirVbn)
  if (FileInChunk (&DbHdr->ObjVbnDir,  
                    &(DbHdr->BlkHdr),        Release,  
                    McStat,                           &DbHdr->ObjVbnDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, "    DB ObjVbnDir         : Chunk[%4u] AT Vbn[%3u]\n",
              DbHdr->ObjVbnDir,  DbHdr->ObjVbnDirVbn);
     TraceMsg (0, Msg);
  }


TRACK(TrackBak,"DbmFileInDbHdr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF DBM1.C                                  !
  +-----------------------------------------------------------------------+*/
