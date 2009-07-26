/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - FM.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
extern   boolean             FileBug;

#ifdef FM
/* PRIVATE */
/*                       commented out in vdbm.c ...                       */
static boolean     SuppressSeek       = False;

static farlongtype LastFileOffset     = 0;

/*                       commented out in the DBM*.c files
       farlongtype LastBlkOffset[20];
*/   
#endif


#define   FilePutSomeBytes          File_PutSomeBytes
#define   FileGetSomeBytes          File_GetSomeBytes
#define   FileOutBlk                File_OutBlk   
#define   FileOutChunk              File_OutChunk 
#define   FileInBlk                 File_InBlk    
#define   FileInChunk               File_InChunk  
#define   FileGetZeroedChunk        File_GetZeroedChunk


/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
/* PUBLIC  */


/*     8.0.2                                                               */
   boolean File_GetSomeBytes
                         (blkhdrtype   *BlkHdr,    farlongtype   FileOffset,
                          unsigned      SomeBytes,
                          ft F,lt Z,zz *Status,    addrtype      BytesAddr);


/*     8.0.5                                                               */
   boolean File_InBlk    (addrtype     *BlkAddr,   bytessize     BlkSize,
                          blkhdrtype   *BlkHdr,    boolean       Release,
                          ft F,lt Z,zz *Status,    vbntype      *VmBlk);

/*     8.0.6                                                               */
   boolean File_InChunk  (chunknum     *Chunk,     blkhdrtype   *BlkHdr,
                          boolean       Release,
                          ft F,lt Z,zz *Status,    vbntype      *VbnBlk);

/*     8.0.7                                                               */
   boolean File_GetZeroedChunk 
                         (vbntype       VbnBlk,    blkhdrtype   *BlkHdr, 
                          boolean       Release,   
                          ft F,lt Z,zz *Status,    numtype      *Chunk);

/*+-----------------------------------------------------------------------+
~P                         END OF FM.H                                    !
  +-----------------------------------------------------------------------+*/
