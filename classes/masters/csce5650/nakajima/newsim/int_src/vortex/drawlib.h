/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - DrawLib.h */

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS                           !
 *+-----------------------------------------------------------------------+*/

#ifndef DRAWLIB_01
#define DRAWLIB_01

#include "obj01.h"
#include "grp.h"
#include "sm.h"
#include "spcl.h"

#define        Dbl_Pie   (double )3.1415927

#ifdef DRAWLIB
          numtype    LibRectangles_Set        = 0;
          numtype    OwnerLibRectangles_Attr  = 0;
          numtype    MemberLibRectangles_Attr = 0;

          numtype    DrawObjs_Set           = 0;
          numtype    OwnerDrawObjs_Attr     = 0;
          numtype    MemberDrawObjs_Attr    = 0;

          numtype    NamedDrawObjs_Set        = 0;
          numtype    OwnerNamedDrawObjs_Attr  = 0;
          numtype    MemberNamedDrawObjs_Attr = 0;

          numtype    DrawObjDir_Attr        = 0;
          numtype    VdrawObjDir_Attr       = 0;
          numtype    VpartsDir_Attr         = 0;
          numtype    Id_Attr                = 0;
          numtype    Nam_Attr               = Nam_AttrNum;

          numtype    VlinksDir_Attr         = 0;
          numtype    VpartLinkDir_Attr      = 0;

          numtype    DrawPart_Tupl          = 0;
          numtype    DrawObj_Slot           = 0;
          numtype    PartObj_Slot           = 0;

          numtype    xCoord_Attr            = 0;
          numtype    yCoord_Attr            = 0;

          numtype    Theta_Attr             = 0;
          numtype    Radius_Attr            = 0;

          numtype    Origin_Attr            = 0;
          numtype    Extent_Attr            = 0;

          numtype    XyOrigin_Attr          = 0;
          numtype    XyExtent_Attr          = 0;

          numtype    PortHole_Attr          = 0;
          numtype    PortId_Attr            = 0;
          numtype    MemberAplPorts_Attr    = 0;
          numtype    Port_XyOrigin_Attr     = 0;
          numtype    Port_XyExtent_Attr     = 0;

          numtype    Xarray_Attr            = 0;
          numtype    Yarray_Attr            = 0;

          numtype    xVarray_Attr           = 0;
          numtype    yVarray_Attr           = 0;

          numtype    Xcoords_Attr           = 0;
          numtype    Ycoords_Attr           = 0;

          numtype    XyCoords_Attr          = 0;

          numtype    XyDir_Attr             = 0;

          numtype    Name_Attr              = 0;

          numtype    RefOrigin_Attr         = 0;
          numtype    RefExtent_Attr         = 0;

          numtype    DrawId_Attr            = 0;

          numtype    Point_Class            = 0;
          numtype    CartesianPoint_Class   = 0;
          numtype    PolarPoint_Class       = 0;
          numtype    DrawObj_Class          = 0;
          numtype    NamedDrawObj_Class     = 0;

          numtype    Rectangle_Class        = 0;

          numtype    DrawLib_Class          = 0;

          numtype    XyPoint_Class          = 0;
          numtype    XyRect_Class           = 0;
          numtype    NamedXyRect_Class      = 0;

          numtype    ArrayRect_Class        = 0;

          numtype    VarrayRect_Class       = 0;

          numtype    IntChunkRect_Class     = 0;

          numtype    DblPtrRect_Class       = 0;
          numtype    VchunkRect_Class       = 0;
          numtype    PortRect_Class         = 0;
          numtype    AplObj_Class           = 0;
          numtype    PortObj_Class          = 0;
          numtype    RefRect_Class          = 0;
          numtype    DbmsToken_Class        = 0;


