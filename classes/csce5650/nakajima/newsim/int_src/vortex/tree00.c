/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Set- TREE00.c*/        
#define  TREE00
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "vchunk.h"
#include "grp.h"
#include "tree00.h"


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

/*+-----------------------------------------------------------------------+
~B-TREE                   SPECIAL SETS                                    !
  +-----------------------------------------------------------------------+*/





/* ------------------------------------------------------------------------ */
boolean    Tree_AddInto
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       handletype     Handle,      ft F,lt Z,zz  *Status)
{
lbtreenode      *LeafNode        = NullPtr;
lbtreenode      *SibNode         = NullPtr;
indextype        KeyIndex        = 0;
indextype        Index           = 0;
indextype        KeyPos          = 0;
sizetype         Pos             = 0;
sizetype         TempPos         = 0;
#ifdef TREE_BUG
tokentype      TreeTkn         = NullToken;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;
#endif

 *Status          = Set_AddInto;
  if (SetBug)
  if (sprintf (Msg, 
        "   Tree_AddInto:: SetHead[%4u%6u] for KeyValue @%6x; Handle=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, KeyValue,  Handle))
     TraceMsg (0, Msg);

  Tree_OpType       = aTreeFindIn;
  Tree_FindIn      (SetHead, KeyValue, McStat, &LeafNode, &KeyPos);
  Tree_OpType       = aTreeAddInto;

  if (*Status == Set_Found)
      *Status  = Set_AlreadyInSet;

  else if (*Status == Set_EmptySet)
  {
    *Status  = Env_Normal;
     /* Create the new Root Node                                          */
     if (Tree_CreateNode(SetHead,  McStat, &SetHead->TreeRootNode, &LeafNode))

     /* Store the Key Value                                               */
     if (Tree_NewKey    (SetHead,            KeyValue,  McStat,  &KeyIndex))
     {
        SetHead->TreeRootNode    = LeafNode->Index;

        LeafNode->NodeKeys[0]    = KeyIndex;
        LeafNode->NodeHandles[0] = Handle;
      ++LeafNode->NumKeys;

     }

  } else if (*Status == Set_NotFound
         ||  *Status == Set_LessThen)  {

    *Status  = Env_Normal;
     /* Store the Key Value                                               */
     if (Tree_NewKey    (SetHead, KeyValue,  McStat,  &KeyIndex))
     if (LeafNode->NumKeys == SetHead->MaxKeys)
     {
        /* Set up Temporary arrays                                        */
        for (Pos = 0; Pos < SetHead->TreeOrder; Pos++)
        {
           SetHead->TempKeys[Pos]    = 0;
           SetHead->TempHandles[Pos] = 0;
        }

        TempPos  = 0;
        Pos      = 0;

        SetHead->TempKeys[KeyPos]    = KeyIndex;
        SetHead->TempHandles[KeyPos] = Handle;

        /* Copy entries from LeafNode to Temp arrays                      */
        while (Pos < SetHead->MaxKeys)
        {
           if (Pos == KeyPos)
              ++TempPos;

           SetHead->TempKeys[TempPos]    = LeafNode->NodeKeys[Pos];
           SetHead->TempHandles[TempPos] = LeafNode->NodeHandles[Pos];
         ++Pos;
         ++TempPos;

        }

        /* Create a new Leaf Node (aka Sibling)                           */
        if (Tree_CreateNode (SetHead,  McStat,  &Index,   &SibNode))
        {
           SibNode->ParentNode  = LeafNode->ParentNode;
           if (SetBug)
           if (sprintf (Msg, "    Create Sibling Leaf Node:: Index=%4u\n",
                        Index))
              TraceMsg (0, Msg);
        }

        /* Clear Number of Keys in Node                                   */
        if (Normal(*Status))
        {
        LeafNode->NumKeys   = 0;
        SibNode->NumKeys    = 0;
        }

        /* Copy appropiate Keys from temp to LeafNode                     */
        if (Normal(*Status))
        for (Pos = 0; 
             Pos < SetHead->MinKeys;
           ++Pos)
        {
           LeafNode->NodeKeys[Pos]     = SetHead->TempKeys[Pos];
           LeafNode->NodeHandles[Pos]  = SetHead->TempHandles[Pos];
         ++LeafNode->NumKeys;

        }

        if (Normal(*Status))
        for (Pos  = SetHead->MinKeys + 1;
             Pos <= SetHead->MaxKeys;     
           ++Pos)
        {
           SibNode->NodeKeys[   Pos - 1 - SetHead->MinKeys]
                                      =   SetHead->TempKeys[Pos];
           SibNode->NodeHandles[Pos - 1 - SetHead->MinKeys]
                                      =   SetHead->TempHandles[Pos];
         ++SibNode->NumKeys;

        }

        /* Fill remaining entries in LeafNode with null values            */
        if (Normal(*Status))
        for (Pos = SetHead->MinKeys; 
             Pos < SetHead->MaxKeys;
           ++Pos)
        {
           LeafNode->NodeKeys[Pos]     = 0;
           LeafNode->NodeHandles[Pos]  = Null_Handle;
        }

        /* Save the LeafNode and SibNode Images                           */
        /* Promote Key and Handle                                         */
        if (Normal(*Status))
        if (LeafNode->ParentNode == 0)
        {
           /* Create a new   RootNode                                     */
           if (Tree_PromoteRootNode (SetHead,  
                                     SetHead->TempKeys[SetHead->MinKeys],
                                     SetHead->TempHandles[SetHead->MinKeys],
                                     LeafNode, SibNode,    McStat))
           {
              LeafNode->ParentNode   = SetHead->TreeRootNode;
              SibNode->ParentNode    = SetHead->TreeRootNode;
           }

           /* ReSave the LeafNode and SibNode Images                      */

        } else {

           /* Promote the Median Key to LeafNode Parent Node                 */
           if (SetBug)
           if (sprintf (Msg, 
                   "    Promote median key to Leaf Node Parent:: Index=%4u\n",
                        LeafNode->ParentNode))
              TraceMsg (0, Msg);

           /* Promote into a ParentNode                                   */
           if (Normal(*Status))
               Tree_PromoteInternalNode
                            (SetHead,      SibNode,      LeafNode->ParentNode,
                             SetHead->TempKeys[SetHead->MinKeys],
                             SetHead->TempHandles[SetHead->MinKeys],
                             McStat);
        }

        if (Normal(*Status))
        if (SetBug)
        {
           sprintf  (Msg, "    Sibling Leaf Node[%4u]; Parent[%4u]\n",
                     SibNode->Index, SibNode->ParentNode);
           TraceMsg (0,  Msg);
           Tree_DumpNode  (SetHead,  SibNode);
        }

        /* Free the sibling Node                                          */
        if (Normal(*Status))
            Tree_DefrostNode (SetHead,  SibNode->Index,   McStat);

     } else {
        /* Insert the Key at position KeyPos in the Node                  */
        if (SetBug)
        if (sprintf (Msg, "    Leaf[%4u]:: Insert at Key Pos=%3u, Handle=%4u\n",
                     LeafNode->Index, KeyPos, Handle))
           TraceMsg (0, Msg);

        for (Pos = LeafNode->NumKeys;
             Pos > KeyPos;
           --Pos)
        {
          LeafNode->NodeKeys[Pos]     = LeafNode->NodeKeys[Pos - 1];
          LeafNode->NodeHandles[Pos]  = LeafNode->NodeHandles[Pos - 1];
        }

        LeafNode->NodeKeys[KeyPos]    = KeyIndex;
        LeafNode->NodeHandles[KeyPos] = Handle;
      ++LeafNode->NumKeys;

     }
  }

  if (Normal(*Status))
  if (SetBug)
  {
     sprintf  (Msg, "    Current Leaf Node[%4u]; Parent[%4u]\n",
               LeafNode->Index, LeafNode->ParentNode);
     TraceMsg (0,  Msg);
     Tree_DumpNode  (SetHead,  LeafNode);
  }

  /* Free the Leaf Node                                                   */
  if (Normal(*Status))
      Tree_DefrostNode (SetHead,  LeafNode->Index,   McStat);

TRACK(TrackBak,"Tree_AddInto\n");
return (STAT);
}


/* ------------------------------------------------------------------------ */
boolean    Tree_FindIn
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,
                       lbtreenode   **LeafNode,    indextype     *KeyPos)
{
indextype          Index        = 0;
boolean            AddInto      = False;
statustype         ShellStatus  = Env_Normal;
#ifdef TREE_BUG
tokentype          TreeTkn      = NullToken;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;
#endif

  if (SetBug)
  if (sprintf (Msg, 
        "   Tree_FindIn:: SeatHead[%4u%6u] for KeyValue @%6x;\n",
               SetHead->TreeDbId, SetHead->Token.Handle, KeyValue))
     TraceMsg (0, Msg);

  if ((Index   = SetHead->TreeRootNode) == 0)
       *Status    = Set_EmptySet;
  else
  {
     if (*Status == Set_AddInto)
     {
        AddInto   = True;
       *Status    = Env_Normal;
     }
     Tree_RecurseSearch  (SetHead, KeyValue, Index, McStat, LeafNode, KeyPos); 

     if (*Status == Set_EndOfSet)
         *Status  = Set_NotFound;

     if (AddInto == False 
     ||  *Status == Set_Found)
     {
        if (SetBug)
        {
           sprintf  (Msg, "    Current Leaf Node[%4u]; Parent[%4u]\n",
                     (*LeafNode)->Index, (*LeafNode)->ParentNode);
           TraceMsg (0,  Msg);
           Tree_DumpNode  (SetHead,  *LeafNode);
        }

        Tree_DefrostNode (SetHead,  (*LeafNode)->Index,   ShellMcStat);
     }
  }

TRACK(TrackBak,"Tree_FindIn\n");
return (STAT);
}


boolean    Tree_Traverse
                      (ltreesethead  *SetHead,     indextype      LeafIndex,  
                       numtype       *IterNum,     treeiterfunc   IterFunc,
                       ft F,lt Z,zz  *Status)
{
lbtreenode        *LeafNode     = NullPtr;
indextype          ChildIndex   = 0;
numtype            Pos          = 0;
tokentype          Token        = NullToken;
static int         RecurseNum   = 0;

  if (SetBug)
  if (*IterNum == 0)
  if (sprintf (Msg, 
        "   Tree_Traverse:: SeatHead[%4u%6u] LeafIndex=%4u;\n",
               SetHead->TreeDbId, SetHead->Token.Handle, LeafIndex))
     TraceMsg (0, Msg);

  Token.DbId     =  SetHead->TreeDbId;

  if (*IterNum == 0)
  {
     RecurseNum       = 0;
     if ((LeafIndex   = SetHead->TreeRootNode) == 0)
         *Status      = Set_EmptySet;
     else
     {
       *IterNum = 1;
        if (Tree_GetFrozenNode  (SetHead,  SetHead->TreeRootNode, 
                                 McStat,           &LeafNode))
        if (Normal(*Status))
        if (SetBug)
        if (sprintf (Msg, "    Root Node ::     Iter= %3u\n", *IterNum))
        if (TraceMsg (0, Msg))
           Tree_DumpNode   (SetHead,  LeafNode);

     }

  } else {
     if (Tree_GetFrozenNode  (SetHead,           LeafIndex, 
                              McStat,           &LeafNode))
     if (SetBug)
     if (sprintf (Msg, "    Leaf Node ::     Iter= %3u\n", IterNum))
     if (TraceMsg (0, Msg))
         Tree_DumpNode   (SetHead,  LeafNode);
  }

  if (Normal(*Status))
  for (Pos = 0;
       Pos < LeafNode->NumKeys  &&  Normal(*Status);
     ++Pos)
  {
     if ((ChildIndex = LeafNode->NodeLeafs[Pos]) > 0)
        Tree_Traverse (SetHead,  ChildIndex, IterNum,  IterFunc,  McStat);

 if (Normal(*Status))
     if ((Token.Handle = LeafNode->NodeHandles[Pos]) > 0)
     {
        IterFunc (&Token, *IterNum, McStat);
       *IterNum += 1;
      ++RecurseNum;

     }
  }
  if (Normal(*Status))
  {
     if ((ChildIndex = LeafNode->NodeLeafs[Pos]) > 0)
        Tree_Traverse (SetHead,  ChildIndex, IterNum,  IterFunc,  McStat);
  }
  if (Normal(*Status))
     Tree_DefrostNode (SetHead,   LeafIndex,   McStat);

TRACK(TrackBak,"Tree_Traverse\n");
return (STAT);
}


boolean    Tree_IterateOn
                      (ltreesethead  *SetHead,      ft F,lt Z,zz  *Status,
                       indextype     *CurrLeaf,     indextype     *KeyPos,
                       numtype       *IterNum,      handletype    *Handle)
{
lbtreenode        *LeafNode     = NullPtr;
tokentype          Token        = NullToken;

  if (SetBug)
  if (*IterNum == 0)
  if (sprintf (Msg, 
        "   Tree_IterateOn:: SeatHead[%4u%6u] LeafIndex=%4u;\n",
               SetHead->TreeDbId, SetHead->Token.Handle, *CurrLeaf))
     TraceMsg (0, Msg);

  Token.DbId     =  SetHead->TreeDbId;

  if (*IterNum == 0)
  {
    *KeyPos           = 0;
     if ((*CurrLeaf   = SetHead->TreeRootNode) == 0)
         *Status      = Iterate_EndOf;
     else
     {
       *IterNum = 1;
        if (Tree_GetFrozenNode  (SetHead,  SetHead->TreeRootNode, 
                                 McStat,           &LeafNode))
        if (Normal(*Status))
        if (SetBug)
        if (sprintf (Msg, "    Root Node ::     Iter= %3u\n", *IterNum))
        if (TraceMsg (0, Msg))
           Tree_DumpNode   (SetHead,  LeafNode);

     }

  } else {
     if (Tree_GetFrozenNode  (SetHead,          *CurrLeaf, 
                              McStat,           &LeafNode))
     if (SetBug)
     if (sprintf (Msg, "    Leaf Node ::     Iter= %3u\n", *IterNum))
     if (TraceMsg (0, Msg))
         Tree_DumpNode   (SetHead,  LeafNode);
  }

TRACK(TrackBak,"Tree_IterateOn\n");
return (STAT);
}


boolean    Tree_Validate
                      (ltreesethead  *SetHead,     indextype      LeafIndex,  
                       numtype       *IterNum,     ft F,lt Z,zz  *Status)
{
lbtreenode        *LeafNode     = NullPtr;
lbtreenode        *ParentNode   = NullPtr;
indextype          ChildIndex   = 0;
numtype            Pos          = 0;
numtype            Sos          = 0;
tokentype          Token        = NullToken;
static int         RecurseNum   = 0;
#ifdef TREE_BUG
tokentype          TreeTkn      = NullToken;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;
#endif

  if (*IterNum == 0)
  if (SetBug)
  if (sprintf (Msg, 
        "   Tree_VALIDATE:: SeatHead[%4u%6u] LeafIndex=%4u;\n",
               SetHead->TreeDbId, SetHead->Token.Handle, LeafIndex))
     TraceMsg (0, Msg);

  Token.DbId     =  SetHead->TreeDbId;

  if (*IterNum == 0)
  {
     RecurseNum       = 0;
     if ((LeafIndex   = SetHead->TreeRootNode) == 0)
         *Status      = Set_EmptySet;
     else
     {
       *IterNum = 1;
        if (Tree_GetFrozenNode  (SetHead,  SetHead->TreeRootNode, 
                                 McStat,           &LeafNode))
        if (SetBug)
        if (sprintf (Msg, "    Root Node ::     Iter= %3u\n", *IterNum))
        if (TraceMsg (0, Msg))
           Tree_DumpNode   (SetHead,  LeafNode);
     }

  } else {
     if (Tree_GetFrozenNode  (SetHead,           LeafIndex, 
                              McStat,           &LeafNode))
     if (SetBug)
     if (sprintf (Msg, "    Leaf Node ::     Iter= %3u\n", *IterNum))
     if (TraceMsg (0, Msg))
         Tree_DumpNode   (SetHead,  LeafNode);

     if (Tree_GetFrozenNode  (SetHead,           LeafNode->ParentNode, 
                              McStat,           &ParentNode))
     for (Sos  = 0;
          ParentNode->NodeLeafs[Sos] != LeafNode->Index
     &&   Sos                         < ParentNode->NumKeys+1;
        ++Sos)
     {
        ;
     }
     if (Normal(*Status))
        Tree_DefrostNode    (SetHead,  LeafNode->ParentNode,  McStat);

     if (Sos > ParentNode->NumKeys)
     {
      sprintf (Msg, 
    "***ERROR @ Tree_Validate:: SetHead[%4u:%6u] for LeafNode [%4u]; Pos=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, 
               LeafNode->Index, Pos);
      SendMsg  (0, Msg);
      TraceMsg (0, "\n Dump of LeafNode\n");
      Tree_DumpNode (SetHead, LeafNode);
      TraceMsg (0, "\n Dump of ParentNode... LeafIndex not found.\n");
      Tree_DumpNode (SetHead, ParentNode);

      Tree_DefrostNode    (SetHead,  LeafNode->Index,       McStat);
      Tree_DefrostNode    (SetHead,  LeafNode->ParentNode,  McStat);

     *Status = Tree_UnmatchedParent;
      return (STAT);
     }
  }

  if (Normal(*Status))
  for (Pos = 0;
       Pos < LeafNode->NumKeys  &&  Normal(*Status);
     ++Pos)
  {
     if ((ChildIndex = LeafNode->NodeLeafs[Pos]) > 0)
        Tree_Validate (SetHead,  ChildIndex, IterNum,  McStat);

     if (Normal(*Status))
     if ((Token.Handle = LeafNode->NodeHandles[Pos]) > 0)
     {
       *IterNum += 1;
      ++RecurseNum;
     }
  }
  if (Normal(*Status))
  {
     if ((ChildIndex = LeafNode->NodeLeafs[Pos]) > 0)
        Tree_Validate (SetHead,  ChildIndex, IterNum,  McStat);
  }
  if (Normal(*Status))
     Tree_DefrostNode (SetHead,   LeafIndex,   McStat);


TRACK(TrackBak,"Tree_Validate\n");
return (STAT);
}


boolean    Tree_Delete   (ft F,lt Z,zz *Status,       ltreesethead **SetHead)
{
bytessizetype size         = (bytessizetype )sizeof (ltreesethead);
dbheader     *DbHdr        = NullPtr;
ltreesethead *TreeHead     = *SetHead;
bytessizetype ArraySize    = 0;
numtype       TreeOrder    = (*SetHead)->TreeOrder;
tokentype     Vtoken       = NullToken;
indextype     Index        = (*SetHead)->Token.Handle;
static
nametype      NullName     = " ";

 if (SetBug || DeBug)
 if (sprintf (Msg, "\n   Delete the Tree [%2u:%4u]  \n",
               TreeHead->Token.DbId, TreeHead->Token.Handle))
    TraceMsg (0, Msg);

 ArraySize             = TreeOrder     * sizeof (indextype);
 if (TmGetCoreDb         ((&TreeHead->Token),          McStat,       &DbHdr))
 {
    Vtoken.DbId         = TreeHead->Token.DbId;
    Vtoken.Handle       = DbHdr->TreeNames;
 }

 if (CoreFreeCoreSpace   ((addrtype *)&TreeHead->TempKeys,       ArraySize,
                          McStat))
 {
    ArraySize          = TreeOrder     * sizeof (handletype);
    if (CoreFreeCoreSpace((addrtype *)&TreeHead->TempHandles,    ArraySize,
                          McStat))
    {
    ArraySize          =(TreeOrder+1)  * sizeof (indextype);
        CoreFreeCoreSpace((addrtype *)&TreeHead->TempHandles,    ArraySize,
                          McStat);
    }
 }

 if (Normal(*Status))
 if (Grp_DeletePacket    (&TreeHead->Token,  TreeHead->TreeNodesDir, McStat))
 if (TreeHead->TreeKeysDir)
     Grp_DeletePacket    (&TreeHead->Token,  TreeHead->TreeKeysDir,  McStat);

 if (Normal(*Status))
 if (Vchunk_PutUnit      (&Vtoken,           Index,       (addrtype )NullName,
                          McStat))
 if (Grp_DeleteEntry    (&TreeHead->Token,  DbHdr->TreeHeadDir,     Index,
                           McStat))
    CoreFreeCoreSpace    ((addrtype *)SetHead,      size,            McStat);

 *SetHead  = NullPtr;

TRACK(TrackBak,"Tree_Delete\n");
return(STAT);
}


/* ------------------------------------------------------------------------ */
boolean    Tree_PromoteRootNode
                      (ltreesethead  *SetHead,
                       indextype      RootKey,     handletype     RootHandle,     
                       lbtreenode    *LessNode,    lbtreenode    *GtrNode,  
                       ft F,lt Z,zz  *Status)
{
lbtreenode      *RootNode        = NullPtr;
indextype        Index           = 0;

  if (SetBug)
  if (sprintf (Msg, "   Tree_PromoteRootNode:: Less_Leaf= %3u; Gtr_Leaf=%4u\n",
               LessNode->Index,  GtrNode->Index))
     TraceMsg (0, Msg);

  /* Create a new Root Node                                                */
  if (Tree_CreateNode (SetHead,  McStat,  &Index,   &RootNode))
  {
      SetHead->TreeRootNode    = Index;

      RootNode->NumKeys        = 1;

      RootNode->NodeKeys[0]    = RootKey;
      RootNode->NodeHandles[0] = RootHandle;

      RootNode->NodeLeafs[0]   = LessNode->Index;
      RootNode->NodeLeafs[1]   = GtrNode->Index;

  }
  if (SetBug)
  if (RootNode != NullPtr)
  {
     sprintf  (Msg, "    Root    .... Node[%4u]; Parent{%4u]\n",
               RootNode->Index, RootNode->ParentNode);
     TraceMsg (0,  Msg);
     Tree_DumpNode  (SetHead,  RootNode);
  }

  /* Free the Root Node                                                    */
  if (Normal(*Status))
     Tree_DefrostNode (SetHead,  RootNode->Index,   McStat);


TRACK(TrackBak,"Tree_PromoteRootNode\n");
return (STAT);
}


boolean    Tree_PromoteInternalNode
                      (ltreesethead  *SetHead,     
                       lbtreenode    *LeafNode,    indextype      ParentIndex,
                       indextype      KeyIndex,    handletype     Handle,     
                       ft F,lt Z,zz  *Status)
{

lbtreenode      *ParentNode      = NullPtr;
lbtreenode      *SibNode         = NullPtr;
lbtreenode      *ChildNode       = NullPtr;
indextype        KeyPos          = 0;
indextype        Index           = 0;
sizetype         Pos             = 0;
sizetype         TempPos         = 0;

  if (SetBug)
  if (sprintf (Msg, "   Tree_PromoteInternalNode:: Parent= %3u; \n",
               ParentIndex))
  if (TraceMsg (0, Msg))
  if (sprintf (Msg, 
     "                              Leaf  = %3u, KeyIndex=%4u,  Handle=%6u\n",
               LeafNode->Index, KeyIndex, Handle))
     TraceMsg (0, Msg);

  if (SetHead->Key2ary)
     AddInto_MemberTkn.Handle = Handle;

  if (Tree_GetFrozenNode (SetHead,  ParentIndex, McStat,  &ParentNode))
  if (ParentNode->NumKeys  == SetHead->MaxKeys)
  {
      /* Set up Temporary arrays                                        */
      for (Pos = 0; Pos < SetHead->TreeOrder; Pos++)  
      {  
         SetHead->TempKeys[Pos]    = 0;  
         SetHead->TempHandles[Pos] = 0;  
         SetHead->TempLeafs[Pos]   = 0;  
      }  
      SetHead->TempLeafs[Pos]      = 0;  
        
        
      SetHead->TempLeafs[0]        = ParentNode->NodeLeafs[0];

      /* Find Insertion position                                        */  
      Tree_GetNodeInsertPos (SetHead,   KeyIndex,   ParentNode, 
                             McStat,               &KeyPos);
      if (*Status == Set_GtrThen
      ||  *Status == Set_LessThen)
          *Status = Env_Normal;

      if (Normal(*Status))
      {
      /* Copy entries from LeafNode to Temp arrays                      */
         SetHead->TempKeys[KeyPos]     = KeyIndex;
         SetHead->TempHandles[KeyPos]  = Handle;
         SetHead->TempLeafs[KeyPos+1]  = LeafNode->Index;

      }

      TempPos  = 0;
      Pos      = 0;  
      if (Normal(*Status))
      /* Copy existing keys                                             */
      while (Pos < SetHead->MaxKeys)  
      {  
         if (Pos == KeyPos)  
            ++TempPos;  
        
         SetHead->TempKeys[TempPos]    = ParentNode->NodeKeys[Pos];
         SetHead->TempHandles[TempPos] = ParentNode->NodeHandles[Pos];  
         SetHead->TempLeafs[TempPos+1] = ParentNode->NodeLeafs[Pos+1];  

       ++Pos;  
       ++TempPos;  
      }
        
      /* Create a new Leaf Node (aka Sibling)                           */
      if (Normal(*Status))
      if (Tree_CreateNode (SetHead,  McStat,  &Index,   &SibNode))  
      {  
         SibNode->ParentNode  = ParentNode->ParentNode;  
         if (SetBug)  
         if (sprintf (Msg, "    Create Sibling Leaf Node:: Index=%4u\n",  
                      Index))  
            TraceMsg (0, Msg);  
      }  
        
      /* Clear Number of Keys in Node                                   */
      ParentNode->NumKeys   = 0;  
      SibNode->NumKeys      = 0;  

      ParentNode->NodeLeafs[0]  = SetHead->TempLeafs[0];
  
      /* Copy appropiate Keys from temp to LeafNode                     */
      if (Normal(*Status))  
      for (Pos = 0;   
           Pos < SetHead->MinKeys;  
         ++Pos)  
      {  
         ParentNode->NodeKeys[Pos]       = SetHead->TempKeys[Pos];  
         ParentNode->NodeHandles[Pos]    = SetHead->TempHandles[Pos];  
         ParentNode->NodeLeafs[Pos+1]    = SetHead->TempLeafs[Pos+1];  
       ++ParentNode->NumKeys;  
        
      }
      SibNode->NodeLeafs[0]   = SetHead->TempLeafs[SetHead->MinKeys + 1];
  
      if (Normal(*Status))
      for (Pos  = SetHead->MinKeys + 1;  
           Pos <= SetHead->MaxKeys;       
         ++Pos)  
      {  
         SibNode->NodeKeys[   Pos - 1 - SetHead->MinKeys]  
                                    =   SetHead->TempKeys[Pos];  
         SibNode->NodeHandles[Pos - 1 - SetHead->MinKeys]  
                                    =   SetHead->TempHandles[Pos];  
         SibNode->NodeLeafs[  Pos     - SetHead->MinKeys]  
                                    =   SetHead->TempLeafs[Pos + 1];  
       ++SibNode->NumKeys;  
        
      }
        
      /* Fill remaining entries in LeafNode with null values            */
      if (Normal(*Status))  
      for (Pos = SetHead->MinKeys;   
           Pos < SetHead->MaxKeys;  
         ++Pos)  
      {  
         ParentNode->NodeKeys[   Pos]        = 0;  
         ParentNode->NodeHandles[Pos]        = Null_Handle;  
         ParentNode->NodeLeafs[  Pos + 1]    = 0;  
      }  

      /* Update the Child Parent links                                  */
      if (Normal(*Status))  
      for (Pos  = 0;
           Pos <= SibNode->NumKeys;   
         ++Pos)  
      {  
         if (Tree_GetFrozenNode (SetHead,  SibNode->NodeLeafs[Pos],  
                                 McStat,  &ChildNode))  
            ChildNode->ParentNode        = SibNode->Index;  

         if (SetBug)  
         {  
            sprintf  (Msg, "    Child   Leaf Node[%4u]; Parent[%4u]\n",  
                      ChildNode->Index, ChildNode->ParentNode);  
            TraceMsg (0,  Msg);  
         }  
           
         /* Free the child node                                            */
         if (Normal(*Status))  
             Tree_DefrostNode (SetHead,  ChildNode->Index,   McStat);  
         
      }  
        
      /* Promote Key and Handle                                            */  
      if (ParentNode->ParentNode == 0)  
      {  
         /* Create a new   RootNode                                     */  
         if (Tree_PromoteRootNode (SetHead,    
                                   SetHead->TempKeys[SetHead->MinKeys],  
                                   SetHead->TempHandles[SetHead->MinKeys],  
                                   ParentNode, SibNode,    McStat))  
         {  
            ParentNode->ParentNode = SetHead->TreeRootNode;  
            SibNode->ParentNode    = SetHead->TreeRootNode;  
         }  
        
      } else {
        
         /* Promote the Median Key to LeafNode Parent Node                 */
         if (SetBug)  
         if (sprintf (Msg,   
                 "    Promote median key to Leaf Node Parent:: Index=%4u\n",  
                      LeafNode->ParentNode))  
            TraceMsg (0, Msg);  
        
         /* Promote into a ParentNode                                   */
         if (Normal(*Status))  
             Tree_PromoteInternalNode  
                          (SetHead,      SibNode,     ParentNode->ParentNode,  
                           SetHead->TempKeys[SetHead->MinKeys],  
                           SetHead->TempHandles[SetHead->MinKeys],  
                           McStat);
      }  
        
      if (Normal(*Status))
      if (SetBug)  
      {  
         sprintf  (Msg, "    Sibling Leaf Node[%4u]; Parent[%4u]\n",  
                   SibNode->Index, SibNode->ParentNode);  
         TraceMsg (0,  Msg);  
         Tree_DumpNode  (SetHead,  SibNode);
      }  
      /* Free the sibling Node                                            */
      if (Normal(*Status))
          Tree_DefrostNode (SetHead,  SibNode->Index,   McStat);

  } else {
     /* Insert the Key at position KeyPos in the Parent Node               */

     /* Find Insertion position                                        */  
     Tree_GetNodeInsertPos (SetHead,   KeyIndex,   ParentNode, 
                            McStat,               &Pos);
     if (*Status == Set_GtrThen
     ||  *Status == Set_LessThen)
         *Status = Env_Normal;

     /* Shift any keys right                                              */
     for (TempPos = ParentNode->NumKeys;
          TempPos > Pos;
          --TempPos)
     { 
       ParentNode->NodeKeys[TempPos]    = ParentNode->NodeKeys[TempPos - 1];
       ParentNode->NodeHandles[TempPos] = ParentNode->NodeHandles[TempPos- 1];
       ParentNode->NodeLeafs[TempPos+1] = ParentNode->NodeLeafs[TempPos];
     }

     /* Store Node                                                        */
     if (Normal(*Status))
     {
        ParentNode->NumKeys++;
        ParentNode->NodeKeys[Pos]    = KeyIndex;
        ParentNode->NodeHandles[Pos] = Handle;
        ParentNode->NodeLeafs[Pos+1] = LeafNode->Index;
     }

     if (Normal(*Status))
     if (SetBug)
     {
        sprintf  (Msg, "    Parent  Leaf Node[%4u]; Parent_Of[%4u]\n",
                  ParentNode->Index, ParentNode->ParentNode);
        TraceMsg (0,  Msg);
        Tree_DumpNode  (SetHead,  ParentNode);
     }
  }
  if (Normal(*Status))
     Tree_DefrostNode (SetHead,  ParentIndex,   McStat);

TRACK(TrackBak,"Tree_PromoteInternalNode\n");
return (STAT);
}


boolean    Tree_RecurseSearch
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       indextype      Index,       ft F,lt Z,zz  *Status,
                       lbtreenode   **LeafNode,    indextype     *KeyPos)
{
indextype          NodeIndex    = 0;

 *KeyPos          = 0;

  if (SetBug)
  if (sprintf (Msg, "   TreeRecurse:: Node[%4u]\n", Index))
    TraceMsg  (0,   Msg);

  if (Tree_GetFrozenNode      (SetHead,  Index,    McStat,   LeafNode))
  if (Index                 == SetHead->TreeRootNode)
  if ((*LeafNode)->NumKeys  == 0)
       *Status               = Set_EndOfSet;

  if (SetBug)
  if (Index == SetHead->TreeRootNode)
  {
     sprintf  (Msg, "    ROOT         Node[%4u]; Parent_Of[%4u]\n",
               (*LeafNode)->Index, (*LeafNode)->ParentNode);
     TraceMsg (0,  Msg);
     Tree_DumpNode  (SetHead,  *LeafNode);

  } else {
     sprintf  (Msg, "    LEAF         Node[%4u]; Parent_Of[%4u]\n",
               (*LeafNode)->Index, (*LeafNode)->ParentNode);
     TraceMsg (0,  Msg);
     Tree_DumpNode  (SetHead,  *LeafNode);
  }

  while (Normal(*Status))
  {

     if (SetBug)
     if (sprintf (Msg, "                   Node[%4u] KeyPos  =%3u\n",
                 (*LeafNode)->Index, *KeyPos))
       TraceMsg  (0,   Msg);
 
     if (*KeyPos == 0)
     {
         Tree_GetRecursePos (SetHead,  KeyValue, *LeafNode, McStat, KeyPos);
         if (Normal(*Status)) 
           *Status      = Set_Found;

     } else if (*KeyPos == (*LeafNode)->NumKeys) {
        if ((*LeafNode)->NodeLeafs[*KeyPos] == 0)
        {
           *Status = Set_EndOfSet;

        } else {
           *Status = Set_GtrThen;
        }

     } else {
        Tree_CompareKey  (SetHead,                             KeyValue, 
                          (*LeafNode)->NodeKeys[*KeyPos],
                          (*LeafNode)->NodeHandles[*KeyPos],   McStat);
        if (Normal(*Status))
          *Status   = Set_Found;
     }

     if (*Status == Set_LessThen)
     {
       *Status = Env_Normal;
        if ((NodeIndex = (*LeafNode)->NodeLeafs[*KeyPos]) > 0)
        {
           if (Tree_DefrostNode   (SetHead,    (*LeafNode)->Index,    McStat))
               Tree_RecurseSearch (SetHead,      KeyValue, NodeIndex,
                                   McStat,       LeafNode, KeyPos); 
        } else
          *Status = Set_EndOfSet;

     } else if (*Status == Set_GtrThen) {

       *Status  = Env_Normal;
        if (*KeyPos + 1 == (*LeafNode)->NumKeys
        && (NodeIndex    = (*LeafNode)->NodeLeafs[*KeyPos+1]) > 0)
        {
           if (Tree_DefrostNode   (SetHead,    (*LeafNode)->Index,    McStat))
               Tree_RecurseSearch (SetHead,      KeyValue, NodeIndex,
                                   McStat,       LeafNode, KeyPos); 

        } else if (*KeyPos < (*LeafNode)->NumKeys)
          ++(*KeyPos);

        else
          *Status = Set_EndOfSet;

     }
  }

TRACK(TrackBak,"Tree_RecurseSearch\n");
return (STAT);
}


