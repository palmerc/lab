/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - FILE.c */
#define FM
#include <stdio.h>
#include <unistd.h>
#include "defines.h"
#include "typedefs.h"
#include "files.h"
#include "core01.h"
#include "mem010.h"
#include "blk.h"
#include "fm.h"
 

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...FM... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[46] =
{
   "     FilePutSomeBytes  =%6u\n",
   "    PUT to   File @ Offset= %lu",
   ", size(%u) =",
   "  %u\n" ,
   "  %u\n" ,
   "  %x\n",
   "  \n",
   "     FileGetSomeBytes  =%8u\n",
   "     SEEK To Offset    =%8ld\n",
   "***FileGetSomeBytes... SeekStatus= %u\n",
   "***FileGetSomeBytes... CheckCount= %u\n",
   "     Get bytes from File @ Offset= %lu",
   ", Value(*%4u) =",
   "  %u\n" ,
   "  %u\n" ,
   "  %x\n" ,
   "  \n",
   "    FileOutBLock... Size=%6u\n",
   "     Seek To Offset= %lu\n" ,
   "    Write To FileBlk Offset= %8lu; From @%x\n",
   "\n  FileOutChunk ... Block[%3u] Not Loaded.\n" ,
   "    FileOutChunk    =%6u; @ %x\n" ,
   " *** ERROR ... @FileOutChunk count=%2u, size=%3u\n",
   "            As  Vbn =%6u\n" ,
   "\n      Expand VbnBlk =%6u... Size=%4u Units=%4u; StackPtr=%u\n",
   "           From... %8u --> %8u\n",
   "   ***Chunk[%4u]... ChangeIn Unit Size... %4u --> %4u\n",
   "      UnitSize= %3u, Count=%4u, StackPtr=%4u;\n",
   "    At file Offset= %8lu\n",
   "    At File Offset= %8lu (* %3u)\n",
   " *** ERROR... BadDiskWrite for Count=%u\n",
   "    FileOutChunk    =%6u; to Vbn[%4u]\n",
   "    FileInBlk...    Vbn =%6u;  Size  = %6u\n" ,
   "    New Region for Blk. @%6u\n",
   "    From       BlkIndex =%6u;  Offset= %8u \n",
   " *** ZERO File Offset for Vbn = %6u; BlkIndex= %6u;\n",
   "    Seek To Offset  =%8lu\n" ,
   "    Read InTo Core  @%8x\n",
   "  FILE read ERROR #  Vbn[%3u]\n",
   "    Load VbnBlk[%4u]\n" ,
   "    ReadInto Chunk[%6u] Unit Count=%6u, Size=%4u Stack=%6u\n",
   " ***ERROR... FileInChunk. Size %4u != BlkSize %4u\n",
   "    FILE IN From Blk[%3u] ZERO Chunk[%4u](%2u * %2u)\n",
   "    ZERO BLOCK [%3u] ZERO Chunk[%4u](%2u * %2u)\n",
   " File <%s>:  can't open  file\n",
   " File <%s>   Opened For COMMIT::\n"
};
#endif

#define MemGetVbn              Mem_GetWord
#define MemPutVbn              Mem_PutWord

#define MemGetChunkNum         Mem_GetWord
#define MemPutChunkNum         Mem_PutWord

#define MemPutIndex            Mem_PutWord
#define MemGetIndex            Mem_GetWord
#define MemPopIndex            Mem_PopWord
#define MemPushIndex           Mem_PushWord

static idtype   DbmDbId             = 0;

/* #define USE_BLK
*/
#ifdef USE_BLK
#define MemFreeFileBlk         BlkFreeFileBlk

#else
#define MemFreeFileBlk     Mem_FreeFileBlk
extern boolean MemFreeFileBlk (blkhdrtype   *BlkHdr,
                               ft F,lt Z,zz *Status,    vbntype      *Vbn);
#endif

/*+-----------------------------------------------------------------------+*
~FM                             FILE
 *+-----------------------------------------------------------------------+*/


