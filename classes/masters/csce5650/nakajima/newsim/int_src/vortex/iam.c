/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/

/* Shell - Iam.c */
#define   IAM
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
~IAM                     IMAGE ATTRIBUTE MANAGER                          !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                   ....       ATTRIBUTE    ....
   .......................................................................*/

/* C; C++                                                                 */      
boolean ImagePutAttrValue (tokentype    *Token,    numtype       AttrNum,
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

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb      (Token,       McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc  (CoreDb,      Token->Handle,
                          McStat,                             &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aChunkAttr
  ||  AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr)
  {
    *Status = Image_AttrNotPutArrayType;

     TraceMsg (0, "    Image_PutValue:: Invalid Attribute Type for PutValue.\n");
     TraceMsg (0, "    Arrays Must be instantiated with Array Methods.\n");
     sprintf (Msg, "          for  Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  } else {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  }

  if (Normal(*Status))
  if (AttrDesc->AttrType        == aStrHndlAttr)
  {
     StrPtr                      = (char *)Value;

     KernelPutAttr       (Token,   AttrNum,     Value,     McStat);

  } else if (AttrDesc->AttrType == aObjRefAttr) {
     if (MemLoc                 == 1)
        KernelPutAttr    (Token,   AttrNum,  (addrtype )&Value,     McStat);

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
              KernelPutAttr
                         (Token,   AttrNum,     (addrtype )&RefTkn, McStat);
        }
     }

  } else
     KernelPutAttr       (Token,   AttrNum,     Value,     McStat);


TRACK(TrackBak,"ImagePutAttrValue\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageGetAttrValue (tokentype    *DbmsToken,numtype       AttrNum,
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
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
RefObj       *RefObjVal    = NullPtr;

  AttrTkn.DbId   = RenvId;

  if (DbmsToken->DbId)
  if (TmFetchCoreDb     (DbmsToken,    McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       DbmsToken->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType == aObjRefAttr
  &&  MemLoc             == 0)
     KernelGetAttr      (DbmsToken,   AttrNum,      
                         McStat,      (addrtype )&VchunkTkn);
  else
     KernelGetAttr      (DbmsToken,   AttrNum,      McStat,      Value);

  if (Normal(*Status))
  if (MemLoc                    == 0)
  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
  /* move internally stored Vchunk (Handle / Token) to Vstruc       */
     VchunkTkn.DbId              = DbmsToken->DbId;
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
     VchunkTkn.DbId              = DbmsToken->DbId;
     VchunkTkn.CoreDbId          = 0;
     MoveBytes   (Value, (addrtype )&VchunkTkn.Handle, sizeof (handletype));

     if (VchunkTkn.Handle        > 0)
     {
        if (Vchunk_GetAddress  (&VchunkTkn,  McStat, (addrtype *)&ArrayPtrs))
        if (Vchunk_GetStackPtr (&VchunkTkn,  McStat,             &Vsize))
        {
            Vsize               /= 3;
            ArrayHndls           = (handletype *)ArrayPtrs;
            Handles              = (handletype *)&ArrayHndls[Vsize * 2];
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

     if (ClassBug)
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
     if (ClassBug)
     if (sprintf (Msg, "  Get TknRef[%4u:%6u]Attr  ObjAddr @%8x;\n",
                 RefObjVal->ObjTkn.DbId, RefObjVal->ObjTkn.Handle,
                 RefObjVal->ObjAddr))
         TraceMsg (0, Msg);
  }


TRACK(TrackBak,"ImageGetAttrValue\n");
return (STAT);
}
 
int     ImageCompareAttr (tokentype    *Token,     numtype       AttrNum,
                          addrtype      CompValue, ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status)
{
int           i            = 0;
bytetype     *ByteValue    = NULL;
idtype       *IdValue      = NULL;
inttype      *IntValue     = NULL;
tokentype    *TknValue     = NULL;
tokentype    *CompTkn      = NULL;
float        *FloatValue   = NULL;
double       *DoubleValue  = NULL;
addrtype     *AddrValue    = NULL;
handletype   *HndlValue    = NULL;

dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     VchunkTkn    = NullToken;
vchunktype   *Vstruc       = NullPtr;
sizetype      Vsize        = 0;
addrtype      Image        = NullPtr;
addrtype      BytePtr      = NullPtr;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
addrtype      Value        = NullPtr;
sizetype      Size         = 0;

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,    McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))
  if (EnvFetchAttrSize   (AttrNum,     McStat,                &Size))
  if (MemLoc)
  {
     if (AttrDesc->AttrType         == aTknRefAttr)
       Size       = sizeof (RefObj);
     else if (AttrDesc->AttrType    == aVarrayAttr)
       Size       = sizeof (varraytype);
     else if (AttrDesc->AttrType    == aChunkAttr)
       Size       = sizeof (vchunktype);
  }
  if (Normal(*Status))
  if (AttrDesc->AttrType            != aStrHndlAttr)
     Core0_MoreCore   (Size,   McStat,  &Value);

  if (AttrDesc->AttrType == aObjRefAttr
  &&  MemLoc             == 0)
     KernelGetAttr      (Token,   AttrNum,      
                         McStat,      (addrtype )&VchunkTkn);
  else
     KernelGetAttr      (Token,   AttrNum,      McStat,     Value);

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
        Vstruc                   =  (vchunktype *)Value;
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
            Vsize               /= 3;
            ArrayHndls           = (handletype *)ArrayPtrs;
            Handles              = (handletype *)&ArrayHndls[Vsize * 2];
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
                  Image,         Value))
        TraceMsg (0, Msg);

  } else if (AttrDesc->AttrType == aTknRefAttr) {

     BytePtr    = (addrtype )((char *)Value     + sizeof (addrtype));

     MoveBytes   (Value,  (addrtype )&VchunkTkn,  sizeof (tokentype));

     if (ClassBug)
     if (sprintf (Msg, "  Get TknRef[%4u:%6u]Attr  Value @%8x; BytePtr @%8x\n",
                 VchunkTkn.DbId, VchunkTkn.Handle, Value, BytePtr))
         TraceMsg (0, Msg);

     MoveBytes   (&Image,                Value,   sizeof (addrtype));
     MoveBytes   ((addrtype )&VchunkTkn, BytePtr, sizeof (tokentype));

  }

  if (Normal(*Status))
  switch (AttrDesc->TypeType)
  {

      case aDbmsString :
      case aString     :
                  if (AttrDesc->UnitSize == 1)
                  {
                     ByteValue    = (bytetype *)CompValue;
                     if (*(bytetype *)Value < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)Value > *ByteValue)
                       i = 1;
                  } else
                     i = strcmp  ((char *)Value,  (char *)CompValue);
                  break;

      case aLink       :
                  if (Size == sizeof (tokentype ))
                  {
                     TknValue    = (tokentype *)CompValue;
                     CompTkn     = (tokentype *)Value;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;

                  } else {
                     HndlValue   = (handletype *)CompValue;
                     if (*(handletype *)Value < *HndlValue)
                       i = -1;
                     else if (*(handletype *)Value > *HndlValue)
                       i = 1;

                  }
                  break;

      case aReal       :
                  if (Size == sizeof (float)) 
                  {
                     FloatValue    = (float *)CompValue;
                     if (*(float *)Value < *FloatValue)
                       i = -1;
                     else if (*(float *)Value > *FloatValue)
                       i = 1;

                  } else if (Size == sizeof (double)) {
                     DoubleValue    = (double *)CompValue;
                     if (*(double *)Value < *DoubleValue)
                       i = -1;
                     else if (*(double *)Value > *DoubleValue)
                       i = 1;

                  }
                  break;

      case aAddr       :
                  AddrValue    = (addrtype *)CompValue;
                  if (*(addrtype *)Value != AddrValue)
                       i = 1;
                  break;


      case aEnum       :
      case aBoolean    :
      case aValue      :
      case aChunk      :
      case aInteger    :
          
         switch (Size)
         {
            case 1     :
                     ByteValue    = (bytetype *)CompValue;
                     if (*(bytetype *)Value < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)Value > *ByteValue)
                       i = 1;
                     break;

            case 2     :
                     IdValue    = (idtype *)CompValue;
                     if (*(idtype *)Value < *IdValue)
                       i = -1;
                     else if (*(idtype *)Value > *IdValue)
                       i = 1;
                     break;

            case 4     :
                     IntValue    = (inttype *)CompValue;
                     if (*(inttype *)Value < *IntValue)
                       i = -1;
                     else if (*(inttype *)Value > *IntValue)
                       i = 1;
                     break;


            default    :
