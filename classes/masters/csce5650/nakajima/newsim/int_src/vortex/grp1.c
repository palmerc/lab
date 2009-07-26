/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* Env - grp.c */
#define  GRP1_C
#define  GRP_C

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "env0.h" 
#include "core01.h"
#include "mem010.h"
#include "gdbm.h"
#include "vchunk.h"
#include "files.h"
 
#ifdef NULL_TRACE_STRS
#define TraceStr(StrNum)  TraceStr[0]
static const char   *TraceStr[1] = {"   ...GRP1... message\n" };

#else
#define TraceStr(StrNum)  TraceStr[StrNum]
static const char *TraceStr[17] =
{
   "    GrpNewPacket (CoreDb[%2u] @ %x)  for GrpNum = %3u\n",
   "       GrpHdrDir = %4u,  GrpVbnDir = %4u;\n",
   "   NEW GrpHdr[%3u] @ %x Regions=%3u Entries/Region=%4u; Base=%3u\n",
   "    GrpSetPacketCache[%3d]  = %s\n",
   " Error in GrpSetPacketCache :: DbId= %3u; GrpNum=%6u\n",
   "    GrpSetAccessType[%3d]  = %s\n",
   " Error in GrpSetAccessType:: DbId= %3u; GrpNum=%6u\n",
   "    GrpFreezePacket[%3d]\n",
   " Error in GrpFreezePacket:: DbId= %3u; GrpNum=%6u\n",
   "    GrpThawPacket[%3d]\n",
   " Error in GrpThawPacket:: DbId= %3u; GrpNum=%6u\n",
   "    GrpDefrostPacket[%3d]\n",
   " Error in GrpThawPacket:: DbId= %3u; GrpNum=%6u\n",
   "    Free Packet[%3d]  @ %x\n",
   " Error in GrpFreePacket:: DbId= %3u; GrpNum=%6u\n",
   "    Delete Packet[%3d]  @ %x\n",
   " Error in GrpDeletePacket:: DbId= %3u; GrpNum=%6u\n"
};
#endif

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
#define MemPutHandle         Mem_PutWord
#define MemGetHandle         Mem_GetWord
 
#define MemPushLocalGrpNum   Mem_PushWord
#define MemPutLocalGrpNum    Mem_PutWord
#define MemGetLocalGrpNum    Mem_GetWord
 
#define MemPushEntryIndex     Mem_PushWord
#define MemPutEntryIndex      Mem_PutWord
#define MemGetEntryIndex      Mem_GetWord
 
#define MemPushEntryAddr      Mem_PushAddr
#define MemPutEntryAddr       Mem_PutAddr
#define MemGetEntryAddr       Mem_GetAddr
#define MemGetEntry           Mem_GetAddr
 
#define MemPutGrpNum         Mem_PutWord
#define MemGetGrpNum         Mem_GetWord
#define MemPushGrpNum        Mem_PushWord

#define MemPushAttrNum       Mem_PushWord
#define MemGetAttrNum        Mem_GetWord
#define MemPutAttrNum        Mem_PutWord
 
#define MemPushObjHdr        Mem_PushAddr
#define MemPutObjHdr         Mem_PutAddr
#define MemGetObjHdr         Mem_GetAddr
 
#define MemPushChunkNum      Mem_PushWord
#define MemGetChunkNum       Mem_GetWord
#define MemPutChunkNum       Mem_PutWord
 
#define MemPushIndex         Mem_PushWord
#define MemGetIndex          Mem_GetWord
#define MemPutIndex          Mem_PutWord
#define MemPopIndex          Mem_PopWord
 
#define MemPutRegionNum      Mem_PutWord
#define MemGetRegionNum      Mem_GetWord
 
#define MemPutOffset         Mem_PutWord
#define MemGetOffset         Mem_GetWord
 
#define MemPushLink          Mem_PushWord
#define MemPutLink           Mem_PutWord
#define MemGetLink           Mem_GetWord
 
#define MemPushExEntry        Mem_PushWord
#define MemPopExEntry         Mem_PopWord
 
 
#define MemGetVbn            Mem_GetWord
#define MemGetChunkNum       Mem_GetWord
 
