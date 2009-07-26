/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - Env0.c */
/*+-----------------------------------------------------------------------+
  |                             VORTEx                                   |
  |                           ENVIORNMENT                                 |
  +-----------------------------------------------------------------------+*/
#define ENV0
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"

#ifdef ENV01
#include "odbm.h"
#include "env1.h"
#include "om.h"
#include "oa0.h"
#include "voa.h"


/* these Constants are defined in the initial BuildEnv run.             */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        DbPrimal_Index         (indextype )0
#define        ObjDesc_Index          (indextype )1

#define        DbPrimal_Class         (word )0
#define        ObjDesc_Class          (numtype )1

#define        ObjDesc_Hndl           (handletype )3

#define  Alloc_Primals     (indextype )1
#define  Alloc_Descs       (indextype )600
#endif

#endif


#define CoreMoreCore       Core_MoreCore
#define Core0MoreCore      Core0_MoreCore
#define MemMakeOry         Mem_MakeOry
#define MemTestTheOry      Mem_TestTheOry
#define MemPushAddr        Mem_PushAddr
#define MemPushWord        Mem_PushWord
#define MemPutWord         Mem_PutWord
#define MemPutLong         Mem_PutLong
#define DbmLoadDbHdr       Dbm_LoadDbHdr

extern boolean CoreMoreCore   (bytessize  Size,
                               ft F,lt Z,zz *Status,    addrtype   *Addr);

extern boolean Core0MoreCore  (bytessize     Size,
                               ft F,lt Z,zz *Status,    addrtype   *Addr);

extern boolean MemMakeOry     (ft F,lt Z,zz *Status,    addrtype   *TheOry);
extern boolean MemTestTheOry  (ft F,lt Z,zz *Status);

extern boolean MemPushWord    (numtype       Chunk,     word        WordValue,
                               ft F,lt Z,zz *Status,    indextype  *StackPtr);

extern boolean MemPutWord     (numtype       Chunk,     indextype   Index,
                               word          WordValue, 
                               ft F,lt Z,zz *Status);

extern boolean MemPutLong     (numtype       Chunk,     indextype     Index,
                               lt64          WordValue, ft F,lt Z,zz *Status);

extern boolean MemPushAddr    (numtype       Chunk,     addrtype    Addr,
                               ft F,lt Z,zz *Status,    indextype  *StackPtr);

extern boolean DbmLoadDbHdr   (tokentype    *DbToken,
                               char         *LoadFileName,
                               ft F,lt Z,zz *Status,    dbheader  **CoreDb);

#define MemPutLocalObjNum   Mem_PutWord
#define MemPushSize         Mem_PushWord
#define MemPushObjNum       Mem_PushWord
#define MemPutObjNum        Mem_PutWord
#define MemPushObjHdr       Mem_PushAddr
#define MemPutOffset        Mem_PutWord
#define MemPutHandle        Mem_PutWord
#define MemPutFieldNum      Mem_PutWord
#define MemPushFieldNum     Mem_PushWord
#define MemPushFieldOffset  Mem_PushWord
#define MemPushAttrNum      Mem_PushWord
#define MemPutAttrNum       Mem_PutWord
#define MemPushNum          Mem_PushWord

extern    farlongtype       Core_PageSize;
extern    boolean           WriteHdr;
extern    char              DbFileName[];

/*+-----------------------------------------------------------------------+
~01                           VORTEx                                     !
  +-----------------------------------------------------------------------+*/
boolean    VORTEx (ft F,lt Z,zz *Status)
{

 *Status = Env_Normal;
 if (CreateImage   (McStat ,  &Image01))
 if (ImplodeTheOry (Image01,  McStat)) 
 {
#ifdef ENV01
    RenvTkn.DbId   = RenvId;
    RenvTkn.Handle = Renv0_Hndl;
#endif

    if (DeBug == 7)
        MemTestTheOry  (McStat);
 }

 if (Normal(*Status))
 if (!Reload_Env) 
 {
    if(CreateKernel (Image01,  McStat,  &CoreDb0,  &CoreDb1))
        ;

 } else {

    if (!DbmBug)
       WriteHdr  = True;

    if (LoadKernel  (Image01,  McStat,  &CoreDb0,  &CoreDb1))
    if (LoadEnv0    (McStat ))
#ifdef ENV01
    if (LoadObj0    (McStat ))
#endif
       *Status = Env_Installed;
 }

 sprintf (Msg, " VORTEx_Status == %d || %x\n", *Status, *Status);
 TraceMsg (MSGFILE, Msg);

 TRACK(TrackBak,"VORTEx\n");
return(STAT);
}
 

