/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Env- QUERY.c*/        
#define  QUERY
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "voa.h"
#include "env0.h"
#include "odbm.h"
#include "oa0.h"
#include "env1.h"
#include "vchunk.h"
#include "grp.h"
#include "km.h"
#include "env01.h"
#include "list01.h"
#include "query.h"
#include "trans.h"
extern boolean OaGetObject    (tokentype    *Token,
                               ft F,lt Z,zz *Status,    addrtype     *Object);

extern boolean OaDumpObject   (int           TraceSwi,  tokentype    *Token,
                               ft F,lt Z,zz *Status);

extern boolean OaGetString    (idtype        DbId,      objheader    *ObjHdr,
                               indextype     StrIndex,
                               ft F,lt Z,zz *Status,    char        **String);
#define   Mem_GetObjNum   Mem_GetWord

#ifdef __ZTC__
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#else

#ifdef MCSTAT
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (Anchor->DbId <= MaxCoreDbId \
 && ((*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr)) ? True : \
     TmFetchCoreDb(Anchor,Stat,DbInCore)
#else
#define TmGetCoreDb(Anchor,Stat,DbInCore) \
           (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
            TmFetchCoreDb(Anchor,Stat,DbInCore)
#endif 
#endif 

#ifdef __NOSTR__
extern  char *strpbrk (char *s1,  char *s2);


//  char *strpbrk (char *s1,  char *s2)
//  {
//
//   return (s1);
//  }
#endif
/*+-----------------------------------------------------------------------+
~M-QUERY                      METHODS                                     !
  +-----------------------------------------------------------------------+*/

boolean    Query_Create
                      (ft F,lt Z,zz  *Status,      dbmsquery    **DbmsQuery)
{
dbmsquery    *Query        = NullPtr;
static boolean       FirstTime    = True;
static bytessizetype TknsSize     = 0;

   if (CoreMoreCore     (sizeof (dbmsquery),
                         McStat,       (addrtype *)&Query))

   if (Mem_MakeChunk    (sizeof (numtype),         200, 100, 0, 
                         McStat,                   &Query->ClassStack))
   if (Mem_GetChunkAddr (Query->ClassStack,  
                         McStat,       (addrtype *)&Query->ClassSet))

   if (Mem_MakeChunk    (sizeof (querysort),        10, 10, 0, 
                         McStat,                   &Query->SortStack))
   if (Mem_GetChunkAddr (Query->SortStack,  
                         McStat,       (addrtype *)&Query->SortSet))

   if (Mem_MakeChunk    (sizeof (querycomd),        100, 20, 0, 
                         McStat,                   &Query->ComdStack))
   if (Mem_GetChunkAddr (Query->ComdStack,  
                         McStat,       (addrtype *)&Query->ComdSet))

   if (Mem_MakeChunk    (sizeof (queryfield),       100, 20, 0, 
                         McStat,                   &Query->FieldStack))
   if (Mem_GetChunkAddr (Query->FieldStack, 
                         McStat,       (addrtype *)&Query->Fields))

   if (Mem_MakeChunk    (sizeof (bytetype),         4048, 1024, 0, 
                         McStat,                   &Query->ValueStack))
   if (Mem_GetChunkAddr (Query->ValueStack, 
                         McStat,       (addrtype *)&Query->Values))

   {
      Query->EnvType        = NullPtr;

      Query->ClassId        = 0;
      Query->MapIsActive    = False;
      Query->CendentsOk     = False;
      Query->NumClasses     = 0;

      Query->NumSorts       = 0;
      Query->NumComds       = 0;
      Query->NumFields      = 0;
      Query->NumValues      = 0;

      Query->NumOpens       = 0;
      Query->NumCloses      = 0;

      Query->LocalBitVec    = NullPtr;
      Query->ResultsDir     = NullToken;
      Query->NumResults     = 0;
      Query->SortTknsDir    = 0;

     *DbmsQuery             = Query;
   }

   if (Normal(*Status))
   if (FirstTime)
   {
      TknsSize    = sizeof (fieldspec) * 10;
      FirstTime   = False;
      Core_MoreCore  (TknsSize, McStat,  (addrtype *)&EnvField.FieldTkns);
   }

TRACK(TrackBak,"Query_Create\n");
return(STAT);
}


boolean    Query_BeginBuild
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status)
{

/* Check to see if this is a previously used Query Object                 */
   if (Query->ClassId
   ||  Query->NumFields)
   {
      Query->ClassId        = 0;
      Query->MapIsActive    = False;
      Query->CendentsOk     = False;
      Query->NumClasses     = 0;

      Query->NumSorts       = 0;
      Query->NumComds       = 0;
      Query->NumFields      = 0;
      Query->NumValues      = 0;

      Query->NumOpens       = 0;
      Query->NumCloses      = 0;

      Query->NumResults     = 0;
      Query->ResultsDir     = NullToken;
      Query->SortTknsDir    = 0;

      if (Mem_PutStackPtr (Query->ClassStack,  0,  McStat))
      if (Mem_PutStackPtr (Query->SortStack,   0,  McStat))
      if (Mem_PutStackPtr (Query->ComdStack,   0,  McStat))
      if (Mem_PutStackPtr (Query->FieldStack,  0,  McStat))
          Mem_PutStackPtr (Query->ValueStack,  0,  McStat);
   }

TRACK(TrackBak,"Query_BeginBuild\n");
return(STAT);
}


boolean    Query_ScopeOn    
                      (dbmsquery     *Query,       char          *Class,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status)
{
   if (Env_GetClassId     (Class,     McStat,  &Query->ClassId))
       Query1_ScopeOn     (Query,     Query->ClassId,  CendentsOk,  McStat);

TRACK(TrackBak,"Query_ScopeOn\n");
return(STAT);
}


boolean    Query1_ScopeOn
                      (dbmsquery     *Query,       numtype        ClassId,
                       boolean        CendentsOk,  ft F,lt Z,zz  *Status)
{
tokentype     BitFieldTkn  = NullToken;
bitfieldtype *BitField     = NullPtr;
numtype       BitNum       = 0;
indextype     ByteCount    = 0;

   Query->ClassId       = ClassId;
   Query->CendentsOk    = CendentsOk;
   Query->NumClasses    = 1;
   if (Mem_AssertStackPtr (Query->ClassStack,   Query->NumClasses,
                           McStat, (addrtype *)&Query->ClassSet))
   {
      Query->ClassSet[0]    = ClassId;
      if (Trans_MapIsActive  (ClassId,   McStat))
         Query->MapIsActive = True;
   }

   if (Normal(*Status))
   if (CendentsOk)
   if (Env_FetchObjCendents (ClassId,   McStat,    &BitFieldTkn))
   if (BitFieldTkn.Handle
   &&  BitFieldTkn.DbId)
   {
      if (OaGetVchunkAddr (&BitFieldTkn, McStat,  (addrtype *)&BitField))
      if (OaGetVchunkStackPtr 
                          (&BitFieldTkn, McStat,  &ByteCount))
      for (BitNum = 0; (++BitNum / 8)  <  ByteCount; )
      {
        if ( (BitField[BitNum >> 3] & 1 << (BitNum & 7)) )
        {
           if (Mem_AssertStackPtr (Query->ClassStack,   Query->NumClasses+1,
                                   McStat, (addrtype *)&Query->ClassSet))
              Query->ClassSet[Query->NumClasses++] = BitNum;
        }
      }
   } else
     Query->CendentsOk    = False;


TRACK(TrackBak,"Query1_ScopeOn\n");
return(STAT);
}




boolean    Query_SortBy
                      (dbmsquery     *Query,       char          *Member,
                       sortorder      SortOrder,   ft F,lt Z,zz  *Status)
{
numtype   AttrId   = 0;

   if (Env_GetAttrId     (Member,     McStat,  &AttrId))
      Query1_SortBy      (Query,      AttrId,   SortOrder,   McStat);


TRACK(TrackBak,"Query_SortBy\n");
return(STAT);
}


boolean    Query1_SortBy
                      (dbmsquery     *Query,       numtype        AttrId,
                       sortorder      SortOrder,   ft F,lt Z,zz  *Status)
{
querysort  SortObj;
numtype    RefObjNum  = 0;
numtype    AplId      = C_Apl;
numtype    SubLevel   = 1;
fieldspec  FieldTkns;
indextype  StackPtr   = 0;
envclass   RefType    = aNonEnvClass;

   FieldTkns.AttrId        = AttrId;
   FieldTkns.Index         = 0;

   SortObj.BaseOffset1     = 0;
   SortObj.FieldOffset1    = 0;

   SortObj.ValueClass      = aVoid;

   if (Query->MapIsActive)
      Trans_FetchObjFieldSpec   (Query->ClassId, AplId,&FieldTkns,  SubLevel,
                                 McStat,               &SortObj.UnitSize,
                                &AttrId,               &SortObj.AttrType, 
                                &RefObjNum,            &SortObj.UnitType,
                                &SortObj.FieldOffset0, &SortObj.FieldOffset1,
                                &SortObj.BaseOffset0,  &SortObj.BaseOffset1);
   else
      Env_FetchObjAttrSpec      (Query->ClassId,        AttrId,     McStat,
                                &SortObj.AttrType,     &RefObjNum, &RefType,
                                &SortObj.UnitSize,     &SortObj.UnitType,
                                &SortObj.ValueClass,   &SortObj.BaseOffset0);

   if (Normal(*Status))
   {
      SortObj.BaseAttr     = AttrId;

      SortObj.FieldAttr    = SortObj.BaseAttr;
      SortObj.FieldOffset0 = SortObj.BaseOffset0;

      SortObj.SortOrder    = SortOrder;

      Query->NumSorts++;

      SortObj.SeqNum       = Query->NumSorts;

      Mem_PushSomeBytes     (Query->SortStack,    sizeof (querysort),
                            (addrtype )&SortObj, McStat,  &StackPtr);
   }

TRACK(TrackBak,"Query1_SortBy\n");
return(STAT);
}



boolean    Query_OpenParen
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status)
{
querycomd    ComdObj;
indextype    StackPtr   = 0;

   ComdObj.ComdType     = aBeginExp;
   ComdObj.QueryField   = 0;
   ComdObj.BoolOp       = aNullBoolOp;
   ComdObj.TrueJump     = 0;
   ComdObj.FalseJump    = 0;

   Query->OpenComds[Query->NumOpens++] = Query->NumComds;

   Query->NumComds++;

   Mem_PushSomeBytes        (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);

TRACK(TrackBak,"Query_OpenParen\n");
return(STAT);
}



