/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/*                              QUERY.H                                    */
#ifndef QUERY_H
#define QUERY_H

#include "bitvec.h"

#ifndef ENV1_H

typedef struct TypeTypeDesc 
{
      handletype     Handle;
      numtype        TypeNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      char           AliasNam[MAXNAME];
      numtype        DescNum;
      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
}              typedesctype;
 
#endif

#define  EXCLUSIVE   False
#define  INCLUSIVE   True
/*+-----------------------------------------------------------------------+
~T                             TYPEDEFS                                   !
  +-----------------------------------------------------------------------+*/
typedef enum   ScopeOpType
               {aEXclusive,         aINclusive,
                aNonScopeOp=EXT_ENUM                          } scopeoptype;
typedef scopeoptype      scopeop;  

typedef enum   QueryOpType
               {aIsA,               aIsKindOf,
                aCompareOp,         aSubQuery,
                aBooleanOp,
                aPushBool,          aPopBool,
                aJumpSwi,           aEndQuery,       aClearStack,
                aBeginExp,          aEndExp,
                aNonQueryOp=EXT_ENUM                          } queryoptype;
typedef queryoptype      queryop;  


typedef struct QuerySortType  
{
   indextype      SeqNum;         

   numtype        BaseAttr;       
   sizetype       BaseOffset0;    
   sizetype       BaseOffset1;    

   numtype        FieldAttr;      
   sizetype       FieldOffset0;   
   sizetype       FieldOffset1;   

   attrtype       AttrType;       
   typetype       UnitType;       
   sizetype       UnitSize;       
   valueclass     ValueClass;     

   sortorder      SortOrder;      

}             querysort;


typedef struct QueryFieldType
{
   indextype      SeqNum;         

   numtype        BaseAttr;       
   sizetype       BaseOffset0;    
   sizetype       BaseOffset1;    

   numtype        FieldAttr;      
   sizetype       FieldOffset0;   
   sizetype       FieldOffset1;   

   attrtype       AttrType;       
   typetype       UnitType;       
   sizetype       UnitSize;       
   valueclass     ValueClass;     

   compareop      CompOp;         

   sizetype       ValueOffset;    

   boolean        IsValueAssert;  
                                  
                                  

}             queryfield;


typedef struct QueryComdType  
{
   indextype      SeqNum;         

   queryop        ComdType;       

   numtype        QueryField;     

   booleanop      BoolOp;         
   numtype        TrueJump;       
   numtype        FalseJump;      

}             querycomd;


typedef struct DbmsQueryType
{
   typedesctype  *EnvType;        

   numtype        ClassId;        
   boolean        MapIsActive;    

   boolean        CendentsOk;     
   numtype        ClassStack;     
                                  
   numtype        NumClasses;     
   numtype       *ClassSet;       

   numtype        SortStack;      
   numtype        NumSorts;       
   querysort     *SortSet;        

   numtype        ComdStack;      
   numtype        NumComds;       
   querycomd     *ComdSet;        

   numtype        FieldStack;     
   numtype        NumFields;      
   queryfield    *Fields;         

   numtype        ValueStack;     
   numtype        NumValues;      
   bytetype      *Values;         

   numtype        NumOpens;       
   numtype        OpenComds[10];  
   numtype        NumCloses;      
   numtype        CloseComds[10]; 


   bitvectype    *LocalBitVec;    
                                  

   tokentype      ResultsDir;     
   numtype        NumResults;     

   numtype        SortTknsDir;    

}              dbmsquery;
typedef  dbmsquery  valuequery;

/*+-----------------------------------------------------------------------+
~V                             VARIABLES                                  !
  +-----------------------------------------------------------------------+*/
#ifdef QUERY
static  fieldstruc   EnvField;
static  char        *FieldOps          = {".["};
static  boolean      Query_TypeIsToken = False;

        char        *ScopeOpName[ 2]   = {
                                         "aEXclusive",       "aINclusive" };

        char        *QueryOpName[12]  = {
                                         "aIsA",             "aIsKindOf", 
                                         "aCompareOp",       "aSubQuery",
                                         "aBooleanOp",
                                         "aPushBool",        "aPopBool",
                                         "aJumpSwi",         "aEndQuery",
                                         "aClearStack",      "aBeginExp",
                                         "aEndExp"      };

static   wordtype    OprVsStateTbl[10][20] = {

    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0,10, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0,10, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0,10, 0, 0,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 0, 0, 0},

    {0, 0, 7, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

    {0, 0, 6, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0} };

#else
extern  char        *QueryOpName[];

#endif

/*+-----------------------------------------------------------------------+
~M                            METHODS                                     !
  +-----------------------------------------------------------------------+*/
