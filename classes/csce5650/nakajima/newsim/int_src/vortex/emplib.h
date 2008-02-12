/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - PersonLib.h */
/* *************************** DRAW01.H  ********************************* */
/*  Header for PersonLib.h ::                                              */
/*                                                                         */
/* *********************************************************************** */

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS                           !
 *+-----------------------------------------------------------------------+*/

#ifndef PERSONLIB_01
#define PERSONLIB_01

#include <stdarg.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "obj01.h"

#ifdef PERSONLIB
          numtype    PersonLib_Class           = 0;
          numtype    Descriptor_Attr           = 0;
          numtype    MaxId_Attr                = 0;

          numtype    PersonObjs_Set            = 0;
          numtype    OwnerPersonObjs_Attr      = 0;
          numtype    MemberPersonObjs_Attr     = 0;

          numtype    PersonNames_Set           = 0;
          numtype    OwnerPersonNames_Attr     = 0;
          numtype    MemberPersonNames_Attr    = 0;

          numtype    CompanyDB_Class           = 0;
          numtype    CompanyName_Attr          = 0;
          numtype    AddressTkn_Attr           = 0;

#else
   extern numtype    PersonLib_Class;
   extern numtype    Descriptor_Attr;
   extern numtype    MaxId_Attr;

   extern numtype    PersonObjs_Set;
   extern numtype    OwnerPersonObjs_Attr;
   extern numtype    MemberPersonObjs_Attr;

   extern numtype    PersonNames_Set;
   extern numtype    OwnerPersonNames_Attr;
   extern numtype    MemberPersonNames_Attr;

   extern numtype    CompanyDB_Class;
   extern numtype    CompanyName_Attr;
   extern numtype    AddressTkn_Attr;

#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
 
/* PERSONLIB                                                               */
typedef struct TypePersonLib
{
   addrtype              That;
   tokentype             Token;
   numtype               DbNum;
   wordtype              EnvType;
   numtype               DescNum;
   slottokentype         PrimalSlotAttr;
   slottoken             PrimalGroupSlotAttr;

   char                 *Descriptor;

   numtype               MaxId;

   ownerhandle           OwnerPersonObjs;
   ownerhandle           OwnerPersonNames;

} PersonLib;
 

/* COMPANY                                                                 */
typedef struct Company
{
   addrtype              That;
   tokentype             Token;
   numtype               DbNum;
   wordtype              EnvType;
   numtype               DescNum;
   slottokentype         PrimalSlotAttr;
   slottoken             PrimalGroupSlotAttr;

   char                 *Descriptor;

   numtype               MaxId;

   ownerhandle           OwnerPersonObjs;
   ownerhandle           OwnerPersonNames;

   char                  CompanyName[128];
   tokentype             AddressTkn;

} Company;
 

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

#ifdef PERSONLIB

          PersonLib *PersonLib01               = NullPtr;
          tokentype  EmpTkn010                 = Null_Token;

          longtype   PersonCount               = 0;
          boolean    PersonBug                 = False;
#else 

extern PersonLib *PersonLib01;
extern tokentype  EmpTkn010;

extern longtype   PersonCount;
extern boolean    PersonBug;

#endif

/* .......................................................................
                      ....     Person LIB      ....
   .......................................................................*/

       boolean PersonLib_InitLibrary
                         (ft F,lt Z,zz *Status);

       boolean PersonLib_InitClass
                         (ft F,lt Z,zz *Status);

#ifdef INCLUDE_ALL_CODE                      
       void    PersonLib_Topology       
                         (void);
#endif /* INCLUDE_ALL_CODE   */                   

       boolean Person_InitDb
                         (char         *DbName,  char         *DbFileName,  
                          numtype       Option,
                          ft F,lt Z,zz *Status,  tokentype    *PrimalTkn);

       boolean PersonLib_QueryDb
                         (char         *DbName,
                          ft F,lt Z,zz *Status,    boolean      *Active);

       boolean PersonLib_CreateDb
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  DbAccess,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,
                          tokentype    *PrimalTkn, PersonLib   **PrimalObj);

       boolean PersonLib_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    PersonLib   **PrimalObj);

       boolean PersonLib_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    PersonLib   **PrimalObj);

       boolean PersonLib_DeleteDb
                         (tokentype    *PrimalTkn, ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~M                          PERSON SETS                                   !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....   PERSONOBJS      ....
   .......................................................................*/

       boolean PersonObjs_InitClass 
                         (ft F,lt Z,zz *Status);

       boolean PersonObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean PersonObjs_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);

       boolean PersonObjs_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status);


/* .......................................................................
                    ....   PERSONNAMES     ....
   .......................................................................*/

       boolean PersonNames_InitClass 
                         (ft F,lt Z,zz *Status);

       boolean PersonNames_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean PersonNames_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);

       boolean PersonNames_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF EMPLIB.H                                !
  +-----------------------------------------------------------------------+*/
#endif