boolean    Query_AffixCompare
                      (dbmsquery     *Query,       char          *Member,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status)
{
numtype   AttrId  = 0;
char     *StrSeg  = NullPtr;

   if (Query  == NullPtr)
      *Status  = Env_NullPtr;

   if (Normal(*Status))
   if ((StrSeg   = strpbrk (Member,  FieldOps)) == NULL)
   {
      if (Env_GetAttrId       (Member,     McStat,  &AttrId))
         Query1_AffixCompare  (Query,      AttrId,   CompOp,  Value,  McStat);

   } else {

      if (Env_GetFieldStruc   (Member,     McStat,  &EnvField))
         Query2_AffixCompare  (Query,      EnvField.FieldTkns,   
                               EnvField.SubLevel,    CompOp,  Value,  McStat);
   }

TRACK(TrackBak,"Query_AffixCompare\n");
return(STAT);
}


boolean    Query1_AffixCompare
                      (dbmsquery     *Query,       numtype        AttrId,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status)
{
queryfield    FldObj;
querycomd     ComdObj;
numtype       RefObjNum  = 0;
numtype       AplId      = C_Apl;
numtype       SubLevel   = 1;
fieldspec     FieldTkns;
indextype     StackPtr   = 0;
char         *StrValue   = NullPtr;
tokentype     AttrTkn    = NullToken;
attrdesctype *AttrDesc   = NullPtr;
tokentype     BldDescTkn = NullToken;
objdesctype  *BldDesc    = NullPtr;
sizetype      TknOffset  = 0;
sizetype      FldOffset  = 0;
envclass      RefType    = aNonEnvClass;


   FieldTkns.AttrId        = AttrId;
   FieldTkns.Index         = 0;

   FldObj.BaseOffset1      = 0;
   FldObj.FieldOffset1     = 0;

   FldObj.ValueClass       = aVoid;
   FldObj.IsValueAssert    = False; 

   if (Query->MapIsActive)
      Trans_FetchObjFieldSpec   (Query->ClassId, AplId,&FieldTkns,  SubLevel,
                                 McStat,               &FldObj.UnitSize,
                                &AttrId,               &FldObj.AttrType, 
                                &RefObjNum,            &FldObj.UnitType,
                                &FldObj.FieldOffset0,  &FldObj.FieldOffset1,
                                &FldObj.BaseOffset0,   &FldObj.BaseOffset1);
   else
      Env_FetchObjAttrSpec      (Query->ClassId,        AttrId,     McStat,
                                &FldObj.AttrType,      &RefObjNum,  &RefType,
                                &FldObj.UnitSize,      &FldObj.UnitType,
                                &FldObj.ValueClass,    &FldObj.BaseOffset0);

   if (Normal(*Status))
      Query_CheckType           (Query->ClassId,  AttrId,   FldObj.AttrType,
                                 Value,           CompOp,   McStat);

   if (Normal(*Status))
   if (FldObj.AttrType  == aObjRefAttr)
   {
      AttrTkn.DbId       = RenvId;
      if (EnvFetchAttrHandle (AttrId,      McStat,      &AttrTkn.Handle))
      if (OaGetObject        (&AttrTkn,    McStat,  (addrtype *)&AttrDesc))
      {
         BldDescTkn.DbId          =  RenvId;
         BldDescTkn.Handle        =  AttrDesc->BuildDesc;
         if (OaGetObject     (&BldDescTkn,  McStat,  (addrtype *)&BldDesc))
         if (Env_FetchObjAttrOffset
                             (Query->MapIsActive,
                              BldDesc->ObjNum,      Token_Attr,
                              McStat,              &TknOffset))

         if (Mem_PushEntity  (Query->ValueStack,   sizeof (addrtype), Value,
                              McStat,             &FldObj.ValueOffset))
             Mem_PushEntity  (Query->ValueStack,   sizeof (sizetype),
                              (addrtype )&TknOffset,
                              McStat,             &FldOffset);
         Query->NumValues++;
      }

   } else if (FldObj.UnitType   != aString) {
      Mem_PushEntity      (Query->ValueStack,   FldObj.UnitSize,     Value,
                           McStat,             &FldObj.ValueOffset);
   } else {
      StrValue            = *(char **)Value;
      FldObj.UnitSize     = strlen (StrValue) + 1;

      Mem_PushEntity      (Query->ValueStack,   FldObj.UnitSize,     StrValue,
                           McStat,             &FldObj.ValueOffset);
   }

   if (Normal(*Status))
   {
      Query->NumValues++;

      FldObj.BaseAttr      = AttrId;

      FldObj.FieldAttr     = FldObj.BaseAttr;
      FldObj.FieldOffset0  = FldObj.BaseOffset0;

      FldObj.CompOp        = CompOp;

      Query->NumFields++;
      FldObj.SeqNum        = Query->NumFields;

      Mem_PushSomeBytes   (Query->FieldStack,  sizeof (queryfield),
                           (addrtype )&FldObj, McStat,  &StackPtr);
   }

   if (Normal(*Status))
   {
      ComdObj.ComdType     = aCompareOp;
      ComdObj.QueryField   = Query->NumFields - 1;
      ComdObj.BoolOp       = aNullBoolOp;
      ComdObj.TrueJump     = 0;
      ComdObj.FalseJump    = 0;

      Query->NumComds++;

      Mem_PushSomeBytes      (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);
   }
   if (Normal(*Status))
   if (ClassBug || DeBug)
   {
       Query_DumpComdObj  (&ComdObj);
       Query_DumpFieldObj (Query,     &FldObj);
   }

TRACK(TrackBak,"Query1_AffixCompare\n");
return(STAT);
}