boolean    CreateImage    (ft F,lt Z,zz *Status,  imagetype **Image)
{
imagetype  *TheImage     = NullPtr;
objtype    *TheCoreImage = NullPtr;

  if (!Reload_Env) 
  {
     sprintf (Msg, " ALLOC   CORE_0    :: %u\n", sizeof(objtype));
     SendMsg (0, Msg);
     if (Core0MoreCore (sizeof(objtype), McStat,  (addrtype *)&TheCoreImage)) 
     {
        sprintf (Msg, "                   KERNEL @ %x \n", TheCoreImage);
        SendMsg (0, Msg);
     }
  }

  if (Normal(*Status)) 
  {
  /* if (FAR_MEM) Core1BlockElems   = (farlongtype )farcoreleft() -  64; */
     sprintf (Msg, " ALLOC   CORE_1    :: %u\n", sizeof(imagetype));
     SendMsg (MSGFILE, Msg);
     if (CoreMoreCore (sizeof(imagetype), McStat,  (addrtype *)&TheImage)) 
     {
       *Image            = (imagetype *)TheImage;
     }
  }

 TRACK(TrackBak,"CreateImage\n");
return(STAT);
}
 

boolean    ImplodeTheOry (imagetype     *Image,   ft F,lt Z,zz *Status)
{
addrtype   Theory   = NullPtr;

  if (MemMakeOry (McStat , &Theory)) 
  {
     Image->InTheOry  = Theory;
     sprintf (Msg, " BHOOLE NATH\n");
     TraceMsg (MSGFILE, Msg);
  }

 TRACK(TrackBak,"ImplodeTheOry\n");
return(STAT);
}
 

boolean    CreateKernel (imagetype      *Image,   ft F,lt Z,zz *Status,
                         dbheader      **Image0,  dbheader    **Image1)
{
  if (CoreMoreCore (sizeof(corekernel), McStat,  (addrtype *)&Kernel)) 
  {
    Image->InCore           = (addrtype )Kernel;
    SysBaseAddr.Address     = Image->InCore;

    if (HmMakeDbHdr (Env_Objs,   Env_Attrs,   RenvId, McStat,    Image0)) 
    {
        CoreDbs[RenvId]     = *Image0;
        (*Image0)->DbId     = RenvId;
        sprintf (Msg, " KERNEL in CORE[%2u] @ %x\n", RenvId, *Image0);
        TraceMsg (0, Msg);
    }
   *Image1                  = NullPtr;
 }

 TRACK(TrackBak,"CreateKernel\n");
return(STAT);
}
 

boolean    LoadKernel (imagetype *Image,   ft F,lt Z,zz *Status,
                       dbheader **Image0,   dbheader **Image1)
{
  *Image0    =    NullPtr;
  *Image1    =    NullPtr;

  if (CoreMoreCore (sizeof(corekernel), McStat,  (addrtype *)&Kernel)) 
  {
    Image->InCore        = (addrtype )Kernel;
    SysBaseAddr.Address  = Image->InCore;

    if (DbmLoadDbHdr (&RenvTkn, DbFileName, McStat, Image0)) 
    {
        CoreDbs[RenvId]  = *Image0;
        sprintf (Msg, " KERNEL in CORE[%2u] Restored @ %x\n",
                 (*Image0)->DbId, *Image0);
        TraceMsg (0, Msg);
    }

#ifdef ENV01
    if (Normal(*Status))
    if (Reload_Wenv) 
    {
       if (DbmLoadDbHdr (&WenvTkn, WenvFileName, McStat, Image1))
           CoreDbs[WenvId] = *Image1;
    }
#endif

  }

#ifdef ENV01
 if (Normal(*Status))
 if (OaGetVchunkAddr (&HdrSizeVchunkTkn,   McStat, (addrtype *)&ObjHdrSize))
 if (OaGetVchunkAddr (&AllocQtyVchunkTkn,  McStat, (addrtype *)&ObjAllocQty))
 if (OaGetVchunkAddr (&RegionQtyVchunkTkn, McStat, (addrtype *)&ObjRegionQty))
 if (OaGetVchunkAddr (&SectorQtyVchunkTkn, McStat, (addrtype *)&ObjSectorQty))

 if (DeBug)
 if (OaDumpVchunkVchunk  (&SectorQtyVchunkTkn, McStat))
     OaDumpVchunk        (&HdrSizeVchunkTkn, McStat);
#endif

 TRACK(TrackBak,"LoadKernel\n");
return(STAT);
}
 
/*+-----------------------------------------------------------------------+
~E0                           ENV0                                        !
  +-----------------------------------------------------------------------+*/
 

boolean    LoadEnv0     (ft F,lt Z,zz *Status)
{

#ifdef ENV01
 if (TmRenvToken   (McStat ,   RenvTknPtr))

  OaGetObject (RenvTknPtr,  McStat, (addrtype *)&Env0);
#endif

 TRACK(TrackBak,"LoadEnv0\n");
return(STAT);
}
 
#ifdef ENV01
 

boolean    LoadObj0 (ft F,lt Z,zz *Status)
{
addrtype     ObjDesc  = NullPtr;
tokentype    ObjToken;

  ObjToken.DbId    = RenvId;
  ObjToken.Handle  = ObjDesc_Hndl;

  OaGetObject  (&ObjToken,   McStat,       &ObjDesc);

 TRACK(TrackBak,"LoadObj0\n");
return(STAT);
}
#endif /* ENV0  */
/*+-----------------------------------------------------------------------+
~M                         END OF ENV0.C                                  !
  +-----------------------------------------------------------------------+*/
