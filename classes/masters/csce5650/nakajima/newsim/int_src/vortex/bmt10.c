/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ******************************  Edb0.C   *****************************  */
/* *  Entry to the BMT test module.                                        */
/* *                               Recipies in the Developers CookBook     */
/* *                                                                       */
/* *         The BMT (Engineering Database Benchmark) Test Module          */
/* *                                                                       */
/* *********************************************************************** */

#define  __DISPLAY_TIME__

/* BMT_C.c */
#define BMT_C
#ifdef __DISPLAY_TIME__
#include <time.h>
#endif /* __DISPLAY_TIME__ */
#include "bmt01.h"
#include "files.h"
#include "grp.h"
#include "vchunk.h"
#include "rects.h"
#include "emplib.h"
#include "empobj.h"
#include "list01.h"

extern numtype TestObj_Class;
extern boolean    OaDumpObjHdr (int TraceSwi,  tokentype     *Anchor,
                                num ObjNum,    ft F,lt Z,zz *Status);


typedef Part2  Part;
extern boolean       DeBugSet;

/*+-----------------------------------------------------------------------+
~C                       BMT MACRO METHODS                                !
  +-----------------------------------------------------------------------+*/
boolean BMT_LookUpParts    (longtype      LookUps,  ft F,lt Z,zz *Status)
{
tokentype      PartTkn      = NullToken;
unsigned int   i            = 0;
#ifdef __DISPLAY_TIME__
time_t        NullTime     = 0;
time_t        Elapsed      = 0;
float          Seconds      = 0;
#endif /* __DISPLAY_TIME__ */
partidtype     FromPart     = 0;
boolean        TempClassBug = ClassBug;
boolean        TempDbmBug   = DbmBug;

   PartTkn.DbId          = LibTkn.DbId;

   LookUp_Asserts        = 0;
   Part_Asserts          = 0;
   DrawObj_Asserts       = 0;
   NamedObj_Asserts      = 0;
   Person_Asserts        = 0;
   TestObj_Asserts       = 0;

   LookUp_NullAsserts    = 0;
   Part_NullAsserts      = 0;
   DrawObj_NullAsserts   = 0;
   NamedObj_NullAsserts  = 0;
   Person_NullAsserts    = 0;
   TestObj_NullAsserts   = 0;


   sprintf  (Msg, "\n LOOK UP %6u Random Parts and Export each Part.\n",
             LookUps);
   TraceMsg (0,   Msg);

#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif /* __DISPLAY_TIME__ */

   i = 0;
   while (Normal(*Status)
   &&     ++i   <= LookUps)
   {
      FromPart          = Random(NumVlists);
      FromPart          = Vlists[FromPart];

      if (Part_GetToken  (&LibTkn,                 FromPart,
                          McStat,                 &PartTkn))
      if (PartTkn.Handle)
          BMT_Validate   (&PartTkn,                McStat);

      if (ClassBug          || OaBug
      ||  PartTkn.Handle    == 0
      ||  i % 100           == 0)
      if (sprintf (Msg, " Set %6u. Part# %8u Handle= %8u.\n",
                   i, FromPart,  PartTkn.Handle))
      if (TraceMsg (0, Msg))

      if (DrawBug)
      if (ClassBug || i == 100)
      if (PartTkn.Handle)
         Object_Dump    (&PartTkn);
   }

   ClassBug   = TempClassBug;
   DbmBug     = TempDbmBug;

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n LookUp Time = %4.2f sec; for %6u parts; Asserts =%6lu\n",
             Seconds, i, LookUp_Asserts);
   TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */
   sprintf  (Msg, "\n LookUp for %6u parts; Asserts =%6lu\n",
                   i, LookUp_Asserts);
   TraceMsg (0, Msg);

   sprintf  (Msg,
           "       <Part2    >  Asserts =%6lu; NULL Asserts =%6lu.\n",
             Part_Asserts, Part_NullAsserts);
   TraceMsg (0, Msg);
   sprintf  (Msg,
           "       <DrawObj  >  Asserts =%6lu; NULL Asserts =%6lu.\n",
             DrawObj_Asserts, DrawObj_NullAsserts);
   TraceMsg (0, Msg);
   sprintf  (Msg,
           "       <NamedObj >  Asserts =%6lu; NULL Asserts =%6lu.\n",
             NamedObj_Asserts, NamedObj_NullAsserts);
   TraceMsg (0, Msg);
   sprintf  (Msg,
           "       <Person   >  Asserts =%6lu; NULL Asserts =%6lu.\n",
             Person_Asserts, Person_NullAsserts);
   TraceMsg (0, Msg);
   sprintf  (Msg,
           "       <TestObj  >  Asserts =%6lu; NULL Asserts =%6lu.\n",
             TestObj_Asserts, TestObj_NullAsserts);
   TraceMsg (0, Msg);

   LookUp_Asserts        = 0;
   Part_Asserts          = 0;
   DrawObj_Asserts       = 0;
   NamedObj_Asserts      = 0;
   Person_Asserts        = 0;
   TestObj_Asserts       = 0;

   LookUp_NullAsserts    = 0;
   Part_NullAsserts      = 0;
   DrawObj_NullAsserts   = 0;
   NamedObj_NullAsserts  = 0;
   Person_NullAsserts    = 0;
   TestObj_NullAsserts   = 0;

