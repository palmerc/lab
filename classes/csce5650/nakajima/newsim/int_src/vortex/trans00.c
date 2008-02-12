/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* TRANS00.c */
#define  TRANS00
#include "defines.h"
#include "typedefs.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "oa0.h"
#include "voa.h"
#include "vchunk.h"
#include "vom.h"
#include "env1.h"
#include "oa1.h"
#include "trans.h"
 
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        Image01_ObjNum       (objnumtype )142

#define        That_AttrNum           (numtype )1
#define        Token_Attr             (numtype )2

#define        Handle_AttrNum         (numtype )2
#define        DbId_AttrNum           (numtype )3
#define        CoreDbId_AttrNum       (numtype )4

#define        AttrNum_AttrNum        (numtype )9

#define        RenvAttrs_Hndl         (handletype )677

#endif

#define MemMakeChunk       Mem_MakeChunk 
#define MemPutStackPtr     Mem_PutStackPtr
#define MemGetStackPtr     Mem_GetStackPtr
#define MemGetChunkAddr    Mem_GetChunkAddr
#define MemPutAddr         Mem_PutAddr
#define MemGetAddr         Mem_GetAddr
#define MemPushSomeBytes   Mem_PushSomeBytes
#define MemGetSomeBytes    Mem_GetSomeBytes
#define MemPushFieldNum    Mem_PushWord
#define MemGetFieldNum     Mem_GetWord
#define MemGetObjNum       Mem_GetWord
#define MemGetLocalObjNum  Mem_GetWord

extern boolean KernelFindIn (handletype    SetDesc,  tokentype    *OwnerTkn,
                             addrtype      KeyValue,
                             ft F,lt Z,zz *Status,   tokentype    *MemberTkn);
extern boolean KernelPutAttr(tokentype    *Token,     numtype    EnvAttrNum,
                             addrtype      AttrValue, ft F,lt Z,zz *Status);
extern boolean KernelGetAttr(tokentype    *Token,     numtype    EnvAttrNum,
                             ft F,lt Z,zz *Status,    addrtype   Value);

static boolean   ImageMapInvoked       = False;
/*+-----------------------------------------------------------------------+*
~C                        TRANSFER MANAGER
 *+-----------------------------------------------------------------------+*/
boolean TransInitMap      (numtype       ClassId, size_t        ObjSize,
                           numtype       AplId,   ft F,lt Z,zz *Status)
{
fieldtoken     FieldTkn;
indextype      StackPtr         = 0;
tokentype      ObjToken         = NullToken;
objdesctype   *ObjDesc          = NullPtr;
typesize       ClassSize        = 0;
indextype      FieldCount       = 0;
tokentype      VfieldsTkn       = NullToken;
objnumtype    *Vfields          = NullPtr;
boolean        ClassMapExists   = False;
imagemaptype   ImageMap;
maparraystype *MapArraysAddr    = NullPtr;

   ImageMapInvoked   = False;

   VfieldsTkn.DbId   = RenvId;
   ObjToken.DbId     = RenvId;
   FieldTkn.Index    = ClassId;
   FieldTkn.Offset   = ObjSize;

   if (Normal(*Status))
   if (C_ObjMapDir   == 0
   ||  C_ImageMapDir == 0)
       TransCreateMapDirs(AplId,  McStat,  &C_ObjMapDir,   &C_ImageMapDir);

   if (Normal(*Status))
       MemGetAddr        (C_ObjMapDir,      ClassId,
                          McStat,              (addrtype *)&MapArraysAddr);

   if (MapArraysAddr)
   {
     if (ClassBug)
     TraceMsg (0,
      " Object Class for C_Image already loaded!.\n");

     return(False);
   }

   if (Normal(*Status))
   {
      if (EnvFetchObjHandle (ClassId,       McStat,  &ObjToken.Handle))
      if (EnvFetchObjSize   (ClassId,       McStat,  &ObjSize))
      if (ClassBug || OaBug || DeBug)
      if (sprintf (Msg, "  C_TransInvokeMap :: ObjNum= %2u; ClassSize= %3u;\n",
                   ClassId, ClassSize))
          TraceMsg (0, Msg);

      if (Normal(*Status))
      if (OaGetObject         (&ObjToken,      McStat, (addrtype *)&ObjDesc))
          VfieldsTkn.Handle  = ObjDesc->Fields;

      if (Normal(*Status))
      if (OaGetVchunkAddr     (&VfieldsTkn,    McStat, (addrtype *)&Vfields))
          OaGetVchunkStackPtr (&VfieldsTkn,    McStat,           &FieldCount);

   }

   if (Normal(*Status))
   if (MapArraysAddr == NullPtr)
   {  
      TransNewImageMap     (C_ObjMapDir,     C_ImageMapDir,   ClassId,  
                            FieldCount,      McStat,         
                           &ClassMapExists, &ImageMap,       &MapArraysAddr);
      ClassMapExists      = False;

   } else {
      TransNewImageMap     (C_ObjMapDir,     C_ImageMapDir,   ClassId,  
                            FieldCount,      McStat,         
                           &ClassMapExists, &ImageMap,       &MapArraysAddr);
      if (ClassMapExists     !=  True)
      {
        TraceMsg (0, "\n ***ERROR*** ImageMap should already exist\n");
       *Status   = Err_BadDesign;
      }
   }

   if (Normal(*Status))
   if (ClassMapExists)
   {
     ImageMapInvoked = True;
     TraceMsg (0, " Object Class for C_Image Loaded from Disk!\n");
     return(STAT);
   }

   if (Normal(*Status))
   if (ObjClassMap  == 0)
       MemMakeChunk     (sizeof (fieldtoken),    50,            10,
                         RenvId,                 McStat,       &ObjClassMap);

   /* Clear the chunk to build a new translation map                       */
   if (Normal(*Status))
   if (MemPutStackPtr   (ObjClassMap,            0,             McStat))
   if (MemPushSomeBytes (ObjClassMap,            sizeof (fieldtoken),
                         (addrtype )&FieldTkn,   McStat,       &StackPtr))


   if (EnvFetchObjHandle (ClassId,     McStat,    &ObjToken.Handle))
   if (OaGetObject       (&ObjToken,   McStat,    (addrtype *)&ObjDesc))
       ObjDesc->PsudoSize   = ObjSize;

TRACK(TrackBak,"TransInitClassMap\n");
return (STAT);
}


boolean TransAppendToMap  (numtype       AttrId,  size_t        Offset,
                           numtype       AplId,   ft F,lt Z,zz *Status)
{
fieldtoken  FieldTkn;
indextype   StackPtr  = 0;

   if (ImageMapInvoked == True)
      return(STAT);

   FieldTkn.Index      = AttrId;
   FieldTkn.Offset     = Offset;

   /* split the Token attribute into its member parts                      */
   if (AttrId         == Token_Attr) 
   {
      FieldTkn.Index   = Handle_AttrNum;
      FieldTkn.Offset  = Offset;
      MemPushSomeBytes     (ObjClassMap,            sizeof (fieldtoken),
                            (addrtype )&FieldTkn,   McStat,       &StackPtr);

      Offset          += sizeof (handletype);
      FieldTkn.Index   = DbId_AttrNum;
      FieldTkn.Offset  = Offset;
      if (Normal(*Status))
          MemPushSomeBytes (ObjClassMap,            sizeof (fieldtoken),
                            (addrtype )&FieldTkn,   McStat,       &StackPtr);

      Offset          += sizeof (idtype);
      FieldTkn.Index   = CoreDbId_AttrNum;
      FieldTkn.Offset  = Offset;
   } 

   /* Push the attribute Id and position onto the stack                    */
   if (Normal(*Status))
       MemPushSomeBytes    (ObjClassMap,            sizeof (fieldtoken),
                            (addrtype )&FieldTkn,   McStat,       &StackPtr);

TRACK(TrackBak,"TransAppendToMap\n");
return (STAT);
}


