/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* C APPS - draw01.c */
/* *************************  DRAW02.C     *************************
// * The use of subtypes and generic type to implement a real object
// *  Adapted from C++ Report  by Ted Goldstein
// *****************************************************************
*/
#define RECTS_C
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "rects.h"
#include "vchunk.h"

extern  boolean      DrawBug;

/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                              METHODS                                  !
  +-----------------------------------------------------------------------+*/


boolean Rects_InitLibrary
                         (ft F,lt Z,zz *Status)
{
boolean   TempLibBug = LibBug;

  if (ClassBug)
     TraceMsg (0, "\n\n  Initialize Rects Library ...\n\n");

  if (XyRect_InitClass                        (McStat))

  if (NamedXyRect_InitClass                   (McStat))

  if (ArrayRect_InitClass                     (McStat))

  if (DblPtrRect_InitClass                    (McStat))

  if (VarrayRect_InitClass                    (McStat))

  if (IntChunkRect_InitClass                  (McStat))

  if (VchunkRect_InitClass                    (McStat))

  if (RefRect_InitClass                       (McStat))

      PortRect_InitClass                      (McStat);

TRACK(TrackBak,"Rects_InitLibrary\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+*/
/* XYRECT                                                                  */
/*+-----------------------------------------------------------------------+*/
boolean XyRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
XyRect          Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  XY RECT  Map...\n");
  if (EnvGetMemberId     ("Id",                 McStat, &Id_Attr))
  if (EnvGetMemberId     ("XyExtent",           McStat, &XyExtent_Attr))
  if (EnvGetMemberId     ("XyOrigin",           McStat, &XyOrigin_Attr))
  if (EnvGetClassId      ("XyRect",             McStat, &XyRect_Class))
  if (EnvGetClassId      ("XyPoint",            McStat, &XyPoint_Class))
  if (EnvInitClassMap    (XyRect_Class,         sizeof(XyRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr  )&Rect;
     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset = (size_t)((longaddr )&Rect.PartObjSlotAttr  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);


     Offset   = (size_t)((longaddr )&Rect.XyOrigin  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyOrigin_Attr,         Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.XyExtent  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyExtent_Attr,         Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

  if (Normal(*Status))
  if (EnvInitClassMap    (XyPoint_Class,        sizeof(XyPoint),   
                          C_Apl,                McStat))
      EnvInvokeMap       (C_Apl,                McStat);
TRACK(TrackBak,"XyRect_InitClass\n");
return (STAT);
}


XyRect  *XyRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token  = NullToken;
XyRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  XyRect_new0 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_NewImage (AnchorTkn,    XyRect_Class,  
                        McStat,      &Token,          (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);
      Image->MemberDrawObjs      = NullHandle;
/*
//    Image->MemberDrawObjs      = NullToken;
*/

      Image->draw                = XyRect_draw;
      Image->area                = XyRect_area;
      Image->destruct            = XyRect_delete;

      Image->XyOrigin            = *o;
      Image->XyExtent            = *e;

      if (Normal(*Status))
      DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 
 
XyRect  *XyRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
XyRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  XyRect_new1 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = XyRect_draw;
      Image->area                = XyRect_area;
      Image->destruct            = XyRect_delete;
   }

STAT;
return (Image);
}
 

lt64        XyRect_area (XyRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->XyExtent.xCoord - This->XyOrigin.xCoord)
                * (This->XyExtent.yCoord - This->XyOrigin.yCoord) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of XyRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     XyRect_draw (XyRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->XyOrigin.xCoord :%d\n", This->XyOrigin.xCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyOrigin.yCoord :%d\n", This->XyOrigin.yCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyExtent.xCoord :%d\n", This->XyExtent.xCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyExtent.yCoord :%d\n", This->XyExtent.yCoord);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the XyRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"XyRect_Draw\n");
return (STAT);
}
 
boolean    XyRect_dump (XyRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  XyRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"XyRect_Dump\n");
return (STAT);
}
 
boolean    XyRect_error (XyRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** XyRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"XyRect_Error\n");
return (STAT);
}

void XyRect_show (XyRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  XyRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof XyRect    : %3u\n",   sizeof(XyRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs, 
            This->Token.DbId,            This->MemberDrawObjs);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyOrigin.xCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyOrigin.xCoord - StartAddr, &This->XyOrigin.xCoord,
            This->XyOrigin.xCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyOrigin.yCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyOrigin.yCoord - StartAddr, &This->XyOrigin.yCoord,
            This->XyOrigin.yCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyExtent.xCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyExtent.xCoord - StartAddr, &This->XyExtent.xCoord,
            This->XyExtent.xCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyExtent.yCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyExtent.yCoord - StartAddr, &This->XyExtent.yCoord,
            This->XyExtent.yCoord);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");
return;
}


