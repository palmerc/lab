/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* TRANS01.c */
#define  TRANS01
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "vom.h"
#include "oa0.h"
#include "voa.h"
#include "vchunk.h"
#include "env1.h"
#include "oa1.h"
#include "shell.h"
#include "trans.h"
#include "api.h"

#define        That_AttrNum           (numtype )1
#define        Handle_AttrNum         (numtype )2

extern boolean ImageInitClassMap
                             (numtype       ClassId, size_t        ObjSize,
                              numtype       AplId,   ft F,lt Z,zz *Status);
 
extern boolean DbmInvokeVchunk
                             (tokentype    *Token,
                              ft F,lt Z,zz *Status, vchunktkntype *VchunkTkn);

extern addrtype Image01_GetFreeStoreAddr 
                             (void);

#define Core0MoreCore      Core0_MoreCore  
#define MemGetAddr         Mem_GetAddr
#define MemGetWord         Mem_GetWord
#define MemGetFieldNum      Mem_GetWord
#define MemGetObjNum        Mem_GetWord
#define MemGetLocalObjNum   Mem_GetWord
#define MemGetObjHdr        Mem_GetAddr

extern boolean Core0MoreCore  (bytessizetype  Size,
                               ft F,lt Z,zz *Status,    addrtype *Addr);

extern boolean CppCreateObject(numtype       ObjNum, 
                               ft F,lt Z,zz *Status,  addrtype    *Object);

extern boolean CppCastObject  (numtype       ObjNum,  numtype      BaseObjNum,
                               ft F,lt Z,zz *Status,  addrtype    *Object);


#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

numtype  Ref_BaseClass = 0;

/*+-----------------------------------------------------------------------+*
~C                        TRANSFER MANAGER
 *+-----------------------------------------------------------------------+*/
boolean C_GetObjectImage  (tokentype    *Token,    numtype       AplId,
                           ft F,lt Z,zz *Status,   addrtype     *This)
{
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype   *FieldMap       = NullPtr;
typesize   *CppOffsets     = NullPtr;
typesize    ObjSize        = 0;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  {
      HmGetObjectAddr    (CoreDb,    Token->Handle,  McStat,   This);

      TRACK(TrackBak,"C_GetObjectImage\n");
      return (STAT);
  }

  if (Normal(*Status))
  if (HmGetObjNum       (CoreDb, Token->Handle,   McStat,        &ObjNum))
  if (MemGetAddr        (C_ObjMapDir,                             ObjNum,
                         McStat,                     (addrtype *)&FieldMap))
  if (FieldMap != NullPtr)
  {
     CppOffsets   =  (typesize *)FieldMap[1];
     ObjSize      =  (typesize  )CppOffsets[0];
  } else
    *Status = Trans_NullMap;


  if (ObjSize == 0)
    *Status = Trans_NullObjSize;

  if (Normal(*Status))
  if (Core0MoreCore ((bytessizetype )ObjSize, McStat, This))
  {
     C_FaxToThis     (Token,    *This,   AplId,  McStat);
  }

TRACK(TrackBak,"C_GetObjectImage\n");
return(STAT);
}


boolean C_ObjectNewImage  (tokentype    *Token,    numtype       ObjNum,
                           numtype       AplId,
                           ft F,lt Z,zz *Status,   addrtype     *This)
{
addrtype   *FieldMap       = NullPtr;
typesize    ObjSize        = 0;
typesize   *CppOffsets     = NullPtr;
numtype    *FieldAttrs     = NullPtr;
count       i              = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    That           = NullPtr;

  if (MemGetAddr        (C_ObjMapDir,                        ObjNum,
                         McStat,                (addrtype *)&FieldMap))
  if (FieldMap != NullPtr)
  {
     CppOffsets   =  (typesize *)FieldMap[1];
     FieldAttrs   =  (numtype  *)FieldMap[4];
     if ((ObjSize =  (typesize  )CppOffsets[0]) == 0)
         *Status  = Trans_NullObjSize;
  } else
    *Status       = Trans_NullMap;

  if (Normal(*Status))
  if (Core0MoreCore ((bytessizetype )ObjSize, McStat, This))
     That       = *This;
  else
    *Status  = Trans_CoreNoMore;

  i = 1;
  if (Normal(*Status))
  if (FieldAttrs[i] == That_AttrNum)
  {
     CppObjectAddr   = (addrtype )((char *)That  + CppOffsets[i]);
     MoveBytes    (This,    CppObjectAddr,  sizeof(addrtype));
     i++;
  }

  if (Normal(*Status))
  if (FieldAttrs[i] == Handle_AttrNum)
  {
     CppObjectAddr   = (addrtype )((char *)That  + CppOffsets[i]);
     MoveBytes    ((addrtype )Token,    CppObjectAddr,  sizeof(tokentype));
  }

  if (Normal(*Status))
  if (HmPutObjectMemLoc (CoreDbs[Token->DbId],  Token->Handle,
                         (numtype )1,           McStat))
      HmPutObjectAddr   (CoreDbs[Token->DbId],  Token->Handle,
                        *This,                  McStat);

TRACK(TrackBak,"C_ObjectNewImage\n");
return(STAT);
}


