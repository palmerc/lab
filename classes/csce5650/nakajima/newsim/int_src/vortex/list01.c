/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C SHELL - LIST01.c */
#define LIST01_C

#include "defines.h"
#include "typedefs.h"
#include "envdefs.h"
#include "im.h"
#include "primal.h"
#include "list01.h"
extern void    Object_Dump    (tokentype    *Token);
 
extern boolean EnvFetchAttrSize 
                              (numtype       AttrNum,
                               ft F,lt Z,zz *Status,    sizetype  *AttrSize);

extern boolean TmIsValid     (tokentype *Token,   ft F,lt Z,zz *Status);

extern boolean KernelGetAttr  (tokentype *Token,       numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,   addrtype   Value);

extern boolean KernelIamA     (tokentype    *Token,    numtype    EnvObj,
                               ft F,lt Z,zz *Status);
extern boolean OaCreateObject (tokentype    *Anchor,   numtype    EnvObj,
                               ft F,lt Z,zz *Status,   tokentype *Token);
extern boolean OaGetObject    (tokentype *Token,
                               ft F,lt Z,zz *Status,   addrtype  *Object);
extern boolean OaCompare     (boolean   CppType,    tokentype  *Token,
                              addrtype  Object,     numtype   EnvAttrNum,
                              addrtype  CompValue,  ptr_cmp   CmpCode,
                              ft F,lt Z,zz *Status);

extern boolean OaUpdateObject (tokentype    *Token,    ft F,lt Z,zz *Status);

extern boolean OaDeleteObject (tokentype    *Token,    ft F,lt Z,zz *Status);


extern numtype Env_GetClassId (char         *ClassName,
                               ft F,lt Z,zz *Status,   numtype  *ClassId);


/*+-----------------------------------------------------------------------+
~LIST                          LIST01                                     !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....      system        ....
   .......................................................................*/

/*     14.0.0                                                             */
boolean   List01_Init    (ft F,lt Z,zz *Status)
{

   if (DbListNode_Class == 0)
   {
      if (Env_GetClassId   ("DbListHead",        McStat, &DbListHead_Class))
          Env_GetClassId   ("DbListNode",        McStat, &DbListNode_Class);

      if (sprintf (Msg, " LIST HEADS  loaded ... DbListHead_Class = %2u\n",
                   DbListHead_Class))
      if (TraceMsg (0, Msg))
      if (sprintf (Msg, "                        DbListNode_Class = %2u\n",
                   DbListNode_Class))
          TraceMsg (0, Msg);
   }

TRACK(TrackBak,"List01_Init\n");
return(STAT);
}



/* .......................................................................
                    ....      general       ....
   .......................................................................*/

/*     14.1.1                                                             */
boolean List01_Create    (tokentype    *Anchor,    tokentype    *Owner,
                          char         *NameOfList,
                          numtype       CompCode,  numtype       WithinCode,
                          ft F,lt Z,zz *Status,    tokentype    *ListTkn)
{
listheadtype *HeadObj         = NullPtr;

  if (OaCreateObject (Anchor,    DbListHead_Class,  McStat, ListTkn))
  if (OaGetObject    (ListTkn,   McStat,       (addrtype *)&HeadObj))
  {
     HeadObj->OwnerTkn         = *Owner;
     HeadObj->FirstNode        = NullHandle;
     HeadObj->LastNode         = NullHandle;
     HeadObj->CurrentNode      = NullHandle;
     HeadObj->NodeCount        = 0;
     HeadObj->NodeClass        = DbListNode_Class;
     strncpy (HeadObj->ListName, NameOfList,  sizeof (nametype));
  }

TRACK(TrackBak,"List01_Create\n");
return(STAT);
}




/*     14.1.1                                                             */
boolean List01_FindListHead
                         (tokentype    *Anchor,    char         *NameOfList,
                          ft F,lt Z,zz *Status,    tokentype    *ListTkn)
{
listheadtype *HeadObj    = NullPtr;
tokentype     ItorTkn    = NullToken;
boolean       FOUND      = False;

 *Status                 = Iterate_Begin;
  while (!FOUND
  &&     Primal_IterateOnClassObjects
                         (Anchor, DbListHead_Class, McStat,
                          &ItorTkn,   ListTkn))
  {
     if (OaGetObject    (ListTkn,   McStat,       (addrtype *)&HeadObj))
     if (!strncmp (HeadObj->ListName, NameOfList,  sizeof (nametype)))
        FOUND   = True;
  }
  if (*Status   == Iterate_EndOf)
      *Status    = Env_Normal;

  if (!FOUND)
  {
    *ListTkn  = NullToken;
    *Status   = List01_NotFound;
  }

TRACK(TrackBak,"List01_FindListHead\n");
return(STAT);
}


