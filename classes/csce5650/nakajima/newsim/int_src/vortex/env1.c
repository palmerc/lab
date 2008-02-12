/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV - Env1.c */
/*+-----------------------------------------------------------------------+
  |                             VORTEx                                    |
  |                           ENVIORNMENT                                 |
  +-----------------------------------------------------------------------+*/
#define ENV1_C
#include <time.h>
#include "defines.h"
#include "typedefs.h"
#include "omidefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "vdbm.h"
#include "oa0.h"
#include "voa.h"
#include "oa1.h"
#include "env1.h"

#define        DefaultValue_AttrNum       (numtype )0
extern boolean Trans_FetchAttrOffset
                        (numtype       ObjNum,      numtype       AplId,
                         numtype       FieldNum,    ft F,lt Z,zz *Status,
                         sizetype     *CppOffset);

extern boolean Trans_FetchFieldOffset
                        (tokentype    *ObjTkn,      numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                           numtype      *AttrId,      attrtype     *AttrType,
                           numtype      *RefObjNum,   typetype     *FieldType,
                           sizetype     *DbOffset,    sizetype     *CppOffset,
                           sizetype     *DbBase,      sizetype     *CppBase);

/*+-----------------------------------------------------------------------+
~ENV                          ENV1                                        !
  +-----------------------------------------------------------------------+*/
/*
#define  Extend_Codes      (indextype )101


#define        ObjDesc_Class          (numtype )1

#define        PortDesc_ObjNum        (objnumtype )12

#define        ArgDesc_Hndl           (handletype )142
#define        RenvDescs_Hndl         (handletype )688
#define        RenvCodeDescs_Hndl     (handletype )689

#define  Env01_BadDescTypeType   2005

#define  Env_AttrNotInAplClassDef  -21
#define  Env_AttrNotInDbmsClassDef -22

#define  Code_NotAnObjDesc      14001
*/
/*+-----------------------------------------------------------------------+*
~OA                          EXTERNS                                      !
 *+-----------------------------------------------------------------------+*/
extern boolean OaDumpObjHdr   (int           TraceSwi,  tokentype  *Anchor,  
                               num           ObjNum, 
                               ft F,lt Z,zz *Status);

extern boolean OaDumpObject   (int           TraceSwi,  tokentype  *Token,
                               ft F,lt Z,zz *Status);
 
extern boolean KernelGetAttr  (tokentype    *Token,    numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype   Value);

extern boolean KernelFindIn   (handletype    SetDesc,  tokentype *OwnerTkn,
                               addrtype      KeyValue,
                               ft F,lt Z,zz *Status,   tokentype *MemberTkn);


#define MemPutLocalObjNum   Mem_PutWord

#define MemPushSize         Mem_PushWord
#define MemGetSize          Mem_GetWord

#define MemPushObjNum       Mem_PushWord
#define MemPutObjNum        Mem_PutWord
#define MemGetObjNum        Mem_GetWord

#define MemPushObjHdr       Mem_PushAddr


#define MemPutHandle        Mem_PutWord
#define MemGetHandle        Mem_GetWord

#define MemPutFieldNum      Mem_PutWord
#define MemPushFieldNum     Mem_PushWord
#define MemPushFieldOffset  Mem_PushWord

#define MemPushAttrNum      Mem_PushWord
#define MemPutAttrNum       Mem_PutWord
#define MemGetAttrNum       Mem_GetWord

#define MemPushNum          Mem_PushWord


/*+-----------------------------------------------------------------------+*/
/*+                         OBJ DESC                                      +*/
/*+-----------------------------------------------------------------------+*/

boolean    EnvNewObjDesc    (tokentype    *Anchor,  desctype      ObjDescType,
                             ft F,lt Z,zz *Status,
                             objdesctype **ObjDesc, tokentype    *Token)
{  /* allocate space for An ObjectDesc  */
  *Status   = Err_NotImplemented;


TRACK(TrackBak,"EnvNewObjDesc\n");
return (STAT);
}
 

boolean    EnvFetchObjNum     (tokentype    *ObjTkn,
                               ft F,lt Z,zz *Status, numtype *ObjNum)
{ 
dbheader   *CoreDb  = NullPtr;
  if (TmFetchCoreDb  (ObjTkn,  McStat,  &CoreDb))
      HmGetObjNum    (CoreDb,                   ObjTkn->Handle,
                      McStat,                   ObjNum);

TRACK(TrackBak,"EnvFetchObjNum\n");
return (STAT);
}
 

boolean    EnvFetchObjHandle  (numtype       ObjNum,
                               ft F,lt Z,zz *Status, handletype *ObjHandle)
{ 
   MemGetHandle (CoreDb0->ObjHandleDir, ObjNum,  McStat, (word *)ObjHandle);

TRACK(TrackBak,"EnvFetchObjHandle\n");
return (STAT);
}
 

boolean    EnvPairObjHandle   (numtype       ObjNum,  handletype ObjHandle,
                               ft F,lt Z,zz *Status)
{ 
   MemPutHandle (CoreDb0->ObjHandleDir, ObjNum,  ObjHandle,  McStat);

TRACK(TrackBak,"EnvPairObjHandle\n");
return (STAT);
}
 
boolean    EnvFetchObjSize   (numtype       ObjNum,
                              ft F,lt Z,zz *Status,  bytessize *SizeInBytes)
{
numtype    LocalObjNum = 0; 
objheader *ObjHdr      = NullPtr;

  if (MemGetObjNum (CoreDb0->LocalObjNumMap, ObjNum, McStat, &LocalObjNum))
  if (LocalObjNum == 0)
      EnvInstallObjHdr  (&RenvTkn, ObjNum, McStat,   &LocalObjNum);

  if (Normal(*Status))
      OmGetObjHdr  (&RenvTkn, LocalObjNum, McStat, &ObjHdr);

  if (ObjHdr)
     *SizeInBytes = ObjHdr->ObjSize;
  else
     *Status      = Env_ObjNoHdr;

TRACK(TrackBak,"EnvFetchObjSize\n");
return (STAT);
}
 
boolean    EnvFetchObjName   (numtype       ObjNum,
                              ft F,lt Z,zz *Status,  char    *ObjName)
{
handletype     ObjHandle = 0;
objdesctype   *ObjDesc   = NullPtr;
tokentype      ObjDescTkn;
indextype      Index     = 0;

    if (MemGetHandle    (CoreDb0->ObjHandleDir,               ObjNum, 
                         McStat,                             &ObjHandle))
    {
      ObjDescTkn.DbId     = RenvId;
      ObjDescTkn.Handle   = ObjHandle;
      OmGetObject       (&ObjDescTkn,         
                         McStat,       &Index,   (addrtype *)&ObjDesc);
      strcpy (ObjName,   ObjDesc->Nam);
    }

TRACK(TrackBak,"EnvFetchObjName\n");
return (STAT);
}

boolean    EnvFetchClassSize (numtype       ObjNum,
                              ft F,lt Z,zz *Status,  size_t  *ClassSize)
{
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;

  if (MemGetHandle    (CoreDb0->ObjHandleDir,               ObjNum,
                       McStat,                             &ObjDescHndl))
  {
     ObjDescTkn.DbId   = RenvId;
     ObjDescTkn.Handle = ObjDescHndl;
     OaGetObject      (&ObjDescTkn,    McStat, (addrtype *)&ObjDesc);
  }

  if (Normal(*Status))
  if ((*ClassSize  =  ObjDesc->PsudoSize) == 0)
     *ClassSize    =  ObjDesc->ObjSize;

TRACK(TrackBak,"EnvFetchClassSize\n");
return (STAT);
}
 
boolean EnvFetchLastObjCount  (ft F,lt Z,zz *Status,   numtype *ObjCount)
{
    *ObjCount = CoreDb0->LastEnvObjCount;

TRACK(TrackBak,"EnvFetchObjNum\n");
return (STAT);
}

boolean    EnvFetchObjCestrs  (numtype       ObjNum,
                               ft F,lt Z,zz *Status, tokentype *ObjCestors)
{ 
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;
indextype     Index       = 0;
 
  ObjDescTkn.DbId   = RenvId;
  ObjCestors->DbId  = RenvId;

  if (MemGetHandle    (CoreDb0->ObjHandleDir, ObjNum,  McStat, &ObjDescHndl))
  {
      ObjDescTkn.Handle   = ObjDescHndl;
      OmGetObject     (&ObjDescTkn,         
                       McStat,               &Index,   (addrtype *)&ObjDesc);
  }

  if (Normal(*Status))
      ObjCestors->Handle = ObjDesc->CestorsOfObj;

TRACK(TrackBak,"EnvFetchObjCestrs\n");
return (STAT);
}
 