boolean    Query2_AffixCompare
                      (dbmsquery     *Query,       fieldspec      FieldTkns[],
                       numtype        SubLevel,
                       compareop      CompOp,      addrtype       Value,
                       ft F,lt Z,zz  *Status)
{
queryfield FldObj;
querycomd  ComdObj;
numtype    RefObjNum  = 0;
numtype    AttrId     = 0;
numtype    AplId      = C_Apl;
indextype  StackPtr   = 0;

   FldObj.BaseOffset1      = 0;
   FldObj.FieldOffset1     = 0;

   FldObj.ValueClass       = aVoid;
   FldObj.IsValueAssert   = False; 

   if (Query->MapIsActive)
      Trans_FetchObjFieldSpec   (Query->ClassId, AplId, FieldTkns,  SubLevel,
                                 McStat,               &FldObj.UnitSize,
                                &AttrId,               &FldObj.AttrType, 
                                &RefObjNum,            &FldObj.UnitType,
                                &FldObj.FieldOffset0,  &FldObj.FieldOffset1,
                                &FldObj.BaseOffset0,   &FldObj.BaseOffset1);

   else
      Env_FetchObjFieldSpec     (Query->ClassId,        FieldTkns,  SubLevel,
                                 McStat,               &FldObj.UnitSize,
                                &AttrId,               &FldObj.AttrType, 
                                &RefObjNum,            &FldObj.UnitType,
                                &FldObj.FieldOffset0,  &FldObj.BaseOffset0);

   if (Normal(*Status))
      Query_CheckType           (Query->ClassId,  AttrId,   FldObj.AttrType,
                                 Value,           CompOp,   McStat);
   if (Normal(*Status))
   if (FldObj.UnitType   != aString)
   {
      Mem_PushEntity      (Query->ValueStack,   FldObj.UnitSize,     Value,
                           McStat,             &FldObj.ValueOffset);

   } else {
      FldObj.UnitSize     = strlen  ((char *)Value) + 1;
      Mem_PushEntity      (Query->ValueStack,   FldObj.UnitSize,     Value,
                           McStat,             &FldObj.ValueOffset);
   }

   if (Normal(*Status))
   {
      Query->NumValues++;

      FldObj.BaseAttr      = FieldTkns[0].AttrId;

      FldObj.FieldAttr     = AttrId;

      FldObj.CompOp        = CompOp;

      Query->NumFields++;

      FldObj.SeqNum        = Query->NumFields;

      Mem_PushSomeBytes        (Query->FieldStack,      sizeof (queryfield),
                               (addrtype )&FldObj, McStat, &StackPtr);
   }

   if (Normal(*Status))
   {
      ComdObj.ComdType     = aCompareOp;
      ComdObj.QueryField   = Query->NumFields - 1;
      ComdObj.BoolOp       = aNullBoolOp;
      ComdObj.TrueJump     = 0;
      ComdObj.FalseJump    = 0;

      Query->NumComds++;

      Mem_PushSomeBytes      (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);
   }

TRACK(TrackBak,"Query2_AffixCompare\n");
return(STAT);
}


boolean    Query_AffixRefQuery
                      (dbmsquery     *Query,       char          *Member,
                       dbmsquery     *SubQuery,    ft F,lt Z,zz  *Status)
{
numtype   AttrId  = 0;
char     *StrSeg  = NullPtr;

   if (Query  == NullPtr)
      *Status  = Env_NullPtr;

   if (Normal(*Status))
   if ((StrSeg   = strpbrk (Member,  FieldOps)) == NULL)
   {
      if (Env_GetAttrId       (Member,     McStat,  &AttrId))
         Query1_AffixRefQuery (Query,      AttrId,   SubQuery,  McStat);

   } else {
     *Status = Err_NotImplemented;
   }

TRACK(TrackBak,"Query_AffixRefQuery\n");
return(STAT);
}


boolean    Query1_AffixRefQuery
                      (dbmsquery     *Query,       numtype        AttrId,
                       dbmsquery     *SubQuery,    ft F,lt Z,zz  *Status)
{
queryfield    FldObj;
querycomd     ComdObj;
numtype       RefObjNum  = 0;
numtype       AplId      = C_Apl;
numtype       SubLevel   = 1;
fieldspec     FieldTkns;
indextype     StackPtr   = 0;
tokentype     AttrTkn    = NullToken;
attrdesctype *AttrDesc   = NullPtr;
tokentype     BldDescTkn = NullToken;
objdesctype  *BldDesc    = NullPtr;
envclass      RefType    = aNonEnvClass;

   AttrTkn.DbId            = RenvId;

   FieldTkns.AttrId        = AttrId;
   FieldTkns.Index         = 0;

   FldObj.BaseOffset1      = 0;
   FldObj.FieldOffset1     = 0;

   FldObj.ValueClass       = aVoid;
   FldObj.IsValueAssert    = False; 

   if (Query->MapIsActive)
      Trans_FetchObjFieldSpec   (Query->ClassId, AplId,&FieldTkns,  SubLevel,
                                 McStat,               &FldObj.UnitSize,
                                &AttrId,               &FldObj.AttrType, 
                                &RefObjNum,            &FldObj.UnitType,
                                &FldObj.FieldOffset0,  &FldObj.FieldOffset1,
                                &FldObj.BaseOffset0,   &FldObj.BaseOffset1);
   else
      Env_FetchObjAttrSpec      (Query->ClassId,        AttrId,     McStat,
                                &FldObj.AttrType,      &RefObjNum, &RefType,
                                &FldObj.UnitSize,      &FldObj.UnitType,
                                &FldObj.ValueClass,    &FldObj.BaseOffset0);

   if (Normal(*Status))
   if (FldObj.AttrType  == aObjRefAttr
   ||  FldObj.AttrType  == aTknRefAttr)
   {
      if (EnvFetchAttrHandle (AttrId,      McStat,      &AttrTkn.Handle))
      if (OaGetObject        (&AttrTkn,    McStat,  (addrtype *)&AttrDesc))
      {
         BldDescTkn.DbId          =  RenvId;
         BldDescTkn.Handle        =  AttrDesc->BuildDesc;
         if (OaGetObject     (&BldDescTkn,  McStat,  (addrtype *)&BldDesc))
         if (BldDesc->ObjNum     == SubQuery->ClassId)
             Mem_PushEntity  (Query->ValueStack,   sizeof (addrtype),
                              (addrtype )&SubQuery,
                              McStat,             &FldObj.ValueOffset);
         else {
           *Status  = Query_BadSubQueryClassId;
            sprintf (Msg, 
" Query1_AffixRefQuery:: RefClass(%4u) Must be Same As Member RefType(%4u)\n",
                     SubQuery->ClassId, BldDesc->ObjNum);
            TraceMsg (0, Msg);
         }
      }

   } else {
     *Status = Query_AttrNotReference;
      TraceMsg (0, 
          " Query1_AffixRefQuery:: Attribute Must be of type reference to\n");
      sprintf (Msg, "     for ClassId=%4u; AttrId=%4u.\n",
               Query->ClassId, AttrId);
      TraceMsg (0, Msg);
   }

   if (Normal(*Status))
   {
      Query->NumValues++;

      FldObj.BaseAttr      = AttrId;

      FldObj.FieldAttr     = FldObj.BaseAttr;
      FldObj.FieldOffset0  = FldObj.BaseOffset0;

      FldObj.CompOp        = aEQ;

      Query->NumFields++;
      FldObj.SeqNum        = Query->NumFields;

      Mem_PushSomeBytes   (Query->FieldStack,  sizeof (queryfield),
                           (addrtype )&FldObj, McStat,  &StackPtr);
   }

   if (Normal(*Status))
   {
      ComdObj.ComdType     = aSubQuery;
      ComdObj.QueryField   = Query->NumFields - 1;
      ComdObj.BoolOp       = aNullBoolOp;
      ComdObj.TrueJump     = 0;
      ComdObj.FalseJump    = 0;

      Query->NumComds++;

      Mem_PushSomeBytes      (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);
   }

   if (Normal(*Status))
   if (ClassBug || DeBug)
   {
       Query_DumpComdObj  (&ComdObj);
       Query_DumpFieldObj (Query,     &FldObj);
   }

TRACK(TrackBak,"Query1_AffixRefQuery\n");
return(STAT);
}


