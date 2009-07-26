/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* c Shell - Obj01.c */
#define  OBJ01_C

#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "core01.h"
#include "hm.h"
#include "tm.h"
#include "env0.h"
#include "odbm.h"
#include "envdefs.h"
#include "env1.h"
#include "km.h"
#include "vchunk.h"
#include "im.h"
#include "iam.h"
#include "obj01.h"


/*+-----------------------------------------------------------------------+
~CSH                        C   SHELL                                     !
  +-----------------------------------------------------------------------+*/

extern  tokentype    RenvTkn;
extern  tokentype    WenvTkn;
/*
//static  boolean      CppType = True;
*/ 


/*+-----------------------------------------------------------------------+
  !                 ....       OBJECT       ....                          !
  +-----------------------------------------------------------------------+*/

/*     4.1.1                                                               */
boolean Object_Create
                         (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
    if (ImageDbCreate (Anchor,  ClassId,   McStat,   Token)) 
    {
       if (ClassBug || DeBug || OaBug)
       if (sprintf (Msg, "  Object Createed [%3u:%8u]...\n",
                    Token->DbId, Token->Handle))
          TraceMsg (0, Msg);
    } else
       TraceMsg (0, " ***ERROR @ Object_DbCreate\n");


TRACK(TrackBak,"Object_Create\n");
return (STAT);
}

 

/*     4.1.4                                                               */
void    Object_Dump       (tokentype    *Token)
{
   if (ClassBug || DeBug || OaBug)
       TraceMsg (0, "\n Object_DbDump \n");

   ImageDumpObject   (Token);
}


/*     4.1.7                                                               */
numtype Object_IsA       (tokentype    *Token,     ft F,lt Z,zz *Status)
{
numtype    ClassNum     = 0;

  KernelWhatAmI           (Token,     McStat,     &ClassNum);

STAT;
return (ClassNum);
}


/*     4.1.8                                                               */
boolean Object_IsKindOf  (tokentype    *Token,     numtype       ClassId,
                          ft F,lt Z,zz *Status)
{

  KernelIamA              (Token,     ClassId,      McStat);
  if (*Status == Kernl_IamNot)
  {
      *Status  = Env_Normal;
       return (False);
  }

TRACK(TrackBak,"Object_IsKindOf\n");
return(STAT);
}




/*     4.1.17                                                              */
boolean Object_Delete    (tokentype    *Token,     ft F,lt Z,zz *Status) 
{

     ImageDeleteObject (Token, C_Apl,   McStat);

     if (Normal(*Status))
     {
        if (ClassBug || DeBug || OaBug)
        if (sprintf (Msg, "  Object Deleted \n"))
           TraceMsg (0, Msg);
     } else {

        TraceMsg (0, " ***ERROR @ Image01_:DbDelete\n");
        sprintf (Msg, "\n    Object_DbDelete [%3u:%8u]...\n",
                 Token->DbId, Token->Handle);
        TraceMsg (0, Msg);
     }

TRACK(TrackBak,"Object_Delete\n");
return (STAT);
}


/* ........................................................................
                    .... Image Of Interface ....
   ........................................................................*/

/*     4.2.0                                                               */
boolean Object_NewImage  (tokentype    *Anchor,   numtype       ClassId,
                          ft F,lt Z,zz *Status,
                          tokentype    *Token,    addrtype     *Image) 
{

    *Image           = (addrtype )NullPtr;
     ImageNewObject    (Anchor,   ClassId, McStat,   Token,   Image);

TRACK(TrackBak,"Object_NewImage\n");
return (STAT);
}


/*     4.2.1                                                               */
boolean Object_GetImage  (tokentype    *Token,
                          ft F,lt Z,zz *Status,   addrtype      *Image) 
{

    *Image           = (addrtype )NullPtr;
     ImageGetObject    (Token,   McStat,   Image);

TRACK(TrackBak,"Object_GetImage\n");
return (STAT);
}


