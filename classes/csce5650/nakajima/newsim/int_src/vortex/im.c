/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Shell - Im.c */
#define   IM
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "mem010.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "env1.h"
#include "km.h"
#include "trans.h"
#include "vchunk.h"
#include "oa0.h"
#include "im.h"

/* these Constants are defined in the initial BuildEnv run.              */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        DbId_AttrNum               (numtype )3

#define        Id_AttrNum                 (numtype )143

#define        DbFileName_AttrNum         (numtype )158

#define        WenvDbs_Hndl            (handletype )692
#endif

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

#define MemMakeChunk       Mem_MakeChunk 
#define MemPutStackPtr     Mem_PutStackPtr
#define MemPushSomeBytes   Mem_PushSomeBytes
#define DbmLoadDbHdr       Dbm_LoadDbHdr 

extern boolean OaDumpObject   (int           TraceSwi,  tokentype *Token,
                               ft F,lt Z,zz *Status);

extern boolean OaDumpEmbedded (int           TraceSwi,  numtype       MemLoc,
                               tokentype    *Token,     addrtype      Object, 
                               handletype    BuildDesc, numtype       NumMaps,
                               sizetype      MapOffset, ft F,lt Z,zz *Status);

extern boolean DbmLoadDbHdr   (tokentype    *DbToken, char      *LoadFileName,
                               ft F,lt Z,zz *Status,  dbheader **CoreDb);

extern boolean EnvFetchObjHandle
                              (numtype   ObjNum, 
                               ft F,lt Z,zz *Status,   handletype *ObjHandle);

extern tokentype  WenvTkn;
extern idtype     RenvId;

extern numtype    F77StructDir;

/*+-----------------------------------------------------------------------+
~IM                         IMAGE MAN                                     !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....     PRIMAL        ....
   .......................................................................*/
/* C; C++                                                                 */      
boolean ImageCreateDb     (char        *SchemaName,
                           char        *DbName,    char        *NewFileName,
                           dbaccesstype DbAccess,
                           ft F,lt Z,zz *Status,   tokentype   *DbToken)
{
    if (Normal(*Status))
    if (!KernelFindIn    (WenvDbs_Hndl,   &WenvTkn,   (addrtype )DbName,
                          McStat,          DbToken))
    {
     *Status = Env_Normal;
      KernelCreateDb     (SchemaName,   DbName,   NewFileName,
                          DbAccess,
                          McStat,                 DbToken);
    }

TRACK(TrackBak,"ImageCreateDb\n");
return (STAT);
}





/* C; C++                                                                 */      
boolean ImageConfigClass (tokentype    *PrimalTkn, numtype       ClassId,
                          numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          ft F,lt Z,zz *Status)
{

  OaConfigClass   (PrimalTkn,    ClassId,     RgnEntrys,    AllocRgns,
                   ExtendRgns,   AllocXs,     ExtendXs,     AccessType,
                   DiskCache,    CacheLimit,  CacheLoad,    McStat);

TRACK(TrackBak,"ImageConfigClass\n");
return (STAT);
}


/* .......................................................................
                    ....      OBJECT        ....
   .......................................................................*/

/* C                                                                      */      
boolean ImageDbCreate     (tokentype    *Anchor,  idtype        ClassId,
                           ft F,lt Z,zz *Status,  tokentype    *Token)
{
    KernelCreateObject   (Anchor, ClassId, McStat, Token);


TRACK(TrackBak,"ImageDbCreate\n");
return (STAT);
}
 

 


/* C                                                                      */      
boolean ImageNewObject (tokentype    *Anchor,   numtype    ClassId,
                        ft F,lt Z,zz *Status,
                        tokentype    *Token,    addrtype  *This)
{
    if (OaCreateObject    (Anchor,   ClassId,  McStat,  Token))
    if (C_ObjectNewImage  (Token,    ClassId,  C_Apl,   McStat,  This))
  
    if (DeBug)
           OaDumpObject    (0, Token, McStat);

TRACK(TrackBak,"ImageNewObject\n");
return (STAT);
}
 

