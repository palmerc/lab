/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ***************************  PersonLib.c  ****************************  */
/* *  Test program for the PersonLib library                               */
/* *                               Recipies in the Developers CookBook     */
/* *       2.1   PersonLib - main directory of people by address           */
/* *********************************************************************** */

#define PERSONLIB
#include "emplib.h"
#include "empobj.h"
#include "tree01.h"
#include "vchunk.h"

/*+-----------------------------------------------------------------------+
~DL                          EMPOLOYE LIBRARY                             !
  !                              METHODS                                  !
  +-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+*/
/* EMP LIB                                                                 */
/*+-----------------------------------------------------------------------+*/

boolean PersonLib_InitLibrary
                         (ft F,lt Z,zz *Status)
{
/* Initialize the Class maps of those Class types accessed in this program */
/* i.e all Class types in schema that will be instantiated.                */

  if (PersonLib_Class == 0)
  {
     TraceMsg (0, "\n Initialize the Class maps of this schema.\n");
     if (PersonLib_InitClass                       (McStat))
     if (Address_InitClass                         (McStat))
         Person_InitClass                          (McStat);

  }

TRACK(TrackBak,"PersonLib_InitLibrary\n");
return (STAT);
}


boolean PersonLib_InitClass  (ft F,lt Z,zz *Status)
{
  if (PersonLib_Class  == 0)
  {
     if (Env_GetAttrId     ("Descriptor",         McStat, &Descriptor_Attr))
     if (Env_GetAttrId     ("MaxId",              McStat, &MaxId_Attr))
     if (Env_GetAttrId     ("CompanyName",        McStat, &CompanyName_Attr))
     if (Env_GetAttrId     ("AddressTkn",         McStat, &AddressTkn_Attr))
         Env_GetClassId    ("PersonLib",          McStat, &PersonLib_Class);
  }

  if (Normal(*Status))
  if (Env_InitClassMap     (PersonLib_Class,      sizeof (PersonLib),   
                            C_Apl,                McStat))
      Env_InvokeMap        (C_Apl,                McStat);

  if (Normal(*Status))
  if (PersonObjs_InitClass                       (McStat))
      PersonNames_InitClass                      (McStat);

TRACK(TrackBak,"PersonLib_InitClass\n");
return (STAT);
}


boolean Person_InitDb    (char         *DbName,  char         *DbFileName,  
                          numtype       Option,
                          ft F,lt Z,zz *Status,  tokentype    *PrimalTkn)
{
numtype        RgnEntries      = 0;

  PersonLib_InitLibrary  (McStat);

 /*  ..................   CREATE THE PERSON LIBRARY  ...................  */
  if (Normal(*Status))
  if (!PersonLib_ActivateDbByName
                         (DbName,       DbFileName,      Read_Write,
                          McStat,      &PersonLib01))
  {
    *Status        = Env_Normal;
     if (Option    > 10000)
        RgnEntries = Option;
     else
        RgnEntries = 10000;

     if (PersonLib_CreateDb
                         (DbName,    DbFileName,
                          Read_Write,
                          RgnEntries,    RgnEntries*3,
                          10000,         10000,
                          10010,         10000,   
                          McStat,        PrimalTkn,    &PersonLib01))
         Primal_ConfigYourClass 
                         (PrimalTkn,    Person_Class,  1000, 20, 10,
                          10000,   1000, Read_Write,    True, 20, 20, McStat);
  } else
   *PrimalTkn  =  PersonLib01->Token;

TRACK(TrackBak,"Person_InitDb\n");
return (STAT);
}


boolean PersonLib_QueryDb
                         (char         *DbName,
                          ft F,lt Z,zz *Status,    boolean      *Active)
{

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  PersonLib_QueryDb:: <%21s>\n",
                 DbName))

  *Active    = False;
  *Status    = Err_NotImplemented;

TRACK(TrackBak,"PersonLib_QueryDb\n");
return (STAT);
}