#else
   extern numtype    LibRectangles_Set;
   extern numtype    OwnerLibRectangles_Attr;
   extern numtype    MemberLibRectangles_Attr;
   extern numtype    DrawObjs_Set;
   extern numtype    OwnerDrawObjs_Attr;
   extern numtype    MemberDrawObjs_Attr;
   extern numtype    NamedDrawObjs_Set;
   extern numtype    OwnerNamedDrawObjs_Attr;
   extern numtype    MemberNamedDrawObjs_Attr;
   extern numtype    DrawObjDir_Attr;
   extern numtype    VdrawObjDir_Attr;
   extern numtype    VpartsDir_Attr;
   extern numtype    VlinksDir_Attr;
   extern numtype    VpartLinkDir_Attr;
   extern numtype    DrawPart_Tupl;
   extern numtype    DrawObj_Slot;
   extern numtype    PartObj_Slot;
   extern numtype    Id_Attr;
   extern numtype    Nam_Attr;
   extern numtype    xCoord_Attr;
   extern numtype    yCoord_Attr;
   extern numtype    Theta_Attr;
   extern numtype    Radius_Attr;
   extern numtype    Origin_Attr;
   extern numtype    Extent_Attr;
   extern numtype    XyOrigin_Attr;
   extern numtype    XyExtent_Attr;
   extern numtype    PortHole_Attr;
   extern numtype    PortId_Attr;
   extern numtype    MemberAplPorts_Attr;
   extern numtype    Port_XyOrigin_Attr;
   extern numtype    Port_XyExtent_Attr;
   extern numtype    Xarray_Attr;
   extern numtype    Yarray_Attr;
   extern numtype    xVarray_Attr;
   extern numtype    yVarray_Attr;
   extern numtype    Xcoords_Attr;
   extern numtype    Ycoords_Attr;
   extern numtype    XyCoords_Attr;
   extern numtype    XyDir_Attr;
   extern numtype    Name_Attr;
   extern numtype    RefOrigin_Attr;
   extern numtype    RefExtent_Attr;

   extern numtype    DrawId_Attr;

   extern numtype    Point_Class;
   extern numtype    CartesianPoint_Class;
   extern numtype    PolarPoint_Class;
   extern numtype    DrawObj_Class;
   extern numtype    NamedDrawObj_Class;
   extern numtype    Rectangle_Class;
   extern numtype    DrawLib_Class;
   extern numtype    XyPoint_Class;
   extern numtype    XyRect_Class;
   extern numtype    NamedXyRect_Class;
   extern numtype    ArrayRect_Class;
   extern numtype    VarrayRect_Class;
   extern numtype    IntChunkRect_Class;
   extern numtype    DblPtrRect_Class;
   extern numtype    VchunkRect_Class;
   extern numtype    PortRect_Class;
   extern numtype    AplObj_Class;
   extern numtype    PortObj_Class;
   extern numtype    DbmsToken_Class;
   extern numtype    RefRect_Class;

#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
 
/* aAttr                                                                   */
/* aSetHeadAttr                                                            */
/* aSetNodeAttr                                                            */
/* aSlotTokenAttr                                                          */

typedef struct TypeDrawLib
{
   addrtype          That;
   tokentype         Token;
   numtype           DbNum;
   wordtype          EnvType;
   numtype           DescNum;
   slottokentype     PrimalSlotAttr;
   slottoken         PrimalGroupSlotAttr;

   numtype           DrawId;
   grp(tokentype)    DrawObjDir;
   vgrp(tokentype)   VdrawObjDir;

   ownerhandle       OwnerDrawObjs;
   ownerhandle       OwnerNamedDrawObjs;
   ownerhandle       OwnerLibRectangles;

} DrawLib;
 

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

#ifdef DRAWLIB

          DrawLib   *DrawLib01              = NullPtr;
          tokentype  LibTkn010              = Null_Token;
          tokentype  VchunkTkn00            = Null_Token;
          numtype    VpartsDir              = 0;
          longtype   PartCount              = 0;
          boolean    DrawBug                = False;
          tokentype  RectListTkn            = Null_Token;
#else 

extern DrawLib   *DrawLib01;
extern tokentype  LibTkn010;
extern tokentype  VchunkTkn00;
extern numtype    VpartsDir;
extern longtype   PartCount;
extern boolean    DrawBug;
extern tokentype  RectListTkn;

#endif

/* .......................................................................
                    ....     DRAW LIB      ....
   .......................................................................*/

       boolean DrawLib_InitLibrary
                         (ft F,lt Z,zz *Status);

       boolean DrawLib_InitClass
                         (ft F,lt Z,zz *Status);

       boolean DrawLib_InitDb   
                         (char         *DbName,  char         *DbFileName,  
                          numtype       Option,
                          ft F,lt Z,zz *Status,  tokentype    *PrimalTkn);

       boolean DrawLib_QueryDb
                         (char         *DbName,
                          ft F,lt Z,zz *Status,    boolean      *Active);

       boolean DrawLib_CreateDb
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, DrawLib     **PrimalObj);

       boolean DrawLib_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    DrawLib     **PrimalObj);

       boolean DrawLib_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    DrawLib     **PrimalObj);

       boolean DrawLib_DeleteDb
                         (tokentype    *PrimalTkn, ft F,lt Z,zz *Status);


       numtype DrawLib_NewId
                         (ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF DRAWLIB.H                               !
  +-----------------------------------------------------------------------+*/
#endif
