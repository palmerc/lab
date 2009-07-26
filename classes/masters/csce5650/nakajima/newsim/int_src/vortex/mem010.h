/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef MEM01_H
#define MEM01_H

/* ENV-MEM01.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
typedef  boolean         bitfield;
typedef  byte            chunktype;
typedef  numtype         numchunk;
typedef  numtype         indexchunk;
typedef  numtype         indexhead;
typedef  numtype         addrchunk;
typedef  numtype         addrhead;
typedef  unsigned short  chunkflags;       

typedef  struct   TypeStrucHdr
{
        numtype       TextChunk;
        vbntype       TextVbn;
        indextype     FirstFreeGap;
} struchdr;

typedef  struct   TypeStringStruc  
{
       indextype      TextOffset;
       sizetype       TextSize;
       indextype      NextFreeGap;
} stringstruc;

typedef  struct   TypeVaryStruc
{
       indextype      ChunkOffset;
       sizetype       VarySize;
       indextype      NextFreeGap;
       typetype       VaryType;
} varystruc;


/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
#define NewChunkChunk          Mem_NewChunkChunk 
#define DumpChunkChunk         Mem_DumpChunkChunk 

#define MemNewChunkBlk         Mem_NewChunkBlk
#define MemPutChunkStruc       Mem_PutChunkStruc

#define MemMakeChunk           Mem_MakeChunk
#define MemMakeCppChunk        Mem_MakeCppChunk
#define MemMakeStrChunk        Mem_MakeStrChunk 

#define MemDumpChunk           Mem_DumpChunk   
#define MemFreeChunk           Mem_FreeChunk   
#define MemFreeChunkNum        Mem_FreeChunkNum
#define MemClearChunkSpace     Mem_ClearChunkSpace
#define MemFreeChunkSpace      Mem_FreeChunkSpace

#define MemSwapOutChunk        Mem_SwapOutChunk
#define MemSwapInChunk         Mem_SwapInChunk 
#define MemCopyOutChunk        Mem_CopyOutChunk
#define MemCopyInChunk         Mem_CopyInChunk 

#define MemGetFileBlk          Mem_GetFileBlk    
#define MemFreeFileBlk         Mem_FreeFileBlk    
#define MemNewXmemBlk          Mem_NewXmemBlk    
#define MemFreeXmemBlk         Mem_FreeXmemBlk   
#define MemGetFreeXmemVbn      Mem_GetFreeXmemVbn   
#define MemGetFreeXmemBlk      Mem_GetFreeXmemBlk   

#define MemFreezeChunk         Mem_FreezeChunk 
#define MemDefrostChunk        Mem_DefrostChunk

#define MemGetChunkSize        Mem_GetChunkSize
#define MemGetChunkStruc       Mem_GetChunkStruc
#define MemDumpChunkStruc      Mem_DumpChunkStruc
#define MemPutChunkAddr        Mem_PutChunkAddr
#define MemGetChunkAddr        Mem_GetChunkAddr

#define MemPutStackPtr         Mem_PutStackPtr 
#define MemGetStackPtr         Mem_GetStackPtr 

#define MemPushEntity          Mem_PushEntity
#define MemPutEntity           Mem_PutEntity   
#define MemPopEntity           Mem_PopEntity
#define MemGetEntity           Mem_GetEntity   

#define MemNewBitChunk         Mem_NewBitChunk 
#define MemPushBit             Mem_PushBit     
#define MemPutBit              Mem_PutBit 
#define MemPopBit              Mem_PopBit
#define MemGetBit              Mem_GetBit

#define MemPushShort           Mem_PushShort   
#define MemPutShort            Mem_PutShort    
#define MemPopShort            Mem_PopShort    
#define MemGetShort            Mem_GetShort    

#define MemPushWord            Mem_PushWord    
#define MemPutWord             Mem_PutWord     
#define MemPopWord             Mem_PopWord     
#define MemGetWord             Mem_GetWord     

#define MemPushLong            Mem_PushLong    
#define MemPutLong             Mem_PutLong     
#define MemPopLong             Mem_PopLong     
#define MemGetLong             Mem_GetLong     

#define MemPushAddr            Mem_PushAddr    
#define MemPutAddr             Mem_PutAddr     
#define MemPopAddr             Mem_PopAddr     
#define MemGetAddr             Mem_GetAddr     

#define MemPushToken           Mem_PushToken   
#define MemPutToken            Mem_PutToken    
#define MemPopToken            Mem_PopToken    
#define MemGetToken            Mem_GetToken    

#define MemPushFuncPtr         Mem_PushFuncPtr 
#define MemPutFuncPtr          Mem_PutFuncPtr  
#define MemPopFuncPtr          Mem_PopFuncPtr  
#define MemGetFuncPtr          Mem_GetFuncPtr  

#define MemPushSomeBytes       Mem_PushSomeBytes
#define MemPutSomeBytes        Mem_PutSomeBytes
#define MemAssertPutBytes      Mem_AssertPutBytes
#define MemPopSomeBytes        Mem_PopSomeBytes
#define MemGetSomeBytes        Mem_GetSomeBytes
#define MemAssertGetBytes      Mem_AssertGetBytes

#define MemNewString           Mem_NewString   
#define MemPushString          Mem_PushString  
#define MemPutString           Mem_PutString   
#define MemPopString           Mem_PopString   
#define MemGetString           Mem_GetString   
#define MemDeleteString        Mem_DeleteString

       boolean Mem_NewChunkChunk 
                         (ft F,lt Z,zz *Status,    numtype  *ChunkNum);


       boolean Mem_NewChunkBlk
                         (sizeinbytes   BlkSize,   ft F,lt Z,zz *Status,
                          numtype      *ChunkNum,  addrtype *ChunkBlk);

       boolean Mem_PutChunkStruc
                         (numtype       Chunk,    chunkstruc   *ChunkRef,
                          idtype        CoreDbId, ft F,lt Z,zz *Status);

       boolean Mem_FreeChunkNum
                         (numtype      *ChunkNum,  ft F,lt Z,zz *Status);

       boolean Mem_ClearChunkSpace
                         (numtype       Chunk,     ft F,lt Z,zz *Status);

       boolean Mem_FreeChunkSpace
                         (numtype       Chunk,     ft F,lt Z,zz *Status);

       boolean Mem_FreeXmemBlk
                         (ft F,lt Z,zz *Status,    vbntype  *Vbn);

       boolean Mem_NewXmemBlk
                         (bytessize     SizeInBytes,
                          ft F,lt Z,zz *Status,
                          vbntype      *Vbn,       lt64     *XmemOffset);

       boolean Mem_GetFreeXmemVbn
                         (ft F,lt Z,zz *Status,    vbntype  *Vbn);

       boolean Mem_GetFreeXmemBlk
                         (bytessize     SizeInBytes, 
                          ft F,lt Z,zz *Status,   indextype *BlkIndex);

       boolean Mem_PutChunkAddr
                         (numtype       Chunk,     addrtype  ChunkAddr,
                          ft F,lt Z,zz *Status);


/*     3.1.1                                                               */
       boolean Mem_MakeChunk
                         (bytessize     UnitSize,  indextype     AllocQty,
                          indextype     ExtendQty, idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    numtype      *Chunk);