boolean    C_InvokeAtThis
                         (tokentype    *Token,    addrtype      This,
                          numtype       AplId,    ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
numtype     ObjNum         = 0;
addrtype   *FieldMap       = NullPtr;
typesize   *CppOffsets     = NullPtr;
numtype    *FieldAttrs     = NullPtr;
count       i              = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    That           = This;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))
  if (MemGetAddr           (C_ObjMapDir,                       ObjNum,
                            McStat,               (addrtype *)&FieldMap))
  if (FieldMap)
  {
     CppOffsets      =  (typesize *)FieldMap[1];
     FieldAttrs      =  (numtype  *)FieldMap[4];
  } else
    *Status = Trans_NullMap;

  i = 1;
  if (Normal(*Status))
  if (FieldAttrs[i] == That_AttrNum)
  {
     CppObjectAddr   = (addrtype )((char *)This  + CppOffsets[i]);
     MoveBytes    ((addrtype )&That,    CppObjectAddr,  sizeof(addrtype));
     i++;
  }

  if (Normal(*Status))
  if (FieldAttrs[i] == Handle_AttrNum)
  {
     CppObjectAddr   = (addrtype )((char *)This  + CppOffsets[i]);
     MoveBytes    ((addrtype )Token,    CppObjectAddr,  sizeof(tokentype));
  }

  if (Normal(*Status))
  if (HmPutObjectMemLoc (CoreDbs[Token->DbId],  Token->Handle,
                         (numtype )1,           McStat))
      HmPutObjectAddr   (CoreDbs[Token->DbId],  Token->Handle,
                         This,                  McStat);

TRACK(TrackBak,"C_InvokeAtThis\n");
return (STAT);
}

boolean    C_FaxToThis   (tokentype    *Token,    addrtype      Object,
                          numtype       AplId,    ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
numtype     ObjNum         = 0;
indextype   NumArrayAttrs  = 0;
boolean     PureDbStruc    = False;
typesize    ObjSize        = 0;
indextype   ThatIndex      = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    ImageOffset    = NullPtr;
statustype  DumpStatus     = Env_Normal;
maparrays  *FieldMap       = NullPtr;
addrtype    DbObject       = NullPtr;
numtype     LocalObj       = 0;
numtype     MemLoc         = 0;

  /* Get DbObject                                                          */
  if (TmGetCoreDb       (Token,                   McStat,        &CoreDb))
  if (HmGetObjNum       (CoreDb, Token->Handle,   McStat,        &ObjNum))

  if (Normal(*Status))
  if (OaGetObject       (Token,                   McStat,        &DbObject))

  /* Object In Core... Return                                              */
  if (DbObject == Object)
  {
      TRACK(TrackBak,"C_FaxToThis\n");
      return (STAT);
  }

  if (Normal(*Status))
  if (MemGetLocalObjNum (CoreDbs[Token->DbId]->LocalObjNumMap,   ObjNum,
                         McStat,                                &LocalObj))
  if (MemGetAddr        (C_ObjMapDir,                            ObjNum,
                         McStat,                     (addrtype *)&FieldMap))

  if (FieldMap)
  {
     NumArrayAttrs=  (indextype )FieldMap->ArrayAttrs[0];
     PureDbStruc  =  (boolean   )FieldMap->DbOffsets[0];
     ThatIndex    =  (indextype )FieldMap->FieldTypeTypes[0];
     ObjSize      =  (typesize  )FieldMap->CppOffsets[0];
  } else
    *Status = Trans_NullMap;


  if (Normal(*Status))
  if (HmGetObjectMemLoc (CoreDb,                    Token->Handle,
                         McStat,                   &MemLoc))
  if (MemLoc == 1)
  {
    if (AplId == Cpp_Apl
    &&  Image01_GetFreeStoreAddr () == Object
    &&  DeBug == -1)
    {
      if (Core0FreeCoreSpace   (&Object,  (bytessize )ObjSize, McStat))
         Object  = DbObject;

    } else {
      MoveBytes            (DbObject,      Object,     ObjSize);

      if (Core0FreeCoreSpace   (&DbObject,  (bytessize )ObjSize, McStat))
      {
         CppObjectAddr = Object;
         ImageOffset   = (addrtype )((char *)Object     
                       + FieldMap->CppOffsets[ThatIndex]);
         MoveBytes       ((addrtype )&CppObjectAddr,  (addrtype )ImageOffset, 
                          sizeof (addrtype));
      }
      if (Normal(*Status))
         OaInvokeCppObject (Token,  Object,  McStat);
    }
    TRACK(TrackBak,"C_FaxToThis\n");
    return (STAT);
  }


  /* Object Not In Core... Retrieve                                        */

  if (Normal(*Status))
  if (MemGetLocalObjNum (CoreDbs[Token->DbId]->LocalObjNumMap,   ObjNum,
                         McStat,                                &LocalObj))
  if (MemGetAddr        (C_ObjMapDir,                            ObjNum,
                         McStat,                     (addrtype *)&FieldMap))

  if (FieldMap)
  {
     NumArrayAttrs=  (indextype )FieldMap->ArrayAttrs[0];
     PureDbStruc  =  (boolean   )FieldMap->DbOffsets[0];
     ThatIndex    =  (indextype )FieldMap->FieldTypeTypes[0];
     ObjSize      =  (typesize  )FieldMap->CppOffsets[0];
  } else
    *Status = Trans_NullMap;

  if (Normal(*Status))
  if (PureDbStruc   == False
  ||  NumArrayAttrs  > 0)
  {
     C_FaxToClassFields (Token->DbId,   LocalObj,   DbObject,    Object,
                         ObjNum,        FieldMap,   AplId,       McStat);

  } else {
     MoveBytes          (DbObject,      Object,     ObjSize);
  }

  /* Adjust the Pointer To 'this ' if a C++ image                         */
  if (Normal(*Status))
  if (ThatIndex                 > 0
  || FieldMap->FieldAttrIds[1] == That_AttrNum)
  {
     CppObjectAddr = Object;
     ImageOffset   = (addrtype )((char *)Object     
                   + FieldMap->CppOffsets[ThatIndex]);
     MoveBytes       ((addrtype )&CppObjectAddr,    (addrtype )ImageOffset, 
                      sizeof (addrtype));
  }

  if (Normal(*Status))
     OaInvokeCppObject (Token,  Object,  McStat);

  if (DeBug || OaBug)
     OaDumpObject      (0,      Token,   Dump_Status);


TRACK(TrackBak,"C_FaxToThis\n");
return (STAT);
}


