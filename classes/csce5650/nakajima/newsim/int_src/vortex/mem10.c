/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#define MEM_C
#define MEM10
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem00.h"
#include "mem01.h"
#include "mem10.h"


/*+-----------------------------------------------------------------------+
~CM                         MEM10 -- Chunk Manager Shell Interface        !
  +-----------------------------------------------------------------------+*/
boolean    MemFreezeChunk (numtype  Chunk,  ft F,lt Z,zz *Status)
{
  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk, McStat))
      Chunk_Addr(Chunk) = NullPtr;

TRACK(TrackBak,"MemFreezeChunk\n");
return(STAT);
}


boolean    MemDefrostChunk (numtype  Chunk,  ft F,lt Z,zz *Status)
{
  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk, McStat))
      Chunk_Addr(Chunk) = NullPtr;

TRACK(TrackBak,"MemDefrostChunk\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean    MemGetChunkSize(numtype       Chunk,
                           ft F,lt Z,zz *Status,    indextype  *UnitCount,
                           bytessize    *UnitSize)
{
  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk,        McStat))
   {
      *UnitCount = Unit_Count    (Chunk);
      *UnitSize  = Unit_Size     (Chunk);
   }

TRACK(TrackBak,"MemGetChunkSize\n");
return(STAT);
}
 

boolean    MemGetChunkStruc (numtype     Chunk,    ft F,lt Z,zz *Status,
                             chunkstruc *ChunkRef)
{
   Mem_ChunkExpanded          = 0;
   if (ChunkExists (Chunk,      McStat))
   {
      ChunkRef->UnitSize      = Unit_Size       (Chunk);
      ChunkRef->UnitCount     = Unit_Count      (Chunk);
      ChunkRef->StackPtr      = Stack_Ptr       (Chunk);
      ChunkRef->StringText    = Text_Chunk      (Chunk);
      ChunkRef->MaxUnitLength = MaxUnit_Length  (Chunk);
      ChunkRef->ExtendQty     = Extend_Qty      (Chunk);

   }

TRACK(TrackBak,"MemGetChunkStruc\n");
return(STAT);

}


boolean    MemDumpChunkStruc (chunkstruc *ChunkRef,    ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"MemDumpChunkStruc\n");
return(STAT);

}

/*+-----------------------------------------------------------------------+*/
 
boolean    MemPutChunkAddr (numtype       Chunk,  addrtype  Addr,  
                            ft F,lt Z,zz *Status)
{
  if (Chunk > NumChunks) {
     *Status = Err_BadChunk;
  } else if (!Chunk) {
     *Status = Err_BadChunk;
  }
  Mem_ChunkExpanded                   = 0;

  if (Normal(*Status))
      Chunk_Addr(Chunk) = Addr;
TRACK(TrackBak,"MemPutChunkAddr\n");
return(STAT);
}
 

boolean    MemGetChunkAddr (numtype       Chunk,  
                            ft F,lt Z,zz *Status,  addrtype  *Addr)
{
  if (Chunk > NumChunks) {
     *Status = Err_BadChunk;
  } else if (!Chunk) {
     *Status = Err_BadChunk;
  }
  Mem_ChunkExpanded                   = 0;

  if (Normal(*Status))
     *Addr = Chunk_Addr(Chunk);

TRACK(TrackBak,"MemGetChunkAddr\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
 
boolean    MemPutStackPtr (numtype       Chunk,   indextype   StackPtr,
                           ft F,lt Z,zz *Status)
{
indextype   CurrStackPtr  = 0;
sizetype    TempByte      = 0;
sizetype    StartByte     = 0;
sizetype    EndByte       = 0;
sizetype    ExtendBytes   = 0;
bytetype   *ChunkAddr     = NullPtr;

  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk, McStat))
   {
     CurrStackPtr   = Stack_Ptr(Chunk);

     if ((Stack_Ptr(Chunk) = StackPtr) > (Unit_Count(Chunk)) )
     if (Chunk_IsNumeric(Chunk))
        GrowNumericChunk (Chunk, McStat);
     else
        GrowTextChunk    (Chunk, McStat);
   }

   if (Normal(*Status))
   if (ClearChunk)
   {
     ChunkAddr  = (bytetype *)Chunk_Addr(Chunk); 
     StartByte  = (sizetype )(CurrStackPtr * Unit_Size(Chunk));
     EndByte    = (sizetype )(StackPtr     * Unit_Size(Chunk)) - 1;
     if (StartByte > EndByte)
     {
       TempByte  = StartByte;
       StartByte = EndByte;
       EndByte   = TempByte;   
     }
     ExtendBytes= EndByte                  - StartByte         + 1;

     ChunkAddr   += StartByte;
     memset ((void *)ChunkAddr, (int )NULL, ExtendBytes);

   }

TRACK(TrackBak,"MemPutStackPtr\n");
return(STAT);
}
 

boolean    Mem_AssertStackPtr 
                          (numtype       Chunk,   indextype   StackPtr,
                           ft F,lt Z,zz *Status,  addrtype   *ChunkAddr)
{
   if (MemPutStackPtr   (Chunk,  StackPtr,  McStat))
      *ChunkAddr    = Chunk_Addr(Chunk);


TRACK(TrackBak,"Mem_AssertStackPtr\n");
return(STAT);
}


