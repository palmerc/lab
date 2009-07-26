/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C_Shell - OBJ01.h */
#ifndef OBJ01_H
#define OBJ01_H

#include "env01.h"
#include "primal.h"

/*+-----------------------------------------------------------------------+
  !                 ....       OBJECT       ....                          !
  +-----------------------------------------------------------------------+*/

#ifndef PSH01_HPP

/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
/* General Definitions required by all application modules                 */
/* Tokens  to Primal and Null Dbs                                          */

extern   numtype       Ref_BaseClass;
extern   tokentype     PrimalTkn;
extern   tokentype     NullToken;

typedef  addrtype      versionref; 
typedef  funcptrtype   cppfuncptr;
typedef  tokentype     porttype; 


#define array(type)        type  *
#define varray(type)       Varray_##type
#define vchunk(type)       Vchunk_##type
#define Reference(class)   Ref##class
#define refobjtype(class)  Ref##class
#define refobj(class)      Ref##class
#define portobj(baseclass) porttype
#define Port(baseclass)    porttype
#define reftoken(class)    tokentype
#define refhandle(class)   tokentype

#define GRP(type)          grptype
#define grp(type)          grptype

#define VGRP(type)         vgrptype
#define vgrp(type)         vgrptype

#define for_IterateOn_Set(SetId,OwnerTkn,ItNum,F,Z,Status,NodeTkn,Token) \
 for(ItNum=0;(ItNum=Owner_SetIterateOn(SetId,ItNum,F,Z,Status,NodeTkn,Token))>0;)

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/


/* .......................................................................
                    ....    OBJECT        ....
   .......................................................................*/

typedef struct TypeObject
{
  tokentype             Token;

}  Object;


typedef struct TypeImage01
{
  addrtype              That;
  tokentype             Token;

}  Image01;


/* .......................................................................
                    ....      REF OBJ       ....
   .......................................................................*/

#ifndef ENV1_H
typedef struct TypeObjRef
{
  addrtype              ObjAddr;
  tokentype             ObjTkn;

}  RefObj;
#endif

/* .......................................................................
                    ....    API OBJ         ....
   .......................................................................*/

typedef struct TypeApiObj
{
  addrtype              That;
  tokentype             Token;
  addrtype              PortHead;
  ownerhandle           OwnerApiPorts;

}  ApiObj;


typedef struct TypePortObj
{
  addrtype              That;
  tokentype             Token;
  addrtype              PortHole;
  numtype               PortId;
  memberhandle          MemberApiPorts;

}  PortObj;


/* .......................................................................
                   ....      VARRAY        ....
   .......................................................................*/
#ifndef ENV1_H
typedef struct TypeVarray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  void                 *Varray;

}  varraytype;

typedef struct TypeVchunk
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  void                 *Varray;
  indextype             VstackPtr;

}  vchunktype;
#endif

typedef struct ShortArray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  short                *Varray;
}ShortArray;
typedef ShortArray  shortarraytype;
 

typedef struct IntArray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  int                  *Varray;

}IntArray;
typedef IntArray   intarraytype;

typedef struct IntChunk
{
 tokentype              VchunkTkn;
 indextype              Vsize;
 int                   *Varray;
 indextype              VstackPtr;

}IntChunk;
typedef IntChunk  intchunktype;

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/



#endif /* PSH01_HPP */

/* ~   4.1.1                                                               */
       boolean Object_Create
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token);


/* ~   4.1.4                                                               */
       void    Object_Dump
                         (tokentype    *Token);



/* ~   4.1.7                                                               */
       numtype Object_IsA
                         (tokentype    *Token,     ft F,lt Z,zz *Status);

/* ~   4.1.8                                                               */
       boolean Object_IsKindOf
                         (tokentype    *Token,     numtype       ClassId,
                          ft F,lt Z,zz *Status);


/* ~   4.1.17                                                              */
       boolean Object_Delete 
                         (tokentype    *Token,     ft F,lt Z,zz *Status); 

/* ........................................................................
                    .... Image Of Interface ....
   ........................................................................*/

/* ~   4.2.0                                                               */
       boolean Object_NewImage  
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,
                          tokentype    *Token,     addrtype     *Image);

