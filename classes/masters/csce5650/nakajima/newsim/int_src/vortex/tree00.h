/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/*                              SPCL.H                                      */
#ifndef TREE00_H
#define TREE00_H

typedef enum   TypeTreeOp
               {aTreeAddInto,  aTreeFindIn, aTreeDeleteFrom,
                aTreeTraverse, aTreeDelete, aNonTreeOp=EXT_ENUM } treeoptype;

#ifdef TREE00
#ifndef TREE0
        tokentype   AddInto_MemberTkn  = Null_Token;
        addrtype    FindIn_2aryKey     = NullPtr;
        treeoptype  Tree_OpType        = aNonTreeOp;
#else
  extern  tokentype   AddInto_MemberTkn;
  extern  addrtype    FindIn_2aryKey;
  extern  treeoptype  Tree_OpType;
#endif  /* TREE0 */

#else
extern  tokentype   AddInto_MemberTkn;
extern  addrtype    FindIn_2aryKey;
extern  treeoptype  Tree_OpType;
#endif /* TREE00 */
/*+-----------------------------------------------------------------------+
                        SPCL    Typedefs                                  !
  +-----------------------------------------------------------------------+*/
typedef struct bTreeSetHeadType
{
/* private:                                                                */
   addrtype       This;           
   tokentype      Token;          

   numtype        PrimaryKey;     
   numtype        Key2ary;        

   handletype     BuildDesc;
   handletype     lOwnerObject;

   sizetype       KeySize;        
   typetype       KeyType;        

   idtype         TreeDbId;       
   numtype        TreeOrder;      
   numtype        TreeNodesDir;   
   indextype      TreeRootNode;   
   sizetype       NodeSize;       
   numtype        NodeCount;      
   numtype        KeyCount;       

   numtype        TreeKeysDir;    
   numtype        MaxKeys;        
   numtype        MinKeys;        

   indextype     *TempKeys;       
   handletype    *TempHandles;    
   handletype    *TempLeafs;      

}             btreesethead;
typedef btreesethead   ltreesethead; 

typedef struct lBtreeNodeType
{
   indextype      Index;          
   indextype      ParentNode;     
   numtype        NumKeys;        
   indextype     *NodeKeys;       
   handletype    *NodeHandles;    
   indextype     *NodeLeafs;      

}             lbtreenode;
typedef lbtreenode     btreenode; 


typedef int  (*ptr_TreeCompare )  (addrtype    Value1, addrtype    Value2);
typedef ptr_TreeCompare  treecompfunc;

/* ------------------------------------------------------------------------*/
/*                        SPCL     Procedures                              */
/* ------------------------------------------------------------------------*/
/*    14.0  Tree                                                           */

       boolean    Tree_AddInto
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       handletype     Handle,      ft F,lt Z,zz  *Status);

       boolean    Tree_FindIn
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,
                       lbtreenode   **LeafNode,    indextype     *KeyPos);

       boolean    Tree_Traverse
                      (ltreesethead  *SetHead,     indextype      LeafIndex, 
                       numtype       *IterNum,     treeiterfunc   IterFunc,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_Validate
                      (ltreesethead  *SetHead,     indextype      LeafIndex, 
                       numtype       *IterNum,     ft F,lt Z,zz  *Status);

       boolean    Tree_IterateOn
                      (ltreesethead  *SetHead,      ft F,lt Z,zz  *Status,
                       indextype     *CurrLeaf,     indextype     *KeyPos,
                       numtype       *IterNum,      handletype    *Handle);


       boolean    Tree_Delete
                      (ft F,lt Z,zz *Status,       ltreesethead **SetHead);

       boolean    Tree_DeleteNode
                      (ltreesethead  *SetHead,     indextype     NodeIndex,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_DeleteFrom
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,      handletype    *Handle);

#ifndef __GUI__
       boolean    Tree_Compare2aryKey
                      (ltreesethead *SetHead,      handletype    ObjHandle,
                       ft F,lt Z,zz *Status);
#endif

#ifdef TREE00
       boolean    Tree_PromoteRootNode
                      (ltreesethead  *SetHead,
                       indextype      RootKey,     handletype     RootHandle,     
                       lbtreenode    *LessNode,    lbtreenode    *GtrNode,  
                       ft F,lt Z,zz  *Status);

       boolean    Tree_PromoteInternalNode
                      (ltreesethead  *SetHead,
                       lbtreenode    *LeafNode,    indextype      ParentIndex,
                       indextype      KeyIndex,    handletype     KeyHandle,     
                       ft F,lt Z,zz  *Status);

       boolean    Tree_RecurseSearch
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       indextype      Index,       ft F,lt Z,zz  *Status,
                       lbtreenode   **LeafNode,    indextype     *KeyPos);

       boolean    Tree_GetRecursePos
                      (ltreesethead *SetHead,      addrtype      KeyValue,
                       lbtreenode   *LeafNode,
                       ft F,lt Z,zz *Status,       indextype    *KeyPos);

       boolean    Tree_CreateNode
                      (ltreesethead  *SetHead,     ft F,lt Z,zz  *Status,
                       indextype     *Index,       lbtreenode   **TreeNode);

       boolean    Tree_GetFrozenNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status,      lbtreenode   **LeafNode);

       boolean    Tree_FreezeNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_DefrostNode
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_NodeIsFrosted
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status);

       void       Tree_DumpNode
                      (ltreesethead  *SetHead,     lbtreenode    *LeafNode);

       boolean    Tree_NewKey
                      (ltreesethead  *SetHead,     addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,      indextype     *Index);

       boolean    Tree_GetFrozenKey
                      (ltreesethead  *SetHead,     indextype     *Index,
                       ft F,lt Z,zz  *Status,      addrtype      *KeyValue);

       boolean    Tree_DefrostKey
                      (ltreesethead  *SetHead,     indextype      Index,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_GetNodeInsertPos
                      (ltreesethead *SetHead,      indextype     KeyIndex,
                       lbtreenode   *LeafNode,
                       ft F,lt Z,zz *Status,       indextype    *KeyPos);

       boolean    Tree_CompareKey
                      (ltreesethead *SetHead,      addrtype      CompValue,
                       indextype     KeyIndex,     handletype    ObjHandle,
                       ft F,lt Z,zz *Status);

       boolean    Tree_Adjust
                      (ltreesethead  *SetHead,     lbtreenode   *LeafNode,
                       ft F,lt Z,zz  *Status);

       boolean    Tree_Redistribute
                      (ltreesethead  *SetHead,     indextype     KeyPos,
                       lbtreenode    *LessNode,    lbtreenode   *ParentNode,  
                       lbtreenode    *GtrNode,     ft F,lt Z,zz *Status);

       boolean    Tree_Concatenate
                      (ltreesethead  *SetHead,     indextype     KeyPos,
                       lbtreenode    *LessNode,    lbtreenode   *ParentNode,  
                       lbtreenode    *GtrNode,     ft F,lt Z,zz *Status);

       boolean    Tree_Devoid
                      (ft F,lt Z,zz *Status,       ltreesethead **SetHead);

       boolean    Tree_DeleteByName
                      (tokentype    *Anchor,       nametype       TreeName,
                       ft F,lt Z,zz *Status);

       boolean    Tree_Test 
                      (tokentype     *Anchor,      numtype       Option,
                       ft F,lt Z,zz  *Status);
#endif

/* ------------------------------------------------------------------------*/
/*                                 END                                     */
/* ------------------------------------------------------------------------*/
#endif
