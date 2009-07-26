/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C APPS - EmpObj.c */
/* ******************************  EmpObj.c  ****************************  */
/* *  Test program for the Employee library                                */
/* *                               Recipies in the Developers CookBook     */
/* *       2.2   EmpObj - Procedures of EmpOb                              */
/* *********************************************************************** */

#define PERSON
#include "emplib.h"
#include <time.h>
#include "empobj.h"
#include "vchunk.h"
#include "tree01.h"

/*+-----------------------------------------------------------------------+
~M                         EMPLOYE METHODS                                !
  +-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*/
/* ADDRESS                                                                 */
/*+-----------------------------------------------------------------------+*/

boolean Address_InitClass (ft F,lt Z,zz *Status)
{
longaddr        StartAddr = 0;
Address         AddObj;

  if (Address_Class  == 0)
  {
     if (Env_GetAttrId      ("OwnerTkn",        McStat, &OwnerTkn_Attr))
     if (Env_GetAttrId      ("AddressType",     McStat, &AddressType_Attr))
     if (Env_GetAttrId      ("Street",          McStat, &Street_Attr))
     if (Env_GetAttrId      ("Suite",           McStat, &Suite_Attr))
     if (Env_GetAttrId      ("City",            McStat, &City_Attr))
     if (Env_GetAttrId      ("District",        McStat, &District_Attr))
     if (Env_GetAttrId      ("State",           McStat, &State_Attr))
     if (Env_GetAttrId      ("Country",         McStat, &Country_Attr))
     if (Env_GetAttrId      ("ZipCode",         McStat, &ZipCode_Attr))
     if (Env_GetAttrId      ("Phone",           McStat, &Phone_Attr))
     if (Env_GetAttrId      ("Fax",             McStat, &Fax_Attr))
         Env_GetClassId     ("Address",         McStat, &Address_Class);
  }

  StartAddr   = (longaddr     )&AddObj;
  if (Normal(*Status))
  if (Env_InitClassMap      (Address_Class,     sizeof (Address),   
                             C_Apl,             McStat))
  {
#ifdef  __OLD_WAY__
     Offset    = (size_t)((longaddr )&AddObj.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Token   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.OwnerTkn  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (OwnerTkn_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.AddressType - StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (AddressType_Attr,      Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Street    -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Street_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Suite     -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Suite_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.City      -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (City_Attr,             Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.District  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (District_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.State     -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (State_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Country   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Country_Attr,          Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.ZipCode  - StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (ZipCode_Attr,          Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Phone            -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Phone_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&AddObj.Fax    -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Fax_Attr,              Offset,     McStat))
#endif /* __OLD_WAY__ */

     if (Normal(*Status))
         Env_InvokeMap      (C_Apl,             McStat);

  }

TRACK(TrackBak,"Address_InitClass\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+*/
/* PERSON                                                                  */
/*+-----------------------------------------------------------------------+*/
boolean Person_InitClass (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
Person          EmpObj;

  if (Person_Class  == 0)
  {
     if (Env_GetAttrId      ("PersonId",        McStat, &PersonId_Attr))
     if (Env_GetAttrId      ("LastName",        McStat, &LastName_Attr))
     if (Env_GetAttrId      ("FirstName",       McStat, &FirstName_Attr))
     if (Env_GetAttrId      ("Gender",          McStat, &Gender_Attr))
     if (Env_GetAttrId      ("Residence",       McStat, &Residence_Attr))
     if (Env_GetAttrId      ("InputDate",       McStat, &InputDate_Attr))
     if (Env_GetAttrId      ("InputTime",       McStat, &InputTime_Attr))
     if (Env_GetAttrId      ("BirthDate",       McStat, &BirthDate_Attr))
     if (Env_GetAttrId      ("MaritalStatus",   McStat, &MaritalStatus_Attr))
     if (Env_GetAttrId      ("PersonalAssoc",   McStat, &PersonalAssoc_Attr))
     if (Env_GetAttrId      ("Children",        McStat, &Children_Attr))
         Env_GetClassId     ("Person",          McStat, &Person_Class);
  }

  if (Normal(*Status))
      PersonParts_InitClass                    (McStat);

  StartAddr   = (longaddr     )&EmpObj;
  if (Normal(*Status))
  if (Env_InitClassMap      (Person_Class,      sizeof (Person),   
                             C_Apl,             McStat))
  {
     Offset    = (size_t)((longaddr )&EmpObj.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.Token   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.PersonId  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (PersonId_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.LastName  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (LastName_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.FirstName -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (FirstName_Attr,        Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.Gender    -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Gender_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.Residence -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Residence_Attr,        Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.InputDate -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (InputDate_Attr,        Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.InputTime -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (InputTime_Attr,        Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.BirthDate -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (BirthDate_Attr,        Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.MaritalStatus  - StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MaritalStatus_Attr,    Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.Descriptor       -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Descriptor_Attr,       Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.PersonalAssoc   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (PersonalAssoc_Attr,    Offset,     McStat))
        Offset = (size_t)((longaddr )&EmpObj.Children -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Children_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr  )&EmpObj.MemberPersonObjs -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberPersonObjs_Attr, Offset,  McStat))
        Offset = (size_t)((longaddr  )&EmpObj.MemberPersonNames -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberPersonNames_Attr,Offset,  McStat))
        Offset = (size_t)((longaddr  )&EmpObj.OwnerPersonParts -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (OwnerPersonParts_Attr, Offset,  McStat))
         Env_InvokeMap      (C_Apl,                          McStat);
  }

TRACK(TrackBak,"Person_InitClass\n");
return (STAT);
}

Person  *Person_new0     (tokentype    *Anchor,    ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Person         *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Person_New0 Anchor= [%4u:%6u]\n",
                Anchor->DbId, Anchor->Handle))
      TraceMsg (0, Msg);

   if (Object_Create    (Anchor,       Person_Class,    McStat,   &Token))

   if (Object_GetImage  (&Token,       McStat,        (addrtype *)&Image))
   {
      Image->InputTime         = clock();
      Image->PersonId          = ++PersonLib01->MaxId;
   }
   if (Normal(*Status))
       Member_KeySetAddInto  (PersonObjs_Set,    &EmpTkn010,   &Token,  
                              McStat);

STAT;
return (Image);
}
 

Person  *Person_new1     (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Person         *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Person_New1 Anchor= [%4u:%6u]; ClassId=%4u\n",
                Anchor->DbId, Anchor->Handle, ClassId))
      TraceMsg (0, Msg);

   if (Object_Create   (Anchor,       ClassId,          McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   {
      Image->InputTime         = clock();
      Image->PersonId          = ++PersonLib01->MaxId;
   }
   if (Normal(*Status))
       Member_KeySetAddInto  (PersonObjs_Set,    &EmpTkn010,   &Token,  
                              McStat);

STAT;
return (Image);
}


Person  *Person_new2     (tokentype    *EmpTkn,    ft F,lt Z,zz *Status)
{
Person      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Person_new2 Anchor= [%4u:%6u]\n",
                EmpTkn->DbId, EmpTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (EmpTkn,       McStat,         (addrtype *)&Image))
      ;

STAT;
return (Image);
}
 

boolean Person_Create0  (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
time_t    Time     = clock();
numtype    PersonId = 0;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Person:: Create [%3u:%8u]\n",
                Anchor->DbId,  Anchor->Handle))
       TraceMsg (0, Msg);

   PersonId          = ++PersonLib01->MaxId;
   if (Object_Create   (Anchor, Person_Class,          McStat, Token))
   if (Attr_ValuePut   (Token,  InputTime_Attr,   
                        (addrtype )&Time,     McStat))
       Attr_ValuePut   (Token,  PersonId_Attr, 
                        (addrtype )&PersonId,          McStat);

   if (Normal(*Status))
       Member_KeySetAddInto  (PersonObjs_Set,    &EmpTkn010,   Token,  
                              McStat);

TRACK(TrackBak,"Person_Create0\n");
return (STAT);
}


boolean Person_Create1   (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
time_t    Time     = clock();
numtype    PersonId = 0;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Person:: Create [%3u:%8u];  ClassId=%4u\n",
                Anchor->DbId,  Anchor->Handle, ClassId))
       TraceMsg (0, Msg);

   PersonId          = ++PersonLib01->MaxId;
   if (Object_Create   (Anchor, ClassId,               McStat, Token))
   if (Attr_ValuePut   (Token,  InputTime_Attr, 
                        (addrtype )&Time,              McStat))
       Attr_ValuePut   (Token,  PersonId_Attr, 
                        (addrtype )&PersonId,          McStat);
   if (Normal(*Status))
       Member_KeySetAddInto  (PersonObjs_Set,    &EmpTkn010,   Token,  
                              McStat);

TRACK(TrackBak,"Person_Create1\n");
return (STAT);
}


boolean Person_Import    (tokentype    *LibTkn,     char         *ImportField,
                          importspec   *TabFields,  numtype       TabCount,
                          ft F,lt Z,zz *Status,     tokentype    *PersonTkn)
{
Person        *Person       = NullPtr;
tokentype      AddressTkn   = NullToken;
Address       *Address      = NullPtr;
static char    TabStr[128]  = "\0";
numtype        i            = 0;
numtype        j            = 0;
char          *FieldPtr     = ImportField;
boolean        EndTabStr    = False;
numtype        SpaceOut     = 0;
boolean        TempSetBug   = SetBug;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Person:: Import [%3u:%8u] :\n %s\n",
               LibTkn->DbId,  LibTkn->Handle, ImportField))
       TraceMsg (0, Msg);

   if (Object_NewImage  (LibTkn,      Address_Class, 
                         McStat,     &AddressTkn,     (addrtype *)&Address))
   {
      Address->OwnerTkn         = NullToken;

      Address->AddressType      = aResidence;

      Address->Street           = NullPtr;
      Address->Suite            = NullPtr;
      Address->City[0]          = '\0';
      Address->District[0]      = '\0';
      Address->State[0]         = '\0';
      Address->Country[0]       = '\0';
      Address->ZipCode          = '\0';
      Address->Phone[0]         = '\0';
      Address->Fax[0]           = '\0';
   }

   if (Normal(*Status))
   if (Object_NewImage  (LibTkn,      Person_Class, 
                         McStat,      PersonTkn,      (addrtype *)&Person))
   {
      Person->PersonId          = ++PersonLib01->MaxId;

      Person->LastName[0]       = '\0';
      Person->FirstName[0]      = '\0';

      Person->Gender            = 'U';

      Person->Residence.ObjTkn  = AddressTkn;
      Person->Residence.ObjAddr = Address;

      Person->InputDate         = clock();
      Person->InputTime         = clock();

      Person->BirthDate         = 0;
      Person->MaritalStatus     = 0;

      Person->Descriptor        = NullPtr;

      Person->PersonalAssoc[0]  = '\0';

      Person->Children          = NullPtr;

      Address->OwnerTkn         = *PersonTkn;
   }

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg,  " LINE = <%s>\n", FieldPtr))
       TraceMsg (0, Msg);

   if (Normal(*Status))
   for (i = 0; i < TabCount && Normal(*Status); ++i)
   {
      j           = 0;
      EndTabStr   = False;
      SpaceOut    = 0;
      while (EndTabStr   == False)
      {
         if (*FieldPtr   != 011 
         &&  *FieldPtr   != '\0'
         &&  *FieldPtr   != '\n')
         {
           if (*FieldPtr   == ' ')
           {
             if (++SpaceOut == 1)
               TabStr[j++]   =  *FieldPtr;

           } else {
              SpaceOut       = 0;
              TabStr[j++]    =  *FieldPtr;
           }
         } else {
            TabStr[j]        =  '\0';
            EndTabStr        = True;
         }
         FieldPtr++; 
      }

      if (Normal(*Status))
      if (ClassBug)
      {
         sprintf (Msg,
                  " Field= %4u. ClassId=%3u AttrId=%4u. Str= <%s>\n",
                  i, TabFields[i].ClassId, TabFields[i].AttrId, TabStr);
         TraceMsg (0, Msg);
      }

      if ( (size_t) (strlen (TabStr)) > 0 )
      if (TabFields[i].ClassId      == Address_Class)
      {
         if (TabFields[i].AttrId    == Street_Attr)
         {
#ifdef __BAD_CODE__
               Attr_ValuePut     (&AddressTkn,              Street_Attr,    
                                  (addrtype )TabStr,        McStat);
#else
           ;
#endif

         } else if (TabFields[i].AttrId    == Suite_Attr)    {
#ifdef __BAD_CODE__
               Attr_ValuePut     (&AddressTkn,              Suite_Attr,    
                                  (addrtype )TabStr,        McStat);
#else
           ;
#endif
         } else if (TabFields[i].AttrId    == City_Attr)     {
               strncpy (Address->City,     TabStr, MAXNAME);

         } else if (TabFields[i].AttrId    == State_Attr)    {
               strncpy (Address->State,    TabStr, MAXNAME);

         } else if (TabFields[i].AttrId    == Country_Attr)  {
               strncpy (Address->Country,  TabStr, MAXNAME);

         } else if (TabFields[i].AttrId    == ZipCode_Attr)  {
               Address->ZipCode  = atol ((char *)TabStr);

         } else if (TabFields[i].AttrId    == Phone_Attr)    {
               strncpy (Address->Phone,    TabStr, 12);

         } else if (TabFields[i].AttrId    == Fax_Attr)      {
               strncpy (Address->Fax,      TabStr, 12);
         } 

      } else if (TabFields[i].ClassId      == Person_Class)   {

         if (TabFields[i].AttrId           == LastName_Attr)
         {
               strncpy (Person->LastName,  TabStr, MAXNAME);

         } else if (TabFields[i].AttrId    == FirstName_Attr) {
               strncpy (Person->FirstName, TabStr, MAXNAME);
         }

      } else if (TabFields[i].ClassId      != 0) {
         sprintf (Msg,
    " ...Warning... Invalid Tab Field; Field= %4u. ClassId=%3u AttrId=%4u.\n",
                  i, TabFields[i].ClassId, TabFields[i].AttrId);
         TraceMsg (0, Msg);
      }
   }