boolean    TransInvokeMap  (numtype       AplId,       ft F,lt Z,zz *Status)
{
tokentype      ObjToken;
handletype     MapAttrCount     = 0;
fieldtoken    *FieldTknMap      = NullPtr;
numtype        AttrNum          = 0;
indextype      AttrCount        = 0;
typeunsize     Offset           = 0;
numtype        ObjNum           = 0;
typesize       ClassSize        = 0;
int            Adjust           = 0;
int            Increment        = 0;
objdesctype   *ObjDesc          = NullPtr;
tokentype      VclasssTkn;
tokentype      VoffsetsTkn;
tokentype      VfieldsTkn;
indextype      FieldCount       = 0;
booleantype    AttrFound        = False;
objnumtype    *Vclasss          = NullPtr;
typeunsize    *Voffsets         = NullPtr;
objnumtype    *Vfields          = NullPtr;
indextype      Vindex           = 0;
tokentype      AttrTkn;
attrdesctype  *EnvAttr          = NullPtr;
tokentype      TypeTkn          = NullToken;
typedesctype  *EnvType          = NullPtr;
tokentype      BldDescTkn       = NullToken;
objdesctype   *BldDesc          = NullPtr;
int            i                = -1;
indextype      StackPtr         = 0;
boolean        FillOffsets      = False;
fieldtoken     FieldTkn;
typeunsize     Null_Offset      = -1;

 if (ImageMapInvoked == True)
     return(STAT);

 ObjToken.DbId         = RenvId;
 VclasssTkn.DbId       = RenvId;
 VoffsetsTkn.DbId      = RenvId;
 VfieldsTkn.DbId       = RenvId;
 AttrTkn.DbId          = RenvId;
 TypeTkn.DbId          = RenvId;
 BldDescTkn.DbId       = RenvId;


 if (MemGetStackPtr      (ObjClassMap,    McStat,              &MapAttrCount))
 if (MemGetChunkAddr     (ObjClassMap,    McStat,  (addrtype *)&FieldTknMap))
 if (MapAttrCount)
 {
    ObjNum      = FieldTknMap[0].Index;
    ClassSize   = FieldTknMap[0].Offset;

    EnvFetchObjHandle (ObjNum,  McStat,  &ObjToken.Handle);

    if (ClassBug || OaBug || DeBug)
    if (sprintf (Msg, "  TransInvokeMap :: ObjNum= %2u; ClassSize= %3u;\n",
                 ObjNum, ClassSize))
    if (TraceMsg (0, Msg))
    if (OaBug || DeBug)
       OaDumpObject (0, &ObjToken,  McStat);

    if (MapAttrCount == 1)
    if (MemPutStackPtr   (ObjClassMap,    FieldCount+2, McStat))
    if (MemPutStackPtr   (ObjClassMap,    1,            McStat))
    if (MemGetChunkAddr  (ObjClassMap,    McStat,  (addrtype *)&FieldTknMap))
       FillOffsets    = True;

 } else
   *Status = Trans_NullMap;

 if (Normal(*Status))
 if (OaGetObject (&ObjToken,   McStat,     (addrtype *)&ObjDesc))
 {
    VclasssTkn.Handle   = ObjDesc->ClassFields;
    VoffsetsTkn.Handle  = ObjDesc->ClassOffsets;
    VfieldsTkn.Handle   = ObjDesc->Fields;
 }

 if (Normal(*Status))
 if (OaGetVchunkAddr     (&VfieldsTkn,    McStat,  (addrtype *)&Vfields))
 if (OaGetVchunkStackPtr (&VfieldsTkn,    McStat,              &FieldCount))

 if (OaPutVchunkStackPtr (&VclasssTkn,    FieldCount,           McStat))
 if (OaPutVchunkStackPtr (&VoffsetsTkn,   FieldCount+1,         McStat))

 if (OaGetVchunkAddr     (&VclasssTkn,    McStat,  (addrtype *)&Vclasss))
     OaGetVchunkAddr     (&VoffsetsTkn,   McStat,  (addrtype *)&Voffsets);

 if (Normal(*Status))
 {
    Offset   = 0;
    Adjust   = 0;
    i = 0;
    while (Normal(*Status)
    &&     i < FieldCount)
    {
       if (FillOffsets)
       {
       ++MapAttrCount;
         FieldTkn.Index        = Vclasss[i];

         if (EnvFetchAttrHandle (Vclasss[i], McStat,  &AttrTkn.Handle))
         if (OaGetObject        (&AttrTkn,   McStat,  (addrtype *)&EnvAttr))
             TypeTkn.Handle    = EnvAttr->TypeObj;

         if (Normal(*Status))
         if (OaGetObject        (&TypeTkn,   McStat,  (addrtype *)&EnvType))
         {
	     if ( EnvType->TypeType == aAddr )
                EnvType->Size = sizeof( addrtype ) ;
             EnvAlignMember     (EnvAttr->AttrType,     EnvType->TypeType,
                                 EnvType->Size, (sizetype *)&Offset);
             FieldTkn.Offset   = Offset;
             if (EnvAttr->AttrType  == aVarrayAttr
             ||  EnvAttr->AttrType  == aArrayAttr   
             ||  EnvAttr->AttrType  == aDblPtrAttr
             ||  EnvAttr->AttrType  == aChunkAttr   
             ||  EnvAttr->AttrType  == aStrHndlAttr   
             ||  EnvAttr->AttrType  == aObjRefAttr
             ||  EnvAttr->AttrType  == aTknRefAttr)
             {
               Ut_AlignMember   (aAddr,   sizeof (addrtype), &FieldTkn.Offset);
               Offset =  FieldTkn.Offset;
             }
             MemPushSomeBytes   (ObjClassMap,            sizeof (fieldtoken),
                                 (addrtype )&FieldTkn,   McStat,  &StackPtr);
         }

         if (Normal(*Status))
         {
            switch (EnvAttr->AttrType)
            {
               case aObjRefAttr  :
               case aDblPtrAttr  :
               case aArrayAttr   :
                  Increment  = sizeof  (addrtype)   *  EnvAttr->UnitSize;
                  break;

               case aGrpAttr     :
               case aVgrpAttr    :
                  Increment  = sizeof  (tokentype)  *  EnvAttr->UnitSize;
                  break;

               case aTknRefAttr  :
                  Increment  = sizeof  (RefObj)     *  EnvAttr->UnitSize;
                  break;

               case aChunkAttr  : 
                  Increment  = sizeof  (vchunktype) *  EnvAttr->UnitSize;
                  break;

               case aVarrayAttr  :
                  Increment  = sizeof  (varraytype) *  EnvAttr->UnitSize;
                  break;

               case aMapRefAttr  :
                  BldDescTkn.Handle  = EnvAttr->BuildDesc;
                  if (OaGetObject    (&BldDescTkn,  
                                      McStat,       (addrtype *)&BldDesc))
                     Increment = BldDesc->ObjSize   *  EnvAttr->UnitSize;
                  break;

               case aStrHndlAttr :
                  Increment    = sizeof (addrtype)  *  EnvAttr->UnitSize;
                  break;

               default           :
                  Increment    = EnvType->Size      *  EnvAttr->UnitSize;
                  break;
            }
            Offset    += Increment;
            Adjust    += Increment;

         }

       }
       i++;
    }
 }


 if (Normal(*Status))
    ObjDesc->PsudoSize   = ClassSize;

TRACK(TrackBak,"TransInvokeMap\n");
return(STAT);
}