boolean    Query_AffixBoolOp
                      (dbmsquery     *Query,       booleanop      BoolOp,
                       ft F,lt Z,zz  *Status)
{
querycomd  ComdObj;
indextype  StackPtr   = 0;

   if (ClassBug || DeBug)
   if (sprintf (Msg, "\n   Query_AffixBoolOp:: @%6x\n",
                Query))
      TraceMsg (0, Msg);

   ComdObj.ComdType     = aBooleanOp;
   ComdObj.QueryField   = 0;
   ComdObj.BoolOp       = BoolOp;
   ComdObj.TrueJump     = 0;
   ComdObj.FalseJump    = 0;

   Query->NumComds++;

   Mem_PushSomeBytes        (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);


TRACK(TrackBak,"Query_AffixBoolOp\n");
return(STAT);
}


boolean    Query_CloseParen
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status)
{
querycomd  ComdObj;
indextype  StackPtr   = 0;
numtype    Index      = 0;

   if (ClassBug || DeBug)
   if (sprintf (Msg, "\n   Query_CloseParen:: @%6x\n",
                Query))
      TraceMsg (0, Msg);

   ComdObj.ComdType     = aEndExp;
   ComdObj.QueryField   = 0;
   ComdObj.BoolOp       = aNullBoolOp;
   ComdObj.TrueJump     = 0;
   ComdObj.FalseJump    = 0;

   Index                = Query->NumOpens - Query->NumCloses - 1;
   Query->CloseComds[Query->NumCloses++]  = Query->OpenComds[Index];

   Query->OpenComds[Index]                = Query->NumComds;

   Query->NumComds++;

   Mem_PushSomeBytes        (Query->ComdStack,    sizeof (querycomd),
                             (addrtype )&ComdObj, McStat, &StackPtr);

TRACK(TrackBak,"Query_CloseParen\n");
return(STAT);
}


boolean    Query_EndBuild
                      (dbmsquery     *Query,       ft F,lt Z,zz  *Status)
{
int        i          = 0;
int        j          = 0;
numtype    NumOpens   = 0;
numtype    Jump       = 0;
queryop    QueryOp    = aNonQueryOp;
   if (ClassBug || DeBug)
   if (sprintf (Msg, "\n   Query_EndBuild:: @%6x\n",
                Query))
      TraceMsg (0, Msg);
   
   i = 0; 
   while (i < Query->NumComds)
   {
      i++;
      QueryOp                       = Query->ComdSet[i-1].ComdType; 
      Query->ComdSet[i-1].SeqNum    = i;

      switch (QueryOp) 
      {
         case  aIsA               :
              break;

         case  aIsKindOf          :
              break;

         case  aCompareOp         :
              break;

         case  aSubQuery          :
              break;

         case  aBooleanOp         :
             Jump                = Query->OpenComds[NumOpens - 1];
             if (Normal(*Status))
             {
              switch (Query->ComdSet[i-1].BoolOp) 
              {
               case  aNullBoolOp  :
                    break;

               case  aAND         :
                    Query->ComdSet[i-1].TrueJump  = i;
                    Query->ComdSet[i-1].FalseJump = Jump;
                    break;

               case  aOR          :
                    Query->ComdSet[i-1].FalseJump = i;
                    Query->ComdSet[i-1].TrueJump  = Jump;
                    break;

               case  aExOR        :
                    break;

               case  aNOT         :
                    break;

               case  aNAND        :
                    break;

               case  aNOR         :
                    break;

               case  aNxOR        :
                    break;
              }
             }
             break;

         case  aBeginExp          :
              ++NumOpens;
              Jump                = Query->OpenComds[NumOpens - 1];
              break;

         case  aEndExp            :
              --NumOpens;
              break;

         case  aPushBool          :
              j = 0;
              break;
         case  aPopBool           :
              j = 0;
              break;
         case  aJumpSwi           :
              j = 0;
              break;
         case  aEndQuery          :
              j = 0;
              break;
         case  aClearStack        :
              j = 0;
              break;

         default                  :
              j = 0;
      }
   }

TRACK(TrackBak,"Query_EndBuild\n");
return(STAT);
}