boolean    MemGetStackPtr (numtype       Chunk,
                           ft F,lt Z,zz *Status,   indextype  *StackPtr)
{
  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk, McStat))
     *StackPtr = Stack_Ptr(Chunk);

   if (*Status == Core_GapFound)
       *Status = Err_BadDesign;

TRACK(TrackBak,"MemGetStackPtr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+
~CM                Derived Chunk Types for Push, Pop, Put, Get            !
  +-----------------------------------------------------------------------+*/

boolean    MemPushEntity  (numtype       Chunk,  sizeinbytes ByteCount,
                           addrtype      Entity,
                           ft F,lt Z,zz *Status, indextype  *ByteOffset)
{
indextype     StackPtr   = 0;
addrtype      BytePtr    = NullPtr;

  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk,  McStat))
   if (Chunk_IsData(Chunk))
   {
     *ByteOffset        = (indextype  )Stack_Ptr(Chunk);

      StackPtr          = *ByteOffset       + ByteCount;

      AlignStruc ((sizetype *)&StackPtr);
      Stack_Ptr(Chunk)  = StackPtr;

      if (StackPtr     >= Unit_Count(Chunk))
         GrowNumericChunk (Chunk,             McStat);

      if (Normal(*Status))
      {
         BytePtr = ((char *)(Chunk_Addr(Chunk))) + *ByteOffset;
         MoveBytes (Entity, BytePtr, ByteCount);
      }

   } else {
     *Status = Err_BadUnitType;
      DumpChunkChunk (0, Chunk);
   }

TRACK(TrackBak,"MemPushEntity\n");
return(STAT);
}
 
 
boolean    MemPutEntity   (numtype   Chunk,      indextype     ByteOffset,
                           bytessize ByteCount,
                           addrtype  Entity,     ft F,lt Z,zz *Status)
{
indextype     StackPtr   = 0;
addrtype      BytePtr    = NullPtr;

  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk,  McStat))
   if (Chunk_IsData(Chunk))
   {
      StackPtr        = ByteOffset + ByteCount;
      if (StackPtr    >  Stack_Ptr(Chunk))
      {
         TraceMsg (0, Msg);
         DumpChunkChunk (0, Chunk);
        *Status    = Err_IndexOutOfRange;

      } else {
         BytePtr = ((char *)(Chunk_Addr(Chunk))) + ByteOffset;
         MoveBytes (Entity, BytePtr, ByteCount);
      }
   }

TRACK(TrackBak,"MemPutEntity\n");
return(STAT);
}
 

boolean MemExtractEntity (numtype       Chunk,   indextype     ByteOffset, 
                          sizeinbytes   Count,   ft F,lt Z,zz *Status)
{
indextype     StackPtr   = 0;
bytetype     *BytePtr    = NullPtr;
bytetype     *NewPtr     = NullPtr;

  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk,  McStat))
   if (Chunk_IsData(Chunk))
   {
      NewPtr  = (bytetype *)Chunk_Addr(Chunk);
      BytePtr = (bytetype *)(((char *)(Chunk_Addr(Chunk))) + ByteOffset);

      for (StackPtr    = (indextype )Stack_Ptr(Chunk);
           StackPtr--  > ByteOffset;)
      {
        *NewPtr++ = *BytePtr++;
      }
      Stack_Ptr(Chunk)  -= Count;
   }

TRACK(TrackBak,"MemExtractEntity\n");
return(STAT);
}
 