boolean XyRect_delete (XyRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
XyRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  XyRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " XyRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"XyRect_delete\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+*/
/* NAMED XYRECT                                                            */
/*+-----------------------------------------------------------------------+*/
boolean NamedXyRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
NamedXyRect     Rect;

  if (ClassBug)
     TraceMsg (0, "\n\n  NAMED XY RECT  Map...\n");

  if (XyRect_Class == 0)
  if (EnvGetMemberId     ("Id",                 McStat, &Id_Attr))
  if (EnvGetMemberId     ("XyExtent",           McStat, &XyExtent_Attr))
  if (EnvGetMemberId     ("XyOrigin",           McStat, &XyOrigin_Attr))
  if (EnvGetClassId      ("XyPoint",            McStat, &XyPoint_Class))
  if (EnvInitClassMap    (XyPoint_Class,        sizeof(XyPoint),   
                          C_Apl,                McStat))
      EnvInvokeMap       (C_Apl,                McStat);

  if (Normal(*Status))
  if (EnvGetMemberId     ("Name",               McStat, &Name_Attr))
  if (EnvGetClassId      ("NamedXyRect",        McStat, &NamedXyRect_Class))
  if (EnvInitClassMap    (NamedXyRect_Class,    sizeof(NamedXyRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Nam             -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Nam_Attr,              Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberNamedDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberNamedDrawObjs_Attr, Offset,  McStat);

     Offset   = (size_t)((longaddr )&Rect.XyOrigin        -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyOrigin_Attr,         Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.XyExtent        -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyExtent_Attr,         Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Name            -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Name_Attr,             Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }


TRACK(TrackBak,"NamedXyRect_InitClass\n");
return (STAT);
}


NamedXyRect  *NamedXyRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype     Token       = NullToken;
NamedXyRect  *Image       = NullPtr;
char          Name[64];
char          Buffer[32];
static numtype Index      = 0;
boolean       TempSetBug  = SetBug;
boolean       TempClassBug= ClassBug;

   Index   = DrawLib_NewId (McStat);
   Name[0] = '\0';
   Ut_IntToStr   ((int )Index, Name);
   if (Index == 1 || DeBug || OaBug || ClassBug)
   if (sprintf (Msg,
   "  NamedXyRect_new1:: <%12s> [%4u:%6u] XyPoints @ (%x, %x)\n",
                Name, AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_NewImage (AnchorTkn,    NamedXyRect_Class,  
                        McStat,      &Token,          (addrtype *)&Image))
#ifdef    __BAD_CODE__
   if (Attr_ValuePut   (&Token,       Name_Attr,       Name,       McStat))
#endif /* __BAD_CODE__  */
   {
      Image->Id                  = Index;

      Image->MemberDrawObjs      = NullHandle;
      Image->MemberNamedDrawObjs = NullHandle;

      Image->draw                = NamedXyRect_draw;
      Image->area                = NamedXyRect_area;
      Image->destruct            = NamedXyRect_delete;

      strcpy (Image->Nam,  Name);
      Image->XyOrigin            = *o;
      Image->XyExtent            = *e;

      if (Normal(*Status))
      if (DrawObjs_AddInto       (AnchorTkn,  &Token,  McStat))
      if (strlen (Name))
          NamedDrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

   SetBug   = TempSetBug;
   ClassBug = TempClassBug;

STAT;
return (Image);
}
 
 
NamedXyRect  *NamedXyRect_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
NamedXyRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  NamedXyRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = NamedXyRect_draw;
      Image->area                = NamedXyRect_area;
      Image->destruct            = NamedXyRect_delete;
   }

STAT;
return (Image);
}
 
lt64        NamedXyRect_area (NamedXyRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->XyExtent.xCoord - This->XyOrigin.xCoord)
                * (This->XyExtent.yCoord - This->XyOrigin.yCoord) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of NamedXyRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     NamedXyRect_draw (NamedXyRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->XyOrigin.xCoord :%d\n", This->XyOrigin.xCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyOrigin.yCoord :%d\n", This->XyOrigin.yCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyExtent.xCoord :%d\n", This->XyExtent.xCoord);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyExtent.yCoord :%d\n", This->XyExtent.yCoord);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the NamedXyRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"NamedXyRect_Draw\n");
return (STAT);
}
 
boolean    NamedXyRect_dump (NamedXyRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  NamedXyRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"NamedXyRect_Dump\n");
return (STAT);
}
 
boolean    NamedXyRect_error (NamedXyRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** NamedXyRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"NamedXyRect_Error\n");
return (STAT);
}

void NamedXyRect_show (NamedXyRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  NamedXyRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof NamedXyRect    : %3u\n",   sizeof(NamedXyRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);

  sprintf (Msg,  " This->Nam                 : +%3d @%x   = <%s>\n",
           (char *)&This->Nam         - StartAddr, &This->Nam,
            This->Nam);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberNamedDrawObjs : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberNamedDrawObjs - StartAddr,
           &This->MemberNamedDrawObjs,
            This->Token.DbId,                 This->MemberNamedDrawObjs);
/*
//          This->MemberNamedDrawObjs.DbId,   This->MemberNamedDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyOrigin.xCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyOrigin.xCoord - StartAddr, &This->XyOrigin.xCoord,
            This->XyOrigin.xCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyOrigin.yCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyOrigin.yCoord - StartAddr, &This->XyOrigin.yCoord,
            This->XyOrigin.yCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyExtent.xCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyExtent.xCoord - StartAddr, &This->XyExtent.xCoord,
            This->XyExtent.xCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyExtent.yCoord     : +%3d @%x   = %4u\n",
           (char *)&This->XyExtent.yCoord - StartAddr, &This->XyExtent.yCoord,
            This->XyExtent.yCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Name                : +%3d @%x   = <%s>\n",
           (char *)&This->Name        - StartAddr, &This->Name,
            This->Name);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");
return;
}


boolean NamedXyRect_delete (NamedXyRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
NamedXyRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  NamedXyRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " NamedXyRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"NamedXyRect_delete\n");
return (STAT);
}
 
 
/* .......................................................................
                    ....   ARRAYRECT      ....
   .......................................................................*/
 
