/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env - Sa.c */
#define SA_C
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "voa.h"
#include "oa0.h"
#include "env1.h"
#include "oa1.h"
#include "sa.h"
 
/*+-----------------------------------------------------------------------+
~ENV                             SA                                       !
  +-----------------------------------------------------------------------+*/
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        Alloc_RootSlots        (indextype  )50

#define        lKeyedSetHead_ObjNum   (objnumtype )73
#define        gKeyedSetHead_ObjNum   (objnumtype )74

#define        lSetNode_ObjNum        (objnumtype )85
#define        gSetNode_ObjNum        (objnumtype )86


#define        ObjNum_AttrNum         (numtype )5

#define        Nam_AttrNum            (numtype )19

#define        SetHeadType_AttrNum    (numtype )44

#define        OwnerAttrNum_AttrNum   (numtype )61
#define        MemberAttrNum_AttrNum  (numtype )62

#define        PrimaryKey_AttrNum     (numtype )64

#define        lOwnerObject_AttrNum   (numtype )88
#define        lMemberObject_AttrNum  (numtype )89

#define        gOwnerObject_AttrNum   (numtype )90

#define        lFirstLink_AttrNum     (numtype )91
#define        gFirstLink_AttrNum     (numtype )92

#define        lLastLink_AttrNum      (numtype )93
#define        gLastLink_AttrNum      (numtype )94

#define        lPrevLink_AttrNum      (numtype )95
#define        gPrevLink_AttrNum      (numtype )96

#define        lNextLink_AttrNum      (numtype )97
#define        gNextLink_AttrNum      (numtype )98

#define        RootNodes_AttrNum      (numtype )155

#define        RenvDescs_Hndl         (handletype )688

#endif

/*+-----------------------------------------------------------------------+*
~E                            EXTERMS                                     !
 *+-----------------------------------------------------------------------+*/
extern boolean OaDumpObject
                          (int           TraceSwi,  tokentype    *Token,
                           ft F,lt Z,zz *Status);

extern boolean OaDumpObjHdr
                          (int           TraceSwi,  tokentype    *Token,
                           numtype       ObjNum,    ft F,lt Z,zz *Status);

extern boolean KernelFindIn   (handletype    SetDesc,  tokentype *OwnerTkn,
                               addrtype      KeyValue,
                               ft F,lt Z,zz *Status,   tokentype *MemberTkn);

extern boolean KernelPutAttr  (tokentype *Token,       numtype    EnvAttrNum,
                               addrtype   AttrValue,   ft F,lt Z,zz *Status);
extern boolean KernelGetAttr  (tokentype *Token,       numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype   Value);


#define MemGetWord         Mem_GetWord
extern boolean MemGetWord     (numtype       Chunk,    indextype   Index,
                               ft F,lt Z,zz *Status,   word       *WordValue);
 
#define        MemGetLocalObjNum    Mem_GetWord


/*+-----------------------------------------------------------------------+
~SA                        SET ACTOR                                      !
  +-----------------------------------------------------------------------+*/
boolean SetInitSetHeads  (ft F,lt Z,zz *Status)
{
tokentype      HeadDescTkn;
int            i          = -1;
numtype        ObjNum     = 0;

    while (Normal(*Status)
    &&     ++i < NumHeadTypes)
    {
       if (KernelFindIn     (RenvDescs_Hndl, &RenvTkn, (addrtype )HeadNam[i],
                             McStat,         &HeadDescTkn))
       {
          if (KernelGetAttr (&HeadDescTkn,    ObjNum_AttrNum,
                             McStat,         &ObjNum))
           HeadObjNum[i] = ObjNum;
       } else {
          sprintf (Msg, "SetDesc[%u] %s :: not a Desc\n",
                   i, HeadNam[i]);
          TraceMsg (0, Msg);
         *Status = Set_NotAnObjDesc;
       }
    }
    sprintf (Msg, " SET HEADS  loaded ... HeadObjNum[2]= %2u\n",
             HeadObjNum[2]);
    TraceMsg (0, Msg);

TRACK(TrackBak,"SetInitSetHeads\n");
return(STAT);
}


boolean    SaAddInto     (handletype    SetDescHndl,
                          tokentype    *OwnerTkn, tokentype *MemberTkn,
                          ft F,lt Z,zz *Status,   tokentype *NodeTkn)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc         = NullPtr;