/* if (!strcmp (Person->LastName,  "Rosecrans")
   ||  !strcmp (Person->LastName,  "Rose")
   ||  !strcmp (Person->LastName,  "Rosen")
   ||  !strcmp (Person->LastName,  "Rosek"))
   {
      sprintf (Msg,
            "\n ADDINTO PersonNames [%3u:%8u]->Name:=%16s, %16s;\n",
               PersonTkn->DbId,    PersonTkn->Handle,
               Person->LastName,   Person->FirstName); 
      TraceMsg (0, Msg);
      SetBug = True;
   }
*/
   if (Normal(*Status))
   if ( (size_t) (strlen (Person->LastName) ) > 0)
       Member_KeySetAddInto  (PersonNames_Set,   LibTkn,  PersonTkn,  
                              McStat);
   SetBug  = TempSetBug;

   if (Normal(*Status))
       Member_KeySetAddInto  (PersonObjs_Set,    LibTkn,  PersonTkn,  
                              McStat);

   if (*Status == Set_AlreadyInSet)
   {
        sprintf (Msg, " *** Duplicate Person[%3u:%8u]. Name = %16s, %16s;\n",
                 PersonTkn->DbId,    PersonTkn->Handle,
                 Person->LastName,   Person->FirstName); 
        TraceMsg (0, Msg);
       *Status  = Env_Normal;

      --PersonLib01->MaxId;

        if (Object_Delete      (PersonTkn,    McStat))
        if (Object_Delete      (&AddressTkn,  McStat))
        {
          *PersonTkn  = NullToken;
          *Status     = Set_AlreadyInSet;
        }
   }