/*     4.2.2                                                               */
boolean Object_ImageIsActive 
                         (tokentype    *Token,     ft F,lt Z,zz *Status)
{
addrtype      This  = NullPtr;

     if (Env_IsValidToken       (Token,    McStat))
        return (Image_IsActive  (Token,    McStat,    &This));

TRACK(TrackBak,"Object_ImageIsActive\n");
return (STAT);
}


/*     4.2.4                                                               */
boolean Object_PutImage  (tokentype    *Token,     ft F,lt Z,zz *Status) 
{
addrtype Object  = NullPtr;
boolean  Release = True;
boolean  FreeMem = True;

     if (ImageGetObject (Token, McStat, &Object))
         ImageReFaxToDb (C_Apl,  Token,   Object,  Release,  FreeMem, 
                         McStat);

     if (!Normal(*Status))
     if (sprintf (Msg,  " ***ERROR @ Object_DbPut @%6x TO [%3u:%6u]\n",
                  Object, Token->DbId, Token->Handle))
        TraceMsg (0,    Msg);

TRACK(TrackBak,"Object_PutImage\n");
return (STAT);
}
 

/*     4.2.5                                                               */
boolean Object_FreeImage (tokentype    *Token,     ft F,lt Z,zz *Status) 
{

     ImageFreeDbObject (C_Apl, Token, McStat);

TRACK(TrackBak,"Object_FreeImage\n");
return (STAT);
}
 

/*     4.2.6                                                               */
boolean Object_CommitImage
                         (tokentype    *Token,     boolean       Release,
                          ft F,lt Z,zz *Status)
{
addrtype   NullAddr   = NullPtr;

     ImageCommitObject  (C_Apl, Token, &NullAddr,  Release,  McStat);

TRACK(TrackBak,"Object_CommitImage\n");
return (STAT);
}


/*     4.2.7                                                               */
void    Image_DumpPseudo
                         (tokentype    *Anchor,    numtype       ClassId,
                          addrtype      Pseudo)
{
     ImageDumpPseudo    (Anchor->DbId,  ClassId,  Pseudo);

return;
}



/*+-----------------------------------------------------------------------+
  !                 ....        ATTR        ....                          !
  +-----------------------------------------------------------------------+*/

/*     6.0  Attribute                                                      */



/* ........................................................................
                    ....  Generic Value     ....
   ........................................................................*/

/*     6.2.1                                                               */
boolean Attr_ValuePut    (tokentype    *Token,     numtype       AttrId,
                          addrtype      Value,     ft F,lt Z,zz *Status)
{

  ImagePutAttrValue  (Token,  AttrId,  Value,  McStat);

TRACK(TrackBak,"Attr_ValuePut\n");
return (STAT);
}

/*     6.2.2                                                               */
boolean Attr_ValueGet    (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    addrtype      Value)

{
  ImageGetAttrValue      (Token,       AttrId,    McStat,      Value);

TRACK(TrackBak,"Attr_ValueGet\n");
return (STAT);
}


/*     6.2.3                                                               */
int     Attr_ValueCompare
                         (tokentype    *Token,     numtype       AttrId,
                          addrtype      CompValue, ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status)
{
int     i = 0;

  i = ImageCompareAttr    (Token,    AttrId,   CompValue,    CmpCode, 
                           McStat);

TRACK(TrackBak,"Attr_ValueCompare\n");
STAT;
return (i);
}


/* ........................................................................
                    .... Direct Reference  ....
   ........................................................................*/
/*     6.3.1                                                               */
boolean Attr_ObjTknPut   (tokentype    *Token,     numtype       AttrId,
                          tokentype    *ObjTkn,    ft F,lt Z,zz *Status)
{
  ImagePutObjTkn (Token,  AttrId,  ObjTkn,     McStat);

TRACK(TrackBak,"Attr_ObjTknPut\n");
return (STAT);
}


/*     6.3.2                                                               */
boolean Attr_ObjTknGet   (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    tokentype    *ObjTkn)
{
  ImageGetObjTkn (Token,  AttrId,  McStat,  ObjTkn);

TRACK(TrackBak,"Attr_ObjTknGet\n");
return (STAT);
}