boolean    MemGetEntity   (numtype       Chunk,     indextype  ByteOffset,
                           bytessize     ByteCount,
                           ft F,lt Z,zz *Status,    addrtype  *Entity)
{
char         *BytePtr    = NullPtr;

  Mem_ChunkExpanded                = 0;
   if (ChunkExists (Chunk,  McStat))
   if (Chunk_IsData(Chunk))
   { 
      Core0_AllocString (ByteCount, McStat, (char **)Entity);

      BytePtr = ((char *)(Chunk_Addr(Chunk))) + ByteOffset;

      if ((ByteOffset +ByteCount) > Unit_Count(Chunk))
      {
          if (SetGetSwi)
            *Status    = Set_EndOfSet;

          else {
             DumpChunkChunk (0, Chunk);
            *Status    = Err_IndexOutOfRange;
          }
      }
      if (Normal(*Status))
         MoveBytes (BytePtr, *Entity, ByteCount);
   }

TRACK(TrackBak,"MemGetEntity\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean    MemNewBitChunk (numtype       BitWidth,   idtype      CoreDbId,
                           ft F,lt Z,zz *Status,     numtype    *BitChunk)
{
bitfieldtype *BitField   = NullPtr;
sizetype      NumBits    = (BitWidth | 7)+1;
sizetype      NumBytes   = NumBits / 8;

  if (MemMakeChunk    (sizeof (bitfieldtype),  NumBytes,  NumBytes,
                       CoreDbId,               McStat,    BitChunk))
  if (MemPutStackPtr  (*BitChunk,              NumBytes,  McStat))
  if (MemGetChunkAddr (*BitChunk,              McStat, (addrtype *)&BitField))
         memset       (BitField,    '\0',      NumBytes);

TRACK(TrackBak,"MemNewBitChunk\n");
return(STAT);
}

boolean    MemPushBit    (numtype       Chunk,      numtype       BitValue,
                          ft F,lt Z,zz *Status,     indextype    *StackPtr)
{
   *Status = Err_NotImplemented;

TRACK(TrackBak,"MemPushBit\n");
return(STAT);
}
 
boolean    MemPutBit     (numtype       Chunk,      indextype     BitNum,
                          numtype       BitValue,   ft F,lt Z,zz *Status)
{
bitfieldtype    *BitField  = NullPtr;
indextype        ByteNum   = BitNum >> 3;
boolean          PushFlag  = False;
numtype          Value     = 0;

  Mem_ChunkExpanded                = 0;
if (ChunkExists       (Chunk,      McStat))
{
   if (ByteNum       >= Unit_Count (Chunk))
   {
     Stack_Ptr(Chunk) = ByteNum;
     GrowNumericChunk (Chunk,       McStat);
     Stack_Ptr(Chunk) = Unit_Count (Chunk);
   }

   if (ChkPutChunk (Chunk, ByteNum, sizeof(bitfieldtype), McStat, &PushFlag))
   {
       BitField    = (bitfieldtype *)Chunk_Addr(Chunk);

       if (BitValue) /* Set   */
          BitField[ ByteNum ] |= BitValue << (BitNum & 7);
       else          /* Reset */
          BitField[ ByteNum ] &= ~(1 << (BitNum & 7));
   }

   if (MemBug)
   {
      Value       =  (BitField[ ByteNum ] & 1 << (BitNum & 7)) ? 1: 0;
   }
}

TRACK(TrackBak,"MemPutBit\n");
return(STAT);
}
 
boolean    MemPopBit     (numtype       Chunk,
                          ft F,lt Z,zz *Status,     numtype     *BitValue,
                          indextype    *StackPtr )
{
   *Status = Err_NotImplemented;

TRACK(TrackBak,"MemPopBit\n");
return(STAT);
}
 
boolean    MemGetBit     (numtype       Chunk,      indextype    BitNum,
                          ft F,lt Z,zz *Status,     numtype     *BitValue)
{
indextype        ByteNum   = BitNum >> 3;
bitfieldtype    *BitField  = NullPtr;
indextype        ByteCount = 0;

     if (ChkGetChunk     (Chunk, ByteNum, sizeof(bitfieldtype), McStat))
     if (MemGetStackPtr  (Chunk, McStat,  &ByteCount))
     if (ByteNum >= ByteCount)
        *Status = Mem_BitIndexGtrByteCount;

     if (Normal(*Status))
     {
         BitField =  (bitfieldtype *)Chunk_Addr(Chunk);
        *BitValue =  (BitField[ ByteNum ] & 1 << (BitNum & 7)) ? 1: 0;
     }

TRACK(TrackBak,"MemGetBit\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
boolean    MemPushShort (numtype       Chunk,  shorttype    ShortValue,
                        ft F,lt Z,zz *Status, indextype  *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkPushChunk (Chunk, sizeof(shorttype), McStat))
   {
      *StackPtr = Stack_Ptr(Chunk)++;
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(shorttype));
       Ptr      = (addrtype )(Base + Inset);
      *( ((short *)(Chunk_Addr(Chunk))) + *StackPtr )  = ShortValue;
       if (*(short *)Ptr != ShortValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPushShort\n");
return(STAT);
}
 
boolean    MemPutShort  (numtype   Chunk,     indextype     Index,
                        shorttype  ShortValue, ft F,lt Z,zz *Status)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;
boolean PushFlag = False;
   if (ChkPutChunk (Chunk, Index, sizeof(shorttype), McStat, &PushFlag))
   if (PushFlag)
   {indextype  StackPtr;
      MemPushShort (Chunk, ShortValue, McStat, &StackPtr);
   } else {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(shorttype));
       Ptr      = (addrtype )(Base + Inset);
     *( ((short *)(Chunk_Addr(Chunk))) + Index ) = ShortValue;
       if (*(short *)Ptr != ShortValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPutShort\n");
return(STAT);
}
 
boolean    MemPopShort  (numtype   Chunk,     ft F,lt Z,zz *Status,
                        shorttype *ShortValue, indextype    *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;
   if (ChkPopChunk (Chunk, sizeof(shorttype), McStat)) {
      *StackPtr = --Stack_Ptr(Chunk);
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(shorttype));
       Ptr      = (addrtype )(Base + Inset);
      *ShortValue = *( ((short *)(Chunk_Addr(Chunk))) + *StackPtr );
       if (*(short *)Ptr != *ShortValue)
          *Status = Err_BadStackMath;
   }
TRACK(TrackBak,"MemPopShort\n");
return(STAT);
}
 
boolean    MemGetShort  (numtype       Chunk,  indextype   Index,
                        ft F,lt Z,zz *Status, shorttype   *ShortValue)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;
   if (ChkGetChunk (Chunk, Index, sizeof(shorttype), McStat))
   if (*Status != Set_EndOfSet) {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(shorttype));
       Ptr      = (addrtype )(Base + Inset);
      *ShortValue = *(((short *)(Chunk_Addr(Chunk))) + Index);
       if (*(short *)Ptr != *ShortValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemGetShort\n");
return(STAT);
}
/*+-----------------------------------------------------------------------+*/
 
boolean    MemPushWord (numtype       Chunk,  wordtype    WordValue,
                        ft F,lt Z,zz *Status, indextype  *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkPushChunk (Chunk, sizeof(wordtype), McStat)) {
      *StackPtr = Stack_Ptr(Chunk)++;
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(wordtype));
       Ptr      = (addrtype )(Base + Inset);
      *( ((word *)(Chunk_Addr(Chunk))) + *StackPtr )  = WordValue;
       if (*(word *)Ptr != WordValue)
          *Status = Err_BadStackMath;
   }
TRACK(TrackBak,"MemPushWord\n");
return(STAT);
}
 