TRACK(TrackBak,"Person_Import\n");
return (STAT);
}
 

boolean Person_Export    (tokentype    *EmpTkn,     filename      OutputFile,
                          ft F,lt Z,zz *Status)
{
   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Person:: Export [%3u:%8u]= %s\n",
                EmpTkn->DbId,  EmpTkn->Handle, OutputFile))
       TraceMsg (0, Msg);

TRACK(TrackBak,"Person_Export\n");
return (STAT);
}
 

boolean Person_delete    (Person     **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
Person   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  Person_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " Person [%2u:%8u] %s in DB\n",
            (*This)->Token.DbId, (*This)->Token.Handle, DeleteStr[DbEvent]))
      TraceMsg (0, Msg);

   switch (DbEvent)
   {
      case dbFree:
           Object_FreeImage   (&That->Token,  McStat);
           break;

      case dbCommit:
           Object_CommitImage (&That->Token,  True,    McStat);
           break;

      case dbDelete:
           Object_Delete      (&That->Token,  McStat);
   }

  *This   = NullPtr;

TRACK(TrackBak,"Person_delete\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/* PERSON PARTS                                                            */
/*+-----------------------------------------------------------------------+*/
boolean PersonParts_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " PersonParts::InitClass...\n");

  if (!PersonParts_Set)
