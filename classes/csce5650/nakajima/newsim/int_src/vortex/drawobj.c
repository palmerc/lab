/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C APPS - DrawObj.c */
/* ******************************  DrawObj.c  ***************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       2.2   DrawObj - Procedures of DrawOb and NamedDrawObj.          */
/* *********************************************************************** */

#define DRAWOBJ
#include <stdarg.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "drawlib.h"
#include "drawobj.h"
#include "vchunk.h"
#include "tree01.h"

/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                             DRAWOBJ                                   !
  !                             METHODS                                   !
  +-----------------------------------------------------------------------+*/



/*+-----------------------------------------------------------------------+*/
/* DRAWOBJ                                                                 */
/*+-----------------------------------------------------------------------+*/
boolean DrawObj_InitClass  (ft F,lt Z,zz *Status)
{
  if (DrawObj_Class  == 0)
  {
     if (Env_GetAttrId      ("PartObjSlotAttr", McStat, &PartObj_Slot))
     if (Env_GetAttrId      ("DrawObjSlotAttr", McStat, &DrawObj_Slot))
     if (EnvGetMemberId     ("Id",              McStat, &Id_Attr))
     if (EnvGetMemberId     ("MemberDrawObjs",  McStat, &MemberDrawObjs_Attr))

         EnvGetClassId      ("DrawObj",      McStat, &DrawObj_Class);
  }

  if (Normal(*Status))
  if (DrawObjs_InitClass (McStat))

  if (EnvInitClassMap    (DrawObj_Class,       sizeof(DrawObj),   
                          C_Apl,               McStat))
      EnvInvokeMap       (C_Apl,               McStat);


TRACK(TrackBak,"DrawObj_InitClass\n");
return (STAT);
}

DrawObj  *DrawObj_new0   (tokentype    *AnchorTkn, ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
DrawObj        *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  DrawObj_New0 Anchor= [%4u:%6u]\n",
                AnchorTkn->DbId, AnchorTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    DrawObj_Class,    McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id       = ++DrawLib01->DrawId;
      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = DrawObj_delete;
   }
   if (Normal(*Status))
      DrawObjs_AddInto (&LibTkn010,  &Token,  McStat);

STAT;
return (Image);
}
 

DrawObj  *DrawObj_new1   (tokentype    *AnchorTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
DrawObj        *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  DrawObj_New1 Anchor= [%4u:%6u]; ClassId=%4u\n",
                AnchorTkn->DbId, AnchorTkn->Handle, ClassId))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    ClassId,          McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id       = ++DrawLib01->DrawId;
      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = DrawObj_delete;
   }

   if (Normal(*Status))
      DrawObjs_AddInto (&LibTkn010,  &Token,  McStat);

STAT;
return (Image);
}

DrawObj  *DrawObj_new2   (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
DrawObj      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  DrawObj_new2 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = DrawObj_delete;
   }

STAT;
return (Image);
}
 
boolean     DrawObj_import
                         (DrawObj      *This,      ft F,lt Z,zz *Status)   
{
   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj: import; Id =%3u\n", This->Id))
      TraceMsg (0, Msg);

return (STAT);
}
 
boolean     DrawObj_export
                         (DrawObj      *This, ft F,lt Z,zz *Status)   
{
   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj: export; Id =%3u\n", This->Id))
      TraceMsg (0, Msg);

return (STAT);
}
 

boolean DrawObj_draw     (DrawObj      *This,      ft F,lt Z,zz *Status)    
{
   if (This->draw)
      return  (This->draw       (This,     McStat));
   else {
      STAT;
      return  (False);
   }
}      

lt64    DrawObj_area     (DrawObj      *This,      ft F,lt Z,zz *Status)    
{
   if (This->draw)
      return  (This->area       (This,     McStat));
   else {
      STAT;
      return  (False);
   }
}      
 

boolean DrawObj_delete   (DrawObj     **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
DrawObj   *That  = *This;

   if (That->destruct != DrawObj_delete)
      return  (That->destruct     (This,     DbEvent,   McStat));


   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  DrawObj_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " DrawObj [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"DrawObj_delete\n");
return (STAT);
}
 

