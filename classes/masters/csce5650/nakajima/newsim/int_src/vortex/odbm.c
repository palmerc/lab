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
#define ODBM
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"
#include "omidefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "gdbm.h"
#include "odbm.h"
#include "files.h"
#include "env0.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...ODBM... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[54] =
{
   "\n    OBJHDR[%3u] Obj[%3u](* %3u) obj's %4u rgn's %3u; In Core= %s\n",
   "                                  ObjHdr committed  ...@Vbn[%4u]\n",
   "    DB ObjHdrDir         : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj VbnDir           : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldNumMap      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj LocalAttrNumMap  : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj ClassOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldRefDir      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldMapDir      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldArrayDir    : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj ExObjectTbl      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Grp RgnStatusDirVbn : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj RegionVbnDir     : Chunk[%4u] AT Vbn[%3u]\n",
   "     RollOut Regions :: Current Rgn[%4u]... Chunk[%4u]; CoreAddr @%x\n",
   "       CopyOUT RgnChunk        To SwapVbn[%4u]\n",
   "       Region [%4u] @Vbn[%3u] Not Loaded.\n",
   "       Chunk [%4u] @%x; SwapVbn[%4u];\n",
   "       Copy From      SwapVbn[%4u];  To @%x\n",
   "       Region[%4u]   Chunk  [%4u];  To Vbn[%4u]\n",
   "       CopyIN CurrRgnChunk[%4u] from SwapVbn[%4u] To @%x\n",
   " ***Error...ObjHdrNotExist for LocalObj=%3u MaxVbn=%u\n",
   " ***Error...ObjHdrNotExist for LocalObj=%3u Vbn=%u\n",
   "\n LOAD OBJHDR[%2u] from FilePtr = %x\n" ,
   " DbmLoad CoreDb[%2u]::ObjHdr[%2u] has a NULL FilePtr" ,
   "    OBJ HDR restored from FileVbn[%3u]\n",
   "  CoreDb[%2u] ObjHdr[%2u] Loaded @ %x\n",
   "    Obj ExObjectTbl     : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj RegionVbnDir    : Chunk[%4u] AT Vbn[%3u]\n",
   "    Region StackPtr     : %4u\n",
   "    OBJ RegionChunkDir  : Chunk[%4u] Transient. \n",
   "    OBJ RegionSwapDir   : Chunk[%4u] Transient. \n",
   "    Obj FieldNumMap      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj LocalAttrNumMap  : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj ClassOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldRefDir      : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldMapDir     : Chunk[%4u] AT Vbn[%3u]\n",
   "    Obj FieldArrayDir   : Chunk[%4u] AT Vbn[%3u]\n",
   "    OBJ Region[%2u]  Chunk[%2u]  :  AT Vbn[%3u]\n",
   "   DbmPairRgnObjects<FALSE>::    ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   " BAD PAIR Handles for RegionChunk[%3u] @%x count %4u\n",
   "   OmPairRgnObjects<-->Handles; ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   "    pair Region[](%3u) Object @%x;  Handle= %u @%x\n " ,
   " Outer Region Address Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   " Region[%3u] Object[%3u] Handle= %4u\n",
   " Null Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   "\n   UnPairRgnObjects<FALSE>::    ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   " BAD UNPAIR Handles for RegionChunk[%3u] @%x count %4u\n",
   "\n   UnPair Objects<-->Handles; ObjSize=%3u RgnChunk[%2u] @%x * %4u\n",
   "    unpair Region[](%3u) @Offset= %4u;  Handle= %u @%x\n " ,
   " Outer Region Address Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   " Null Handle[%4u] In Region*%3u :: RegionIndex = %u\n",
   "    Obj ObjectLockDir    : Chunk[%4u] AT Vbn[%3u]\n"
};
#endif

