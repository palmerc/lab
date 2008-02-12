/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ****************************  Draw701.c    ***************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       1.7   Draw701 - Test of all Attribute types allowable.          */
/* *                     (i.e direct access to the Image of the Object)    */
/* *********************************************************************** */

#define  DRAW7
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "draw07.h"

        boolean Draw701   (tokentype    *Anchor,   boolean       TestBug,
                           numtype       Option,   ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~C                           DRAW 7                                       !
  +-----------------------------------------------------------------------+*/
boolean Draw701           (tokentype    *Anchor,   boolean       TestBug,
                           numtype       Option,   ft F,lt Z,zz *Status)
{
tokentype                  LibTkn000             = *Anchor;
tokentype                  RefTkn                = NullToken;
TestObj                    Ref0;
TestObj                   *ref0                  = NullPtr;
int                        j                     = 0;
int                        k                     = 0;
int                        Compare               = 0;
char                      *StrAddr               = NullPtr;
fieldstruc                *LinkField0            = NullPtr;
fieldstruc                *LinkField1            = NullPtr;
fieldstruc                *LinkField2            = NullPtr;
static Connection          Null_link             = {0, "\0", 0};
static XyPoint             Null_XyOrigin         = {0,0};
static varray(short)       Null_ShortArray       = {Null_Token, 0, NullPtr};
static vchunk(int)         Null_IntChunk         = {Null_Token, 0, NullPtr, 0};
static refobjtype(Point)   Null_RefOrigin        = {NullPtr, Null_Token};
numtype                    Null_GrpNum           = 0;
boolean                    StrCpyIsA             = False;
char                       Char_Scalar           = 'A';
static nametype            Char_InStruct         = "nametype string";
static cstring             Char_InStruct1        = "cstring  string";
static char               *str1                  = "char    *array";
cstring                    Char_Array            = NullPtr;
char                     **StrPtrs               = NullPtr;
numtype                    NumNames              = 10;
static  char              *Names[10]             = {
                              "FullFix",     "Pre1111",        "Q2Mid123",
                              "Pre12Mid12",  "Q1Post",         "Pre12Post", 
                              "Q2Mid123Post","Pre12Mid123Post","wildstring",
                              "stringwild"         };
signed short               Short_Scalar          = -9;
unsigned short             ShortU_Scalar         = 10;
signed                     Int_Scalar            = -11;
unsigned                   IntU_Scalar           = 12;
lt64                       Long_Scalar           = -13;
longtype                   LongU_Scalar          = 14;
float                      Float_Scalar          = 15.1;
double                     Double_Scalar         = 16.1;
tokentype                  Token_Scalar          = *Anchor;
handletype                 Handle_Scalar         = 18;
static short               Xarray[5]             = {1,2,3,4,5};
numtype                    NumXarrays            = 5;
numtype                    UnitCount             = 0;
numtype                    UnitCount0            = 0;
static short               XyCoords[2][5]        = { {-1,-2,-3,-4,-5},
                                                     {6,7,8,9,10} };
short                     *ShortArray            = NullPtr;
short                     *XyCoordPtrs           = NullPtr;
numtype                    NumXyCoordPtrs        = 2;
numtype                    NumXyCoords           = 5;
addrtype                  *Void_Ptr              = NullPtr;
numtype                    NumVoidPtrs           = 5;
bytessizetype              Void_AllocSize        = 0;
colortype                  Color                 = aGreen;
static XyPoint             XyOrigin              = {101,102};
static Connection          link[3]               = { {1, "link1", 10},
                                                     {2, "link2", 20},
                                                     {3, "link3", 30} };
tokentype                  OriginTkn             = NullToken;
CartesianPoint            *Origin                = NullPtr;
shorttype                  xAxisVal              = 24;
shorttype                  yAxisVal              = 240;
XyPoint                   *Points                = NullPtr;
numtype                    NumPoints             = 2;
boolean                    IsActive              = True;
bytetype                  *Bytes                 = NullPtr;
static bytetype            Byte_Array[5]         = {10,20,30,40,50};
numtype                    NumBytes              = 5;
short                     *Yarray                = NullPtr;
numtype                    NumYarrays            = 5;
varray(short)              xArrayStruc;
short                     *xArray                = NullPtr;
numtype                    NumxArrays            = 5;
int                       *Xcoords               = NullPtr;
numtype                    NumXcoords            = 5;
vchunk(int)                XcoordsStruc;
vchunktoken                XyDirTkn              = NullToken;                
int                       *XyDir                 = NullPtr;
numtype                    NumXyDirs             = 5;
tokentype                  BitMapTkn             = NullToken;
numtype                    NumBits               = 10;
Point                     *RefOrigin             = NullPtr;
numtype                    NumVparts             = 100;
numtype                    GrpNum                = 0;

grpheader                 *GrpHdr                = NullPtr;
numtype                    GrpIndex              = 0;
sizetype                   EntrySize             = sizeof (XyPoint);
numtype                    RgnHndls              = 1000;
numtype                    AllocRgns             = (RgnHndls / 25) + 27;
numtype                    RgnEntrys             = 25;
numtype                    AllocXs               = 1000;
numtype                    BaseIndex             = 0;
accesstype                 AccessType            = Read_Write;
boolean                    DiskCache             = False;
numtype                    CacheLimit            = 10;
numtype                    CacheLoad             = 10;
numtype                    NumBuffRgns           = 10;
numtype                    BaseRegion            = 0;
sizetype                   UnitSize              = 0;
static boolean             FirstTime             = True;

  if (Normal(*Status))
  {
/*+-----------------------------------------------------------------------+*/
/* TEST OBJ                                                                */
/*+-----------------------------------------------------------------------+*/

    if (TestBug)
    TraceMsg (0, "\n *** Begin test for TestObj\n");

/* Create an instance of this object in the DB                            */
    if (Env_NewFieldStruc    ("link[0]", McStat,  &LinkField0))
    if (Env_NewFieldStruc    ("link[1]", McStat,  &LinkField1))
    if (Env_NewFieldStruc    ("link[2]", McStat,  &LinkField2))

    if (Option  == 1)
    {
        IsActive    = False;

        if (TestBug)
        TraceMsg (0, " CREATE Object internal to DBMS, and PUT Values\n");

        if (Object_Create    (&LibTkn000,       TestObj_Class,
                              McStat,          &RefTkn))
        if (RefTkn.Handle == 0) TestBug = True;
        if (TestBug)
        {
          sprintf (Msg, " TestObj Created. Token[%3u:%8u]\n",
                   RefTkn.DbId, RefTkn.Handle);
          TraceMsg (0, Msg);
        }


        FirstTime   = False;

    } else {
        IsActive    = True;

        if (TestBug)
        TraceMsg (0, " CREATE Object, GET Image, and PUT Values.\n");

        if (Object_NewImage  
                             (&LibTkn000,        TestObj_Class,
                              McStat,   &RefTkn, (addrtype *)&ref0))
        if (RefTkn.Handle == 0) TestBug = True;
        if (TestBug)
        {
          sprintf (Msg, " TestObj Created. Token[%3u:%8u] Image @%8x\n",
                   RefTkn.DbId, RefTkn.Handle, ref0);
          TraceMsg (0, Msg);
          TraceMsg (0, " Display contents of NEW TestObject IMAGE...\n");
          Object_Dump        (&RefTkn);
        }
    }

    if (Normal(*Status))
    {
       if (Attr_ValuePut     (&RefTkn,   Char_Scalar_Attr,
                              (addrtype )&Char_Scalar,
                              McStat))

       if (Option       <= 1)
           Attr_ValuePut     (&RefTkn,   Char_InStruct_Attr, 
                              (addrtype )Char_InStruct,
                              McStat);
       else if (Option  == 2)
           Attr_ValuePut 
                             (&RefTkn,   Char_InStruct_Attr,
                              "A Long Name",
                              McStat);
       else if (Option  == 2)
           Attr_ValuePut     (&RefTkn,   Char_InStruct_Attr,
                              (addrtype )Char_InStruct1,
                              McStat);
       if (Normal(*Status))
       if (Attr_StringCreate (&RefTkn,   Char_Array_Attr,    sizeof (nametype),
                              McStat,   &StrAddr))
          strcpy (StrAddr, str1);

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Char_PtrArray_Array_Attr,    
                              NumNames,                      sizeof (char *),
                              McStat,           (addrtype *)&StrPtrs))
       for (j= -1; Normal(*Status) && ++j < NumNames; )
       {
          if (Attr_SubArrayCreate
                             (&RefTkn,   Char_PtrArray_Array_Attr,   j, 
                              strlen(Names[j])+1,         sizeof (char ),
                              McStat,           (addrtype *)&StrAddr))
             strcpy (StrAddr, Names[j]);
       }

       if (Normal(*Status))
       if (Attr_ValuePut     (&RefTkn,   Short_Scalar_Attr,
                              (addrtype )&Short_Scalar,
                              McStat))

       if (Attr_ValuePut     (&RefTkn,   ShortU_Scalar_Attr,
                              (addrtype )&ShortU_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Int_Scalar_Attr,
                              (addrtype )&Int_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   IntU_Scalar_Attr,
                              (addrtype )&IntU_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Long_Scalar_Attr,
                              (addrtype )&Long_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   LongU_Scalar_Attr,
                              (addrtype )&LongU_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Float_Scalar_Attr,
                              (addrtype )&Float_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Double_Scalar_Attr,
                              (addrtype )&Double_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Token_Scalar_Attr,
                              (addrtype )&Token_Scalar,
                              McStat))
       if (Attr_ValuePut     (&RefTkn,   Handle_Scalar_Attr,
                              (addrtype )&Handle_Scalar,
                              McStat))
       if (Attr_ArrayCreate  (&RefTkn,   Xarray_Attr,    
                              NumXarrays,              sizeof (short),
                              McStat,           (addrtype *)&ShortArray))
       for (j= -1; Normal(*Status) && ++j < NumXarrays; )
       {
         ShortArray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   XyCoords_Attr,    
                              NumXyCoordPtrs,         sizeof (short *),
                              McStat,           (addrtype *)&ShortArray))
       for (j= -1; Normal(*Status) && ++j < NumXyCoordPtrs; )
       {
          if (Attr_SubArrayCreate
                             (&RefTkn,   XyCoords_Attr,   j, 
                              NumXyCoords,            sizeof (short),
                              McStat,           (addrtype *)&ShortArray))
          for (k= -1; Normal(*Status) && ++k < NumXyCoords; )
          {
            ShortArray[k]  = XyCoords[j][k];
          }
       }

       if (Normal(*Status))
       if (TestBug)
       {
          Attr_ArrayCreate   (&RefTkn,   Void_Ptr_Attr,    
                              NumVoidPtrs,            sizeof (addrtype ),
                              McStat,                 (addrtype *)&Void_Ptr);
          if (*Status == Trans_MixedArraySize)
          {
            *Status    = Env_Normal;
             if (TestBug)
             TraceMsg (0, "  Void_Ptr           'ArrayCreate'     PASSED.\n");
          } else
             TraceMsg (0, "  Void_Ptr           'ArrayCreate'     FAILED.\n");

          if (TestBug)
          TraceMsg (0, "\n");
       }
       if (Normal(*Status))
       if (Attr_ValuePut     (&RefTkn,   Color_Attr,   (addrtype )&Color,
                              McStat))

       if (Attr_ValuePut     (&RefTkn,   XyOrigin_Attr, 
                              (addrtype )&XyOrigin,               McStat))

       if (Field_ValuePut    (&RefTkn,   LinkField0,   (addrtype )&link[0],
                              McStat))
       if (Field_ValuePut    (&RefTkn,   LinkField1,   (addrtype )&link[1],
                              McStat))
       if (Field_ValuePut    (&RefTkn,   LinkField2,   (addrtype )&link[2],
                              McStat))

       if (Normal(*Status))
       if (IsActive == False)
       {
          if (Object_Create  (&RefTkn,   CartesianPoint_Class,
                              McStat,                          &OriginTkn))
          if (Attr_ValuePut  (&OriginTkn,xCoord_Attr,  (addrtype )&xAxisVal,
                              McStat))
          if (Attr_ValuePut  (&OriginTkn,yCoord_Attr,  (addrtype )&yAxisVal,
                              McStat))

              Attr_ObjTknPut (&RefTkn,   Origin_Attr,             &OriginTkn,
                              McStat);
       } else {

          if (Object_NewImage(&RefTkn,   CartesianPoint_Class,  McStat,
                              &OriginTkn,         (addrtype  *)&Origin))
          {
             Origin->xCoord  = 24;
             Origin->yCoord  = 240;

             /* Can use Attr_ValuePut or Attr_ObjTknPut with same result
                of maintaining the 'Reference Count'integrity.
                This is valid also when changing the Object of Reference. */

             Attr_ValuePut   (&RefTkn,   Origin_Attr,  (addrtype )Origin,
                              McStat);
          }                                             
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Points_Attr,    
                              NumPoints,                   sizeof (XyPoint),
                              McStat,           (addrtype *)&Points))
       {
          Points[0].xCoord  = 26;
          Points[0].yCoord  = -26;
          Points[1].xCoord  = 126;
          Points[1].yCoord  = -126;
       }

       if (Normal(*Status))
       if (Attr_ValuePut     (&RefTkn,   IsActive_Attr, 
                              (addrtype )&IsActive,               McStat))

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Byte_Array_Attr,    
                              NumBytes,               sizeof (bytetype),
                              McStat,           (addrtype *)&Bytes))
       for (j= -1; Normal(*Status) && ++j < NumBytes; )
       {
         Bytes[j]   = Byte_Array[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Yarray_Attr,    
                              NumYarrays,             sizeof (short),
                              McStat,           (addrtype *)&Yarray))
       for (j= -1; Normal(*Status) && ++j < NumYarrays; )
       {
         Yarray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   xArray07_Attr,    
                              NumxArrays,             sizeof (short),
                              McStat,           (addrtype *)&xArray))

       if (Attr_ValueGet     (&RefTkn,   xArray07_Attr, 
                              McStat,    (addrtype )&xArrayStruc))

       if (Vchunk_PutStackPtr(&xArrayStruc.VchunkTkn,      NumxArrays, 
                              McStat,         (addrtype *)&xArray))

       for (j= -1; Normal(*Status) && ++j < NumxArrays; )
       {
         xArray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Xcoords_Attr,    
                              NumXcoords,             sizeof (int),
                              McStat,           (addrtype *)&Xcoords))
       if (Attr_ValueGet     (&RefTkn,   Xcoords_Attr, 
                              McStat,    (addrtype )&XcoordsStruc))

       if (Vchunk_PutStackPtr(&XcoordsStruc.VchunkTkn,     NumXcoords, 
                              McStat,         (addrtype *)&Xcoords))
       for (j= -1; Normal(*Status) && ++j < NumXcoords; )
       {
         Xcoords[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Vchunk_Create     (&RefTkn,   sizeof (int),     5,     10,
                              McStat,          &XyDirTkn))
       if (Attr_ValuePut     (&RefTkn,   XyDir_Attr,    
                             (addrtype )&XyDirTkn,                McStat))
       if (Vchunk_PutStackPtr(&XyDirTkn,                          NumXyDirs,
                              McStat,                (addrtype *)&XyDir))
       for (j= -1; Normal(*Status) && ++j < NumXyDirs; )
       {
         XyDir[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (BitField_Create   (&RefTkn,   NumBits,   McStat,      &BitMapTkn))
       if (Attr_ValuePut     (&RefTkn,   BitMap_Attr,    
                             (addrtype )&BitMapTkn,               McStat))
       for (j= -1; Normal(*Status) && ++j < NumBits; ++j)
       {
           BitField_Put      (&BitMapTkn,       j,     1,         McStat);
       }

       if (Normal(*Status))
       if (Attr_RefTknPut    (&RefTkn,   RefOrigin_Attr,          &OriginTkn,
                              McStat))    
       if (Attr_TokenToRef   (&RefTkn,   RefOrigin_Attr,
                              McStat,                 (addrtype *)&RefOrigin))    
       if (IsActive)
       if (RefOrigin != (Point *)Origin)
       {
          TraceMsg (0, "  ***SWIzzle MisMatch on Reference Token value.\n");
          sprintf (Msg, "     RefOrigin (%8x) != Origin (%8x)\n",
                   RefOrigin,  Origin);
          TraceMsg (0, Msg);
       }

       if (TestBug)
       if (Normal(*Status))
       if (GrpNewPacket      (&RefTkn,   EntrySize,   AllocRgns,   RgnEntrys,
                              AllocXs,   BaseIndex,   AccessType,  DiskCache,
                              CacheLimit,CacheLoad,   NumBuffRgns,
                              McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))

       if (Attr_ValuePut     (&RefTkn,   VpartsDir_Attr,(addrtype)&GrpNum, 
                              McStat))

       for (j= -1; Normal(*Status) && ++j < NumVparts; ++j)
       {
          Grp_PutNewEntry    (&RefTkn,   GrpNum,       (addrtype )&j,
                              McStat,   &GrpIndex);
       }

/* 00. DUMP                                                               */
       if (Normal(*Status))
       if (TestBug)
       {
          TraceMsg (0, "\n Display contents of RefOrigin Object ...\n");
          Object_Dump          (&OriginTkn);

          TraceMsg (0, " Display contents of Instantiated Object ...\n");
          Object_Dump          (&RefTkn);
       }

/* 00. COMMIT                                                             */
       if (Normal(*Status))
       if (IsActive)
       {
          if (TestBug)
          TraceMsg (0, "\n  COMMIT && FREE TestObj (and sub-structs) to DB.\n");

          if (TestObj_delete      (&ref0,       dbCommit,   McStat))
          if (Object_CommitImage  (&OriginTkn,  True,       McStat))
          if (TestBug)
          {
             TraceMsg (0,
                     " Display contents of Committed RefOrigin  Object ...\n");
             Object_Dump          (&OriginTkn);
             TraceMsg (0, 
                     " Display contents of Committed TestObject ...\n");
             Object_Dump          (&RefTkn);
          }
       }

       if (Normal(*Status))
       if (Void_Ptr      != NullPtr)
          Core0_FreeCore  ((addrtype *)&Void_Ptr,   Void_AllocSize,  McStat);
    }


/*  Retrieve TestObj Values with Direct Get Commands                      */

    if (Normal(*Status))
    {

       if (TestBug)
       TraceMsg (0, "\n Retrieve TestObj Values from Db.\n");

       if (IsActive)
       {
          if (TestBug)
          {
          TraceMsg (0, " Process TestObj IN Activate state (the Image).\n");
          TraceMsg (0, "   GET from the Image to a transient Copy.\n");
          }

          if (Object_GetImage (&RefTkn,     McStat,  (addrtype *)&ref0))
          if (TestBug)
          {
             sprintf (Msg, " TestObj Retrieved. Token[%3u:%8u] @%6x. \n\n",
                      ref0->Token.DbId, ref0->Token.Handle, ref0);
             TraceMsg (0, Msg);

             Object_Dump    (&RefTkn);

             sprintf (Msg, " Current State of OriginTkn[%3u:%8u]\n",
                      OriginTkn.DbId, OriginTkn.Handle);
             TraceMsg (0, Msg);

             Object_Dump    (&OriginTkn);
          }
       }

       if (Normal(*Status))
       if (IsActive   == Object_ImageIsActive   (&RefTkn,         McStat))
       {
           if (TestBug)
           TraceMsg (0, "  RefTkn      'Object_ImageIsActive' PASSED.\n");
       } else
           TraceMsg (0, "  RefTkn      'Object_ImageIsActive' FAILED.\n");


       Ref0.That                   = (addrtype )&Ref0;
       Ref0.Token                  = NullToken;
       Ref0.Char_Scalar            = ' ';
       Ref0.Char_InStruct[0]       = '\0';
       Ref0.Char_Array             = NullPtr;
       Ref0.Char_PtrArray_Array    = NullPtr;
       Ref0.Short_Scalar           = 0;
       Ref0.ShortU_Scalar          = 0;
       Ref0.Int_Scalar             = 0;
       Ref0.IntU_Scalar            = 0;
       Ref0.Long_Scalar            = 0;
       Ref0.LongU_Scalar           = 0;
       Ref0.Float_Scalar           = 0.0;
       Ref0.Double_Scalar          = 0.0;
       Ref0.Token_Scalar           = NullToken;
       Ref0.Handle_Scalar          = 0;
       Ref0.Xarray                 = NullPtr;
       Ref0.XyCoords               = NullPtr;
       Ref0.Void_Ptr               = NullPtr;
       Ref0.Color                  = aNonColor;
       Ref0.XyOrigin               = Null_XyOrigin;
       Ref0.link[0]                = Null_link;
       Ref0.link[1]                = Null_link;
       Ref0.link[2]                = Null_link;
       Ref0.Origin                 = NullPtr;
       Ref0.Points                 = NullPtr;
       Ref0.IsActive               = False;
       Ref0.Byte_Array             = NullPtr;
       Ref0.Char_String            = NullPtr;
       Ref0.Yarray                 = NullPtr;
       Ref0.xArray                 = Null_ShortArray;
       Ref0.Xcoords                = Null_IntChunk;
       Ref0.XyDir                  = NullToken;
       Ref0.BitMap                 = NullToken;
       Ref0.RefOrigin              = Null_RefOrigin;
       Ref0.VpartsDir              = Null_GrpNum;

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Char_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Char_Scalar))
       if ((Compare = 
           Attr_ValueCompare (&RefTkn,   Char_Scalar_Attr,
                              &Char_Scalar,          NullPtr,  McStat))
       == 0)
       {
           if (TestBug)
           TraceMsg (0, "  Char_Scalar        'ValueCompare'      PASSED.\n");
       } else
           TraceMsg (0, "  Char_Scalar        'ValueCompare'      FAILED.\n");

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Char_InStruct_Attr, 
                              McStat,    (addrtype )&Ref0.Char_InStruct))
       if ((Compare = 
           Attr_ValueCompare (&RefTkn,   Char_InStruct_Attr,
                              &Char_InStruct,        NullPtr,  McStat))
       == 0)
       {
           if (TestBug)
           TraceMsg (0, "  Char_InStruct      'ValueCompare'      PASSED.\n");
       } else {
          if (IsActive)
          {
           if (TestBug)
           TraceMsg (0, "  Char_InStruct      'ValueCompare'      PASSED.\n");
          } else
           TraceMsg (0, "  Char_InStruct      'ValueCompare'      FAILED.\n");
       }

       StrAddr   = NullPtr;
       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Char_Array_Attr,
                              McStat,    (addrtype )&Ref0.Char_Array))

       if (Attr_StringGet    (&RefTkn,   Char_Array_Attr,
                              McStat,   &StrCpyIsA,      &Char_Array))
       if (!strcmp (Char_Array,          (char *)Ref0.Char_Array))
       {
          if ((IsActive  == True  && StrCpyIsA  == False)
          ||  (IsActive  == False && StrCpyIsA  == True))
          {
           if (TestBug)
           TraceMsg (0, "  Char_Array         'ValueCompare'      PASSED.\n");
          } else
           TraceMsg (0, "  Char_Array         'ValueCompare'      FAILED.\n");
       } else
           TraceMsg (0, "  Char_Array         'ValueCompare'      FAILED.\n");


       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Char_PtrArray_Array_Attr,    
                              McStat,
                             (addrtype )&Ref0.Char_PtrArray_Array))

       if (Attr_ValueGet     (&RefTkn,   Short_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Short_Scalar))
       if ((Compare = 
           Attr_ValueCompare (&RefTkn,   Short_Scalar_Attr,
                              &Short_Scalar,         NullPtr,  McStat))
       == 0)
       {
           if (TestBug)
           TraceMsg (0, "  Short_Scalar       'ValueCompare'      PASSED.\n");
       } else
           TraceMsg (0, "  Short_Scalar       'ValueCompare'      FAILED.\n");


       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   ShortU_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.ShortU_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Int_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Int_Scalar))

       if (Attr_ValueGet     (&RefTkn,   IntU_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.IntU_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Long_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Long_Scalar))

       if (Attr_ValueGet     (&RefTkn,   LongU_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.LongU_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Float_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Float_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Double_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Double_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Token_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Token_Scalar))

       if (Attr_ValueGet     (&RefTkn,   Handle_Scalar_Attr,
                              McStat,    (addrtype )&Ref0.Handle_Scalar))

       if (Attr_ArrayGetSize (&RefTkn,   Xarray_Attr,    
                              McStat,   &UnitCount,      &UnitSize))
       if (UnitCount      != NumXarrays
       ||  UnitSize       != sizeof (short))
           TraceMsg (0, "  Xarray             'ArrayGetSize'      FAILED.\n");
       else  if (TestBug)
           TraceMsg (0, "  Xarray             'ArrayGetSize'      PASSED.\n");

       if (Normal(*Status))
       if (IsActive)
           Attr_ValueGet     (&RefTkn,   Xarray_Attr,
                              McStat,    (addrtype )&Ref0.Xarray);
       else {
           Attr0_ArrayActivate
                             (&RefTkn,   Xarray_Attr,    
                              McStat,   &UnitCount0,    
                             &UnitSize,  (addrtype *)&Ref0.Xarray);
           if (UnitCount0 != UnitCount
           ||  UnitSize   != sizeof (short))
              TraceMsg (0,
                        "  Xarray             'ArrayActivate'     FAILED.\n");
           else if (TestBug)
              TraceMsg (0, 
                        "  Xarray             'ArrayActivate'     PASSED.\n");
       }

       if (Normal(*Status))
       if (Attr_ArrayGetSize (&RefTkn,   XyCoords_Attr,    
                              McStat,   &UnitCount,      &UnitSize))
       if (UnitCount      != NumXyCoordPtrs)
           TraceMsg (0, "  XyCoords           'ArrayGetSize'      FAILED.\n");
       else  if (TestBug)
           TraceMsg (0, "  XyCoords           'ArrayGetSize'      PASSED.\n");

       if (Normal(*Status))
       if (IsActive   == False)
       {
           Attr0_ArrayActivate 
                             (&RefTkn,   XyCoords_Attr,    
                               McStat,   &UnitCount, 
                              &UnitSize, (addrtype *)&Ref0.XyCoords);
           if (UnitCount  != NumXyCoordPtrs)
              TraceMsg (0, 
                        "  XyCoords           'ArrayActivate'     FAILED.\n");
           else  if (TestBug)
              TraceMsg (0,
                        "  XyCoords           'ArrayActivate'     PASSED.\n");

       } else
           Attr_ValueGet     (&RefTkn,   XyCoords_Attr,
                              McStat,    (addrtype  )&Ref0.XyCoords);

       if (Normal(*Status))
       if (Attr_SubArrayGetSize 
                              (&RefTkn,  XyCoords_Attr,    1, 
                               McStat,   &UnitCount))
       if (UnitCount      != NumXyCoords)
           TraceMsg (0, "  XyCoords           'SubArrayGetSize'   FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  XyCoords           'SubArrayGetSize'   PASSED.\n");


       if (Normal(*Status))
       if (TestBug)
       if (Attr_ValueGet     (&RefTkn,   Void_Ptr_Attr,
                              McStat,    (addrtype )&Ref0.Void_Ptr))

       /* Void_Ptr_Attr should be set to NULL when Put or Committed       */
       if (Ref0.Void_Ptr != NullPtr)
           TraceMsg (0, "  Void_Ptr           'Attr_ValueGet'     FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  Void_Ptr           'Attr_ValueGet'     PASSED.\n");

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Color_Attr,
                              McStat,    (addrtype )&Ref0.Color))

       if (Attr_ValueGet     (&RefTkn,   XyOrigin_Attr, 
                              McStat,    (addrtype )&Ref0.XyOrigin))

       if (Field_ValueGet    (&RefTkn,   LinkField0,
                              McStat,    (addrtype )&Ref0.link[0]))
       if (Field_ValueGet    (&RefTkn,   LinkField1,
                              McStat,    (addrtype )&Ref0.link[1]))
       if (Field_ValueGet    (&RefTkn,   LinkField2,
                              McStat,    (addrtype )&Ref0.link[2]))

       if (IsActive)
           Attr_ValueGet     (&RefTkn,   Origin_Attr, 
                              McStat,    (addrtype )&Ref0.Origin);
       else {
          if (Attr_ObjTknGet (&RefTkn,   Origin_Attr, 
                              McStat,   &OriginTkn))
              Attr_ValueGet  (&RefTkn,   Origin_Attr, 
                              McStat,    (addrtype )&Ref0.Origin);
       }

       if (Normal(*Status))
       if (Env_TokenIsEquiv  (&OriginTkn,           &Ref0.Origin->Token))
       {
          if (TestBug)
          TraceMsg (0, "  Origin             'ObjTknGet'         PASSED!\n");
       } else
          TraceMsg (0, "  Origin             'ObjTknGet'         FAILED!\n");

       if (Normal(*Status))
           Attr0_ArrayActivate
                             (&RefTkn,   Points_Attr,    
                              McStat,   &NumPoints,  
                             &UnitSize, (addrtype *)&Ref0.Points);
       if (IsActive)
       {
          if (*Status  == Attr_ImageIsActive)
          {
             if (TestBug)
             TraceMsg (0,
                       "  Points             'ArrayActivate'     PASSED!\n");
            *Status   = Env_Normal;
             Attr_ValueGet   (&RefTkn,   Points_Attr, 
                              McStat,    (addrtype )&Ref0.Points);
          } else
             TraceMsg (0, 
                       "  Points             'ArrayActivate'     FAILED!\n");
       } else if (Normal(*Status)) {
          if (TestBug)
          TraceMsg (0, "  Points             'ArrayActivate'     PASSED!\n");
       } else
          TraceMsg (0, "  Points             'ArrayActivate'     FAILED!\n");

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   IsActive_Attr, 
                              McStat,    (addrtype )&Ref0.IsActive))

       if (IsActive)
       {
          if (Attr_ValueGet  (&RefTkn,   Byte_Array_Attr,    
                              McStat,   (addrtype *)&Ref0.Byte_Array))
           Attr_ArrayGetSize (&RefTkn,   Byte_Array_Attr,    
                              McStat,   &UnitCount,      &UnitSize);
       } else
          Attr0_ArrayActivate
                             (&RefTkn,   Byte_Array_Attr,    
                              McStat,   &UnitCount,      &UnitSize,
                              (addrtype *)&Ref0.Byte_Array);

       if (UnitCount      != NumBytes
       ||  UnitSize       != sizeof (bytetype))
           TraceMsg (0, "  Byte_Array         'Array method'      FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  Byte_Array         'Array method'      PASSED.\n");


       if (Normal(*Status))
       if (IsActive)
       {
          if (Attr_ValueGet  (&RefTkn,   Yarray_Attr,    
                              McStat,   (addrtype *)&Ref0.Yarray))
           Attr_ArrayGetSize (&RefTkn,   Yarray_Attr,    
                              McStat,   &UnitCount,      &UnitSize);
       } else
          Attr0_ArrayActivate
                             (&RefTkn,   Yarray_Attr,    
                              McStat,   &UnitCount,      &UnitSize,
                              (addrtype *)&Ref0.Yarray);

       if (Normal(*Status))
       if (UnitCount      != NumYarrays
       ||  UnitSize       != sizeof (short))
           TraceMsg (0, "  Yarray             'Array method'      FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  Yarray             'Array method'      PASSED.\n");

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   xArray07_Attr,    
                              McStat,    (addrtype *)&Ref0.xArray))
       if (Ref0.xArray.Vsize    != NumxArrays)
           TraceMsg (0, "  xArray             'varray method'     FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  xArray             'varray method'     PASSED.\n");

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Xcoords_Attr,    
                              McStat,    (addrtype *)&Ref0.Xcoords))
       if (Ref0.Xcoords.Vsize     != NumXcoords
       ||  Ref0.Xcoords.VstackPtr != NumXcoords)
           TraceMsg (0, "  Xcoords            'vchunk method'     FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  Xcoords            'vchunk method'     PASSED.\n");


       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   XyDir_Attr,    
                              McStat,    (addrtype )&Ref0.XyDir))

       if (Attr_ValueGet     (&RefTkn,   BitMap_Attr,    
                             McStat,    (addrtype )&Ref0.BitMap))

       if (Attr_ValueGet     (&RefTkn,   RefOrigin_Attr,    
                             McStat,     (addrtype )&Ref0.RefOrigin))

       if (Attr_TokenToRef   (&RefTkn,   RefOrigin_Attr,
                              McStat,   (addrtype *)&Ref0.RefOrigin.ObjAddr))    

       if (Attr_ValueGet     (&RefTkn,   VpartsDir_Attr,
                              McStat,    (addrtype )&Ref0.VpartsDir))

