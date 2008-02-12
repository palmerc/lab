/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env (Set)- Tree01.c */        
#define  TREE01
#define  SPCL1

#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "oa0.h"
#include "env1.h"
#include "oa1.h"
#include "vchunk.h"
#include "grp.h"
#include "tree00.h"
#include "tree01.h"

/*+-----------------------------------------------------------------------+
~SPCL1                    SPECIAL SETS                                    !
  +-----------------------------------------------------------------------+*/
extern boolean KernelGetAttr  
                         (tokentype    *Token,      numtype      EnvAttrNum,
                          ft F,lt Z,zz *Status,     addrtype     Value);



/* .......................................................................
                   ....     SPCL TREE      ....
   .......................................................................*/

extern boolean OaDumpObject
                          (int           TraceSwi,  tokentype    *Token,
                           ft F,lt Z,zz *Status);

/* .......................................................................
                   ....     SPCL TREE      ....
   .......................................................................*/
       boolean Env_GetClassId 
                         (char         *ClassName, 
                          ft F,lt Z,zz *Status,    numtype      *ClassId);

#define        Env_GetMemberId  Env_GetAttrId
       boolean Env_GetAttrId 
                         (char         *AttrName,  
                          ft F,lt Z,zz *Status,    numtype      *AttrId);

       boolean Env_GetAttrInfo
                         (numtype       AttrId,
                          ft F,lt Z,zz *Status,    attrinfotype *AttrInfo);
                                           
       boolean Env_InitClassMap 
                         (numtype       ClassId,   size_t        ObjSize,
                          numtype       AplId,     ft F,lt Z,zz *Status);

       boolean Env_InvokeMap
                         (numtype       AplId,     ft F,lt Z,zz *Status);

       boolean Object_Create
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean Object_PutImage
                         (tokentype    *Token,     ft F,lt Z,zz *Status); 

       boolean Object_CommitImage
                         (tokentype    *Token,     boolean       Release,
                          ft F,lt Z,zz *Status);

       boolean Object_GetImage
                         (tokentype    *Token,
                          ft F,lt Z,zz *Status,    addrtype     *Image);

       void    Object_Dump
                         (tokentype    *Token);

       boolean Attr_ArrayCreate
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,    addrtype     *Array);

/*+-----------------------------------------------------------------------+
~SET                             SPCL                                     !
  +-----------------------------------------------------------------------+*/

boolean    SpclAddIntoTree
                      (handletype    SetDescHndl,
                       tokentype    *OwnerTkn,     tokentype     *MemberTkn,
                       ft F,lt Z,zz *Status,       tokentype     *NodeTkn)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc         = NullPtr;