boolean    C_FaxToClassFields
                         (idtype        DbId,     numtype       LocalObj,
                          addrtype      DbObject, addrtype      Object,
                          numtype       ObjNum,   maparrays    *FieldMap,
                          numtype       AplId,    ft F,lt Z,zz *Status)
{
indextype   NumArrayAttrs  = 0;
indextype   NumMapFields   = 0;
boolean     PureDbStruc    = False;
typesize    ObjSize        = 0;
count       i              = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    DbObjectAddr   = NullPtr;
indextype   AttrCount      = 0;
typesize    AttrDbSize     = 0;
typesize    Attr01Size     = 0;
typesize    BaseDbOffset   = 0;
typesize    Base01Offset   = 0;
int         k              = 0;
tokentype  *RefTkn         = NullPtr;
longtype    LongIndex      = 0;
indextype   StrIndex       = 0;
char       *StrAddr        = NullPtr;
indextype   UnitCount      = 0;
addrtype    Array          = NullPtr;
tokentype   VchunkTkn      = NullToken;
chunkstruc  ChunkStruc;  
handletype *ArrayHndls     = NullPtr;
handletype *SubArrays      = NullPtr;
addrtype   *Arrays         = NullPtr;
int         j              = 0;
indextype   NumSubArrays   = 0;
tokentype   SubChunkTkn    = NullToken;
objheader  *ObjHdr         = NullPtr;
vchunktkntype  Vchunk;
addrtype    Null_Ptr       = NullPtr;
typesize    Offset         = 0;

  VchunkTkn.DbId     = DbId;
  SubChunkTkn.DbId   = DbId;

  /* Object Not In Core... Retrieve                                        */

  NumMapFields =  (indextype )FieldMap->RefObjNums[0];
  NumArrayAttrs=  (indextype )FieldMap->ArrayAttrs[0];
  PureDbStruc  =  (boolean   )FieldMap->DbOffsets[0];
  ObjSize      =  (typesize  )FieldMap->CppOffsets[0];

  while (++i <= NumMapFields
  &&     Normal(*Status)) 
  {
       DbObjectAddr       = (addrtype )((char *)DbObject 
                                              + FieldMap->DbOffsets[i]);
       CppObjectAddr      = (addrtype )((char *)Object
                                              + FieldMap->CppOffsets[i]);
       if ((AttrCount    = AttrTypeSizes[
                                     FieldMap->FieldAttrTypes[i] ]) != 1)
       {
          Attr01Size     = AttrCount;
          AttrDbSize     = AttrCount;
          AttrCount      = FieldMap->FieldSizes[i] / AttrCount;
          if (FieldMap->FieldAttrTypes[i] == aSetHeadAttr
          ||  FieldMap->FieldAttrTypes[i] == aSetNodeAttr)
          {
             AttrCount      = 1;
          }
       } else
          Attr01Size     = 0;

       BaseDbOffset      = 0;
       Base01Offset      = 0;
       k                 = 0;

       if (FieldMap->FieldAttrIds[i] != That_AttrNum)
       while (k++ < AttrCount)
       {
         CppObjectAddr   = (addrtype )((char *)CppObjectAddr + Base01Offset);
         DbObjectAddr    = (addrtype )((char *)DbObjectAddr  + BaseDbOffset);

         switch (FieldMap->FieldAttrTypes[i])
         {
       
          case aAttr         :
          case aPortRefAttr  :
          case aSetNodeAttr  :
          case aSetHeadAttr  :
          case aMtrxHeadAttr :
          case aRowHeadAttr  :
          case aColHeadAttr  :
          case aMateNodeAttr :
          case aPointNodeAttr:
          case aSlotHndlAttr :
          case aSlotTokenAttr:
          case aVchunkAttr   :
          case aGrpAttr      :
          case aVgrpAttr     :
            MoveBytes   (DbObjectAddr,   CppObjectAddr,
                         FieldMap->FieldSizes[i]);
            break;

          case aObjRefAttr   :
            RefTkn   =   (tokentype *)DbObjectAddr;
            Ref_BaseClass = FieldMap->RefObjNums[i];
            if (RefTkn->Handle)
            {
               if(C_RefToAddr    (RefTkn,       AplId,
                                  McStat,       (addrtype *)CppObjectAddr))
                  Hm_IncrementMemRef
                                 (CoreDbs[DbId], RefTkn->Handle,  McStat);
            } else {
               MoveBytes ((addrtype )&NullToken,     DbObjectAddr,
                          FieldMap->FieldSizes[i]);

               MoveBytes (&Null_Ptr,                 CppObjectAddr, 
                          FieldMap->FieldSizes[i]);
            }
            break;

          case aMapRefAttr   :
            C_MapRefToAddr (DbId,                     LocalObj,
                            DbObjectAddr,             CppObjectAddr,
                            FieldMap->RefObjNums[i],  FieldMap->FieldSizes[i],
                            AplId,                    McStat);
            break;

          case aTknRefAttr  :
            MoveBytes   (&Null_Ptr, CppObjectAddr,  sizeof (addrtype));
            CppObjectAddr = (addrtype )((char *)CppObjectAddr
                                      + sizeof (addrtype));
            MoveBytes   (DbObjectAddr, CppObjectAddr,  sizeof (tokentype));
            break;

          case aStrHndlAttr  :
               LongIndex = *(longtype *)DbObjectAddr;
               StrIndex  = LongIndex;
               StrAddr   = NullPtr;
               if (StrIndex)
               if (MemGetObjHdr      (CoreDbs[DbId]->ObjHdrDir, LocalObj,
                                      McStat,     (addrtype *)&ObjHdr))
                   OaGetString (DbId,          ObjHdr,        StrIndex,
                                McStat,                      &StrAddr);
               if (StrAddr)
                  MoveBytes     ((addrtype )&StrAddr,  CppObjectAddr, 
                                 sizeof(char *));
               else
                  StrAddr = NullStr;
               break;


          case aArrayAttr    :
            VchunkTkn.DbId = DbId;
            MoveBytes   (DbObjectAddr, (addrtype )&VchunkTkn.Handle,
                         sizeof (handletype));
            if (VchunkTkn.Handle)
            {
               VchunkGetAddress (&VchunkTkn,       McStat,       &Array);
               MoveBytes  ((addrtype )&Array,  CppObjectAddr, 
                           sizeof (addrtype));
            }
            break;

          case aChunkAttr    :
          case aVarrayAttr   :
            UnitCount      = 0;
            Array          = NullPtr;
            VchunkTkn.DbId = DbId;
            MoveBytes (DbObjectAddr, (addrtype )&VchunkTkn.Handle, 
                       sizeof (handletype));
            if (VchunkTkn.Handle)
            if (VchunkGetVstruc   (&VchunkTkn,  McStat,     &ChunkStruc))
            if (VchunkGetAddress  (&VchunkTkn,  McStat,     &Array))
                UnitCount = ChunkStruc.UnitCount;

            MoveBytes ((addrtype )&VchunkTkn,    CppObjectAddr,
                       sizeof(tokentype));

            CppObjectAddr = (addrtype )((char *)CppObjectAddr 
                                                      + sizeof(tokentype));
            MoveBytes ((addrtype )&UnitCount,   CppObjectAddr,  
                                                        sizeof(indextype));
            if (BytePtrAlignment  <= 4)
              Offset      = sizeof (indextype);
            else
              Offset      = sizeof (addrtype);
            CppObjectAddr = (addrtype )((char *)CppObjectAddr 
                                                      + Offset);
            MoveBytes((addrtype )&Array, CppObjectAddr, sizeof(addrtype));

            if (FieldMap->FieldAttrTypes[i] != aChunkAttr)
              break;

            CppObjectAddr = (addrtype )((char *)CppObjectAddr 
                                                      + sizeof(addrtype));
            MoveBytes((addrtype )&ChunkStruc.StackPtr, CppObjectAddr,
                      sizeof(indextype));
            break;

          case aDblPtrAttr   :
            MoveBytes   (DbObjectAddr, (addrtype )&VchunkTkn.Handle,
                         sizeof (handletype));
            if (VchunkTkn.Handle)
               VchunkGetAddress (&VchunkTkn,  McStat, (addrtype *)&Arrays);

            MoveBytes   ((addrtype )&Arrays,    CppObjectAddr,
                         sizeof (addrtype));

            if (VchunkTkn.Handle)
            if (VchunkGetVstruc (&VchunkTkn,      McStat, &ChunkStruc))
            {
               ArrayHndls    = (handletype *)Arrays;
               NumSubArrays  = ChunkStruc.StackPtr / 3;
               SubArrays     = (handletype *)&ArrayHndls[NumSubArrays * 2];
               j             = -1;

               while (Normal(*Status)
               &&     ++j    < NumSubArrays)
               {
#ifdef __BCC__
                  memcpy (&SubChunkTkn.Handle, 
                          &SubArrays[j],           sizeof (handletype));
#else
                  SubChunkTkn.Handle = SubArrays[j];
#endif

                  if (SubChunkTkn.Handle)
                  if (DbmInvokeVchunk  (&SubChunkTkn,    McStat,   &Vchunk))
                      VchunkGetAddress (&SubChunkTkn,    McStat,   
                                        (addrtype *)&Arrays[j]);
               }
               if (Normal(*Status))
               if (DeBug  || OaBug || ClassBug)
                  VchunkDump  (&VchunkTkn,   McStat);
            }
            break;

          default            :
           *Status = Err_NotImplemented;
         }
       
         Base01Offset  += Attr01Size;
         BaseDbOffset  += AttrDbSize;
       }
  }

TRACK(TrackBak,"C_FaxToClassFields\n");
return (STAT);
}