boolean    Query_AssertOnObject
                      (tokentype     *Token,       dbmsquery     *Query,
                       ft F,lt Z,zz  *Status)
{
dbheader   *CoreDb         = NullPtr;
addrtype    Object         = NullPtr;
addrtype    DbObject       = NullPtr;
numtype     MemLoc         = 0;
numtype     EnvObj         = 0;
numtype     LocalObj       = 0;
boolean     ComputeBase    = False;
querycomd   ComdObj;
queryfield  FieldObj;
boolean     EvalStack[20];
numtype     NumEvals       = 0;
int         i              = 0;
boolean     Eval           = True;
sizetype    BaseOffset0    = 0;
sizetype    BaseOffset1    = 0;
addrtype    FieldAddr      = NullPtr;
sizetype    FieldOffset    = 0;
addrtype    ValueAddr      = NullPtr;
numtype     NumOpens       = 0;
numtype     NumCloses      = 0;
boolean     BoolVal        = False;
boolean     BoolVal0       = False;
tokentype   VchunkTkn      = NullToken;
tokentype   RefTkn         = NullToken;
tokentype   OwnerTkn       = NullToken;
tokentype   MemberTkn      = NullToken;
tokentype   NodeTkn        = NullToken;
boolean     ArrayIsA       = False;
addrtype    BytePtr        = NullPtr;
longaddr    Offset         = 0;
dbmsquery  *Query0         = NullPtr;
indextype   StackPtr       = 0;
indextype   StrIndex       = 0;
objheader  *ObjHdr         = NullPtr;
numtype     NumAsserts     = 0;
numtype     SetId          = 0;
numtype     ItNum          = 0;

  if (Query  == NullPtr)
    *Status  = Env_NullPtr;

  if (ClassBug)
  if (sprintf(Msg, "\n  Query_AssertOnObject::  [%3u:%4u]\n",
              Token->DbId, Token->Handle))
  if (TraceMsg (0, Msg))
     OaDumpObject (0, Token, McStat);


  if (Normal(*Status))
  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectAddr      (CoreDb,  Token->Handle,  McStat,   &Object))
  if (Object  == NullPtr)
  {
     if (ClassBug)
     if (sprintf(Msg, "\n  Query_AssertOnObject::  [%3u:%4u] Was Deleted.\n",
                 Token->DbId, Token->Handle))
        TraceMsg (0, Msg);
     return (False);
  }

  if (Normal(*Status))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &EnvObj))
  if (EnvObj             != Query->ClassId)
  {
      ComputeBase         = True;
      if (ClassBug)
      if (sprintf(Msg, "    EnvObj =%4u; BaseClass =%4u;\n",
                  EnvObj, Query->ClassId))
         TraceMsg (0, Msg);
  }

  while (Normal(*Status)
  &&     i                < Query->NumComds
  &&     Eval)
  {
     ComdObj  = Query->ComdSet[i];
     FieldObj = Query->Fields[ComdObj.QueryField];

     switch (ComdObj.ComdType)
     {
       case    aIsA         :
           if (EnvObj == ComdObj.QueryField)
              EvalStack[NumEvals++] = True;
           else
              EvalStack[NumEvals++] = False;
           break;

       case    aIsKindOf    :
           BoolVal = False;
           for (i = 0; i < Query->NumClasses && BoolVal == False; i++)
           {
              if (Query->ClassSet[i] == ComdObj.QueryField)
              {
                 BoolVal  = True;
              }
           }
           EvalStack[NumEvals++] = BoolVal;
           break;

       case    aCompareOp   :
       case    aSubQuery    :
           if (MemLoc  == 0)
           {
              if (ComputeBase)
              {
                 Env_FetchObjAttrOffset(MemLoc,
                                        EnvObj,     FieldObj.BaseAttr,
                                        McStat,    &BaseOffset0);
                 FieldOffset          = FieldObj.FieldOffset0
                                      - FieldObj.BaseOffset0
                                      + BaseOffset0;
              } else
                 FieldOffset          = FieldObj.FieldOffset0;

           } else {
              if (ComputeBase)
              {
                 Trans_FetchAttrOffsets
                                       (EnvObj,  C_Apl,  FieldObj.BaseAttr,
                                        McStat, &BaseOffset0, &BaseOffset1);
                 FieldOffset          = FieldObj.FieldOffset1
                                      - FieldObj.BaseOffset1
                                      + BaseOffset1;
              } else
                 FieldOffset          = FieldObj.FieldOffset1;

           }
           FieldAddr    = (addrtype )((char *)Object        + FieldOffset);
           ValueAddr    = (addrtype )((char *)Query->Values 
                                            + FieldObj.ValueOffset);

           if (Normal(*Status))
           if (MemLoc == 0)
           {
              switch (FieldObj.AttrType)
              {
                 case aObjRefAttr   :
                 case aTknRefAttr   :
                     RefTkn            = *(tokentype  *)FieldAddr;
                     break;

                 case aVchunkAttr   :
                     VchunkTkn         = *(tokentype  *)FieldAddr;
                     ArrayIsA          = True;
                     break;

                 case aChunkAttr    :
                 case aVarrayAttr   :
                 case aArrayAttr    :
                 case aDblPtrAttr   :
                     VchunkTkn.DbId    = Token->DbId;
                     VchunkTkn.Handle  = *(handletype *)FieldAddr;
                     ArrayIsA          = True;
                     break;
              }

           } else {
              switch (FieldObj.AttrType)
              {
                 case aTknRefAttr   :
                     BytePtr     = (addrtype )((char *)FieldAddr 
                                               + sizeof (addrtype));
                     MoveBytes     (BytePtr,  (addrtype )&RefTkn,
                                    sizeof (tokentype));
                     break;

                 case aObjRefAttr   :
                     Offset      = (longaddr )((char *)ValueAddr   
                                                     + FieldObj.ValueOffset);
                     BytePtr     = (addrtype )((char *)FieldAddr   + Offset);
                     MoveBytes     (BytePtr,  (addrtype )&RefTkn,
                                    sizeof (tokentype));
                     break;

                 case aVchunkAttr   :
                 case aChunkAttr    :
                 case aVarrayAttr   :
                     ArrayIsA    = True;
                     VchunkTkn   = *(tokentype  *)FieldAddr;
                     break;

                 case aArrayAttr    :
                 case aDblPtrAttr   :
                     ArrayIsA       = True;
                     if (OaRevokeCppObject (Token,                 McStat))
                     if (OaGetObject       (Token,   McStat,      &DbObject))
                     {
                        if (ComputeBase)
                        {
                           Env_FetchObjAttrOffset
                                           (MemLoc,
                                            EnvObj,  FieldObj.BaseAttr,
                                            McStat,          &BaseOffset0);
                           FieldOffset    = FieldObj.FieldOffset0
                                          - FieldObj.BaseOffset0
                                          + BaseOffset0;
                        } else
                           FieldOffset    = FieldObj.FieldOffset0;

                        FieldAddr         = (addrtype )((char *)DbObject 
                                                              + FieldOffset);
                        VchunkTkn.Handle  = *(handletype *)FieldAddr;

                        if (Normal(*Status))
                         OaInvokeCppObject (Token,   Object,       McStat);
                     }
                     break;
              }
           }

           if (Normal(*Status))
           if (ArrayIsA) 
           {
              Offset      = (longaddr )((char *)ValueAddr   
                                              + FieldObj.ValueOffset);

              BytePtr     = (addrtype )((char *)FieldAddr   + Offset);
              MoveBytes     (BytePtr,  (addrtype )&NumAsserts,
                             sizeof (numtype));

              BytePtr     = (addrtype )((char *)BytePtr  + sizeof (numtype));
              MoveBytes     (BytePtr,  (addrtype )&Query0,
                             sizeof (addrtype));

              StackPtr           = 0;
              if (Vchunk_IsValidToken  (&VchunkTkn,  McStat))  
                  Vchunk_GetStackPtr   (&VchunkTkn,  McStat,  &StackPtr);

              if (FieldObj.IsValueAssert) /* Value Query test on elements
                                             then compare to #asserts      */
              {
                 if (StackPtr == 0)
                    EvalStack[NumEvals++] = False;
                 else
                    ;

              } else {  /* NumAsserts .CompOp. StackPtr                    */

                 EvalStack[NumEvals++] = Query_CompareValue
                             (aAttr,     aInteger,  sizeof (indextype),
                              FieldObj.ValueClass,  
                              (addrtype )&StackPtr, FieldObj.CompOp,
                              ValueAddr,            McStat);
              }

           } else if (FieldObj.AttrType   == aSetHeadAttr)  {

              Offset      = (longaddr )((char *)ValueAddr   
                                              + FieldObj.ValueOffset);

              BytePtr     = (addrtype )((char *)FieldAddr   + Offset);
              MoveBytes     (BytePtr,  (addrtype )&NumAsserts,
                             sizeof (numtype));

              BytePtr     = (addrtype )((char *)BytePtr  + sizeof (numtype));
              MoveBytes     (BytePtr,  (addrtype )&Query0,
                             sizeof (addrtype));

              BytePtr     = (addrtype )((char *)BytePtr  + sizeof (addrtype));
              MoveBytes     (BytePtr,  (addrtype )&SetId,
                             sizeof (numtype));

              if (FieldObj.UnitSize == sizeof (handletype))
              {
                 OwnerTkn.DbId     = Token->DbId;
                 OwnerTkn.Handle   = *(handletype *)FieldAddr;
              } else 
                 OwnerTkn          = *(tokentype *)FieldAddr;

              if (ClassBug || DeBug >= 0 || OaBug)
              if (sprintf (Msg, 
                 " Object Set Owner[%2u:%4u]; Query @%x\n",
                           OwnerTkn.DbId, OwnerTkn.Handle, (addrtype )Query0))
                 TraceMsg (0, Msg);

              if (OwnerTkn.Handle == 0
              || !Env_IsValidToken  (&OwnerTkn,  McStat))
              {
                 EvalStack[NumEvals++] = Query_CompareValue
                             (aAttr,     aInteger,  sizeof (indextype),
                              FieldObj.ValueClass,  
                              (addrtype )&StackPtr, FieldObj.CompOp,
                              &OwnerTkn.Handle,             McStat);

              } else if (Query0  == NullPtr) {
                 if (KernelFirstOf ((handletype )SetId,     &OwnerTkn, 
                                    McStat,     &NodeTkn,   &MemberTkn))
                 for (ItNum   = 1;
                     KernelNextOf  ((handletype )SetId,     &OwnerTkn,
                                    McStat,     &NodeTkn,   &MemberTkn)
                  && Normal(*Status); ItNum++)
                   ;

                 if (*Status == Set_EndOfSet
                 ||  *Status == Set_EmptySet)
                     *Status  = Env_Normal;

                 EvalStack[NumEvals++] = Query_CompareValue
                             (aAttr,     aInteger,  sizeof (indextype),
                              FieldObj.ValueClass,  
                              (addrtype )&ItNum,   FieldObj.CompOp,
                              &OwnerTkn.Handle,             McStat);

              } else {
#ifdef __INCLUDE_ALL_CODE__
                 if (Query_AssertOnSet  (SetId,  &OwnerTkn,   Query0,  
                                         McStat, &ItNum,     &VchunkTkn))
#endif
                    EvalStack[NumEvals++] = Query_CompareValue
                             (aAttr,     aInteger,  sizeof (indextype),
                              FieldObj.ValueClass,  
                              (addrtype )&ItNum,    FieldObj.CompOp,
                              &OwnerTkn.Handle,             McStat);
              }

           } else if (FieldObj.AttrType   == aStrHndlAttr)  {
              if (MemLoc == 0)
              if (HmGetLocalObjNum 
                             (CoreDb,                        Token->Handle,
                              McStat,                       &LocalObj))
              if (MemGetAddr (CoreDb->ObjHdrDir, (indextype )LocalObj,
                              McStat,           (addrtype *)&ObjHdr))
              {
                 StrIndex   = *(indextype *)FieldAddr;
                 OaGetString (Token->DbId,  ObjHdr,          StrIndex,
                              McStat,              (char **)&FieldAddr);

              }
              if (Normal(*Status))
                 EvalStack[NumEvals++] = Query_CompareWild
                             ((char *)FieldAddr,  FieldObj.CompOp, 
                              (char *)ValueAddr,  McStat);

           } else if (FieldObj.AttrType   == aObjRefAttr
                  ||  FieldObj.AttrType   == aTknRefAttr)   {

              Query0   = *(dbmsquery **)ValueAddr;   

              if (ClassBug || DeBug || OaBug)
              if (sprintf (Msg, 
                 " Object Reference[%2u:%4u]; Query @%x\n",
                           RefTkn.DbId, RefTkn.Handle, (addrtype )Query0))
                 TraceMsg (0, Msg);

              if (Env_IsValidToken  (&RefTkn,  McStat))
              if (Query0  != NullPtr)
                 EvalStack[NumEvals++] = Query_AssertOnObject
                                               (&RefTkn,  Query0,  McStat);

           } else {
              if (FieldObj.UnitType != aString)
                 EvalStack[NumEvals++] = Query_CompareValue
                             (FieldObj.AttrType,    FieldObj.UnitType,
                              FieldObj.UnitSize,    FieldObj.ValueClass,  
                              FieldAddr,            FieldObj.CompOp,
                              ValueAddr,            McStat);
              else { 
                 EvalStack[NumEvals++] = Query_CompareWild
                             ((char *)FieldAddr,  FieldObj.CompOp, 
                              (char *)ValueAddr,  McStat);
              }
           }
           break;

       case    aBooleanOp   :

           BoolVal0  = EvalStack[--NumEvals];

           switch (ComdObj.BoolOp) 
           {
              case  aAND          :
                  if (BoolVal0   == False)
                  {
                     EvalStack[NumEvals++] = False;
                     i                     = ComdObj.FalseJump - 1;
                  }
                  break;

              case  aOR           :
                  if (BoolVal0   == True)
                  {
                     EvalStack[NumEvals++] = True;
                     i                     = ComdObj.TrueJump  - 1;
                  }
                  break;

              case  aExOR         :
                  break;

              case  aNOT          :
                  break;

              case  aNAND         :
                  break;

              case  aNOR          :
                  break;

              case  aNxOR         :
              default             :
                  break;
           }
           break;


       case    aBeginExp    :
           NumOpens++;
           break;

       case    aEndExp      :
           if (++NumCloses  == Query->NumCloses)
           {
              if (ClassBug)
              if (sprintf (Msg, "    Assert := %s;\n",
                           BoolStr[EvalStack[NumEvals-1]]))
                 TraceMsg (0, Msg);
              return (EvalStack[--NumEvals]);
           }
           break;
     }
   ++i;
  }

  if (Normal(*Status))
  {
     if (ClassBug)
     if (sprintf (Msg, "    Assert := %s;\n",
                  BoolStr[EvalStack[--NumEvals]]))
        TraceMsg (0, Msg);

     return (EvalStack[--NumEvals]);
  }

