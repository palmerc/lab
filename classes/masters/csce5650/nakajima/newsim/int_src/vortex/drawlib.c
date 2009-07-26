/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* C APPS - draw01.c */

/* ******************************  DrawLib.c  ***************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       2.1   DrawLib - main directory of draw objects.                 */
/* *********************************************************************** */

#define DRAWLIB
#include <stdarg.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "drawlib.h"
#include "drawobj.h"
#include "point.h"
#include "rect.h"
#include "tree01.h"
#include "vchunk.h"
#include "list01.h"

/* NOTE: the 'Random' macro assumes 'Limit' is a power of 2 */
#define   Random(Limit)   (RANDOM_FUNC () & (Limit -1))

/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                              METHODS                                  !
  +-----------------------------------------------------------------------+*/

 extern boolean Rects_InitLibrary
                         (ft F,lt Z,zz *Status);
 
 extern boolean Draw7_Init
                         (ft F,lt Z,zz *Status);
 

/*+-----------------------------------------------------------------------+*/
/* DRAW LIB                                                                */
/*+-----------------------------------------------------------------------+*/

boolean DrawLib_InitLibrary
                         (ft F,lt Z,zz *Status)
{
/* Initialize the Class maps of those Class types accessed in this program */
  if (DrawLib_Class == 0)
  {
     TraceMsg (0, "\n Initialize the Class maps\n");
/*                                                       <CFS [14.0.0 ] >  */
     if (List01_Init                             (McStat))

     if (DrawLib_InitClass                       (McStat))

     if (DrawObj_InitClass                       (McStat))
     if (NamedDrawObj_InitClass                  (McStat))
     if (LibRectangles_InitClass                 (McStat))


     if (Point_InitClass                         (McStat))

     if (CartesianPoint_InitClass                (McStat))

     if (PolarPoint_InitClass                    (McStat))

     if (Rectangle_InitClass                     (McStat))

         Rects_InitLibrary                       (McStat);
      if (Normal(*Status))
         Draw7_Init                              (McStat);

  }
  LibBug = False;

TRACK(TrackBak,"DrawLib_InitLibrary\n");
return (STAT);
}

boolean DrawLib_InitClass  (ft F,lt Z,zz *Status)
{
  if (DrawLib_Class  == 0)
  {
/*                                                       <CFS [2.1.4  ] >  */
     if (EnvGetMemberId     ("DrawId",             McStat, &DrawId_Attr))
     if (EnvGetMemberId     ("DrawObjDir",         McStat, &DrawObjDir_Attr))
     if (EnvGetMemberId     ("VdrawObjDir",        McStat, &VdrawObjDir_Attr))
/*                                                       <CFS [2.1.3  ] >  */
         EnvGetClassId      ("DrawLib",      McStat, &DrawLib_Class);
  }

  if (Normal(*Status))
/*                                                       <CFS [2.2.1  ] >  */
  if (EnvInitClassMap    (DrawLib_Class,       sizeof(DrawLib),   
                          C_Apl,               McStat))
/*                                                       <CFS [2.2.3  ] >  */
      EnvInvokeMap       (C_Apl,               McStat);

  if (Normal(*Status))
  if (DrawObjs_InitClass                      (McStat))
  if (NamedDrawObjs_InitClass                 (McStat))
      LibRectangles_InitClass                 (McStat);

TRACK(TrackBak,"DrawLib_InitClass\n");
return (STAT);
}



