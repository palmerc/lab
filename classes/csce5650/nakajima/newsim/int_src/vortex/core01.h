/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV-CORE.h  */
#ifndef CORE_01
#define CORE_01

/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
/*
typedef  boolean          bitfield;
typedef  byte             chunktype;

typedef  numtype          numchunk;
typedef  numtype          indexchunk;
typedef  numtype          indexhead;
typedef  numtype          addrchunk;
typedef  numtype          addrhead;
typedef  unsigned short   chunkflags;       
*/

typedef struct TypeCorePageSpace
{
     addrtype       CoreAddr;
     farlongtype    FreeSize;
}                         pagespacetype;


/*+-----------------------------------------------------------------------+*/

#ifdef CORE
extern   indextype     Alloc_Chunks;
extern   boolean       SWAP;
extern   numtype       Mem_ChunkExpanded;

static   numtype       KrnlFreeAddrChunk          = 0;
static   addrtype     *KrnlFreeAddr               = NullPtr;
static   addrtype     *KrnlFreeAddr0              = NullPtr;
static   numtype       KrnlFreeSizeChunk          = 0;
static   numtype      *KrnlFreeSize               = NullPtr;
static   numtype       KrnlChunkUnits             = 0;

static   indextype     KrnlHeadFreeGaps           = 0;

static   numtype       Regions                    = 0;
static   count         Num1Regions                = 0;

static   addrtype      CoreBlock01                = NullPtr;

static   farlongtype   Core_PageSize              = CorePage_Size;
static   farlongtype   Core_MaxEntrySize          = CorePage_Size;

static   farlongtype   Core1BlockElems            = 0;
static   addrtype      CoreBlock1                 = NULL;
static   addrtype      CoreBlock1Base             = NULL;
static   farlongtype   CurrBlock1Size             = 0;

static   count         Core1Blocks                = 0;



static   pagespacetype PageSpace[100];


static   booleantype   FirstTry                   = True; /* Should be False*/

static   farlongtype   Alloc_Amount               = sizeof (int) 
                                                  == 2 ? 512000L : 1024000L;

static   farlongtype   CoreFreeStoreSize          = (farlongtype )0;

static   addrtype      VoidLowestAddr             = NullPtr;

static   farlongtype   VoidAllocated              = (farlongtype )0;
static   farlongtype   VoidDeAllocated            = (farlongtype )0;
static   farlongtype   VoidUsed                   = (farlongtype )0;
static   farlongtype   VoidMaxUsed                = (farlongtype )0;

static   farlongtype   VoidBlksAllocated          = (farlongtype )0;
static   farlongtype   VoidBlksDeAllocated        = (farlongtype )0;

static   farlongtype   CoreBlksRequested          = (farlongtype )0;
static   farlongtype   CoreBlksFreeed             = (farlongtype )0;
static   farlongtype   CoreBlksReclaimed          = (farlongtype )0;

static   farlongtype   CoreBlksExactReclaimed     = (farlongtype )0;
static   farlongtype   CoreBlksGapReclaimed       = (farlongtype )0;
static   farlongtype   CoreBlksPageSpaceReclaimed = (farlongtype )0;
static   farlongtype   CoreBlksReGapped           = (farlongtype )0;

static   farlongtype   Core1Size                  = 0;
static   farlongtype   Core1Freeed                = (farlongtype )0;
static   farlongtype   Core1Reclaimed             = (farlongtype )0;
static   farlongtype   Core1Used                  = (farlongtype )0;

         farlongtype   CoreBlk_GapSpace           = (farlongtype )64;

extern   indextype     KrnlBlocks_AllocQty;
extern   indextype     KrnlBlocks_ExtendQty;

#else
extern   farlongtype   CoreBlk_GapSpace;

#endif

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
#define VoidExtendCore           Void_ExtendCore     
#define VoidFreeCore             Void_FreeCore       
#define SysFarCoreLeft           Void_SysFarCoreLeft     
#define CoreShowStats            Core_ShowStats      
#define CoreMoreCore             Core_MoreCore       
#define Core0MoreCore            Core0_MoreCore      
#define CoreFreeCoreSpace        Core_FreeCoreSpace
#define Core0FreeCoreSpace       Core0_FreeCoreSpace
#define Core0_FreeCore           Core0_FreeCoreSpace
#define CoreAddGap               Core_AddGap         
#define CoreGetFreeSpace         Core_GetFreeSpace
#define CoreGetPageSpace         Core_GetPageSpace
#define CoreGetGapSpace          Core_GetGapSpace
#define CoreReGap                Core_ReGap          
#define CoreReGapFreeAddrs       Core_ReGapFreeAddrs
#define CoreMoveBytes            Core_MoveBytes      
#define MoveBytes                Ut_MoveBytes      
#define CompareBytes             Ut_CompareBytes      
 
 
       addrtype
               VoidExtendCore (farlongtype  *SizeInBytes);

       void    VoidFreeCore   (addrtype *CoreAddr,  farlongtype SizeInBytes);

       farlongtype
               SysFarCoreLeft (void);


       void    Core_SetPageSize
                              (farlongtype   PageSize,  
                               farlongtype   MaxEntrySize);

       void    Core_AssignChunks
                              (numtype       AddrChunk, addrtype *FreeAddrs, 
                               numtype       SizeChunk, numtype  *FreeSizes);

       void    CoreShowStats  (void);


       boolean CoreMoreCore   (bytessizetype Size,
                               ft F,lt Z,zz *Status,    addrtype *Addr);

       boolean Core0MoreCore  (bytessizetype Size,
                               ft F,lt Z,zz *Status,    addrtype *Addr);

       boolean CoreFreeCoreSpace
                              (addrtype     *CoreAddr,  bytessize SizeInBytes,
                               ft F,lt Z,zz *Status);

       boolean Core0FreeCoreSpace
                              (addrtype     *CoreAddr,  bytessize SizeInBytes,
                               ft F,lt Z,zz *Status);


       boolean Core0_AllocString
                              (sizetype      StrSize,
                               ft F,lt Z,zz *Status,    char        **String);


       boolean Core0_FreeString  
                              (char         *String,    ft F,lt Z,zz *Status);



       void    MoveBytes      (addrtype      Source,    addrtype  Target, 
                               bytessize     ByteCount);

       int     CompareBytes   (addrtype      Source,    addrtype  CmpVal, 
                               bytessize     ByteCount,
                               ft F,lt Z,zz *Status);
 
/*+-----------------------------------------------------------------------+
~P                         END OF CORE.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