TRACK(TrackBak,"Query_AssertOnObject\n");
return(STAT);
}


boolean    Query_AssertOnDb
                      (tokentype     *PrimalTkn,   dbmsquery     *Query,
                       ft F,lt Z,zz  *Status,      
                       numtype       *NumEntrys,   tokentype     *Vtoken)
{
dbheader     *DbHdr        = NullPtr;
numtype       LocalObjNum  = 0;
numtype       LocalObj     = 0;
tokentype     ItorTkn      = NullToken;
int           i            = 0;
boolean       Found        = False;
bytetype      BitVal       = 0;
tokentype    *Results      = NullPtr;
boolean       BoolVal      = False;
indextype     StackIndex   = 0;

  ItorTkn.DbId    = PrimalTkn->DbId;
  ItorTkn.Handle  = 1;

  if (ClassBug)
  if (sprintf(Msg, 
      "  Query_AssertOnDb::  DbId =%3u ClassId =%4u; Cendents= %s\n",
              PrimalTkn->DbId, Query->ClassId, BoolStr[Query->CendentsOk]))
      TraceMsg (0, Msg);

  if (TmGetCoreDb        (PrimalTkn,            McStat,   &DbHdr))
      Query_GetClassList (PrimalTkn,        Query,   McStat,  &LocalObjNum);

  while (Normal(*Status)
  &&   ++ItorTkn.Handle  < DbHdr->NextHandle)
  {
     Found         = False;
     if (HmGetLocalObjNum (DbHdr,   ItorTkn.Handle,  McStat,  &LocalObj))
     if (LocalObj != Null_ObjNum)
     if (Query->CendentsOk  == False)
     {
        if (LocalObjNum     == LocalObj)
           Found   = True;
     } else {
        if (BitVec_GetBit (Query->LocalBitVec,  LocalObj,  McStat,  &BitVal))
        if (BitVal)
           Found   = True;
     }
     if (Found)
     {
        BoolVal = Query_AssertOnObject 
                              (&ItorTkn,   Query,   McStat);
        if (Normal(*Status)
        &&  BoolVal)
        {
           Vchunk_PushUnit    (&Query->ResultsDir, (addrtype )&ItorTkn, 
                               McStat,             &StackIndex);
           Query->NumResults++;
        }
     }
  }
  if (*Status  == Iterate_EndOf)
      *Status   = Env_Normal;

  if (Normal(*Status))
  {
     *NumEntrys   = Query->NumResults;
     *Vtoken      = Query->ResultsDir;
  }

  if (Normal(*Status))
  if (ClassBug || DeBug)
  {
    sprintf  (Msg, "\n  Query_AssertOnDb:: NumResults = %4u;\n", 
              Query->NumResults);
    TraceMsg (0,   Msg);
    if (Vchunk_GetAddress (&Query->ResultsDir, McStat, (addrtype *)&Results))
    for (i=0; i < Query->NumResults; ++i)
    {
       sprintf  (Msg, "  %-6d.  [%3u:%4u]\n",
                 i, Results[i].DbId, Results[i].Handle);
       TraceMsg (0, Msg);
    }
    TraceMsg (0, "\n");
  }

TRACK(TrackBak,"Query_AssertOnDb\n");
return(STAT);
}




