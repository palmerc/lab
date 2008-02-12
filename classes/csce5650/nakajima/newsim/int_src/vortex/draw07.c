/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C APPS - draw07.c 
// *************************  DRAW07.C     *************************
// * The use of subtypes and generic type to implement a real object
// *     a Test of Attribute syntax
// *****************************************************************
*/
#define DRAW07
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "draw07.h"
#include "vchunk.h"

/*+-----------------------------------------------------------------------+
~DL                           DRAW LIBRARY                                !
  !                              METHODS                                  !
  +-----------------------------------------------------------------------+*/


boolean Draw7_Init       (ft F,lt Z,zz *Status)
{

  if (ClassBug)
     TraceMsg (0, "\n\n  Initialize Test Object...\n\n");

  if (TestObj_Class  == 0)
     TestObj_InitClass  (McStat);

TRACK(TrackBak,"Draw7_Init\n");
return (STAT);
}

/* .......................................................................
                    ....    REFRECT       ....
   .......................................................................*/
 
boolean TestObj_InitClass  (ft F,lt Z,zz *Status)
{
  if (ClassBug)
     TraceMsg (0, "\n\n  TEST OBJ  Map...\n");

  if (EnvGetMemberId     ("xCoord",             McStat, &xCoord_Attr))
  if (EnvGetMemberId     ("yCoord",             McStat, &yCoord_Attr))
  if (EnvGetClassId      ("XyPoint",            McStat, &XyPoint_Class))
  if (Normal(*Status))
  if (EnvInitClassMap    (XyPoint_Class,        sizeof(XyPoint),   
                          C_Apl,                McStat))
      EnvInvokeMap       (C_Apl,                McStat);

  if (Normal(*Status))
  if (EnvGetMemberId     ("link",               McStat, &link_Attr))
  if (EnvGetClassId      ("Connection",         McStat, &Connection_Class))
  if (EnvGetMemberId     ("to",                 McStat, &to_Attr))
  if (EnvGetMemberId     ("type",               McStat, &type_Attr))
  if (EnvGetMemberId     ("length",             McStat, &length_Attr))
  if (Normal(*Status))
  if (EnvInitClassMap    (Connection_Class,     sizeof(Connection),   
                          C_Apl,                McStat))
      EnvInvokeMap       (C_Apl,                McStat);

  if (Normal(*Status))
  if (EnvGetClassId      ("CartesianPoint",     McStat, &CartesianPoint_Class))

  if (EnvGetMemberId     ("Theta",              McStat, &Theta_Attr))
  if (EnvGetMemberId     ("Radius",             McStat, &Radius_Attr))
  if (EnvGetClassId      ("PolarPoint",         McStat, &PolarPoint_Class))

  if (EnvGetMemberId     ("Char_Scalar",        McStat, &Char_Scalar_Attr))
  if (EnvGetMemberId     ("Char_InStruct",      McStat, &Char_InStruct_Attr))
  if (EnvGetMemberId     ("Char_Array",         McStat, &Char_Array_Attr))
  if (EnvGetMemberId     ("Char_PtrArray_Array",McStat,
                                                &Char_PtrArray_Array_Attr))

  if (EnvGetMemberId     ("Short_Scalar",       McStat, &Short_Scalar_Attr))
  if (EnvGetMemberId     ("ShortU_Scalar",      McStat, &ShortU_Scalar_Attr))

  if (EnvGetMemberId     ("Int_Scalar",         McStat, &Int_Scalar_Attr))
  if (EnvGetMemberId     ("IntU_Scalar",        McStat, &IntU_Scalar_Attr))

  if (EnvGetMemberId     ("Long_Scalar",        McStat, &Long_Scalar_Attr))
  if (EnvGetMemberId     ("LongU_Scalar",       McStat, &LongU_Scalar_Attr))

  if (EnvGetMemberId     ("Float_Scalar",       McStat, &Float_Scalar_Attr))
  if (EnvGetMemberId     ("Double_Scalar",      McStat, &Double_Scalar_Attr))

  if (EnvGetMemberId     ("Token_Scalar",       McStat, &Token_Scalar_Attr))
  if (EnvGetMemberId     ("Handle_Scalar",      McStat, &Handle_Scalar_Attr))

  if (EnvGetMemberId     ("Xarray",             McStat, &Xarray_Attr))
  if (EnvGetMemberId     ("XyCoords",           McStat, &XyCoords_Attr))
  if (EnvGetMemberId     ("Void_Ptr",           McStat, &Void_Ptr_Attr))
  if (EnvGetMemberId     ("Color",              McStat, &Color_Attr))
  if (EnvGetMemberId     ("XyOrigin",           McStat, &XyOrigin_Attr))
  if (EnvGetMemberId     ("Origin",             McStat, &Origin_Attr))
  if (EnvGetMemberId     ("Points",             McStat, &Points_Attr))
  if (EnvGetMemberId     ("IsActive",           McStat, &IsActive_Attr))
  if (EnvGetMemberId     ("Byte_Array",         McStat, &Byte_Array_Attr))
  if (EnvGetMemberId     ("Char_String",        McStat, &Char_String_Attr))
  if (EnvGetMemberId     ("Yarray",             McStat, &Yarray_Attr))
  if (EnvGetMemberId     ("xArray",             McStat, &xArray07_Attr))
  if (EnvGetMemberId     ("Xcoords",            McStat, &Xcoords_Attr))
  if (EnvGetMemberId     ("XyDir",              McStat, &XyDir_Attr))
  if (EnvGetMemberId     ("BitMap",             McStat, &BitMap_Attr))
  if (EnvGetMemberId     ("RefOrigin",          McStat, &RefOrigin_Attr))
  if (EnvGetMemberId     ("VpartsDir",          McStat, &VpartsDir_Attr))
  if (EnvGetMemberId     ("VdrawObjDir",        McStat, &VdrawObjDir_Attr))
  if (EnvGetMemberId     ("VconnectsDir",       McStat, &VconnectsDir_Attr))
      EnvGetClassId      ("TestObj",            McStat, &TestObj_Class);
  if (Normal(*Status))
  if (EnvInitClassMap    (TestObj_Class,        sizeof(TestObj),   
                          C_Apl,                McStat))
  {
      EnvInvokeMap       (C_Apl,                McStat);
      if (LibBug)
         TestObj_Topology   ();
  }

TRACK(TrackBak,"TestObj_InitClass\n");
return (STAT);
}