boolean    TransBuildMap  (numtype       AplId,       ft F,lt Z,zz *Status)
{

tokentype      ObjToken         = NullToken;
objdesctype   *ObjDesc          = NullPtr;
handletype     MapAttrCount     = 0;
fieldtoken    *FieldTknMap      = NullPtr;
numtype        ObjNum           = 0;
typesize       ClassSize        = 0;
tokentype      VfieldsTkn       = NullToken;
objnumtype    *Vfields          = NullPtr;
indextype      FieldCount       = 0;
typesize       ObjSize          = 0;
boolean        ClassMapExists   = False;
boolean        ClassImageExists = False;
imagemaptype   ImageMap;
maparraystype *MapArraysAddr    = NullPtr;

 if (ImageMapInvoked == True)
 {
    ImageMapInvoked = False;
    return(STAT);
 }
 OverRideInitClass     = True;

 ObjToken.DbId         = RenvId;
 VfieldsTkn.DbId       = RenvId;

 if (Normal(*Status))
 if (MemGetStackPtr      (ObjClassMap,  McStat,              &MapAttrCount))
 if (MemGetChunkAddr     (ObjClassMap,  McStat,  (addrtype *)&FieldTknMap))
 if (MapAttrCount)
 {
    ObjNum      = FieldTknMap[0].Index;
    ClassSize   = FieldTknMap[0].Offset;

 } else
   *Status = Trans_NullMap;

 if (Normal(*Status))
 if (C_ObjMapDir   == 0
 ||  C_ImageMapDir == 0)
     TransCreateMapDirs(AplId,  McStat,  &C_ObjMapDir,   &C_ImageMapDir);

 if (Normal(*Status))
     MemGetAddr        (C_ObjMapDir,      ObjNum,
                        McStat,              (addrtype *)&MapArraysAddr);

 if (MapArraysAddr)
 if (!OverRideInitClass)
   return(STAT);


 if (Normal(*Status))
 {
    if (EnvFetchObjHandle (ObjNum,        McStat,  &ObjToken.Handle))
    if (EnvFetchObjSize   (ObjNum,        McStat,  &ObjSize))
    if (OaGetObject         (&ObjToken,      McStat,  (addrtype *)&ObjDesc))
        VfieldsTkn.Handle  = ObjDesc->Fields;

    if (Normal(*Status))
    if (OaGetVchunkAddr     (&VfieldsTkn,    McStat,  (addrtype *)&Vfields))
        OaGetVchunkStackPtr (&VfieldsTkn,    McStat,              &FieldCount);

 }

 /* Load/Create the ImageMap for this Obj Class translation               */
 if (Normal(*Status))
 if (MapArraysAddr == NullPtr)
 {
    TransNewImageMap       (C_ObjMapDir,     C_ImageMapDir,   ObjNum,  
                            FieldCount,      McStat,         
                           &ClassMapExists, &ImageMap,       &MapArraysAddr);
    ClassMapExists        = False;

 } else {
    TransNewImageMap       (C_ObjMapDir,     C_ImageMapDir,   ObjNum,  
                            FieldCount,      McStat,         
                           &ClassMapExists, &ImageMap,       &MapArraysAddr);
    if (ClassMapExists     !=  True)
    {
      TraceMsg (0, "\n ***ERROR*** ImageMap should already exist\n");
     *Status   = Err_BadDesign;
    }
 }

 if (Normal(*Status))
 if (ClassMapExists)
 if (!OverRideInitClass)
 {
   TraceMsg (0, " Object Class for C_Image Loaded from Disk!\n");
   return(STAT);

 }

 if (Normal(*Status))
 {
   MapArraysAddr->CppOffsets[0]    = (typesize )ClassSize;
   if (ClassSize != ObjSize)
       MapArraysAddr->DbOffsets[0] = False;
   else
       MapArraysAddr->DbOffsets[0] = True;
 }

 if (Normal(*Status))
     TransBuildFields  (MapAttrCount,  FieldTknMap, Vfields,       FieldCount,
                        ObjDesc->Nam, &ImageMap,    MapArraysAddr, McStat);

TRACK(TrackBak,"TransBuildMap\n");
return(STAT);
}


boolean TransGetMap     (numtype       ObjNum,    numtype       AplId,
                         ft F,lt Z,zz *Status,    classmaptype *ClassMap)
{
addrtype   *FieldMap       = NullPtr;
typesize    ObjSize        = 0;

  if (Normal(*Status))
  if (MemGetAddr        (C_ObjMapDir,                          ObjNum,
                         McStat,                  (addrtype *)&FieldMap))
  if (EnvFetchObjSize   (ObjNum,        McStat,               &ObjSize)) 
  if (FieldMap)
  {
     ClassMap->DbOffsets      =  (typesize *)FieldMap[0];
     ClassMap->CppOffsets     =  (typesize *)FieldMap[1];
     ClassMap->FieldSizes     =  (typesize *)FieldMap[2];
     ClassMap->FieldAttrTypes =  (attrtype *)FieldMap[3];
     ClassMap->FieldAttrIds   =  (numtype  *)FieldMap[4];
     ClassMap->RefObjNums     =  (numtype  *)FieldMap[5];
     ClassMap->ArrayAttrs     =  (numtype  *)FieldMap[6];
     ClassMap->FieldTypeTypes =  (typetype *)FieldMap[7];

     ClassMap->ObjNum         =  ObjNum;
     ClassMap->NumMapFields   =  (indextype )ClassMap->RefObjNums[0];
     ClassMap->NumArrayAttrs  =  (indextype )ClassMap->ArrayAttrs[0];
     ClassMap->ClassSize      =  ObjSize;
     ClassMap->ImageSize      =  (typesize  )ClassMap->CppOffsets[0];
     ClassMap->PureDbStruc    =  (boolean   )ClassMap->DbOffsets[0];
     ClassMap->ThatIndex      =  (typesize  )ClassMap->FieldTypeTypes[0];


  } else
    *Status = Trans_NullMap;


TRACK(TrackBak,"TransGetMap\n");
return(STAT);
}


boolean Trans_DumpMap   (numtype       ObjNum,    numtype       AplId,
                         ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"Trans_DumpMap\n");
return(STAT);
}