settype          SetDescType     = aLocalSet;
objtype         *Owner           = NullPtr;
objtype         *Member          = NullPtr;
tokentype        SetMemberTkn;
lsetheaddesc    *SetHead         = NullPtr;
tokentype        SetHeadTkn;
lsetnodedesc    *SetNode         = NullPtr;
gsetnodedesc    *gSetNode        = NullPtr;
lsetnodedesc    *NewSetNode      = NullPtr;
gsetnodedesc    *gNewSetNode     = NullPtr;
tokentype        SetNodeTkn;
tokentype        NewSetNodeTkn;
tokentype        CurrTkn;
tokentype        PrevTkn;
count            Members         = 0;
bytessize        KeySize         = 0;
addrtype         Dummy           = PtrToVoid;
ptr_cmp          CmpCode         = NullPtr;
unsigned char    Value[MAXSTRING];
addrtype         ValuePtr        =  (addrtype )&Value[0];
statustype       DumpStatus      = Env_Normal;
int              TmpDeBug        = DeBug;
indextype        One             = 1;
tokentype        VchunkTkn;
tokentype        VrootToken;
indextype        AllocQty        = 0;
bytetype         Size1Value      = 0;
wordtype         Size2Value      = 0;
numtype          PrimaryKey      = 0;
numtype          MemberAttrNum   = 0;
numtype          NodeObjNum      = 0;

  memset (Value, NULL, MAXNAME);

  SetHeadTkn.DbId   = OwnerTkn->DbId;
  SetNodeTkn.DbId   = OwnerTkn->DbId;
  SetMemberTkn.DbId = OwnerTkn->DbId;
  CurrTkn.DbId      = OwnerTkn->DbId;
  PrevTkn.DbId      = OwnerTkn->DbId;
  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;


  if (OaGetObject   (OwnerTkn,            McStat,   (addrtype *)&Owner)) 
  if (OaGetObject   (MemberTkn,           McStat,   (addrtype *)&Member)) 
  if (OaGetObject   (&SetDescTkn,         McStat,   (addrtype *)&SetDesc)) 
  {
     PrimaryKey     = SetDesc->PrimaryKey;
     SetDescType    = SetDesc->SetType;
     MemberAttrNum  = SetDesc->MemberAttrNum;
     NodeObjNum     = SetDesc->NodeObjNum;
  }

  if (Normal(*Status))
      OaGet        (False,               OwnerTkn, (addrtype  )Dummy,
                    SetDesc->OwnerAttrNum,
                    McStat,              (addrtype )&(SetHeadTkn.Handle));

  if (!Normal(*Status))
  {
     *Status   = Set_NotAnOwnerClass;

      sprintf (Msg,"***ERROR*** OwnerAttr[%4u] Not of Owner Object\n",
                SetDesc->OwnerAttrNum);
      TraceMsg (0, Msg);
      sprintf (Msg, " AT AddIntoSet:: %14s SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
      TraceMsg (0, Msg);
      sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle);
      TraceMsg (0, Msg);
      TraceMsg (0, "   For SetDesc...\n");
      OaDumpObject (0, &SetDescTkn, Dump_Status);

  }

  if (Normal(*Status))
  if (OaGetAttrSize (&RenvTkn,                        PrimaryKey, 
                      McStat,             (addrtype )&KeySize))
  if (KeySize > MAXNAME)
     *Status = Err_NotImplemented;


  if (Normal(*Status))
  if (SetHeadTkn.Handle == NullHandle)
  {
     if (SetDesc->SetType   == aGlobalKeyedSet)
        OaCreateObject  (OwnerTkn,        gKeyedSetHead_ObjNum,
                         McStat,         &SetHeadTkn);
     else
        OaCreateObject  (OwnerTkn,        lKeyedSetHead_ObjNum,
                         McStat,         &SetHeadTkn);

     if (Normal(*Status))
     if (OaGetObject    (&SetHeadTkn,     McStat,     (addrtype *)&SetHead))
     {
        SetHead->BuildDesc     = SetDescHndl;
        SetHead->lOwnerObject  = OwnerTkn->Handle;
        SetHead->lFirstLink    = Null_Handle;
        SetHead->lLastLink     = Null_Handle;
        OaPut (False, OwnerTkn, (addrtype )Owner, SetDesc->OwnerAttrNum,
               (addrtype )&(SetHeadTkn.Handle),   McStat);
     }

  } else {
     if (OaGetObject    (&SetHeadTkn,     McStat,     (addrtype *)&SetHead))
         OaUpdateObject (&SetHeadTkn,     McStat);
  }

  if (Normal(*Status))
  if (KernelGetAttr (MemberTkn, PrimaryKey, McStat,  ValuePtr))
  {
        if (SetBug || DeBug)
        if (sprintf (Msg, "\n  SetAddInto %14s[%2u:%4u];  Owner[%2u:%4u]\n",
                     SetDesc->Nam,      SetDescTkn.DbId,  SetDescTkn.Handle,
                     OwnerTkn->DbId,    OwnerTkn->Handle))
        if (TraceMsg (0, Msg))
        if (sprintf (Msg, "              Member[%2u:%4u] Key(*%u) = ",
                     MemberTkn->DbId,    MemberTkn->Handle, KeySize))
        if (TraceMsg (0, Msg))

        if (PrimaryKey == Nam_AttrNum)
        {
            sprintf   (Msg, " %s\n", (char *)Value);
            TraceMsg  (0, Msg);
        } else {
           if (TraceBytes (0, Value, KeySize))
              TraceMsg (0, "\n");
        }

        if (SetBug || DeBug)
        if (sprintf (Msg, "     Set Head    [%2u:%4u] @ %x\n", 
                     SetHeadTkn.DbId, SetHeadTkn.Handle, SetHead))
        if (TraceMsg (0, Msg))
        if (sprintf (Msg, "             ->First link [%2u:%4u] \n", 
                     SetHeadTkn.DbId, SetHead->lFirstLink))
        if (TraceMsg (0, Msg))
        if (sprintf (Msg, "             ->Last  link [%2u:%4u] \n",
                     SetHeadTkn.DbId,  SetHead->lLastLink))
           TraceMsg (0, Msg);
        if (SetBug || DeBug)
           OaDumpObject (0, &SetHeadTkn, Dump_Status);
 
 } else {

      sprintf (Msg,"***ERROR*** Primary Key Attr[%4u] Not of Member Class\n",
                PrimaryKey);
      TraceMsg (0, Msg);
      sprintf (Msg, " AT AddIntoSet:: %14s SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
      TraceMsg (0, Msg);
      sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle);
      TraceMsg (0, Msg);
     *Status = Set_SetNotOfObjCestrs;
  }

  if (Normal(*Status)) 
  if (SetDescType    == aIndexedSet
  ||  SetDescType    == aLocalKeyedSet
  ||  SetDescType    == aGlobalKeyedSet)
  {
 /*.........................................................................*/
      if (SetHead->lFirstLink == NullHandle)
      {
          if (PrimaryKey   == Nam_AttrNum) 
             AllocQty                = 53;
          else if (SetDescType == aIndexedSet)
          {
             SetHead->lFirstLink     = One;
             SetHead->lLastLink      = NullHandle;
             AllocQty                = Alloc_DbObjs;
          } else if (PrimaryKey != ObjNum_AttrNum) 
             AllocQty                = Alloc_RootSlots;
          else
             AllocQty                = 0;

          if (AllocQty)
          if (SetHead->RootNodes ==  0)
          if (OaCreateVchunk  (&SetHeadTkn,     sizeof (handletype),
                               AllocQty,        Extend_Fields,
                               McStat,         &VrootToken))
              KernelPutAttr   (&SetHeadTkn,     RootNodes_AttrNum, 
                               (addrtype )&(VrootToken.Handle),
                               McStat);
      }

      if (Normal(*Status)) 
      if (SetDescType == aIndexedSet)
      {
/* This is a convolution of Addresing for Architectures with 
   ******************    HIGH to LOW Byte Postioning  *******************  */
         if (KeySize <= sizeof (idtype))
         {
             memcpy ((char *)&Size2Value, (char *)ValuePtr, KeySize);

             Size1Value  = (bytetype )Size2Value;
             Size2Value  = Size1Value;
             memcpy ((char *)ValuePtr,   (char *)&Size2Value, 
                     sizeof (wordtype));

             if (SetBug || DeBug)
             if (sprintf (Msg, 
             "     KeyValue Resize From(*%2u)= %u -> (*%2u)= %u =>=> (%2u)%u\n",
                          KeySize,  Size1Value, sizeof(wordtype), Size2Value,
                          sizeof (wordtype), *(wordtype *)ValuePtr))
                 TraceMsg (0, Msg);
         }
/*  ******************    HIGH to LOW Byte Postioning  *******************  */

         if (SaPutRootNode (&SetHeadTkn,    SetHead,  &SetDescTkn, ValuePtr, 
                              MemberTkn,     McStat))
             *Status   = Env_Normal;
         if (Normal(*Status))
             OaPut          (False,          MemberTkn, (addrtype  )Member, 
                             MemberAttrNum,
                             (addrtype )&SetHeadTkn,                McStat);
         if (DeBug)
             OaDumpObject (0, &SetHeadTkn, Dump_Status);
         DumpStatus = Env_Normal;
         DeBug = TmpDeBug;

         TRACK(TrackBak,"SetAddInTo\n");
         return (STAT);
      } 
  }
 /*.........................................................................*/

  if (Normal(*Status))


  if (OaCreateObject (OwnerTkn,       NodeObjNum,  McStat,     &NewSetNodeTkn))
  if (OaGetObject    (&NewSetNodeTkn, McStat,      (addrtype *)&NewSetNode))
  if (OaPut          (False,          MemberTkn,   (addrtype   )Member, 
                      MemberAttrNum,
                      (addrtype )&NewSetNodeTkn,                McStat))
  {
      NewSetNode->lOwnerObject   = SetHeadTkn.Handle;
      NewSetNode->lPrevLink      = Null_Handle;
      NewSetNode->lNextLink      = Null_Handle;
      CurrTkn.Handle             = Null_Handle;
      PrevTkn.Handle             = Null_Handle;

      if (SetDesc->SetType         == aGlobalKeyedSet)
      {
         gNewSetNode                = (gsetnodedesc *)NewSetNode;
         gNewSetNode->gMemberObject = *MemberTkn;

      } else 
         NewSetNode->lMemberObject  = MemberTkn->Handle;

      if (SetBug || DeBug)
        OaDumpObject (0, &NewSetNodeTkn, Dump_Status);
  }

  if (Normal(*Status))
  if (SetHead->lFirstLink == NullHandle)
  {
        SetHead->lFirstLink        = NewSetNodeTkn.Handle;
        SetHead->lLastLink         = NewSetNodeTkn.Handle;

        SetNodeTkn.Handle          = Null_Handle;

        *Status                    = Set_InSerted;
        if (DeBug >= 2)
           OaDumpObject (0, &SetHeadTkn, Dump_Status);
        DumpStatus = Env_Normal;

  } else {
     if (SetHead->lLastLink)
     {
       SetNodeTkn.Handle  =  SetHead->lLastLink;
       if (OaGetObject (&SetNodeTkn,   McStat,   (addrtype *)&SetNode)) 
       {
          if (SetDesc->SetType    == aGlobalKeyedSet)
          {
             gSetNode              = (gsetnodedesc *)SetNode;
             SetMemberTkn          = gSetNode->gMemberObject;
          } else
             SetMemberTkn.Handle   = SetNode->lMemberObject;

          OaCompare (False,    &SetMemberTkn, Dummy,  PrimaryKey,
                     ValuePtr,  CmpCode,      McStat);
          if (*Status == Oa_GtrThen)
          {
            *Status = Env_Normal;
             PrevTkn.Handle = SetNode->Handle;
             CurrTkn.Handle = NullHandle;
            *Status = Set_GtrThen;

          } else if (*Status == Oa_LessThen)
             *Status = Env_Normal;
       }
     }

     if (Normal(*Status))
     if (SaGetRootNode (&SetHeadTkn,  SetHead,  &SetDescTkn,   ValuePtr, 
                          McStat,     &CurrTkn,  &VchunkTkn))
        *Status = Set_GtrThen;

     while ((*Status        == Set_GtrThen)
     &&     (CurrTkn.Handle != NullHandle ))
     {
       SetNodeTkn.Handle  =  CurrTkn.Handle;
      *Status = Env_Normal;
       if (OaGetObject (&SetNodeTkn,   McStat,   (addrtype *)&SetNode)) 
       {
          if (SetDesc->SetType      == aGlobalKeyedSet)
          {
             gSetNode              = (gsetnodedesc *)SetNode;
             SetMemberTkn          = gSetNode->gMemberObject;

          } else
             SetMemberTkn.Handle   = SetNode->lMemberObject;

          if (DeBug >= 2)
          if (sprintf (Msg,
                       "     SETNODE [%2u:%4u] @ %x  lPrevLink [%2u:%4u]\n",
                       SetNodeTkn.DbId, SetNodeTkn.Handle, 
                       SetNode, SetNodeTkn.DbId, SetNode->lPrevLink))
          if (TraceMsg (0, Msg))
          if (sprintf (Msg,
                     "                                 lNextLink [%2u:%4u]\n",
                       SetNodeTkn.DbId, SetNode->lNextLink))
          if (TraceMsg (0, Msg))
          if (sprintf (Msg, "      SETMEMBER [%2u:%4u] \n",  
                       SetMemberTkn.DbId, SetMemberTkn.Handle))
              TraceMsg (0, Msg);
          Members++;
          OaCompare (False,    &SetMemberTkn, Dummy,  PrimaryKey,
                     ValuePtr,  CmpCode,      McStat);
       }

       if (Normal(*Status))
       {
          *Status = Set_AlreadyInSet;
          if (SetBug || DeBug)
          if (sprintf  (Msg,
                        "     Member[%2u:%4u]  Already In Set\n", 
                        MemberTkn->DbId,  MemberTkn->Handle))
          if (TraceMsg (0, Msg))

          if (PrimaryKey == Nam_AttrNum)
          {
              sprintf   (Msg, " %s\n", (char *)Value);
              TraceMsg  (0, Msg);
          } else {
             if (TraceBytes (0, Value, KeySize))
                TraceMsg (0, "\n");
          }

       } else if (*Status == Oa_LessThen) {
          *Status = Env_Normal;
          PrevTkn.Handle  = SetNode->lPrevLink;
          if (SetBug ||  DeBug)
          if (sprintf  (Msg, "     Current Member[%2u:%4u]  > New[%2u:%4u]\n", 
                        SetMemberTkn.DbId,    SetMemberTkn.Handle,
                        MemberTkn->DbId,      MemberTkn->Handle))
          if (TraceMsg (0, Msg))
          if (sprintf  (Msg, "     PrevTkn [%2u,%4u], SetNodeTkn[%2u:%4u]\n", 
                        PrevTkn.DbId,    PrevTkn.Handle,
                        SetNodeTkn.DbId, SetNodeTkn.Handle))
             TraceMsg  (0, Msg);
          if (KernelPutAttr (&SetNodeTkn,    lPrevLink_AttrNum,
                             (addrtype )&(NewSetNodeTkn.Handle),   McStat))
          if (KernelPutAttr (&NewSetNodeTkn, lPrevLink_AttrNum,
                             (addrtype )&(PrevTkn.Handle),   McStat))
          if (KernelPutAttr (&NewSetNodeTkn, lNextLink_AttrNum,
                             (addrtype )&(SetNodeTkn.Handle),   McStat))
          if (PrevTkn.Handle)
              KernelPutAttr (&PrevTkn,       lNextLink_AttrNum,
                             (addrtype )&(NewSetNodeTkn.Handle),   McStat);
          else
              KernelPutAttr (&SetHeadTkn,    lFirstLink_AttrNum,
                             (addrtype )&(NewSetNodeTkn.Handle),   McStat);
 
          DumpStatus = Env_Normal;
          if (DeBug)
             OaDumpObject (0, &NewSetNodeTkn, Dump_Status);

          if (DeBug)
              TraceMsg (0, "   Inserted Into Set \n");
          DeBug     = TmpDeBug;

          if (Normal(*Status))
            *Status   = Set_InSerted;
          else {
             OaDumpObject  (0, &SetHeadTkn, Dump_Status);
             VrootToken.DbId        = SetHeadTkn.DbId;
             VrootToken.Handle      = SetHead->RootNodes;
             OaDumpVchunk  (&VrootToken,    Dump_Status);
          }
       } else if (*Status == Oa_GtrThen) {
          PrevTkn.Handle             = SetNode->Handle;
          CurrTkn.Handle             = SetNode->lNextLink;
         *Status                     = Set_GtrThen;
       }
     }
  }

  if (*Status == Set_InSerted)
  {
     *Status   = Env_Normal;

  } else if ((*Status        == Set_GtrThen)
        &&   (CurrTkn.Handle == Null_Handle))
  {
     *Status  = Env_Normal;
      if (SetBug || DeBug)
      if (sprintf  (Msg, "     AppendToSet... Member[%2u:%4u]\n", 
                    MemberTkn->DbId,      MemberTkn->Handle))
         TraceMsg (0, Msg);
      if (DeBug)
      if (sprintf  (Msg, "       PrevTkn [%2u:%4u], SetNodeTkn[%2u:%4u]\n", 
                    PrevTkn.DbId,    PrevTkn.Handle,
                    SetNodeTkn.DbId, SetNodeTkn.Handle))
         TraceMsg (0, Msg);
      if (KernelPutAttr (&SetHeadTkn,                    lLastLink_AttrNum,
                         (addrtype )&NewSetNodeTkn.Handle, McStat))
      if (KernelPutAttr (&NewSetNodeTkn,                 lPrevLink_AttrNum,
                         (addrtype )&PrevTkn.Handle,       McStat))
      if (PrevTkn.Handle)
          KernelPutAttr (&PrevTkn,                       lNextLink_AttrNum,
                         (addrtype )&NewSetNodeTkn.Handle, McStat);
      if (DeBug) 
          OaDumpObject  (0, &NewSetNodeTkn, Dump_Status);
  }

  if (Normal(*Status))
  if (SaPutRootNode (&SetHeadTkn,    SetHead,  &SetDescTkn,   ValuePtr, 
                      &NewSetNodeTkn, McStat))
     *NodeTkn      =   NewSetNodeTkn;

