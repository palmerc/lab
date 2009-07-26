/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OaDmp.c */
#define OADMP
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "grp.h"
#include "oa0.h"
#include "voa.h"
#include "vchunk.h"
#include "env1.h"
#include "oa1.h"

/*+-----------------------------------------------------------------------+*
~OADMP                       EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        ObjDesc_ObjNum         (objnumtype )1

#define        ReferenceType_Hndl     (handletype )190
#define        EnumValues_Hndl        (handletype )665

#endif

#define MemGetVbn           Mem_GetWord
#define MemPutVbn           Mem_PutWord
#define MemGetChunkNum      Mem_GetWord
#define MemPutChunkNum      Mem_PutWord
#define MemGetLocalObjNum   Mem_GetWord
#define MemPutLocalObjNum   Mem_PutWord
#define MemPushSize         Mem_PushWord
#define MemGetSize          Mem_GetWord
#define MemGetAttrSize      Mem_GetWord
#define MemPushObjNum       Mem_PushWord
#define MemPutObjNum        Mem_PutWord
#define MemGetObjNum        Mem_GetWord
#define MemPushObjHdr       Mem_PushAddr
#define MemPutOffset        Mem_PutWord
#define MemGetOffset        Mem_GetWord
#define MemGetAttrNum       Mem_GetWord
#define MemGetObjHdr        Mem_GetAddr

extern boolean DbmLoadObjHdr  (tokentype    *DbToken,   numtype     LocalObj,
                               ft F,lt Z,zz *Status,    objheader **ObjHdr);

extern boolean DbmDumpVchunkVchunk
                              (tokentype    *Token,   ft F,lt Z,zz *Status);
extern boolean DbmDumpVchunk  (tokentype    *Token,
                               numtype       Limit0, numtype        Limit1,
                               ft F,lt Z,zz *Status);

extern boolean KernelFindIn
                          (handletype    SetDesc,   tokentype    *OwnerTkn,
                           addrtype      KeyValue,
                           ft F,lt Z,zz *Status,    tokentype    *MemberTkn);
 
extern  char        *DescNam[];
extern  char        *LowTypeNam[];

       boolean OaDumpEmbedded
                          (int           TraceSwi,  numtype       MemLoc,
                           tokentype    *Token,     addrtype      Object, 
                           handletype    BuildDesc, numtype       NumMaps,
                           sizetype      MapOffset, ft F,lt Z,zz *Status);

        char        *booleanValue[2] = { "False", "True" };
        boolean      LinkSwi = False;


/*+-----------------------------------------------------------------------+*
~OA                        OBJECT DUMP                                    !
 *+-----------------------------------------------------------------------+*/
boolean    OaDumpObjHdr (int TraceSwi,  tokentype     *Anchor,
                         num ObjNum,    ft F,lt Z,zz *Status)
{
 
TRACK(TrackBak,"OaDumpObjHdr\n");
return (STAT);
}



boolean    OaDumpObjHndls (tokentype    *Anchor,  numtype      ObjNum,
                           ft F,lt Z,zz *Status)
{
 
TRACK(TrackBak,"OaDumpObjHndls\n");
return (STAT);
}
 

boolean    OaDumpObject (int           TraceSwi,  tokentype *Token,
                         ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"OaDumpObject\n");
return (STAT);
}


boolean    OaDumpEmbedded (int           TraceSwi,  numtype       MemLoc,
                           tokentype    *Token,     addrtype      Object, 
                           handletype    BuildDesc, numtype       NumMaps,
                           sizetype      MapOffset, ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"OaDumpEmbedded\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF OADMP.C                                 !
  +-----------------------------------------------------------------------+*/