boolean    C_MapRefToAddr (idtype        DbId,     numtype       LocalObj,
                           addrtype      DbObject, addrtype      Object,
                           numtype       ObjNum,   sizetype      MapSize,
                           numtype       AplId,    ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
maparrays  *FieldMap       = NullPtr;
indextype   NumArrayAttrs  = 0;
indextype   NumMapFields   = 0;
boolean     PureDbStruc    = False;
typesize    ObjSize        = 0;
typesize    DbObjSize      = 0;
indextype   NumMaps        = 0;
count       l              = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    DbObjectAddr   = NullPtr;
tokentype   AnchorTkn      = NullToken;

  AnchorTkn.DbId = DbId;
  /* Get FieldMap for this class                                           */
  if (TmFetchCoreDb     (&AnchorTkn,                  McStat,   &CoreDb))
  if (MemGetAddr        (C_ObjMapDir,                            ObjNum,
                         McStat,                     (addrtype *)&FieldMap))

  if (FieldMap  == NullPtr)
  {
    if (EnvFetchObjSize   (ObjNum,        McStat,               &ObjSize))
    if (ImageInitClassMap (ObjNum,        ObjSize,    C_Apl,     McStat))
    if (TransInvokeMap    (C_Apl,         McStat))
    if (TransBuildMap     (C_Apl,         McStat))
    if (MemGetAddr        (C_ObjMapDir,                          ObjNum,
                           McStat,                  (addrtype *)&FieldMap))
    if (FieldMap == NullPtr)
       *Status = Trans_NullMap;
  }

  if (Normal(*Status))
  {
     NumMapFields =  (indextype )FieldMap->RefObjNums[0];
     NumArrayAttrs=  (indextype )FieldMap->ArrayAttrs[0];
     PureDbStruc  =  (boolean   )FieldMap->DbOffsets[0];
     ObjSize      =  (typesize  )FieldMap->CppOffsets[0];
  }

  if (Normal(*Status))
  if (!PureDbStruc
  ||  NumArrayAttrs > 0)
  {

   EnvFetchObjSize (ObjNum,  McStat, &DbObjSize);
   NumMaps      =  MapSize   / DbObjSize; 

   l                = 0;
   while (Normal(*Status)
   &&     ++l      <= NumMaps)
   {
     DbObjectAddr       = (addrtype )((char *)DbObject
                                +  ((l-1) * DbObjSize));
     CppObjectAddr      = (addrtype )((char *)Object
                                +  ((l-1) * ObjSize));

     C_FaxToClassFields  (DbId,          LocalObj,   DbObjectAddr,
                          CppObjectAddr, ObjNum,     FieldMap,    
                          AplId,         McStat);

   }

  } else {
     MoveBytes   (DbObject,   Object,   MapSize);
  }

TRACK(TrackBak,"C_MapRefToAddr\n");
return (STAT);
}
 

