/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* c Shell - Vchunk.c */
#define  VCHUNK_C
#define  GRP_C

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "mem010.h"
#include "fm.h"
#include "env0.h"
#include "vdbm.h"
#include "gdbm.h"
#include "vchunk.h"

#define MemThawChunk        Mem_DefrostChunk
#define MemDirtyChunk       Mem_DefrostChunk
#define MemCleanChunk       Mem_DefrostChunk

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

/*+-----------------------------------------------------------------------+
~VCHUNK                   VCHUNK  SHELL                                   !
  +-----------------------------------------------------------------------+*/

/*     10.1.1                                                              */
boolean Vchunk_Create    (tokentype    *Anchor,    sizetype      UnitSize,
                          indextype     UnitCount, indextype     ExtendQty,
                          ft F,lt Z,zz *Status,    tokentype    *Vtoken)
{
addrtype   ChunkAddr = NullPtr;

  Vtoken->DbId          = Anchor->DbId;
  DbmNewVchunk           (Anchor,       UnitSize,   UnitCount,  ExtendQty,
                          McStat,
                          (indextype  *)&(Vtoken->Handle),     &ChunkAddr);


TRACK(TrackBak,"Vchunk_Create\n");
return (STAT);
}

/*     10.1.1.1                                                            */
boolean Vchunk_IsValidToken 
                         (tokentype    *Token,     ft F,lt Z,zz *Status)
{
boolean        IsValid      = False;
dbheader      *CoreDb       = NullPtr;
grpheader     *GrpHdr       = NullPtr;
vchunktkntype *VchunkDir    = NullPtr;
numtype        RgnNum       = 0;
indextype      LocalIndex   = 0;
vchunktkntype  VchunkTkn;
tokentype      DbmsTkn      = NullToken;
numtype        BaseRegion   = 0;

  if (Token->DbId   == 0
  ||  Token->Handle == 0)
     return (IsValid);

  DbmsTkn.DbId    = Token->DbId;
  DbmsTkn.Handle  = 1;

  if ((IsValid = TmIsValid (&DbmsTkn,   McStat)) == True)
  {
     IsValid   = False;
     if (TmFetchCoreDb     (&DbmsTkn, McStat,    &CoreDb))
     if ((GrpHdr = CoreDb->VchunkGrpHdr) == NullPtr)
     {
        if (GrpGetPacket    (&DbmsTkn,        CoreDb->VchunkGrpNum,     
                             McStat,         &CoreDb->VchunkGrpHdr))
           GrpHdr   = CoreDb->VchunkGrpHdr;
        if (Normal(*Status))
        if (DbmBug)
           Grp_DumpStruct   (&DbmsTkn,   CoreDb->VchunkGrpNum,  McStat);
     }

     if (Normal(*Status))
     {
        if (Token->Handle < GrpHdr->BaseIndex)
        {
           RgnNum     = 0;
           LocalIndex = Token->Handle;

        } else {
           RgnNum     = (Token->Handle  - GrpHdr->BaseIndex)
                       / GrpHdr->RegionEntrys;
           if (GrpHdr->BaseIndex)
              RgnNum++;

           LocalIndex = (Token->Handle  - GrpHdr->BaseIndex)
                       % GrpHdr->RegionEntrys;
        }
        if (GrpGetRegion (CoreDb,     GrpHdr,                    RgnNum,     
                          McStat,    &BaseRegion,   (addrtype *)&VchunkDir))
           VchunkTkn    = VchunkDir[LocalIndex];

        if (VchunkTkn.InCore != 0
        ||  VchunkTkn.OnDisk != 0)
           IsValid   = True;
        else
           IsValid   = False;
     }
  }

STAT;
TRACK(TrackBak,"Vchunk_IsValidToken\n");

 *Status   = Env_Normal;

return (IsValid);
}