/* 00. DUMP                                                               */
       if (Normal(*Status))
       if (TestBug)
       {
          TraceMsg (0, "\n Display contents of RefOrigin Object ...\n");
          Object_Dump          (&OriginTkn);

          TraceMsg (0, " Display contents of 'Pseudo'  Object Ref0...\n");
          Image_DumpPseudo     (&LibTkn000,       TestObj_Class,  
                                (addrtype  )&Ref0);
       }

/* 00. FREE                                                               */
       if (Normal(*Status))
       if (IsActive)
       {
          if (TestBug)
          TraceMsg (0, "\n  FREE TestObj (and sub-structs) to DB.\n");

          if (LibTkn000.DbId != PrimalTkn.DbId)
          if (Object_FreeImage  (&RefTkn,        McStat))
              Object_FreeImage  (&OriginTkn,     McStat);

       } else if (Option == 10) {
          if (TestBug)
          TraceMsg (0, "\n  FREE StrCpy's and 'delete' Arrays(to DB).\n");

          if (Attr_StrCpyFree
                             (McStat,   &Char_Array))

          if (TestBug)
             TraceMsg (0, "       StrCpy   Char_Array      Freeed.\n");


          if (Normal(*Status))
          if (Attr0_ArrayDeActivate
                             (&RefTkn,   Xarray_Attr,        dbCommit, 
                              McStat))
          if (TestBug)
             TraceMsg (0, "       Array    Xarray          Committed\n");

          if (Normal(*Status))
          if (Attr0_ArrayDeActivate
                             (&RefTkn,   XyCoords_Attr,      dbFree, 
                              McStat))
          if (TestBug)
             TraceMsg (0, "       Array    XyCoords        Freeed\n");

          if (Normal(*Status))
          if (Attr0_ArrayDeActivate
                             (&RefTkn,   Points_Attr,        dbFree, 
                              McStat))
          if (TestBug)
             TraceMsg (0, "       Array    Points          Freeed\n");

          if (Normal(*Status))
          if (Attr0_ArrayDeActivate
                             (&RefTkn,   Byte_Array_Attr,    dbFree, 
                              McStat))
          if (TestBug)
             TraceMsg (0, "       Array    Byte_Array      Freeed\n");

          if (Normal(*Status))
          if (Attr0_ArrayDeActivate
                             (&RefTkn,   Yarray_Attr,        dbFree, 
                              McStat))
          if (TestBug)
             TraceMsg (0, "       Array    Yarray          Freeed\n");
       }
    }

  }

  if (Normal(*Status))
  {
       if (IsActive)
       {
          ref0   = NullPtr;
          Object_GetImage (&RefTkn,     McStat,  (addrtype *)&ref0);
          StrPtrs  = NullPtr;

          if (TestBug)
          Object_Dump          (&RefTkn);
       }

       if (Normal(*Status))
       if (TestBug)
           TraceMsg (0, "\n   Assert New Size of Char_Ptr_Array.\n");


#ifdef __BAD_CODE__
       if (Normal(*Status))
       if (Attr_ArrayAssertSize
                             (&RefTkn,   Char_PtrArray_Array_Attr,    
                              NumNewNames,
                              McStat,           (addrtype *)&StrPtrs))
       if (TestBug)
       if (TraceMsg (0, "     Char_PtrArray_Array Expanded.\n"))

       if (TraceMsg (0, "   ReAssert Size of Char_PtrArray_Array[0:9].\n"))
           TraceMsg (0, "            And Assign New set of Names.\n");


       for (j= -1; Normal(*Status) && ++j < NumNewNames; )
       {
          if (Attr_SubArrayAssertSize
                             (&RefTkn,   Char_PtrArray_Array_Attr,   j, 
                              sizeof (nametype),
                              McStat,           (addrtype *)&StrAddr))
          {
             strcpy (StrAddr, NewNames[j]);

             sprintf (Msg, "     Char_PtrArray_Array[%2u] = %s.\n",
                      j, NewNames[j]);
             if (TestBug)
             TraceMsg (0, Msg);
          }
       }
       if (Normal(*Status))
       {
           if (TestBug)
           TraceMsg (0, "  Char_PtrArray_Array  'resize of'       PASSED.\n");
       } else 
           TraceMsg (0, "  Char_PtrArray_Array  'resize of'       FAILED.\n");
#endif /* __BAD_CODE__ */

       if (Normal(*Status))
       if (TestBug)
           TraceMsg (0, "\n   Delete XyCoords[0] SubArray.\n");

       if (Normal(*Status))
           Attr_SubArrayDelete
                             (&RefTkn,   XyCoords_Attr,  0,  McStat);
       if (Normal(*Status))
       if (IsActive)
       {
          if (ref0->XyCoords[0] != NullPtr)
           TraceMsg (0, "  XyCoords           'SubArrayDelete'    FAILED.\n");
          else if (TestBug)
           TraceMsg (0, "  XyCoords           'SubArrayDelete'    PASSED.\n");

       } else {
          if (Attr0_ArrayActivate
                             (&RefTkn,   XyCoords_Attr,    
                              McStat,   &UnitCount,      &UnitSize,
                             (addrtype *)&XyCoordPtrs))
          {
             if (XyCoordPtrs[0] != NullPtr)
                TraceMsg (0, 
                        "  XyCoords           'SubArrayDelete'    FAILED.\n");
             else if (TestBug)
                TraceMsg (0,
                        "  XyCoords           'SubArrayDelete'    PASSED.\n");
          } else
           TraceMsg (0, "  XyCoords           'SubArrayDelete'    FAILED.\n");
       }

       if (Normal(*Status))
       if (TestBug)
           TraceMsg (0, "\n   Delete Byte_Array   Array.\n");

       if (Normal(*Status))
           Attr_ArrayDelete  (&RefTkn,   Byte_Array_Attr,    McStat);

       if (Normal(*Status))
       if (Attr_ValueGet     (&RefTkn,   Byte_Array_Attr,    
                              McStat,    (addrtype )&Bytes))
       if (Bytes  != NullPtr)
          if (IsActive)
          {
           if (TestBug)
           TraceMsg (0, "  Byte_Array         'SubArrayDelete'    PASSED.\n");
          } else
           TraceMsg (0, "  Byte_Array         'SubArrayDelete'    FAILED.\n");
       else if (TestBug)
           TraceMsg (0, "  Byte_Array         'SubArrayDelete'    PASSED.\n");

/* 00. COMMIT                                                             */
       if (Normal(*Status))
       if (IsActive)
       {
          if (TestBug)
          TraceMsg (0, "\n Commit Object with ReSized Arrays...\n");

          Object_CommitImage (&RefTkn,   True,       McStat);
       }

/* 00. DUMP                                                               */
       if (Normal(*Status))
       if (TestBug)
       {
          TraceMsg (0, "\n Display contents of Revised Object Arrays...\n");
          Object_Dump          (&RefTkn);
       }
  }

  sprintf  (Msg, "\n END of GETs and PUTs TEST... IsActive = %s.\n\n",
            BoolStr[IsActive]);
  if (TestBug)
  TraceMsg (0, Msg);

  if (Normal(*Status))
  if (Env_DeleteFieldStruc    (McStat,  &LinkField0))
  if (Env_DeleteFieldStruc    (McStat,  &LinkField1))
      Env_DeleteFieldStruc    (McStat,  &LinkField2);

TRACK(TrackBak,"Draw701\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF TESTOBJ.C                               !
  +-----------------------------------------------------------------------+*/