boolean    Tree_GetRecursePos
                      (ltreesethead *SetHead,      addrtype      KeyValue,
                       lbtreenode   *LeafNode,
                       ft F,lt Z,zz *Status,       indextype    *KeyPos)
{
indextype        Pos             = 0;
numtype          LowKey          = 0;
numtype          MidKey          = 0;
numtype          HighKey         = 0;
numtype          Span            = 0;
boolean          NotFound        = True;

  *KeyPos                       = 0;  

   /* Find Insertion position                                            */  
   if (LeafNode->NumKeys)
   {
       NotFound   = False;
      *Status     = Env_Normal;
       Tree_CompareKey (SetHead,                         KeyValue, 
                        LeafNode->NodeKeys[LowKey],
                        LeafNode->NodeHandles[LowKey],   McStat);

       if (Normal(*Status))
       {
          *KeyPos     = LowKey;
          *Status     = Set_Found;

       } else if (*Status    == Set_LessThen) {
          *KeyPos     = LowKey;

       } else {

         if (LeafNode->NumKeys > 1)
         {
           Pos        = LeafNode->NumKeys - 1;
          *Status     = Env_Normal;

           Tree_CompareKey (SetHead,                      KeyValue, 
                            LeafNode->NodeKeys[Pos],
                            LeafNode->NodeHandles[Pos],   McStat);

           if (Normal(*Status))
           {
             *KeyPos     = Pos;
             *Status     = Set_Found;

           } else if (*Status    == Set_GtrThen)  {
             *KeyPos     = Pos;

           } else {
             *Status     = Set_GtrThen;
              NotFound   = True;
           }
         } else
           NotFound   = True;

       }
   }

if (NotFound)
{
   if (LeafNode->NumKeys  > 10)
   {
     *Status          = Env_Normal;

      HighKey         = Pos;
      Span            = (HighKey - LowKey) / 2;
      MidKey          = LowKey + Span;

      while (NotFound)
      {
        *Status       = Env_Normal;
        *KeyPos       = MidKey;
         Tree_CompareKey (SetHead,                         KeyValue, 
                          LeafNode->NodeKeys[MidKey],
                          LeafNode->NodeHandles[MidKey],   McStat);

         if (Normal(*Status))
         {
            NotFound  = False;
           *Status    = Set_Found;

         } else {
            if (*Status  == Set_GtrThen)
            {
               LowKey    = MidKey;
              *KeyPos    = MidKey + 1;
              *Status    = Set_LessThen;

            } else
               HighKey   = MidKey;

            if (HighKey - LowKey > 1)
            {
               Span      = (HighKey - LowKey) / 2;
               MidKey    =  LowKey  + Span;
            } else
               NotFound  = False;
         }
      }

   } else {
     ;
/*
   ++(*KeyPos);
      while (*Status  == Set_GtrThen
      &&     *KeyPos   < LeafNode->NumKeys)
      {
         *Status = Env_Normal;
          Tree_CompareKey     (SetHead,                          KeyValue, 
                               LeafNode->NodeKeys[*KeyPos],
                               LeafNode->NodeHandles[*KeyPos],   McStat);
          if (*Status == Set_GtrThen)
          if (*KeyPos < LeafNode->NumKeys)
             ++(*KeyPos);
      }
*/
   }
}

   if (SetBug)
   if (sprintf (Msg, "    Recurse LeafNode Key Position =%3u; Status = %4d\n",
                *KeyPos, *Status))
      TraceMsg (0, Msg);


TRACK(TrackBak,"Tree_GetRecursePos\n");
return (STAT);
}