objtype         *Owner           = NullPtr;
objtype         *Member          = NullPtr;
ltreesethead    *SetHead         = NullPtr;
tokentype        SetHeadTkn;
sizetype         KeySize         = 0;
typetype         KeyType         = aInteger;
addrtype         Dummy           = PtrToVoid;
unsigned char    Value[MAXSTRING];
addrtype         ValuePtr        =  (addrtype )&Value[0];
statustype       DumpStatus      = Env_Normal;
attrobjtype      AttrDesc;
unsigned char    Key2aryValue[64];
addrtype         Key2aryVal      =  (addrtype )&Key2aryValue[0];

  Key2aryValue[0]   = '\0';
  Tree_OpType       = aTreeAddInto;
  AddInto_MemberTkn = *MemberTkn;

  memset (Value, NULL, MAXNAME);

  SetHeadTkn.DbId   = OwnerTkn->DbId;
  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;
  
  NodeTkn->DbId     = OwnerTkn->DbId;
  NodeTkn->Handle   = 0;


  if (TempKeys_Attr == 0)
  if (Env_GetMemberId  ("TempHandles",   McStat,     &TempHandles_Attr))
  if (Env_GetMemberId  ("TempKeys",      McStat,     &TempKeys_Attr))
  if (Env_GetMemberId  ("TempLeafs",     McStat,     &TempLeafs_Attr))
  if (Env_GetClassId   ("bTreeSetHead",  McStat,     &bTreeSetHead_Class))
  if (Env_InitClassMap (bTreeSetHead_Class,           sizeof(ltreesethead),   
                        C_Apl,                        McStat))
      Env_InvokeMap    (C_Apl,                        McStat);

  if (SetBug || DeBug)
  if (sprintf (Msg, "\n   SpclAddIntoTree:: SetTkn[%2u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle))
  if (TraceMsg (0, Msg))
  if (sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (OwnerTkn,            McStat,   (addrtype *)&Owner)) 
  if (OaGetObject   (MemberTkn,           McStat,   (addrtype *)&Member)) 
  if (OaGetObject   (&SetDescTkn,         McStat,   (addrtype *)&SetDesc)) 

  if (!OaGet        (False,               OwnerTkn, (addrtype  )Dummy,
                     SetDesc->OwnerAttrNum,
                     McStat,              (addrtype )&(SetHeadTkn.Handle)))
  {
      sprintf (Msg,"***ERROR***\n      OwnerAttr[%4u] Not of Owner Object\n",
                SetDesc->OwnerAttrNum);
      TraceMsg (0, Msg);
      sprintf (Msg, " AT AddIntoSet:: <%-14s> SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
      TraceMsg (0, Msg);
      sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle);
      TraceMsg (0, Msg);
      TraceMsg (0, "   For SetDesc...\n");

      OaDumpObject (0, &SetDescTkn, Dump_Status);

     *Status = Set_SetNotOfObjCestrs;
  }

  if (Normal(*Status))
  if (Env_GetAttrInfo  (SetDesc->PrimaryKey,   McStat,   &AttrDesc))
  {
     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
  }

  if (Normal(*Status))
  if (SetDesc->Key2ary)
  if (!OaGet        (False,               MemberTkn, (addrtype  )Dummy,
                     SetDesc->Key2ary,
                     McStat,                         (addrtype  )Key2aryVal))
  {
      sprintf (Msg,"***ERROR***   2ary KeyAttr[%4u] Not of Member Object\n",
                SetDesc->Key2ary);
      TraceMsg (0, Msg);
      sprintf (Msg, " AT AddIntoSet:: <%-14s> SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
      TraceMsg (0, Msg);
      sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle);
      TraceMsg (0, Msg);
      TraceMsg (0, "   For SetDesc...\n");

      OaDumpObject (0, &SetDescTkn, Dump_Status);

     *Status = Set_SetNotOfObjCestrs;

  } else {
     FindIn_2aryKey    = Key2aryVal;

  }

  if (Normal(*Status))
  if (SetHeadTkn.Handle == NullHandle)
  if (Tree_Create    (SetDescHndl,     OwnerTkn,  7,  5000,   KeySize,
                      KeyType,         McStat,  &SetHeadTkn))
  {
      OaPut          (False,           OwnerTkn,      (addrtype )Owner, 
                      SetDesc->OwnerAttrNum,     (addrtype )&SetHeadTkn,
                      McStat);
  }

  if (Normal(*Status))
  if (KernelGetAttr     (MemberTkn, SetDesc->PrimaryKey, McStat, ValuePtr))
  {
   if (SetBug || DeBug || DbmBug)
   {
     if (sprintf (Msg,
                  "\n   SpclAddIntoTree %14s[%2u:%4u];  Owner[%2u:%4u]\n",
                  SetDesc->Nam,      SetDescTkn.DbId,  SetDescTkn.Handle,
                  OwnerTkn->DbId,    OwnerTkn->Handle))
     if (TraceMsg (0, Msg))

     if (KeySize > sizeof(indextype))
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%12s>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, Value);
     else
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%4u>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, *(numtype *)Value);
     TraceMsg (0, Msg);

     if (sprintf (Msg, "              SetHead[%2u:%4u]\n\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle))
        TraceMsg (0, Msg);
   }
  } else {

     sprintf (Msg,"***ERROR*** Primary Key Attr[%4u] Not of Member Class\n",
              SetDesc->PrimaryKey);
     TraceMsg (0, Msg);
     sprintf (Msg, " AT AddIntoSet:: %14s SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
     TraceMsg (0, Msg);
     sprintf (Msg,"                 Owner [%2u:%4u]   Member[%2u:%4u]\n", 
              OwnerTkn->DbId,   OwnerTkn->Handle,
              MemberTkn->DbId,  MemberTkn->Handle);
     TraceMsg (0, Msg);
    *Status = Set_KeyNotOfObjCestrs;
  }

  if (Normal(*Status))
  if (OaPut          (False,          MemberTkn,       (addrtype  )Member, 
                      SetDesc->MemberAttrNum,
                      (addrtype )&(SetHeadTkn.Handle),             McStat))
  /* The Image is required as the AddInto scheme uses the temporary arrays */
  if (Object_GetImage(&SetHeadTkn,    McStat,         (addrtype *)&SetHead))
     Tree_AddInto   (SetHead,        ValuePtr, MemberTkn->Handle, McStat);

  if (SetBug)
  {
     if (KeySize <= 4)
         sprintf  (Msg, "      SpclAddIntoTree ::  <%6u>;\n",
                  *(numtype *)ValuePtr);
     else 
         sprintf  (Msg, "      SpclAddIntoTree ::  <%-12s>;\n",
                   Value);
     if (TraceMsg (0,   Msg))
     if (SetHead)
     if (sprintf  (Msg, "         Nodes =%4u;   Keys     = %4u;\n\n",
                   SetHead->NodeCount, SetHead->KeyCount))
         TraceMsg (0,   Msg);
  }
  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"SpclAddInToTree\n");
return (STAT);
}


/* ------------------------------------------------------------------------ */
boolean    SpclFindIn2aryTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       addrtype       KeyValue,    addrtype       Key2aryVal,
                       ft F,lt Z,zz  *Status,
                       tokentype     *NodeTkn,     tokentype     *MemberTkn)
{

  Tree_OpType       = aTreeFindIn;
  FindIn_2aryKey    = Key2aryVal;

  SpclFindInTree     (SetDescHndl, OwnerTkn, KeyValue,
                      McStat,      NodeTkn,  MemberTkn); 

TRACK(TrackBak,"SpclFindIn2aryTree\n");
return (STAT);
}