boolean DrawObj_Create0  (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
numtype    DrawId   =  0;

   if (DrawLib01)
      DrawId   =  ++DrawLib01->DrawId;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj:: Create [%3u:%8u]\n",
                Anchor->DbId,  Anchor->Handle))
       TraceMsg (0, Msg);

   if (Object_Create   (Anchor, DrawObj_Class,          McStat, Token))
       Attr_ValuePut   (Token,  Id_Attr,    (addrtype )&DrawId,     McStat);

TRACK(TrackBak,"DrawObj_Create0\n");
return (STAT);
}

boolean DrawObj_Create1  (tokentype    *Anchor,    numtype       ClassId,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
numtype    DrawId   =  ++DrawLib01->DrawId;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj:: Create [%3u:%8u];  ClassId=%4u\n",
                Anchor->DbId,  Anchor->Handle, ClassId))
       TraceMsg (0, Msg);

   if (Object_Create   (Anchor, ClassId,                McStat, Token))
   if (Attr_ValuePut   (Token,  Id_Attr,    (addrtype )&DrawId, McStat))
      SetAddInto       (DrawObjs_Set,      &LibTkn010,  Token,  McStat);

TRACK(TrackBak,"DrawObj_Create1\n");
return (STAT);
}

boolean DrawObj_Import   (tokentype    *DrawTkn,    ft F,lt Z,zz *Status)
{
   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj:: Import [%3u:%8u]\n",
                DrawTkn->DbId,  DrawTkn->Handle))
       TraceMsg (0, Msg);

TRACK(TrackBak,"DrawObj_Import\n");
return (STAT);
}
 
boolean DrawObj_Export   (tokentype    *DrawTkn,    ft F,lt Z,zz *Status)
{
   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n DrawObj:: Export [%3u:%8u]\n",
                DrawTkn->DbId,  DrawTkn->Handle))
       TraceMsg (0, Msg);

TRACK(TrackBak,"DrawObj_Export\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/* DRAWOBJS                                                                */
/*+-----------------------------------------------------------------------+*/
boolean DrawObjs_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " DrawObjs::InitClass...\n");

  if (!DrawObjs_Set)
/*                                                       <CFS [2.2.5  ] >  */
  if (EnvGetMemberId    ("OwnerDrawObjs",
                         McStat,               &OwnerDrawObjs_Attr))
  if (EnvGetMemberId    ("MemberDrawObjs",
                         McStat,               &MemberDrawObjs_Attr))
/*                                                       <CFS [2.1.5  ] >  */
      EnvGetSetId       ("DrawObjs",      
                         McStat,               &DrawObjs_Set);

TRACK(TrackBak,"DrawObjs_InitClass\n");
return (STAT);
}
 
 
boolean OwnerOfDrawObjs (tokentype *Member,
                              ft F,lt Z,zz *Status,    tokentype *Owner)
{
/*                                                       <CFS [7.2.1  ] >  */
   Member_SetOwnerOf (DrawObjs_Set, Member, McStat, Owner);

TRACK(TrackBak,"OwnerOfDrawObjs\n");
return (STAT);
}
 
 
boolean DrawObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
/*                                                       <CFS [7.2.5  ] >  */
   SetAddInto (DrawObjs_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"DrawObjs_AddInto\n");
return (STAT);
}
 
boolean DrawObjs_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
/*                                                       <CFS [7.1.5  ] >  */
   SetFindIn  (DrawObjs_Set,      OwnerTkn,   KeyValue,
               McStat,            MemberTkn); 

TRACK(TrackBak,"FindInDrawObjs\n");
return (STAT);
}
 
