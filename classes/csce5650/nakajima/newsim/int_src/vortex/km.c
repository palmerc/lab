/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Kernel - KM.c */
#define  KRNL
#include "defines.h"
#include <stdarg.h>
#include "typedefs.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "env0.h"
#include "dbm.h"
#include "odbm.h"
#include "oa0.h"
#include "voa.h"
#include "env1.h"
#include "oa1.h"
#include "sm.h"
#include "km.h"
#ifdef __NOSTR__
#include <string.h>
#endif

/*+-----------------------------------------------------------------------+*
~KM                            EXTERNS                                     !
 *+-----------------------------------------------------------------------+*/
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        ObjNum_AttrNum             (numtype )5
#define        lOwnerObject_AttrNum       (numtype )87
#define        SchemaPrimalDesc_AttrNum   (numtype )157

#define        TraitObjs_Hndl         (handletype )668
#define        RenvSchemas_Hndl       (handletype )684
#endif

#define MemGetWord           Mem_GetWord
#define MemGetObjNum         Mem_GetWord
#define MemGetSize           Mem_GetWord

extern bytessize   *ObjHdrSize;

/*+-----------------------------------------------------------------------+
~ENV                           KERNEL                                     !
  +-----------------------------------------------------------------------+*/
boolean KernelCreateDb    (char         *SchemaNam,
                           char         *DbName,    char       *NewFile,
                           dbaccesstype  DbAccess,
                           ft F,lt Z,zz *Status,    tokentype  *DbPrimal)
{
tokentype   SchemaTkn       = NullToken;
tokentype   PrimalDescTkn   = NullToken;
numtype     PrimalObjNum    = 0;
statustype  DumpStatus      = Env_Normal;
int         TmpDeBug        = DeBug;

   SchemaTkn.DbId     = RenvTkn.DbId;
   PrimalDescTkn.DbId = RenvTkn.DbId;
   if (KernelFindIn  (RenvSchemas_Hndl, &RenvTkn,     (addrtype )SchemaNam,
                      McStat,           &SchemaTkn))
   {
     *Status = Env_Normal;
      DeBug  = TmpDeBug;
      if (DeBug || OaBug)
      if (TraceMsg      (0, "  The Schema Primal Descriptor ::\n"))
          OaDumpObject  (0,                &SchemaTkn,   Dump_Status);

      if (KernelGetAttr (&SchemaTkn,        SchemaPrimalDesc_AttrNum,
                         McStat,           &PrimalDescTkn))
     
      if (DeBug || OaBug)
          OaDumpObject  (0,                &PrimalDescTkn,   Dump_Status);

      if (Normal(*Status))
      if (KernelGetAttr (&PrimalDescTkn,    ObjNum_AttrNum,
                         McStat,           &PrimalObjNum))

          OaCreateDb    (&SchemaTkn,        PrimalObjNum,  
                         DbName,            NewFile,       DbAccess,
                         McStat,            DbPrimal);

   } else if (*Status == Set_NotFound) {
      sprintf  (Msg, "  Schema[%-14s] Not in RenvSchemas\n", SchemaNam);
      TraceMsg (0, Msg);

   }
   DeBug = TmpDeBug;

TRACK(TrackBak,"KernelCreateDb\n");
return (STAT);
}
 

 

boolean  KernelLoadDbHdr    (tokentype    *DbToken,  char       *DbFileName,  
                             ft F,lt Z,zz *Status)
{
dbheader  *CoreDb  = NullPtr;

     DbmLoadDbHdr   (DbToken,  DbFileName,  McStat,  &CoreDb);

TRACK(TrackBak,"KernelLoadDb\n");
return (STAT);
}
 

 
boolean KernelFreezeObjClass
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status)
{
   OaFreezeObjClass (Anchor,  EnvObj,  McStat);

TRACK(TrackBak,"KernelDeleteDb\n");
return (STAT);
}


 