boolean    SpclFindInTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,
                       tokentype     *NodeTkn,     tokentype     *MemberTkn)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
objtype         *Owner       = NullPtr;
ltreesethead    *SetHead     = NullPtr;
tokentype        SetHeadTkn  = NullToken;
numtype          PrimaryKey  = 0;
bytessize        KeySize     = 0;
addrtype         Dummy       = NullPtr;
lbtreenode      *LeafNode    = NullPtr;
indextype        KeyPos      = 0;

  Tree_OpType       = aTreeFindIn;
  FindIn_2aryKey    = NullPtr;

  if (TempKeys_Attr == 0)
  if (Env_GetMemberId  ("TempHandles",   McStat,     &TempHandles_Attr))
  if (Env_GetMemberId  ("TempKeys",      McStat,     &TempKeys_Attr))
  if (Env_GetMemberId  ("TempLeafs",     McStat,     &TempLeafs_Attr))
  if (Env_GetClassId   ("bTreeSetHead",  McStat,     &bTreeSetHead_Class))
  if (Env_InitClassMap (bTreeSetHead_Class,           sizeof(ltreesethead),   
                        C_Apl,                        McStat))
      Env_InvokeMap    (C_Apl,                        McStat);

  SetHeadTkn.DbId   = OwnerTkn->DbId;
  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  MemberTkn->DbId   = OwnerTkn->DbId;
  NodeTkn->DbId     = OwnerTkn->DbId;
  NodeTkn->Handle   = 0;

  if (SetBug || DeBug)
  if (sprintf (Msg, "\n   SpclFindInTree:: SetTkn[%2u:%4u]... OwnerTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->DbId,   OwnerTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (&SetDescTkn,  McStat,     (addrtype *)&SetDesc)) 
  if (OaGetObject   (OwnerTkn,     McStat,     (addrtype *)&Owner)) 
  {
     PrimaryKey     = SetDesc->PrimaryKey;
  }

  if (Normal(*Status))
  if (DeBug || SetBug)
  if (sprintf  (Msg, "    SetDesc @%8x ->PrimaryKey @%8x; KeyAttrNum=%3u\n",
                SetDesc,  &SetDesc->PrimaryKey, PrimaryKey))
      TraceMsg (0,   Msg);

  if (Normal(*Status))
  if (OaGetAttrSize (&RenvTkn,                  PrimaryKey, 
                      McStat,       (addrtype )&KeySize))

  if (OaGet         (False,                     OwnerTkn, 
                     (addrtype )Dummy,          SetDesc->OwnerAttrNum,
                     McStat,        (addrtype )&SetHeadTkn.Handle))
  if (SetHeadTkn.Handle == NullHandle)
  {
      if (DeBug || SetBug)
        TraceMsg (0, "    ... Empty Set.\n");

     *Status = Set_EmptySet;

  }

  if (Normal(*Status))
  if (OaGet          (False,                                 OwnerTkn, 
                      (addrtype )Dummy,             SetDesc->OwnerAttrNum,
                      McStat,         (addrtype )&SetHeadTkn.Handle))
  if (Object_GetImage(&SetHeadTkn,     McStat,  (addrtype *)&SetHead))
  if (OaUpdateObject (&SetHeadTkn,     McStat))
  {
     if (SetBug || DeBug)
     {
       if (KeySize <= 4)
           sprintf (Msg, 
      "\n  FindInTree  %14s[%2u:%4u];  Owner[%2u:%4u] Key(* %2u)= <%6d>\n", 
                  SetDesc->Nam,    SetDescTkn.DbId,  SetDescTkn.Handle,
                  OwnerTkn->DbId,  OwnerTkn->Handle,
                  KeySize,         *(int *)KeyValue);
       else
           sprintf (Msg, 
      "\n  FindInTree  %14s[%2u:%4u];  Owner[%2u:%4u] Key(* %2u)= <%-12s>\n", 
                  SetDesc->Nam,    SetDescTkn.DbId,  SetDescTkn.Handle,
                  OwnerTkn->DbId,  OwnerTkn->Handle,
                  KeySize,         (char *)KeyValue);

        TraceMsg (0, Msg);
     }

     if (DeBug || SetBug)
     if (sprintf (Msg, "     PrimaryKey(*%4u)  = %3u\n",
                  KeySize, PrimaryKey))
        TraceMsg (0, Msg);

     if (DeBug || SetBug)
     if (sprintf (Msg, "     KeyAttr (%3u) size %u \n",
                  PrimaryKey, KeySize))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "     SetHead    [%3u:%4u] @ %x\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle, SetHead))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "             ->TreeNodesDir[%3u:%4u] \n",
                  SetHeadTkn.DbId, SetHead->TreeNodesDir))
     if (TraceMsg (0, Msg))
     if (sprintf (Msg, "             ->TreeKeysDir [%3u:%4u] \n",
                  SetHeadTkn.DbId, SetHead->TreeKeysDir))
        TraceMsg (0, Msg);
  }
  if (Normal(*Status))
     Tree_FindIn       (SetHead,    KeyValue,  
                        McStat,    &LeafNode,   &KeyPos);

  if (DeBug  || SetBug)
  {
      if (KeySize > 4)
        sprintf (Msg,
                 "      SpclFindInTree:: <%-12s> LeafNode[%4u] Pos=%3u;\n",
                 (char *)KeyValue, LeafNode->Index, KeyPos);
      else
        sprintf (Msg,
                 "      SpclFindInTree:: <%6d  > LeafNode[%4u] Pos=%3u;\n",
                *(int *)KeyValue, LeafNode->Index, KeyPos);

      TraceMsg (0,   Msg);

      sprintf  (Msg, "                        Status      = %d;\n",
                *Status);
      TraceMsg (0,   Msg);
  }

  if (*Status == Set_Found)
  {
      *Status  = Env_Normal;
       MemberTkn->Handle = LeafNode->NodeHandles[KeyPos];

  }
  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"SpclFindInTree\n");
