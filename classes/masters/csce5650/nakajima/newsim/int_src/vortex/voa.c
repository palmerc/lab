/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - VOA.c */
#define VOA
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "env0.h"
#include "fm.h"
#include "vdbm.h"
#include "vom.h"
#include "voa.h"

#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...MEM00... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[12] =
{
"   OaCreateVchunk  [%3u:%4u];\n",
"   OaSwapOutVchunk [%3u:%4u] Chunk =%4u; Vbn =%4u\n",
"   OaSwapInVchunk  [%3u:%4u] Chunk =%4u; Vbn =%4u \n",
"   Vchunk[%3u] Chunk=%4u...",
"   OaGetVchunkAddr [%3u:%4u] @%8x \n",
"   OaPutVchunkAddr [%3u:%4u] %x \n",
"   OaGetVchunkAllocQty[%3u:%4u] =%4u \n",
"   OaGetVchunkStackPtr[%3u:%4u] =%4u \n",
"   OaPutVchunkStackPtr[%3u:%4u] =%4u \n",
"       ... Vchunk[%3u:%4u]  Index %u > StackPtr %u\n",
"       ... Vchunk[%3u:%4u]  Index %u >= StackPtr %u\n",
"   Vchunk[%3u:%4u] ..."
};
#endif

#define PrimalId  RenvId

/* Vchunks in RENV and WENV are not constructed with GRP's                 */
/*+-----------------------------------------------------------------------+*
~VOA                          EXTERNS                                     !
 *+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*
~OA                         OBJECT ACTOR                                  !
  !                            Vchunk                                     !
 *+-----------------------------------------------------------------------+*/

boolean    OaCreateVchunk (tokentype    *Anchor,     sizetype      UnitSize,
                           indextype     UnitCount,  indextype     ExtendQty, 
                           ft F,lt Z,zz *Status,     tokentype    *Token)
{
addrtype   ChunkAddr = NullPtr;

  Token->DbId     = Anchor->DbId;
  Token->CoreDbId = Anchor->CoreDbId;
     DbmNewVchunk      (Anchor,  UnitSize,            UnitCount,  ExtendQty,
                        McStat,  (indextype  *)&(Token->Handle), &ChunkAddr);
  if (DeBug || OaBug)
/* "   OaCreateVchunk  [%3u:%4u];\n" */
  if (sprintf (Msg, TraceStr(0), 
               Token->DbId, Token->Handle))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaCreateVchunk\n");
return (STAT);
}
 

boolean    OaFreezeVchunk (tokentype   *Token,   ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemDefrostChunk (VchunkToken.InCore, McStat);

TRACK(TrackBak,"OaFreezeVchunk\n");
return (STAT);
}
 

 

boolean    OaDumpVchunkVchunk (tokentype  *Token,   ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
/* "   Vchunk[%3u] Chunk=%4u..." */
  if (sprintf           (Msg,  TraceStr(3), 
                         Token->Handle, VchunkToken.InCore))
  if (TraceMsg          (0,    Msg))
  if (VchunkToken.InCore)
     DumpChunkChunk (0, VchunkToken.InCore);

TRACK(TrackBak,"OaDumpVchunkVchunk\n");
return (STAT);
}
 

boolean    OaGetVchunkAddr (tokentype    *Token,
                            ft F,lt Z,zz *Status,  addrtype    *ChunkAddr)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemGetChunkAddr (VchunkToken.InCore, McStat, ChunkAddr);

  if (DeBug || OaBug)
/* "   OaGetVchunkAddr [%3u:%4u] @%8x \n" */
  if (sprintf (Msg, TraceStr(4), 
               Token->DbId, Token->Handle, *ChunkAddr))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaGetVchunkAddr\n");
return (STAT);
}
 

boolean    OaPutVchunkAddr (tokentype    *Token,  addrtype    ChunkAddr,
                            ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemPutChunkAddr (VchunkToken.InCore, ChunkAddr, McStat);

  if (DeBug || OaBug)
/* "   OaPutVchunkAddr [%3u:%4u] %x \n" */
  if (sprintf (Msg, TraceStr(5), 
               Token->DbId, Token->Handle, ChunkAddr))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaPutVchunkAddr\n");
return (STAT);
}
 

boolean    OaGetVchunkAllocQty (tokentype    *Token,
                                ft F,lt Z,zz *Status,   indextype  *AllocQty)
{
vchunktkntype VchunkToken;
numtype       UnitSize    = 0;

  if (OmGetVchunkToken  (Token,              McStat,    &VchunkToken))
      MemGetChunkSize   (VchunkToken.InCore, McStat,     AllocQty,  &UnitSize);

  if (DeBug || OaBug)
/* "   OaGetVchunkAllocQty[%3u:%4u] =%4u \n" */
  if (sprintf (Msg, TraceStr(6), 
               Token->DbId, Token->Handle, *AllocQty))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaGetVchunkAllocQty\n");
return (STAT);
}
 