boolean ArrayRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
ArrayRect       Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  ARRAY RECT  Map...\n");
  if (EnvGetMemberId     ("Xarray",             McStat, &Xarray_Attr))
  if (EnvGetMemberId     ("Yarray",             McStat, &Yarray_Attr))
  if (EnvGetClassId      ("ArrayRect",          McStat, &ArrayRect_Class))
  if (EnvInitClassMap    (ArrayRect_Class,      sizeof(ArrayRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);


     Offset   = (size_t)((longaddr )&Rect.Xarray   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Xarray_Attr,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Yarray    -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Yarray_Attr,          Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"ArrayRect_InitClass\n");
return (STAT);
}


ArrayRect  *ArrayRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token  = NullToken;
ArrayRect      *Image  = NullPtr;
addrtype     ArrayAddr = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  ArrayRect_new1 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create     (AnchorTkn,   ArrayRect_Class,  McStat,   &Token))
   if (Attr_ArrayCreate  (&Token,      Xarray_Attr,     4,  sizeof(short),
                          McStat,     &ArrayAddr)) 
   if (Object_GetImage   (&Token,      McStat,         (addrtype *)&Image))
   if (Attr_ArrayCreate  (&Token,      Yarray_Attr,     4,  sizeof(short),
                          McStat,     &ArrayAddr)) 

   if (!Normal(*Status))
   if (ClassBug)
   {
       ArrayRect_show    (Image);
       Object_Dump       (&Token);
   }

   if (Normal(*Status))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = ArrayRect_draw;
      Image->area                = ArrayRect_area;
      Image->destruct            = ArrayRect_delete;

      Image->Xarray[0]           = o->xCoord;
      Image->Yarray[0]           = o->yCoord;

      Image->Xarray[1]           = e->xCoord;
      Image->Yarray[1]           = o->yCoord;

      Image->Xarray[2]           = o->xCoord;
      Image->Yarray[2]           = e->yCoord;

      Image->Xarray[3]           = e->xCoord;
      Image->Yarray[3]           = e->yCoord;

      if (Normal(*Status))
      DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 
 
ArrayRect  *ArrayRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
ArrayRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  ArrayRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = ArrayRect_draw;
      Image->area                = ArrayRect_area;
      Image->destruct            = ArrayRect_delete;
   }

STAT;
return (Image);
}
 
lt64        ArrayRect_area (ArrayRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->Xarray[3] - This->Xarray[0])
                * (This->Yarray[3] - This->Yarray[0]) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of ArrayRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     ArrayRect_draw (ArrayRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->Xarray[0] :%d\n", This->Xarray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Yarray[0] :%d\n", This->Xarray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Xarray[3] :%d\n", This->Yarray[3]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Yarray[3] :%d\n", This->Yarray[3]);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the ArrayRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"ArrayRect_Draw\n");
return (STAT);
}
 
boolean    ArrayRect_dump (ArrayRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  ArrayRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"ArrayRect_Dump\n");
return (STAT);
}
 
boolean    ArrayRect_error (ArrayRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** ArrayRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"ArrayRect_Error\n");
return (STAT);
}

void ArrayRect_show (ArrayRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  ArrayRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof ArrayRect    : %3u\n",   sizeof(ArrayRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Xarray              : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->Xarray  - StartAddr, &This->Xarray,
            This->Xarray[0],This->Xarray[1],This->Xarray[2],This->Xarray[3]);
  TraceMsg (0, Msg);


  sprintf (Msg,  " This->Yarray              : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->Yarray  - StartAddr, &This->Yarray,
            This->Yarray[0],This->Yarray[1],This->Yarray[2],This->Yarray[3]);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");

return;
}

boolean ArrayRect_delete (ArrayRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
ArrayRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  ArrayRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " ArrayRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"ArrayRect_delete\n");
return (STAT);
}
 
/* .......................................................................
                    ....    DBLPTRRECT    ....
   .......................................................................*/
 
boolean DblPtrRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
DblPtrRect      Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  DBL PTR RECT  Map...\n");
  if (EnvGetMemberId     ("XyCoords",           McStat, &XyCoords_Attr))
  if (EnvGetClassId      ("DblPtrRect",         McStat, &DblPtrRect_Class))
  if (EnvInitClassMap    (DblPtrRect_Class,     sizeof(DblPtrRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.XyCoords -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyCoords_Attr,         Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"DblPtrRect_InitClass\n");
return (STAT);
}


DblPtrRect  *DblPtrRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token     = NullToken;
DblPtrRect  *Image     = NullPtr;
addrtype     ArrayAddr = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, 
                "  DblPtrRect_new1 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create     (AnchorTkn,   DblPtrRect_Class,  McStat,   &Token))
   if (Attr_ArrayCreate  (&Token,      XyCoords_Attr,     2,  sizeof(short*),
                          McStat,     &ArrayAddr)) 
   if (Attr_SubArrayCreate  
                         (&Token,      XyCoords_Attr,  0, 4,  sizeof(short),
                          McStat,     &ArrayAddr)) 
   if (Attr_SubArrayCreate  
                         (&Token,      XyCoords_Attr,  1, 4,  sizeof(short),
                          McStat,     &ArrayAddr)) 
   if (Object_GetImage   (&Token,      McStat,         (addrtype *)&Image))
   if (DeBug || OaBug || ClassBug)
      DblPtrRect_show (Image);

   if (Normal(*Status))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = DblPtrRect_draw;
      Image->area                = DblPtrRect_area;
      Image->destruct            = DblPtrRect_delete;

      Image->XyCoords[0][0]   = o->xCoord;
      Image->XyCoords[1][0]   = o->yCoord;

      Image->XyCoords[0][1]   = e->xCoord;
      Image->XyCoords[1][1]   = o->yCoord;

      Image->XyCoords[0][2]   = o->xCoord;
      Image->XyCoords[1][2]   = e->yCoord;

      Image->XyCoords[0][3]   = e->xCoord;
      Image->XyCoords[1][3]   = e->yCoord;

      if (Normal(*Status))
      DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);

      if (DeBug || OaBug || ClassBug)
         DblPtrRect_show (Image);
   }