/*    14.0  Query                                                          */
       boolean    Query_Create
                      (ft F,lt Z,zz  *Status,      dbmsquery    **Query);


       boolean    Query_BeginBuild
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status);


       boolean    Query_ScopeOn     
                      (dbmsquery     *Query,       char          *Class,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status);
       boolean    Query1_ScopeOn
                      (dbmsquery     *Query,       numtype        ClassId,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status);

       boolean    Query_ScopeOnValue     
                      (valuequery    *Query,       char          *Type,
                       ft F,lt Z,zz  *Status);
       boolean    Query1_ScopeOnValue     
                      (valuequery    *Query,       numtype        TypeId,
                       ft F,lt Z,zz  *Status);

       boolean    Query_DeScope
                      (dbmsquery     *Query,       char          *Class,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status);
       boolean    Query1_DeScope
                      (dbmsquery     *Query,       numtype        ClassId,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status);


       boolean    Query_SortBy
                      (dbmsquery     *Query,       char          *Member,
                       sortorder      SortOrder,   ft F,lt Z,zz  *Status);
       boolean    Query1_SortBy
                      (dbmsquery     *Query,       numtype        AttrId,
                       sortorder      SortOrder,   ft F,lt Z,zz  *Status);
       boolean    Query2_SortBy
                      (dbmsquery     *Query,       fieldspec      FieldTkns[],
                       numtype        SubLevel,    sortorder      SortOrder,
                       ft F,lt Z,zz  *Status);


       boolean    Query_OpenParen
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status);


/*     Assert that object is of a given Class type                         */
       boolean    Query_AffixIsA
                      (dbmsquery     *Query,       char          *Class,  
                       ft F,lt Z,zz  *Status);
       boolean    Query1_AffixIsA
                      (dbmsquery     *Query,       numtype        ClassId,
                       ft F,lt Z,zz  *Status);


/*     Assert that object is of a given Class type OR Derived from         */
       boolean    Query_AffixIsKindOf
                      (dbmsquery     *Query,       char          *Class,  
                       ft F,lt Z,zz  *Status);
       boolean    Query1_AffixIsKindOf
                      (dbmsquery     *Query,       numtype        ClassId,
                       ft F,lt Z,zz  *Status);


/*     Member must NOT be of type Array, SetHead, SetMember, or Slot       */
       boolean    Query_AffixCompare
                      (dbmsquery     *Query,       char          *Member,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status);

       boolean    Query1_AffixCompare
                      (dbmsquery     *Query,       numtype        AttrId,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status);
       boolean    Query2_AffixCompare
                      (dbmsquery     *Query,       fieldspec      FieldTkns[],
                       numtype        SubLevel,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status);

/*     Member must be of type RefObj or "pointer to Dbms class instance"   */
       boolean    Query_AffixRefQuery
                      (dbmsquery     *Query,       char          *Member,
                       dbmsquery     *SubQuery,    ft F,lt Z,zz  *Status);

       boolean    Query1_AffixRefQuery
                      (dbmsquery     *Query,       numtype        AttrId,
                       dbmsquery     *SubQuery,    ft F,lt Z,zz  *Status);
       boolean    Query2_AffixRefQuery
                      (dbmsquery     *Query,       fieldspec      FieldTkns[],
                       numtype        SubLevel,    dbmsquery     *SubQuery,
                       ft F,lt Z,zz  *Status);


       boolean    Query_AffixBoolOp
                      (dbmsquery     *Query,       booleanop      BoolOp,
                       ft F,lt Z,zz  *Status);

       boolean    Query_CloseParen
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status);


       boolean    Query_EndBuild
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status);


       void       Query_Dump
                      (dbmsquery     *Query);


       boolean    Query_AssertOnObject
                      (tokentype     *Token,       dbmsquery     *Query,
                       ft F,lt Z,zz  *Status);

       boolean    Query_AssertOnDb
                      (tokentype     *Anchor,      dbmsquery     *Query,
                       ft F,lt Z,zz  *Status,      
                       numtype       *NumEntrys,   tokentype     *Vtoken);

       boolean    Query_Delete
                      (ft F,lt Z,zz *Status,       dbmsquery    **Query);

#ifdef QUERY
       boolean    Query_CheckType
                      (numtype       ClassId,      numtype       AttrId,
                       attrtype      AttrType,     addrtype      Value,
                       compareop     CompOp,       ft F,lt Z,zz *Status);

       boolean    Query_GetClassList
                      (tokentype     *DbToken,     dbmsquery     *Query,
                       ft F,lt Z,zz  *Status,      numtype       *LocalObjNum);

       void       Query_DumpSortObj 
                      (querysort     *SortObj);
       void       Query_DumpComdObj 
                      (querycomd     *ComdObj);
       void       Query_DumpFieldObj
                      (dbmsquery     *Query,       queryfield    *FieldObj);

       boolean    Query_CompareValue
                      (attrtype       AttrType,    typetype       UnitType,
                       sizetype       UnitSize,    valueclass     ValueClass,  
                       addrtype       VarValue,    compareop      CompOp,
                       addrtype       CompValue,   ft F,lt Z,zz  *Status);

       boolean    Query_CompareWild
                      (char          *VarValue,    compareop      CompOp,
                       char          *WildSpec,    ft F,lt Z,zz  *Status);

       boolean    Query_AppendAssert
                      (dbmsquery     *Query,       tokentype     *Token,
                       ft F,lt Z,zz  *Status);

#endif

/*+-----------------------------------------------------------------------+
~P                         END OF QUERY.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
