/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* TRANS10.c */
#define  TRANS10
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
#include "shell.h"
#include "trans.h"
#include "api.h"
 
#define        That_AttrNum           (numtype )1

#define DumpChunkChunk     Mem_DumpChunkChunk

extern boolean ImageInitClassMap
                             (numtype       ClassId, size_t        ObjSize,
                              numtype       AplId,   ft F,lt Z,zz *Status);
 
extern boolean CppRefToTkn    (addrtype      ThisObj, numtype      ObjNum, 
                               ft F,lt Z,zz *Status,  tokentype   *ObjTkn);

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
boolean    C_ReFaxToDb   (tokentype    *Token,   addrtype      Object,
                          boolean       Release, boolean       FreeImage,
                          numtype       AplId,  
                          ft F,lt Z,zz *Status,  bytessize    *ImageSize)
{
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    DbObject       = NullPtr;
addrtype   *FieldMap       = NullPtr;
typesize   *DbOffsets      = NullPtr;
typesize   *CppOffsets     = NullPtr;
typesize   *FieldSizes     = NullPtr;
attrtype   *FieldTypes     = NullPtr;
typetype   *TypeTypes      = NullPtr;
numtype    *FieldAttrs     = NullPtr;      
numtype    *RefObjNums     = NullPtr;      
numtype    *ArrayAttrs     = NullPtr;      
indextype   NumArrayAttrs  = 0;
indextype   NumMapFields   = 0;
count       i              = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    DbObjectAddr   = NullPtr;
longaddr    NullValue      = 0;
statustype  DumpStatus     = Env_Normal;
boolean     PureDbStruc    = False;
typesize    ObjSize        = 0;
sizetype    TransferSize   = 0;
indextype   ThatIndex      = 0;
tokentype   ObjDescTkn;
objdesctype *ObjDesc       = NullPtr;
addrtype    CppRefObj      = NullPtr;
tokentype  *CppRefTkn      = NullPtr;
addrtype   *CppRefPtr      = NullPtr;
tokentype  *DbRefTkn       = NullPtr;
addrtype    DbRefObj       = NullPtr;
tokentype   AttrTkn        = NullToken;
attrdesctype *EnvAttr      = NullPtr;
numtype     LocalObj       = 0;
objheader  *ObjHdr         = NullPtr;
longtype    LongIndex      = 0;
indextype   StrIndex       = 0;
char       *StrAddr        = NullPtr;
int         k              = 0;
indextype   AttrCount      = 0;
typesize    AttrDbSize     = 0;
typesize    Attr01Size     = 0;
typesize    BaseDbOffset   = 0;
typesize    Base01Offset   = 0;

tokentype      VchunkTkn      = NullToken;
tokentype      DblPtrTkn      = NullToken;
numtype        InCore         = 0;
vchunktkntype  DblVchunk;
vchunktkntype  Vchunk;
chunkstruc     ChunkStruc;  
int            j              = 0;
handletype    *ArrayHndls     = NullPtr;
handletype    *SubArrays      = NullPtr;
addrtype      *Arrays         = NullPtr;
indextype      Index          = 0;
indextype      NumSubArrays   = 0;
numtype        IndexInCore    = 0;

static boolean        FirstTime             = True;
static boolean        FirstTime_ObjRef      = True;

  AttrTkn.DbId      = Token->DbId;
  VchunkTkn.DbId    = Token->DbId;
  DblPtrTkn.DbId    = Token->DbId;

  if (Normal(*Status))
  if (TmGetCoreDb       (Token,                   McStat,        &CoreDb))
  if (HmGetObjectMemLoc (CoreDb,  Token->Handle,  McStat,        &MemLoc))
  if (MemLoc)
  {
      HmGetObjectAddr   (CoreDb,  Token->Handle,  McStat,        &Object);
  } else
    return (STAT);

  if (Normal(*Status))
  if (OaRevokeCppObject (Token,                   McStat))
  if (HmGetObjNum       (CoreDb,  Token->Handle,  McStat,        &ObjNum))
  if (OaGetObject       (Token,                   McStat,        &DbObject))

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
     ArrayAttrs   =  (numtype  *)FieldMap[6];
     TypeTypes    =  (typetype *)FieldMap[7];

     NumMapFields =  (indextype )RefObjNums[0];
     NumArrayAttrs=  (indextype )ArrayAttrs[0];
     PureDbStruc  =  (boolean   )DbOffsets[0];
     ObjSize      =  (typesize  )CppOffsets[0];
     ThatIndex    =  (indextype )TypeTypes[0];

    *ImageSize    =  (bytessize )CppOffsets[0];

     CppObjectAddr    = (addrtype )((char *)Object + CppOffsets[ThatIndex]);
  } else
    *Status = Trans_NullMap;


  if (Normal(*Status))
  if (PureDbStruc   == False
  ||  NumArrayAttrs  > 0)
  {
    while (++i <= NumMapFields
    &&     Normal(*Status)) 
    {

       TransferSize       = FieldSizes[i];

       CppObjectAddr      = (addrtype )((char *)Object   + CppOffsets[i]);
       DbObjectAddr       = (addrtype )((char *)DbObject + DbOffsets[i]);

       if (FieldAttrs[i] == That_AttrNum)
         CppObjectAddr    = &NullValue;

       if ((AttrCount    = AttrTypeSizes[FieldTypes[i]]) != 1)
       {
          Attr01Size     = AttrCount;
          AttrDbSize     = AttrCount;
          AttrCount      = FieldSizes[i] / AttrCount;

          if (FieldTypes[i] == aSetHeadAttr
          ||  FieldTypes[i] == aSetNodeAttr)
          {
             AttrCount      = 1;
          }

       } else
          Attr01Size     = 0;

       BaseDbOffset      = 0;
       Base01Offset      = 0;
       k                 = 0;

       if (FieldAttrs[i] != That_AttrNum)
       while (k++ < AttrCount)
       {
         CppObjectAddr   = (addrtype )((char *)CppObjectAddr + Base01Offset);
         DbObjectAddr    = (addrtype )((char *)DbObjectAddr  + BaseDbOffset);

         switch (FieldTypes[i])
         {

          case aObjRefAttr   :

             if (FirstTime_ObjRef)
             if (ClassBug          == True)
                FirstTime_ObjRef    = False;
             CppRefPtr = (addrtype *)((char *)Object    + CppOffsets[i]);
             DbRefTkn  = (tokentype *)((char *)DbObject  + DbOffsets[i]);

             CppRefObj = *CppRefPtr;

             if (((tokentype *)DbRefTkn)->Handle == 0)
             if (CppRefObj       != NullPtr)
/*
//           if ( (long) CppRefObj        > NullPtr)
*/
					C_RefToTkn  (CppRefObj,  RefObjNums[i],     AplId,
									 McStat,           (tokentype *)DbRefTkn);
				 else {
					 OaDumpObject (0,  Token, Dump_Status);
				 }

				 if (FirstTime_ObjRef)
				 {
					 FirstTime_ObjRef   = False;
				 }
					break;

			 case aPortRefAttr  :
					break;

			 case aTknRefAttr   :
				 DbRefTkn  = (tokentype *)((char *)DbObject  + DbOffsets[i]);
				 if (FirstTime)
				 if (ClassBug   == True)
					 FirstTime    = False;
				 CppRefPtr = (addrtype *)CppObjectAddr;

				 CppRefObj = *CppRefPtr;
				 CppRefTkn = (tokentype *)((char *)CppObjectAddr
							  + sizeof (addrtype));

				 DbRefObj  = (addrtype  )DbObjectAddr;

				 if (CppRefTkn->DbId && CppRefTkn->Handle)
				 {
					 MoveBytes     (CppRefTkn,   DbObjectAddr,  sizeof(tokentype));

				 } else if (CppRefObj != NullPtr) {

					 if (EnvFetchAttrHandle (FieldAttrs[i], 
													 McStat,        &AttrTkn.Handle))
					 if (OaGetObject        (&AttrTkn, 
													 McStat,    (addrtype *)&EnvAttr))
					 {
						 ObjDescTkn.DbId    = RenvId;
						 ObjDescTkn.Handle  = EnvAttr->BuildDesc;
					 }
					 if (Normal(*Status))
					 if (OaGetObject   (&ObjDescTkn, McStat, (addrtype *)&ObjDesc)) 
						  C_RefToTkn   (CppRefObj,  ObjDesc->ObjNum, AplId,
											  McStat,        (tokentype *)DbObjectAddr);

				 } else  {
					 MoveBytes     (&NullToken,  DbObjectAddr,  sizeof(tokentype));

				 }

				 if (FirstTime)
				 {
					 FirstTime   = False;
				 }
				 break;

			 case aStrHndlAttr  :
					LongIndex = *(longtype *)DbObjectAddr;
					StrIndex  = LongIndex;
					StrAddr   = *(char **)CppObjectAddr;

					if (HmGetLocalObjNum (CoreDb, Token->Handle, McStat, &LocalObj))
					if (OmGetObjHdr      (Token,  LocalObj,      McStat, &ObjHdr))
					if (StrAddr)
							OaDumpObject (0, Token, McStat);

					if (StrAddr)
					if (StrIndex == 0)
					{
						if (OaPutString (Token->DbId,   ObjHdr,   StrAddr,
											  McStat,                 &StrIndex))
							 MoveBytes   ((addrtype )&StrIndex,    DbObjectAddr,
												sizeof (indextype));
					} else
						 MemPutString   (ObjHdr->TextChunk,       StrIndex,
											  (char *)CppObjectAddr,   McStat);

					if (StrAddr != NullPtr
					&&  Release == True)
					{
						Core0_FreeString (StrAddr,   McStat);
					}
					break;


			 case aChunkAttr    :
			 case aVarrayAttr   :
          case aArrayAttr    :
          case aVchunkAttr   :
              if (FieldTypes[i] == aArrayAttr)
              {
                  VchunkTkn.Handle  =  *(handletype *)DbObjectAddr;
                  VchunkTkn.DbId    =  Token->DbId;
                  TransferSize      =  sizeof (handletype);

              } else if(FieldTypes[i] == aVchunkAttr)  {
                  MoveBytes (CppObjectAddr, DbObjectAddr, FieldSizes[i]);
                  VchunkTkn         =  *(tokentype *)DbObjectAddr;
                  TransferSize      =  FieldSizes[i];

              } else {
                  VchunkTkn =  *(tokentype *)CppObjectAddr;
                  MoveBytes (CppObjectAddr, DbObjectAddr, FieldSizes[i]);
                  TransferSize      =  FieldSizes[i];
              }

              if (VchunkTkn.DbId   > 0
              &&  VchunkTkn.Handle > 0)
              {
                 if (DbmGetVchunkTkn (&VchunkTkn, McStat,      &Vchunk))
                 if ((InCore = Vchunk.InCore) > 0)
#ifdef   __FREEZE_VCHUNKS__
                     DbmCommitVchunk (&VchunkTkn,   Release,    McStat);
#else
                 if (FileOutChunk    (&Vchunk.InCore,
                                      &(CoreDb->BlkHdr),        Release,    
                                      McStat,                  &Vchunk.OnDisk))
                     DbmPutVchunkTkn (&VchunkTkn,   &Vchunk,    McStat);
#endif /* __FREEZE_VCHUNKS__  */

              }
              break;


          case aGrpAttr    :
          case aVgrpAttr   :
              TransferSize      = FieldSizes[i];
              MoveBytes (CppObjectAddr, DbObjectAddr, FieldSizes[i]);
              VchunkTkn         =  *(tokentype *)DbObjectAddr;
              break;


          case aDblPtrAttr   :
              TransferSize      = 4;
              DblPtrTkn.Handle  =  *(handletype *)DbObjectAddr;
              DblPtrTkn.DbId    =  Token->DbId;

              if (DblPtrTkn.Handle > 0)
              if (DbmGetVchunkTkn (&DblPtrTkn,   McStat,     &DblVchunk))
              if ((InCore          = DblVchunk.InCore) > 0)
              {
                 if (VchunkGetAddress  
                                  (&DblPtrTkn,
                                   McStat,      (addrtype *)&Arrays))
                 if (VchunkGetVstruc 
                                  (&DblPtrTkn,  McStat,     &ChunkStruc))
                 {
                 ArrayHndls    = (handletype *)Arrays;
                 NumSubArrays  = ChunkStruc.StackPtr / 3;
                 SubArrays     = (handletype *)&ArrayHndls[NumSubArrays * 2];
                 }

                 j = -1 ;
                 while (Normal(*Status)
                 &&     ++j      < NumSubArrays)
                 {
#ifdef __BCC_45__
                    memcpy  (&Index, 
                             &SubArrays[j], sizeof (handletype));
#else
                    Index            = SubArrays[j];
#endif
                    VchunkTkn.Handle = Index;
                    if (Index)
                    if (DbmGetVchunkTkn 
                                  (&VchunkTkn,     McStat,     &Vchunk))
                    if ((IndexInCore  = Vchunk.InCore) >  0)
#ifdef   __FREEZE_VCHUNKS__
                       DbmCommitVchunk (&VchunkTkn,   Release,  McStat);
#else
                    if (FileOutChunk 
                                  (&Vchunk.InCore,
                                   &(CoreDb->BlkHdr),           Release,    
                                   McStat,              &Vchunk.OnDisk))
                       DbmPutVchunkTkn (&VchunkTkn,     &Vchunk,McStat);
#endif /* __FREEZE_VCHUNKS__  */

                 }

                 if (Normal(*Status))
#ifdef   __FREEZE_VCHUNKS__
                     DbmCommitVchunk 
                                  (&DblPtrTkn,   Release,  McStat);
#else
                 if (FileOutChunk (&DblVchunk.InCore,
                                   &(CoreDb->BlkHdr),            Release,    
                                   McStat,            &DblVchunk.OnDisk))
                     DbmPutVchunkTkn 
                                  (&DblPtrTkn,        &DblVchunk,McStat);
#endif /* __FREEZE_VCHUNKS__  */

              }
              break;

          case aMapRefAttr   :
               C_MapRefToDb   (Token->DbId,  RefObjNums[i],    FieldSizes[i],
                               DbObjectAddr, CppObjectAddr,    AplId,
                               Release,      McStat);
               if (DeBug || OaBug || ClassBug)
                  TraceMsg (0, "   End of Embedded Object\n\n");
               break;

          case aSetNodeAttr  :
          case aSetHeadAttr  :

          case aAttr         :
          case aSlotHndlAttr :
          case aSlotTokenAttr:
               default       :

                TransferSize = FieldSizes[i];
                MoveBytes (CppObjectAddr, DbObjectAddr, FieldSizes[i]);
               break;
         }

         if (DeBug || OaBug || ClassBug)
         switch (FieldTypes[i])
         {
          case aObjRefAttr   :
          case aPortRefAttr  :
          case aTknRefAttr   :
          case aMapRefAttr   :
               break;

          default            :
	       break;
         }

         Base01Offset  += Attr01Size;
         BaseDbOffset  += AttrDbSize;
       }

    }

  } else {
     MoveBytes   (Object,   DbObject,   ObjSize);

  }

