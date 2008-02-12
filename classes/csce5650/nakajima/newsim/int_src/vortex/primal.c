/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* c Shell - Primal.c */
#define  PRIMAL_C

#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "dbm.h"
#include "env0.h"
#include "envdefs.h"
#include "km.h"
#include "im.h"
#include "vchunk.h"
#include "vdbm.h"
#include "domain.h"
#include "env01.h"
#include "primal.h"
 
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        DbId_AttrNum               (numtype )3
#define        Id_AttrNum                 (numtype )143
#define        DbFileName_AttrNum         (numtype )158

#define        WenvDbs_Hndl            (handletype )698

#endif

#define MemGetObjNum         Mem_GetWord

extern boolean DbmFileOutVchunks 
                              (dbheader     *DbHdr,    boolean       Release,
                               ft F,lt Z,zz *Status);

extern boolean DbmCommitDb    (tokentype    *Anchor,  boolean    RollOut,
                               ft F,lt Z,zz *Status);

extern boolean DbmLoadDbHdr   (tokentype    *DbToken, char      *LoadFileName,
                               ft F,lt Z,zz *Status,  dbheader **CoreDb);

extern boolean OaReclaimHandles
                              (tokentype    *Anchor,  numtype      ObjNum,
                               ft F,lt Z,zz *Status);

extern  tokentype    WenvTkn;


#ifdef __ZTC__
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#else

#ifdef MCSTAT
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (Anchor->DbId <= MaxCoreDbId \
 && ((*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr)) ? True : \
     TmFetchCoreDb(Anchor,Stat,DbInCore)
#else
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#endif 
#endif 
/*     3.0                                                                 */
/*+-----------------------------------------------------------------------+
  !                 ....       PRIMAL       ....                          !
  +-----------------------------------------------------------------------+*/

/*     3.1.1                                                              */
boolean    Primal_CreateDb
                         (char         *SchemaName,char         *DbName,
                          char         *DbFileName,dbaccesstype  DbAccess,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,    tokentype    *PrimalTkn)
{
  TraceMsg (0, "  Primal_CreateDb Accessed !!!\n");

  Alloc_Handles          = RgnHndls;
  Extend_Handles         = HndlRgns;

  Alloc_Vchunks          = RgnVhndls;
  Extend_Vchunks         = VhndlRgns;

  DbXmemBlocks_AllocQty  = RgnXmems;
  DbXmemBlocks_ExtendQty = XmemRgns;

  if (DbName  == NullPtr)
  {
     TraceMsg (0, "  Primal_CreateDb:: NULL DbName\n");
    *Status  = Primal_NullDbName;
  }

  if (DbFileName  == NullPtr)
  {
     TraceMsg (0, "  Primal_CreateDb:: NULL DbFileName\n");
    *Status  = Primal_NullDbFileName;
  }

  if (Normal(*Status))
  if (!KernelFindIn   (WenvDbs_Hndl,     &WenvTkn,    (addrtype )DbName,
                       McStat,                                   PrimalTkn))
  {
    if (*Status == Set_NotFound)
    {
      *Status = Env_Normal;
       ImageCreateDb  (SchemaName,  DbName,       DbFileName,   DbAccess,
                       McStat,      PrimalTkn);
    }
  } else {
     Primal_ActivateDbByName (DbName, DbFileName, DbAccess, 
                              McStat,             PrimalTkn);
    *Status = Csh_DbAlreadyExists;
  }

TRACK(TrackBak,"Primal_CreateDb\n");
return (STAT);
}


/*     3.1.2                                                              */
boolean   Primal_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    tokentype    *PrimalTkn)
{
return(False);
}


/*     3.1.3                                                              */
boolean   Primal_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,ft F,lt Z,zz *Status)
{
return(False);
}



/*     3.1.9                                                              */
boolean Primal_FreeDb    (tokentype    *PrimalTkn, ft F,lt Z,zz *Status)
{

 *Status   = Err_NotImplemented;

return (STAT);
}

