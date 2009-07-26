/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* EDB-BMT.h */

#ifndef BMT_01
#define BMT_01
/*
                  Engineering Database Benchmark
                                by
                    'R. Catell' &&   'J. Skeen'
*/
#include "defines.h"
#include "typedefs.h"
#include "obj01.h"
#include "vchunk.h"
#include "grp.h"
#include "query.h"

#define DRAWLIB_IN_LINK

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
typedef   longtype        partidtype;
typedef   char            parttype[10];
typedef   longtype        datetype;

#define   Random(Limit)   (RANDOM_FUNC () % Limit)

/* .......................................................................
                   ....   VIRTUAL PARTS DIR ....
   .......................................................................*/
typedef struct TypePartToken
{
  handletype            Handle;
  handletype            FromList; 

}  parttkntype;



/* .......................................................................
                    ....       PARTLIB     ....
   .......................................................................*/


typedef struct PartLib_Type
{
/*
// public:
// attributes inherited from Image01
*/
   addrtype      That;
   tokentype     Token;
   numtype       DbNum;
   envclass      EnvType;
   numtype       DescNum;
   slottoken     PrimalSlotAttr;
   slottoken     PrimalGroupSlotAttr;

/*
// public:
*/
   intarraytype  PartsListHead;
   numtype       VpartsDir;
   numtype       VlinksDir;

   ownertoken    OwnerTosFroms;

} PartLib;


/* .......................................................................
                    ....     CONNECTION    ....
   .......................................................................*/

typedef struct Connection_Type
{
/*
// public:
// attributes of a unique 'part A connected to a part B' instance
*/
   partidtype    to;
   parttype      type;
   longtype      length;

} Connection;

/* .......................................................................
                    ....       PART        ....
   .......................................................................*/
typedef struct Part2
{
/*
// public:
// attributes inherited from Image01
*/
   addrtype      That;
   tokentype     Token;


/*
// public:
// attributes of a unique part instance
*/
   partidtype    id;
   parttype      type;
   longtype      xx;
   longtype      yy;
   datetype      build;

/*
// private:
// Connectors for the 3 explicit <to>'s by 'id' 
*/
   Connection    link[3];

/*
// an array of Parts that 'point' (are connected) to this part
*/
   tokentype     FromList;

   ownertoken    RowHeadTosFroms;
   ownertoken    ColHeadTosFroms;

   slottoken     DrawObjSlotAttr;

   membertoken   MemberPersonParts;

} Part2;


typedef struct TypePartLink
{
  indextype      NextLink; 
  partidtype     id;

}  partlinktype;



#ifdef BMT01
static dbmsquery *Query0             = NullPtr;
static numtype    val0               = 5;
static numtype    Exports            = 0;

static dbmsquery *Query2             = NullPtr;
static char      *StateNam1          = "CA";
static char      *StateNam2          = "T*";

static dbmsquery *Query1             = NullPtr;
static char      *NameComp0          = "H*";              /* FullFix     */
static char      *NameComp1          = "P*";              /* FullFix     */
       numtype    Person_Asserts     = 0;
       numtype    Person_NullAsserts = 0;


static dbmsquery *Query3             = NullPtr;
static numtype    GTEval1            = 3;
static numtype    GTEval2            = 4;
static numtype    GTEval3            = 5;
static numtype    LTEval             = 10;
       numtype    DrawObj_Asserts    = 0;
       numtype    DrawObj_NullAsserts= 0;

static dbmsquery *Query4             = NullPtr;
static char      *GTEnam1            = "Pre*";            /* PreFix      */
static char      *GTEnam2            = "??Mid???";        /* MidFix      */
static char      *GTEnam3            = "Pre??Mid??";      /* PreMidFix   */
static char      *GTEnam4            = "??Post";          /* PostFix     */
static char      *GTEnam5            = "Pre??Post";       /* PrePostFix  */
static char      *GTEnam6            = "??Mid???Post";    /* MidPostFix  */
static char      *GTEnam7            = "Pre??Mid???Post"; /* PreMidPost  */
static numtype    LTEnames           = 7;
       numtype    NamedObj_Asserts   = 0;
       numtype    NamedObj_NullAsserts = 0;