/* ------------------------------------------------------------------------ */
boolean    Tree_CreateNode
                      (ltreesethead  *SetHead,     ft F,lt Z,zz  *Status,
                       indextype     *Index,       lbtreenode   **LeafNode)
{
tokentype   TreeTkn    = NullToken;
lbtreenode *TreeNode   = NullPtr;
indextype   Offset     = SetHead->MaxKeys * sizeof (handletype);

/* Align Address boundarys of the succedding arrays                       */
   Ut_AlignStruc    (&Offset);

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;

  if (Grp_CreateEntry  (&TreeTkn,    SetHead->TreeNodesDir, 
                        McStat,      Index))
  if (Grp_GetFrozenEntry
                       (&TreeTkn,    SetHead->TreeNodesDir,   *Index,
                        McStat,     (addrtype *)LeafNode))
  {
  ++SetHead->NodeCount;

    TreeNode              = *LeafNode;
    TreeNode->Index       = *Index;              
    TreeNode->ParentNode  = 0;              
    TreeNode->NumKeys     = 0;              

    TreeNode->NodeKeys    = (indextype  *)((char *)TreeNode 
                                                +  sizeof(lbtreenode));
    TreeNode->NodeHandles = (handletype *)((char *)TreeNode->NodeKeys
                                                +  Offset);
    TreeNode->NodeLeafs   = (indextype  *)((char *)TreeNode->NodeHandles
                                                +  Offset);
  }

TRACK(TrackBak,"Tree_CreateNode\n");
return (STAT);
}



