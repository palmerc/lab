/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C APPS - Point.c */
/* ******************************  Point.c  *****************************  */
/* *  Test program for the DrawLib library                                 */
/* *                               Recipies in the Developers CookBook     */
/* *       2.3   Point   - Procedures of Point, CartesianPoint, PolarPoint.*/
/* *********************************************************************** */

#define POINT
#include <stdarg.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "drawlib.h"
#include "point.h"

extern boolean Core0_MoreCore (bytessizetype  Size,
                               ft F,lt Z,zz  *Status,    addrtype *Addr);

/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                        COORDINATE POINTS                              !
  !                             METHODS                                   !
  +-----------------------------------------------------------------------+*/
#undef __HP__
/*+-----------------------------------------------------------------------+*/
/* POINT                                                                   */
/*+-----------------------------------------------------------------------+*/
/* POINT */

boolean Point_InitClass  (ft F,lt Z,zz *Status)
{
#ifdef __HP__
  tokentype (*pmf3)(Point*)        = &Point_GetToken;
#else
  tokentype (*pmf3)(Point*)        = Point_GetToken;
#endif

  if (Normal(*Status))
  if (EnvGetClassId      ("Point",             McStat, &Point_Class))
  if (EnvInitClassMap    (Point_Class,         sizeof(Point),   
                          C_Apl,               McStat))
      EnvInvokeMap       (C_Apl,               McStat);

  Env_LoadGetTknCode (Point_Class, (funcptrtype  )pmf3,  McStat);

TRACK(TrackBak,"Point_InitClass\n");
return (STAT);
}

tokentype  Point_GetToken(Point   *This)
{

return (This->Token);
}

short      Point_x       (Point   *This)
{

   return  (This->Point_x       (This));
}      
 
short      Point_y      (Point          *This) 
{
   return  (This->Point_y       (This));
}
 
double     Point_Theta  (Point          *This) 
{
   return  (This->Point_Theta   (This));
}
 
double     Point_Radius (Point          *This) 
{ 
   return  (This->Point_Radius  (This));
}



/*+-----------------------------------------------------------------------+*/
/* CARTESIAN POINT                                                         */
/*+-----------------------------------------------------------------------+*/

boolean CartesianPoint_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset           = 0;
longaddr        StartAddr        = 0;
CartesianPoint  Point;
#ifdef __HP__
  void*         (*pmf1)(size_t)  = &CartesianPoint_New;
#else
  void*         (*pmf1)(size_t);
    pmf1   = (void* (* )(size_t))CartesianPoint_New;
#endif

  if (xCoord_Attr == 0)
  {
     if (EnvGetMemberId     ("xCoord",        McStat, &xCoord_Attr))
     if (EnvGetMemberId     ("yCoord",        McStat, &yCoord_Attr))
         EnvGetClassId      ("CartesianPoint",McStat, &CartesianPoint_Class);
  }

  StartAddr   = (longaddr)&Point;

  if (Normal(*Status))
  if (EnvInitClassMap    (CartesianPoint_Class,    sizeof(CartesianPoint),   
                          C_Apl,               McStat))
  {
     Offset    = (size_t)((longaddr )&Point.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.Token   -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.xCoord  -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (xCoord_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.yCoord  -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (yCoord_Attr,           Offset,     McStat))
         Env_InvokeMap      (C_Apl,                             McStat);
  }
  if (Normal(*Status))
     Env_LoadCreateCode  (CartesianPoint_Class, (funcptrtype )pmf1, McStat);


TRACK(TrackBak,"CartesianPoint_InitClass\n");
return (STAT);
}

CartesianPoint *CartesianPoint_New
                         (size_t        ClassSize)
{
CartesianPoint *Image       = NullPtr;
statustype      ShellStatus = Env_Normal;

   ClassSize = sizeof (CartesianPoint);

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  CartesianPoint_New:: ClassSize= %2u \n",
                ClassSize))
      SendMsg (0, Msg);

   if (Core0_MoreCore   ((bytessizetype )ClassSize,  
                          ShellMcStat,               (addrtype *)&Image))
   {
      Image->Point_x      = CartesianPoint_x;
      Image->Point_y      = CartesianPoint_y;
      Image->Point_Theta  = CartesianPoint_Theta;
      Image->Point_Radius = CartesianPoint_Radius;
   }

return (Image);
} 

CartesianPoint *CartesianPoint_new0
                         (tokentype    *AnchorTkn,
                          short         x,         short         y,
                          ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
CartesianPoint *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  CartesianPoint_New0 Anchor= [%4u:%6u] @ (%4u,%4u)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, x, y))
      SendMsg (0, Msg);

   if (Object_NewImage  (AnchorTkn,    CartesianPoint_Class,  
                         McStat,      &Token,     (addrtype *)&Image))