#define MemGetSize           Mem_GetWord
#define MemPushSize          Mem_PushWord


/*+-----------------------------------------------------------------------+
~GRP                    GENERIC REGION PACKET                             !
  +-----------------------------------------------------------------------+*/
/*     12.1.1                                                              */
boolean Grp_NewPacket    (tokentype    *Anchor,    sizetype      EntrySize,
                          numtype       AllocRgns, numtype       RgnEntrys,
                          numtype       AllocXs,   numtype       BaseIndex, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          numtype       BuffRgns,
                          ft F,lt Z,zz *Status,    numtype      *GrpNum,
                          grpheader   **GrpHdr,    numtype      *BaseRegion)
{
dbheader    *CoreDb          = NullPtr;
indextype    StackPtr        = 0;
addrtype     NullHdr         = NullPtr;

 /* Invoke a new Packet instance in this DB                                */

*GrpHdr     = NullPtr;

 if (TmGetCoreDb       (Anchor,   McStat,   &CoreDb)) 
 if (Normal(*Status))
 if (CoreDb->HeadFreeGrps == 0)
 {
    /* Null the GrpHdr pointer for this GrpNum                             */
    if (MemPushWord     (CoreDb->GrpVbnDir,  0,       McStat, &StackPtr))
    if (MemPushAddr     (CoreDb->GrpHdrDir,  NullHdr, McStat, &StackPtr))
        MemGetChunkAddr (CoreDb->GrpHdrDir,
                         McStat,            (addrtype *)&CoreDb->GrpHdrs);

   *GrpNum    = ++CoreDb->GrpCount; 
    if (*GrpNum != StackPtr)
      *Status = Grp_BadGrpCount;

 } else {
    *GrpNum   = CoreDb->HeadFreeGrps;
     MemGetAddr   (CoreDb->GrpHdrDir,  *GrpNum,  
                   McStat,              (addrtype *)&CoreDb->HeadFreeGrps);
 }

 if (Normal(*Status))
 {
   /* make new Grp header and link into Dbheader                           */
   if (Normal(*Status))   
   if (CoreMoreCore      (sizeof(grpheader),  McStat,    (addrtype *)GrpHdr))

   if (MemPutAddr        (CoreDb->GrpHdrDir, *GrpNum,   *(addrtype *)GrpHdr, 
                          McStat))
   if (MemPutWord        (CoreDb->GrpVbnDir, *GrpNum,    0,          McStat))
   {
      (**GrpHdr).EnvObjType    = EnvType_AttrNum;
      if (DiskCache           == True
      &&  XmemFilePtr         == NULL)
          DiskCache            = False;

      (**GrpHdr).PacketAccess  = AccessType;
      (**GrpHdr).DiskCache     = DiskCache;

      (**GrpHdr).GrpNum        = *GrpNum;
      (**GrpHdr).EntrySize     = EntrySize;

      (**GrpHdr).EntryCount    = 0;
      (**GrpHdr).RegionEntrys  = RgnEntrys;
      (**GrpHdr).RegionCount   = 0;
      (**GrpHdr).RegionIndex   = 0;
      (**GrpHdr).NoRgnsInCore  = True;

      if (FREEZE_GRP_PACKETS  == True)
      {
         (**GrpHdr).FreezePacket  = True;
         (**GrpHdr).DiskCache     = False;
      } else {
         (**GrpHdr).FreezePacket  = False;
         (**GrpHdr).DiskCache     = True;
      }

      (**GrpHdr).ExEntryCount  = 0;
      (**GrpHdr).NextEx        = 0;

      (**GrpHdr).NextEntry     = 1;

      (**GrpHdr).BaseIndex     = BaseIndex;
      (**GrpHdr).BaseRegion    = 0;

      if (BuffRgns == 0)
        BuffRgns   = 5;
      else if (BuffRgns > 50)
	BuffRgns   = 50;
      (**GrpHdr).NumBuffRgns   = BuffRgns;

      if (CacheLimit == 0)
      {
         CacheLimit = BuffRgns;
         CacheLoad  = BuffRgns;
      }
      (**GrpHdr).CacheLimit    = CacheLimit;
      (**GrpHdr).CacheLoad     = CacheLoad;

      (**GrpHdr).RgnStatusDir  = 0;
      (**GrpHdr).RgnStatusDirVbn = 0;
      (**GrpHdr).RgnStats      = NullPtr;

      (**GrpHdr).GrpIndexDir   = 0;
      (**GrpHdr).GrpIndexDirVbn= 0;
      (**GrpHdr).GrpDirCount   = 0;
      (**GrpHdr).HeadExIndexs  = 0;
      (**GrpHdr).GrpDirAddr    = NullPtr;
      if ( RgnEntrys > 1 )
	 (**GrpHdr).RegionEntrys  = 8162/EntrySize;
   }
 
   /* Install the generic Grp header Chunks                                */
   if (Normal(*Status))
   { 
      GrpMakeHdrChunks   (Anchor,  *GrpHdr,  AllocRgns,  AllocXs,  McStat);
     *BaseRegion = (**GrpHdr).BaseRegion;
 
   }
 }
 
TRACK(TrackBak,"GrpNewPacket\n");
return (STAT);
}
 

 
/*+-----------------------------------------------------------------------+*/