boolean    C_RefToAddr    (tokentype    *Token,   numtype       AplId,
                           ft F,lt Z,zz *Status,  addrtype     *This)
{
cppobjecttype  *ItSelf  = NullPtr;
dbheader   *CoreDb      = NullPtr;
indextype   LocalObjNum = 0;
indextype   ObjNum      = 0;
numtype     MemLoc      = 0;
addrtype    That        = 0;

  *This  = NullPtr;
   if (TmIsValid(Token, McStat))
   {
      if (OaGetObject (Token, McStat, (addrtype *)&ItSelf))
      {
      }

      if (Normal(*Status))
      if (TmGetCoreDb       (Token,     McStat,        &CoreDb))
      if (HmGetLocalObjNum  (CoreDb,                    Token->Handle,
                             McStat,                   &LocalObjNum))
      if (MemGetObjNum      (CoreDb->ObjNumMap,         LocalObjNum,
                             McStat,                   &ObjNum))
      if (HmGetObjectMemLoc (CoreDb,                    Token->Handle,
                             McStat,                   &MemLoc))
#ifdef IMAGE_WITH_THAT
      if (ItSelf->That != NullPtr
      ||  MemLoc)
#else
      if (MemLoc)
#endif /* IMAGE_WITH_THAT  */
      {
          if (MemLoc == 0)
          if (TraceMsg (0, 
                      "***ERROR... Itself->That !=NullPtr && MemLoc == 0\n"))
              HmPutObjectMemLoc (CoreDbs[Token->DbId],  Token->Handle,
                                 (numtype )1,           McStat);

         *This  = (addrtype )ItSelf; 
      } else {
          if (Build_ByCpp)
              CppCreateObject   (ObjNum,           McStat,   &That);
          else
              C_CreateObject    (ObjNum,   AplId,  McStat,   &That);

          if (Normal(*Status))
          {
            *This  = That;
             C_FaxToThis (Token, *This, AplId,   McStat);
          }

      }

      if (Normal(*Status))
      if (Build_ByCpp)
      if (Ref_BaseClass)
         CppCastObject     (ObjNum, Ref_BaseClass, McStat,  This);

   }

TRACK(TrackBak,"C_RefToAddr\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~C++I                    ARRAY TRANS INTERFACE                            !
  +-----------------------------------------------------------------------+*/
boolean C_CreateArray    (tokentype    *Token,     numtype       AttrNum, 
                          indextype     UnitCount, sizetype      UnitSize,
                          ft F,lt Z,zz *Status,    
                          tokentype    *VchunkTkn, addrtype     *Array)
{
addrtype    This           = NullPtr;
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
addrtype   *FieldMap       = NullPtr;
typesize   *DbOffsets      = NullPtr;
typesize   *CppOffsets     = NullPtr;
numtype    *FieldAttrs     = NullPtr;
attrtype   *FieldTypes     = NullPtr;
count       i              = 0;
addrtype    DbObjectAddr   = NullPtr;
addrtype    CppObjectAddr  = NullPtr;
addrtype    CppStrucAddr   = NullPtr;
numtype    *RefObjNums     = NullPtr;      
indextype   NumMapFields   = 0;
boolean     Found          = False;
indextype   ExtendQty      = UnitCount;
tokentype   Anchor         = NullToken;
sizetype    ArraySize      = 0;
tokentype   TypeTkn        = NullToken;
typedesctype  *EnvType     = NullPtr;
objdesctype   *EnvObj      = NullPtr;
indextype   StackPtr       = 0;
typesize    Offset         = 0;

  TypeTkn.DbId    = RenvId;
  Anchor.DbId     = Token->DbId;
  Anchor.Handle   = 1;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  if (HmGetObjectAddr      (CoreDb,  Token->Handle,  McStat,   &This))
      OaRevokeCppObject    (Token,                   McStat);

  if (Normal(*Status))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))
  if (OaGetObject          (Token,                   McStat,   &DbObject))
  if (MemLoc)
      OaInvokeCppObject    (Token,   This,           McStat);

  if (Normal(*Status))
  if (MemGetAddr        (C_ObjMapDir,                       ObjNum,
                         McStat,               (addrtype *)&FieldMap))
  if (FieldMap)
  {
     DbOffsets       =  (typesize *)FieldMap[0];
     CppOffsets      =  (typesize *)FieldMap[1];
     FieldTypes      =  (attrtype *)FieldMap[3];
     FieldAttrs      =  (numtype  *)FieldMap[4];
     RefObjNums      =  (numtype  *)FieldMap[5];
     NumMapFields    =  (indextype )RefObjNums[0];

     i               = 1;
     while (Found   == False 
     &&     i       <= NumMapFields)
     {
       if (FieldAttrs[i] == AttrNum)
           Found = True;
       else
         ++i;
     }   
     if (!Found)
        *Status = Trans_ArrayNotOfClass;

  } else
    *Status = Trans_NullMap;

  if (Normal(*Status))
  {
     TypeTkn.Handle    = RefObjNums[i];
     if (OaGetObject   (&TypeTkn,  McStat, (addrtype *)&EnvType))
     if (EnvType->EnvType  == aEnvObj)
     {
        EnvObj         = (objdesctype *)EnvType;
        ArraySize      = EnvObj->ObjSize;
     } else 
        ArraySize      = EnvType->Size;

     if (Normal(*Status))
     if (FieldTypes[i] == aDblPtrAttr)
     {
        UnitCount     *= 3;
        UnitSize       = sizeof (handletype);

     } else if (FieldTypes[i] == aArrayAttr)   {
        if (ArraySize != UnitSize)
           *Status     = Trans_MixedArraySize;

     } else if (FieldTypes[i] != aStrHndlAttr) {
        if (ArraySize != UnitSize)
           *Status     = Trans_MixedArraySize;
     }
     if ( ArraySize )
	*Status = Env_Normal;
  }

  if (Normal(*Status))
  if (FieldTypes[i] != aStrHndlAttr)
  {
     if (VchunkCreate      (&Anchor,    UnitSize,      UnitCount, 
                            ExtendQty,  McStat,        VchunkTkn)) 
     if (VchunkGetAddress  (VchunkTkn,  McStat,        Array))
     {
        DbObjectAddr    =  (addrtype )((char *)DbObject  + DbOffsets[i]);
        MoveBytes         ((addrtype )&VchunkTkn->Handle,  DbObjectAddr, 
                           sizeof(handletype));
     }
     if (Normal(*Status))
     if (FieldTypes[i] == aArrayAttr
     ||  FieldTypes[i] == aVarrayAttr
     ||  FieldTypes[i] == aDblPtrAttr)
         VchunkPutStackPtr (VchunkTkn,  UnitCount, McStat, (addrtype *)Array);

  } else {
     *Array = (addrtype )calloc (1, UnitCount);

  }

  if (Normal(*Status))
  if (MemLoc)
  {
     CppObjectAddr   = (addrtype )((char *)This  + CppOffsets[i]);
     if (FieldTypes[i] == aVarrayAttr
     ||  FieldTypes[i] == aChunkAttr)
     {
       CppStrucAddr       = CppObjectAddr;

       /* VchunkTkn                                                        */
       MoveBytes  ((addrtype )VchunkTkn,   CppObjectAddr,  sizeof(tokentype));
       /* Vsize                                                            */
       Offset       += sizeof (tokentype);
       CppStrucAddr  = (addrtype )((char *)CppObjectAddr + Offset);
       MoveBytes  ((addrtype )&UnitCount,  CppStrucAddr,   sizeof(indextype));
       /* Varray                                                           */
       Offset       += sizeof (indextype);
       Ut_AlignMember  (aAddr,      sizeof (addrtype), &Offset);

       CppStrucAddr  = (addrtype )((char *)CppObjectAddr + Offset);
       MoveBytes       ((addrtype )Array,  CppStrucAddr,   sizeof(addrtype));
       /* VstackPtr                                                        */
       if (FieldTypes[i] == aChunkAttr)
       {
          Offset       += sizeof (addrtype);
          CppStrucAddr  = (addrtype )((char *)CppObjectAddr  + Offset);
          MoveBytes  ((addrtype )&StackPtr,  CppStrucAddr,  sizeof(indextype));
       }

     } else {
       MoveBytes  ((addrtype )Array,       CppObjectAddr,  sizeof(addrtype));
     }

  }

TRACK(TrackBak,"C_CreateArray\n");
return (STAT);
}