/* ------------------------------------------------------------------------ */
boolean    Query_CompareValue
                         (attrtype      AttrType,    typetype      UnitType,
                          sizetype      UnitSize,    valueclass    ValueClass,  
                          addrtype      VarValue,    compareop     CompOp,
                          addrtype      CompValue,   ft F,lt Z,zz *Status)
{
int          i               = 0;
bytetype    *ByteValue       = NULL;
idtype      *IdValue         = NULL;
inttype     *IntValue        = NULL;
tokentype   *TknValue        = NULL;
tokentype   *CompTkn         = NULL;
float       *FloatValue      = NULL;
double      *DoubleValue     = NULL;
addrtype    *AddrValue       = NULL;
handletype  *HndlValue       = NULL;
double       AlignValue;
addrtype     AlignComp       = (addrtype )&AlignValue;

  if (UnitType  != aString
  &&  UnitSize  <= sizeof (double))
      MoveBytes     (CompValue,    AlignComp,    UnitSize);

  switch (UnitType)
  {

      case aDbmsString          :
                 *Status = Err_NotImplemented;
                  break;

      case aString     :
/*
//                return (Query_CompareWild  (VarValue, CompOp, CompValue,
//                        McStat));
*/
                  i = strcmp  ((char *)VarValue,  (char *)CompValue);
                  break;

      case aLink       :
                  if (UnitSize == sizeof (tokentype ))
                  {
                     TknValue    = (tokentype *)AlignComp;
                     CompTkn     = (tokentype *)VarValue;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;

                  } else {
                     HndlValue   = (handletype *)AlignComp;
                     if (*(handletype *)VarValue < *HndlValue)
                       i = -1;
                     else if (*(handletype *)VarValue > *HndlValue)
                       i = 1;

                  }
                  break;

      case aReal       :
                  if (UnitSize == sizeof (float)) 
                  {
                     FloatValue    = (float *)AlignComp;
                     if (*(float *)VarValue < *FloatValue)
                       i = -1;
                     else if (*(float *)VarValue > *FloatValue)
                       i = 1;

                  } else if (UnitSize == sizeof (double)) {
                     DoubleValue    = (double *)AlignComp;
                     if (*(double *)VarValue < *DoubleValue)
                       i = -1;
                     else if (*(double *)VarValue > *DoubleValue)
                       i = 1;

                  }
                  break;

      case aAddr       :
                  AddrValue    = (addrtype *)AlignComp;
                  if (*(addrtype *)VarValue != AddrValue)
                       i = 1;
                  break;


      case aEnum       :
      case aBoolean    :
      case aValue      :
      case aChunk      :
      case aInteger    :
          
         switch (UnitSize)
         {
            case 1     :
                     ByteValue    = (bytetype *)AlignComp;
                     if (*(bytetype *)VarValue < *ByteValue)
                       i = -1;
                     else if (*(bytetype *)VarValue > *ByteValue)
                       i = 1;
                     break;

            case 2     :
                     IdValue    = (idtype *)AlignComp;
                     if (*(idtype *)VarValue < *IdValue)
                       i = -1;
                     else if (*(idtype *)VarValue > *IdValue)
                       i = 1;
                     break;

            case 4     :
                     IntValue    = (inttype *)AlignComp;
                     if (*(inttype *)VarValue < *IntValue)
                       i = -1;
                     else if (*(inttype *)VarValue > *IntValue)
                       i = 1;
                     break;


#ifdef LARGE
            case sizeof (tokentype )  :
                     TknValue    = (tokentype *)AlignComp;
                     CompTkn     = (tokentype *)VarValue;
                     if (CompTkn->Handle < TknValue->Handle)
                       i = -1;
                     else if (CompTkn->Handle > TknValue->Handle)
                       i = 1;
                     if (i == 0
                     && CompTkn->DbId  != TknValue->DbId)
                       i = -1;
                     break;
#endif

            default    :
                     if (UnitSize <= sizeof (tokentype ))
                         i = memcmp  ((char *)VarValue, 
                                      (char *)CompValue, (size_t )UnitSize);
                     else 
                         i = strncmp ((char *)VarValue, 
                                       (char *)CompValue, (size_t )UnitSize);
         }
         break;
  }

  if (Normal (*Status))
  switch (CompOp)
  {
     case aEQ  :
         if (i   == 0)
            return(True); 
         else 
            return (False);

     case aLT  :
         if (i    < 0)
            return(True); 
         else
            return (False);

     case aGT  :
         if (i    > 0)
            return(True);
         else
            return (False);

     case aGTE :
         if (i   == 0
         ||  i    > 0)
            return(True);
         else
            return (False);

     case aLTE :
         if (i   == 0
         ||  i    < 0)
            return(True);
         else
            return (False);

     case aNEQ :
         if (i   != 0)
            return(True);
         else
            return (False);

     case aModEQ0 :
         break;
  }


TRACK(TrackBak,"Query_CompareValue\n");
return(STAT);
}


