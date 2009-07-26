/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/




#ifndef MEM00_H
#define MEM00_H

/* ENV-MEM00.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
typedef  boolean         bitfield;
typedef  byte            chunktype;
typedef  numtype         numchunk;
typedef  numtype         indexchunk;
typedef  numtype         indexhead;
typedef  numtype         addrchunk;
typedef  numtype         addrhead;     /* ptr to a indexed chunk of addrs */
typedef  longtype        chunkflags;       


#ifdef MEM_C
typedef struct TypeChunk 
{
         numtype      ChunkNum;  
         addrtype     Addr;      
         sizeinbytes  UnitSize;  
                                 
         indextype    StackPtr;  
         indextype    UnitCount; 
         indextype    BlockUnits;
         indextype    ExtendQty; 
                                 
         sizeinbytes  MaxUnitLen;

         boolean      IsCPP;     
         boolean      IsPtrs;    
         boolean      IsVary;    
         boolean      IsData;    
         boolean      IsString;  
         boolean      IsText;    

         boolean      IsSameSize;
         boolean      IsInCore;  
         boolean      IsActive;  
         boolean      IsClean;   
         boolean      IsFrozen;  
         boolean      IsGrown;   
}                         chunk;
 
typedef	struct ChunkHdrType 
{                                 
         tokentype    ThisToken;
         numtype      ChunkCount; 
         chunk      **Chunks;     
         indextype    ChunkQty;   
         indextype    GrowQty;    
         indextype    BlockUnits; 
         chunk       *FreeChunks; 
} ChunkHdr;
 
typedef struct Ory 
{                                 
         addrtype    *ChunkAddr;  
         numtype     *NextChunk;  
         chunkflags  *Flags;      
         sizeinbytes *UnitSize;   
         indextype   *UnitCount;  
         indextype   *StackPtr;   
         numtype     *StringText; 
         indextype   *ExtendQty;  
         sizeinbytes *MaxUnitLen; 
         indextype    NextFree;   
} OryType;
 
typedef struct TypeBlkHdr 
{ 
         filetokentype BlkFile;
#ifdef STRUC_ALIGN_BY8
         filetokentype BlkFile_64;
#endif /* STRUC_ALIGN_BY8 */
         idtype        DbId;
         idtype        CoreDbId;
         numtype       HeadFreeVbns;
         numtype       BlkDirChunk;
         numtype       HeadFreeBlks;
         numtype       HeadNullBlks;
         numtype       BlkTknChunk;
         numtype       BlkSizeChunk;
         farlongtype   EofMark;
} blkhdrtype;
#endif
 
/*
typedef  struct   TypeChunkStruc
{
        numtype       CRC;
        bytessize     UnitSize;
        indextype     UnitCount;
        indextype     StackPtr;
        numtype       StringText;
        sizeinbytes   MaxUnitLength;
        indextype     ExtendQty;
        valueclass    UnitType;
} chunkstruc;
*/

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

/*
typedef struct    TypeBlkTkn
{
       farlongtype    Offset;
       indextype      Owner;
}                         blktkntype;
*/

/*+-----------------------------------------------------------------------+*/
#ifdef MEM00
         OryType           *Theory               = NULL;

         numtype            Mem_ChunkExpanded    = 0;

         boolean            ClearChunk           = False;

         boolean            SetGetSwi            = False;

/* Mem :: Chunk Space                                                      */
static   indextype          Alloc_Chunks         = 3001;
static   indextype          Extend_Chunks        = 300;

         indextype          KrnlBlocks_AllocQty  = 2806;
         indextype          KrnlBlocks_ExtendQty = 1806;

static   indextype          XmemBlocks_AllocQty  = 300;
static   indextype          XmemBlocks_ExtendQty = 100;

static   numtype            ChunkAddrChunk       = 0; /* sizeof (chunknum) */

static   numtype            NumChunkChunks       = 3;
         numtype            NumChunks            = 3;

static   word               ChunksAllocated      = 0;
         numtype            ChunksFreeed         = 0;
static   numtype            ChunksReclaimed      = 0;

/*
static   numtype            FirstChunkNum        = 0;
static   count              NumHandleChunks      = 3;
*/

static   addrtype          *ChunkTheoryAddr      = NullPtr;

static  char               *ValueClassNam[21]    = {"aVoid",        
                                           "aBitField", "aChar",    "aUnChar",
                                "aShort",  "aUnShort",  "aInt",     "aUnInt",
                                "aLong",   "aUnLong",   "aQuad",    "aOctet",
                                           "aVoidPtr",  "aFloat",   "aDouble",
                                           "aEnumType", "aStruct",  "aUnion",
                                           "aComplex",  "aObject",  "aOid"  };
