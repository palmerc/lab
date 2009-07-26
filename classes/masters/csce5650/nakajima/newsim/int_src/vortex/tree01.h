/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/*                              SPCL.H                                      */
#ifndef SPCL1_01
#define SPCL1_01

/*+-----------------------------------------------------------------------+
                        SPCL    Typedefs                                  !
  +-----------------------------------------------------------------------+*/

#ifdef SPCL1
       numtype    bTreeSetHead_Class = 0;
static numtype    TempKeys_Attr      = 0;
static numtype    TempHandles_Attr   = 0;
static numtype    TempLeafs_Attr     = 0;


#else
extern numtype    bTreeSetHead_Class;

#endif
/*
typedef int  (*ptr_TreeCompare )  (addrtype    Value1, addrtype    Value2);
typedef ptr_TreeCompare  treecompfunc;
*/
/* ------------------------------------------------------------------------*/
/*                        SPCL     Procedures                              */
/* ------------------------------------------------------------------------*/
       boolean    SpclAddIntoTree
                      (handletype    SetDescHndl,
                       tokentype    *OwnerTkn,     tokentype     *MemberTkn,
                       ft F,lt Z,zz *Status,       tokentype     *NodeTkn);

       boolean    SpclFindInTree
                      (handletype     SetDescHndl,
                       tokentype     *OwnerTkn,    addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,
                       tokentype     *NodeTkn,     tokentype     *MemberTkn);

       boolean    SpclFindIn2aryTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       addrtype       KeyValue,    addrtype       Key2aryVal,
                       ft F,lt Z,zz  *Status,
                       tokentype     *NodeTkn,     tokentype     *MemberTkn);

       boolean    SpclTraverseTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       treeiterfunc   IterFunc,    ft F,lt Z,zz  *Status);

       boolean    Spcl_TreeIterateOn
                      (numtype        SetId,       tokentype     *OwnerTkn,
                       ft F,lt Z,zz  *Status,      indextype     *KeyPos,
                       numtype       *ItNum,       tokentype     *MemberTkn,
                       addrtype      *SetHead,     indextype     *LeafNode);

       boolean    SpclDeleteFromTree
                      (handletype     SetDescHndl, 
                       tokentype     *MemberTkn,   ft F,lt Z,zz  *Status);

       boolean    SpclDeleteTree
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       ft F,lt Z,zz  *Status);


/*  LOCAL */
       boolean    Tree_Create
                      (handletype    SetDescHndl,  tokentype     *OwnerTkn, 
                       numtype       TreeOrder,    numtype        AllocKeys,
                       sizetype      KeySize,      typetype       KeyType,         
                       ft F,lt Z,zz *Status,       tokentype     *TreeTkn);

#ifdef TREE01
       boolean    Tree_Compare2aryKey
                      (ltreesethead *SetHead,      handletype    ObjHandle,
                       ft F,lt Z,zz *Status);
#endif

/* ------------------------------------------------------------------------*/
/*                                 END                                     */
/* ------------------------------------------------------------------------*/
#endif