/* ........................................................................
                    ....    Reference      ....
   ........................................................................*/

/*     6.3.3                                                               */
boolean Attr_RefTknPut   (tokentype    *Token,     numtype       AttrId,
                          tokentype    *RefTkn,    ft F,lt Z,zz *Status)
{
RefObj     RefObjVal;
   
  RefObjVal.ObjAddr   = NullPtr;
  RefObjVal.ObjTkn    = *RefTkn;

  ImageRefTknPut (Token,  AttrId,  &RefObjVal,  McStat);

TRACK(TrackBak,"Attr_RefTknPut\n");
return (STAT);
}


/*     6.3.4                                                               */
boolean Attr_TokenToRef  (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    addrtype     *RefObject)
{

 *RefObject   = NullPtr;
  ImageTokenToRef  (Token,   AttrId,   McStat,  RefObject);

TRACK(TrackBak,"Attr_TokenToRef\n");
return (STAT);
}


/* ........................................................................
                    ....      String    ....
   ........................................................................*/
/*     6.4.1                                                               */
boolean Attr_StringCreate
                         (tokentype    *Token,     numtype       AttrId,
                          sizetype      AllocSize,
                          ft F,lt Z,zz *Status,    cstring      *String)
{
  ImageNewString      (Token,  AttrId,  AllocSize,  McStat,   String);

TRACK(TrackBak,"Attr_StringCreate\n");
return (STAT);
}


/*     6.4.2                                                               */
boolean Attr_StringGet   (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,   
                          boolean      *StrCpyIsA, cstring      *String)
{
  ImageGetString      (Token,  AttrId,  McStat,   StrCpyIsA,  String);

TRACK(TrackBak,"Attr_StringGet\n");
return (STAT);
}


/*     6.4.3                                                               */
boolean Attr_StrCpyGet   (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    cstring      *String)
{
boolean      StrCpyIsA = False;
sizetype     StrSize   = 0;
cstring      StrValue  = NullPtr;

  if (ImageGetString  (Token,  AttrId,  McStat,  &StrCpyIsA,   &StrValue))
  if (StrCpyIsA     == False)
  {
     if ((StrSize    = strlen (StrValue)) > 0)
     if (Core0_MoreCore   (++StrSize, McStat, (addrtype *)String))
         strncpy (*(char **)String,  StrValue, StrSize);
  } else
   *String   = StrValue;
TRACK(TrackBak,"Attr_StrCpyGet\n");
return (STAT);
}


/*     6.4.4                                                               */
boolean Attr_StrCpyFree  (ft F,lt Z,zz *Status,    cstring      *String)
{
sizetype     StrSize   = 0;

  if ((StrSize       = strlen (*String)) > 0)
     Core0_FreeCoreSpace   ((addrtype *)String, ++StrSize, McStat);

TRACK(TrackBak,"Attr_StrCpyFree\n");
return (STAT);
}


/* ........................................................................
                    ....  Dynamic Array     ....
   ........................................................................*/

/*     6.5.1                                                               */
boolean Attr_ArrayCreate (tokentype    *Token,     numtype       AttrId,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,    addrtype     *Array)
{
tokentype  VchunkTkn   = NullToken;

   *Array   = NullPtr;
    ImageCreateArray  (Token,    AttrId,      UnitCount,     UnitSize,
                       McStat,  &VchunkTkn,   Array);

TRACK(TrackBak,"Attr_ArrayCreate\n");
return (STAT);
}
 

/*     6.5.1.1                                                             */
boolean Attr_SubArrayCreate
                         (tokentype    *Token,     
                          numtype       AttrId,    indextype     ArrayIndex,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,    addrtype     *Array)
{
tokentype  VchunkTkn   = NullToken;

   *Array   = NullPtr;
    ImageCreateSubArray  (Token,         AttrId,     ArrayIndex, 
                          UnitCount,     UnitSize,
                          McStat,       &VchunkTkn,  Array);

TRACK(TrackBak,"Attr_SubArrayCreate\n");
return (STAT);
}