#ifdef LARGE
                if (Size == sizeof (tokentype))
                {
                     TknValue    = (tokentype *)CompValue;
                     CompTkn     = (tokentype *)Value;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;
                 } else {
#endif
                     if (Size <= sizeof (tokentype ))
                         i = memcmp  ((char *)Value, 
                                      (char *)CompValue, (size_t )Size);
                     else 
                         i = strncmp ((char *)Value, 
                                       (char *)CompValue, (size_t )Size);
#ifdef LARGE
                 }
#endif
         }
         break;
  }
  if (Normal(*Status))
  if (AttrDesc->AttrType            != aStrHndlAttr)
     Core0_FreeCore   ((addrtype *)&Value,  Size,   McStat);

STAT;
TRACK(TrackBak,"ImageCompareAttr\n");
return (i);
}


boolean ImagePutObjTkn   (tokentype    *Token,     numtype       AttrNum,
                          tokentype    *ObjTkn,    ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      ObjAddr      = NullPtr;

  AttrTkn.DbId   = RenvId;

  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
              " Image::PutObjTkn To [%2u:%4u] Attr= %2u; ObjTkn[%4u:%6u]\n",
               Token->DbId, Token->Handle, AttrNum,
               ObjTkn->DbId, ObjTkn->Handle))
     TraceMsg (0, Msg);

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aObjRefAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotObjRefType;

     TraceMsg (0, " Image_PutObjTkn:: Attribute Not a ObjRef Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (MemLoc             == 0)
  {
     KernelPutAttr      (Token,   AttrNum,     (addrtype )ObjTkn, McStat);

  } else {

     if (ObjTkn->DbId      > 0
     &&  ObjTkn->Handle    > 0)
     {
        if (ImageGetObject (ObjTkn,    McStat,    &ObjAddr))
            KernelPutAttr  (Token,     AttrNum,   (addrtype )&ObjAddr, 
                            McStat);
     }
  }

TRACK(TrackBak,"ImagePutObjTkn\n");
return (STAT);
}
 

boolean ImageGetObjTkn   (tokentype    *Token,     numtype       AttrNum,
                          ft F,lt Z,zz *Status,    tokentype    *ObjTkn)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      ObjAddr      = NullPtr;
