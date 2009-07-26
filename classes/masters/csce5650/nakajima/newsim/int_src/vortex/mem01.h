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

#ifdef MEM01

static   FilePtrType        XmemFilePtr       = NULL;
static   farlongtype        XmemSize          = 0;
                                                  
static   indextype          XmemHeadFreeVbns  = 0;
static   numtype            XmemBlkDirChunk   = 0;

static   indextype          XmemHeadFreeBlks  = 0;
static   indextype          XmemHeadNullBlks  = 0;
static   numtype            XmemBlkTknChunk   = 0;
static   numtype            XmemBlkSizeChunk  = 0;

static   indextype         *XmemBlkDir        = NullPtr;
static   blktkntype        *XmemBlkTkn        = NullPtr;
static   bytessize         *XmemBlkSize       = NullPtr;

static   FILE              *MemBlkFile        = NullPtr;

static   indextype          MemHeadFreeVbns   = 0;
static   numtype            MemBlkDirChunk    = 0;

static   indextype          MemHeadFreeBlks   = 0;
static   indextype          MemHeadNullBlks   = 0;
static   numtype            MemBlkTknChunk    = 0;
static   numtype            MemBlkSizeChunk   = 0;

static   indextype         *MemBlkDir         = NullPtr;
static   blktkntype        *MemBlkTkn         = NullPtr;
static   bytessize         *MemBlkSize        = NullPtr;

#endif

/* ENV-MEM01.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
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

       boolean MemMakeChunk   
                         (bytessize     UnitSize,  indextype AllocQty,
                          indextype     ExtendQty, idtype    CoreDbId,
                          ft F,lt Z,zz *Status,    numtype  *Chunk);

       boolean MemMakeCppChunk
                         (bytessize     CppObjSize,indextype AllocQty,
                          indextype     ExtendQty, idtype    CoreDbId,
                          ft F,lt Z,zz *Status,    numtype  *Chunk);

       boolean MemMakeStrChunk 
                         (bytessize     MaxLength, indextype NumStrings,
                          indextype     ExtendQty, bytessize TextLength,
                          idtype        CoreDbId,
                          ft F,lt Z,zz *Status,    numtype  *Chunk);
 
       boolean MemDumpChunk   
                         (int           MsgLevel,  numtype   Chunk,
                          ft F,lt Z,zz *Status);
 
       boolean Mem_DumpChunkPart   
                         (int           MsgLevel,  numtype       Chunk,
                          numtype       Ilow,      numtype       Ihigh,
                          ft F,lt Z,zz *Status);

       boolean MemFreeChunk 
                         (numtype      *ChunkNum,  ft F,lt Z,zz *Status);

       boolean MemFreeChunkNum
                         (numtype      *ChunkNum,  ft F,lt Z,zz *Status);

       boolean MemClearChunkSpace
                         (numtype       Chunk,     ft F,lt Z,zz *Status);

       boolean MemFreeChunkSpace
                         (numtype       Chunk,     ft F,lt Z,zz *Status);


       boolean MemSwapOutChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *Vbn);

       boolean MemSwapInChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *Vbn);


       boolean MemCopyOutChunk
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          vbntype      *SwapVbn);

       boolean MemCopyInChunk 
                         (numtype       Chunk,     numtype       SwapVbn,
                          addrtype      SwapAddr,  ft F,lt Z,zz *Status);

       boolean Mem_MoveChunk 
                         (numtype       Chunk,     ft F,lt Z,zz *Status,
                          addrtype     *FreeAddr,  sizeinbytes  *FreeSize);


       boolean Mem_MakeXmemChunks 
                         (indextype     AllocQty,  indextype     ExtendQty,
                          ft F,lt Z,zz *Status);

       void    Mem_AssignXmemFile
                         (char         *FileName);

       void    Mem_CloseXmemFile
                         (void);


       boolean MemGetFileBlk
                         (bytessize     SizeInBytes,
                          blkhdrtype   *BlkHdr,    ft F,lt Z,zz *Status,
                          vbntype      *Vbn,       lt64         *FileOffset);

       boolean MemFreeFileBlk 
                         (blkhdrtype   *BlkHdr,
                          ft F,lt Z,zz *Status,    vbntype      *Vbn);



       boolean MemNewXmemBlk 
                         (bytessize     Bytes,     ft F,lt Z,zz *Status,
                          vbntype      *Vbn,       lt64         *XmemOffset);

       boolean MemFreeXmemBlk 
                         (ft F,lt Z,zz *Status,    vbntype      *Vbn);


       void    Mem_ShowXmemStats 
                         (void);

/*+-----------------------------------------------------------------------+
~P                         END OF MEM01.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
