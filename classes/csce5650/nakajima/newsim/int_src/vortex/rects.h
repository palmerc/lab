/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - rects.h */

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/

#ifndef RECTS_01
#define RECTS_01

#include  "rect.h"

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/

/* ************************** DEFINES  **********************************  */
/* ........................   <obj01.h>  ................................. */

/* ........................................................................
typedef enum   TypeTypeTypeDesc
               {aString,  aInteger,  aReal,       aLink,  aEnum,
                aBoolean, aChunk,    aDbmsString, aValue, aAddr,
                aRefObj                                         } typetype;

typedef enum   TypeAttrType
               {aAttr,         aObjRefAttr,   aMapRefAttr,   aPortRefAttr,
                aTknRefAttr,   aStrHndlAttr,  aChunkAttr, 
                aSetHeadAttr,  aSetNodeAttr,  
                aMtrxHeadAttr, aRowHeadAttr,  aColHeadAttr,
                aSlotHndlAttr, aSlotTokenAttr,
                aVchunkAttr,   aArrayAttr,    aVarrayAttr,   aDblPtrAttr,
                aNonTypeAttr=EXT_ENUM      }             attrtype;

typedef enum   TypeDeleteType
               {dbFree,     dbCommit, dbDelete   }              deletetype;

typedef  struct   TypeDbmsString {
   bytessizetype   Size;
   booleantype     IsaString;
	union string_or_int {
          char         *StringValue;
          farlongtype   IntValue;
	}           Value;
}                         dbmsstringtype;
typedef  dbmsstringtype   dbmsstring;

typedef enum   TypeSetType
               {aLocalSet,          aGlobalSet, 
                aLocalKeyedSet,     aGlobalKeyedSet,
                aLocalKeyed2arySet, aGlobalKeyed2arySet,
                aLocalTableSet,     aGlobalTableSet,
                aLocalTreeSet,      aGlobalTreeSet,
                aIndexedSet,        aSpecialSet}                settype;

typedef enum   TypeSetHeadType
               {aLSetHead,          aGSetHead,  
                aLKeyedSetHead,     aGKeyedSetHead,
                aLKeyed2arySetHead, aGKeyed2arySetHead,
                aLTableSetHead,     aGTableSetHead,
                aLTreeSetHead,      aGTreeSetHead,
                aIndexedSetHead,    aSpclSetHead,
                aLSetNode,          aGSetNode    }              setheadtype;

typedef struct TypeVarray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  void                 *Varray;

}  varraytype;
typedef varraytype      Varray;

typedef struct IntArray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  int                  *Varray;
}  intarraytype;

typedef struct ShortArray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  short                *Varray;
};

typedef struct IntChunk
{
 tokentype              VchunkTkn;
 indextype              Vsize;
 int                   *Varray;
 indextype              VstackPtr;

}intchunktype;
...........................................................................*/
/* ************************* END DEFINES  *******************************  */


/* Example of aStruct                                                      */
typedef struct XyPoint        
{
   shorttype             xCoord;
   shorttype             yCoord;

} XyPoint;


/* Example of aMapRefAttr                                                  */
typedef struct XyRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct XyRect          *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct XyRect          *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct XyRect         **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   XyPoint               XyOrigin;
   XyPoint               XyExtent;

} XyRect;



/* Example of aStrHndlAttr                                                 */
typedef struct NamedXyRect
{
   addrtype             That;
   tokentype            Token;
   numtype              Id;

   memberhandle         MemberDrawObjs;
   slottoken            PartObjSlotAttr;

   boolean  (*draw)     (struct NamedXyRect     *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct NamedXyRect     *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct NamedXyRect    **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   nametype             Nam;
   memberhandle         MemberNamedDrawObjs;

   XyPoint              XyOrigin;
   XyPoint              XyExtent;

   char                *Name;

} NamedXyRect;



/* Example of aArrayAttr                                                   */
typedef struct ArrayRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct ArrayRect       *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct ArrayRect       *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct ArrayRect      **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   shorttype            *Xarray;
   shorttype            *Yarray;

} ArrayRect;