#else

#ifdef MEM_C
extern   OryType           *Theory;

extern   numtype            NumChunks;

extern   numtype            Mem_ChunkExpanded;

extern   numtype            ChunksFreeed;

extern   boolean            ClearChunk;

extern   boolean            SetGetSwi;


#else
extern   numtype            ChunkAddrChunk;
extern   numtype            NumChunkChunks;

extern   farlongtype        HandleChunk[];
extern   count              NumHandleChunks;

extern   numtype            NumChunks;
extern   numtype            Mem_ChunkExpanded;

#endif

#endif
/*+-----------------------------------------------------------------------+
~M                            MACROS                                       !
  +-----------------------------------------------------------------------+*/
#ifdef MEM_C
#define  ChunkAddrChunkNum   ChunkAddrChunk
#define  Num_ChunkChunks    (ChunkHdr.ChunkCount)
 
#define  ChunkChunkNum    0

#define  ChunkAddr_Chunk  0
#define  NextChunk_Chunk  1
#define  DbIdChunk_Chunk  1
#define  Flags_Chunk      2

#define  UnitSize_Chunk   3
#define  UnitCount_Chunk  4
#define  StackPtr_Chunk   5
#define  StringText_Chunk 6
#define  ExtendQty_Chunk  7
#define  MaxUnitLen_Chunk 8
#define  UnitType_Chunk   9
#define  CRC_Chunk        10

#ifdef LARGE 
#define  CRC_Offset        32
#define  UnitSize_Offset   28
#define  UnitCount_Offset  24
#define  StackPtr_Offset   20
#define  StringText_Offset 16
#define  MaxUnitLen_Offset 12
#define  ExtendQty_Offset   8
#define  UnitType_Offset    4

#else

#define  UnitSize_Offset   16
#define  UnitCount_Offset  14
#define  StackPtr_Offset   12
#define  StringText_Offset 10
#define  MaxUnitLen_Offset  8
#define  ExtendQty_Offset   6
#define  UnitType_Offset    4
#define  CRC_Offset         2

#endif

#define  Is_CPP         0x0001 
  /* true if a C++ object                   */
#define  Is_Ptrs        0x0002
  /* Chunk is ptrs to variable length data  */
#define  Is_Vary        0x0004
#define  IsNot_Vary    ~0x0004
  /* true if holds variable length data     */
#define  Is_Data        0x0008
#define  IsNot_Data    ~0x0008
   /* true if holds data                    */
#define  Is_String      0x0010
#define  IsNot_String  ~0x0010
   /* true if Unit Type is a Cstring        */
#define  Is_Text        0x0020
#define  IsNot_Text    ~0x0020
   /* true if block of text                 */
#define  Is_Numeric     0x0040
#define  IsNot_Numeric ~0x0040
   /* false if block of sizeinbytes         */
#define  Is_InCore      0x0080
#define  IsNot_InCore  ~0x0080
   /* true if chunk space is in core        */
#define  Is_Active      0x0100
#define  IsNot_Active  ~0x0100
   /* true if currently active              */
#define  Is_Clean       0x0200
#define  IsNot_Clean   ~0x0200
   /* true if not been PUT to               */
#define  Is_Frozen      0x0400
#define  IsNot_Frozen  ~0x0400
   /* true if Frozen from mem roll out      */
#define  Is_Grown       0x0800
#define  IsNot_Grown   ~0x0800
   /* true if chunk block grew in size      */
 
/* Chunk[0]                                 */
#define  Chunk_Addr(ChunkNum)\
		((Theory->ChunkAddr[ChunkNum]))


/* Chunk[1]                                 */
#define  Next_Chunk(ChunkNum)\
		((Theory->NextChunk[ChunkNum]))

#define  CoreDbId_Chunk(ChunkNum)\
		((Theory->NextChunk[ChunkNum]))


/* Chunk[2]                                 */
#define  Chunk_Flags(ChunkNum)\
		((Theory->Flags[ChunkNum]))



#define  Chunk_IsCpp(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Cpp))
#define  IsCpp_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |=  Is_Cpp))
#define  IsNotCpp_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |=  IsNot_Cpp))
 
#define  Chunk_IsPtrs(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Ptrs))
 
#define  Chunk_IsVary(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Vary))
#define  IsVary_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Vary))
#define  IsNotVary_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= IsNot_Vary))
 
#define  Chunk_IsData(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Data))
#define  IsData_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Data))
 
#define  Chunk_IsString(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_String))
#define  IsString_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_String))
#define  IsNotString_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= IsNot_String))
 