/*     10.1.2                                                              */
boolean Vchunk_GetVstruc (tokentype    *Vtoken,    ft F,lt Z,zz *Status, 
                          vchunkstruc  *VchunkStruc)
{
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,          &Vchunk))
      MemGetChunkStruc   (Vchunk.InCore,   McStat,           VchunkStruc);

TRACK(TrackBak,"Vchunk_GetVstruc\n");
return (STAT);
}


/*     10.1.3                                                              */
boolean Vchunk_DumpVstruc
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,          &Vchunk))
      DumpChunkChunk     (0,               Vchunk.InCore);


TRACK(TrackBak,"Vchunk_DumpVstruc\n");
return (STAT);
}



/*     10.1.4                                                              */
boolean Vchunk_Freeze    (tokentype    *Vtoken,    ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,          &Vchunk))
      MemFreezeChunk     (Vchunk.InCore,   McStat);


TRACK(TrackBak,"Vchunk_Freeze\n");
return (STAT);
}





/*     10.1.11                                                             */
boolean Vchunk_GetAddress
                         (tokentype    *Vtoken,    
                          ft F,lt Z,zz *Status,    addrtype     *VchunkAddr)
{
vchunktkntype Vchunk;
/*
  if (Vtoken->Handle == 1)
    DbmBug = True;
*/

  if (DbmInvokeVchunk    (Vtoken,          McStat,         &Vchunk))
      MemGetChunkAddr    (Vchunk.InCore,   McStat,          VchunkAddr);

TRACK(TrackBak,"Vchunk_GetAddress\n");
return (STAT);
}



/*     10.1.12                                                             */
boolean Vchunk_PutAddress
                         (tokentype    *Vtoken,    addrtype      VchunkAddr,    
                          ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;


  if (DbmInvokeVchunk    (Vtoken,          McStat,         &Vchunk))
      MemPutChunkAddr    (Vchunk.InCore,   VchunkAddr,      McStat);

TRACK(TrackBak,"Vchunk_PutAddress\n");
return (STAT);
}


/*     10.1.13                                                             */
boolean Vchunk_GetStackPtr
                         (tokentype    *Vtoken,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr)
{
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,         &Vchunk))
      MemGetStackPtr     (Vchunk.InCore,   McStat,          StackPtr);

TRACK(TrackBak,"Vchunk_GetStackPtr\n");
return (STAT);
}


/*     10.1.14                                                             */
boolean Vchunk_PutStackPtr
                         (tokentype    *Vtoken,    indextype     StackPtr,    
                          ft F,lt Z,zz *Status,    addrtype     *Varray)
{
vchunktkntype Vchunk;


  if (DbmInvokeVchunk    (Vtoken,          McStat,         &Vchunk))
  if (MemPutStackPtr     (Vchunk.InCore,   StackPtr,        McStat))
      MemGetChunkAddr    (Vchunk.InCore,   McStat,          Varray);

TRACK(TrackBak,"Vchunk_PutStackPtr\n");
return (STAT);
}


/*     10.1.14.1                                                           */
boolean Vchunk_GetChunkSize
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status,
                          indextype    *UnitCount, sizetype     *UnitSize)
{
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,         &Vchunk))
      MemGetChunkSize    (Vchunk.InCore,   McStat,        
                          UnitCount,           (bytessize *)UnitSize);

TRACK(TrackBak,"Vchunk_GetChunkSize\n");
return (STAT);
}