/*     6.5.2                                                               */
boolean Attr0_ArrayActivate
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    numtype      *UnitCount,
                          sizetype     *UnitSize,  addrtype     *Varray)
{ 
tokentype  VchunkTkn   = NullToken;

   if (Object_ImageIsActive             (Token,     McStat))
      *Status    = Attr_ImageIsActive;

   if (Normal(*Status))
   if (ImageArrayActivate (Token,       AttrId,     McStat,
                           &VchunkTkn,  UnitCount,  UnitSize,  Varray)) 
   if (VchunkTkn.Handle  == 0)
   {
      *Status = Attr_NullArray;
       TraceMsg (0, " ***Error... @Attr_ArrayActivate :: Null Vchunk Token\n");

       sprintf (Msg, "     for Token[%2u:%8u] AttrId= %4u\n",
                Token->DbId, Token->Handle, AttrId);
       TraceMsg (0, Msg);
   }

TRACK(TrackBak,"Attr0_ArrayActivate\n");
return (STAT);
}


/*     6.5.3                                                               */
boolean Attr0_ArrayDeActivate
                         (tokentype    *Token,     numtype       AttrId,
                          deletetype    Action,    ft F,lt Z,zz *Status)
{ 

   if (Object_ImageIsActive             (Token,     McStat))
     *Status    = Attr_ArrayIsActive;
   else
      ImageArrayDeActivate (Token,       AttrId,    McStat);

TRACK(TrackBak,"Attr0_ArrayDeActivate\n");
return (STAT);
}


/*     6.5.4                                                               */
boolean Attr_ArrayGetSize
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,   
                          numtype      *UnitCount, sizetype     *UnitSize)
{ 
   ImageArrayGetSize  (Token,       AttrId,     
                       McStat,      UnitCount,  UnitSize);

TRACK(TrackBak,"Attr_ArrayGetSize\n");
return (STAT);
}


/*     6.5.4.1                                                             */
boolean Attr_SubArrayGetSize
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,
                          ft F,lt Z,zz *Status,    numtype      *UnitCount)
{
   ImageSubArrayGetSize  (Token,   AttrId,   Index,  McStat,  UnitCount);

TRACK(TrackBak,"Attr_SubArrayGetSize\n");
return (STAT);
}


/*     6.5.5                                                               */
boolean Attr_ArrayAssertSize
                         (tokentype    *Token,     numtype       AttrId,
                          numtype       UnitCount,
                          ft F,lt Z,zz *Status,    addrtype     *Varray)
{
   ImageArrayAssertSize  (Token,   AttrId,   UnitCount,  McStat,  Varray);

TRACK(TrackBak,"Attr_ArrayAssertSize\n");
return (STAT);
}


/*     6.5.5.1                                                             */
boolean Attr_SubArrayAssertSize
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,     numtype       UnitCount,
                          ft F,lt Z,zz *Status,    addrtype     *Varray)
{
   ImageSubArrayAssertSize  (Token,   AttrId,   Index,  UnitCount, 
                             McStat,  Varray);

TRACK(TrackBak,"Attr_SubArrayAssertSize\n");
return (STAT);
}


/*     6.5.6                                                               */
boolean Attr_ArrayDelete (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status)
{
   ImageArrayDelete      (Token,  AttrId,  McStat);

TRACK(TrackBak,"Attr_ArrayDelete\n");
return (STAT);
}


