/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Shell - SHELL.c */
#define  SHELL
#include "defines.h"
#include <stdarg.h>
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "om.h"
#include "voa.h"
#include "oa0.h"
#include "env1.h"
#include "oa1.h"
#include "km.h"
#include "sa.h"
#include "shell.h"


/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        RenvObjs_Hndl          (handletype )674
#endif

#define MemMakeChunk       Mem_MakeChunk
#define MemPutStackPtr     Mem_PutStackPtr
#define MemPutFuncPtr      Mem_PutFuncPtr 
#define MemGetFuncPtr      Mem_GetFuncPtr 

extern boolean EnvFetchClassSize
                              (numtype       ObjNum,
                               ft F,lt Z,zz *Status,  size_t    *ClassSize);

extern boolean EnvFetchLastObjCount
                              (ft F,lt Z,zz *Status,  numtype   *ObjCount);

extern boolean MemMakeChunk   (bytessize     UnitSize,  indextype   AllocQty,
                               indextype     ExtendQty, idtype      CoreDbId,
                               ft F,lt Z,zz *Status,    numtype    *Chunk);

extern boolean MemPutStackPtr (numtype       Chunk,     indextype StackPtr,
                               ft F,lt Z,zz *Status);

extern boolean MemPutFuncPtr  (numtype       Chunk,   indextype     Index,
                               funcptrtype   FuncPtr, ft F,lt Z,zz *Status);


extern boolean MemGetFuncPtr  (numtype       Chunk,   indextype     Index,
                               ft F,lt Z,zz *Status,  funcptrtype  *FuncPtr);

static  numtype      GetTknCodeDir  = 0;   

/*+-----------------------------------------------------------------------+
~SM                       SHELL :: CODE                                   !
  +-----------------------------------------------------------------------+*/


boolean    ShellLoadObjCode   (numtype       ObjNum, funcptrtype CodeAddr,  
                               ft F,lt Z,zz *Status)
{ 
   sprintf (Msg,
          "\n...NOTE... ShellLoadCode:: Class[%4u] will NOT be Activated.\n\n",
            ObjNum);
   TraceMsg (0, Msg);


TRACK(TrackBak,"ShellLoadObjCode\n");
return (STAT);
}


boolean ShellGetObjCode     (numtype       ObjNum,
                             ft F,lt Z,zz *Status,    
                             funcptrtype  *CodeAddr,  size_t  *ClassSize)
{
   if (ClassBug)
   {
      sprintf (Msg, 
               "\n...NOTE... ShellLoadCode:: Class[%4u] IS NOT Activated.\n\n",
               ObjNum);
      TraceMsg (0, Msg);
   }

   if (Normal(*Status))
       EnvFetchClassSize (ObjNum,       McStat,                ClassSize);


   if (DeBug || OaBug) 
   if (sprintf  (Msg, "   Obj CodeAddr class size=%3u\n", *ClassSize))
       TraceMsg (0, Msg);

TRACK(TrackBak,"ShellGetObjCode\n");
return (STAT);
}




boolean    ShellLoadTokenCode (numtype       ObjNum, funcptrtype CodeAddr,  
                               ft F,lt Z,zz *Status)
{ 
numtype   LastEnvObjCount = 0;

   if (!GetTknCodeDir)
   if (EnvFetchLastObjCount  (McStat,         &LastEnvObjCount))
   if (MemMakeChunk   (sizeof (funcptrtype),   LastEnvObjCount+1,
                       Extend_Codes,           RenvId,
                       McStat,                &GetTknCodeDir))
       MemPutStackPtr (GetTknCodeDir,            LastEnvObjCount+1,
                       McStat);

   if (ClassBug)
      MemBug = True;

   if (Normal(*Status))
       MemPutFuncPtr     (GetTknCodeDir, ObjNum, CodeAddr,       McStat);

   if (ClassBug)
      MemBug = False;

   if (ClassBug || DeBug)
   if (sprintf  (Msg, " ShellLoadTokenCode ObjNum = %4u; Chunk[%4u]\n",
                ObjNum, GetTknCodeDir))
       TraceMsg (0, Msg);         

TRACK(TrackBak,"ShellLoadTokenCode\n");
return (STAT);
}


boolean ShellGetTokenCode   (numtype       ObjNum,
                             ft F,lt Z,zz *Status, funcptrtype  *CodeAddr)
{

  *CodeAddr   = NULL;

   if (DeBug || OaBug) 
   if (sprintf  (Msg, "   ShellGetTokenCode... ObjClass= %4u\n", ObjNum))
       TraceMsg (0, Msg);

   if (ClassBug)
       MemBug = True;

   if (GetTknCodeDir)
      MemGetFuncPtr         (GetTknCodeDir, ObjNum,  McStat,  CodeAddr);

   if (ClassBug)
      MemBug = False;


TRACK(TrackBak,"ShellGetTokenCode\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~P                         END OF SHELL.C                                 !
  +-----------------------------------------------------------------------+*/