boolean Kernel_GetClassObjectCount
                          (tokentype    *Token,   numtype       EnvObj,
                           ft F,lt Z,zz *Status,  numtype      *Count)
{
dbheader    *CoreDb     = NullPtr;
objheader   *ObjHdr     = NullPtr;
localnumtype LocalObj   = 0;
vbntype      Vbn        = 0;

 *Count                = 0;
  if (TmFetchCoreDb    (Token,                  McStat,  &CoreDb))
  if (Mem_GetWord      (CoreDb->LocalObjNumMap, EnvObj,   McStat, &LocalObj))
  if (LocalObj)
  {
    if (Mem_GetAddr    (CoreDb->ObjHdrDir,      LocalObj, 
                        McStat,                 (addrtype *)&ObjHdr))
    if (ObjHdr == NullPtr)
    {
       if (CoreDb->ObjVbnDirVbn)
       {
          if (Mem_GetWord 
                       (CoreDb->ObjVbnDir,      LocalObj,
                        McStat,                &Vbn))          
          if (Vbn) 
          {
             if (DbmLoadObjHdr
                       (Token,  LocalObj,       McStat,     &ObjHdr))
               *Count  = ObjHdr->ObjectCount - ObjHdr->ExObjectCount;
          }
       }
    } else
      *Count           = ObjHdr->ObjectCount - ObjHdr->ExObjectCount;
  }

TRACK(TrackBak,"Kernel_GetClassObjectCount\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~KM                       KERNEL:: OBJ                                    !
  +-----------------------------------------------------------------------+*/
boolean  KernelNew          (tokentype    *Anchor,    numtype      EnvObj,
                             ft F,lt Z,zz *Status,   
                             tokentype    *Token,     addrtype    *ObjAddr)
{
     if (OaCreateObject  (Anchor,  EnvObj,   McStat,  Token))
         OaGetObject     (Token,   McStat,   ObjAddr);

TRACK(TrackBak,"KernelNew\n");
return (STAT);
}
 
boolean  KernelCreateObject (tokentype    *Anchor,    numtype      EnvObj,
                             ft F,lt Z,zz *Status,    tokentype   *Token)
{
     OaCreateObject  (Anchor,  EnvObj,   McStat,  Token);

TRACK(TrackBak,"KernelCreateObject\n");
return (STAT);
}
 
boolean  KernelNewObject   (tokentype    *Anchor,  numtype    EnvObj,
                            numtype       NumInits, 
                            numtype      *AttrNums,addrtype  *Values,
                            ft F,lt Z,zz *Status,  tokentype *Token)
{

     if (OaCreateObject  (Anchor,  EnvObj,   McStat,   Token))
         OaInitObject    (Token,   NumInits, AttrNums, Values,  McStat);


TRACK(TrackBak,"KernelNewObject\n");
return (STAT);
}
 

boolean  KernelCreateArray (tokentype   *DbmsToken,  numtype   AttrNum,
                            indextype    UnitCount,  sizetype  UnitSize,
                            ft F,lt Z,zz *Status,    
                            tokentype    *VchunkTkn, addrtype *ArrayAddr)
{
     OaCreateArray  (DbmsToken,       AttrNum,      UnitCount,   UnitSize,
                     McStat,          VchunkTkn,    ArrayAddr); 

TRACK(TrackBak,"KernelCreateArray\n");
return (STAT);
}
 
boolean  KernelCreateVarray (tokentype   *DbmsToken,  numtype   AttrNum,
                            indextype    UnitCount,  sizetype  UnitSize,
                            ft F,lt Z,zz *Status,    
                            tokentype    *VchunkTkn, addrtype *ArrayAddr)
{
     OaCreateVarray (DbmsToken,       AttrNum,      UnitCount,   UnitSize,
                     McStat,          VchunkTkn,    ArrayAddr); 

TRACK(TrackBak,"KernelCreateVarray\n");
return (STAT);
}
 

boolean KernelIamA        (tokentype    *Token,     numtype   EnvObj,
                           ft F,lt Z,zz *Status)  
{
numtype       ObjNum        = 0;
tokentype     ObjCestrsTkn;
bytetype      BitVal        = 1;

   if (Token->DbId   == 0
   &&  Token->Handle  > 0)
      ObjNum = Token->Handle;
   else
      EnvFetchObjNum     (Token,         McStat,           &ObjNum);

   if (Normal(*Status))
   if (ObjNum != EnvObj)
   if (EnvFetchObjCestrs  (ObjNum,        McStat,           &ObjCestrsTkn)) 
   if (KernelGetBit       (&ObjCestrsTkn, EnvObj,  McStat,  &BitVal))
   if (!BitVal)                              
      *Status = Kernl_IamNot;

   if (ClassBug || DeBug) 
   if (sprintf (Msg, "   Kernel_IamA (Obj =%2u)... for [%2u:%4u] = %s\n",  
                ObjNum, Token->DbId, Token->Handle, BoolStr[BitVal]))
       TraceMsg (0, Msg);

TRACK(TrackBak,"KernelIamA\n");
return (STAT);
}
 

boolean KernelWhatAmI     (tokentype    *Token,
                           ft F,lt Z,zz *Status,    numtype   *EnvObj)
{
   if (Token->DbId   == 0
   &&  Token->Handle  > 0)
     *EnvObj = Token->Handle;
   else
      EnvFetchObjNum      (Token,         McStat,           EnvObj);

TRACK(TrackBak,"KernelWhatAmI\n");
return (STAT);
}
 

 

boolean KernelGet         (tokentype    *Token,
                           ft F,lt Z,zz *Status,    addrtype *ObjAddr)
{ 
   OaGetObject (Token, McStat, ObjAddr);

TRACK(TrackBak,"KernelGet\n");
return (STAT);
}
 

boolean KernelGetObject   (tokentype    *Token,     numtype   EnvObjNum,
                           ft F,lt Z,zz *Status,    addrtype  Object)
{ 
addrtype   ObjectAddr = NullPtr;
   if (OaGetObject (Token, McStat, &ObjectAddr))
      MoveBytes (ObjectAddr, Object, ObjHdrSize[EnvObjNum]);

TRACK(TrackBak,"KernelGetObject\n");
return (STAT);
}
 

boolean KernelPutObject   (tokentype  *Token,     numtype       EnvObjNum,
                           addrtype    Object,    ft F,lt Z,zz *Status)
{ 
addrtype   ObjectAddr = NullPtr;

   if (OaGetObject (Token, McStat, &ObjectAddr))
      MoveBytes (Object, ObjectAddr, ObjHdrSize[EnvObjNum]);

TRACK(TrackBak,"KernelPutObject\n");
return (STAT);
}
 

 
/*+-----------------------------------------------------------------------+
~KM                       KERNEL:: ATTR                                   !
  +-----------------------------------------------------------------------+*/
boolean    KernelGetAttrInfo
                         (numtype       AttrId,
                          ft F,lt Z,zz *Status,     attrobjtype *AttrInfo)
{
dbheader       *CoreDb          = NullPtr;
tokentype       AttrToken       = NullToken;
attrdesctype   *AttrDesc        = NullPtr;
indextype       LocalAttrNum    = 0;
sizetype        AttrSize        = 0;

  AttrToken.DbId       = RenvId;

  CoreDb               = CoreDbs[RenvId];

  if (EnvFetchAttrHandle
                      (AttrId,
                       McStat,                     &(AttrToken.Handle)))
  if (KernelGet       (&AttrToken,   McStat,       (addrtype *)&AttrDesc))

  if (MemGetObjNum    (CoreDb->LocalAttrNumMap,    (indextype )AttrId,
                       McStat,                     &LocalAttrNum))
  if (MemGetSize      (CoreDb->LocalAttrSizeTbl,   (indextype )LocalAttrNum,
                       McStat,                     (word *)&AttrSize))
  
  {
    strcpy (AttrInfo->AttrName,      AttrDesc->Nam);
    AttrInfo->AttrType            =  AttrDesc->AttrType;
    AttrInfo->CppType             =  AttrDesc->CppType; 
    AttrInfo->TypeType            =  AttrDesc->TypeType;
    AttrInfo->Size                =  AttrSize;

  }

TRACK(TrackBak,"KernelGetAttrInfo\n");
return (STAT);
}




boolean Kernel_GetFieldStruc
                         (char         *FieldName,   
                          ft F,lt Z,zz *Status,    fieldstruc   *FieldStruc)
{ 
tokentype    AttrTkn    = NullToken;
numtype      AttrId;
nametype     AttrNam;
static char  WildOps[3] = {".["};
size_t       StrSize    = 0;
char        *StrSeg     = NullPtr;
char         IntNam[10];
size_t       MaxSize    = strlen (FieldName);
int          i          = 0;

  AttrNam[0] = '\0';
  IntNam[0]  = '\0';

  if (ClassBug)
  if (sprintf (Msg, " Kernel_GetFieldStruc for MemberName= <%21s>\n",
               FieldName))
     TraceMsg   (0, Msg);

  FieldStruc->SubLevel     = 0;
  FieldStruc->IsIndexed    = False;
  strcpy                    (FieldStruc->FieldName, FieldName);
  StrSize                  = MaxSize;

#ifndef __NOSTR__
  while (Normal(*Status)
  &&     FieldName[0]     != '\0')
  {
     /* Get Next Attribute Name                                         */
     if ((StrSeg           = strpbrk (FieldName,  WildOps)) != NULL)
     {
        StrSize            = StrSeg  - FieldName;
        strncpy  (AttrNam, FieldName, StrSize);
        AttrNam[StrSize]   = '\0';

     } else {
        strcpy   (AttrNam, FieldName);
        StrSize            = strlen (FieldName);
     }
     if (ClassBug || SetBug)
     if (sprintf (Msg, " Truncated Name:= <%s> size=%3u\n",
                  AttrNam, strlen (AttrNam)))
      TraceMsg   (0, Msg);

     if (KernelFindIn     (RenvDescs_Hndl,               &RenvTkn,   
                           (addrtype )AttrNam,
                           McStat,          (tokentype *)&AttrTkn))
     if (KernelGetAttr    (&AttrTkn,                      AttrNum_AttrNum,
                           McStat,          (addrtype   )&AttrId))
     {
        if (ClassBug)
        if (sprintf (Msg, "               AttrId             = %3u\n",
                     AttrId))
           TraceMsg (0, Msg);

        FieldStruc->FieldTkns[FieldStruc->SubLevel].AttrId  = AttrId;
        FieldStruc->FieldTkns[FieldStruc->SubLevel].Index   = 0;
        FieldStruc->SubLevel                               += 1;
     }

     /* Check For Field Index                                           */
     if (Normal(*Status))
     {
        i=0;
        while (i++ < StrSize)
          ++FieldName; 

        if (FieldName[0] == '[')
        {
        ++FieldName; 
          if ((StrSeg    = strpbrk (FieldName, "]")) != NULL)
          {
             StrSize           = StrSeg - FieldName;
             strncpy  (IntNam, FieldName, StrSize);
             IntNam[StrSize]   = '\0';

             FieldStruc->FieldTkns[FieldStruc->SubLevel - 1].Index 
                               = atoi (IntNam);
             FieldName         = StrSeg;
           ++FieldName;
             if (FieldName[0] == '\0')
             FieldStruc->IsIndexed    = True;

          } else
            *Status = Lex_BadSyntax;
        }
        if (FieldName[0] == '.')
          ++FieldName;

        if (ClassBug)
        if (sprintf (Msg, "               Index              = %3u\n",
                     FieldStruc->FieldTkns[FieldStruc->SubLevel - 1].Index)) 
           TraceMsg (0, Msg);
     }
  }
#endif

TRACK(TrackBak,"Kernel_GetFieldStruc\n");
return (STAT);
}


boolean    KernelGetFieldInfo
                         (fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,     attrobjtype *AttrInfo)
{
numtype         AttrId          = 0;

  AttrId             = FieldStruc->FieldTkns[FieldStruc->SubLevel-1].AttrId;

  KernelGetAttrInfo  (AttrId,   McStat,  AttrInfo);

TRACK(TrackBak,"KernelGetFieldInfo\n");
return (STAT);
}



boolean    KernelPutAttr (tokentype   *Token,      numtype    EnvAttrNum,
                          addrtype     Value,      ft F,lt Z,zz *Status)
{ 
addrtype     Object       = NullPtr;

   OaPut (False, Token, Object, EnvAttrNum, Value, McStat);

TRACK(TrackBak,"KernelPutAttr\n");
return (STAT);
}
 

boolean    KernelGetAttr (tokentype    *Token,      numtype   EnvAttrNum,
                          ft F,lt Z,zz *Status,     addrtype  Value )
{ 
addrtype     Object       = PtrToVoid;

   OaGet (False, Token, Object, EnvAttrNum, McStat, Value);

TRACK(TrackBak,"KernelGetAttr\n");
return (STAT);
}


boolean    KernelPutField 
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status)
{

   OaPutField    (Token, FieldStruc, Value, McStat);

TRACK(TrackBak,"KernelPutField\n");
return (STAT);
}


boolean    KernelGetField 
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value)
{ 
   OaGetField    (Token,  FieldStruc,  McStat,  Value);

TRACK(TrackBak,"KernelGetField\n");
return (STAT);
}


 
int        KernelCompare (tokentype   *Token,      numtype       AttrNum,
                          addrtype     Value,      ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status)
{ 
addrtype     Object       = NullPtr;
int          i            = 0;

   OaCompare (False, Token, Object, AttrNum, Value, CmpCode, McStat);

   if (Normal(*Status))
      return (0);
   if (*Status  == Oa_GtrThen)
   {
     *Status = Env_Normal;
      return (1);
   } else if (*Status  == Oa_LessThen) {
     *Status = Env_Normal;
      return (-1);
   }

STAT;
TRACK(TrackBak,"KernelCompare\n");
return (i);
}
 