boolean    Tree_GetFrozenNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status,      lbtreenode   **LeafNode)
{
tokentype   TreeTkn    = NullToken;
lbtreenode        *TreeNode   = NullPtr;
indextype          Offset     = SetHead->MaxKeys * sizeof (handletype);

/* Align Address boundarys of the succedding arrays                       */
  Ut_AlignStruc      (&Offset);

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
  if (Grp_GetFrozenEntry  (&TreeTkn,  SetHead->TreeNodesDir, Index,
                           McStat,  (addrtype *)LeafNode))
  {
    TreeNode              = *LeafNode;
    TreeNode->NodeKeys    = (indextype *)((char *)TreeNode 
                          +                sizeof(lbtreenode));
    TreeNode->NodeHandles = (handletype *)((char *)TreeNode->NodeKeys
                                                +  Offset);
    TreeNode->NodeLeafs   = (indextype  *)((char *)TreeNode->NodeHandles
                                                +  Offset);
  }

TRACK(TrackBak,"Tree_GetFrozenNode\n");
return (STAT);
}


boolean    Tree_FreezeNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status)
{
tokentype   TreeTkn = NullToken;

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
  Grp_FreezeEntry    (&TreeTkn,  SetHead->TreeNodesDir, Index, McStat);

TRACK(TrackBak,"Tree_FreezeNode\n");
return (STAT);
}