boolean    Env_FetchObjCendents
                              (numtype       ObjNum,
                               ft F,lt Z,zz *Status, tokentype *ObjCendents)
{ 
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;
indextype     Index       = 0;
 
  ObjDescTkn.DbId    = RenvId;
  ObjCendents->DbId  = RenvId;

  if (MemGetHandle    (CoreDb0->ObjHandleDir, ObjNum,  McStat, &ObjDescHndl))
  {
      ObjDescTkn.Handle   = ObjDescHndl;
      OmGetObject     (&ObjDescTkn,         
                       McStat,               &Index,   (addrtype *)&ObjDesc);
  }

  if (Normal(*Status))
      ObjCendents->Handle = ObjDesc->CendentsOfObj;

TRACK(TrackBak,"Env_FetchObjCendents\n");
return (STAT);
}
 

boolean    EnvFetchSchemaObjs  (numtype      ObjNum,
                               ft F,lt Z,zz *Status, tokentype *SchemaObjs)
{ 
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;

  SchemaObjs->DbId  = RenvId;
  if (MemGetHandle    (CoreDb0->ObjHandleDir, ObjNum,  McStat, &ObjDescHndl))
  {
      ObjDescTkn.DbId    = RenvId;
      ObjDescTkn.Handle  = ObjDescHndl;
      if (OaGetObject    (&ObjDescTkn,       McStat, (addrtype *)&ObjDesc))
          SchemaObjs->Handle = ObjDesc->CestorsOfObj;
  }

TRACK(TrackBak,"EnvFetchSchemaObjs\n");
return (STAT);
}
 