/*                                                       <CFS [2.2.5  ] >  */
  if (EnvGetMemberId    ("OwnerPersonParts",
                         McStat,               &OwnerPersonParts_Attr))
  if (EnvGetMemberId    ("MemberPersonParts",
                         McStat,               &MemberPersonParts_Attr))
/*                                                       <CFS [2.1.5  ] >  */
      EnvGetSetId       ("PersonParts",      
                         McStat,               &PersonParts_Set);

TRACK(TrackBak,"PersonParts_InitClass\n");
return (STAT);
}
 
 
boolean PersonParts_OwnerOf
                         (tokentype *Member,
                          ft F,lt Z,zz *Status,    tokentype *Owner)
{
/*                                                       <CFS [7.2.1  ] >  */
   Member_SetOwnerOf (PersonParts_Set, Member, McStat, Owner);

TRACK(TrackBak,"OwnerOfPersonParts\n");
return (STAT);
}
 
 
boolean PersonParts_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
/*                                                       <CFS [7.2.5  ] >  */
   Member_KeySetAddInto (PersonParts_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"PersonParts_AddInto\n");
return (STAT);
}
 
boolean PersonParts_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
/*                                                       <CFS [7.1.5  ] >  */
   Owner_KeySetFindIn  (PersonParts_Set,      OwnerTkn,   KeyValue,
                        McStat,            MemberTkn); 