boolean C_CreateSubArray (tokentype    *Token,     
                          numtype       AttrNum,  indextype    ArrayIndex,
                          indextype     UnitCount,sizetype     UnitSize, 
                          ft F,lt Z,zz *Status,
                          tokentype    *VchunkTkn,addrtype    *Array)
{
addrtype    This           = NullPtr;
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
addrtype   *FieldMap       = NullPtr;
typesize   *DbOffsets      = NullPtr;
numtype    *FieldAttrs     = NullPtr;
count       i              = 0;
addrtype    DbObjectAddr   = NullPtr;
numtype    *RefObjNums     = NullPtr;      
indextype   NumMapFields   = 0;
boolean     Found          = False;
indextype   ExtendQty      = UnitCount;
tokentype   Anchor         = NullToken;
sizetype    ArraySize      = 0;
tokentype   TypeTkn        = NullToken;
typedesctype  *EnvType     = NullPtr;
tokentype   BaseTkn        = NullToken;
addrtype   *BaseArray      = NullPtr;
handletype *ArrayHndls     = NullPtr;
handletype *SubArray       = NullPtr;
indextype   NumSubArrays   = 0;

  BaseTkn.DbId    = Token->DbId;
  TypeTkn.DbId    = RenvId;
  Anchor.DbId     = Token->DbId;
  Anchor.Handle   = 1;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  if (HmGetObjectAddr      (CoreDb,  Token->Handle,  McStat,   &This))
      OaRevokeCppObject    (Token,                   McStat);

  if (Normal(*Status))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))
  if (OaGetObject          (Token,                   McStat,   &DbObject))
  if (MemLoc)
      OaInvokeCppObject    (Token,   This,           McStat);

  if (Normal(*Status))
  if (MemGetAddr        (C_ObjMapDir,                       ObjNum,
                         McStat,               (addrtype *)&FieldMap))
  if (FieldMap)
  {
     DbOffsets       =  (typesize *)FieldMap[0];
     FieldAttrs      =  (numtype  *)FieldMap[4];
     RefObjNums      =  (numtype  *)FieldMap[5];
     NumMapFields    =  (indextype )RefObjNums[0];

     i               = 1;
     while (Found   == False 
     &&     i       <= NumMapFields)
     {
       if (FieldAttrs[i] == AttrNum)
           Found = True;
       else
         ++i;
     }   
     if (!Found)
        *Status = Trans_ArrayNotOfClass;

  } else
    *Status = Trans_NullMap;

  if (Normal(*Status))
  {
     TypeTkn.Handle     = RefObjNums[i];
     if (OaGetObject   (&TypeTkn,  McStat, (addrtype *)&EnvType))
     if ((ArraySize = EnvType->Size) != UnitSize)
     {
#ifndef __RISC64__
        *Status     = Trans_MixedArraySize;
        sprintf (Msg, " C_CreateSubArray:: [%4u:%6u] Attr=%4u; Index=%4u\n",
                 Token->DbId, Token->Handle, AttrNum, ArrayIndex);
        TraceMsg (0,  Msg);
        sprintf (Msg, "                   SchemaSize(%4u) != UnitSize(%4u)\n", 
                 ArraySize, UnitSize);
        TraceMsg (0,  Msg);
#else
        if (ArraySize == 0)
           *Status     = Trans_MixedArraySize;

#endif /*  __RISC64__  */
     } else {
        DbObjectAddr    =  (addrtype )((char *)DbObject  + DbOffsets[i]);
        MoveBytes         (DbObjectAddr, (addrtype )&BaseTkn.Handle, 
                           sizeof(handletype));
        if (BaseTkn.Handle == 0)
        {
          *Status = Trans_NullBaseArray;
        }
     }
  }

  if (Normal(*Status))
  if (VchunkGetAddress  (&BaseTkn,   McStat,  (addrtype *)&BaseArray))
  if (VchunkGetStackPtr (&BaseTkn,   McStat,       &NumSubArrays))
  if (NumSubArrays)
  if (VchunkCreate      (&Anchor,    UnitSize,      UnitCount, 
                         ExtendQty,  McStat,        VchunkTkn)) 
  if (VchunkPutStackPtr (VchunkTkn,  UnitCount,     McStat,     Array))
  if ((NumSubArrays /= 3)   > 0)
  {
     BaseArray[ArrayIndex]  = *Array;
     ArrayHndls             = (handletype *)BaseArray;
     SubArray               = (handletype *)&ArrayHndls[NumSubArrays * 2];
     SubArray[ArrayIndex]   = VchunkTkn->Handle;

  } else
     *Status  = Trans_NullBaseArraySize;