/* Example of aDblPtrAttr                                                  */
typedef struct DblPtrRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct DblPtrRect      *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct DblPtrRect      *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct DblPtrRect     **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);

   short               **XyCoords;

} DblPtrRect;



/* Example of aVarrayAttr                                                  */
typedef struct VarrayRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct VarrayRect      *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct VarrayRect      *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct VarrayRect     **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);

   ShortArray            xVarray;
   ShortArray            yVarray;

} VarrayRect;



/* Example of aChunkAttr                                                   */
typedef struct IntChunkRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct IntChunkRect    *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct IntChunkRect    *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct IntChunkRect   **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);

   IntChunk              Xcoords;
   IntChunk              Ycoords;

} IntChunkRect;



/* Example of aVchunkAttr                                                  */
typedef struct VchunkRect
{     
/* Object with 4 XyPoints stored in a user controlled dynamic array        */
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct VchunkRect      *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct VchunkRect      *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct VchunkRect     **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   vchunktoken           XyDir;
} VchunkRect;



/* aRefObjAttr                                                             */
typedef struct RefPoint
{
   Point                *ObjAddr;
   tokentype             ObjTkn;

} RefPoint;


/* Example of aTknRefAttr                                                  */
typedef struct RefRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct RefRect         *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct RefRect         *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct RefRect        **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   Reference(Point)      RefOrigin;
   Reference(Point)      RefExtent;

} RefRect;



/* Example of aPortRefAttr                                                 */
typedef struct PortRect
{
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct PortRect        *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct PortRect        *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct PortRect       **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);

   addrtype              PortHole;
   numtype               PortId;
   memberhandle          MemberAplPorts;
   Port(CartesianPoint)  Port_XyOrigin;
   Port(CartesianPoint)  Port_XyExtent;

} PortRect;


/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/
       boolean Rects_InitLibrary
                         (ft F,lt Z,zz *Status);