STAT;
return (Image);
}
 
 
DblPtrRect  *DblPtrRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
DblPtrRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  DblPtrRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = DblPtrRect_draw;
      Image->area                = DblPtrRect_area;
      Image->destruct            = DblPtrRect_delete;
   }

STAT;
return (Image);
}
 
lt64        DblPtrRect_area (DblPtrRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->XyCoords[0][3] - This->XyCoords[0][0])
                * (This->XyCoords[1][3] - This->XyCoords[1][0]) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of DblPtrRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     DblPtrRect_draw (DblPtrRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->XyCoords[0][0] :%d\n", This->XyCoords[0][0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyCoords[1][0] :%d\n", This->XyCoords[1][0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyCoords[0][3] :%d\n", This->XyCoords[0][3]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->XyCoords[1][3] :%d\n", This->XyCoords[1][3]);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the DblPtrRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"DblPtrRect_Draw\n");
return (STAT);
}
 
boolean    DblPtrRect_dump (DblPtrRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  DblPtrRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"DblPtrRect_Dump\n");
return (STAT);
}
 
boolean    DblPtrRect_error (DblPtrRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** DblPtrRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"DblPtrRect_Error\n");
return (STAT);
}

void DblPtrRect_show (DblPtrRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  DblPtrRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof DblPtrRect    : %3u\n",   sizeof(DblPtrRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyCoords[0]         : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->XyCoords[0]  - StartAddr, &This->XyCoords[0],
            This->XyCoords[0][0],This->XyCoords[0][1],
            This->XyCoords[0][2],This->XyCoords[0][3]);
  TraceMsg (0, Msg);


  sprintf (Msg,  " This->XyCoords[1]         : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->XyCoords[1]  - StartAddr, &This->XyCoords[1],
            This->XyCoords[1][0],This->XyCoords[1][1],
            This->XyCoords[1][2],This->XyCoords[1][3]);
  TraceMsg (0, Msg);


  TraceMsg (0, "\n");

return;
}

boolean DblPtrRect_delete (DblPtrRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
DblPtrRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  DblPtrRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " DblPtrRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"DblPtrRect_delete\n");
return (STAT);
}
 
 
/* .......................................................................
                    ....   VARRAYRECT     ....
   .......................................................................*/
 
boolean VarrayRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
VarrayRect      Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  VARRAY RECT  Map...\n");
  if (EnvGetMemberId     ("xVarray",           McStat, &xVarray_Attr))
  if (EnvGetMemberId     ("yVarray",           McStat, &yVarray_Attr))
  if (EnvGetClassId      ("VarrayRect",        McStat, &VarrayRect_Class))
  if (EnvInitClassMap    (VarrayRect_Class,    sizeof(VarrayRect),   
                          C_Apl,               McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);



     Offset   = (size_t)((longaddr )&Rect.xVarray -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (xVarray_Attr,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.yVarray -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (yVarray_Attr,          Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"VarrayRect_InitClass\n");
return (STAT);
}


VarrayRect  *VarrayRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token     = NullToken;
VarrayRect  *Image     = NullPtr;
addrtype     ArrayAddr = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, 
                "  VarrayRect_new1 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create     (AnchorTkn,   VarrayRect_Class,  McStat,   &Token))
   if (Attr_ArrayCreate  (&Token,      xVarray_Attr,     4,  sizeof(short),
                          McStat,     &ArrayAddr)) 
   if (Attr_ArrayCreate  (&Token,      yVarray_Attr,     4,  sizeof(short),
                          McStat,     &ArrayAddr)) 
   if (Object_GetImage   (&Token,      McStat,         (addrtype *)&Image))
   if (DeBug || OaBug || ClassBug)
      VarrayRect_show (Image);

   if (Normal(*Status))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = VarrayRect_draw;
      Image->area                = VarrayRect_area;
      Image->destruct            = VarrayRect_delete;

      Image->xVarray.Varray[0]   = o->xCoord;
      Image->yVarray.Varray[0]   = o->yCoord;

      Image->xVarray.Varray[1]   = e->xCoord;
      Image->yVarray.Varray[1]   = o->yCoord;

      Image->xVarray.Varray[2]   = o->xCoord;
      Image->yVarray.Varray[2]   = e->yCoord;

      Image->xVarray.Varray[3]   = e->xCoord;
      Image->yVarray.Varray[3]   = e->yCoord;

      if (Normal(*Status))
         DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);

      if (DeBug || OaBug || ClassBug)
         VarrayRect_show (Image);
   }

STAT;
return (Image);
}
 
 
VarrayRect  *VarrayRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
VarrayRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  VarrayRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = VarrayRect_draw;
      Image->area                = VarrayRect_area;
      Image->destruct            = VarrayRect_delete;
   }

STAT;
return (Image);
}
 
lt64        VarrayRect_area (VarrayRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->xVarray.Varray[3] - This->xVarray.Varray[0])
                * (This->yVarray.Varray[3] - This->yVarray.Varray[0]) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of VarrayRect = %d\n", RecArea))
      TraceMsg (0, Msg);
STAT;
return (RecArea);
}
 

boolean     VarrayRect_draw (VarrayRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->xVarray[0] :%d\n", This->xVarray.Varray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->yVarray[0] :%d\n", This->xVarray.Varray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->xVarray[3] :%d\n", This->yVarray.Varray[3]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->yVarray[3] :%d\n", This->yVarray.Varray[3]);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the VarrayRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"VarrayRect_Draw\n");
return (STAT);
}
 