/*     3.1.2                                                               */
       boolean Mem_MakeCppChunk
                         (bytessize     CppObjSize,indextype     AllocQty,
                          indextype     ExtendQty, idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    numtype      *Chunk);

/*     3.1.3                                                               */
       boolean Mem_MakeStrChunk 
                         (bytessize     MaxLength, indextype     NumStrings,
                          indextype     ExtendQty, bytessize     TextLength,
                          idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    numtype      *Chunk);
 
/*     3.1.4                                                               */
       void    Mem_DumpChunkChunk 
                         (int           Dump,      numtype       Chunk);



/*     3.1.5                                                               */
       boolean Mem_SwapOutChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *Vbn);

/*     3.1.6                                                               */
       boolean Mem_SwapInChunk 
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *Vbn);

/*     3.1.7                                                               */
       boolean Mem_CopyOutChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *SwapVbn);

/*     3.1.8                                                               */
       boolean Mem_CopyInChunk 
                         (numtype       Chunk,     numtype       SwapVbn,
                          addrtype      SwapAddr,  ft F,lt Z,zz *Status);

/*     3.1.8.1                                                             */
       boolean Mem_MoveChunk 
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          addrtype     *FreeAddr,  sizeinbytes  *FreeSize);


/*     3.1.9                                                               */
       boolean Mem_FreezeChunk 
                         (numtype       Chunk,     ft F,lt Z,zz *Status);

/*     3.1.10                                                              */
       boolean Mem_DefrostChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status);



/*     3.1.11.0                                                            */
       boolean Mem_DumpChunk   
                         (int           MsgLevel,  numtype       Chunk,
                          ft F,lt Z,zz *Status);
 
/*     3.1.11.1                                                            */
       boolean Mem_DumpChunkPart   
                         (int           MsgLevel,  numtype       Chunk,
                          numtype       Ilow,      numtype       Ihigh,
                          ft F,lt Z,zz *Status);
 
