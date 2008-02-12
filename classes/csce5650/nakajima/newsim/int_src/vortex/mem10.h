/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef MEM10_H
#define MEM10_H

/* ENV-MEM10.h  */
/*+-----------------------------------------------------------------------+
~P                           PROCEDURES                                   !
  +-----------------------------------------------------------------------+*/
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
#define MemExtractEntity       Mem_ExtractEntity
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

       boolean MemFreezeChunk (numtype       Chunk,     ft F,lt Z,zz *Status);

       boolean MemDefrostChunk(numtype       Chunk,     ft F,lt Z,zz *Status);


       boolean MemGetChunkSize(numtype       Chunk,
                               ft F,lt Z,zz *Status,   indextype *UnitCount,
                               bytessize *UnitSize);
       boolean MemGetChunkStruc
                              (numtype       Chunk,     ft F,lt Z,zz *Status,
                               chunkstruc   *ChunkRef);

       boolean MemDumpChunkStruc
                              (chunkstruc   *ChunkRef,  ft F,lt Z,zz *Status);


       boolean MemPutChunkAddr(numtype       Chunk,     addrtype  ChunkAddr,
                               ft F,lt Z,zz *Status);

       boolean MemGetChunkAddr(numtype       Chunk,
                               ft F,lt Z,zz *Status,    addrtype *ChunkAddr);


       boolean MemPutStackPtr (numtype       Chunk,     indextype StackPtr,
                               ft F,lt Z,zz *Status);

       boolean Mem_AssertStackPtr
                              (numtype       Chunk,     indextype  StackPtr,
                               ft F,lt Z,zz *Status,    addrtype  *ChunkAddr);

       boolean MemGetStackPtr (numtype       Chunk,
                               ft F,lt Z,zz *Status,    indextype *StackPtr);


       boolean MemPushEntity  (numtype       Chunk,    bytessize  ByteCount,
                               addrtype      Entity,
                               ft F,lt Z,zz *Status,   indextype *ByteOffset);

       boolean MemPutEntity   (numtype       Chunk,    indextype  ByteOffset,
                               bytessize     ByteCount, addrtype  Entity,
                               ft F,lt Z,zz *Status);

       boolean MemExtractEntity
                              (numtype       Chunk,    indextype     Offset, 
                               bytessize     Count,    ft F,lt Z,zz *Status);

       boolean MemGetEntity   (numtype       Chunk,    indextype  ByteOffset,
                               bytessize     ByteCount,
                               ft F,lt Z,zz *Status,   addrtype  *Entity);
 

       boolean MemNewBitChunk (numtype       BitWidth, idtype     CoreDbId,
                               ft F,lt Z,zz *Status,   numtype   *BitChunk);

       boolean MemPushBit     (numtype       Chunk,    word       BitValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemPutBit      (numtype       Chunk,    indextype  Index,
                               word          BitValue, ft F,lt Z,zz *Status);

       boolean MemPopBit      (numtype       Chunk,    ft F,lt Z,zz *Status,
                               word         *BitValue, indextype *StackPtr);

       boolean MemGetBit      (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   word      *BitValue);


       boolean MemPushShort   (numtype       Chunk,    shorttype  ShortValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemPutShort    (numtype       Chunk,    indextype  Index,
                               shorttype     ShortVal, ft F,lt Z,zz *Status);

       boolean MemPopShort    (numtype       Chunk,    ft F,lt Z,zz *Status,
                               shorttype    *ShortVal, indextype *StackPtr);

       boolean MemGetShort    (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   shorttype *ShortValue);
 

       boolean MemPushWord    (numtype       Chunk,    word       WordValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemPutWord     (numtype       Chunk,    indextype  Index,
                               word          WordValue,ft F,lt Z,zz *Status);

       boolean MemPopWord     (numtype       Chunk,    ft F,lt Z,zz *Status,
                               word         *WordValue,indextype *StackPtr);

       boolean MemGetWord     (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   word      *WordValue);
 

       boolean MemPushLong    (numtype       Chunk,    lt64       LongValue,
                               ft F,lt Z,zz *Status,   indextype *StackPtr);

       boolean MemPutLong     (numtype       Chunk,    indextype  Index,
                               lt64          LongValue,ft F,lt Z,zz *Status);

       boolean MemPopLong     (numtype       Chunk,    ft F,lt Z,zz *Status,
                               lt64         *LongValue,indextype *StackPtr);

       boolean MemGetLong     (numtype       Chunk,    indextype  Index,
                               ft F,lt Z,zz *Status,   lt64      *LongValue);
 

       boolean MemPushAddr    (numtype       Chunk,   addrtype    Addr,
                               ft F,lt Z,zz *Status,  indextype  *StackPtr);

       boolean MemPutAddr     (numtype       Chunk,   indextype   Index,
                               addrtype      Addr,    ft F,lt Z,zz *Status);

       boolean MemPopAddr     (numtype       Chunk,   ft F,lt Z,zz *Status,
                               addrtype     *Addr,    indextype  *StackPtr);

       boolean MemGetAddr     (numtype       Chunk,   indextype   Index,
                               ft F,lt Z,zz *Status,  addrtype   *Addr);
 

       boolean MemPushToken   (numtype       Chunk,   tokentype  *Token,
                               ft F,lt Z,zz *Status,  indextype  *StackPtr);

       boolean MemPutToken    (numtype       Chunk,   indextype   Index,
                               tokentype    *Token,   ft F,lt Z,zz *Status);

       boolean MemPopToken    (numtype       Chunk,   ft F,lt Z,zz *Status,
                               tokentype    *Token,   indextype  *StackPtr);

       boolean MemGetToken    (numtype       Chunk,   indextype   Index,
                               ft F,lt Z,zz *Status,  tokentype  *Token);


       boolean MemPushSomeBytes
                              (numtype       Chunk,     bytessize   ByteCount,
                               addrtype      SomeBytes,    
                               ft F,lt Z,zz *Status,    indextype  *StackPtr);

       boolean MemPutSomeBytes(numtype       Chunk,     indextype   Index,
                               bytessize     ByteCount, addrtype    SomeBytes,
                               ft F,lt Z,zz *Status);


       boolean MemPopSomeBytes(numtype       Chunk,     bytessize   ByteCount,
                               ft F,lt Z,zz *Status,   
                               addrtype      SomeBytes, indextype  *StackPtr);

       boolean MemGetSomeBytes(numtype       Chunk,     indextype   Index,
                               bytessize     ByteCount,
                               ft F,lt Z,zz *Status,    addrtype    SomeBytes);

 

       boolean MemPushFuncPtr (numtype       Chunk,   funcptrtype   FuncPtr,
                               ft F,lt Z,zz *Status,  indextype    *StackPtr);

       boolean MemPopFuncPtr  (numtype       Chunk,   ft F,lt Z,zz *Status,
                               funcptrtype  *FuncPtr, indextype    *StackPtr);


       boolean MemPutFuncPtr  (numtype       Chunk,   indextype     Index,
                               funcptrtype   FuncPtr, ft F,lt Z,zz *Status);

       boolean MemGetFuncPtr  (numtype       Chunk,   indextype     Index,
                               ft F,lt Z,zz *Status,  funcptrtype  *FuncPtr);

       boolean MemNewString   (numtype       Chunk,     char       *String,    
                               ft F,lt Z,zz *Status,    indextype  *StackPtr);

       boolean MemDeleteString(numtype       Chunk,     indextype   StackPtr,
                               ft F,lt Z,zz *Status);

       boolean MemPushString  (numtype       Chunk,     char       *String,    
                               ft F,lt Z,zz *Status,    indextype  *StackPtr);

       boolean MemPutString   (numtype       Chunk,     indextype   Index,
                               char         *String,    ft F,lt Z,zz *Status);

       boolean MemPopString   (numtype       Chunk,     ft F,lt Z,zz *Status,   
                               char        **String,    indextype  *StackPtr);

       boolean MemGetString   (numtype       Chunk,     indextype   Index,
                               ft F,lt Z,zz *Status,    char      **String);

/*+-----------------------------------------------------------------------+
~P                         END OF MEM10.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