boolean PersonLib_CreateDb
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, PersonLib     **PrimalObj)
{
sizetype      EntrySize    = sizeof (handletype);
numtype       AllocRgns    = (RgnHndls / 125) + 27;
tokentype     TreeTkn      = NullToken;
attrobjtype   AttrDesc;
sizetype      KeySize      = 0;
typetype      KeyType      = aInteger;
numtype       TreeOrder    = 10;

  if (ClassBug)
  if (sprintf(Msg, "  PersonLibCreate:: Db= '%32s'; Handles Per Region=%4u\n", 
              DbName,  RgnHndls))
    TraceMsg (0, Msg);

  if (Primal_CreateDb ("PersonSchema",  DbName,     DbFileName,  DbAccess,     
                       HndlRgns,        RgnHndls,   VhndlRgns,   RgnVhndls,
                       XmemRgns,        RgnXmems,   McStat,      PrimalTkn))
  if (ClassBug)
      Object_Dump     (PrimalTkn);


  if (Normal(*Status))
#ifndef   __FREEZE_VCHUNKS__
  if (PrimalCommitDb    (PrimalTkn,   "\0",     False,      McStat))
#endif /* __FREEZE_VCHUNKS__  */

  if (Object_GetImage (PrimalTkn,     McStat,  (addrtype *)PrimalObj))

  if (Env_GetAttrInfo (LastName_Attr,            McStat,     &AttrDesc))
  {
    (*PrimalObj)->Descriptor  = NullPtr;
    (*PrimalObj)->MaxId       = 0;

     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
     Tree_Create     (PersonNames_Set,        PrimalTkn,   TreeOrder, 
                      RgnHndls,               KeySize,     KeyType,
                      McStat,                &TreeTkn);
  }

  if (Normal(*Status))
  if (Env_GetAttrInfo (PersonId_Attr,         McStat,     &AttrDesc))
  {
     KeySize    = AttrDesc.Size;
     KeyType    = AttrDesc.TypeType;
     Tree_Create     (PersonObjs_Set,         PrimalTkn,   TreeOrder, 
                      RgnHndls,               KeySize,     KeyType,
                      McStat,                &TreeTkn);
  }

  if (Normal(*Status))
  {
#ifndef   __FREEZE_VCHUNKS__
       TraceMsg (0, "\n PrimalTkn BEFORE Commit Object::");
       Object_Dump      (PrimalTkn);

       if (Object_CommitImage
                           (PrimalTkn,   True,                McStat))
       {
          TraceMsg (0, "\n PrimalTkn AFTER  Commit Object ::");
          Object_Dump      (PrimalTkn);
       }

       if (Normal(*Status))
       if (Primal_CommitDb (PrimalTkn,   "\0",     False,     McStat))
       if (ClassBug || DeBug)
       {
          TraceMsg (0, "\n PrimalTkn AFTER Commit DB::");
          Object_Dump      (PrimalTkn);
       }
#else
       if (Object_CommitImage
                           (PrimalTkn,   False,               McStat))
#endif /* __FREEZE_VCHUNKS__  */

       if (Normal(*Status))
       if (Object_GetImage  (PrimalTkn,   McStat,  (addrtype *)PrimalObj))
          PersonLib01       = *PrimalObj;

       Print_Msg         = True;
       WriteHdr          = False;

  } else if (*Status    == Csh_DbAlreadyExists) {

   *Status               = Env_Normal;
    if (Object_GetImage     (PrimalTkn,    McStat,    (addrtype *)PrimalObj))
       PersonLib01          = *PrimalObj;
  }

#ifdef   __EDB_BENCHMARK__
  TraceMsg (0, "\n Primal Token to Person Library::\n");
  Object_Dump (PrimalTkn);
  TraceMsg (0, "\n PersonNames Set Head Token::\n");
  Object_Dump (&TreeTkn);
#endif /* __EDB_BENCHMARK__  */

TRACK(TrackBak,"PersonLib_CreateDb\n");
return (STAT);
}


boolean PersonLib_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    PersonLib     **PrimalObj)
{
   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  PersonLib_ActivateDbByName:: <%21s>\n",
                 DbName))
   if (TraceMsg (0, Msg))
   if (sprintf  (Msg, "                 FileName := <%21s>; Access=%s\n",
                 DbFileName, DbAccessStr[AccessType]))
      TraceMsg  (0, Msg);

  *PrimalObj = NullPtr;

   if (!Primal_ActivateDbByName
                         (DbName,    DbFileName, AccessType,
                          McStat,               &EmpTkn010))
      return (False);

   if (Object_GetImage   (&EmpTkn010,    McStat, (addrtype *)PrimalObj))
      PersonLib01          = *PrimalObj;

   if (Normal(*Status))
   {
      TraceMsg (0, "\n DB Activated::");
      Object_Dump      (&EmpTkn010);
   }

TRACK(TrackBak,"PersonLib_ActivateDbByName\n");
return (STAT);
}


boolean PersonLib_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    PersonLib      **PrimalObj)
{
static char   FileName[128]  = "\0";

   if (DbFileName  != NullPtr)
     strcpy (FileName, DbFileName);

   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  PersonLibActivate:: [%3u:%4u]  DbFile= '%32s'\n",
                 PrimalTkn->DbId, PrimalTkn->Handle,  FileName))
      TraceMsg  (0, Msg);

   if (!Primal_ActivateDbByToken
                           (PrimalTkn,    DbFileName,  AccessType, McStat))
      return (False);

   if (Object_GetImage     (PrimalTkn,    McStat,      (addrtype *)PrimalObj))
      PersonLib01          = *PrimalObj;

TRACK(TrackBak,"PersonLib_ActivateDbByToken\n");
return (STAT);
}


