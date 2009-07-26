/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV-LIST01.h */

#ifndef LIST01_H
#define LIST01_H
/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/

typedef struct DbListHeadDesc
{
   addrtype      That;
   handletype    Handle;
   idtype        DbId;
   idtype        CoreDbId;

   nametype      ListName;
   tokentype     OwnerTkn;

   handletype    FirstNode;
   handletype    LastNode;
   handletype    CurrentNode;

   numtype       NodeCount;

   numtype       NodeClass;
}              listheadtype;

typedef struct DbListNodeDesc
{
   handletype    Handle;
   handletype    PrevNode;
   handletype    NextNode;
   tokentype     NodeObject;
}              listnodetype;

/*+-----------------------------------------------------------------------+
  !                 ....    TOKEN  LIST     ....                          !
  +-----------------------------------------------------------------------+*/


#ifdef LIST01_C
       numtype  DbListHead_Class = 0;
       numtype  DbListNode_Class = 0;
#else
extern numtype  DbListHead_Class;
extern numtype  DbListNode_Class;
#endif

/* .......................................................................
                    ....      system        ....
   .......................................................................*/
/* ~   14.0.0                                                             */
       boolean List01_Init  
                         (ft F,lt Z,zz *Status);


/* .......................................................................
                    ....      general       ....
   .......................................................................*/
/* ~   14.1.1.0                                                           */
       boolean List01_Create
                         (tokentype    *Anchor,    tokentype    *Owner,
                          char         *NameOfList,
                          numtype       CompCode,  numtype       WithinCode,
                          ft F,lt Z,zz *Status,    tokentype    *ListTkn);


/* ~   14.1.1.2                                                           */
       boolean List01_FindListHead
                         (tokentype    *Anchor,    char         *NameOfList,
                          ft F,lt Z,zz *Status,    tokentype    *ListTkn);

/*     14.1.2                                                             */
       boolean List01_CreateNode
                         (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn);

/* ~   14.1.3                                                             */
       boolean Is_List01Head
                         (tokentype    *Token,     ft F,lt Z,zz *Status);

/* ~   14.1.4                                                             */
       boolean Is_List01Node
                         (tokentype    *Token,     ft F,lt Z,zz *Status);

/* ~   14.1.5                                                             */
       boolean Is_List01Member
                         (tokentype    *ListTkn,   tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn);

/* ~   14.1.6                                                             */
       boolean List01_GetCount  
                         (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,    numtype      *ItemCount);

/* ~   14.1.7                                                             */
       boolean List01_Reset
                         (tokentype    *ListTkn,   ft F,lt Z,zz *Status);

/* ~   14.1.8                                                             */
       boolean List01_SetCurrent
                         (tokentype    *ListTkn,   tokentype    *NodeTkn,
                          ft F,lt Z,zz *Status);

/* ~   14.1.9                                                             */
       boolean List01_GetCurrent
                         (tokentype    *ListTkn, 
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn);

/* ~   14.1.10                                                            */
       boolean List01_IsEmpty
                         (tokentype    *ListTkn,   ft F,lt Z,zz *Status);


/* .......................................................................
                    ....      INSERTION     ....
   .......................................................................*/

/* ~   14.2.3                                                             */
       boolean List01_Append
                         (tokentype    *ListTkn,   tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);


/* .......................................................................
                    ....     traverseal     ....
   .......................................................................*/
/* ~   14.4.1                                                             */
       boolean List01_FirstIn
                         (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn,
                          tokentype    *MemberTkn);

/* ~   14.4.2                                                             */
       boolean List01_NextIn
                         (tokentype    *ListTkn,   tokentype    *CurrTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn,
                          tokentype    *MemberTkn);


/* ~   14.4.5                                                             */
       boolean List01_IterateOn
                         (tokentype    *ListTkn,  
                          iteratetype   Method,    ft F,lt Z,zz *Status);



/*+-----------------------------------------------------------------------+
~M                          END OF LIST01.H                               !
  +-----------------------------------------------------------------------+*/
#endif
