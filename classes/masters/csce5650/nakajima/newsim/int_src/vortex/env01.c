/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* c Shell - Env01.c */
#define  ENV01_CSH
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "envdefs.h"
#include "km.h"
#include "im.h"
#include "shell.h"
#include "vchunk.h"
#include "env01.h"

/* these Constants are defined in the initial BuildEnv run.               */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        ObjTraits_Hndl         (handletype )658
#define        RenvObjs_Hndl          (handletype )674
#define        RenvViews_Hndl         (handletype )685
#define        RenvDescs_Hndl         (handletype )688
#define        WenvDbs_Hndl           (handletype )692

#endif


extern  tokentype    RenvTkn;
extern  tokentype    WenvTkn;

extern void    Core_ShowStats (void);
extern void    Mem_ShowStats (void);

extern boolean EnvFetchObjHandle
                             (numtype       ObjNum,
                              ft F,lt Z,zz *Status,   handletype  *ObjHandle);
extern boolean EnvFetchObjName
                             (numtype       ObjNum,
                              ft F,lt Z,zz *Status,   char        *ObjName);

extern boolean EnvPairCode   (numtype       CodeNum,  funcptrtype  CodeAddr,
                              ft F,lt Z,zz *Status);

extern void    DumpChunkChunk(int           Dump,     numtype      Chunk);

extern  numtype     KrnlFreeAddrChunk;
extern  numtype     XmemBlkSizeChunk;
extern  numtype     XmemBlkDirChunk;

extern  numtype     ChunksFreeed;
extern  numtype     ChunksReclaimed;
extern  word        ChunksAllocated;

extern  numtype   CacheAtRgnChunks;
extern  numtype   FileInRgnChunks;
extern  numtype   FileOutRgnChunks;
extern  numtype   CopyInRgnChunks;
extern  numtype   CopyOutRgnChunks;
extern  numtype   CleanRgnChunks;

/*+-----------------------------------------------------------------------+
  !                  ....         ENV        ....                         !
  +-----------------------------------------------------------------------+*/



/*     2.1.2                                                               */
boolean Env_GetTypeId    (char         *TypeName,  
                          ft F,lt Z,zz *Status,    numtype      *TypeId)
{
tokentype  TypeTkn    = NullToken;

/* Return the Handle to the Type Descriptor                                */
  if (ClassBug)
  if (sprintf (Msg, " Env_GetTypeId  for Type = %21s\n",
               TypeName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvDescs_Hndl,               &RenvTkn,
                       (addrtype )TypeName,
                        McStat,          (tokentype *)&TypeTkn))

      KernelGetAttr    (&TypeTkn,                     TypeNum_AttrNum,
                        McStat,          (addrtype   )TypeId);
  if (ClassBug)
  if (sprintf (Msg, "               TypeId             = %3u\n",
               *TypeId))
     TraceMsg (0, Msg);

  if (!Normal(*Status))
  if (sprintf  (Msg, "***ERROR (%6u)***  @Env_GetTypeId\n",
                *Status))
  if (TraceMsg (0, Msg))
  if (sprintf  (Msg, "              TypeName = %s\n\n",
                TypeName))
  if (TraceMsg (0, Msg))
  {
     *Status  = Env_IdErrata;
     return (False);
  }

TRACK(TrackBak,"Env_GetTypeId\n");
return (STAT);
}


/*     2.1.3                                                               */
boolean Env_GetClassId
                      (char         *ClassName,  
                       ft F,lt Z,zz *Status,    numtype      *ClassId)
{
tokentype  ClassTkn    = NullToken;

/* Return the Id to the meta Class type                                    */

  if (ClassBug)
  if (sprintf (Msg, " Env_GetClassId   for      <%s>.\n",
               ClassName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvObjs_Hndl,                &RenvTkn,   
                       (addrtype )ClassName,
                        McStat,          (tokentype *)&ClassTkn))

      KernelGetAttr    (&ClassTkn,                    ObjNum_AttrNum,
                        McStat,         (addrtype    )ClassId);
  if (ClassBug)
  if (sprintf (Msg, "               ClassId            = %3u\n",
               *ClassId))
     TraceMsg   (0, Msg);

  if (!Normal(*Status))
  if (sprintf  (Msg, "***ERROR (%6u)***  @Env_GetClassId\n",
                *Status))
  if (TraceMsg (0, Msg))
  if (sprintf  (Msg, "              ClassName  = %s\n\n",
                ClassName))
  if (TraceMsg (0, Msg))
  {
     *Status  = Env_IdErrata;
     return (False);
  }

TRACK(TrackBak,"Env_GetClassId\n");
return (STAT);
}