boolean    MemPutWord  (numtype   Chunk,     indextype     Index,
                        wordtype  WordValue, ft F,lt Z,zz *Status)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;
boolean PushFlag = False;

   if (ChkPutChunk (Chunk, Index, sizeof(wordtype), McStat, &PushFlag))
   if (PushFlag)
   {indextype  StackPtr;
      MemPushWord (Chunk, WordValue, McStat, &StackPtr);
   } else {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(wordtype));
       Ptr      = (addrtype )(Base + Inset);
     *( ((word *)(Chunk_Addr(Chunk))) + Index ) = WordValue;
       if (*(word *)Ptr != WordValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPutWord\n");
return(STAT);
}
 
boolean    MemPopWord  (numtype   Chunk,     ft F,lt Z,zz *Status,
                        wordtype *WordValue, indextype    *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkPopChunk (Chunk, sizeof(wordtype), McStat)) {
      *StackPtr = --Stack_Ptr(Chunk);
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(wordtype));
       Ptr      = (addrtype )(Base + Inset);
      *WordValue = *( ((word *)(Chunk_Addr(Chunk))) + *StackPtr );
       if (*(word *)Ptr != *WordValue)
          *Status = Err_BadStackMath;
   }
TRACK(TrackBak,"MemPopWord\n");
return(STAT);
}
 