boolean    VarrayRect_dump (VarrayRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  VarrayRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"VarrayRect_Dump\n");
return (STAT);
}
 
boolean    VarrayRect_error (VarrayRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** VarrayRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"VarrayRect_Error\n");
return (STAT);
}

void VarrayRect_show (VarrayRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  VarrayRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof VarrayRect    : %3u\n",   sizeof(VarrayRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->xVarray.Varray      : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->xVarray.Varray  - StartAddr, &This->xVarray.Varray,
            This->xVarray.Varray[0],This->xVarray.Varray[1],
            This->xVarray.Varray[2],This->xVarray.Varray[3]);
  TraceMsg (0, Msg);


  sprintf (Msg,  " This->yVarray.Varray      : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->yVarray.Varray  - StartAddr, &This->yVarray.Varray,
            (This->yVarray.Varray)[0],(This->yVarray.Varray)[1],
            (This->yVarray.Varray)[2],(This->yVarray.Varray)[3]);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");

return;
}

boolean VarrayRect_delete (VarrayRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
VarrayRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  VarrayRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " VarrayRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"VarrayRect_delete\n");
return (STAT);
}
 
/* .......................................................................
                    ....  INTCHUNKRECT    ....
   .......................................................................*/
 
boolean IntChunkRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
IntChunkRect    Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  INT ARRAY RECT  Map...\n");

  if (EnvGetMemberId     ("Xcoords",            McStat, &Xcoords_Attr))
  if (EnvGetMemberId     ("Ycoords",            McStat, &Ycoords_Attr))
  if (EnvGetClassId      ("IntChunkRect",       McStat, &IntChunkRect_Class))
  if (EnvInitClassMap    (IntChunkRect_Class,      sizeof(IntChunkRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);



     Offset   = (size_t)((longaddr )&Rect.Xcoords -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Xcoords_Attr,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Ycoords -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Ycoords_Attr,          Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }
      EnvInvokeMap       (C_Apl,                McStat);
TRACK(TrackBak,"IntChunkRect_InitClass\n");
return (STAT);
}

IntChunkRect  *IntChunkRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token     = NullToken;
IntChunkRect  *Image     = NullPtr;
addrtype     ArrayAddr = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, 
                "  IntChunkRect_new1 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create     (AnchorTkn,   IntChunkRect_Class,  McStat,   &Token))
   if (Attr_ArrayCreate  (&Token,      Xcoords_Attr,     4,  sizeof(int),
                          McStat,     &ArrayAddr)) 
   if (Attr_ArrayCreate  (&Token,      Ycoords_Attr,     4,  sizeof(int),
                          McStat,     &ArrayAddr)) 
   if (Object_GetImage   (&Token,      McStat,         (addrtype *)&Image))
   if (DeBug || OaBug || ClassBug)
      IntChunkRect_show (Image);

   if (Normal(*Status))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = IntChunkRect_draw;
      Image->area                = IntChunkRect_area;
      Image->destruct            = IntChunkRect_delete;

      Image->Xcoords.Varray[0]   = (int )o->xCoord;
      Image->Ycoords.Varray[0]   = (int )o->yCoord;

      Image->Xcoords.Varray[1]   = (int )e->xCoord;
      Image->Ycoords.Varray[1]   = (int )o->yCoord;

      Image->Xcoords.Varray[2]   = (int )o->xCoord;
      Image->Ycoords.Varray[2]   = (int )e->yCoord;

      Image->Xcoords.Varray[3]   = (int )e->xCoord;
      Image->Ycoords.Varray[3]   = (int )e->yCoord;

      if (Normal(*Status))
         DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
      if (DeBug || OaBug || ClassBug)
         IntChunkRect_show (Image);
   }

STAT;
return (Image);
}
 
 
IntChunkRect  *IntChunkRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
IntChunkRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  IntChunkRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = IntChunkRect_draw;
      Image->area                = IntChunkRect_area;
      Image->destruct            = IntChunkRect_delete;
   }

STAT;
return (Image);
}
 
lt64        IntChunkRect_area (IntChunkRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (This->Xcoords.Varray[3] - This->Xcoords.Varray[0])
                * (This->Ycoords.Varray[3] - This->Ycoords.Varray[0]) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of IntChunkRect = %d\n", RecArea))
      TraceMsg (0, Msg);
STAT;
return (RecArea);
}
 

boolean     IntChunkRect_draw (IntChunkRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->Xcoords[0] :%d\n", This->Xcoords.Varray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Ycoords[0] :%d\n", This->Xcoords.Varray[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Xcoords[3] :%d\n", This->Ycoords.Varray[3]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Ycoords[3] :%d\n", This->Ycoords.Varray[3]);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the IntChunkRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"IntChunkRect_Draw\n");
return (STAT);
}
 
boolean    IntChunkRect_dump (IntChunkRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  IntChunkRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"IntChunkRect_Dump\n");
return (STAT);
}
 
boolean    IntChunkRect_error (IntChunkRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** IntChunkRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"IntChunkRect_Error\n");
return (STAT);
}

