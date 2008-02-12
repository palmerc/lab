/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env - Sm.c */
#define SM_C
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "voa.h"
#include "oa0.h"
#include "env1.h"
#include "oa1.h"
#include "sa.h"
#include "sm.h"
#include "tree01.h"
 
/*+-----------------------------------------------------------------------+
~ENV                             SM                                       !
  +-----------------------------------------------------------------------+*/
/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        lKeyedSetHead_ObjNum   (objnumtype )66

#define        lSetNode_ObjNum        (objnumtype )78


#define        ObjNum_AttrNum         (numtype )5

#define        Nam_AttrNum            (numtype )19

#define        SetHeadType_AttrNum    (numtype )44

#define        OwnerAttrNum_AttrNum   (numtype )61

#define        PrimaryKey_AttrNum     (numtype )64

#define        lOwnerObject_AttrNum   (numtype )88
#define        lMemberObject_AttrNum  (numtype )89

#define        lFirstLink_AttrNum     (numtype )91

#define        lLastLink_AttrNum      (numtype )93

#define        lPrevLink_AttrNum      (numtype )95

#define        lNextLink_AttrNum      (numtype )97

#define        RenvDescs_Hndl         (handletype )694

#endif

/*+-----------------------------------------------------------------------+*
~E                            EXTERMS                                     !
 *+-----------------------------------------------------------------------+*/
extern boolean KernelGetAttr  (tokentype *Token,       numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype   Value);

extern numtype lKeyedSetNode_ObjNum;



/*+-----------------------------------------------------------------------+
~SM                        SET MANAGER                                    !
  +-----------------------------------------------------------------------+*/

boolean SetInitSet        (handletype    SetDescHndl,tokentype    *OwnerTkn,
                           numtype       AllocSize,  numtype       Order,
                           ft F,lt Z,zz *Status)
{
tokentype        SetDescTkn      = NullToken;
setdesctype     *SetDesc         = NullPtr;
settype          SetDescType     = aLocalSet;
objtype         *Owner           = NullPtr;
lsetheaddesc    *SetHead         = NullPtr;
tokentype        SetHeadTkn      = NullToken;
addrtype         Dummy           = NullPtr;
numtype          AllocQty        = 0;
tokentype        VrootToken      = NullToken;
 
  SetHeadTkn.DbId   = OwnerTkn->DbId;
  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (OaGetObject   (OwnerTkn,            McStat,   (addrtype *)&Owner)) 
  if (OaGetObject   (&SetDescTkn,         McStat,   (addrtype *)&SetDesc)) 
  if (OaGet         (False,               OwnerTkn, (addrtype  )Dummy,
                     SetDesc->OwnerAttrNum,
                     McStat,              (addrtype )&(SetHeadTkn.Handle)))
     SetDescType    = SetDesc->SetType;

  if (Normal(*Status))
  if (SetHeadTkn.Handle == NullHandle)
  {
     if (OaCreateObject (OwnerTkn,        lKeyedSetHead_ObjNum,
                         McStat,         &SetHeadTkn))
     if (OaGetObject    (&SetHeadTkn,     McStat,     (addrtype *)&SetHead))
     {
        SetHead->BuildDesc     = SetDescHndl;
        SetHead->lOwnerObject  = OwnerTkn->Handle;
        SetHead->lFirstLink    = Null_Handle;
        SetHead->lLastLink     = Null_Handle;
        OaPut (False, OwnerTkn, (addrtype )Owner, SetDesc->OwnerAttrNum,
               (addrtype )&(SetHeadTkn.Handle),   McStat);
     }

  } else  
    *Status = Set_BadDescName;

  if (Normal(*Status))
  {
     if (SetBug || DeBug)
     if (sprintf (Msg, "\n  Set::InitSet  %14s[%2u:%4u];  Owner[%2u:%4u]\n",
                  SetDesc->Nam,      SetDescTkn.DbId,  SetDescTkn.Handle,
                  OwnerTkn->DbId,    OwnerTkn->Handle))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "     Set Head    [%2u:%4u]; Alloc Members=%4u\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle, AllocSize))
         TraceMsg (0, Msg);
  }

  if (Normal(*Status)) 
  if (SetDescType    == aIndexedSet
  ||  SetDescType    == aLocalKeyedSet
  ||  SetDescType    == aGlobalKeyedSet)
  {
     if (SetDesc->PrimaryKey   == Nam_AttrNum) 
        AllocQty                = 53;
     else if (SetDescType == aIndexedSet)
     {
        SetHead->lFirstLink     = 1;
        SetHead->lLastLink      = NullHandle;
        AllocQty                = Alloc_DbObjs;
     } else
        AllocQty                = AllocSize / Order;

     if (AllocQty)
     if (OaCreateVchunk  (&SetHeadTkn,     sizeof (handletype),
                          AllocQty,        Extend_Fields,
                          McStat,         &VrootToken))
        SetHead->RootNodes      = VrootToken.Handle;
  }

TRACK(TrackBak,"SetInitSet\n");
return (STAT);
}
 