#define  Chunk_IsText(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Text))
#define  IsText_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Text))
#define  IsNotText_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= IsNot_Text))
 
#define  Chunk_IsNumeric(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Numeric))
#define  IsNumeric_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Numeric))
 
#define  Chunk_IsInCore(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_InCore))
#define  IsInCore_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_InCore))
#define  IsNotInCore_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_NotInCore))
 
#define  Chunk_IsActive(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Active))
#define  IsActive_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Active))
 
#define  Chunk_IsClean(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Clean))
#define  Dirty_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= IsNot_Clean))
#define  Clean_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Clean))
 
#define  Chunk_IsFrozen(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Frozen))
#define  Thaw_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= IsNot_Frozen))
#define  Freeze_Chunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Frozen))
 
#define  Chunk_IsGrown(ChunkNum)\
		((Theory->Flags[ChunkNum] & Is_Grown))
#define  GrownChunk(ChunkNum)\
		((Theory->Flags[ChunkNum] |= Is_Grown))


#define  Unit_Size(ChunkNum)\
 (*(sizeinbytes *)((char *)Theory->ChunkAddr[ChunkNum] - UnitSize_Offset))

#define  Unit_Count(ChunkNum)\
 (*(indextype   *)((char *)Theory->ChunkAddr[ChunkNum] - UnitCount_Offset))

#define  Stack_Ptr(ChunkNum)\
 (*(indextype   *)((char *)Theory->ChunkAddr[ChunkNum] - StackPtr_Offset))

#define  Text_Chunk(ChunkNum)\
 (*(numtype     *)((char *)Theory->ChunkAddr[ChunkNum] - StringText_Offset))

#define  Extend_Qty(ChunkNum)\
 (*(sizeinbytes *)((char *)Theory->ChunkAddr[ChunkNum] - ExtendQty_Offset))

#define  MaxUnit_Length(ChunkNum)\
 (*(indextype   *)((char *)Theory->ChunkAddr[ChunkNum] - MaxUnitLen_Offset))
 
#define  Unit_Type(ChunkNum)\
 (*(indextype   *)((char *)Theory->ChunkAddr[ChunkNum] - UnitType_Offset))
 
#define  CRC_Value(ChunkNum)\
 (*(indextype   *)((char *)Theory->ChunkAddr[ChunkNum] - CRC_Offset))
 
#define  NonNull_Chunk(ChunkNum)\
                ((Theory->Addr[ChunkNum] == NULL))

#define  SameSize_Chunk(ChunkNum,TypeSize)\
 ((*(sizeinbytes *)((char *)Theory->ChunkAddr[ChunkNum] - UnitSize_Offset) \
    == (sizeinbytes)TypeSize))

#define  NotSameSize_Chunk(ChunkNum,TypeSize)\
 ((*(sizeinbytes *)((char *)Theory->ChunkAddr[ChunkNum] - UnitSize_Offset) \
    != (sizeinbytes)TypeSize))
 
#endif

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
#define MemMakeOry             Mem_MakeOry
#define MemNewOry              Mem_NewOry
#define MemMakeOryChunks       Mem_MakeOryChunks
#define MemTestTheOry          Mem_TestTheOry
#define InitBlocks             Mem_InitBlocks
#define BlockCount             Mem_BlockCount
#define NewRegion              Mem_NewRegion 
 
#define FreeChunkChunk         Mem_FreeChunkChunk 
#define ExpandChunkTables      Mem_ExpandChunkTables
#define DumpChunkChunk         Mem_DumpChunkChunk 

#define MakeChunk              Chunk_MakeChunk
#define NewChunkChunk          Mem_NewChunkChunk
#define InitChunk              Chunk_InitChunk
#define MemNewChunkBlk         Mem_NewChunkBlk  
#define MemPutChunkStruc       Mem_PutChunkStruc
#define GrowNumericChunk       Chunk_GrowNumericChunk
#define GrowTextChunk          Chunk_GrowTextChunk
#define ExpandChunk            Chunk_ExpandChunk

#ifndef __MACROIZE_MEM__ 
#define ChunkExists            Chunk_ChunkExists
       boolean Chunk_ChunkExists    
                         (numtype       ChunkNum,  ft F,lt Z,zz *Status);
#else 

#ifdef MCSTAT
#define ChunkExists(ChunkNum,Stat)  True
#else
#define ChunkExists(ChunkNum,Stat)  True
#endif

#endif /* __MACROIZE_MEM__ */



#define ChkPutChunk            Chunk_ChkPutChunk
#define ChkPopChunk            Chunk_ChkPopChunk
#define ChkGetChunk            Chunk_ChkGetChunk

