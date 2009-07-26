/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - Rect.h */
/* **************************** Rect.H    ********************************
 *  Header for Draw01.h  Rectangle composed of two points
 *                       for both Cartesian and Polar coordinates
 *  Adapted from C++ Report  by Ted Goldstein
 ************************************************************************* */

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/

#ifndef RECT_01
#define RECT_01

#include "drawlib.h"
#include "drawobj.h"
#include "tree01.h"

#include "point.h"


#ifndef DRAWLIB_01
#ifdef RECT  
          numtype    MemberLibRectangles_Attr = 0;

          numtype    Id_Attr                = 0;
          numtype    Nam_Attr               = Nam_AttrNum;

          numtype    Origin_Attr            = 0;
          numtype    Extent_Attr            = 0;

          numtype    XyOrigin_Attr          = 0;
          numtype    XyExtent_Attr          = 0;

          numtype    Xcoords_Attr           = 0;
          numtype    Ycoords_Attr           = 0;

          numtype    XyCoords_Attr          = 0;

          numtype    XyDir_Attr             = 0;

          numtype    Name_Attr              = 0;
          numtype    RefOrigin_Attr         = 0;
          numtype    RefExtent_Attr         = 0;

          numtype    PortHole_Attr          = 0;
          numtype    PortId_Attr            = 0;
          numtype    MemberAplPorts_Attr    = 0;

          numtype    Port_XyOrigin_Attr     = 0;
          numtype    Port_XyExtent_Attr     = 0;

          numtype    Xarray_Attr            = 0;
          numtype    Yarray_Attr            = 0;

          numtype    xVarray_Attr           = 0;
          numtype    yVarray_Attr           = 0;


          numtype    Rectangle_Class        = 0;
          numtype    XyRect_Class           = 0;
          numtype    NamedXyRect_Class      = 0;

          numtype    ArrayRect_Class        = 0;
          numtype    DblPtrRect_Class       = 0;

          numtype    VarrayRect_Class       = 0;

          numtype    IntChunkRect_Class     = 0;
          numtype    VchunkRect_Class       = 0;


          numtype    RefRect_Class          = 0;
          numtype    PortRect_Class         = 0;

          numtype    AplObj_Class           = 0;
          numtype    PortObj_Class          = 0;
          numtype    DbmsToken_Class        = 0;

#else
   extern numtype    MemberLibRectangles_Attr;
   extern numtype    Id_Attr;
   extern numtype    Origin_Attr;
   extern numtype    Extent_Attr;
   extern numtype    XyOrigin_Attr;
   extern numtype    XyExtent_Attr;
   extern numtype    PortHole_Attr;
   extern numtype    PortId_Attr;
   extern numtype    MemberAplPorts_Attr;
   extern numtype    Port_XyOrigin_Attr;
   extern numtype    Port_XyExtent_Attr;
   extern numtype    Xcoords_Attr;
   extern numtype    Ycoords_Attr;
   extern numtype    XyCoords_Attr;
   extern numtype    Xarray_Attr;
   extern numtype    Yarray_Attr;

   extern numtype    xVarray_Attr;
   extern numtype    yVarray_Attr;
   extern numtype    XyDir_Attr;
   extern numtype    Name_Attr;
   extern numtype    RefOrigin_Attr;
   extern numtype    RefExtent_Attr;

   extern numtype    Rectangle_Class;
   extern numtype    DrawLib_Class;
   extern numtype    XyRect_Class;
   extern numtype    NamedXyRect_Class;
   extern numtype    ArrayRect_Class;
   extern numtype    DblPtrRect_Class;
   extern numtype    VarrayRect_Class;
   extern numtype    IntChunkRect_Class;
   extern numtype    VchunkRect_Class;
   extern numtype    PortRect_Class;
   extern numtype    AplObj_Class;
   extern numtype    PortObj_Class;
   extern numtype    DbmsToken_Class;
   extern numtype    RefRect_Class;

#endif /*  RECT        */
#endif /*  DRAWLIB_01  */

typedef  enum             TypeDrawObj
              {aRectangle,  aXyRect,      aNamedXyRect,  aArrayRect,
               aDblPtrRect, aVarrayRect,  aIntChunkRect, 
               aRefRect,    aVchunkRect,  
               aNonDrawObj =EXT_ENUM                   } drawobjtype;

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
 
/* aObjRefAttr                                                             */
typedef struct Rectangle
{     
   addrtype              That;
   tokentype             Token;
   numtype               Id;
   memberhandle          MemberDrawObjs;
   slottoken             PartObjSlotAttr;

   boolean  (*draw)     (struct Rectangle       *This,  ft F,lt Z,zz *Status);
   lt64     (*area)     (struct Rectangle       *This,  ft F,lt Z,zz *Status);
   boolean  (*destruct) (struct Rectangle      **This,  deletetype    DbEvent,
                                  ft F,lt Z,zz *Status);
   Point                *Origin;
   Point                *Extent;
   memberhandle          MemberLibRectangles;

} Rectangle;
 

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....    RECTANGLE      ....
   .......................................................................*/
       boolean Rectangle_InitClass
                         (ft F,lt Z,zz *Status);


       Rectangle  *Rectangle_new0
                         (tokentype    *AnchorTkn,            
                          short         i0,        short         i1,  
                          short         i01,       ft F,lt Z,zz *Status);

       Rectangle  *Rectangle_new1
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status);

       Rectangle  *Rectangle_new2
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status);

       Rectangle  *Rectangle_new3
                         (tokentype    *AnchorTkn,            
                          tokentype    *oTkn,      tokentype    *eTkn,
                          ft F,lt Z,zz *Status);

       Rectangle  *Rectangle_new4
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       lt64    Rectangle_area
                         (Rectangle    *This,      ft F,lt Z,zz *Status);


       boolean Rectangle_draw
                         (Rectangle    *This,      ft F,lt Z,zz *Status);

       void    Rectangle_show
                         (Rectangle    *This);

       boolean Rectangle_error
                         (Rectangle    *This,      char         *msg, 
                          ft F,lt Z,zz *Status);

       boolean Rectangle_delete
                         (Rectangle   **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);


/* .......................................................................
                    ....   LIB RECTANGLES  ....
   .......................................................................*/
       boolean LibRectangles_InitClass 
                         (ft F,lt Z,zz *Status);

       boolean OwnerOfLibRectangles
                         (tokentype    *Member,
                          ft F,lt Z,zz *Status,    tokentype    *Owner);


       boolean LibRectangles_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean LibRectangles_FindIn 
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);
 
       boolean LibRectangles_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF RECT.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