inttype DrawObjs_IterateOn
                         (tokentype    *OwnerTkn,  inttype       ItNum,
                          ft F,lt Z,zz *Status,
                          tokentype    *NodeTkn,   tokentype    *Token)
{
tokentype    CurrTkn = *NodeTkn;

  if (ItNum == 0)
/*                                                       <CFS [7.1.3  ] >  */
     SetFirstOf (DrawObjs_Set,  OwnerTkn, McStat, NodeTkn, Token);
  else
/*                                                       <CFS [7.2.3  ] >  */
     SetNextOf  (DrawObjs_Set, &CurrTkn,  McStat, NodeTkn, Token);

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


/*+-----------------------------------------------------------------------+*/
/* NAMEDDRAWOBJ                                                           */
/*+-----------------------------------------------------------------------+*/
boolean NamedDrawObj_InitClass
                         (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
NamedDrawObj    Named;

/* Initialize the Class maps of those Class types accessed in this program */
  if (NamedDrawObj_Class == 0)
  {
     if (EnvGetMemberId     ("Nam",          McStat, &Nam_Attr))
     if (EnvGetMemberId     ("MemberNamedDrawObjs", 
                                            McStat, &MemberNamedDrawObjs_Attr))

         EnvGetClassId      ("NamedDrawObj", McStat, &NamedDrawObj_Class);
  }

  StartAddr   = (longaddr)&Named;
  if (Normal(*Status))
  if (NamedDrawObjs_InitClass                 (McStat))
  if (EnvInitClassMap    (NamedDrawObj_Class,  sizeof(NamedDrawObj),   
                          C_Apl,               McStat))
  {
     Offset    = (size_t)((longaddr )&Named.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.Token   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.Id      -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Id_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.MemberDrawObjs  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.PartObjSlotAttr  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (PartObj_Slot,          Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.Nam  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Nam_Attr,              Offset,     McStat))
        Offset = (size_t)((longaddr )&Named.MemberNamedDrawObjs  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberNamedDrawObjs_Attr, Offset,  McStat))
         Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"NamedDrawObj_InitClass\n");
return (STAT);
}


NamedDrawObj  *NamedDrawObj_new0  
                         (tokentype    *AnchorTkn, ft F,lt Z,zz *Status)
{
tokentype          Token     = NullToken;
NamedDrawObj      *Image     = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  NamedDrawObj_New0 Anchor= [%4u:%6u]\n",
                AnchorTkn->DbId, AnchorTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,  NamedDrawObj_Class,    McStat,   &Token))

   if (Object_GetImage (&Token,     McStat,   (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id               = ++DrawLib01->DrawId;

      Ut_IntToStr ((int )Image->Id,   Image->Nam);

      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = NamedDrawObj_delete;

   }
   if (Normal(*Status))
   if (DrawObjs_AddInto      (&LibTkn010,  &Token,  McStat))
       NamedDrawObjs_AddInto (&LibTkn010,  &Token,  McStat);

STAT;
return (Image);
}
 

NamedDrawObj  *NamedDrawObj_new1
                         (tokentype    *AnchorTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status)
{
tokentype          Token     = NullToken;
NamedDrawObj      *DrawImage = NullPtr;
DrawObj           *Image     = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  NamedDrawObj_New1 Anchor= [%4u:%6u]; ClassId=%4u\n",
                AnchorTkn->DbId, AnchorTkn->Handle, ClassId))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    ClassId,          McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&DrawImage))
   {
      Image                      = (DrawObj *)DrawImage;

      if (DrawLib01)
         Image->Id               = ++DrawLib01->DrawId;

      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = DrawObj_delete;
   }

STAT;
return (DrawImage);
}

NamedDrawObj  *NamedDrawObj_new2 
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
tokentype          Token     = *RecTkn;
DrawObj           *Image     = NullPtr;
NamedDrawObj      *DrawImage = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  NamedDrawObj_new2 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (&Token,       McStat,    (addrtype *)&DrawImage))
   {
      Image                      = (DrawObj *)DrawImage;
      Image->draw                = NULL;
      Image->area                = NULL;
      Image->destruct            = DrawObj_delete;
   }

STAT;
return (DrawImage);
}
 

boolean NamedDrawObj_Create0
                         (tokentype    *Anchor,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
numtype    DrawId   =  0;
static
nametype   Nam      = {"Create0_"};

   if (DrawLib01)
      DrawId   =  ++DrawLib01->DrawId;

   Ut_IntToStr   ((int )DrawId, Nam);

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n NamedDrawObj:: Create [%3u:%8u] = <%s>\n",
                Anchor->DbId,  Anchor->Handle, Nam))
       TraceMsg (0, Msg);

   if (Object_Create   (Anchor, NamedDrawObj_Class,     McStat,     Token))
   if (Attr_ValuePut   (Token,  Id_Attr,    (addrtype )&DrawId,     McStat))
       Attr_ValuePut   (Token,  Nam_Attr,   (addrtype )&Nam,        McStat);

   if (Normal(*Status))
   if (DrawObjs_AddInto      (&LibTkn010,  Token,  McStat))
       NamedDrawObjs_AddInto (&LibTkn010,  Token,  McStat);

