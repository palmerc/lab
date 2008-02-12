/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#define  __DISPLAY_TIME__
/* BMT01.c */
#define BMT01

#include <time.h>
#include "bmt01.h"
#include "files.h"
#include "rects.h"
#include "emplib.h"
#include "empobj.h"
#include "list01.h"

typedef Part2  Part;

       boolean Draw7     (tokentype    *Anchor,  boolean       TestBug,
                          numtype       Option,  ft F,lt Z,zz *Status);

tokentype        FirstRect  = Null_Token;
/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/
boolean BMT_CreateParts    (longtype      Count,  ft F,lt Z,zz *Status)
{
tokentype         PersonTkn      = NullToken;
tokentype         PartTkn        = NullToken;
unsigned int      id             = 0;
unsigned int      i              = 0;
#ifdef __DISPLAY_TIME__
time_t            NullTime       = 0;
time_t            Elapsed        = 0;
float             Seconds        = 0;
#endif /* __DISPLAY_TIME__ */
longtype          X              = 0;
longtype          Y              = 0;
longtype          TypeNum        = 0;
numtype           DrawType       = 0;
numtype           PersonId       = 0;
short             x0             = 0;
short             y0             = 0;
short             x1             = 0;
short             y1             = 0;
CartesianPoint   *c3             = NullPtr;
CartesianPoint   *c4             = NullPtr;
Rectangle        *r0             = NullPtr;
tokentype        RecTkn          = NullToken;
XyPoint          c8;
XyPoint          c9;
IntChunkRect    *IntChunk01      = NullPtr;
XyRect          *Xy01            = NullPtr;
NamedXyRect     *Xy02            = NullPtr;
ArrayRect       *Array01         = NullPtr;
DblPtrRect      *DblPtr01        = NullPtr;
VarrayRect      *Varray01        = NullPtr;
VchunkRect      *Vchunk01        = NullPtr;
RefRect         *ref0            = NullPtr;
DrawObj         *BaseObj         = NullPtr;
Part            *PartObj         = NullPtr;
static boolean   FirstTime       = True;

   PartTkn.DbId        = LibTkn.DbId;

   sprintf (Msg, "\n Create %u New Parts\n", Count);
   TraceMsg (0,   Msg);

#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif

   i           = 0;
   id          = CurrentId;
   while (Normal(*Status)
   &&     ++i <= Count)
   {
      X        = Random(512);
      Y        = Random(1024);
      TypeNum  = Random(8);

      /*  Rectangle type                                                  */
      DrawType = Random(8);

      /*  Origin                                                          */
      x0       = (short )X;
      y0       = (short )Y;
      /*  Extent                                                          */
      x1       = x0 + (short )Random(16) + 1;
      y1       = y0 + (short )Random(16) + 1;

      c8.xCoord = x0;
      c8.yCoord = y0;
      c9.xCoord = x1;
      c9.yCoord = y1;

      /*  Owner of Part                                                   */
      PersonId = Random(PersonLib01->MaxId) + 1;
      if (PersonObjs_FindIn (&EmpTkn010,  (addrtype )&PersonId,
                             McStat,                 &PersonTkn))
      {
         Part_Create        (&LibTkn,     ++CurrentId,  X,  Y,  TypeNum,
                             McStat,                           &PartTkn);

         if (ClassBug || DeBug || i % QueBug == 1)
         if (sprintf (Msg, " Create Part %6u. Token[%3u:%8u].\n",
                      CurrentId, PartTkn.DbId, PartTkn.Handle))
         if (TraceMsg (0, Msg))
         if (i == Count)
         if (DrawBug)
            Object_Dump (&PartTkn);

         if (Normal(*Status))
         if (Object_GetImage     (&PartTkn,     McStat,   (addrtype *)&PartObj))
         if (PersonParts_AddInto (&PersonTkn,  &PartTkn,  McStat))
         switch (DrawType)
         {
           case   aRectangle    :
                if ((c3 = CartesianPoint_new0
                                 (&LibTkn010,  x0,         y0,
                                               McStat)) != NullPtr)
                if ((c4 = CartesianPoint_new0
                                 (&LibTkn010,  x1,         y1,
                                               McStat)) != NullPtr)
                if ((r0 = Rectangle_new1
                                 (&LibTkn010, (Point *)c3,(Point *)c4,
                                               McStat)) != NullPtr)
                    RecTkn      = r0->Token;

                BaseObj         = (DrawObj *)r0;

                if (Normal(*Status))
                   List01_Append     (&RectListTkn,  &RecTkn,  McStat);

               break;

           case   aXyRect       :
               if ((Xy01  = XyRect_new0 (&LibTkn010, &c8,  &c9,
                                               McStat)) != NullPtr)
                  RecTkn = Xy01->Token;
               BaseObj         = (DrawObj *)Xy01;
               break;

           case   aNamedXyRect  :
               if (FirstTime)
               {
                  FirstRect   = RecTkn;
                  FirstTime   = False;

                  if (DrawBug)
                  if (TraceMsg (0, "\n First <NamedXyRect>::\n"))
                     Object_Dump (&RecTkn);

               }
               if ((Xy02  = NamedXyRect_new0 (&LibTkn010, &c8,  &c9,
                                              McStat)) != NullPtr)
                  RecTkn = Xy02->Token;
               BaseObj         = (DrawObj *)Xy02;
               break;

           case   aArrayRect    :
               if ((Array01  = ArrayRect_new0 (&LibTkn010, &c8,  &c9,
                                               McStat)) != NullPtr)
                  RecTkn = Array01->Token;
               BaseObj         = (DrawObj *)Array01;
               break;

           case   aDblPtrRect   :
               if ((DblPtr01 = DblPtrRect_new0 (&LibTkn010, &c8,  &c9,
                                                McStat)) != NullPtr)
                  RecTkn = DblPtr01->Token;
               BaseObj         = (DrawObj *)DblPtr01;
               break;

           case   aVarrayRect   :
               if ((Varray01  = VarrayRect_new0 (&LibTkn010, &c8,  &c9,
                                               McStat)) != NullPtr)
                  RecTkn = Varray01->Token;
               BaseObj         = (DrawObj *)Varray01;
                if (FirstTime)
                {
                    FirstRect   = RecTkn;
                    FirstTime   = False;

                    if (DrawBug)
                    if (TraceMsg (0, "\n First RECTANGLE::\n"))
                       Object_Dump (&RecTkn);

                }
               break;

           case   aIntChunkRect :
               if ((IntChunk01  = IntChunkRect_new0 (&LibTkn010, &c8,  &c9,
                                               McStat)) != NullPtr)
                  RecTkn = IntChunk01->Token;
               BaseObj         = (DrawObj *)IntChunk01;
               break;

           case   aVchunkRect   :
               if ((Vchunk01  = VchunkRect_new0 (&LibTkn010, &c8,  &c9,
                                               McStat)) != NullPtr)
                  RecTkn = Vchunk01->Token;
               BaseObj         = (DrawObj *)Vchunk01;
               break;

           case   aRefRect      :
                if ((c3 = CartesianPoint_new0
                                 (&LibTkn010,  x0,         y0,
                                               McStat)) != NullPtr)
                if ((c4 = CartesianPoint_new0
                                 (&LibTkn010,  x1,         y1,
                                               McStat)) != NullPtr)
                if ((ref0 = RefRect_new1
                                 (&LibTkn010, (Point *)c3,(Point *)c4,
                                               McStat)) != NullPtr)
                    RecTkn      = ref0->Token;

               BaseObj         = (DrawObj *)ref0;
               break;

           default              :
              *Status  = Err_BadDesign;
               BaseObj         = NullPtr;
               break;
         }

         if (Normal(*Status))
         {
            BaseObj->PartObjSlotAttr  = PartTkn;
            PartObj->DrawObjSlotAttr  = RecTkn;
         }
/*
//         Tuple_Pair  (DrawPart_Tupl,   PartObj_Slot,  &RecTkn,
//                                       DrawObj_Slot,  &PartTkn,  McStat);
*/

      } else if (*Status == Set_NotFound)  {
        *Status  = Env_Normal;
         sprintf (Msg,
                  " Entry %6u. PersonsObjs for Key: Id = <%6u>. NOT FOUND\n",
                  i, PersonId);
         TraceMsg (0, Msg);
         i--;
      }
   }

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n Create Time = %4.2f sec, for %d Parts. CurrentId=%6u\n",
             Seconds,  Count, CurrentId);
   TraceMsg (MSGFILE, Msg);