boolean DrawLib_InitDb   (char         *DbName,  char         *DbFileName,  
                          numtype       Option,
                          ft F,lt Z,zz *Status,  tokentype    *PrimalTkn)
{
numtype        RgnEntries      = 0;

  DrawLib_InitLibrary    (McStat);

 /*  ..................   CREATE THE DRAW   LIBRARY  ...................  */
  if (Normal(*Status))
  if (!DrawLib_ActivateDbByName
                         (DbName,       DbFileName,      Read_Write,
                          McStat,      &DrawLib01))
  {
    *Status        = Env_Normal;
     if (Option    > 10000)
        RgnEntries = Option;
     else
        RgnEntries = 10000;

     if (DrawLib_CreateDb
                         (DbName,    DbFileName,
                          Read_Write,
                          RgnEntries,    RgnEntries*3,
                          10000,         10000,
                          10010,         10000,   
                          McStat,        PrimalTkn,    &DrawLib01))
         Primal_ConfigYourClass 
                         (PrimalTkn,    DrawObj_Class,  1000, 20, 10,
                          10000,   1000, Read_Write,    True, 20, 20, McStat);
  } else
   *PrimalTkn  =  DrawLib01->Token;

TRACK(TrackBak,"DrawLib_InitDb\n");
return (STAT);
}


boolean DrawLib_QueryDb
                         (char         *DbName,
                          ft F,lt Z,zz *Status,    boolean      *Active)
{

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  DrawLib_QueryDb:: <%21s>\n",
                 DbName))

  *Active    = False;
  *Status    = Err_NotImplemented;

TRACK(TrackBak,"DrawLib_QueryDb\n");
return (STAT);
}