void IntChunkRect_show (IntChunkRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  IntChunkRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof IntChunkRect    : %3u\n",   sizeof(IntChunkRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs, 
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Xcoords.Varray      : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->Xcoords.Varray  - StartAddr, &This->Xcoords.Varray,
            This->Xcoords.Varray[0],This->Xcoords.Varray[1],
            This->Xcoords.Varray[2],This->Xcoords.Varray[3]);
  TraceMsg (0, Msg);


  sprintf (Msg,  " This->Ycoords.Varray      : +%3d @%x   = %4u %4u %4u %4u\n",
           (char *)&This->Ycoords.Varray  - StartAddr, &This->Ycoords.Varray,
            (This->Ycoords.Varray)[0],(This->Ycoords.Varray)[1],
            (This->Ycoords.Varray)[2],(This->Ycoords.Varray)[3]);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");

return;
}

boolean IntChunkRect_delete 
                         (IntChunkRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
IntChunkRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  IntChunkRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " IntChunkRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"IntChunkRect_delete\n");
return (STAT);
}
 
/* .......................................................................
                    ....    VCHUNKRECT    ....
   .......................................................................*/
 
boolean VchunkRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
VchunkRect      Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  VCHUNK  RECT  Map...\n");
  if (EnvGetMemberId     ("XyDir",              McStat, &XyDir_Attr))
  if (EnvGetClassId      ("VchunkRect",         McStat, &VchunkRect_Class))
  if (EnvInitClassMap    (VchunkRect_Class,     sizeof(VchunkRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);



     Offset   = (size_t)((longaddr )&Rect.XyDir   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (XyDir_Attr,            Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"VchunkRect_InitClass\n");
return (STAT);
}

#define   Vchunk(i,j)  Vchunk[i*4+j]
 
VchunkRect  *VchunkRect_new0
                         (tokentype    *AnchorTkn,            
                          XyPoint      *o,         XyPoint      *e,
                          ft F,lt Z,zz *Status)
{
tokentype    Token     = NullToken;
VchunkRect  *Image     = NullPtr;
short       *Vchunk    = NullPtr;
tokentype    VchunkTkn = NullToken;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, 
                "  VchunkRect_new1 Anchor= [%4u:%6u] XyPoints @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create       (AnchorTkn,   VchunkRect_Class,  McStat,   &Token))
   if (Object_GetImage     (&Token,      McStat,         (addrtype *)&Image))
   if (Vchunk_Create       (&Token,      sizeof(short),  8,   0,
                            McStat,     &VchunkTkn))
   if (Vchunk_PutStackPtr  (&VchunkTkn,  8,     McStat,  (addrtype *)&Vchunk))
   if (Normal(*Status))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = VchunkRect_draw;
      Image->area                = VchunkRect_area;
      Image->destruct            = VchunkRect_delete;

      Image->XyDir  = VchunkTkn;
      Vchunk[0]  = o->xCoord;
      Vchunk[1]  = o->yCoord;

      Vchunk[2]  = e->xCoord;
      Vchunk[3]  = o->yCoord;

      Vchunk[4]  = o->xCoord;
      Vchunk[5]  = e->yCoord;

      Vchunk[6]  = e->xCoord;
      Vchunk[7]  = e->yCoord;

      if (Normal(*Status))
         DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
      if (DeBug || OaBug || ClassBug)
         VchunkRect_show (Image);
   }

STAT;
return (Image);
}
 
 
VchunkRect  *VchunkRect_new1     (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
VchunkRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  VchunkRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = VchunkRect_draw;
      Image->area                = VchunkRect_area;
      Image->destruct            = VchunkRect_delete;
   }

STAT;
return (Image);
}
 

lt64        VchunkRect_area (VchunkRect   *This, ft F,lt Z,zz *Status)   
{
lt64         RecArea   = 0;
short       *Vchunk    = NullPtr;

   if (Vchunk_GetAddress (&This->XyDir,  McStat,  (addrtype *)&Vchunk))
      RecArea = abs( (Vchunk[6] - Vchunk[0])
                   * (Vchunk[7] - Vchunk[1]) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of VchunkRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     VchunkRect_draw (VchunkRect   *This,  ft F,lt Z,zz *Status)
{
short       *Vchunk    = NullPtr;
 
   if (Vchunk_GetAddress (&This->XyDir,  McStat,  (addrtype *)&Vchunk))
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->x   [0][0] :%d\n", Vchunk[0]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->y   [0][1] :%d\n", Vchunk[1]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->x   [3][0] :%d\n", Vchunk[6]);
      TraceMsg (0, Msg);
      sprintf (Msg, " This->y   [3][1] :%d\n", Vchunk[7]);
      TraceMsg (0, Msg);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the VchunkRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

   }

TRACK(TrackBak,"VchunkRect_Draw\n");
return (STAT);
}
 
boolean    VchunkRect_dump (VchunkRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  VchunkRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"VchunkRect_Dump\n");
return (STAT);
}
 
boolean    VchunkRect_error (VchunkRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** VchunkRect :: %s  @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"VchunkRect_Error\n");
return (STAT);
}