/*     12.1.2                                                              */
boolean Grp_SetPacketCache
                         (tokentype    *Anchor,    numtype       GrpNum,
                          boolean       DiskCache, ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;
vbntype      Vbn      = 0;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr == NullPtr)
     {
        if(CoreDb->GrpHdrDirVbn)
        if (MemGetVbn     (CoreDb->GrpVbnDir,               GrpNum,
                           McStat,                         &Vbn))          
            DbmLoadGrpHdr (Anchor, GrpNum,     McStat,     &GrpHdr);
     }
     if (GrpHdr)
         GrpHdr->DiskCache  = DiskCache;
     else
        *Status  = Grp_PacketNotExist;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpSetPacketCache\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.3                                                              */
boolean Grp_SetPacketAccess
                         (tokentype    *Anchor,    numtype       GrpNum,
                          accesstype    AccessType,ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;
vbntype      Vbn      = 0;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr == NullPtr)
     {
        if(CoreDb->GrpHdrDirVbn)
        if (MemGetVbn     (CoreDb->GrpVbnDir,               GrpNum,
                           McStat,                         &Vbn))          
            DbmLoadGrpHdr (Anchor, GrpNum,     McStat,     &GrpHdr);
     }

     if (GrpHdr)
         GrpHdr->PacketAccess = AccessType;
     else
        *Status  = Grp_PacketNotExist;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpSetPacketAccess\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.4                                                              */
boolean Grp_FreezePacket (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;
vbntype      Vbn      = 0;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr == NullPtr)
     {
        if(CoreDb->GrpHdrDirVbn)
        if (MemGetVbn     (CoreDb->GrpVbnDir,               GrpNum,
                           McStat,                         &Vbn))          
            DbmLoadGrpHdr (Anchor, GrpNum,     McStat,     &GrpHdr);
     }

     if (GrpHdr)
         GrpHdr->FreezePacket  = True;
     else
        *Status  = Grp_PacketNotExist;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpFreezePacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.5                                                              */
boolean Grp_ThawPacket   (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr)
         GrpHdr->FreezePacket  = False;
     else
        *Status  = Grp_PacketNotLoaded;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpThawPacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.6                                                              */
boolean Grp_DefrostPacket
                         (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr)
         GrpHdr->FreezePacket  = False;
     else
        *Status  = Grp_PacketNotLoaded;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpDefrostPacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.7                                                              */