boolean DrawLib_CreateDb
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, DrawLib     **PrimalObj)
{
numtype       GrpNum       = 0;
tokentype     GrpTkn       = NullToken;
grpheader    *GrpHdr       = NullPtr;
sizetype      EntrySize    = sizeof (handletype);
numtype       AllocRgns    = (RgnHndls / 125) + 27;
numtype       RgnEntrys    = 125;
numtype       AllocXs      = 1000;
numtype       BaseIndex    = 2;
accesstype    AccessType   = Read_Write;
boolean       DiskCache    = True;
numtype       CacheLimit   = AllocRgns / 10;
numtype       CacheLoad    = AllocRgns / 10;
numtype       NumBuffRgns  = AllocRgns;
numtype       BaseRegion   = 0;
tokentype     TreeTkn1     = NullToken;
tokentype     TreeTkn2     = NullToken;
attrobjtype   AttrDesc;
sizetype      KeySize      = 0;
typetype      KeyType      = aInteger;
numtype       TreeOrder    = 100;

  if (ClassBug)
  if (sprintf(Msg, "  DrawLibCreate:: Db= '%32s'; Handles Per Region=%4u\n", 
              DbName,  RgnHndls))
    TraceMsg (0, Msg);

/*                                                       <CFS [3.1.1  ] >  */
  if (Primal_CreateDb ("DrawSchema", DbName,     DbFileName,  DbAccess,     
                       HndlRgns,     RgnHndls,   VhndlRgns,   RgnVhndls,
                       XmemRgns,     RgnXmems,   McStat,      PrimalTkn))
/*                                                       <CFS [4.1.4  ] >  */
  if (ClassBug)
      Object_Dump (PrimalTkn);


  if (Normal(*Status))
/*                                                       <CFS [12.1.1 ] >  */
  if (GrpNewPacket    (PrimalTkn, EntrySize,   AllocRgns,   RgnEntrys,
                       AllocXs,   BaseIndex,   AccessType,  DiskCache,
                       CacheLimit,CacheLoad,   NumBuffRgns,
                       McStat,   &GrpNum,     &GrpHdr,     &BaseRegion))
/*                                                       <CFS [6.2.2  ] >  */
  {
     GrpTkn.DbId     = PrimalTkn->DbId;
     GrpTkn.Handle   = GrpNum;
     Attr_ValuePut   (PrimalTkn, DrawObjDir_Attr,(addrtype)&GrpTkn,
                      McStat);
  }
/*                                                       <CFS [3.1.10 ] >  */
#ifndef   __FREEZE_VCHUNKS__
  if (Normal(*Status))
     PrimalCommitDb   (PrimalTkn,   "\0",     False,      McStat);
#endif /* __FREEZE_VCHUNKS__  */

  if (RgnHndls == 20001)
    return (STAT);

  if (Normal(*Status))
/*                                                       <CFS [4.2.1  ] >  */
  if (Object_GetImage (PrimalTkn,      McStat,  (addrtype *)PrimalObj))


/*                                                       <CFS [7.0.2  ] >  */
  if (SetInitSet      (DrawObjs_Set,           PrimalTkn,   RgnHndls, 10,
                       McStat))

/*                                                       <CFS [6.0.1  ] >  */
  if (Env_GetAttrInfo    (Nam_AttrNum,   McStat,  &AttrDesc))
  {
     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
/*                                                       <CFS [16.1.1 ] >  */
     Tree_Create     (NamedDrawObjs_Set,      PrimalTkn,   TreeOrder, 
                      RgnHndls,               KeySize,     KeyType,
                      McStat,                &TreeTkn1);
  }

/*
  if (Env_GetAttrInfo    (Id_AttrNum,   McStat,  &AttrDesc))
  {
     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
     Tree_Create     (LibRectangles_Set,      PrimalTkn,   TreeOrder, 
                      RgnHndls,               KeySize,     KeyType,
                      McStat,                &TreeTkn2);
  }
*/

  if (Normal(*Status))
  {
      (*PrimalObj)->OwnerNamedDrawObjs = TreeTkn1.Handle;

/*
      (*PrimalObj)->OwnerLibRectangles = TreeTkn2.Handle;
*/
      GrpHdr->EnvObjType         = EnvType_AttrNum;
      VpartsDir                  = GrpNum;

    sprintf (Msg, "   DrawLibCreate:: Db[%3u]; VpartsDir=%4u\n", 
             PrimalTkn->DbId,  VpartsDir);

    TraceMsg (0, Msg);
    if (ClassBug)
    {
       Object_Dump (PrimalTkn);
       Object_Dump (&TreeTkn1);
    }
  }

  if (Normal(*Status))
  {
/*                                                       <CFS [4.2.5  ] >  */
       if (Object_CommitImage
                      (PrimalTkn,   False,               McStat))
#ifndef   __FREEZE_VCHUNKS__
       if (PrimalCommitDb  (PrimalTkn,   "\0",     False,     McStat))
       if (ClassBug)
          Object_Dump (PrimalTkn);
#endif /* __FREEZE_VCHUNKS__  */

       if (Normal(*Status))
           Object_GetImage   (PrimalTkn,   McStat,  (addrtype *)PrimalObj);
       DrawLib01         = *PrimalObj;
       Print_Msg         = True;
       WriteHdr          = False;

  } else if (*Status    == Csh_DbAlreadyExists) {

   *Status               = Env_Normal;
    if (Object_GetImage     (PrimalTkn,    McStat,    (addrtype *)PrimalObj))

/*                                                       <CFS [12.0.2 ] >  */
    if (GrpGetPacket        (PrimalTkn,  (*PrimalObj)->DrawObjDir.Handle,
                             McStat,                             &GrpHdr))
    {
       VpartsDir         = (*PrimalObj)->DrawObjDir.Handle;
       DrawLib01         = *PrimalObj;
    }
  }
  if (ClassBug)
  Object_Dump (PrimalTkn);

  if (Normal(*Status))
  if (ClassBug)
  if (TraceMsg (0, "\n DUMP VpartsDir Grp Struct::\n"))
     Grp_DumpStruct (PrimalTkn,  VpartsDir,  McStat);

TRACK(TrackBak,"DrawLib_CreateDb\n");
return (STAT);
}

boolean DrawLib_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    DrawLib     **PrimalObj)
{
grpheader    *GrpHdr         = NullPtr;

/*                                                       <CFS [3.1.2  ] >  */

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  DrawLib_ActivateDbByName:: <%21s>\n",
                 DbName))
   if (TraceMsg (0, Msg))
   if (sprintf  (Msg, "                 FileName := <%21s>; Access=%s\n",
                 DbFileName, DbAccessStr[AccessType]))
      TraceMsg  (0, Msg);

  *PrimalObj = NullPtr;

   if (!Primal_ActivateDbByName
                         (DbName,    DbFileName, AccessType,
                          McStat,               &LibTkn010))
       return (False);

   if (Object_GetImage   (&LibTkn010,    McStat, (addrtype *)PrimalObj))

   if ((*PrimalObj)->DrawObjDir.Handle != 0)
   if (GrpGetPacket      (&LibTkn010, (*PrimalObj)->DrawObjDir.Handle,
                          McStat,               &GrpHdr))
      VpartsDir          = (*PrimalObj)->DrawObjDir.Handle;

   DrawLib01          = *PrimalObj;

   if (Normal(*Status))
   if (ClassBug)
   if (TraceMsg (0, "\n DUMP VpartsDir Grp Struct::\n"))
      Grp_DumpStruct (&LibTkn010,  VpartsDir,  McStat);