if (*Status != Set_AlreadyInSet)
{

  if (SetBug || DeBug || !Normal(*Status))
  {
     if (sprintf (Msg,
              "   AddedIntoSet %14s [%2u:%4u];  Owner [%2u:%4u]\n", 
              SetDesc->Nam,    SetDescTkn.DbId, SetDescTkn.Handle,
                               OwnerTkn->DbId,  OwnerTkn->Handle))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg,
                  "                              Member[%2u:%4u]\n", 
              MemberTkn->DbId,  MemberTkn->Handle))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg,
                  "      SetNode [%2u:%4u]       lPrevLink [%2u:%4u]\n",
                  NewSetNodeTkn.DbId, NewSetNodeTkn.Handle, 
                  PrevTkn.DbId, PrevTkn.Handle))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg,
                  "                              lNextLink [%2u:%4u]\n",
                  SetNodeTkn.DbId,    SetNodeTkn.Handle))
         TraceMsg (0, Msg);
     DumpStatus = Env_Normal;

     if (Normal(*Status) && DeBug)
        OaDumpObject (0, &NewSetNodeTkn, Dump_Status);
  }
  DeBug = TmpDeBug;
}

TRACK(TrackBak,"SetAddInTo\n");
return (STAT);
}
 

boolean    SaFindIn      (handletype    SetDescHndl, tokentype    *OwnerTkn,
                          addrtype      KeyValue,    ft F,lt Z,zz *Status,  
                          tokentype    *NodeTkn,     tokentype    *MemberTkn)

