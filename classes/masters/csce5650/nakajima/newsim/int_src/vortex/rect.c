/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C APPS - Rect.c */
/* ******************************  Rect.c  ******************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       2.4   Rect    - Procedures of Rectangle, XyRect, XyTrap,        */
/* *                       PortRect and RefRect.                           */
/* *********************************************************************** */

#define RECT
#include <stdarg.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "rect.h"
#include "vchunk.h"

extern boolean KernelInsertIn    (handletype    SetHndl, 
                                  tokentype    *CurrMember,
                                  tokentype    *NewMember,
                                  ft F,lt Z,zz *Status);

extern boolean KernelLastOf      (handletype    SetHndl, tokentype *Owner,
                                  ft F,lt Z,zz *Status,  tokentype *LastNode,
                                  tokentype    *LastMember);

extern boolean KernelOwnerOf     (handletype    SetHndl, tokentype *Member,
                                  ft F,lt Z,zz *Status,  tokentype *Owner);

extern boolean KernelPrevOf      (handletype    SetHndl, tokentype *Member,
                                  ft F,lt Z,zz *Status,  tokentype *PrevNode,
                                  tokentype    *PrevMember);
 
/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                         POLYGON CLASSES                               !
  !                             METHODS                                   !
  +-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
/* RECTANGLE                                                               */
/*+-----------------------------------------------------------------------+*/
boolean Rectangle_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
Rectangle       Rect;

/* Initialize the Class maps of those Class types accessed in this program */
  if (Rectangle_Class == 0)
  {
     if (EnvGetMemberId     ("Extent",             McStat, &Extent_Attr))
     if (EnvGetMemberId     ("Origin",             McStat, &Origin_Attr))

     if (EnvGetClassId      ("Rectangle",          McStat, &Rectangle_Class))
     if (EnvGetClassId      ("XyRect",             McStat, &XyRect_Class))
     if (EnvGetClassId      ("RefRect",            McStat, &RefRect_Class))
         EnvGetClassId      ("PortRect",           McStat, &PortRect_Class);
  }

  StartAddr   = (longaddr )&Rect;
  if (Normal(*Status))
  if (EnvInitClassMap    (Rectangle_Class,     sizeof(Rectangle),   
                          C_Apl,               McStat))
  {
     Offset    = (size_t)((longaddr )&Rect.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.Token   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.Id      -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Id_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.MemberDrawObjs  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberDrawObjs_Attr,   Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.PartObjSlotAttr  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (PartObj_Slot,   Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.Origin  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Origin_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr )&Rect.Extent  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Extent_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr  )&Rect.MemberLibRectangles  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberLibRectangles_Attr, Offset,  McStat))
         Env_InvokeMap      (C_Apl,                             McStat);
  }

TRACK(TrackBak,"Rectangle_InitClass\n");
return (STAT);
}

Rectangle  *Rectangle_new0
                         (tokentype    *AnchorTkn,            
                          short         i0,        short         i1,  
                          short         i01,       ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Rectangle      *Image  = NullPtr;
CartesianPoint *o      = NullPtr;
CartesianPoint *e      = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Rectangle_New0 Anchor= [%4u:%6u] @ (%4u,%4u, %4u)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, i0, i1, i01))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    Rectangle_Class,  McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id               = ++DrawLib01->DrawId;
      Image->MemberDrawObjs      = NullHandle;
/*
//    Image->MemberDrawObjs      = NullToken;
*/
      Image->draw                = Rectangle_draw;
      Image->area                = Rectangle_area;
      Image->destruct            = Rectangle_delete;

      Image->Origin              = (Point *)o;
      Image->Extent              = (Point *)e;
      Image->MemberLibRectangles = NullHandle;

      if (DrawObjs_AddInto       (AnchorTkn,  &Token,  McStat))
          LibRectangles_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 

Rectangle  *Rectangle_new1
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Rectangle      *Image  = NullPtr;
static  boolean Trace  = False;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Rectangle_New1 Anchor= [%4u:%6u] @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_NewImage (AnchorTkn,    Rectangle_Class,  
                        McStat,      &Token,          (addrtype *)&Image))

   {
      if (DrawLib01)
         Image->Id               = DrawLib_NewId (McStat);

      Image->MemberDrawObjs      = NullHandle;
/*
//    Image->MemberDrawObjs      = NullToken;
*/
      Image->draw                = Rectangle_draw;
      Image->area                = Rectangle_area;
      Image->destruct            = Rectangle_delete;

      Image->Origin              = o;
      Image->Extent              = e;

      Image->MemberLibRectangles = NullHandle;

      if (Normal(*Status))
      if (DrawObjs_AddInto       (AnchorTkn,  &Token,  McStat))
          LibRectangles_AddInto  (AnchorTkn,  &Token,  McStat);
   }
   if (Trace)
   {
      Trace = False;
      OaBug = False;
   }
