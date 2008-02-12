/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Dev - Point.h */
/* **************************** POINT.H  *********************************
   *  Header for Draw01.h  Rectangle composed of two points
   *                       for both Cartesian and Polar coordinates
   *  Adapted from C++ Report  by Ted Goldstein
   *********************************************************************** */

#ifndef POINT_01
#define POINT_01

/*+-----------------------------------------------------------------------+*
~d                           DEFINES / TYPEDEFS 
 *+-----------------------------------------------------------------------+*/
#ifndef DRAWLIB_01
#ifdef POINT
          numtype    xCoord_Attr            = 0;
          numtype    yCoord_Attr            = 0;

          numtype    Theta_Attr             = 0;
          numtype    Radius_Attr            = 0;

          numtype    Point_Class            = 0;
          numtype    CartesianPoint_Class   = 0;
          numtype    PolarPoint_Class       = 0;
#else
   extern numtype    xCoord_Attr;
   extern numtype    yCoord_Attr;
   extern numtype    Theta_Attr;
   extern numtype    Radius_Attr;
   extern numtype    Point_Class;
   extern numtype    CartesianPoint_Class;
   extern numtype    PolarPoint_Class;
#endif
#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
 
typedef struct Point
{
   addrtype          That;
   tokentype         Token;

   short            (*Point_x)      (struct Point   *This);
   short            (*Point_y)      (struct Point   *This);

   doubletype       (*Point_Theta)  (struct Point   *This);
   doubletype       (*Point_Radius) (struct Point   *This);

} Point;
 

typedef struct CartesianPoint
{ 
   addrtype          That;
   tokentype         Token;

   short            (*Point_x)      (struct CartesianPoint *This);
   short            (*Point_y)      (struct CartesianPoint *This);
   doubletype       (*Point_Theta)  (struct CartesianPoint *This);
   doubletype       (*Point_Radius) (struct CartesianPoint *This);

   shorttype         xCoord;
   shorttype         yCoord;

} CartesianPoint;
 

typedef struct PolarPoint
{
   addrtype          That;
   tokentype         Token;

   short            (*Point_x)      (struct PolarPoint     *This);
   short            (*Point_y)      (struct PolarPoint     *This);

   doubletype       (*Point_Theta)  (struct PolarPoint     *This);
   doubletype       (*Point_Radius) (struct PolarPoint     *This);

   doubletype        Theta;
   doubletype        Radius;

} PolarPoint;
 


/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/* .......................................................................
                    ....      POINT        ....
   .......................................................................*/
       boolean     Point_InitClass 
                         (ft F,lt Z,zz *Status);

       tokentype   Point_GetToken(Point   *This);

       short   Point_x   (Point        *This);

       short   Point_y   (Point        *This);

       double  Point_Theta
                         (Point        *This);

       double  Point_Radius
                         (Point        *This);


/* .......................................................................
                    ....  CARTESIAN POINT  ....
   .......................................................................*/

       boolean CartesianPoint_InitClass
                         (ft F,lt Z,zz *Status);

       CartesianPoint *CartesianPoint_New
                         (size_t        ClassSize);

       CartesianPoint *CartesianPoint_new0
                         (tokentype    *AnchorTkn,
                          short         x,         short         y,
                          ft F,lt Z,zz *Status);

       CartesianPoint *CartesianPoint_new1
                         (tokentype    *PointTkn,  ft F,lt Z,zz *Status); 

       short   CartesianPoint_x
                         (CartesianPoint *This);

       short   CartesianPoint_y    
                         (CartesianPoint *This);

       double  CartesianPoint_Theta 
                         (CartesianPoint *This);

       double  CartesianPoint_Radius 
                         (CartesianPoint *This);

       void    CartesianPoint_show
                         (CartesianPoint *This);

       boolean CartesianPoint_delete
                         (CartesianPoint **This,   deletetype    DbEvent,
                          ft F,lt Z,zz    *Status);


/* .......................................................................
                    ....   POLAR POINT     ....
   .......................................................................*/

       boolean PolarPoint_InitClass
                         (ft F,lt Z,zz *Status);

       PolarPoint *PolarPoint_New
                         (size_t        ClassSize);

       PolarPoint *PolarPoint_new0
                         (tokentype    *AnchorTkn,
                          double        Theta,     double        Radius,
                          ft F,lt Z,zz *Status);

       PolarPoint *PolarPoint_new1
                         (tokentype    *PointTkn,  ft F,lt Z,zz *Status); 

       short   PolarPoint_x
                         (PolarPoint   *This);

       short   PolarPoint_y    
                         (PolarPoint   *This);

       double  PolarPoint_Theta 
                         (PolarPoint   *This);  

       double  PolarPoint_Radius 
                         (PolarPoint   *This);

       void    PolarPoint_show
                         (PolarPoint   *This);

       boolean PolarPoint_delete
                         (PolarPoint  **This,      deletetype    DbEvent,
                          ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF POINT.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