boolean    Tree_DefrostNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status)
{
tokentype   TreeTkn = NullToken;

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
  Grp_DefrostEntry    (&TreeTkn,  SetHead->TreeNodesDir, Index, McStat);

TRACK(TrackBak,"Tree_DefrostNode\n");
return (STAT);
}


boolean    Tree_NodeIsFrosted
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status)
{
tokentype   TreeTkn = NullToken;

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
return (Grp_EntryIsFrosted (&TreeTkn,  SetHead->TreeNodesDir, Index, McStat));
}


void       Tree_DumpNode
                      (ltreesethead  *SetHead,     lbtreenode    *LeafNode)
{
inttype            i            = 0;

  if (LeafNode != NullPtr)
  {
      if (SetBug)
      {
      sprintf  (Msg, "    SetHead[%4u:%4u]  RootNode[%4u]\n",
                SetHead->TreeDbId, SetHead->Token.Handle, 
                SetHead->TreeRootNode);
      TraceMsg (0,  Msg);
      }
      sprintf  (Msg, "    Leaf Node[%4u];   Parent[%4u];   Num Keys=%4u\n",
                LeafNode->Index, LeafNode->ParentNode, LeafNode->NumKeys);
      TraceMsg (0,  Msg);
      TraceMsg (0,   "       Keys     []::\n          ");
      for (i = 0; i < LeafNode->NumKeys; )
      {
         sprintf  (Msg, "    %4u", LeafNode->NodeKeys[i]);
         TraceMsg (0,  Msg);
         if (++i % 5 == 0)
            TraceMsg (0,  "\n          ");
      }
      TraceMsg (0,   "\n       Handles  []::\n          ");
      for (i = 0; i < LeafNode->NumKeys; )
      {
         sprintf  (Msg, "    %4u", LeafNode->NodeHandles[i]);
         TraceMsg (0,  Msg);
         if (++i % 5 == 0)
            TraceMsg (0,  "\n          ");
      }
      TraceMsg (0,   "\n       NodeLeafs[]::\n          ");
      for (i = 0; i < LeafNode->NumKeys+1; )
      {
         sprintf  (Msg, "    %4u", LeafNode->NodeLeafs[i]);
         TraceMsg (0,  Msg);
         if (++i % 5 == 0)
            TraceMsg (0,  "\n          ");
      }
      TraceMsg (0,   "\n\n");
  }
return;
}