/* C                                                                      */      
boolean ImageGetObject (tokentype     *Token,
                        ft F,lt Z,zz  *Status,  addrtype      *This)
{
addrtype  That  = *This;

    if (*This == NullPtr)
    {
        Build_ByCpp = False;
        C_GetObjectImage (Token,   C_Apl,     McStat,    This);

    } else {
        Build_ByCpp = True;
        C_FaxToThis      (Token,   That,      Cpp_Apl,   McStat);

        if (!Normal(*Status) || DeBug || OaBug)
        if (sprintf (Msg, 
                     "  Image_GetObject for This @%x\n", That))
           TraceMsg (0, Msg);
    }

TRACK(TrackBak,"ImageGetObject\n");
return (STAT);
}
 
 
/*    C++                                                                 */      
boolean ImageCreateObject (krnlobjtype  *This,    tokentype    *Anchor,
                           idtype        ClassId, numtype       AplId,
                           ft F,lt Z,zz *Status)
{
tokentype     DbToken;

    if (DeBug || OaBug)
    if (sprintf (Msg, 
                 " Image::CreateObject Type[%2u] : Anchor[%2u:%4u]\n",
                  ClassId, Anchor->DbId, Anchor->Handle))
       TraceMsg (0, Msg);

    if (OaCreateObject   (Anchor,   (numtype  )ClassId,  McStat,  &DbToken))
    if (C_FaxToThis      (&DbToken, (addrtype )This,     AplId,    McStat))

    if (DeBug)
           OaDumpObject    (0, &This->Token, McStat);

TRACK(TrackBak,"ImageCreateObject\n");
return (STAT);
}
 

/*    C++                                                                 */      
boolean Image_IsActive (tokentype    *Token,
                        ft F,lt Z,zz *Status,   addrtype      *This)
{
dbheader   *CoreDb         = NullPtr;
numtype     MemLoc         = 0;

 *This   = NullPtr;

  if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
  if (HmGetObjectMemLoc    (CoreDb,  Token->Handle,  McStat,   &MemLoc))
  if (MemLoc)
  {
      HmGetObjectAddr    (CoreDb,    Token->Handle,  McStat,   This);

      return (True);
  }

STAT;
return (False);
}


