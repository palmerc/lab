/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* Set- TREE0.c*/        
#define  TREE0
#define  TREE00
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "env0.h"
#include "mem010.h"
#include "vchunk.h"
#include "grp.h"
#include "tree00.h"

extern boolean Dbm_CommitDb 
                         (tokentype    *DbToken,   boolean       RollOut,
                          ft F,lt Z,zz *Status);

       boolean Test_Iterate 
                         (tokentype    *Token,     indextype     IterNum,
                          ft F,lt Z,zz *Status);

       boolean Tree_Test (tokentype    *Anchor,    numtype       Option,
                          ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~SPCL1                    SPECIAL SETS                                    !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                   ....       TREE      ....
   .......................................................................*/

/* ------------------------------------------------------------------------ */
boolean    Tree_DeleteFrom
                      (ltreesethead  *SetHead,     addrtype      KeyValue,
                       ft F,lt Z,zz  *Status,      handletype   *Handle)
{
lbtreenode      *DeleteNode      = NullPtr;
lbtreenode      *SuccNode        = NullPtr;
indextype        SuccIndex       = 0;
indextype        KeyPos          = 0;
int              Pos             = 0;
indextype        DeleteKeyPos    = 0;
tokentype        TreeTkn         = NullToken;
numtype          IterNum  = 0;
boolean          TempSetBug  = False;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;


  if (SetBug)
  if (sprintf (Msg, 
        "   Tree_DeleteEntry:: SetHead[%4u:%6u] for KeyValue @%6x;\n",
               SetHead->TreeDbId, SetHead->Token.Handle, KeyValue))
     TraceMsg (0, Msg);

  Tree_FindIn      (SetHead, KeyValue, McStat, &DeleteNode, &KeyPos);

  if (*Status == Set_Found)
  {
    *Status    = Env_Normal;
    *Handle    = DeleteNode->NodeHandles[KeyPos];

     DeleteNode->NodeHandles[KeyPos] = 0;
     DeleteKeyPos = 0;
     DeleteNode->NodeKeys[KeyPos] = 0;
  }
  if (GrpBug || SetBug)
  if (sprintf (Msg, "\n   Delete node[%4u:%6u] from tree::\n",
               SetHead->TreeDbId, DeleteNode->Index))
  if (TraceMsg  (0, Msg))
      Tree_DumpNode     (SetHead,    DeleteNode);

  if (Normal(*Status))
  if (Tree_FreezeNode   (SetHead,    DeleteNode->Index,    McStat))

  if (DeleteNode->NodeLeafs[0]      == 0)   /* Is this a leaf node?       */
  {
     /* Remove Key from LeafNode                                          */
     if (SetBug)
        TraceMsg (0, "  Remove Key from LeafNode\n");

   --DeleteNode->NumKeys;

     for (Pos = KeyPos;
          Pos < DeleteNode->NumKeys;
        ++Pos)
     {
        DeleteNode->NodeKeys[Pos]    = DeleteNode->NodeKeys[Pos+1];
        DeleteNode->NodeHandles[Pos] = DeleteNode->NodeHandles[Pos+1];
     }

     DeleteNode->NodeKeys[DeleteNode->NumKeys]    = 0;
     DeleteNode->NodeHandles[DeleteNode->NumKeys] = Null_Handle;

     if (GrpBug || SetBug)
     if (TraceMsg  (0, "\n   Delete node:: Revised\n"))
        Tree_DumpNode    (SetHead,  DeleteNode);

     /* adjust Tree if reguired                                           */
     if (DeleteNode->NumKeys < SetHead->MinKeys)
        Tree_Adjust      (SetHead,  DeleteNode,        McStat);
     else
        Tree_DefrostNode (SetHead,  DeleteNode->Index, McStat);

  } else {
     /* Delete Node is an internal node in the tree                       */
     /* Replace deleted key with immediate successor                      */
     if (SetBug)
     {
        TraceMsg (0, "   Delete Node is an internal node in the tree.\n");
        TraceMsg (0, "   Replace deleted key with immediate successor.\n");
     }

     SuccIndex              = DeleteNode->NodeLeafs[KeyPos+1];  
     if (Tree_GetFrozenNode (SetHead,  SuccIndex, McStat,  &SuccNode))  

     while (Normal(*Status)
     &&     SuccNode->NodeLeafs[0] != 0)
     {
        SuccIndex             = SuccNode->NodeLeafs[0];
        if (Tree_DefrostNode   (SetHead,  SuccNode->Index,     McStat))  
            Tree_GetFrozenNode (SetHead,  SuccIndex, McStat,  &SuccNode);  
     }
     if (SetBug)
     if (TraceMsg  (0, "\n   Successor node::\n"))
         Tree_DumpNode     (SetHead,    SuccNode);

     /* first key is the 'swappee'                                        */
     if (Normal(*Status))
     {
        DeleteNode->NodeKeys[KeyPos]      = SuccNode->NodeKeys[0];
        DeleteNode->NodeHandles[KeyPos]   = SuccNode->NodeHandles[0];

        /* deleted swapped key from successor page                        */
      --SuccNode->NumKeys;
     }

     if (Normal(*Status))
     for (Pos = 0;
          Pos < SuccNode->NumKeys;
        ++Pos)
     {
        SuccNode->NodeKeys[Pos]    = SuccNode->NodeKeys[Pos+1];
        SuccNode->NodeHandles[Pos] = SuccNode->NodeHandles[Pos+1];
        SuccNode->NodeLeafs[Pos+1] = SuccNode->NodeLeafs[Pos+2];
     }

     if (Normal(*Status))
     {
        SuccNode->NodeKeys[SuccNode->NumKeys]    = 0;
        SuccNode->NodeHandles[SuccNode->NumKeys] = Null_Handle;
        SuccNode->NodeLeafs[SuccNode->NumKeys+1] = 0;
     }

     if (SetBug)
     if (TraceMsg  (0, "\n   Successor node:: Revised\n"))
         Tree_DumpNode     (SetHead,    SuccNode);

     /* adjust Tree if required                                           */
     if (Normal(*Status))
     if (SuccNode->NumKeys < SetHead->MinKeys)
     {
        if (SetBug)
        {
           sprintf (Msg, "   SuccNode->NumKeys(%4u) < SetHead->MinKeys(%4u)\n", 
                    SuccNode->NumKeys,  SetHead->MinKeys);
           TraceMsg (0, Msg);
        }
        if (Normal(*Status))
        if (Tree_DefrostNode (SetHead,  DeleteNode->Index,   McStat))
            Tree_Adjust      (SetHead,  SuccNode,            McStat);

     } else {
        if (SetBug)
           TraceMsg (0, "   Free the Successor Node\n");

        /* Free the Successor Node                                        */
        Tree_DefrostNode (SetHead,  SuccNode->Index,   McStat);

        /* Free the Delete Node                                           */
        if (Normal(*Status))
        Tree_DefrostNode (SetHead,  DeleteNode->Index, McStat);
     }
  }

#ifdef TREE_BUG
   if (Normal(*Status))
   if (SetBug == False)
   {
       IterNum  = 0;
       Tree_Validate  (SetHead,  SuccIndex,  &IterNum,  McStat);
       SetBug = TempSetBug;
   }

   if (Normal(*Status))
   if (SetHead->TreeNodesDir)
   if (Grp_FrostStatus      (&TreeTkn, SetHead->TreeNodesDir,
                             McStat)
   ==  False)
   if (TraceMsg (0, "\n  TreeNodesDir GRP is still Frosted!\n"))
      *Status    =  Tree_GRPisFrosted;

   if (Normal(*Status))
   if (SetHead->TreeKeysDir)
   if (Grp_FrostStatus      (&TreeTkn, SetHead->TreeKeysDir,
                             McStat)
   ==  False)
   if (TraceMsg (0, "\n  TreeKeysDir GRP is still Frosted!\n"))
      *Status    =  Tree_GRPisFrosted;
#endif


TRACK(TrackBak,"Tree_DeleteFrom\n");
return (STAT);
}


boolean    Tree_Adjust
                      (ltreesethead  *SetHead,     lbtreenode   *LeafNode,
                       ft F,lt Z,zz  *Status)
{
lbtreenode      *ParentNode      = NullPtr;
lbtreenode      *LessNode        = NullPtr;
lbtreenode      *GtrNode         = NullPtr;
int              Pos             = 0;
indextype        SibGtrKeys      = 0;
indextype        SibLessKeys     = 0;

  if (SetBug)
  if (sprintf (Msg, 
        "   Tree_Adjust:: SetHead[%4u:%6u] for LeafNode [%4u];\n",
               SetHead->TreeDbId, SetHead->Token.Handle, LeafNode->Index))
     TraceMsg (0, Msg);

  if (LeafNode->ParentNode  == 0)
  {
     if (SetBug)
       TraceMsg (0, "  LeafNode is the ROOT node\n");

     Tree_DefrostNode    (SetHead,  LeafNode->Index,  McStat);
     return (STAT);
  }

  /* Get parent node                                                      */
  if (Tree_GetFrozenNode (SetHead,  LeafNode->ParentNode,  
                          McStat,  &ParentNode))  

  /* Find pointer to leafnode in the parent node                          */
  for (Pos = 0;
       ParentNode->NodeLeafs[Pos] != LeafNode->Index
  &&   Pos                         < ParentNode->NumKeys+1;
     ++Pos)
  {
       ;
  }
  if (Pos >= ParentNode->NumKeys + 1)
  {
      if (ParentNode->NumKeys   == 0)
        *Status = Err_BadDesign;
      else {
        Tree_DefrostNode    (SetHead,  LeafNode->Index,       McStat);
        Tree_DefrostNode    (SetHead,  LeafNode->ParentNode,  McStat);
      }
      sprintf (Msg, 
    "***ERROR @ Tree_Adjust:: SetHead[%4u:%6u] for LeafNode [%4u]; Pos=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, 
               LeafNode->Index, Pos);
      SendMsg  (0, Msg);
      TraceMsg (0, "\n Dump of LeafNode\n");
      Tree_DumpNode (SetHead, LeafNode);
      TraceMsg (0, "\n Dump of ParentNode... LeafIndex not found.\n");
      Tree_DumpNode (SetHead, ParentNode);

     *Status = Err_BadDesign;
  }

  if (Normal(*Status))
  if (SetBug)
  {
      sprintf (Msg, "   LeafNode Ptr in Parent->NodeLeafs[%4u]; Keys=%4u\n", 
               Pos, ParentNode->NumKeys);
      TraceMsg  (0, Msg);
  }

  /* Read sibling nodes                                                   */
  if (Normal(*Status))
  if (Pos  < ParentNode->NumKeys)
  if (Tree_GetFrozenNode (SetHead,  ParentNode->NodeLeafs[Pos+1],  
                          McStat,  &GtrNode))
      SibGtrKeys  = GtrNode->NumKeys;

  if (Normal(*Status))
  if (Pos  > 0)
  if (Tree_GetFrozenNode (SetHead,  ParentNode->NodeLeafs[Pos-1],  
                          McStat,  &LessNode))
      SibLessKeys = LessNode->NumKeys;

  /* Decide to Redistribute or Concatenate                                */
  if (Normal(*Status))
  if (SibLessKeys  > SibGtrKeys)
  {
   --Pos;
     if (LessNode == NullPtr)
        *Status    = Err_BadDesign;

     if (GtrNode  != NullPtr)
        Tree_DefrostNode   (SetHead,   GtrNode->Index,  McStat);

     if (SetBug)
     if (TraceMsg  (0, "\n   Less node::\n"))
         Tree_DumpNode     (SetHead,   LessNode);

     if (Normal(*Status))
     if (LessNode->NumKeys  >= SetHead->MinKeys)
        Tree_Redistribute  (SetHead,   Pos,  LessNode,  ParentNode, LeafNode,
                            McStat);
     else
        Tree_Concatenate   (SetHead,   Pos,  LessNode,  ParentNode, LeafNode,
                            McStat);
  } else {

     if (GtrNode  == NullPtr)
        *Status    = Err_BadDesign;

     if (LessNode != NullPtr)
        Tree_DefrostNode   (SetHead,   LessNode->Index, McStat);

     if (SetBug)
     if (TraceMsg  (0, "\n   Gtr node::\n"))
         Tree_DumpNode     (SetHead,   GtrNode);

     if (Normal(*Status))
     if (GtrNode->NumKeys  >= SetHead->MinKeys)
        Tree_Redistribute  (SetHead,   Pos,  LeafNode,  ParentNode, GtrNode,
                            McStat);
     else
        Tree_Concatenate   (SetHead,   Pos,  LeafNode,  ParentNode, GtrNode,
                            McStat);
  }

TRACK(TrackBak,"Tree_Adjust\n");
return (STAT);
}


boolean    Tree_Redistribute
                      (ltreesethead *SetHead,     indextype     KeyPos,
                       lbtreenode   *LessNode,    lbtreenode   *ParentNode,  
                       lbtreenode   *GtrNode,     ft F,lt Z,zz *Status)
{
lbtreenode      *ChildNode       = NullPtr;
int              Pos             = 0;

  /* note: this function is ONLY called for leaf nodes!                   */
  if (SetBug)
  {
     sprintf (Msg, "   Tree_Redistribute:: SetHead [%4u:%6u] Pos=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, KeyPos);
     TraceMsg (0, Msg);

     sprintf (Msg, 
     "                       LessNode[%4u]; Parent[%4u]; GtrNode[%4u]\n",
                  LessNode->Index, ParentNode->Index, GtrNode->Index);
     TraceMsg (0, Msg);

  }

  if (LessNode->NodeLeafs[0] == 0)
  {
     /* Working with leaves                                               */
     if (LessNode->NumKeys > GtrNode->NumKeys)
     {
        /* slide a key from lesser to greater                             */
        /* move keys in greater to the left by one                        */
        for (Pos = GtrNode->NumKeys;
             Pos > 0;
           --Pos)
        {
           GtrNode->NodeKeys[Pos]    = GtrNode->NodeKeys[Pos-1];
           GtrNode->NodeHandles[Pos] = GtrNode->NodeHandles[Pos-1];
        }

        /* store parent separator key in greater page                     */
        GtrNode->NodeKeys[0]    = ParentNode->NodeKeys[KeyPos];
        GtrNode->NodeHandles[0] = ParentNode->NodeHandles[KeyPos];

        /* increment greater page's key count                             */
      ++GtrNode->NumKeys;

        /* decrement lessor page's key count                              */
      --LessNode->NumKeys;

        /* move last key in less page to parent as separator              */
        ParentNode->NodeKeys[KeyPos]  
                                 = LessNode->NodeKeys[LessNode->NumKeys];
        ParentNode->NodeHandles[KeyPos] 
                                 = LessNode->NodeHandles[LessNode->NumKeys];

        /* clear last key in less page                                    */
        LessNode->NodeKeys[LessNode->NumKeys]     = 0;
        LessNode->NodeHandles[LessNode->NumKeys]  = Null_Handle;

     } else {
        /* slide a key from greater to lessor                             */
        /* add parent key to lessor page                                  */
        LessNode->NodeKeys[LessNode->NumKeys]
                                           = ParentNode->NodeKeys[KeyPos];
        LessNode->NodeHandles[LessNode->NumKeys] 
                                           = ParentNode->NodeHandles[KeyPos];

        /* increment lessor page's key count                              */
      ++LessNode->NumKeys;
    
        /* insert in parent the lowest key in greater page                */
        ParentNode->NodeKeys[KeyPos]    = GtrNode->NodeKeys[0];
        ParentNode->NodeHandles[KeyPos] = GtrNode->NodeHandles[0];
    
        /* decrement # of keys in greater page                            */
      --GtrNode->NumKeys;
    
        /* move keys in greater page to left                              */
        for (Pos = 0;
             Pos < GtrNode->NumKeys;
           ++Pos)
        {
           GtrNode->NodeKeys[Pos]    = GtrNode->NodeKeys[Pos+1];
           GtrNode->NodeHandles[Pos] = GtrNode->NodeHandles[Pos+1];
        }

        /* make last key blank                                            */
        GtrNode->NodeKeys[Pos]    = 0;
        GtrNode->NodeHandles[Pos] = Null_Handle;
     }

  } else {
     if (LessNode->NumKeys > GtrNode->NumKeys)
     {
        /* slide a key from lesser to greater                             */
        /* move keys in greater to the left by one                        */
        for (Pos = GtrNode->NumKeys;
             Pos > 0;
           --Pos)
        {
           GtrNode->NodeKeys[Pos]    = GtrNode->NodeKeys[Pos-1];
           GtrNode->NodeHandles[Pos] = GtrNode->NodeHandles[Pos-1];
           GtrNode->NodeLeafs[Pos+1] = GtrNode->NodeLeafs[Pos];
        }
        GtrNode->NodeLeafs[1]        = GtrNode->NodeLeafs[0];

        /* store parent separator key in greater page                     */
        GtrNode->NodeKeys[0]    = ParentNode->NodeKeys[KeyPos];
        GtrNode->NodeHandles[0] = ParentNode->NodeHandles[KeyPos];
        GtrNode->NodeLeafs[0]   = LessNode->NodeLeafs[LessNode->NumKeys];

        /* update child link                                              */
        if (Tree_GetFrozenNode (SetHead,  GtrNode->NodeLeafs[0],  
                                McStat,  &ChildNode))
        {
            ChildNode->ParentNode        = GtrNode->Index;
            if (SetBug)
            {
              sprintf (Msg, "    ChildNode[%4u] gets new ParentNode= %4u\n",
                       ChildNode->Index, GtrNode->Index);
              TraceMsg  (0, Msg);
            }
            Tree_DefrostNode   (SetHead,  ChildNode->Index,     McStat);
        }
    
        /* increment greater page's key count                             */
      ++GtrNode->NumKeys;

        /* decrement lessor page's key count                              */
      --LessNode->NumKeys;

        /* move last key in less page to parent as separator              */
        ParentNode->NodeKeys[KeyPos]  
                                 = LessNode->NodeKeys[LessNode->NumKeys];
        ParentNode->NodeHandles[KeyPos]
                                 = LessNode->NodeHandles[LessNode->NumKeys];

        /* clear last key in less page                                    */
        LessNode->NodeKeys[LessNode->NumKeys]     = 0;
        LessNode->NodeHandles[LessNode->NumKeys]  = Null_Handle;
        LessNode->NodeLeafs[LessNode->NumKeys + 1]= 0;

     } else {
        /* slide a key from greater to lessor                             */
        /* add parent key to lessor page                                  */
        LessNode->NodeKeys[LessNode->NumKeys]
                                           = ParentNode->NodeKeys[KeyPos];
        LessNode->NodeHandles[LessNode->NumKeys] 
                                           = ParentNode->NodeHandles[KeyPos];
        LessNode->NodeLeafs[LessNode->NumKeys + 1] 
                                           = GtrNode->NodeLeafs[0];

        /* update child link                                              */
        if (Tree_GetFrozenNode (SetHead,  GtrNode->NodeLeafs[0],  
                                McStat,  &ChildNode))
        {
            ChildNode->ParentNode        = LessNode->Index;
            if (SetBug)
            {
              sprintf (Msg, "    ChildNode[%4u] gets new ParentNode= %4u\n",
                       ChildNode->Index, LessNode->Index);
              TraceMsg  (0, Msg);
            }

            Tree_DefrostNode   (SetHead,  ChildNode->Index,     McStat);
        }

        /* increment lessor page's key count                              */
      ++LessNode->NumKeys;
    
        /* insert in parent the lowest key in greater page                */
        ParentNode->NodeKeys[KeyPos]    = GtrNode->NodeKeys[0];
        ParentNode->NodeHandles[KeyPos] = GtrNode->NodeHandles[0];
    
        /* decrement # of keys in greater page                            */
      --GtrNode->NumKeys;
    
        /* move keys in greater page to left                              */
        for (Pos = 0;
             Pos < GtrNode->NumKeys;
           ++Pos)
        {
           GtrNode->NodeKeys[Pos]    = GtrNode->NodeKeys[Pos+1];
           GtrNode->NodeHandles[Pos] = GtrNode->NodeHandles[Pos+1];
           GtrNode->NodeLeafs[Pos]   = GtrNode->NodeLeafs[Pos+1];
        }
        GtrNode->NodeLeafs[Pos]   = GtrNode->NodeLeafs[Pos+1];

        /* make last key blank                                            */
        GtrNode->NodeKeys[Pos]    = 0;
        GtrNode->NodeHandles[Pos] = Null_Handle;
        GtrNode->NodeLeafs[Pos+1] = 0;
     }
  }
  if (SetBug)
  {
     TraceMsg  (0, "\n After Redistribution::\n");
     TraceMsg  (0, " Less node::\n");
     Tree_DumpNode     (SetHead,   LessNode);
     TraceMsg  (0, " Parent node::\n");
     Tree_DumpNode     (SetHead,   ParentNode);
     TraceMsg  (0, " Gtr node::\n");
     Tree_DumpNode     (SetHead,   GtrNode);
  }

  if (Normal(*Status))
  if (Tree_DefrostNode   (SetHead,  LessNode->Index,     McStat))
  if (Tree_DefrostNode   (SetHead,  GtrNode->Index,      McStat))
      Tree_DefrostNode   (SetHead,  ParentNode->Index,   McStat);

TRACK(TrackBak,"Tree_Redestribute\n");
return (STAT);
}


boolean    Tree_Concatenate
                      (ltreesethead *SetHead,     indextype     KeyPos,
                       lbtreenode   *LessNode,    lbtreenode   *ParentNode,  
                       lbtreenode   *GtrNode,     ft F,lt Z,zz *Status)
{
lbtreenode      *ChildNode       = NullPtr;
sizetype         Pos             = 0;
sizetype         LessPos         = 0;
sizetype         GtrPos          = 0;

  if (SetBug)
  {
     sprintf (Msg, "   Tree_Concatenate :: SetHead [%4u:%6u] Pos=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, KeyPos);
     TraceMsg (0, Msg);

     sprintf (Msg, 
     "                       LessNode[%4u]; Parent[%4u]; GtrNode[%4u]\n",
                  LessNode->Index, ParentNode->Index, GtrNode->Index);
     TraceMsg (0, Msg);
  }

  /* Move seperator key from parent node into less node                   */
  LessNode->NodeKeys[LessNode->NumKeys]    = ParentNode->NodeKeys[KeyPos];
  LessNode->NodeHandles[LessNode->NumKeys] = ParentNode->NodeHandles[KeyPos];
  LessNode->NodeLeafs[LessNode->NumKeys +1]= GtrNode->NodeLeafs[0];

++LessNode->NumKeys;

  /* Delete seperator from parent node                                    */
--ParentNode->NumKeys;

  for (Pos = KeyPos;
       Pos < ParentNode->NumKeys;
     ++Pos)
  {
     ParentNode->NodeKeys[Pos]      = ParentNode->NodeKeys[Pos + 1];
     ParentNode->NodeHandles[Pos]   = ParentNode->NodeHandles[Pos + 1];
     ParentNode->NodeLeafs[Pos + 1] = ParentNode->NodeLeafs[Pos + 2];
  }

  /* clear unused key in parent                                           */
  ParentNode->NodeKeys[Pos]         = 0;
  ParentNode->NodeHandles[Pos]      = Null_Handle;
  ParentNode->NodeLeafs[Pos + 1]    = 0;

  /* Copy keys from Gtr node to Less node                                 */
  GtrPos   = 0;
  LessPos  = LessNode->NumKeys;

  while (GtrPos  < GtrNode->NumKeys)
  {
   ++LessNode->NumKeys;

     LessNode->NodeKeys[LessPos]      = GtrNode->NodeKeys[GtrPos];
     LessNode->NodeHandles[LessPos]   = GtrNode->NodeHandles[GtrPos];
     LessNode->NodeLeafs[LessPos + 1] = GtrNode->NodeLeafs[GtrPos + 1];

   ++LessPos;
   ++GtrPos;
  }

  /* Delete Gtr node                                                       */
  if (Tree_DefrostNode (SetHead,  GtrNode->Index,  McStat))
  if (Tree_DeleteNode  (SetHead,  GtrNode->Index,  McStat))

  /* Is this a leaf node                                                   */
  if (LessNode->NodeLeafs[0] != 0)
  {
     if (SetBug)
        TraceMsg (0, "   This a Leaf Node:  adjust child ptrs\n");

     /* adjust child pointers to point to Less node                        */
     for (Pos  = 0;
          Pos <= LessNode->NumKeys  && Normal(*Status);
        ++Pos)
     {
        if (Tree_GetFrozenNode (SetHead,  LessNode->NodeLeafs[Pos],  
                                McStat,  &ChildNode))
        {
            ChildNode->ParentNode        = LessNode->Index;
            if (SetBug)
            {
              sprintf (Msg, "    ChildNode[%4u] gets new ParentNode= %4u\n",
                       ChildNode->Index, LessNode->Index);
              TraceMsg  (0, Msg);
            }
            Tree_DefrostNode   (SetHead,  ChildNode->Index,     McStat);
        }
     }
  }

  /* Save Less node and Parent node                                        */
  if (Normal(*Status))
  if (ParentNode->NumKeys == 0)
  {
     /* note:: Only the root node can ever be deleted to zero keys         */
     if (Tree_DefrostNode (SetHead,  ParentNode->Index,  McStat))
     if (Tree_DeleteNode  (SetHead,  ParentNode->Index,  McStat))
     {
         LessNode->ParentNode     = 0;
         SetHead->TreeRootNode    = LessNode->Index;
         Tree_DefrostNode (SetHead, LessNode->Index,    McStat);
     }

     if (SetBug)
     {
        TraceMsg  (0, "\n After Concatenation::\n");
        TraceMsg  (0, " Less node::\n");
        Tree_DumpNode     (SetHead,   LessNode);
        TraceMsg  (0, " Parent node was deleted::\n");
        TraceMsg  (0, " Gtr node was deleted::\n");
        sprintf (Msg, "  SetHead->TreeRootNode =%4u\n", LessNode->Index);
        TraceMsg  (0, Msg);
     }

  } else {
     if (Tree_DefrostNode (SetHead,  LessNode->Index,    McStat))
     if (ParentNode->ParentNode  == 0)
         SetHead->TreeRootNode    = ParentNode->Index;

     if (SetBug)
     {
        TraceMsg  (0, "\n After Concatenation::\n");
        TraceMsg  (0, " Less node::\n");
        Tree_DumpNode     (SetHead,   LessNode);
        TraceMsg  (0, " Parent node::\n");
        Tree_DumpNode     (SetHead,   ParentNode);
        TraceMsg  (0, " Gtr node was deleted::\n");
     }

     /* if Parent node is too small, adjust the tree                       */
     if (ParentNode->NumKeys  < SetHead->MinKeys)
         Tree_Adjust      (SetHead,  ParentNode,         McStat);
     else
         Tree_DefrostNode (SetHead,  ParentNode->Index,  McStat);
  }


TRACK(TrackBak,"Tree_Concatenate\n");
return (STAT);
}


boolean    Tree_DeleteNode
                      (ltreesethead  *SetHead,     indextype     Index,
                       ft F,lt Z,zz  *Status)
{
tokentype   TreeTkn    = NullToken;

  TreeTkn.DbId    = SetHead->TreeDbId;
  TreeTkn.Handle  = 1;

  if (SetBug)
  {
     sprintf (Msg, 
     "   Tree_DeleteNode :: SetHead[%4u:%6u] for DeleteNode->Index=%4u\n",
               SetHead->TreeDbId, SetHead->Token.Handle, Index);
     TraceMsg (0, Msg);
  }

  if (Grp_DeleteEntry  (&TreeTkn,    SetHead->TreeNodesDir,   Index,
                        McStat))
   --SetHead->NodeCount;

TRACK(TrackBak,"Tree_DeleteNode\n");
return (STAT);
}

/* ------------------------------------------------------------------------ */
/*                           END TREE0.C                                    */
/* ------------------------------------------------------------------------ */

boolean  Test_Iterate    (tokentype    *Token,   indextype       IterNum,
                          ft F,lt Z,zz *Status)
{
numtype             j      = IterNum;

       if (j == 0 || j % 1000 == 0)
       {
          sprintf (Msg, " Object %4d. Iterate [%3u:%4u]\n",
                   IterNum, Token->DbId, Token->Handle);
          TraceMsg (0, Msg);
       }

TRACK(TrackBak,"Test_Iterate\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF TREE_TEST                               !
  +-----------------------------------------------------------------------+*/