boolean    Tree_NewKey
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,      indextype     *Index)
{
tokentype   TreeTkn    = NullToken;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;

  SetHead->KeyCount       += 1;

  if (SetHead->KeySize <= sizeof (indextype))
     *Index             = *(indextype *)KeyValue;
  else
     /* Store the Key Value in the Grp object                             */
     Grp_PutNewEntry   (&TreeTkn, SetHead->TreeKeysDir, KeyValue,
                        McStat,   Index);

TRACK(TrackBak,"Tree_NewKey\n");
return (STAT);
}


boolean    Tree_GetFrozenKey
                      (ltreesethead  *SetHead,     indextype     *Index,
                       ft F,lt Z,zz  *Status,      addrtype      *KeyValue)
{
tokentype   TreeTkn    = NullToken;

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
  if (SetHead->KeySize <= sizeof (indextype))
    *KeyValue  = (addrtype )Index;

  else
     Grp_GetFrozenEntry  (&TreeTkn,  SetHead->TreeKeysDir, *Index,
                           McStat,                          KeyValue);

TRACK(TrackBak,"Tree_GetFrozenKey\n");
return (STAT);
}


boolean    Tree_DefrostKey
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status)
{
tokentype   TreeTkn = NullToken;

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;
 
  if (SetHead->KeySize > sizeof (indextype))
     Grp_DefrostEntry  (&TreeTkn,  SetHead->TreeKeysDir, Index, McStat);

TRACK(TrackBak,"Tree_DefrostKey\n");
return (STAT);
}