extern boolean OaDumpObjHdr (int TraceSwi,  tokentype     *Anchor,
                             num ObjNum,    ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+*
~DBM                            ODBM
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

/*+-----------------------------------------------------------------------+*
~DBM                           0->1  
 *+-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+*
 ~L                          DOWN LOAD
 *+-----------------------------------------------------------------------+*/
boolean DbmLoadObjHdr    (tokentype    *DbToken,  numtype     LocalObj,
                          ft F,lt Z,zz *Status,   objheader **ObjHdr)
{
boolean     TmpWriteHdr  = WriteHdr;
boolean     TmpSwapBug   = SwapBug;
int         TmpDeBug     = DeBug;
vbntype     MaxVbn       = 0;
vbntype     ObjHdrVbn    = 0;
boolean     Release      = False;    
dbheader   *DbHdr        = NullPtr;
statustype  DumpStatus   = Env_Normal;

  if (!DbmBug)
     WriteHdr    = True;

  SwapBug        = SwapBug;
  DeBug          = DeBug;

  DbmDbId        = DbToken->DbId;

  if (TmFetchCoreDb  (DbToken,           McStat, &DbHdr))
  if (MemGetStackPtr (DbHdr->ObjVbnDir,  McStat, &MaxVbn))
  if (LocalObj >= MaxVbn)
  if (sprintf (Msg, TraceStr(21),
/* " ***Error...ObjHdrNotExist for LocalObj=%3u MaxVbn=%u\n" */
               LocalObj, MaxVbn))
  if (TraceMsg (0, Msg))
  {
      MemDumpChunk   (0,  DbHdr->ObjVbnDir,  Dump_Status);
     *Status = Dbm_ObjHdrNotExist;
  }

  if (Normal(*Status))
  if (MemGetVbn (DbHdr->ObjVbnDir,      LocalObj, McStat,      &ObjHdrVbn))

  if (!ObjHdrVbn)
  if (sprintf (Msg, TraceStr(22),
/* " ***Error...ObjHdrNotExist for LocalObj=%3u Vbn=%u\n" */
               LocalObj, ObjHdrVbn))
  if (TraceMsg (0, Msg))
  {
      if (DeBug)
         MemDumpChunk   (0,  DbHdr->ObjVbnDir,  Dump_Status);
     *Status = Dbm_ObjHdrNotExist;
  }

  if (Normal(*Status))
  if (DbHdr->BlkHdr.BlkFile)
  {
#ifdef __DISPLAY_MESSAGE__
     if (DeBug || DbmBug)
     if (sprintf (Msg, TraceStr(23),
/* "\n LOAD OBJHDR[%2u] from FilePtr = %x\n"  */
                  LocalObj, DbHdr->BlkHdr.BlkFile))
         TraceMsg (0, Msg);
#endif /* __DISPLAY_MESSAGE__ */
  } else {
     *Status = File_NotFound;
#ifdef __DISPLAY_MESSAGE__
      sprintf (Msg, TraceStr(24),
/* " DbmLoad CoreDb[%2u]::ObjHdr[%2u] has a NULL FilePtr"  */
               DbHdr->DbId, LocalObj);
      SendMsg (0, Msg);
#endif /* __DISPLAY_MESSAGE__ */
  } 

  /* Reserve space for ObjHeader */
  if (Normal(*Status))
  if (CoreMoreCore (sizeof(objheader),  McStat,     (addrtype *)ObjHdr))

  if (FileInBlk    ((addrtype *)ObjHdr,        (bytessize )sizeof (objheader),
                    &DbHdr->BlkHdr,     Release,  McStat,      &ObjHdrVbn)) 

  if (DeBug || DbmBug)
  if (sprintf (Msg, TraceStr(25),
/* "    OBJ HDR restored from FileVbn[%3u]\n" */
               ObjHdrVbn))
     TraceMsg (0, Msg);
 
  if (Normal(*Status))
  {
     (*ObjHdr)->ObjectLockDir      = 0;
     (*ObjHdr)->FieldNumMap        = 0;
     (*ObjHdr)->LocalAttrNumMap    = 0;
     (*ObjHdr)->FieldOffsetDir     = 0;
     (*ObjHdr)->ClassOffsetDir     = 0;
     (*ObjHdr)->FieldRefDir        = 0;
     (*ObjHdr)->FieldMapDir        = 0;
     (*ObjHdr)->FieldArrayDir      = 0;

     (*ObjHdr)->NoRegionsInCore    = True;


     if (FREEZE_GRP_PACKETS  == True)
     {
        (*ObjHdr)->FreezeClass     = 1;
        (*ObjHdr)->DiskCache       = False;
     } else {
        (*ObjHdr)->FreezeClass     = 0;
        (*ObjHdr)->DiskCache       = True;
     }
     if ((*ObjHdr)->ObjNum        == lSetNode_ObjNum
     ||  (*ObjHdr)->ObjNum        == gSetNode_ObjNum)
        (*ObjHdr)->FreezeClass     = 1;

     (*ObjHdr)->RegionChunkDir     = 0;
     (*ObjHdr)->RegionVbnDir       = 0;
     (*ObjHdr)->RegionSwapDir      = 0;
     (*ObjHdr)->RegionFreezeDir    = 0;

     (*ObjHdr)->TextChunk          = 0;
     (*ObjHdr)->ExObjectTbl        = 0;
     (*ObjHdr)->RgnStatusDir       = 0;
  }

  if (Normal(*Status)) 
  if (DbmFileInObjHdr    (*ObjHdr,  DbmDbId, &DbHdr->BlkHdr, Release, McStat))
  if (MemPutAddr         (DbHdr->ObjHdrDir,   LocalObj,
                          *(addrtype *)ObjHdr,    McStat))
  if (DeBug)
      MemDumpChunk   (0,  DbHdr->ObjHdrDir,  Dump_Status);

#ifdef __DISPLAY_MESSAGE__
  if (DeBug >= 0)
  if (sprintf (Msg, TraceStr(26),
/* "  CoreDb[%2u] ObjHdr[%2u] Loaded @ %x\n" */
               DbToken->DbId, LocalObj, *ObjHdr))
      TraceMsg (0, Msg);
#endif /* __DISPLAY_MESSAGE__ */
  if (DbmBug)
  if (*ObjHdr)
    OaDumpObjHdr  (0, DbToken,  (*ObjHdr)->ObjNum,   McStat);


  SwapBug        = TmpSwapBug;
  DeBug          = TmpDeBug;
  WriteHdr       = TmpWriteHdr;

/*  ...  Done by calling procedure after return from here  ...
  if (Normal(*Status)) 
      OmChkImage    (DbToken,    (*ObjHdr)->ObjNum,    McStat);
*/

TRACK(TrackBak,"DbmLoadObjHdr\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+*/
boolean DbmFileInObjHdr  (objheader  *ObjHdr,    idtype        DbId,
                          blkhdrtype *BlkHdr,    boolean       Release,
                          ft F,lt Z,zz *Status )
{
struchdr     *StrucHdr   = NullPtr;

  DbmFileInGrpHdr  ((grpheader *)ObjHdr,      DbId,
                    BlkHdr,      Release,     McStat);
  if (DbId == RenvId)
     ObjHdr->FreezeClass  = 1;

  if (Normal(*Status))
  if (ObjHdr->ObjectLockDirVbn)
  if (FileInChunk (&ObjHdr->ObjectLockDir,    BlkHdr,        Release,  
                    McStat,                 &ObjHdr->ObjectLockDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(53),
/* "    Obj ObjectLockDir    : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->ObjectLockDir,ObjHdr->ObjectLockDirVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (FileInChunk (&ObjHdr->FieldNumMap,      BlkHdr,        Release,  
                    McStat,                 &ObjHdr->FieldNumMapVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(32),
/* "    Obj FieldNumMap      : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->FieldNumMap,  ObjHdr->FieldNumMapVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (FileInChunk (&ObjHdr->LocalAttrNumMap,      BlkHdr,        Release,  
                    McStat,                 &ObjHdr->LocalAttrNumMapVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(33),
/* "    Obj LocalAttrNumMap  : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->LocalAttrNumMap,  ObjHdr->LocalAttrNumMapVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (FileInChunk (&ObjHdr->FieldOffsetDir,      BlkHdr,        Release,  
                    McStat,                 &ObjHdr->FieldOffsetDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(34),
/* "    Obj FieldOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->FieldOffsetDir,  ObjHdr->FieldOffsetDirVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (FileInChunk (&ObjHdr->ClassOffsetDir,      BlkHdr,        Release,  
                    McStat,                 &ObjHdr->ClassOffsetDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(35),
/* "    Obj ClassOffsetDir   : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->ClassOffsetDir,  ObjHdr->ClassOffsetDirVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (FileInChunk (&ObjHdr->FieldRefDir,         BlkHdr,        Release,  
                    McStat,                 &ObjHdr->FieldRefDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(36),
/* "    Obj FieldRefDir      : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->FieldRefDir,  ObjHdr->FieldRefDirVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (ObjHdr->FieldMapCount)
  if (FileInChunk (&ObjHdr->FieldMapDir,     BlkHdr,        Release,  
                    McStat,                 &ObjHdr->FieldMapDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(37),
/* "    Obj FieldMapDir     : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->FieldMapDir,           ObjHdr->FieldMapDirVbn);
     TraceMsg (0, Msg);
  }
  if (Normal(*Status))
  if (ObjHdr->FieldArrayCount)
  if (FileInChunk (&ObjHdr->FieldArrayDir,   BlkHdr,        Release,  
                    McStat,                 &ObjHdr->FieldArrayDirVbn)) 
  if (DeBug || DbmBug) {
     sprintf (Msg, TraceStr(38),
/* "    Obj FieldArrayDir   : Chunk[%4u] AT Vbn[%3u]\n" */
              ObjHdr->FieldArrayDir,  ObjHdr->FieldArrayDirVbn);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (ObjHdr->TextChunkVbn)
  if (FileInChunk     (&ObjHdr->TextChunk,   BlkHdr,        Release,  
                       McStat,              &ObjHdr->TextChunkVbn)) 
  if (MemGetChunkAddr (ObjHdr->TextChunk,    McStat,   (addrtype *)&StrucHdr))
  {
     if (StrucHdr->TextVbn)
        FileInChunk   (&StrucHdr->TextChunk,   BlkHdr,        Release,  
                       McStat,                &StrucHdr->TextVbn); 
     if (DeBug || DbmBug) 
     {
        sprintf (Msg, TraceStr(38),
/* "    Obj TextChunk   : Chunk[%4u] AT Vbn[%3u]\n" */
                 ObjHdr->TextChunk,  ObjHdr->TextChunkVbn);
        TraceMsg (0, Msg);
     }
  }

TRACK(TrackBak,"DbmFileInObjHdr\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+*
 ~L                          REGIONS  
 *+-----------------------------------------------------------------------+*/
boolean DbmFileInRegionChunk (dbheader     *CoreDb,    objheader *ObjHdr,
                              indextype     RgnIndex,  boolean    Release,
                              ft F,lt Z,zz *Status,    numtype   *RgnChunk )
{
*Status = Err_BadDesign;
return(STAT);
}


/*+-----------------------------------------------------------------------+*/
boolean    DbmPairRgnObjects  (dbheader       *CoreDb,  objheader    *ObjHdr,
                               numtype    CurrRgnChunk, ft F,lt Z,zz *Status)
{ 
indextype   Index         = 0;
indextype   Offset        = 0;
indextype   Objects       = 0;
byte       *RegionAddr    = NullPtr;
handletype *HandlePtr     = NullPtr;
addrtype    ObjectAddr    = NullPtr;
int         TempDeBug     = DeBug;
booleantype TempMemBug    = MemBug;
numtype     MemLoc        = 0;

 if (!PairObjects)
 {
    TRACK(TrackBak,"DbmPairRgnObjects\n");
    return (STAT);
 }

 if (MemGetChunkAddr (CurrRgnChunk,          McStat, (addrtype *)&RegionAddr))
     MemGetStackPtr  (CurrRgnChunk,          McStat, &Objects);

 if (!RegionAddr || !Objects)
 {
    *Status = Odbm_BadSwapChunks;
     sprintf (Msg, TraceStr(41),
/* " BAD PAIR Handles for RegionChunk[%3u] @%x count %4u\n" */
              CurrRgnChunk, RegionAddr, Objects);
     TraceMsg (0, Msg);
 }


 if (SWAP)
 {
   DeBug = 0;
   MemBug = False;
   while (Normal(*Status) 
   &&     Index < Objects)
   {
     Offset     = (ObjHdr->ObjSize * Index) + ObjHdr->HandleOffset;
     ObjectAddr =  (addrtype )(RegionAddr + (ObjHdr->ObjSize * Index));
     HandlePtr  = (handletype *)(RegionAddr + Offset);

     if (*HandlePtr          == 1
     &&  ObjHdr->ObjectCount != 1)
     {
        sprintf (Msg,  
        " DbmPairRgnObjects:: RegionChunk[%3u] Index = %2u; Handle=%4u\n",
                CurrRgnChunk,  Index, *HandlePtr);
        TraceMsg (0, Msg);
        TraceMsg (0, 
        " ***ERROR... Should not be equal 1, as is NOT a Primal!\n");

       *Status = Dbm_BadHandleInRegion;

     }
     if (*HandlePtr)
     {
        if (HmGetObjectMemLoc (CoreDb,  *HandlePtr, McStat, &MemLoc))
        if (MemLoc == 0)
            HmPutObjectAddr   (CoreDb,  *HandlePtr, ObjectAddr, McStat); 
        if (!Normal(*Status))
        if (sprintf (Msg, 
              TraceStr(45),
/* " Region[%3u] Object[%3u] Handle= %4u\n" */
                CurrRgnChunk,   Index, *HandlePtr))
           TraceMsg (0, Msg);

     }
     ++Index;
   }
   MemBug = TempMemBug;
   DeBug = TempDeBug;
 }

TRACK(TrackBak,"DbmPairRgnObjects\n");
return (STAT);
}
 

boolean    DbmUnPairRgnObjects
                              (dbheader       *CoreDb,  objheader    *ObjHdr,
                               numtype    CurrRgnChunk, ft F,lt Z,zz *Status)
{ 
indextype   Index         = 0;
indextype   Offset        = 0;
indextype   Objects       = 0;
byte       *RegionAddr    = NullPtr;
handletype *HandlePtr     = NullPtr;
addrtype    NullAddrValue = NullPtr;
int         TempDeBug     = DeBug;
booleantype TempMemBug    = MemBug;
numtype     MemLoc        = 0;

   *Status = Err_NotImplemented;


TRACK(TrackBak,"DbmUnPairRgnObjects\n");
return (STAT);
}
 

void       DbmDumpObjHdr (int           TraceSwi, 
                          objheader    *ObjHdr,    ft F,lt Z,zz *Status)
{

STAT; 
return;
}

/*+-----------------------------------------------------------------------+
~M                         END OF ODBM.C                                  !
  +-----------------------------------------------------------------------+*/