boolean Trans_MapIsActive
                        (numtype       ObjNum,      ft F,lt Z,zz *Status)
{
addrtype   *FieldMap       = NullPtr;

  if (Normal(*Status))
  if (MemGetAddr        (C_ObjMapDir,                          ObjNum,
                         McStat,                  (addrtype *)&FieldMap))
  if (FieldMap == NullPtr)
    return(False);

TRACK(TrackBak,"Trans_MapIsActive\n");
return(STAT);
}


/* .....................................................................  */

boolean    TransCreateMapDirs 
                        (numtype       AplId,       ft F,lt Z,zz *Status,
                         numtype      *ObjMapDir,   numtype      *ImageMapDir)
{
static char   *AttrName        = "C_ImageMapDir";
tokentype      AttrTkn         = NullToken;
vchunktkntype  VchunkToken;

 if (Normal(*Status))
 if (*ObjMapDir == 0)
 {
   if (MemMakeChunk    (sizeof (addrtype),      CoreDb0->LastEnvObjCount+1,
                        Extend_DbObjNums,       RenvId,
                        McStat,                 ObjMapDir))
       MemPutStackPtr  (*ObjMapDir,             CoreDb0->LastEnvObjCount+1,
                        McStat);
 }

 if (Normal(*Status))
 if (*ImageMapDir      == 0)
 {
    ImageMapTkn.DbId    = WenvId;
    if (!KernelFindIn    (RenvAttrs_Hndl,                &RenvTkn,   
                          (addrtype )AttrName,
                           McStat,          (tokentype *)&AttrTkn))
       TraceMsg (0, "  Error... Attr C_ImageMapDir_AttrNum Not in Renv\n");

    if (Normal(*Status))
    if (KernelGetAttr    (&AttrTkn,                    AttrNum_AttrNum,
                          McStat,  (addrtype   )&C_ImageMapDir_AttrNum))
    if (KernelGetAttr     (&WenvTkn,                    C_ImageMapDir_AttrNum, 
                           McStat,          (addrtype )&ImageMapTkn.Handle))

    if (ImageMapTkn.Handle == 0)
    if (OaCreateVchunk    (&WenvTkn,  sizeof (imagemaptype), 
                           CoreDb0->LastEnvObjCount + 1 - Image01_ObjNum,
                           100,        McStat,         &ImageMapTkn))
    if (OaPutVchunkStackPtr
                          (&ImageMapTkn, 
                           CoreDb0->LastEnvObjCount + 1 - Image01_ObjNum,
                           McStat))
    if (KernelPutAttr     (&WenvTkn,                    C_ImageMapDir_AttrNum,
                           (addrtype )&ImageMapTkn.Handle,   McStat))
    if (ClassBug || OaBug || DeBug)
       TraceMsg (0, "  ImageMap Vchunk Created;\n");


    if (Normal(*Status))
    if (OmGetVchunkToken   (&ImageMapTkn, McStat, &VchunkToken))
       *ImageMapDir   = VchunkToken.InCore;

 }

TRACK(TrackBak,"TransCreateMapDirs\n");
return(STAT);
}