TRACK(TrackBak,"BMT_LookUpParts\n");
return (STAT);
}


boolean BMT_CommitParts    (boolean       Release, ft F,lt Z,zz *Status)
{
tokentype        PartTkn        = NullToken;
numtype          PartId         = 0;
int              i              = 0;
#ifdef __DISPLAY_TIME__
time_t           NullTime       = 0;
time_t           Elapsed        = 0;
float            Seconds        = 0;
#endif /* __DISPLAY_TIME__ */
tokentype        FOIdTkn        = NullToken;
boolean          TempClassBug   = ClassBug;
tokentype        PersonTkn      = NullToken;
static boolean   FirstTime      = True;
int              j              = 0;
tokentype        CurrentTkn     = NullToken;
tokentype        ItorTkn        = NullToken;

   Parts          = 0;
   DrawObjs       = 0;
   NamedObjs      = 0;
   Points         = 0;
   Persons        = 0;

   PartTkn.DbId        = LibTkn.DbId;
   sprintf (Msg, "\n COMMIT All Image copies:: Release=<%s>; Max Parts=%4u\n",
            BoolStr[Release], MaxParts);
   TraceMsg (0,   Msg);

#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif /* __DISPLAY_TIME__ */

   if (FirstTime)
   {
       FirstTime = False;
   }

   i           = -1;
   while (Normal(*Status)
   &&    ++i   < NumVlists)
   {
      PartId                 = Vlists[i];
      if (Part_GetToken        (&LibTkn,               PartId,
                                McStat,               &PartTkn))
      if (Env_IsValidToken     (&PartTkn,              McStat))
      {
         if (ClassBug)
         if (sprintf (Msg, "  Commit part= %u\n", PartId))
            TraceMsg (0,   Msg);

         if (ExportGeneric)
            Part_ImportFrom    (&ApiTestTkn,  (void *)&FOIdTkn,
                                &PartTkn,              McStat);
         else if (ExportParts)
            Part_ExportTo      (&PartTkn,            &ApiTestTkn,
                                (void *)&FOIdTkn,     McStat);
         if (ClassBug)
            Object_Dump        (&PartTkn);

/*   Get the Tuple Pair from Part to Draw object                           */

         if (Normal(*Status))
         if (Object_ImageIsActive       (&PartTkn,     McStat))
            BMT_CommitPartDrawObj       (&PartTkn,     Release,    McStat);

         if (Normal(*Status))
         if (PersonParts_OwnerOf        (&PartTkn,     McStat,    &PersonTkn))
         if (Env_IsValidToken           (&PersonTkn,   McStat))
         if (Object_ImageIsActive       (&PersonTkn,   McStat))
         {
            Object_CommitImage          (&PersonTkn,   Release,    McStat);
            Persons++;
         }

         if (Normal(*Status))
         if (Object_ImageIsActive       (&PartTkn,     McStat))
         {
            Object_CommitImage          (&PartTkn,     Release,    McStat);
            Parts++;
         }
      }
      ClassBug   = TempClassBug;
   }

   sprintf (Msg,  " <%6u> Part            images'  Committed.\n", Parts);
   TraceMsg (0, Msg);
   sprintf (Msg,  " <%6u> DrawObj         images'  Committed.\n", DrawObjs);
   sprintf (Msg,  "                 <%6u> are Named.\n", NamedObjs);
   TraceMsg (0, Msg);
   sprintf (Msg,  " <%6u> Point           images'  Committed.\n", Points);
   TraceMsg (0, Msg);
   sprintf (Msg,  " <%6u> Person          images'  Committed.\n", Persons);
   TraceMsg (0, Msg);

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n COMMIT Parts(* %8d)::  Time = %4.2f sec;\n",
             i, Seconds);
   TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */

   sprintf  (Msg, "\n COMMIT Parts(* %8d)\n", i);
   TraceMsg (0, Msg);

   if (Normal(*Status))
   {
     TraceMsg (0, "\n Commit TestObj_Class        in <Primal> DB.\n");
    *Status  = Iterate_Begin;
     j = 0;
     do
     {
        if (Primal_IterateOnClassObjects (&PrimalTkn,  TestObj_Class,
                                          McStat,     &ItorTkn,  &CurrentTkn))
        if (*Status  != Iterate_EndOf)
        if (Object_ImageIsActive         (&CurrentTkn, McStat))
            Object_CommitImage           (&CurrentTkn, Release,   McStat);

        if (j == 0 || j % QueBug == 0)
        {
           sprintf (Msg,
                    " ItNum %6u. Token[%3u:%8u]. TestObj        Committed.\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle);
           TraceMsg (0, Msg);
        }
        j++;
        ClassBug   = TempClassBug;

     } while (Normal(*Status));
     if (*Status  == Iterate_EndOf)
         *Status   = Env_Normal;

     sprintf (Msg,  " <%6u> TestObj         images'  Committed.\n", j-1);
     TraceMsg (0, Msg);
   }

   if (Normal(*Status))
   {
     TraceMsg (0, "\n Commit CartesianPoint_Class in <Primal> DB.\n");
    *Status  = Iterate_Begin;
     j = 0;
     do
     {
        if (Primal_IterateOnClassObjects (&PrimalTkn,  CartesianPoint_Class,
                                          McStat,     &ItorTkn,  &CurrentTkn))
        if (*Status  != Iterate_EndOf)
        if (Object_ImageIsActive         (&CurrentTkn, McStat))
           Object_CommitImage            (&CurrentTkn, Release,   McStat);

        if (j == 0 || j % QueBug == 0)
        {
           sprintf (Msg,
                    " ItNum %6u. Token[%3u:%8u]. CartesianPoint Committed.\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle);
           TraceMsg (0, Msg);
        }
        j++;

     } while (Normal(*Status));
     if (*Status  == Iterate_EndOf)
         *Status   = Env_Normal;

     sprintf (Msg,  " <%6u> CartesianPoint  images'  Committed.\n", j-1);
     TraceMsg (0, Msg);
   }

   ClassBug   = TempClassBug;

TRACK(TrackBak,"BMT_CommitParts\n");
return (STAT);
}


boolean BMT_DeleteParts    (longtype      Deletes,  ft F,lt Z,zz *Status)
{
tokentype      PartTkn      = NullToken;
unsigned int   i            = 0;
#ifdef __DISPLAY_TIME__
time_t         NullTime     = 0;
time_t         Elapsed      = 0;
float          Seconds      = 0;
#endif /* __DISPLAY_TIME__ */
partidtype     FromPart     = 0;
boolean        TempClassBug = ClassBug;
static boolean FirstTime    = True;

   DeleteDrawObjects = True;

   PartTkn.DbId          = LibTkn.DbId;

   sprintf  (Msg, "\n DELETE  %6u Random Parts.\n",
             Deletes);
   TraceMsg (0,   Msg);

#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif /* __DISPLAY_TIME__ */

   if (Deletes >= NumVlists)
       Deletes  = NumVlists / 2;

   i = 0;
   while (Normal(*Status)
   &&     ++i   <= Deletes)
   {

      FromPart          = Random(NumVlists);
      FromPart          = Vlists[FromPart];

      if (Part_GetToken         (&LibTkn,             FromPart,
                                 McStat,             &PartTkn))
      if (ClassBug
      ||  PartTkn.Handle    == 0
      ||  i % QueBug        == 0)
      if (sprintf (Msg, " Delete %6u. Part# %8u Handle= %8u.\n",
                   i, FromPart,  PartTkn.Handle))
         TraceMsg (0, Msg);

      if (Normal(*Status))
      if (Env_IsValidToken      (&PartTkn,            McStat))

      if (BMT_DeletePartDrawObj (&PartTkn,            McStat))
          Part_Delete           (&PartTkn,            McStat);
   ClassBug   = TempClassBug;
   }

   FirstTime  = False;

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n Delete Time = %4.2f sec; for %6u parts;\n", Seconds, i);
   TraceMsg(MSGFILE,Msg);
#else
#endif /* __DISPLAY_TIME__ */
   sprintf  (Msg, "\n Delete for %6u parts;\n", i);
   TraceMsg (0, Msg);

TRACK(TrackBak,"BMT_DeleteParts\n");
return (STAT);
}


boolean BMT_DeleteAllObjects
                         (ft F,lt Z,zz *Status)
{
bytessize        BlkSize1       = NumVlists * sizeof (int);
int             *FreeParts      = NullPtr;
tokentype        PartTkn        = NullToken;
numtype          PartId         = 0;
tokentype        ToTkn          = NullToken;
int              i              = 0;
#ifdef __DISPLAY_TIME__
time_t           NullTime       = 0;
time_t           Elapsed        = 0;
float            Seconds        = 0;
#endif /* __DISPLAY_TIME__ */
boolean          TempClassBug   = ClassBug;
static boolean   FirstTime      = True;
boolean          TempDeleteDraws= DeleteDrawObjects;

/*
   DeBugSet  = True;
   OaDumpObjHdr (1, &LibTkn,  212, Dump_Status);
*/

   if (DeletePartObjects == False)
      return (STAT);

   DeleteDrawObjects = True;

   Parts          = 0;
   DrawObjs       = 0;
   NamedObjs      = 0;
   Points         = 0;
   Persons        = 0;
   ToTkn.DbId     = LibTkn.DbId;
   PartTkn.DbId   = LibTkn.DbId;

   sprintf (Msg, "\n DELETE All objects; Max Parts=%4u\n",
            MaxParts);
   TraceMsg (0,   Msg);
   TraceMsg (0,
   "    <Part>, <*DrawObj>, <*Point>, <Person>, <Address> && <TestObj>;\n");
#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif /* __DISPLAY_TIME__ */

   if (FirstTime)
   {
       ClassBug  = False;
   }

   if (Core0_MoreCore     (BlkSize1,    McStat,   (addrtype *)&FreeParts))
   for (i       = -1;  ++i < NumVlists; )
   {
        FreeParts[i]       = Vlists[i];
   }

   i            = NumVlists;
   while (Normal(*Status)
   &&   --i    >= 0)
   {
      PartId                  = FreeParts[i];
      if (ClassBug || i % QueBug == 0)
      if (sprintf (Msg, "  Delete %6d. PartId =%8u.\n",
                   i, PartId))
          TraceMsg (0,   Msg);

      if (Part_GetToken        (&LibTkn,               PartId,
                                McStat,               &PartTkn))
      if (Env_IsValidToken     (&PartTkn,              McStat))
      {
         if (ClassBug || i % QueBug == 0)
         if (sprintf (Msg, "  Delete %6d. object[%4u:%8u]\n",
                      i, PartTkn.DbId, PartTkn.Handle))
            TraceMsg (0,   Msg);

         if (ClassBug)
            Object_Dump        (&PartTkn);

         if (BMT_DeletePartDrawObj      (&PartTkn,       McStat))

         if (Normal(*Status))
             Part_Delete                (&PartTkn,       McStat);

         Parts++;
      }
      ClassBug   = TempClassBug;
      FirstTime  = False;
   }

   i            = MaxParts + 1;
   while (Normal(*Status)
   &&   --i    >= 0)
   {
      PartId           = i;
      if (ClassBug || i % QueBug == 0)
      if (sprintf (Msg, "  Delete %6d. Entry[PartId :=%8u].\n",
                   i, PartId))
          TraceMsg (0,   Msg);
      Grp_DeleteEntry   (&PartTkn,  VpartsDir,  PartId,        McStat);
   }
   if (Normal(*Status))
      Grp_PutEntry      (&LibTkn,   VpartsDir,  0, (addrtype )&NullToken,
                         McStat);
   MaxParts     = 0;
   CurrentId    = 0;
   StartCommit  = 0;
   NumVlists    = 0;

   if (Normal(*Status))
   if (Core0_FreeCoreSpace 
                        ((addrtype *)&FreeParts,     BlkSize1,  McStat))
      VchunkPutStackPtr (&VlistTkn,                  0, 
                         McStat,        (addrtype *)&Vlists);

   sprintf (Msg,  " <%6u> Part            objects  Deleted.\n", Parts);
   TraceMsg (0, Msg);
   sprintf (Msg,  " <%6u> DrawObj         objects  Deleted.\n", DrawObjs);
   TraceMsg (0, Msg);
   sprintf (Msg,  "                 <%6u> are Named.\n", NamedObjs);
   TraceMsg (0, Msg);
   sprintf (Msg,  " <%6u> Point           objects  Deleted.\n", Points);
   TraceMsg (0, Msg);

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n DELETE Objects Time = %4.2f sec;\n",
             Seconds);
   TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */

   sprintf  (Msg, "\n DELETE Objects ...\n");
   TraceMsg (0, Msg);

   if (Normal(*Status))
     BMT_DeleteTestObjs   (McStat);

   ClassBug   = TempClassBug;

   DeleteDrawObjects= TempDeleteDraws;

TRACK(TrackBak,"BMT_DeleteAllObjects\n");
return (STAT);
}