/* ReAdjust That Address                                                   */
  if (Normal(*Status))
  if (ThatIndex)
  {
     CppObjectAddr    = &NullValue;
     DbObjectAddr     = (addrtype )((char *)DbObject + DbOffsets[ThatIndex]);
#ifdef IMAGE_WITH_THAT
     MoveBytes   (CppObjectAddr, DbObjectAddr,   sizeof (addrtype));
#endif /* IMAGE_WITH_THAT  */

  }

  if (Normal(*Status))
     OaUpdateObject    (Token,           McStat);

  if (ClassBug || DeBug || OaBug)
     OaDumpObject  (0, Token, Dump_Status);

  DbObject   = Object;
  if (Normal(*Status))
  if (!Release)
     OaInvokeCppObject (Token,  Object,  McStat);
  else {
     if (FreeImage)
     {
        if (AplId == Cpp_Apl)
           Core0FreeCoreSpace  (&Object, (bytessize   )ObjSize, McStat);
        else
           Core0FreeCoreSpace  (&Object, (bytessize   )ObjSize, McStat);

     } 
  }
TRACK(TrackBak,"C_ReFaxToDb\n");
return (STAT);
}

boolean    C_GroupArrays (tokentype    *Token,   addrtype      Object,
                          boolean       Release, numtype       AplId,
                          ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"C_GroupArrays\n");