boolean    OaGetVchunkStackPtr (tokentype    *Token,
                                ft F,lt Z,zz *Status,   indextype  *StackPtr)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemGetStackPtr (VchunkToken.InCore, McStat,  StackPtr);

  if (DeBug || OaBug)
/* "   OaGetVchunkStackPtr[%3u:%4u] =%4u \n" */
  if (sprintf (Msg, TraceStr(7), 
               Token->DbId, Token->Handle, *StackPtr))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaGetVchunkStackPtr\n");
return (STAT);
}
 

boolean    OaPutVchunkStackPtr (tokentype    *Token,  indextype   StackPtr,
                                ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken (Token, McStat, &VchunkToken))
     MemPutStackPtr (VchunkToken.InCore, StackPtr, McStat);

  if (DeBug || OaBug)
/* "   OaPutVchunkStackPtr[%3u:%4u] =%4u \n" */
  if (sprintf (Msg, TraceStr(8), 
               Token->DbId, Token->Handle, StackPtr))
     TraceMsg (0, Msg);

TRACK(TrackBak,"OaPutVchunkStackPtr\n");
return (STAT);
}
 

boolean    OaPushUnit        (tokentype    *Token,   addrtype    SomeBytes,
                              ft F,lt Z,zz *Status,  indextype  *StackPtr)
{
indextype     UnitCount     = 0;
bytessize     UnitSize      = 0;
vchunktkntype VchunkToken;
indextype     ByteStackPtr  = 0;

  if (OmGetVchunkToken  (Token,              McStat,    &VchunkToken))
  if (MemGetChunkSize   (VchunkToken.InCore, McStat,    &UnitCount, &UnitSize))
      MemPushSomeBytes  (VchunkToken.InCore, UnitSize,   SomeBytes,
                         McStat,            &ByteStackPtr);

  if (Normal(*Status))
  if (MemGetStackPtr    (VchunkToken.InCore, McStat,     StackPtr))
     *StackPtr = *StackPtr - 1;

TRACK(TrackBak,"OaPushUnit\n");
return (STAT);
}
 

boolean    OaPutUnit     (tokentype    *Token,     indextype     Index,    
                          addrtype      SomeBytes, ft F,lt Z,zz *Status)
{
indextype     UnitCount  = 0;
bytessize     UnitSize   = 0;
vchunktkntype VchunkToken;

  if (OmGetVchunkToken  (Token,              McStat, &VchunkToken))
  if (MemGetChunkSize   (VchunkToken.InCore, McStat, &UnitCount, &UnitSize))
      MemPutSomeBytes   (VchunkToken.InCore, Index,   UnitSize,  SomeBytes,
                         McStat);

TRACK(TrackBak,"OaPutUnit\n");
return (STAT);
}
 

boolean    OaGetUnit         (tokentype    *Token,   indextype   Index,    
                              ft F,lt Z,zz *Status,  addrtype    SomeBytes)
{
indextype     UnitCount  = 0;
bytessize     UnitSize   = 0;
vchunktkntype VchunkToken;

  if (OmGetVchunkToken  (Token,              McStat,   &VchunkToken))
  if (MemGetChunkSize   (VchunkToken.InCore, McStat,   &UnitCount, &UnitSize))
      MemGetSomeBytes   (VchunkToken.InCore, Index,     UnitSize,
                         McStat,             SomeBytes);

TRACK(TrackBak,"OaGetUnit\n");
return (STAT);
}
 

 

boolean    OaDumpVchunk  (tokentype  *Token,   ft F,lt Z,zz *Status)
{
vchunktkntype VchunkToken;

  if (OmGetVchunkToken  (Token,  McStat, &VchunkToken))
/* "   Vchunk[%3u:%4u] ..." */
  if (sprintf           (Msg,  TraceStr(11),
                         Token->DbId, Token->Handle))
  if (TraceMsg          (0,    Msg))
     MemDumpChunk (0, VchunkToken.InCore, McStat);

TRACK(TrackBak,"OaDumpVchunk\n");
return (STAT);
}
 

boolean    OaDeleteVchunk  (tokentype    *Token,  ft F,lt Z,zz *Status)
{
  *Status  = Err_NotImplemented;


TRACK(TrackBak,"OaDeleteVchunk\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF VOA.C                                   !
  +-----------------------------------------------------------------------+*/