#else
#endif

   sprintf  (Msg,
             "\n  <%6d> Parts Created. CurrentId=%6u\n",
             Count, CurrentId);
   TraceMsg (0, Msg);
   TraceMsg (0, "\n Connect each instantiated Part TO 3 unique Parts\n");

#ifdef __DISPLAY_TIME__
   NullTime = clock();
#endif /* __DISPLAY_TIME__  */

   MaxParts    = CurrentId;
   OneCent     = MaxParts / 100;
   if (OneCent == 0)
       OneCent = 1;

   HalfCent     = MaxParts / 200;
   if (HalfCent == 0)
       HalfCent = 1;

   if (Normal(*Status))
      Vchunk_GetAddress   (&VlistTkn,   McStat,   (addrtype *)&Vlists);

   while (Normal(*Status)
   &&     ++id <= MaxParts)
   {
      Part_GetToken     (&LibTkn,                 id,
                         McStat,                 &PartTkn);

      if (ClassBug || DeBug || id % QueBug == 1)
      if (sprintf (Msg, " Connect Part %6u. Token[%3u:%8u]\n",
                    id, PartTkn.DbId, PartTkn.Handle))
         TraceMsg (0, Msg);

      if (Normal(*Status))
          Part_Connect  (&PartTkn,    id,         McStat);

      if (id == 100)
      if (DrawBug)
         Object_Dump    (&PartTkn);
   }

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - NullTime;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg,
             "\n Connect Time = %4.2f sec\n",
             Seconds);
   TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__  */

TRACK(TrackBak,"BMT_CreateParts\n");
return (STAT);
}