return (STAT);
}


/* ------------------------------------------------------------------------ */
boolean    SpclTraverseTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       treeiterfunc   IterFunc,    ft F,lt Z,zz  *Status)
{
tokentype        SetDescTkn  = NullToken;
setdesctype     *SetDesc     = NullPtr;
ltreesethead    *SetHead     = NullPtr;
tokentype        SetHeadTkn  = NullToken;
numtype          IterNum     = 0;
indextype        LeafIndex   = 0;
addrtype         Dummy       = NullPtr;

  Tree_OpType       = aTreeTraverse;

  if (TempKeys_Attr == 0)
  if (Env_GetMemberId  ("TempHandles",   McStat,     &TempHandles_Attr))
  if (Env_GetMemberId  ("TempKeys",      McStat,     &TempKeys_Attr))
  if (Env_GetMemberId  ("TempLeafs",     McStat,     &TempLeafs_Attr))
  if (Env_GetClassId   ("bTreeSetHead",  McStat,     &bTreeSetHead_Class))
  if (Env_InitClassMap (bTreeSetHead_Class,           sizeof(ltreesethead),   
                        C_Apl,                        McStat))
      Env_InvokeMap    (C_Apl,                        McStat);

  SetHeadTkn.DbId   = OwnerTkn->DbId;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (SetBug || DeBug)
  if (sprintf (Msg, 
         "\n   SpclTraverseTree:: SetTkn[%2u:%4u]... OwnerTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->DbId,   OwnerTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (&SetDescTkn,
                     McStat,                   (addrtype *)&SetDesc)) 

  if (OaGet         (False,                     OwnerTkn, 
                     (addrtype )Dummy,          SetDesc->OwnerAttrNum,
                     McStat,        (addrtype )&SetHeadTkn.Handle))
  if (SetHeadTkn.Handle)
  { 
     if (OaGetObject   (&SetHeadTkn,   McStat,    (addrtype *)&SetHead))

        Tree_Traverse  (SetHead,  LeafIndex,  &IterNum,  IterFunc,  McStat);

  } else {
    if (DeBug || SetBug)
      TraceMsg (0, "    ... Empty Set.\n");
   *Status          = Set_EmptySet;
  }

  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"SpclTraverseTree\n");
return (STAT);
}