tokentype     BldDescTkn   = NullToken;
objdesctype  *BldDesc      = NullPtr;
sizetype      Offset       = 0;
addrtype      BytePtr      = NullPtr;

  AttrTkn.DbId   = RenvId;
 *ObjTkn         = NullToken;

  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
              " Image::GetObjTkn From [%2u:%4u] Attr= %2u;\n",
               Token->DbId, Token->Handle, AttrNum))
     TraceMsg (0, Msg);

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aObjRefAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotObjRefType;

     TraceMsg (0, " Image_GetObjTkn:: Attribute Not a ObjRefType.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (MemLoc             == 0)
  {
     KernelGetAttr      (Token,   AttrNum,   McStat, (addrtype *)ObjTkn);

  } else {

     if (KernelGetAttr  (Token,   AttrNum,   McStat, (addrtype *)&ObjAddr))
     if (ObjAddr         != NullPtr)
     {
        BldDescTkn.DbId          =  RenvId;
        BldDescTkn.Handle        =  AttrDesc->BuildDesc;
        if (OaGetObject  (&BldDescTkn,  McStat,  (addrtype *)&BldDesc))
        if (Env_FetchObjAttrOffset
                         (MemLoc,      BldDesc->ObjNum,      Token_Attr,
                          McStat,              &Offset))
        {
           BytePtr     = (addrtype )((char *)ObjAddr   + Offset);
           MoveBytes     (BytePtr,  (addrtype )ObjTkn,   sizeof (tokentype));
        }
     }
  }

TRACK(TrackBak,"ImageGetObjTkn\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageRefTknPut    (tokentype    *Token,    numtype       AttrNum,
                           RefObj       *RefObjVal,ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;

  AttrTkn.DbId   = RenvId;


  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aTknRefAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {
    *Status = Image_AttrNotRefTknType;

     TraceMsg (0, " Image_RefTknPut:: Attribute Not an Reference Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
      KernelPutAttr     (Token,    AttrNum,   (addrtype )RefObjVal,  McStat);

TRACK(TrackBak,"ImageRefTknPut\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageTokenToRef   (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,   addrtype     *RefObject)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     RefTkn       = NullToken;
RefObj        RefObjVal;

 *RefObject      = NullPtr;
  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aTknRefAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {
    *Status = Image_AttrNotRefTknType;

     TraceMsg (0, " Image_TokenToRef:: Attribute Not a Reference Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (MemLoc             == 0)
  {
     if (KernelGetAttr  (Token,                     AttrNum,      
                         McStat,        (addrtype )&RefTkn))
     if (!TmIsValid     (&RefTkn,     McStat))
     {
       *RefObject         = NullPtr;

     } else {
        if (ImageGetObject (&RefTkn,  McStat,          RefObject))
            Hm_IncrementMemRef
                           (CoreDb,   RefTkn.Handle,   McStat);
     }

  } else {
     if (KernelGetAttr  (Token,       AttrNum,
                         McStat,      (addrtype )&RefObjVal))

     if (!TmIsValid (&RefObjVal.ObjTkn,  McStat))
     {
         RefObjVal.ObjTkn           = NullToken;
         RefObjVal.ObjAddr          = NullPtr;

     } else if (RefObjVal.ObjAddr  == NullPtr) {
         if (ImageGetObject (&RefObjVal.ObjTkn,  McStat,          RefObject))
         if (Hm_IncrementMemRef
                            (CoreDb,   RefObjVal.ObjTkn.Handle,   McStat))
             RefObjVal.ObjAddr      = *RefObject;

         if (ClassBug)
         if (sprintf (Msg, "  Get TknRef[%4u:%6u]Attr  ObjAddr @%8x;\n",
                      RefObjVal.ObjTkn.DbId, RefObjVal.ObjTkn.Handle,
                      RefObjVal.ObjAddr))
            TraceMsg (0, Msg);
     }
  }

TRACK(TrackBak,"ImageTokenToRef\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageNewString    (tokentype    *Token,    numtype       AttrNum,
                           sizetype      StrSize,  
                           ft F,lt Z,zz *Status,   cstring      *StrValue)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
boolean       StringIsA    = False;
tokentype     VchunkTkn    = NullToken;

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType  == aStrHndlAttr) 
     StringIsA                   = False;

  else if (AttrDesc->AttrType  == aArrayAttr 
       &&  AttrDesc->TypeType  == aString)
     StringIsA                   = True;
  else
     StringIsA                   = False;

  if (StringIsA)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrName= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotStringType;

     TraceMsg (0, " Image_NewString:: Attr Not a Allocatable String Type.\n");
     TraceMsg (0, "                   Must be of type <char *>.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

     sprintf (Msg, "        For AttrType = %s; AttrName= %s\n", 
              AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
     ImageCreateArray   (Token,  AttrNum,  StrSize,    sizeof (char ),
                         McStat,         &VchunkTkn,  (addrtype *)StrValue);

TRACK(TrackBak,"ImageNewString\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageGetString    (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,
                           boolean      *StrCpyIsA,cstring      *StrValue)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
boolean       StringIsA    = False;
sizetype      StrSize      = 0;
cstring       Cstring      = NullPtr;
handletype    Chandle      = 0;
tokentype     Vtoken       = NullToken;

 *StrCpyIsA      = False;
  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aStrHndlAttr)  /* a varying type       */
  {
     StringIsA                   = True;

  } else if (AttrDesc->AttrType == aAttr)   {
     if   (AttrDesc->TypeType   == aString
     &&    AttrDesc->UnitSize    > 1)             /* a InStruct string    */
     StringIsA                   = True;

  } else if (AttrDesc->AttrType == aArrayAttr 
         &&  AttrDesc->TypeType == aString) {
     StringIsA                   = True;

  } else
     StringIsA                   = False;

  if (StringIsA)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotStringType;

     TraceMsg (0, " Image_GetString:: Attribute Not a String Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (MemLoc  == 0)
     KernelGetAttr      (Token,   AttrNum,  McStat, (addrtype *)&Chandle);
  else
     KernelGetAttr      (Token,   AttrNum,  McStat, (addrtype *)&Cstring);

  if (MemLoc  == 0)
  {
     if (AttrDesc->AttrType == aArrayAttr) 
     {
        Vtoken.DbId         = Token->DbId;
        Vtoken.Handle       = Chandle;
        if (Vtoken.Handle   > 0)
            Vchunk_GetAddress  (&Vtoken,     McStat,  (addrtype *)&Cstring);
     }
     if (Normal(*Status))
     if (Cstring              != NullPtr) 
     {
        if (AttrDesc->AttrType   != aStrHndlAttr)
        {
          *StrCpyIsA    = True;
           if ((StrSize = strlen (Cstring)) > 0)
           if (Core0MoreCore  (StrSize +1, McStat, (addrtype *)StrValue))
              strncpy (*(char **)StrValue,  Cstring, StrSize);
        } else
          *StrValue   = Cstring;
     }
  } else
   *StrValue   = Cstring;

TRACK(TrackBak,"ImageGetString\n");
return (STAT);
}
 

/* .......................................................................
                   ....       ARRAY        ....
   .......................................................................*/
/* C++                                                                    */      
boolean ImageGetAttrUnitSize
                          (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,   sizetype     *UnitSize)
{
numtype       ObjNum       = 0;
attrtype      AttrType     = aNonTypeAttr;
numtype       RefObjNum    = 0; 
typetype      UnitType     = aNonTypeType;
valueclass    ValueType    = aNonValueClass;
sizetype      Offset       = 0;
tokentype     TypeTkn      = NullToken;
typedesctype *EnvType      = NullPtr;
envclass      RefType      = aNonEnvClass;

    TypeTkn.DbId      = RenvId;

    if (EnvFetchObjNum       (Token,       McStat,      &ObjNum))
    if (Env_FetchObjAttrSpec (ObjNum,      AttrNum,
                              McStat,     &AttrType,    &RefObjNum, &RefType,
                              UnitSize,   &UnitType,    &ValueType,
                             &Offset))
    if (RefObjNum)
    if (RefType          == aEnvObj)
       EnvFetchObjSize (RefObjNum,  McStat, UnitSize);

    else {
       TypeTkn.Handle     = RefObjNum;
       if (OaGetObject   (&TypeTkn,  McStat, (addrtype *)&EnvType))
          *UnitSize       = EnvType->Size;
    }

TRACK(TrackBak,"ImageGetAttrUnitSize\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageCreateArray  (tokentype    *Token,    numtype       AttrNum,
                           indextype     UnitCount,sizetype      UnitSize,
                           ft F,lt Z,zz *Status,
                           tokentype    *VchunkTkn,addrtype     *Varray)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;

  AttrTkn.DbId   = RenvId;

  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, " Image::CreateArray for [%2u:%4u] Attr= %2u @%x\n",
               Token->DbId, Token->Handle, AttrNum, *Varray))
     TraceMsg (0, Msg);

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_CreateArray :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (AttrDesc->AttrType        == aDblPtrAttr)
  {
     UnitSize          = sizeof (handletype);
     C_CreateArray      (Token,       AttrNum,      UnitCount,  UnitSize,
                         McStat,      VchunkTkn,    Varray);
  } else
     C_CreateArray      (Token,       AttrNum,      UnitCount,  UnitSize,
                         McStat,      VchunkTkn,    Varray);

  if (Normal(*Status))
  if (AttrDesc->AttrType        == aArrayAttr)
     Vchunk_PutStackPtr (VchunkTkn,   UnitCount,    McStat,     Varray);
  else if (AttrDesc->AttrType   == aDblPtrAttr)
     Vchunk_PutStackPtr (VchunkTkn,   UnitCount *3, McStat,     Varray);
  else
     Vchunk_PutStackPtr (VchunkTkn,   0,            McStat,     Varray);

  if (!Normal(*Status) || DeBug)
  { 
     sprintf (Msg, "  Image::CreateArray for Token[%2u:%4u] @%x\n",
              Token->DbId, Token->Handle, *Varray);
     TraceMsg (0, Msg);
     if (DeBug || OaBug)
         OaDumpObject    (0, Token, McStat);

  }

TRACK(TrackBak,"ImageCreateArray\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageCreateSubArray (tokentype    *Token,     
                             numtype       AttrNum,  indextype ArrayIndex,
                             indextype     UnitCount,sizetype  UnitSize, 
                             ft F,lt Z,zz *Status,
                             tokentype    *VchunkTkn, addrtype *Varray)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;

  AttrTkn.DbId   = RenvId;

  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, " Image::CreateArray for [%2u:%4u] Attr= %2u @%x\n",
               Token->DbId, Token->Handle, AttrNum, *Varray))
     TraceMsg (0, Msg);

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_CreateArray :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (C_CreateSubArray  (Token,       AttrNum,      ArrayIndex,
                         UnitCount,   UnitSize,
                         McStat,      VchunkTkn,    Varray))
     Vchunk_PutStackPtr (VchunkTkn,   UnitCount,    McStat,     Varray);

  if (!Normal(*Status) || DeBug)
  { 
     sprintf (Msg, 
              "  Image::CreateSubArray for Token[%2u:%4u] Index=%4u @%x\n",
              Token->DbId, Token->Handle, ArrayIndex, *Varray);
     TraceMsg (0, Msg);
     if (DeBug || OaBug)
         OaDumpObject    (0, Token, McStat);

  }

TRACK(TrackBak,"ImageCreateSubArray\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageArrayActivate
                          (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,   tokentype    *Vtoken,
                           numtype      *Vsize,    sizetype     *UnitSize,
                           addrtype     *Varray)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
int           i            = 0;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     VchunkTkn    = NullToken;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
numtype       UnitCount    = 0;
vchunktype    Vstruc;

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_ArrayActivate :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  Vtoken->DbId                   = Token->DbId;
  if (Normal(*Status))
  if (MemLoc)
     *Status   = Attr_ImageIsActive;

  else if (MemLoc     == 0)
  {
     if (KernelGetAttr  (Token,       AttrNum,  McStat,  (addrtype )Vtoken))

     if (Vtoken->Handle          > 0)
     {
        if (Vchunk_GetStackPtr (Vtoken,     McStat,              Vsize))
        if (Vchunk_GetChunkSize
                               (Vtoken,     McStat,  &UnitCount, UnitSize))
            Vchunk_GetAddress  (Vtoken,     McStat,  (addrtype *)Varray);

        if (Normal (*Status))
        if (AttrDesc->AttrType        == aDblPtrAttr) 
        {
          *Vsize                      /= 3;
           ArrayPtrs                   = *(addrtype **)Varray;
           ArrayHndls                  = (handletype *)ArrayPtrs;
           Handles                     = (handletype *)&ArrayHndls[*Vsize * 2];
           VchunkTkn.DbId              = Vtoken->DbId;

           for (i = -1; ++i < *Vsize && Normal(*Status); )
           {
              if ((VchunkTkn.Handle    = Handles[i]) > 0)
              {
                   Vchunk_GetAddress (&VchunkTkn,  
                                      McStat,    (addrtype *)&ArrayPtrs[i]);
              } else
                   ArrayPtrs[i]        = NullPtr;
           }
        }

     } else {
        *Varray                  = NullPtr;
        *Vsize                   = 0;
     }

  } else if (AttrDesc->AttrType        == aArrayAttr
         ||  AttrDesc->AttrType        == aDblPtrAttr)
  {
     if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,        &Object))
     if (OaRevokeCppObject (Token,                   McStat))
     if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,        &ObjNum))
     if (OaGetObject       (Token,                   McStat,        &DbObject))

     if (ClassBug || DeBug || OaBug)
     if (sprintf (Msg, 
        " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
                  ObjNum, Object, DbObject))
        TraceMsg (0, Msg);

     if (Normal (*Status))
     if (KernelGetAttr     (Token,   AttrNum,    McStat,  (addrtype )Vtoken))
         OaInvokeCppObject (Token,   Object,     McStat);

     if (Vtoken->Handle          > 0)
     {
        if (Vchunk_GetStackPtr (Vtoken,     McStat,              Vsize))
        if (Vchunk_GetAddress  (Vtoken,     McStat,  (addrtype *)Varray))
        if (Vchunk_GetChunkSize
                               (Vtoken,     McStat,  &UnitCount, UnitSize))

        if (AttrDesc->AttrType  == aDblPtrAttr) 
          *Vsize                /= 3;

     } else {
        *Varray                  = NullPtr;
        *Vsize                   = 0;
     }

  } else {
     if (KernelGetAttr  (Token,   AttrNum,  McStat,  (addrtype )&Vstruc))
     {
        *Vtoken                  = Vstruc.VchunkTkn;
        *Varray                  = Vstruc.Varray;
        *Vsize                   = Vstruc.Vsize;
         if (Vstruc.VchunkTkn.Handle   > 0)
             Vchunk_GetChunkSize
                        (Vtoken,     McStat,  &UnitCount,        UnitSize);
     }
  }

TRACK(TrackBak,"ImageArrayActivate\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageArrayDeActivate
                          (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     Vtoken       = NullToken;
sizetype      Vsize        = 0;
addrtype      Varray       = NullPtr;
int           i            = 0;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     VchunkTkn    = NullToken;

  AttrTkn.DbId   = RenvId;

  if (Token->DbId)
  if (TmFetchCoreDb     (Token,        McStat,                &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,       Token->Handle,
                         McStat,                              &MemLoc))

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_ArrayActivate :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  Vtoken.DbId                    = Token->DbId;
  if (Normal(*Status))
  if (MemLoc == 0)
  {
     if (KernelGetAttr  (Token, AttrNum,    McStat,  (addrtype   )&Vtoken))
     if (Vtoken.Handle           > 0)
     {
        if (Normal (*Status))
        if (AttrDesc->AttrType        == aDblPtrAttr) 
        if (Vchunk_GetStackPtr (&Vtoken,    McStat,  (indextype *)&Vsize))
        if (Vchunk_GetAddress  (&Vtoken,    McStat,  (addrtype  *)&Varray))
        {
           Vsize                      /= 3;
           ArrayPtrs                   = (addrtype   *)Varray;
           ArrayHndls                  = (handletype *)ArrayPtrs;
           Handles                     = (handletype *)&ArrayHndls[Vsize * 2];
           VchunkTkn.DbId              = Vtoken.DbId;

           for (i = -1; ++i < Vsize && Normal(*Status); )
           {
              if ((VchunkTkn.Handle    = Handles[i]) > 0)
                   Vchunk_GetAddress (&VchunkTkn,  
                                      McStat,    (addrtype *)&ArrayPtrs[i]);
           }
        }
     }

  }

TRACK(TrackBak,"ImageArrayDeActivate\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageArrayGetSize (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,    
                           numtype      *UnitCount,sizetype     *UnitSize)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
tokentype     Vtoken       = NullToken;

 *UnitCount       = 0;
 *UnitSize        = 0;

  AttrTkn.DbId   = RenvId;
  Vtoken.DbId    = Token->DbId;
  Vtoken.Handle  = 0;

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_ArrayGetSize :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (Token->DbId)
  if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                         McStat,                           &MemLoc))

  if (MemLoc)
  if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
  if (OaGetObject       (Token,                   McStat,  &DbObject))
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
             " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
               ObjNum, Object, DbObject))
     TraceMsg (0, Msg);

  if (Normal (*Status))
  if (KernelGetAttr     (Token,   AttrNum,   McStat,  (addrtype )&Vtoken))
  if (MemLoc)
      OaInvokeCppObject (Token,   Object,    McStat);

  if (Normal (*Status))
  if (Vtoken.Handle           > 0)
  if (Vchunk_GetChunkSize
                        (&Vtoken, McStat,    UnitCount,           UnitSize))
  if (Vchunk_GetStackPtr
                        (&Vtoken, McStat,    (indextype *)UnitCount))
  if (AttrDesc->AttrType     == aDblPtrAttr)
     *UnitCount /= 3;

