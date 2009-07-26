/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#define MEM_C
#define MEM00
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem00.h"

#ifndef   CORE_CRC_CHECK                /* PUT wrapper on VOID blocks     */
#undef    VALIDATE_MEM_CHUNKS           /* DO NOT validate end of Chunk   */
#endif /* CORE_CRC_CHECK  */            /* PUT wrapper on VOID blocks     */

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[2] = {"\0", "   ...MEM00... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[50] =
{
   " A Null Trace String Msg",
   "  Theory       = %x\n",
   " *TheOry       = %x\n",
   "  ChunkTblPtr  = %x\n",
   "  Theory->ChunkAddr  = %x\n",
   "  ChunkAddrPtr  = %x\n",
   " *ChunkAddrPtr  = %x\n",
   "  ChunkTblPtr[0]     = %x\n",
   "  Theory->ChunkAddr  = %x\n",
   "  Theory->NextChunk  = %x\n",
   "  Theory->(%d)          = %x\n",
   "  ChunkTablePtr[%d]     = %x\n",
   " &Theory->NextChunk[%d] = %x\n",
   "  NextChunk(%d)         = %d\n",
   "Chunk(%d) [%x]  :: StackPtr = %d\n",
   "Get Chunk(%d)[%d] = %d\n",
   "Put Chunk(%d)[%d] = %d\n",
   "Get Chunk(%d)[%d] = %d\n",
   "Push XmemBlkSize[%d] = %d\n",
   "Push XmemBlkSize[%d] = %d\n",
   "Get XmemBlkSize[0] = %d\n",
   "Pop XmemBlkSize[%d] = %d\n",
   "Pop XmemBlkSize[%d] = %d\n",
   "GetSomeBytes from  Chunk(%d)[%d] = %d\n",
   "    New     Chunk     %6u.  NextFree= %6u\n",
   "    Reclaim Chunk     %6u.  NextFree= %6u\n",
   "        Mem  FreeChunkChunk  [%6u]; 2nd Free=%6u\n",
   "     &Chunk(%d)_ChunkAddr @%x\n",
   "     &Chunk[%4u] @%8x;                           CRC:= <%-8x>\n",
   "    Chunk_NewChunk[%6u] @ %x\n",
   "   InitChunk [%6u] UnitSize= %8u... Size_Conflict!!!\n",
   "    MemGetChunkBlk As Chunk[%6u] Blk@ %x BlkSize=%5u\n",
   "    MemAssignChunkStruc To Chunk[%6u]\n",
   "      MemPutChunkStruc[%6u] ...\n",
   "      Unit Size =%3u, Count =%6u; StackPtr =%6u\n",
   "\n   Mem_ExpandChunk::   [%4u] StackPtr =%6u; By ExtendQty=%6u\n",
   "                       Current StackPtr=%6u;\n",
   "   NO bytes moved... SourceSize=%5u, TargetSize=%5u;\n",
   "     Theory->ChunkAddr  = %x\n",
   "     Copy from Source @%x TO Target @%x * %u Bytes\n",
   "     Theory->@%x  = %x; StackPtr=%4u\n",
   "     Copy from Source(*%8u) @%x TO Target(*%8u) @%x\n",
   " ChunkNum[%4u] > NumChunks=%4u\n",
   " UnitSize %u !=  SizeOfUnit %u ::",
   " Index = %4u >= StackPtr = %4u ::",
   " UnitSize %u !=  SizeOfUnit %u ::",
   " UnitSize %u !=  SizeOfUnit %u ::",
   " Index = %4u >= StackPtr = %4u ::",
   " UnitSize %u !=  SizeOfUnit %u ::",
   " *** Chunk not Numeric || Data. ::"
};
#endif

/*    */
#define MemMakeChunk           Mem_MakeChunk
#define MemDumpChunk           Mem_DumpChunk   
#define MemGetChunkAddr        Mem_GetChunkAddr
#define MemPutStackPtr         Mem_PutStackPtr 
#define MemGetStackPtr         Mem_GetStackPtr 
#define MemPushWord            Mem_PushWord    
#define MemPutWord             Mem_PutWord     
#define MemPopWord             Mem_PopWord     
#define MemGetWord             Mem_GetWord     
#define MemGetSomeBytes        Mem_GetSomeBytes

       boolean MemMakeChunk   (bytessize     UnitSize,  indextype AllocQty,
                               indextype     ExtendQty, idtype    CoreDbId,
                               ft F,lt Z,zz *Status,    numtype  *Chunk);

       boolean MemDumpChunk   
                              (int           MsgLevel,  numtype   Chunk,
                               ft F,lt Z,zz *Status);

       boolean Mem_DumpChunkPart   
                              (int           MsgLevel,  numtype       Chunk,
                               numtype       Ilow,      numtype       Ihigh,
                               ft F,lt Z,zz *Status);

       void    Mem_AssignXmemFile
                              (char         *FileName);

       boolean Mem_MakeXmemChunks 
                         (indextype     AllocQty,  indextype     ExtendQty,
                          ft F,lt Z,zz *Status);

       void    Mem_ShowXmemStats
                              (void);

       boolean MemGetChunkAddr(numtype       Chunk,
                               ft F,lt Z,zz *Status,    addrtype *ChunkAddr);

       boolean MemPutStackPtr (numtype       Chunk,     indextype StackPtr,
                               ft F,lt Z,zz *Status);

       boolean MemGetStackPtr (numtype       Chunk,
                               ft F,lt Z,zz *Status,    indextype *StackPtr);

       boolean MemPushWord    (numtype       Chunk,    word       WordValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemPutWord     (numtype       Chunk,    indextype  Index,
                               word          WordValue,ft F,lt Z,zz *Status);

       boolean MemPopWord     (numtype       Chunk,    ft F,lt Z,zz *Status,
                               word         *WordValue,indextype *StackPtr);

       boolean MemGetWord     (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   word      *WordValue);

       boolean MemGetSomeBytes(numtype       Chunk,     indextype   Index,
                               bytessize     ByteCount,
                               ft F,lt Z,zz *Status,    addrtype    SomeBytes);
 
#define MemPushIndex           Mem_PushWord
#define MemPutIndex            Mem_PutWord
#define MemGetIndex            Mem_GetWord
#define MemPopIndex            Mem_PopWord


/*+-----------------------------------------------------------------------+
~CM                         MEM00 -- Ory Creation                         !
  +-----------------------------------------------------------------------+*/
void    Mem_Init  
               (indextype     Alloc_FreeAddrs,  indextype  Extend_FreeAddrs,
                char         *Xmem_File,
                indextype     Alloc_MemChunks,  indextype  Extend_MemChunks,
                indextype     Alloc_XmemBlks,   indextype  Extend_XmemBlks)
{
  KrnlBlocks_AllocQty    = Alloc_FreeAddrs;
  KrnlBlocks_ExtendQty   = Extend_FreeAddrs;
  Alloc_Chunks           = Alloc_MemChunks;
  Extend_Chunks          = Extend_MemChunks;

  Mem_AssignXmemFile      (Xmem_File);         
  XmemBlocks_AllocQty    = Alloc_XmemBlks;
  XmemBlocks_ExtendQty   = Extend_XmemBlks;

return;
}


boolean   MemMakeOry (ft F,lt Z,zz *Status,   addrtype  *TheOry)
{

 if (MemNewOry     (McStat,      TheOry))
     MemTestTheOry (McStat);
 
TRACK(TrackBak,"MemMakeOry\n");
return (STAT);
}
 

boolean    MemNewOry (ft F,lt Z,zz *Status,   addrtype  *TheOry)
{
addrtype     TableChunkAddr = NullPtr;
numtype      ChunkChunk     = 0;
addrtype     *ChunkAddrPtr  = NullPtr;
addrtype     *ChunkTblPtr   = NullPtr;
unsigned     i              = 0;
addrtype     ChunkBlk       = NullPtr;
static  int  FirstTimeIn    = 0;

  if (Theory        == NullPtr
  ||  FirstTimeIn++ == 0)
  {
     if (CoreMoreCore (sizeof(OryType),   McStat,   (addrtype *)(&Theory)))
     {
        ChunkTblPtr = (addrtype *)Theory;
       *TheOry      = (addrtype  )Theory;

     }
     if (Normal(*Status)) 
     {
         ChunkBlk     = (addrtype )GET_MEM (1,
                               InitBlocks (sizeof(addrtype))    );
         ChunkBlk     = (addrtype )((char *)ChunkBlk + sizeof (chunkstruc));
     }

     if (Normal(*Status)) 
     if ((Theory->ChunkAddr = (addrtype *)ChunkBlk)
     !=  NullPtr)
     {
        /* and set the first ChunkAddr to point to itself (C++ This)       */

        ChunkAddrPtr      =  Theory->ChunkAddr;
       *ChunkAddrPtr      =  (addrtype )ChunkAddrPtr;
     }

     if (Normal(*Status))
     {
       ++ChunkAddrPtr;
         ChunkBlk     = (addrtype )GET_MEM (1,
                               InitBlocks (sizeof(chunknum))    );
         ChunkBlk     = (addrtype )((char *)ChunkBlk + sizeof (chunkstruc));
        *ChunkAddrPtr = (addrtype *)ChunkBlk;

       ++ChunkAddrPtr;
         ChunkBlk     = (addrtype )GET_MEM (1,
                               InitBlocks (sizeof(chunkflags))    );
         ChunkBlk     = (addrtype )((char *)ChunkBlk + sizeof (chunkstruc));
        *ChunkAddrPtr = (addrtype *)ChunkBlk;

         ChunkAddrPtr =  Theory->ChunkAddr;
     }

     if (Normal(*Status))
     { 
        for (  ChunkChunk = ChunkAddrChunk;
             ++ChunkChunk < NumChunkChunks; )
        {
            *(++ChunkTblPtr) = *(++ChunkAddrPtr);
        }
        ChunkTblPtr = Theory->ChunkAddr;

        for (ChunkChunk = ChunkAddrChunk;
             ChunkChunk < NumChunkChunks; ChunkChunk++)
        {
          i                                 = ChunkChunk;
          TableChunkAddr                    = ChunkTblPtr[i];

          Unit_Count      (ChunkChunk)      = Alloc_Chunks;
          Extend_Qty      (ChunkChunk)      = Extend_Chunks;
          Stack_Ptr       (ChunkChunk)      = NumChunkChunks;
          Next_Chunk      (ChunkChunk)      = 0;
          CRC_Value       (ChunkChunk)      = CHUNK_CONSTANT;

          IsNotText_Chunk (ChunkChunk);
          IsInCore_Chunk  (ChunkChunk);
          IsData_Chunk    (ChunkChunk);
          IsNumeric_Chunk (ChunkChunk);

        }

        ChunkChunk                  = ChunkAddr_Chunk;
        Unit_Size(ChunkChunk++)     = (sizeinbytes)sizeof (addrtype);
        Unit_Size(ChunkChunk++)     = (sizeinbytes)sizeof (chunknum);
        Unit_Size(ChunkChunk++)     = (sizeinbytes)sizeof(chunkflags);
     }

     if (Normal(*Status))
     if (MemBug)
     {
        TraceMsg (0, " Dump The Chunk Chunks\n");
        for (ChunkChunk = ChunkAddrChunk; 
             ChunkChunk < NumChunkChunks;   ChunkChunk++)
           DumpChunkChunk (0, ChunkChunk);
     }

     if (Normal(*Status))
        MemMakeOryChunks (McStat);

  } else
    *Status = Env_OryExists;

TRACK(TrackBak,"  strcpy (MemNewOry\n");
return (STAT);
}
 

boolean    MemMakeOryChunks (ft F,lt Z,zz *Status)
{
numtype       FreeAddrChunk          = 0;
addrtype     *FreeAddr               = NullPtr;
numtype       FreeSizeChunk          = 0;
numtype      *FreeSize               = NullPtr;

   if (MemMakeKrnlChunk 
                    (sizeof(addrtype),        KrnlBlocks_AllocQty, 
                     KrnlBlocks_ExtendQty,    0,
                     McStat,                             &FreeAddrChunk))
   if (MemMakeKrnlChunk 
                    (sizeof(bytessize),       KrnlBlocks_AllocQty, 
                     KrnlBlocks_ExtendQty,    0,
                     McStat,                             &FreeSizeChunk))
   {
      FreeAddr  = (addrtype *)Theory->ChunkAddr[FreeAddrChunk];
      FreeSize  = (numtype  *)Theory->ChunkAddr[FreeSizeChunk];
      Core_AssignChunks  (FreeAddrChunk,      FreeAddr,
                          FreeSizeChunk,      FreeSize);
   }

   if (DeBug || CoreBug) 
   {
      DumpChunkChunk (1, FreeAddrChunk);
      DumpChunkChunk (1, FreeSizeChunk);
   }


   if (Normal(*Status))
      Mem_MakeXmemChunks (XmemBlocks_AllocQty, XmemBlocks_ExtendQty,
                          McStat);
TRACK(TrackBak,"MakeOryChunks\n");
return (STAT);
}
 

boolean    MemMakeKrnlChunk  
                          (bytessize     UnitSize,  indextype   AllocQty,
                           indextype     ExtendQty, idtype      CoreDbId,
                           ft F,lt Z,zz *Status,    numtype    *ChunkNum)
{
boolean        VarLength      = False;
boolean        PtrChunk       = False;
sizeinblocks   Blocks         = 0;
addrtype       ChunkAddr      = NullPtr;
farlongtype    AllocCore1     = 0;
sizetype       ByteAlignment  = 0;
bytessizetype  SizeInBytes    = 0;

  if (!UnitSize || !AllocQty)
  {
     *Status = Err_BadUnitSize;
  }

  if (Normal(*Status))
  if (! AllocQty)
     Blocks  = InitBlocks (UnitSize);
  else
     Blocks  = BlockCount (UnitSize, AllocQty);

  Blocks   = Blocks * Block_Size;

  if (Normal(*Status))
  if (Blocks % CoreBlk_GapSpace)
  {
     ByteAlignment   = CoreBlk_GapSpace - (Blocks  % CoreBlk_GapSpace);
     SizeInBytes     = Blocks  + ByteAlignment;
  }

  if (Normal(*Status))
  {
     AllocCore1      = (farlongtype )SizeInBytes;
     CoreBug = True;
     if ((ChunkAddr  = (addrtype )VoidExtendCore (&AllocCore1)) == NULL)
        *Status      = Env_CoreNoMore;
     CoreBug = False;

     if (AllocCore1 != SizeInBytes)
        *Status      = Env_CoreNoMore;
  }

  if (Normal(*Status))
  if (NewChunkChunk       (McStat ,  ChunkNum))
  {
     ChunkAddr                 = (addrtype )((char *)ChunkAddr
                                                     + sizeof (chunkstruc));
     Chunk_Addr  (*ChunkNum)   = ChunkAddr;
     Unit_Count  (*ChunkNum)   = AllocQty;
  }

  if (Normal(*Status))
  if (InitChunk    (*ChunkNum, VarLength, PtrChunk, UnitSize,
                    ExtendQty, CoreDbId,            McStat))
  {
     IsNumeric_Chunk    (*ChunkNum);
     IsData_Chunk       (*ChunkNum);
  }

TRACK(TrackBak,"MemMakeKrnlChunk\n");
return (STAT);
}


boolean MemTestTheOry (ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"MemTestTheOry\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~RM                         REGION  MANAGER                               !
  +-----------------------------------------------------------------------+*/
 
boolean    NewRegion      (sizeinblocks  Blocks,
                           ft F,lt Z,zz *Status, addrtype *Region)
{

  CoreMoreCore (Blocks * Block_Size, McStat, Region);

TRACK(TrackBak,"NewRegion\n");
return(STAT);
}
 
sizeinblocks
           InitBlocks     (bytessize     UnitSize)
{
sizeinbytes   RegionSize = sizeof (chunkstruc) + (UnitSize * Alloc_Chunks);
sizeinblocks  Blocks     = RegionSize / Block_Size;
 
StackTrack ("InitBlocks\n");

return ((RegionSize % Block_Size > 0) ? ++Blocks : Blocks);
}
 
sizeinblocks
           BlockCount     (bytessize     UnitSize, indextype  UnitCount)
{
sizeinbytes   RegionSize = sizeof (chunkstruc) + (UnitSize * UnitCount);
sizeinblocks  Blocks     = RegionSize / Block_Size;
 
StackTrack ("BlockCount\n");
return ((RegionSize % Block_Size > 0) ? ++Blocks : Blocks);
}

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
boolean    NewChunkChunk (ft F,lt Z,zz *Status,     numtype   *ChunkNum)
{
chunknum ChunkChunk = 0;
int      TempDeBug  = DeBug;

  if ((*ChunkNum    = Theory->NextFree)          == 0)
  {
     if ((*ChunkNum = Stack_Ptr(ChunkAddrChunk)) == Unit_Count(ChunkAddrChunk))
     {
        ExpandChunkTables (McStat);
        Mem_DumpChunkPart (0,  0, 0, NumChunkChunks, McStat);
        DeBug = TempDeBug;
     }

     for (ChunkChunk = ChunkAddrChunk;
          ChunkChunk < NumChunkChunks; ChunkChunk++)
     {
          ++Stack_Ptr(ChunkChunk);
     }
   ++NumChunks;
   ++ChunksAllocated;

     Next_Chunk        (*ChunkNum) = 0;


  } else {
     Theory->NextFree = Next_Chunk(*ChunkNum);
     ChunksReclaimed++;
     Next_Chunk        (*ChunkNum) = 0;

  }

TRACK(TrackBak,"NewChunkChunk\n");
return(STAT);
}
 
boolean    FreeChunkChunk  (numtype      *Chunk,  ft F,lt Z,zz *Status)
{
/* Procedure NOT called by anyone !!!!                                     */
   if (*Chunk)
   {
      ChunksFreeed++;
      Next_Chunk(*Chunk)  = Theory->NextFree;
      Theory->NextFree    = *Chunk;
     *Chunk               = 0;
   }

TRACK(TrackBak,"FreeChunkChunk\n");
return (STAT);
}
 
boolean    ExpandChunkTables (ft F,lt Z,zz *Status)
{
indextype      StackPtr       = 0;
numtype        ChunkChunk     = 0;
static numtype Expands        = 0;

   sprintf (Msg, "\n   *** INFO *** ExpandChunkChunks[%4u]!\n", ++Expands);
   TraceMsg (MSGFILE, Msg);

   while (Normal(*Status)
   &&     ChunkChunk     < NumChunkChunks)
   {
     ExpandChunk  (ChunkChunk,   McStat,     &StackPtr);

   ++ChunkChunk;
   }

TRACK(TrackBak,"ExpandChunkTables\n");
return (STAT);
}
 
void       DumpChunkChunk (int       Dump,      chunknum     ChunkNum)
{

return;
}


/*+-----------------------------------------------------------------------+
~CM                         MEM00 -- Chunk Manager                        !
  +-----------------------------------------------------------------------+*/
 
boolean    MakeChunk(boolean       VarLength,   boolean     PtrChunk,
                     bytessize     UnitSize,    indextype   AllocQty,
                     indextype     ExtendQty,   idtype      CoreDbId,
                     ft F,lt Z,zz *Status,      numtype    *ChunkNum)
{
static boolean FirstChunk = True;

    *Status = Env_Normal;
    if (! UnitSize)
      *Status = Err_BadUnitSize;

    if (Normal(*Status))
    if (Chunk_NewChunk  (UnitSize,  AllocQty,  McStat,   ChunkNum))

        InitChunk (*ChunkNum, VarLength, PtrChunk, UnitSize,
                    ExtendQty, CoreDbId,           McStat);

    if (Normal(*Status) && abs(DeBug) >= 4
    ||  FirstChunk)
    {
       DumpChunkChunk (0, *ChunkNum);
       FirstChunk = False;
    }
    if (MemBug) 
    if (TraceMsg (0, "    MAKE Chunk "))
        DumpChunkChunk (0, *ChunkNum);

TRACK(TrackBak,"MakeChunk\n");
return(STAT);
}

boolean    Chunk_NewChunk  
                     (sizeinbytes   UnitSize,   indextype    AllocQty,
                      ft F,lt Z,zz *Status,     numtype     *ChunkNum)
{
sizeinblocks  Blocks     = 0;
addrtype      ChunkAddr  = NullPtr;

 if (! AllocQty)
    Blocks  = InitBlocks (UnitSize);
 else
    Blocks  = BlockCount (UnitSize, AllocQty);

 if (NewRegion           (Blocks,   McStat,     &ChunkAddr))
 if (NewChunkChunk       (McStat ,  ChunkNum))
 {
     ChunkAddr                 = (addrtype )((char *)ChunkAddr
                                                     + sizeof (chunkstruc));
     Chunk_Addr  (*ChunkNum)   = ChunkAddr;
     Unit_Count  (*ChunkNum)   = AllocQty;
 }


TRACK(TrackBak,"Chunk_NewChunk\n");
return(STAT);
}
 
boolean    InitChunk    (numtype       ChunkNum,   boolean       VaryLength,
                         boolean       PtrChunk,   bytessize     UnitSize,
                         indextype     ExtendQty,  idtype        CoreDbId,
                         ft F,lt Z,zz *Status)
{
   if (VaryLength)
   {
      Unit_Size      (ChunkNum) = sizeof(bytetype);
      Extend_Qty     (ChunkNum) = 0;
      MaxUnit_Length (ChunkNum) = UnitSize;

   } else {
      Unit_Size      (ChunkNum) = UnitSize;
      Extend_Qty     (ChunkNum) = ExtendQty;
      MaxUnit_Length (ChunkNum) = 0;
   }
   Stack_Ptr         (ChunkNum) = (chunkindex)VaryLength;
   CoreDbId_Chunk    (ChunkNum) = CoreDbId;

   VaryLength   ?  IsText_Chunk   (ChunkNum) : IsNotText_Chunk   (ChunkNum);
   Clean_Chunk       (ChunkNum);
   Thaw_Chunk        (ChunkNum);
   PtrChunk     ?  IsString_Chunk (ChunkNum) : IsNotString_Chunk (ChunkNum);
   IsActive_Chunk    (ChunkNum);
   IsInCore_Chunk    (ChunkNum);

   CRC_Value         (ChunkNum) = CHUNK_CONSTANT;

   if (NotSameSize_Chunk          (ChunkNum,  UnitSize))
   {
       DumpChunkChunk (0, ChunkNum);
      *Status = Err_BadUnitSize;
   }

TRACK(TrackBak,"InitChunk\n");
return(STAT);
}
 
boolean    MemNewChunkBlk
                     (sizeinbytes   BlkSize,      ft F,lt Z,zz *Status,
                      numtype      *ChunkNum,     addrtype     *ChunkBlk)
{
addrtype      ChunkAddr  = NullPtr;
sizeinblocks  Blocks     = 0;
sizeinbytes   ChunkSize  = 0;

 ChunkSize = BlkSize - sizeof (chunkstruc);
 Blocks    = BlockCount (ChunkSize, 1);

 if (NewRegion           (Blocks,   McStat,      &ChunkAddr))
 if (NewChunkChunk       (McStat ,  ChunkNum))
 {
    *ChunkBlk                  = ChunkAddr;
     ChunkAddr                 = (addrtype )((char *)ChunkAddr
                                                     + sizeof (chunkstruc));
     Chunk_Addr  (*ChunkNum)   = ChunkAddr;
 }

TRACK(TrackBak,"MemNewChunkBlk\n");
return(STAT);
}
 

boolean    MemPutChunkStruc
                     (numtype       Chunk,    chunkstruc   *ChunkRef,
                      idtype        CoreDbId, ft F,lt Z,zz *Status)
{

 Next_Chunk        (Chunk) = 0;
 CoreDbId_Chunk    (Chunk) = CoreDbId;

 ChunkRef->StringText      ? IsText_Chunk (Chunk) : IsNotText_Chunk (Chunk);
 IsNotText_Chunk   (Chunk);
 Clean_Chunk       (Chunk);
 Thaw_Chunk        (Chunk);
 IsNotString_Chunk (Chunk);
 IsActive_Chunk    (Chunk);
 IsInCore_Chunk    (Chunk);

 IsNumeric_Chunk   (Chunk);
 IsData_Chunk      (Chunk);

TRACK(TrackBak,"MemPutChunkStruc\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    GrowNumericChunk (numtype  ChunkNum,  ft F,lt Z,zz *Status)
{
indextype     StackPtr = 0;

   ExpandChunk     (ChunkNum,   McStat,        &StackPtr);

TRACK(TrackBak,"GrowNumericChunk\n");
return(STAT);
}

boolean    GrowTextChunk (numtype     ChunkNum,  ft F,lt Z,zz *Status)
{
indextype     StackPtr = 0;

 if (ChunkBug || DeBug || MemBug)
 if (TraceMsg        (0, "\n   *** INFO *** GrowTextChunk   ... "))
    DumpChunkChunk      (0,          ChunkNum);

 if (ExpandChunk     (ChunkNum,   McStat,        &StackPtr))

 if (ChunkBug || DeBug || MemBug)
 if (TraceMsg    (0, "                New Chunk Specs ... "))
     DumpChunkChunk  (0,          ChunkNum);
 
TRACK(TrackBak,"GrowTextChunk\n");
return(STAT);
}
 
boolean    ExpandChunk (chunknum      ChunkNum,
                        ft F,lt Z,zz *Status,   indextype   *StackPtr)
{
addrtype       Source         = NullPtr;
addrtype       SourceRegion   = NullPtr;
sizeinbytes    SourceSize     = 0;
addrtype       TargetRegion   = NullPtr;
indextype      NewCount       = 0;
sizeinbytes    TargetSize     = 0;
addrtype      *ChunkAddrPtr   = NullPtr;
addrtype      *ValuePtr       = NullPtr;
booleantype    TmpSwap        = SWAP;
static numtype NumChunkChunkFrees  = 0;

  SWAP         = False; 

  SourceRegion = Chunk_Addr (ChunkNum);
  SourceRegion = (addrtype )((char *)SourceRegion - sizeof (chunkstruc));
  Source       = SourceRegion;

  SourceSize   = Unit_Size  (ChunkNum) *  Unit_Count (ChunkNum);
  SourceSize  += sizeof (chunkstruc);

  NewCount     = Unit_Count (ChunkNum) +  Extend_Qty (ChunkNum);

 *StackPtr     = Stack_Ptr  (ChunkNum);
  if (NewCount < *StackPtr)
     NewCount  = *StackPtr;

  TargetSize   = Unit_Size  (ChunkNum)  * NewCount;
  TargetSize  += sizeof (chunkstruc);

  if (TargetSize <= SourceSize)
  {
      *Status = Err_NoBytesMoved;
      sprintf (Msg, TraceStr(37),
/* "   NO bytes moved... SourceSize=%5u, TargetSize=%5u;\n" */
               SourceSize, TargetSize);
      TraceMsg (0, Msg);
      DumpChunkChunk   (0, ChunkNum);
  }

  if (Normal(*Status))
  if (ChunkNum   < NumChunkChunks)
  {
     if (NewRegion (TargetSize,   McStat,  (addrtype *)&ChunkAddrPtr))
     {
        TargetRegion   = (addrtype  )ChunkAddrPtr;
        ChunkAddrPtr   = (addrtype *)((char *)ChunkAddrPtr 
                                               + sizeof (chunkstruc));
     }

     if (Normal(*Status))
     if (ChunkNum == 0)
     {
        Theory->ChunkAddr = ChunkAddrPtr;

       *ChunkAddrPtr      =  (addrtype   )ChunkAddrPtr;
        ChunkTheoryAddr   =  ChunkAddrPtr;

     } else {
        ChunkTheoryAddr++;
     }
 
     if (Normal(*Status))
     {
        memcpy  ((char *)TargetRegion, (char *)SourceRegion, SourceSize);

        TargetRegion  = (addrtype )((char *)TargetRegion
                                             + sizeof (chunkstruc));

        if (ChunkNum == 0)
        {
           Theory->ChunkAddr = (addrtype   *)ChunkAddrPtr;

        } else if (ChunkNum == 1) {
           Theory->NextChunk = (numtype    *)ChunkAddrPtr;

        } else if (ChunkNum == 2) {
           Theory->Flags     = (chunkflags *)ChunkAddrPtr;

        } else {
           memcpy  ((char *)ChunkTheoryAddr,
                    (char *)&TargetRegion,            sizeof(addrtype));
        }
        ValuePtr                        = ChunkTheoryAddr;

        Theory->ChunkAddr[ChunkNum]     = TargetRegion;
        Unit_Count(ChunkNum)            = NewCount;
        Mem_ChunkExpanded               = ChunkNum;
     }

     if (++NumChunkChunkFrees > 3)
        CoreFreeCoreSpace (&Source,  SourceSize,   McStat);
     else
        FREE_MEM   (Source);

  } else { 
     if (NewRegion   (TargetSize,      McStat,     (addrtype *)&ChunkAddrPtr))
     {
        TargetRegion = ChunkAddrPtr;

        memcpy  ((char *)ChunkAddrPtr, (char *)SourceRegion, SourceSize);

        TargetRegion  = (addrtype )((char *)TargetRegion
                                             + sizeof (chunkstruc));
        ChunkAddrPtr  = (addrtype *)TargetRegion;

        Chunk_Addr(ChunkNum)    = TargetRegion;
        Unit_Count(ChunkNum)    = NewCount;
     }
     if (Normal(*Status))
        CoreFreeCoreSpace (&Source,  SourceSize,   McStat);
  }

  if (Normal(*Status))
      Mem_ChunkExpanded      = ChunkNum;

  SWAP  = TmpSwap; 

TRACK(TrackBak,"ExpandChunk\n");
return(STAT);
}
 

boolean    Mem_ExpandKrnlChunk  
                            (chunknum      ChunkNum,
                             ft F,lt Z,zz *Status,   addrtype    *KrnlAddr)
{
addrtype      Source         = NullPtr;
addrtype      SourceRegion   = NullPtr;
sizeinbytes   SourceSize     = 0;
addrtype      TargetRegion   = NullPtr;
indextype     NewCount       = 0;
sizeinbytes   TargetSize     = 0;
addrtype     *ChunkAddrPtr   = NullPtr;
farlongtype   AllocCore1     = 0;
sizetype      ByteAlignment  = 0;
int           TmpDeBug       = DeBug;
booleantype   TmpSwap        = SWAP;

  SWAP          = False; 

  SourceRegion  = Chunk_Addr (ChunkNum);
  SourceRegion  = (addrtype )((char *)SourceRegion - sizeof (chunkstruc));
  Source        = SourceRegion;

  SourceSize    = Unit_Size  (ChunkNum) *  Unit_Count (ChunkNum);
  SourceSize   += sizeof (chunkstruc);

  NewCount      = Unit_Count (ChunkNum) +  Extend_Qty (ChunkNum);

  TargetSize    = Unit_Size  (ChunkNum)  * NewCount;
  TargetSize   += sizeof (chunkstruc);

  if ((AllocCore1 = TargetSize) % CoreBlk_GapSpace)
  {
     ByteAlignment       = CoreBlk_GapSpace - (TargetSize % CoreBlk_GapSpace);
     AllocCore1         += ByteAlignment;
     TargetSize          = AllocCore1;
  }
  if ((ChunkAddrPtr      = (addrtype *)VoidExtendCore (&AllocCore1)) 
                         == NULL)
  {
    *Status              = Env_CoreNoMore;

  } else if (AllocCore1 != TargetSize)
    *Status              = Env_CoreNoMore;

  if (Normal(*Status))
  {
     TargetRegion        = ChunkAddrPtr;

     memcpy  ((char *)ChunkAddrPtr, (char *)SourceRegion, SourceSize);

     TargetRegion         = (addrtype )((char *)TargetRegion
                                             + sizeof (chunkstruc));
     ChunkAddrPtr         = (addrtype *)TargetRegion;

     Chunk_Addr(ChunkNum) = TargetRegion;
     Unit_Count(ChunkNum) = NewCount;
    *KrnlAddr             = TargetRegion; 
  }


  AllocCore1 = SourceSize;
  if (Normal(*Status))
  if (SourceSize % CoreBlk_GapSpace)
  {
     ByteAlignment       = CoreBlk_GapSpace - (SourceSize % CoreBlk_GapSpace);
     AllocCore1         += ByteAlignment;
  }

  if (Normal(*Status))
     VoidFreeCore       (&Source,     AllocCore1);

  DeBug = TmpDeBug;
  SWAP  = TmpSwap; 

TRACK(TrackBak,"Mem_ExpandKrnlChunk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
#ifndef   __MACROIZE_MEM__ 
boolean    Chunk_ChunkExists
                         (numtype      ChunkNum,  ft F,lt Z,zz *Status)
{
  if (ChunkNum > NumChunks)
  {
     *Status = Err_BadChunk;

  } else if (!ChunkNum) {
     *Status = Err_BadChunk;

  } else if (Chunk_Addr(ChunkNum) == NullPtr) {
     *Status = Err_BadChunk;

  } else if (CRC_Value(ChunkNum) != CHUNK_CONSTANT) {
      DumpChunkChunk (0, ChunkNum);
     *Status = Mem_ChunkCRCisBad;

#ifdef     VALIDATE_MEM_CHUNKS
#ifdef     SYSTEM_ALLOC

  } else if (Chunk_Validate(ChunkNum) != CHUNK_CONSTANT) {

      DumpChunkChunk (0, ChunkNum);

     *Status = Mem_ChunkCRCisBad;

#endif  /* SYSTEM_ALLOC */
#endif  /* VALIDATE_MEM_CHUNKS */
  }

  Mem_ChunkExpanded                = 0;

TRACK(TrackBak,"Chunk_ChunkExists\n");
return(STAT);
}
#endif /* __MACROIZE_MEM__ */

/*+-----------------------------------------------------------------------+*/
boolean    ChkPushChunk (numtype       ChunkNum,  size_t    SizeOfUnit,
                         ft F,lt Z,zz *Status)
{

 TRACK(TrackBak,"ChkPushCunk\n");

 *Status    = Env_Normal;
 Mem_ChunkExpanded                = 0;
 if (ChunkExists (ChunkNum,McStat))
 {
   if (Chunk_IsData      (ChunkNum))
   if (Chunk_IsNumeric   (ChunkNum))
   if ((size_t)Unit_Size (ChunkNum)   == SizeOfUnit)
   {
     if (Stack_Ptr       (ChunkNum)   >= Unit_Count  (ChunkNum))
        GrowNumericChunk (ChunkNum,      McStat);
     return (STAT);
   }

   *Status = Err_BadUnitType;
    DumpChunkChunk (0, ChunkNum);
 }

TRACK(TrackBak,"ChkPushChunk\n");
return(STAT);
}
 
boolean    ChkPutChunk (numtype       ChunkNum,   indextype   Index,
                        size_t        SizeOfUnit,
                        ft F,lt Z,zz *Status,     boolean    *PushFlag)
{
  *Status    = Env_Normal;
  *PushFlag  = False;
  Mem_ChunkExpanded                = 0;
   if (ChunkExists (ChunkNum, McStat))
   {
     if (Chunk_IsData      (ChunkNum)
     &&  Chunk_IsNumeric   (ChunkNum)
     &&  (size_t)Unit_Size (ChunkNum) == SizeOfUnit)
     {
        if (Index   >  Stack_Ptr(ChunkNum))
        {
           DumpChunkChunk (0, ChunkNum);
           *Status    = Err_IndexOutOfRange;

        } else if (Index              == Stack_Ptr (ChunkNum))
           *PushFlag = True;

     } else {
        *Status = Err_BadUnitType;
        DumpChunkChunk (0, ChunkNum);
     }
   }

TRACK(TrackBak,"ChkPutChunk\n");
return(STAT);
}
 
boolean    ChkPopChunk (numtype       ChunkNum,   size_t SizeOfUnit,
                        ft F,lt Z,zz *Status)
{
  TRACK(TrackBak,"ChkPopChunk\n");
  Mem_ChunkExpanded                = 0;
  if (ChunkExists           (ChunkNum,      McStat))
  {
     if (Chunk_IsData       (ChunkNum))
     if (Chunk_IsNumeric    (ChunkNum))
     if ((size_t)Unit_Size  (ChunkNum)   == SizeOfUnit)
     {
       *Status    = Env_Normal;
        return(STAT);
     }

     *Status = Err_BadUnitType;
     DumpChunkChunk (0, ChunkNum);
  }

TRACK(TrackBak,"ChkPopChunk\n");
return(STAT);
}
 
boolean    ChkGetChunk (numtype     ChunkNum,    indextype     Index,
                        size_t      SizeOfUnit,  ft F,lt Z,zz *Status)
{
indextype  StackPtr = 0;

  Mem_ChunkExpanded                = 0;
  if (ChunkExists (ChunkNum,McStat)) 
  {
     if (Chunk_IsData        (ChunkNum)
     &&  Chunk_IsNumeric     (ChunkNum)
     && (size_t)Unit_Size    (ChunkNum)   == SizeOfUnit)
     {
       StackPtr = Stack_Ptr  (ChunkNum);
       if (Index   >= StackPtr)
       {
          if (SetGetSwi)
            *Status    = Set_EndOfSet;

          else {
             DumpChunkChunk (0, ChunkNum);
            *Status    = Err_IndexOutOfRange;
          }
       }

     } else {
        if ((size_t)Unit_Size     (ChunkNum) != SizeOfUnit)
        {
           *Status = Err_BadUnitType;

        } else {
           *Status = Err_BadChunk;
        }
        DumpChunkChunk (0, ChunkNum);
     }
  }

TRACK(TrackBak,"ChkGetChunk\n");
return(STAT);
}
 
void Mem_ShowStats       (void)
{
   TraceMsg (0, "\n\n         Env01 Memory Stats\n\n");

   TraceMsg (0, "  Chunk Manager Statistics::\n");

   sprintf 
         (Msg, "\n Chunks Allocated         :: %u\n",  ChunksAllocated);
   TraceMsg (0, Msg);

   sprintf (Msg, " Chunks Freeed            :: %u\n",  ChunksFreeed);
   TraceMsg (0, Msg);

   sprintf (Msg, " Chunks Reclaimed         :: %u\n\n",ChunksReclaimed);
   TraceMsg (0, Msg);

   TraceMsg (0, "\n Dump of Chunk ADDRESS'S   ::   ");
   DumpChunkChunk (0, ChunkAddrChunk);

   Mem_ShowXmemStats   ();

   TraceMsg (0, "\n");

return;
}

void Mem_DumpStats       (void)
{
numtype      ChunkNum     = 0;
indextype    StackPtr     = Stack_Ptr(ChunkAddrChunk);
sizetype     BlkSize      = 0;

   sprintf (Msg, "\n Dump of Chunk STAT'S      :: StackPtr=%6u\n", StackPtr);
   TraceMsg (0, Msg);
   TraceMsg (0, 
     "\n Chunk#  Address  DbId  UnitSize  UnitCount  StackPtr  BlkSize\n");
   for (ChunkNum = 0; (ChunkNum < StackPtr) && (ChunkNum < 200); ++ChunkNum)
   {
    if (Chunk_Addr(ChunkNum) != NullPtr)
    {
      BlkSize  = Unit_Count(ChunkNum) * Unit_Size(ChunkNum)
               + sizeof (chunkstruc);

      sprintf (Msg, " %6u %8x  %4u  %8u  %9u  %8d  %7u\n",
               ChunkNum, Chunk_Addr(ChunkNum), Next_Chunk(ChunkNum), 
               Unit_Size(ChunkNum),
               Unit_Count(ChunkNum), Stack_Ptr(ChunkNum), BlkSize);
      TraceMsg (0, Msg);
    }

   }
   TraceMsg (0, "\n");

return;
}

#ifdef     VALIDATE_MEM_CHUNKS
#ifdef     SYSTEM_ALLOC

numtype    Chunk_Validate
                         (numtype     ChunkNum)
{
numtype        CRCvalue       = 0;
sizetype       UnitSize       = Unit_Size  (ChunkNum);
numtype        UnitCount      = Unit_Count (ChunkNum);
addrtype       CoreAddr       = Chunk_Addr (ChunkNum);

sizetype       ByteAlignment  = 0;
farlongtype    FarSize        = 0;
addrtype       EndCoreBlkAddr = NullPtr;

    CoreAddr         = (addrtype )((char *)CoreAddr 
                     - (sizeof (chunkstruc)   + CORE_BLOCK_ALIGN));

    FarSize          = (UnitSize * UnitCount) + sizeof (chunkstruc)
                     + CORE_BLOCK_ALIGN       + 4;

    if (FarSize % CoreBlk_GapSpace)
    {
       ByteAlignment = CoreBlk_GapSpace - (FarSize % CoreBlk_GapSpace);
       FarSize      += ByteAlignment;
    }
    EndCoreBlkAddr   = (addrtype )((char  *)CoreAddr + (FarSize - 4));

    CRCvalue         = *(numtype *)EndCoreBlkAddr;

return(CRCvalue);
}
#endif /* SYSTEM_ALLOC */
#endif /* VALIDATE_MEM_CHUNKS  */

/*+-----------------------------------------------------------------------+
~M                         END OF MEM00.C                                 !
  +-----------------------------------------------------------------------+*/