int        KernelFieldCompare 
                         (tokentype   *Token,      fieldstruc   *FieldStruc,
                          addrtype     Value,      ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status)
{ 
addrtype     Object       = NullPtr;
int          i            = 0;

   i = OaCompareField (False,  Token,     Object,  FieldStruc,
                       Value,  CmpCode,            McStat);

STAT;
TRACK(TrackBak,"KernelCompare\n");
return (i);
}
 
/*+-----------------------------------------------------------------------+
~KM                       KERNEL:: BITFIELD                               !
  +-----------------------------------------------------------------------+*/
boolean  KernelCreateBitField
                          (tokentype    *Anchor,      sizetype   AllocBits,
                           ft F,lt Z,zz *Status,    
                           tokentype    *BitFieldTkn)
{
bitfieldtype *BitField   = NullPtr;
sizetype      NumBits    = (AllocBits | 7)+1;
indextype     AllocBytes =  NumBits / 8;

     if (OaCreateVchunk  (Anchor,      sizeof (bitfieldtype), 
                          AllocBytes,  Extend_BitField,
                          McStat,      BitFieldTkn))
     if (OaGetVchunkAddr (BitFieldTkn, McStat,  (addrtype *)&BitField))
     if (OaPutVchunkStackPtr
                         (BitFieldTkn, AllocBytes,   McStat))
         memset          (BitField,    '\0',         AllocBytes);

TRACK(TrackBak,"KernelCreateBitField\n");
return (STAT);
}


