/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#define MEM_C
#define MEM01
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem00.h"
#include "mem01.h"


#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[2] = {"\0", "   ...MEM01... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char   *TraceStr[53] = 
{
   " *** ERROR ... MemMakeChunk ZERO(0) UnitSize=%u, AllocQty=%u\n",
   "  New TextPtrChunk[%6u];\n",
   "  StrucHdr::TextChunk = %3u;\n",
   "      Value[%4u]= ",
   "        Mem  FreeChunk       [%6u]; 2nd Free=%6u\n",
   "        Mem  FreeChunkNum    [%6u]; 2nd Free=%6u\n",
   "     CLEAR Chunk[%4u] @%x * Bytes= %6u\n",
   "        Mem  FreeChunk    [%6u]  Bytes =%8u  @%x\n",
   "    SwapOUT Chunk[%6u](*%4u) Units= %6u @%8x; Vbn=%6u\n",
   "      TO              BlkIndex %6u; BlkOffset= %8u\n",
   "      Seek To File Offset    = %8ld\n",
   "      Write From Chunk       @ %8x\n",
   "          From Chunk           [%6u](* %4u) units= %6u\n",
   "    SwapIN  Chunk[%6u](*%4u); Vbn=%6u\n",
   "      FROM Vbn as     BlkIndex %6u; BlkOffset= %8u\n",
   "      Seek To File Offset    = %8ld\n",
   "      Read  Into Chunk       @ %8x\n",
   "     COPY From Vbn[%6u] XmemBlk[%6u] Offset= %8ld\n",
   "      TO   Chunk[%6u]\n",
   "\n    CopyOUT Chunk[%6u](*%4u) Units= %6u @%8x; Blk Size=%4u\n",
   "      TO   Vbn[%6u] = BlkIndex %6u; BlkOffset= %8u\n",
   "     SEEK To File Offset    = %8ld\n",
   "     COPY To  Vbn[%3u] Offset= %ld \n",
   "     FROM Chunk(%3u) Base address @%8x; for %6u bytes\n",
   "\n    CopyIN  Chunk[%6u];  From Vbn= %6u To SwapAddr @%6x\n",
   "***ERROr...MemCopyInChunk Chunk[%3u] Swap Vbn= 0\n",
   "      FROM Vbn[%6u] = BlkIndex %6u; BlkOffset= %8u; BlkSize=%4u\n",
   "     SEEK To File Offset    = %8ld\n",
   "     CopyIN  From XmemBlk[%4u]  Offset= %6ld\n",
   "     TO   Chunk(%3u) %4u units * %2u bytes\n",
   "      MemFreeFileBlk referenced by Vbn = %6u\n",
   "      MemGetFileBlk for CUR Vbn = %6u @Offset= %8u\n",
   "      MemGetFileBlk for NEW Vbn = %6u @Offset= %8u\n",
   "          PrePend ",
   "          PostPend",
   "Blk[%4u]  Size     =%8lu;\n",
   "          To  GapBlk [%4u]  New Size =%8lu; @%8x\n",
   "          Insert Blk=%6u Into NullBlks List; Next= %6u\n",
   "          Delete Blk=%6u From FreeBlks List; Prev= %6u\n",
   "          Insert Blk=%6u Into NullBlks List; Next= %6u\n",
   "  NumFreeBlks In ReGap= %4u\n",
   "          Insert Blk=%6u Into FreeBlks List; Next= %6u\n",
   "          Insert Vbn=%6u Into FreeVbns List; Next= %6u\n",
   "      New      Vbn  =%6u; BlkIndex=%6u @FileOffset =%8u\n",
   "      Get Free Vbn  =%6u\n",
   "      Get New  Vbn  =%6u\n",
   "      ReClaim EqualSized FreeBlk= %6u, Prev=%6u\n",
   "  NumFreeBlks In Search = %4u\n",
   "      Get NULL Blk  =%6u\n",
   "      Get New  Blk  =%6u\n",
   "      ReGap Blk     =%6u   Offset   @%8u Tosize=%6u\n",
   "      Increment Eof =%6u  To %6u\n",
   "     TO   Chunk(%3u) Base address @%8x; for %6u bytes\n"
};
#endif
#define MemGetChunkAddr        Mem_GetChunkAddr
#define MemPutStackPtr         Mem_PutStackPtr 
#define MemGetStackPtr         Mem_GetStackPtr 
#define MemPushWord            Mem_PushWord    
#define MemPutWord             Mem_PutWord     
#define MemPopWord             Mem_PopWord     
#define MemGetWord             Mem_GetWord     
#define MemGetSomeBytes        Mem_GetSomeBytes

       boolean MemGetChunkAddr(numtype       Chunk,
                               ft F,lt Z,zz *Status,    addrtype *ChunkAddr);

       boolean MemPutStackPtr (numtype       Chunk,     indextype StackPtr,
                               ft F,lt Z,zz *Status);

       boolean MemGetStackPtr (numtype       Chunk,
                               ft F,lt Z,zz *Status,    indextype *StackPtr);

       boolean MemPushWord    (numtype       Chunk,    word       WordValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemGetWord     (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   word      *WordValue);

       boolean MemGetSomeBytes(numtype       Chunk,     indextype   Index,
                               bytessize     ByteCount,
                               ft F,lt Z,zz *Status,    addrtype    SomeBytes);

       boolean NewRegion      (sizeinblocks  Blocks,
                               ft F,lt Z,zz *Status,    addrtype *Region);


#define MemPutIndex         Mem_PutWord
#define MemGetIndex         Mem_GetWord
#define MemPopIndex         Mem_PopWord
#define MemPushIndex        Mem_PushWord

/*+-----------------------------------------------------------------------+*/
 
boolean    MemMakeChunk   (bytessize     UnitSize,  indextype   AllocQty,
                           indextype     ExtendQty, idtype      CoreDbId,
                           ft F,lt Z,zz *Status,    numtype    *Chunk)
{
boolean  VaryLength  = False;
boolean  PtrChunk    = False;

  if (!UnitSize || !AllocQty)
  {
     *Status = Err_BadUnitSize;
  }

  if (Normal(*Status))
  if (MakeChunk         (VaryLength, PtrChunk, UnitSize, AllocQty, ExtendQty,
                         CoreDbId,   McStat,   Chunk))
  {
     IsNumeric_Chunk    (*Chunk);
     IsData_Chunk       (*Chunk);
  }

TRACK(TrackBak,"MemMakeChunk\n");
return(STAT);
}
 
boolean MemMakeCppChunk (bytessize     CppObjSize,   indextype   AllocQty,
                         indextype     ExtendQty,    idtype      CoreDbId,
                         ft F,lt Z,zz *Status,       numtype    *Chunk)
{
boolean  VaryLength  = False;
boolean  PtrChunk    = True;

  MakeChunk   (VaryLength, PtrChunk,  CppObjSize,  AllocQty,  ExtendQty,
               CoreDbId,   McStat,    Chunk);

TRACK(TrackBak,"MemMakeCppChunk\n");
return(STAT);
}
 
boolean MemMakeStrChunk (bytessize     MaxLength, indextype   NumStrings,
                         indextype     ExtendQty, bytessize   TextLength,
                         idtype        CoreDbId,
                         ft F,lt Z,zz *Status,    numtype    *StrChunk)
{
boolean    VaryLength  = False;
boolean    PtrChunk    = True;
numtype    TextChunk   = 0;
struchdr  *StrucHdr    = NullPtr;


  if (MakeChunk (VaryLength,   PtrChunk,  sizeof (stringstruc),  NumStrings, 
                 ExtendQty,    CoreDbId,  McStat,                StrChunk))

  VaryLength  = True;
  PtrChunk    = False;
  if (Normal(*Status)
  &&  *StrChunk)
  if (MakeChunk (VaryLength,   PtrChunk,  sizeof (char),         TextLength, 
                 ExtendQty*32, CoreDbId,  McStat,               &TextChunk))
  {
     Extend_Qty      (TextChunk)  = ExtendQty * 32;
     MaxUnit_Length  (TextChunk)  = MaxLength;

     IsData_Chunk    (*StrChunk);

     StrucHdr                     = (struchdr *)Chunk_Addr (*StrChunk);
     StrucHdr->TextChunk          = TextChunk;
     StrucHdr->TextVbn            = 0;
     StrucHdr->FirstFreeGap       = 0;
     Stack_Ptr         (*StrChunk)++;

  }

TRACK(TrackBak,"MemMakeStrChunk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean MemDumpChunk  (int           MsgLevel,    numtype   Chunk,
                       ft F,lt Z,zz *Status)
{
  if (!Normal(*Status))
      DumpChunkChunk (0,        Chunk);
  else
      DumpChunkChunk (MsgLevel, Chunk);
 
TRACK(TrackBak,"MemDumpChunk\n");
return(STAT);
}

boolean Mem_DumpChunkPart
                      (int           MsgLevel,    numtype       Chunk,
                       numtype       Ilow,        numtype       Ihigh,
                       ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"MemDumpChunkPart\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean    MemFreeChunk   (numtype      *Chunk,  ft F,lt Z,zz *Status)
{

   if (*Chunk)
   if (MemClearChunkSpace (*Chunk,  McStat))
   if (MemFreeChunkSpace  (*Chunk,  McStat))
   {
      ChunksFreeed++;
      Next_Chunk(*Chunk)  = Theory->NextFree;
      Theory->NextFree    = *Chunk;

     *Chunk               = 0;

   }

TRACK(TrackBak,"MemFreeChunk\n");
return (STAT);
}

boolean    MemFreeChunkNum
                          (numtype      *Chunk,  ft F,lt Z,zz *Status)
{
   if (*Chunk)
   {
      ChunksFreeed++;
      Next_Chunk(*Chunk)  = Theory->NextFree;
      Theory->NextFree    = *Chunk;

      Chunk_Addr(*Chunk)  = NullPtr;
     *Chunk               = 0;
   }

TRACK(TrackBak,"MemFreeChunkNum\n");
return (STAT);
}

boolean MemClearChunkSpace (numtype    Chunk,     ft F,lt Z,zz *Status)
{
indextype   UnitCount = Unit_Count(Chunk);
bytessize   UnitSize  = Unit_Size(Chunk);
addrtype    ChunkAddr = Chunk_Addr(Chunk);
bytessize   ChunkSize = 0;

   Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk, McStat))
   {
      ChunkSize = UnitSize * (bytessize )UnitCount;
   }            

TRACK(TrackBak,"MemClearChunkSpace\n");
return(STAT);
}
 
boolean MemFreeChunkSpace (numtype      Chunk,  ft F,lt Z,zz *Status)
{
indextype   UnitCount = Unit_Count   (Chunk);
bytessize   UnitSize  = Unit_Size    (Chunk);
addrtype    ChunkAddr = Chunk_Addr   (Chunk);
bytessize   ChunkSize = UnitSize    * (bytessize )UnitCount;

  ChunkSize            += sizeof (chunkstruc);
  ChunkAddr             = (addrtype )((char *)ChunkAddr
                                              - sizeof (chunkstruc));

  CoreFreeCoreSpace       (&ChunkAddr,  ChunkSize,  McStat);
  Chunk_Addr  (Chunk)   = NullPtr;

  if (!Normal(*Status))
    DumpChunkChunk (0, Chunk);

TRACK(TrackBak,"MemFreeChunkSpace\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*                      Swap Chunk for ChunkManager                        */
/*+-----------------------------------------------------------------------+*/
boolean MemSwapOutChunk (numtype    Chunk,    ft F,lt Z,zz *Status,
                         vbntype   *Vbn)
{
 
TRACK(TrackBak,"MemSwapOutChunk\n");
return(STAT);
}
 
boolean MemSwapInChunk  (numtype       Chunk,  
                         ft F,lt Z,zz *Status, vbntype  *Vbn)
{
 
TRACK(TrackBak,"MemSwapInChunk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*                      Swap Chunk OverLays for File Manager               */
/*+-----------------------------------------------------------------------+*/
boolean MemCopyOutChunk (numtype    Chunk,    ft F,lt Z,zz *Status,
                         vbntype   *SwapVbn)
{
 
TRACK(TrackBak,"MemCopyOutChunk\n");
return(STAT);
}
 
boolean MemCopyInChunk  (numtype       Chunk,      numtype       SwapVbn,
                         addrtype      SwapAddr,   ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"MemCopyInChunk\n");
return(STAT);
}
 

boolean Mem_MoveChunk   (numtype       ChunkNum,  ft F,lt Z,zz *Status,  
                         addrtype     *FreeAddr,  sizeinbytes  *FreeSize)
{
addrtype      Source         = NullPtr;
addrtype      SourceRegion   = NullPtr;
sizeinbytes   SourceSize     = 0;
addrtype      TargetRegion   = NullPtr;
addrtype     *ChunkAddrPtr   = NullPtr;

 *FreeAddr      =  NullPtr;
 *FreeSize      =  0;

  if (DbmBug)
  if (TraceMsg (0, " BEFORE move ::    "))
      Mem_DumpChunkChunk (0, ChunkNum);

  SourceRegion  = Chunk_Addr (ChunkNum);
  SourceRegion  = (addrtype )((char *)SourceRegion - sizeof (chunkstruc));
  Source        = SourceRegion;

  SourceSize   = Unit_Size  (ChunkNum) *  Unit_Count (ChunkNum);
  SourceSize  += sizeof (chunkstruc);

  if (Normal(*Status))
  {
/*   This is a Normal Chunk Expansion                                      */
     if (NewRegion   (SourceSize,      McStat,     (addrtype *)&ChunkAddrPtr))
     {
        TargetRegion = ChunkAddrPtr;

        if (DbmBug || MemBug)
        {
           sprintf (Msg, TraceStr(41),
/* "     Copy from Source(* %8u) @%x TO Target @%x\n" */
                    SourceSize, SourceRegion, TargetRegion);
           TraceMsg (0, Msg);
        }

        memcpy  ((char *)ChunkAddrPtr, (char *)SourceRegion, SourceSize);

/*      Adjust for the chunkstruc Header Block                            */
        TargetRegion  = (addrtype )((char *)TargetRegion
                                          + sizeof (chunkstruc));
        ChunkAddrPtr  = (addrtype *)TargetRegion;

/*      Set the Chunk Stats on the New Chunk                              */
        Chunk_Addr(ChunkNum)    = TargetRegion;

        if (DbmBug)
        if (TraceMsg (0, " AFTER  move ::    "))
           Mem_DumpChunkChunk (0, ChunkNum);
     }

    *FreeAddr      =  Source;
    *FreeSize      =  SourceSize;

     if (DbmBug)
     {
        sprintf (Msg, 
          " Mem_MoveChunk [%6u] :: TO Target @%8x; Size :=%6u,  From @%8x.\n",
                 ChunkNum, TargetRegion, SourceSize, SourceRegion);
        TraceMsg (0, Msg);
     }
  }

TRACK(TrackBak,"Mem_MoveChunk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*
~MFM             Memory Manager Xmem BlkHdr
 *+-----------------------------------------------------------------------+*/
boolean    Mem_MakeXmemChunks 
                         (indextype     AllocQty,  indextype     ExtendQty,
                          ft F,lt Z,zz *Status)
{
/* Create The VirtualFile ChunkChunks                                      */

   if (MemMakeChunk   (sizeof(indextype),      AllocQty, 
                       ExtendQty,              0,
                       McStat,                &XmemBlkDirChunk))
   if (MemPutStackPtr (XmemBlkDirChunk,        1,                   McStat))
   if (MemMakeChunk   (sizeof(blktkntype),     AllocQty, 
                       ExtendQty,              0,
                       McStat,                &XmemBlkTknChunk))
   if (MemPutStackPtr (XmemBlkTknChunk,        1,                   McStat))

   if (MemMakeChunk   (sizeof(bytessize ),     AllocQty,
                       ExtendQty,              0,
                       McStat,                &XmemBlkSizeChunk))
   if (MemPutStackPtr (XmemBlkSizeChunk,       1,                   McStat))
   {
      XmemSize         = 0;

      MemBlkFile       = XmemFilePtr;

      MemBlkDirChunk   = XmemBlkDirChunk;
      MemBlkTknChunk   = XmemBlkTknChunk;
      MemBlkSizeChunk  = XmemBlkSizeChunk;

      XmemHeadFreeVbns = 
       MemHeadFreeVbns = 0;

      XmemHeadFreeBlks =
       MemHeadFreeBlks = 0;

      XmemHeadNullBlks =
       MemHeadNullBlks = 0;

      XmemBlkDir       =
       MemBlkDir       = (indextype  *)Theory->ChunkAddr[XmemBlkDirChunk];

      XmemBlkTkn       =
       MemBlkTkn       = (blktkntype *)Theory->ChunkAddr[XmemBlkTknChunk];

      XmemBlkSize      =
       MemBlkSize      = (bytessize  *)Theory->ChunkAddr[XmemBlkSizeChunk];
   }

   if (DeBug)
      DumpChunkChunk (1, XmemBlkDirChunk);

TRACK(TrackBak,"  strcpy (Mem_MakeXmemChunks\n");
return (STAT);
}


void    Mem_AssignXmemFile    (char         *Mem_File)
{
/* Open The VirtualFile Of The Memory Manager                            */

   if (strlen  (Mem_File) == 0 || !strcmp( Mem_File , "NULL") )
   {
      SWAP   = False;

   } else if ((XmemFilePtr = fopen(Mem_File, "w+b")) == NULL) {
      fprintf (stderr, "%s:  can't open Xmem_File\n", Mem_File);
      SendMsg (0, "***WARNING... Swap Chunks will be suppressed\n"); 
      SWAP   = False;

   } else {
      if (FREEZE_GRP_PACKETS == False)
         SWAP   = True;

#ifdef NO_DISKCACHE
       if (setvbuf (XmemFilePtr, NULL, _IONBF, 0))
       SendMsg (0, "***ERROR... setvbuf() function failed\n"); 
#endif

   }

return;
}

void    Mem_CloseXmemFile (void)
{
  fclose (XmemFilePtr);
return;
}

/*+-----------------------------------------------------------------------+*
~MFM             Virtual FileBlock Xmem Interface
 *+-----------------------------------------------------------------------+*/
 
boolean MemGetFileBlk   (bytessize     SizeInBytes, blkhdrtype *BlkHdr,
                         ft F,lt Z,zz *Status,
                         vbntype      *Vbn,         lt64       *FileOffset)
{
indextype   BlkIndex    = 0;
blktkntype  BlkTkn;
lt64        TmpXmemSize = XmemSize;

 *FileOffset            = 0;

  XmemFilePtr           = BlkHdr->BlkFile;
  XmemSize              = BlkHdr->EofMark;

  XmemHeadFreeVbns      = BlkHdr->HeadFreeVbns;
  XmemBlkDirChunk       = BlkHdr->BlkDirChunk;
  XmemHeadFreeBlks      = BlkHdr->HeadFreeBlks;
  XmemHeadNullBlks      = BlkHdr->HeadNullBlks;
  XmemBlkTknChunk       = BlkHdr->BlkTknChunk;
  XmemBlkSizeChunk      = BlkHdr->BlkSizeChunk;

  if (MemGetChunkAddr    (BlkHdr->BlkDirChunk,
                          McStat,                   (addrtype *)&XmemBlkDir))
  if (MemGetChunkAddr    (BlkHdr->BlkTknChunk,
                          McStat,                   (addrtype *)&XmemBlkTkn))
  if (MemGetChunkAddr    (BlkHdr->BlkSizeChunk,
                          McStat,                   (addrtype *)&XmemBlkSize))

  if (*Vbn)
  {
     if (MemGetIndex     (XmemBlkDirChunk,  *Vbn,      McStat,  &BlkIndex))
     if (MemGetSomeBytes (XmemBlkTknChunk,   BlkIndex,   sizeof (blktkntype),
                          McStat,                    (addrtype )&BlkTkn))
        *FileOffset     = BlkTkn.Offset;

  } else {
     MemNewXmemBlk   (SizeInBytes,       McStat,   Vbn,      FileOffset);
  }

  BlkHdr->HeadFreeVbns  = XmemHeadFreeVbns;
  BlkHdr->HeadFreeBlks  = XmemHeadFreeBlks;
  BlkHdr->HeadNullBlks  = XmemHeadNullBlks;

  BlkHdr->EofMark       = XmemSize;
  XmemSize              = TmpXmemSize;
  XmemFilePtr           = MemBlkFile;
  XmemHeadFreeVbns      = MemHeadFreeVbns;
  XmemBlkDirChunk       = MemBlkDirChunk;
  XmemHeadFreeBlks      = MemHeadFreeBlks;
  XmemHeadNullBlks      = MemHeadNullBlks;
  XmemBlkTknChunk       = MemBlkTknChunk;
  XmemBlkSizeChunk      = MemBlkSizeChunk;
  XmemBlkDir            = MemBlkDir;
  XmemBlkTkn            = MemBlkTkn;
  XmemBlkSize           = MemBlkSize;

TRACK(TrackBak,"MemGetFileBlk\n");
return(STAT);
}


boolean MemFreeFileBlk  (blkhdrtype   *BlkHdr,
                         ft F,lt Z,zz *Status,   vbntype      *Vbn)
{
  *Status  = Err_NotImplemented;


TRACK(TrackBak,"MemFreeFileBlk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*                      Virtual Chunk Disk/Core Moves                      */
/*+-----------------------------------------------------------------------+*/
boolean MemNewXmemBlk (bytessize   SizeInBytes,  ft F,lt Z,zz *Status,  
                       vbntype    *Vbn,          lt64         *XmemOffset)
{
  *Status  = Err_NotImplemented;


TRACK(TrackBak,"MemNewXmemBlk\n");
return(STAT);
}
 

boolean MemFreeXmemBlk  (ft F,lt Z,zz *Status,    vbntype    *Vbn)
{
  *Status  = Err_NotImplemented;


TRACK(TrackBak,"MemFreeXmemBlk\n");
return(STAT);
}
 

 
void Mem_ShowXmemStats       (void)
{
   TraceMsg (0, " Dump of XmemBlkSizeChunk  ::    ");
   DumpChunkChunk (0, XmemBlkSizeChunk);

   TraceMsg (0, "\n Dump of XmemBlkDirChunk   ::    ");
   DumpChunkChunk (0, XmemBlkDirChunk);

return;
}

/*+-----------------------------------------------------------------------+
~M                         END OF MEM01.C                                 !
  +-----------------------------------------------------------------------+*/