TRACK(TrackBak,"C_CreateSubArray\n");
return (STAT);
}


 
boolean C_CreateFieldArray
                         (tokentype    *Token,     numtype       AplId,
                          fieldspec    *FieldTkns, numtype       SubLevel,
                          indextype     UnitCount, sizetype      UnitSize,
                          ft F,lt Z,zz *Status,    
                          tokentype    *VchunkTkn, addrtype     *Array)
{
addrtype    This           = NullPtr;
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
typesize    DbOffset       = NullPtr;
typesize    CppOffset      = NullPtr;
numtype     AttrId         = NullPtr;
attrtype    AttrType       = aAttr;
numtype     RefObjNum      = NullPtr;      
sizetype    FieldSize      = 0;
typetype    FieldType      = aRefObj;
addrtype    DbObjectAddr   = NullPtr;
addrtype    CppObjectAddr  = NullPtr;
indextype   ExtendQty      = UnitCount;
tokentype   Anchor         = NullToken;
sizetype    ArraySize      = 0;
tokentype   TypeTkn        = NullToken;
typedesctype  *EnvType     = NullPtr;
indextype   StackPtr       = 0;
sizetype    DbBase         = 0;
sizetype    CppBase        = 0;

  TypeTkn.DbId    = RenvId;
  Anchor.DbId     = Token->DbId;
  Anchor.Handle   = 1;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  if (HmGetObjectAddr      (CoreDb,  Token->Handle,  McStat,   &This))
      OaRevokeCppObject    (Token,                   McStat);

  if (Normal(*Status))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))
  if (OaGetObject          (Token,                   McStat,   &DbObject))

  if (Normal(*Status))
  if (Trans_FetchFieldOffset (Token,      AplId,      FieldTkns,  SubLevel,
                              McStat,    &FieldSize, &AttrId,    &AttrType, 
                             &RefObjNum, &FieldType, &DbOffset,  &CppOffset,
                             &DbBase,    &CppBase))
  if (MemLoc)
      OaInvokeCppObject      (Token,      This,       McStat);
  if (Normal(*Status))
  {
     TypeTkn.Handle    = RefObjNum;
     if (OaGetObject   (&TypeTkn,  McStat, (addrtype *)&EnvType))
     if (AttrType    == aDblPtrAttr)
     {
        UnitCount     *= 3;
        UnitSize       = sizeof (handletype);

     } else if (AttrType != aStrHndlAttr) {
        if ((ArraySize = EnvType->Size)    != UnitSize)
           *Status     = Trans_MixedArraySize;
     }
  }

  if (Normal(*Status))
  if (AttrType   != aStrHndlAttr)
  {
     if (VchunkCreate      (&Anchor,    UnitSize,      UnitCount, 
                            ExtendQty,  McStat,        VchunkTkn)) 
     if (VchunkGetAddress  (VchunkTkn,  McStat,        Array))
     {
        DbObjectAddr    =  (addrtype )((char *)DbObject  + DbOffset);
        MoveBytes         ((addrtype )&VchunkTkn->Handle,  DbObjectAddr, 
                           sizeof(handletype));
     }
     if (Normal(*Status))
     if (AttrType == aArrayAttr
     ||  AttrType == aDblPtrAttr)
         VchunkPutStackPtr (VchunkTkn,  UnitCount, McStat, (addrtype *)Array);

  } else {
     *Array = (addrtype )calloc (1, UnitCount);

  }

  if (Normal(*Status))
  if (MemLoc)
  {
     CppObjectAddr   = (addrtype )((char *)This  + CppOffset);
     if (AttrType == aVarrayAttr
     ||  AttrType == aChunkAttr)
     {
       /* VchunkTkn                                                        */
       MoveBytes  ((addrtype )VchunkTkn,   CppObjectAddr,  sizeof(tokentype));
       /* Vsize                                                            */
       CppObjectAddr = (addrtype )((char *)CppObjectAddr + sizeof(tokentype));
       MoveBytes  ((addrtype )&UnitCount,  CppObjectAddr,  sizeof(indextype));
       /* Varray                                                           */
       CppObjectAddr = (addrtype )((char *)CppObjectAddr + sizeof(indextype));
       MoveBytes       ((addrtype )Array,  CppObjectAddr,  sizeof(addrtype));
       /* VstackPtr                                                        */
       if (AttrType == aChunkAttr)
       {
          CppObjectAddr = (addrtype )((char *)CppObjectAddr
                        + sizeof(addrtype));
          MoveBytes  ((addrtype )&StackPtr,  CppObjectAddr,  sizeof(addrtype));
       }

     } else {
       MoveBytes  ((addrtype )Array,       CppObjectAddr,  sizeof(addrtype));
     }
  }