boolean      BMT_Validate
                         (tokentype    *PartTkn, ft F,lt Z,zz *Status)
{
Part            *PartObj        = NullPtr;
Part             PartObj0;
tokentype        ToTkn          = NullToken;

boolean          TempClassBug   = ClassBug;
boolean          TempMtrxBug    = MtrxBug;
boolean          TempSetBug     = SetBug;

tokentype        FoundTkn       = NullToken;
tokentype        PersonTkn      = NullToken;
Person          *EmpObj         = NullPtr;
tokentype        AddressTkn     = NullToken;
Address         *AddressObj     = NullPtr;
tokentype        c3Tkn          = NullToken;
tokentype        c4Tkn          = NullToken;
Rectangle       *r0             = NullPtr;
tokentype        RectTkn        = NullToken;
IntChunkRect    *IntChunk01     = NullPtr;
XyRect          *Xy01           = NullPtr;
NamedXyRect     *Xy02           = NullPtr;
ArrayRect       *Array01        = NullPtr;
DblPtrRect      *DblPtr01       = NullPtr;
VarrayRect      *Varray01       = NullPtr;
VchunkRect      *Vchunk01       = NullPtr;
RefRect         *ref0           = NullPtr;
DrawObj         *BaseObj        = NullPtr;
numtype          DrawClass      = 0;
tokentype        NextNode       = NullToken;
tokentype        NextTkn        = NullToken;
tokentype        CurrentNode    = NullToken;
tokentype        CurrentTkn     = NullToken;
boolean          NotFound       = True;
#ifdef     __INCLUDE_MTRX__
matrixitor       Itrator;
#endif /* __INCLUDE_MTRX__ */
static numtype   NumExports1    = 0;
boolean          TempDrawBug    = DrawBug;

  NextNode.DbId    = LibTkn010.DbId;
  NextTkn.DbId     = LibTkn010.DbId;

  CurrentNode.DbId = LibTkn010.DbId;
  CurrentTkn.DbId  = LibTkn010.DbId;

++NumExports1;
  if (DrawBug && NumExports1 % QueBug == 0)
  if (sprintf (Msg, " BMT_Validate %8u.  Token[%3u:%8u]\n",
               NumExports1, PartTkn->DbId, PartTkn->Handle))
      TraceMsg (0, Msg);

/* DO Basic Look at the PART object                                        */
  if (Normal(*Status))
  if (Object_GetImage (PartTkn,      McStat, (addrtype *)&PartObj))
  {
     strcpy (PartObj0.type,  PartObj->type);
     PartObj0.xx           = PartObj->xx;
     PartObj0.yy           = PartObj->yy;
     PartObj0.build        = PartObj->xx;
     PartObj0.link[0]      = PartObj->link[0];
     PartObj0.link[1]      = PartObj->link[1];
     PartObj0.link[2]      = PartObj->link[2];

     ToTkn.DbId            = PartTkn->DbId;


#ifdef     __INCLUDE_MTRX__

/*   Validate the Matrix Association of the TosFroms                       */
     for (ItNum = -1; ++ItNum < 3 && Normal(*Status); )
     {
        ToPart             = PartObj->link[ItNum].to;

        if (Grp_GetEntry     (PartTkn,  VpartsDir,  ToPart,
                              McStat,  (addrtype *)&PartToken))
           ToTkn.Handle    =  PartToken.Handle;

        if (Normal(*Status))
        if (Env_IsValidToken (&ToTkn,                  McStat))
           Matrix_WhosAt (TosFroms_Mtrx,     PartTkn, &ToTkn,
                          McStat,           &Itrator, &NodeTkn);
        if (*Status == Mtrx_NotMated)
        {
#ifdef __DISPLAY_ERRORS__
           sprintf (Msg,
         "\n ***ERROR*** From[%4u:%6u] To[%4u:%6u] not MATE'D in TosFroms.\n",
                    PartTkn->DbId,   PartTkn->Handle,
                    ToTkn.DbId,      ToTkn.Handle);
           TraceMsg (0, Msg);
           TraceMsg (0, "\n TO   Token::\n");
           Object_Dump  (PartTkn);
           TraceMsg (0, "\n FROM Token::\n");
           Object_Dump  (&ToTkn);
#endif /* __DISPLAY_ERRORS__ */
          *Status   = Env_Normal;
        }
     }
#endif /* __INCLUDE_MTRX__ */

  }

/* DO a DataBase Transaction on a ubiquous TestObj object                  */
  if (Normal(*Status))
      BMT_DbTransaction  (McStat);


/* DO Basic Operations on Part-DrawObj relationships                       */
  if (Normal(*Status))
  {
/*   Get the Tuple Pair from Part to Draw object                           */

     RectTkn   = PartObj->DrawObjSlotAttr;

     if (Env_IsValidToken (&RectTkn,   McStat))
     if ((DrawClass = Object_IsA (&RectTkn,  McStat)) > 0)
     if (DrawClass         == Rectangle_Class)
     {
        if ((r0  = Rectangle_new4   (&RectTkn,     McStat)) != NullPtr)
        {
            FoundTkn = r0->Token;
            c3Tkn    = r0->Origin->Token;
            c4Tkn    = r0->Extent->Token;
            if (ClassBug)
               Rectangle_show  (r0);
        }

/*      Find The Rectangle in the 'DbRectangles' List (by handle)          */
        if (Normal(*Status))
        if (List01_Reset     (&RectListTkn,  McStat))
        if (List01_FirstIn   (&RectListTkn,  McStat,
                              &CurrentNode, &CurrentTkn))
        if (Normal(*Status))
        {
           do
           {
              if (CurrentTkn.DbId    == RectTkn.DbId
              &&  CurrentTkn.Handle  == RectTkn.Handle)
                 NotFound   = False;
              else {
                 if (List01_NextIn    (&RectListTkn, &CurrentNode,
                                    McStat,       &NextNode,    &NextTkn))
                 {
                    CurrentNode.Handle = NextNode.Handle;
                    CurrentTkn.Handle  = NextTkn.Handle;
                 }
              }

           } while (Normal(*Status) && NotFound);
        }
        if (*Status == List01_EndOfList  || *Status == List01_EmptyList)
       *Status = Env_Normal;

        if (NotFound
        || !Normal(*Status))
        {
#ifdef __DISPLAY_ERRORS__
           sprintf (Msg,
            "\n ***ERROR*** Rectangle[%4u:%6u] not found in LIST. Id=%6u\n",
                    RectTkn.DbId,   RectTkn.Handle, r0->Id);

           TraceMsg (0, Msg);
           Object_Dump  (&RectTkn);
#endif /* __DISPLAY_ERRORS__ */
           if (Normal(*Status))
              *Status  = Err_BadDesign;
        }


/*      Find the object in the 'LibRectangles' Set (by DrawId)  (Btree)    */
        if (Normal(*Status))
           LibRectangles_FindIn  (&LibTkn010,  (addrtype )&r0->Id,
                                  McStat,                 &FoundTkn);
        if (*Status  == Set_NotFound)
        {
#ifdef __DISPLAY_ERRORS__
            sprintf (Msg,
    "\n ***ERROR*** Rectangle[%4u:%6u] not found in LibRectangles. Id=%6u\n",
                     RectTkn.DbId,   RectTkn.Handle, r0->Id);

            TraceMsg (0, Msg);
            Object_Dump  (&RectTkn);
#endif /* __DISPLAY_ERRORS__ */
           *Status  = Err_BadDesign;
        }


     } else if (DrawClass  == XyRect_Class)       {

        if ((Xy01  = XyRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = Xy01->Token;

     } else if (DrawClass  == NamedXyRect_Class)  {

/*      Find the object in the 'DrawObjs' Set (by Nam as a char*) (Btree)   */

        if ((Xy02  = NamedXyRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = Xy02->Token;

        NamedDrawObjs_FindIn  (&LibTkn010,  (addrtype )Xy02->Nam,
                               McStat,                &FoundTkn);
        if (*Status  == Set_NotFound)
        {
#ifdef __DISPLAY_ERRORS__
            sprintf (Msg,
 "\n ***ERROR*** NamedXyRect[%4u:%6u] not found in NamedDrawObjs. Nam= <%s>\n",
                     RectTkn.DbId,   RectTkn.Handle, Xy02->Nam);
            TraceMsg (0, Msg);

            Object_Dump  (&RectTkn);
#endif /* __DISPLAY_ERRORS__ */
           *Status  = Env_Normal;
        }

     } else if (DrawClass  == ArrayRect_Class)    {
        if ((Array01  = ArrayRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = Array01->Token;

     } else if (DrawClass  == DblPtrRect_Class)   {
        if ((DblPtr01  = DblPtrRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = DblPtr01->Token;

     } else if (DrawClass  == VarrayRect_Class)   {
        if ((Varray01  = VarrayRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = Varray01->Token;

     } else if (DrawClass  == IntChunkRect_Class) {
        if ((IntChunk01  = IntChunkRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = IntChunk01->Token;

     } else if (DrawClass  == VchunkRect_Class)   {
        if ((Vchunk01  = VchunkRect_new1   (&RectTkn,     McStat)) != NullPtr)
            FoundTkn = Vchunk01->Token;

     } else if (DrawClass  == RefRect_Class)      {

        if ((ref0  = RefRect_new3   (&RectTkn,     McStat)) != NullPtr)
        {
            FoundTkn = ref0->Token;
            c3Tkn    = ref0->RefOrigin.ObjTkn;
            c4Tkn    = ref0->RefExtent.ObjTkn;
        }
     }
  }

/* DO Basic Operations on Part-Parent  relationships                       */
  if (Normal(*Status))
  {
     if (PersonParts_OwnerOf (PartTkn,      McStat,           &PersonTkn))
     {
        if (Object_GetImage  (&PersonTkn,   McStat,   (addrtype *)&EmpObj))
           AddressTkn       = EmpObj->Residence.ObjTkn;

        if (Normal(*Status))
        if (Env_IsValidToken (&AddressTkn,   McStat))
           Object_GetImage   (&AddressTkn,
                              McStat,             (addrtype *)&AddressObj);
     } else {
#ifdef __DISPLAY_ERRORS__
        sprintf (Msg,
 "\n ***ERROR*** Owner of PersonParts for Part[%4u:%6u] not found. id= <%6u>\n",
                 PartTkn->DbId,   PartTkn->Handle, PartObj->id);
        TraceMsg (0, Msg);
        Object_Dump  (PartTkn);
#endif /* __DISPLAY_ERRORS__ */
        EmpObj  = NullPtr;
       *Status  = Env_Normal;
     }

/*   Find the object in the 'ParentObjs' Set (by PersonId)                 */
     if (Normal(*Status))
     if (EmpObj  != NullPtr)
     {
        PersonObjs_FindIn    (&EmpTkn010,  (addrtype )&EmpObj->PersonId,
                              McStat,                &FoundTkn);
        if (*Status  == Set_NotFound)
        {
          *Status  = Env_Normal;
           if (DrawBug)
           {
#ifdef __DISPLAY_ERRORS__
           sprintf (Msg,
        "\n ***ERROR*** PersonObjs[%4u:%6u] not found. PersonId= <%6u>\n",
                    EmpObj->Token.DbId,   EmpObj->Token.Handle,
                    EmpObj->PersonId);
           TraceMsg (0, Msg);
           Object_Dump  (&EmpObj->Token);
#endif /* __DISPLAY_ERRORS__ */
           }
        }
     }
  }


/* DO Basic Virtual Methods on Draw Object                                 */
  if (Normal(*Status))
  if (Env_IsValidToken  (&RectTkn,     McStat))
  {
     Object_GetImage    (&RectTkn,     McStat,  (addrtype *)&BaseObj);

/*   Assert Virtual methods on  'DrawObj'                                  */

     DrawBug    = False;

     if (Normal(*Status))
        (*BaseObj->area) (BaseObj,  McStat);

     if (Normal(*Status))
     if (Exports % QueBug == 0)
        (*BaseObj->draw) (BaseObj,  McStat);

     DrawBug    = TempDrawBug;

  }

/* DO a random Query on one of the objects related to this Part            */
  if (Normal(*Status))
  if (NumExports1  % 10 && NumExports1 > 2)
      BMT_QueryOn        (PartTkn,     False,        McStat);
  else
      Draw7              (&PrimalTkn,  False,    0,  McStat);


/* FIND the object in the 'ParentNames' Set (by LastName)                  */
  if (Normal(*Status))
  if (EmpObj   != NullPtr)
  {
      PersonNames_FindIn   (&EmpTkn010,  (addrtype )EmpObj->LastName,
                            McStat,                &FoundTkn);
      if (*Status  == Set_NotFound)
      {
#ifdef __DISPLAY_ERRORS__
         sprintf (Msg,
             "\n ***ERROR*** PersonNames[%4u:%6u] not found. LastName= <%s>\n",
                  EmpObj->Token.DbId,   EmpObj->Token.Handle,
                  EmpObj->LastName);
         TraceMsg (0, Msg);
         Object_Dump  (&EmpObj->Token);
#endif /* __DISPLAY_ERRORS__ */
        *Status  = Env_Normal;
      }
  }

/* FIND the object in the 'DrawObjs' Set (by Id)                           */
  if (Normal(*Status))
  if (Env_IsValidToken  (&RectTkn,     McStat))
  {
     DrawObjs_FindIn  (&LibTkn010,  (addrtype )&BaseObj->Id,
                       McStat,                 &FoundTkn);
     if (*Status  == Set_NotFound)
     {
#ifdef __DISPLAY_ERRORS__
         sprintf (Msg,
     "\n ***ERROR*** DrawObj[%6u:%6u] not found in DrawObjs. Nam= <%s>\n",
                  RectTkn.DbId,   RectTkn.Handle, BaseObj->Id);
         TraceMsg (0, Msg);
         Object_Dump  (&RectTkn);
#endif /* __DISPLAY_ERRORS__ */
        *Status  = Env_Normal;
     }
  }

  MtrxBug  = TempMtrxBug;
  SetBug   = TempSetBug;
  ClassBug = TempClassBug;

  Exports++;

  if (!Normal(*Status))
  {
     TraceMsg (0, "\n BMT_Validate:: Part Object\n");
     Object_Dump   (PartTkn);
  }

TRACK(TrackBak,"BMT_Validate\n");
return (STAT);
}


boolean      BMT_ValidateNamedDrawObjs
                         (ft F,lt Z,zz *Status)
{
int              j              = 0;
tokentype        CurrentTkn     = NullToken;
tokentype        FoundTkn       = NullToken;
tokentype        ItorTkn        = NullToken;
boolean          TempSetBug     = SetBug;
nametype         Name;

/* Iteration for NAMED DRAW OBJS SET                                     */

   if (Normal(*Status))
   {
     TraceMsg (0, "\n   VALIDATE NamedDrawObjs   in <DrawLib > DB.\n");
    *Status  = Iterate_Begin;

     j = 0;

     do
     {
        j++;
        Primal_IterateOnClassObjects (&LibTkn010,  NamedXyRect_Class,
                                       McStat,     &ItorTkn,  &CurrentTkn);

/*      Find the object in the 'DrawObjs' Set (by Nam as a char*) (Btree)   */

/*
        if (j > 2000)
          SetBug = True;
*/

        if (Normal(*Status))
        if (Attr_ValueGet            (&CurrentTkn,  Nam_Attr,
                                      McStat,      (addrtype *)&Name))
           NamedDrawObjs_FindIn      (&LibTkn010,  (addrtype )Name,
                                      McStat,                &FoundTkn);

        if (*Status  != Iterate_EndOf)
        if (j == 0 || j % QueBug == 0)
        {
           sprintf (Msg,
                    " ItNum %6u. Token[%3u:%8u]. <%-16s>: Validated.\n",
                    j, CurrentTkn.DbId, CurrentTkn.Handle, Name);
           TraceMsg (0, Msg);
        }

        if (*Status  == Set_NotFound)
        {
#ifdef __DISPLAY_ERRORS__
            sprintf (Msg,
         "\n ***ERROR NamedXyRect[%4u:%6u] NOT in NamedDrawObjs. Nam= <%s>\n",
                     CurrentTkn.DbId,   CurrentTkn.Handle, Name);
            TraceMsg (0, Msg);
#endif /* __DISPLAY_ERRORS__ */
        }
        SetBug   = TempSetBug;

     } while (Normal(*Status));
     if (*Status  == Iterate_EndOf)
         *Status   = Env_Normal;

     sprintf (Msg,
             " <%6u> NamedXyRects Validated in <NamedDrawObjs> set.\n", j-1);
     TraceMsg (0, Msg);
   }

TRACK(TrackBak,"BMT_ValidateNamedDrawObjs\n");
return (STAT);
}


boolean      BMT_QueryOn (tokentype    *PartTkn, boolean       TraceOn,
                          ft F,lt Z,zz *Status)
{
numtype          ClassId        = 0;
tokentype        RectTkn        = NullToken;
tokentype        PersonTkn      = NullToken;
numtype          QueryType      = 0;
boolean          Assert         = False;
longtype         TempAsserts    = TestObj_Asserts;
longtype         TempNullAsserts= TestObj_NullAsserts;


  if (!Env_IsValidToken         (PartTkn,   McStat))
  {
     sprintf (Msg, " BMT_Query.  Part Token[%3u:%8u] is Invalid!\n",
              PartTkn->DbId, PartTkn->Handle);
     TraceMsg (0, Msg);
     return (STAT);
  }

  QueryType        = Random (4);

  switch (QueryType)
  {
    case   0 :  /*   Assert Query on the Test object                       */
         if (TraceOn)
            TraceMsg (0, "  Assert Query on a TestObj.\n");
         TestObj_Asserts      = 0;
         TestObj_NullAsserts  = 0;
         Draw7  (&PrimalTkn,  False,    0,  McStat);
         if (TestObj_Asserts)
           Assert  = True;

         TestObj_Asserts     += TempAsserts;
         TestObj_NullAsserts += TempNullAsserts;

         break;

    case   1 :  /*   Assert Query on the Part object                       */
         if (TraceOn)
            TraceMsg (0, "  Assert Query on a Part.\n");

         Assert       = Query_AssertOnObject
                          (PartTkn,        Query0,          McStat);
         if (Assert)
          ++Part_Asserts;
         else
          ++Part_NullAsserts;

         break;

    case   2 :  /*   Assert Query on the Draw object                       */

/*       Get the Tuple Pair from Part to Draw object                       */
         if (Attr_ValueGet            (PartTkn,     DrawObjSlotAttr_Attr,
                                       McStat,      (addrtype *)&RectTkn))

         if (Env_IsValidToken         (&RectTkn,   McStat))
         if ((ClassId    = Object_IsA (&RectTkn,   McStat)) > 0)
         {
            if (ClassId   == NamedXyRect_Class)
            {
               if (TraceOn)
                  TraceMsg (0, "  Assert Query on a NamedXyRect.\n");
               Assert = Query_AssertOnObject
                          (&RectTkn,        Query4,         McStat);
               if (Assert)
                ++NamedObj_Asserts;
               else
                ++NamedObj_NullAsserts;

            } else {
               if (TraceOn)
                  TraceMsg (0, "  Assert Query on a DrawObj.\n");

               Assert = Query_AssertOnObject
                          (&RectTkn,        Query3,         McStat);
               if (Assert)
                ++DrawObj_Asserts;
               else
                ++DrawObj_NullAsserts;
            }

         } else {
            sprintf (Msg, " BMT_Query.  DrawPart Token[%3u:%8u] is Invalid!\n",
                     RectTkn.DbId, RectTkn.Handle);
            TraceMsg (0, Msg);
         }

         break;

    case   3 :  /*   Assert Query on the Person object                     */

         if (!PersonParts_OwnerOf (PartTkn,    McStat,     &PersonTkn))
         {
#ifdef __DISPLAY_ERRORS__
            sprintf (Msg,
         "\n ***ERROR*** Owner of PersonParts for Part[%4u:%6u] not found.\n",
                     PartTkn->DbId,   PartTkn->Handle);
            TraceMsg (0, Msg);
            Object_Dump  (PartTkn);
#endif /* __DISPLAY_ERRORS__ */
           *Status  = Env_Normal;
         } else {
            if (TraceOn)
               TraceMsg (0, "  Assert Query on a Person && Address.\n");

            Assert    = Query_AssertOnObject
                          (&PersonTkn,      Query1,         McStat);
            if (Assert)
             ++Person_Asserts;
            else
             ++Person_NullAsserts;
         }
         break;

  }
  if (Assert)
   ++LookUp_Asserts;
  else
   ++LookUp_NullAsserts;

  if (TraceOn)
  if (sprintf (Msg, " BMT_Query.  Token[%3u:%8u]   Assert  = %s\n",
               PartTkn->DbId, PartTkn->Handle, BoolStr[Assert]))
      TraceMsg (0, Msg);

  if (!Normal(*Status))
  {
     TraceMsg (0, "\n BMT_Query:: Part Object\n");
     Object_Dump   (PartTkn);
  }

TRACK(TrackBak,"BMT_QueryOn\n");
return (STAT);
}


boolean      BMT_DbTransaction
                          (ft F,lt Z,zz *Status)
{
numtype        Option    = 0;
static numtype NumHits   = 0;

   if (NumHits++ % 2)
      return (STAT);

   if (LibBug)
      TraceMsg (0,   "\n Initiate DB TRANSACTION's of class <TestObj>\n");

   Option  = Random(4) ;

   switch (Option)
   {
      case 1:
           Draw7  (&PrimalTkn,  False,    1,       McStat);
           break;

      case 3:
           Draw7  (&PrimalTkn,  False,    3,       McStat);
           break;

      case 4:
           Draw7  (&PrimalTkn,  False,    Option,  McStat);
           break;

      case 0:
      default:
           Draw7  (&PrimalTkn,  False,    1,       McStat);
           break;
   }
/*
  if Option == 0  ::  create 0
            Draw7   (0);    does Query on all existing TestObj objects
                      Note :: this is called from BMT_QueryOn

  if Option == 1  ::  create 1
            Draw7   (1);    create TestObj and GET image and populate/test
                            with Gets and Puts on the image.
                            and do a Query on the object

  if Option == 2  ::  create 3
            Draw701 (1);
            Draw701 (2);
            Draw7   (0);    and do a Query on last testobj

  if Option == 3  ::  create 1
            Draw701 (1);    create TestObj and do in place transactions
                            with Gets and Puts

  if Option == 4  ::  create 1
            Draw701 (2);    create TestObj and do transactions
                            with Gets and Puts on the image.
*/

TRACK(TrackBak,"BMT_DbTransaction\n");
return (STAT);
}


boolean BMT_CommitPartDrawObj
                         (tokentype    *PartTkn,  boolean     Release,
                          ft F,lt Z,zz *Status)
{
boolean          TempClassBug   = ClassBug;
tokentype        c3Tkn          = NullToken;
tokentype        c4Tkn          = NullToken;
Rectangle       *r0             = NullPtr;
RefRect         *ref0           = NullPtr;
tokentype        FoundTkn       = NullToken;
tokentype        RectTkn        = NullToken;
numtype          DrawClass      = 0;

   if (Env_IsValidToken     (PartTkn,              McStat))
   {
      if (ClassBug)
      if (sprintf (Msg, "  Commit Draw Object of this part [%4u:%8u]\n",
                   PartTkn->DbId, PartTkn->Handle))
         TraceMsg (0,   Msg);

      if (ClassBug)
         Object_Dump        (PartTkn);

/*    Get the Tuple Pair from Part to Draw object                        */

      Attr_ValueGet         (PartTkn,     DrawObjSlotAttr_Attr,
                             McStat,      (addrtype *)&RectTkn);


      if (Normal(*Status))
      if (Env_IsValidToken           (&RectTkn,     McStat))
      if ((DrawClass = Object_IsA    (&RectTkn,     McStat)) > 0)
      if (DrawClass         == Rectangle_Class)
      {
         if ((r0  = Rectangle_new4   (&RectTkn,     McStat)) != NullPtr)
         {
            FoundTkn                = r0->Token;
            c3Tkn                   = r0->Origin->Token;
            c4Tkn                   = r0->Extent->Token;
         }
         if (Normal(*Status))
         if (Object_CommitImage      (&RectTkn,     Release,   McStat))
         if (Object_CommitImage      (&c3Tkn,       Release,   McStat))
             Object_CommitImage      (&c4Tkn,       Release,   McStat);

         DrawObjs++;
         Points      += 2;

      } else if (DrawClass == RefRect_Class) {

         if ((ref0  = RefRect_new3   (&RectTkn,     McStat)) != NullPtr)
         {
             FoundTkn               = ref0->Token;
             c3Tkn                  = ref0->RefOrigin.ObjTkn;
             c4Tkn                  = ref0->RefExtent.ObjTkn;
         }
         if (Normal(*Status))
         if (Object_CommitImage      (&RectTkn,     Release,   McStat))
         if (Object_CommitImage      (&c3Tkn,       Release,   McStat))
             Object_CommitImage      (&c4Tkn,       Release,   McStat);

         DrawObjs++;
         Points      += 2;

      } else {
         Object_CommitImage          (&RectTkn,     Release,   McStat);
         DrawObjs++;
      }
   }
   ClassBug   = TempClassBug;

TRACK(TrackBak,"BMT_CommitPartDrawObj\n");
return (STAT);
}


boolean BMT_DeletePartDrawObj
                         (tokentype    *PartTkn,   ft F,lt Z,zz *Status)
{
boolean          TempClassBug   = ClassBug;
tokentype        c3Tkn          = NullToken;
tokentype        c4Tkn          = NullToken;
Rectangle       *r0             = NullPtr;
RefRect         *ref0           = NullPtr;
tokentype        FoundTkn       = NullToken;
tokentype        RectTkn        = NullToken;
numtype          DrawClass      = 0;
numtype          Id             = 0;
nametype         DrawName;
   DrawName[0]            = '\0';

   if (DeleteDrawObjects)
   if (Normal(*Status))
   if (Env_IsValidToken     (PartTkn,              McStat))
   {
      if (ClassBug)
      if (sprintf (Msg, "  UnPair From <Part > Object[%4u:%8u]\n",
                   PartTkn->DbId, PartTkn->Handle))
         TraceMsg (0,   Msg);

      if (ClassBug)
         Object_Dump        (PartTkn);

/*    Get the Tuple Pair from Part to Draw object                        */

      if (Attr_ValueGet     (PartTkn,     DrawObjSlotAttr_Attr,
                             McStat,      (addrtype *)&RectTkn))

      if (Attr_ValuePut     (PartTkn,     DrawObjSlotAttr_Attr,
                             (addrtype *)&NullToken,   McStat))

    if (Env_IsValidToken  (&RectTkn,                 McStat))
    {

      if (Attr_ValueGet     (&RectTkn,    Id_Attr,
                             McStat,      (addrtype *)&Id))

          DrawClass = Object_IsA        (&RectTkn,     McStat);

      if (Normal(*Status))
      if (Env_IsValidToken           (&RectTkn,     McStat))
      if (DrawClass         == Rectangle_Class)
      {
         if ((r0  = Rectangle_new4   (&RectTkn,     McStat)) != NullPtr)
         {
            FoundTkn                = r0->Token;
            c3Tkn                   = r0->Origin->Token;
            c4Tkn                   = r0->Extent->Token;
         }
         if (Normal(*Status))
         if (Object_Delete           (&RectTkn,     McStat))
         if (Object_Delete           (&c3Tkn,       McStat))
             Object_Delete           (&c4Tkn,       McStat);

         DrawObjs++;
         Points      += 2;

         /* Validate removal from Set <LibRectangles>                     */
         if (Normal(*Status))
         {
            LibRectangles_FindIn     (&LibTkn010,  (addrtype )&Id,
                                      McStat,                 &FoundTkn);
            if (*Status  == Set_NotFound
            ||  *Status  == Set_EmptySet)

                *Status   = Env_Normal;

            else {
#ifdef __DISPLAY_ERRORS__
               sprintf (Msg,
       "  ***ERROR*** [%4u:%8u] Still In Set <LibRectangles>;  Id =<%6u>.\n",
                      RectTkn.DbId, RectTkn.Handle,  Id);
               TraceMsg (0,   Msg);
#endif /* __DISPLAY_ERRORS__ */
              *Status  = Env_Normal;
            }
         }

      } else if (DrawClass == RefRect_Class) {

         if ((ref0  = RefRect_new3   (&RectTkn,     McStat)) != NullPtr)
         {
             FoundTkn               = ref0->Token;
             c3Tkn                  = ref0->RefOrigin.ObjTkn;
             c4Tkn                  = ref0->RefExtent.ObjTkn;
         }
         if (Normal(*Status))
         if (Object_Delete           (&RectTkn,     McStat))
         if (Object_Delete           (&c3Tkn,       McStat))
             Object_Delete           (&c4Tkn,       McStat);

         DrawObjs++;
         Points      += 2;

      } else if (DrawClass == NamedXyRect_Class) {

         if (Attr_ValueGet     (&RectTkn,    Nam_Attr,
                                McStat,      (addrtype *)&DrawName))
#ifndef __BAD_CODE__
             Object_Delete     (&RectTkn,     McStat);
#endif

         /* Validate removal from Set <NamedDrawObjs>                     */
         if (Normal(*Status))
         {
            NamedDrawObjs_FindIn     (&LibTkn010,  (addrtype )&DrawName,
                                      McStat,                 &FoundTkn);
            if (*Status  == Set_NotFound
            ||  *Status  == Set_EmptySet)

                *Status   = Env_Normal;

            else {
#ifdef __DISPLAY_ERRORS__
               sprintf (Msg,
     "  ***ERROR*** <Draw >[%4u:%8u] Still In Set <NamedDrawObjs>;  Id =<%6u>.\n",
                      RectTkn.DbId, RectTkn.Handle,  Id);
               TraceMsg (0,   Msg);
#endif /* __DISPLAY_ERRORS__ */
              *Status  = Env_Normal;
            }
         }
         DrawObjs++;
         NamedObjs++;

      } else {
         Object_Delete               (&RectTkn,     McStat);
         DrawObjs++;

      }

      /* Validate removal from Set <DrawObjs>                            */
      if (Normal(*Status))
      {
          DrawObjs_FindIn            (&LibTkn010,  (addrtype )&Id,
                                      McStat,                 &FoundTkn);
          if (*Status  == Set_NotFound
          ||  *Status  == Set_EmptySet)

              *Status   = Env_Normal;

          else {
#ifdef __DISPLAY_ERRORS__
             sprintf (Msg,
             "  ***ERROR*** [%4u:%8u] Still In Set <DrawObjs>;  Id =<%6u>.\n",
                      RectTkn.DbId, RectTkn.Handle,  Id);
             TraceMsg (0,   Msg);
#endif /* __DISPLAY_ERRORS__ */
            *Status  = Env_Normal;
          }
      }
    }
   }
   ClassBug   = TempClassBug;

TRACK(TrackBak,"BMT_DeletePartDrawObj\n");
return (STAT);
}


boolean      BMT_TraverseSets
                         (ft F,lt Z,zz *Status)
{
tokentype        NextNode       = NullToken;
tokentype        NextTkn        = NullToken;
tokentype        CurrentNode    = NullToken;
tokentype        CurrentTkn     = NullToken;
int              ItNum          = 0;
numtype          TempQueBug     = QueBug;
treeiterfunc     IterNamesFunc  = (treeiterfunc )BMT_Iter2;

  NextNode.DbId    = LibTkn010.DbId;
  NextTkn.DbId     = LibTkn010.DbId;

  CurrentNode.DbId = LibTkn010.DbId;
  CurrentTkn.DbId  = LibTkn010.DbId;

/*
//   QueBug = 1;
*/
/*   List all entries of DrawObjs Set                                      */
     if (Normal(*Status))
     {
        TraceMsg (0, "\n SET  <DrawObjs>    entries::\n");
        for (ItNum = 0;
            (ItNum =  DrawObjs_IterateOn (&LibTkn010,             ItNum,
                                          McStat,  &CurrentNode, &CurrentTkn))
                  >= 0  && Normal(*Status);)
        {
           BMT_Iter3   (&CurrentTkn,  ItNum,  McStat);
        }
        sprintf (Msg, "   Iteration count =%6u\n",  Iter3_Count);
        TraceMsg (0, Msg);
     }

/*   List all entries of NamedDrawObjs Set                                 */
     if (Normal(*Status))
     {
/*
//      QueBug = 1;
*/
        TraceMsg (0, "\n SET  <NamedDrawObjs>  entries::\n");
        NamedDrawObjs_IterateOn (&LibTkn010,     BMT_Iter4,  McStat);
	if( *Status == Iterate_EndOf )
	   *Status = Env_Normal;
        sprintf (Msg, "   Iteration count =%6u\n",  Iter4_Count);
        TraceMsg (0, Msg);
        QueBug = TempQueBug;
     }

/*   List all entries of LibRectangles Set                                 */
     if (Normal(*Status))
     {
        TraceMsg (0, "\n SET  <LibRectangles>  entries::\n");
        LibRectangles_IterateOn (&LibTkn010,     BMT_Iter3,  McStat);
	if( *Status == Iterate_EndOf )
	   *Status = Env_Normal;
        sprintf (Msg, "   Iteration count =%6u\n",  Iter3_Count);
        TraceMsg (0, Msg);
     }

/*   List all entries of DbRectangles List                                 */
     if (Normal(*Status))
     {
        TraceMsg (0, "\n LIST <DbRectangles>   entries::\n");
        List01_IterateOn        (&RectListTkn,   BMT_Iter1,  McStat);
	if( *Status == Iterate_EndOf )
	   *Status = Env_Normal;
        sprintf (Msg, "   Iteration count =%6u\n",  Iter1_Count);
        TraceMsg (0, Msg);
     }

/*   List all entries of PersonNames Set                                   */
     if (Normal(*Status))
     {
        TraceMsg (0, "\n SET  <PersonNames  >  entries::\n");
        PersonNames_IterateOn (&EmpTkn010,     IterNamesFunc,   McStat);
	if( *Status == Iterate_EndOf )
	   *Status = Env_Normal;
        sprintf (Msg, "   Iteration count =%6u\n",  Iter2_Count);
        TraceMsg (0, Msg);
     }
     QueBug = TempQueBug;

#ifdef     __INCLUDE_MTRX__
/*   List all entries of TosFroms Matrix                                   */
     if (!Normal(*Status))
     {
        TraceMsg (0, "\n MATRIX <TosFroms>       entries::\n");
/*
        Matrix_DumpMatrix  (TosFroms_Mtrx, &LibTkn, True, 0, 0,
                                                      True, 0, 0,  McStat);
*/
        TraceMsg (0, "\n");
     }
#endif /* __INCLUDE_MTRX__ */

TRACK(TrackBak,"BMT_TraverseSets\n");
return (STAT);
}



boolean BMT_Iter1        (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status)
{
/* Iteration for  DB RECTANGLES LIST                                     */
   if (ItNum        == 1)
      Iter1_Count    = 1;
   else
      Iter1_Count++;

   if ((ItNum - 1) % QueBug == 0)
   {
     sprintf (Msg, "  %6u. [%4u:%6u]\n",
              ItNum, Token->DbId, Token->Handle);
     TraceMsg (0, Msg);
   }

TRACK(TrackBak,"BMT_Iter1\n");
return (STAT);
}


boolean  BMT_Iter2       (tokentype    *Token,     indextype     ItNum,
                          ft F,lt Z,zz *Status)
{
boolean             IsValid = False;
numtype             j      = ItNum;
Person             *P010   = NullPtr;
/* Iteration for     PERSON NAMES SET                                    */

   if (ItNum        == 1)
      Iter2_Count    = 1;
   else
      Iter2_Count++;

    if (Env_IsValidToken             (Token,        McStat))
    if (Object_GetImage (Token,       McStat,       (addrtype *)&P010))
        IsValid                     = True;

    if ((ItNum-1) % QueBug == 0)
    {
       if (j == 0 || j % QueBug == 0
       ||  (P010->LastName[0] > 'Q'
         && P010->LastName[0] < 'S'))
       {
          sprintf (Msg, " %6d. [%3u:%8u] Name:= <%-16s>, %-16s",
                    ItNum, Token->DbId, Token->Handle,
                    P010->LastName, P010->FirstName);
          TraceMsg (0, Msg);
          if (!IsValid)
             TraceMsg (0, "   IN_VALID.\n");
          else
             TraceMsg (0, "\n");
       }

    } else if (IsValid  == False) {

       sprintf (Msg, " %6d. [%3u:%8u]   IN_VALID.\n",
                    ItNum, Token->DbId, Token->Handle);
       TraceMsg (0, Msg);
    }

TRACK(TrackBak,"BMT_Iter2\n");
return (STAT);
}


boolean  BMT_Iter3       (tokentype    *Token,     indextype     ItNum,
                          ft F,lt Z,zz *Status)
{
boolean             IsValid = False;
numtype             Id      = 0;
tokentype           NodeTkn = NullToken;

/* Iteration for       DRAW OBJS SET                                     */
   if (ItNum        == 1)
      Iter3_Count    = 1;
   else
      Iter3_Count++;

    NodeTkn.DbId                = Token->DbId;
    if (Env_IsValidToken         (Token,                    McStat))
    if (Attr_ValueGet            (Token,                    Id_Attr,
                                  McStat,      (addrtype *)&Id))
    if (Attr_ValueGet            (Token,        MemberDrawObjs_Attr,
                                  McStat,      (addrtype *)&NodeTkn))
        IsValid                  = True;

    if ((ItNum-1) % QueBug == 0  || IsValid  == False)
    {
       sprintf (Msg, " %6d. [%3u:%8u]  := <%-8u>; @[:%6u]",
                ItNum, Token->DbId, Token->Handle,
                Id, NodeTkn.Handle);
       TraceMsg (0, Msg);
       if (!IsValid)
          TraceMsg (0, "   IN_VALID.\n");
       else
          TraceMsg (0, "\n");
    }

TRACK(TrackBak,"BMT_Iter3\n");
return (STAT);
}


boolean  BMT_Iter4       (tokentype    *Token,     indextype     ItNum,
                          ft F,lt Z,zz *Status)
{
boolean             IsValid = False;
nametype            Name;
/* Iteration for NAMED DRAW OBJS SET                                     */

   if (ItNum        == 1)
      Iter4_Count    = 1;
   else
      Iter4_Count++;

    if (Env_IsValidToken         (Token,        McStat))
    if (Attr_ValueGet            (Token,        Nam_Attr,
                                  McStat,      (addrtype *)&Name))
       IsValid                  = True;

    if ((ItNum-1) % QueBug == 0  || IsValid  == False)
    {
       sprintf (Msg, " %6d. [%3u:%8u]  := <%-8s>;",
                ItNum, Token->DbId, Token->Handle, Name);
       TraceMsg (0, Msg);
       if (!IsValid)
          TraceMsg (0, "   IN_VALID.\n");
       else
          TraceMsg (0, "\n");
    }

TRACK(TrackBak,"BMT_Iter4\n");
return (STAT);
}


boolean BMT_ExportPart   (longtype      X,       longtype      Y,
                          parttype      Type,    ft F,lt Z,zz *Status)
{
static  numtype   NumExports2   = 0;

   if (ClassBug)
   if (sprintf (Msg, "  BMT_ExportPart:: X=%d,  Y=%d; PartType=%u\n",
                X, Y, Type))
       TraceMsg (0,   Msg);

   NumExports2++;

TRACK(TrackBak,"BMT_ExportPart\n");
return (STAT);
}


boolean BMT_ImportPart   (longtype     *X,       longtype     *Y,
                          ft F,lt Z,zz *Status)
{
   if (ClassBug)
   if (sprintf (Msg, "  BMT_ImportPart:: X=%d,  Y=%d;\n",
                X, Y))
       TraceMsg (0,   Msg);

TRACK(TrackBak,"BMT_ImportPart\n");
return (STAT);
}



boolean BMT_BuildPersonLib
                         (tokentype    *LibToken,  numtype       MaxPersons,
                          ft F,lt Z,zz *Status)
{
#ifdef __DISPLAY_TIME__
time_t              NullTime           = 0;
time_t              CurrTime           = 0;
time_t              Elapsed            = 0;
float                Seconds            = 0.0;
#endif /* __DISPLAY_TIME__ */

int                  j                  = -1;
tokentype            PersonTkn          = NullToken;
Person              *Person             = NullPtr;
tokentype            AddressTkn         = NullToken;
static char          TabStr[MAXLINE+1]  = " ";
char                *TabStrPtr          = TabStr;
FilePtrType          TabFilePtr         = NullPtr;
numtype              TabCount           = 15;
importspec           TabFields[15];

     TraceMsg (0, "\n ***NOTE***  Persons Library Extended!\n");
     sprintf (Msg, "\n Create <%5u> Persons.\n",
              MaxPersons);
     TraceMsg (0, Msg);

     if ( (TabFilePtr = InFilePtr)                  == NULL)
     {
        fprintf (stderr, "***ERROR... Can't open TAB file <%s>\n",
                 InFileName);
       *Status   = Err_NotImplemented;

     } else {

        TabFields[0].ClassId   = 0;             /* Country    */
        TabFields[0].AttrId    = 0;

        TabFields[1].ClassId   = Address_Class;
        TabFields[1].AttrId    = Street_Attr;

        TabFields[2].ClassId   = Address_Class;
        TabFields[2].AttrId    = Suite_Attr;

        TabFields[3].ClassId   = Address_Class;
        TabFields[3].AttrId    = City_Attr;

        TabFields[4].ClassId   = Address_Class;
        TabFields[4].AttrId    = State_Attr;

        TabFields[5].ClassId   = Address_Class;
        TabFields[5].AttrId    = Country_Attr;

        TabFields[6].ClassId   = Address_Class;
        TabFields[6].AttrId    = ZipCode_Attr;

        TabFields[7].ClassId   = 0;            /* Line of Buisness   */
        TabFields[7].AttrId    = 0;

        TabFields[8].ClassId   = Address_Class;
        TabFields[8].AttrId    = Phone_Attr;

        TabFields[9].ClassId   = Address_Class;
        TabFields[9].AttrId    = Fax_Attr;

        TabFields[10].ClassId  = 0;           /* title               */
        TabFields[10].AttrId   = 0;

        TabFields[11].ClassId  = 0;           /* Dept                */
        TabFields[11].AttrId   = 0;

        TabFields[12].ClassId  = 0;           /* Notes               */
        TabFields[12].AttrId   = 0;

        TabFields[13].ClassId  = Person_Class;
        TabFields[13].AttrId   = FirstName_Attr;

        TabFields[14].ClassId  = Person_Class;
        TabFields[14].AttrId   = LastName_Attr;
     }

#ifdef __DISPLAY_TIME__
     NullTime = clock();
#endif /* __DISPLAY_TIME__ */

     j   = -1;
     while (Normal(*Status)
     &&     ++j < MaxPersons
     &&     (TabStrPtr  = fgets (TabStr,  MAXLINE,  TabFilePtr)) != NULL)
     {
        Person_Import    (&EmpTkn010,    TabStrPtr,   TabFields,  TabCount,
                          McStat,       &PersonTkn);
        if (*Status  == Set_AlreadyInSet)
        {
           --j;
            *Status    = Env_Normal;

        } else if (Normal(*Status)) {
           if (Object_GetImage    (&PersonTkn,  McStat,     (addrtype *)&Person))
           {
              AddressTkn    = Person->Residence.ObjTkn;
              if (j == 0 || j % QueBug == 0)
              {
                 sprintf (Msg,
                         " ItNum %6u. Person[%3u:%8u]. Name= %-16s, %-16s;\n",
                          j, PersonTkn.DbId, PersonTkn.Handle,
                          Person->LastName, Person->FirstName);
                 TraceMsg (0, Msg);
              }
           }
           if (Normal(*Status))
           if (Object_CommitImage (&PersonTkn,   False, McStat))
               Object_CommitImage (&AddressTkn,  False, McStat);
        }

     }
     if (Normal(*Status))
     {
        TraceMsg (0, "\n LAST Person Read::\n");
        sprintf (Msg, " ItNum %6u. Person[%3u:%8u]. Name= %-16s, %-16s;\n",
                 j, PersonTkn.DbId, PersonTkn.Handle,
                 Person->LastName, Person->FirstName);
        TraceMsg (0, Msg);
        if (DrawBug)
        {
        Object_Dump (&PersonTkn);
        Object_Dump (&AddressTkn);
        }
     }

#ifdef __DISPLAY_TIME__
     Elapsed    = clock() - NullTime;
     Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
     sprintf  (Msg, "\n Instantiate/Commit Time = %4.2f sec, for %u Person's\n",
               Seconds, j);
     TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__ */

TRACK(TrackBak,"BMT_BuildPersonLib\n");
return (STAT);
}


boolean      BMT_InitQuerys
                          (ft F,lt Z,zz *Status)
{
  if (Query0 == NullPtr)
  {
     TraceMsg (0, "\n BUILD <Query0>   for <Part2>  class::\n");
     sprintf  (Msg, "\n  if (link[1].length %s  %3u) ::\n",
               CompareOpLex[aGTE], val0);
     TraceMsg (0, Msg);

     if (Query_Create         (McStat,      &Query0))
     if (Query_BeginBuild     (Query0,       McStat))
     if (Query_ScopeOn        (Query0,      "Part2",  True,  McStat))

     if (Query_OpenParen      (Query0,       McStat))

     if (Query_AffixCompare   (Query0,     "c1.length",      aGTE, 
                               (addrtype )&val0,        McStat))

     if (Query_AffixBoolOp    (Query0,      aAND,       McStat))

     if (Query_AffixCompare   (Query0,     "c2.length",      aGTE, 
                               (addrtype )&val0,        McStat))

     if (Query_AffixBoolOp    (Query0,      aAND,       McStat))

     if (Query_AffixCompare   (Query0,     "c3.length",      aGTE, 
                               (addrtype )&val0,        McStat))

     if (Query_CloseParen     (Query0,       McStat))
     if (Query_EndBuild       (Query0,       McStat))
     if (ClassBug)
         Query_Dump           (Query0);

  }

 /* Build a Query for the Address Class.... Test By State Of Residence   */
  if (Normal(*Status))
  if (Query2 == NullPtr)
  {

     TraceMsg (0, "\n Build Query2 for <Address>   class::\n");
     sprintf (Msg, "\n  if (State == %s || State == %s)\n",
              StateNam1,  StateNam2); 
     TraceMsg (0, Msg);

     if (Query_Create         (McStat,     &Query2))

     if (Query_BeginBuild     (Query2,      McStat))

     if (Query_ScopeOn        (Query2,     "Address",  
                               INCLUSIVE,   McStat))

     if (Query_OpenParen      (Query2,      McStat))

     if (Query_AffixCompare   (Query2,     "State",    aEQ, 
                               (addrtype )&StateNam1,  McStat))

     if (Query_AffixBoolOp    (Query2,     aOR,        McStat))

     if (Query_AffixCompare   (Query2,     "State",    aEQ, 
                               (addrtype )&StateNam2,  McStat))

     if (Query_CloseParen     (Query2,      McStat))
     if (Query_EndBuild       (Query2,      McStat))
     if (ClassBug)
     {
         Query_Dump           (Query2);
     }

  }

 /* Build a Query for the Person Class .... Test On Last Name            */
  if (Normal(*Status))
  if (Query1 == NullPtr)
  {
     TraceMsg (0, "\n Build Query1 for <Person>    class::\n");
     sprintf (Msg, 
      "\n  if (LastName  >= %s && LastName <= %s && Query0(Residence)) ::\n",
              NameComp0, NameComp1);
     TraceMsg (0, Msg);

     if (Query_Create         (McStat,      &Query1))
     if (Query_BeginBuild     (Query1,       McStat))
     if (Query_ScopeOn        (Query1,      "Person",   True,        McStat))
     if (Query_SortBy         (Query1,      "LastName", aAscending,  McStat))

     if (Query_OpenParen      (Query1,       McStat))

     if (Query_AffixCompare   (Query1,      "LastName", aGTE, 
                               (addrtype )&NameComp0,   McStat))

     if (Query_AffixBoolOp    (Query1,       aAND,      McStat))

     if (Query_AffixCompare   (Query1,      "LastName", aLTE, 
                               (addrtype )&NameComp1,   McStat))

     if (Query_AffixBoolOp    (Query1,       aAND,      McStat))

     if (Query_AffixRefQuery  (Query1,      "Residence",   
                               Query2,       McStat))

     if (Query_CloseParen     (Query1,       McStat))
     if (Query_EndBuild       (Query1,       McStat))
     if (ClassBug)
         Query_Dump           (Query1);

  }

 /* Build a Query for the DrawObj Class.... Test By Id                   */
  if (Normal(*Status))
  if (Query3 == NullPtr)
  {
     GTEval1 = 3 * QueBug;
     GTEval2 = GTEval1     + 1;
     GTEval3 = GTEval1     + 2;
     LTEval  = 7 * QueBug;

     TraceMsg (0,  "\n BUILD <Query3> for <DrawObj>    class::\n");
     sprintf (Msg, "\n  if (Id  >= %3u \n", GTEval1);
     TraceMsg (0, Msg);
     sprintf (Msg,   "  &&  (Id >= %3u && Id <= %3u)\n",
              GTEval1,  GTEval2,  LTEval);
     TraceMsg (0, Msg);
     sprintf (Msg,   "  &&  Id >= %3u)\n",
              GTEval3);
     TraceMsg (0, Msg);

     if (Query_Create         (McStat,     &Query3))
     if (Query_BeginBuild     (Query3,       McStat))
     if (Query_ScopeOn        (Query3,      "DrawObj",  True,        McStat))
     if (Query_SortBy         (Query3,      "Id",       aAscending,  McStat))

     if (Query_OpenParen      (Query3,       McStat))
     if (Query_AffixCompare   (Query3,      "Id",       aGTE, 
                               (addrtype )&GTEval1,     McStat))

     if (Query_AffixBoolOp    (Query3,      aAND,       McStat))

     if (Query_OpenParen      (Query3,       McStat))
     if (Query_AffixCompare   (Query3,      "Id",       aGTE, 
                               (addrtype )&GTEval2,     McStat))
     if (Query_AffixBoolOp    (Query3,      aAND,       McStat))
     if (Query_AffixCompare   (Query3,      "Id",       aLTE, 
                               (addrtype )&LTEval,      McStat))
     if (Query_CloseParen     (Query3,       McStat))

     if (Query_AffixBoolOp    (Query3,      aAND,       McStat))
     if (Query_AffixCompare   (Query3,      "Id",       aGTE, 
                               (addrtype )&GTEval3,    McStat))
     if (Query_CloseParen     (Query3,       McStat))
     if (Query_EndBuild       (Query3,       McStat))
     if (ClassBug)
         Query_Dump           (Query3);
  }

/* Build a Query for the NamedDrawObj Class .... Test String Combos       */
  if (Normal(*Status))
  if (Query4 == NullPtr)
  {
     TraceMsg (0, "\n BUILD <Query4> for <NamedDrawObj>   class::\n");
     sprintf (Msg, "\n  if (Nam == %10s\n",
              GTEnam1); 
     TraceMsg (0, Msg);
     sprintf (Msg,   "  || (Nam == %10s  || == %10s   || == %10s\n",
              GTEnam2,  GTEnam3, GTEnam4); 
     TraceMsg (0, Msg);
     sprintf (Msg,   "       || == %10s  || == %10s   || == %10s)\n",
              GTEnam5,  GTEnam6,  GTEnam7);
     TraceMsg (0, Msg);
     sprintf (Msg,   "  && Id <= %u)\n",
              LTEnames);
     TraceMsg (0, Msg);

     if (Query_Create         (McStat,     &Query4))

     if (Query_BeginBuild     (Query4,      McStat))

     if (Query_ScopeOn        (Query4,     "NamedDrawObj",  
                               INCLUSIVE,   McStat))
     if (Query_SortBy         (Query4,     "Id",       aAscending,  McStat))

     if (Query_OpenParen      (Query4,      McStat))

     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam1,    McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))

     if (Query_OpenParen      (Query4,      McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam2,     McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam3,     McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam4,     McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam5,     McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam6,     McStat))

     if (Query_AffixBoolOp    (Query4,     aOR,        McStat))
     if (Query_AffixCompare   (Query4,     "Nam",      aEQ, 
                               (addrtype )&GTEnam7,     McStat))

     if (Query_CloseParen     (Query4,      McStat))

     if (Query_AffixBoolOp    (Query4,     aAND,       McStat))
     if (Query_AffixCompare   (Query4,     "Id",       aLTE, 
                               (addrtype )&LTEnames,   McStat))

     if (Query_CloseParen     (Query4,      McStat))
     if (Query_EndBuild       (Query4,      McStat))
     if (ClassBug)
     {
         WriteHdr   = True;
         Query_Dump           (Query4);
         WriteHdr   = False;
     }
  }

TRACK(TrackBak,"BMT_InitQuerys\n");
return (STAT);
}


/*+-----------------------------------------------------------------------+
~P                         END OF BMT01.C                                 !
  +-----------------------------------------------------------------------+*/