/* C; C++                                                                 */      
boolean ImageCommitObject    (numtype       AplId,  tokentype    *Token,
                              addrtype      This,   boolean       Release,
                              ft F,lt Z,zz *Status)
{
bytessize   ImageSize   = 0;

   if (ClassBug || DeBug || OaBug)
      TraceMsg (0,  " ImageCommitObject ::\n");

   if (AplId == Cpp_Apl)
   {
     C_ReFaxToDb     (Token,    This,       Release,   True,  AplId,  
                      McStat,   &ImageSize);

   } else if (AplId == C_Apl) {

     C_ReFaxToDb     (Token,    This,       Release,   True,  AplId,
                      McStat,   &ImageSize);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageCommitObject\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageDeleteObject (tokentype    *Token,  numtype       AplId,
                           ft F,lt Z,zz *Status)
{
    C_DeleteObject (Token, AplId, McStat);

TRACK(TrackBak,"ImageDeleteObject\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageDumpObject (tokentype *Token)
{
statustype ShellStatus  = Env_Normal;

   OaDumpObject  (0,  Token, ShellMcStat);

return (ShellStatus ? True : False);
}
 

/*     4.2.10                                                              */
void    ImageDumpPseudo  (idtype        Ref_DbId,  numtype       ClassId,
                          addrtype      Pseudo)
{

return;
}


 
/* .......................................................................
                    ....       CLASS        ....
   .......................................................................*/
/* C; C++                                                                 */      
boolean ImageInitClassMap (numtype       ClassId, size_t        ObjSize,
                           numtype       AplId,   ft F,lt Z,zz *Status)
{
indextype    StackPtr  = 0;
f77tokentype F77Struct;


   TransInitMap         (ClassId,    ObjSize,    AplId,  McStat);

   if (AplId        == F77_Apl)
   if (F77StructDir == 0)
   if (Normal(*Status))
   if (MemMakeChunk     (sizeof (f77tokentype),  50,            10,
                         RenvId,                 McStat,       &F77StructDir))
   if (MemPutStackPtr   (F77StructDir,           0,             McStat))
       MemPushSomeBytes (F77StructDir,            sizeof (f77tokentype),
                         (addrtype )&F77Struct,   McStat,       &StackPtr);

TRACK(TrackBak,"ImageInitClassMap\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageAppendToMap  (numtype       AttrId,  size_t      Offset,
                           numtype       AplId,   addrtype    AplStruct,
                           ft F,lt Z,zz *Status)
{
indextype   StackPtr  = 0;

   TransAppendToMap     (AttrId,    Offset,  AplId,  McStat);

   if (AplId          == F77_Apl)
       MemPushSomeBytes (F77StructDir,            sizeof (f77tokentype),
                         (addrtype )AplStruct,    McStat,       &StackPtr);

TRACK(TrackBak,"ImageAppendToMap\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageInvokeMap    (numtype       AplId,   ft F,lt Z,zz *Status)
{

   if (AplId == Cpp_Apl
   ||  AplId == C_Apl)
   {
     if (TransInvokeMap  (AplId,   McStat))
         TransBuildMap   (AplId,   McStat);

   } else if (AplId == F77_Apl) {
     if (TransInvokeMap  (AplId,   McStat))
         TransBuildMap   (AplId,   McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageInvokeMap\n");
return (STAT);
}


/* C; C++; Trans                                                          */      
boolean ImageGetClassMap (numtype       ClassId,   numtype       AplId,
                          ft F,lt Z,zz *Status,    classmaptype *ClassMap)
{
   if (AplId == Cpp_Apl
   ||  AplId == C_Apl)
       TransGetMap     (ClassId,   AplId,   McStat,  ClassMap);
   else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageGetClassMap\n");
return (STAT);
}




/* C; C++                                                                 */      
boolean ImageFreeClassById   (numtype       AplId,  idtype        DbId, 
                              numtype       ClassId,ft F,lt Z,zz *Status)
{
   if (AplId == Cpp_Apl)
   {
     C_FreeClass     (DbId,     ClassId,   AplId,    McStat);

   } else if (AplId == C_Apl) {

     C_FreeClass     (DbId,     ClassId,   AplId,    McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageFreeClassById\n");
return (STAT);
}
 

boolean ImageFreeClass       (numtype       AplId,  tokentype    *Token,
                              ft F,lt Z,zz *Status)
{
dbheader   *CoreDb         = NullPtr;
numtype     ClassId        = 0;

   if (TmGetCoreDb          (Token,                   McStat,   &CoreDb))
   if (HmGetObjNum          (CoreDb,  Token->Handle,  McStat,   &ClassId))

   if (AplId == Cpp_Apl)
   {
     C_FreeClass     (Token->DbId,     ClassId,   AplId,  McStat);

   } else if (AplId == C_Apl) {

     C_FreeClass     (Token->DbId,     ClassId,   AplId,  McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageFreeClass\n");
return (STAT);
}
 

/* C; C++                                                                 */      
boolean ImageCommitClass  (numtype       AplId,   tokentype    *Token,
                           numtype       ClassId, ft F,lt Z,zz *Status)
{

   if (AplId == Cpp_Apl)
   {
     C_CommitClass     (Token,  ClassId,  AplId,    McStat);

   } else if (AplId == C_Apl) {

     C_CommitClass     (Token,  ClassId,  AplId,    McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageCommitClass\n");
return (STAT);
}
 
boolean Image_GetClassObjectCount
                          (tokentype    *Token,   numtype       ClassId,
                           ft F,lt Z,zz *Status,  numtype      *Count)
{

  Kernel_GetClassObjectCount (Token, ClassId, McStat, Count);


TRACK(TrackBak,"ImageGetClassObjectCount\n");
return (STAT);
}
 
/* .......................................................................
                    ....      VORTEx       ....
   .......................................................................*/
/* C; C++                                                                 */      
boolean ImageFaxToThis       (numtype       AplId,  tokentype    *Token,
                              addrtype      This,   ft F,lt Z,zz *Status)
{
   if (AplId == Cpp_Apl)
   {
      Build_ByCpp = True;
      C_FaxToThis     (Token,    This,   AplId,    McStat);

   } else if (AplId == C_Apl) {

      Build_ByCpp = False;
      C_FaxToThis     (Token,    This,   AplId,    McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageFaxToThis\n");
return (STAT);
}


/* (not called by anyone)                                                 */      
boolean ImageAssertToThis    (tokentype    *Token,    addrtype     Object,
                              ft F,lt Z,zz *Status)
{
numtype      MemLoc    = 0;
dbheader    *CoreDb    = NullPtr;

 if (Token->DbId)
 {
    if (DeBug || OaBug)
       OaDumpObject    (0,        Token,         McStat);

    if (TmFetchCoreDb    (Token,    McStat,       &CoreDb))
    if (CoreDb)
    if (HmGetObjectMemLoc
                         (CoreDb,   Token->Handle, McStat,   &MemLoc))
    if (!MemLoc)
    {
       if (HmPutObjectMemLoc
                         (CoreDb,   Token->Handle, 1,        McStat))
           HmPutObjectAddr  
                         (CoreDb,   Token->Handle, Object,   McStat);

    } else
       *Status = Cpp_DbNotActivated;

 } else
  *Status = Cpp_InvalidToken;

TRACK(TrackBak,"ImageAssertToThis\n");
return (STAT);
}


boolean ImageGetActiveObject
                          (numtype       ClassId,  tokentype    *Token,
                           ft F,lt Z,zz *Status,   addrtype     *Object)
{
addrtype     NewObject = NullPtr;
numtype      MemLoc    = 0;
dbheader    *CoreDb    = NullPtr;
numtype      ObjNum    = 0;

   *Object = NullPtr;

if (Token->DbId)
{
    if (TmFetchCoreDb    (Token,    McStat,       &CoreDb))
    if (CoreDb)
    {
       if (HmGetObjNum   (CoreDb,   Token->Handle, McStat,   &ObjNum))
       if (ObjNum == ClassId)
       {
          if (HmGetObjectMemLoc
                         (CoreDb,   Token->Handle, McStat,   &MemLoc))
          if (MemLoc)
          {
             OaGetObject (Token,                   McStat,   &NewObject);
            *Object = NewObject;
             if (ClassBug || DeBug || OaBug)
             if (sprintf (Msg, 
                          "  Return Address Of Object IN OuterRealm @%x\n",
                         *Object))
                TraceMsg (0, Msg);
          } else
            *Status = Cpp_ObjectNotActivated;

       } else
         *Status = Cpp_NotOfThisClass;

    } else
       *Status = Cpp_DbNotActivated;

} else
  *Status = Cpp_InvalidToken;


TRACK(TrackBak,"ImageGetActiveObject\n");
return (STAT);
}




/* C                                                                      */      
boolean ImageReFaxToDb        (numtype       AplId,  
                               tokentype    *Token,   addrtype      This,
                               boolean       Release, boolean       FreeMem,
                               ft F,lt Z,zz *Status)
{
bytessize   ImageSize   = 0;

   if (AplId == Cpp_Apl)
   {
     C_ReFaxToDb     (Token,    This,      Release,   FreeMem,  AplId,
                      McStat,  &ImageSize);

   } else if (AplId == C_Apl) {

     C_ReFaxToDb     (Token,    This,      Release,   FreeMem,  AplId,
                      McStat,  &ImageSize);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageReFaxToDb\n");
return (STAT);
}


/* C; C++                                                                 */      
boolean ImageFreeDbObject        (numtype       AplId,  tokentype    *Token,
                                  ft F,lt Z,zz *Status)
{
   if (AplId == Cpp_Apl)
   {
     C_FreeObject     (Token,    AplId,    True,     McStat);

   } else if (AplId == C_Apl) {

     C_FreeObject     (Token,    AplId,    True,     McStat);

   } else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageFreeDbObject\n");
return (STAT);
}


/*    C++                                                                 */      
boolean ImageFreeCppObject
                         (tokentype    *Token,     boolean       FreeImage,
                          ft F,lt Z,zz *Status)
{
     C_FreeObject     (Token,    Cpp_Apl,    FreeImage,     McStat);

TRACK(TrackBak,"ImageFreeCppObject\n");
return (STAT);
}


/*    C++                                                                 */      
boolean ImageRevokeObject (numtype       AplId,  tokentype    *Token,
                           ft F,lt Z,zz *Status)
{
   if (AplId == Cpp_Apl)
       OaRevokeCppObject (Token,  McStat);
   else
      *Status = Err_NotImplemented;

TRACK(TrackBak,"ImageRevokeObject\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~M                         END OF IM.C                                    !
  +-----------------------------------------------------------------------+*/