STAT;
return (Image);
}
 
Rectangle  *Rectangle_new2
                         (tokentype    *AnchorTkn,            
                          Point        *o,         Point        *e,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Rectangle      *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Rectangle_New2 Anchor= [%4u:%6u] @ (%x, %x)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, o, e))
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    Rectangle_Class,  McStat,   &Token))

   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   {
      if (DrawLib01)
         Image->Id    = ++DrawLib01->DrawId;
      Image->MemberDrawObjs      = NullHandle;
/*
//    Image->MemberDrawObjs      = NullToken;
*/
      Image->draw                = Rectangle_draw;
      Image->area                = Rectangle_area;
      Image->destruct            = Rectangle_delete;
      Image->Origin              = o;
      Image->Extent              = e;
      Image->MemberLibRectangles = NullHandle;

      if (DrawObjs_AddInto       (AnchorTkn,  &Token,  McStat))
          LibRectangles_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}

Rectangle  *Rectangle_new3
                         (tokentype    *AnchorTkn,            
                          tokentype    *oTkn,      tokentype    *eTkn,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
Rectangle      *Image  = NullPtr;
Point          *o      = NullPtr;
Point          *e      = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg,
      "  Rectangle_new3 Anchor= [%4u:%6u] @ o= [%4u:%6u], e= [%4u:%6u]\n",
                AnchorTkn->DbId, AnchorTkn->Handle, 
                oTkn->DbId, eTkn->Handle,oTkn->DbId, eTkn->Handle)) 
      TraceMsg (0, Msg);

   if (Object_Create   (AnchorTkn,    Rectangle_Class,  McStat,   &Token))
   if (Object_GetImage (&Token,       McStat,         (addrtype *)&Image))
   if (Object_GetImage (oTkn,         McStat,         (addrtype *)&o))
   if (Object_GetImage (eTkn,         McStat,         (addrtype *)&e))
   {
      if (DrawLib01)
         Image->Id    = ++DrawLib01->DrawId;
      Image->MemberDrawObjs      = NullHandle;
/*
//    Image->MemberDrawObjs      = NullToken;
*/
      Image->draw                = Rectangle_draw;
      Image->area                = Rectangle_area;
      Image->destruct            = Rectangle_delete;
      Image->Origin              = o;
      Image->Extent              = e;

      Image->MemberLibRectangles = NullHandle;

      if (DrawObjs_AddInto       (AnchorTkn,  &Token,  McStat))
          LibRectangles_AddInto  (AnchorTkn,  &Token,  McStat);
   }

STAT;
return (Image);
}
 
Rectangle  *Rectangle_new4
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
Rectangle      *Image  = NullPtr;
tokentype       PointTkn  = NullToken;
numtype         DrawClass = 0;
CartesianPoint *c0        = NullPtr;
PolarPoint     *p0        = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  Rectangle_new4 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   {
      Image->draw                = Rectangle_draw;
      Image->area                = Rectangle_area;
      Image->destruct            = Rectangle_delete;
   }
   /* The Coordinate Point virtual methods must be assigned               */
   if (Normal(*Status))
   if (Image->Origin != NullPtr)
   {
      PointTkn   = Image->Origin->Token;
      DrawClass  = Object_IsA (&PointTkn,  McStat);
      if (DrawClass == CartesianPoint_Class)
         c0 = CartesianPoint_new1 (&PointTkn,  McStat);
      else
         p0 = PolarPoint_new1     (&PointTkn,  McStat);

   }

   if (Normal(*Status))
   if (Image->Extent != NullPtr)
   {
      PointTkn   = Image->Extent->Token;
      DrawClass  = Object_IsA (&PointTkn,  McStat);
      if (DrawClass == CartesianPoint_Class)
         c0 = CartesianPoint_new1 (&PointTkn,  McStat);
      else
         p0 = PolarPoint_new1     (&PointTkn,  McStat);
   }

STAT;
return (Image);
}
 
lt64        Rectangle_area (Rectangle   *This, ft F,lt Z,zz *Status)   
{
lt64 RecArea = 0;

   RecArea = abs( (Point_x(This->Extent) - Point_x(This->Origin))
                * (Point_y(This->Extent) - Point_y(This->Origin)) );

   if (DrawBug || ClassBug)
   if (sprintf (Msg, "\n Area of Rectangle = %d\n", RecArea))
      TraceMsg (0, Msg);

STAT;
return (RecArea);
}
 

