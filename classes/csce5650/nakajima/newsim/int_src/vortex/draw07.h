/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - Draw7.h */
/*
// **************************** DRAW7.H   ********************************
// *  Header for Draw07.h  Test Object of syntax.             
// *        For access via Create/Commit/Delete && Get/Put Methods ONLY
// *  Adapted from C++ Report  by Ted Goldstein
// ***********************************************************************
*/
/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/

#ifndef DRAW7_07
#define DRAW7_07

#include "obj01.h"
#include "vchunk.h"
#include "grp.h"
#include "point.h"

#define        Dbl_Pie   (double )3.1415927
#ifdef DRAW07
   numtype     TestObj_Class                 = 0;

   numtype     Char_Scalar_Attr              = 0;
   numtype     Char_InStruct_Attr            = 0;
   numtype     Char_Array_Attr               = 0;
   numtype     Char_PtrArray_Array_Attr      = 0;

   numtype     Short_Scalar_Attr             = 0;
   numtype     ShortU_Scalar_Attr            = 0;
   numtype     Int_Scalar_Attr               = 0;
   numtype     IntU_Scalar_Attr              = 0;

   numtype     Long_Scalar_Attr              = 0;
   numtype     LongU_Scalar_Attr             = 0;

   numtype     Float_Scalar_Attr             = 0;
   numtype     Double_Scalar_Attr            = 0;

   numtype     Token_Scalar_Attr             = 0;
   numtype     Handle_Scalar_Attr            = 0;

   numtype     Void_Ptr_Attr                 = 0;
   numtype     Color_Attr                    = 0;
   numtype     IsActive_Attr                 = 0;
   numtype     xArray07_Attr                 = 0;
   numtype     Points_Attr                   = 0;
   numtype     Char_String_Attr              = 0;
   numtype     Byte_Array_Attr               = 0;
   numtype     BitMap_Attr                   = 0;

   numtype     Connection_Class              = 0;
   numtype     link_Attr                     = 0;
   numtype     to_Attr                       = 0;
   numtype     type_Attr                     = 0;
   numtype     length_Attr                   = 0;

   numtype     VconnectsDir_Attr             = 0;

   extern numtype     VpartsDir_Attr;
   extern numtype     VlinksDir_Attr;
   extern numtype     VpartLinkDir_Attr;

   extern numtype     XyPoint_Class;
   extern numtype     Xarray_Attr;
   extern numtype     XyCoords_Attr;
   extern numtype     Origin_Attr;
   extern numtype     XyOrigin_Attr;
   extern numtype     Yarray_Attr;
   extern numtype     Xcoords_Attr;
   extern numtype     XyDir_Attr;
   extern numtype     RefOrigin_Attr;
   extern numtype     VdrawObjDir_Attr;
#else
   extern numtype     TestObj_Class;

   extern numtype     Char_Scalar_Attr;
   extern numtype     Char_InStruct_Attr;
   extern numtype     Char_Array_Attr;
   extern numtype     Char_PtrArray_Array_Attr;

   extern numtype     Short_Scalar_Attr;
   extern numtype     ShortU_Scalar_Attr;
   extern numtype     Int_Scalar_Attr;
   extern numtype     IntU_Scalar_Attr;

   extern numtype     Long_Scalar_Attr;
   extern numtype     LongU_Scalar_Attr;

   extern numtype     Float_Scalar_Attr;
   extern numtype     Double_Scalar_Attr;

   extern numtype     Token_Scalar_Attr;
   extern numtype     Handle_Scalar_Attr;

   extern numtype     Void_Ptr_Attr;
   extern numtype     Color_Attr;
   extern numtype     IsActive_Attr;
   extern numtype     Points_Attr;
   extern numtype     Char_String_Attr;
   extern numtype     Byte_Array_Attr;
   extern numtype     xArray07_Attr;
   extern numtype     BitMap_Attr;

   extern numtype     Connection_Class;
   extern numtype     link_Attr;
   extern numtype     to_Attr;
   extern numtype     type_Attr;
   extern numtype     length_Attr;

   extern numtype     VpartsDir_Attr;
   extern numtype     VlinksDir_Attr;
   extern numtype     VpartLinkDir_Attr;

   extern numtype     XyPoint_Class;
   extern numtype     Xarray_Attr;
   extern numtype     XyCoords_Attr;
   extern numtype     Origin_Attr;
   extern numtype     XyOrigin_Attr;
   extern numtype     Yarray_Attr;
   extern numtype     Xcoords_Attr;
   extern numtype     XyDir_Attr;
   extern numtype     RefOrigin_Attr;

   extern numtype     VdrawObjDir_Attr;
   extern numtype     VconnectsDir_Attr;