void VchunkRect_show (VchunkRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;
statustype     ShellStatus  = Env_Normal;

  sprintf (Msg, "\n The  VchunkRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof VchunkRect    : %3u\n",   sizeof(VchunkRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->XyDir               : +%3d @%x   = [%4u:%6u]\n",
           (char *)&This->XyDir  - StartAddr, &This->XyDir,
            This->XyDir.DbId,  This->XyDir.Handle);
  TraceMsg (0, Msg);
  Vchunk_Dump  (&This->XyDir,   ShellMcStat);

  TraceMsg (0, "\n");

return;
}

boolean VchunkRect_delete (VchunkRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
VchunkRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  VchunkRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " VchunkRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"VchunkRect_delete\n");
return (STAT);
}

/* .......................................................................
                    ....    REFRECT       ....
   .......................................................................*/
 
boolean RefRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
RefRect         Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  REF RECT  Map...\n");
  if (EnvGetMemberId     ("RefOrigin",          McStat, &RefOrigin_Attr))
  if (EnvGetMemberId     ("RefExtent",          McStat, &RefExtent_Attr))
  if (EnvGetClassId      ("RefRect",            McStat, &RefRect_Class))
  if (EnvInitClassMap    (RefRect_Class,        sizeof(RefRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);



     Offset   = (size_t)((longaddr )&Rect.RefOrigin  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (RefOrigin_Attr,         Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.RefExtent  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (RefExtent_Attr,         Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"RefRect_InitClass\n");
return (STAT);
}


RefRect    *RefRect_new0
                         (tokentype    *AnchorTkn,            
                          short         i0,        short         i1,  
                          short         i01,       ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
RefRect        *Image  = NullPtr;
CartesianPoint *o      = NullPtr;
CartesianPoint *e      = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  RefRect_New0 Anchor= [%4u:%6u] @ (%4u,%4u, %4u)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, i0, i1, i01))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    RefRect_Class,  McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,       (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = RefRect_draw;
      Image->area                = RefRect_area;
      Image->destruct            = RefRect_delete;

      Image->RefOrigin.ObjAddr   = (Point *)o;
      Image->RefExtent.ObjAddr   = (Point *)e;

      DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 

RefRect  *RefRect_new1
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status)
{
tokentype     Token  = NullToken;
RefRect      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  RefRect_New1 Anchor= [%4u:%6u] @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create     (AnchorTkn,    RefRect_Class,  McStat,   &Token))
   if (Object_GetImage   (&Token,       McStat,       (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = RefRect_draw;
      Image->area                = RefRect_area;
      Image->destruct            = RefRect_delete;

      Image->RefOrigin.ObjAddr   = o;
      Image->RefOrigin.ObjTkn    = o->Token;
      Image->RefExtent.ObjAddr   = e;
      Image->RefExtent.ObjTkn    = e->Token;

      if (Normal(*Status))
         DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 
RefRect  *RefRect_new2
                         (tokentype    *AnchorTkn,            
                          tokentype    *oTkn,      tokentype    *eTkn,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
RefRect        *Image  = NullPtr;
Point          *o      = NullPtr;
Point          *e      = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg,
      "  RefRect_new2 Anchor= [%4u:%6u] @ o= [%4u:%6u], e= [%4u:%6u]\n",
                AnchorTkn->DbId, AnchorTkn->Handle, 
                oTkn->DbId, eTkn->Handle,oTkn->DbId, eTkn->Handle)) 
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    RefRect_Class,  McStat,     &Token))
   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   if (Object_GetImage (oTkn,         McStat,         (addrtype *)&o))
   if (Object_GetImage (eTkn,         McStat,         (addrtype *)&e))
   {
      if (DrawLib01)
         Image->Id               = ++DrawLib01->DrawId;

      Image->MemberDrawObjs      = NullHandle;

      Image->draw                = RefRect_draw;
      Image->area                = RefRect_area;
      Image->destruct            = RefRect_delete;

      Image->RefOrigin.ObjAddr   = o;
      Image->RefOrigin.ObjTkn    = o->Token;
      Image->RefExtent.ObjAddr   = e;
      Image->RefExtent.ObjTkn    = e->Token;

      DrawObjs_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 
RefRect  *RefRect_new3
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
RefRect        *Image     = NullPtr;
Point          *Point0    = NullPtr;
tokentype       PointTkn  = NullToken;
numtype         DrawClass = 0;
CartesianPoint *c0        = NullPtr;
PolarPoint     *p0        = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  RefRect_new3 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = RefRect_draw;
      Image->area                = RefRect_area;
      Image->destruct            = RefRect_delete;
   }


   if (Normal(*Status))
   if (Image->RefOrigin.ObjTkn.Handle)
   if (Object_GetImage (&Image->RefOrigin.ObjTkn, McStat, (addrtype *)&Point0))
       Image->RefOrigin.ObjAddr  = Point0;

   if (Normal(*Status))
   if (Image->RefExtent.ObjTkn.Handle)
   if (Object_GetImage (&Image->RefExtent.ObjTkn, McStat, (addrtype *)&Point0))
       Image->RefExtent.ObjAddr  = Point0;

   /* The Coordinate Point virtual methods must be assigned               */
   if (Normal(*Status))
   if (Image->RefOrigin.ObjTkn.Handle)
   {
      PointTkn   = Image->RefOrigin.ObjTkn;
      DrawClass  = Object_IsA (&PointTkn,  McStat);
      if (DrawClass == CartesianPoint_Class)
         c0 = CartesianPoint_new1 (&PointTkn,  McStat);
      else
         p0 = PolarPoint_new1     (&PointTkn,  McStat);
   }

   if (Normal(*Status))
   if (Image->RefExtent.ObjTkn.Handle)
   {
      PointTkn   = Image->RefExtent.ObjTkn;
      DrawClass  = Object_IsA (&PointTkn,  McStat);
      if (DrawClass == CartesianPoint_Class)
         c0 = CartesianPoint_new1 (&PointTkn,  McStat);
      else
         p0 = PolarPoint_new1     (&PointTkn,  McStat);
   }

STAT;
return (Image);
}
 
lt64        RefRect_area (RefRect   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (Point_x(This->RefExtent.ObjAddr)
                 - Point_x(This->RefOrigin.ObjAddr))
                * (Point_y(This->RefExtent.ObjAddr)
                 - Point_y(This->RefOrigin.ObjAddr)) );

   if (DrawBug || DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "\n Area of RefRect = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     RefRect_draw (RefRect   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || DeBug || OaBug || ClassBug)
   {
    sprintf (Msg, "\n This->RefOrigin->x() :%d\n", 
                                        Point_x(This->RefOrigin.ObjAddr));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->RefOrigin->y() :%d\n", 
                                        Point_y(This->RefOrigin.ObjAddr));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->RefExtent->x() :%d\n", 
                                        Point_x(This->RefExtent.ObjAddr));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->RefExtent->y() :%d\n", 
                                        Point_y(This->RefExtent.ObjAddr));
      TraceMsg (0, Msg);
   } else {

      Point_x(This->RefOrigin.ObjAddr);
      Point_y(This->RefOrigin.ObjAddr);
      Point_x(This->RefExtent.ObjAddr);
      Point_y(This->RefExtent.ObjAddr);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the RefRect :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

      TraceMsg (0, "   RefOrigin :: Point");
      Object_Dump (&This->RefOrigin.ObjTkn);

      TraceMsg (0, "   RefExtent :: Point");
      Object_Dump (&This->RefExtent.ObjTkn);
   }