/*     2.1.4                                                               */
boolean Env_GetAttrId    (char         *AttrName,  
                          ft F,lt Z,zz *Status,    numtype      *AttrId)
{
tokentype  AttrTkn    = NullToken;

/* Return the Id to the Attribute type                                     */
  if (ClassBug)
  if (sprintf (Msg, " Env_GetAttrId    for      <%s>. \n",
               AttrName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvDescs_Hndl,               &RenvTkn,   
                       (addrtype )AttrName,
                        McStat,          (tokentype *)&AttrTkn))

      KernelGetAttr    (&AttrTkn,                     AttrNum_AttrNum,
                        McStat,          (addrtype   )AttrId);
  if (ClassBug)
  if (sprintf (Msg, "               AttrId             = %3u\n",
               *AttrId))
     TraceMsg   (0, Msg);

  if (!Normal(*Status))
  if (sprintf  (Msg, "***ERROR (%6u)***  @Env_GetAttrId\n",
                *Status))
  if (TraceMsg (0, Msg))
  if (sprintf  (Msg, "              AttrName = %s\n\n",
                AttrName))
  if (TraceMsg (0, Msg))
  {
     *Status  = Env_IdErrata;
     return (False);
  }

TRACK(TrackBak,"Env_GetAttrId\n");
return (STAT);
}


boolean Env_NewFieldStruc
                         (char         *FieldName, 
                          ft F,lt Z,zz *Status,    fieldstruc  **FieldStruc)
{
static bytessizetype StrucSize    = sizeof (fieldstruc);
static bytessizetype TknsSize     = sizeof (fieldspec) * 10;
fieldstruc          *EnvField;

   if (Core_MoreCore  (StrucSize,   McStat,  (addrtype *)&EnvField))
   if (Core_MoreCore  (TknsSize,    McStat,  (addrtype *)&EnvField->FieldTkns))
   if (Env_GetFieldStruc
                      (FieldName,   McStat,               EnvField))
     *FieldStruc     = EnvField;

TRACK(TrackBak,"Env_NewFieldStruc\n");
return (STAT);
}

boolean Env_GetFieldStruc
                         (char         *FieldName, 
                          ft F,lt Z,zz *Status,    fieldstruc   *FieldStruc)
{

/* Return the Field Struc for this Class Member Specification              */
  if (ClassBug)
  if (sprintf (Msg, " Env_GetFieldStruc for <%s>\n",
               FieldName))
     TraceMsg   (0, Msg);

  if (!Kernel_GetFieldStruc
                       (FieldName,     McStat,   FieldStruc))
  if (sprintf  (Msg, "***ERROR (%6u)***  @Env_GetFieldStruc\n",
                *Status))
  if (TraceMsg (0, Msg))
  if (sprintf  (Msg, "              FieldName = %s\n\n",
                FieldName))
  if (TraceMsg (0, Msg))
  {
     *Status  = Env_IdErrata;
     return (False);
  }

TRACK(TrackBak,"Env_GetFieldStruc\n");
return (STAT);
}


boolean Env_DeleteFieldStruc
                         (ft F,lt Z,zz *Status,    fieldstruc  **FieldStruc)
{
static bytessizetype StrucSize    = sizeof (fieldstruc);
static bytessizetype TknsSize     = sizeof (fieldspec) * 10;
fieldstruc          *EnvField     = *FieldStruc;

   if (Core_FreeCoreSpace  ((addrtype *)&EnvField->FieldTkns,     TknsSize,
                            McStat))
       Core_FreeCoreSpace  ((addrtype *)FieldStruc,  StrucSize,   McStat);

TRACK(TrackBak,"Env_DeleteFieldStruc\n");
return (STAT);
}