/* .......................................................................
                    ....    XYRECT      ....
   .......................................................................*/
       boolean XyRect_InitClass
                         (ft F,lt Z,zz *Status);


       XyRect  
              *XyRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       XyRect
              *XyRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    XyRect_area
                         (XyRect       *This,      ft F,lt Z,zz *Status);


       boolean XyRect_draw
                         (XyRect       *This,      ft F,lt Z,zz *Status);

       boolean XyRect_dump
                         (XyRect       *This,      ft F,lt Z,zz *Status);

       void    XyRect_show
                         (XyRect       *This);

       boolean XyRect_delete
                         (XyRect      **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    .... NAMEDXYRECT    ....
   .......................................................................*/
       boolean NamedXyRect_InitClass
                         (ft F,lt Z,zz *Status);


       NamedXyRect
              *NamedXyRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       NamedXyRect
              *NamedXyRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    NamedXyRect_area
                         (NamedXyRect  *This,      ft F,lt Z,zz *Status);


       boolean NamedXyRect_draw
                         (NamedXyRect  *This,      ft F,lt Z,zz *Status);

       boolean NamedXyRect_dump
                         (NamedXyRect  *This,      ft F,lt Z,zz *Status);

       void    NamedXyRect_show
                         (NamedXyRect  *This);

       boolean NamedXyRect_delete
                         (NamedXyRect **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....   ARRAYRECT      ....
   .......................................................................*/
       boolean ArrayRect_InitClass
                         (ft F,lt Z,zz *Status);

       ArrayRect
              *ArrayRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       ArrayRect
              *ArrayRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    ArrayRect_area
                         (ArrayRect    *This,      ft F,lt Z,zz *Status);


       boolean ArrayRect_draw
                         (ArrayRect    *This,      ft F,lt Z,zz *Status);

       boolean ArrayRect_dump
                         (ArrayRect    *This,      ft F,lt Z,zz *Status);

       void    ArrayRect_show
                         (ArrayRect    *This);

       boolean ArrayRect_delete
                         (ArrayRect   **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....   DBLPTRRECT    ....
   .......................................................................*/
       boolean DblPtrRect_InitClass
                         (ft F,lt Z,zz *Status);

       DblPtrRect
              *DblPtrRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       DblPtrRect
              *DblPtrRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    DblPtrRect_area
                         (DblPtrRect   *This,      ft F,lt Z,zz *Status);


       boolean DblPtrRect_draw
                         (DblPtrRect   *This,      ft F,lt Z,zz *Status);

       boolean DblPtrRect_dump
                         (DblPtrRect   *This,      ft F,lt Z,zz *Status);

       void    DblPtrRect_show
                         (DblPtrRect   *This);

       boolean DblPtrRect_delete
                         (DblPtrRect  **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....   VARRAYRECT     ....
   .......................................................................*/
       boolean VarrayRect_InitClass
                         (ft F,lt Z,zz *Status);
       VarrayRect
              *VarrayRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       VarrayRect
              *VarrayRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    VarrayRect_area
                         (VarrayRect   *This,      ft F,lt Z,zz *Status);


       boolean VarrayRect_draw
                         (VarrayRect   *This,      ft F,lt Z,zz *Status);

       boolean VarrayRect_dump
                         (VarrayRect   *This,      ft F,lt Z,zz *Status);

       void    VarrayRect_show
                         (VarrayRect   *This);

       boolean VarrayRect_delete
                         (VarrayRect  **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);


/* .......................................................................
                    ....  INTARRAYRECT    ....
   .......................................................................*/
       boolean IntChunkRect_InitClass
                         (ft F,lt Z,zz *Status);

       IntChunkRect
              *IntChunkRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       IntChunkRect
              *IntChunkRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    IntChunkRect_area
                         (IntChunkRect *This,      ft F,lt Z,zz *Status);


       boolean IntChunkRect_draw
                         (IntChunkRect *This,      ft F,lt Z,zz *Status);

       boolean IntChunkRect_dump
                         (IntChunkRect *This,      ft F,lt Z,zz *Status);

       void    IntChunkRect_show
                         (IntChunkRect *This);

       boolean IntChunkRect_delete
                         (IntChunkRect **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);



/* .......................................................................
                    ....   VCHUNKRECT    ....
   .......................................................................*/
       boolean VchunkRect_InitClass
                         (ft F,lt Z,zz *Status);

       VchunkRect
              *VchunkRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status);

       VchunkRect
              *VchunkRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    VchunkRect_area
                         (VchunkRect   *This,      ft F,lt Z,zz *Status);


       boolean VchunkRect_draw
                         (VchunkRect   *This,      ft F,lt Z,zz *Status);

       boolean VchunkRect_dump
                         (VchunkRect   *This,      ft F,lt Z,zz *Status);

       void    VchunkRect_show
                         (VchunkRect   *This);

       boolean VchunkRect_delete
                         (VchunkRect  **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....    REFRECT       ....
   .......................................................................*/
       boolean RefRect_InitClass
                         (ft F,lt Z,zz *Status);
       RefRect
              *RefRect_new0
                         (tokentype    *AnchorTkn,            
                          short         i0,        short         i1,  
                          short         i01,       ft F,lt Z,zz *Status);

       RefRect
              *RefRect_new1
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status);

       RefRect
              *RefRect_new2
                         (tokentype    *AnchorTkn,            
                          tokentype    *oTkn,      tokentype    *eTkn,
                          ft F,lt Z,zz *Status);

       RefRect
              *RefRect_new3
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    RefRect_area
                         (RefRect      *This,      ft F,lt Z,zz *Status);


       boolean RefRect_draw
                         (RefRect      *This,      ft F,lt Z,zz *Status);

       boolean RefRect_dump
                         (RefRect      *This,      ft F,lt Z,zz *Status);

       void    RefRect_show
                         (RefRect      *This);

       boolean RefRect_delete
                         (RefRect     **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/* .......................................................................
                    ....    PORTRECT      ....
   .......................................................................*/
       boolean PortRect_InitClass
                         (ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF RECTS.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