#endif /*  BMT01 */



#ifdef BMT01
       boolean   DeleteDrawObjects   = False;
       boolean   DeletePartObjects   = False;
       numtype   id_Attr0            = 0;
       numtype   PartLib_Class       = 0;
       numtype   PartsListHead_Attr  = 0;

#ifndef DRAWLIB_IN_LINK
       numtype   VpartsDir_Attr      = 0;
       numtype   VlinksDir_Attr      = 0;

       numtype   DrawPart_Tupl       = 0;
       numtype   DrawObj_Slot        = 0;
       numtype   PartObj_Slot        = 0;

       numtype   Connection_Class    = 0;
       numtype   to_Attr             = 0;
       numtype   type_Attr           = 0;
       numtype   length_Attr         = 0;
       numtype   MemberPersonParts_Attr = 0;

#else
extern numtype   VpartsDir_Attr;
extern numtype   VlinksDir_Attr;

extern numtype   Connection_Class;
extern numtype   to_Attr;
extern numtype   type_Attr;
extern numtype   length_Attr;
extern numtype   id_Attr;
extern numtype   MemberPersonParts_Attr;

#endif /* DRAWLIB_IN_LINK */

       numtype   Part_Class          = 0;
       numtype   xx_Attr             = 0;
       numtype   yy_Attr             = 0;
       numtype   build_Attr          = 0;
       numtype   c1_Attr             = 0;
       numtype   c2_Attr             = 0;
       numtype   c3_Attr             = 0;
       numtype   FromList_Attr       = 0;
       numtype   TosFroms_Mtrx       = 0;
       numtype   OwnerTosFroms_Attr  = 0;
       numtype   RowHeadTosFroms_Attr= 0;
       numtype   ColHeadTosFroms_Attr= 0;
       numtype   DrawObjSlotAttr_Attr= 0;

       numtype   PartLink_Class      = 0;
       numtype   NextLink_Attr       = 0;

/*
       numtype   INFO_Attr           = 0;
       numtype   Generic_Class       = 0;
       numtype   Ints_Attr           = 0;
       numtype   IntLengths_Attr     = 0;
       numtype   Reals_Attr          = 0;
       numtype   RealLengths_Attr    = 0;
       numtype   Dbls_Attr           = 0;
       numtype   DblLengths_Attr     = 0;
       numtype   Chars_Attr          = 0;
       numtype   CharLengths_Attr    = 0;
       numtype   CharSizes_Attr      = 0;
*/
       numtype   Part_Map            = 0;     
       numtype   PartId_Field        = 0;
       numtype   type_Field          = 0;
       numtype   x_Field             = 0;
       numtype   y_Field             = 0;
       numtype   date_Field          = 0;
       numtype   c1To_Field          = 0;
       numtype   c1Type_Field        = 0;
       numtype   c1Len_Field         = 0;
       numtype   c2To_Field          = 0;
       numtype   c2Type_Field        = 0;
       numtype   c2Len_Field         = 0;
       numtype   c3To_Field          = 0;
       numtype   c3Type_Field        = 0;
       numtype   c3Len_Field         = 0;
       numtype   FromList_Field      = 0;

       PartLib  *PartLib01           = NullPtr;
       tokentype LibTkn              = Null_Token;
       int      *PartsTbl            = NullPtr;
       tokentype PartTblTkn          = Null_Token;
       numtype   VlinksDir           = 0;
       tokentype VlistTkn            = Null_Token;
       indextype NumVlists           = 0;
       numtype  *Vlists              = NullPtr;

#ifndef DRAWLIB_IN_LINK
       numtype   VpartsDir           = 0;
       longtype  PartCount           = 0;