/*     2.1.5                                                               */
boolean Env_GetSetId     (char         *SetName,   
                          ft F,lt Z,zz *Status,    numtype      *SetId)
{
tokentype  SetTkn    = NullToken;

/* Return the Handle to the Set Descriptor                                 */

  if (ClassBug)
  if (sprintf (Msg, " Env_GetSetId  for Set = %21s\n",
               SetName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvDescs_Hndl,                &RenvTkn,   
                       (addrtype )SetName,
                        McStat,           (tokentype *)&SetTkn))
      KernelGetAttr    (&SetTkn,                        SetNum_AttrNum,
                        McStat,           (addrtype    )SetId);

 *SetId = (numtype )SetTkn.Handle;

  if (ClassBug)
  if (sprintf (Msg, "               SetId  = %3u, SetDesc[ 1:%8u]\n",
               *SetId, SetTkn.DbId, SetTkn.Handle))
     TraceMsg   (0, Msg);

TRACK(TrackBak,"Env_GetSetId\n");
return (STAT);
}


/*     2.1.6                                                               */
boolean Env_GetTupleId   (char         *TupleName, 
                          ft F,lt Z,zz *Status,    numtype      *TupleId)
{
tokentype  TupleTkn    = NullToken;

/* Return the Handle to the Tuple Descriptor                               */

  if (ClassBug)
  if (sprintf (Msg, " Env_GetTupleId  for Tuple = %21s\n",
               TupleName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvDescs_Hndl,                &RenvTkn,   
                       (addrtype )TupleName,
                        McStat,           (tokentype *)&TupleTkn))
     *TupleId = (numtype )TupleTkn.Handle;

  if (ClassBug)
  if (sprintf (Msg, "               TupleId  = %3u, TupleDesc[ 1:%8u]\n",
               *TupleId, TupleTkn.DbId, TupleTkn.Handle))
     TraceMsg   (0, Msg);

TRACK(TrackBak,"Env_GetTupleId\n");
return (STAT);
}


/*     2.1.7                                                               */
boolean Env_GetMatrixId  (char         *MatrixName,
                          ft F,lt Z,zz *Status,    numtype      *MatrixId)
{
tokentype  MatrixTkn    = NullToken;

/* Return the Handle to the Matrix Descriptor                               */

  if (ClassBug)
  if (sprintf (Msg, " Env_GetMatrixId  for Matrix = %21s\n",
               MatrixName))
     TraceMsg   (0, Msg);

  if (KernelFindIn     (RenvDescs_Hndl,                &RenvTkn,   
                       (addrtype )MatrixName,
                        McStat,           (tokentype *)&MatrixTkn))
     *MatrixId = (numtype )MatrixTkn.Handle;

  if (ClassBug)
  if (sprintf (Msg, "               MatrixId  = %3u, MatrixDesc[ 1:%8u]\n",
               *MatrixId, MatrixTkn.DbId, MatrixTkn.Handle))
     TraceMsg   (0, Msg);

TRACK(TrackBak,"Env_GetMatrixId\n");
return (STAT);
}




/*     2.1.15                                                              */
boolean Env_LoadCreateCode
                         (numtype       ClassId,    funcptrtype   CodePtr,
                          ft F,lt Z,zz *Status)
{
/* Load the address of the dynamic Class create code                      */

  if (ClassBug)
  if (sprintf (Msg, " Env_LoadCreateCode    for ClassId = %4u\n",
               ClassId))
     TraceMsg   (0, Msg);

  ShellLoadObjCode   (ClassId,     CodePtr,     McStat);

TRACK(TrackBak,"Env_LoadCreateCode\n");
return (STAT);
}