boolean    TransNewImageMap
                       (numtype       ObjMapDir,  numtype         ImageMapDir,
                        numtype       ObjNum,     numtype         FieldCount,
                        ft F,lt Z,zz *Status,     boolean        *MapExists,
                        imagemaptype *ImageMap,   maparraystype **FieldsDir)
{
numtype        FieldsDirChunk   = 0;
tokentype      DbOffsetTkn      = NullToken;
typesize      *DbOffsets        = NullPtr;
tokentype      CppOffsetTkn     = NullToken;
typesize      *CppOffsets       = NullPtr;
tokentype      FieldSizeTkn     = NullToken;
typesize      *FieldSizes       = NullPtr;
tokentype      FieldTypeTkn     = NullToken;
attrtype      *FieldTypes       = NullPtr;
tokentype      TypeTypeTkn      = NullToken;
typetype      *TypeTypes        = NullPtr;
tokentype      FieldAttrTkn     = NullToken;
numtype       *FieldAttrs       = NullPtr;      
tokentype      RefObjNumTkn     = NullToken;
numtype       *RefObjNums       = NullPtr;      
tokentype      ArrayAttrTkn     = NullToken;
numtype       *ArrayAttrs       = NullPtr;      

*MapExists    =  False;

 if (MemMakeChunk      (sizeof (addrtype), 8,
                        0,                 WenvId,
                        McStat,                              &FieldsDirChunk))
 if (MemPutStackPtr    (FieldsDirChunk,    8,                   McStat))
 if (MemGetChunkAddr   (FieldsDirChunk,    McStat,  (addrtype *)FieldsDir))
     MemPutAddr        (ObjMapDir,         ObjNum, *(addrtype *)FieldsDir,
                        McStat);

 if (Normal(*Status))
 if (MemGetSomeBytes   (ImageMapDir,                     V(ObjNum), 
                        sizeof(imagemaptype),
                        McStat,               (addrtype )ImageMap))
 if (ImageMap->DbOffsetsHndl)
 {
   *MapExists    =  True;

    if (OaGetUnit (&ImageMapTkn,  V(ObjNum), McStat,  (addrtype )ImageMap))
    {
        DbOffsetTkn.DbId    =  WenvId;  
        DbOffsetTkn.Handle  =  ImageMap->DbOffsetsHndl;

        CppOffsetTkn.DbId   =  WenvId;  
        CppOffsetTkn.Handle =  ImageMap->CppOffsetsHndl;

        FieldAttrTkn.DbId   =  WenvId;  
        FieldAttrTkn.Handle =  ImageMap->FieldAttrIdsHndl;   

        FieldTypeTkn.DbId   =  WenvId;  
        FieldTypeTkn.Handle =  ImageMap->FieldAttrTypesHndl;

        TypeTypeTkn.DbId    =  WenvId;  
        TypeTypeTkn.Handle  =  ImageMap->FieldTypeTypesHndl;

        FieldSizeTkn.DbId   =  WenvId;  
        FieldSizeTkn.Handle =  ImageMap->FieldSizesHndl;

        RefObjNumTkn.DbId   =  WenvId;  
        RefObjNumTkn.Handle =  ImageMap->FieldRefObjNumsHndl;  

        ArrayAttrTkn.DbId   =  WenvId;  
        ArrayAttrTkn.Handle =  ImageMap->ArrayAttrsHndl;
    }

 } else {
/* Create the Class Map Arrays                                             */

    if (OaBug || DeBug)
       TraceMsg (0, "  Create The ClassMapArrays\n");

    if (OaCreateVchunk    (&WenvTkn,   sizeof (typesize),
                           FieldCount + 30,    10,                 
                           McStat,                              &DbOffsetTkn))
    if (OaPutVchunkStackPtr 
                          (&DbOffsetTkn,       FieldCount + 29,  McStat))

    if (OaCreateVchunk    (&WenvTkn,   sizeof (typesize),
                           FieldCount + 30,    10, 
                           McStat,                              &CppOffsetTkn  ))
    if (OaPutVchunkStackPtr
                          (&CppOffsetTkn,      FieldCount + 29,  McStat))


    if (OaCreateVchunk    (&WenvTkn,   sizeof (typesize),
                           FieldCount + 30,    10, 
                           McStat,                              &FieldSizeTkn  ))    
    if (OaPutVchunkStackPtr
                          (&FieldSizeTkn,      FieldCount + 29,  McStat))

    if (OaCreateVchunk    (&WenvTkn,   sizeof (attrtype),
                           FieldCount + 30,    10, 
                           McStat,                              &FieldTypeTkn  ))
    if (OaPutVchunkStackPtr 
                          (&FieldTypeTkn  ,    FieldCount + 29,  McStat))

    if (OaCreateVchunk    (&WenvTkn,  sizeof(handletype),
                           FieldCount + 30,    10, 
                           McStat,                              &FieldAttrTkn  ))
    if (OaPutVchunkStackPtr
                          (&FieldAttrTkn,      FieldCount + 29,  McStat))

    if (OaCreateVchunk    (&WenvTkn,  sizeof(handletype),
                           FieldCount + 30,    10, 
                           McStat,                              &RefObjNumTkn  ))
    if (OaPutVchunkStackPtr
                          (&RefObjNumTkn,      FieldCount + 29,    McStat))

    if (OaCreateVchunk    (&WenvTkn,  sizeof(handletype),
                           FieldCount,         10, 
                           McStat,                              &ArrayAttrTkn  ))
    if (OaPutVchunkStackPtr
                          (&ArrayAttrTkn,      1,          McStat))

    if (OaCreateVchunk    (&WenvTkn,  sizeof(handletype),
                           FieldCount + 30,    10, 
                           McStat,                              &TypeTypeTkn  ))
    if (OaPutVchunkStackPtr
                          (&TypeTypeTkn  ,     FieldCount + 29,  McStat))
    {
       ImageMap->DbOffsetsHndl        = DbOffsetTkn.Handle;
       ImageMap->CppOffsetsHndl       = CppOffsetTkn.Handle;
       ImageMap->FieldAttrIdsHndl     = FieldAttrTkn.Handle;      
       ImageMap->FieldAttrTypesHndl   = FieldTypeTkn.Handle;
       ImageMap->FieldTypeTypesHndl   = TypeTypeTkn.Handle;
       ImageMap->FieldSizesHndl       = FieldSizeTkn.Handle;
       ImageMap->FieldRefObjNumsHndl  = RefObjNumTkn.Handle;      
       ImageMap->ArrayAttrsHndl       = ArrayAttrTkn.Handle;

       OaPutUnit (&ImageMapTkn,  V(ObjNum), (addrtype )ImageMap, McStat);

    }

 }

 /* Set Address's in the FieldsDir struct                                  */
 if (Normal(*Status))
 if (OaGetVchunkAddr   (&DbOffsetTkn  ,     McStat, (addrtype *)&DbOffsets))
 if (MemPutAddr        (FieldsDirChunk,     0,
                        (addrtype )DbOffsets,                    McStat))

 if (OaGetVchunkAddr   (&CppOffsetTkn  ,    McStat, (addrtype *)&CppOffsets))
 if (MemPutAddr        (FieldsDirChunk,     1,
                        (addrtype )CppOffsets,                   McStat))

 if (OaGetVchunkAddr   (&FieldSizeTkn  ,     McStat, (addrtype *)&FieldSizes))
 if (MemPutAddr        (FieldsDirChunk,     2,
                        (addrtype )FieldSizes,                   McStat))

 if (OaGetVchunkAddr   (&FieldTypeTkn  ,     McStat, (addrtype *)&FieldTypes))
 if (MemPutAddr        (FieldsDirChunk,     3,
                        (addrtype )FieldTypes,                   McStat))

 if (OaGetVchunkAddr   (&FieldAttrTkn  ,     McStat, (addrtype *)&FieldAttrs))
 if (MemPutAddr        (FieldsDirChunk,     4,
                        (addrtype )FieldAttrs,                   McStat))

 if (OaGetVchunkAddr   (&RefObjNumTkn  ,     McStat, (addrtype *)&RefObjNums))
 if (MemPutAddr        (FieldsDirChunk,     5,
                        (addrtype )RefObjNums,                   McStat))
                                                     
 if (OaGetVchunkAddr   (&ArrayAttrTkn  ,     McStat, (addrtype *)&ArrayAttrs))
 if (MemPutAddr        (FieldsDirChunk,     6,
                        (addrtype )ArrayAttrs,                   McStat))
 if (OaGetVchunkAddr   (&TypeTypeTkn  ,     McStat, (addrtype *)&TypeTypes))
     MemPutAddr        (FieldsDirChunk,     7,
                        (addrtype )TypeTypes,                    McStat);


 if (Normal(*Status))
 if (*MapExists == False)
     OaPutVchunkStackPtr
                       (&ArrayAttrTkn  ,    1,                   McStat);

TRACK(TrackBak,"TransNewImageMap\n");
return(STAT);
}

