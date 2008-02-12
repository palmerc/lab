/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ******************************  Draw7.c    ***************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       1.7   Draw7 - Test of all Attribute types allowable.            */
/* *                     (i.e direct access to the Image of the Object)    */
/* *********************************************************************** */

#define  DRAW7

#include "defines.h"
#include "typedefs.h"
#include "draw07.h"
#include "query.h"


        boolean Draw7    (tokentype    *Anchor,   boolean       TestBug,
                          numtype       Option,   ft F,lt Z,zz *Status);

extern  boolean Draw701  (tokentype    *Anchor,   boolean       TestBug,
                          numtype       Option,   ft F,lt Z,zz *Status);

#define Draw7_JumpToEnd   999
extern  longtype  TestObj_Asserts;
extern  longtype  TestObj_NullAsserts;

/*+-----------------------------------------------------------------------+
~C                           DRAW 7                                       !
  +-----------------------------------------------------------------------+*/
boolean Draw7            (tokentype    *Anchor,   boolean       TestBug,
                          numtype       Option,   ft F,lt Z,zz *Status)
{
/*
  if Option == 0  ::  create 0
            Draw7   (0);    does Query on all existing TestObj objects
                      Note :: this is called from BMT_QueryOn

  if Option == 1  ::  create 1
            Draw7   (1);    create TestObj and GET image and populate/test
                            with Gets and Puts on the image.
                            and do a Query on the object

  if Option == 2  ::  create 3
            Draw701 (1);
            Draw701 (2);
            Draw7   (0);    and do a Query on last testobj

  if Option == 3  ::  create 1
            Draw701 (1);    create TestObj and do in place transactions
                            with Gets and Puts

  if Option == 4  ::  create 1                    
            Draw701 (2);    create TestObj and do transactions
                            with Gets and Puts on the image.
*/
tokentype      LibTkn000    = *Anchor;

tokentype      RefTkn       = NullToken;
TestObj       *ref0         = NullPtr;
TestObj       *ref1         = NullPtr;
int            i            = 0;
int            j            = 0;
int            k            = 0;

char         **StrPtrs      = NullPtr;

boolean        Assert       = False;
static 
nametype       str1         = "NameType";
numtype        val0         = 6;
numtype        val1         = 5;

tokentype      CurrentTkn   = NullToken;
numtype        NumEntrys    = 0;
tokentype      Vtoken       = NullToken;
tokentype     *ResultsTkns  = NullPtr;



static dbmsquery          *Query0       = NullPtr;
static fieldstruc         *LinkField0   = NullPtr;
static fieldstruc         *LinkField1   = NullPtr;
static fieldstruc         *LinkField2   = NullPtr;


static
numtype        NumNames     = 10;
static  char  *Names[10]    = {"FullFix",     "Pre1111",        "Q2Mid123",
                              "Pre12Mid12",  "Q1Post",         "Pre12Post", 
                              "Q2Mid123Post","Pre12Mid123Post",
                              "wildstring",  "stringwild"};

char          *StrAddr      = NullPtr;

short         *ShortArray   = NullPtr;

char                       Char_Scalar           = 'A';
static 
nametype                   Char_InStruct         = "nametype string";
unsigned short             ShortU_Scalar         = 10;
signed                     Int_Scalar            = -11;
unsigned                   IntU_Scalar           = 12;
lt64                       Long_Scalar           = -13;
longtype                   LongU_Scalar          = 14;
float                      Float_Scalar          = 15.15;
double                     Double_Scalar         = 16.16;
tokentype                  Token_Scalar          = *Anchor;
handletype                 Handle_Scalar         = 18;
short                      Xarray[5]             = {1,2,3,4,5};
static short               XyCoords[2][5]        = { {-1,-2,-3,-4,-5},
                                                     {6,7,8,9,10} };
colortype                  Color                 = aGreen;
static XyPoint             XyOrigin              = {101,102};
static Connection          link[3]               = { {1, "link1", 10},
                                                     {2, "link2", 20},
                                                     {3, "link3", 30} };

tokentype                  OriginTkn             = NullToken;
CartesianPoint            *Origin                = NullPtr;
XyPoint                   *Points                = NullPtr;
boolean                    IsActive              = True;
bytetype                  *Bytes                 = NullPtr;
static bytetype            Byte_Array[5]         = {10,20,30,40,50};
short                     *Yarray                = NullPtr;
short                     *xArray                = NullPtr;
int                       *Xcoords               = NullPtr;
vchunktoken                XyDirTkn              = NullToken;                
int                       *XyDir                 = NullPtr;
tokentype                  BitMapTkn             = NullToken;
Point                     *RefOrigin             = NullPtr;

numtype       GrpNum        = 0;
grpheader    *GrpHdr        = NullPtr;
numtype       GrpIndex      = 0;
sizetype      EntrySize     = sizeof (XyPoint);
numtype       RgnHndls      = 1000;
numtype       AllocRgns     = (RgnHndls / 25) + 27;
numtype       RgnEntrys     = 25;
numtype       AllocXs       = 1000;
numtype       BaseIndex     = 0;
accesstype    AccessType    = Read_Write;
boolean       DiskCache     = False;
numtype       CacheLimit    = 10;
numtype       CacheLoad     = 10;
numtype       NumBuffRgns   = 10;
numtype       BaseRegion    = 0;
numtype       ObjCount      = 0;
static boolean FirstTime    = True;

  if (FirstTime)

  if (Draw7_Init        (McStat))
  if (Env_NewFieldStruc ("link[0]",           McStat,  &LinkField0))
  if (Env_NewFieldStruc ("link[1]",           McStat,  &LinkField1))
  if (Env_NewFieldStruc ("link[2]",           McStat,  &LinkField2))
  {
     FirstTime       = False;

     if (TestBug)
     if (TraceMsg (0, "\n Build a Query for <TestObj>::\n"))
         TraceMsg (0, "\n  if(link[1].length >= 3) ::\n");

     if (Query_Create         (McStat,      &Query0))
     if (Query_BeginBuild     (Query0,       McStat))
     if (Query1_ScopeOn       (Query0,      TestObj_Class,  EXCLUSIVE,  McStat))

     if (Query_OpenParen      (Query0,       McStat))

     if (Query_AffixCompare   (Query0,     "link[1].length",      aGTE, 
                               (addrtype )&val0,        McStat))

     if (Query_CloseParen     (Query0,       McStat))
     if (Query_EndBuild       (Query0,       McStat))
     if (ClassBug || Option < 10)
     if (TestBug)
         Query_Dump           (Query0);

  }


  if (Normal(*Status))
  {
/*+-----------------------------------------------------------------------+*/
/* TEST OBJ                                                                */
/*+-----------------------------------------------------------------------+*/
    if (TestBug)
    TraceMsg (0, "\n *** Begin test for TestObj\n");

/* Create an instance of this object in the DB                            */

    if (Option == 3)
    {
        Draw701  (Anchor,  TestBug,  1,    McStat);
       *Status  = Draw7_JumpToEnd;

    } else if (Option == 4) {
        Draw701  (Anchor,  TestBug,  2,    McStat);
       *Status  = Draw7_JumpToEnd;

    } else if (Option == 2)  {
       if (Draw701  (Anchor,  TestBug,  1,    McStat))
           Draw701  (Anchor,  TestBug,  2,    McStat);
    }
  }


  if (Option == 1
  ||  Option == 2)
  {
    if (TestBug)
       TraceMsg (0, "\n  Create Object and GET Image.\n");

    if (Normal(*Status))
    if ((ref0 = TestObj_new0      (&LibTkn000, McStat))!= NullPtr)
    {
       RefTkn                      = ref0->Token;
       if (RefTkn.Handle == 0) TestBug = True;
       if (TestBug)
       if (TraceMsg (0, "\n Dump Virgin Image\n"))
          Object_Dump  (&RefTkn);

       ref0->Char_Scalar           = Char_Scalar;

       strncpy ((char *)ref0->Char_InStruct, 
                (char *)Char_InStruct,        sizeof (nametype));

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
       {
          ref0->Short_Scalar       = -ShortU_Scalar;

          ref0->ShortU_Scalar      =  ShortU_Scalar;

          ref0->Int_Scalar         = Int_Scalar;

          ref0->IntU_Scalar        = IntU_Scalar;

          ref0->Long_Scalar        = Long_Scalar;

          ref0->LongU_Scalar       = LongU_Scalar;

          ref0->Float_Scalar       = Float_Scalar;

          ref0->Double_Scalar      = Double_Scalar;

          ref0->Token_Scalar       = Token_Scalar;

          ref0->Handle_Scalar      = Handle_Scalar;
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Xarray_Attr,    
                              5,                       sizeof (short),
                              McStat,           (addrtype *)&ShortArray))
       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         ShortArray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   XyCoords_Attr,    
                              2,                      sizeof (short *),
                              McStat,           (addrtype *)&ShortArray))
       for (j= -1; Normal(*Status) && ++j < 2; )
       {
          if (Attr_SubArrayCreate
                             (&RefTkn,   XyCoords_Attr,   j, 
                              5,                      sizeof (short),
                              McStat,           (addrtype *)&ShortArray))
          for (k= -1; Normal(*Status) && ++k < 5; )
          {
            ShortArray[k]  = XyCoords[j][k];
          }
       }

       if (Normal(*Status))
          ref0->Void_Ptr           = NullPtr;

       if (Normal(*Status))
       {
          ref0->Color              = Color;

          ref0->XyOrigin           = XyOrigin;

          ref0->link[0]            = link[0];
          ref0->link[1]            = link[1];
          ref0->link[2]            = link[2];
       }

       if (Normal(*Status))
       if (Object_NewImage   (&RefTkn,   CartesianPoint_Class,  McStat,
                              &OriginTkn,         (addrtype  *)&Origin))
       {
          Origin->xCoord  = 24;
          Origin->yCoord  = 240;

#ifdef Generic_Use
          Attr_ValuePut   (&RefTkn,   Origin_Attr, 
                           (addrtype )Origin,                McStat);
#else
          Attr_ObjTknPut  (&RefTkn,   Origin_Attr,          &Origin->Token,
                           McStat);
#endif
       }                                             

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Points_Attr,    
                              2,                      sizeof (XyPoint),
                              McStat,           (addrtype *)&Points))
       {
          Points[0].xCoord  = 26;
          Points[0].yCoord  = -26;

          Points[1].xCoord  = 126;
          Points[1].yCoord  = -126;
       }

       if (Normal(*Status))
          ref0->IsActive           = IsActive;

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Byte_Array_Attr,    
                              5,                      sizeof (bytetype),
                              McStat,           (addrtype *)&Bytes))
       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         Bytes[j]   = Byte_Array[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Yarray_Attr,    
                              5,                      sizeof (short),
                              McStat,           (addrtype *)&Yarray))
       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         Yarray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   xArray07_Attr,    
                              5,                      sizeof (short),
                              McStat,           (addrtype *)&xArray))

       if (Vchunk_PutStackPtr (&(ref0->xArray.VchunkTkn),  5, 
                              McStat,           (addrtype *)&xArray))
           ref0->xArray.Vsize  = 5;

       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         xArray[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Attr_ArrayCreate  (&RefTkn,   Xcoords_Attr,    
                              5,                      sizeof (int),
                              McStat,           (addrtype *)&Xcoords))
       if (Vchunk_PutStackPtr (&(ref0->Xcoords.VchunkTkn),  5, 
                              McStat,           (addrtype *)&Xcoords))
           ref0->Xcoords.VstackPtr  = 5;

       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         Xcoords[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (Vchunk_Create     (&RefTkn,   sizeof (int),     5,     10,
                              McStat,          &XyDirTkn))
       if (Attr_ValuePut     (&RefTkn,   XyDir_Attr,    
                             (addrtype )&XyDirTkn,                McStat))
       if (Vchunk_PutStackPtr(&XyDirTkn, 5,     McStat, (addrtype *)&XyDir))
       for (j= -1; Normal(*Status) && ++j < 5; )
       {
         XyDir[j]   = Xarray[j];
       }

       if (Normal(*Status))
       if (BitField_Create   (&RefTkn,   10,    McStat,          &BitMapTkn))
       if (Attr_ValuePut     (&RefTkn,   BitMap_Attr,    
                             (addrtype )&BitMapTkn,               McStat))
       for (j= -1; Normal(*Status) && ++j < 10; ++j)
       {
           BitField_Put      (&BitMapTkn,       j,     1,         McStat);
       }

       if (Normal(*Status))
       if (Attr_RefTknPut    (&RefTkn,   RefOrigin_Attr,          &OriginTkn,
                              McStat))    
       if (Attr_TokenToRef   (&RefTkn,   RefOrigin_Attr,
                              McStat,                 (addrtype *)&RefOrigin))    
       if (RefOrigin != (Point   *)Origin)
          TraceMsg (0, "  ***SWIzzle MisMatch on Reference Token value.\n");

       if (Normal(*Status))
       if (TestBug)
       if (GrpNewPacket      (&RefTkn,   EntrySize,   AllocRgns,   RgnEntrys,
                              AllocXs,   BaseIndex,   AccessType,  DiskCache,
                              CacheLimit,CacheLoad,   NumBuffRgns,
                              McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))
       if (Attr_ValuePut     (&RefTkn,   VpartsDir_Attr,(addrtype)&GrpNum, 
                              McStat))
       for (j= -1; Normal(*Status) && ++j < 100; ++j)
       {
          Grp_PutNewEntry    (&RefTkn,   GrpNum,       (addrtype )&j,
                              McStat,   &GrpIndex);
       }

       if (Normal(*Status))
       if (TestBug)
       {
          sprintf (Msg, " TestObj %4u created. Token[%3u:%8u] @%6x.\n",
                   i, ref0->Token.DbId, ref0->Token.Handle, ref0);
          TraceMsg (0, Msg);

          TraceMsg (0, "\n Current State of Image (before Commit)\n");
          Object_Dump        (&RefTkn);
          Object_Dump        (&OriginTkn);
       }
    }

    i = 0;
    if (Normal(*Status))
    {
       if (TestBug)
          TraceMsg (0, "\n  Commit TestObj (and sub-structs) to the DB.\n");

       if (TestObj_delete      (&ref0,       dbCommit,   McStat))
       if (Object_CommitImage  (&OriginTkn,  True,       McStat))
       if (TestBug)
       {
          TraceMsg (0, "\n Current State of Image (after Commit/release)\n");
          Object_Dump          (&RefTkn);
          Object_Dump          (&OriginTkn);
       }

    }

    if (Normal(*Status))
    {
       sprintf (Msg, "\n Retrieve TestObj #%4u From Db with RefTkn\n",i);
       if (TestBug)
       TraceMsg (0, Msg);

      if ((ref1  = TestObj_new1   (&RefTkn,     McStat))!= NullPtr)
      {
          if (RefTkn.DbId      != ref1->Token.DbId
          ||  RefTkn.Handle    != ref1->Token.Handle)
          {
              sprintf (Msg, 
         "\n ***ERROR...  [%4u:%8u] Retrieved as ref1->Token[%3u:%8u]\n",
                   RefTkn.DbId, RefTkn.Handle,
                   ref1->Token.DbId, ref1->Token.Handle);
              TraceMsg (0, Msg);
          } else if (TestBug) {
             sprintf (Msg, " TestObj Retrieved. Token[%3u:%8u] @%6x.\n",
                      ref1->Token.DbId, ref1->Token.Handle, ref1);
             TraceMsg (0, Msg);
          }

          if (TestBug)
          {
             Object_Dump    (&RefTkn);

             sprintf (Msg, " Current State of OriginTkn[%3u:%8u]\n",
                      OriginTkn.DbId, OriginTkn.Handle);
             TraceMsg (0, Msg);

             Object_Dump    (&OriginTkn);

             TraceMsg (0, "\n DeReference The RefOrigin Attribute Value\n");
          }
          if (Attr_TokenToRef   
                         (&RefTkn,   RefOrigin_Attr,
                          McStat,                 (addrtype *)&RefOrigin))
          if (TestBug)
          {
             sprintf (Msg, " State of OriginTkn[%3u:%8u] AFTER DeReferenced.\n",
                      ref1->RefOrigin.ObjTkn.DbId, 
                      ref1->RefOrigin.ObjTkn.Handle);
             TraceMsg (0, Msg);
             Object_Dump    (&ref1->RefOrigin.ObjTkn);
          }
          if (TestBug)
             TestObj_show          (ref1);
      }

      if (Normal(*Status))
      {  
          sprintf (Msg, "\n TestObj FREE. Token[%3u:%8u].\n",
                   ref1->Token.DbId, ref1->Token.Handle);
          if (TestBug)
             TraceMsg (0, Msg);
          if (LibTkn000.DbId  != PrimalTkn.DbId)
             TestObj_delete (&ref1, dbFree,  McStat);
      }

      if (Normal(*Status))
      if (TestBug)
          Object_Dump    (&RefTkn);

    }
  }

  if (Normal(*Status))
  if (Option == 2)
  {
     sprintf (Msg, "\n  Assert As :: %4u .%s.  %4u\n", 
              val0, CompareOpName[aGTE], val1);
     if (TestBug)
        TraceMsg (0, Msg);

     Assert = Query_AssertOnObject
                       (&RefTkn,   Query0,         McStat);

     sprintf (Msg, "\n Assert on Object [%4u:%6u] = %s\n",
              RefTkn.DbId, RefTkn.Handle, BoolStr[Assert]);
     if (TestBug)
        TraceMsg (0, Msg);

     if (Normal(*Status))
       *Status  = Draw7_JumpToEnd;
  }

 /* QUERY on instantiated TestObj's to compare with poet times.          */
  if (Option == 0)
  if (Normal(*Status))
  {

     if (TestBug)
     if (sprintf  (Msg, "\n Query_AssertOnDb [%3u:%8u] ::\n",
                   LibTkn000.DbId, LibTkn000.Handle))
        TraceMsg (0,   Msg);

     if (Primal_GetClassObjectCount
                              (&LibTkn000,  TestObj_Class,
                               McStat,     &ObjCount))
     if (Query_AssertOnDb     (&LibTkn000,  Query0,     
                               McStat,     &NumEntrys, &Vtoken))
     TestObj_Asserts      += NumEntrys;
     TestObj_NullAsserts  += (ObjCount - NumEntrys);

     j        = -1;

     if (Normal(*Status))
     if (NumEntrys)
     if (TestBug)
     if (sprintf  (Msg, "\n Activate/Free Vchunk [%3u:%8u] references ::\n",
                   Vtoken.DbId, Vtoken.Handle))
        TraceMsg (0,   Msg);

     if (Normal(*Status))
     if (NumEntrys)
     if (Vchunk_GetAddress    (&Vtoken,     
                               McStat,     (addrtype *)&ResultsTkns))
     while (Normal(*Status)
     &&   ++j     < NumEntrys)
     {    
        CurrentTkn      = ResultsTkns[j];

        if (TestBug)
        {
           sprintf (Msg, " Query Result for ItNum=%4u. Token[%3u:%8u].\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle);
           TraceMsg (0, Msg);

           if (Object_GetImage  (&CurrentTkn,  McStat,  (addrtype *)&ref1))

           if (TestBug)
           if (j == 0 || j % 1 == 0)
           {
              sprintf (Msg, " ItNum %4u. Token[%3u:%8u]. Char_InStruct= %s\n",
                       j+1, CurrentTkn.DbId, CurrentTkn.Handle,
                       ref1->Char_InStruct);
              TraceMsg (0, Msg);
           }
           if (Normal(*Status))
           if (LibTkn000.DbId  != PrimalTkn.DbId)
              Object_FreeImage  (&CurrentTkn,  McStat);
        }
     }

     if (Normal(*Status))
     if (Vchunk_IsValidToken (&Vtoken, McStat))
        Vchunk_Delete (&Vtoken,  McStat);
  }

  if (*Status  == Draw7_JumpToEnd)
      *Status   = Env_Normal;

  if (Normal(*Status))
  if (TestBug)
  {
    TraceMsg (0, "\n Commit all objects that were instantiated\n");

    if (PrimalTkn.DbId != LibTkn000.DbId)
       Primal_CommitDb  (&LibTkn000,   "\0", True,   McStat);

  }

TRACK(TrackBak,"Draw7\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF DRAW7.C                                  !
  +-----------------------------------------------------------------------+*/