boolean    DeleteObjDesc (tokentype *Token, ft F,lt Z,zz *Status)
{
objdesctype  *ObjDesc = NullPtr;

  OaGetObject (Token, McStat, (addrtype *)&ObjDesc);

TRACK(TrackBak,"DeleteObjDesc\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*+                         OBJ HEADER                                    +*/
/*+-----------------------------------------------------------------------+*/

boolean    EnvBldObjHdr (tokentype    *Anchor,        numtype   EnvObjNum,
                         indextype     AllocQty,
                         ft F,lt Z,zz *Status,
                         numtype      *FieldNumMap, chunknum *FieldOffsetDir,
                         chunknum     *ClassOffsetDir,
                         indextype   **FieldCount,  chunknum *LocalAttrNumMap,
                         indextype   **FieldRefCount,
                         chunknum     *FieldRefDir)
{
objheader *ObjHdr        = NullPtr;

 if (Anchor->DbId != RenvId)
   *Status = Env_BadDbId;

 if (AllocQty) 
 {
   if (OmNewObjHdr (&RenvTkn,  EnvObjNum,  AllocQty, McStat,  &ObjHdr)) 
   {
      ObjHdr->ObjSize =  ObjHdrSize[EnvObjNum];
      ObjHdr->FieldCount = 0;
      if (DeBug || OaBug)
         TraceMsg (0, "   NEW ");
   } else {
     if (*Status == Om_ObjHdrExists) 
     {
       *Status = Env_Normal;
        OmGetObjHdr (&RenvTkn, EnvObjNum, McStat, &ObjHdr);
     }
     if (DeBug || OaBug)
        TraceMsg (0, "   GET ");
   }
   if (ObjHdr == NULL)
       *Status = Env_NullObjHdr;

   if (Normal(*Status))
   {
      *LocalAttrNumMap =   ObjHdr->LocalAttrNumMap;
      *FieldNumMap     =   ObjHdr->FieldNumMap;
      *FieldOffsetDir  =   ObjHdr->FieldOffsetDir;
      *ClassOffsetDir  =   ObjHdr->ClassOffsetDir;
      *FieldCount      = &(ObjHdr->FieldCount);
      *FieldRefCount   = &(ObjHdr->FieldRefCount);
      *FieldRefDir     =   ObjHdr->FieldRefDir;
   } else {
      *Status = Env01_BadDescTypeType;
   }
 } else  {
   sprintf (Msg, " *** ObjHeader for EnvObj = %u has ZERO AllocQty\n",
            EnvObjNum);
   TraceMsg (0, Msg);
   *Status = Env_ZeroAllocQty;
 }


TRACK(TrackBak,"EnvBldObjHdr\n");
return (STAT);
}
 

boolean    EnvInstallObjHdr (tokentype    *Anchor,     numtype   EnvObjNum,
                             ft F,lt Z,zz *Status,     numtype  *LocalObj)
{  /* Allocate space for An ObjectDesc                                     */
objheader *ObjHdr        = NullPtr;
sizetype   ObjSize       = 0;
addrtype   HdrAddr       = NullPtr;

 if (ObjAllocQty[EnvObjNum]) 
 {
   if (DeBug || OaBug || FileBug)
   if (sprintf (Msg, " Install ObjHdr for Obj[%3u] In CoreDb[%2u]\n",
            EnvObjNum, Anchor->DbId))
       TraceMsg (0, Msg);

   if (OmNewObjHdr (Anchor,    EnvObjNum,  ObjAllocQty[EnvObjNum],
                     McStat,  &ObjHdr)) 
   {
      ObjHdr->ObjSize =  ObjHdrSize[EnvObjNum];
      ObjHdr->FieldCount = 0;
      if (DeBug || OaBug)
          TraceMsg (0, "   NEW ");

   } else {
     if (*Status == Om_ObjHdrExists) 
     {
        *Status = Env_Normal;
        OmGetObjHdr (Anchor,   EnvObjNum,  McStat,   &ObjHdr);
     }
     if (DeBug || OaBug)
        TraceMsg (0, "   GET ");
   }
   if (ObjHdr == NULL) *Status = Env_NullObjHdr;
   if (Normal(*Status)) 
   {
      *LocalObj  = ObjHdr->LocalObjNum;

      if (ObjHdrSize[EnvObjNum] == 0)
      {
         if (OaGetUnit        (&HdrSizeVchunkTkn, EnvObjNum,
                               McStat,            (addrtype )&ObjSize))
         if (OaGetVchunkAddr  (&HdrSizeVchunkTkn, 
                               McStat,            &HdrAddr))
         if (ObjSize)
            ObjHdrSize[EnvObjNum] = ObjSize;
         else
           *Status = Err_BadDesign;
      }

      if (Normal(*Status)) 

      if (InstallObjChunks (Anchor, ObjHdr, EnvObjNum, McStat))
      if (*Status == Env_ZeroAllocQty)
           *Status = Env_Normal;

   } else {
      *Status = Env01_BadDescTypeType;
   }
 } else  {
   sprintf (Msg, " *** ObjHeader for EnvObj = %u has ZERO AllocQty\n",
            EnvObjNum);
   TraceMsg (0, Msg);
   *Status = Env_ZeroAllocQty;
 }

TRACK(TrackBak,"EnvInstallObjHdr\n");
return (STAT);
}
 

boolean InstallObjChunks (tokentype *Anchor,    objheader    *ObjHdr,
                          numtype    ObjNum,    ft F,lt Z,zz *Status)
{
dbheader     *CoreDb          = NullPtr;
objdesctype  *EnvObj          = NullPtr;
indextype     StackPtr        = 0;
tokentype     ObjToken;
tokentype     TypeToken;
tokentype     AttrToken;
attrdesctype *EnvAttr         = NullPtr;
typedesctype *EnvType         = NullPtr;
tokentype     VclasssTkn;
tokentype     VoffsetsTkn;
tokentype     VfieldToken;
wordtype     *Vclasss         = NullPtr;
typeunsize   *Voffsets        = NullPtr;
wordtype     *Vchunk          = NullPtr;
indextype     Vindex          = 0;
indextype     LocalAttrNum    = 0;
numtype       FieldNum        = 0;
indextype     Fields          = 0;
indextype     Classs          = 0;
indextype     FieldCount      = 0;
indextype     ClassCount      = 0;
indextype     FieldRefCount   = 0;
indextype     FieldArrayCount = 0;
indextype     FieldMapCount   = 0;
bytessize     Offset          = 0;
typeunsize    ClassOffset     = 0;
boolean       AttrFound       = False;
boolean       SystemObj       = True;
sizetype      EnvTypeSize     = 0;
tokentype     BldDescTkn;
objdesctype  *BldDesc         = NullPtr;
objheader    *BldObjHdr       = NullPtr;
maptkntype    BldMapTkn;
reftkntype    FieldRefTkn;

   ObjToken.DbId     = RenvId;
   TypeToken.DbId    = RenvId;
   AttrToken.DbId    = RenvId;
   VclasssTkn.DbId   = RenvId;
   VoffsetsTkn.DbId  = RenvId;
   VfieldToken.DbId  = RenvId;
   BldDescTkn.DbId   = RenvId;

   BldMapTkn.FieldNum   = 0;
   BldMapTkn.LocalObj   = 0;

   if (TmFetchCoreDb     (Anchor,    McStat, &CoreDb))
   if (EnvFetchObjHandle (ObjNum,    McStat, &ObjToken.Handle))
   if (OaGetObject       (&ObjToken, McStat, (addrtype *)&EnvObj))
   {
       VfieldToken.Handle = EnvObj->Fields;
       VclasssTkn.Handle  = EnvObj->ClassFields;
       VoffsetsTkn.Handle = EnvObj->ClassOffsets;
       if (EnvObj->ObjNum > EOS_ObjNum)
           SystemObj = False;
   }

   if (Normal(*Status))
   if (OaGetVchunkAddr     (&VclasssTkn,   McStat, (addrtype *)&Vclasss)) 
   if (OaGetVchunkAddr     (&VoffsetsTkn,  McStat, (addrtype *)&Voffsets)) 
   if (OaGetVchunkAddr     (&VfieldToken,  McStat, (addrtype *)&Vchunk)) 
   if (OaGetVchunkStackPtr (&VfieldToken,  McStat, &Fields))
   if (OaGetVchunkStackPtr (&VclasssTkn,   McStat, &Classs))
   if (DeBug)
       OaDumpVchunk        (&VfieldToken,  McStat);

   Vindex = 0;
   if (Fields)
   while (Normal(*Status) && Fields-- > 0) 
   {
      if (EnvFetchAttrHandle (Vchunk[Vindex],
                              McStat,           &AttrToken.Handle))

      if (OaGetObject (&AttrToken, McStat, (addrtype *)&EnvAttr)) 
      {
         if (EnvAttr->AttrNum   == That_AttrNum)
             ObjHdr->EnvObjType  = That_AttrNum;

         TypeToken.Handle = EnvAttr->TypeObj;
         if (OaGetObject (&TypeToken, McStat, (addrtype *)&EnvType)) 
         if (DeBug)
         if (sprintf (Msg, " Build FieldMapType :: \n"))
             TraceMsg (0, Msg);


         if (Normal(*Status))
         if (EnvAttr->AttrType == aMapRefAttr)
         {
            BldDescTkn.Handle = EnvAttr->BuildDesc;
            if (OaGetObject (&BldDescTkn,   McStat, (addrtype *)&BldDesc)) 
            if (OaInstallObjHdr (Anchor,                  BldDesc->ObjNum, 
                                 McStat,                 &BldObjHdr))
                EnvTypeSize   = BldDesc->ObjSize;
            if (StrucAlignment  >= 4)
            {

            }

            if (DeBug || OaBug)
            if (sprintf (Msg,
                         " MapRefAttr  BldDesc->ObjNum = %2u; Size= %3u\n",
                         BldDesc->ObjNum, EnvTypeSize))
            if (TraceMsg (0, Msg))
            if (sprintf (Msg, "             ObjHdr->ObjNum  = %2u;\n",
                         BldObjHdr->ObjNum))
               TraceMsg (0, Msg);

            if (OaGetObject (&ObjToken,  McStat, (addrtype *)&EnvObj))
            if (OaGetObject (&AttrToken, McStat, (addrtype *)&EnvAttr)) 
                OaGetObject (&TypeToken, McStat, (addrtype *)&EnvType);

         } else {
            if (EnvAttr->AttrType == aArrayAttr)
                EnvTypeSize = sizeof (handletype);

            else if (EnvAttr->AttrType == aDblPtrAttr)
                EnvTypeSize = sizeof (handletype);

            else if (EnvAttr->AttrType == aVarrayAttr)
                EnvTypeSize = sizeof (handletype);

            else if (EnvAttr->AttrType == aVchunkAttr)
                EnvTypeSize = sizeof (tokentype);

            else
                EnvTypeSize   = EnvType->Size;

            if (EnvAttr->AttrNum != That_AttrNum)
            {
               if (EnvAttr->AttrNum == DefaultValue_AttrNum)
                  EnvAlignMember (EnvAttr->AttrType,          aAddr,
                                  EnvTypeSize,      (sizetype *)&Offset);
               else
                  EnvAlignMember (EnvAttr->AttrType,          EnvType->TypeType,
                                  EnvTypeSize,      (sizetype *)&Offset);
            } else {
                EnvTypeSize     = 0;
            }
         }
         EnvTypeSize *= EnvAttr->UnitSize;
         if (EnvAttr->UnitSize > 1)
         if (AdminBug || DeBug)
         if (sprintf (Msg, "     EnvTypeSize = %3u\n", EnvTypeSize))
             TraceMsg (0, Msg);

         if (Normal(*Status))
             MemPushFieldOffset (ObjHdr->FieldOffsetDir,     Offset,
                                 McStat,                    &FieldNum);

         ClassCount  = 0;
         AttrFound   = False;
         ClassOffset = -1;
         if (Normal(*Status))
         while (ClassCount < Classs
         &&    !AttrFound)
         {
            if (Vclasss[ClassCount] == Vchunk[Vindex])
            {
                AttrFound   = True;
                ClassOffset = Offset;
                Voffsets[ClassCount] = Offset;
            }
            ++ClassCount;
         }
         if (Normal(*Status))
             MemPushFieldOffset (ObjHdr->ClassOffsetDir,
                                 (typesize )ClassOffset,
                                 McStat,                    &FieldNum);

         if (Normal(*Status))
         if (! strcmp (EnvType->Nam, "referencetype")) 
         {
            FieldRefTkn.FieldNum = FieldNum;
            FieldRefTkn.CppAlign = 0;
            if (MemPushSomeBytes (ObjHdr->FieldRefDir,   sizeof (reftkntype),
                                  (addrtype )&FieldRefTkn,
                                  McStat,            &StackPtr))
               FieldRefCount++;

            if (DeBug || OaBug)
            if (sprintf (Msg, " RefPtrType for FieldNum = %2u offset@ %3u\n",
                         FieldRefTkn.FieldNum, Offset))
                SendMsg (0, Msg);

         } else if (EnvAttr->AttrType == aMapRefAttr) {
            if (FieldMapCount == 0)
            if (MemMakeChunk   (sizeof(maptkntype),    Alloc_FieldRefs, 
                                Extend_FieldRefs,      RenvId,
                                McStat,               &(ObjHdr->FieldMapDir)))
                MemPutStackPtr (ObjHdr->FieldMapDir,  (indextype  )1,
                                McStat); 

            BldMapTkn.FieldNum =  FieldNum;
            BldMapTkn.LocalObj =  BldObjHdr->LocalObjNum;

            FieldMapCount++;

            if (Normal(*Status))
            if (MemPushSomeBytes
                               (ObjHdr->FieldMapDir,   sizeof (maptkntype),
                                (addrtype )&BldMapTkn,
                                McStat,                       &StackPtr))

            if (DeBug || OaBug)
            if (sprintf (Msg,
                    " MapType for FieldNum =%2u LocalObj=%4u; offset@ %3u\n",
                         BldMapTkn.FieldNum, BldMapTkn.LocalObj, Offset))
                TraceMsg (0, Msg);

         } else if (! strcmp (EnvAttr->Nam, "Handle")) {
            OmPutHandleOffset (Anchor, ObjNum, Offset, McStat);

         } else if (!SystemObj  
                &&  (EnvType->TypeType == aChunk
                  || EnvAttr->AttrType == aArrayAttr
                  || EnvAttr->AttrType == aDblPtrAttr
                  || EnvAttr->AttrType == aVarrayAttr))
         {
            if (FieldArrayCount == 0)
            if (MemMakeChunk   (sizeof(numtype),       Alloc_FieldRefs, 
                               Extend_FieldRefs,      RenvId,
                               McStat,               &(ObjHdr->FieldArrayDir)))
                MemPutStackPtr (ObjHdr->FieldArrayDir, (indextype  )1,
                               McStat); 

            if (Normal(*Status))
            if (MemPushFieldNum (ObjHdr->FieldArrayDir,   FieldNum,
                                 McStat,                 &StackPtr))
            if (DeBug || OaBug)
            if (sprintf (Msg, " ArrayType for FieldNum = %2u offset@ %3u\n",
                         FieldNum, Offset))
                TraceMsg (0, Msg);
            FieldArrayCount++;
         }

         if (Normal(*Status))
         if (EnvFetchLocalAttrNum (Anchor,             EnvAttr->AttrNum, 
                                   McStat,            &LocalAttrNum))
         if (!LocalAttrNum 
         &&  (Anchor->DbId != RenvId)) 
         {

            /* Update Map from this Db LocalAttrNum to Env AttrNum         */
            if (MemPushNum        (CoreDb->AttrNumMap,
                                   EnvAttr->AttrNum,
                                   McStat,            &LocalAttrNum))

            /* Put Current known size of Attribute Type                    */
            if (MemPushNum        (CoreDb->LocalAttrSizeTbl,
                                   EnvTypeSize,
                                   McStat,            &LocalAttrNum))

            /* Update Map from EnvAttrNum to this Db LocalAttrNum          */
            if (MemPutAttrNum     (CoreDb->LocalAttrNumMap,   
                                   (indextype  )EnvAttr->AttrNum, 
                                   (chunknum )LocalAttrNum,    McStat))

               /* Increment this Db's LocalAttr count                      */
               ++CoreDb->LocalAttrCount;
         }

         /* Update Map from FieldNum to this Db LocalAttrNum               */
         if (Normal(*Status))
         if (MemPushNum           (ObjHdr->LocalAttrNumMap,
                                   LocalAttrNum,
                                   McStat,                &StackPtr))

         /* Update Map from this Db's LocalAttrNum to FieldNum             */
         if (MemPutFieldNum       (ObjHdr->FieldNumMap,    LocalAttrNum, 
                                   (chunknum )StackPtr,    McStat))
               FieldCount++;
         if (DeBug)
         if (sprintf (Msg, 
              " PUSH Attr[%3u](%3u)  %22s  @ OffSet= %4u size %u\n", 
              EnvAttr->AttrNum, LocalAttrNum,      EnvAttr->Nam, 
              Offset,           EnvTypeSize))
            TraceMsg (0, Msg);

         /* Append Field Slot of AttrSize bytes to the ObjHdr Struct       */
         if ((int )EnvTypeSize >= 0)
            Offset += EnvTypeSize;
         else
            Offset += sizeof (strhandle);
      }
      Vindex++;
   }
 
   if (Normal(*Status)) 
   {
      EnvAlignStruc( False, False, &Offset );
      ObjHdr->ObjSize         = Offset;
      ObjHdr->PsudoSize       = Offset;
      ObjHdrSize[ObjNum]      = Offset;

      ObjHdr->FieldCount      = FieldCount;
      ObjHdr->FieldRefCount   = FieldRefCount;
      ObjHdr->FieldMapCount   = FieldMapCount;
      ObjHdr->FieldArrayCount = FieldArrayCount;
      if (DeBug || FileBug) 
         OaDumpObjHdr (0, Anchor, ObjNum, McStat);

   }

TRACK(TrackBak,"InstallObjChunks\n");
return(STAT);
}
 

boolean EnvBldObjImage    (dbheader     *CoreDb,     objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,     numtype      *FieldChunk,
                           numtype      *OffsetChunk,indextype    *FieldCount)
{
objdesctype  *EnvObj         = NullPtr;
indextype     StackPtr       = 0;
tokentype     ObjToken;
tokentype     AttrToken;
attrdesctype *EnvAttr        = NullPtr;
numtype       EnvLocalAttr   = 0;
numtype       LocalAttr      = 0;
tokentype     VfieldToken;
wordtype     *Vchunk;
indextype     Vindex         = 0;
bytessizetype AttrSize       = 0;
bytessizetype FieldOffset    = 0;

   ObjToken.DbId     = RenvId;
   AttrToken.DbId    = RenvId;
   VfieldToken.DbId  = RenvId;

   if (sprintf (Msg, " LastEnvAttrCount:: Renv = %4u  DB = %4u\n",
               CoreDbs[RenvId]->LastEnvAttrCount, CoreDb->LastEnvAttrCount))
   if (TraceMsg (0, Msg))
   if (CoreDbs[RenvId]->LastEnvAttrCount >= CoreDb->LastEnvAttrCount)
   if (MemPutStackPtr    (CoreDb->LocalAttrNumMap,
                          1+ CoreDb0->LastEnvAttrCount,
                          McStat))
       CoreDb->LastEnvAttrCount = CoreDb0->LastEnvAttrCount;

   if (Normal(*Status))
   if (EnvFetchObjHandle (ObjHdr->ObjNum,    McStat,    &ObjToken.Handle))
   if (OaGetObject       (&ObjToken, McStat, (addrtype *)&EnvObj)) 
       VfieldToken.Handle = EnvObj->Fields;

   if (Normal(*Status))
   if (OaGetVchunkStackPtr (&VfieldToken,    McStat,        FieldCount))

   if (MemMakeChunk      (sizeof (numtype), (*FieldCount + 1),    10,
                          CoreDb->DbId,      McStat,        FieldChunk))
   if (MemPutStackPtr    (*FieldChunk,       1,             McStat))

   if (MemMakeChunk      (sizeof (numtype), (*FieldCount + 1),    10,
                          CoreDb->DbId,      McStat,        OffsetChunk))
   if (MemPutStackPtr    (*OffsetChunk,      1,             McStat))
   {   
       TraceMsg       (0,       " Build Attributes\n");
       sprintf (Msg,  "    Base Image ObjNum[%3u] ObjSize=%4u Fields=%2u\n",
                EnvObj->ObjNum,  ObjHdrSize[EnvObj->ObjNum], *FieldCount);
       TraceMsg (0, Msg);
   }

   Vindex = 0;
   if (Normal(*Status))
   if (OaGetVchunkAddr       (&VfieldToken,  McStat,   (addrtype *)&Vchunk))

   while (Normal(*Status) && Vindex < *FieldCount) 
   {
      if (EnvFetchAttrHandle (Vchunk[Vindex],
                              McStat,                  &AttrToken.Handle))
      if (OaGetObject        (&AttrToken, McStat,      (addrtype *)&EnvAttr)) 

      if (MemGetAttrNum      (CoreDb0->LocalAttrNumMap, EnvAttr->AttrNum,
                              McStat,                  &EnvLocalAttr))
      if (MemGetSize         (CoreDb0->LocalAttrSizeTbl,EnvLocalAttr,
                              McStat,                  &AttrSize))

      if (MemGetAttrNum      (CoreDb->LocalAttrNumMap,  EnvAttr->AttrNum,
                              McStat,                  &LocalAttr))

      if ( EnvAttr->TypeType == aAddr && AttrSize != 0 )
	 AttrSize = sizeof( addrtype ) ;
      
      if (Normal(*Status))
      if (!LocalAttr)
      if (MemPushAttrNum     (CoreDb->AttrNumMap,       EnvAttr->AttrNum,
                              McStat,                  &LocalAttr))
      if (MemPushSize        (CoreDb->LocalAttrSizeTbl, AttrSize,
                              McStat,                  &LocalAttr))
      if (MemPutAttrNum      (CoreDb->LocalAttrNumMap,  EnvAttr->AttrNum,
                              LocalAttr,                McStat))
      {
         CoreDb->LocalAttrCount = LocalAttr;
      }

      if (Normal(*Status))
      if (MemPushAttrNum     (*FieldChunk,              LocalAttr,
                              McStat,                  &StackPtr))
          MemPushSize        (*OffsetChunk,             FieldOffset,
                              McStat,                  &StackPtr);

      if (EnvAttr->AttrNum == That_AttrNum)
         AttrSize  = 0;

      FieldOffset += AttrSize;
      FieldOffset += (AttrSize % Byte_Alignment);
    ++Vindex;
   }


TRACK(TrackBak,"EnvBldObjImage\n");
return(STAT);
}


boolean    EnvNewObjHdr        (tokentype   *Anchor,   numtype       EnvObjNum,
                                indextype    AllocQty, ft F,lt Z,zz *Status)
{ 
 /* allocate space for An ObjectDesc                                       */
 /* This function is not called by AnyOne                                  */
objheader *ObjHdr        = NullPtr;
 if (Anchor->DbId != RenvId)
    *Status = Env_BadDbId;

 if (AllocQty) 
 {
   if (OmNewObjHdr (&RenvTkn,  EnvObjNum,  AllocQty, McStat,  &ObjHdr)) 
   {
      ObjHdr->ObjSize =  ObjHdrSize[EnvObjNum];
      if (DeBug)
      if (sprintf (Msg, 
              "New Obj[%2d] :: Local ObjHdr[%3u] Env0 size %d * alloc(%d)\n",
               EnvObjNum,             ObjHdr->LocalObjNum,
               ObjHdrSize[EnvObjNum], ObjAllocQty[EnvObjNum]))
         TraceMsg (1, Msg);
   } else
      *Status = Env01_BadDescTypeType;
 } else  {
   sprintf (Msg, " *** ObjHeader for EnvObj = %u has ZERO AllocQty\n",
            EnvObjNum);
   TraceMsg (0, Msg);
   *Status = Env_Normal;
 }

TRACK(TrackBak,"EnvNewObjHdr\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
 
boolean    EnvInstallAttr (handletype   AttrHandle,  numtype       EnvAttrNum,
                           bytessize    Size,        ft F,lt Z,zz *Status)
{  /* Append to DbHeader Attr Chunks...   */
indextype    LocalAttrNum = 0;

    if (MemPushSize   (CoreDb0->LocalAttrSizeTbl,  Size,
                       McStat,                    &LocalAttrNum))
    if (MemPushObjNum (CoreDb0->AttrNumMap,        EnvAttrNum,
                       McStat,                    &LocalAttrNum))
    if (MemPutHandle  (CoreDb0->AttrHandleDir,     (indextype )EnvAttrNum,
                       AttrHandle,                 McStat))
    if (MemPutObjNum  (CoreDb0->LocalAttrNumMap,   (indextype )EnvAttrNum,
                       (num )LocalAttrNum,         McStat)) 
    {
       CoreDb0->LocalAttrCount = CoreDb0->LastEnvAttrCount = LocalAttrNum;
       if (CoreDbs[PrimalId])
       if (MemPutObjNum  (CoreDbs[PrimalId]->LocalAttrNumMap,
                          (indextype )EnvAttrNum,    0,         McStat)) 
           CoreDbs[PrimalId]->LocalAttrCount = CoreDb0->LastEnvAttrCount;
    }

TRACK(TrackBak,"EnvInstallAttr\n");
return (STAT);
}
 

boolean    EnvFetchLocalAttrNum ( 
                               tokentype    *Anchor,  numtype   EnvAttrNum,
                               ft F,lt Z,zz *Status,  numtype  *LocalAttrNum)
{ 
statustype  ShellStatus  = Env_Normal;
dbheader   *CoreDb       = NullPtr;

  if (TmFetchCoreDb (Anchor, McStat, &CoreDb))
  if (EnvAttrNum     <=  CoreDb->LastEnvAttrCount)
  {
      MemGetObjNum     (CoreDb->LocalAttrNumMap,  (indextype )EnvAttrNum,
                        McStat,                    LocalAttrNum);
  } else {
      MemPutStackPtr   (CoreDb->LocalAttrNumMap,
                        1+ CoreDb0->LastEnvAttrCount,      McStat);
      CoreDb->LastEnvAttrCount = CoreDb0->LastEnvAttrCount;
     *LocalAttrNum  = 0;
  }

  if (!Normal(*Status))
  if (CoreDb != NullPtr)
     HmDumpDbHdr  (0, Anchor, ShellMcStat);


TRACK(TrackBak,"EnvFetchLocalAttrNum\n");
return (STAT);
}
 

boolean    EnvFetchAttrHandle (numtype       AttrNum,
                               ft F,lt Z,zz *Status,  handletype *AttrHandle)
{ 
  MemGetHandle (CoreDb0->AttrHandleDir, AttrNum,  McStat, AttrHandle);

TRACK(TrackBak,"EnvFetchAttrHandle\n");
return (STAT);
}
 

boolean    EnvFetchAttrSize   (numtype       AttrNum,
                               ft F,lt Z,zz *Status,  sizetype  *AttrSize)
{ 
  MemGetWord   (CoreDb0->LocalAttrSizeTbl,              AttrNum,  
                McStat,                     (wordtype *)AttrSize);

TRACK(TrackBak,"EnvFetchAttrSize\n");
return (STAT);
}
 

boolean    CppEnvGetThatTkn (addrtype      CppObject, numtype      ObjNum,
                             ft F,lt Z,zz *Status,    tokentype  **CppToken)
{
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;
tokentype     VfieldsTkn;
tokentype     VoffsetsTkn;
indextype     Vindex      = 0;
booleantype   AttrFound   = False;
indextype     FieldCount  = 0;
numtype      *Vfields     = NullPtr;
typesize     *Voffsets    = NullPtr;
numtype       AttrNum     = Handle_AttrNum;
typesize      Offset      = 0;

     SendMsg (0, " *** ERROR *** CppEnvGetThatTkn **************\n");
     SendMsg (0, "              SHOULD BE INVOKED\n");

     if (MemGetHandle    (CoreDb0->ObjHandleDir,               ObjNum,
                          McStat,                             &ObjDescHndl))
/*
     if (HmGetObjectAddr (CoreDb0,                             ObjDescHndl,
                          McStat,                 (addrtype *)&ObjDesc))
*/
     {
         ObjDescTkn.DbId   = RenvId;
         ObjDescTkn.Handle = ObjDescHndl;
         OaGetObject     (&ObjDescTkn,    McStat, (addrtype *)&ObjDesc);
     }

     if (Normal(*Status))
     {
         VoffsetsTkn.DbId    =  RenvId;
         VoffsetsTkn.Handle  =  ObjDesc->ClassOffsets;
         VfieldsTkn.DbId     =  RenvId;
         VfieldsTkn.Handle   =  ObjDesc->Fields;
         if (Normal(*Status))
         if (OaGetVchunkAddr (&VoffsetsTkn,   McStat,  (addrtype *)&Voffsets))
         if (OaGetVchunkAddr (&VfieldsTkn,    McStat,  (addrtype *)&Vfields))
         if (OaGetVchunkStackPtr
                             (&VfieldsTkn,    McStat,  &FieldCount))

         while (Vindex < FieldCount
         &&    !AttrFound)
         {
            if (Vfields[Vindex] == AttrNum)
            {
                Offset    = Voffsets[Vindex];
                AttrFound = True;
            }
            Vindex++;
         }
     }

     if (AttrFound)
     {
       *CppToken = (tokentype*)((char *)CppObject + (indextype )Offset);
        if (DeBug >= 0)
        if (sprintf (Msg, "  CppEnvGetThatTkn:: [%2u:%4u] @%x\n",
                     (*CppToken)->DbId, (*CppToken)->Handle, *CppToken))
            TraceMsg (0, Msg);

     } else
       *Status   = Err_BadDesign;

TRACK(TrackBak,"CppEnvGetThatTkn\n");
return (STAT);
}
 

boolean    EnvFetchAttrOffset (tokentype    *ObjTkn,  numtype    AttrNum,
                               objheader    *ObjHdr,  numtype    FieldNum,
                               ft F,lt Z,zz *Status,  sizetype  *Offset)
{ 
numtype       MemLoc      = 0;
tokentype     VfieldsTkn;
tokentype     VoffsetsTkn;
booleantype   AttrFound   = False;
numtype       ObjNum      = 0;
sizetype      DbOffset    = 0;
dbheader   *CoreDb  = NullPtr;

  if (TmFetchCoreDb      (ObjTkn,  McStat,  &CoreDb))
  if (HmGetObjectMemLoc  (CoreDb,                         ObjTkn->Handle,
                          McStat,                        &MemLoc))

  if (Normal(*Status))
  if (MemLoc)
  {
     if (HmGetObjNum      (CoreDb,                        ObjTkn->Handle,
                           McStat,                       &ObjNum))
     if (Trans_FetchAttrOffset 
                          (ObjNum,             Cpp_Apl,   FieldNum, 
                           McStat,             Offset))
        AttrFound = True;

  } else {
    MemGetSize    (ObjHdr->FieldOffsetDir,    (indextype )FieldNum,
                   McStat,                                Offset);
    AttrFound    = True;
  }

  if (!AttrFound)
   *Status = Env_AttrNotInDbmsClassDef;

TRACK(TrackBak,"EnvFetchAttrOffset\n");
return (STAT);
}
 

boolean    Env_FetchObj0AttrOffset 
                              (numtype       MemLoc,  numtype    AttrNum,
                               objheader    *ObjHdr,
                               ft F,lt Z,zz *Status,  sizetype  *Offset)
{ 
numtype      *Vfields         = NullPtr;
booleantype   Found           = False;
booleantype   AttrFound       = False;
indextype     Vindex          = 0;
numtype       NumFields       = 0;

  if (MemLoc)
  {
     if (Env_FetchObjAttrOffset 
                          (MemLoc,  ObjHdr->ObjNum,    AttrNum, 
                           McStat,                     Offset))
        AttrFound = True;

  } else {
     Vindex            = 0;
     NumFields         = ObjHdr->FieldCount;

     if (MemGetChunkAddr  (ObjHdr->FieldNumMap,  McStat, (addrtype *)&Vfields)) 
     if (NumFields)
     while (Normal(*Status) 
     &&     NumFields--     > 0
     &&     Found          == False) 
     {
        if (Vfields[Vindex]  == AttrNum)
        {
           Found  = True;

        } else
         ++Vindex;
     }
     if (Found)
     if (MemGetSize       (ObjHdr->FieldOffsetDir,   (indextype )Vindex,
                           McStat,                               Offset))
        AttrFound    = True;
  }

  if (!AttrFound)
   *Status = Env_AttrNotInDbmsClassDef;

TRACK(TrackBak,"Env_FetchObj0AttrOffset\n");
return (STAT);
}
 

boolean Env_FetchObjAttrOffset
                          (numtype       MemLoc,
                           numtype       ObjNum,    numtype       AttrNum,    
                           ft F,lt Z,zz *Status,    sizetype     *Offset)
{
tokentype     ObjDescTkn      = NullToken;
objdesctype  *EnvObj          = NullPtr;
tokentype     VoffsetsTkn;
tokentype     VfieldToken;
typeunsize   *Voffsets        = NullPtr;
numtype      *Vfields         = NullPtr;
indextype     Vindex          = 0;
numtype       NumFields       = 0;
boolean       Found           = False;
numtype       LocalObjNum     = 0;
             
   VfieldToken.DbId  = RenvId;
   VoffsetsTkn.DbId  = RenvId;

   ObjDescTkn.DbId   = RenvId;

   if (MemGetHandle    (CoreDb0->ObjHandleDir,    ObjNum,  
                        McStat,                  &ObjDescTkn.Handle))
   if (OaGetObject     (&ObjDescTkn,    McStat, (addrtype *)&EnvObj))
   {
      VfieldToken.Handle   = EnvObj->Fields;
      VoffsetsTkn.Handle   = EnvObj->ClassOffsets;
   }

   if (Normal(*Status))
   if (OaGetVchunkAddr     (&VfieldToken,  McStat, (addrtype *)&Vfields)) 
   if (OaGetVchunkAddr     (&VoffsetsTkn,  McStat, (addrtype *)&Voffsets)) 
       OaGetVchunkStackPtr (&VfieldToken,  McStat,             &NumFields);

   Vindex         = 0;
   if (NumFields)
   while (Normal(*Status) 
   &&     NumFields--     > 0
   &&     Found          == False) 
   {
      if (Vfields[Vindex]  == AttrNum)
      {
        *Offset     = Voffsets[Vindex];
         Found  = True;

      } else
       ++Vindex;
   }

   if (!Found)
   {
      sprintf (Msg,
           " Env_FetchObjAttrOffset:: AttrNum =%4u; NotFound in Class=%4u\n",
               AttrNum, ObjNum);
      TraceMsg (0, Msg);
   }


TRACK(TrackBak,"Env_FetchObjAttrOffset\n");
return (STAT);
}


boolean Env_FetchObjAttrSpec
                          (numtype       ObjNum,    numtype       AttrNum,    
                           ft F,lt Z,zz *Status,    attrtype     *AttrType,  
                           numtype      *RefObjNum, envclass     *RefEnvType,
                           sizetype     *UnitSize,  typetype     *UnitType,
                           valueclass   *ValueType, sizetype     *Offset)
{
tokentype     ObjDescTkn      = NullToken;
objdesctype  *EnvObj          = NullPtr;
tokentype     AttrDescTkn     = NullToken;
attrdesctype *EnvAttr         = NullPtr;
tokentype     BldDescTkn      = NullToken;
objdesctype  *BldDesc         = NullPtr;
tokentype     VoffsetsTkn;
tokentype     VfieldToken;
typeunsize   *Voffsets        = NullPtr;
numtype      *Vfields         = NullPtr;
indextype     Vindex          = 0;
numtype       NumFields       = 0;
boolean       Found           = False;
             
   VfieldToken.DbId  = RenvId;
   VoffsetsTkn.DbId  = RenvId;

   ObjDescTkn.DbId   = RenvId;
   AttrDescTkn.DbId  = RenvId;
   BldDescTkn.DbId   = RenvId;

   if (MemGetHandle    (CoreDb0->ObjHandleDir,    ObjNum,  
                        McStat,                  &ObjDescTkn.Handle))
   if (OaGetObject     (&ObjDescTkn,    McStat, (addrtype *)&EnvObj))
   {
      VfieldToken.Handle   = EnvObj->Fields;
      VoffsetsTkn.Handle   = EnvObj->ClassOffsets;
   }

   if (Normal(*Status))
   if (OaGetVchunkAddr     (&VfieldToken,  McStat, (addrtype *)&Vfields)) 
   if (OaGetVchunkAddr     (&VoffsetsTkn,  McStat, (addrtype *)&Voffsets)) 
       OaGetVchunkStackPtr (&VfieldToken,  McStat,             &NumFields);

   Vindex         = 0;
   if (NumFields)
   while (Normal(*Status) 
   &&     NumFields--     > 0
   &&     Found          == False) 
   {
      if (Vfields[Vindex]  == AttrNum)
      {
         if (EnvFetchAttrHandle (AttrNum,      McStat, &AttrDescTkn.Handle))
         if (OaGetObject        (&AttrDescTkn, McStat, (addrtype *)&EnvAttr)) 
         {
           *AttrType   = EnvAttr->AttrType;
           *RefObjNum  = 0;
           *UnitSize   = EnvAttr->UnitSize;
           *UnitType   = EnvAttr->TypeType;
           *Offset     = Voffsets[Vindex];
           *RefEnvType = aEnvValue;

            Found  = True;

            if ((BldDescTkn.Handle = EnvAttr->BuildDesc) > 0)
            {
               if (OaGetObject (&BldDescTkn,   McStat, (addrtype *)&BldDesc)) 
                 *RefObjNum   = BldDesc->ObjNum;
              *RefEnvType     = aEnvObj;

            } else {
               if (EnvAttr->AttrType == aArrayAttr
               ||  EnvAttr->AttrType == aVarrayAttr
               ||  EnvAttr->AttrType == aChunkAttr
               ||  EnvAttr->AttrType == aDblPtrAttr)
               {
                 *RefObjNum   = EnvAttr->TypeObj;
                 *RefEnvType  = aEnvType;
               }
            }
         }

      } else
       ++Vindex;
   }

   if (!Found)
   {
      sprintf (Msg,
               " Env_FetchObjAttrSpec:: AttrNum =%4u; NotFound in Class=%4u\n",
               AttrNum, ObjNum);
      TraceMsg (0, Msg);
   }

TRACK(TrackBak,"Env_FetchObjAttrSpec\n");
return (STAT);
}


boolean    Env_FetchFieldOffset
                        (tokentype    *ObjTkn,      fieldspec     FieldTkns[],
                         numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
			 sizetype     *Offset)
{ 
numtype       MemLoc      = 0;
booleantype   AttrFound   = False;
numtype       ObjNum      = 0;
sizetype      DbOffset    = 0;
numtype       AttrId      = 0;
attrtype      AttrType    = aNonTypeAttr;
numtype       RefObjNum   = 0;
typetype      FieldType   = aNonTypeType;
sizetype      BaseOffset  = 0;
dbheader   *CoreDb  = NullPtr;
tokentype     BldDescTkn  = NullToken;
objdesctype  *BuildDesc   = NullPtr;
sizetype      DbBase      = 0;
sizetype      CppBase     = 0;

BldDescTkn.DbId       = RenvId;

  if (TmFetchCoreDb      (ObjTkn,  McStat,  &CoreDb))
  if (HmGetObjectMemLoc  (CoreDb,                         ObjTkn->Handle,
                          McStat,                        &MemLoc))
  if (Normal(*Status))
  if (MemLoc)
  {
     if (Trans_FetchFieldOffset 
                          (ObjTkn,         Cpp_Apl, 
                           FieldTkns,      SubLevel,
	                   McStat,         FieldSize,    &AttrId,
                           &AttrType,      &RefObjNum,    &FieldType,
                           &DbOffset,       Offset,
                           &DbBase,        &CppBase))
     {
        AttrFound = True;
        if (RefObjNum)
          if (EnvFetchObjHandle (RefObjNum,  McStat,  &BldDescTkn.Handle))
          if (OaGetObject       (&BldDescTkn,
                                 McStat,    (addrtype *)&BuildDesc))
          {
             *FieldSize   = BuildDesc->ObjSize;
             *Offset     += (*FieldSize * FieldTkns[SubLevel-1].Index);
          }
      }

  } else {
     if (HmGetObjNum      (CoreDb,                        ObjTkn->Handle,
                           McStat,                       &ObjNum))
     if (Env_FetchObjFieldSpec
                          (ObjNum,         FieldTkns,     SubLevel,
                           McStat,         FieldSize,    &AttrId,
                          &AttrType,      &RefObjNum,    &FieldType,
                           Offset,        &BaseOffset))
        AttrFound    = True;
  }

  if (!AttrFound)
   *Status = Env_AttrNotInDbmsClassDef;

TRACK(TrackBak,"EnvFetchFieldOffset\n");
return (STAT);
}
 

boolean    Env_FetchObjFieldSpec
                        (numtype       ObjNum,      fieldspec     FieldTkns[],
                         numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         numtype      *AttrId,      attrtype     *AttrType,    
                         numtype      *RefObjNum,   typetype     *FieldType,
                         sizetype     *Offset,      sizetype     *BaseOffset)
{ 
numtype       Level       = 0;
handletype    ObjDescHndl = NullHandle;
tokentype     ObjDescTkn;
objdesctype  *ObjDesc     = NullPtr;
tokentype     VfieldsTkn;
numtype      *Vfields     = NullPtr;
tokentype     VoffsetsTkn;
typeunsize   *Voffsets    = NullPtr;
indextype     Vindex      = 0;
indextype     FieldCount  = 0;
booleantype   AttrFound   = False;
tokentype     AttrToken   = NullToken;
attrdesctype *AttrDesc    = NullPtr;
int           i           = 0;
tokentype     BldDescTkn  = NullToken;
objdesctype  *BuildDesc   = NullPtr;

  BldDescTkn.DbId    = RenvId;
  AttrToken.DbId     = RenvId;
  ObjDescTkn.DbId    = RenvId;

 *FieldSize          = 0;
 *AttrId             = 0;
 *AttrType           = aAttr;
 *RefObjNum          = 0;
 *FieldType          = aString;
 *Offset             = 0;
 *BaseOffset         = 0;

  Level     = 0;

  while (Normal(*Status)
  &&     Level < SubLevel)
  {
     if (MemGetHandle    (CoreDb0->ObjHandleDir,            ObjNum,
                          McStat,                          &ObjDescHndl))
     if (HmGetObjectAddr (CoreDb0,                          ObjDescHndl,
                          McStat,              (addrtype *)&ObjDesc))
     if (!ObjDesc)
     {
       ObjDescTkn.Handle = ObjDescHndl;
       OaGetObject     (&ObjDescTkn,    McStat, (addrtype *)&ObjDesc);
     }

     if (Normal(*Status))
     {
        AttrFound           = False;
        VoffsetsTkn.DbId    = RenvId;
        VoffsetsTkn.Handle  = ObjDesc->ClassOffsets;

        VfieldsTkn.DbId     = RenvId;
        VfieldsTkn.Handle   = ObjDesc->Fields;

        Vindex              = 0;
     }
     if (Normal(*Status))
     if (OaGetVchunkAddr (&VoffsetsTkn,   McStat,  (addrtype *)&Voffsets))
     if (OaGetVchunkAddr (&VfieldsTkn,    McStat,  (addrtype *)&Vfields))
     if (OaGetVchunkStackPtr
                         (&VfieldsTkn,    McStat,              &FieldCount))
     while (Normal(*Status)
     &&     Vindex < FieldCount
     &&    !AttrFound)
     {
        if (Vfields[Vindex] == FieldTkns[Level].AttrId)
        {
           if (Voffsets[Vindex] < 0)
             *Status         = Env_AttrNotInAplClassDef;
           else {
              AttrFound      = True;
             *Offset        += Voffsets[Vindex];
           }

        } else
         ++Vindex;
     }
     if (AttrFound          == False)
        *Status = Env_AttrNotInDbmsClassDef;

     if (Level              == 0)
        *BaseOffset          = *Offset;
     else
        *Offset     += (ObjDesc->ObjSize * FieldTkns[Level-1].Index);

     if (Normal(*Status))
     {
        /* Current Field Attr must be of type MapRefAttr                  */
        /* (to allow nested Field specification).                         */

        if (EnvFetchAttrHandle
                          (FieldTkns[Level].AttrId,
                           McStat,                  &AttrToken.Handle))

        if (OaGetObject   (&AttrToken,   McStat,   (addrtype *)&AttrDesc))
        {
          *FieldSize         = AttrDesc->UnitSize;
          *AttrId            = AttrDesc->AttrNum;
          *AttrType          = AttrDesc->AttrType;
          *FieldType         = AttrDesc->TypeType;

           if ((BldDescTkn.Handle = AttrDesc->BuildDesc) > 0)
           if (OaGetObject    (&BldDescTkn, 
                               McStat,             (addrtype *)&BuildDesc))
             *RefObjNum      = BuildDesc->ObjNum;
        }

      ++Level;
        if (Normal(*Status))
        if (Level               < SubLevel)
        if (AttrDesc->AttrType != aMapRefAttr)
        {
           sprintf  (Msg,
               "*** ERRor...@ Level =%4u; AttrId=%4u Not a Nested Object\n",
                     Level, FieldTkns[Level].AttrId);
           TraceMsg (0, Msg);         

          *Status      = Env_AttrNotInDbmsClassDef;
        } else {
           ObjNum      = *RefObjNum;
        }

        if (Normal(*Status))
        if (Level              == SubLevel)
        if (*RefObjNum)
        {
          *FieldSize   = BuildDesc->ObjSize;
          *Offset     += (*FieldSize * FieldTkns[Level-1].Index);
        }
     }
  }

  if (!Normal(*Status))
  {
     sprintf  (Msg, " Env_FetchObjFieldSpec:: ObjNum=%4u\n",
               ObjNum);
     TraceMsg (0, Msg);         
     for (i=0; i < SubLevel; i++) 
     {
        sprintf  (Msg, " Field %4u. AttrId=%4u; Index=%4u.\n",
                  i, FieldTkns[i].AttrId,  FieldTkns[i].Index);
        TraceMsg (0, Msg);         
     }
     sprintf  (Msg,"         @ Level =%4u; AttrId=%4u, Index=%4u\n",
               Level, FieldTkns[Level].AttrId, FieldTkns[Level].Index);
     TraceMsg (0, Msg);         

  }

TRACK(TrackBak,"Env_FetchObjFieldSpec\n");
return (STAT);
}
 

boolean    EnvFetchOffsetMap  (tokentype    *ObjTkn,    objheader *ObjHdr,  
                               ft F,lt Z,zz *Status,    typesize  *MapSize,
                               numtype      *OffsetMap, indextype *Inset)
{ 
handletype    ObjDescHndl = NullHandle;
objdesctype  *ObjDesc     = NullPtr;
tokentype     ObjDescTkn;
indextype     Index       = 0;
numtype       ShellType   = 0;
tokentype     VoffsetsTkn;
vchunktkntype VchunkTkn;
indextype     FieldCount  = 0;
typesize     *Voffsets    = NullPtr;
dbheader   *CoreDb  = NullPtr;

/* ...........................  NOTE .................................

   THIS IS NOT IMPLEMENTED CORRECTLY   (SEE EnvFetchAttrOffset).
   HOWEVER NO ONE CALLS THIS PROCEDURE..  YET!

   ............................ NOTE ................................. */


  VoffsetsTkn.DbId    =  RenvId;
  ObjDescTkn.DbId     =  RenvId;

  if (TmFetchCoreDb   (ObjTkn,  McStat,  &CoreDb))
/* This should be HmGetObjSchellType                                   */
  if (HmGetObjectMemLoc
                      (CoreDb,                              ObjTkn->Handle,
                       McStat,                             &ShellType))
/* This should be HmGetObjSchellType                                   */

  if (MemGetHandle    (CoreDb0->ObjHandleDir,               ObjHdr->ObjNum,
                       McStat,                             &ObjDescHndl))
  {
     ObjDescTkn.Handle  = ObjDescHndl;
     OmGetObject      (&ObjDescTkn,         
                       McStat,       &Index,   (addrtype *)&ObjDesc);
  }

  if (Normal(*Status))
  if (ObjDesc->ClassOffsets
  &&  Global_Apl == Cpp_Apl)
  {
     *Inset      = 0;
      VoffsetsTkn.Handle  =  ObjDesc->ClassOffsets;
      if (DbmGetVchunkTkn  (&VoffsetsTkn,     McStat,            &VchunkTkn))
      if (MemGetChunkAddr  (VchunkTkn.InCore, McStat,(addrtype *)&Voffsets))
      if (MemGetStackPtr   (VchunkTkn.InCore, McStat,            &FieldCount))
      {
         *MapSize    =  Voffsets[FieldCount-1];
         *OffsetMap  =  VchunkTkn.InCore;
      }


  } else {
     *Inset      = 1;
     *MapSize    =  ObjDesc->ObjSize;
     *OffsetMap  =  ObjHdr->FieldOffsetDir;
  }

TRACK(TrackBak,"EnvFetchOffsetMap\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
boolean    EnvInitCodes (ft F,lt Z,zz *Status )
{
tokentype      ArgObjDescTkn;
numtype        ArgDescObjNum       = 0;
tokentype      CodeArgsSetDescTkn;
    if (!CodeArgs_Hndl)
    if (sprintf (Msg, " size of codedesc      :: %3u\n", 
                 sizeof(codedesctype)))
    if (SendMsg (0, Msg))

    if (KernelFindIn  (RenvDescs_Hndl, &RenvTkn,       "ArgDesc",
                       McStat,         &ArgObjDescTkn)) 
    if (KernelGetAttr (&ArgObjDescTkn,  ObjNum_AttrNum,
                       McStat,         &ArgDescObjNum))
    if (KernelFindIn  (RenvDescs_Hndl, &RenvTkn,       "CodeArgs",
                       McStat,         &CodeArgsSetDescTkn))
    {
       if (ArgDesc_Hndl != ArgObjDescTkn.Handle)
           *Status = Env0_Corrupted;
       CodeArgs_Hndl = CodeArgsSetDescTkn.Handle;
       SendMsg        (0, " CODES  loaded ...\n");
    } else {
       TraceMsg (0, "CodeDesc :: not a Descriptor in Renv\n" );
      *Status = Code_NotAnObjDesc;
    }

TRACK(TrackBak,"EnvInitCodes\n");
return(STAT);
}

boolean    EnvMakeCodeChunks  (ft F,lt Z,zz *Status)
{ 
   if (MemMakeChunk (sizeof (funcptrtype),   Env0->CodeCount+1,   
                     Extend_Codes,
                     RenvId,                 McStat,       &CodeDirChunk))
       MemPutStackPtr
                    (CodeDirChunk,           Env0->CodeCount,   McStat);


TRACK(TrackBak,"EnvMakeCodeChunks\n");
return (STAT);
}

boolean    EnvPairCode        (numtype       CodeNum, funcptrtype  CodeAddr,
                               ft F,lt Z,zz *Status)
{ 
  MemPutFuncPtr (CodeDirChunk,  CodeNum,  CodeAddr,  McStat);

TRACK(TrackBak,"EnvPairCode\n");
return (STAT);
}

boolean    EnvFetchCode       (numtype       CodeNum,
                               ft F,lt Z,zz *Status,  funcptrtype *CodeAddr)
{ 
  MemGetFuncPtr (CodeDirChunk, CodeNum,  McStat,  CodeAddr);


TRACK(TrackBak,"EnvFetchCode\n");
return (STAT);
}
 
void    EnvAlignMember    (attrtype      Attr_Type, typetype      Type_Type,
                           sizetype      Size,      sizetype     *Offset)
{ /* Offset    :: Current Size of Struct... [Returns with Aligned Position] 
     Type      :: Type of New Member
     Size      :: Size of New Member
     ByteAlign :: Byte Alignment                                           */

sizetype  ByteAlign  = 0;
sizetype  Align      = 0;

 if (DeBug || ClassBug)
 if (sprintf (Msg,  "    Type<%12s> Size(%3u)     @%4u", 
              TypeTypeNam[Type_Type], Size, *Offset))
    TraceMsg (0, Msg);

 if (*Offset)
 {
   switch (Attr_Type)
   {
     case  aObjRefAttr :
        if ((Align = sizeof (tokentype)) > sizeof (addrtype))
        {
           ByteAlign = Byte4Alignment;
        } else 
           ByteAlign = Byte4Alignment;
        break;

     case  aArrayAttr  :
     case  aDblPtrAttr :
     case  aMapRefAttr :
     case  aVarrayAttr :
     case  aChunkAttr  :
     case  aVchunkAttr :
        ByteAlign = Byte4Alignment;
        break;

     default           :
     switch (Type_Type)
     {
        case aString    :
                  ByteAlign = Byte_Alignment;
                  break;

        case aLink      :
#ifdef LARGE
                  ByteAlign = Byte4Alignment;
#else
                  ByteAlign = Byte2Alignment;
#endif
                  break;

        case aInteger   :
                  if (Size == 1)
                  {
                     ByteAlign = Byte_Alignment;
 
                  } else if (Size == 2) {
                     ByteAlign = Byte2Alignment;

                  } else if (Size == 4) {
                     ByteAlign = Byte4Alignment;

                  } else { 
                     ByteAlign = Byte4Alignment;
                  }
                  break;

        case aReal       :
                  if (Size == sizeof (float)) 
                     ByteAlign = Byte4Alignment;

                  else if (Size == sizeof (double)) 
                     ByteAlign = Byte8Alignment;
                  break;

        case aEnum        :
        case aBoolean     :
                  ByteAlign = Byte4Alignment;
                  break;

        case aChunk       :
                  ByteAlign = Byte2Alignment;
                  break;

        case aAddr        :
                  ByteAlign = BytePtrAlignment;
                  break;

        case aRefObj      :
        case aDbmsString  :
        case aValue       :
                  ByteAlign = Byte4Alignment;
                  break;
     }
     break;
   }
   Align       = *Offset   %  ByteAlign;

   if (Align)
        Align  = ByteAlign - (*Offset %  ByteAlign);

  *Offset     +=  Align;

 }
 if (DeBug || ClassBug)
 if (sprintf (Msg, "  for ByteAlign_%u;  Align=%4u\n",
              ByteAlign, Align))
 if (TraceMsg (0, Msg))
 if (sprintf (Msg,  "                     Adjusted  Offset=%4u\n", 
              *Offset))
     TraceMsg (0, Msg);

return;
}

void    EnvAlignStruc     (boolean       DblAlign, boolean       AddrAlign,
                           sizetype     *Offset)
{ /* Offset    :: Current Size of Struct 
     ByteAlign :: Byte Alignment for structs [compiler specific]           */

#ifdef STRUC_ALIGN_BY64
sizetype      Struc_Alignment  = Byte4Alignment;     
#else
sizetype      Struc_Alignment  = Byte4Alignment;     
#endif /* STRUC_ALIGN_BY64  */

sizetype  ByteAlign = 0;

     if (DblAlign)
     {
        ByteAlign  = *Offset %  Byte8Alignment;
        if (ByteAlign)
           ByteAlign  = Byte8Alignment - ByteAlign;

     } else if (AddrAlign) {
        ByteAlign  = *Offset %  BytePtrAlignment;
        if (ByteAlign)
           ByteAlign  = BytePtrAlignment - ByteAlign;

     } else {
        ByteAlign  = *Offset %  Struc_Alignment;
        if (ByteAlign)
           ByteAlign  = Struc_Alignment - ByteAlign;
     }

    *Offset    +=  ByteAlign;

     if (DeBug || ClassBug)
     if (sprintf (Msg, "  StrucAlignment = %2u\n", ByteAlign))
         TraceMsg (0, Msg);

return;
}

/*+-----------------------------------------------------------------------+
~ED                         ENVDESC                                       !
  +-----------------------------------------------------------------------+*/
boolean    TestEnv0      (ft F,lt Z,zz *Status)
{
/*  *** End Test ***  */
 TRACK(TrackBak,"TestEnv0\n");
return(STAT);
}
 
numtype     ObjCount        = 0;
 

boolean    CreateObjDesc (tokentype    *Anchor,  ft F,lt Z,zz *Status,
                          objdesctype **ObjDesc, tokentype    *Token)
{  /* allocate space for An ObjectDesc  */
numtype    ObjNum   = 0;
indextype  StackPtr = 0;
indextype  ObjIndex = 0;
time_t     Tod      = 0;

  time (&Tod);
  if (OaCreateObject   (Anchor,  ObjDesc_Class,  McStat,    Token))
  if (OaGetObject      (Token,   McStat,        (addrtype *)ObjDesc))
  if (ObjCount) 
  {
     if (MemPushObjNum    (CoreDb0->ObjNumMap,   ObjNum,  McStat, &StackPtr))
         MemPushObjHdr    (CoreDb0->ObjHdrDir,   NullPtr, McStat, &StackPtr);

  } else {
      MemGetStackPtr      (CoreDb0->ObjNumMap,            McStat, &StackPtr);
      StackPtr--;
  }

  if (Normal(*Status))
  if (++Env0->ObjCount
   && StackPtr        == ++ObjCount) 
  {
     ObjIndex = ObjNum = (*ObjDesc)->ObjNum  = ObjCount;
     if (MemPutLocalObjNum (CoreDb0->LocalObjNumMap, ObjIndex, ObjNum,
                            McStat))
     /* Stamp The Creation Time for this Obj  Type   in RENV              */
         MemPutLong        (CoreDb0->ObjTimeStampDir,   ObjIndex,
                            (longwordtype )Tod,         McStat); 


     if (DeBug)
     if (sprintf (Msg, " Obj Class (%2d) @%x\n", (*ObjDesc)->ObjNum, *ObjDesc))
        TraceMsg (1, Msg);
  } else 
     *Status = Env01_BadObjNumMap;
 
TRACK(TrackBak,"CreateObjDesc\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF ENV1.C                                  !
  +-----------------------------------------------------------------------+*/
