/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef DRAWOBJ_01
#define DRAWOBJ_01

/* Dev - DrawObj.h
// **************************** DRAWObj.H ********************************
// *  Header for Draw01.h  Base Class descriptors for all objects
// *                       that are of the draw catorgy            
// ***********************************************************************
*/

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/

#ifndef DRAWLIB_01
#ifdef DRAWOBJ
          numtype    MemberNamedDrawObjs_Attr = 0;

          numtype    Id_Attr                = 0;
          numtype    Nam_Attr               = Nam_AttrNum;

          numtype    DrawObj_Class          = 0;
          numtype    NamedDrawObj_Class     = 0;
#else
   extern numtype    MemberNamedDrawObjs_Attr;

   extern numtype    Id_Attr;
   extern numtype    Nam_Attr;

   extern numtype    DrawObj_Class;
   extern numtype    NamedDrawObj_Class;

#endif
#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
 
typedef struct DrawObj
{
   addrtype          That;
   tokentype         Token;
   numtype           Id;

   memberhandle      MemberDrawObjs;
   slottoken         PartObjSlotAttr;

   boolean      (*draw)     (struct DrawObj  *This,  ft F,lt Z,zz *Status);
   lt64         (*area)     (struct DrawObj  *This,  ft F,lt Z,zz *Status);
   boolean      (*destruct) (struct DrawObj **This,  deletetype    DbEvent,
                                ft F,lt Z,zz *Status);
}DrawObj;


typedef struct NamedDrawObj
{
   addrtype          That;
   tokentype         Token;
   numtype           Id;

   memberhandle      MemberDrawObjs;
   slottoken         PartObjSlotAttr;

   boolean      (*draw)    (struct NamedDrawObj  *This, ft F,lt Z,zz *Status);
   lt64         (*area)    (struct NamedDrawObj  *This, ft F,lt Z,zz *Status);
   boolean      (*destruct)(struct NamedDrawObj **This, deletetype    DbEvent,
                                    ft F,lt Z,zz *Status);
   nametype          Nam;
   memberhandle      MemberNamedDrawObjs;

}NamedDrawObj;

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....    DRAWOBJ        ....
   .......................................................................*/
       boolean DrawObj_InitClass 
                         (ft F,lt Z,zz *Status);

       DrawObj *DrawObj_new0
                         (tokentype    *AnchorTkn, ft F,lt Z,zz *Status);

       DrawObj *DrawObj_new1   
                         (tokentype    *AnchorTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status);

       DrawObj *DrawObj_new2
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       boolean DrawObj_export
                         (DrawObj      *This,      ft F,lt Z,zz *Status);

       boolean DrawObj_draw
                         (DrawObj      *This,      ft F,lt Z,zz *Status);

       lt64    DrawObj_area
                         (DrawObj      *This,      ft F,lt Z,zz *Status);

       boolean DrawObj_delete
                         (DrawObj     **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

       boolean DrawObj_Create0
                         (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean DrawObj_Create1  
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean DrawObj_Import   
                         (tokentype    *DrawTkn,   ft F,lt Z,zz *Status);

       boolean DrawObj_Export   
                         (tokentype    *DrawTkn,   ft F,lt Z,zz *Status);

/* .......................................................................
                    ....     DRAW OBJS     ....
   .......................................................................*/

       boolean DrawObjs_InitClass 
                         (ft F,lt Z,zz *Status);


       boolean OwnerOfDrawObjs
                         (tokentype    *Member,
                          ft F,lt Z,zz *Status,    tokentype    *Owner);


       boolean DrawObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean DrawObjs_FindIn 
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);
 
       inttype DrawObjs_IterateOn
                         (tokentype    *OwnerTkn,  inttype       ItNum,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token);


/* .......................................................................
                    ....  NAMED DRAWOBJ   ....
   .......................................................................*/
       boolean NamedDrawObj_InitClass 
                         (ft F,lt Z,zz *Status);

       NamedDrawObj *NamedDrawObj_new0
                         (tokentype    *AnchorTkn, ft F,lt Z,zz *Status);

       NamedDrawObj *NamedDrawObj_new1   
                         (tokentype    *AnchorTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status);

       NamedDrawObj *NamedDrawObj_new2
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       boolean NamedDrawObj_Create0
                         (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean NamedDrawObj_Create1  
                         (tokentype    *Anchor,    numtype       ClassId,
                          nametype      Nam,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean NamedDrawObj_ExportFunc  
                         (tokentype    *Token,     indextype     IterNum,
                          ft F,lt Z,zz *Status);

       boolean NamedDrawObj_delete
                         (NamedDrawObj **This,     deletetype    DbEvent,
                          ft F,lt Z,zz *Status);
/* .......................................................................
                    ....   NAMED DRAW OBJS ....
   .......................................................................*/

       boolean NamedDrawObjs_InitClass 
                         (ft F,lt Z,zz *Status);

       boolean NamedDrawObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean NamedDrawObjs_FindIn 
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);
 
       boolean NamedDrawObjs_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,  
                          ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF DRAWOBJ.H                               !
  +-----------------------------------------------------------------------+*/
#endif