boolean BMT_DeleteTestObjs
                         (ft F,lt Z,zz *Status)
{
#ifdef __DISPLAY_TIME__
time_t           NullTime       = 0;
time_t           Elapsed        = 0;
float            Seconds        = 0;
#endif /* __DISPLAY_TIME__ */
int              j              = 0;
tokentype        CurrentTkn     = NullToken;
tokentype        ItorTkn        = NullToken;
static boolean   FirstTime      = True;
boolean          TempClassBug   = ClassBug;

  TraceMsg (0, "\n DELETE All TestObj objects;\n");

#ifndef __BMT01__
  FirstTime = False;
#endif

#ifdef __DISPLAY_TIME__
  NullTime = clock();
#endif /* __DISPLAY_TIME__ */

  if (Normal(*Status))
  {
     TraceMsg (0, "\n Delete TestObj_Class        in <Primal> DB.\n");
    *Status  = Iterate_Begin;
     j = 0;
     do
     {
        if (Primal_IterateOnClassObjects (&PrimalTkn,  TestObj_Class,
                                          McStat,     &ItorTkn,  &CurrentTkn))
        {
           if (FirstTime  || CurrentTkn.Handle == 0)
           {
              FirstTime = False;
           }
           if (ClassBug)
              Object_Dump  (&CurrentTkn);
           Object_Delete                 (&CurrentTkn, McStat);
        }

        if (*Status  != Iterate_EndOf)
        if (j == 0 || j % QueBug == 0  || !Normal(*Status))
        {
           sprintf (Msg,
                    " ItNum %6u. Token[%3u:%8u]. TestObj        Deleted.\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle);
           TraceMsg (0, Msg);
        }
        j++;
        ClassBug = TempClassBug;

     } while (Normal(*Status));
     if (*Status  == Iterate_EndOf)
         *Status   = Env_Normal;

     sprintf (Msg,  " <%6u> TestObj         objects  Deleted.\n", j-1);
     TraceMsg (0, Msg);
  }

  if (Normal(*Status))
  {
     TraceMsg (0, "\n Commit CartesianPoint_Class in <Primal> DB.\n");
    *Status  = Iterate_Begin;
     j = 0;
     do
     {
        if (Primal_IterateOnClassObjects (&PrimalTkn,  CartesianPoint_Class,
                                          McStat,     &ItorTkn,  &CurrentTkn))
           Object_Delete                 (&CurrentTkn, McStat);

        if (*Status  != Iterate_EndOf)
        if (j == 0 || j % QueBug == 0)
        {
           sprintf (Msg,
                    " ItNum %6u. Token[%3u:%8u]. CartesianPoint Deleted.\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle);
           TraceMsg (0, Msg);
        }
        j++;

     } while (Normal(*Status));
     if (*Status  == Iterate_EndOf)
         *Status   = Env_Normal;

     sprintf (Msg,  " <%6u> CartesianPoint  objects  Deleted.\n", j-1);
     TraceMsg (0, Msg);
  }

#ifdef __DISPLAY_TIME__
  Elapsed  = clock() - NullTime;
  Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
  sprintf  (Msg,
           "\n DELETE TestObj and Point objects... Time = %4.2f sec\n",
            Seconds);
  TraceMsg (MSGFILE,   Msg);
#else
#endif /* __DISPLAY_TIME__ */

  sprintf  (Msg, "\n DELETE TestObj and Point objects... \n");
  TraceMsg (0,   Msg);

TRACK(TrackBak,"BMT_DeleteTestObjs\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF BMT10.C                                 !
  +-----------------------------------------------------------------------+*/