TRACK(TrackBak,"FindInPersonParts\n");
return (STAT);
}
 
inttype PersonParts_IterateOn
                         (tokentype    *OwnerTkn,  inttype       ItNum,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token)
{
tokentype    CurrTkn = *NodeTkn;

  if (ItNum == 0)
/*                                                       <CFS [7.1.3  ] >  */
     Owner_SetFirstOf (PersonParts_Set,  OwnerTkn, McStat, NodeTkn, Token);
  else
/*                                                       <CFS [7.2.3  ] >  */
     Member_SetNextOf (PersonParts_Set, &CurrTkn,  McStat, NodeTkn, Token);

  if (Normal(*Status))
      ItNum++;

  else if (*Status == Set_EndOfSet)  
  {
     *Status = Env_Normal;
      ItNum  *= -1;
  } else if (*Status == Set_EmptySet)  {
     *Status = Env_Normal;
      ItNum   = -1;
  }

STAT;
return (ItNum);
}

#ifdef INCLUDE_ALL_CODE                      
void Person_Topology  (void)
{
         longaddr      StartAddr = 0;
static   Person        That;
static   Person       *This      = &That;

  TraceMsg (0, "\n\n  <Person> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof Person             : =%3u\n",   sizeof(Person));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->PersonId            : +%3ld\n",
           (longaddr )&This->PersonId             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->LastName            : +%3ld\n",
           (longaddr )&This->LastName             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->FirstName           : +%3ld\n",
           (longaddr )&This->FirstName            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Gender              : +%3ld\n",
           (longaddr )&This->Gender               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Residence           : +%3ld\n",
           (longaddr )&This->Residence            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->InputDate           : +%3ld\n",
           (longaddr )&This->InputDate            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->InputTime           : +%3ld\n",
           (longaddr )&This->InputTime            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->BirthDate           : +%3ld\n",
           (longaddr )&This->BirthDate            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->MaritalStatus       : +%3ld\n",
           (longaddr )&This->MaritalStatus        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Descriptor          : +%3ld\n",
           (longaddr )&This->Descriptor           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->PersonalAssoc       : +%3ld\n",
           (longaddr )&This->PersonalAssoc        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Children            : +%3ld\n",
           (longaddr )&This->Children             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->MemberPersonObjs    : +%3ld\n",
           (longaddr )&This->MemberPersonObjs     - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->MemberPersonNames   : +%3ld\n",
           (longaddr )&This->MemberPersonNames    - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->OwnerPersonParts    : +%3ld\n\n",
           (longaddr )&This->OwnerPersonParts     - StartAddr);
  TraceMsg (0, Msg);

return;
}

void Address_Topology  (void)
{
         longaddr      StartAddr = 0;
static   Address       That;
static   Address      *This      = &That;

  TraceMsg (0, "\n\n  <Address> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof Address            : =%3u\n",   sizeof(Address));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->OwnerTkn            : +%3ld\n",
           (longaddr )&This->OwnerTkn             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->AddressType         : +%3ld\n",
           (longaddr )&This->AddressType           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Street              : +%3ld\n",
           (longaddr )&This->Street               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Suite               : +%3ld\n",
           (longaddr )&This->Suite                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->City                : +%3ld\n",
           (longaddr )&This->City                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->District            : +%3ld\n",
           (longaddr )&This->District             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->State               : +%3ld\n",
           (longaddr )&This->State                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Country             : +%3ld\n",
           (longaddr )&This->Country              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->ZipCode             : +%3ld\n",
           (longaddr )&This->ZipCode              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Phone               : +%3ld\n",
           (longaddr )&This->Phone                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Fax                 : +%3ld\n\n",
           (longaddr )&This->Fax                   - StartAddr);
  TraceMsg (0, Msg);

return;
}
#endif /* INCLUDE_ALL_CODE   */                   

/*+-----------------------------------------------------------------------+
~M                        END OF EMPOBJ.C                                 !
  +-----------------------------------------------------------------------+*/