boolean    Spcl_TreeIterateOn
                      (numtype        SetId,       tokentype     *OwnerTkn,
                       ft F,lt Z,zz  *Status,      indextype     *KeyPos,
                       numtype       *ItNum,       tokentype     *MemberTkn,
                       addrtype      *SetHead,     indextype     *LeafNode)
{
tokentype        SetDescTkn  = NullToken;
setdesctype     *SetDesc     = NullPtr;
tokentype        SetHeadTkn  = NullToken;
ltreesethead    *TreeHead    = NullPtr;
indextype        LeafIndex   = 0;
addrtype         Dummy       = NullPtr;

  Tree_OpType           = aTreeTraverse;

  if (*ItNum           == 0)
  {
     if (TempKeys_Attr == 0)
     if (Env_GetMemberId  ("TempHandles",   McStat,     &TempHandles_Attr))
     if (Env_GetMemberId  ("TempKeys",      McStat,     &TempKeys_Attr))
     if (Env_GetMemberId  ("TempLeafs",     McStat,     &TempLeafs_Attr))
     if (Env_GetClassId   ("bTreeSetHead",  McStat,     &bTreeSetHead_Class))
     if (Env_InitClassMap (bTreeSetHead_Class,           sizeof(ltreesethead),   
                           C_Apl,                        McStat))
         Env_InvokeMap    (C_Apl,                        McStat);

     SetHeadTkn.DbId   = OwnerTkn->DbId;

     SetDescTkn.DbId   = RenvTkn.DbId;
     SetDescTkn.Handle = SetId;

     if (SetBug || DeBug)
     if (sprintf (Msg, 
         "\n   Spcl_TreeIterateOn:: SetTkn[%2u:%4u]... OwnerTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->DbId,   OwnerTkn->Handle))
         TraceMsg (0, Msg);

     if (Normal(*Status))
     if (OaGetObject   (&SetDescTkn,
                        McStat,                   (addrtype *)&SetDesc)) 

     if (OaGet         (False,                     OwnerTkn, 
                        (addrtype )Dummy,          SetDesc->OwnerAttrNum,
                        McStat,        (addrtype )&SetHeadTkn.Handle))
     if (SetHeadTkn.Handle == NullHandle)
     {
        if (DeBug || SetBug)
          TraceMsg (0, "    ... Empty Set.\n");
       *Status = Set_EmptySet;

     } else
         OaGetObject   (&SetHeadTkn,   McStat,    (addrtype *)&SetHead);
  }

  if (Normal(*Status))
  {
     MemberTkn->DbId   = OwnerTkn->DbId;
     TreeHead          = *(ltreesethead **)SetHead;
     LeafIndex         = *LeafNode;
     Tree_IterateOn   (TreeHead,  McStat,    LeafNode,  KeyPos,
                       ItNum,    &MemberTkn->Handle);
  }

  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"Spcl_TreeIterateOn\n");
return (STAT);
}


boolean    SpclDeleteFromTree
                         (handletype     SetDescHndl, tokentype   *MemberTkn,
                          ft F,lt Z,zz  *Status)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
ltreesethead    *SetHead     = NullPtr;
tokentype        SetHeadTkn;
addrtype         Dummy       = NullPtr;
sizetype         KeySize     = 0;
typetype         KeyType     = aInteger;
unsigned char    Value[MAXSTRING];
addrtype         ValuePtr    =  (addrtype )&Value[0];
handletype       Handle      = NullHandle;
attrobjtype      AttrDesc;

  Tree_OpType       = aTreeDeleteFrom;

  SetHeadTkn.DbId   = MemberTkn->DbId;

  MemberTkn->DbId   = MemberTkn->DbId;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (SetBug || DeBug)
  if (sprintf (Msg, 
         "\n   SpclDeleteTreeSet:: SetTkn[%2u:%4u]... MemberTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              MemberTkn->DbId,   MemberTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (&SetDescTkn,
                     McStat,                   (addrtype *)&SetDesc)) 

  if (OaGet         (False,                     MemberTkn, 
                     (addrtype )Dummy,          SetDesc->MemberAttrNum,
                     McStat,        (addrtype )&SetHeadTkn.Handle))
  if (SetHeadTkn.Handle == NullHandle)
  {
      if (DeBug || SetBug)
        TraceMsg (0, "    ... Empty Set.\n");

     *Status = Set_EmptySet;

  }

  if (OaGetObject      (&SetHeadTkn,   McStat,  (addrtype *)&SetHead))
  if (Env_GetAttrInfo  (SetDesc->PrimaryKey,      McStat,   &AttrDesc))
  {
     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
  }

  if (Normal(*Status))
  if (KernelGetAttr     (MemberTkn, SetDesc->PrimaryKey, McStat, ValuePtr))
  {
   if (SetBug || DeBug || DbmBug)
   {
     if (sprintf (Msg,
                  "\n   SpclTreeDeleteFrom %14s[%2u:%4u];\n",
                  SetDesc->Nam,      SetDescTkn.DbId,  SetDescTkn.Handle))
     if (TraceMsg (0, Msg))

     if (KeySize > sizeof(indextype))
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%12s>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, Value);
     else
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%4u>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, *(numtype *)Value);
     TraceMsg (0, Msg);

     if (sprintf (Msg, "              SetHead[%2u:%4u]\n\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle))
        TraceMsg (0, Msg);
   }
  } else {

     sprintf (Msg,"***ERROR*** Primary Key Attr[%4u] Not of Member Class\n",
              SetDesc->PrimaryKey);
     TraceMsg (0, Msg);
     sprintf (Msg, " AT AddIntoSet:: %14s SetTkn[%2u:%4u]\n",
              SetDesc->Nam, SetDescTkn.DbId, SetDescTkn.Handle);
     TraceMsg (0, Msg);
     sprintf (Msg,"                 Member[%2u:%4u]\n", 
              MemberTkn->DbId,  MemberTkn->Handle);
     TraceMsg (0, Msg);
    *Status = Set_KeyNotOfObjCestrs;
  }

  if (Normal(*Status))
     Tree_DeleteFrom   (SetHead,  (addrtype )ValuePtr,  McStat,  &Handle);

  if (!Normal(*Status))
  if (SetDesc)
  {
     if (sprintf (Msg,
                  "\n   SpclTreeDeleteFrom %14s[%2u:%4u];\n",
                  SetDesc->Nam,      SetDescTkn.DbId,  SetDescTkn.Handle))
     if (TraceMsg (0, Msg))

     if (KeySize > sizeof(indextype))
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%12s>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, Value);
     else
         sprintf (Msg, "              Member [%2u:%4u] Key(*%u)[%4u] = <%4u>\n",
                  MemberTkn->DbId,    MemberTkn->Handle, 
                  KeySize,            SetDesc->PrimaryKey, *(numtype *)Value);
     TraceMsg (0, Msg);

     if (sprintf (Msg, "              SetHead[%2u:%4u]\n\n", 
                  SetHeadTkn.DbId, SetHeadTkn.Handle))
        TraceMsg (0, Msg);
  }

  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"SpclDeleteFromTree\n");