/*     3.1.10                                                             */
boolean Primal_CommitDb  (tokentype    *PrimalTkn, char         *DbFileName,
                          boolean       Release,   ft F,lt Z,zz *Status)
{
 *Status   = Err_NotImplemented;

return (STAT);
}


/*     3.1.11                                                             */
boolean Primal_DeleteDb  (tokentype    *PrimalTkn, ft F,lt Z,zz *Status)
{
 *Status   = Err_NotImplemented;

return (STAT);
}

/* .......................................................................
                    ....  Class Activity   ....
   .......................................................................*/
/*     3.2.1                                                              */
boolean Primal_ConfigYourClass
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          ft F,lt Z,zz *Status)
{

  ImageConfigClass   (PrimalTkn,    ClassId,     RgnEntrys,    AllocRgns,
                      ExtendRgns,   AllocXs,     ExtendXs,     AccessType,
                      DiskCache,    CacheLimit,  CacheLoad,    McStat);

return (STAT);
}




/*     3.2.2                                                              */
boolean Primal_GetClassObjectCount
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status,    numtype      *Count)
{

  Image_GetClassObjectCount (PrimalTkn,  ClassId,    McStat,  Count);

TRACK(TrackBak,"Primal_GetClassObjectCount\n");
return (STAT);
}


/*     3.2.8                                                              */
boolean Primal_IterateOnClassObjects
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status,
                          tokentype    *ItorTkn,   tokentype    *Token)
{
dbheader     *DbHdr        = NullPtr;
numtype       LocalObjNum  = 0;
boolean       NextObject   = False;

 *Token   = NullToken;

  if (*Status == Iterate_Begin)
  {
    *Status    = Env_Normal;

     ItorTkn->Handle = 2;                  /* start after handle to Primal */

     if (TmGetCoreDb  (PrimalTkn,                McStat,         &DbHdr))
     if (MemGetObjNum (DbHdr->LocalObjNumMap,    ClassId, 
                       McStat,                  &LocalObjNum))
     if (LocalObjNum == 0)
       *Status          = Iterate_EndOf;
     else
        ItorTkn->DbId   = (idtype )LocalObjNum;
  } else
     TmGetCoreDb      (PrimalTkn,                McStat,         &DbHdr);

  while (Normal(*Status)
  &&     NextObject      == False
  &&   ++ItorTkn->Handle  < DbHdr->NextHandle)
  {
     if (HmGetLocalObjNum (DbHdr,   ItorTkn->Handle,  McStat,  &LocalObjNum))
     if (ItorTkn->DbId  == (idtype )LocalObjNum)
        NextObject  = True;
  }
  if (*Status         == Iterate_EndOf)
     return (False);

  if (Normal(*Status))
  if (NextObject      == False)
  {
     *Status           = Iterate_EndOf;
      return (False);
  } else {
      Token->DbId      = PrimalTkn->DbId;
      Token->Handle    = ItorTkn->Handle;
  }


TRACK(TrackBak,"Primal_IterateOnClassObjects\n");
return (STAT);
}


/*     3.2.9                                                              */
boolean Primal_FreezeClass
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status)
{

  KernelFreezeObjClass      (PrimalTkn,     ClassId,      McStat);

TRACK(TrackBak,"PrimalFreezeClass\n");
return (STAT);
}




/*     3.2.12                                                             */
boolean Primal_FreeClass 
                         (tokentype    *Token,     numtype       ClassId,
                          ft F,lt Z,zz *Status)
{

  ImageFreeClassById   (C_Apl,   Token->DbId,  ClassId,   McStat);

TRACK(TrackBak,"Primal_FreeClass\n");
return (STAT);
}



/*+-----------------------------------------------------------------------+
~M                         END OF PRIMAL.C                                !
  +-----------------------------------------------------------------------+*/