TRACK(TrackBak,"DrawLib_ActivateDbByName\n");
return (STAT);
}


boolean DrawLib_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    DrawLib     **PrimalObj)
{
grpheader    *GrpHdr         = NullPtr;
char     FileName[128];

   strcpy (FileName, "\0");

   if (DbFileName  != NullPtr)
     strcpy (FileName, DbFileName);

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  DrawLibActivate:: [%3u:%8u]  DbFile= '%32s'\n",
                 PrimalTkn->DbId, PrimalTkn->Handle,  FileName))
      TraceMsg  (0, Msg);

/*                                                       <CFS [3.1.3  ] >  */
   if (!Primal_ActivateDbByToken
                           (PrimalTkn,    DbFileName,  AccessType, McStat))
       return (False);

   if (Normal(*Status))
       Object_GetImage     (PrimalTkn,    McStat,    (addrtype *)PrimalObj);

   if (Normal(*Status))
   if ((*PrimalObj)->DrawObjDir.Handle != 0)
   if (GrpGetPacket      (PrimalTkn,  (*PrimalObj)->DrawObjDir.Handle,
                          McStat,               &GrpHdr))
      VpartsDir          = (*PrimalObj)->DrawObjDir.Handle;

   DrawLib01          = *PrimalObj;

   if (Normal(*Status))
   if (ClassBug)
   if (TraceMsg (0, "\n DUMP VpartsDir Grp Struct::\n"))
      Grp_DumpStruct (&LibTkn010,  VpartsDir,  McStat);

TRACK(TrackBak,"DrawLib_ActivateDbByToken\n");
return (STAT);
}


boolean DrawLib_DeleteDb (tokentype    *PrimalTkn, ft F,lt Z,zz *Status)
{
/*                                                       <CFS [3.1.11 ] >  */

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  DrawLib_DeleteDb:: [%3u:%8u]\n",
                 PrimalTkn->DbId, PrimalTkn->Handle))
      TraceMsg  (0, Msg);

  *Status = Err_NotImplemented;

TRACK(TrackBak,"DrawLib_DeleteDb\n");
return (STAT);
}
 

numtype DrawLib_NewId    (ft F,lt Z,zz *Status)
{
numtype    NewId    = 0;
boolean    NotFound = True;
tokentype  FoundTkn = NullToken;

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  DrawLib_NewId:: [%3u:%8u]\n",
                 LibTkn010.DbId, LibTkn010.Handle))
      TraceMsg  (0, Msg);

   NewId        = ++DrawLib01->DrawId;
   NotFound     = False;

   while (Normal(*Status)
   &&     NotFound)
   {
     NewId        = Random((1024*256)) + 1;

     DrawObjs_FindIn   (&LibTkn010,  (addrtype )&NewId,
                        McStat,                 &FoundTkn);
     if (*Status     == Set_EmptySet
     ||  *Status     == Set_EndOfSet
     ||  *Status     == Set_NotFound)
     {
        *Status       = Env_Normal;
         NotFound     = False;
         if (NewId    > DrawLib01->DrawId)
            DrawLib01->DrawId = NewId;
     } else if (*Status   == Set_AlreadyInSet)
        *Status       = Env_Normal;
   }

return (NewId);
}
 

/*+-----------------------------------------------------------------------+
~M                        END OF DRAWLIB.C                                !
  +-----------------------------------------------------------------------+*/