TRACK(TrackBak,"C_CreateFieldArray\n");
return (STAT);
}


boolean C_CreateFieldSubArray 
                         (tokentype    *Token,     numtype       AplId,
                          fieldspec    *FieldTkns, numtype       SubLevel,
                          indextype     ArrayIndex,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,
                          tokentype    *VchunkTkn, addrtype     *Array)
{
addrtype    This           = NullPtr;
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
typesize    DbOffset       = NullPtr;
typesize    CppOffset      = NullPtr;
numtype     AttrId         = NullPtr;
attrtype    AttrType       = aAttr;
numtype     RefObjNum      = NullPtr;      
sizetype    FieldSize      = 0;
typetype    FieldType      = aRefObj;
addrtype    DbObjectAddr   = NullPtr;
indextype   ExtendQty      = UnitCount;
tokentype   Anchor         = NullToken;
sizetype    ArraySize      = 0;
tokentype   TypeTkn        = NullToken;
typedesctype  *EnvType     = NullPtr;
tokentype   BaseTkn        = NullToken;
addrtype   *BaseArray      = NullPtr;
handletype *ArrayHndls     = NullPtr;
handletype *SubArrays      = NullPtr;
indextype   NumSubArrays   = 0;
sizetype    DbBase         = 0;
sizetype    CppBase        = 0;

  BaseTkn.DbId    = Token->DbId;
  TypeTkn.DbId    = RenvId;
  Anchor.DbId     = Token->DbId;
  Anchor.Handle   = 1;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  if (HmGetObjectAddr      (CoreDb,  Token->Handle,  McStat,   &This))
      OaRevokeCppObject    (Token,                   McStat);

  if (Normal(*Status))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ObjNum))
  if (OaGetObject          (Token,                   McStat,   &DbObject))

  if (Normal(*Status))
  if (Trans_FetchFieldOffset (Token,      AplId,      FieldTkns,  SubLevel,
                              McStat,    &FieldSize, &AttrId,    &AttrType, 
                             &RefObjNum, &FieldType, &DbOffset,  &CppOffset,
                             &DbBase,    &CppBase))
  if (MemLoc)
      OaInvokeCppObject      (Token,      This,       McStat);

  if (Normal(*Status))
  {
     TypeTkn.Handle     = RefObjNum;
     if (OaGetObject   (&TypeTkn,  McStat, (addrtype *)&EnvType))
     if ((ArraySize = EnvType->Size) != UnitSize)
     {
        *Status     = Trans_MixedArraySize;
     } else {
        DbObjectAddr    =  (addrtype )((char *)DbObject  + DbOffset);
        MoveBytes         (DbObjectAddr, (addrtype )&BaseTkn.Handle, 
                           sizeof(handletype));
        if (BaseTkn.Handle == 0)
        {
          *Status = Trans_NullBaseArray;
        }
     }
  }

  if (Normal(*Status))
  if (VchunkGetAddress  (&BaseTkn,   McStat,  (addrtype *)&BaseArray))
  if (VchunkGetStackPtr (&BaseTkn,   McStat,       &NumSubArrays))
  if (NumSubArrays)
  if (VchunkCreate      (&Anchor,    UnitSize,      UnitCount, 
                         ExtendQty,  McStat,        VchunkTkn)) 
  if (VchunkPutStackPtr (VchunkTkn,  UnitCount,     McStat,     Array))
  if ((NumSubArrays /= 3)   > 0)
  {
     ArrayHndls             = (handletype *)BaseArray;
     BaseArray[ArrayIndex]  = *Array;
     SubArrays              = (handletype *)&ArrayHndls[NumSubArrays * 2];
     SubArrays[ArrayIndex]  = VchunkTkn->Handle;

  } else
     *Status  = Trans_NullBaseArraySize;

TRACK(TrackBak,"C_CreateFieldSubArray\n");
return (STAT);
}


 
/*+-----------------------------------------------------------------------+
~C++I                    C++ OBJECT  INTERFACE                            !
  +-----------------------------------------------------------------------+*/
boolean C_CreateObject (numtype       ObjNum, numtype       AplId,
                        ft F,lt Z,zz *Status, addrtype     *Object)
{
 void*          (*pmf1)(size_t)  = NullPtr;
 size_t         ClassSize        = 0;

   if (ShellGetObjCode  (ObjNum, McStat, (funcptrtype  *)&pmf1,  &ClassSize))
   if (pmf1)
     *Object       = (addrtype )(*pmf1)(ClassSize);
   else
      Core0MoreCore ((bytessizetype    )ClassSize,        McStat, Object);

TRACK(TrackBak,"C_CreateObject\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF TRANS01.C                                !
  +-----------------------------------------------------------------------+*/