boolean Grp_FreePacket   (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;
indextype    RgnCount = 0;
int          i        = 0;
numtype     *RgnDir   = NullPtr;
vbntype     *SwapDir  = NullPtr;


  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr)
     {
        if (MemGetStackPtr  (GrpHdr->RegionChunkDir,     McStat,   &RgnCount)) 
        if (MemGetChunkAddr (GrpHdr->RegionChunkDir,     McStat,
                             (addrtype *)&RgnDir)) 
        if (MemGetChunkAddr (GrpHdr->RegionSwapDir,      McStat,
                             (addrtype *)&SwapDir)) 
        for (i = -1; ++i < RgnCount && Normal(*Status); )
        {
           if (RgnDir[i])
              MemFreeChunk   (&RgnDir[i],         McStat);

           if (Normal(*Status))
           if (SwapDir[i])
              MemFreeXmemBlk (McStat,                    &SwapDir[i]);
        }

        if (Normal(*Status))
        if (MemFreeChunk  (&GrpHdr->RegionChunkDir,    McStat))
        if (MemFreeChunk  (&GrpHdr->RegionSwapDir,     McStat))
        if (MemFreeChunk  (&GrpHdr->DirtyRegionDir,    McStat))
        if (MemFreeChunk  (&GrpHdr->RegionFreezeDir,   McStat))
        if (MemFreeChunk  (&GrpHdr->RgnBuffNumDir,     McStat))
        if (MemFreeChunk  (&GrpHdr->RgnAccessedDir,    McStat))
        if (MemFreeChunk  (&GrpHdr->RegionVbnDir,      McStat))
        if (MemFreeChunk  (&GrpHdr->ExEntryTbl,        McStat))
        if (MemFreeChunk  (&GrpHdr->BuffRgnsDir,       McStat))
        if (MemFreeChunk  (&GrpHdr->BuffNodesDir,      McStat))
            CoreFreeCoreSpace
                          ((addrtype *)&GrpHdr, sizeof(grpheader), McStat);

     } else
        *Status  = Grp_PacketNotLoaded;

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpFreePacket\n");
return (STAT);
}
 
/*     12.1.8                                                              */
boolean Grp_CommitPacket (tokentype    *Anchor,    numtype       GrpNum,
                          boolean       Relaese,   ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
  }

TRACK(TrackBak,"Grp_CommitPacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/

/*     12.1.8                                                              */
boolean Grp_DeletePacket (tokentype    *Anchor,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
/* GrpNum is the Packet Index the DbHdr->GrpHdrDir                         */
dbheader    *CoreDb   = NullPtr;
grpheader   *GrpHdr   = NullPtr;
numtype      Vbn      = 0;

  if (TmGetCoreDb   (Anchor,                      McStat,            &CoreDb))
  if (MemGetAddr    (CoreDb->GrpHdrDir, GrpNum,   McStat,(addrtype *)&GrpHdr))
  {
     if (GrpHdr == NullPtr)
     {
        if(CoreDb->GrpHdrDirVbn)
        if (MemGetVbn       (CoreDb->GrpHdrDirVbn,            GrpNum,
                             McStat,                         &Vbn))          
        if (Vbn)
            DbmDeleteGrpHdr (Anchor, GrpNum,     McStat);

     } else
        DbmDeleteGrpHdr     (Anchor, GrpNum,     McStat);

     if (Normal(*Status))
        GrpFreePacket  (Anchor,  GrpNum,   McStat);

  } else {
    *Status = Grp_GrpHdrNotExist;
  }

TRACK(TrackBak,"GrpDeletePacket\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*                       API TRACE PROCEDURES                              */
/*+-----------------------------------------------------------------------+*/
/*     12.1.9                                                              */
boolean Grp_ShowStats    (tokentype    *Token,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
grpheader     *GrpHdr          = NullPtr;

 if (CoreDbs[Token->DbId]->GrpCount)
 {
   if (Normal(*Status))
   if (GrpGetPacket (Token, GrpNum,  McStat, &GrpHdr))
       Grp_DumpStruct (Token,  GrpNum, McStat);
 }

TRACK(TrackBak,"GrpShowStats\n");
return (STAT);
}

/*     12.1.10                                                             */
boolean Grp_DumpStruct   (tokentype    *Token,    numtype       GrpNum,
                          ft F,lt Z,zz *Status)
{
 
TRACK(TrackBak,"Grp_DumpStruct\n");
return (STAT);
}


/*     12.1.11                                                             */
boolean Grp_DumpEntrys   (tokentype    *Token,     numtype       GrpNum,
                          indextype     Low,       indextype     High,
                          indextype     Limit,     dumpentryptr  DumpEntryPtr,
                          ft F,lt Z,zz *Status)
{
 
TRACK(TrackBak,"Grp_DumpEntrys\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF GRP1.C                                  !
  +-----------------------------------------------------------------------+*/
