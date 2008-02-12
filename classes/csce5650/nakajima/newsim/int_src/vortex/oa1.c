/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OA1.c */
#define OA1
#include <time.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "env0.h"
#include "odbm.h"
#include "vchunk.h"
#include "om.h"
#include "oa0.h"
#include "voa.h"
#include "oa1.h"
#include "env1.h"
#include "sm.h"
#include "grp.h"
 
/*+-----------------------------------------------------------------------+*
~E                            EXTERMS                                     !
 *+-----------------------------------------------------------------------+*/
extern boolean OaDumpObject
                          (int           TraceSwi,  tokentype    *Token,
                           ft F,lt Z,zz *Status);

extern boolean OaDumpObjHdr
                          (int           TraceSwi,  tokentype    *Token,
                           numtype       ObjNum,    ft F,lt Z,zz *Status);

extern boolean DbmLoadObjHdr
                          (tokentype    *DbToken,   numtype       LocalObj,
                           ft F,lt Z,zz *Status,    objheader   **ObjHdr);
 
#ifdef API_INCLUDE
extern boolean TransGetMap
                              (numtype       ObjNum,    numtype       AplId,
                               ft F,lt Z,zz *Status,    classmaptype *ClassMap);
#endif

#define MemGetObjNum         Mem_GetWord
#define MemGetLocalObjNum    Mem_GetWord
#define MemGetAttrNum        Mem_GetWord
#define MemGetSize           Mem_GetWord
#define MemGetVbn            Mem_GetWord
 
#ifdef __ZTC__
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#else

#ifdef MCSTAT
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (Anchor->DbId <= MaxCoreDbId \
 && ((*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr)) ? True : \
     TmFetchCoreDb(Anchor,Stat,DbInCore)
#else
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)

#endif 
#endif 
/*+-----------------------------------------------------------------------+*
~OA1                    OBJECT MEMBER ACTOR                               !
 *+-----------------------------------------------------------------------+*/
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        SetHead_ObjNum         (objnumtype )62
#define        Node_ObjNum            (objnumtype )76 

#define        ObjNum_AttrNum         (numtype )5

#define        CodeNum_AttrNum        (numtype )17
#define        BaseCreateCode_AttrNum (numtype )72

#define Code_ForeignNotCreate   14000

#endif

/*+-----------------------------------------------------------------------+*/
/*                      ATTRIBUTE COGNISANT                                */
/*+-----------------------------------------------------------------------+*/

boolean OaIamA            (tokentype    *Token,     numtype       EnvObj,
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
   if (BitField_Get       (&ObjCestrsTkn, EnvObj,  McStat,  &BitVal))
   if (!BitVal)                              
      *Status = Kernl_IamNot;

TRACK(TrackBak,"OaIamA\n");
return (STAT);
}
 