/*     10.1.15                                                             */
boolean Vchunk_Copy      (tokentype    *Vtoken0,   tokentype    *Vtoken1, 
                          ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk0;
vchunktkntype Vchunk1;
vchunkstruc   VchunkStruc0;
vchunkstruc   VchunkStruc1;
addrtype      VchunkAddr0   = NullPtr;
addrtype      VchunkAddr1   = NullPtr;

  if (DbmInvokeVchunk    (Vtoken0,         McStat,    &Vchunk0))
  if (MemGetChunkStruc   (Vchunk0.InCore,  McStat,    &VchunkStruc0))
  if (MemGetChunkAddr    (Vchunk0.InCore,  McStat,    &VchunkAddr0))

  if (DbmInvokeVchunk    (Vtoken1,         McStat,    &Vchunk1))
  if (MemGetChunkStruc   (Vchunk1.InCore,  McStat,    &VchunkStruc1))
  if (MemGetChunkAddr    (Vchunk1.InCore,  McStat,    &VchunkAddr1))

  if (VchunkStruc0.UnitSize == VchunkStruc1.UnitSize)
  {
     if (MemPutStackPtr  (Vchunk1.InCore,  VchunkStruc1.StackPtr,    McStat))
         MoveBytes       (VchunkAddr0,     VchunkAddr1, 
                          VchunkStruc0.UnitSize * VchunkStruc0.UnitCount);
  } else
    *Status = Vchunk_NotSameUnitSize;

TRACK(TrackBak,"Vchunk_Copy\n");
return (STAT);
}


/*     10.1.16.0                                                           */
boolean Vchunk_Dump      (tokentype    *Vtoken,    ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;

  if (Vtoken->Handle)
  {
     if (DbmGetVchunkTkn         (Vtoken,     McStat,         &Vchunk))
     if (Vchunk.InCore  == 0)
     {
     } else {
         MemDumpChunk       (0,          Vchunk.InCore,   McStat);
     }

  } else {
  }

TRACK(TrackBak,"Vchunk_Dump\n");
return (STAT);
}


/*     10.1.16.1                                                           */
boolean Vchunk_DumpPartial
                         (tokentype    *Vtoken,    
                          numtype       Ilow,      numtype       Ihigh,
                          ft F,lt Z,zz *Status)
{
vchunktkntype Vchunk;

  if (Vtoken->Handle)
  {
     if (DbmGetVchunkTkn         (Vtoken,     McStat,         &Vchunk))
     if (Vchunk.InCore  == 0)
     {
     } else {
         Mem_DumpChunkPart  (0,    Vchunk.InCore,  Ilow,   Ihigh,  McStat);
     }

  } else {
  }

TRACK(TrackBak,"Vchunk_DumpPartial\n");
return (STAT);
}


/*     10.1.17                                                             */
boolean Vchunk_Free      (tokentype    *Vtoken,    ft F,lt Z,zz *Status)
{

  DbmFreeVchunk        (Vtoken,          McStat);

TRACK(TrackBak,"Vchunk_Free\n");
return (STAT);
}


/*     10.1.18                                                             */
boolean Vchunk_Commit    (tokentype    *Vtoken,    boolean       Release,
                          ft F,lt Z,zz *Status)
{

  DbmCommitVchunk        (Vtoken,    Release,    McStat);

TRACK(TrackBak,"Vchunk_Commit\n");
return (STAT);
}


/*     10.1.19                                                             */
boolean Vchunk_Delete
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status)
{

  DbmDeleteVchunk        (Vtoken,          McStat);

TRACK(TrackBak,"Vchunk_Delete\n");
return (STAT);
}


/* ........................................................................
                    ....        unit        ....
   ........................................................................*/

/*     10.2.1                                                              */
boolean Vchunk_PushUnit  (tokentype    *Vtoken,    addrtype      SomeBytes,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr)
{
indextype     UnitCount     = 0;
bytessize     UnitSize      = 0;
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,    &Vchunk))
  if (MemGetChunkSize    (Vchunk.InCore,   McStat,    &UnitCount, &UnitSize))
      MemPushSomeBytes   (Vchunk.InCore,   UnitSize,   SomeBytes,
                          McStat,                                  StackPtr);

TRACK(TrackBak,"Vchunk_PushUnit\n");
return (STAT);
}


boolean Vchunk_PutUnit   (tokentype    *Vtoken,    indextype     Index,    
                          addrtype      SomeBytes, ft F,lt Z,zz *Status)
{
indextype     UnitCount     = 0;
bytessize     UnitSize      = 0;
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,    &Vchunk))
  if (MemGetChunkSize    (Vchunk.InCore,   McStat,    &UnitCount, &UnitSize))
      MemPutSomeBytes    (Vchunk.InCore,   Index,      UnitSize,   SomeBytes,
                          McStat);