return (STAT);
}

boolean    C_MapRefToDb   (idtype        DbId,      numtype       ObjNum, 
                           sizetype      MapSize,   addrtype      DbObject,
                           addrtype      Object,    numtype       AplId,
                           boolean       Release,   ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
addrtype   *FieldMap       = NullPtr;
typesize   *DbOffsets      = NullPtr;
typesize   *CppOffsets     = NullPtr;
typesize   *FieldSizes     = NullPtr;
attrtype   *FieldTypes     = NullPtr;
numtype    *FieldAttrs     = NullPtr;      
numtype    *RefObjNums     = NullPtr;      
numtype    *ArrayAttrs     = NullPtr;      
indextype   NumArrayAttrs  = 0;
indextype   NumMapFields   = 0;
count       i              = 0;
addrtype    BaseCppAddr    = NullPtr;
addrtype    BaseDbAddr     = NullPtr;
sizetype    DbObjSize      = 0;
addrtype    CppObjectAddr  = NullPtr;
addrtype    DbObjectAddr   = NullPtr;
boolean     PureDbStruc    = False;
typesize    ObjSize        = 0;
sizetype    TransferSize   = 0;
tokentype   ObjDescTkn;
objdesctype *ObjDesc     = NullPtr;
addrtype    CppRefObj    = NullPtr;
tokentype  *CppRefTkn    = NullPtr;
addrtype   *CppRefPtr    = NullPtr;
addrtype    DbRefTkn     = NullPtr;
addrtype    DbRefObj     = NullPtr;
attrdesctype *EnvAttr    = NullPtr;
numtype     LocalObj       = 0;
objheader  *ObjHdr         = NullPtr;
longtype    LongIndex      = 0;
indextype   StrIndex       = 0;
char       *StrAddr        = NullPtr;
int         l              = 0;
indextype   NumMaps        = 0;
int         k              = 0;
indextype   AttrCount      = 0;
typesize    AttrDbSize     = 0;
typesize    Attr01Size     = 0;
typesize    BaseDbOffset   = 0;
typesize    Base01Offset   = 0;

  if (Normal(*Status))
  if (MemGetAddr        (C_ObjMapDir,                            ObjNum,
                         McStat,                     (addrtype *)&FieldMap))
  if (FieldMap == NullPtr)
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
  if (FieldMap)
  {
     DbOffsets    =  (typesize *)FieldMap[0];
     CppOffsets   =  (typesize *)FieldMap[1];
     FieldSizes   =  (typesize *)FieldMap[2];
     FieldTypes   =  (attrtype *)FieldMap[3];
     FieldAttrs   =  (numtype  *)FieldMap[4];
     RefObjNums   =  (numtype  *)FieldMap[5];
     ArrayAttrs   =  (numtype  *)FieldMap[6];

     NumMapFields =  (indextype )RefObjNums[0];
     NumArrayAttrs=  (indextype )ArrayAttrs[0];
     PureDbStruc  =  (boolean   )DbOffsets[0];
     ObjSize      =  (typesize  )CppOffsets[0];

  }

  if (Normal(*Status))
  if (!PureDbStruc
  ||  NumArrayAttrs > 0)
  {
     EnvFetchObjSize (ObjNum,  McStat, &DbObjSize);
     NumMaps       =  MapSize / DbObjSize;

     l             = 0;
     while (Normal(*Status)
     &&     ++l   <= NumMaps)
     {
       BaseDbAddr  = (addrtype )((char *)DbObject  + 
                                  ((l-1) * DbObjSize));
       BaseCppAddr = (addrtype )((char *)Object
                                + ((l-1) * ObjSize));
       i           = 0;
       while (++i <= NumMapFields
       &&     Normal(*Status)) 
       {
         TransferSize      = FieldSizes[i];

         DbObjectAddr      = (addrtype )((char *)BaseDbAddr  + DbOffsets[i]);
         CppObjectAddr     = (addrtype )((char *)BaseCppAddr + CppOffsets[i]);

         if ((AttrCount    = AttrTypeSizes[FieldTypes[i]]) != 1)
         {
            Attr01Size     = AttrCount;
            AttrDbSize     = AttrCount;
            AttrCount      = FieldSizes[i] / AttrCount;
         } else
            Attr01Size     = 0;

         BaseDbOffset      = 0;
         Base01Offset      = 0;
         k                 = 0;

         if (FieldAttrs[i] != That_AttrNum)
         while (k++ < AttrCount)
         {
           DbObjectAddr  = (addrtype )((char *)DbObjectAddr  + BaseDbOffset);
           CppObjectAddr = (addrtype )((char *)CppObjectAddr + Base01Offset);

           switch (FieldTypes[i])
           {
            case aObjRefAttr   :

               CppRefPtr = (addrtype *)((char *)Object    + CppOffsets[i]);
               DbRefTkn  = (addrtype  )((char *)DbObject  + DbOffsets[i]);

               CppRefObj = *CppRefPtr;

               if ((CppRefObj = *CppRefPtr) != 0)
                 C_RefToTkn  (CppRefObj,  RefObjNums[i],  AplId,
                              McStat,        (tokentype *)DbRefTkn);
                 break;


            case aPortRefAttr  :
                 break;

            case aTknRefAttr   :

               CppRefPtr = (addrtype *)CppObjectAddr;

               CppRefObj = *CppRefPtr;
               CppRefTkn = (tokentype *)((char *)CppObjectAddr
                         + sizeof (addrtype));

               DbRefObj  = (addrtype  )DbObjectAddr;

               if (CppRefTkn->DbId && CppRefTkn->Handle)
               {
                  MoveBytes     (CppRefTkn,   DbObjectAddr,  sizeof(tokentype));

               } else if (CppRefObj != NullPtr) {
                  ObjDescTkn.DbId    = RenvId;
                  ObjDescTkn.Handle  = EnvAttr->BuildDesc;

                  if (OaGetObject (&ObjDescTkn, McStat, (addrtype *)&ObjDesc))

                      C_RefToTkn  (CppRefObj,   ObjDesc->ObjNum,   AplId,
                                   McStat,      (tokentype *)DbObjectAddr);

               } else  {
                  MoveBytes     (&NullToken,  DbObjectAddr,  sizeof(tokentype));

               }
               break;

            case aStrHndlAttr  :
                 LongIndex = *(longtype *)DbObjectAddr;
                 StrIndex  = LongIndex;
                 StrAddr   = *(char **)CppObjectAddr;
                 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap,  ObjNum, 
                                        McStat,                 &LocalObj))
                 if (MemGetObjHdr      (CoreDb->ObjHdrDir,       LocalObj,
                                        McStat,     (addrtype *)&ObjHdr))
                 if (StrIndex == 0)
                 {
                    if (OaPutString    (DbId,   ObjHdr,          StrAddr,
                                        McStat,                 &StrIndex))
                        MoveBytes      ((addrtype )&LongIndex,   DbObjectAddr,
                                        sizeof (indextype));
                 } else
                     MemPutString   (ObjHdr->TextChunk,         StrIndex,
                                     (char *)CppObjectAddr,     McStat);

                 break;


            case aVchunkAttr   :
            case aChunkAttr    :

            case aArrayAttr    :
            case aVarrayAttr   :
            case aDblPtrAttr   :
                   TransferSize = 0;
                 break;


            case aMapRefAttr   :
                 C_MapRefToDb  (DbId,         RefObjNums[i],    FieldSizes[i],
                                DbObjectAddr, CppObjectAddr,    AplId,
                                Release,      McStat);
                 break;

            case aSetNodeAttr  :
            case aSetHeadAttr  :
                 AttrCount      = 1;

            case aAttr         :
            case aSlotHndlAttr :
            case aSlotTokenAttr:
                 default       :

                  TransferSize = FieldSizes[i];
                  MoveBytes    (CppObjectAddr, DbObjectAddr, FieldSizes[i]);
                 break;
           }

           if (DeBug || OaBug || ClassBug)
           switch (FieldTypes[i])
           {
            case aObjRefAttr   :
            case aPortRefAttr  :
            case aTknRefAttr   :
            case aMapRefAttr   :
                 break;
            default            :
	       break;
           }

           BaseDbOffset  += AttrDbSize;
           Base01Offset  += Attr01Size;
         }

       }
     }

  } else {

     MoveBytes   (Object,   DbObject,   MapSize);
  }

