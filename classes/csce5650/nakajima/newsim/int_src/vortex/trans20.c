/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* TRANS20.c */
#define  TRANS20
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "fm.h"
#include "env0.h"
#include "odbm.h"
#include "vdbm.h"
#include "om.h"
#include "vom.h"
#include "oa0.h"
#include "voa.h"
#include "vchunk.h"
#include "env1.h"
#include "oa1.h"
#include "sm.h"
#include "trans.h"
#include "api.h"
 
#define        That_AttrNum           (numtype )1

#define MemGetFieldNum      Mem_GetWord
#define MemGetObjNum        Mem_GetWord
#define MemGetLocalObjNum   Mem_GetWord
#define MemGetObjHdr        Mem_GetAddr

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

extern farlongtype CoreBlk_GapSpace;

/*+-----------------------------------------------------------------------+*
~C                        TRANSFER MANAGER
 *+-----------------------------------------------------------------------+*/
boolean    C_FreeClass    (idtype        DbId,    numtype       ClassId,
                           numtype       AplId,   ft F,lt Z,zz *Status)
{
   *Status = Err_NotImplemented;

TRACK(TrackBak,"C_FreeClass\n");
return (STAT);
}


boolean    C_FreeObject    (tokentype    *Token,     numtype       AplId,
                            boolean       FreeImage, ft F,lt Z,zz *Status)
{
   *Status = Err_NotImplemented;

TRACK(TrackBak,"C_FreeObject\n");
return (STAT);
}


boolean    C_DeleteObject (tokentype    *Token,   numtype       AplId,
                           ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
addrtype    Image          = NullPtr;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
addrtype   *FieldMap       = NullPtr;
typesize   *DbOffsets      = NullPtr;
typesize   *CppOffsets     = NullPtr;
typesize   *FieldSizes     = NullPtr;
attrtype   *FieldTypes     = NullPtr;
numtype    *FieldAttrs     = NullPtr;      
numtype    *RefObjNums     = NullPtr;      
indextype   NumMapFields   = 0;
typesize    ObjSize        = 0;
tokentype   DbToken        = *Token;


  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))

  if (Normal(*Status))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  {

    if (HmGetObjectAddr    (CoreDb,  Token->Handle,  McStat,  &Image))
    if (HmPutObjectAddr    (CoreDb,  Token->Handle,  NullPtr,  McStat))
    if (HmPutObjectMemLoc  (CoreDb,  Token->Handle,  0,        McStat))
        OaGetObject        (Token,   McStat,        &DbObject);

    if (Normal(*Status))
    if (MemGetAddr        (C_ObjMapDir,                            ObjNum,
                           McStat,                     (addrtype *)&FieldMap))
    if (FieldMap)
    {
       DbOffsets    =  (typesize *)FieldMap[0];
       CppOffsets   =  (typesize *)FieldMap[1];
       FieldSizes   =  (typesize *)FieldMap[2];
       FieldTypes   =  (attrtype *)FieldMap[3];
       FieldAttrs   =  (numtype  *)FieldMap[4];
       RefObjNums   =  (numtype  *)FieldMap[5];

       NumMapFields =  (indextype )RefObjNums[0];
       ObjSize      =  (typesize  )CppOffsets[0];

    } else
      *Status = Trans_NullMap;

  }

  if (Normal(*Status))
  {
     OaDelete           (&DbToken,      McStat);
  }

  if (MemLoc)
  if (Normal(*Status))
  if (AplId == Cpp_Apl)
       Core0FreeCoreSpace  (&Image, (bytessize )ObjSize, McStat);
  else
       Core0FreeCoreSpace  (&Image, (bytessize )ObjSize, McStat);

TRACK(TrackBak,"C_DeleteObject\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF TRANS20.C                                !
  +-----------------------------------------------------------------------+*/