/*     6.5.6.1                                                             */
boolean Attr_SubArrayDelete
                         (tokentype    *Token,     numtype       AttrId,
                          indextype     Index,     ft F,lt Z,zz *Status)
{
   ImageSubArrayDelete      (Token,  AttrId,  Index,  McStat);

TRACK(TrackBak,"Attr_ArrayDelete\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
  !                 ....       FIELD         ....                         !
  +-----------------------------------------------------------------------+*/

/* ........................................................................
                    ....  Generic Value     ....
   ........................................................................*/

/*     6s.2.1                                                              */
boolean Field_ValuePut   (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status)
{
  ImagePutFieldValue (Token,  FieldStruc,  Value,   McStat);

TRACK(TrackBak,"Field_ValuePut\n");
return (STAT);
}


/*     6s.2.2                                                              */
boolean Field_ValueGet   (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value)
{
  ImageGetFieldValue  (Token,  FieldStruc,   McStat,  Value);

TRACK(TrackBak,"Field_ValueGet\n");
return (STAT);
}




/*+-----------------------------------------------------------------------+
  !                 ....         SET        ....                          !
  +-----------------------------------------------------------------------+*/

/*     7.0  Set                                                            */


/* .......................................................................
                    ....     Set Owner      ....
   .......................................................................*/
/*     7.1    Owner                                                        */



/*     7.1.2                                                               */
inttype Owner_SetIterateOn
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          inttype       ItNum,     ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token)
{
tokentype    CurrTkn = *NodeTkn;

  if (ItNum == 0)
     KernelFirstOf   ((handletype )SetId,  OwnerTkn, McStat, NodeTkn, Token);
  else
     KernelNextOf    ((handletype )SetId, &CurrTkn,  McStat, NodeTkn, Token);

  if (Normal(*Status))
      ItNum++;

  if (*Status == Set_EndOfSet
  ||  *Status == Set_EmptySet)
      *Status  = Iterate_EndOf;

STAT;
return (ItNum);
}


/*     7.1.3                                                               */
boolean Owner_SetFirstOf
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          ft F,lt Z,zz *Status,    tokentype    *NodeTkn,
                          tokentype    *FirstTkn)
{
     KernelFirstOf   (SetId,  OwnerTkn, McStat, NodeTkn, FirstTkn);

TRACK(TrackBak,"Owner_SetFirstOf\n");
return (STAT);
}


/*     7.1.5                                                               */
boolean Owner_KeySetFindIn
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          addrtype      KeyValue,
                          ft F,lt Z,zz *Status,    tokentype    *MemberTkn)
{
     KernelFindIn   (SetId,  OwnerTkn, KeyValue, McStat, MemberTkn);

TRACK(TrackBak,"Owner_KeySetFindIn\n");
return (STAT);
}

/*     7.1.6                                                               */
boolean Owner_KeySetFindInBy2aryKey
                         (numtype       SetId,     tokentype    *OwnerTkn,
                          addrtype      KeyValue,
                          ft F,lt Z,zz *Status,    tokentype    *MemberTkn)
{

  *Status = Err_NotImplemented;

TRACK(TrackBak,"Owner_KeySetFindInBy2aryKey\n");
return (STAT);
}


/* .......................................................................
                    ....     Set Member     ....
   .......................................................................*/
/*     7.2  Member                                                         */

/*     7.2.1                                                               */
boolean Member_SetOwnerOf
                         (numtype       SetId,     tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status,    tokentype    *OwnerTkn)
{
   KernelOwnerOf   (SetId,  MemberTkn, McStat, OwnerTkn);

TRACK(TrackBak,"Member_SetOwnerOf\n");
return (STAT);
}



/*     7.2.3                                                               */
boolean Member_SetNextOf
                         (numtype       SetId,     tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status, 
                          tokentype    *NodeTkn,   tokentype    *NextTkn)
{
tokentype   CurrNodeTkn  = NullToken;

   KernelNextOf (SetId, &CurrNodeTkn,  McStat, NodeTkn, NextTkn);

TRACK(TrackBak,"Member_SetNextOf\n");
return (STAT);
}


/*     7.2.5                                                               */
boolean   Member_KeySetAddInto  
                         (numtype       SetId,     tokentype     *Owner,
                          tokentype    *Member,    ft F,lt Z,zz *Status)
{

   KernelAddInto      (SetId, Owner, Member, McStat);

TRACK(TrackBak,"Member_KeySetAddInto\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF OBJ01.C                                 !
  +-----------------------------------------------------------------------+*/