TRACK(TrackBak,"RefRect_Draw\n");
return (STAT);
}
 
boolean    RefRect_dump (RefRect   *This, ft F,lt Z,zz *Status) 
{
   TraceMsg (0, "\n The  RefRect ::\n");
   Object_Dump (&This->Token);

TRACK(TrackBak,"RefRect_Dump\n");
return (STAT);
}
 
boolean    RefRect_error (RefRect   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** RefRect:: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"RefRect_Error\n");
return (STAT);
}

void RefRect_show (RefRect   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  RefRect MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof RefRect            : %3u\n",   sizeof(RefRect));
  TraceMsg (0, Msg);

  sprintf (Msg,  " This                      :      @%x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->That                : +%3d @%x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token               : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Id                  : +%3d @%x   = %2u\n",
           (char *)&This->Id       - StartAddr, &This->Id, This->Id);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberDrawObjs      : +%3d @%x   = [%4u:%2u]\n",
           (char *)&This->MemberDrawObjs - StartAddr,
           &This->MemberDrawObjs,
            This->Token.DbId,            This->MemberDrawObjs);
/*
//          This->MemberDrawObjs.DbId,   This->MemberDrawObjs.Handle);
*/
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->RefOrigin              : +%3d @%x   = %x\n",
           (char *)&This->RefOrigin   - StartAddr, &This->RefOrigin,
            &This->RefOrigin);
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->RefOrigin.ObjAddr      : = %x\n",
           This->RefOrigin.ObjAddr);
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->RefOrigin.ObjTkn       : = [%4u:%6u]\n",
            This->RefOrigin.ObjTkn.DbId, This->RefOrigin.ObjTkn.Handle);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->RefExtent              : +%3d @%x   = %x\n",
           (char *)&This->RefExtent   - StartAddr, &This->RefExtent,
           &This->RefExtent);
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->RefExtent.ObjAddr      : = %x\n",
           This->RefExtent.ObjAddr);
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->RefExtent.ObjTkn       : = [%4u:%6u]\n",
            This->RefExtent.ObjTkn.DbId, This->RefExtent.ObjTkn.Handle);
  TraceMsg (0, Msg);


  TraceMsg (0, "\n");
return;
}

boolean RefRect_delete (RefRect   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
RefRect   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  RefRect_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " RefRect [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"RefRect_delete\n");
return (STAT);
}
 
/* .......................................................................
                    ....   PORTRECT       ....
   .......................................................................*/
 
boolean PortRect_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
PortRect        Rect;
  if (ClassBug)
     TraceMsg (0, "\n\n  PORT RECT  Map...\n");
  if (EnvGetMemberId     ("PortHole",           McStat, &PortHole_Attr))
  if (EnvGetMemberId     ("PortId",             McStat, &PortId_Attr))
  if (EnvGetMemberId     ("MemberAplPorts",     McStat, &MemberAplPorts_Attr))
  if (EnvGetMemberId     ("Port_XyOrigin",      McStat, &Port_XyOrigin_Attr))
  if (EnvGetMemberId     ("Port_XyExtent",      McStat, &Port_XyExtent_Attr))
  if (EnvGetClassId      ("PortRect",           McStat, &PortRect_Class))
  if (EnvInitClassMap    (PortRect_Class,       sizeof(PortRect),   
                          C_Apl,                McStat))
  {
     StartAddr= (longaddr )&Rect;

     Offset   = (size_t)((longaddr )&Rect.That    -  StartAddr);
     Env_AppendToMap    (That_AttrNum,          Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Token   -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Token_Attr,            Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Id      -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Id_Attr,               Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PartObjSlotAttr -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PartObj_Slot,   Offset,     McStat);


     Offset   = (size_t)((longaddr )&Rect.PortHole  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PortHole_Attr,         Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.PortId  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (PortId_Attr,           Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.MemberAplPorts  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (MemberAplPorts_Attr,   Offset,     McStat);


     Offset   = (size_t)((longaddr )&Rect.Port_XyOrigin  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Port_XyOrigin_Attr,    Offset,     McStat);

     Offset   = (size_t)((longaddr )&Rect.Port_XyExtent  -  StartAddr);
     if (Normal(*Status))
     Env_AppendToMap    (Port_XyExtent_Attr,    Offset,     McStat);

     if (Normal(*Status))
     Env_InvokeMap      (C_Apl,                             McStat);
  }
TRACK(TrackBak,"PortRect_InitClass\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                        END OF RECTS.C                                  !
  +-----------------------------------------------------------------------+*/
