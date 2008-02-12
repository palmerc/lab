/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - CORE.c */

#define CORE
#include <math.h>
#ifdef FAR_MEM
#include <alloc.h>
#endif
#include "defines.h"
#include "typedefs.h"
#include "core01.h"

#ifdef DEBUG_CORE0
static   numtype       Core0_Chunk                = 0;

#endif

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[2] = {"\0", "   ...CORE... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char   *TraceStr[90] =
{
   "    Alloc from the VOID %8lu bytes;\n",
   "  *** WARNING ... Partial Core Page = %lu\n",
   "            Extend VOID %8lu bytes;  New Void =%8lu\n",
   "    VOID Extended:: Void Range (* %-8lu) @ %8x :: %8x\n",
   " CoreMoreCore... Non Aligned Address= %8x\n",
   "                 Core1Size          = %8lu\n",
   "    VOID FreeCore:: Void Range (* %-8lu) @ %8x :: %8x\n",
   " Allocated %6u bytes @%x\n",
   " Allocated %6u bytes @%x\n",
   " De Allocate block[%2u] @ %x\n",
   "  *** INFO *** Far Core Available = %lu\n",
   "    CoreMoreCore ::  Size= %6lu; Alignment =%u\n",
   "    CoreMoreCore ::  Request=%8lu > PageSize=%8lu\n",
   "\n CORE REQUEST for SizeInBytes : %lu > CoreBlock1Left %lu\n",
   "  Core required : %lu >  Current Core Size : %lu\n",
   "      Page[%3u] FreeSpace =%8lu   @%x.\n",
   "   FAR CORE AVAIL   : %lu\n",
   "             BASE   : %x\n",
   "           OFFSET   : %x\n",
   "        CURR SIZE   : %lu; BLOCKS[%u]\n",
   "          REQUEST   : %lu\n",
   "     MEM    AVAIL   : %lu\n",
   " CORE NO MORE  for Core1BlockElems= %lu EXTRA\n",
   "  CoreBlock1 Address Aligned by bytes= %2u\n",
   "     CORE    BASE   : @ %x\n",
   "     CORE1   SIZE   : %lu\n",
   "        Extend  Core Size   : %8lu; PageBLOCK[%u]\n",
   "        For     Request     : %8lu;\n",
   "                Core1 Size  : %8lu  @%8x\n\n",
   "     MEM    AVAIL   : %lu\n",
   "     New Region[%6u] * %4u @ %x\n",
   " CoreMoreCore... Non Aligned Address= %8x\n",
   "                 Core1Size          = %8lu\n",
   "        Add  FreeCoreGap[%6u] size = %8lu; @%8x\n",
   "        GapAddr[%6u] @%x, Size = %8lu; EndGapAddr @%x\n",
   "          Append FreeSpace[%4u]      = %8lu;\n",
   "         NewGapSize                   =%8lu; EndGapAddr   @%x\n",
   "          Pend   FreeSpace[%4u]      = %8lu;\n",
   "         NewGapSize                   =%8lu; StartGapAddr @%x\n",
   "        FreeStack      Units          = %8lu\n",
   "        Core ReGaped:: Count = %6u, SizeMoved= %6lu\n",
   "        Core Spliced:: NewGapSize     = %8lu\n",
   "        Reclaim FreeSpace [%6u]  Bytes =%8lu; @ %x Size= %6u\n",
   "        ReGap   FreeBlk   [%6u] To @%8x For %6u bytes\n",
   "        Append  FreeSpace[%6u]\n",
   "        Reclaim CoreSpace (*%-6lu)  @%x\n",
   "      NO Free Space for Bytes =%lu; StackPtr = %2u\n",
   " CoreGetFreeSpace... Non Aligned Address= %8x\n",
   "        Reclaim From Page[%6u] =   %8lu;  FreeSize =%8lu @%x\n",
   " CoreGetPageSpace... Non Aligned Address= %8x\n",
   "        Reclaim FreeSpace [%6u]  Bytes =%8lu; @ %x Size= %6u\n",
   "        ReGap   FreeBlk   [%6u] To @%8x For %6u bytes\n",
   "        Append  FreeSpace[%6u]\n",
   "        Reclaim CoreSpace (> )  %lu\n",
   "      NO Free Space for Bytes =%lu; StackPtr = %2u\n",
   " CoreGetGapSpace... Non Aligned Address= %8x\n",
   "     FreeStack Size= %6lu  Units; @ %x\n",
   "       GapAddr[%5u] @%x,  Size = %8lu; EndGapAddr @%x\n",
   "         Append FreeSpace[%4u] =%8lu;\n",
   "         NewGapSize                   =%8lu; EndGapAddr @%x\n",
   "         Pend   FreeSpace[%4u] =%8lu;\n",
   "         NewGapSize                    =%8lu; StartGapAddr @%x\n",
   "         Core(* %8u)  ReGaped:: Count   = %6u, ReGapSize= %6u\n",
   "         FreeAddr's ReGaped:: StackPtr=%6u, New      = %6u\n",
   "     FreeStack Size= %6lu  Units;\n",
   "      FreeAddr's ReGaped:: StackPtr=%6u, New      = %6u\n",
   "    CoreMoveBytes(%u) @%x to %x NewSize(%u)\n",
   " Void      Allocated :: %8lu\n",
   " Void   De Allocated :: %8lu\n",
   " Void   Used         :: %8lu\n",
   " Blks      Allocated :: %8lu\n",
   " Blks   De Allocated :: %8lu\n\n",
   " Regions  Allocated  :: %8u\n",
   " Core   Used         :: %8lu\n",
   " Core   Freeed       :: %8lu\n",
   " Core   Reclaimed    :: %8lu\n\n",
   " CoreBlks Requested  :: %8lu\n",
   " CoreBlks Freeed     :: %8lu\n\n",
   " CoreBlks Reclaimed  :: %8lu\n",
   "     By   Exact Size :: %8lu\n",
   "     By   Gaps       :: %8lu\n",
   "     By   Page Space :: %8lu\n\n",
   "       %6u Bytes moved from %x to %x ",
   " %4u\n",
   " %4u\n",
   "<[%4lu:%4lu]> || <%6xX : %6xX>\n",
   "<%4ld>  || <%6xX>\n",
   "       %u Bytes compared testvalue= %u \n",
   " Error... Duplicate Address Free'ed  GapAddr @%x, Size = %8lu;\n",
   "       Current GapSpace[%4u]  StartAddr @%x, EndAddr @%x; Size = %8lu;\n"
   };
#endif

#define DumpChunkChunk     Mem_DumpChunkChunk
#define MemDumpChunk       Mem_DumpChunk
#define MemPutStackPtr     Mem_PutStackPtr
#define MemGetStackPtr     Mem_GetStackPtr
#define MemGetChunkAddr    Mem_GetChunkAddr
#define MemPushAddr        Mem_PushAddr
#define MemPushWord        Mem_PushWord
#define MemGetSomeBytes    Mem_GetSomeBytes
extern void    DumpChunkChunk (int           Dump,      numtype   ChunkNum);

extern boolean MemDumpChunk   (int           MsgLevel,  numtype   Chunk,
                               ft F,lt Z,zz *Status);

extern boolean Mem_DumpChunkPart
                              (int           MsgLevel,  numtype       Chunk,
                               numtype       Ilow,      numtype       Ihigh,
                               ft F,lt Z,zz *Status);

extern boolean Mem_ExpandKrnlChunk
                              (chunknum      ChunkNum,
                               ft F,lt Z,zz *Status,   addrtype    *KrnlAddr);

extern boolean MemGetStackPtr (numtype       Chunk,
                               ft F,lt Z,zz *Status,  indextype    *StackPtr);

extern boolean MemPutStackPtr (numtype       Chunk,   indextype     StackPtr,
                               ft F,lt Z,zz *Status);

extern boolean MemGetChunkAddr(numtype       Chunk,
                               ft F,lt Z,zz *Status,  addrtype     *ChunkAddr);

extern boolean MemPushWord    (numtype       Chunk,   word          WordValue,
                               ft F,lt Z,zz *Status,  indextype    *StackPtr);

extern boolean MemPushAddr    (numtype       Chunk,   addrtype      Addr,
                               ft F,lt Z,zz *Status,  indextype    *StackPtr);

extern boolean Mem_MakeChunk
                              (bytessize     UnitSize,indextype     AllocQty,
                               indextype     ExtendQty, idtype      CoreDbId,
                               ft F,lt Z,zz *Status,  numtype      *Chunk);

#ifdef FAR_MEM
extern void far * _Cdecl farcalloc
                               (unsigned lt64 Blocks, unsigned lt64 Bytes);
extern void       _Cdecl farfree
                               (void far    *ptr);
#else
extern void    *calloc         (SIZE_T       Blocks, SIZE_T      Bytes);
extern void     free           (void        *ptr);
#endif

static  statustype   VoidStatus  = Env_Normal;

/*+-----------------------------------------------------------------------+
~VM                         VOID MANAGER                                  !
  +-----------------------------------------------------------------------+*/

void       Core_SetPageSize
                          (farlongtype    PageSize,  farlongtype MaxEntrySize)
{
   Core_PageSize     = PageSize;
   Core_MaxEntrySize = MaxEntrySize;

   Core1BlockElems   = Core_PageSize;

return;
}


addrtype   VoidExtendCore (farlongtype   *SizeInBytes)
{
lt64          *LongPtr        = NullPtr;
addrtype       CoreAddr       = NullPtr;
addrtype       EndCoreBlkAddr = NullPtr;
sizetype       ByteAlignment  = 0;
farlongtype    FarSize        = 0;
static
boolean        FirstTime      = False;
static
boolean        FirstLimit     = True;

    VoidStatus   = Env_Normal;

#ifdef __BCC__
    sprintf (Msg, "    Extend VOID [%6u] %8lu bytes;  New Void =%8lu\n",
             VoidBlksAllocated + 1, *SizeInBytes, VoidUsed);
    TraceMsg (0, Msg);

#endif

#ifdef CORE_LIMIT
    if (*SizeInBytes > CORE_LIMIT)
    {
       sprintf (Msg, TraceStr(0),
/*  "    Alloc from the VOID %8lu bytes;\n"  */
                 *SizeInBytes);
        TraceMsg (0, Msg);
       return (CoreAddr);
    }
#endif

#ifdef OLDWAY
    if (*SizeInBytes % CoreBlk_GapSpace)
    {
       ByteAlignment = CoreBlk_GapSpace - (*SizeInBytes % CoreBlk_GapSpace);
      *SizeInBytes  += ByteAlignment;
    }
#else

#ifdef CORE_CRC_CHECK

    FarSize          = *SizeInBytes + CORE_BLOCK_ALIGN + 4;
    if (FarSize % CoreBlk_GapSpace)
    {
       ByteAlignment = CoreBlk_GapSpace - (FarSize % CoreBlk_GapSpace);
       FarSize      += ByteAlignment;
    }

#else
    FarSize          = *SizeInBytes; 
    if (FarSize % CoreBlk_GapSpace)
    {
       ByteAlignment = CoreBlk_GapSpace - (FarSize % CoreBlk_GapSpace);
       FarSize      += ByteAlignment;
    }

#endif  /* CORE_CRC_CHECK */

#endif  /* OLDWAY */

    if ((FarSize + VoidUsed) > VOID_BOUNDARY)
    {
       if (FirstLimit)
          SendMsg (0,   "\n *** WARNING *** VOID_BOUNDARY EXCEEDED!!!\n");
       else
          SendMsg (0,   "\n *** ERROR   *** VOID_BOUNDARY EXCEEDED!!!\n");

       sprintf (Msg, "      BOUNDARY := %8d;  RESERVE := %8d;\n",
                VOID_BOUNDARY, VOID_RESERVE);
       SendMsg (0, Msg);

       if (FirstLimit)
          VOID_BOUNDARY  += VOID_RESERVE;
       FirstLimit         = False;

       return (NullPtr);
    }

    if ((LongPtr  = (lt64           *)GET_MEM (1, FarSize)) == NULL)
    {
      if (!FirstTry)
      {
        return (NullPtr);

      } else {
        FirstTry     = False;
        do
        {
          *SizeInBytes = *SizeInBytes - 256;
           LongPtr     = (lt64          *)GET_MEM (1, *SizeInBytes);
        } while (*SizeInBytes >= 256 && LongPtr == NULL);

        if (LongPtr)
        if (sprintf (Msg, TraceStr(1),
/*  "  *** WARNING ... Partial Core Page = %lu\n"  */
                     *SizeInBytes))
           SendMsg (0, Msg);
      }
    }

    if (! CoreFreeStoreSize && FirstTry)
       SysFarCoreLeft();
    else
       CoreFreeStoreSize -= FarSize;

#ifdef    CORE_CRC_CHECK
    memset             ((addrtype )LongPtr, 0x55,   4);
    EndCoreBlkAddr   = (addrtype )((char  *)LongPtr + (FarSize - 4));
    memset             (EndCoreBlkAddr,     0x55,   4);

    CoreAddr         = (addrtype )((char *)LongPtr + CORE_BLOCK_ALIGN);

#else
    CoreAddr         = (addrtype )LongPtr;

#endif /* CORE_CRC_CHECK */

    VoidAllocated   += FarSize;
    VoidUsed        += FarSize;

    if (VoidUsed > VoidMaxUsed)
       VoidMaxUsed        = VoidUsed;

    VoidBlksAllocated++;


    if (FirstTime)
    {
       VoidLowestAddr   = CoreAddr;
       FirstTime        = False;

    } else if (CoreAddr < VoidLowestAddr) {
       VoidLowestAddr   = CoreAddr;
    }



    if (StrucAlignment)
    {
      if ((farlongtype )CoreAddr % StrucAlignment)
      {
         sprintf (Msg, TraceStr(4),
/*  " CoreMoreCore... Non Aligned Address= %8x\n"  */
                   LongPtr);
         TraceMsg (0, Msg);
         sprintf (Msg, TraceStr(5),
/*  "                 Core1Size          = %8lu\n"  */
                   Core1Size);
         SendMsg (0, Msg);

         VoidStatus   = Core_NotInAlignment;
      }
    }

    if ( DeBug )
    {
       sprintf(Msg, "Extend VOID [%6u] := %8lu bytes;  VoidUsed := %8lu\n",
             VoidBlksAllocated , *SizeInBytes, VoidUsed);
       TraceMsg (0, Msg);
    }

return (CoreAddr);
}


void VoidFreeCore         (addrtype      *CoreAddr,  farlongtype SizeInBytes)
{
farlongtype    FarSize        = 0;
sizetype       ByteAlignment  = 0;
addrtype       EndCoreBlkAddr = NullPtr;
lt64          *LongPtr = NullPtr;

    VoidStatus   = Env_Normal;

#ifdef __BCC__
   sprintf (Msg, /* TraceStr[6] */
            "    VOID FreeCore:: Void Range (* %-8lu) @ %8x :: %8x\n",
            SizeInBytes, *CoreAddr,
            (addrtype )(*(farlongtype **)CoreAddr + SizeInBytes));
   TraceMsg (0, Msg);

#else
   if (CoreBug)
   if (sprintf (Msg, TraceStr(6),
/*  "    VOID FreeCore:: Void Range (* %-8lu) @ %8x :: %8x\n"  */
                 SizeInBytes, *CoreAddr,
                 (addrtype )(*(farlongtype **)CoreAddr + SizeInBytes)))
       TraceMsg (0, Msg);
#endif
   if (*CoreAddr)
   {

#ifdef    CORE_CRC_CHECK

      FarSize          = SizeInBytes + CORE_BLOCK_ALIGN + 4;
      if (FarSize % CoreBlk_GapSpace)
      {
         ByteAlignment = CoreBlk_GapSpace - (FarSize % CoreBlk_GapSpace);
         FarSize      += ByteAlignment;
      }
     *CoreAddr         = (addrtype )((char  *)*CoreAddr - CORE_BLOCK_ALIGN);
      LongPtr          = *(numtype **)CoreAddr;

      EndCoreBlkAddr   = (addrtype )((char  *)*CoreAddr + (FarSize - 4));

      if (*(numtype *)LongPtr               != CORE_CONSTANT)
      {
         SendMsg (0, "\n ***ERROR... in Management of VOID\n");
         SendMsg (0, "    FIRST 4 bytes in VOID Block OVER-WRITEN !\n");
         sprintf (Msg, "     VOID address := @%8x\n", *CoreAddr);
         TraceMsg (0, Msg);

         VoidStatus   = Core_CRCoverWritten;

      } else if (*(numtype *)EndCoreBlkAddr != CORE_CONSTANT) {

         SendMsg (0, "\n ***ERROR... in Management of VOID\n");
         SendMsg (0, "    LAST  4 bytes in VOID Block OVER-WRITEN !\n");
         sprintf (Msg, "     VOID address := @%8x\n", *CoreAddr);
         TraceMsg (0, Msg);

         VoidStatus   = Core_CRCoverWritten;
      }

#else

      FarSize          = SizeInBytes;
      if (FarSize % CoreBlk_GapSpace)
      {
         ByteAlignment = CoreBlk_GapSpace - (FarSize % CoreBlk_GapSpace);
         FarSize      += ByteAlignment;
      }

#endif /* CORE_CRC_CHECK */

      VoidDeAllocated += FarSize;
      VoidUsed        -= FarSize;

      FREE_MEM         (*(void **)CoreAddr);


   } else {
     sprintf (Msg, "***ERROR... Core Address <%8x> Below Void <%8x>\n",
              *CoreAddr, VoidLowestAddr);
     TraceMsg (0, Msg);

     VoidStatus     = Core_NULLCoreAddrOfFree;
   }
  *CoreAddr         = NullPtr;

   VoidBlksDeAllocated++;

    if ( DeBug )
    {
       sprintf(Msg, "Free   VOID [%6u] := %8lu bytes;  VoidUsed := %8lu\n",
             VoidBlksDeAllocated , SizeInBytes, VoidUsed);
       TraceMsg (0, Msg);
    }

StackTrack ("VoidFreeCore\n");
return;
}


farlongtype SysFarCoreLeft   (void)
{
indextype      CoreBlks           = 0;
addrtype       CoreAddr[20];
counttype      i                  = 0;
farlongtype    AllocAmount        = Core_PageSize;
static
sizetype       SizeOfInt          = sizeof(int);

 if (FirstTry == False
 ||  FirstTry == True)
   return (1024000L);


 if (FirstTry == True)
 {
    FirstTry = False;
    if (SizeOfInt <= 2)
       return (1024000L);

    while ((CoreAddr[i]  = (addrtype )GET_MEM (1, AllocAmount)) != NULL)
    {
       Alloc_Amount += AllocAmount;
       sprintf (Msg, TraceStr(7),
/*  " Allocated %6u bytes @%x\n"  */
                AllocAmount, CoreAddr[i]);
       SendMsg (0, Msg);
       ++i;
    }

    AllocAmount -= 100;
    while ( (AllocAmount &&
             (CoreAddr[i]  = (addrtype )GET_MEM (1, AllocAmount)) == NULL))
    {
       AllocAmount -= 100;
    }

    if (CoreAddr[i])
    {
       Alloc_Amount += AllocAmount;
       sprintf (Msg, TraceStr(8),
/*  " Allocated %6u bytes @%x\n"  */
                AllocAmount, CoreAddr[i]);
       SendMsg (0, Msg);
    } else
      --i;

    if (Alloc_Amount)
    {
       CoreBlks     = i;
       while (i >= 0)
       {
          sprintf (Msg, TraceStr(9),
/*  " De Allocate block[%2u] @ %x\n"  */
                   i, CoreAddr[i]);
          SendMsg (0, Msg);
          FREE_MEM (CoreAddr[i]);
          i--;
       }
       CoreFreeStoreSize = (Core_PageSize * CoreBlks) + AllocAmount;
       sprintf (Msg, TraceStr(10),
/*  "  *** INFO *** Far Core Available = %lu\n"  */
                CoreFreeStoreSize);
       SendMsg (0, Msg);

    } else
       SendMsg (0, " *** CORE *** CoreNoMore\n");
 }

return (CoreFreeStoreSize);
}

/*+-----------------------------------------------------------------------+
~CM                         CORE  MANAGER                                 !
  +-----------------------------------------------------------------------+*/
boolean    CoreMoreCore   (bytessizetype RequestSize,
                           ft F,lt Z,zz *Status,      addrtype   *Region)
{
farlongtype    AllocCore1     = 0;
bytessizetype  SizeInBytes    = RequestSize;

     AllocCore1      = (farlongtype )SizeInBytes;
     if ((*Region    = (addrtype )VoidExtendCore (&AllocCore1)) == NULL)
        *Status      = Env_CoreNoMore;
     else
        *Status      = VoidStatus;

     Core1Used      += (farlongtype )SizeInBytes;

#ifdef    NULL_TRACE_STRS
  if (sprintf (Msg, "            Core1::  Used=        %6lu;\n",
               Core1Size))
      TraceMsg (0, Msg);
#endif /* NULL_TRACE_STRS */

     if (CoreBug || Core1Bug)
     if (sprintf (Msg,
  "                          Core1_MORE Core (*%6lu) @%8x; Alloc= %4u\n",
               RequestSize,  *Region, SizeInBytes))
        TraceMsg (0, Msg);

/* Used for DeBug ..................
 static addrtype  CheckAddr0  = (addrtype )0x454cf4;
 static addrtype  CheckAddr1  = (addrtype )0x454dc4;

  if (*Region >= CheckAddr0
  &&  *Region <= CheckAddr1)
  {
     sprintf (Msg, "\n ... Core0_MoreCore:: CheckAddr=@%x\n", *Region);
     TraceMsg (0, Msg);
  }
*/
TRACK(TrackBak,"CoreMoreCore\n");
return(STAT);
}


boolean CoreFreeCoreSpace (addrtype     *CoreAddr,  bytessize   FreeSize,
                           ft F,lt Z,zz *Status)
{
 bytessizetype  SizeInBytes    = FreeSize;

  if (CoreBug  || DeBug > 0  || Core1Bug)
  if (sprintf (Msg,
  "                          Core1_FREE Core (*%6lu) @%8x; Alloc= %4u\n",
               FreeSize, *CoreAddr, SizeInBytes))
     TraceMsg (0, Msg);

  VoidFreeCore    (CoreAddr,    SizeInBytes);
 *Status        = VoidStatus;

  Core1Used    -= (farlongtype )SizeInBytes;

 *CoreAddr  = NullPtr;

  Core1Freeed += (farlongtype )SizeInBytes;


TRACK(TrackBak,"CoreFreeCoreSpace\n");
return(STAT);
}


boolean    Core0MoreCore  (bytessizetype SizeInBytes,
                           ft F,lt Z,zz *Status,      addrtype   *Region)
{
 bytessizetype  AllocSize  = SizeInBytes;
#ifdef DEBUG_CORE0
indextype   StackPtr   = 0;
#endif

 farlongtype    AllocCore0     = 0;

#ifdef NULL_TRACE_STRS
     if (sprintf (Msg, "    Core0MoreCore ::  Size= %6lu;\n",
                  SizeInBytes))
        TraceMsg (0, Msg);
#endif
     AllocCore0      = (farlongtype )SizeInBytes;
     Core1Used      += (farlongtype )SizeInBytes;
     if ((*Region    = (addrtype )VoidExtendCore (&AllocCore0)) == NULL)
        *Status      = Env_CoreNoMore;
     else
        *Status      = VoidStatus;

#ifdef DEBUG_CORE0

  if (Core0_Chunk == 0)
  {
    Mem_MakeChunk   (sizeof(addrtype),  10000,           1000,
                    0,                McStat,         &Core0_Chunk);
  }
  if (Normal(*Status))
    MemPushAddr (Core0_Chunk,  *Region,  McStat,  &StackPtr);

#endif

  if (CoreBug  || DeBug > 0 || Core0Bug)
  {
     if (sprintf (Msg,
  "                          Core0_MORE Core (*%6lu) @%8x; Alloc= %4u\n",
               SizeInBytes, *Region, AllocSize))
        TraceMsg (0, Msg);
  }

TRACK(TrackBak,"Core0MoreCore\n");
return(STAT);
}


boolean Core0FreeCoreSpace(addrtype     *CoreAddr,  bytessize   FreeSize,
                           ft F,lt Z,zz *Status)
{
 bytessizetype  SizeInBytes    = FreeSize;

#ifdef DEBUG_CORE0
addrtype      *Core0Chunk     = NullPtr;
indextype      StackPtr       = 0;
int            i              = 0;
int            j              = 0;
boolean        NotFound       = True;

  if (MemGetChunkAddr (Core0_Chunk,  McStat,  (addrtype *)&Core0Chunk))
  if (MemGetStackPtr  (Core0_Chunk,  McStat,  &StackPtr))
  while (i < StackPtr
  &&     NotFound
  &&     Normal(*Status))
  {
     if (Core0Chunk[i] == *CoreAddr)
     {
        NotFound = False;
        Core0Chunk[i]    = NullPtr;
        j = i;
        while (j < StackPtr - 1)
        {
           Core0Chunk[j] = Core0Chunk[j+1];
           if (Core0Chunk[j] == *CoreAddr)
           {
              sprintf (Msg,
          "  ***ERROR***  Core0_FREE Core (*%6lu) @%8x Have DUPLICATE;\n",
               FreeSize, *CoreAddr);
              TraceMsg (0, Msg);
              Mem_DumpChunk (0, Core0_Chunk,  McStat);
             *Status = Err_BadDesign;
           }
           j++;
        }
        if (Normal(*Status))
        MemPutStackPtr (Core0_Chunk,  --StackPtr,    McStat);
     }
     i++;
  }

  if (NotFound == True)
  {
     sprintf (Msg,
  "  ***ERROR***  Core0_FREE Core (*%6lu) @%8x Previously FREEED;\n",
               FreeSize, *CoreAddr);
     TraceMsg (0, Msg);
     Mem_DumpChunk (0, Core0_Chunk,  McStat);
    *Status = Err_BadDesign;
  }


#endif

  if (CoreBug  || DeBug > 0 || Core0Bug)
  if (sprintf (Msg,
  "                          Core0_FREE Core (*%6lu) @%8x; Alloc= %4u\n",
               FreeSize, *CoreAddr, SizeInBytes))
     TraceMsg (0, Msg);

  VoidFreeCore    (CoreAddr,    SizeInBytes);

 *Status        = VoidStatus;

  Core1Used    -= (farlongtype )SizeInBytes;

/*  if (SWAP)
*/
 *CoreAddr  = NullPtr;

  Core1Freeed += (farlongtype )SizeInBytes;

TRACK(TrackBak,"Core0FreeCoreSpace\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+
~SM                      STRING   MANAGER                                 !
  +-----------------------------------------------------------------------+*/
boolean    Core0_AllocString
                          (sizetype      StrSize,
                           ft F,lt Z,zz *Status,    char       **String)
{
   Core0MoreCore(StrSize,McStat,(addrtype *) String);

   if (ClassBug || CoreBug || DeBug > 0)
   if (sprintf  (Msg, "    Core0_AllocString(*%4u) @%x\n", StrSize, *String))
       TraceMsg (0, Msg);

TRACK(TrackBak,"Core0_AllocString\n");
return(STAT);
}


boolean Core0_FreeString  (char         *String,   ft F,lt Z,zz *Status)
{
   sizetype  ByteCount = strlen( String ) + 1;

   if (ClassBug || CoreBug || DeBug > 0)
   if (sprintf  (Msg, "    Core0_FreeString @%x\n", String))
       TraceMsg (0, Msg);

   AlignStruc( &ByteCount );
   Core0FreeCoreSpace((addrtype *) &String, ByteCount, McStat);

TRACK(TrackBak,"Core0_FreeString\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+
~CM                         GAP   MANAGER                                 !
  +-----------------------------------------------------------------------+*/
void    Core_AssignChunks
                          (numtype       AddrChunk, addrtype     *FreeAddrs,
                           numtype       SizeChunk, numtype      *FreeSizes)
{
   KrnlFreeAddrChunk  = AddrChunk;
   KrnlFreeAddr       = FreeAddrs;
   KrnlFreeAddr0      = KrnlFreeAddr        - sizeof (chunkstruc);

   KrnlFreeSizeChunk  = SizeChunk;
   KrnlFreeSize       = FreeSizes;

   KrnlChunkUnits     = KrnlBlocks_AllocQty - 1;

return;
}



boolean    CoreMoveBytes  (addrtype      Source,      sizeinbytes   SourceSize,
                           sizeinbytes   TargetSize,
                           ft F,lt Z,zz *Status,      addrtype     *Target )
{
  if (SourceSize == 0)
  {
    *Target = Source;
    *Status = Err_NoBytesMoved;
  }

  if (Normal(*Status))
  if (CoreMoreCore     (TargetSize,              McStat,     Target))
     memcpy            ((char *)*Target, (char *)Source,     SourceSize);

  if (sprintf (Msg, TraceStr(66),
/*  "    CoreMoveBytes(%u) @%x to %x NewSize(%u)\n"  */
               SourceSize, Source, *Target, TargetSize))
     SendMsg (0, Msg);

  if (Normal(*Status))
     CoreFreeCoreSpace (&Source,                 SourceSize, McStat);

TRACK(TrackBak,"CoreMoveBytes\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+
~CS                         CORE STATS                                    !
  +-----------------------------------------------------------------------+*/

#ifdef NULL_TRACE_STRS
void CoreShowStats       (void)
{
statustype  ShellStatus   = Env_Normal;

   TraceMsg (MSGFILE, "\n  Memory Manager Statistics::\n\n");
   sprintf (Msg, TraceStr(67),
/*  " Void      Allocated :: %lu\n"  */
            VoidAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(68),
/*  " Void   De Allocated :: %lu\n"  */
            VoidDeAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(69),
/*  " Void   CurrentSize  :: %lu\n"  */
            VoidUsed);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, " Void   MAX Alloc'd  (at any point in Run) :: %8lu\n\n",  VoidMaxUsed);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, TraceStr(70),
/*  " Blks      Allocated :: %lu\n"  */
            VoidBlksAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(71),
/*  " Blks   De Allocated :: %lu\n\n"  */
            VoidBlksDeAllocated);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, TraceStr(72),
/*  " Regions  Allocated :: %u\n"  */
            Num1Regions);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(73),
/*  " Core   Used         :: %lu\n"  */
            Core1Size);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(74),
/*  " Core   Freeed       :: %lu\n"  */
            Core1Freeed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(75),
/*  " Core   Reclaimed    :: %lu\n\n"  */
            Core1Reclaimed);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, " Core   Current Size :: %8lu\n\n",Core1Used);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, TraceStr(76),
/*  " CoreBlks Requested  :: %lu\n"  */
            CoreBlksRequested);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(77),
/*  " CoreBlks Freeed     :: %lu\n\n"  */
            CoreBlksFreeed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(78),
/*  " CoreBlks Reclaimed  :: %lu\n"  */
            CoreBlksReclaimed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(79),
/*  "     By   Exact Size :: %lu\n"  */
            CoreBlksExactReclaimed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, TraceStr(80),
/*  "     By   Gaps       :: %lu\n"  */
            CoreBlksGapReclaimed);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, TraceStr(81),