boolean FileGetSomeBytes (blkhdrtype   *BlkHdr,     farlongtype  FileOffset,
                          unsigned      SomeBytes,
                          ft F,lt Z,zz *Status,     addrtype     BytesAddr)
{
size_t  CheckCount  = 0;
int     SeekStatus  = 0;

    SeekStatus = fseek (BlkHdr->BlkFile, FileOffset, SEEK_SET);
    if (SeekStatus != 0)
    {
        sprintf  (Msg, TraceStr(9),
/* "***FileGetSomeBytes... SeekStatus= %u\n" */
                  SeekStatus);
        TraceMsg  (0,   Msg);
       *Status = File_BadDiskWrite;
    }

    CheckCount = fread (BytesAddr,       SomeBytes,  (size_t )1,
                        BlkHdr->BlkFile);
    if (CheckCount != (int )1)
    {
        sprintf  (Msg, TraceStr(10),
/* "***FileGetSomeBytes... CheckCount= %u\n" */
                  CheckCount);
        TraceMsg  (0,   Msg);
       *Status = File_BadDiskWrite;
    }

    if (SomeBytes == 0)
       *Status = Err_NoBytesMoved;

TRACK(TrackBak,"FileGetSomeBytes\n");
return(STAT);
}
 

 
 
boolean FileInBlk       (addrtype     *BlkAddr, bytessize  BlkSize, 
                         blkhdrtype   *BlkHdr,  boolean    Release,
                         ft F,lt Z,zz *Status,  vbntype   *VbnBlk)
{
bytessize   UnitSize     = BlkSize;
indextype   UnitCount    = (indextype )1;
lt64        FileOffset   = 0;
indextype   VbnBlkIndex  = 0;
blktkntype  BlkTkn;
int         FileRead     = 0;

/*   This is a ChunkRef Chunk || a ObjHdr || a File Block                 */
  if (*BlkAddr      == NullPtr)
      CoreMoreCore    (BlkSize,              McStat,    BlkAddr);

  if (Normal(*Status))
  if (MemGetIndex     (BlkHdr->BlkDirChunk, *VbnBlk,    
                       McStat,              &VbnBlkIndex))
      MemGetSomeBytes (BlkHdr->BlkTknChunk,  VbnBlkIndex, 
                       sizeof (blktkntype),
                       McStat,   (addrtype )&BlkTkn);

  if (Normal(*Status))
  {  
     FileOffset    = BlkTkn.Offset;

     if (FileOffset <= 0)
     {
        sprintf (Msg, TraceStr(35),
/* " *** ZERO File Offset for Vbn = %6u; BlkIndex= %6u;\n" */
                *VbnBlk, VbnBlkIndex);
        TraceMsg (0, Msg);
     }
  }

/* Copy from File space to CoreSpace                                       */
  if (Normal(*Status))
  {
     fseek            (BlkHdr->BlkFile,     FileOffset,     SEEK_SET);
     FileRead = fread (*BlkAddr,                 (unsigned )UnitSize, 
                       (unsigned )UnitCount,                BlkHdr->BlkFile);

     if (FileRead != (int )UnitCount)
     {
         sprintf (Msg, TraceStr(38),
/* "  FILE read ERROR #  Vbn[%3u]\n" */
                  *VbnBlk);
         TraceMsg (0, Msg);
         perror("    Error Msg ");
        *Status = File_BadDiskRead;

     }
     LastFileOffset = FileOffset + UnitCount;
  }

  if (Normal(*Status))
  if (Release)
      MemFreeFileBlk   (BlkHdr,     McStat,   VbnBlk);

TRACK(TrackBak,"FileInBlk\n");
return(STAT);
}

