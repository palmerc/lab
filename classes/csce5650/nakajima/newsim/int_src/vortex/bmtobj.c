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
#define BMTOBJ

#ifdef __DISPLAY_TIME__
#include <time.h>
#endif /* __DISPLAY_TIME__ */

#include "bmt01.h"

typedef Part2  Part;

       boolean BMT_Iter1 (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);
       boolean BMT_Iter2 (tokentype    *Token,   numtype       ItNum,
                          ft F,lt Z,zz *Status);

extern boolean PersonParts_InitClass 
                         (ft F,lt Z,zz *Status);

extern  numtype MemberPersonParts_Attr;
extern  boolean DrawBug;
/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

boolean DrawPartPair_InitClass (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, " DrawPartPair::InitClass...\n");

  if (!DrawPart_Tupl)
/*                                                       <CFS [2.2.5  ] >  */
  if (Env_GetAttrId     ("DrawObjSlotAttr",
                         McStat,               &DrawObj_Slot))
  if (Env_GetAttrId     ("PartObjSlotAttr",
                         McStat,               &PartObj_Slot))
/*                                                       <CFS [2.1.5  ] >  */
      Env_GetTupleId    ("DrawPartPair",      
                         McStat,               &DrawPart_Tupl);

TRACK(TrackBak,"DrawPartPair_InitClass\n");
return (STAT);
}
 

/* .......................................................................
                    ....       PART        ....
   .......................................................................*/