return (STAT);
}



boolean    SpclDeleteTree
                         (handletype     SetDescHndl, tokentype     *OwnerTkn,
                          ft F,lt Z,zz  *Status)
{
tokentype        SetDescTkn;
setdesctype     *SetDesc     = NullPtr;
ltreesethead    *SetHead     = NullPtr;
tokentype        SetHeadTkn;
addrtype         Dummy       = NullPtr;

  Tree_OpType       = aTreeDelete;

  SetHeadTkn.DbId   = OwnerTkn->DbId;

  SetDescTkn.DbId   = RenvTkn.DbId;
  SetDescTkn.Handle = SetDescHndl;

  if (SetBug || DeBug)
  if (sprintf (Msg, "\n   SpclDeleteTree :: SetTkn[%2u:%4u]... OwnerTkn[%3u:%4u]\n",
              SetDescTkn.DbId, SetDescTkn.Handle,
              OwnerTkn->DbId,   OwnerTkn->Handle))
      TraceMsg (0, Msg);

  if (OaGetObject   (&SetDescTkn,
                     McStat,                (addrtype *)&SetDesc)) 
  if (OaGet         (False,                              OwnerTkn, 
                     (addrtype )Dummy,                   SetDesc->OwnerAttrNum,
                     McStat,                 (addrtype )&SetHeadTkn.Handle))
  if (Object_GetImage (&SetHeadTkn, McStat, (addrtype *)&SetHead))

  if (DeBug || SetBug)
  if (sprintf  (Msg, 
                "      SpclDeleteTree:: Set<%-12s>  OwnerTkn[%3u:%4u];\n",
                SetDesc->Nam, OwnerTkn->DbId, OwnerTkn->Handle))
      TraceMsg (0,   Msg);

  if (Normal(*Status))
     Tree_Delete    (McStat,      &SetHead);

  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"SpclDeleteTree\n");
return (STAT);
}


/* .......................................................................
                   ....       TREE      ....
   .......................................................................*/