boolean    MemGetWord  (numtype       Chunk,  indextype   Index,
                        ft F,lt Z,zz *Status, wordtype   *WordValue)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkGetChunk (Chunk, Index, sizeof(wordtype), McStat))
   if (*Status != Set_EndOfSet)
   {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(wordtype));
       Ptr      = (addrtype )(Base + Inset);
      *WordValue = *(((word *)(Chunk_Addr(Chunk))) + Index);
       if (*(word *)Ptr != *WordValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemGetWord\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+*/
boolean    MemPushLong (numtype       Chunk,  lt64        LongValue,
                        ft F,lt Z,zz *Status, indextype  *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkPushChunk (Chunk, sizeof(lt64    ), McStat)) {
      *StackPtr = Stack_Ptr(Chunk)++;
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(lt64    ));
       Ptr      = (addrtype )(Base + Inset);
      *( ((lt64     *)(Chunk_Addr(Chunk))) + *StackPtr )  = LongValue;
       if (*(lt64     *)Ptr != LongValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPushLong\n");
return(STAT);
}
 
boolean    MemPutLong  (numtype   Chunk,     indextype     Index,
                        lt64      LongValue, ft F,lt Z,zz *Status)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;
boolean PushFlag = False;

   if (ChkPutChunk (Chunk, Index, sizeof(lt64    ), McStat, &PushFlag))
   if (PushFlag)
   {indextype  StackPtr;
      MemPushLong (Chunk, LongValue, McStat, &StackPtr);
   } else {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(lt64    ));
       Ptr      = (addrtype )(Base + Inset);
     *( ((lt64     *)(Chunk_Addr(Chunk))) + Index ) = LongValue;
       if (*(lt64     *)Ptr != LongValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPutLong\n");
return(STAT);
}
 
boolean    MemPopLong  (numtype   Chunk,     ft F,lt Z,zz *Status,
                        lt64     *LongValue, indextype    *StackPtr)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkPopChunk (Chunk, sizeof(lt64    ), McStat))
   {
      *StackPtr = --Stack_Ptr(Chunk);
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(*StackPtr * sizeof(lt64    ));
       Ptr      = (addrtype )(Base + Inset);
      *LongValue = *( ((lt64     *)(Chunk_Addr(Chunk))) + *StackPtr );
       if (*(lt64     *)Ptr != *LongValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemPopLong\n");
return(STAT);
}
 
boolean    MemGetLong  (numtype       Chunk,  indextype   Index,
                        ft F,lt Z,zz *Status, lt64       *LongValue)
{
char     *Base  = 0;
indextype Inset = 0;
addrtype  Ptr   = 0;

   if (ChkGetChunk (Chunk, Index, sizeof(lt64    ), McStat))
   if (*Status != Set_EndOfSet)
   {
       Base     = (char    *)Chunk_Addr(Chunk);
       Inset    = (indextype )(Index * sizeof(lt64    ));
       Ptr      = (addrtype )(Base + Inset);
      *LongValue = *(((lt64     *)(Chunk_Addr(Chunk))) + Index);
       if (*(lt64     *)Ptr != *LongValue)
          *Status = Err_BadStackMath;
   }

TRACK(TrackBak,"MemGetLong\n");
return(STAT);
}
/*+-----------------------------------------------------------------------+*/
 
boolean    MemPushAddr (numtype       Chunk,  addrtype    AddrValue,
                        ft F,lt Z,zz *Status, indextype  *StackPtr)
{
addrtype *BytePtr = NullPtr;

   if (ChkPushChunk (Chunk, sizeof(addrtype), McStat)) {
       BytePtr  = (addrtype *)Chunk_Addr(Chunk);
      *StackPtr = Stack_Ptr(Chunk)++;
      *(BytePtr + *StackPtr )  = AddrValue;
   }

TRACK(TrackBak,"MemPushAddr\n");
return(STAT);
}
 
boolean    MemPutAddr  (numtype  Chunk,     indextype      Index,
                        addrtype  Addr,      ft F,lt Z,zz *Status)
{
boolean PushFlag = False;
addrtype     ChunkSlotAddr = NullPtr;

   if (ChkPutChunk (Chunk, Index, sizeof(addrtype), McStat, &PushFlag))
   if (PushFlag)
   {indextype  StackPtr;
      MemPushAddr (Chunk, Addr, McStat, &StackPtr);
   } else
     *( ((addrtype *)(Chunk_Addr(Chunk))) + Index ) = Addr;

   ChunkSlotAddr = (addrtype )((char *)Chunk_Addr(Chunk) + Index);

TRACK(TrackBak,"MemPutAddr\n");
return(STAT);
}
 
boolean    MemPopAddr  (numtype   Chunk,     ft F,lt Z,zz *Status,
                        addrtype *Addr,      indextype    *StackPtr)
{
   if (ChkPopChunk (Chunk, sizeof(addrtype), McStat)) {
      *StackPtr = --Stack_Ptr(Chunk);
      *Addr = *( ((addrtype *)(Chunk_Addr(Chunk))) + *StackPtr );
   }
TRACK(TrackBak,"MemPopAddr\n");
return(STAT);
}
 
boolean    MemGetAddr  (numtype       Chunk,  indextype   Index,
                        ft F,lt Z,zz *Status, addrtype   *Addr)
{
addrtype     ChunkSlotAddr = NullPtr;


   if (ChkGetChunk (Chunk, Index, sizeof(addrtype), McStat))
   if (*Status != Set_EndOfSet)
   {
     *Addr = *(((addrtype *)(Chunk_Addr(Chunk))) + Index);
      ChunkSlotAddr = (addrtype )((char *)Chunk_Addr(Chunk) + Index);
   }

TRACK(TrackBak,"MemGetAddr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
 
boolean    MemPushToken (numtype       Chunk,  tokentype  *Token,
                         ft F,lt Z,zz *Status, indextype  *StackPtr)
{
   if (ChkPushChunk (Chunk, sizeof(tokentype), McStat)) {
      *StackPtr = Stack_Ptr(Chunk)++;
      *( ((tokentype *)(Chunk_Addr(Chunk))) + *StackPtr )  = *Token;
   }
TRACK(TrackBak,"MemPushToken\n");
return(STAT);
}
 
boolean    MemPutToken  (numtype    Chunk,     indextype     Index,
                         tokentype *Token,     ft F,lt Z,zz *Status)
{
boolean PushFlag = False;
   if (ChkPutChunk (Chunk, Index, sizeof(tokentype), McStat, &PushFlag))
   if (PushFlag)
   {indextype  StackPtr;
      MemPushToken (Chunk, Token, McStat, &StackPtr);
   } else
     *( ((tokentype *)(Chunk_Addr(Chunk))) + Index ) = *Token;

TRACK(TrackBak,"MemPutToken\n");
return(STAT);
}
 
boolean    MemPopToken  (numtype   Chunk,      ft F,lt Z,zz *Status,
                        tokentype *Token,      indextype    *StackPtr)
{
   if (ChkPopChunk (Chunk, sizeof(tokentype), McStat)) {
      *StackPtr = --Stack_Ptr(Chunk);
      *Token = *( ((tokentype *)(Chunk_Addr(Chunk))) + *StackPtr );
   }
TRACK(TrackBak,"MemPopToken\n");
return(STAT);
}
 
boolean    MemGetToken  (numtype       Chunk,  indextype    Index,
                         ft F,lt Z,zz *Status, tokentype   *Token)
{
   if (ChkGetChunk (Chunk, Index, sizeof(tokentype), McStat))
   if (*Status != Set_EndOfSet)
     *Token = *(((tokentype *)(Chunk_Addr(Chunk))) + Index);

TRACK(TrackBak,"MemGetToken\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
 
boolean    MemPushSomeBytes (numtype       Chunk,      sizeinbytes ByteCount,
                             addrtype      SomeBytes,    
                             ft F,lt Z,zz *Status,     indextype  *StackPtr)
{
addrtype  BytePtr = NullPtr;
indextype Offset  = 0;

   if (ChkPushChunk (Chunk, ByteCount, McStat))
   {
     *StackPtr = Stack_Ptr(Chunk)++;
      Offset   = *StackPtr * (indextype  )ByteCount;
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + Offset;
      MoveBytes (SomeBytes, BytePtr, ByteCount);
   }

TRACK(TrackBak,"MemPushSomeBytes\n");
return(STAT);
}
 
boolean    MemPutSomeBytes  (numtype       Chunk,     indextype   Index,
                             sizeinbytes   ByteCount, addrtype    SomeBytes,
                             ft F,lt Z,zz *Status)
{
boolean    PushFlag  = False;
addrtype   BytePtr   = NullPtr;
indextype  StackPtr  = 0;

   if (ChkPutChunk     (Chunk, Index,     ByteCount, McStat, &PushFlag))
   if (PushFlag)
      MemPushSomeBytes (Chunk, ByteCount, SomeBytes, McStat, &StackPtr);

   else {
   /* BytePtr  = (addrtype )(((char *)(Chunk_Addr(Chunk)))
                         + (ByteCount * Index)); */

      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + (ByteCount * Index);

      MoveBytes        (SomeBytes,        BytePtr,   ByteCount);
     *Status   = Env_Normal;

   }

TRACK(TrackBak,"MemPutSomeBytes\n");
return(STAT);
}
 

 
boolean    MemPopSomeBytes  (numtype       Chunk,      bytessize   ByteCount,
                             ft F,lt Z,zz *Status,   
                             addrtype      SomeBytes,  indextype  *StackPtr)
{
addrtype  BytePtr    = NullPtr;
indextype Offset     = 0;

   if (ChkPopChunk (Chunk, ByteCount, McStat))
   {
      *StackPtr = --Stack_Ptr(Chunk);
      Offset   = *StackPtr * (indextype  )ByteCount;
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + Offset;
      MoveBytes (BytePtr, SomeBytes, ByteCount);
   }

TRACK(TrackBak,"MemPopSomeBytes\n");
return(STAT);
}
 
boolean    MemGetSomeBytes  (numtype       Chunk,       indextype   Index,
                             sizeinbytes   ByteCount,
                             ft F,lt Z,zz *Status,      addrtype    SomeBytes)
{
addrtype BytePtr = NullPtr;

   if (ChkGetChunk (Chunk, Index, ByteCount, McStat))
   if (*Status != Set_EndOfSet)
   {
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + (ByteCount * Index);
      MoveBytes (BytePtr, SomeBytes, ByteCount);
      *Status  = Env_Normal;
   }

TRACK(TrackBak,"MemGetSomeBytes\n");
return(STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
 
boolean    MemPushFuncPtr (numtype       Chunk,  funcptrtype FuncPtr,
                           ft F,lt Z,zz *Status, indextype  *StackPtr)
{
addrtype  BytePtr   = NullPtr;
indextype Offset    = 0;
indextype ByteCount = sizeof (funcptrtype);

   if (ChkPushChunk (Chunk, ByteCount, McStat))
   {
     *StackPtr = Stack_Ptr(Chunk)++;
      Offset   = *StackPtr *  ByteCount;
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + Offset;
      MoveBytes ((addrtype )&FuncPtr, BytePtr, ByteCount);
   }

TRACK(TrackBak,"MemPushFuncPtr\n");
return(STAT);
}
 
boolean    MemPutFuncPtr  (numtype     Chunk,     indextype      Index,
                           funcptrtype FuncPtr,   ft F,lt Z,zz  *Status)
{
boolean    PushFlag  = False;
addrtype   BytePtr   = NullPtr;
indextype  StackPtr  = 0;
indextype  ByteCount = sizeof (funcptrtype);


   if (ChkPutChunk     (Chunk, Index,    ByteCount, McStat, &PushFlag))
   if (PushFlag)
      MemPushFuncPtr   (Chunk, FuncPtr,             McStat, &StackPtr);

   else {
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + (ByteCount * Index);

      MoveBytes        ((addrtype )&FuncPtr,        BytePtr, ByteCount);
     *Status   = Env_Normal;
   }

TRACK(TrackBak,"MemPutFuncPtr\n");
return(STAT);
}
 
boolean    MemPopFuncPtr  (numtype      Chunk,     ft F,lt Z,zz *Status,
                           funcptrtype *FuncPtr,   indextype    *StackPtr)
{
indextype  ByteCount = sizeof (funcptrtype);
addrtype   BytePtr   = NullPtr;

   if (ChkPopChunk (Chunk, ByteCount, McStat))
   {
     *StackPtr = --Stack_Ptr(Chunk);
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + *StackPtr;
      MoveBytes (BytePtr, (addrtype )&FuncPtr,    ByteCount);
   }

TRACK(TrackBak,"MemPopFuncPtr\n");
return(STAT);
}
 
boolean    MemGetFuncPtr  (numtype       Chunk,  indextype    Index,
                           ft F,lt Z,zz *Status, funcptrtype *FuncPtr)
{
indextype  ByteCount = sizeof (funcptrtype);
addrtype   BytePtr   = NullPtr;

   if (ChkGetChunk (Chunk, Index, ByteCount, McStat))
   if (*Status != Set_EndOfSet)
   {
      BytePtr  = ((char *)(Chunk_Addr(Chunk))) + (ByteCount * Index);
      MoveBytes (BytePtr, (addrtype )FuncPtr,    ByteCount);
      *Status  = Env_Normal;
   }

TRACK(TrackBak,"MemGetFuncPtr\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+*/
 
boolean    MemNewString     (numtype       Chunk,     char         *String,    
                             ft F,lt Z,zz *Status,    indextype    *StackPtr)
{
sizeinbytes   ByteCount  = 0;
struchdr     *StrucHdr   = NullPtr;
stringstruc  *CurrStruc  = NullPtr;
indextype     NextGap    = 0;
boolean       GapFound   = False;

   StrucHdr    = (struchdr *)Chunk_Addr(Chunk);
   ByteCount   = strlen(String) + 1;

   if ((NextGap    =  StrucHdr->FirstFreeGap) > 0)
   {
      GapFound     = True;
     *StackPtr     = NextGap;

      CurrStruc    = (stringstruc *)(  (char *)Chunk_Addr(Chunk)
                                     + (sizeof (stringstruc) * NextGap) );
      StrucHdr->FirstFreeGap = CurrStruc->NextFreeGap;
   }

   if (!GapFound)
     MemPushString (Chunk,  String,   McStat,   StackPtr);
   else
     MemPutString  (Chunk, *StackPtr, String,   McStat);

TRACK(TrackBak,"MemNewString   \n");
return(STAT);
}
 

boolean    MemDeleteString  (numtype       Chunk,     indextype   Index,
                             ft F,lt Z,zz *Status)
{
indextype     StackPtr   = 0;
sizeinbytes   ByteCount  = 0;
struchdr     *StrucHdr   = NullPtr;
stringstruc  *StrStruc   = NullPtr;
stringstruc  *StrStruc0  = NullPtr;
sizetype      TextOffset = 0;

   StrucHdr    = (struchdr *)Chunk_Addr(Chunk);
   StrStruc    = (stringstruc *)( (char *)Chunk_Addr(Chunk)
                          + (sizeof (stringstruc) * Index) );
   ByteCount   = StrStruc->TextSize;

   StrStruc0   = StrStruc;
   TextOffset  = StrStruc->TextOffset;

   if (MemExtractEntity  (StrucHdr->TextChunk, StrStruc->TextOffset,  
                          StrStruc->TextSize,            McStat))

   if (Stack_Ptr(Chunk))
   {
      for (StackPtr    = (indextype )Stack_Ptr(Chunk), ++StrStruc;
           StackPtr--  > 0;       ++StrStruc)
      {
         if (StrStruc->TextOffset  > TextOffset)
             StrStruc->TextOffset -= TextOffset;
      }
   }
   
   StrStruc0->TextOffset   = 0;
   StrStruc0->TextSize     = 0;
   StrStruc0->NextFreeGap  = StrucHdr->FirstFreeGap;

   StrucHdr->FirstFreeGap  = StackPtr;

TRACK(TrackBak,"MemDeleteString\n");
return(STAT);
}


boolean    MemPushString    (numtype       Chunk,     char         *String,    
                             ft F,lt Z,zz *Status,    indextype    *StackPtr)
{
sizeinbytes   ByteCount  = 0;
sizeinbytes   StrSize    = 0;
stringstruc  *StrStruc   = NullPtr;
numtype       TextChunk  = 0;
char         *TextPtr    = NullPtr;
struchdr     *StrucHdr   = NullPtr;

   if (ChkPushChunk (Chunk, sizeof (stringstruc), McStat))
   {
      StrucHdr  = (struchdr *)Chunk_Addr(Chunk);

     *StackPtr  = (indextype )Stack_Ptr(Chunk)++;

      StrStruc  = (stringstruc *)( ((char *)(Chunk_Addr(Chunk))) 
                                   + (sizeof (stringstruc) * *StackPtr) );
      StrSize   = strlen (String) + 1;
      if (StrSize > MaxUnit_Length(StrucHdr->TextChunk))
      {
        StrSize = MaxUnit_Length(StrucHdr->TextChunk);
        String[StrSize] = '\0';
      }

      ByteCount = StrSize;
      StrStruc->TextSize = ByteCount;

      TextChunk = StrucHdr->TextChunk;     
 
      if (ByteCount  > 1)
      {
	 AlignStruc((sizetype *) &ByteCount); 
         StrStruc->TextSize = ByteCount;

         if (MemPushEntity (TextChunk, ByteCount, String,
                            McStat,              &StrStruc->TextOffset))
         {
            TextPtr = ((char *)(Chunk_Addr(TextChunk))) + StrStruc->TextOffset;
            MoveBytes (String, TextPtr, StrSize);
         }

      } else
         StrStruc->TextOffset = 0;
   }

TRACK(TrackBak,"MemPushString\n");
return(STAT);
}
 

boolean    MemPutString     (numtype       Chunk,  indextype     Index,
                             char         *String, ft F,lt Z,zz *Status)
{
boolean       PushFlag   = False;
indextype     StackPtr   = 0;
sizeinbytes   ByteCount  = 0;
stringstruc  *StrStruc   = NullPtr;
numtype       TextChunk  = 0;
struchdr     *StrucHdr   = NullPtr;
sizetype      StrSize    = 0;
sizetype      TextSize   = 0;
indextype     TextOffset = 0;

   if (ChkPutChunk   (Chunk, Index,  sizeof (stringstruc), McStat, &PushFlag))
   if (PushFlag)
   {
      MemPushString  (Chunk, String,   McStat,  &StackPtr);

   } else {
      StrucHdr  = (struchdr *)Chunk_Addr(Chunk);

      StrStruc  = (stringstruc *)( ((char *)(Chunk_Addr(Chunk))) 
                                   + (sizeof (stringstruc) * Index) );

      TextChunk = StrucHdr->TextChunk;     

      StrSize = strlen (String) + 1;

      if ((ByteCount = StrSize) > 1 )
	 AlignStruc((sizetype *) &ByteCount) ;

      if ( ByteCount > StrStruc->TextSize )
      {
         TextSize         = StrStruc->TextSize;
         TextOffset       = StrStruc->TextOffset;

         if (MemExtractEntity  (TextChunk, TextOffset,  TextSize, McStat))

         if (MemPushEntity     (TextChunk, ByteCount, String,
                                McStat,                &StrStruc->TextOffset))
         for (StackPtr    = (indextype )Stack_Ptr(Chunk), ++StrStruc;
              StackPtr--  > 0;       ++StrStruc)
         {
           if (StrStruc->TextOffset  > TextOffset)
               StrStruc->TextOffset -= TextOffset;
         }
      }
   }

TRACK(TrackBak,"MemPutString\n");
return(STAT);
}
 

boolean    MemPopString     (numtype       Chunk,     ft F,lt Z,zz *Status,   
                             char        **String,    indextype    *StackPtr)
{
sizeinbytes   ByteCount  = 0;
stringstruc  *StrStruc   = NullPtr;
numtype       TextChunk  = 0;
char         *TextPtr    = NullPtr;
struchdr     *StrucHdr   = NullPtr;

   if (ChkPopChunk (Chunk, sizeof(stringstruc), McStat))
   {
      StrucHdr  = (struchdr *)Chunk_Addr(Chunk);
      *StackPtr = --Stack_Ptr(Chunk);
      StrStruc  = (stringstruc *)( ((char *)(Chunk_Addr(Chunk))) 
                                   + (sizeof (stringstruc) * *StackPtr) );
      ByteCount = StrStruc->TextSize;

      Core0_AllocString (ByteCount, McStat, String);

      TextChunk = StrucHdr->TextChunk;     
      TextPtr   = ((char *)(Chunk_Addr(TextChunk))) + StrStruc->TextOffset;

      if (Normal(*Status))
         MoveBytes   (TextPtr, *String, ByteCount);
   }

TRACK(TrackBak,"MemPopString\n");
return(STAT);
}
 

boolean    MemGetString     (numtype       Chunk,       indextype   Index,
                             ft F,lt Z,zz *Status,      char      **String)
{
sizeinbytes   ByteCount  = 0;
stringstruc  *StrStruc   = NullPtr;
numtype       TextChunk  = 0;
char         *TextPtr    = NullPtr;
struchdr     *StrucHdr   = NullPtr;

   if (Chunk)
   if (ChkGetChunk (Chunk, Index, sizeof (stringstruc), McStat))
   if (*Status != Set_EndOfSet)
   if (Index    > 0)
   { 
     *Status    = Env_Normal;
      StrucHdr  = (struchdr *)Chunk_Addr(Chunk);
      StrStruc  = (stringstruc *)( ((char *)(Chunk_Addr(Chunk))) 
                                   + (sizeof (stringstruc) * Index) );
      ByteCount = StrStruc->TextSize;
      if (!ByteCount)
         ByteCount = 1;

      Core0_AllocString (ByteCount, McStat, String);

      TextChunk = StrucHdr->TextChunk;     
      TextPtr = ((char *)(Chunk_Addr(TextChunk))) + StrStruc->TextOffset;

      if (Normal(*Status))
      if ( ByteCount > 1 )
         MoveBytes (TextPtr, *String, ByteCount);
      else
         strcpy (*String, "\0");

   } else {
      if (Core0_AllocString (1, McStat, String))
         strcpy (*String, "\0");
   }

TRACK(TrackBak,"MemGetString\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF MEM10.C                                 !
  +-----------------------------------------------------------------------+*/