#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
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
                aGrpAttr,      aVgrpAttr,
                aNonTypeAttr=EXT_ENUM      }             attrtype;

typedef enum   TypeDeleteType
               {dbFree,     dbCommit, dbDelete   }              deletetype;

typedef  struct   TypeDbmsString
{
   bytessizetype   Size;
   booleantype     IsaString;
	union string_or_int 
   {
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
#define GRP(class)         numtype
 ..........................................................................*/

typedef  struct varray(short)
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  short*                Varray;
} varray(short);


typedef  struct vchunk(int)
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  int*                  Varray;
  indextype             VstackPtr;
} vchunk(int);

typedef   longtype        partidtype;
typedef   char            parttype[10];
typedef   longtype        datetype;

typedef struct Connection_Type
{
   partidtype    to;
   parttype      type;
   longtype      length;

} Connection;


typedef  tokentype   BitField;

/*+-----------------------------------------------------------------------+
~C                          TEST  OBJECTS                                 !
  +-----------------------------------------------------------------------+*/

typedef    enum colortype   {aBlue,  aGreen,  aRed, aWhite,
                             aNonColor=EXT_ENUM              } colortype;


typedef struct XyPoint        
{
   shorttype         xCoord;
   shorttype         yCoord;

}XyPoint;

#ifndef POINT_01
typedef struct Point
{
   addrtype          That;
   tokentype         Token;
   short            (*Point_x)      (struct Point07 *This);
   short            (*Point_y)      (struct Point07 *This);
   doubletype       (*Point_Theta)  (struct Point07 *This);
   doubletype       (*Point_Radius) (struct Point07 *This);
}Point;

typedef struct CartesianPoint
{
   addrtype          That;
   tokentype         Token;
   short            (*Point_x)      (struct CartesianPoint07 *This);
   short            (*Point_y)      (struct CartesianPoint07 *This);
   doubletype       (*Point_Theta)  (struct CartesianPoint07 *This);
   doubletype       (*Point_Radius) (struct CartesianPoint07 *This);
   shorttype         xCoord;
   shorttype         yCoord;
}CartesianPoint;


typedef struct PolarPoint
{
   addrtype          That;
   tokentype         Token;
   short            (*Point_x)      (struct PolarPoint07   *This);
   short            (*Point_y)      (struct PolarPoint07   *This);
   doubletype       (*Point_Theta)  (struct PolarPoint07   *This);
   doubletype       (*Point_Radius) (struct PolarPoint07   *This);
   doubletype        Theta;
   doubletype        Radius;
}PolarPoint;
#endif

typedef struct RefPoint
{
   Point            *ObjAddr;
   tokentype         ObjTkn;

}RefPoint;


typedef struct TestObj
{
   addrtype                   That;
   tokentype                  Token;
   char                       Char_Scalar;
   char                       Char_InStruct[32];
   unsigned char             *Char_Array;
   char                     **Char_PtrArray_Array;
          short               Short_Scalar;
   unsigned short             ShortU_Scalar;
   int                        Int_Scalar;
   unsigned                   IntU_Scalar;
   lt64                       Long_Scalar;
   longtype                   LongU_Scalar;
   float                      Float_Scalar;
   double                     Double_Scalar;
   tokentype                  Token_Scalar;
   handletype                 Handle_Scalar;
   short                     *Xarray;
   short                    **XyCoords;
   addrtype                  *Void_Ptr;
   colortype                  Color;
   XyPoint                    XyOrigin;
   Connection                 link[3];
   Point                     *Origin;
   XyPoint                   *Points;
   boolean                    IsActive;
   bytetype                  *Byte_Array;
   varyingtype                Char_String;
   array(short)               Yarray;
   varray(short)              xArray;
   vchunk(int)                Xcoords;
   vchunktoken                XyDir;
   BitField                   BitMap;
   refobjtype(Point)          RefOrigin;
   numtype                    VpartsDir;
   grp(tokentype)             VdrawObjDir;
   vgrp(Connection)           VconnectsDir;

}TestObj;


/* .......................................................................
                    ....    TESTOBJ       ....
   .......................................................................*/

       boolean   Draw7_Init                       (ft F,lt Z,zz *Status);

       boolean   TestObj_InitClass
                         (ft F,lt Z,zz *Status);

       TestObj  *TestObj_new0
                         (tokentype    *AnchorTkn, ft F,lt Z,zz *Status);

       TestObj  *TestObj_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status);

       void      TestObj_show
                         (TestObj      *This);

       void      TestObj_Topology 
                         (void);

       boolean   TestObj_delete
                         (TestObj     **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF DRAW07.H                                !
  +-----------------------------------------------------------------------+*/
#endif