/*     14.1.2.1                                                           */
boolean   List01_CreateNode
                          (tokentype    *ListTkn,
                           ft F,lt Z,zz *Status,     tokentype  *NodeTkn)
{
listheadtype *HeadObj         = NullPtr;
listnodetype *NodeObj         = NullPtr;

  if (OaGetObject    (ListTkn,   McStat,    (addrtype *)&HeadObj))
  if (OaCreateObject (ListTkn,                           HeadObj->NodeClass,
                      McStat,                            NodeTkn))
  if (OaGetObject    (NodeTkn,   McStat,    (addrtype *)&NodeObj))
  {
     HeadObj->CurrentNode      = NodeTkn->Handle;

     NodeObj->PrevNode         = ListTkn->Handle;
     NodeObj->NextNode         = NullHandle;
     NodeObj->NodeObject       = NullToken;
  }

TRACK(TrackBak,"List01_CreateNode\n");
return(STAT);
}


/*+-----------------------------------------------------------------------+*/

/*     14.1.3                                                             */
boolean    Is_List01Head (tokentype    *ListTkn,   ft F,lt Z,zz *Status)
{

  KernelIamA (ListTkn, DbListHead_Class, McStat);

return(STAT);
}
 

/*     14.1.4                                                             */
boolean    Is_List01Node   (tokentype    *ListTkn,   ft F,lt Z,zz *Status)
{

    KernelIamA (ListTkn, DbListNode_Class, McStat);

return(STAT); 
}
 
/*     14.1.5                                                             */
boolean Is_List01Member  (tokentype    *ListTkn,   tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn)
{
listheadtype *HeadObj         = NullPtr;
tokentype     NextNode        = NullToken;
listnodetype *NodeObj         = NullPtr;

  NextNode.DbId   = ListTkn->DbId;

  if (ListBug || DeBug)
  if (sprintf (Msg, "\n   Is_List01Member[%4u:%6u]:: Token=[%2u:%4u]  \n",
               ListTkn->DbId,    ListTkn->Handle,
               MemberTkn->DbId,  MemberTkn->Handle))
     TraceMsg (0, Msg);

  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
  for (NextNode.Handle  = HeadObj->FirstNode; 
       Normal(*Status) && NextNode.Handle;)
  {
     if (OaGetObject (&NextNode,   McStat,   (addrtype *)&NodeObj))
     if (NodeObj->NodeObject.Handle == MemberTkn->Handle
     &&  NodeObj->NodeObject.DbId   == MemberTkn->DbId)
     {
        if (ListBug || DeBug)
        if (sprintf (Msg, "     ...NodeTkn =[%4u:%6u]\n",
                     NodeTkn->DbId, NodeTkn->Handle))
           TraceMsg (0, Msg);

        return (True);

     } else
        NextNode.Handle = NodeObj->NextNode;
  }
  if (ListBug || DeBug)
     TraceMsg (0, "\n   ... Not a Member.\n");

  if (Normal(*Status))
     return (False);

TRACK(TrackBak,"Is_List01Member\n");
return (STAT);
}



/*+-----------------------------------------------------------------------+*/
/*     14.1.6                                                             */
boolean   List01_GetCount
                         (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,    numtype   *ItemCount)
{
listheadtype  *HeadObj         = NullPtr;

 *ItemCount      = 0;
  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
     *ItemCount  = HeadObj->NodeCount;

TRACK(TrackBak,"List01_GetCount\n");
return (STAT);
}


/*     14.1.7                                                             */
boolean   List01_Reset   (tokentype    *ListTkn,   ft F,lt Z,zz *Status)
{
listheadtype  *HeadObj         = NullPtr;

  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
     HeadObj->CurrentNode      = HeadObj->FirstNode;

TRACK(TrackBak,"List01_Reset\n");
return (STAT);
}