/*     3.1.12                                                              */
       boolean Mem_FreeChunk   
                         (numtype      *ChunkNum,  ft F,lt Z,zz *Status);



/*     3.1.13                                                              */
       boolean Mem_GetChunkSize
                         (numtype       Chunk,
                          ft F,lt Z,zz *Status,    indextype    *UnitCount,
                          bytessize    *UnitSize);

/*     3.1.14                                                              */
       boolean Mem_GetChunkAddr
                         (numtype       Chunk,
                          ft F,lt Z,zz *Status,    addrtype     *ChunkAddr);

/*     3.1.15                                                              */
       boolean Mem_GetStackPtr 
                         (numtype       Chunk,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.1.16                                                              */
       boolean Mem_PutStackPtr 
                         (numtype       Chunk,     indextype     StackPtr,
                          ft F,lt Z,zz *Status);

/*     3.1.16.1                                                            */
       boolean Mem_AssertStackPtr 
                         (numtype       Chunk,     indextype     StackPtr,
                          ft F,lt Z,zz *Status,    addrtype     *ChunkAddr);

/*     3.1.17                                                              */
       boolean Mem_GetChunkStruc
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          chunkstruc   *ChunkRef);

/*     3.1.18                                                              */
       boolean Mem_DumpChunkStruc
                         (chunkstruc   *ChunkRef,  ft F,lt Z,zz *Status);



/*     3.2.1.0                                                             */
       boolean Mem_NewBitChunk 
                         (numtype       BitWidth,  idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    numtype      *BitChunk);

/*     3.2.1.1                                                             */
       boolean Mem_PushBit     
                         (numtype       Chunk,     word          BitValue,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.1.2                                                             */
       boolean Mem_PutBit 
                         (numtype       Chunk,     indextype     Index,
                          word          BitValue,  ft F,lt Z,zz *Status);

/*     3.2.1.3                                                             */
       boolean Mem_PopBit
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          word         *BitValue,  indextype    *StackPtr);

/*     3.2.1.4                                                             */
       boolean Mem_GetBit
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    word         *BitValue);



/*     3.2.2.1                                                             */
       boolean Mem_PushShort   
                         (numtype       Chunk,     shorttype     ShortValue,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.2.2                                                             */
       boolean Mem_PutShort    
                         (numtype       Chunk,     indextype     Index,
                          shorttype     ShortVal,  ft F,lt Z,zz *Status);

/*     3.2.2.3                                                             */
       boolean Mem_PopShort    
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          shorttype    *ShortVal,  indextype    *StackPtr);

/*     3.2.2.4                                                             */
       boolean Mem_GetShort    
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    shorttype    *ShortValue);
 


/*     3.2.3.1                                                             */
       boolean Mem_PushWord    
                         (numtype       Chunk,     word          WordValue,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.3.2                                                             */
       boolean Mem_PutWord     
                         (numtype       Chunk,     indextype     Index,
                          word          WordValue, ft F,lt Z,zz *Status);

/*     3.2.3.3                                                             */
       boolean Mem_PopWord     
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          word         *WordValue, indextype    *StackPtr);

/*     3.2.3.4                                                             */
       boolean Mem_GetWord     
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    word         *WordValue);
 


/*     3.2.4.1                                                             */
       boolean Mem_PushLong    
                         (numtype       Chunk,     lt64          LongValue,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.4.2                                                             */
       boolean Mem_PutLong     
                         (numtype       Chunk,     indextype     Index,
                          lt64          LongValue, ft F,lt Z,zz *Status);

/*     3.2.4.3                                                             */
       boolean Mem_PopLong     
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          lt64         *LongValue, indextype    *StackPtr);

/*     3.2.4.4                                                             */
       boolean Mem_GetLong     
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    lt64         *LongValue);
 


/*     3.2.5.1                                                             */
       boolean Mem_PushAddr    
                         (numtype       Chunk,     addrtype      Addr,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.5.2                                                             */
       boolean Mem_PutAddr     
                         (numtype       Chunk,     indextype     Index,
                          addrtype      Addr,      ft F,lt Z,zz *Status);

/*     3.2.5.3                                                             */
       boolean Mem_PopAddr     
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          addrtype     *Addr,      indextype    *StackPtr);

/*     3.2.5.4                                                             */
       boolean Mem_GetAddr     
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    addrtype     *Addr);
 