TestObj    *TestObj_new0
                         (tokentype    *AnchorTkn, ft F,lt Z,zz *Status)
{
tokentype       Token  = NullToken;
TestObj        *Image  = NullPtr;
CartesianPoint   *o      = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  TestObj_New0 Anchor= [%4u:%6u]\n",
                AnchorTkn->DbId, AnchorTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_Create    (AnchorTkn,    TestObj_Class,  McStat,   &Token))

   if (Object_GetImage  (&Token,       McStat,       (addrtype *)&Image))
   {
      Image->RefOrigin.ObjAddr = (Point   *)o;
   }

STAT;
return (Image);
}
 

TestObj  *TestObj_new1
                         (tokentype    *RecTkn,    ft F,lt Z,zz *Status)
{
TestObj      *Image  = NullPtr;
Point          *Point0 = NullPtr;

   if (DeBug || OaBug || ClassBug)
   if (sprintf (Msg, "  TestObj_new1 Anchor= [%4u:%6u]\n",
                RecTkn->DbId, RecTkn->Handle))
      TraceMsg (0, Msg);

   if (Object_GetImage (RecTkn,       McStat,         (addrtype *)&Image))
   if (Image->RefOrigin.ObjTkn.Handle)
   if (Object_GetImage (&Image->RefOrigin.ObjTkn, McStat, (addrtype *)&Point0))
       Image->RefOrigin.ObjAddr  = Point0;

STAT;
return (Image);
}
 