{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
settype          SetDescType = aLocalSet;
lsetheaddesc    *SetHead     = NullPtr;
tokentype        SetHeadTkn;
lsetnodedesc    *SetNode     = NullPtr;
gsetnodedesc    *gSetNode    = NullPtr;
tokentype        SetNodeTkn;
tokentype        VchunkTkn;
tokentype        CurrTkn;
tokentype        PrevTkn;
count            Members     = 0;
bytessize        KeySize     = 0;
addrtype         Dummy       = NullPtr;
ptr_cmp          CmpCode     = NullPtr;
statustype       DumpStatus  = Env_Normal;
numtype          PrimaryKey  = 0;

  SetHeadTkn.DbId   = OwnerTkn->DbId;

  SetNodeTkn.DbId   = OwnerTkn->DbId;
  SetNodeTkn.Handle = OwnerTkn->DbId;

  CurrTkn.DbId      = OwnerTkn->DbId;
  PrevTkn.DbId      = OwnerTkn->DbId;

  MemberTkn->DbId   = OwnerTkn->DbId;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (SetBug || DeBug >= 1)
  if (sprintf (Msg, "\n   FindInSet:: SetTkn[%2u:%4u]... OwnerTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->DbId,   OwnerTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (&SetDescTkn,
                     McStat,                   (addrtype *)&SetDesc)) 
  if (OaGet         (False,                     OwnerTkn, 
                     (addrtype )Dummy,          SetDesc->OwnerAttrNum,
                     McStat,        (addrtype )&SetHeadTkn.Handle))
      PrimaryKey   = SetDesc->PrimaryKey;

  if (*Status  == Oa_AttrNotPresent)
      *Status   = Set_NotAnOwnerClass;

  if (Normal(*Status))
  if (DeBug || SetBug)
  if (sprintf  (Msg, "    SetDesc @%8x ->PrimaryKey @%8x; KeyAttrNum=%3u\n",
                SetDesc,  &SetDesc->PrimaryKey, PrimaryKey))
      TraceMsg (0,   Msg);

  if (DeBug)
      OaDumpObject  (0,  &SetDescTkn,   McStat);

/*
  if (TraceMsg (0,   Msg))
  if (OaGet         (False,                    &SetDescTkn, 
                     (addrtype )Dummy,          PrimaryKey_AttrNum,
                     McStat,        (addrtype )&PrimaryKey))
  if (sprintf  (Msg, "                      PrimaryKeyAttr= %3u\n", 
                PrimaryKey))
      TraceMsg (0,   Msg);
*/

  if (Normal(*Status))
  if (OaGetAttrSize (&RenvTkn,                  PrimaryKey, 
                      McStat,       (addrtype )&KeySize))

  if (SetHeadTkn.Handle == NullHandle)
  {
      if (DeBug || SetBug)
        TraceMsg (0, "    ... Empty Set.\n");
     *Status = Set_EmptySet;
      SetNodeTkn.Handle = NullHandle;
  }

  if (Normal(*Status))
  if (OaGetObject (&SetHeadTkn,   McStat,   (addrtype *)&SetHead))
  {
     if (SetBug || DeBug)
     if (sprintf (Msg, 
                 "\n  FindInSet  %14s[%2u:%4u];  Owner[%2u:%4u] Key(* %2u)=", 
                  SetDesc->Nam,    SetDescTkn.DbId,  SetDescTkn.Handle,
                  OwnerTkn->DbId,  OwnerTkn->Handle, KeySize))
     if (TraceMsg (0, Msg))
     if (PrimaryKey == Nam_AttrNum)
     {
        sprintf   (Msg, " %s\n", (char *)KeyValue);
        TraceMsg  (0, Msg);
     } else {
        if (KeySize == sizeof (numtype))
        {
           sprintf  (Msg, "%6u\n", *(numtype *)KeyValue);
           TraceMsg (0, Msg);

        } else {
           TraceBytes (0, KeyValue, KeySize);
           TraceMsg (0, "\n");
        }
     }

     if (DeBug || SetBug)
     if (OaDumpObject   (0,  &SetHeadTkn, McStat))
     if (sprintf (Msg, "     KeyAttr (%3u) size %u \n",
                  PrimaryKey, KeySize))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "     SetHead    [0:%4u] @ %x\n", 
                  SetHeadTkn.Handle, SetHead))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "             ->First link [0:%4u] \n", 
                  SetHead->lFirstLink))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "             ->Last  link [0:%4u] \n",
                  SetHead->lLastLink))
        TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  if (SetHead->lFirstLink == NullHandle)
     *Status = Set_EmptySet;
  else {
     PrevTkn.Handle = NullHandle;
     if (SaGetRootNode (&SetHeadTkn,   SetHead,   &SetDescTkn,   KeyValue,
                          McStat,      &CurrTkn,   &VchunkTkn))
     if (*Status == Set_IsAnIndexedSet)
     {
         if (CurrTkn.Handle)
         {
            MemberTkn->Handle = CurrTkn.Handle;
           *Status = Set_Found;

         } else
           *Status = Set_NotFound;
     } else
        *Status = Set_GtrThen;

     while ((Normal(*Status) || *Status == Set_GtrThen)
     &&      CurrTkn.Handle  !=  NullHandle)
     {
       SetNodeTkn.Handle      =  CurrTkn.Handle;
      *Status                 = Env_Normal;
       if (OaGetObject (&SetNodeTkn,   McStat,   (addrtype *)&SetNode))
       {
          if (SetDescType    == aGlobalKeyedSet)
          {
            gSetNode          = (gsetnodedesc *)SetNode;
           *MemberTkn         = gSetNode->gMemberObject;
          } else 
            MemberTkn->Handle = SetNode->lMemberObject;

          if (TmIsValid   (MemberTkn,   McStat))
          {
             if (DeBug || SetBug)
             if (sprintf (Msg, 
                          "      SetNode [%4u:%8u] lNextLink [%4u:%8u]\n",
                      SetNodeTkn.DbId,  SetNodeTkn.Handle, 
                      SetNodeTkn.DbId,  SetNode->lNextLink))
             if (TraceMsg (0, Msg))
             if (sprintf (Msg, 
                          "      Member  [%4u:%8u] lPrevLink [%4u:%8u]\n",  
                      SetNodeTkn.DbId,  SetNode->lPrevLink))
                 TraceMsg (0, Msg);

             Members++;

             PrevTkn.Handle = CurrTkn.Handle;
             OaCompare (False,    MemberTkn, Dummy,   PrimaryKey,
                        KeyValue, CmpCode,   McStat);

          } else {
             TraceMsg (0, "\n ***WARNING... Invalid MemberTkn in Set\n");
             sprintf (Msg, "      SetNode [%4u:%8u] lNextLink [%4u:%8u]\n",
               SetNodeTkn.DbId,  SetNodeTkn.Handle, 
               SetNodeTkn.DbId,  SetNode->lNextLink);
             TraceMsg (0, Msg);
             sprintf (Msg, "      Member  [%4u:%8u] \n",  
                      MemberTkn->DbId, MemberTkn->Handle);
             TraceMsg (0, Msg);

             CurrTkn.Handle = SetNode->lNextLink;
             TraceMsg (0, "\n Set Head::\n");
             OaDumpObject      (0, &SetHeadTkn, McStat);
             TraceMsg (0, "\n Set Node::\n");
             OaDumpObject      (0, &CurrTkn, McStat);
             *Status  = Env_Normal;
              if (SaDeleteNode  (SetDescHndl,  &NullToken,  
				 &SetNodeTkn, McStat))
                 *Status         = Set_GtrThen;
          }
       }

       if (Normal(*Status))
       {
         *Status      = Set_Found;
         *NodeTkn     = SetNodeTkn;

          if (SetBug || DeBug)
          if (sprintf  (Msg, "     MATCHED in Member[%2u:%4u]::(%3u) \n", 
                    MemberTkn->DbId,      MemberTkn->Handle, 
                    PrimaryKey))
             TraceMsg (0, Msg);
       } else if (*Status == Oa_GtrThen || *Status == Oa_LessThen) {
          if (DeBug || SetBug)
          if (sprintf  (Msg, "     NO Match in Member[%2u:%4u]\n", 
                        MemberTkn->DbId,      MemberTkn->Handle))
             TraceMsg (0, Msg);
          if (*Status == Oa_LessThen)
              *Status = Set_LessThen;
          else
              *Status = Set_GtrThen;
          if (CurrTkn.Handle == SetNode->lNextLink)
             *Status = Set_BadSet;
          else if (*Status == Set_GtrThen) 
            CurrTkn.Handle = SetNode->lNextLink;
       }
     }
  }

  if (*Status == Set_Found)
     *Status = Env_Normal;
  else
     MemberTkn->Handle = NullHandle;

  if (*Status == Set_GtrThen) {
      if (DeBug  || SetBug)
      if (sprintf (Msg,
                   "     NoMatchInSet ... End Of List. LastMember[%2u:%4u]\n",
                   PrevTkn.DbId, PrevTkn.Handle))
         TraceMsg (0, Msg);
     *Status = Set_NotFound;
  } else if (*Status == Set_LessThen) {
      if (DeBug || SetBug)
      if (sprintf (Msg,
                   "     NoMatchInSet ... Less Than CurrentMember[%2u:%4u]\n",
                   CurrTkn.DbId, CurrTkn.Handle))
         TraceMsg (0, Msg);
     *Status = Set_NotFound;
  }

  if (!Normal(*Status)
  &&  *Status != Set_NotFound) {
      if ( *Status != Set_EmptySet ) {
          sprintf (Msg,
                   "  @@FINDIN %14s [0:%4u] BY Key= %3u WITH  Owner[0:%4u]\n", 
              SetDesc->Nam,    SetDescTkn.Handle,
              *(word *)KeyValue, OwnerTkn->Handle);
          TraceMsg (0, Msg);
          sprintf  (Msg, "     BAD FINDIN for Member[%2u:%4u]\n", 
                    MemberTkn->DbId,      MemberTkn->Handle);
          TraceMsg (0, Msg);
          if (SetNode)
          {
          sprintf (Msg, "      SETNODE [0:%4u] @ %x  lNextLink [0:%4u]\n",
                   SetNodeTkn.Handle, SetNode, SetNode->lNextLink);
          TraceMsg (0, Msg);
          }
          sprintf (Msg, "      SETMEMBER [0:%4u] \n",  MemberTkn->Handle);
          TraceMsg (0, Msg);
          sprintf (Msg, "      for SETHEAD    [0:%4u] @ %x\n", 
                   SetHeadTkn.Handle, SetHead);
          TraceMsg (0, Msg);
          if (SetHead)
          {
          sprintf (Msg, "                 ->FIRST link [0:%4u] \n",
                   SetHead->lFirstLink);
          TraceMsg (0, Msg);
          }
      } else
         *Status = Set_NotFound;
  } else if (DeBug 
         &&  SetNodeTkn.Handle
         &&  (*Status != Set_EmptySet)) {
     TraceMsg (0, "  DUMP  SetNodeTkn @ Exit from FindIN \n");
     OaDumpObject (0, &SetNodeTkn, Dump_Status);

  } else if (*Status == Set_NotFound && (SetBug || DeBug)) {
     sprintf (Msg,
             "  in SET %14s [%2u:%4u] Owner[0:%4u]\n", 
              SetDesc->Nam,    SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->Handle);
     TraceMsg (0, Msg);
     sprintf  (Msg, "     NOTFOUND for SetType[%2u] Member[%2u:%4u] Key=%u\n", 
               SetDescType, MemberTkn->DbId,  MemberTkn->Handle,
               *(word *)KeyValue);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"SetFindIn\n");