boolean    TransBuildFields 
                       (handletype    MapAttrCount,fieldtoken    *FieldTknMap,
                        objnumtype   *Vfields,     indextype      FieldCount,
                        char         *ObjName,
                        imagemaptype *ImageMap,    maparraystype *ClassMap,
                        ft F,lt Z,zz *Status) 
{
tokentype      DbOffsetTkn      = NullToken;
typesize      *DbOffsets        = NullPtr;
tokentype      CppOffsetTkn     = NullToken;
typesize      *CppOffsets       = NullPtr;
tokentype      FieldSizeTkn     = NullToken;
typesize      *FieldSizes       = NullPtr;
tokentype      FieldTypeTkn     = NullToken;
attrtype      *FieldTypes       = NullPtr;
tokentype      TypeTypeTkn      = NullToken;
typetype      *TypeTypes        = NullPtr;
tokentype      FieldAttrTkn     = NullToken;
numtype       *FieldAttrs       = NullPtr;      
tokentype      RefObjNumTkn     = NullToken;
numtype       *RefObjNums       = NullPtr;      
tokentype      ArrayAttrTkn     = NullToken;
numtype       *ArrayAttrs       = NullPtr;      
indextype      NumMapFields     = 0;
indextype      NumArrayAttrs    = 0;
typesize       CppOffset        = 0;
typesize       TrueCppOffset    = 0;
indextype      DbIndex          = 0;
typesize       DbOffset         = 0;
typesize       TrueDbOffset     = 0;
numtype        AttrNum          = 0;
typesize       FieldSize        = 0;
tokentype      AttrTkn          = NullToken;
attrdesctype  *EnvAttr          = NullPtr;
booleantype    AttrFound        = False;
indextype      CppIndex         = 0;
tokentype      TypeTkn          = NullToken;
typedesctype  *EnvType          = NullPtr;
tokentype      BldDescTkn       = NullToken;
objdesctype   *BuildDesc        = NullPtr;
int            i                = -1;
boolean        PureDbStruc      = True;
boolean        TempBug          = ClassBug;
boolean        HasThatAttr      = False;
boolean        TempClassBug     = ClassBug;

 if (Normal(*Status))
 {
    DbOffsetTkn.DbId    =  WenvId;  
    DbOffsetTkn.Handle  =  ImageMap->DbOffsetsHndl;
    DbOffsets           =  ClassMap->DbOffsets;
    PureDbStruc         =  (boolean )ClassMap->DbOffsets[0];

    CppOffsetTkn.DbId   =  WenvId;  
    CppOffsetTkn.Handle =  ImageMap->CppOffsetsHndl;
    CppOffsets          =  ClassMap->CppOffsets;

    FieldAttrTkn.DbId   =  WenvId;  
    FieldAttrTkn.Handle =  ImageMap->FieldAttrIdsHndl;      
    FieldAttrs          =  ClassMap->FieldAttrIds;  

    FieldTypeTkn.DbId   =  WenvId;  
    FieldTypeTkn.Handle =  ImageMap->FieldAttrTypesHndl;
    FieldTypes          =  ClassMap->FieldAttrTypes;

    TypeTypeTkn.DbId    =  WenvId;  
    TypeTypeTkn.Handle  =  ImageMap->FieldTypeTypesHndl;
    TypeTypes          =  ClassMap->FieldTypeTypes;

    FieldSizeTkn.DbId   =  WenvId;  
    FieldSizeTkn.Handle =  ImageMap->FieldSizesHndl;
    FieldSizes          =  ClassMap->FieldSizes;

    RefObjNumTkn.DbId   =  WenvId;  
    RefObjNumTkn.Handle =  ImageMap->FieldRefObjNumsHndl;      
    RefObjNums          =  ClassMap->RefObjNums;

    ArrayAttrTkn.DbId   =  WenvId;  
    ArrayAttrTkn.Handle =  ImageMap->ArrayAttrsHndl;
    ArrayAttrs          =  ClassMap->ArrayAttrs;

 }

 AttrTkn.DbId    = RenvId;
 BldDescTkn.DbId = RenvId;     
 TypeTkn.DbId    = RenvId;     

 if (Normal(*Status))
    OaPutVchunkStackPtr  (&ArrayAttrTkn,    1,  McStat);


 NumMapFields  = 0;
 DbIndex       = 0;

 DbOffset      = 0;
 TrueDbOffset  = 0;
 TrueCppOffset = 0;

 if (Normal(*Status))
 while (DbIndex < FieldCount)
 {
    if (NumMapFields >= FieldCount+29)
    {
     /* Resize Chunks and Reset Addresses in ClassMapDir  */
       TraceMsg (0, 
            " Ctrans:: Resize Chunks and Reset Addresses in ClassMapDir\n");
      *Status = Err_NotImplemented;
    }

    AttrNum               = Vfields[DbIndex];

    if (EnvFetchAttrSize   (AttrNum,         McStat,  &FieldSize))
    if (EnvFetchAttrHandle (AttrNum,         McStat,  &AttrTkn.Handle))
    if (OaGetObject        (&AttrTkn,        McStat,  (addrtype *)&EnvAttr))
    {
       TypeTkn.Handle     = EnvAttr->TypeObj;
       OaGetObject         (&TypeTkn,  McStat, (addrtype *)&EnvType);
       if (NumMapFields == 0)
       {
          FieldAttrs[0]      = (numtype  )EnvAttr->AttrType;
          FieldTypes[0]      = (attrtype )EnvType->TypeType;
          FieldSizes[0]      = (typesize )EnvType->Size;
          if (AttrNum       == That_AttrNum)
            HasThatAttr      = True;
       }
    }

/* Loop through all the 'C' ObjClass Fields                                */
    AttrFound   = False;
    CppIndex    = 0;

    if (Normal(*Status))
    while (!AttrFound
    &&     ++CppIndex < MapAttrCount)
    {
       if (FieldTknMap[CppIndex].Index == AttrNum)
       {
          AttrFound        = True;
          CppOffset        = FieldTknMap[CppIndex].Offset;
       }
    }

    if (Normal(*Status))
    if (!AttrFound
    &&  MapAttrCount != 1)
    {
       sprintf (Msg, " ***Warning... Translation Map 1->0: \"%21s\"\n",
                ObjName);
       SendMsg (0, Msg);
       sprintf (Msg, " ...Attribute [%4u] = \"%21s\".  Not in API Image\n",
                AttrNum, EnvAttr->Nam);
       SendMsg (0, Msg);

       sprintf (Msg, " Attribute Fields[*%4u] of Object ::\n", FieldCount);
       TraceMsg (0, Msg);
       sprintf (Msg, " Appended  Fields[*%4u] specified ::\n", MapAttrCount);
       TraceMsg (0, Msg);

       i = MapAttrCount;
       if (FieldCount > MapAttrCount)
         i = FieldCount;

       for (CppIndex    = 0; ++CppIndex < i; )
       {
          sprintf (Msg, "  Attr [%4u] = %4u;  AppendAttr [%4u] = %4u\n",
                   CppIndex,  Vfields[CppIndex-1], 
                   CppIndex,  FieldTknMap[CppIndex].Index);
          TraceMsg (0, Msg);
       }


    } else {
/*    Add this Field to the Object Class Mapping Arrays                    */

       NumMapFields++;

       TempBug   = ClassBug;
       ClassBug  = False;
       if (HasThatAttr       == False)
       if (EnvAttr->AttrType == aArrayAttr
       ||  EnvAttr->AttrType == aDblPtrAttr
       ||  EnvAttr->AttrType == aVarrayAttr
       ||  EnvAttr->AttrType == aChunkAttr)
       {
            if (OaPushUnit      (&ArrayAttrTkn,  (addrtype )&NumMapFields,
                                 McStat,                    &NumArrayAttrs))
            if (ClassBug || DeBug || OaBug)
            if (sprintf (Msg, "    ArrayType[%2u] for MapNum=%3u Index=%3u\n",
                      NumArrayAttrs, NumMapFields, ArrayAttrs[NumArrayAttrs]))
               TraceMsg (0, Msg);

            if (EnvAttr->AttrType == aVchunkAttr) 
               EnvAlignMember(EnvAttr->AttrType,        EnvType->TypeType,
                              sizeof (tokentype),      (sizetype *)&DbOffset);
            else
               EnvAlignMember(EnvAttr->AttrType,        aAddr,
                              sizeof (handletype),     (sizetype *)&DbOffset);

       } else
             EnvAlignMember  (EnvAttr->AttrType,        EnvType->TypeType,
                              EnvType->Size,           (sizetype *)&DbOffset);

       ClassBug  = TempBug;

       if (MapAttrCount == 1)
             CppOffset             = DbOffset;

       if (DbOffset          != CppOffset
       ||  EnvAttr->AttrType == aObjRefAttr
       ||  EnvAttr->AttrType == aTknRefAttr
       ||  EnvAttr->AttrType == aStrHndlAttr) 
             PureDbStruc           = False;

       CppOffsets[NumMapFields]    = CppOffset;
       DbOffsets[NumMapFields]     = DbOffset;
       FieldSizes[NumMapFields]    = FieldSize;
       FieldTypes[NumMapFields]    = EnvAttr->AttrType;
       FieldAttrs[NumMapFields]    = AttrNum;
       TypeTypes[NumMapFields]     = EnvType->TypeType;
       if (AttrNum == That_AttrNum)
          TypeTypes[0]             = (typetype )NumMapFields;


       if (EnvAttr->AttrType == aArrayAttr
       ||  EnvAttr->AttrType == aVarrayAttr
       ||  EnvAttr->AttrType == aChunkAttr
       ||  EnvAttr->AttrType == aDblPtrAttr)
       {
          if ((BldDescTkn.Handle = EnvAttr->BuildDesc) > 0)
             RefObjNums[NumMapFields]   = BldDescTkn.Handle;
          else 
             RefObjNums[NumMapFields]   = EnvAttr->TypeObj;

       } else if ((BldDescTkn.Handle    = EnvAttr->BuildDesc) > 0) {
          if (OaGetObject    (&BldDescTkn, 
                              McStat,             (addrtype *)&BuildDesc))
             RefObjNums[NumMapFields]   = BuildDesc->ObjNum;

       } else
             RefObjNums[NumMapFields]   = 0;

       if (Normal(*Status))
       if (EnvAttr->AttrType == aMapRefAttr)
       {
           BldDescTkn.Handle             = EnvAttr->BuildDesc;

           TransBuildMapField 
                      (BuildDesc,    DbOffsets,    FieldSizes,
                       McStat,      &NumMapFields,&NumArrayAttrs);
       }

       if (AttrNum   == That_AttrNum)
       {
          HasThatAttr = True;
       } else {
          DbOffset   += FieldSize;
          HasThatAttr = False;
       }
    }
    DbIndex++;

 }

 if (Normal(*Status))
 {

    RefObjNums[0]    =  (numtype   )NumMapFields;
    ArrayAttrs[0]    =  (numtype   )NumArrayAttrs;
    if (CppOffsets[0]  == 140)
       DbOffsets[0]     =  (typesize  )False;
    else
       DbOffsets[0]     =  (typesize  )PureDbStruc;
 }

/* Update the StackPointers to the FieldMap Arrays                         */
 if (Normal(*Status))
 {
    if (ClassBug || OaBug || DeBug)
    if (sprintf (Msg,  "  Adjust the StackPtrs for NumMapFields= %3u\n",
                 NumMapFields++))
       TraceMsg (0, Msg);

    if (OaPutVchunkStackPtr 
                          (&DbOffsetTkn  ,      NumMapFields,     McStat))

    if (OaPutVchunkStackPtr
                          (&CppOffsetTkn  ,     NumMapFields,     McStat))

    if (OaPutVchunkStackPtr
                          (&FieldSizeTkn  ,     NumMapFields,     McStat))

    if (OaPutVchunkStackPtr 
                          (&FieldTypeTkn  ,     NumMapFields,     McStat))

    if (OaPutVchunkStackPtr
                          (&FieldAttrTkn  ,     NumMapFields,     McStat))

    if (OaPutVchunkStackPtr
                          (&RefObjNumTkn  ,     NumMapFields,     McStat))

        OaPutVchunkStackPtr
                          (&TypeTypeTkn  ,      NumMapFields,     McStat);

 }

 ClassBug = TempClassBug;

TRACK(TrackBak,"TransBuildFields\n");
return(STAT);
}