boolean    SetAddInto    (handletype SetDescHndl,   tokentype    *OwnerTkn,
                          tokentype *MemberTkn,     ft F,lt Z,zz *Status)
{
tokentype        NodeTkn  = NullToken;
tokentype        SetDescTkn;
setdesctype     *SetDesc         = NullPtr;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;
  if (OaGetObject      (&SetDescTkn,  McStat,   (addrtype *)&SetDesc)) 

  if (SetDesc->SetType  == aLocalTableSet
  ||  SetDesc->SetType  == aGlobalTableSet)
     *Status   = Err_NotImplemented;

  else if (SetDesc->SetType  == aLocalTreeSet
       ||  SetDesc->SetType  == aGlobalTreeSet)
     SpclAddIntoTree   (SetDescHndl,  OwnerTkn,  MemberTkn,
                        McStat,                 &NodeTkn);
  else
     SaAddInto           (SetDescHndl,  OwnerTkn,  MemberTkn,   
                          McStat,                 &NodeTkn);

TRACK(TrackBak,"SetAddInto\n");
return (STAT);
}


boolean    SetFindIn     (handletype     SetDescHndl,   tokentype  *OwnerTkn,
                          addrtype       KeyValue,
                          ft F,lt Z,zz  *Status,        tokentype  *MemberTkn)
{
tokentype        NodeTkn         = NullToken;
tokentype        SetDescTkn      = NullToken;
setdesctype     *SetDesc         = NullPtr;

  LastInIndexedSet.DbId    = OwnerTkn->DbId;
  LastInIndexedSet.Handle  = 0;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;
  if (OaGetObject      (&SetDescTkn,  McStat,   (addrtype *)&SetDesc)) 
  if (SetDesc->SetType  == aLocalTableSet
  ||  SetDesc->SetType  == aGlobalTableSet)
  {
     *Status   = Err_NotImplemented;

  } else if (SetDesc->SetType  == aLocalTreeSet
         ||  SetDesc->SetType  == aGlobalTreeSet) {
     SpclFindInTree    (SetDescHndl,  OwnerTkn,  KeyValue,
                        McStat,      &NodeTkn,   MemberTkn);
     LastInIndexedSet = NodeTkn;

  } 
  else
     SaFindIn          (SetDescHndl,  OwnerTkn,  KeyValue,
                        McStat,      &NodeTkn,   MemberTkn);

TRACK(TrackBak,"SetFindIn\n");
return (STAT);
}


boolean SetIterateOn     (handletype    SetDescHndl, tokentype    *OwnerTkn,
                          iteratetype   Method,      ft F,lt Z,zz *Status)
{
tokentype     CurrentTkn;
tokentype     CurrentNode;
tokentype     NextTkn;
tokentype     NextNode;
indextype     ItNum        = 0;
int           TempDeBug    = DeBug;

 CurrentTkn.DbId    = OwnerTkn->DbId;
 CurrentNode.DbId   = OwnerTkn->DbId;
 NextTkn.DbId       = OwnerTkn->DbId;
 NextNode.DbId      = OwnerTkn->DbId;
 DeBug = TempDeBug;

 *Status = Env_Normal;
 if (SetBug || DeBug)
 if (sprintf (Msg, "\n   IterateOn the Set by Owner[%2u:%4u]  \n",
               OwnerTkn->DbId, OwnerTkn->Handle))
    TraceMsg (0, Msg);

 if (OwnerTkn->Handle)
 if (SetFirstOf     (SetDescHndl,     OwnerTkn,
                     McStat,         &CurrentNode,   &CurrentTkn))
 {
    if (Normal(*Status))
    do {   
       ++ItNum;
       if ((*Method)  (&CurrentTkn,   ItNum,          McStat))
       if (SetNextOf  (SetDescHndl,  &CurrentNode,  
                       McStat,       &NextNode,      &NextTkn))
       {
           CurrentNode.Handle = NextNode.Handle;
           CurrentTkn.Handle  = NextTkn.Handle;
       }
    } while (Normal(*Status));
 }
 if (*Status == Set_EndOfSet  || *Status == Set_EmptySet)
     *Status = Env_Normal;
 
TRACK(TrackBak,"SetIterateOn\n");
return(STAT);
}
 