/*
//   if (Object_Create    (AnchorTkn,    CartesianPoint_Class,
//                         McStat,                              &Token))

//   if (Object_GetImage  (&Token,       McStat,    (addrtype *)&Image))
*/
   {
      Image->xCoord       = x;
      Image->yCoord       = y;
      Image->Point_x      = CartesianPoint_x;
      Image->Point_y      = CartesianPoint_y;
      Image->Point_Theta  = CartesianPoint_Theta;
      Image->Point_Radius = CartesianPoint_Radius;
   }


STAT;
return (Image);
} 
 
CartesianPoint *CartesianPoint_new1
                         (tokentype    *PointTkn,  ft F,lt Z,zz *Status)
{
CartesianPoint *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  CartesianPoint_New1 Anchor= [%4u:%6u]\n",
                PointTkn->DbId, PointTkn->Handle))
      SendMsg (0, Msg);

   if (Object_GetImage (PointTkn,      McStat,    (addrtype *)&Image))
   {
      Image->Point_x      = CartesianPoint_x;
      Image->Point_y      = CartesianPoint_y;
      Image->Point_Theta  = CartesianPoint_Theta;
      Image->Point_Radius = CartesianPoint_Radius;
   }

STAT;
return (Image);
} 
 
 
short     CartesianPoint_x      (CartesianPoint   *This) 
{
   return This->xCoord;
}      
 
short     CartesianPoint_y      (CartesianPoint   *This) 
{
   return This->yCoord;
}      
 
double    CartesianPoint_Radius (CartesianPoint   *This) 
{
   return (sqrt(This->xCoord * This->xCoord 
              + This->yCoord * This->yCoord));
}
 
double    CartesianPoint_Theta  (CartesianPoint   *This) 
{
double Theta = 0.0;

   if (This->xCoord == 0) 
      return (This->yCoord >= 0 ? (Dbl_Pie / 2) : (-Dbl_Pie/2));
 
   Theta = atan (This->yCoord / This->xCoord);

   if (This->xCoord >= 0) 
   {
      if (This->yCoord >= 0)
         return (Theta);
      else 
         return (2 * Dbl_Pie + Theta);
   } else
      return (Dbl_Pie + Theta);
}

void  CartesianPoint_show (CartesianPoint *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n CartsianPoint(*%3u) ::[%2u:%8u] \n",
           sizeof(CartesianPoint), Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr          = (longaddr )This;
  sprintf (Msg,  " this                  : @%x\n",   This );
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->That            : +%3d @%x   = %x\n",
           (char *)&This->That - StartAddr,
           &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token           : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token - StartAddr, &This->Token, 
            This->Token.DbId,      This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->xCoord          : +%3d @%x   = %2u\n",
           ((char *)&(This->xCoord)) - StartAddr,
           &This->xCoord, This->xCoord );
  TraceMsg (0, Msg);


  sprintf (Msg,  " This->yCoord          : +%3d @%x   = %2u\n",
           (char *)&This->yCoord - StartAddr,
           &This->yCoord, This->yCoord );
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");
return;
}

 
 
boolean    CartesianPoint_delete
                         (CartesianPoint **This,   deletetype    DbEvent,
                          ft F,lt Z,zz    *Status) 
{
CartesianPoint  *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  CartesianPoint_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " CartesianPoint[%2u:%8u] %s in DB\n",
            (*This)->Token.DbId, (*This)->Token.Handle, DeleteStr[DbEvent]))
      TraceMsg (0, Msg);

   switch (DbEvent)
   {
      case dbFree:
/*                                                       <CFS [4.2.8  ] >  */
           Object_FreeImage   (&That->Token,  McStat);
           break;

      case dbCommit:
/*                                                       <CFS [4.2.9  ] >  */
           Object_CommitImage (&That->Token,  True,    McStat);
           break;

      case dbDelete:
/*                                                       <CFS [4.1.18 ] >  */
           Object_Delete      (&That->Token,  McStat);
   }

  *This   = NullPtr;


TRACK(TrackBak,"CartesianPoint_delete\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+*/
/* POLAR POINT                                                             */
/*+-----------------------------------------------------------------------+*/

boolean PolarPoint_InitClass  (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
PolarPoint      Point;

#ifdef __HP__
  void* (*pmf1)(size_t)  = &PolarPoint_New;
#else
  void* (*pmf1)(size_t);
    pmf1   = (void* (* )(size_t))PolarPoint_New;

#endif

  if (Theta_Attr == 0)
  {
     if (EnvGetMemberId     ("Theta",              McStat, &Theta_Attr))
     if (EnvGetMemberId     ("Radius",             McStat, &Radius_Attr))
         EnvGetClassId      ("PolarPoint",         McStat, &PolarPoint_Class);
  }

  StartAddr   = (longaddr)&Point;
  if (Normal(*Status))
  if (EnvInitClassMap    (PolarPoint_Class,    sizeof(PolarPoint),   
                          C_Apl,               McStat))
  {
     Offset    = (size_t)((longaddr )&Point.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.Token   -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.Theta   -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (Theta_Attr,           Offset,     McStat))
        Offset = (size_t)((longaddr )&Point.Radius  -  StartAddr);
     if (Normal(*Status))
     if (Env_AppendToMap    (Radius_Attr,           Offset,     McStat))
         Env_InvokeMap      (C_Apl,                             McStat);
  }
  if (Normal(*Status))
     Env_LoadCreateCode  (PolarPoint_Class, (funcptrtype  )pmf1,  McStat);


TRACK(TrackBak,"PolarPoint_InitClass\n");
return (STAT);
}

PolarPoint *PolarPoint_New
                         (size_t        ClassSize)
{
PolarPoint *Image       = NullPtr;
statustype  ShellStatus = Env_Normal;

   ClassSize = sizeof (PolarPoint);

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  PolarPoint_New:: ClassSize= %2u \n",
                ClassSize))
      SendMsg (0, Msg);

   if (Core0_MoreCore     ((bytessizetype )ClassSize,  
                          ShellMcStat,              (addrtype *)&Image))
   {
      Image->Point_x      = PolarPoint_x;
      Image->Point_y      = PolarPoint_y;
      Image->Point_Theta  = PolarPoint_Theta;
      Image->Point_Radius = PolarPoint_Radius;
   }