boolean    TransBuildMapField
                        (objdesctype  *BuildDesc,   typesize     *DbOffsets,
                         typesize     *FieldSizes,  ft F,lt Z,zz *Status,
                         indextype    *NumMapFields,indextype  *NumArrayAttrs)
{
addrtype      *SubObjFieldMap   = NullPtr;
typesize      *SubObjDbOffsets  = NullPtr;
typesize      *SubObjCppOffsets = NullPtr;
typesize      *SubObjFieldSizes = NullPtr;
attrtype      *SubObjFieldTypes = NullPtr;
typetype      *SubObjTypeTypes  = NullPtr;
numtype       *SubObjFieldAttrs = NullPtr;      
numtype       *SubObjRefObjNums = NullPtr;      
numtype       *SubObjArrayAttrs = NullPtr;      
indextype      SubObjNumArrayAttrs= 0;
numtype        NumMaps          = 0;
sizetype       AttrSize         = 0;
sizetype       SubObjClassSize  = 0;

    if (MemGetAddr   (C_ObjMapDir,   BuildDesc->ObjNum,
                      McStat,        (addrtype *)&SubObjFieldMap))
    if (SubObjFieldMap)
    {
       SubObjDbOffsets    =  (typesize *)SubObjFieldMap[0];
       SubObjCppOffsets   =  (typesize *)SubObjFieldMap[1];
       SubObjFieldSizes   =  (typesize *)SubObjFieldMap[2];
       SubObjFieldTypes   =  (attrtype *)SubObjFieldMap[3];
       SubObjFieldAttrs   =  (numtype  *)SubObjFieldMap[4];
       SubObjRefObjNums   =  (numtype  *)SubObjFieldMap[5];
       SubObjArrayAttrs   =  (numtype  *)SubObjFieldMap[6];
       SubObjTypeTypes    =  (typetype *)SubObjFieldMap[7];

       SubObjNumArrayAttrs=  (indextype )SubObjArrayAttrs[0];

       SubObjClassSize    =  (sizetype  )SubObjCppOffsets[0];

       AttrSize           = FieldSizes[*NumMapFields];

       NumMaps            = AttrSize / BuildDesc->ObjSize;
       if (NumMaps        > 1)
          FieldSizes[*NumMapFields] = SubObjClassSize * NumMaps;

       if (SubObjClassSize  != BuildDesc->ObjSize)
          DbOffsets[0]    = False;

      *NumArrayAttrs     += SubObjNumArrayAttrs;

    }


TRACK(TrackBak,"TransBuildMapField\n");
return(STAT);
}


boolean Trans_FetchAttrOffset
                        (numtype       ObjNum,      numtype       AplId,
                         numtype       FieldNum,    ft F,lt Z,zz *Status,
                         sizetype     *CppOffset)
{
classmap   ClassMap;
count       i              = 0;
boolean     AttrFound      = False;

  if (TransGetMap       (ObjNum,    AplId,   McStat,  &ClassMap))
    *CppOffset         = ClassMap.CppOffsets[FieldNum];
  else
    *CppOffset  = 0;

  if (!Normal(*Status)
  ||  OaBug  ||  DeBug) 
  {
     sprintf  (Msg, " Trans_FetchAttrOffsets:: ImageOffset=%4u\n",
              *CppOffset);
     TraceMsg (0, Msg);         
  }

TRACK(TrackBak,"Trans_FetchAttrOffset\n");
return(STAT);
}


boolean Trans_FetchAttrOffsets
                        (numtype       ObjNum,      numtype       AplId,
                         numtype       AttrId,      ft F,lt Z,zz *Status,
                         sizetype     *DbOffset,    sizetype     *CppOffset)
{
classmap   ClassMap;
count       i              = 0;
boolean     AttrFound      = False;

 *DbOffset   = 0;
 *CppOffset  = 0;

  AttrFound            = False;
  if (TransGetMap       (ObjNum,    AplId,   McStat,  &ClassMap))
  {
     i                 = 1;
     while (AttrFound == False 
     &&     i         <= ClassMap.NumMapFields)
     {
        if (ClassMap.FieldAttrIds[i] == AttrId)
        {
           AttrFound   = True;
          *DbOffset    += ClassMap.DbOffsets[i];
          *CppOffset   += ClassMap.CppOffsets[i];
        } else
         ++i;
     }   
     if (!AttrFound)
     {
        sprintf  (Msg,
          " @Trans_FetchAttrOffsets:: AttrId[%4u] NOT in Class[%4u] Def.\n",
                  AttrId, ClassMap.ObjNum);
        TraceMsg (0, Msg);         
        Trans_DumpMap (ClassMap.ObjNum,  Cpp_Apl,  McStat);
       *Status         = Trans_AttrNotInClassDef;
     }
  }
  if (!Normal(*Status)
  ||  OaBug  ||  DeBug) 
  {
     sprintf  (Msg, " Trans_FetchAttrOffsets:: DbOffset=%4u, ImageOffset=%4u\n",
               *DbOffset,  *CppOffset);
     TraceMsg (0, Msg);         
  }

TRACK(TrackBak,"Trans_FetchAttrOffsets\n");
return(STAT);
}