/* ~   4.2.1                                                               */
       boolean Object_GetImage
                         (tokentype    *Token,
                          ft F,lt Z,zz *Status,    addrtype     *Image);

/* ~   4.2.2                                                               */
       boolean Object_ImageIsActive
                         (tokentype    *Token,     ft F,lt Z,zz *Status);


/* ~   4.2.4                                                               */
       boolean Object_PutImage
                         (tokentype    *Token,     ft F,lt Z,zz *Status); 

/* ~   4.2.5                                                               */
       boolean Object_FreeImage 
                         (tokentype    *Token,     ft F,lt Z,zz *Status); 

/* ~   4.2.6                                                               */
       boolean Object_CommitImage
                         (tokentype    *Token,     boolean       Release,
                          ft F,lt Z,zz *Status); 

/* ~   4.2.7                                                              */
       void    Image_DumpPseudo
                         (tokentype    *Anchor,    numtype       ClassId,
                          addrtype      Pseudo);
                          




/*+-----------------------------------------------------------------------+
  !                 ....        ATTR        ....                          !
  +-----------------------------------------------------------------------+*/
/*     6.0  Attribute  <csh.h>                                             */

/* ........................................................................
                    ....  Generic Value     ....
   ........................................................................*/
/* ~   6.2.1                                                               */
       boolean Attr_ValuePut
                         (tokentype    *Token,     numtype       AttrId,
                          addrtype      Value,     ft F,lt Z,zz *Status);

/* ~   6.2.2                                                               */
       boolean Attr_ValueGet
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    addrtype      Value);

/* ~   6.2.3                                                               */
       int     Attr_ValueCompare
                         (tokentype    *Token,     numtype       AttrId,
                          addrtype      CompValue, ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status);

/* ........................................................................
                    .... Direct Reference  ....
   ........................................................................*/
/* ~   6.3.1                                                               */
       boolean Attr_ObjTknPut   
                         (tokentype    *Token,     numtype       AttrId,
                          tokentype    *ObjTkn,    ft F,lt Z,zz *Status);


/* ~   6.3.2                                                               */
       boolean Attr_ObjTknGet
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    tokentype    *ObjTkn);

/* ........................................................................
                    ....    Reference      ....
   ........................................................................*/
/* ~   6.3.3                                                               */
       boolean Attr_TokenToRef
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    addrtype     *RefObject);

/* ~   6.3.4                                                               */
       boolean Attr_RefTknPut
                         (tokentype    *Token,     numtype       AttrId,
                          tokentype    *RefTkn,    ft F,lt Z,zz *Status);

/* ........................................................................
                    ....     String         ....
   ........................................................................*/
/* ~   6.4.1                                                               */
       boolean Attr_StringCreate
                         (tokentype    *Token,     numtype       AttrId,
                          sizetype      AllocSize,
                          ft F,lt Z,zz *Status,    cstring      *String);

/* ~   6.4.2                                                               */
       boolean Attr_StringGet
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,   
                          boolean      *StrCpyIsA, cstring      *String);

/* ~   6.4.3                                                               */
       boolean Attr_StrCpyGet
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    cstring      *String);

/* ~   6.4.4                                                               */
       boolean Attr_StrCpyFree
                         (ft F,lt Z,zz *Status,    cstring      *String);

/* ........................................................................
                    ....  Dynamic Array     ....
   ........................................................................*/

/* ~   6.5.1                                                               */
       boolean Attr_ArrayCreate
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,    addrtype     *Array);

/* ~   6.5.1.1                                                             */
       boolean Attr_SubArrayCreate
                         (tokentype    *Token,     
                          numtype       AttrId,    indextype     ArrayIndex,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,    addrtype     *Array);

/* ~   6.5.2                                                               */
       boolean Attr0_ArrayActivate
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    numtype      *UnitCount,
                          sizetype     *UnitSize,  addrtype     *Array);

/* ~   6.5.3                                                               */
       boolean Attr0_ArrayDeActivate
                         (tokentype    *Token,     numtype       AttrId,
                          deletetype    Action,    ft F,lt Z,zz *Status);

/* ~   6.5.4                                                               */
       boolean Attr_ArrayGetSize
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,   
                          numtype      *UnitCount, sizetype     *UnitSize);