boolean Part_InitClass   (ft F,lt Z,zz *Status)
{
size_t          Offset    = 0;
longaddr        StartAddr = 0;
Part2           PartObj;

/*
ClassBug = True;
*/
 if (Part_Class == 0)
 {
  if (Env_GetClassId      ("Connection",         McStat,  &Connection_Class))
  if (Env_GetAttrId       ("to",                 McStat,  &to_Attr))
  if (Env_GetAttrId       ("type",               McStat,  &type_Attr))
  if (Env_GetAttrId       ("length",             McStat,  &length_Attr))
  if (Env_InitClassMap    (Connection_Class,     sizeof(Connection),
                           C_Apl,                McStat))
      Env_InvokeMap       (C_Apl,                McStat);

  if (Normal(*Status))
  if (Env_GetClassId      ("Part2",              McStat,  &Part_Class))
  if (Env_GetAttrId       ("id",                 McStat,  &id_Attr0))
  if (Env_GetAttrId       ("xx",                 McStat,  &xx_Attr))
  if (Env_GetAttrId       ("yy",                 McStat,  &yy_Attr))
  if (Env_GetAttrId       ("build",              McStat,  &build_Attr))
  if (Env_GetAttrId       ("c1",                 McStat,  &c1_Attr))
  if (Env_GetAttrId       ("c2",                 McStat,  &c2_Attr))
  if (Env_GetAttrId       ("c3",                 McStat,  &c3_Attr))
  if (Env_GetAttrId       ("FromParts",          McStat,  &FromList_Attr))
  if (Env_GetAttrId       ("RowHeadTosFroms",    McStat,  
                                                &RowHeadTosFroms_Attr))
  if (Env_GetAttrId       ("ColHeadTosFroms",    McStat,  
                                                &ColHeadTosFroms_Attr))
      Env_GetAttrId       ("DrawObjSlotAttr",    McStat,  
                                                &DrawObjSlotAttr_Attr);
  if (Normal(*Status))
      PersonParts_InitClass                     (McStat);
/*
  ClassBug = False;
*/
  StartAddr   = (longaddr     )&PartObj;
  if (Normal(*Status))
  if (Env_InitClassMap      (Part_Class,        sizeof (Part2),   
                             C_Apl,             McStat))
  {
     Offset    = (size_t)((longaddr )&PartObj.That    -  StartAddr);
     if (Env_AppendToMap    (That_AttrNum,          Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.Token   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (Token_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.id  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (id_Attr0,              Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.type      -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (type_Attr,             Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.xx        -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (xx_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.yy        -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (yy_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.build     -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (build_Attr,            Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.link[0]   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (c1_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.link[1]   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (c2_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.link[2]   -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (c3_Attr,               Offset,     McStat))
        Offset = (size_t)((longaddr )&PartObj.FromList  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (FromList_Attr,         Offset,     McStat))
        Offset = (size_t)((longaddr  )&PartObj.RowHeadTosFroms  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (RowHeadTosFroms_Attr,  Offset,     McStat))
        Offset = (size_t)((longaddr  )&PartObj.ColHeadTosFroms  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (ColHeadTosFroms_Attr,  Offset,     McStat))
        Offset = (size_t)((longaddr  )&PartObj.DrawObjSlotAttr  -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (DrawObjSlotAttr_Attr,  Offset,     McStat))
        Offset = (size_t)((longaddr  )&PartObj.MemberPersonParts -  StartAddr);

     if (Normal(*Status))
     if (Env_AppendToMap    (MemberPersonParts_Attr, Offset,  McStat))
/*
ClassBug = True;
*/
     if (Normal(*Status))
         Env_InvokeMap      (C_Apl,                             McStat);
  }
/*
//ClassBug = False;
*/
 }

TRACK(TrackBak,"Part_Init\n");
return (STAT);
}


boolean      Part_Create  (tokentype    *AnchorTkn,
                           partidtype    Id,     longtype      X,
                           longtype      Y,      longtype      TypeNum,
                           ft F,lt Z,zz *Status, tokentype    *PartTkn)
{
Part          *PartObj      = NullPtr;
tokentype      VchunkTkn    = NullToken;
parttkntype    PartToken;
#ifdef __DISPLAY_TIME__
time_t        *TimePtr      = NullPtr;
#endif /* __DISPLAY_TIME__ */

   if (Object_NewImage (AnchorTkn,    Part_Class,
                        McStat,       PartTkn,   (addrtype *)&PartObj))
   if (Vchunk_Create   (AnchorTkn,    sizeof (partidtype),    FromCount,
                        ExtendQty,    McStat,                &VchunkTkn))
   {
      PartToken.Handle       = PartTkn->Handle;

      PartToken.FromList     = VchunkTkn.Handle;

      Grp_PutEntry     (PartTkn,    VpartsDir,     Id, 
                        (addrtype )&PartToken,     McStat);
   }

   if (Normal(*Status))
   {
      PartObj->id       = Id;
      strcpy  (PartObj->type, TypeName[TypeNum]);
      PartObj->xx       = X;
      PartObj->yy       = Y;

#ifdef __DISPLAY_TIME__
#ifndef   __BMT01__
      PartObj->build    = (datetype )time(TimePtr);
#else
      PartObj->build    = (datetype )PartTkn->Handle;
#endif /* __BMT01__  */

#else
      PartObj->build    = (datetype )PartTkn->Handle;
#endif /* __DISPLAY_TIME__ */

      PartObj->FromList = VchunkTkn;

      Vlists[NumVlists++]  = Id;
      VchunkPutStackPtr (&VlistTkn,                  NumVlists, 
                         McStat,        (addrtype *)&Vlists);
   } 

   if (!Normal(*Status) 
   ||  ClassBug)
   if (sprintf (Msg, " Create  Part[%6u]. Token[%3u:%8u].\n",
               Id, PartTkn->DbId, PartTkn->Handle))
       TraceMsg (0, Msg);

   if (ClassBug)
      TraceMsg (0, "     Commit Image.\n");

   if (!Normal(*Status))
   {
      sprintf (Msg, " *** Error... PartCreate:: Part %6u. Token[%3u:%8u].\n",
               Id, PartTkn->DbId, PartTkn->Handle);
      TraceMsg (0, Msg);
   }

TRACK(TrackBak,"Part_Create\n");
return (STAT);
}


boolean      Part_GetToken
                          (tokentype    *Anchor,  partidtype    PartId,
                           ft F,lt Z,zz *Status,  tokentype    *Token)
{
parttkntype  PartToken;

  Token->DbId = Anchor->DbId;

  if (Grp_GetEntry  (Anchor,     VpartsDir,  PartId, 
                     McStat,    (addrtype  )&PartToken))
      Token->Handle = PartToken.Handle;

TRACK(TrackBak,"Part_GetToken\n");
return (STAT);
}


boolean      Part_Connect (tokentype    *PartTkn, partidtype    FromPart,
                           ft F,lt Z,zz *Status)
{
tokentype      ToTkn        = NullToken;
Part          *PartObj      = NullPtr;
partidtype     ToPart       = 0;
unsigned int   j            = 0;
tokentype      VchunkTkn    = NullToken;
indextype      StackPtr     = 0;
parttkntype    PartToken;
#ifdef     __INCLUDE_MTRX__
matrixitor     MtrxItor;
tokentype      PointTkn     = NullToken;
#endif /*  __INCLUDE_MTRX__ */

partidtype     ToParts[3];
boolean        BadMatch     = True;


   ToParts[0]      = 0;
   ToParts[1]      = 0;
   ToParts[2]      = 0;

   ToTkn.DbId      = PartTkn->DbId;
   VchunkTkn.DbId  = PartTkn->DbId;

   Object_GetImage     (PartTkn,     McStat,   (addrtype *)&PartObj);

   j = 0;
   while (Normal(*Status)
   &&     j    < 3)
   {
      BadMatch  = True;
      while (BadMatch)
      {
#ifdef __EDB_BENCHMARK__
         if (Random(16) > 0)
         {
            ToPart  = FromPart + Random(OneCent) - HalfCent;
            if (ToPart < HalfCent)
               ToPart     += HalfCent;
            else if (ToPart > (MaxParts - HalfCent))
               ToPart     -= HalfCent;

            if (ToPart <= 0
            || ToPart > MaxParts)
               ToPart      = Random(MaxParts) + 1;   

         } else {
            ToPart         = Random(MaxParts) + 1;
         }
#else
         ToPart            = Random(NumVlists);
         ToPart            = Vlists[ToPart];
#endif /* __EDB_BENCHMARK__   */

         if (ToPart   != ToParts[0]
         &&  ToPart   != ToParts[1]
         &&  ToPart   != ToParts[2])
            BadMatch   = False;
      }
      ToParts[j]              = ToPart;

      PartObj->link[j].to     = ToPart;
      strcpy (PartObj->link[j].type, ConnectType); 
      PartObj->link[j].length = ToPart % 10 + 1;

      if (Grp_GetEntry  (PartTkn,  VpartsDir,  ToPart, 
                         McStat,  (addrtype *)&PartToken))
      {
         ToTkn.Handle     =  PartToken.Handle;
         VchunkTkn.Handle =  PartToken.FromList;
      }
      if (ClassBug || FromPart <= 1)
      if (sprintf (Msg,
                   "   Connect  Part %6u. Token[%3u:%8u] FromList=%6u.\n",
                   ToPart, ToTkn.DbId, ToTkn.Handle, PartToken.FromList))
          TraceMsg (0, Msg);

      if (Normal(*Status))
      {
         if (VchunkTkn.Handle != 0)
         {
            VchunkPushUnit   (&VchunkTkn,  (addrtype )&FromPart,
                              McStat,                 &StackPtr);
         } else { 
            sprintf (Msg, "\n Warning... ToPart= %6u; Has no From List.\n",
                     ToPart);
            TraceMsg (0, Msg);
            Object_Dump (&ToTkn);
         }
      }

#ifdef     __INCLUDE_MTRX__
      if (Normal(*Status))
      if (Object_GetImage     (&ToTkn,     McStat,   (addrtype *)&ToObj))
      {
         if (FirstTime)
         {
           FirstTime  = False;
         }

         Matrix_ExpressMate
                       (TosFroms_Mtrx,  &PartLib01->OwnerTosFroms,
                        &LibTkn,         PartTkn,   &ToTkn,  
                        &PartObj->RowHeadTosFroms,  &ToObj->ColHeadTosFroms,
                        (addrtype )&PartObj->id,    (addrtype )&ToObj->id,
                        False,
                        McStat,         &MtrxItor, &PointTkn);

         if (FirstTime)
         {
           FirstTime  = False;
           MtrxBug    = False;
           SetBug     = False;
           ClassBug   = TmpClassBug;
         }
         if (*Status == Mtrx_AlreadyMated)
         {
            sprintf (Msg,
                   "   Part %6u. [%4u:%6u] X ToTkn[%3u:%8u] Already MATED!\n",
                     ToPart, PartTkn->DbId, PartTkn->Handle, 
                             ToTkn.DbId,    ToTkn.Handle);
            TraceMsg (0, Msg);
           *Status  = Env_Normal;
         }
      }
#endif /*  __INCLUDE_MTRX__ */

      j++;
   }

   if (ClassBug)
   if (FromPart % 100 == 0)
      Object_Dump (PartTkn);

TRACK(TrackBak,"Part_Connect\n");
return (STAT);
}


boolean      Part_Traverse
                          (tokentype    *PartTkn,  longtype  Level,
                           ft F,lt Z,zz *Status)
{
tokentype      ToTkn        = NullToken;
Part          *PartObj      = NullPtr;
partidtype     ToPart       = 0;
unsigned int   j            = 0;
parttkntype    PartToken;
static 
handletype     SeedHandle   = 0;
static  numtype Traverses   = 0;
static  numtype CallCount    = 0;

   if (Level == 0)
   {
     SeedHandle  = PartTkn->Handle;
     DrawObjs       = 0;
     Points         = 0;
   }

   if (Level < Traverse_Limit)
   {
      Traverses++;
      if (Object_GetImage (PartTkn,      McStat, (addrtype *)&PartObj))

      if (ClassBug || Traverses %  QueBug == 0)
      if (sprintf (Msg, " Traverse Part [%3u:%8u] Level = %2u.\n",
                   PartTkn->DbId, PartTkn->Handle, Level))
          TraceMsg (0, Msg);

      if (Normal(*Status))
      if (Traverses % 10 == 0)
      {
         if (CallCount++ % 2 == 0)
            BMT_Validate  (PartTkn,                McStat);
         else
            BMT_DeletePartDrawObj 
                           (PartTkn,      McStat);

      } else
         BMT_ExportPart    (PartObj->xx,  PartObj->yy,   PartObj->type,
                            McStat);

      j = 0;
      while (Normal(*Status)
      &&     j    < 3)
      {
         PartCount++;
         ToPart       = PartObj->link[j].to;
         ToTkn.DbId   = PartTkn->DbId;

         if (ClassBug)
         if (sprintf (Msg, "   Part->link[%2u].to Level[%2u] Id = %u\n",
                      j+1, Level, ToPart))
             TraceMsg (0, Msg);

         if (ToPart)
         if (Env_IsValidToken (PartTkn,              McStat))
         {
            if (Grp_GetEntry  (PartTkn,  VpartsDir,  ToPart, 
                               McStat,  (addrtype *)&PartToken))
               ToTkn.Handle =  PartToken.Handle;

            if (Normal(*Status))
            if (ToTkn.Handle  !=  SeedHandle)
               Part_Traverse  (&ToTkn,       Level+1,          McStat);
            else { 
              sprintf (Msg, 
                       " SEED In Traverse Part [%3u:%8u] @ Level = %2u.\n",
                       PartTkn->DbId, PartTkn->Handle, Level);
              TraceMsg (0, Msg);
              return (STAT);
            }
         }
         j++;
      }

   }

TRACK(TrackBak,"Part_Traverse\n");
return (STAT);
}


boolean      Part_Reverse (tokentype    *PartTkn,  longtype  Level,
                           ft F,lt Z,zz *Status)
{
tokentype      FromList;
tokentype      FromTkn;
Part          *PartObj      = NullPtr;
partidtype     FromPart     = 0;
unsigned int   j            = 0;
indextype      StackPtr     = 0;
parttkntype    PartToken;
static
handletype     SeedHandle   = 0;
static  numtype Reverses    = 0;
static  numtype CallCount    = 0;

   if (Level == 0)
   {
     SeedHandle  = PartTkn->Handle;
     DrawObjs       = 0;
     Points         = 0;
   }

   if (Level < Traverse_Limit)
   {
      Reverses++;
      if (ClassBug || Reverses %  QueBug == 0)
      if (sprintf (Msg, " ReVerse Traverse Part [%3u:%8u] Level = %2u.\n",
                   PartTkn->DbId, PartTkn->Handle, Level))
          TraceMsg (0, Msg);

      if (Object_GetImage (PartTkn,      McStat, (addrtype *)&PartObj))
      {
         if (Reverses % 10 == 0)
         {
            if (CallCount++ % 2 == 0)
               BMT_Validate   (PartTkn,      McStat);
            else
               BMT_DeletePartDrawObj 
                              (PartTkn,      McStat);
         } else
            BMT_ExportPart    (PartObj->xx,  PartObj->yy,   PartObj->type,
                               McStat);

         FromList.DbId     = PartTkn->DbId;
         FromList.Handle   = PartObj->FromList.Handle;
         if (Normal(*Status))
         if (FromList.Handle == 0)
         {
           sprintf (Msg,
                    " ***No From.... for Reverse Traverse Part[%3u:%8u].\n",
                    PartTkn->DbId, PartTkn->Handle);
           TraceMsg (0, Msg);
         } else
            VchunkGetStackPtr (&FromList,   McStat,      &StackPtr);
      } else {
          sprintf (Msg, " ReVerse Traverse Part [%3u:%8u] Level = %2u.\n",
                   PartTkn->DbId, PartTkn->Handle, Level);
          TraceMsg (0, Msg);
      }

/*    PartId's are stored in the virtual FromList Array in the GRP        */
/*    this requires a double get from the Vchunks                         */

      j              = 0;
      FromTkn.DbId   = PartTkn->DbId;
      while (Normal(*Status)
      &&     j    < StackPtr)
      {
         PartCount++;
         if (Vchunk_GetUnit  (&FromList,    j, 
                              McStat,       (addrtype )&FromPart))
         if (FromPart)
         {
            if (Grp_GetEntry  (PartTkn,  VpartsDir,  FromPart, 
                               McStat,  (addrtype *)&PartToken))
               FromTkn.Handle =  PartToken.Handle;

            if (ClassBug || FromTkn.Handle == 0)
            if (sprintf (Msg, "   FromList[%2u] Level[%2u] Part[%3u:%8u]\n",
                         j, Level, FromTkn.DbId, FromTkn.Handle))
                TraceMsg (0, Msg);

            if (Normal(*Status))
            if (FromTkn.Handle  !=  SeedHandle)
            {
               if (!Part_Reverse   (&FromTkn, Level+1,    McStat))
               {
                  sprintf (Msg, 
                  "\n ERROR FromList[%2u] of FromPart[%3u]= NULL handle\n",
                         j, FromPart);
                  TraceMsg (0, Msg);
                  Object_Dump     (&FromTkn);
               }

            } else { 
               sprintf (Msg, " SEED In Reverse  Part [%3u:%8u] @ Level = %2u.\n",
                        FromTkn.DbId, FromTkn.Handle, Level);
               TraceMsg (0, Msg);
               return (STAT);
            }
         }
         j++;
      }
   }

TRACK(TrackBak,"Part_Reverse\n");
return (STAT);
}


boolean      Part_ExportTo 
                          (tokentype    *PartTkn,
                           tokentype    *ApiTkn,    void         *FOId,
                           ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"Part_ExportTo\n");
return (STAT);
}


boolean      Part_ImportFrom
                          (tokentype    *ApiTkn,    void         *FOId,
                           tokentype    *PartTkn,   ft F,lt Z,zz *Status)
{

TRACK(TrackBak,"Part_ImportFrom\n");
return (STAT);
}


boolean      Part_DisConnect 
                          (tokentype    *PartTkn, ft F,lt Z,zz *Status)
{
tokentype      ToTkn        = NullToken;
Part          *PartObj      = NullPtr;
partidtype     ToPart       = 0;
int            i            = 0;
int            j            = 0;
tokentype      VchunkTkn    = NullToken;
indextype      StackPtr     = 0;
numtype       *Vchunk       = NullPtr;
parttkntype    PartToken;
boolean        NotFound     = True;
statustype     DumpStatus   = Env_Normal;
static boolean FirstTime    = True;

   ToTkn.DbId      = PartTkn->DbId;
   VchunkTkn.DbId  = PartTkn->DbId;

   if (Object_GetImage     (PartTkn,     McStat,   (addrtype *)&PartObj))
   if (ClassBug || FirstTime)
   if (sprintf (Msg,
              "   PartDisconnect:: Token[%3u:%8u] id:= %6u for each link.\n",
                PartTkn->DbId, PartTkn->Handle, PartObj->id))
       TraceMsg (0, Msg);

   i = 0;
   while (Normal(*Status)
   &&     i    < 3)
   {
      PartToken.FromList  = 0;
      ToPart   = PartObj->link[i].to;

      if (ToPart)
      if (Grp_GetEntry  (PartTkn,  VpartsDir,  ToPart, 
                         McStat,  (addrtype *)&PartToken))
      {
         ToTkn.Handle     =  PartToken.Handle;
         VchunkTkn.Handle =  PartToken.FromList;
         NotFound         = True;
      }

      if (ClassBug || FirstTime)
      if (sprintf (Msg,
                   "   DisConnect  link    [%4u]:= %6u; PartToken[%6u:%6u].\n",
                   i, ToPart, PartToken.Handle, PartToken.FromList))
          TraceMsg (0, Msg);

      if (ToPart)
      if (Normal(*Status))
      if (Env_IsValidToken    (&ToTkn,      McStat))
      if (VchunkTkn.Handle != 0)
      if (Vchunk_GetAddress   (&VchunkTkn,  McStat,(addrtype *)&Vchunk))
      if (Vchunk_GetStackPtr  (&VchunkTkn,  McStat,            &StackPtr))
      for (j  = 0,          NotFound = True;
           j  < StackPtr && NotFound; ) 
      {
         if (Vchunk[j] == PartObj->id)
            NotFound    = False;
         else
            j++;
      }

      if (ToPart)
      if (Normal(*Status))
      if (NotFound       == False)
      {
         if (Vchunk_PutStackPtr  (&VchunkTkn,             --StackPtr,  
                                  McStat,      (addrtype *)&Vchunk))
         while (j  < StackPtr +1) 
         {
            Vchunk[j] = Vchunk[j+1];
            j++;
         }
      } else {
         sprintf (Msg,
        "   DisConnect  PartObj->id :=%6u; NOT in FromList of ToPart[%4u].\n",
                  PartObj->id, ToPart);
            TraceMsg (0, Msg);
         Vchunk_Dump             (&VchunkTkn,               Dump_Status);
      }
      i++;
   }

   if (ClassBug)
      Object_Dump (PartTkn);

   FirstTime     = False;

TRACK(TrackBak,"Part_DisConnect\n");
return (STAT);
}


boolean      Part_Delete (tokentype    *PartTkn, ft F,lt Z,zz *Status)
{
tokentype      FromTkn      = NullToken;
Part          *PartObj      = NullPtr;
Part          *FromObj      = NullPtr;
partidtype     FromPart     = 0;
int            i            = 0;
int            j            = 0;
tokentype      VchunkTkn    = NullToken;
indextype      StackPtr     = 0;
numtype       *Vchunk       = NullPtr;
parttkntype    PartToken;
boolean        NotFound     = True;
static boolean FirstTime    = True;
boolean        TempClassBug = ClassBug;
boolean        TempGrpBug   = GrpBug;
boolean        TempSwapBug  = SwapBug;
handletype     WatchHandle  = 1734;

   if (ClassBug || FirstTime)
   if (sprintf (Msg, "\n   PartDelete    :: Token[%3u:%8u].\n",
                PartTkn->DbId, PartTkn->Handle))
       TraceMsg (0, Msg);

   FromTkn.DbId    = PartTkn->DbId;
   VchunkTkn.DbId  = PartTkn->DbId;

   if (Part_DisConnect     (PartTkn,      McStat))

   if (Object_GetImage     (PartTkn,      McStat,   (addrtype *)&PartObj))
       VchunkTkn          = PartObj->FromList;

   if (Normal(*Status))
   if (VchunkTkn.Handle != 0)
   if (Vchunk_GetAddress   (&VchunkTkn,   McStat,   (addrtype *)&Vchunk))
       Vchunk_GetStackPtr  (&VchunkTkn,   McStat,               &StackPtr);

   if (Normal(*Status))
   if (ClassBug || FirstTime)
   if (sprintf (Msg, "   DeleteFromList:: Vchunk[%2u:%8u]. (*%4u)\n",
                VchunkTkn.DbId, VchunkTkn.Handle, StackPtr))
       TraceMsg (0, Msg);

   i = 0;
   while (Normal(*Status)
   &&     i    <  StackPtr)
   {
      FromPart   = Vchunk[i];

      if (Grp_GetEntry  (PartTkn,  VpartsDir,  FromPart, 
                         McStat,  (addrtype *)&PartToken))
         FromTkn.Handle   =  PartToken.Handle;

      if (ClassBug && FirstTime)
      if (FromTkn.Handle == WatchHandle)
      {
         ClassBug = True;
         GrpBug = True;
         SwapBug = True;
      }
      if (ClassBug || FirstTime)
      if (sprintf (Msg,
                   "   DisConnect  FromList[%4u]:=%6u;  Token[%6u:%6u].\n",
                   i, FromPart, PartToken.Handle, PartToken.FromList))
          TraceMsg (0, Msg);

      if (Normal(*Status))
      if (Env_IsValidToken    (&FromTkn,     McStat))
      if (Object_GetImage     (&FromTkn,     McStat,   (addrtype *)&FromObj))
      {
         for (j  = 0,          NotFound = True;
              j  < 3        && NotFound;        j++) 
         {
            if (FromObj->link[j].to == PartObj->id)
            {
               NotFound              = False;
               FromObj->link[j].to   = 0;
            }
         }
         if (NotFound)
         if (sprintf (Msg,
             "   DisConnect  Part %6u. Token[%3u:%8u] id=%6u NOT in Link[*].\n",
                   FromPart, FromTkn.DbId, FromTkn.Handle, PartObj->id))
            TraceMsg (0, Msg);
      }
      i++;
   }

   i                      = PartObj->id -1;
   NotFound               = True;
   if (Normal(*Status))
   if (Vchunk_GetAddress   (&VlistTkn,    McStat,   (addrtype *)&Vlists))
   if (Vlists[i]         == PartObj->id
   &&  i                  < NumVlists)
   {
    --NumVlists;
      NotFound            = False;
      if (i               < NumVlists)
         Vlists[i]        = Vlists[NumVlists];
      else
         Vlists[i]        = 0;

      if (ClassBug || FirstTime)
      if (sprintf (Msg, "   Vlists[%4u] :=%6u;\n", i, Vlists[i]))
          TraceMsg (0, Msg);

   } else {
      for (i  = -1,           NotFound   = True;   
         ++i  < NumVlists  && NotFound; )
      {
        if (Vlists[i]    == PartObj->id)
        {
         --NumVlists;
           NotFound       = False;
           if (i             < NumVlists)
              Vlists[i]   = Vlists[NumVlists];
           else
              Vlists[i]   = 0;

           if (ClassBug || FirstTime)
           if (sprintf (Msg, "   Vlists[%4u] :=%6u;\n", i, Vlists[i]))
               TraceMsg (0, Msg);
        }
      }
   }

   if (Normal(*Status))
   if (NotFound)
   if (sprintf (Msg,
       "   PartDelete:: Token[%3u:%8u] PartId=%6u NOT in Vlists array.\n",
                PartTkn->DbId, PartTkn->Handle, PartObj->id))
      TraceMsg (0, Msg);

   if (Normal(*Status))
       Vchunk_PutStackPtr (&VlistTkn,               NumVlists,  
                           McStat,     (addrtype *)&Vlists);

   PartToken.Handle   =  NullHandle;
   PartToken.FromList =  NullHandle;

   if (Normal(*Status))
   if (Grp_PutEntry           (PartTkn,    VpartsDir,       PartObj->id, 
                               (addrtype )&PartToken,       McStat))

       Object_Delete          (PartTkn,                     McStat);

   ClassBug      = TempClassBug;
   GrpBug        = TempGrpBug;
   SwapBug       = TempSwapBug;
   FirstTime     = False;

TRACK(TrackBak,"Part_Delete\n");
return (STAT);
}


void Part_Topology       (void)
{
         longaddr    StartAddr = 0;
static   Part2       That;
static   Part2      *This      = &That;

  TraceMsg (0, "\n\n  <Part2> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof Part2              : =%3u\n",   sizeof(Part2));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->id                  : +%3ld\n",
           (longaddr )&This->id                   - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->type                : +%3ld\n",
           (longaddr )&This->type                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->xx                  : +%3ld\n",
           (longaddr )&This->xx                   - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->yy                  : +%3ld\n",
           (longaddr )&This->yy                   - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->build               : +%3ld\n",
           (longaddr )&This->build                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[0]             : +%3ld\n",
           (longaddr )&This->link[0]              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[1]             : +%3ld\n",
           (longaddr )&This->link[1]              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[2]             : +%3ld\n",
           (longaddr )&This->link[2]              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->FromList            : +%3ld\n",
           (longaddr )&This->FromList             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->RowHeadTosFroms     : +%3ld\n",
           (longaddr )&This->RowHeadTosFroms      - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->ColHeadTosFroms     : +%3ld\n",
           (longaddr )&This->ColHeadTosFroms      - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->DrawObjSlotAttr     : +%3ld\n",
           (longaddr )&This->DrawObjSlotAttr      - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->MemberPersonParts   : +%3ld\n",
           (longaddr )&This->MemberPersonParts    - StartAddr);
  TraceMsg (0, Msg);

return;
}

/*+-----------------------------------------------------------------------+
~P                         END OF BMTOBJ.C                                !
  +-----------------------------------------------------------------------+*/
