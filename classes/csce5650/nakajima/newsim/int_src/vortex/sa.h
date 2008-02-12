/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV-Sa.h */
#ifndef SA_H
#define SA_H
/*+-----------------------------------------------------------------------+
~V                          VARIABLES                                     !
  +-----------------------------------------------------------------------+*/
#ifdef SA_C
       tokentype  LastInIndexedSet = Null_Token;
#else
extern tokentype  LastInIndexedSet;
#endif

/*+-----------------------------------------------------------------------+
~P                             PROCEDURES                                 !
  +-----------------------------------------------------------------------+*/
       boolean SetInitSetHeads
                          (ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+*/
/*                         LOCAL KEYED SET                                 */
/*+-----------------------------------------------------------------------+*/
       boolean SaAddInto  (handletype    SetDesc,
                           tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                           ft F,lt Z,zz *Status,    tokentype    *NodeTkn);

       boolean SaFindIn   (handletype    SetDesc,   tokentype    *OwnerTkn,
                           addrtype      KeyValue,  ft F,lt Z,zz *Status,  
                           tokentype    *NodeTkn,   tokentype    *MemberTkn);

       boolean SaDeleteSetHead
                          (handletype    SetDesc,
                           tokentype    *HeadTkn,   ft F,lt Z,zz *Status);

       boolean SaDeleteNode
                          (handletype    SetDesc,   tokentype    *Member,
                           tokentype    *NodeTkn,   ft F,lt Z,zz *Status);

       boolean    SaAdjustRootNode 
                         (setdesctype   *SetDesc,    indextype      KeyIndex,
                          tokentype     *SetHeadTkn, lsetheaddesc  *SetHead,
                          handletype    *RootNodes,  indextype      StackPtr,
                          tokentype     *NextTkn,    ft F,lt Z,zz  *Status);

       boolean SaPutRootNode 
                          (tokentype    *SetHeadTkn, 
                           lsetheaddesc *SetHead,
                           tokentype    *SetDescTkn,addrtype      KeyValue,
                           tokentype    *SetNodeTkn,
                           ft F,lt Z,zz *Status);

       boolean SaGetRootNode
                          (tokentype    *SetHeadTkn, 
                           lsetheaddesc *SetHead,
                           tokentype    *SetDescTkn, 
                           addrtype      KeyValue,
                           ft F,lt Z,zz *Status,    tokentype    *RootTkn,
                           tokentype    *VchunkTkn);


/*+-----------------------------------------------------------------------+
~P                         END OF SA.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