boolean PersonLib_DeleteDb (tokentype    *PrimalTkn, ft F,lt Z,zz *Status)
{
   if (ClassBug || DeBug > 0)
   if (sprintf  (Msg, "  PersonLib_DeleteDb:: [%3u:%4u]\n",
                 PrimalTkn->DbId, PrimalTkn->Handle))
      TraceMsg  (0, Msg);

   Primal_DeleteDb         (PrimalTkn,    McStat);

TRACK(TrackBak,"PersonLib_DeleteDb\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+*/
/* PERSON OBJS                                                             */
/*+-----------------------------------------------------------------------+*/
boolean PersonObjs_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " Persons::InitClass...\n");

  if (!PersonObjs_Set)
  if (EnvGetMemberId    ("OwnerPersonObjs",
                         McStat,               &OwnerPersonObjs_Attr))
  if (EnvGetMemberId    ("MemberPersonObjs",
                         McStat,               &MemberPersonObjs_Attr))
      EnvGetSetId       ("PersonObjs",      
                         McStat,               &PersonObjs_Set);

TRACK(TrackBak,"PersonObjs_InitClass\n");
return (STAT);
}
 
boolean PersonObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
/*                                                       <CFS [7.2.5  ] >  */
   Member_KeySetAddInto (PersonObjs_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"PersonObjs_AddInto\n");
return (STAT);
}
 
boolean PersonObjs_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
/*                                                       <CFS [7.1.5  ] >  */
   Owner_KeySetFindIn  (PersonObjs_Set,      OwnerTkn,   KeyValue,
                        McStat,            MemberTkn); 

TRACK(TrackBak,"FindInPersonObjs\n");
return (STAT);
}
 
boolean PersonObjs_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status)
{

/*                                                       <CFS [7.3.1  ] >  */
  SpclTraverseTree (PersonObjs_Set,  OwnerTkn, IterFunc, McStat);

  if (*Status == Set_EndOfSet  
  ||  *Status == Set_EmptySet)
      *Status  = Iterate_EndOf;

TRACK(TrackBak,"PersonObjs_IterateOn\n");
return (STAT);
}
 
 
/*+-----------------------------------------------------------------------+*/
/* PERSON NAMES                                                            */
/*+-----------------------------------------------------------------------+*/
boolean PersonNames_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " PersonNamas::InitClass...\n");

  if (!PersonNames_Set)
  if (EnvGetMemberId    ("OwnerPersonNames",
                         McStat,               &OwnerPersonNames_Attr))
  if (EnvGetMemberId    ("MemberPersonNames",
                         McStat,               &MemberPersonNames_Attr))
      EnvGetSetId       ("PersonNames",      
                         McStat,               &PersonNames_Set);

TRACK(TrackBak,"PersonNames_InitClass\n");
return (STAT);
}
 

boolean PersonNames_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
/*                                                       <CFS [7.2.5  ] >  */
   Member_KeySetAddInto (PersonNames_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"PersonNames_AddInto\n");
return (STAT);
}
 
boolean PersonNames_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
/*                                                       <CFS [7.1.5  ] >  */
   Owner_KeySetFindIn  (PersonNames_Set,      OwnerTkn,   KeyValue,
                        McStat,            MemberTkn); 

TRACK(TrackBak,"FindInPersonNames\n");
return (STAT);
}
 
boolean PersonNames_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status)
{

/*                                                       <CFS [7.3.1  ] >  */
  SpclTraverseTree (PersonNames_Set,  OwnerTkn, IterFunc, McStat);

  if (*Status == Set_EndOfSet  
  ||  *Status == Set_EmptySet)
      *Status  = Iterate_EndOf;

TRACK(TrackBak,"PersonNames_IterateOn\n");
return (STAT);
}
 
#ifdef INCLUDE_ALL_CODE                      
void PersonLib_Topology       (void)
{
         longaddr    StartAddr = 0;
static   PersonLib     That;
static   PersonLib    *This      = &That;

  TraceMsg (0, "\n\n  <PersonLib2> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof PersonLib          : =%3u\n",   sizeof(PersonLib));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->DbNum               : +%3ld\n",
           (longaddr )&This->DbNum                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->EnvType             : +%3ld\n",
           (longaddr )&This->EnvType             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->DescNum             : +%3ld\n",
           (longaddr )&This->DescNum              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->PrimalSlotAttr      : +%3ld\n",
           (longaddr )&This->PrimalSlotAttr       - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Descriptor          : +%3ld\n",
           (longaddr )&This->Descriptor           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->MaxId               : +%3ld\n",
           (longaddr )&This->MaxId                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->OwnerPersonObjs     : +%3ld\n",
           (longaddr )&This->OwnerPersonObjs      - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->OwnerPersonNames    : +%3ld\n\n",
           (longaddr )&This->OwnerPersonNames     - StartAddr);
  TraceMsg (0, Msg);

return;
}

#endif /* INCLUDE_ALL_CODE   */                   

/*+-----------------------------------------------------------------------+
~M                        END OF EMPLIB.C                                 !
  +-----------------------------------------------------------------------+*/