boolean SetDeleteFrom    (handletype    SetDescHndl, tokentype    *Member,
                          ft F,lt Z,zz *Status)
{
tokentype       SetDescTkn;
tokentype       NodeTkn;
setdesctype    *SetDesc     = NullPtr;

 SetDescTkn.DbId    = RenvId;
 SetDescTkn.Handle  = SetDescHndl;

 if (ClassBug)
    OaDumpObject  (0,  &SetDescTkn,  McStat);

 if (OaGetObject      (&SetDescTkn,    McStat,    (addrtype *)&SetDesc))
 if (SetDesc->SetType == aLocalTableSet
 ||  SetDesc->SetType == aGlobalTableSet)
 {
    *Status = Err_NotImplemented ;
 } else if (SetDesc->SetType  == aLocalTreeSet
        ||  SetDesc->SetType  == aGlobalTreeSet)  {

     SpclDeleteFromTree (SetDescHndl,  Member,    McStat);

 } else {
    NodeTkn.DbId   = Member->DbId;
    if (KernelGetAttr    (Member,                   SetDesc->MemberAttrNum,
                          McStat,       (addrtype )&NodeTkn.Handle))
       SaDeleteNode     (SetDescHndl,  Member,    &NodeTkn,         McStat);
 }

TRACK(TrackBak,"SetDeleteFrom\n");
return(STAT);
}


boolean SetDeleteSet     (handletype    SetDescHndl, tokentype    *Owner,
                          ft F,lt Z,zz *Status)
{
   *Status   = Err_NotImplemented;

TRACK(TrackBak,"SetDeleteSet\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+*/
/*                           GENERAL SET METHODS                           */
/*+-----------------------------------------------------------------------+*/
boolean    SetFirstOf    (handletype    SetDescHndl,   tokentype  *OwnerTkn,
                          ft F,lt Z,zz *Status,        tokentype  *NodeTkn,
                          tokentype    *MemberTkn)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
lsetheaddesc    *SetHead     = NullPtr;
tokentype        SetHeadTkn;
tokentype        VchunkTkn;
indextype        One         = 1;
indextype        Zero        = 0;
addrtype         Dummy       = NullPtr;

  NodeTkn->DbId     = OwnerTkn->DbId;
  NodeTkn->Handle   = NullHandle;

  MemberTkn->DbId   = OwnerTkn->DbId;
  MemberTkn->Handle = NullHandle;

  SetHeadTkn.DbId   = OwnerTkn->DbId;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (!OwnerTkn->Handle)
  {
      *Status = Set_EmptySet;
      if (SetBug || DeBug)
        TraceMsg (0, "    ... !!  NULL Owner for this  Set\n");
  }

  if (Normal(*Status))
  if (OaGetObject (&SetDescTkn,   McStat,   (addrtype *)&SetDesc)) 
  if (OaGet       (False,         OwnerTkn, Dummy, 
                   SetDesc->OwnerAttrNum,
                   McStat,        (addrtype )&(SetHeadTkn.Handle)))
  if (SetHeadTkn.Handle == NullHandle)
     *Status = Set_EmptySet;
 
  if (Normal(*Status))
      OaGetObject (&SetHeadTkn,   McStat,   (addrtype *)&SetHead);

  if ((!Normal(*Status) && SetBug)
  ||   DeBug)
  {
     if (*Status != Set_EmptySet)
     if (sprintf (Msg, "\n  SetFirstOf  %14s[%2u:%4u]  SetOwner[%2u:%4u]\n", 
              SetDesc->Nam,   SetDescTkn.DbId,  SetDescTkn.Handle,  
              OwnerTkn->DbId, OwnerTkn->Handle))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "     SetHead            [%2u:%4u] @ %x\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle, SetHead))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "            ->Firstlink [%2u:%4u] \n", 
                  SetHeadTkn.DbId, SetHead->lFirstLink))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "            ->Lastlink  [%2u:%4u] \n",
                  SetHeadTkn.DbId, SetHead->lLastLink))
        TraceMsg (0, Msg);
  }
 
  if (Normal(*Status))
  if (SetHead->lFirstLink == NullHandle)
     *Status = Set_EmptySet;
  
  if (*Status == Set_EmptySet)
  {
      MemberTkn->Handle   = NullHandle;
      if (DeBug)
         TraceMsg (0, "    ... Empty Set\n");
  } else if (Normal(*Status)) {
      if (SetDesc->SetType == aIndexedSet)
      {
         CurrVchunkIndex    = 0;
         if (SaGetRootNode (&SetHeadTkn,       SetHead,   &SetDescTkn, 
                             (addrtype )&Zero,  McStat,
                              MemberTkn,       &VchunkTkn))
         {
             NodeTkn->Handle     = VchunkTkn.Handle;
             NodeTkn->DbId       = VchunkTkn.DbId;
            *Status              = Env_Normal;
         }
         if (SetBug || DeBug)
         if (sprintf (Msg, "     Get IndexedSet[%2u]  Member[%2u:%4u]\n",
                      One, MemberTkn->DbId,   MemberTkn->Handle))
            TraceMsg (0, Msg);
            
      } else {
         NodeTkn->DbId   = SetHeadTkn.DbId;
         NodeTkn->Handle = SetHead->lFirstLink;
         if (OaGet     (False,              NodeTkn, Dummy,
                        lMemberObject_AttrNum,
                        McStat,             (addrtype )&(MemberTkn->Handle)))
            MemberTkn->DbId = NodeTkn->DbId;
         if (Normal(*Status))
         if (SetBug || DeBug)
         if (sprintf (Msg, "     Node[%2u:%4u]  Member[%2u,%4u]\n",
                      NodeTkn->DbId,     NodeTkn->Handle,
                      MemberTkn->DbId,   MemberTkn->Handle))
            TraceMsg (0, Msg);
      }
  }
 