return (Image);
} 
 
 
PolarPoint *PolarPoint_new0
                         (tokentype    *AnchorTkn,
                          double        Theta,     double        Radius,
                          ft F,lt Z,zz *Status)
{
tokentype   Token  = NullToken;
PolarPoint *Image  = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  PolarPoint_New0 Anchor= [%4u:%6u] @ (%f,%f)\n",
                AnchorTkn->DbId, AnchorTkn->Handle, Theta, Radius))
      SendMsg (0, Msg);

   if (Object_Create    (AnchorTkn,    PolarPoint_Class,
                         McStat,                              &Token))

   if (Object_GetImage  (&Token,       McStat,    (addrtype *)&Image))
   {
      Image->Theta        = Theta;
      Image->Radius       = Radius;
      Image->Point_x      = PolarPoint_x;
      Image->Point_y      = PolarPoint_y;
      Image->Point_Theta  = PolarPoint_Theta;
      Image->Point_Radius = PolarPoint_Radius;
   }

STAT;
return (Image);
} 
 
PolarPoint *PolarPoint_new1
                         (tokentype    *PointTkn,  ft F,lt Z,zz *Status)
{
PolarPoint *Image       = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  PolarPoint_New1 Anchor= [%4u:%6u]\n",
                PointTkn->DbId, PointTkn->Handle))
      SendMsg (0, Msg);

   if (Object_GetImage (PointTkn,      McStat,    (addrtype *)&Image))
   {
      Image->Point_x      = PolarPoint_x;
      Image->Point_y      = PolarPoint_y;
      Image->Point_Theta  = PolarPoint_Theta;
      Image->Point_Radius = PolarPoint_Radius;
   }

STAT;
return (Image);
} 
 
 
short     PolarPoint_x      (PolarPoint   *This) 
{
   return (short )(This->Radius * cos(This->Theta));
}      
 
short     PolarPoint_y      (PolarPoint   *This) 
{
   return (short )(This->Radius * sin(This->Theta));
}      
 
double    PolarPoint_Radius (PolarPoint   *This) 
{
   return This->Radius;
}
 
double    PolarPoint_Theta  (PolarPoint   *This) 
{
   return This->Theta;
}
 
void  PolarPoint_show (PolarPoint *This)
{
longaddr       StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n The  PolarPoint    MAP:: [%2u:%8u]\n",
           sizeof(PolarPoint), Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr          = (longaddr )This;
  sprintf (Msg,  " this                  : @%x\n",   This );
  TraceMsg (0, Msg);
  sprintf (Msg,  " This->That            : +%3d @%x   = %x\n",
           (char *)&This->That - StartAddr,
           &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Token           : +%3d @%x   = [%2u:%8u]\n",
           (char *)&This->Token - StartAddr, &This->Token, 
            This->Token.DbId,      This->Token.Handle );
  TraceMsg (0, Msg);

  TraceMsg (0, Msg);
  StartAddr          = (longaddr )This;
 
  sprintf (Msg,  " This->Theta           : +%3d @%x   = %2u\n",
           (char *)&This->Theta  - StartAddr,
           &This->Theta,  This->Theta  );
  TraceMsg (0, Msg);

  sprintf (Msg,  " This->Radius          : +%3d @%x   = %2u\n",
           (char *)&This->Radius - StartAddr,
           &This->Radius, This->Radius );
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");
return;
}

boolean    PolarPoint_delete
                         (PolarPoint **This,   deletetype    DbEvent,
                          ft F,lt Z,zz    *Status) 
{
PolarPoint  *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  PolarPoint_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " PolarPoint[%2u:%8u] %s in DB\n",
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


TRACK(TrackBak,"PolarPoint_delete\n");
return (STAT);
}
 

/*+-----------------------------------------------------------------------+
~M                        END OF POINT.C                                  !
  +-----------------------------------------------------------------------+*/