/*     2.1.16                                                              */
boolean Env_LoadGetTknCode
                         (numtype       ClassId,    funcptrtype   CodePtr,
                          ft F,lt Z,zz *Status)
{
/* Load the address of the dynamic Class create code                      */

  if (ClassBug)
  if (sprintf (Msg, " Env_LoadGetTknCode    for ClassId = %4u\n",
               ClassId))
     TraceMsg   (0, Msg);

  ShellLoadTokenCode   (ClassId,     CodePtr,     McStat);

TRACK(TrackBak,"Env_LoadGetTknCode\n");
return (STAT);
}


/* ........................................................................
                    ....     Meta Class     ....
   ........................................................................*/

/*     2.2.1                                                               */
boolean Env_InitClassMap (numtype       ClassId,   size_t        ObjSize,
                          numtype       AplId,     ft F,lt Z,zz *Status)
{

  if (ClassBug)
  if (sprintf (Msg, " Env_InitClassMap[%4u]... Apl= %3u  ObjSize = %4u\n",
               ClassId,  AplId, ObjSize))
     TraceMsg   (0, Msg);

  ImageInitClassMap         (ClassId,  ObjSize,  AplId,  McStat);

TRACK(TrackBak,"Env_InitClassMap\n");
return (STAT);
}


/*     2.2.2                                                               */
boolean Env_AppendToMap  (numtype       AttrId,    size_t        Offset,
                          ft F,lt Z,zz *Status)
{
addrtype  AplStruct  = NullPtr;

  if (ClassBug
  ||  AttrId == 0)
  if (sprintf (Msg, 
               " Env_AppendToMap ::AttrNum = %3u; Offset = %4u\n",
               AttrId,  Offset))
     TraceMsg   (0, Msg);

  if (AttrId == 0)
  {
     TraceMsg   (0, "***ERROR*** @Env_AppendToMap... NullAttrId\n");
    *Status   = Env_IdErrata;
  }

  if (Normal(*Status))
     ImageAppendToMap   (AttrId,    Offset,     C_Apl,  AplStruct,  McStat);

TRACK(TrackBak,"Env_AppendToMap\n");
return (STAT);
}


/*     2.2.3                                                               */
boolean EnvInvokeMap     (numtype       AplId,     ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg   (0, " Env_InvokeMap\n");

  ImageInvokeMap        (AplId,   McStat);

TRACK(TrackBak,"Env_InvokeMap\n");
return (STAT);
}


/*     2.2.4                                                               */
boolean EnvGetClassMap   (numtype       ClassId,   numtype       AplId,
                          ft F,lt Z,zz *Status,    classmaptype *ClassMap)
{

  if (ClassBug)
  if (sprintf (Msg, " Env_GetClassMap :: ClassId = %3u; AplId = %4u\n",
               ClassId,  AplId))
     TraceMsg   (0, Msg);

  ImageGetClassMap   (ClassId, AplId,   McStat,  ClassMap);

TRACK(TrackBak,"Env_GetClassMap\n");
return (STAT);
}




/* ~   2.2.6                                                               */
boolean Env_GetAttrInfo  (numtype       AttrId,
                          ft F,lt Z,zz *Status,    attrinfotype *AttrInfo)
{
  if (ClassBug)
  if (sprintf(Msg, "   Env_GetAttrInfo Attr= %3u\n", AttrId))
     TraceMsg (0, Msg);

  KernelGetAttrInfo (AttrId,   McStat,  AttrInfo);

  if (ClassBug)
  if (sprintf(Msg, "        AttrInfoType = %21s\n", AttrInfo->AttrName))
     TraceMsg (0, Msg);

TRACK(TrackBak,"Env_GetAttrInfo\n");
return (STAT);
}



/*     2.2.11                                                              */
boolean Env_IsValidToken (tokentype    *Token,     ft F,lt Z,zz *Status)
{
boolean    IsValid   = False;

  if (ClassBug)
  if (sprintf (Msg, " Env_IsValidToken:: [%3u:%8u]\n",
               Token->DbId,  Token->Handle))
     TraceMsg   (0, Msg);

  if (Token->DbId > 0)
     IsValid   = TmIsValid         (Token,                     McStat);

STAT;
TRACK(TrackBak,"Env_IsValidToken\n");

 *Status   = Env_Normal;

return (IsValid);
}