TRACK(TrackBak,"SetFirstOf\n");
return (STAT);
}
 

boolean    SetNextOf    (handletype    SetDescHndl,   tokentype  *CurrentNode,
                          ft F,lt Z,zz *Status,        tokentype  *NextNode,
                          tokentype    *NextMember)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
addrtype         Dummy       = NullPtr;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;
  if (CurrentNode->Handle == NullHandle)
     *Status = Set_EndOfSet;

  if (Normal(*Status))
  if (OaGetObject (&SetDescTkn,   McStat,   (addrtype *)&SetDesc)) 
  if (SetDesc->SetType == aIndexedSet)
  {
     SetGetSwi = True;
     OaGetUnit (CurrentNode, ++CurrVchunkIndex,
                McStat,       (addrtype )&(NextMember->Handle));
     SetGetSwi = False;
     NextNode->DbId     = CurrentNode->DbId;
     NextNode->Handle   = CurrentNode->Handle;
  } else {
     OaGet       (False,        CurrentNode, (addrtype )Dummy, 
                  lNextLink_AttrNum,
                  McStat,       (addrtype )&(NextNode->Handle));
     NextNode->DbId = CurrentNode->DbId;
  }

  if (Normal(*Status))
  if (NextNode->Handle == NullHandle)
     *Status = Set_EndOfSet;
   
  if (Normal(*Status))
  if (SetDesc->SetType != aIndexedSet)
  if (OaGet  (False,              NextNode, Dummy,
              lMemberObject_AttrNum,
              McStat,             (addrtype )&(NextMember->Handle)))
     NextMember->DbId = NextNode->DbId;

  if (*Status == Set_EndOfSet)
  {
     NextNode->DbId     = CurrentNode->DbId;
     NextNode->Handle   = NullHandle;
     NextMember->DbId   = CurrentNode->DbId;
     NextMember->Handle = NullHandle;
  }
 
  if (SetBug ||  DeBug)
  if (sprintf (Msg, "\n    SetNextOf... Current   [%2u,%4u]  Next [%2u,%4u]\n",
               CurrentNode->DbId,     CurrentNode->Handle,
               NextNode->DbId,        NextNode->Handle))
  if (TraceMsg (0, Msg))
  if (sprintf (Msg, "                 Member [%2u,%4u]\n",
               NextMember->DbId,   NextMember->Handle))
    TraceMsg (0, Msg);
 
TRACK(TrackBak,"SetNextOf\n");
return (STAT);
}


 

/*+-----------------------------------------------------------------------+
~M                         END OF SM.C                                    !
  +-----------------------------------------------------------------------+*/