boolean  KernelDeleteBitField
                          (tokentype    *BitFieldTkn, ft F,lt Z,zz *Status)
{

  OaDeleteVchunk  (BitFieldTkn, McStat);

TRACK(TrackBak,"KernelDeleteBitField\n");
return (STAT);
}
 

boolean  KernelPutBit     (tokentype    *BitFieldTkn,    indextype     BitNum,
                           bytetype      BitValue,       ft F,lt Z,zz *Status)
{
bitfieldtype    *BitField  = NullPtr;
indextype        ByteNum   = BitNum >> 3;
indextype        ByteCount = 0;
numtype          Value     = 0;

     if (OaGetVchunkStackPtr  (BitFieldTkn, McStat,  &ByteCount))
     if (ByteNum >= ByteCount)
         OaPutVchunkStackPtr  (BitFieldTkn, ByteNum,  McStat);

     if (Normal(*Status))
     if (OaGetVchunkAddr      (BitFieldTkn, McStat,  (addrtype *)&BitField))
         BitField[ ByteNum ] |= BitValue << (BitNum & 7);

     if (Normal(*Status))
        Value =  (BitField[BitNum >> 3] & 1 << (BitNum & 7)) ? 1: 0;

     if (Value != BitValue)
     if (sprintf (Msg, "   ERROR... Kernel_PutBit[%4u] @byte %3u =%u\n",
                  BitNum, ByteNum, BitValue))
        SendMsg (0, Msg);

TRACK(TrackBak,"KernelPutBit\n");
return (STAT);
}