boolean    ForeignCreate  (numtype       ObjNum,    addrtype      ForeignTkn,  
                           ft F,lt Z,zz *Status,    addrtype     *ForeignAddr )
{
tokentype    Token;
handletype   ObjHandle;
handletype   CodeHandle;
addrtype     DumObj         = NullPtr;
indextype    CodeNum        = 0;
p_create     CreateCodeAddr = (p_create )NullPtr;

     if (EnvFetchObjHandle (ObjNum, McStat, &ObjHandle))
     {
        Token.DbId   = RenvId;
        Token.Handle = ObjHandle;
     }
     if (Normal(*Status))
     if (OaGet (False,   &Token,  (addrtype )DumObj, BaseCreateCode_AttrNum,
                McStat,  (addrtype )&CodeHandle)) 
     {
        Token.DbId   = RenvId;
        Token.Handle = CodeHandle;
     }

     if (Normal(*Status))
     if (OaGet (False,   &Token,  (addrtype )DumObj, CodeNum_AttrNum,
                McStat,   (addrtype )&CodeNum))
     if (EnvFetchCode  (CodeNum, McStat, (funcptrtype *)&CreateCodeAddr))
     if (! (*CreateCodeAddr) (ForeignTkn, ForeignAddr))
        *Status = Code_ForeignNotCreate;

TRACK(TrackBak,"ForeignCreate\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/

boolean  OaInitObject     (tokentype    *Token,     numtype       NumInits, 
                           numtype      *AttrNums,  addrtype     *Values,
                           ft F,lt Z,zz *Status)
{
#ifdef API_INCLUDE
indextype    Index          = 0;
short        i              = 0;
boolean      Match          = False;
addrtype     DbOffset       = NullPtr;
sizetype     AttrSize       = 0;
indextype    ExtendQty      = 0;
tokentype    VchunkTkn      = NullToken;

classmaptype ClassMap;
#endif


#ifdef API_INCLUDE
   VchunkTkn.DbId  = RenvId;

/* This Procedure can only be accessed from KernelNewObject Interface      */
/* Therfor the Object is ASSURED to BE IN CORE as Prev Call is OaCreate    */

  if (CoreDbs[Token->DbId])
      MemGetAddr (CoreDbs[Token->DbId]->ObjectAddrDir,  Token->Handle,
                  McStat,                  (addrtype *)&Object);

  if (Normal(*Status))
  if (Object)
  {
     if (EnvFetchObjNum (Token,           McStat,  &ObjNum))
     if (TransGetMap    (C_Apl,     ObjNum,   McStat,  &ClassMap))

     for (i=0;  i < NumInits; i++)
     {
       for (Index = 1,  Match = False;
                Index <= ClassMap.NumMapFields && ! Match; Index++ )
       {
         if (AttrNums[i] == ClassMap.FieldAttrIds[Index])
            Match  = True;
       }

       if (Match)
       {
         DbOffset  = (addrtype )((char *)Object + ClassMap.DbOffsets[Index]);

         if (ClassMap.FieldAttrTypes[Index] == aArrayAttr
         ||  ClassMap.FieldAttrTypes[Index] == aDblPtrAttr
         ||  ClassMap.FieldAttrTypes[Index] == aVarrayAttr)
         {
            ExtendQty   = *(indextype *)Values[i];

            if (EnvFetchAttrSize (ClassMap.FieldAttrIds[Index],
                                  McStat,                    &AttrSize))
            if (OaCreateVchunk   (Token,                      AttrSize,
                                  *(sizetype *)Values[i],     ExtendQty, 
                                  McStat,                    &VchunkTkn)) 
            if (OaPutVchunkStackPtr 
                                 (&VchunkTkn,  ExtendQty,     McStat))
            
            if (OaGetVchunkAddr  (&VchunkTkn,  McStat,       &Array))
            
            if (DeBug || OaBug)
            if (sprintf (Msg, "  Assign Array Address To @%x\n", Array))
               TraceMsg (0, Msg);

            if (Normal(*Status))
               MoveBytes (&VchunkTkn.Handle, DbOffset, sizeof (handletype));

         } else
            MoveBytes (Values[i], DbOffset, ClassMap.FieldSizes[Index]);

       } else
         *Status = Oa_AttrNotPresent;
     }

  } else 
#endif

    *Status = Oa_NotInCreateMode;


TRACK(TrackBak,"OaInitObject\n");
return (STAT);
}
 

boolean    OaCopy         (tokentype    *TokenA,    tokentype     *TokenB,
                           ft F,lt Z,zz *Status)
{
  *Status   = Err_NotImplemented;


TRACK(TrackBak,"OaCopy\n");
return(STAT);
}
 

boolean OaGetDbObjNums    (int           TraceSwi, tokentype    *Anchor,
                           ft F,lt Z,zz *Status)
{
dbheader     *DbHdr      = NullPtr;
indextype     ObjectCount   = 0;
indextype     ObjIndex   = 0;
objheader    *ObjHdr     = NullPtr;
objdesctype  *ObjDesc    = 0;
numtype       EnvObjNum  = 0;
tokentype     ObjTkn;
numtype       Items      = 0;
wordtype      Vbn        = 0;
time_t        Tod        = 0;

  ObjTkn.DbId  = RenvId;

  sprintf  (Msg, "             OBJ Classes In Db[%2u]\n", Anchor->DbId);
  TraceMsg (0,   Msg);
  if (TraceSwi)
      TraceMsg (0,   "                 with number of instances\n\n");

  if (TmFetchCoreDb  (Anchor,               McStat,    &DbHdr))
  if (sprintf  (Msg, "                 TimeStampChunk : %2u\n",
                DbHdr->ObjTimeStampDir))
  if (TraceMsg (0,   Msg))

  while (Normal(*Status)
  &&   ++ObjIndex <= DbHdr->LocalObjCount)
  {
     ObjectCount  = 0;
     if (MemGetWord  (DbHdr->ObjNumMap,     ObjIndex,   McStat, &EnvObjNum))
     if (MemGetAddr  (DbHdr->ObjHdrDir,     ObjIndex,   
                      McStat,              (addrtype *)&ObjHdr))
     if (Anchor->DbId == RenvId)
         MemGetLong        (DbHdr->ObjTimeStampDir, EnvObjNum,    McStat, 
                            (lt64 *)&Tod);
      else
         MemGetLong        (DbHdr->ObjTimeStampDir, ObjIndex,  McStat, 
                            (lt64 *)&Tod);
     if (Normal(*Status))    
     if (ObjHdr)
         ObjectCount  = ObjHdr->ObjectCount;
     else if (TraceSwi) {
         if (MemGetVbn     (DbHdr->ObjVbnDir,   ObjIndex,   McStat, &Vbn))
         if (Vbn)
         if (DbmLoadObjHdr (Anchor,             ObjIndex,   McStat, &ObjHdr))
             ObjectCount  = ObjHdr->ObjectCount;
         else
             ObjectCount  = 0;
     }

     if (EnvFetchObjHandle
                     (EnvObjNum,            McStat,          &ObjTkn.Handle))
     if (OaGetObject (&ObjTkn,              McStat,    (addrtype *)&ObjDesc))
     {
        if ((TraceSwi  == 0)
        ||  (TraceSwi  && ObjectCount > 0))
        {
           if (Items++ % 3 == 0
           &&  Items       != 1)
               TraceMsg (0, "\n");
           if (TraceSwi)
               sprintf  (Msg,  " %18s(%3u) ", ObjDesc->Nam, ObjectCount);
           else 
               sprintf  (Msg,  " %24s",       ObjDesc->Nam);
           TraceMsg (0, Msg);
        }
     }
  }
  TraceMsg (0, "\n");
 
TRACK(TrackBak,"OaGetDbObjNums\n");
return(STAT);
}


boolean   OaDelete        (tokentype    *Token,     ft F,lt Z,zz *Status)
{ 
dbheader       *CoreDb          = NullPtr;
addrtype        Object          = NullPtr;
numtype         ObjNum          = 0;
numtype         LocalObj        = 0;
objheader      *ObjHdr          = NullPtr;

 if (TmIsValid      (Token,  McStat)  == False) 
 {
   sprintf (Msg, 
      "\n ***ERROR*** OaDelete :: Object[%2u:%4u]\n", 
          Token->DbId, Token->Handle);
   TraceMsg (0,  Msg);
  *Status  = Oa_ObjectHasBeenDeleted;
 }

 if (ClassBug || DeBug || OaBug)
 if (sprintf (Msg, "   Oa_Delete... Token= [%2u:%4u]\n",  
              Token->DbId, Token->Handle))
    TraceMsg (0, Msg);

 if (TmGetCoreDb       (Token,            McStat,     &CoreDb))

 if (EnvFetchObjNum    (Token,            McStat,     &ObjNum))
 if (MemGetLocalObjNum (CoreDb->LocalObjNumMap,        ObjNum,
                        McStat,                       &LocalObj))
 if (OmGetObjHdr       (Token,                         LocalObj,
                        McStat,                       &ObjHdr))

 if (OaGetObject       (Token,            McStat,     &Object))
 if (OaUpdateObject    (Token,            McStat))
     OaDeleteFields    (Token,  LocalObj, ObjHdr,      Object,   McStat);

 if (Normal(*Status))
#ifdef   __SWAP_GRP_POS__
   OmDeleteObject      (Token,            ObjHdr,      Object,   McStat);

#else
 if (HmGetObjectIndex  (CoreDb,                        Token->Handle,
                        McStat,                       &ObjIndex))
 if (MemPushWord       (ObjHdr->ExObjectTbl,           ObjIndex,
                        McStat,                       &StackPtr))
 if (TmFreeToken       (McStat,                        Token))
     ObjHdr->ExObjectCount++;

 /* ZERO the objsize slot within the region                                */
 if (Normal(*Status))
    memset (Object, 0, ObjHdr->ObjSize);
#endif /* __SWAP_GRP_POS__  */


TRACK(TrackBak,"OaDelete\n");
return(STAT);
}


boolean    OaDeleteFields (tokentype    *Token,     numtype       LocalObj,
                           objheader    *ObjHdr,    addrtype      Object,
                           ft F,lt Z,zz *Status)
{
dbheader       *CoreDb          = NullPtr;
bytessizetype   FieldSize       = 0;
bytessizetype   FieldOffset     = 0;
counttype       FieldCount      = 0;
addrtype        FieldAddr       = NullPtr;
numtype         AttrNum         = 0;
numtype         LocalAttr       = 0;
tokentype       AttrToken;
attrdesctype   *AttrDesc        = NullPtr;
static tokentype ValueTkn       = Null_Token;
static addrtype  ValuePtr       = &ValueTkn;
indextype       Index           = 0;
tokentype       VchunkTkn       = NullToken;
chunkstruc      ChunkStruc;  
int             j               = 0;
addrtype       *Arrays          = NullPtr;
handletype     *ArrayHndls      = NullPtr;
handletype     *SubArrays       = NullPtr;
indextype       NumSubArrays    = 0;
handletype      ArrayOwner      = 0;
indextype       FieldMapCount   = 0;
maptkntype      RefMapTkn;
static boolean  FirstDeleteNode = True;
boolean         TempClassBug    = ClassBug;
boolean         TempSetBug      = SetBug;

 AttrToken.DbId        = RenvId;
 ValueTkn.DbId         = Token->DbId;

 if (TmGetCoreDb       (Token,            McStat,     &CoreDb))
 while (Normal(*Status)
 &&   ++FieldCount     <= ObjHdr->FieldCount)
 {
    if (MemGetAttrNum (ObjHdr->LocalAttrNumMap,             FieldCount,
                       McStat,                             &LocalAttr))
    if (MemGetAttrNum (CoreDb->AttrNumMap,                  LocalAttr,
                       McStat,                             &AttrNum))
    if (EnvFetchAttrHandle
                      (AttrNum,
                       McStat,                  &(AttrToken.Handle)))
    if (OaGetObject   (&AttrToken,   McStat,   (addrtype *)&AttrDesc))
    if (AttrDesc->AttrType != aAttr)
    {
       ValueTkn.DbId     = Token->DbId;
       ValueTkn.Handle   = NullHandle;

       if (MemGetSize (CoreDb->LocalAttrSizeTbl,            LocalAttr,
                       McStat,                             &FieldSize))
       if (MemGetSize (ObjHdr->FieldOffsetDir,              FieldCount,
                       McStat,                             &FieldOffset))
       {
           FieldAddr = (addrtype )((char *)Object    +      FieldOffset);

           if (FieldSize <= sizeof (tokentype))
           {
              MoveBytes  (FieldAddr,          ValuePtr,        FieldSize);

              if (OaBug || ClassBug || DeBug)
              if (sprintf (Msg,
                           "                           ValueTkn= [%2u:%4u]\n",
                           ValueTkn.DbId, ValueTkn.Handle))
                 TraceMsg (0, Msg);
           }
       }

       if (Normal(*Status))
       {
         switch (AttrDesc->AttrType)
         {
         case  aObjRefAttr      :
/*          Decrement the Usage Count of Object in Reference.              */
/*          If Object in Reference to be Deleted, Do so.                   */
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   DeReference the Object Ref Attr ...\n"))
               TraceMsg (0, Msg);
            break;

         case  aMapRefAttr      :
          ++FieldMapCount;
            if (MemGetSomeBytes 
                             (ObjHdr->FieldMapDir,       FieldMapCount,
                              sizeof (maptkntype),
                              McStat,                   &RefMapTkn))
            if (RefMapTkn.FieldNum  == FieldCount)
                OaDeleteEmbedded 
                             (Token,               RefMapTkn.LocalObj,
                              FieldAddr,           McStat); 
            else {
               sprintf (Msg,  
                " ***ERROR*** Oa_Delete:: FieldNum(%4u) != RefFieldNum(%4u)/n",
                        FieldCount,  RefMapTkn.FieldNum);
               TraceMsg (0, Msg);
            }
            break;

         case  aPortRefAttr :
         case  aTknRefAttr  :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            break;

         case  aSetNodeAttr :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;

            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the Set Node...\n"))
               TraceMsg (0, Msg);

            if (FirstDeleteNode)
            {
               FirstDeleteNode = False;
            }

            SetDeleteFrom     (AttrDesc->BuildDesc, Token, McStat);

            if (*Status == Set_NotFound)
            {
              *Status  = Env_Normal;
               sprintf (Msg, 
                        "   Delete from Set...Token[%4u:%6u] not in set!\n",
                        Token->DbId, Token->Handle);
               TraceMsg (0, Msg);

            }
            break;

         case  aSetHeadAttr :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;

            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the Set ...\n"))
               TraceMsg (0, Msg);

            SetDeleteSet      (AttrDesc->BuildDesc,  Token, McStat);
            break;

         case  aStrHndlAttr     :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the Dbms String ...\n"))
               TraceMsg (0, Msg);

            Index    = (indextype )ValueTkn.Handle;
/*
//          MemDeleteString   (ObjHdr->TextChunk,  Index,   McStat);
*/
            break;

         case  aSlotHndlAttr    :
         case  aSlotTokenAttr   :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   UnTuple this Object ...\n"))
               TraceMsg (0, Msg);
            break;

         case  aMtrxHeadAttr    :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the Matrix...\n"))
               TraceMsg (0, Msg);
            break;

         case  aRowHeadAttr     :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete/UnMate the Row...\n"))
               TraceMsg (0, Msg);
            break;

         case  aColHeadAttr     :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete/Unmate The Column   ...\n"))
               TraceMsg (0, Msg);
            break;

         case  aMateNodeAttr     :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Unmate the Row and Column   ...\n"))
               TraceMsg (0, Msg);
            break;

         case  aPointNodeAttr     :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the Point Node      ...\n"))
               TraceMsg (0, Msg);
            break;

         case aChunkAttr    :
         case aVchunkAttr   :
         case aArrayAttr    :
         case aVarrayAttr   :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete The Virtual Chunk ...\n"))
               TraceMsg (0, Msg);

            if (Normal(*Status))
            if (ValueTkn.Handle > 0)
                Vchunk_Delete   (&ValueTkn,     McStat);
            break;

         case aDblPtrAttr   :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete The Indirect Virtual Chunk ...\n"))
               TraceMsg (0, Msg);

            VchunkTkn.DbId     = Token->DbId;
            j                  = -1;
            if (VchunkGetAddress  (&ValueTkn,
                                   McStat,      (addrtype *)&Arrays))
            if (VchunkGetVstruc   (&ValueTkn,   McStat,     &ChunkStruc))

            if (DeBug  || OaBug || ClassBug)
            if (sprintf (Msg, "  Array Of Pointers; StackPtr =%4u;\n",
                        ChunkStruc.StackPtr))
                TraceMsg (0, Msg);
 
            ArrayOwner          = ValueTkn.Handle;
            ArrayHndls = (handletype *) Arrays;
            NumSubArrays  = ChunkStruc.StackPtr / 3;
            SubArrays = (handletype *) &ArrayHndls[NumSubArrays*2];

            while (Normal(*Status)
            &&     ++j   < NumSubArrays)
            {
#ifdef __BCC__
                 memcpy (&Index, 
                         &SubArrays[j], sizeof (handletype));
#else
               Index             = (handletype )SubArrays[j];
#endif
               VchunkTkn.Handle  = Index;
               if (Index)
                   Vchunk_Delete   (&VchunkTkn,     McStat);

               if (DeBug  || OaBug || ClassBug)
               if (sprintf (Msg, "  Delete SubVarray[%2u] Vchunk[%3u:%4u]\n",
                            j, VchunkTkn.DbId, Index))
                  TraceMsg (0, Msg);
            }
            ValueTkn.Handle  = ArrayOwner;
            if (Normal(*Status))
            if (ArrayOwner)
                Vchunk_Delete   (&ValueTkn,     McStat);
            break;

         case  aGrpAttr         :
         case  aVgrpAttr        :
            if (ValueTkn.Handle == 0
            ||  ValueTkn.DbId   == 0)
                break;
            if (OaBug || ClassBug || DeBug)
            if (sprintf (Msg, "   Delete the dependent GRP    ...\n"))
               TraceMsg (0, Msg);

              Grp_DeletePacket  (&ValueTkn, ValueTkn.Handle,  McStat);

            break;

         default            :
            break;
         }
       }
    }
 }
 ClassBug = TempClassBug;
 SetBug = TempSetBug;

TRACK(TrackBak,"OaDeleteFields\n");
return(STAT);
}



boolean    OaDeleteEmbedded 
                          (tokentype    *Token,     numtype       LocalObj,
                           addrtype      Object,    ft F,lt Z,zz *Status)
{
objheader      *ObjHdr          = NullPtr;

 if (OmGetObjHdr     (Token,     LocalObj,    McStat,    &ObjHdr))
     OaDeleteFields  (Token,     LocalObj,    ObjHdr,     Object,   McStat);

TRACK(TrackBak,"OaDeleteEmbedded\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+*/
/*                            ATTRIBUTE                                    */
/*+-----------------------------------------------------------------------+*/
boolean OaCreateArray     (tokentype    *DbmsToken, numtype   AttrNum,
                           indextype     UnitCount, sizetype  UnitSize,
                           ft F,lt Z,zz *Status,    
                           tokentype    *VchunkTkn, addrtype *ArrayAddr)
{
indextype   ExtendQty = UnitCount;
numtype     MemLoc    = 0;
addrtype    CppAddr   = NullPtr;
addrtype    NullObject= NullPtr;
statustype  DumpStatus  = Env_Normal;
addrtype    Vchunk    = NullPtr;

    if (TmIsValid      (DbmsToken,  McStat)  == False) 
    {
       sprintf (Msg,  "\n ***ERROR*** OaDelete :: Object[%2u:%4u]\n", 
          DbmsToken->DbId, DbmsToken->Handle);
       TraceMsg (0,  Msg);
      *Status  = Oa_ObjectHasBeenDeleted;
    }

    if (Normal(*Status))
    if (HmGetObjectMemLoc (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                           McStat,                   &MemLoc))

    if (DeBug || OaBug)
    if (sprintf (Msg, 
            " OaCreateArray DbmsObject[%2u:%4u](Attr %2u)*%2u MemLoc= %1u\n",
            DbmsToken->DbId, DbmsToken->Handle, AttrNum, UnitCount, MemLoc))
       TraceMsg (0, Msg);

    if (Normal(*Status))
    if (MemLoc)
    if (HmGetObjectAddr   (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                           McStat,                   &CppAddr))
        HmPutObjectAddr   (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                           NullPtr,                   McStat);

    if (Normal(*Status))
    if (VchunkCreate      (DbmsToken,  UnitSize,      UnitCount, 
                           ExtendQty, 
                           McStat,     VchunkTkn)) 

/* ....................... LOOKS SUSPICOUS ................................*/

    if (VchunkPutStackPtr (VchunkTkn,  UnitCount,     McStat,   &Vchunk))
/* ....................... LOOKS SUSPICOUS ................................*/

    if (DeBug || OaBug)
    if (sprintf (Msg, "  Assign Array Address To @%x\n",   *ArrayAddr))
       TraceMsg (0, Msg);

    if (Normal(*Status))
    if (*ArrayAddr == NullPtr)
    {
       if (VchunkGetAddress (VchunkTkn,     McStat,            ArrayAddr))
       if (DeBug || OaBug)
       if (sprintf (Msg, "  Assign Array Address To VchunkAddr = @%x\n",
                    *ArrayAddr))
          TraceMsg (0, Msg);

       if (Normal(*Status))
       if (OaPut          (False,           DbmsToken,         NullObject,
                           AttrNum,        (addrtype )VchunkTkn,   McStat))
       if (OaBug)
           OaDumpObject   (0,               DbmsToken,         Dump_Status);
                        
       if (Normal(*Status))
       if (MemLoc)
       if (HmPutObjectMemLoc (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                              (numtype )1,               McStat))
           HmPutObjectAddr   (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                              CppAddr,                   McStat);

    } else {

/* ....................... LOOKS SUSPICOUS ................................*/
/* if ArrayAddr is Positve Then 
       Must Assert Address to Chunk NOT as was done with MakeChunk above   */
/* ....................... LOOKS SUSPICOUS ................................*/

       if (OaPut          (False,           DbmsToken,            NullObject,
                           AttrNum,        (addrtype )VchunkTkn,  McStat))
       if (OaBug)
           OaDumpObject   (0,               DbmsToken,         Dump_Status);
                        
       if (Normal(*Status))
       if (MemLoc)
       if (HmPutObjectMemLoc (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                              (numtype )1,               McStat))
       if (HmPutObjectAddr   (CoreDbs[DbmsToken->DbId],  DbmsToken->Handle,
                              CppAddr,                   McStat))
       if (OaBug)
           OaDumpObject      (0,                 DbmsToken,      Dump_Status);
/* ....................... LOOKS SUSPICOUS ................................*/

    }

    if (!Normal(*Status) || DeBug || OaBug || ClassBug)
    { 
       sprintf (Msg, "  OaCreateArray for Token[%2u:%4u] Vchunk = %3u @%x\n",
                DbmsToken->DbId,    DbmsToken->Handle, 
                VchunkTkn->Handle, *ArrayAddr);
       TraceMsg (0, Msg);

    } 

TRACK(TrackBak,"OaCreateArray\n");
return (STAT);
}


boolean OaCreateVarray    (tokentype    *DbmsToken, numtype   AttrNum,
                           indextype     UnitCount, sizetype  UnitSize,
                           ft F,lt Z,zz *Status,    
                           tokentype    *VchunkTkn, addrtype *Varray)
{
indextype   ExtendQty = UnitCount;
numtype     MemLoc    = 0;
addrtype    NullObject= NullPtr;
statustype  DumpStatus  = Env_Normal;

    if (DeBug || OaBug)
    if (sprintf (Msg, 
            " OaCreateVarray DbmsObject[%2u:%4u](Attr %2u)*%2u MemLoc= %1u\n",
            DbmsToken->DbId, DbmsToken->Handle, AttrNum, UnitCount, MemLoc))
       TraceMsg (0, Msg);

    if (VchunkCreate        (DbmsToken,           UnitSize,   UnitCount, 
                             ExtendQty, 
                             McStat,                          VchunkTkn)) 
    if (VchunkPutStackPtr   (VchunkTkn,           0,
                             McStat,                          Varray))

    if (OaPut               (False,               DbmsToken,  NullObject,
                             AttrNum,  (addrtype )VchunkTkn,  McStat))
    if (OaBug)
        OaDumpObject        (0,               DbmsToken,      Dump_Status);

    if (!Normal(*Status) || DeBug || OaBug)
    { 
       sprintf (Msg, "  OaCreateVarray for Token[%2u:%4u] Vchunk = %3u @%x\n",
                DbmsToken->DbId,    DbmsToken->Handle, 
                VchunkTkn->Handle, *Varray);
       TraceMsg (0, Msg);

    } 

TRACK(TrackBak,"OaCreateVarray\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/*+                              PUTS                                     +*/
/*+-----------------------------------------------------------------------+*/
boolean    OaPut          (boolean      CppType,    tokentype    *Token,
                           addrtype     ObjectAddr, numtype       EnvAttrNum,
                           addrtype     AttrValue,  ft F,lt Z,zz *Status)
{ 
num          LocalAttrNum = 0;
num          LocalObjNum  = 0;
num          FieldNum     = 0;
bytessize    Size         = 0;
bytessize    Offset       = 0;
addrtype     BytePtr      = NullPtr;
objheader   *ObjHdr       = NullPtr;
addrtype     Object       = ObjectAddr;
dbheader    *CoreDb       = NullPtr;
tokentype    AttrTkn;
attrdesctype *AttrDesc    = NullPtr;
statustype   DumpStatus   = Env_Normal;
indextype    Index        = 0;
numtype      MemLoc       = 0;
tokentype   *TokenPtr     = NullPtr;
char        *StrPtr       = NullPtr;
char        *String       = NullPtr;
sizetype     StrSize      = 0;

    if (TmIsValid      (Token,   McStat)  == False)
    {
      sprintf (Msg, 
         "\n ***ERROR*** OaPut ::  Object[%2u:%4u]. Token is Invalid!\n", 
             Token->DbId, Token->Handle);
      TraceMsg (0,  Msg);
     *Status  = Oa_ObjectHasBeenDeleted;
    }

    if (DeBug)
    if (sprintf (Msg, "   OaPut TO Object[%2u:%4u] for AttrNum= %u\n",
                 Token->DbId, Token->Handle, EnvAttrNum))
       TraceMsg (0, Msg);
 
    AttrTkn.DbId = RenvId;
    if (Normal(*Status))
    if (CppType)
    {
       Token->Handle = ((objtype *)Object)->Handle;
       if (TmGetCoreDb     (Token,    McStat,   &CoreDb))
          *Status = Oa_BadDbToken;

    } else {
       if (TmGetCoreDb    (Token,    McStat,   &CoreDb))
       if (OaGetObject    (Token,    McStat,   &Object))
           OaUpdateObject (Token,    McStat);

       if (!Normal(*Status))
       {
           sprintf (Msg, "   Object[%2u:%4u] Corrupted ... \n",
                    Token->DbId, Token->Handle);
           TraceMsg (0, Msg);
           if (HmPutObjectAddr (CoreDb, Token->Handle,  NullPtr, Dump_Status))
           if (OmGetObject     (Token,  Dump_Status,   &Index,       &Object))
               OaDumpObject    (0,      Token,                   Dump_Status);
           if (Normal(DumpStatus))
              *Status = Env_Normal;
       }
    }

    if (Normal(*Status))
    if (HmGetObjectMemLoc
                         (CoreDb,  Token->Handle,    McStat,   &MemLoc))
    if (HmGetLocalObjNum (CoreDb,                               Token->Handle,
                          McStat,                              &LocalObjNum))
    if (MemGetAddr       (CoreDb->ObjHdrDir,        (indextype )LocalObjNum,
                          McStat,                  (addrtype *)&ObjHdr))
        MemGetObjNum     (CoreDb->LocalAttrNumMap,  (indextype )EnvAttrNum,
                          McStat,                              &LocalAttrNum);

    if (Normal(*Status))
    if (!LocalAttrNum)
    {
       sprintf (Msg, "   Object[%2u:%4u] does not have AttrNum : %u\n",
                 Token->DbId, Token->Handle, EnvAttrNum);
       TraceMsg (0, Msg);
      *Status = Oa_DbNoThisAttr;
    }

    if (Normal(*Status))
    if (MemGetSize      (CoreDb->LocalAttrSizeTbl,  (indextype )LocalAttrNum,
                         McStat,                                 &Size))
    if (MemGetObjNum    (ObjHdr->FieldNumMap,       (indextype )LocalAttrNum,
                         McStat,                        (num *)&FieldNum))
    if (EnvFetchAttrOffset 
                        (Token,      EnvAttrNum,   ObjHdr,      FieldNum,
                         McStat,                               &Offset))
    if (*Status       == Env_AttrNotInDbmsClassDef)
        *Status        = Oa_AttrNotInDbmsClassDef;

    else if (FieldNum == 0)
        *Status        = Oa_ObjNoThisField;

    if (Normal(*Status))
    if (EnvFetchAttrHandle 
                        (EnvAttrNum, McStat,           &AttrTkn.Handle))
    if (OaGetObject     (&AttrTkn,   McStat,       (addrtype *)&AttrDesc))
    if (AttrDesc->AttrType         == aTknRefAttr)
    {
       if (MemLoc  == 0)
         TokenPtr   = (tokentype *)((char *)AttrValue + sizeof(addrtype));
       else {
         Size       = sizeof (addrtype)   + sizeof (tokentype);
         TokenPtr   = (tokentype *)AttrValue;
       }
       BytePtr      = (addrtype )((char *)Object + Offset);
       MoveBytes      (TokenPtr, BytePtr, Size);

    } else if (AttrDesc->AttrType  == aObjRefAttr) {
       if (MemLoc  == 1)
         Size       = sizeof (addrtype);

       BytePtr      = (addrtype )((char *)Object + Offset);
       MoveBytes      (AttrValue, BytePtr, Size);

    } else if (AttrDesc->TypeType  == aDbmsString) {
       BytePtr      = (addrtype )((char *)Object + Offset);

       if (MemLoc  == 0) 
          OaPutString    (Token->DbId,  ObjHdr,  (char      *)AttrValue, 
                          McStat,                (indextype *)BytePtr);
       else {
          StrPtr      = (char *)AttrValue;
          if (StrPtr)
          if ((StrSize = strlen (StrPtr)+1) >= 1)
          {
             if (Core0_AllocString  (StrSize,  McStat,  &String))
             {
               if (ClassBug || OaBug  || DeBug)
               if (sprintf (Msg, "    ***DbmsString Mallocd  @%x\n", String))
                  TraceMsg (0, Msg);

               strcpy (String,  StrPtr);
               MoveBytes   ((addrtype )&String, BytePtr, sizeof (char *));
             }

          } else
             MoveBytes   (&StrPtr, BytePtr, sizeof (char *));
       }

    } else if (AttrDesc->TypeType  == aString
           &&  AttrDesc->UnitSize   > 1)
    {
       BytePtr      = (addrtype )((char *)Object + Offset);
       StrPtr       = (char *)AttrValue;
       if (StrPtr)
       {
          strncpy ((char *)BytePtr,  StrPtr, AttrDesc->UnitSize - 1);

       } else
          MoveBytes   (AttrValue, BytePtr, sizeof (char *));

    } else {
       BytePtr      = (addrtype )((char *)Object + Offset);
       MoveBytes      (AttrValue, BytePtr, Size);
    }

    if (!Normal(*Status) || DeBug || OaBug) {
       if (sprintf (Msg, "   PUT Attr[%3u](%3u) size %4d ::= ",
                   EnvAttrNum, LocalAttrNum, Size)) 
       if (TraceMsg   (0, Msg))
       if (TraceBytes (0, AttrValue, Size))
       if (TraceMsg   (0, "\n"))
       if (sprintf (Msg, "   TO  object[%2u:%4u](%3u)  offset %3u @%x\n", 
                    Token->DbId, Token->Handle, LocalObjNum,
                    Offset, BytePtr))
           TraceMsg   (0, Msg);
    }       

    if (*Status != Env_Normal)
       OaDumpObject (0, Token, Dump_Status);

TRACK(TrackBak,"OaPut\n");
return (STAT);
}
 

boolean    OaPutString    (idtype        DbId,    objheader    *ObjHdr,
                           char         *String,
                           ft F,lt Z,zz *Status,  indextype    *StrIndex)
{ 
indextype    Index        = *StrIndex;
dbheader    *CoreDb       = CoreDbs[DbId];     
indextype    NumStrings   = 0;

   if (Index)
      MemPutString    (ObjHdr->TextChunk,    Index,
                       String,               McStat);
   else {
      if (!ObjHdr->TextChunk)
      if ( ObjHdr->TextChunkVbn)
         FileInChunk  (&ObjHdr->TextChunk,  &CoreDb->BlkHdr,
                       False,
                       McStat,              &ObjHdr->TextChunkVbn);
      else {
         NumStrings = ObjHdr->RegionObjects;
         MemMakeStrChunk (1024, NumStrings, ObjHdr->RegionObjects,
                          2048, DbId,
                          McStat,           &ObjHdr->TextChunk);
      }
      if (ObjHdr->TextChunk)
      {
         MemNewString (ObjHdr->TextChunk,           String,
                       McStat,                     &Index);
      } else
         *Status = Err_BadDesign;
   }
  *StrIndex = Index;


TRACK(TrackBak,"OaPutString\n");
return (STAT);
}
 

boolean    OaPutToEmbedded 
                         (tokentype    *Token,     numtype    SubClassAttr,
                          numtype       AttrNum,   addrtype   Value,
                          ft F,lt Z,zz *Status)
{
numtype      SubClassObj  = 0;
numtype      LocalAttrNum = 0;
numtype      LocalObjNum  = 0;
numtype      FieldNum     = 0;
bytessize    Size         = 0;
bytessize    SubOffset    = 0;
bytessize    Offset       = 0;
addrtype     BytePtr      = NullPtr;
objheader   *ObjHdr       = NullPtr;
objheader   *SubObjHdr    = NullPtr;
addrtype     Object       = NullPtr;
dbheader    *CoreDb       = NullPtr;
tokentype    AttrTkn;
attrdesctype *AttrDesc    = NullPtr;
tokentype    ObjTkn;
objdesctype *ObjDesc      = NullPtr;
indextype    Index        = 0;
statustype   DumpStatus   = Env_Normal;

    AttrTkn.DbId = RenvId;
    ObjTkn.DbId  = RenvId;

    if (TmGetCoreDb    (Token,    McStat,   &CoreDb))
    if (OaGetObject    (Token,    McStat,   &Object))
        OaUpdateObject (Token,    McStat);

    if (!Normal(*Status))
    {
         sprintf (Msg, "   Object[%2u:%4u] Corrupted ... \n",
                  Token->DbId, Token->Handle);
         TraceMsg (0, Msg);

         if (HmPutObjectAddr (CoreDb, Token->Handle,  NullPtr, Dump_Status))
         if (OmGetObject     (Token,  Dump_Status,   &Index,       &Object))
             OaDumpObject    (0,      Token,                   Dump_Status);
         if (Normal(DumpStatus))
           *Status = Env_Normal;
    }

    if (Normal(*Status))
    if (HmGetLocalObjNum (CoreDb,                               Token->Handle,
                          McStat,                              &LocalObjNum))

    if (MemGetAddr       (CoreDb->ObjHdrDir,       (indextype  )LocalObjNum,
                          McStat,                  (addrtype *)&ObjHdr))

    if (MemGetObjNum     (CoreDb->LocalAttrNumMap, (indextype  )SubClassAttr,
                          McStat,                              &LocalAttrNum))

    if (!LocalAttrNum)
    {
       sprintf (Msg, " Object[%2u:%4u] does not have Attribute %3u\n",
                 Token->DbId, Token->Handle, SubClassAttr);
       TraceMsg (0, Msg);
      *Status = Oa_DbNoThisAttr;
    }

    if (Normal(*Status))
    if (MemGetObjNum     (ObjHdr->FieldNumMap,     (indextype )LocalAttrNum,
                          McStat,                             &FieldNum))
    if (FieldNum)
    if (EnvFetchAttrOffset
                         (Token,  SubClassAttr,    ObjHdr,     FieldNum,
                          McStat,                             &SubOffset))
    if (*Status == Env_AttrNotInDbmsClassDef)
        *Status  = Oa_AttrNotInDbmsClassDef;

    if (Normal(*Status))
    if (EnvFetchAttrHandle
                         (SubClassAttr,     McStat,    &AttrTkn.Handle))
    if (OaGetObject      (&AttrTkn,         McStat, (addrtype *)&AttrDesc))
        ObjTkn.Handle  = AttrDesc->BuildDesc;

    if (Normal(*Status))
    if (OaGetObject      (&ObjTkn,          McStat, (addrtype *)&ObjDesc))
    if (MemGetObjNum     (CoreDb->LocalObjNumMap,       ObjDesc->ObjNum,
                          McStat,                              &LocalObjNum))

    if (MemGetAddr       (CoreDb->ObjHdrDir,                    LocalObjNum,
                          McStat,                  (addrtype *)&SubObjHdr))

        MemGetObjNum     (CoreDb->LocalAttrNumMap, (indextype  )AttrNum,
                          McStat,                              &LocalAttrNum);

    if (Normal(*Status))
    if (!LocalAttrNum)
    {
       sprintf (Msg, " Object[%2u:%4u] SubClass %3u does not have Attr %u\n",
                 Token->DbId, Token->Handle, SubClassAttr, AttrNum);
       TraceMsg (0, Msg);
      *Status = Oa_DbNoThisAttr;
    }

    if (Normal(*Status))
    if (MemGetSize    (CoreDb->LocalAttrSizeTbl,  (indextype )LocalAttrNum,
                       McStat,                    &Size))
    if (MemGetObjNum  (SubObjHdr->FieldNumMap,    (indextype )LocalAttrNum,
                       McStat,                    (numtype *)&FieldNum))

    if (EnvFetchAttrOffset (Token,   AttrNum,   SubObjHdr,    FieldNum,
                            McStat,                          &Offset))
    if (*Status == Env_AttrNotInDbmsClassDef)
        *Status  = Oa_AttrNotInDbmsClassDef;

    if (Normal(*Status))
    if (FieldNum)
    {
       BytePtr  = (addrtype )((char *)Object + SubOffset + Offset);

       if (Size == 2
       &&  sizeof (booleantype) == 1)
       {
          if (EnvFetchAttrHandle (AttrNum,  McStat,     &AttrTkn.Handle))
          if (OaGetObject        (&AttrTkn, McStat, (addrtype *)&AttrDesc))
          if (AttrDesc->TypeType == aEnum)
             Size = 1;
       }

       MoveBytes (Value, BytePtr, Size);

    } else 
        *Status = Oa_ObjNoThisField;

    if (!Normal(*Status) || DeBug || OaBug) {
       if (sprintf (Msg, "   PUT To Embedded Obj=%3u Attr(%3u) size %4d ::= ",
                   SubClassObj,AttrNum, Size)) 
       if (TraceMsg   (0, Msg))
       if (TraceBytes (0, Value, Size))
       if (TraceMsg   (0, "\n"))
       if (sprintf (Msg, "   TO  object[%2u:%4u](%3u)  offset %3u @%x\n", 
                    Token->DbId, Token->Handle, LocalObjNum,
                    Offset, BytePtr))
           TraceMsg   (0, Msg);
    }       

    if (!Normal(*Status))
       OaDumpObject (0, Token, Dump_Status);

TRACK(TrackBak,"OaPutToEmbedded\n");
return (STAT);
}
 

boolean    OaPutField    (tokentype    *Token,     fieldstruc   *FS,
                          addrtype      Value,     ft F,lt Z,zz *Status)
{
dbheader    *CoreDb       = NullPtr;
addrtype     Object       = NullPtr;
numtype      ObjNum       = 0;
sizetype     Offset       = 0;
sizetype     Size         = 0;
addrtype     BytePtr      = NullPtr;
fieldspec   *FieldTkns    = FS->FieldTkns;
indextype    SubLevel     = FS->SubLevel;

    if (TmGetCoreDb       (Token,    McStat,   &CoreDb))
    if (OaGetObject       (Token,    McStat,   &Object))
        OaUpdateObject    (Token,    McStat);

    if (!Normal(*Status))
    {
         sprintf (Msg, "***ERROR... Object[%2u:%4u] Corrupted ... \n",
                  Token->DbId, Token->Handle);
         TraceMsg (0, Msg);
    }

    if (Normal(*Status))
    if (EnvFetchObjNum       (Token,       McStat,     &ObjNum))
    if (Env_FetchFieldOffset 
                         (Token,    FieldTkns,      SubLevel,
                          McStat,   &Size, &Offset))
    {
       BytePtr  = (addrtype )((char *)Object    + Offset);
       MoveBytes             (Value,       BytePtr,               Size);
    }

    if (!Normal(*Status) || DeBug || OaBug || ClassBug)
    {
       if (sprintf (Msg, "   PUT Field :: AttrId=%4u; size (%4d) ::= ",
                   FieldTkns[SubLevel-1].AttrId, Size)) 
       if (TraceMsg   (0, Msg))
       if (TraceBytes (0, Value, Size))
       if (TraceMsg   (0, "\n"))
       if (sprintf (Msg, "   TO  object[%2u:%4u] offset %3u @%x\n", 
                    Token->DbId, Token->Handle, Offset, BytePtr))
           TraceMsg   (0, Msg);
    }       

TRACK(TrackBak,"OaPutField\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+*/
/*+                              GETS                                     +*/
/*+-----------------------------------------------------------------------+*/
boolean    OaGet         (boolean       CppType,    tokentype  *Token,
                          addrtype      ObjectAddr, numtype     EnvAttrNum,
                          ft F,lt Z,zz *Status,     addrtype    AttrValue)
{ 
num          LocalAttrNum = 0;
num          LocalObjNum  = 0;
num          FieldNum     = 0;
bytessize    Size         = 0;
bytessize    Offset       = 0;
addrtype     BytePtr      = NullPtr;
objheader   *ObjHdr       = NullPtr;
addrtype     Object       = ObjectAddr;
dbheader    *CoreDb       = NullPtr;
tokentype    AttrTkn;
attrdesctype *AttrDesc    = NullPtr;
numtype      MemLoc       = 0;
char        *StringPtr    = NullPtr;
indextype    Index        = 0;
statustype   DumpStatus   = Env_Normal;
 
    if (TmIsValid      (Token,   McStat)  == False)
    {
      sprintf (Msg, 
         "\n ***ERROR*** OaGet :: Object[%2u:%4u]. Token is Invalid!\n", 
             Token->DbId, Token->Handle);
      TraceMsg (0,  Msg);
     *Status  = Oa_ObjectHasBeenDeleted;
    }
    AttrTkn.DbId     = RenvId;

    if (Normal(*Status))
    if (CppType)
       Token->Handle = ((objtype *)Object)->Handle;
    else 
       OaGetObject   (Token,  McStat, &Object);

    if (Normal(*Status))
    if (TmGetCoreDb      (Token,     McStat,                   &CoreDb))
    if (HmGetObjectMemLoc
                         (CoreDb,    Token->Handle,  McStat,   &MemLoc))
    if (HmGetLocalObjNum (CoreDb,                               Token->Handle,
                          McStat,                              &LocalObjNum))
    if (MemGetAddr       (CoreDb->ObjHdrDir,        (indextype )LocalObjNum,
                          McStat,                  (addrtype *)&ObjHdr))
    if (EnvAttrNum     <= CoreDb->LastEnvAttrCount)
    {
        MemGetObjNum     (CoreDb->LocalAttrNumMap,  (indextype )EnvAttrNum,
                          McStat,                   &LocalAttrNum);
    } else {
        MemPutStackPtr   (CoreDb->LocalAttrNumMap,  CoreDb0->LastEnvAttrCount,
                          McStat);
        LocalAttrNum  = 0;
        CoreDb->LastEnvAttrCount = CoreDb0->LastEnvAttrCount;
    }

    if (Normal(*Status))
    if (!LocalAttrNum)
        *Status = Oa_ObjNoThisAttr;

    if (Normal(*Status))
    if (MemGetSize       (CoreDb->LocalAttrSizeTbl,(indextype )LocalAttrNum,
                          McStat,                             &Size))
    if (MemGetObjNum     (ObjHdr->FieldNumMap,     (indextype )LocalAttrNum,
                          McStat,                      (num *)&FieldNum))

    if (Normal(*Status))
    if (!FieldNum)
        *Status = Oa_AttrNotPresent;

    if (Normal(*Status))
    if (EnvFetchAttrOffset 
                         (Token,       EnvAttrNum,   ObjHdr,   FieldNum,
                          McStat,                             &Offset))
    if (*Status == Env_AttrNotInDbmsClassDef)
        *Status  = Oa_AttrNotInDbmsClassDef;

    if (Normal(*Status))
    if (EnvFetchAttrHandle
                         (EnvAttrNum,  McStat,                &AttrTkn.Handle))
    if (OaGetObject      (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))
    if (Token->DbId > RenvId)
    if (MemLoc)
    {
       if (AttrDesc->AttrType         == aTknRefAttr)
          Size      += sizeof(addrtype);
       else if (AttrDesc->AttrType    == aVarrayAttr)
          Size       = sizeof (varraytype);
       else if (AttrDesc->AttrType    == aChunkAttr)
          Size       = sizeof (vchunktype);
    }

    if (Normal(*Status))
    {
       BytePtr  = (addrtype )((char *)Object + Offset);

       if (Size                 == 2
       &&  sizeof (booleantype) == 1)
          Size  = 1;

       MoveBytes   (BytePtr, AttrValue, Size);

       if (Token->DbId > RenvId)
       if (!MemLoc)
       if (AttrDesc->TypeType        == aDbmsString)
       {
           Index       = *(indextype *)BytePtr;
           StringPtr   = NullPtr;

           OaGetString (Token->DbId,  ObjHdr,                Index,
                        McStat,                    (char **)&StringPtr);

           MoveBytes   ((addrtype )&StringPtr, (addrtype )AttrValue,
                        sizeof (addrtype ));
       }
    }

    if (!Normal(*Status) || DeBug  || OaBug)
    {
       if (sprintf (Msg, 
                   "\n  OA GET Attr[%3u](%3u) Field=%2u size %4u ::=",
                    EnvAttrNum, LocalAttrNum, FieldNum, Size))
       if (TraceMsg (0, Msg))
       if (TraceBytes (0, AttrValue, Size))
       if (TraceMsg (0, "\n"))
       if (sprintf (Msg, "   FROM Object[%2u :%4u](%3u)  @ %x +%3u== %x\n", 
                Token->DbId, Token->Handle, LocalObjNum,
                Object, Offset, BytePtr))
          TraceMsg (0, Msg);
    }       

    if (*Status != Env_Normal)
       OaDumpObject (0, Token, Dump_Status);

TRACK(TrackBak,"OaGet\n");
return (STAT);
}
 

boolean    OaGetString    (idtype        DbId,    objheader    *ObjHdr,
                           indextype     StrIndex,
                           ft F,lt Z,zz *Status,  char        **String)
{ 
dbheader    *CoreDb       = CoreDbs[DbId];     

  *String  = NullPtr;
   if (!ObjHdr->TextChunk)
   if ( ObjHdr->TextChunkVbn)
      FileInChunk  (&ObjHdr->TextChunk,  &CoreDb->BlkHdr,
                    False,
                    McStat,              &ObjHdr->TextChunkVbn);
/*
   else
      *Status = Err_BadDesign;
*/

   if (Normal(*Status))
   if (ObjHdr->TextChunk)
      MemGetString (ObjHdr->TextChunk,    StrIndex,
                    McStat,               String);

TRACK(TrackBak,"OaGetString\n");
return (STAT);
}
 

boolean    OaGetField    (tokentype    *Token,     fieldstruc   *FS,
                          ft F,lt Z,zz *Status,    addrtype      Value)
{
dbheader    *CoreDb       = NullPtr;
addrtype     Object       = NullPtr;
numtype      ObjNum       = 0;
numtype      LocalObjNum  = 0;
objheader   *ObjHdr       = NullPtr;
sizetype     Offset       = 0;
sizetype     Size         = 0;
addrtype     BytePtr      = NullPtr;
statustype   DumpStatus   = Env_Normal;
int          i            = 0;
indextype    Index        = 0;
numtype      MemLoc       = 0;
attrtype     AttrType     = aNonTypeAttr;    
typetype     FieldType    = aNonTypeType;
char        *StringPtr    = NullPtr;
fieldspec   *FieldTkns    = FS->FieldTkns;
indextype    SubLevel     = FS->SubLevel;

    /* This Procedure will Get the VALUE of the DbObject
       NOT of the Image (if resident).                                   */

    if (OaBug || DeBug) 
    {
       sprintf (Msg, "   OaGetField:: From Object[%2u:%4u]; SubLevel=%4u\n",
                Token->DbId, Token->Handle, SubLevel);
       TraceMsg (0, Msg);
       for (i=0; i < SubLevel; i++) 
       {
          sprintf  (Msg, " Field %4u. AttrId=%4u; Index=%4u.\n",
                    i, FieldTkns[i].AttrId,  FieldTkns[i].Index);
          TraceMsg (0, Msg);         
       }
    }

    if (TmGetCoreDb       (Token,    McStat,   &CoreDb))
        OaGetObject       (Token,    McStat,   &Object);

    if (!Normal(*Status))
    {
         sprintf (Msg, "***ERROR... Object[%2u:%4u] Corrupted ... \n",
                  Token->DbId, Token->Handle);
         TraceMsg (0, Msg);

         if (HmPutObjectAddr (CoreDb, Token->Handle,  NullPtr, Dump_Status))
         if (OmGetObject     (Token,  Dump_Status,   &Index,       &Object))
             OaDumpObject    (0,      Token,                   Dump_Status);

         if (Normal(DumpStatus))
           *Status = Env_Normal;
    }

    if (Normal(*Status))
    if (HmGetObjectMemLoc
                         (CoreDb,    Token->Handle,  McStat,   &MemLoc))
    if (HmGetObjNum      (CoreDb,    Token->Handle,  McStat,   &ObjNum))
    if (Env_FetchFieldOffset 
                         (Token,    FieldTkns,      SubLevel,
                          McStat,   &Size, &Offset))
    if (Token->DbId > RenvId)
    if (MemLoc)
    if (AttrType   == aTknRefAttr)
       Offset      += sizeof(addrtype);

    if (Normal(*Status))
    {
       BytePtr  = (addrtype )((char *)Object    + Offset);
       MoveBytes            (BytePtr, Value,      Size);

       if (Token->DbId > RenvId)
       if (!MemLoc)
       if (FieldType  == aDbmsString)
       if (HmGetLocalObjNum 
                         (CoreDb,                               Token->Handle,
                          McStat,                              &LocalObjNum))
       if (MemGetAddr    (CoreDb->ObjHdrDir,        (indextype )LocalObjNum,
                          McStat,                  (addrtype *)&ObjHdr))
       {
           OaGetString (Token->DbId,  ObjHdr, *(longtype  *)BytePtr,
                        McStat,                    (char **)&StringPtr);
           MoveBytes   (StringPtr, (addrtype )Value,  sizeof (addrtype ));
       }

    }

    if (!Normal(*Status) || DeBug || OaBug)
    {
       if (sprintf (Msg, "   PUT Field :: AttrId=%4u; size (%4d) ::= ",
                   FieldTkns[SubLevel-1].AttrId, Size)) 
       if (TraceMsg   (0, Msg))
       if (TraceBytes (0, Value, Size))
       if (TraceMsg   (0, "\n"))
       if (sprintf (Msg, "   TO  object[%2u:%4u] offset %3u @%x\n", 
                    Token->DbId, Token->Handle, Offset, BytePtr))
           TraceMsg   (0, Msg);
    }       

TRACK(TrackBak,"OaGetField\n");
return (STAT);
}



boolean    OaChkForAttr  (tokentype    *Token,    numtype     EnvAttrNum,
                          ft F,lt Z,zz *Status,
                          sizetype     *Size,     sizetype   *Offset)
{ 
dbheader    *CoreDb       = NullPtr;
objheader   *ObjHdr       = NullPtr;
numtype      LocalAttrNum = 0;
numtype      LocalObjNum  = 0;
numtype      FieldNum     = 0;

    if (TmGetCoreDb      (Token,       McStat,      &CoreDb))
    if (HmGetLocalObjNum (CoreDb,                    Token->Handle,
                          McStat,                   &LocalObjNum))
    if (MemGetAddr       (CoreDb->ObjHdrDir,        (indextype )LocalObjNum,
                          McStat,                   (addrtype *)&ObjHdr))
        MemGetObjNum     (CoreDb->LocalAttrNumMap,  (indextype )EnvAttrNum,
                          McStat,                   &LocalAttrNum);
    if (!LocalAttrNum)
        *Status = Oa_AttrNotPresent;

    if (Normal(*Status))
    if (MemGetSize    (CoreDb->LocalAttrSizeTbl,    (indextype )LocalAttrNum,
                       McStat,                      (bytessize *)Size))
    if (MemGetObjNum  (ObjHdr->FieldNumMap,         (indextype )LocalAttrNum,
                       McStat,                      &FieldNum))
    if (!FieldNum)
        *Status = Oa_AttrNotPresent;

    if (Normal(*Status))
        MemGetSize    (ObjHdr->FieldOffsetDir,      (indextype )FieldNum,
                       McStat,                      (bytessize *)Offset);
    if (ClassBug)
    if (sprintf (Msg, "  OA CHK For Attr[%3u] local(%3u) size %u offset %u\n",
                 EnvAttrNum, LocalAttrNum, *Size, *Offset))
    if (TraceMsg (0, Msg))
    if (sprintf (Msg, "   IN Object[%2u :%4u] ::  Status= %3u\n", 
                Token->DbId, Token->Handle, *Status))
        TraceMsg (0, Msg);

TRACK(TrackBak,"OaChkForAttr\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/*                              UTILITY                                    */
/*+-----------------------------------------------------------------------+*/

boolean    OaGetAttrSize  (tokentype    *Anchor,     numtype   EnvAttrNum,
                           ft F,lt Z,zz *Status,     addrtype  AttrSize)
{ 
num          LocalAttrNum = 0;
dbheader    *CoreDb       = NullPtr;

    if (TmGetCoreDb   (Anchor,       McStat,       &CoreDb))
    if (MemGetObjNum  (CoreDb->LocalAttrNumMap,    (indextype )EnvAttrNum,
                       McStat,                     &LocalAttrNum))
    if (MemGetSize    (CoreDb->LocalAttrSizeTbl,   (indextype )LocalAttrNum,
                       McStat,                     (word *)AttrSize))

    if (!Normal(*Status) || DeBug)
    {
       sprintf (Msg, "   GET AttrSize[%3u] (%3u) ::=  %u \n",
                EnvAttrNum, LocalAttrNum,  *(num *)AttrSize);
       TraceMsg (0, Msg);
    }       

TRACK(TrackBak,"OaGetAttrSize\n");
return (STAT);
}
 

boolean    OaCompare     (boolean       CppType,    tokentype  *Token,
                          addrtype      Object,     numtype     EnvAttrNum,
                          addrtype      CompValue,  ptr_cmp     CmpCode,
                          ft F,lt Z,zz *Status)
{
static unsigned char      Value[MAXSTRING];
static addrtype           ValuePtr        =  (addrtype )&Value[0];
int          i               = 0;
bytessize    KeySize         = 0;
bytetype    *ByteValue       = NULL;
short       *IdValue         = NULL;
lt64        *IntValue        = NULL;
#ifdef LARGE
tokentype   *TknValue        = NULL;
tokentype   *CompTkn         = NULL;
#endif
tokentype     AttrTkn;
attrdesctype *AttrDesc       = NullPtr;

  if (TmIsValid      (Token,   McStat)  == False)
  {
    sprintf (Msg, 
         "\n ***ERROR*** OaCompare On Object[%2u:%4u]. Token is Invalid!\n", 
             Token->DbId, Token->Handle);
    TraceMsg (0,  Msg);
   *Status  = Oa_ObjectHasBeenDeleted;
  }
  AttrTkn.DbId     = RenvId;

  memset (Value,     NULL, MAXNAME);

  if (CmpCode != NullPtr)
  {
     TraceMsg (0, "     COMP Func Not Implemented\n");
    *Status = Err_NotImplemented;
  }

  if (Normal(*Status))
  if (EnvFetchAttrHandle (EnvAttrNum,  McStat,                &AttrTkn.Handle))
  if (OaGetObject        (&AttrTkn,    McStat,    (addrtype *)&AttrDesc))
      OaGetAttrSize      (Token,       EnvAttrNum, 
                          McStat,                 (addrtype  )&KeySize);

  if (Normal(*Status))
  if (OaGet          (CppType,    Token,    Object,           EnvAttrNum,
                      McStat,     ValuePtr))

  if (AttrDesc->TypeType  == aString)
  {
     i = Ut_CompareString ((char *)CompValue, (char *)ValuePtr);
/*
     i = strcmp  ((char *)CompValue,  (char *)ValuePtr);
     if (i == 0)
     if (strlen ((char *)CompValue) > strlen((char *)ValuePtr))
         i = 1;
*/
  } else switch (KeySize) {

    case sizeof (bytetype  )   :
                     ByteValue    = (bytetype *)ValuePtr;
                     if (*(bytetype *)CompValue < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)CompValue > *ByteValue)
                       i = 1;
                     break;

    case sizeof (short )     :
                     IdValue    = (short *)ValuePtr;
                     if (*(short *)CompValue < *IdValue)
                       i = -1;
                     else if (*(short *)CompValue > *IdValue)
                       i = 1;
                     break;

    case sizeof (lt64  ) :
                     IntValue    = (lt64 *)ValuePtr;
                     if (*(lt64 *)CompValue < *IntValue)
                       i = -1;
                     else if (*(lt64 *)CompValue > *IntValue)
                       i = 1;
                     break;

#ifdef LARGE
    case sizeof (tokentype )  :
                     TknValue    = (tokentype *)ValuePtr;
                     CompTkn     = (tokentype *)CompValue;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;
                     break;

#endif
    default                   :
                     if (KeySize <= sizeof (tokentype ))
                         i = memcmp  ((char *)CompValue, 
                                      (char *)ValuePtr, (size_t )KeySize);
                     else 
                         i = strncmp ((char *)CompValue, 
                                      (char *)ValuePtr, (size_t )KeySize);
  }

  if (Normal (*Status))
  if (!i)
  {
     if (DeBug || OaBug)
     if (sprintf (Msg, 
         "     COMPARED on Object [%2u:%4u] =",  Token->DbId, Token->Handle))
     if (TraceMsg (0, Msg))
     if (AttrDesc->TypeType  == aString)
     {
        sprintf (Msg, " %s\n", Value);
        TraceMsg   (0, Msg);
     } else {
        TraceBytes (0, ValuePtr, KeySize);
        TraceMsg   (0, "\n");
     }

  } else if (i < 0) {
    *Status = Oa_LessThen;

  } else {
    *Status = Oa_GtrThen;
  }

  if (DeBug || SetBug || OaBug)
  {
     if (KeySize            <= sizeof (tokentype )
     &&  AttrDesc->TypeType != aString)
     {
        sprintf    (Msg, "     OaCompare [%2u:%4u]  Value=",
                    Token->DbId, Token->Handle);
        TraceMsg   (0,   Msg);
        TraceBytes (0,   ValuePtr, KeySize);
        sprintf    (Msg, "; i = %d;",       i);
        TraceMsg   (0, Msg);
        TraceMsg   (0, " CompValue =");
        TraceBytes (0,   CompValue, KeySize);
        TraceMsg   (0, "\n");
     } else {
        sprintf (Msg, "     OaCompare Object[%2u:%4u]  =<%21s>; i = %d\n", 
                 Token->DbId, Token->Handle, Value, i);
        TraceMsg (0,  Msg);
        sprintf (Msg, "                     CompValue  =<%21s>\n", 
                 CompValue);
        TraceMsg (0,  Msg);
     }
  }

TRACK(TrackBak,"OaCompare\n");
return (STAT);
}
 

boolean    OaCompareField
                         (boolean       CppType,    tokentype  *Token,
                          addrtype      Object,     fieldstruc *FS,
                          addrtype      CompValue,  ptr_cmp     CmpCode,
                          ft F,lt Z,zz *Status)
{
static unsigned char      Value[MAXSTRING];
static addrtype           ValuePtr        =  (addrtype )&Value[0];
int          i               = 0;
bytessize    KeySize         = 0;
bytetype    *ByteValue       = NULL;
short       *IdValue         = NULL;
lt64        *IntValue        = NULL;
numtype      EnvAttrNum      = FS->FieldTkns[FS->SubLevel-1].AttrId;

#ifdef LARGE
tokentype   *TknValue        = NULL;
tokentype   *CompTkn         = NULL;
#endif

  memset (Value,     NULL, MAXNAME);

  if (CmpCode != NullPtr)
  {
     TraceMsg (0, "     COMP Func Not Implemented\n");
    *Status = Err_NotImplemented;
  }

  if (OaGetAttrSize  (Token, EnvAttrNum, McStat,   (addrtype )&KeySize))
  if (KeySize > MAXNAME)
     *Status = Err_NotImplemented;

  if (Normal(*Status))
  if (OaGetField     (Token,    FS,   McStat,     ValuePtr))

  switch (KeySize)
  {

    case sizeof (bytetype  )   :
                     ByteValue    = (bytetype *)ValuePtr;
                     if (*(bytetype *)CompValue < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)CompValue > *ByteValue)
                       i = 1;
                     break;

    case sizeof (short )     :
                     IdValue    = (short *)ValuePtr;
                     if (*(short *)CompValue < *IdValue)
                       i = -1;
                     else if (*(short *)CompValue > *IdValue)
                       i = 1;
                     break;

    case sizeof (lt64  ) :
                     IntValue    = (lt64 *)ValuePtr;
                     if (*(lt64 *)CompValue < *IntValue)
                       i = -1;
                     else if (*(lt64 *)CompValue > *IntValue)
                       i = 1;
                     break;

#ifdef LARGE
    case sizeof (tokentype )  :
                     TknValue    = (tokentype *)ValuePtr;
                     CompTkn     = (tokentype *)CompValue;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;
                     break;

#endif
    default                   :
                     if (KeySize <= sizeof (tokentype ))
                         i = memcmp  ((char *)CompValue, 
                                      (char *)ValuePtr, (size_t )KeySize);
                     else 
                         i = strncmp ((char *)CompValue, 
                                      (char *)ValuePtr, (size_t )KeySize);
  }

  if (Normal (*Status))
  if (!i)
  {
     if (DeBug || OaBug)
     if (sprintf (Msg, 
         "     COMPARED on Object [%2u:%4u] =",  Token->DbId, Token->Handle))
     if (TraceMsg (0, Msg))
     if (TraceBytes (0, ValuePtr, KeySize))
        TraceMsg (0, "\n");

  } else if (i < 0) {
    *Status = Oa_LessThen;

  } else {
    *Status = Oa_GtrThen;
  }

  if (DeBug || SetBug || OaBug)
  {
     if (KeySize <= sizeof (tokentype ))
     {
        sprintf    (Msg, "     OaCompare Object[%2u:%4u]  Value=",
                    Token->DbId, Token->Handle);
        TraceMsg   (0,   Msg);
        TraceBytes (0,   ValuePtr, KeySize);
        sprintf    (Msg, "; i = %d  ;",       i);
        TraceMsg   (0, Msg);
        sprintf    (Msg, " CompValue = %d  ;",  (int *)CompValue);
        TraceMsg   (0, Msg);
     } else {
        sprintf (Msg, "     OaCompare Object[%2u:%4u]  = %s; i = %d\n", 
                 Token->DbId, Token->Handle, Value, i);
        TraceMsg (0,  Msg);
     }
  }
TRACK(TrackBak,"OaCompareField\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+
~M                         END OF OA1.C                                   !
  +-----------------------------------------------------------------------+*/