void TestObj_show (TestObj   *This)
{
longaddr           StartAddr = 0;
tokentype      Token     = This->Token;

  sprintf (Msg, "\n  The  TestObj MAP:: [%2u:%8u]\n",
             Token.DbId, Token.Handle);
  TraceMsg (0, Msg);

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof TestObj            : %3u\n",   sizeof(TestObj));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      :      @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3d @%8x   = %x\n",
           (char *)&This->That     - StartAddr, &This->That, This->That);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3d @%8x   = [%2u:%8u]\n",
           (char *)&This->Token    - StartAddr, &This->Token, 
           This->Token.DbId, This->Token.Handle );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_Scalar         : +%3ld @%x   = %c\n",
           (longaddr )&This->Char_Scalar          - StartAddr,
           &This->Char_Scalar,                This->Char_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_InStruct       : +%3ld @%x   = %s\n",
           (longaddr )&This->Char_InStruct        - StartAddr,
           &This->Char_InStruct,              This->Char_InStruct);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_Array          : +%3ld @%x   = @%x\n",
           (longaddr )&This->Char_Array           - StartAddr,
           &This->Char_Array,                 This->Char_Array);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_PtrArray_Array : +%3ld @%x   = @%x\n",
           (longaddr )&This->Char_PtrArray_Array  - StartAddr,
           &This->Char_PtrArray_Array,        This->Char_PtrArray_Array);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Short_Scalar        : +%3ld @%x   = %d\n",
           (longaddr )&This->Short_Scalar         - StartAddr,
           &This->Short_Scalar,               This->Short_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->ShortU_Scalar       : +%3ld @%x   = %u\n",
           (longaddr )&This->ShortU_Scalar        - StartAddr,
           &This->ShortU_Scalar,              This->ShortU_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Int_Scalar          : +%3ld @%x   = %ld\n",
           (longaddr )&This->Int_Scalar           - StartAddr,
           &This->Int_Scalar,                 This->Int_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->IntU_Scalar         : +%3ld @%x   = %lu\n",
           (longaddr )&This->IntU_Scalar          - StartAddr,
           &This->IntU_Scalar,                This->IntU_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Long_Scalar         : +%3ld @%x   = %ld\n",
           (longaddr )&This->Long_Scalar          - StartAddr,
           &This->Long_Scalar,                This->Long_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->LongU_Scalar        : +%3ld @%x   = %lu\n",
           (longaddr )&This->LongU_Scalar         - StartAddr,
           &This->LongU_Scalar,               This->LongU_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Float_Scalar        : +%3ld @%x   = %f\n",
           (longaddr )&This->Float_Scalar         - StartAddr,
           &This->Float_Scalar,               This->Float_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Double_Scalar       : +%3ld @%x   = %g\n",
           (longaddr )&This->Double_Scalar        - StartAddr,
           &This->Double_Scalar,              This->Double_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token_Scalar        : +%3ld @%x   = [%4u:%6u]\n",
           (longaddr )&This->Token_Scalar         - StartAddr,
           &This->Token_Scalar,
            This->Token_Scalar.DbId,          This->Token_Scalar.Handle);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Handle_Scalar       : +%3ld @%x   = %lu\n",
           (longaddr )&This->Handle_Scalar        - StartAddr,
           &This->Handle_Scalar,              This->Handle_Scalar);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Xarray              : +%3ld @%x   = @%x\n",
           (longaddr )&This->Xarray               - StartAddr,
           &This->Xarray,                     This->Xarray);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyCoords            : +%3ld @%x   = @%x\n",
           (longaddr )&This->XyCoords             - StartAddr,
           &This->XyCoords,                   This->XyCoords);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Void_Ptr            : +%3ld @%x   = %x\n",
           (longaddr )&This->Void_Ptr             - StartAddr,
           &This->Void_Ptr,                   This->Void_Ptr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Color               : +%3ld @%x   = %u\n",
           (longaddr )&This->Color                - StartAddr,
           &This->Color,                      This->Color);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyOrigin            : +%3ld @%x\n",
           (longaddr )&This->XyOrigin             - StartAddr,
           &This->XyOrigin,                   This->XyOrigin);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyOrigin.xCoord     : +%3ld @%x   = %4u\n",
           (longaddr )&This->XyOrigin.xCoord      - StartAddr,
           &This->XyOrigin.xCoord,            This->XyOrigin.xCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyOrigin.yCoord     : +%3ld @%x   = %4u\n",
           (longaddr )&This->XyOrigin.yCoord      - StartAddr,
           &This->XyOrigin.yCoord,            This->XyOrigin.yCoord);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[0]             : +%3ld @%x   = %x\n",
           (longaddr )&This->link[0]              - StartAddr,
           &This->link[0],                    This->link[0]);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[1]             : +%3ld @%x   = %x\n",
           (longaddr )&This->link[1]              - StartAddr,
           &This->link[1],                    This->link[1]);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->link[2]             : +%3ld @%x   = %x\n",
           (longaddr )&This->link[2]              - StartAddr,
           &This->link[2],                    This->link[2]);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Origin              : +%3d @%8x   = %x\n",
           (char *)&This->Origin              - StartAddr, &This->Origin,
           &This->Origin);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Points              : +%3d @%8x   = %x\n",
           (char *)&This->Points              - StartAddr, &This->Points,
           &This->Points);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->IsActive            : +%3d @%8x   = %s\n",
           (char *)&This->IsActive            - StartAddr, &This->IsActive,
           BoolStr[This->IsActive]);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Byte_Array          : +%3ld @%x   = %x\n",
           (longaddr )&This->Byte_Array           - StartAddr,
           &This->Byte_Array,                 This->Byte_Array);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_String         : +%3ld @%x   = %x\n",
           (longaddr )&This->Char_String          - StartAddr,
           &This->Char_String,                This->Char_String);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->xArray              : +%3ld @%x   = @%x\n",
           (longaddr )&This->xArray               - StartAddr,
           &This->xArray,                     This->xArray);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->RefOrigin           : +%3d @%8x   = %x\n",
           (char *)&This->RefOrigin           - StartAddr, &This->RefOrigin,
           &This->RefOrigin);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->RefOrigin.ObjAddr   :                  = %x\n",
           This->RefOrigin.ObjAddr);
  TraceMsg (0, Msg);
  sprintf (Msg,  "  This->RefOrigin.ObjTkn    :                  = [%4u:%6u]\n",
            This->RefOrigin.ObjTkn.DbId, This->RefOrigin.ObjTkn.Handle);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n");

  if (This->Token.DbId == 0)
     Image_DumpPseudo     (&PrimalTkn,  TestObj_Class,  This);