boolean  KernelGetBit     (tokentype    *BitFieldTkn,    indextype  BitNum,
                           ft F,lt Z,zz *Status,         bytetype  *BitValue)
{
indextype        ByteNum   = BitNum >> 3;
bitfieldtype    *BitField  = NullPtr;
indextype        ByteCount = 0;

     if (OaGetVchunkAddr (BitFieldTkn, McStat,  (addrtype *)&BitField))
     if (OaGetVchunkStackPtr 
                         (BitFieldTkn, McStat,  &ByteCount))
     if (ByteNum >= ByteCount)
        *Status = Set_IndexGtrAllocRoots;

     if (Normal(*Status))
        *BitValue =  (BitField[BitNum >> 3] & 1 << (BitNum & 7)) ? 1: 0;

     if (DeBug || OaBug) 
     if (sprintf (Msg, "   Kernel_GetBit[%4u] Value=%u from Byte %2u\n", 
                  BitNum, BitValue, ByteNum))
        TraceMsg (0, Msg);

TRACK(TrackBak,"KernelGetBit\n");
return (STAT);
}


boolean  KernelFirstBit   (tokentype    *BitFieldTkn,
                           ft F,lt Z,zz *Status,         indextype *FirstBit)
{
bitfieldtype    *BitField;
indextype        BitNum    = 0;
indextype        ByteCount = 0;

     if (!BitFieldTkn->Handle)
         *Status = Set_EmptySet;

     if (Normal(*Status))
     if (OaGetVchunkAddr (BitFieldTkn, McStat,  (addrtype *)&BitField))
     if (OaGetVchunkStackPtr 
                         (BitFieldTkn, McStat,  &ByteCount))
     if (!ByteCount)
         *Status = Set_EmptySet;

     while (Normal(*Status)
       &&   !( (BitField[BitNum >> 3] & 1 << (BitNum & 7)) )  ) {
       if (++BitNum / 8 >= ByteCount)
          *Status = Set_EndOfSet;
     }
            
     *FirstBit    = BitNum;

TRACK(TrackBak,"KernelFirstBit\n");
return (STAT);
}
 