boolean     Rectangle_draw (Rectangle   *This,  ft F,lt Z,zz *Status)
{
 
   if (DrawBug || ClassBug)
   {
    sprintf (Msg, "\n This->Origin->x() :%d\n", Point_x(This->Origin));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Origin->y() :%d\n", Point_y(This->Origin));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Extent->x() :%d\n", Point_x(This->Extent));
      TraceMsg (0, Msg);
      sprintf (Msg, " This->Extent->y() :%d\n", Point_y(This->Extent));
      TraceMsg (0, Msg);
   } else {

      Point_x(This->Origin);
      Point_y(This->Origin);
      Point_x(This->Extent);
      Point_y(This->Extent);
   }

   if (DeBug || OaBug || ClassBug)
   {
      sprintf (Msg, "\n Draw the Rectangle :: This = %x; Token[%2u:%8u]\n",
               This, This->Token.DbId, This->Token.Handle);

      TraceMsg (0, Msg);
      Object_Dump (&This->Token);

      TraceMsg (0, "   Origin :: Point");
      Object_Dump (&This->Origin->Token);

      TraceMsg (0, "   Extent :: Point");
      Object_Dump (&This->Extent->Token);
   }

TRACK(TrackBak,"Rectangle_Draw\n");
return (STAT);
}
 
void Rectangle_show (Rectangle   *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  Rectangle MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  " sizeof Rectangle    : %3u\n",   sizeof(Rectangle));
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

  sprintf (Msg,  " This->Origin              : +%3d @%x   = %x\n",
           (char *)&This->Origin   - StartAddr, &This->Origin,
            This->Origin);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Extent              : +%3d @%x   = %x\n",
           (char *)&This->Extent   - StartAddr, &This->Extent,
           This->Extent);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->MemberLibRectangles : +%3d @%x   = %2u\n",
           (char *)&This->MemberLibRectangles - StartAddr,
           &This->MemberLibRectangles, This->MemberLibRectangles );
  TraceMsg (0, Msg);


  TraceMsg (0, "\n");
return;
}
 
boolean    Rectangle_error (Rectangle   *This,  char * msg, 
                           ft F,lt Z,zz *Status)
{
   sprintf (Msg, "\n  *** Rectangle :: %s @%x\n", msg, This);
   SendMsg (0, Msg);
   exit(1);

TRACK(TrackBak,"Rectangle_error\n");
return (STAT);
}


boolean Rectangle_delete (Rectangle   **This,   deletetype    DbEvent,
                          ft F,lt Z,zz *Status)
{
Rectangle   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  Rectangle_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " Rectangle [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"Rectangle_delete\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/* LIBRECTANGLES                                                           */
/*+-----------------------------------------------------------------------+*/
boolean LibRectangles_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " LibRectangles::InitClass...\n");

  if (!OwnerLibRectangles_Attr)
/*                                                       <CFS [2.2.5  ] >  */
  if (EnvGetMemberId    ("OwnerLibRectangles",
                         McStat,               &OwnerLibRectangles_Attr))
  if (EnvGetMemberId    ("MemberLibRectangles",
                         McStat,               &MemberLibRectangles_Attr))
/*                                                       <CFS [2.1.5  ] >  */
      EnvGetSetId       ("LibRectangles",      
                         McStat,               &LibRectangles_Set);

TRACK(TrackBak,"LibRectangles_InitClass\n");
return (STAT);
}
 
 
boolean OwnerOfLibRectangles (tokentype *Member,
                              ft F,lt Z,zz *Status,    tokentype *Owner)
{
/*                                                       <CFS [7.2.1  ] >  */

   KernelOwnerOf  (LibRectangles_Set, Member, McStat, Owner);

TRACK(TrackBak,"OwnerOfLibRectangles\n");
return (STAT);
}
 
 
 
boolean LibRectangles_AddInto
                         (tokentype    *OwnerTkn,  tokentype    *MemberTkn,
                          ft F,lt Z,zz *Status)
{
/*                                                       <CFS [7.2.5  ] >  */
   SetAddInto (LibRectangles_Set, OwnerTkn, MemberTkn, McStat);

TRACK(TrackBak,"LibRectangles_AddInto\n");
return (STAT);
}
 
boolean LibRectangles_FindIn
                         (tokentype    *OwnerTkn,  addrtype     KeyValue, 
                          ft F,lt Z,zz *Status,    tokentype   *MemberTkn)
{
/*                                                       <CFS [7.1.5  ] >  */
   SetFindIn  (LibRectangles_Set, OwnerTkn,   KeyValue,
               McStat,            MemberTkn); 

TRACK(TrackBak,"FindInLibRectangles\n");
return (STAT);
}
 
boolean LibRectangles_IterateOn
                         (tokentype    *OwnerTkn,  treeiterfunc  IterFunc,
                          ft F,lt Z,zz *Status)
{
  SpclTraverseTree (LibRectangles_Set,  OwnerTkn, IterFunc, McStat);

  if (*Status == Set_EndOfSet  
  ||  *Status == Set_EmptySet)
      *Status  = Iterate_EndOf;

return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                        END OF RECT.C                                   !
  +-----------------------------------------------------------------------+*/
