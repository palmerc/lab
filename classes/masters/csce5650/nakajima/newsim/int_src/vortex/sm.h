/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV-Sm.h */
#ifndef SM_H
#define SM_H

/*+-----------------------------------------------------------------------+
~V                          VARIABLES                                     !
  +-----------------------------------------------------------------------+*/
#ifdef SM_C
       indextype  CurrVchunkIndex  = 0;
#else
extern indextype  CurrVchunkIndex;
#endif

/* set in mem00.h                                                         */
extern boolean    SetGetSwi;

#define Set_InitSet    SetInitSet

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
       boolean SetDescGet (numtype       SetId,
                           ft F,lt Z,zz *Status,    handletype   *SetDesc);

       boolean SetInitSet (handletype    SetDesc,   tokentype    *OwnerTkn,
                           numtype       AllocSize, numtype       Order,
                           ft F,lt Z,zz *Status);

       boolean SetAddInto (handletype    SetDesc,   tokentype    *OwnerTkn,
                           tokentype    *MemberTkn, ft F,lt Z,zz *Status);

       boolean SetOwnerOf (handletype    SetDesc,   tokentype    *MemberTkn,
                           ft F,lt Z,zz *Status,    tokentype    *OwnerTkn);

       boolean SetFindIn  (handletype    SetDesc,   tokentype    *OwnerTkn,
                           addrtype      KeyValue,
                           ft F,lt Z,zz *Status,    tokentype    *MemberTkn);

       boolean SetIterateOn
                          (handletype    SetDesc,   tokentype    *OwnerTkn,
                           iteratetype   Method,    ft F,lt Z,zz *Status);

       boolean SetDeleteFrom
                          (handletype    SetDesc,
                           tokentype    *Member,    ft F,lt Z,zz *Status);

       boolean SetDeleteSet
                          (handletype    SetDesc,
                           tokentype    *Owner,     ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+*/
/*                           GENERAL SET METHODS                           */
/*+-----------------------------------------------------------------------+*/
       boolean SetFirstOf (handletype    SetDesc,   tokentype    *OwnerTkn,
                           ft F,lt Z,zz *Status,    tokentype    *NodeTkn,
                           tokentype    *FirstTkn);

       boolean SetNextOf  (handletype    SetDesc,   tokentype    *CurrentNode,
                           ft F,lt Z,zz *Status,  
                           tokentype    *NodeTkn,   tokentype    *NextTkn);




/*+-----------------------------------------------------------------------+
~P                         END OF SM.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