TRACK(TrackBak,"ImageArrayGetSize\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageSubArrayGetSize
                         (tokentype    *Token,     numtype       AttrNum,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    numtype      *NumEntrys)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     Vtoken       = NullToken;
indextype     Vsize        = 0;

 *NumEntrys      = 0;

  AttrTkn.DbId   = RenvId;

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aDblPtrAttr)
  {
     if (Token->DbId)
     if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
     if (CoreDb)
     if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                            McStat,                           &MemLoc))

     if (MemLoc)
     if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
     if (OaRevokeCppObject (Token,                   McStat))
     if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
     if (OaGetObject       (Token,                   McStat,  &DbObject))
     if (ClassBug || DeBug || OaBug)
     if (sprintf (Msg, 
        " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
                  ObjNum, Object, DbObject))
        TraceMsg (0, Msg);

     Vtoken.DbId                 = Token->DbId;
     Vtoken.Handle               = 0;

     if (Normal (*Status))
     if (KernelGetAttr     (Token,   AttrNum,   McStat,  (addrtype )&Vtoken))
     if (MemLoc)
         OaInvokeCppObject (Token,   Object,    McStat);

     if (Normal (*Status))
     if (Vtoken.Handle           > 0)
     {
        if (Vchunk_GetStackPtr (&Vtoken,        McStat,             &Vsize))
        if (Index                < (Vsize /3))
        {
           if (Vchunk_GetAddress  
                           (&Vtoken, McStat,    (addrtype *)&ArrayPtrs))
           {
              ArrayHndls         = (handletype *)ArrayPtrs;
              Handles            = (handletype *)&ArrayHndls[Vsize / 3 * 2];
              if ((Vtoken.Handle = Handles[Index]) > 0)
                 Vchunk_GetStackPtr
                               (&Vtoken,        McStat,           NumEntrys);
           } 

        } else {
          *Status                = Image_ArrayIndexOutOfRange;
           TraceMsg (0, 
           " Image_SubArrayGetSize :: Index of SubArray Out OfRange.\n");

           sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
                    Token->DbId, Token->Handle, AttrNum);
           TraceMsg (0, Msg);
           sprintf (Msg, "         Index =%2u  GTR Size =%4u.\n",
                    Index, Vsize/2);
           TraceMsg (0, Msg);
        }
     }

  } else {
    *Status = Image_AttrNotDblPtrType;
     TraceMsg (0, " Image_SubArrayGetSize :: Attribute Not a DblPtr Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

TRACK(TrackBak,"ImageSubArrayGetSize\n");
return (STAT);
}

/* C; C++                                                                 */      
boolean ImageArrayAssertSize 
                          (tokentype    *Token,    numtype       AttrNum,
                           numtype       UnitCount, 
                           ft F,lt Z,zz *Status,   addrtype     *Varray)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
tokentype     Vtoken       = NullToken;
tokentype     Vchunk       = NullToken;
indextype     Vsize        = 0;
int           i            = -1;
farlongtype   SizeInBytes  = 0;
handletype   *Handles      = NullPtr;
addrtype     *TmpAddrs     = NullPtr;
handletype   *ArrayHndls   = NullPtr;
handletype   *NewHndls     = NullPtr;
handletype   *TmpHndls     = NullPtr;
int           DiffSize     = 0;
indextype     Index        = 0;
indextype     Indx0        = 0;

 *Varray          = 0;

  AttrTkn.DbId   = RenvId;
  Vtoken.DbId    = Token->DbId;
  Vtoken.Handle  = 0;

  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
              " Image::ArrayAssertSize [%2u:%4u] EnvAttr= %2u\n",
               Token->DbId, Token->Handle, AttrNum))
     TraceMsg (0, Msg);

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_ArrayAssertSize :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (Token->DbId)
  if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                         McStat,                           &MemLoc))

  if (MemLoc)
  if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
  if (OaGetObject       (Token,                   McStat,  &DbObject))
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
             " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
               ObjNum, Object, DbObject))
     TraceMsg (0, Msg);

  if (Normal (*Status))
  if (KernelGetAttr     (Token,   AttrNum,   McStat,  (addrtype )&Vtoken))
  if (MemLoc)
      OaInvokeCppObject (Token,   Object,    McStat);

  if (Vtoken.Handle           > 0)
  {
     if (AttrDesc->AttrType  == aDblPtrAttr)
     {
        if (Vchunk_GetStackPtr (&Vtoken, McStat,      &Vsize))
        if (Vchunk_PutStackPtr (&Vtoken, UnitCount*3,  McStat,  Varray))
        {
           Vsize      /= 3;
           ArrayHndls  = (handletype *)Varray;
           Handles     = &ArrayHndls[Vsize * 2];
           DiffSize    = UnitCount - Vsize;
        }

        if (Normal(*Status))
        if (UnitCount < Vsize) /* New size is smaller. Retain ChunkNums   */
        {
           for (i = UnitCount ; ++i < Vsize  && Normal(*Status); )
           {
             if ((Vchunk.Handle = Handles[i]) > 0)
               Vchunk_Delete (&Vchunk,   McStat);
           }

           /* Over Write address'                                         */
           NewHndls       = &ArrayHndls[UnitCount * 2];
           for (i = -1; ++i < UnitCount; )
           {
             ArrayHndls[i]  =  Handles[i];
           }
           Vchunk.DbId    = Token->DbId;

        } else { /* NewSize is Larger. Move Them Up                       */
           SizeInBytes   = UnitCount * 3 * sizeof (handletype);

           if (Core0_MoreCore (SizeInBytes,  McStat, (addrtype *)&TmpHndls))
           {
              TmpAddrs         = (addrtype *)TmpHndls;
              for (i = -1; ++i < Vsize; )
                TmpAddrs[i]    =  Varray[i];

              for (i = Vsize - 1; ++i < UnitCount; )
                TmpAddrs[i]    =  NullPtr;

              for (i = - 1; ++i < Vsize; )
              {
                Index           = Vsize     * 2 + i;
                Indx0           = UnitCount * 2 + i;
                TmpHndls[Indx0] = Handles[Index];
              }
              MoveBytes     (TmpAddrs, Varray, UnitCount * 3);

              Core0_FreeCore    ((addrtype *)&TmpHndls,  SizeInBytes, McStat);
           }
        }

     } else 
        Vchunk_PutStackPtr (&Vtoken, UnitCount,  McStat,  Varray);

     if (Normal(*Status)
     &&  ClassBug)
        Vchunk_Dump        (&Vtoken, McStat);

  } else {

    *Status = Image_ArrayWasNotCreated;

     TraceMsg (0, " Image_ArrayAssertSize :: Array was not First Created.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"ImageArrayAssertSize\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageSubArrayAssertSize
                          (tokentype    *Token,    numtype       AttrNum,
                           indextype     Index,    numtype       UnitCount,
                           ft F,lt Z,zz *Status,   addrtype     *Varray)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
tokentype     BldDescTkn   = NullToken;
typedesctype *TypeDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     Vtoken       = NullToken;
indextype     Vsize        = 0;
sizetype      UnitSize     = 0;

 *Varray         = 0;

  AttrTkn.DbId   = RenvId;
  Vtoken.DbId    = Token->DbId;
  Vtoken.Handle  = 0;

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aDblPtrAttr)
  {
     BldDescTkn.DbId          =  RenvId;
     BldDescTkn.Handle        =  AttrDesc->TypeObj;
     if (OaGetObject  (&BldDescTkn,  McStat,  (addrtype *)&TypeDesc))
        UnitSize      = TypeDesc->Size;

     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotDblPtrType;

     TraceMsg (0, " Image_SubArrayAssertSize :: Attribute Not DblPtr Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (Token->DbId)
  if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                         McStat,                           &MemLoc))

  if (MemLoc)
  if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
  if (OaGetObject       (Token,                   McStat,  &DbObject))
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
             " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
               ObjNum, Object, DbObject))
     TraceMsg (0, Msg);

  if (Normal (*Status))
  if (KernelGetAttr     (Token,   AttrNum,   McStat,  (addrtype )&Vtoken))
  if (MemLoc)
      OaInvokeCppObject (Token,   Object,    McStat);

  if (Normal (*Status))
  if (Vtoken.Handle           > 0)
  {
     if (Vchunk_GetStackPtr (&Vtoken,        McStat,             &Vsize))
     if (Index                < (Vsize /3))
     {
        Vsize                /= 3;
        if (Vchunk_GetAddress  (&Vtoken,     McStat, (addrtype *)&ArrayPtrs))
        {
           ArrayHndls         = (handletype *)ArrayPtrs;
           Handles            = (handletype *)&ArrayHndls[Vsize * 2];
           if ((Vtoken.Handle = Handles[Index]) == 0)
           {
              if (VchunkCreate (Token,       UnitSize,            UnitCount, 
                                1,           McStat,             &Vtoken)) 
              if (VchunkPutStackPtr 
                               (&Vtoken,     UnitCount,  McStat,  Varray))
                 Handles[Index] = Vtoken.Handle;

           } else
               Vchunk_PutStackPtr 
                               (&Vtoken,     UnitCount,  McStat,  Varray);
           if (Normal(*Status))
              ArrayPtrs[Index] = Varray;
        } 

     } else {
       *Status                = Image_ArrayIndexOutOfRange;
        TraceMsg (0, 
             " Image_SubArrayAssertSize :: Index of SubArray Out OfRange.\n");

        sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
                 Token->DbId, Token->Handle, AttrNum);
        TraceMsg (0, Msg);
        sprintf (Msg, "         Index[%2u]  >= NumSubArrays(%4u).\n",
                 Index, Vsize/3);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_ArrayWasNotCreated;

     TraceMsg (0, " Image_SubArrayAssertSize :: Array was not First Created.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"ImageSubArrayAssertSize\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageArrayDelete 
                          (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     Vtoken       = NullToken;
indextype     Vsize        = 0;
tokentype     VchunkTkn    = NullToken;
int           i            = 0;

  AttrTkn.DbId   = RenvId;
  Vtoken.DbId    = Token->DbId;
  Vtoken.Handle  = 0;

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aArrayAttr
  ||  AttrDesc->AttrType        == aVarrayAttr
  ||  AttrDesc->AttrType        == aDblPtrAttr
  ||  AttrDesc->AttrType        == aChunkAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotArrayType;

     TraceMsg (0, " Image_ArrayDelete :: Attribute Not an Array Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (Token->DbId)
  if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                         McStat,                           &MemLoc))

  if (MemLoc)
  if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
  if (OaGetObject       (Token,                   McStat,  &DbObject))
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
             " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
               ObjNum, Object, DbObject))
     TraceMsg (0, Msg);

  if (Normal (*Status))
  if (KernelGetAttr     (Token,   AttrNum,   McStat,      (addrtype )&Vtoken))
  if (MemLoc    == 0)
      KernelPutAttr     (Token,   AttrNum,   (addrtype )&NullToken,   McStat);
  else {
     if (OaInvokeCppObject       (Token,     Object,     McStat))
     if (AttrDesc->AttrType       == aArrayAttr
     ||  AttrDesc->AttrType       == aDblPtrAttr)
         KernelPutAttr  (Token,   AttrNum,   (addrtype )&NullAddr,    McStat);
     else if (AttrDesc->AttrType  == aVarrayAttr)
         KernelPutAttr  (Token,   AttrNum,   (addrtype )&NullVarray,  McStat);
     else
         KernelPutAttr  (Token,   AttrNum,   (addrtype )&NullVchunk,  McStat);
  }

  if (Normal (*Status))
  if (Vtoken.Handle                > 0)
  {
     if (MemLoc                   == 0)
     if (AttrDesc->AttrType       == aDblPtrAttr)
     {
        if (Vchunk_GetStackPtr (&Vtoken,    McStat,  (indextype *)&Vsize))
        if (Vchunk_GetAddress  (&Vtoken,    McStat,  (addrtype  *)&ArrayPtrs))
        {
           Vsize                      /= 3;
           ArrayHndls                  = (handletype *)ArrayPtrs;
           Handles                     = (handletype *)&ArrayHndls[Vsize * 2];
           VchunkTkn.DbId              = Vtoken.DbId;

           for (i = -1; ++i < Vsize && Normal(*Status); )
           {
              if ((VchunkTkn.Handle    = Handles[i]) > 0)
                 Vchunk_Delete (&VchunkTkn, McStat);
           }
        }

     } else
        Vchunk_Delete          (&Vtoken,    McStat);

  } else {
    *Status = Image_ArrayWasNotCreated;

     TraceMsg (0, " Image_ArrayDelete :: Array was not First Created.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"ImageArrayDelete\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageSubArrayDelete
                          (tokentype    *Token,    numtype       AttrNum,
                           indextype     Index,    ft F,lt Z,zz *Status)
{
dbheader     *CoreDb       = NullPtr;
numtype       MemLoc       = 0;
tokentype     AttrTkn      = NullToken;
attrdesctype *AttrDesc     = NullPtr;
addrtype      Object       = NullPtr;
numtype       ObjNum       = 0;
addrtype      DbObject     = NullPtr;
int           i            = 0;
handletype   *Handles      = NullPtr;
addrtype     *ArrayPtrs    = NullPtr;
handletype   *ArrayHndls   = NullPtr;
tokentype     Vtoken       = NullToken;
indextype     Vsize        = 0;

  AttrTkn.DbId   = RenvId;
  Vtoken.DbId    = Token->DbId;
  Vtoken.Handle  = 0;

  if (EnvFetchAttrHandle (AttrNum,     McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))

  if (AttrDesc->AttrType        == aDblPtrAttr)
  {
     if (ClassBug || DeBug || OaBug)
     {
        sprintf (Msg, "        For AttrType = %s; AttrMame= %s\n", 
                  AttrTypeNam[AttrDesc->AttrType], AttrDesc->Nam);
        TraceMsg (0, Msg);
     }
  } else {

    *Status = Image_AttrNotDblPtrType;

     TraceMsg (0, " Image_SubArrayDelete :: Attribute Not DblPtr Type.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);

  }

  if (Normal(*Status))
  if (Token->DbId)
  if (TmFetchCoreDb     (Token,   McStat,                  &CoreDb))
  if (CoreDb)
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,
                         McStat,                           &MemLoc))

  if (MemLoc)
  if (HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,  &Object))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,  &ObjNum))
  if (OaGetObject       (Token,                   McStat,  &DbObject))
  if (ClassBug || DeBug || OaBug)
  if (sprintf (Msg, 
             " Get Internal DbObject of Class =%3u Of Image @%x As DB @%x\n",
               ObjNum, Object, DbObject))
     TraceMsg (0, Msg);

  if (Normal (*Status))
  if (KernelGetAttr     (Token,   AttrNum,   McStat,  (addrtype )&Vtoken))
  if (MemLoc)
      OaInvokeCppObject (Token,   Object,    McStat);

  if (Normal (*Status))
  if (Vtoken.Handle           > 0)
  {
     if (Vchunk_GetStackPtr (&Vtoken,        McStat,             &Vsize))
     if (Index                < (Vsize /3))
     {
        if (Vchunk_GetAddress  (&Vtoken,     McStat, (addrtype *)&ArrayPtrs))
        {
           ArrayHndls         = (handletype *)ArrayPtrs;
           Handles            = (handletype *)&ArrayHndls[Vsize/3*2];
           if ((Vtoken.Handle = Handles[i]) > 0)
           if (Vchunk_Delete   (&Vtoken,     McStat))
           {
              ArrayPtrs[i]    = NullPtr;
              Handles[i]      = 0;
           }
        } 

     } else {
       *Status                = Image_ArrayIndexOutOfRange;
        TraceMsg (0, 
             " Image_SubArrayDelete :: Index of SubArray Out OfRange.\n");

        sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
                 Token->DbId, Token->Handle, AttrNum);
        TraceMsg (0, Msg);
        sprintf (Msg, "         Index =%2u  GTR Size =%4u.\n",
                 Index, Vsize);
        TraceMsg (0, Msg);
     }

  } else {
    *Status = Image_ArrayWasNotCreated;

     TraceMsg (0, " Image_SubArrayDelete :: Array was not First Created.\n");
     sprintf (Msg, "     for Token[%2u:%4u] AttrId= %4u.\n",
              Token->DbId, Token->Handle, AttrNum);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"ImageSubArrayDelete\n");