boolean    Tree_GetNodeInsertPos
                      (ltreesethead *SetHead,      indextype     KeyIndex,
                       lbtreenode   *LeafNode,
                       ft F,lt Z,zz *Status,       indextype    *KeyPos)
{
indextype        Index           = KeyIndex;
addrtype         KeyValue        = NullPtr;
indextype        Pos             = 0;
numtype          LowKey          = 0;
numtype          MidKey          = 0;
numtype          HighKey         = 0;
numtype          Span            = 0;
boolean          NotFound        = True;
statustype       ShellStatus     = Env_Normal;

  *KeyPos                       = 0;  

   /* Find Insertion position                                            */  
if (Tree_GetFrozenKey      (SetHead,    &Index,   McStat,  &KeyValue))
if (LeafNode->NumKeys  > 2)
{
      Pos            = LeafNode->NumKeys - 1;
      Tree_CompareKey (SetHead,                      KeyValue, 
                       LeafNode->NodeKeys[Pos],
                       LeafNode->NodeHandles[Pos],   McStat);

      if (Normal(*Status))
      {
         *KeyPos     = Pos;
          NotFound   = False;

      } else if (*Status    == Set_GtrThen)  {
         *KeyPos     = Pos + 1;
          NotFound   = False;

      } else if (*Status    == Set_LessThen)  {

         *Status         = Env_Normal;
          Tree_CompareKey (SetHead,                         KeyValue, 
                           LeafNode->NodeKeys[LowKey],
                           LeafNode->NodeHandles[LowKey],   McStat);

          if (Normal(*Status))
          {
             *KeyPos     = LowKey;
              NotFound   = False;

          } else if (*Status    == Set_LessThen) {
             *KeyPos     = LowKey;
              NotFound   = False;

          } else if (*Status    == Set_GtrThen) {
             (*KeyPos)++;
              NotFound   = True;
          } else {
              NotFound   = False;

          }
      }
}

if (NotFound)
{
  *Status   = Env_Normal;
   if (LeafNode->NumKeys  > 9999)
   {
      HighKey         = LeafNode->NodeKeys[Pos];
      Span            = (HighKey - LowKey) / 2;
      MidKey          = LowKey + Span;
      if (Span % 2)
         ++MidKey;

      while (NotFound)
      {
        *Status    = Env_Normal;
        *KeyPos       = MidKey;
         Tree_CompareKey (SetHead,                         KeyValue, 
                          LeafNode->NodeKeys[MidKey],
                          LeafNode->NodeHandles[MidKey],   McStat);

         if (Normal(*Status))
            NotFound     = False;

         else 
         {
            if (*Status  == Set_GtrThen)
            {
               LowKey    = MidKey;
              (*KeyPos)++;

            } else
               HighKey   = MidKey;

            if (HighKey - LowKey > 1)
            {
               Span      = (HighKey - LowKey) / 2;
               MidKey    =  LowKey  + Span;
               if (Span  % 2)
                 ++MidKey;
            } else
               NotFound  = False;
         }
      }
   } else {

      while (Normal(*Status)
      &&    *KeyPos < LeafNode->NumKeys)
      {
          Tree_CompareKey     (SetHead,                        KeyValue, 
                               LeafNode->NodeKeys[*KeyPos],
                               LeafNode->NodeHandles[*KeyPos], McStat);

          if (*Status == Set_GtrThen)
          {
            *Status = Env_Normal;
             (*KeyPos)++;
          }
      }
   }
}
   if (*Status == Set_LessThen)
       *Status = Env_Normal;

   if (SetBug)
   if (sprintf (Msg, "    Insert at Key Pos=%3u; Status = %2u\n",
                *KeyPos, *Status))
      TraceMsg (0, Msg);

   ShellStatus = Env_Normal;
   Tree_DefrostKey     (SetHead,      KeyIndex,     ShellMcStat);

TRACK(TrackBak,"Tree_GetNodeInsertPos\n");
return (STAT);
}