/*  "     By   Page Space :: %lu\n\n"  */
            CoreBlksPageSpaceReclaimed);
   TraceMsg (MSGFILE, Msg);

   if (ClassBug || CoreBug)
   if (KrnlFreeAddrChunk)
   {
      TraceMsg (MSGFILE, " Dump of KrnlFreeAddrChunk ::   ");
      Mem_DumpChunkPart (MSGFILE, KrnlFreeAddrChunk, 0, 100, ShellMcStat);
      TraceMsg (MSGFILE, "\n Dump of KrnlFreeSizeChunk ::   ");
      Mem_DumpChunkPart (MSGFILE, KrnlFreeSizeChunk, 0, 100, ShellMcStat);
      TraceMsg (MSGFILE, "\n");
   }

return;
}
#else

void CoreShowStats       (void)
{
statustype  ShellStatus   = Env_Normal;

   TraceMsg (MSGFILE, "\n     { VOID  :  CORE }\n\n");
   sprintf (Msg, " Void:  Total    Allocated :: %8lu\n",  VoidAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Void:  Total De Allocated :: %8lu\n",  VoidDeAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " VOID:  Current  USED      :: %8lu\n",  VoidUsed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " VOID:  MAX Alloc'd (run)  :: %8lu\n\n",VoidMaxUsed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Void:  Blks     Allocated :: %8lu\n",  VoidBlksAllocated);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Void:  Blks  De Allocated :: %8lu\n\n",VoidBlksDeAllocated);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, " Core:  Regions  Allocated :: %8u\n",   Num1Regions);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Core:  Total    Allocated :: %8lu\n",  Core1Size);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Core:  Total    Freeed    :: %8lu\n",  Core1Freeed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Core:  Total    Reclaimed :: %8lu\n\n",Core1Reclaimed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " CORE:  Current  USED      :: %8lu\n\n",Core1Used);
   TraceMsg (MSGFILE, Msg);

   sprintf (Msg, " Core:  Blks     Requested :: %8lu\n",  CoreBlksRequested);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Core:  Blks     Freeed    :: %8lu\n\n",  CoreBlksFreeed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, " Core:  Blks     Reclaimed :: %8lu\n",  CoreBlksReclaimed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, "        By       Exact Fit :: %8lu\n",  CoreBlksExactReclaimed);
   TraceMsg (MSGFILE, Msg);
   sprintf (Msg, "        By       Gaps      :: %8lu\n",  CoreBlksGapReclaimed);
   TraceMsg (MSGFILE, Msg);

#ifdef PAGE_SPACE
   sprintf (Msg, "     By   Page Space :: %8lu\n\n",CoreBlksPageSpaceReclaimed);
   TraceMsg (MSGFILE, Msg);
#endif

   if (ClassBug || CoreBug)
   if (KrnlFreeAddrChunk)
   {
      TraceMsg (MSGFILE, " Dump of KrnlFreeAddrChunk ::   ");
      Mem_DumpChunkPart   (MSGFILE, KrnlFreeAddrChunk, 0, 100, ShellMcStat);
      TraceMsg (MSGFILE, "\n Dump of KrnlFreeSizeChunk ::   ");
      Mem_DumpChunkPart   (MSGFILE, KrnlFreeSizeChunk, 0, 100, ShellMcStat);
      TraceMsg (MSGFILE, "\n");
   }

return;
}
#endif /* NULL_TRACE_STRS    */


/*+-----------------------------------------------------------------------+
~CM                       GENERIC EXTERNAL UTILITIES                      !
  +-----------------------------------------------------------------------+*/
void       MoveBytes      (addrtype     Source,      addrtype  Target,
                           sizeinbytes  ByteCount)
{
    memcpy    ((char *)Target,    (char *)Source,    ByteCount);

    if (DeBug >= 2 || MemBug)
    {
       sprintf (Msg, TraceStr(82),
/*  "       %u Bytes moved from %x to %x "  */
                ByteCount, Source, Target);
       TraceMsg (0, Msg);
       switch (ByteCount)
       {
          case  1 :
            sprintf (Msg, TraceStr(83),
/*  " %4u\n"  */
                     *(char      *)Source);
            break;

          case  2 :
            sprintf (Msg, TraceStr(84),
/*  " %4u\n"  */
                    *(short     *)Source);
            break;

          case  8 :
            sprintf (Msg, TraceStr(85),
/*  "<[%4lu:%4lu]> || <%6xX : %6xX>\n"  */
                     *(longtype *)Source,
                     *(longtype *)((char *)Source + sizeof (long)),
                     *(addrtype *)Source,
                     *(addrtype *)((char *)Source + sizeof (addrtype)));
            break;

          case  4 :
          default :
            sprintf (Msg, TraceStr(86),
/*  "<%4ld>  || <%6xX>\n"  */
                     *(lt64     *)Source,  *(addrtype  *)Source);
       }
       TraceMsg (0, Msg);
    }

StackTrack ("MoveBytes\n");
return;
}

int        CompareBytes  (addrtype      Source,      addrtype      CmpVal,
                          sizeinbytes   ByteCount,   ft F,lt Z,zz *Status)
{
int i = 0;

    if (DeBug >= 2)
    if (sprintf (Msg, TraceStr(87),
/*  "       %u Bytes compared testvalue= %u \n"  */
             ByteCount, Source, CmpVal,
             i = memcmp ((char *)Source, (char *)CmpVal, ByteCount)))
       TraceMsg (0, Msg);

    if (i<0)
       *Status = Env_Less;

    else if(i>0)
       *Status = Env_Gtr;

    else
       *Status = Env_Normal;

StackTrack ("CompareBytes\n");
STAT;
return(i);
}

/*+-----------------------------------------------------------------------+
~M                         END OF CORE.C                                  !
  +-----------------------------------------------------------------------+*/