return (STAT);
}


/* .......................................................................
                   ....      VARRAY       ....
   .......................................................................*/
/* C; C++                                                                 */      
boolean ImageCreateVarray (tokentype    *DbmsToken, numtype   AttrNum,
                           indextype     UnitCount, sizetype  UnitSize,
                           ft F,lt Z,zz *Status,
                           tokentype    *VchunkTkn, addrtype *Varray)
{
    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, 
                 " Image::CreateVarray DbmsObject[%2u:%4u] EnvAttr= %2u\n",
                  DbmsToken->DbId, DbmsToken->Handle, AttrNum))
       TraceMsg (0, Msg);

    C_CreateArray      (DbmsToken,   AttrNum,      UnitCount,  UnitSize,
                        McStat,      VchunkTkn,    Varray);

    if (!Normal(*Status) || DeBug)
    { 
       sprintf (Msg, "  Image::CreateVarray for Token[%2u:%4u] @%x\n",
                DbmsToken->DbId, DbmsToken->Handle, *Varray);
       TraceMsg (0, Msg);
       if (DeBug || OaBug)
           OaDumpObject    (0, DbmsToken, McStat);

    } else {
       if (ClassBug || DeBug || OaBug)
       if (sprintf (Msg, "  Image::CreateVarray[%2u:%4u] @%x;\n", 
                    VchunkTkn->DbId, VchunkTkn->Handle, *Varray))
          TraceMsg (0, Msg);
    }