boolean    Tree_CompareKey
                      (ltreesethead *SetHead,      addrtype      CompValue,
                       indextype     KeyIndex,     handletype    ObjHandle,
                       ft F,lt Z,zz *Status)
{
typetype     KeyType         = SetHead->KeyType;
sizetype     KeySize         = SetHead->KeySize;
unsigned char      
             Value[MAXSTRING];
addrtype     ValuePtr        =  (addrtype )&Value[0];
int          i               = 0;
bytetype    *ByteValue       = NULL;
idtype      *IdValue         = NULL;
inttype     *IntValue        = NULL;
tokentype   *TknValue        = NULL;
tokentype   *CompTkn         = NULL;
float       *FloatValue      = NULL;
double      *DoubleValue     = NULL;
addrtype    *AddrValue       = NULL;
handletype  *HndlValue       = NULL;
tokentype    TreeTkn         = NullToken;

  memset (Value, NULL, MAXSTRING);

  TreeTkn.DbId      = SetHead->TreeDbId;
  TreeTkn.Handle    = 1;

  if (SetHead->KeySize <= sizeof (indextype))
     *(indextype *)ValuePtr   =  KeyIndex;

  else
     Grp_GetEntry   (&TreeTkn,  SetHead->TreeKeysDir, KeyIndex,
                     McStat,                          ValuePtr);

  if (Normal(*Status))
  switch (KeyType)
  {

      case aDbmsString          :
                 *Status = Err_NotImplemented;
                  break;

      case aString     :
                  i = Ut_CompareString ((char *)CompValue, (char *)ValuePtr);

                  if (DeBug || SetBug || OaBug)
/*
//                || !strcmp((char *)CompValue, "512")
//                || !strcmp((char *)ValuePtr,  "512"))
*/
                  {
                     sprintf (Msg, 
                     " Tree_CompareKey:: <%-16s> To <%-16s>; i = %d\n", 
                              CompValue, ValuePtr, i);
                     TraceMsg (0,  Msg);
                  }
                  break;

      case aLink       :
                  if (KeySize == sizeof (tokentype ))
                  {
                     TknValue    = (tokentype *)ValuePtr;
                     CompTkn     = (tokentype *)CompValue;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;

                  } else {
                     HndlValue   = (handletype *)ValuePtr;
                     if (*(handletype *)CompValue < *HndlValue)
                       i = -1;
                     else if (*(handletype *)CompValue > *HndlValue)
                       i = 1;

                  }
                  break;

      case aReal       :
                  if (KeySize == sizeof (float)) 
                  {
                     FloatValue    = (float *)ValuePtr;
                     if (*(float *)CompValue < *FloatValue)
                       i = -1;
                     else if (*(float *)CompValue > *FloatValue)
                       i = 1;

                  } else if (KeySize == sizeof (double)) {
                     DoubleValue    = (double *)ValuePtr;
                     if (*(double *)CompValue < *DoubleValue)
                       i = -1;
                     else if (*(double *)CompValue > *DoubleValue)
                       i = 1;

                  }
                  break;

      case aAddr       :
                  AddrValue    = (addrtype *)ValuePtr;
                  if (*(addrtype *)CompValue != AddrValue)
                       i = 1;
                  break;


      case aEnum       :
      case aBoolean    :
      case aValue      :
      case aChunk      :
      case aInteger    :
          
         switch (KeySize)
         {
            case 1     :
                     ByteValue    = (bytetype *)ValuePtr;
                     if (*(bytetype *)CompValue < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)CompValue > *ByteValue)
                       i = 1;
                     break;

            case 2     :
                     IdValue    = (idtype *)ValuePtr;
                     if (*(idtype *)CompValue < *IdValue)
                       i = -1;
                     else if (*(idtype *)CompValue > *IdValue)
                       i = 1;
                     break;

            case 4     :
                     IntValue    = (inttype *)ValuePtr;
                     if (*(inttype *)CompValue < *IntValue)
                       i = -1;
                     else if (*(inttype *)CompValue > *IntValue)
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

            default    :
                     if (KeySize <= sizeof (tokentype ))
                         i = memcmp  ((char *)CompValue, 
                                      (char *)ValuePtr, (size_t )KeySize);
                     else 
                         i = strncmp ((char *)CompValue, 
                                       (char *)ValuePtr, (size_t )KeySize);
         }
         break;
  }

  if (DeBug || SetBug || OaBug)
  {
     if (KeyType != aString)
     if (KeySize <= sizeof (tokentype ))
     {
/*
//      sprintf    (Msg, "     TreeCompareKey Db[%2u] Grp[%4u]  Value=",
//                  SetHead->TreeDbId, SetHead->TreeKeysDir);
//      TraceMsg   (0,   Msg);
*/
        TraceMsg   (0, "                   Compare :: Value=");
        TraceBytes (0,   ValuePtr, KeySize);
        sprintf    (Msg, "; i = %d\n",       i);
        TraceMsg   (0, Msg);

     } else {
        sprintf (Msg, 
                 "                    Compare :: Value=<%12s>; i = %d\n", 
                 Value, i);
        TraceMsg (0,  Msg);

     }
  }

  if (Normal (*Status))
  if (i == 0)
  if (SetHead->Key2ary)
  {
     Tree_Compare2aryKey  (SetHead,  ObjHandle,  McStat);
     if (Normal(*Status))
       i = 0;
  }

  if (Normal (*Status))
  {
    if (!i)
    {
       if (DeBug || OaBug)
       if (sprintf (Msg, 
         "     COMPARED on Key  Index =%6u; Primary KeyValue = ", KeyIndex))
       if (TraceBytes (0, ValuePtr, KeySize))
          TraceMsg (0, "\n");

    } else if (i < 0) {
      *Status = Set_LessThen;

    } else {
      *Status = Set_GtrThen;
    }
  }


TRACK(TrackBak,"Tree_CompareKey\n");
return (STAT);
}
 
/* ------------------------------------------------------------------------ */
/*                           END TREE00.C                                   */
/* ------------------------------------------------------------------------ */
