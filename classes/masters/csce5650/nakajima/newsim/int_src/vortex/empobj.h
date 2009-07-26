/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef PERSON_01
#define PERSON_01

/* Dev - Person.h */

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/

typedef     enum  {aResidence,      aBusiness,
                   aMailDrop,       aNonAddressType=EXT_ENUM}   addresstype;
#ifndef BMT_01
typedef     longtype   datetype;
#endif /* BMT_01  */

typedef     longtype   timetype;

#ifdef PERSON
          numtype    Person_Class           = 0;
          numtype    PersonId_Attr          = 0;
          numtype    LastName_Attr          = 0;
          numtype    FirstName_Attr         = 0;
          numtype    Gender_Attr            = 0;
          numtype    Residence_Attr         = 0;
          numtype    InputDate_Attr         = 0;
          numtype    InputTime_Attr         = 0;
          numtype    BirthDate_Attr         = 0;
          numtype    MaritalStatus_Attr     = 0;
/*
//        numtype    Descriptor_Attr        = 0;  PersonLib defined.
*/
          numtype    PersonalAssoc_Attr     = 0;
          numtype    Children_Attr          = 0;

          numtype    Address_Class          = 0;
          numtype    OwnerTkn_Attr          = 0;
          numtype    AddressType_Attr       = 0;
          numtype    Street_Attr            = 0;
          numtype    Suite_Attr             = 0;
          numtype    City_Attr              = 0;
          numtype    District_Attr          = 0;
          numtype    State_Attr             = 0;
          numtype    Country_Attr           = 0;
          numtype    ZipCode_Attr           = 0;
          numtype    Phone_Attr             = 0;
          numtype    Fax_Attr               = 0;

          numtype    PersonParts_Set        = 0;
          numtype    OwnerPersonParts_Attr  = 0;
          numtype    MemberPersonParts_Attr = 0;
#else
   extern numtype    Person_Class;
   extern numtype    PersonId_Attr;
   extern numtype    LastName_Attr;
   extern numtype    FirstName_Attr;
   extern numtype    Gender_Attr;
   extern numtype    Residence_Attr;
   extern numtype    InputDate_Attr;
   extern numtype    InputTime_Attr;
   extern numtype    BirthDate_Attr;
   extern numtype    MaritalStatus_Attr;
/*
// extern numtype    Descriptor_Attr;          defined in PersonLib file.
*/
   extern numtype    PersonalAssoc_Attr;
   extern numtype    Children_Attr;

   extern numtype    Address_Class;
   extern numtype    OwnerTkn_Attr;
   extern numtype    AddressType_Attr;
   extern numtype    Street_Attr;
   extern numtype    Suite_Attr;
   extern numtype    City_Attr;
   extern numtype    District_Attr;
   extern numtype    State_Attr;
   extern numtype    Country_Attr;
   extern numtype    ZipCode_Attr;
   extern numtype    Phone_Attr;
   extern numtype    Fax_Attr;

   extern numtype    PersonParts_Set;
   extern numtype    OwnerPersonParts_Attr;
   extern numtype    MemberPersonParts_Attr;

#endif

typedef struct ImportSpecType
{
   numtype               ClassId;
   numtype               AttrId;

} importspec;

typedef char           inputline[MAXLINE+1];

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
/* ADDRESS                                                                 */
/*       ObjNum(259)      ObjSize=  148                                    */
/*       AllocQty = 100000  RgnQty =  100   Objs/Rgn = 1000                */
typedef struct Address
{
   addrtype              That;
   tokentype             Token;

   tokentype             OwnerTkn;

   addresstype           AddressType;

   varyingtype           Street;
   varyingtype           Suite;
   nametype              City;
   nametype              District;
   nametype              State;
   nametype              Country;
   lt64                  ZipCode;

   char                  Phone[12];
   char                  Fax[12];

} Address;


typedef struct RefAddress
{
   Address          *ObjAddr;
   tokentype         ObjTkn;

}RefAddress;


/* PERSON                                                                  */
/*       ObjNum(257)      ObjSize=  176                                    */
/*       AllocQty =    1  RgnQty =    1   Objs/Rgn =   1                   */
typedef struct Person
{
   addrtype              That;
   tokentype             Token;
                      
   numtype               PersonId;

   nametype              LastName;
   nametype              FirstName;

   char                  Gender;

   Reference(Address)    Residence;

	datetype              InputDate;
	timetype              InputTime;

	datetype              BirthDate;
	int                   MaritalStatus;

   char                 *Descriptor;

   nametype              PersonalAssoc;

	handletype	         *Children;       /* array of handles to Person's */

   memberhandle          MemberPersonObjs;
   memberhandle          MemberPersonNames;

   ownertoken            OwnerPersonParts;

} Person;


/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+
~M                           ADDRESS                                      !
  +-----------------------------------------------------------------------+*/
       boolean Address_InitClass 
                         (ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~M                           PERSON                                       !
  +-----------------------------------------------------------------------+*/

       boolean Person_InitClass 
                         (ft F,lt Z,zz *Status);

       Person *Person_new0
                         (tokentype    *Anchor,    ft F,lt Z,zz *Status);

       Person *Person_new1   
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status);


       Person *Person_new2
                         (tokentype    *PersonTkn, ft F,lt Z,zz *Status);


       boolean Person_Create0
                         (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token);

       boolean Person_Create1  
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token);


       boolean Person_Import
                         (tokentype    *LibTkn,     char         *ImportField,
                          importspec   *TabFields,  numtype       TabCount,
                          ft F,lt Z,zz *Status,     tokentype    *PersonTkn);

       boolean Person_Export   
                         (tokentype    *PersonTkn, filename      OutputFile,
                          ft F,lt Z,zz *Status);

       boolean Person_delete
                         (Person      **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

#ifdef INCLUDE_ALL_CODE                      
       void    Person_Topology   (void);

       void    Address_Topology  (void);
#endif /* INCLUDE_ALL_CODE   */                   

/*+-----------------------------------------------------------------------+
~M                          PERSON SETS                                   !
  +-----------------------------------------------------------------------+*/

       boolean PersonParts_InitClass 
                         (ft F,lt Z,zz *Status);



       boolean PersonParts_OwnerOf
                         (tokentype    *Member,
                          ft F,lt Z,zz *Status,    tokentype    *Owner);


       boolean PersonParts_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status);

       boolean PersonParts_FindIn 
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn);
 
       inttype PersonParts_IterateOn
                         (tokentype    *OwnerTkn,  inttype       ItNum,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token);


/*+-----------------------------------------------------------------------+
~P                         END OF PERSON.H                                !
  +-----------------------------------------------------------------------+*/
#endif