#ifdef MEM00
/* PRIVATE */
       void    Mem_Init  
               (indextype     Alloc_FreeAddrs,  indextype  Extend_FreeAddrs,
                char         *Xmem_File,
                indextype     Alloc_MemChunks,  indextype  Extend_MemChunks,
                indextype     Alloc_XmemBlks,   indextype  Extend_XmemBlks);

       boolean MemMakeOry     (ft F,lt Z,zz *Status,    addrtype   *TheOry);

       boolean MemNewOry      (ft F,lt Z,zz *Status,    addrtype   *TheOry);

       boolean MemMakeOryChunks
                              (ft F,lt Z,zz *Status);

       boolean MemMakeKrnlChunk  
                              (bytessize     UnitSize,  indextype   AllocQty,
                               indextype     ExtendQty, idtype      CoreDbId,
                               ft F,lt Z,zz *Status,    numtype    *ChunkNum);

       boolean Mem_ExpandKrnlChunk  
                              (chunknum      ChunkNum,
                               ft F,lt Z,zz *Status,    addrtype   *KrnlAddr);

       boolean MemTestTheOry  (ft F,lt Z,zz *Status);


    /* Block Size :: parameter to store all Space
                     in equally sized base blocks                          */

       sizeinbytes   Block_Size             = 1;

       sizeinblocks  
               InitBlocks     (bytessize   UnitSize);

       sizeinblocks
               BlockCount     (bytessize   UnitSize,    indextype UnitCount);


       boolean NewRegion      (sizeinblocks  Blocks,
                               ft F,lt Z,zz *Status,    addrtype *Region);

#ifdef     VALIDATE_MEM_CHUNKS
       numtype Chunk_Validate
                              (numtype     ChunkNum);
#endif /*  VALIDATE_MEM_CHUNKS  */

#endif

#ifdef MEM_C
       boolean NewChunkChunk  (ft F,lt Z,zz *Status,    numtype      *ChunkNum);

       boolean FreeChunkChunk (numtype      *Chunk,     ft F,lt Z,zz *Status);

       boolean ExpandChunkTables 
                              (ft F,lt Z,zz *Status);

       void    DumpChunkChunk (int           Dump,      numtype   ChunkNum);

       boolean MakeChunk      (boolean       VarLength, boolean   PtrChunk,
                               bytessize     UnitSize,  indextype AllocQty,
                               indextype     ExtendQty, idtype    CoreDbId,
                               ft F,lt Z,zz *Status,    numtype  *ChunkNum);

       boolean Chunk_NewChunk (bytessize     UnitSize,  indextype AllocQty,
                               ft F,lt Z,zz *Status,    numtype  *ChunkNum);

       boolean InitChunk      (numtype       ChunkNum,  boolean   VaryLength,
                               boolean       PtrChunk,  bytessize UnitSize,
                               indextype     ExtendQty, idtype    CoreDbId,
                               ft F,lt Z,zz *Status);

       boolean MemNewChunkBlk
                     (sizeinbytes   BlkSize,   ft F,lt Z,zz *Status,
                      numtype      *ChunkNum,  addrtype     *ChunkBlk);

       boolean MemPutChunkStruc
                     (numtype       Chunk,    chunkstruc   *ChunkRef,
                      idtype        CoreDbId, ft F,lt Z,zz *Status);


       boolean GrowNumericChunk
                          (numtype       ChunkNum,  ft F,lt Z,zz *Status);

       boolean GrowTextChunk  
                          (numtype       ChunkNum,  ft F,lt Z,zz *Status);

       boolean ExpandChunk
                          (numtype       ChunkNum,
                           ft F,lt Z,zz *Status,    indextype    *StackPtr);


       boolean Mem_ExpandKrnlChunk  
                          (chunknum      ChunkNum,
                           ft F,lt Z,zz *Status,    addrtype     *KrnlAddr);



       boolean ChkPushChunk   (numtype       ChunkNum,  size_t    SizeOfUnit,
                               ft F,lt Z,zz *Status);

       boolean ChkPutChunk    (numtype       ChunkNum,  indextype Index,
                               size_t        SizeOfUnit,
                               ft F,lt Z,zz *Status,    boolean  *PushFlag);

       boolean ChkPopChunk    (numtype       ChunkNum,  size_t    SizeOfUnit,
                               ft F,lt Z,zz *Status);

       boolean ChkGetChunk    (numtype       ChunkNum,  indextype Index,
                               size_t        SizeOfUnit,
                               ft F,lt Z,zz *Status);

#endif
       void Mem_ShowStats     (void);
       void Mem_DumpStats     (void);

/*+-----------------------------------------------------------------------+
~P                         END OF MEM00.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