/*     14.1.8                                                             */
boolean   List01_SetCurrent
                         (tokentype    *ListTkn,   tokentype  *NodeTkn,
                          ft F,lt Z,zz *Status)
{
listheadtype  *HeadObj         = NullPtr;

  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
     HeadObj->CurrentNode      = NodeTkn->Handle;

TRACK(TrackBak,"List01_SetCurrent\n");
return (STAT);
}


/*     14.1.9                                                             */
boolean   List01_GetCurrent
                         (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,     tokentype  *NodeTkn)
{
listheadtype  *HeadObj         = NullPtr;

 *NodeTkn             = NullToken;
  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
  {
     NodeTkn->Handle   = HeadObj->CurrentNode;
     NodeTkn->DbId     = ListTkn->DbId;
  }

TRACK(TrackBak,"List01_GetCurrent\n");
return (STAT);
}


/*     14.1.10                                                            */
boolean   List01_IsEmpty
                         (tokentype    *ListTkn,    ft F,lt Z,zz *Status)
{
listheadtype  *HeadObj         = NullPtr;

  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
  if (HeadObj->NodeCount)
     return(False);

TRACK(TrackBak,"List01_IsEmpty\n");
return (STAT);
}



/* .......................................................................
                    ....      INSERTION     ....
   .......................................................................*/

 

/*     14.2.3                                                             */
boolean   List01_Append  (tokentype    *ListTkn,   tokentype *MemberTkn,
                          ft F,lt Z,zz *Status)
{
listheadtype *HeadObj         =  NullPtr;
listnodetype *LastObj         =  NullPtr;
listnodetype *PrevObj         =  NullPtr;
tokentype     PrevNode        =  NullToken;
tokentype     LastNode        =  NullToken;

  PrevNode.DbId    = ListTkn->DbId;
  LastNode.DbId    = ListTkn->DbId;

  if (Normal(*Status))
  if (OaGetObject    (ListTkn,   McStat,   (addrtype *)&HeadObj))
     PrevNode.Handle = HeadObj->LastNode;

  if (Normal(*Status))
  if (List01_CreateNode (ListTkn,     McStat,                  &LastNode))
  if (OaGetObject       (&LastNode,   McStat,      (addrtype *)&LastObj))
     LastObj->NodeObject  = *MemberTkn;

  /* If the List is Empty                                                  */
  if (Normal(*Status))
  if (HeadObj->FirstNode == NullHandle)
  {
     LastObj->PrevNode    =  ListTkn->Handle;
     LastObj->NextNode    =  NullHandle;

     HeadObj->FirstNode   =  LastNode.Handle;

  } else {
     LastObj->PrevNode    =  PrevNode.Handle;
     LastObj->NextNode    =  NullHandle;

     if (OaGetObject (&PrevNode,  McStat,           (addrtype *)&PrevObj))
        PrevObj->NextNode =  LastNode.Handle;
  }

  if (Normal(*Status))
  {
     HeadObj->CurrentNode =  LastNode.Handle;
     HeadObj->LastNode    =  LastNode.Handle;
     HeadObj->NodeCount++;
  }

TRACK(TrackBak,"ListAppend\n");
return (STAT);
}
 

/* .......................................................................
                    ....     traverseal     ....
   .......................................................................*/

/*     14.4.1                                                             */
boolean   List01_FirstIn (tokentype    *ListTkn,
                          ft F,lt Z,zz *Status,     tokentype *NodeTkn,
                          tokentype    *MemberTkn)
{
listheadtype *HeadObj         = NullPtr;
listnodetype *NodeObj         = NullPtr;

 *MemberTkn       = NullToken;
 *NodeTkn         = NullToken;

  if (Normal(*Status))
  if (ListTkn->Handle == NullHandle)
     *Status = List01_NullListTkn;
 
  if (Normal(*Status))
  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))
  if (HeadObj->FirstNode == NullHandle)
  {
     *Status = List01_EmptyList;
      if (DeBug || ListBug)
         TraceMsg (0, "    ... Empty List\n");

  } else {
     NodeTkn->DbId   = ListTkn->DbId;
     if ((NodeTkn->Handle = HeadObj->FirstNode) != NullHandle)
     if (OaGetObject (NodeTkn,   McStat,   (addrtype *)&NodeObj))
       *MemberTkn = NodeObj->NodeObject;

     HeadObj->CurrentNode  = NodeTkn->Handle;
  }

  if (ListBug || DeBug)
  if (sprintf (Msg, "     Node[%2u:%4u]  Member[%2u,%4u]\n",
               NodeTkn->DbId,     NodeTkn->Handle,
               MemberTkn->DbId,   MemberTkn->Handle))
     TraceMsg (0, Msg);
 
