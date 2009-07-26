/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Shell - ifm.c */
#define   IFM
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#ifdef __MACROIZE_HM__
#include "mem010.h"
#endif /* __MACROIZE_HM__ */
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "env1.h"
#include "km.h"
#include "trans.h"
#include "vchunk.h"
#include "oa0.h"
#include "im.h"
#include "iam.h"

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

extern boolean OaDumpObject   (int           TraceSwi,  tokentype *Token,
                               ft F,lt Z,zz *Status);

extern idtype     RenvId;
/*+-----------------------------------------------------------------------+
~AM                       IMAGE FIELD MANAGER                             !
  +-----------------------------------------------------------------------+*/

/* C; C++                                                                 */      
boolean ImagePutFieldValue 
                          (tokentype    *Token,    fieldstruc   *FS,
                           addrtype      Value,    ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     BldDescTkn   = NullToken;
objdesctype  *BldDesc      = NullPtr;
sizetype      Offset       = 0;
tokentype     RefTkn       = NullToken;
addrtype      BytePtr      = NullPtr;
char         *StrPtr       = NullPtr;
numtype       AttrId       = FS->FieldTkns[FS->SubLevel-1].AttrId;

  AttrTkn.DbId   = RenvId;
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
              " Image::PutFieldValue Object[%2u:%4u]    AttrId= %2u @%x\n",
               Token->DbId, Token->Handle, AttrId, Value))
     TraceMsg (0, Msg);

  if (Token->DbId)
  if (TmFetchCoreDb      (Token,       McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc  (CoreDb,      Token->Handle,
                          McStat,                             &MemLoc))

  if (EnvFetchAttrHandle (AttrId,      McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aChunkAttr
  ||  AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr)
  {
    *Status = Image_AttrNotPutArrayType;

     TraceMsg (0, " Image_PutFieldValue:: Invalid Attribute Type for PutValue.\n");
     TraceMsg (0, "    Arrays Must be instantiated with Array Methods.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrId);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (AttrDesc->AttrType        == aStrHndlAttr)
  {
     StrPtr                      = (char *)Value;
     if (sprintf (Msg, " Image::PutValue [%2u:%4u] String @%x := <%s>)\n",
                  Token->DbId, Token->Handle, Value,   StrPtr))
        TraceMsg (0, Msg);

     KernelPutField       (Token,   FS,     Value,     McStat);

  } else if (AttrDesc->AttrType == aObjRefAttr) {
     if (MemLoc                 == 0)
        KernelPutField    (Token,   FS,  (addrtype )&Value,     McStat);

     else {
        BldDescTkn.DbId          =  RenvId;
        BldDescTkn.Handle        =  AttrDesc->BuildDesc;
        if (OaGetObject  (&BldDescTkn,  McStat,  (addrtype *)&BldDesc))
        if (Env_FetchObjAttrOffset
                         (MemLoc,      BldDesc->ObjNum,      Token_Attr,
                          McStat,              &Offset))
        {
           BytePtr     = (addrtype )((char *)Value     + Offset);
           MoveBytes     (BytePtr,  (addrtype )&RefTkn,   sizeof (tokentype));
           if (RefTkn.DbId
           &&  RefTkn.Handle)
              KernelPutField
                         (Token,   FS,     (addrtype )&RefTkn, McStat);
        }
        if (ClassBug || DeBug || OaBug)
        if (sprintf (Msg, 
         " Image::PutFieldValue Reference[%2u:%4u]  @%x)\n",
                     RefTkn.DbId, RefTkn.Handle, *(addrtype **)Value))
           TraceMsg (0, Msg);
     }

  } else
     KernelPutField       (Token,   FS,     Value,     McStat);


TRACK(TrackBak,"ImagePutFieldValue\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageGetFieldValue (tokentype    *Token,    fieldstruc   *FS,
                           ft F,lt Z,zz *Status,   addrtype      Value)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     VchunkTkn    = NullToken;
vchunktype   *Vstruc       =  (vchunktype *)Value;
sizetype      Vsize        = 0;
addrtype      Image        = NullPtr;
addrtype      BytePtr      = NullPtr;
int           i            = 0;
handletype   *Handles      = NullPtr;
handletype   *ArrayPtrs    = NullPtr;
RefObj       *RefObjVal    = NullPtr;
numtype       AttrId       = FS->FieldTkns[FS->SubLevel-1].AttrId;

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrId,      McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType == aObjRefAttr
  &&  MemLoc             == 0)
     KernelGetField      (Token,       FS,      
                         McStat,                   (addrtype )&VchunkTkn);
  else                                 
     KernelGetField      (Token,       FS,      McStat,        Value);

  if (Normal(*Status))
  if (MemLoc                    == 0)
  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
  /* move internally stored Vchunk (Handle / Token) to Vstruc       */
     VchunkTkn.DbId              = Token->DbId;
     VchunkTkn.CoreDbId          = 0;
     MoveBytes   (Value, (addrtype )&VchunkTkn.Handle, sizeof (handletype));

     if (AttrDesc->AttrType     == aArrayAttr)
     {
        if (VchunkTkn.Handle     > 0)
            Vchunk_GetAddress  (&VchunkTkn,  McStat,  (addrtype *)Value);
        else
            MoveBytes   (&Image, Value,      sizeof (addrtype));

     } else {
        Vstruc->VchunkTkn        = VchunkTkn;
        if (Vchunk_GetAddress  (&VchunkTkn,  McStat,         &Vstruc->Varray))
        if (Vchunk_GetStackPtr (&VchunkTkn,  McStat,         &Vstruc->Vsize))

        if (AttrDesc->AttrType  == aChunkAttr)
        {
           Vstruc->VstackPtr     = Vstruc->Vsize;
           Vchunk_GetChunkSize (&VchunkTkn,  McStat, &Vstruc->Vsize, &Vsize);
        }
     }

  } else if (AttrDesc->AttrType == aDblPtrAttr) {
  /* move internally stored Vchunk (Handle / Token) to Vstruc       */
     VchunkTkn.DbId              = Token->DbId;
     VchunkTkn.CoreDbId          = 0;
     MoveBytes   (Value, (addrtype )&VchunkTkn.Handle, sizeof (handletype));

     if (VchunkTkn.Handle        > 0)
     {
        if (Vchunk_GetAddress  (&VchunkTkn,  McStat, (addrtype *)&ArrayPtrs))
        if (Vchunk_GetStackPtr (&VchunkTkn,  McStat,             &Vsize))
        {
            Vsize               /= 2;
            Handles              = (handletype *)&ArrayPtrs[Vsize];
        }

        for (i = -1; ++i < Vsize && Normal(*Status); )
        {
          if ((VchunkTkn.Handle  = Handles[i]) > 0)
          {
             Vchunk_GetAddress (&VchunkTkn,  
                                McStat,           (addrtype *)&ArrayPtrs[i]);
          } else
             ArrayPtrs[i]         = NullPtr;
        }
        if (Normal(*Status))
          MoveBytes (&ArrayPtrs, Value,      sizeof (addrtype));

     } else
        MoveBytes   (&Image,     Value,      sizeof (addrtype));

  } else if (AttrDesc->AttrType == aObjRefAttr) {

     if (VchunkTkn.DbId
     &&  VchunkTkn.Handle)
        ImageGetObject         (&VchunkTkn,  McStat,         &Image);

     MoveBytes      (&Image,     Value,      sizeof (addrtype));

     if (ClassBug || DeBug || OaBug)
     if (sprintf (Msg, 
      " Image::GetValue Object[%2u:%4u] Reference @%x (Value := @%x)\n",
                  VchunkTkn.DbId, VchunkTkn.Handle,
                  Image,         *(addrtype **)Value))
        TraceMsg (0, Msg);

  } else if (AttrDesc->AttrType == aTknRefAttr) {

     BytePtr    = (addrtype )((char *)Value     + sizeof (addrtype));

     MoveBytes   (Value,  (addrtype )&VchunkTkn,  sizeof (tokentype));

     if (sprintf (Msg, "  Get TknRef[%4u:%6u]Attr  Value @%8x; BytePtr @%8x\n",
                 VchunkTkn.DbId, VchunkTkn.Handle, Value, BytePtr))
         TraceMsg (0, Msg);

     MoveBytes   (&Image,                Value,   sizeof (addrtype));
     MoveBytes   ((addrtype )&VchunkTkn, BytePtr, sizeof (tokentype));

  }

  if (Normal(*Status))
  if (MemLoc                    == 1)
  if (AttrDesc->AttrType        == aTknRefAttr)
  {
     if ((RefObjVal = *(RefObj **)Value) != NULL)
     if (sprintf (Msg, "  Get TknRef[%4u:%6u]Attr  ObjAddr @%8x;\n",
                 RefObjVal->ObjTkn.DbId, RefObjVal->ObjTkn.Handle,
                 RefObjVal->ObjAddr))
         TraceMsg (0, Msg);
  }


TRACK(TrackBak,"ImageGetFieldValue\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF IFM.C                                   !
  +-----------------------------------------------------------------------+*/