return (STAT);
}
 
boolean    SaPutRootNode (tokentype     *SetHeadTkn, lsetheaddesc *SetHead,
                          tokentype     *SetDescTkn, addrtype      KeyValue,
                          tokentype     *SetNodeTkn, ft F,lt Z,zz *Status)
{
tokentype        RootTkn;
tokentype        MemberTkn;
tokentype        VrootToken;
indextype        RootStackPtr    = 0;
handletype      *RootNodes       = NullPtr;
lsetnodedesc    *SetNode         = NullPtr;
gsetnodedesc    *gSetNode        = NullPtr;
indextype        KeyIndex        = 0;
char            *KeyPtr          = (char *)KeyValue;
addrtype         Dummy           = PtrToVoid;
ptr_cmp          CmpCode         = NullPtr;
unsigned char    Value[MAXSTRING];
setdesctype     *SetDesc         = NullPtr;
settype          SetDescType     = aLocalSet;
numtype          PrimaryKey      = 0;
numtype          AllocSize       = 0;
statustype       DumpStatus      = Env_Normal;
int              k               = 0;

   memset (Value, NULL, MAXNAME);

   if (SetHead->RootNodes)
   {
       RootTkn.DbId           = SetHeadTkn->DbId;
       MemberTkn.DbId         = SetHeadTkn->DbId;
       VrootToken.DbId        = SetHeadTkn->DbId;
       VrootToken.Handle      = SetHead->RootNodes;
       if (OaGetObject     (SetDescTkn,     McStat, (addrtype *)&SetDesc))
       {
           PrimaryKey  = SetDesc->PrimaryKey;
           SetDescType = SetDesc->SetType;
       }

       if (Normal(*Status))
       if (OaGetVchunkAddr     (&VrootToken,               McStat,
                                (addrtype *)&RootNodes))
       if (OaGetVchunkStackPtr (&VrootToken,               McStat,
                                &RootStackPtr))
       if (SetBug)
       if (sprintf (Msg, "   Put RootNode[<=%3u] ... \n", RootStackPtr))
           TraceMsg (0, Msg);

       if (Normal(*Status))
       if (PrimaryKey        == Nam_AttrNum)
       {
           if (!RootStackPtr)
           if (OaPutVchunkStackPtr (&VrootToken,          53,  McStat))
               OaGetVchunkAddr     (&VrootToken,               McStat,
                                    (addrtype *)&RootNodes);
 
           KeyIndex         = (indextype )KeyPtr[0] - (indextype )64;
           if (KeyIndex
           &&  KeyIndex    <= 58) {
              if (KeyIndex  > 26)
                  KeyIndex -= 6;

              if (!RootNodes[KeyIndex])
                 RootNodes[KeyIndex]  = SetNodeTkn->Handle;
              else {
                 RootTkn.Handle   = RootNodes[KeyIndex];
                 if (OaGetObject (&RootTkn,   McStat, (addrtype *)&SetNode))
                 if (SetDescType       == aGlobalKeyedSet)
                 {
                    gSetNode           = (gsetnodedesc *)SetNode;
                    MemberTkn          = gSetNode->gMemberObject;
                 } else 
                    MemberTkn.Handle   = SetNode->lMemberObject;

                 if (Normal(*Status))
                     OaCompare     (False,     &MemberTkn,           Dummy,
                                    PrimaryKey,
                                    KeyValue,   CmpCode,             McStat);
                 if (*Status == Oa_GtrThen)
                 {
                     *Status = Env_Normal;
                      RootNodes[KeyIndex]  = SetNodeTkn->Handle;

                 } else if (*Status == Oa_LessThen)
                     *Status = Env_Normal;
              }
           }

       } else if (SetDescType == aIndexedSet) {
           /* !!!     it is a IndexedSet by <member class>Num                */
           KeyIndex               = *(wordtype *)KeyValue;

           if (KeyIndex          >= RootStackPtr)
           if (OaPutVchunkStackPtr (&VrootToken,  KeyIndex+1,  McStat))
               OaGetVchunkAddr     (&VrootToken,               McStat,
                                    (addrtype *)&RootNodes);

           if (Normal(*Status)) {
              RootNodes[KeyIndex]    = SetNodeTkn->Handle;
             *Status                 = Set_IsAnIndexedSet;
           }
       } else if (SetDescType == aLocalKeyedSet
              ||  SetDescType == aGlobalKeyedSet) {

           if (SetHeadTkn->DbId <= WenvId)
              AllocSize = 10;
           else
              AllocSize = 100;

           KeyIndex = *(wordtype *)KeyValue / AllocSize;

           if (KeyIndex >= RootStackPtr)
           {
              if (DeBug)
              if (sprintf (Msg, " Index[%2u]  > Set_QuickAccessStack[%2u] \n",
                       KeyIndex, RootStackPtr))
                  TraceMsg (0, Msg);

              if (!RootStackPtr  
              &&  Alloc_RootSlots > KeyIndex)
                  RootStackPtr = Alloc_RootSlots;
              else
                  RootStackPtr = KeyIndex + 1;

              if (OaPutVchunkStackPtr (&VrootToken,  RootStackPtr,  McStat))
                  OaGetVchunkAddr     (&VrootToken,                 McStat,
                                       (addrtype *)&RootNodes);
           } 

           if (Normal(*Status))
           if (RootNodes[KeyIndex]  == NullHandle)
           {
              RootNodes[KeyIndex]    = SetNodeTkn->Handle;

           } else {
              RootTkn.Handle         = RootNodes[KeyIndex];
              if (OaGetObject (&RootTkn,   McStat, (addrtype *)&SetNode))
              if (SetDescType       == aGlobalKeyedSet)
              {
                  gSetNode           = (gsetnodedesc *)SetNode;
                  MemberTkn          = gSetNode->gMemberObject;
              } else 
                  MemberTkn.Handle   = SetNode->lMemberObject;

              if (!TmIsValid     (&MemberTkn,   McStat))
                  *Status  = Oa_GtrThen;

              if (Normal(*Status))
/*
//            if (KernelGetAttr (&MemberTkn, PrimaryKey,
//                                McStat,    ValuePtr)) 
*/
                  OaCompare     (False,     &MemberTkn,           Dummy,
                                 PrimaryKey,
                                 KeyValue,   CmpCode,  McStat);

              if (*Status           == Oa_LessThen) 
              {
                 *Status               = Env_Normal;
                  RootNodes[KeyIndex]  = SetNodeTkn->Handle;
              } else if (*Status    == Oa_GtrThen)
                 *Status = Env_Normal;
              if (*Status == Oa_ObjNoThisAttr)
              {
                  TraceMsg (0,   "\n  Error :: Attribute Not Of Object ::\n");
                  sprintf (Msg,  "  @PutRootNode:: StackPtr  =%4u \n", 
                           RootStackPtr);
                  TraceMsg (0, Msg);
                  sprintf (Msg,  "  MemberTkn [  1:%6u]; PrimaryKey =%4u.\n", 
                           MemberTkn.Handle, PrimaryKey);
                  TraceMsg (0, Msg);
                  sprintf (Msg,  "  RootTkn   [  1:%6u]; KeyIndex   =%4u.\n", 
                           RootTkn.Handle,  KeyIndex);
                  TraceMsg (0, Msg);
                  TraceMsg (0, "\n  Set Desc Object ::\n");
                  OaDumpObject (0, SetDescTkn,  Dump_Status);
                  TraceMsg (0, "\n  RootNodes Array ::\n");
                  for (k = 0; k <= KeyIndex; k++)
                  {
                  sprintf (Msg,  "           [%3u] =%4u.\n", 
                           k, RootNodes[k]);
                  TraceMsg (0, Msg);
                  }
              }
           }

       } else 
          *Status = Set_BadQuickPickSet;


       if (SetBug || DeBug)
       if (sprintf (Msg, "  From SetHead->lFirstLink [%2u:%4u] \n", 
                    SetHeadTkn->DbId, SetHead->lFirstLink))
       if (TraceMsg (0, Msg))
       if (sprintf (Msg, "  Of   SetType  = %2u  Key=%3u   Status = %6d \n", 
                    SetDescType, *(wordtype *)KeyValue, *Status))
       if (TraceMsg (0, Msg))
       if (sprintf (Msg,  "  PUT RootNode[%4u] = %4u \n", 
                    KeyIndex, SetNodeTkn->Handle))
           TraceMsg (0, Msg);
   }

TRACK(TrackBak,"SaPutRootNode\n");
return (STAT);
}
 