return;
}


void TestObj_Topology  (void)
{
         longaddr           StartAddr = 0;
static   TestObj        That;
static   TestObj       *This      = &That;

  TraceMsg (0, "\n\n  <TestObj> TOPOLOGY MAP:: \n");

  StartAddr             = (longaddr )This;
  sprintf (Msg,  "  sizeof TestObj            : =%3u\n",   sizeof(TestObj));
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This                      : @%8x\n",   This );
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->That                : +%3ld\n",
           (longaddr )&This->That                 - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token               : +%3ld\n",
           (longaddr )&This->Token                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_Scalar         : +%3ld\n",
           (longaddr )&This->Char_Scalar          - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_InStruct       : +%3ld\n",
           (longaddr )&This->Char_InStruct        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_Array          : +%3ld\n",
           (longaddr )&This->Char_Array           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_PtrArray_Array : +%3ld\n",
           (longaddr )&This->Char_PtrArray_Array  - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Short_Scalar        : +%3ld\n",
           (longaddr )&This->Short_Scalar         - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->ShortU_Scalar       : +%3ld\n",
           (longaddr )&This->ShortU_Scalar        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Int_Scalar          : +%3ld\n",
           (longaddr )&This->Int_Scalar           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->IntU_Scalar         : +%3ld\n",
           (longaddr )&This->IntU_Scalar          - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Long_Scalar         : +%3ld\n",
           (longaddr )&This->Long_Scalar          - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->LongU_Scalar        : +%3ld\n",
           (longaddr )&This->LongU_Scalar         - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Float_Scalar        : +%3ld\n",
           (longaddr )&This->Float_Scalar         - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Double_Scalar       : +%3ld\n",
           (longaddr )&This->Double_Scalar        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Token_Scalar        : +%3ld\n",
           (longaddr )&This->Token_Scalar         - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Handle_Scalar       : +%3ld\n",
           (longaddr )&This->Handle_Scalar        - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Xarray              : +%3ld\n",
           (longaddr )&This->Xarray               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyCoords            : +%3ld\n",
           (longaddr )&This->XyCoords             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Void_Ptr            : +%3ld\n",
           (longaddr )&This->Void_Ptr             - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Color               : +%3ld\n",
           (longaddr )&This->Color                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyOrigin            : +%3ld\n",
           (longaddr )&This->XyOrigin             - StartAddr);
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

  sprintf (Msg,  "  This->Origin              : +%3ld\n",
           (longaddr )&This->Origin              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Points              : +%3ld\n",
           (longaddr )&This->Points              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->IsActive            : +%3ld\n",
           (longaddr )&This->IsActive            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Byte_Array          : +%3ld\n",
           (longaddr )&This->Byte_Array           - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Char_String         : +%3ld\n",
           (longaddr )&This->Char_String          - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Yarray              : +%3ld\n",
           (longaddr )&This->Yarray               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->xArray              : +%3ld\n",
           (longaddr )&This->xArray               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->Xcoords             : +%3ld\n",
           (longaddr )&This->Xcoords              - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->XyDir               : +%3ld\n",
           (longaddr )&This->XyDir                - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->BitMap              : +%3ld\n",
           (longaddr )&This->BitMap               - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->RefOrigin           : +%3ld\n",
           (longaddr )&This->RefOrigin            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->VpartsDir           : +%3ld\n",
           (longaddr )&This->VpartsDir            - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->VdrawObjDir         : +%3ld\n",
           (longaddr )&This->VdrawObjDir          - StartAddr);
  TraceMsg (0, Msg);

  sprintf (Msg,  "  This->VconnectsDir        : +%3ld\n",
           (longaddr )&This->VconnectsDir         - StartAddr);
  TraceMsg (0, Msg);

return;
}


boolean TestObj_delete (TestObj   **This,   deletetype    DbEvent,
                        ft F,lt Z,zz *Status)
{
TestObj   *That  = *This;

   if (DeBug || OaBug || ClassBug)
      TraceMsg (0, "\n  TestObj_delete ... ~Delete\n");

   if (ClassBug)
   if (sprintf (Msg, " TestObj [%2u:%8u] %s in DB\n",
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

TRACK(TrackBak,"TestObj_delete\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF DRAW7.C                                 !
  +-----------------------------------------------------------------------+*/