TRACK(TrackBak,"C_MapRefToDb\n");
return (STAT);
}
 

boolean C_RefToTkn      (addrtype      ThisObj, numtype      ObjNum, 
                         numtype       AplId,
                         ft F,lt Z,zz *Status,  tokentype   *ObjTkn)
{
tokentype               (*pmf1)(addrtype)        = NullPtr;

   if (AplId == Cpp_Apl)
   {
     CppRefToTkn      (ThisObj, ObjNum, McStat, ObjTkn);
     if (Normal(*Status))
        return (STAT);

     if (*Status    == Trans_CppShellNotInvoked)
         *Status     = Env_Normal;
   }

   if (Normal(*Status))
   {
      if (ThisObj)
      {
        if (ShellGetTokenCode  (ObjNum, McStat, (funcptrtype  *)&pmf1))
        if (pmf1)
        {
          *ObjTkn    = (*pmf1)(ThisObj);

        } else {
          if (ThisObj == *(addrtype *)ThisObj)
              ThisObj  = (addrtype )((char *)ThisObj + sizeof (addrtype));

         *ObjTkn    = *(tokentype *)ThisObj;
        }

      } else
          *ObjTkn    = NullToken;

   }

strcpy (TrackBak, "C_RefToTkn\n");
return (STAT);
}