TRACK(TrackBak,"Vchunk_PutUnit\n");
return (STAT);
}



/*     10.2.4                                                              */
boolean Vchunk_GetUnit   (tokentype    *Vtoken,    indextype     Index,    
                          ft F,lt Z,zz *Status,    addrtype      SomeBytes)
{
indextype     UnitCount     = 0;
bytessize     UnitSize      = 0;
vchunktkntype Vchunk;

  if (DbmInvokeVchunk    (Vtoken,          McStat,    &Vchunk))
  if (MemGetChunkSize    (Vchunk.InCore,   McStat,    &UnitCount, &UnitSize))
      MemGetSomeBytes    (Vchunk.InCore,   Index,      UnitSize,  
                          McStat,          SomeBytes);


TRACK(TrackBak,"Vchunk_GetUnit\n");
return (STAT);
}





/*+-----------------------------------------------------------------------+
  !                 ....     BIT FIELD      ....                          !
  +-----------------------------------------------------------------------+*/

/*     11.1.1                                                              */
boolean BitField_Create   (tokentype    *Anchor,    sizetype      AllocBits,
                          ft F,lt Z,zz *Status,    tokentype    *BitFldTkn)
{
bitfieldtype *BitField   = NullPtr;
sizetype      NumBits    = (AllocBits | 7)+1;
indextype     AllocBytes =  NumBits / 8;

  if (VchunkCreate       (Anchor,      sizeof (bitfieldtype), 
                          AllocBytes,                        Extend_BitField,
                          McStat,                            BitFldTkn))

  if (VchunkPutStackPtr  (BitFldTkn,   AllocBytes,
                          McStat,     (addrtype *)&BitField))

         memset          (BitField,    '\0',                 AllocBytes);

TRACK(TrackBak,"BitField_Create\n");
return (STAT);
}




/*     11.1.3                                                              */
boolean BitField_Put      (tokentype    *BitFldTkn, indextype     BitNum,
                          bytetype      BitValue,  ft F,lt Z,zz *Status)
{
bitfieldtype    *BitField  = NullPtr;
indextype        ByteNum   = BitNum >> 3;
indextype        ByteCount = 0;

  if (VchunkGetStackPtr    (BitFldTkn,    McStat,              &ByteCount))
  if (VchunkGetAddress     (BitFldTkn,    McStat,  (addrtype *)&BitField)) 

  if (ByteNum >= ByteCount)
      VchunkPutStackPtr    (BitFldTkn, ByteNum, 
                            McStat,                (addrtype *)&BitField);

  if (BitValue) /* Set   */
      BitField[ ByteNum ] |= BitValue << (BitNum & 7);

  else          /* Reset */
      BitField[ ByteNum ] &= ~(1 << (BitNum & 7));

TRACK(TrackBak,"BitField_Put\n");
return (STAT);
}


/*     11.1.4                                                              */
boolean BitField_Get
                         (tokentype    *BitFldTkn, indextype     BitNum,
                          ft F,lt Z,zz *Status,    bytetype     *BitValue)
{
indextype        ByteNum   = BitNum >> 3;
bitfieldtype    *BitField  = NullPtr;
indextype        ByteCount = 0;

  if (VchunkGetAddress   (BitFldTkn, McStat,  (addrtype *)&BitField))
  if (VchunkGetStackPtr  (BitFldTkn, McStat,              &ByteCount))
  if (ByteNum  >= ByteCount)
        *Status = Set_IndexGtrAllocRoots;

  if (Normal(*Status))
     *BitValue =  (BitField[ ByteNum ] & 1 << (BitNum & 7)) ? 1: 0;

TRACK(TrackBak,"BitField_Get\n");
return (STAT);
}



/*+-----------------------------------------------------------------------+
~P                         END OF VCHUNK.C                                !
  +-----------------------------------------------------------------------+*/