TRACK(TrackBak,"NamedDrawObj_Create0\n");
return (STAT);
}


boolean NamedDrawObj_Create1
                         (tokentype    *Anchor,    numtype       ClassId,
                          nametype      Nam,
                          ft F,lt Z,zz *Status,    tokentype    *Token)
{
numtype        DrawId          = 0;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg,
               "\n NamedDrawObj:: Create Anchor[%3u:%8u];  ClassId=%4u\n",
                Anchor->DbId,  Anchor->Handle, ClassId))
       TraceMsg (0, Msg);

   if (DrawLib01)
      DrawId      =  ++DrawLib01->DrawId;

   if (Normal(*Status))
   if (Object_Create   (Anchor, ClassId,                McStat, Token))
   if (Attr_ValuePut   (Token,  Id_Attr,    (addrtype )&DrawId,     McStat))
   if (Attr_ValuePut   (Token,  Nam_Attr,   (addrtype  )Nam,        McStat))
   {
      if (DrawObjs_AddInto      (&LibTkn010,  Token,  McStat))
          NamedDrawObjs_AddInto (&LibTkn010,  Token,  McStat);
   }

TRACK(TrackBak,"NamedDrawObj_Create1\n");
return (STAT);
}


boolean  NamedDrawObj_ExportFunc  
                         (tokentype    *Token,   indextype       IterNum,
                          ft F,lt Z,zz *Status)
{
numtype             j      = IterNum;
NamedDrawObj       *r010   = NullPtr;

   if (j == 0 || j % QueBug == 0)
   {
      sprintf (Msg, " Object %4d. Iterate [%3u:%8u]\n",
               IterNum, Token->DbId, Token->Handle);
      TraceMsg (0, Msg);
   }

   if (Object_IsA           (Token,  McStat)   == NamedDrawObj_Class)
   {
      r010 = NamedDrawObj_new2 (Token,              McStat);
      if (r010)
      if (DrawObj_export    ((DrawObj  *)r010,    McStat))
          DrawObj_delete    ((DrawObj **)&r010,   dbFree,    McStat);
   }

TRACK(TrackBak,"NamedDrawObj_ExportFunc\n");
return (STAT);
}


boolean NamedDrawObj_delete   
                         (NamedDrawObj **This,    deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
   DrawObj_delete   ((DrawObj **)This,      DbEvent,   McStat);

TRACK(TrackBak,"NamedDrawObj_delete\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+*/
/* NAMEDDRAWOBJS                                                           */
/*+-----------------------------------------------------------------------+*/
boolean NamedDrawObjs_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " NamedDrawObjs::InitClass...\n");

  if (!NamedDrawObjs_Set)
  if (EnvGetMemberId    ("OwnerNamedDrawObjs",
                         McStat,               &OwnerNamedDrawObjs_Attr))
  if (EnvGetMemberId    ("MemberNamedDrawObjs",
                         McStat,               &MemberNamedDrawObjs_Attr))
      EnvGetSetId       ("NamedDrawObjs",      
                         McStat,               &NamedDrawObjs_Set);

TRACK(TrackBak,"NamedDrawObjs_InitClass\n");
return (STAT);
}
 
 
 
boolean NamedDrawObjs_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
   SetAddInto (NamedDrawObjs_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"NamedDrawObjs_AddInto\n");
return (STAT);
}
 
boolean NamedDrawObjs_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
   SetFindIn  (NamedDrawObjs_Set, OwnerTkn,   KeyValue,
               McStat,            MemberTkn); 

TRACK(TrackBak,"FindInNamedDrawObjs\n");
return (STAT);
}
 
boolean NamedDrawObjs_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status)
{

/*                                                       <CFS [7.3.1  ] >  */
  SpclTraverseTree (NamedDrawObjs_Set,  OwnerTkn, IterFunc, McStat);

  if (*Status == Set_EndOfSet  
  ||  *Status == Set_EmptySet)
      *Status  = Env_Normal;

TRACK(TrackBak,"FindInNamedDrawObjs\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                        END OF DRAWOBJ.C                                !
  +-----------------------------------------------------------------------+*/