boolean    SaGetRootNode  (tokentype     *SetHeadTkn,  lsetheaddesc *SetHead,
                            tokentype     *SetDescTkn,  addrtype     KeyValue,
                            ft F,lt Z,zz  *Status,      tokentype    *RootTkn,
                            tokentype     *VchunkTkn)
{
tokentype        VrootToken;
indextype        RootStackPtr    = 0;
handletype      *RootNodes       = NullPtr;
indextype        KeyIndex        = 0;
char            *KeyPtr          = (char *)KeyValue;
statustype       DumpStatus      = Env_Normal;
tokentype        SetNodeTkn;
lsetnodedesc    *SetNode         = NullPtr;
gsetnodedesc    *gSetNode        = NullPtr;
addrtype         Dummy           = PtrToVoid;
tokentype        SetMemberTkn;
ptr_cmp          CmpCode         = NullPtr;
unsigned char    Value[MAXSTRING];
setdesctype     *SetDesc         = NullPtr;
settype          SetDescType     = aLocalSet;
numtype          PrimaryKey      = 0;
numtype          AllocSize       = 0;

     memset (Value, NULL, sizeof (Value));

     RootTkn->Handle   = SetHead->lFirstLink;

     VrootToken.DbId   = SetHeadTkn->DbId;

     SetNodeTkn.DbId   = SetHeadTkn->DbId;
     SetMemberTkn.DbId = SetHeadTkn->DbId;

     if (OaGetObject    (SetDescTkn,  McStat,  (addrtype *)&SetDesc))
     {
         PrimaryKey  = SetDesc->PrimaryKey;
         SetDescType = SetDesc->SetType;
     }

     if (Normal(*Status))
     if (SetHead->RootNodes)
     {
        VrootToken.Handle      = SetHead->RootNodes;
        VchunkTkn->DbId        = VrootToken.DbId;
        VchunkTkn->Handle      = VrootToken.Handle;
        if (OaGetVchunkAddr     (&VrootToken,               McStat,
                                 (addrtype *)&RootNodes))
        if (OaGetVchunkStackPtr (&VrootToken,               McStat,
                                 &RootStackPtr))
        if (RootStackPtr)
        if (PrimaryKey == Nam_AttrNum)
        {
           KeyIndex = (indextype )KeyPtr[0] - (indextype )64;
           if (KeyIndex
           &&  KeyIndex    <= 58)
           {
              if (KeyIndex > 26) 
                 KeyIndex -= 6;

              if (KeyIndex) 
              {
                 while (KeyIndex && ! RootNodes[KeyIndex-1])
                 {
                    --KeyIndex;
                 }
                 if (KeyIndex)
                    --KeyIndex;
              }
              if (RootNodes[KeyIndex])
                 RootTkn->Handle = RootNodes[KeyIndex];
           }

        } else if (SetDescType == aIndexedSet) {

            /* !!!     it is a IndexedSet by <member class>Num             */
           KeyIndex = *(wordtype *)KeyValue;

           if (KeyIndex >= RootStackPtr)
             *Status            = Set_IndexGtrAllocRoots;
           else
              RootTkn->Handle   = RootNodes[KeyIndex];

           if (DeBug)
              OaDumpVchunk (&VrootToken, Dump_Status);

          *Status               = Set_IsAnIndexedSet;

        } else if (SetDescType == aLocalKeyedSet
               ||  SetDescType == aGlobalKeyedSet) {

           if (SetHeadTkn->DbId <= WenvId)
              AllocSize = 10;
           else
              AllocSize = 100;

           KeyIndex      = *(wordtype *)KeyValue;
           if (KeyIndex >= AllocSize) 
           {
              KeyIndex     = *(wordtype *)KeyValue / AllocSize;
              if (KeyIndex > RootStackPtr)
                  KeyIndex = RootStackPtr;
              while (KeyIndex && ! RootNodes[KeyIndex-1])
                 --KeyIndex;

              if (KeyIndex)
                 --KeyIndex;
           } else 
              KeyIndex   = 0;

           if (KeyIndex >= RootStackPtr)
               KeyIndex  = RootStackPtr - 1;

           if (RootNodes[KeyIndex])
               RootTkn->Handle  = RootNodes[KeyIndex];

        } else
          *Status = Set_BadQuickPickSet;

        LastInIndexedSet.DbId   = RootTkn->DbId;
        LastInIndexedSet.Handle = RootTkn->Handle;

        if (SetBug || DeBug)
        if (sprintf (Msg, "   From SetHead->lFirstLink [%2u:%4u] \n", 
                     SetHeadTkn->DbId, SetHead->lFirstLink))
        if (TraceMsg (0, Msg))
        if (sprintf (Msg, "   Of   SetType  = %2u     Status = %6d \n", 
                     SetDescType, *Status))
        if (TraceMsg (0, Msg))

        if (TraceMsg (0,  "   For  Key      ="))
        if (PrimaryKey != Nam_AttrNum)
            sprintf (Msg, "%2u\n", *(wordtype *)KeyValue);
        else
            sprintf (Msg, " %s  startkey=%u from char %c\n",  
                    (char *)KeyValue, KeyPtr[0], (char )KeyPtr[0]);

        if (SetBug || DeBug)
        if (TraceMsg (0, Msg))
        if (sprintf (Msg, "   ...  GET RootTkn[%4u] SetNode->Handle = %4u\n", 
                     KeyIndex,  RootTkn->Handle))
           TraceMsg (0, Msg);

     } else {

        SetNodeTkn.Handle  =  SetHead->lLastLink;
        if (OaGetObject (&SetNodeTkn,   McStat,   (addrtype *)&SetNode))
        {
           if (SetDescType         == aGlobalKeyedSet)
           {
              gSetNode              = (gsetnodedesc *)SetNode;
              SetMemberTkn          = gSetNode->gMemberObject;
           } else 
              SetMemberTkn.Handle   = SetNode->lMemberObject;

           OaCompare (False,    &SetMemberTkn, Dummy,  PrimaryKey,
                      KeyValue,  CmpCode,      McStat);

           if (Normal(*Status)
           || *Status         == Oa_GtrThen)
           {
              RootTkn->Handle  =  SetHead->lLastLink;
             *Status           = Env_Normal;
           } else
             *Status           = Env_Normal;
        }
     }

TRACK(TrackBak,"SaGetRootNode\n");
return (STAT);
}