boolean    C_CommitClass    (tokentype    *Token,   numtype       ClassId,
                             numtype       AplId,   ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
tokentype   ObjTkn         = NullToken;
numtype     MemLoc         = 0;
numtype     ObjNum         = 0;
addrtype    This           = NullPtr;
handletype  Handle         = 0;
numtype     Count          = 0;
boolean     Release        = True;
boolean     FreeMem        = True;
bytessize   ImageSize      = 0;

  if (TmGetCoreDb     (Token,                   McStat,   &CoreDb))
     ObjTkn.DbId  = Token->DbId;

  while (Normal(*Status)
  &&     ++Handle < CoreDb->HandleCount)
  {
     ObjTkn.Handle  = Handle;
     if (HmGetObjectMemLoc    (CoreDb,  ObjTkn.Handle,  McStat,   &MemLoc))
     if (MemLoc)
     if (HmGetObjNum          (CoreDb,  ObjTkn.Handle,  McStat,   &ObjNum))
     if (ObjNum == ClassId)
     {
        Count++;

        C_ReFaxToDb           (&ObjTkn, This,  Release, FreeMem,   AplId,
                               McStat, &ImageSize);
     }
  }

TRACK(TrackBak,"C_CommitClass\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF TRANS10.C                                !
  +-----------------------------------------------------------------------+*/