#else
extern numtype   VpartsDir;
extern longtype  PartCount;

#endif /* DRAWLIB_IN_LINK */

       tokentype ApiTestTkn      = Null_Token;
       tokentype GenLibTkn       = Null_Token;

       PartLib  *PortLib01       = NullPtr;
       tokentype PortLibTkn      = Null_Token;
       int      *PortsTbl        = NullPtr;
       tokentype PortTblTkn      = Null_Token;
       longtype  PortCount       = 0;


       char     *ConnectType     = { "contype00" };     
       char     *TypeName[]      = {"parttype0",  "parttype1",  "parttype2",
                                    "parttype3",  "parttype4",  "parttype7",
                                    "parttype6",  "parttype7",  "parttype8",
                                    "parttype9"};

       indextype AllocParts      = 0;
       indextype FromCount       = 10;
       indextype ExtendQty       = 5;

       boolean   SwapType        = False;
       longtype  MaxParts        = 0;
       longtype  OneCent         = 0;
       longtype  HalfCent        = 0;
       longtype  Traverse_Limit  = 7;
       unsigned  int   CurrentId = 0;
       indextype OldParts        = 0;
       unsigned int   StartCommit= 0;
       longtype  LookUp_Asserts  = 0;
       longtype  LookUp_NullAsserts  = 0;
       longtype  Part_Asserts    = 0;
       longtype  Part_NullAsserts= 0;

       longtype  TestObj_Asserts = 0;
       longtype  TestObj_NullAsserts = 0;

       boolean   ExportParts     = False;
       boolean   ExportGeneric   = False;

       numtype   NumPersons      = 0;
       numtype   Parts           = 0;
       numtype   DrawObjs        = 0;
       numtype   NamedObjs       = 0;
       numtype   Points          = 0;
       numtype   Persons         = 0;

static numtype   Iter1_Count     = 0;
static numtype   Iter2_Count     = 0;
static numtype   Iter3_Count     = 0;
static numtype   Iter4_Count     = 0;

#else
extern boolean   DeleteDrawObjects;
extern boolean   DeletePartObjects;
extern numtype   Parts;
extern numtype   DrawObjs;
extern numtype   NamedObjs;
extern numtype   Points;
extern numtype   Persons;

extern longtype  Ctest_Count;

extern numtype   PartLib_Class;
extern numtype   PartsListHead_Attr;
extern numtype   VlinksDir_Attr;

extern numtype   VpartsDir_Attr;

extern numtype   Connection_Class;
extern numtype   to_Attr;
extern numtype   type_Attr;
extern numtype   length_Attr;

extern numtype   Part_Class;     
extern numtype   id_Attr0;
extern numtype   xx_Attr;
extern numtype   yy_Attr;
extern numtype   build_Attr;
extern numtype   c1_Attr;
extern numtype   c2_Attr;
extern numtype   c3_Attr;
extern numtype   FromList_Attr;
extern numtype   TosFroms_Mtrx;
extern numtype   OwnerTosFroms_Attr;
extern numtype   RowHeadTosFroms_Attr;
extern numtype   ColHeadTosFroms_Attr;
extern numtype   DrawObjSlotAttr_Attr;

extern numtype   DrawPart_Tupl;
extern numtype   DrawObj_Slot;
extern numtype   PartObj_Slot;

extern numtype   PartLink_Class;     
extern numtype   NextLink_Attr;
extern numtype   INFO_Attr;
extern numtype   Generic_Class;     
extern numtype   Ints_Attr;
extern numtype   IntLengths_Attr;
extern numtype   Reals_Attr;
extern numtype   RealLengths_Attr;
extern numtype   Dbls_Attr;
extern numtype   DblLengths_Attr;
extern numtype   Chars_Attr;
extern numtype   CharLengths_Attr;
extern numtype   CharSizes_Attr;
extern numtype   Part_Map;     
extern numtype   PartId_Field;
extern numtype   type_Field;
extern numtype   x_Field;
extern numtype   y_Field;
extern numtype   date_Field;
extern numtype   c1To_Field;
extern numtype   c1Type_Field;
extern numtype   c1Len_Field;
extern numtype   c2To_Field;
extern numtype   c2Type_Field;
extern numtype   c2Len_Field;
extern numtype   c3To_Field;
extern numtype   c3Type_Field;
extern numtype   c3Len_Field;
extern numtype   FromList_Field;