boolean SaDeleteNode     (handletype    SetDescHndl, tokentype    *Member,
                          tokentype    *NodeTkn,     ft F,lt Z,zz *Status)
{
unsigned char    Value[MAXNAME];
tokentype       SetDescTkn      = NullToken;
setdesctype    *SetDesc         = NullPtr;
tokentype       HeadTkn         = NullToken;
lsetheaddesctype *SetHead       = NullPtr;
dbheader       *CoreDb          = NullPtr;
numtype         LocalObj        = 0;
objheader      *ObjHdr          = NullPtr;
tokentype       NextTkn         = NullToken;
tokentype       NextMemberTkn   = NullToken;
tokentype       PrevTkn         = NullToken;
addrtype        NodeObj         = NullPtr;
tokentype       VrootTkn        = NullToken;
numtype         NodeObjNum      = 0;
handletype     *RootNodes       = NullPtr;
indextype       StackPtr        = 0;
indextype       Index           = 0;
handletype      QuickPickHndl   = NullHandle;

 SetDescTkn.DbId    = RenvId;
 SetDescTkn.Handle  = SetDescHndl;

 HeadTkn.DbId   = NodeTkn->DbId;
 PrevTkn.DbId   = NodeTkn->DbId;
 NextTkn.DbId   = NodeTkn->DbId;
 VrootTkn.DbId  = NodeTkn->DbId;
 NextMemberTkn.DbId   = Member->DbId;

 if (SetBug || DeBug)
 if (sprintf (Msg, "\n  Delete the Set Node[%2u:%4u]  \n",
               NodeTkn->DbId, NodeTkn->Handle))
 if (TraceMsg (0, Msg))
 if (SetBug || DeBug)
    OaDumpObject  (0,  NodeTkn, McStat);


 if (Normal(*Status))
 if (TmFetchCoreDb    (NodeTkn,        McStat,         &CoreDb))
 if (OaGetObject      (&SetDescTkn,    McStat,         (addrtype *)&SetDesc))

 if (SetDesc->SetHeadType != aIndexedSetHead)
 {
    if (KernelGetAttr    (NodeTkn,                       lOwnerObject_AttrNum,
                          McStat,             (addrtype )&HeadTkn))

    if (OaGetObject      (&HeadTkn, McStat,  (addrtype *)&SetHead))
    if (SetBug || DeBug)
    if (TraceMsg (0, "\n  Set Head of Node to be deleted::\n"))
        OaDumpObject  (0,  &HeadTkn, McStat);

    if (Normal(*Status))
    if (KernelGetAttr    (&HeadTkn,                      RootNodes_AttrNum,
                          McStat,             (addrtype )&VrootTkn))

    if (KernelGetAttr    (NodeTkn,                       lPrevLink_AttrNum,
                          McStat,             (addrtype )&PrevTkn))
    if (KernelGetAttr    (NodeTkn,                       lNextLink_AttrNum,
                          McStat,             (addrtype )&NextTkn))

    if (PrevTkn.Handle)
    {
        KernelPutAttr    (&PrevTkn,                      lNextLink_AttrNum,
                          (addrtype )&NextTkn,           McStat);
        if (SetBug || DeBug)
        if (TraceMsg (0, "  PrevTkn of Node to be deleted::\n"))
            OaDumpObject  (0, &PrevTkn, McStat);
    } else
        SetHead->lFirstLink  = NextTkn.Handle;

    if (Normal(*Status))
    if (NextTkn.Handle)
    {
        KernelPutAttr    (&NextTkn,                       lPrevLink_AttrNum,
                          (addrtype )&PrevTkn,            McStat);
        if (SetBug || DeBug)
        if (TraceMsg (0, "  NextTkn of Node to be deleted::\n"))
            OaDumpObject  (0,  &NextTkn, McStat);
    } else
        SetHead->lLastLink  = PrevTkn.Handle;

    QuickPickHndl = NodeTkn->Handle;

    if (Normal(*Status))
       OaUpdateObject    (&HeadTkn,                       McStat);


 } else {
    QuickPickHndl = Member->Handle;
 }

 if (Normal(*Status))
 if (QuickPickHndl)
 {
   if (OaGetVchunkAddr      (&VrootTkn,     McStat,   (addrtype *)&RootNodes))
   if (OaGetVchunkStackPtr  (&VrootTkn,     McStat,               &StackPtr))
   while (Index < StackPtr)
   {
     if (RootNodes[Index] == QuickPickHndl)
     {
         if (NextTkn.Handle  == NullHandle)
            RootNodes[Index]  = NullHandle;

         else if (SetDesc->SetType == aIndexedSet)
            RootNodes[Index]  = NullHandle;

         else
            SaAdjustRootNode   (SetDesc,    Index,    &HeadTkn,   SetHead,
                                RootNodes,  StackPtr, &NextTkn,   McStat);
         Index                = StackPtr;
     }
     Index++;
   }
 }
 if (SetBug || DeBug)
 if (SetHead != NullPtr)
 if (sprintf (Msg, 
              "  SetHead[%2u:%4u] ->lFirstLink=%6u, ->lLastLink=%6u\n\n",
               HeadTkn.DbId,        HeadTkn.Handle,
               SetHead->lFirstLink, SetHead->lLastLink))
    TraceMsg (0, Msg);

 if (Normal(*Status))
 if (EnvFetchObjNum   (NodeTkn,            McStat,     &NodeObjNum))
 if (MemGetLocalObjNum
                      (CoreDb->LocalObjNumMap,          NodeObjNum,
                       McStat,                         &LocalObj))
 if (OmGetObjHdr      (NodeTkn,                         LocalObj,
                       McStat,                         &ObjHdr))
 if (OaGetObject      (NodeTkn,        McStat,         &NodeObj))
     OmDeleteObject   (NodeTkn,                         ObjHdr,
                       NodeObj,                         McStat);

TRACK(TrackBak,"SaDeleteNode\n");
return(STAT);
}