boolean FileInChunk     (numtype      *Chunk,    blkhdrtype *BlkHdr,
                         boolean       Release,
                         ft F,lt Z,zz *Status,   vbntype    *VbnBlk)
{
addrtype    ChunkAddr    = NullPtr;
bytessize   UnitSize     = 0;
indextype   UnitCount    = 0;
sizetype    ChunkSize    = 0;
sizetype    BlkSize      = 0;
chunkstruc  ChunkRef; 
chunkstruc *ChunkRefPtr  = &ChunkRef;
addrtype    ChunkBlk     = NullPtr;
indextype   BlkIndex     = 0;

  if (MemGetIndex       (BlkHdr->BlkDirChunk,        *VbnBlk,
                         McStat,                     &BlkIndex))
  if (MemGetWord        (BlkHdr->BlkSizeChunk,        BlkIndex, 
                         McStat,                     &BlkSize))
  if (!*Chunk)
      MemNewChunkBlk    (BlkSize,  McStat,    Chunk, &ChunkBlk);
  else {
     if (MemGetChunkAddr   (*Chunk,   McStat,    &ChunkAddr))
        ChunkBlk  = (addrtype )((char *)ChunkAddr - sizeof (chunkstruc));
  }

  if (Normal(*Status))
  if (FileInBlk (&ChunkBlk,   BlkSize,           
                 BlkHdr,      False,       McStat,    VbnBlk))
  {
     ChunkRefPtr = (chunkstruc *)ChunkBlk;
     UnitSize    = ChunkRefPtr->UnitSize;
     UnitCount   = ChunkRefPtr->UnitCount;

     MemPutChunkStruc   (*Chunk,  (chunkstruc *)ChunkBlk,
                         DbmDbId,               McStat);

     ChunkSize   = UnitSize * UnitCount;
     if (ChunkSize != BlkSize - sizeof (chunkstruc))
     if (sprintf (Msg, TraceStr(41),
/* " ***ERROR... FileInChunk. Size %4u != BlkSize %4u\n" */
                  ChunkSize, BlkSize))
       TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (Release)
      MemFreeFileBlk (BlkHdr,             McStat,        VbnBlk);

TRACK(TrackBak,"FileInChunk\n");
return(STAT);
}
 

boolean FileGetZeroedChunk (vbntype       VbnBlk,    blkhdrtype *BlkHdr, 
                            boolean       Release,   
                            ft F,lt Z,zz *Status,    numtype    *Chunk)  
{
bytessize   UnitSize     = 0;
indextype   UnitCount    = 0;
bytessize   ChunkSize    = 0;
vbntype     ChunkRefVbn  = VbnBlk;
chunkstruc  ChunkRef; 
chunkstruc *ChunkRefPtr  = &ChunkRef;
addrtype    ChunkAddr    = NullPtr;

  /* Load Chunk Struc Into Core                                            */
  if (FileInBlk ((addrtype *)&ChunkRefPtr,              sizeof(chunkstruc),
                 BlkHdr,      Release,     McStat,     &ChunkRefVbn))
  {
     UnitSize  = ChunkRefPtr->UnitSize;
     UnitCount = ChunkRefPtr->UnitCount;
     ChunkSize = UnitCount * UnitSize;

     if (!*Chunk)
        MemMakeChunk (UnitSize,  UnitCount, ChunkRef.ExtendQty,
                      DbmDbId,   McStat,    Chunk);

     if (Normal(*Status))
        MemPutStackPtr (*Chunk, ChunkRef.StackPtr, McStat);


     if (!ChunkSize)
     if (sprintf (Msg, TraceStr(43),
/* "    ZERO BLOCK [%3u] ZERO Chunk[%4u](%2u * %2u)\n" */
                  VbnBlk, *Chunk, UnitCount, UnitSize))
        TraceMsg (0, Msg);

     if (MemGetChunkAddr (*Chunk,     McStat,      &ChunkAddr))
        memset           (ChunkAddr,  NULL,    (size_t )ChunkSize);
  }

  if (Normal(*Status))
  if (DeBug)
      MemDumpChunk   (0,                 *Chunk,         McStat);

TRACK(TrackBak,"FileGetZeroedChunk\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+
~P                         END OF FM.C                                    !
  +-----------------------------------------------------------------------+*/