boolean  KernelNextBit    (tokentype    *BitFieldTkn,    indextype  CurrBitNum,
                           ft F,lt Z,zz *Status,         indextype *NextBitNum)
{
bitfieldtype    *BitField;
indextype        BitNum    = CurrBitNum;
indextype        ByteCount = 0;

     if (OaGetVchunkAddr (BitFieldTkn, McStat,  (addrtype *)&BitField))
     if (OaGetVchunkStackPtr 
                         (BitFieldTkn, McStat,  &ByteCount))
     do {
       if (++BitNum / 8 >= ByteCount)
          *Status = Set_EndOfSet;
     } while (Normal(*Status)
       &&   !( (BitField[BitNum >> 3] & 1 << (BitNum & 7)) )  );
            
     *NextBitNum = BitNum;

TRACK(TrackBak,"KernelNextBit\n");
return (STAT);
}
 
/* Reset ...        BitField[BitNum >> 3] &= ~(1 << (BitNum & 7));      */

/*+-----------------------------------------------------------------------+
~KM                       KERNEL:: SET                                    !
  +-----------------------------------------------------------------------+*/
 

boolean KernelOwnerOf    (handletype    SetHndl,   tokentype *Member,
                          ft F,lt Z,zz *Status,    tokentype *Owner)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