TRACK(TrackBak,"ImageCreateVarray\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageGetVarray (tokentype     *VchunkTkn,
                        ft F,lt Z,zz  *Status,    indextype     *Vsize,
                        indextype     *VstackPtr, addrtype      *Varray)
{
chunkstruc  VchunkStruc;

    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, 
                 "\n Image_Get Varray from DB with Token[%2u:%4u]\n",
                  VchunkTkn->DbId, VchunkTkn->Handle))
       TraceMsg (0, Msg);

    if (VchunkGetAddress    (VchunkTkn, McStat,   Varray))
    if (VchunkGetVstruc     (VchunkTkn, McStat,  &VchunkStruc))
    {
       *Vsize     = VchunkStruc.UnitCount;
       *VstackPtr = VchunkStruc.StackPtr;
    }

    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, "\n Varray[%3u:%4u](*%4u) located at %8x Stack=%4u\n",
                  VchunkTkn->DbId, VchunkTkn->Handle, *Vsize, *Varray,
                  *VstackPtr))
       TraceMsg (0, Msg);


TRACK(TrackBak,"ImageGetVarray\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImagePutVarrayStackPtr
                       (tokentype     *VchunkTkn,  indextype   StackPtr,
                        ft F,lt Z,zz  *Status,
                        indextype     *Vsize,      addrtype   *Varray)
{
chunkstruc  VchunkStruc;

    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, 
                 "\n ImagePutVarray StackPtr = %4u for VarrayTkn[%2u:%4u]\n",
                  StackPtr, VchunkTkn->DbId, VchunkTkn->Handle))
       TraceMsg (0, Msg);

    if (VchunkPutStackPtr   (VchunkTkn, StackPtr, McStat, Varray))
    if (VchunkGetVstruc     (VchunkTkn, McStat,  &VchunkStruc))
       *Vsize = VchunkStruc.UnitCount;

    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, "\n Varray[%3u:%4u](*%4u) located at %8x Stack= %3u\n",
                  VchunkTkn->DbId, VchunkTkn->Handle, *Vsize,
                  *Varray,         StackPtr))
       TraceMsg (0, Msg);

TRACK(TrackBak,"ImagePutVarrayStackPtr\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageGetVarrayStackPtr
                       (tokentype     *VchunkTkn,
                        ft F,lt Z,zz  *Status,     indextype  *StackPtr)
{
    if (ClassBug || DeBug || OaBug)
    if (sprintf (Msg, 
                 "\n Image_Put Varray StackPtr = %4u for VarrayTkn[%2u:%4u]\n",
                  StackPtr, VchunkTkn->DbId, VchunkTkn->Handle))
       TraceMsg (0, Msg);

    VchunkGetStackPtr (VchunkTkn, McStat, StackPtr);


TRACK(TrackBak,"ImageGetVarrayStackPtr\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF IAM.C                                   !
  +-----------------------------------------------------------------------+*/