extern PartLib  *PartLib01;
extern tokentype LibTkn;
extern numtype   VpartsDir;
extern numtype   VlinksDir;
extern int      *PartsTbl;
extern tokentype PartTblTkn;
extern longtype  PartCount;
extern tokentype VlistTkn;
extern indextype NumVlists;
extern numtype  *Vlists;

extern tokentype ApiTestTkn;
extern tokentype GenLibTkn;
extern PartLib  *PortLib01;
extern tokentype PortLibTkn;
extern int      *PortsTbl;
extern tokentype PortTblTkn;
extern longtype  PortCount;

extern char     *ConnectType;
extern char     *TypeName[];

extern indextype AllocParts;
extern indextype FromCount;
extern indextype ExtendQty;

extern boolean   SwapType;
extern longtype  MaxParts;
extern longtype  OneCent;
extern longtype  HalfCent;
extern longtype  Traverse_Limit;
extern unsigned int   CurrentId;
extern indextype OldParts;
extern unsigned int   StartCommit;
extern numtype   NumPersons;

extern longtype  LookUp_Asserts;
extern longtype  LookUp_NullAsserts;
extern longtype  Part_Asserts;
extern longtype  Part_NullAsserts;
extern longtype  TestObj_Asserts;
extern longtype  TestObj_NullAsserts;
extern longtype  DrawObj_Asserts;
extern longtype  DrawObj_NullAsserts;
extern longtype  NamedObj_Asserts;
extern longtype  NamedObj_NullAsserts;
extern longtype  Person_Asserts;
extern longtype  Person_NullAsserts;

extern boolean   ExportParts;
extern boolean   ExportGeneric;
extern longtype  Ctest_Count;

#endif /*  BMT01 */

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....       PARTLIB     ....
   .......................................................................*/
         boolean PartLib_Init
                         (ft F,lt Z,zz *Status);

         void    PartLib_Topology    
                         (void);

         boolean PartLib_Create 
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,  boolean       UseVparts,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, PartLib     **PrimalObj);

         boolean PartLib_ActivateDb
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          ft F,lt Z,zz *Status,    PartLib     **PrimalObj);


/* .......................................................................
                    ....       PART        ....
   .......................................................................*/
         boolean DrawPartPair_InitClass 
                         (ft F,lt Z,zz *Status);

         boolean Part_InitClass
                         (ft F,lt Z,zz *Status);

         void    Part_Topology    
                         (void);

         boolean Part_Create
                         (tokentype    *AnchorTkn,
                          partidtype    Id,      longtype    X,
                          longtype      Y,       longtype    TypeNum,
                          ft F,lt Z,zz *Status,  tokentype  *PartTkn);

         boolean Part_GetToken  
                         (tokentype    *Anchor,  partidtype    PartId,
                          ft F,lt Z,zz *Status,  tokentype    *Token);

         boolean Part_Connect
                         (tokentype    *PartTkn, partidtype  FromPart,
                          ft F,lt Z,zz *Status);

         boolean Part_Traverse  
                         (tokentype    *PartTkn, longtype    Level,
                          ft F,lt Z,zz *Status);

         boolean Part_Reverse
                         (tokentype    *PartTkn, longtype    Level,
                          ft F,lt Z,zz *Status);


         boolean Part_ExportTo
                         (tokentype    *PartTkn,
                          tokentype    *ApiTkn,  void         *FOId,
                          ft F,lt Z,zz *Status);

         boolean Part_ImportFrom
                         (tokentype    *ApiTkn,  void         *FOId,
                          tokentype    *PartTkn, ft F,lt Z,zz *Status);

         boolean Part_DisConnect 
                         (tokentype    *PartTkn, ft F,lt Z,zz *Status);

         boolean Part_Delete 
                         (tokentype    *PartTkn, ft F,lt Z,zz *Status);