tokentype        SetHeadTkn;
tokentype        SetNodeTkn;
addrtype         Dummy       = NullPtr;

  Owner->DbId       = Member->DbId;
  SetHeadTkn.DbId   = Member->DbId;
  SetNodeTkn.DbId   = Member->DbId;
  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetHndl;

  if (OaGetObject (&SetDescTkn,   McStat,   (addrtype *)&SetDesc)) 
  if (OaGet       (False,         Member,   Dummy, 
                   SetDesc->MemberAttrNum,
                   McStat,        (addrtype )&SetNodeTkn))
  if (SetNodeTkn.Handle == NullHandle)
     *Status = Set_EmptySet;

  SetHeadTkn.DbId   = SetNodeTkn.DbId;
  Owner->DbId       = SetNodeTkn.DbId;

  if (Normal(*Status))
  if (SetDesc->SetType == aIndexedSet)
  {
     Owner->Handle = SetNodeTkn.Handle;

  } else if (SetDesc->SetType == aGlobalKeyedSet)  {

     if (OaGet       (False,        &SetNodeTkn,   Dummy, 
                      lOwnerObject_AttrNum,
                      McStat,        (addrtype )&(SetHeadTkn.Handle)))
         OaGet       (False,        &SetHeadTkn,   Dummy, 
                      lOwnerObject_AttrNum,
                      McStat,        (addrtype )&(Owner->Handle));

  } else {
     if (OaGet       (False,        &SetNodeTkn,   Dummy, 
                      lOwnerObject_AttrNum,
                      McStat,        (addrtype )&(SetHeadTkn.Handle)))
         OaGet       (False,        &SetHeadTkn,   Dummy, 
                      lOwnerObject_AttrNum,
                      McStat,        (addrtype )&(Owner->Handle));
  }

TRACK(TrackBak,"KernelOwnerOf\n");
return (STAT);
}
 

 
boolean KernelFirstOf    (handletype    SetHndl,    tokentype *Owner,
                          ft F,lt Z,zz *Status,     tokentype *FirstNode,
                           tokentype   *FirstMember)
{
  SetFirstOf (SetHndl, Owner, McStat, FirstNode, FirstMember);

TRACK(TrackBak,"KernelFirstOf\n");
return (STAT);
}
 

boolean KernelNextOf     (handletype    SetHndl,    tokentype *CurrentNode,
                          ft F,lt Z,zz *Status,     tokentype *NextNode,
                          tokentype    *NextMember)
{
  SetNextOf (SetHndl, CurrentNode, McStat, NextNode, NextMember);

TRACK(TrackBak,"KernelNextOf\n");
return (STAT);
}
 

 

boolean KernelAddInto     (handletype   SetDescHndl,  tokentype    *OwnerTkn,
                           tokentype   *MemberTkn,    ft F,lt Z,zz *Status)
{
   SetAddInto (SetDescHndl, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"KernelAddInto\n");
return (STAT);
}
 

 

boolean KernelFindIn     (handletype    SetDescHndl,  tokentype    *OwnerTkn,
                          addrtype      KeyValue,
                          ft F,lt Z,zz *Status,       tokentype    *MemberTkn)
{
  SetFindIn (SetDescHndl, OwnerTkn, KeyValue, McStat, MemberTkn);

TRACK(TrackBak,"KernelFindIn\n");
return (STAT);
}
 

 
/*+-----------------------------------------------------------------------+
~M                         END OF KM.C                                    !
  +-----------------------------------------------------------------------+*/