/*     2.2.12                                                              */
boolean Env_TokenIsEquiv (tokentype    *Token0,    tokentype    *Token1)
{
   if (Token0->DbId   == Token1->DbId
   &&  Token0->Handle == Token1->Handle)
     return (True);
   else
     return (False);
}

/* DCH 01/24/95 */
/*     2.2.13                                                              */
boolean Env_ReclaimHandles (tokentype    *Token,     ft F,lt Z,zz *Status)
{
boolean    IsValid   = False;

  if (ClassBug)
  if (sprintf (Msg, " Env_ReclaimHandles:: [%3u:%8u]\n",
               Token->DbId,  Token->Handle))
     TraceMsg   (0, Msg);

  if (Token->DbId > 0)
     IsValid   = TmReclaimHandles (Token,                     McStat);

  STAT;
  TRACK(TrackBak,"Env_ReclaimHandles\n");

  *Status   = Env_Normal;

return (IsValid);
}

/* ........................................................................
                    ....     Itterate       ....
   ........................................................................*/



/*     2.7.0                                                               */
boolean Env_GenerateRandomNumbers
                         (indextype     Count,     ft F,lt Z,zz *Status)
{
int            i            = 0;
unsigned       Seed         = 0;

  Seed          = 1008;
  srand(Seed);

  if (Count == 0)
     return (STAT);

  if (Count  > 50000)
      Count  = 50000;

  else if (Count  < 10000)
      Count  = 10000;

  Random_Limit  = Count;

  if (!Vchunk_IsValidToken   (&Random_Tkn,   McStat))
  {
    *Status   = Env_Normal;
     sprintf (Msg, "\n INITIALIZE <%8u>  RANDOM NUMBERS :: seed= %u\n",
              Random_Limit, Seed);
     TraceMsg (0, Msg);

     if (Vchunk_Create      (&WenvTkn,  sizeof (long),   Count,  10000,
                             McStat,                    &Random_Tkn))
     if (Vchunk_PutStackPtr (&Random_Tkn,                Count, 
                             McStat,        (addrtype *)&Random_Dir))
     for (i = -1; ++i < Random_Limit; )
     {
        Random_Dir[i] = rand();
     }
     if (Normal(*Status))
         Vchunk_Commit      (&Random_Tkn,   False,  McStat);

     Random_ItNum  = 0;

     sprintf (Msg, "\n RANDOM NUMBERS Dir [%4u:%8u]. Count= %8u\n",
              Random_Tkn.DbId, Random_Tkn.Handle, Random_Limit);
     TraceMsg (0, Msg);

  } else {

     if (Normal(*Status))
     if (Vchunk_GetStackPtr (&Random_Tkn,    McStat,  &Random_Limit))
     if (Vchunk_GetAddress  (&Random_Tkn,    McStat,  (addrtype *)&Random_Dir))
     {
        Random_ItNum  = 0;
        sprintf (Msg, "\n RANDOM NUMBERS Dir [%4u:%8u].\n",
                 Random_Tkn.DbId, Random_Tkn.Handle);
        TraceMsg (0, Msg);
     }
  }

TRACK(TrackBak,"Env_GenerateRandomNumbers\n");
return (STAT);
}

/*     2.7.0                                                               */
int   Env_Random (numtype       Limit) 
{
int  RandomNumber = 0;

   Random_ItNum++;
   if (Random_Dir     == NullPtr)
      RandomNumber     = rand()                   % Limit;
   else {
      if (Random_ItNum > Random_Limit)
          Random_ItNum = 0;
      RandomNumber     = Random_Dir[Random_ItNum] % Limit;
   }

return(RandomNumber);
}

/*+-----------------------------------------------------------------------+
~M                         END OF ENV01.C                                 !
  +-----------------------------------------------------------------------+*/