TRACK(TrackBak,"ListFirstIn\n");
return (STAT);
}
 

/*     14.4.2                                                             */
boolean   List01_NextIn    (tokentype    *ListTkn,    tokentype  *CurrNodeTkn,
                          ft F,lt Z,zz *Status,     tokentype  *NodeTkn,
                          tokentype    *MemberTkn)
{
listheadtype *HeadObj         = NullPtr;
listnodetype *NodeObj         = NullPtr;

 *NodeTkn    = NullToken;
 *MemberTkn  = NullToken;

  if (Normal(*Status))
  if (ListTkn->Handle == NullHandle)
     *Status = List01_NullListTkn;
 
  if (Normal(*Status))
  if (OaGetObject (ListTkn,   McStat,   (addrtype *)&HeadObj))

  if (CurrNodeTkn->Handle == NullHandle)
     *Status = List01_EmptyList;
 
  if (Normal(*Status))
  if (OaGetObject (CurrNodeTkn,   McStat,   (addrtype *)&NodeObj))

  if (NodeObj->NextNode  == NullHandle)
  {
     HeadObj->CurrentNode  =  HeadObj->LastNode;
    *Status = List01_EndOfList;

  } else {
     NodeTkn->DbId   = ListTkn->DbId;
     if ((NodeTkn->Handle = NodeObj->NextNode) != NullHandle)
     if (OaGetObject (NodeTkn,   McStat,   (addrtype *)&NodeObj))
       *MemberTkn          =  NodeObj->NodeObject;

     HeadObj->CurrentNode  =  NodeTkn->Handle;

  }

  if (ListBug ||  DeBug)
  if (sprintf (Msg, "  ListNextOf... Current[%2u,%4u]  Next[%2u,%4u]\n",
               CurrNodeTkn->DbId,    CurrNodeTkn->Handle,
               NodeTkn->DbId,        NodeTkn->Handle))
  if (TraceMsg (0, Msg))
  if (sprintf (Msg, "                Member [%2u,%4u]\n",
               MemberTkn->DbId,      MemberTkn->Handle))
    TraceMsg (0, Msg);
 
TRACK(TrackBak,"List01_NextIn\n");
return (STAT);
}

 

/*     14.4.5                                                             */
boolean   List01_IterateOn (tokentype   *ListTkn,  
                          iteratetype   Method,     ft F,lt Z,zz *Status)
{
tokentype     CurrentTkn;
tokentype     CurrentNode;
tokentype     NextTkn;
tokentype     NextNode;
indextype     ItNum        = 0;

 if (ListBug || DeBug)
 if (sprintf (Msg, "\n   IterateOn the List by ListTkn[%2u:%4u]  \n",
               ListTkn->DbId, ListTkn->Handle))
    TraceMsg (0, Msg);

 if (ListTkn->Handle)
 if (List01_Reset      (ListTkn,  McStat))
 if (List01_FirstIn    (ListTkn,  McStat,  &CurrentNode,   &CurrentTkn))
 {
    if (Normal(*Status))
    do {   
       ++ItNum;
       if ((*Method)   (&CurrentTkn,   ItNum,          McStat))
       if (List01_NextIn (ListTkn,      &CurrentNode,  
                        McStat,       &NextNode,      &NextTkn))
       {
           CurrentNode.Handle = NextNode.Handle;
           CurrentTkn.Handle  = NextTkn.Handle;
       }
    } while (Normal(*Status));
 }
 if (*Status == List01_EndOfList  || *Status == List01_EmptyList)
     *Status = Env_Normal;
 
TRACK(TrackBak,"List01_IterateOn\n");
return(STAT);
}
 

/*+-----------------------------------------------------------------------+
~M                         END OF LIST01.C                                !
  +-----------------------------------------------------------------------+*/