/*     3.2.6.1                                                             */
       boolean Mem_PushToken   
                         (numtype       Chunk,     tokentype    *Token,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.6.2                                                             */
       boolean Mem_PutToken    
                         (numtype       Chunk,     indextype     Index,
                          tokentype    *Token,     ft F,lt Z,zz *Status);

/*     3.2.6.3                                                             */
       boolean Mem_PopToken    
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          tokentype    *Token,     indextype    *StackPtr);

/*     3.2.6.4                                                             */
       boolean Mem_GetToken    
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    tokentype    *Token);



/*     3.2.7.1                                                             */
       boolean Mem_PushFuncPtr 
                         (numtype       Chunk,     funcptrtype   FuncPtr,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.7.2                                                             */
       boolean Mem_PutFuncPtr  
                         (numtype       Chunk,     indextype     Index,
                          funcptrtype   FuncPtr,   ft F,lt Z,zz *Status);

/*     3.2.7.3                                                             */
       boolean Mem_PopFuncPtr  
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          funcptrtype  *FuncPtr,   indextype    *StackPtr);

/*     3.2.7.4                                                             */
       boolean Mem_GetFuncPtr  
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    funcptrtype  *FuncPtr);
 


/*     3.2.8.0                                                             */
       boolean Mem_NewString   
                         (numtype       Chunk,     char         *String,    
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.8.1                                                             */
       boolean Mem_PushString  
                         (numtype       Chunk,     char         *String,    
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.8.2                                                             */
       boolean Mem_PutString   
                         (numtype       Chunk,     indextype     Index,
                          char         *String,    ft F,lt Z,zz *Status);

/*     3.2.8.3                                                             */
       boolean Mem_PopString   
                         (numtype       Chunk,     ft F,lt Z,zz *Status,   
                          char        **String,    indextype    *StackPtr);

/*     3.2.8.4                                                             */
       boolean Mem_GetString   
                         (numtype       Chunk,     indextype     Index,
                          ft F,lt Z,zz *Status,    char        **String);

/*     3.2.8.5                                                             */
       boolean Mem_DeleteString
                         (numtype       Chunk,     indextype     StackPtr,
                          ft F,lt Z,zz *Status);



/*     3.2.9.1                                                             */
       boolean Mem_PushSomeBytes
                              
                         (numtype       Chunk,     bytessize     ByteCount,
                          addrtype      SomeBytes,    
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/*     3.2.9.2                                                             */
       boolean Mem_PutSomeBytes
                         (numtype       Chunk,     indextype     Index,
                          bytessize     ByteCount, addrtype      SomeBytes,
                          ft F,lt Z,zz *Status);

/*     3.2.9.2.1                                                           */
       boolean Mem_AssertPutBytes
                         (numtype       Chunk,     indextype     Index,
                          bytessize     ByteCount,
                          ft F,lt Z,zz *Status,    addrtype     *BytePtr);

/*     3.2.9.3                                                             */
       boolean Mem_PopSomeBytes
                         (numtype       Chunk,     bytessize     ByteCount,
                          ft F,lt Z,zz *Status,   
                          addrtype      SomeBytes, indextype    *StackPtr);

/*     3.2.9.4                                                             */
       boolean Mem_GetSomeBytes
                         (numtype       Chunk,     indextype     Index,
                          bytessize     ByteCount,
                          ft F,lt Z,zz *Status,    addrtype      SomeBytes);

/*     3.2.9.4.1                                                           */
       boolean Mem_AssertGetBytes
                         (numtype       Chunk,     indextype     Index,
                          bytessize     ByteCount,
                          ft F,lt Z,zz *Status,    addrtype     *BytePtr);
 


/*     3.2.10.1                                                            */
       boolean Mem_PushEntity
                         (numtype       Chunk,    bytessize      ByteCount,
                          addrtype      Entity,
                          ft F,lt Z,zz *Status,   indextype     *ByteOffset);

/*     3.2.10.2                                                            */
       boolean Mem_PutEntity   
                         (numtype       Chunk,     indextype     ByteOffset,
                          bytessize     ByteCount, addrtype      Entity,
                          ft F,lt Z,zz *Status);

/*     3.2.10.3                                                            */
       boolean MemExtractEntity
                         (numtype       Chunk,   indextype     ByteOffset, 
                          bytessize     Count,   ft F,lt Z,zz *Status);

/*     3.2.10.4                                                            */
       boolean Mem_GetEntity   
                         (numtype       Chunk,     indextype     ByteOffset,
                          bytessize     ByteCount,
                          ft F,lt Z,zz *Status,    addrtype     *Entity);


/*     3.3.1                                                               */
       void    Mem_ShowStats
                         (void);

 
/*+-----------------------------------------------------------------------+
~P                         END OF MEM00.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