/* .......................................................................
                    ....       BMT         ....
   .......................................................................*/
         boolean BMT_Test 
                         (char         *EdbName, char         *EdbFileName,
                          char         *DrwName, char         *DrwFileName,
                          char         *EmpName, char         *EmpFileName,
                          int           Count,   int           Loop,
                          int           Samples, 
                          int           LookUps, int           Deletes,
                          int           Stuffs,  int           TraverseLimit,
                          int           PartPct, int           LookUpPct, 
                          int          DeletePct,int           StuffPct,
                          int        DeleteDraws,int           DeleteParts,
                          boolean      CommitDbs,ft F,lt Z,zz *Status);

         boolean BMT_Init  
                         (char         *EdbName, char         *EdbFileName,
                          char         *DrwName, char         *DrwFileName,
                          char         *EmpName, char         *EmpFileName,
                          longtype      Count,   ft F,lt Z,zz *Status);


         boolean BMT_ConfigAllClasses
                         (numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          ft F,lt Z,zz *Status);

         boolean BMT_CreateParts  
                         (longtype      Count,   ft F,lt Z,zz *Status);

         boolean BMT_LookUpParts  
                         (longtype      LookUps, ft F,lt Z,zz *Status);

         boolean BMT_DeleteParts    
                         (longtype      Deletes, ft F,lt Z,zz *Status);

         boolean BMT_CommitParts  
                         (boolean       Release, ft F,lt Z,zz *Status);


         boolean BMT_DeleteAllObjects
                         (ft F,lt Z,zz *Status);

         boolean BMT_DeleteTestObjs
                         (ft F,lt Z,zz *Status);

         boolean BMT_Validate
                         (tokentype    *PartTkn, ft F,lt Z,zz *Status);

         boolean BMT_ValidateNamedDrawObjs
                         (ft F,lt Z,zz *Status);


         boolean BMT_QueryOn 
                         (tokentype    *PartTkn, boolean       TraceOn,
                          ft F,lt Z,zz *Status);

         boolean BMT_DbTransaction
                         (ft F,lt Z,zz *Status);


         boolean BMT_CommitPartDrawObj
                         (tokentype    *PartTkn, boolean       Release,
                          ft F,lt Z,zz *Status);

         boolean BMT_DeletePartDrawObj
                         (tokentype    *PartTkn, ft F,lt Z,zz *Status);

         boolean BMT_TraverseSets
                         (ft F,lt Z,zz *Status);

/* Iteration for  DB RECTANGLES   LIST                                    */
         boolean BMT_Iter1
                         (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);

/* Iteration for  PERSON NAMES    SET                                     */
         boolean BMT_Iter2
                         (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);

/* Iteration for  DRAW OBJS       SET                                     */
         boolean BMT_Iter3
                         (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);

/* Iteration for NAMED DRAW OBJS  SET                                     */
         boolean BMT_Iter4
                         (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);


         boolean BMT_ExportPart 
                         (longtype      X,       longtype      Y,
                          parttype      Type,    ft F,lt Z,zz *Status);

         boolean BMT_ImportPart 
                         (longtype     *X,       longtype     *Y,
                          ft F,lt Z,zz *Status);



         boolean BMT_BuildPersonLib
                         (tokentype    *LibToken,numtype       MaxPersons,
                          ft F,lt Z,zz *Status);

         boolean BMT_InitQuerys
                         (ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~M                         END OF METHODS                                 !
  +-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+
~P                         END OF BMT01.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