boolean    Tree_Create
                      (handletype    SetDescHndl,  tokentype     *OwnerTkn, 
                       numtype       TreeOrder,    numtype        AllocKeys,
                       sizetype      KeySize,      typetype       KeyType,         
                       ft F,lt Z,zz *Status,       tokentype     *TreeTkn)
{
tokentype     SetDescTkn   = NullToken;
tokentype     SetHeadTkn   = NullToken;
ltreesethead *SetHead;
setdesctype  *SetDesc      = NullPtr;
grpheader    *GrpHdr       = NullPtr;
sizetype      EntrySize    = 0;
numtype       AllocRgns    = 0;
numtype       RgnEntrys    = 0;
numtype       AllocXs      = 0;
numtype       BaseIndex    = 1;
accesstype    AccessType   = Read_Write;
boolean       DiskCache    = False;
numtype       CacheLimit   = 0;
numtype       CacheLoad    = 0;
numtype       NumBuffRgns  = 0;
numtype       BaseRegion   = 0;
addrtype      Owner        = NullPtr;
indextype     Index        = 0;
indextype     *TempKeys     = NullPtr;
handletype    *TempHandles  = NullPtr;
indextype     *TempLeafs    = NullPtr;
tokentype     Vtoken       = NullToken;
numtype       MaxKeys      = 0;
numtype       NumLeafs     = 0;
boolean       TempClassBug = ClassBug;
sizetype      Size1        = 0;

   SetDescTkn.DbId   = RenvTkn.DbId;
   SetDescTkn.Handle = SetDescHndl;


   if (TempKeys_Attr == 0)
   if (Env_GetMemberId  ("TempKeys",      McStat,     &TempKeys_Attr))
   if (Env_GetMemberId  ("TempHandles",   McStat,     &TempHandles_Attr))
   if (Env_GetMemberId  ("TempLeafs",     McStat,     &TempLeafs_Attr))
   if (Env_GetClassId   ("bTreeSetHead",  McStat,     &bTreeSetHead_Class))
   {
      if (Env_InitClassMap
                        (bTreeSetHead_Class,           sizeof(ltreesethead),   
                         C_Apl,                        McStat))
          Env_InvokeMap (C_Apl,                        McStat);
   }
   ClassBug   = TempClassBug;

   if (Normal(*Status))
   if (Vchunk_Create  (OwnerTkn,        sizeof (handletype), TreeOrder+1, 0,
                       McStat,         &Vtoken))
   if (OaGetObject    (&SetDescTkn,     McStat,     (addrtype *)&SetDesc)) 

   if (Object_Create  (OwnerTkn,        bTreeSetHead_Class,
                       McStat,         &SetHeadTkn))

   if (Object_GetImage(&SetHeadTkn,     McStat,     (addrtype *)&SetHead))
   {
      SetHead->Token         = SetHeadTkn; 

      SetHead->PrimaryKey    = SetDesc->PrimaryKey;
      SetHead->Key2ary       = SetDesc->Key2ary;

      SetHead->BuildDesc     = SetDescHndl;
      SetHead->lOwnerObject  = OwnerTkn->Handle;

      SetHead->TreeDbId      = OwnerTkn->DbId;
      SetHead->TreeOrder     = TreeOrder;
      SetHead->TreeRootNode  = 0;

      MaxKeys                = TreeOrder - 1;
      NumLeafs               = TreeOrder + 1;

/* Align Address boundarys                                                */
/* Align Address boundarys of the succedding arrays                       */
      Size1                  = MaxKeys     * sizeof (handletype);
      Ut_AlignStruc          (&Size1);
      EntrySize              =               sizeof (lbtreenode)  
                             + Size1
                             + Size1
                             + (NumLeafs   * sizeof (indextype));
      Ut_AlignStruc          (&EntrySize);
      SetHead->NodeSize      = EntrySize;

      SetHead->NodeCount     = 0;
      SetHead->KeyCount      = 0;

      RgnEntrys              = 50;
      AllocRgns              = (AllocKeys / RgnEntrys) + 42;
      AllocXs                = 1000;
      BaseIndex              = 0;

#ifdef    __MINIMIZE_TREE_CACHE__
      CacheLimit             = AllocRgns;
      CacheLoad              = CacheLimit * 2;
      NumBuffRgns            = CacheLimit;
#else
      CacheLimit             = TreeOrder + 1;
      CacheLimit             = 100;
      CacheLoad              = 100;
      NumBuffRgns            = 100;
#endif /* __MINIMIZE_TREE_CACHE__  */

      if (OaPut         (False,   OwnerTkn,        (addrtype )Owner,
                         SetDesc->OwnerAttrNum,    (addrtype )&SetHead->Token,
                         McStat))

/*    Create the GRP for the Tree Nodes                                   */

      if (Grp_NewPacket  (OwnerTkn,  EntrySize,   AllocRgns,   RgnEntrys,
                          AllocXs,   BaseIndex,   AccessType,  DiskCache,
                          CacheLimit,CacheLoad,   NumBuffRgns,
                          McStat,   &SetHead->TreeNodesDir,
                         &GrpHdr,   &BaseRegion))

      if (Grp_CreateEntry(OwnerTkn,  SetHead->TreeNodesDir, 
                          McStat,   &Index))

/*    Create the GRP for the Node Keys                                    */
      {
         EntrySize              = KeySize;
         SetHead->KeySize       = KeySize;
         SetHead->KeyType       = KeyType;
         SetHead->MaxKeys       = TreeOrder - 1;
         SetHead->MinKeys       = TreeOrder / 2;

         if (KeySize > 8)
         {
            RgnEntrys           = 200;
            AllocRgns           = (AllocKeys / 200) + 42;
            AllocXs             = 1000;
            BaseIndex           = 0;

         } else {
            RgnEntrys           = AllocKeys;
            AllocRgns           = 108;
            AllocXs             = 1000;
            BaseIndex           = 0;

         }

#ifndef   __MINIMIZE_TREE_CACHE__
         CacheLimit          = 7;
         CacheLoad           = CacheLimit * 2;
         NumBuffRgns         = CacheLimit;
#endif /* __MINIMIZE_TREE_CACHE__  */

         if (KeySize > sizeof (indextype))
         if (Grp_NewPacket  (OwnerTkn,  EntrySize,   AllocRgns,   RgnEntrys,
                             AllocXs,   BaseIndex,   AccessType,  DiskCache,
                             CacheLimit,CacheLoad,   NumBuffRgns,
                             McStat,   &SetHead->TreeKeysDir,
                            &GrpHdr,   &BaseRegion))

             Grp_CreateEntry(OwnerTkn,  SetHead->TreeKeysDir, 
                             McStat,   &Index);
      }
      if (Normal(*Status))
      if (Attr_ArrayCreate  (&SetHeadTkn,          TempKeys_Attr,
                             TreeOrder,            sizeof (indextype),
                             McStat,  (addrtype *)&TempKeys))

      if (Attr_ArrayCreate  (&SetHeadTkn,          TempHandles_Attr,
                             TreeOrder,            sizeof (handletype),
                             McStat,  (addrtype *)&TempHandles))

          Attr_ArrayCreate  (&SetHeadTkn,          TempLeafs_Attr,
                             TreeOrder + 1,        sizeof (indextype),
                             McStat,  (addrtype *)&TempLeafs);
   }
   if (SetBug)
      Object_Dump (&SetHeadTkn);

   if (Normal(*Status))
   if (Object_CommitImage   (&SetHeadTkn,     False,  McStat))
     *TreeTkn  = SetHeadTkn;

   if (DeBug  || SetBug)
   if (sprintf  (Msg, "      Tree_Create :: [%4u:%6u];\n", 
                 TreeTkn->DbId, TreeTkn->Handle))
       TraceMsg (0,   Msg);

  Tree_OpType       = aNonTreeOp;

TRACK(TrackBak,"Tree_Create\n");
return(STAT);
}