boolean    Trans_FetchFieldOffsets
                        (tokentype    *Token,       numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         sizetype     *DbOffset,    sizetype     *CppOffset)
{
numtype       AttrId     = 0;
attrtype      AttrType   = aNonTypeAttr;   
numtype       RefObjNum  = 0;
typetype      FieldType  = aNonTypeType;
sizetype      DbBase     = 0;
sizetype      CppBase    = 0;

   Trans_FetchFieldOffset (Token,          Cpp_Apl, 
                           FieldTkns,      SubLevel,
                           McStat,         FieldSize,    &AttrId,
                          &AttrType,      &RefObjNum,    &FieldType,
                           DbOffset,       CppOffset,
                          &DbBase,        &CppBase);

TRACK(TrackBak,"Trans_FetchFieldOffsets\n");
return(STAT);
}


boolean    Trans_FetchFieldOffset
                        (tokentype    *Token,       numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         numtype      *AttrId,      attrtype     *AttrType,    
                         numtype      *RefObjNum,   typetype     *FieldType,
                         sizetype     *DbOffset,    sizetype     *CppOffset,
                         sizetype     *DbBase,      sizetype     *CppBase)
{
dbheader   *CoreDb         = NullPtr;
numtype     ObjNum         = 0;
int         i              = 0;

  if (TmFetchCoreDb   (Token,                       McStat,   &CoreDb))
  if (HmGetObjNum     (CoreDb,      Token->Handle,  McStat,   &ObjNum))
      Trans_FetchObjFieldSpec  
                      (ObjNum,    AplId,      FieldTkns, SubLevel,
                       McStat,    FieldSize,  AttrId,    AttrType, RefObjNum,
                       FieldType, DbOffset,   CppOffset, DbBase,   CppBase);


TRACK(TrackBak,"Trans_FetchFieldOffset\n");
return(STAT);
}


boolean    Trans_FetchObjFieldSpec
                        (numtype       ObjNum,      numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         numtype      *AttrId,      attrtype     *AttrType,    
                         numtype      *RefObjNum,   typetype     *FieldType,
                         sizetype     *DbOffset,    sizetype     *CppOffset,
                         sizetype     *DbBase,      sizetype     *CppBase)
{
classmap   ClassMap;
count       i              = 0;
numtype     Level          = 0;
sizetype    MapImageSize   = 0;
sizetype    ImageOffset    = 0;
sizetype    ObjOffset      = 0;
boolean     AttrFound      = False;
indextype   NumMaps        = 0;

 *DbOffset   = 0;
 *CppOffset  = 0;

  Level     = 0;
  while (Normal(*Status)
  &&     Level < SubLevel)
  {
      AttrFound            = False;

      if (TransGetMap       (ObjNum,    AplId,   McStat,  &ClassMap))
      {
         if (Level > 0)
         if (FieldTkns[Level-1].Index  > 0)
         {
            NumMaps        = MapImageSize / ClassMap.ImageSize;
            if (FieldTkns[Level-1].Index  < NumMaps)
            {
            /* Increment offset for MapRefObj array Inset                 */
               ImageOffset    = ClassMap.ImageSize * FieldTkns[Level-1].Index;
              *CppOffset     += ImageOffset;

               ObjOffset     += ClassMap.ClassSize * FieldTkns[Level-1].Index;
              *DbOffset      += ObjOffset;

            } else {
               sprintf  (Msg,
                "  TransFetchFieldOffset::= Field Index=%4u >= NumMaps=%4u\n",
                      FieldTkns[Level].AttrId, NumMaps);
               TraceMsg (0, Msg);         
              *Status = Trans_IndexOutOfRange;
            }
         }

         i                 = 1;
         while (AttrFound == False 
         &&     i         <= ClassMap.NumMapFields)
         {
           if (ClassMap.FieldAttrIds[i] == FieldTkns[Level].AttrId)
               AttrFound   = True;
           else
             ++i;
         }   
         if (!AttrFound)
           *Status         = Trans_AttrNotInClassDef;

      } else {
         sprintf  (Msg,
                "  TransFetchFieldOffset::= ClassMap[%4u] was NOT Invoked.\n",
                   ObjNum);
         TraceMsg (0, Msg);         
      }

      if (Normal(*Status))
      {
       ++Level;
        *DbOffset    += ClassMap.DbOffsets[i];
        *CppOffset   += ClassMap.CppOffsets[i];
         if (Level   == 1)
         {
           *DbBase    = *DbOffset;
           *CppBase   = *CppOffset;
         }
         /* Current Field Attr must be of type MapRefAttr                  */
         /* (to allow nested Field specification).                         */

         if (Normal(*Status))
         if (Level != SubLevel)
         if (ClassMap.FieldAttrTypes[i] != aMapRefAttr)
         {
            sprintf  (Msg,
                "*** ERRor...@ Level =%4u; AttrId=%4u Not a Nested Object\n",
                      Level, FieldTkns[Level].AttrId);
            TraceMsg (0, Msg);         
           *Status = Trans_AttrNotInClassDef;

         } else {
            MapImageSize   = ClassMap.FieldSizes[i];
            ObjNum         = ClassMap.RefObjNums[i];
         }

      }
  }

  if (!AttrFound)
  {
     sprintf  (Msg, " Trans_FetchObjFieldSpec:: ObjNum=%4u\n", ObjNum);
     TraceMsg (0, Msg);         
     sprintf  (Msg,"         @ Level =%4u; AttrId=%4u, Index=%4u\n",
               Level, FieldTkns[Level].AttrId, FieldTkns[Level].Index);
     TraceMsg (0, Msg);         

    *Status = Trans_AttrNotInClassDef;

  } else {
    *FieldSize    = ClassMap.FieldSizes[i];
    *AttrId       = ClassMap.FieldAttrIds[i];
    *AttrType     = ClassMap.FieldAttrTypes[i];
    *RefObjNum    = ClassMap.RefObjNums[i];
    *FieldType    = ClassMap.FieldTypeTypes[i];
  }

  if (!Normal(*Status))
  {
     sprintf  (Msg, " Trans_FetchObjFieldSpec:: ObjNum=%4u\n", ObjNum);
     TraceMsg (0, Msg);         
     for (i=0; i < SubLevel; i++) 
     {
        sprintf  (Msg, " Field %4u. AttrId=%4u; Index=%4u.\n",
                  i, FieldTkns[i].AttrId,  FieldTkns[i].Index);
        TraceMsg (0, Msg);         
     }
  }

TRACK(TrackBak,"Trans_FetchObjFieldSpec\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF TRANS00.C                               !
  +-----------------------------------------------------------------------+*/