boolean    Query_CompareWild
                      (char          *VarValue,    compareop      CompOp,
                       char          *WildSpec,    ft F,lt Z,zz  *Status)
{
static char *WildOps    = "?*";
sizetype     StrSize    = 0;
int          i          = 0;
int          j          = 0;
numtype      VarIndex        = 0;
int          VarLen          = 0;
char         CompValue[MAXSTRING];
char        *WildSeg         = NullPtr;
char        *VarSeg          = VarValue;
char         CharSpec[2];

  strcpy (CompValue, "\0");
  strcpy (CharSpec,  "\0");

  VarLen                  = strlen    (VarValue) + 1;
  VarIndex                = 0;
  while (Normal(*Status)
  &&     i               == 0
  &&     VarIndex         < VarLen
  &&     VarValue[0]     != '\0')
  {
      /* Compare on the word segment up to the wild char specifier         */
      if ((WildSeg    = strpbrk (WildSpec, WildOps)) != NULL)
      {
         StrSize              = (sizetype )(WildSeg - WildSpec);
         strncpy  (CompValue,    WildSpec,  StrSize);
         CompValue[StrSize]   = '\0';
         WildSpec             = &WildSeg[0];
         if ((i   = strncmp (VarValue,  CompValue, StrSize)) != 0)
            break;

         VarIndex            += StrSize;
         VarValue            += StrSize;

      } else {
         VarIndex            += strlen (WildSpec);
         i                    = strcmp (VarValue,  WildSpec);
         break;
      }

      /* If the spec is a series of ? marks                                */
      if (WildSpec[0] == '?')
      {
      /*   advance position of VarValue                                    */
         for (j = 0; WildSpec[0] == '?'; ++WildSpec)
         {
            ++j; ++VarValue;
         }
         if ((VarIndex  += j)     >= VarLen)
         {
            i = -1;
            break;
         }
         if (VarValue[0] == '\0')
         if (WildSpec[0] != '\0')
         if (WildSpec[0] != '*')
            i = -1;

      } else if (WildSpec[0] == '*') {
         /* The spec is an asteric '*'                                     */
       ++WildSpec;
         if ((CharSpec[0]     = WildSpec[0]) == '\0')
            VarIndex          = VarLen;

         else if ((VarSeg     = strpbrk (VarValue, CharSpec)) != NULL)
         {
           StrSize            = (sizetype )(VarSeg - VarValue);
           VarValue           = &VarSeg[0];
           VarIndex          += StrSize;

         } else {
           i = -1;
           VarIndex           = VarLen;
         }

      } else
        *Status  = Lex_BadSyntax;
  }

  if (ClassBug)
  if (sprintf  (Msg, "    Query_CompareWild:: i = %4d\n", 
                i))
      TraceMsg (0, Msg);

  if (Normal (*Status))
  switch (CompOp)
  {
     case aEQ  :
         if (i   == 0)
            return(True); 
         else 
            return (False);

     case aLT  :
         if (i    < 0)
            return(True); 
         else
            return (False);

     case aGT  :
         if (i    > 0)
            return(True);
         else
            return (False);

     case aGTE :
         if (i   == 0
         ||  i    > 0)
            return(True);
         else
            return (False);

     case aLTE :
         if (i   == 0
         ||  i    < 0)
            return(True);
         else
            return (False);

     case aNEQ :
         if (i   != 0)
            return(True);
         else
            return (False);

     case aModEQ0 :
         break;
  }

TRACK(TrackBak,"Query_CompareWild\n");
return(STAT);
}


boolean    Query_CheckType
                      (numtype       ClassId,      numtype       AttrId,
                       attrtype      AttrType,     addrtype      Value,
                       compareop     CompOp,       ft F,lt Z,zz *Status)
{
tokentype    CompTkn   = NullToken;

   if (Normal(*Status))
   if (AttrType     == aArrayAttr
   ||  AttrType     == aVarrayAttr
   ||  AttrType     == aDblPtrAttr
   ||  AttrType     == aChunkAttr
   ||  AttrType     == aVchunkAttr)
   {
     *Status = Query_ArrayNotApplicable;

      TraceMsg (0, 
       " Query_Affix :: Array Attribute Not Applicable.\n");
      sprintf (Msg, "     for ClassId=%4u; AttrId=%4u.\n",
               ClassId, AttrId);
      TraceMsg (0, Msg);
      TraceMsg (0, "   Use <Query_Assert> type Method.\n");
   }

   if (Normal(*Status))
   if (AttrType     == aSetNodeAttr
   ||  AttrType     == aSetHeadAttr
   ||  AttrType     == aSlotHndlAttr
   ||  AttrType     == aSlotTokenAttr)
   {
     *Status = Query_SetOrTupleNotAllowed;

      TraceMsg (0, 
       " Query_Affix :: Set or Tuple Type Not Allowed.\n");
      sprintf (Msg, "     for ClassId=%4u; AttrId=%4u.\n",
               ClassId, AttrId);
      TraceMsg (0, Msg);
      TraceMsg (0, "   Use <Query_AssertOn> type Method.\n");
   }

   if (Normal(*Status))
   if (AttrType     == aObjRefAttr
   ||  AttrType     == aTknRefAttr)
   {
      CompTkn        = *(tokentype *)Value;
      if (!Env_IsValidToken (&CompTkn,  McStat)
      ||  CompOp    != aEQ)
      {
        *Status = Query_InvalidTknCompare;

         TraceMsg (0, 
    " Query1_AffixCompare :: Compare Value must be Valid With comp== aEq\n");
         sprintf (Msg, "     for ClassId=%4u; AttrId=%4u.\n",
               ClassId, AttrId);
         TraceMsg (0, Msg);
      }
   }

TRACK(TrackBak,"Query_CheckType\n");
return(STAT);
}


boolean    Query_GetClassList
                      (tokentype     *DbToken,     dbmsquery     *Query,
                       ft F,lt Z,zz  *Status,      numtype       *LocalObjNum)
{
dbheader     *DbHdr        = NullPtr;
int           i            = 0;
numtype      *EnvObjs      = NullPtr;
int           j            = 0;
numtype       EnvObj       = 0;
boolean       Found        = False;
boolean       NoneFound    = True;

  if (ClassBug)
  if (sprintf(Msg, 
"  Query_GetClassList:: DbId[%4u] ClassId=%4u; Cendents= %s\n",
              DbToken->DbId,  Query->ClassId, BoolStr[Query->CendentsOk]))
      TraceMsg (0, Msg);

  if (TmGetCoreDb      (DbToken,                 McStat,         &DbHdr))
  if (Vchunk_Create    (DbToken,    sizeof (tokentype),  2000,   1000,
                        McStat,                  &Query->ResultsDir))
     Query->NumResults   = 0;

  if (Normal(*Status))
  if (Mem_GetObjNum    (DbHdr->LocalObjNumMap,    Query->ClassId, 
                        McStat,                   LocalObjNum))
  if (Query->CendentsOk)
  {
     if (OaBug  || DeBug)
     {
        HmDumpDbHdr (0, DbToken, McStat);
        TraceMsg  (0, "   LocalObjNumMap   ::");
        Mem_DumpChunkChunk (0, DbHdr->ObjNumMap);
        Mem_DumpChunkChunk (0, DbHdr->LocalObjNumMap);
     }

     i = 1;
     /* LocalObjNumMap maps the EnvObjNum to LocalObjNum if valid       */
     /* ObjNumMap maps the Local ObjNum[i]  to the EnvObjNum            */
     if (Mem_GetChunkAddr(DbHdr->ObjNumMap,
                          McStat,                 (addrtype *)&EnvObjs))
     if (BitVec_Create   (DbHdr->LocalObjCount+1,       0,
                          McStat,                      &Query->LocalBitVec))
     while (Normal(*Status)
     &&   ++i  < DbHdr->LocalObjCount)
     {
        j          = 0;
        EnvObj     = EnvObjs[i];
        Found      = False;
        while (j   < Query->NumClasses
        &&    !Found) 
        {
           if (EnvObjs[i]  == Query->ClassSet[j])
           {
              NoneFound   = False;
              Found       = True;
              BitVec_PutBit  (Query->LocalBitVec,  i,  1,  McStat);
           }
         ++j;  
        }
     }

  } else if (*LocalObjNum)
     NoneFound     = False;

  if (Normal(*Status))
  if (*LocalObjNum == 0
  ||  NoneFound    == True) 
  {
    *Status        = Iterate_EndOf;
     return (True);
  }

TRACK(TrackBak,"Query_GetClassList\n");
return(STAT);
}

void       Query_Dump (dbmsquery     *Query)
{

return;
}


void       Query_DumpComdObj (querycomd     *ComdObj)
{

return;
}


void       Query_DumpSortObj (querysort     *SortObj)
{
return;
}


void       Query_DumpFieldObj (dbmsquery    *Query,   queryfield    *FieldObj)
{
addrtype   ValueAddr  = (addrtype )((char *)Query->Values 
                                          + FieldObj->ValueOffset);

return;
}


/*+-----------------------------------------------------------------------+
~M                         END OF QUERY.C                                 !
  +-----------------------------------------------------------------------+*/