boolean    Tree_Compare2aryKey
                      (ltreesethead *SetHead,      handletype    ObjHandle,
                       ft F,lt Z,zz *Status)
{
addrtype  Object      = PtrToVoid;
tokentype ObjTkn      = NullToken;
char      MemberVal[64];

  switch (Tree_OpType)
  {
     case aTreeAddInto    :
          ObjTkn         = SetHead->Token;
          ObjTkn.Handle  = ObjHandle;

          if (OaGet       (False,   &AddInto_MemberTkn,     Object,
                           SetHead->Key2ary, 
                           McStat,               (addrtype )MemberVal)) 

              OaCompare   (False,   &ObjTkn,                Object,
                           SetHead->Key2ary,     (addrtype )MemberVal,
                           NullPtr,                         McStat); 
          if (*Status == Oa_LessThen)
              *Status  = Set_LessThen;

          else if (*Status == Oa_GtrThen)
              *Status  = Set_GtrThen;

          break;

     case aTreeFindIn     :
          if (FindIn_2aryKey  != NullPtr)
          {
             ObjTkn         = SetHead->Token;
             ObjTkn.Handle  = ObjHandle;
             OaCompare    (False,   &ObjTkn,                Object,
                           SetHead->Key2ary,                FindIn_2aryKey,
                           NullPtr,                         McStat); 
             if (*Status == Oa_LessThen)
                 *Status  = Set_LessThen;

             else if (*Status == Oa_GtrThen)
                 *Status  = Set_GtrThen;

          }
          break;

     case aTreeTraverse   :
          break;

     case aTreeDeleteFrom :
          break;

     case aTreeDelete     :
          break;
  }

TRACK(TrackBak,"Tree_Compare2aryKey\n");
return(STAT);
}

/* ------------------------------------------------------------------------ */
/*                           END TREE01.C                                   */
/* ------------------------------------------------------------------------ */