boolean    SaAdjustRootNode 
                         (setdesctype   *SetDesc,    indextype      PickIndex,
                          tokentype     *SetHeadTkn, lsetheaddesc  *SetHead,
                          handletype    *RootNodes,  indextype   RootStackPtr,
                          tokentype     *NextTkn,    ft F,lt Z,zz  *Status)
{
tokentype        MemberTkn;
lsetnodedesc    *SetNode         = NullPtr;
gsetnodedesc    *gSetNode        = NullPtr;
indextype        KeyIndex        = 0;
char            *KeyPtr          = NullPtr;
unsigned char    Value[MAXSTRING];
addrtype         ValuePtr        =  (addrtype )&Value[0];
settype          SetDescType     = aLocalSet;
numtype          PrimaryKey      = 0;
numtype          AllocSize       = 0;

   memset (Value, NULL, MAXNAME);

   if (SetHead->RootNodes)
   {
       MemberTkn.DbId         = SetHeadTkn->DbId;

       PrimaryKey             = SetDesc->PrimaryKey;
       SetDescType            = SetDesc->SetType;

       if (SetBug)
       if (sprintf (Msg, "   Adjust RootNode[<=%3u] ... \n", RootStackPtr))
           TraceMsg (0, Msg);

       if (Normal(*Status))
       if (OaGetObject      (NextTkn,    McStat,    (addrtype *)&SetNode))
       if (SetDescType       == aGlobalKeyedSet)
       {
          gSetNode           = (gsetnodedesc *)SetNode;
          MemberTkn          = gSetNode->gMemberObject;
       } else 
          MemberTkn.Handle   = SetNode->lMemberObject;

       if (Normal(*Status))
          KernelGetAttr     (&MemberTkn, PrimaryKey,  McStat,    ValuePtr);

       if (Normal(*Status))
       if (PrimaryKey        == Nam_AttrNum)
       {
           KeyPtr           = (char     *)ValuePtr;
           KeyIndex         = (indextype )KeyPtr[0] - (indextype )64;
           if (KeyIndex
           &&  KeyIndex    <= 58)
           {
              if (KeyIndex  > 26)
                  KeyIndex -= 6;
              if (KeyIndex == PickIndex)
                 RootNodes[PickIndex]  = NextTkn->Handle;
              else 
                 RootNodes[PickIndex]  = NullHandle;
           }

       } else if (SetDesc->SetType == aIndexedSet) {
           /* !!!     it is a IndexedSet by <member class>Num                */

           RootNodes[PickIndex] = NullHandle;


       } else if (SetDesc->SetType == aLocalKeyedSet
              ||  SetDesc->SetType == aGlobalKeyedSet) {

           if (SetHeadTkn->DbId <= WenvId)
              AllocSize = 10;
           else
              AllocSize = 100;

           KeyIndex = *(wordtype *)ValuePtr / AllocSize;
           if (KeyIndex < RootStackPtr)
           if (KeyIndex == PickIndex)
              RootNodes[PickIndex]  = NextTkn->Handle;
           else 
              RootNodes[PickIndex]  = NullHandle;

       } else 
          *Status = Set_BadQuickPickSet;


       if (SetBug || DeBug)
       if (sprintf (Msg, "  From SetHead->lFirstLink [%2u:%4u] \n", 
                    SetHeadTkn->DbId, SetHead->lFirstLink))
       if (TraceMsg (0, Msg))
       if (sprintf (Msg, "  Of   SetType  = %2u  Key=%3u   Status = %6d \n", 
                    SetDesc->SetType, *(wordtype *)ValuePtr, *Status))
       if (TraceMsg (0, Msg))
       if (sprintf (Msg,  "  Adjust RootNode[%4u] = %4u \n", 
                    PickIndex, NextTkn->Handle))
           TraceMsg (0, Msg);
   }

TRACK(TrackBak,"SaAdjustRootNode\n");
return (STAT);
}


boolean SaDeleteSetHead (handletype    SetDescHndl, tokentype    *HeadTkn,
                          ft F,lt Z,zz *Status)
{
tokentype       SetDescTkn;
setdesctype    *SetDesc         = NullPtr;
dbheader       *CoreDb          = NullPtr;
numtype         LocalObj        = 0;
objheader      *ObjHdr          = NullPtr;
tokentype       NextNodeTkn;
tokentype       CurrNodeTkn;
tokentype       MemberTkn;
addrtype        CurrNode        = NullPtr;
addrtype        HeadObj         = NullPtr;
numtype         MemberFieldAttrNum  = 0;
tokentype       VrootTkn;
setheadtype     HeadType;
numtype         SetHeadObjNum   = 0;

 SetDescTkn.DbId    = RenvId;
 SetDescTkn.Handle  = SetDescHndl;

 CurrNodeTkn.DbId   = HeadTkn->DbId;
 NextNodeTkn.DbId   = HeadTkn->DbId;
 MemberTkn.DbId     = HeadTkn->DbId;
 VrootTkn.DbId      = HeadTkn->DbId;

 if (!HeadObjNum[2])
    SetInitSetHeads (McStat);

 if (SetBug || DeBug)
 if (sprintf (Msg, "\n   Delete the Set [%2u:%4u]  \n",
               HeadTkn->DbId, HeadTkn->Handle))
    TraceMsg (0, Msg);

 if (OaGetObject      (&SetDescTkn,    McStat,         (addrtype *)&SetDesc))
 if (KernelGetAttr    (&SetDescTkn,                     MemberAttrNum_AttrNum,
                       McStat,              (addrtype )&MemberFieldAttrNum))

 if (KernelGetAttr    (&SetDescTkn,                     SetHeadType_AttrNum,
                       McStat,              (addrtype )&HeadType))
 {
     SetHeadObjNum  = HeadObjNum[HeadType];
     sprintf (Msg, "   setheadtype:= %-21s HeadObjNum[%2u]= %2u\n",
              HeadNam[HeadType], HeadType, HeadObjNum[HeadType]);
     TraceMsg (0, Msg);
 }

 if (HeadType == aLKeyedSetHead
 ||  HeadType == aLSetHead)
 {
    if (TmFetchCoreDb    (HeadTkn,        McStat,       &CoreDb))
    if (MemGetLocalObjNum
                         (CoreDb->LocalObjNumMap,        lSetNode_ObjNum,
                          McStat,                       &LocalObj))
    if (OmGetObjHdr      (HeadTkn,                       LocalObj,
                          McStat,                       &ObjHdr))

    if (KernelGetAttr    (HeadTkn,                       lLastLink_AttrNum,
                          McStat,            (addrtype )&(CurrNodeTkn.Handle)))
    while (Normal(*Status)
    &&     CurrNodeTkn.Handle)
    {
       if (KernelGetAttr (&CurrNodeTkn,                  lPrevLink_AttrNum,
                          McStat,           (addrtype )&(NextNodeTkn.Handle)))

       if (SetDesc->SetType  == aGlobalKeyedSet)
           KernelGetAttr (&CurrNodeTkn,                  gMemberObject_AttrNum,
                          McStat,           (addrtype )&MemberTkn);
       else
           KernelGetAttr (&CurrNodeTkn,                  lMemberObject_AttrNum,
                          McStat,           (addrtype )&MemberTkn);

       if (Normal(*Status))
       if (KernelPutAttr (&MemberTkn,                    MemberFieldAttrNum,
                          (addrtype )&NullToken,         McStat))

       if (OaGetObject   (&CurrNodeTkn,      McStat,    &CurrNode))
       if (OmDeleteObject(&CurrNodeTkn,                  ObjHdr,
                          CurrNode,                      McStat))     
           CurrNodeTkn.Handle  = NextNodeTkn.Handle;
    }

    if (Normal(*Status))
    if (KernelGetAttr    (HeadTkn,                       RootNodes_AttrNum,
                          McStat,            (addrtype )&VrootTkn.Handle))
    if (OaDeleteVchunk   (&VrootTkn,                     McStat))
    if (MemGetLocalObjNum
                         (CoreDb->LocalObjNumMap,        SetHeadObjNum,
                          McStat,                       &LocalObj))
    if (OmGetObjHdr      (HeadTkn,                       LocalObj,
                          McStat,                       &ObjHdr))
    if (OaGetObject      (HeadTkn,        McStat,       &HeadObj))
        OmDeleteObject   (HeadTkn,                       ObjHdr,
                         HeadObj,                       McStat);
 } else
   *Status = Err_NotImplemented;

TRACK(TrackBak,"SaDeleteSetHead\n");
return(STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF SA.C                                    !
   -----------------------------------------------------------------------+*/