/* ~   6.5.4.1                                                             */
       boolean Attr_SubArrayGetSize
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    numtype      *UnitCount);

/* ~   6.5.5                                                               */
       boolean Attr_ArrayAssertSize
                         (tokentype    *Token,     numtype       AttrId,
                          numtype       UnitCount,
                          ft F,lt Z,zz *Status,    addrtype     *Array);

/* ~   6.5.5.1                                                             */
       boolean Attr_SubArrayAssertSize
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,     numtype       UnitCount,
                          ft F,lt Z,zz *Status,    addrtype     *Array);

/* ~   6.5.6                                                               */
       boolean Attr_ArrayDelete
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status);

/* ~   6.5.6.1                                                             */
       boolean Attr_SubArrayDelete
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,     ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
  !                 ....       FIELD         ....                         !
  +-----------------------------------------------------------------------+*/

/* ........................................................................
                    ....  Generic Value     ....
   ........................................................................*/

/* ~   6s.2.1                                                              */
       boolean Field_ValuePut
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status);

/* ~   6s.2.2                                                              */
       boolean Field_ValueGet
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value);



#ifndef PSH01_HPP
/*+-----------------------------------------------------------------------+
  !                 ....         SET        ....                          !
  +-----------------------------------------------------------------------+*/
/*     7.0  Set <csh.h>                                                    */

/* ........................................................................
                    ....     Set Owner      ....
   ........................................................................*/

/*     7.1    Owner                                                        */



/* ~   7.1.2                                                               */
       inttype Owner_SetIterateOn
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          inttype       ItNum,     ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token);

/* ~   7.1.3                                                               */
       boolean Owner_SetFirstOf
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn,
                          tokentype    *FirstTkn);


/* ~   7.1.5                                                               */
       boolean Owner_KeySetFindIn
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          addrtype      KeyValue,
                          ft F,lt Z,zz *Status,    tokentype    *MemberTkn);

/*     7.1.6                                                               */
       boolean Owner_KeySetFindInBy2aryKey
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          addrtype      KeyValue,
                          ft F,lt Z,zz *Status,    tokentype    *MemberTkn);



/* ........................................................................
                    ....     Set Member     ....
   ........................................................................*/

/*     7.2  Member                                                         */

/* ~   7.2.1                                                               */
       boolean Member_SetOwnerOf
                         (numtype       SetId,     tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,    tokentype    *OwnerTkn);


/* ~   7.2.2                                                               */
       boolean Member_SetPrevOf
                         (handletype    SetId,     tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *PrevTkn);

/* ~   7.2.3                                                               */
       boolean Member_SetNextOf
                         (handletype    SetId,     tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *NextTkn);


/* ~   7.2.5                                                               */
       boolean Member_KeySetAddInto
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          tokentype    *MemberTkn, ft F,lt Z,zz *Status);





/* ........................................................................
                    ....  Set Initialization ...
   ........................................................................*/
/* ~   7.4.1                                                               */
/*           if aIndexedSet or aLocalKeyedSet or  aGlobalKeyedSet.         */
       boolean Set_Create
                          (handletype    SetDesc,   tokentype    *OwnerTkn,
                           numtype       AllocSize, numtype       Order,
                           ft F,lt Z,zz *Status);

/* ~   7.4.2                                                               */
/*           if aLocalTreeSet or aGlobalTreeSet.                           */
       boolean Tree_Create
                      (handletype    SetDescHndl,  tokentype     *OwnerTkn, 
                       numtype       TreeOrder,    numtype        AllocKeys,
                       sizetype      KeySize,      typetype       KeyType,         
                       ft F,lt Z,zz *Status,       tokentype     *TreeTkn);



/*+-----------------------------------------------------------------------+
  !                 ....       MATRIX       ....                          !
  +-----------------------------------------------------------------------+*/

#ifdef     __INCLUDE_MTRX__
#include "mtrx01.h"
#endif /*  __INCLUDE_MTRX__  */

#endif /* PSH01_HPP */

/*+-----------------------------------------------------------------------+
~P                         END OF OBJ01.H                                 !
  +-----------------------------------------------------------------------+*/
#endif

