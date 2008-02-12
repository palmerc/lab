/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV- Utils.c */
#define  UT
#ifdef __NOSTR__
#include <string.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "files.h"
#ifdef ENV01
#include "hm.h"
#include "env0.h"
#include "odbm.h"
#include "voa.h"
#include "env1.h"
#endif

extern void       MoveBytes   (addrtype     Source,    addrtype     Target,
                               bytessize    ByteCount);

/*+-----------------------------------------------------------------------+
~C                            UTILS                                       !
  +-----------------------------------------------------------------------+*/


int pow2Round(int val)
{
   int i = 1;

   while (i < val)
      i <<= 1;
   return(i);
}


boolean    PrintErr (ft F,lt Z,zz Status)
{
  char Line[256];

  if (Status > 0  || DeBug)
  if (DeBug
  || (Status  != Om_ObjHdrExists
  &&  Status  != Kernl_IamNot
  &&  Status  != Code_ClassNotMember
  &&  Status  != Set_AlreadyInSet
  &&  Status  != Set_NotFound
  &&  Status  != Set_InSerted
  &&  Status  != Set_EmptySet
  &&  Status  != Mtrx_NullMatrix
  &&  Status  != Mtrx_NotMated
  &&  Status  != Mtrx_AlreadyMated
  &&  Status  != List01_EmptyList
  &&  Status  != List01_AlreadyInList
  &&  Status  != List01_NotFound)     )
  {
     memset (Line, NULL, sizeof (Line));
     sprintf (Line, " *** ERROR [%d] IN procedure :: %17s", Status, TrackBak);
     SendMsg (0, Line);
     sprintf (Line, "                Called FROM  :: %17s  @Line= %u\n", F, Z);
     SendMsg (0, Line);
  }

return (Status  > 0 ? False : True);
}


boolean    SetWatch (addrtype VoidAddr, sizetype WordSize)
{

    WatchAddr  = (char *)VoidAddr;
    WatchSize  = WordSize;
    memcpy  (WatchWord, WatchAddr, WatchSize);

    sprintf (Msg, " Set WATCH On @ %x  Value(* %5u) = ",
             WatchAddr, WatchSize);
    SendMsg (0, Msg);
    TraceBytes (0, (addrtype )WatchAddr, (sizetype )WatchSize);

    Watch_Set  = True;

return (True);
}


boolean    StopWatch (addrtype     VoidAddr)
{
    sprintf (Msg, " Stop WATCH On @ %x\n", VoidAddr);
    SendMsg (0, Msg);
    Watch_Set   = False;
return (True);
}


void     SetBreak   (boolean       Break)
{
   Break_Set   = Break;
}


boolean    VoidTrack (ft F,lt Z,zz *Status)
{
  if (Msg[MsgMaxIndex] != '\0')
  {
     SendMsg  (0, "\n ***WARNING*** Msg Buffer Size OverWritten\n");
    *Status = Env0_Corrupted;
  }

  if (Watch_Set)
  if (!strncmp ((char *)PtrToVoid,  (char *)"VORTEx ", (size_t )9))
  {

#ifdef ENV01
     if (Env0 != NullPtr)
     if (Env0->Handle !=  Renv0_Hndl || Env0->DescNum != 0)
     {
        SendMsg  (0,   " *** RENV0 *** Env0 Corrupted *** RENV0 \n");
        sprintf  (Msg, " *** ERROR *** Renv.Handle == %u OverWriten\n",
                 Env0->Handle);
        SendMsg  (0, Msg);
       *Status = Env0_Corrupted;
        PrintErr (F, Z, *Status);
     }
#endif

     if (Watch_Set)
     {
        if (memcmp    (WatchAddr, (addrtype )&WatchWord[0], WatchSize) != 0)
        {
           sprintf    (Msg, "     Watch @%x   Value   = ", WatchAddr);
           TraceMsg   (0, Msg);
           TraceBytes (0, WatchAddr, WatchSize);
           TraceMsg   (0, "\n");
           sprintf    (Msg, " *** Watch Changed IN    :: %21s",  TrackBak);
           TraceMsg   (0, Msg);
           sprintf    (Msg,
                       "           Called  FROM  :: %21s  @Line= %u\n", F, Z);
           TraceMsg   (0, Msg);
           memcpy     ((addrtype )&WatchWord[0], WatchAddr, WatchSize);
           if (Break_Set)
              *Status = Env_WatchChangedValue;
        }
     }
     PrintTrack(F, Z);

  } else {
     SendMsg  (0, " *** VOID0 *** 0 ACCESS NO 1 *** VOID1 \n");
     III[8] = AsciiEow;
     SendMsg  (0, " ... Msg Buffer = :: \n");
     SendMsg  (0, Msg);
     sprintf  (Msg, " ... 2 Byte OverLoad :: %x,  %x \n",
               *(bytetype *)III, (bytetype )III[4]);
     SendMsg  (0, Msg);
     PrintErr (F, Z, *Status = Env_BlackIII);
     strncpy  (III, "VORTEx ", 9);
     *Status = Env_Normal;
     PrintTrack(F, Z);
  }

return (True);
}


boolean    PrintTrack (ft F,lt Z)
{
  if (Trace1 > TrackBack)
  {
     if (WriteHdr)
     {
        fprintf (MsgFilePtr, " return from %-24s", TrackBak);
        fprintf (MsgFilePtr, " called from %-24s  @Line= %4u", F, Z);
#ifdef    FLUSH_FILES
        fflush  (MsgFilePtr);
#endif /* FLUSH_FILES  */

     } else {
#ifndef __GUI__
        printf (" return from %s", TrackBak);
#endif
        if (OutFilePtr != NULL)
        {
           fprintf (OutFilePtr, " return from %s", TrackBak);
#ifdef    FLUSH_FILES
           fflush  (OutFilePtr);
#endif /* FLUSH_FILES  */
        }
     }
  }
return (True);
}


void       StackTrack (char *ProgNam)
{

  if (Trace1 > TrackBack)
  {
     if (WriteHdr)
     {
        fprintf (MsgFilePtr, " return from %s", ProgNam);
#ifdef    FLUSH_FILES
        fflush  (MsgFilePtr);
#endif /* FLUSH_FILES  */

     } else {
#ifndef __GUI__
       printf (" return from %s", ProgNam);
#endif
       if (OutFilePtr != NULL)
       {
          fprintf (OutFilePtr, " return from %s", ProgNam);
#ifdef    FLUSH_FILES
          fflush  (OutFilePtr);
#endif /* FLUSH_FILES  */
       }

     }
  }

return;
}

#ifdef __UNIX__
#define __GUI__
#endif /* __UNIX__ */

boolean    TraceMsg   (int TraceLevel,  char  *MsgLine)
{
 if (Print_Msg)
 {
    if (abs(DeBug) >= TraceLevel)
    {
       if (WriteHdr || TraceLevel < 0)
       {
          if (MsgFilePtr != NULL)
          {
             fprintf (MsgFilePtr, "%s", MsgLine);
#ifdef    FLUSH_FILES
             fflush  (MsgFilePtr);
#endif /* FLUSH_FILES  */
          }
       } else {
          if (OutFilePtr != NULL)
          {
             fprintf (OutFilePtr, "%s", MsgLine);
#ifdef    FLUSH_FILES
             fflush  (OutFilePtr);
#endif /* FLUSH_FILES  */
          }
#ifndef __GUI__
          printf ("%s", MsgLine);
#endif
       }
    }
 }

return(True);
}


boolean    SendMsg   (int MsgLevel,  char  *MsgLine) {

  if (abs(DeBug) >= MsgLevel)
  {
        if (MsgFilePtr != NULL)
        {
           fprintf (MsgFilePtr, "%s", MsgLine);
#ifdef    FLUSH_FILES
           fflush  (MsgFilePtr);
#endif /* FLUSH_FILES  */
        }
#ifdef __GUI__
        if ((OutFilePtr != NULL) && (MsgLevel > 0))
        {
           fprintf (OutFilePtr, "%s", MsgLine);
#ifdef    FLUSH_FILES
           fflush  (OutFilePtr);
#endif /* FLUSH_FILES  */
        }
#endif /* __GUI__ */
#ifndef __GUI__
        printf ("%s", MsgLine);
#endif
  }

return(True);
}


boolean    TraceBytes (int TraceLevel,  addrtype   SomeBytes, sizetype Size)
{
counttype      Bytes           = 0;
bytetype      *BytePtr         = NullPtr;
short          ShortVal        = 0;
lt64           LongVal         = 0;
char           CharVal         = 0;
unsigned char  UnCharVal       = 0;

if (TraceLevel <= DeBug)
{
  if (Size <= 4)
  {
     if (Size == 1)
     {
        UnCharVal  =  *(unsigned char*)SomeBytes;
        if (*(byte *)SomeBytes <= 127)
           sprintf (Msg, "%8u || X%2x ", UnCharVal, UnCharVal);
        else {
           CharVal  =  *(byte *)SomeBytes;
           sprintf (Msg, "%8d || X%2x ", CharVal,   UnCharVal);
        }

     } else if (Size == 2) {
        if (*(unshorttype *)SomeBytes <= 32000)
           sprintf (Msg, "%8u  ", *(short *)SomeBytes);
        else {
           ShortVal  =  *(short *)SomeBytes;
           sprintf (Msg, "%8d  ", ShortVal);
        }

     } else if (Size == 4) {
        if (*(lt64 *)SomeBytes <= 100000L)
           sprintf (Msg, "%8d",     *(lt64  *)SomeBytes);
        else {
           LongVal  =  *(lt64 *)SomeBytes;
           sprintf (Msg, "%8d  || X%8x", LongVal,  *(char **)SomeBytes);
        }
     } else
        sprintf (Msg, "Null_Size");
     TraceMsg (0, Msg);

  } else {
     BytePtr  = (bytetype *)SomeBytes;
     TraceMsg  (0, "\n          ");
     while (++Bytes <= Size)
     {
        sprintf   (Msg, "%4u ", *BytePtr);
        TraceMsg  (0,   Msg);
        if (Bytes  % 10 == 0
        &&  Bytes       != Size)
           TraceMsg  (0, "\n          ");
        BytePtr++;
     }
  }
}

return(True);
}


boolean    TraceField (int       TraceLevel,  addrtype  FieldAddr,
                       sizetype  Size,        typetype  Type)
{
counttype      Bytes        = 0;
char           StrVal[32];
bytetype      *BytePtr      = NullPtr;

#ifdef ENV01
statustype     DumpStatus    = Env_Normal;
tokentype      VchunkTkn;
#endif

/*
struct  EnumValueType {
   byte   Low;
   byte   High;
}             EnumValue;
*/
   if (TraceLevel >= 999)
     TraceMsg (TraceLevel, "Ut_TraceLevelOutOfRange\n");

   switch (Type)
   {
      case aString    :
                  MoveBytes ((addrtype )FieldAddr, (addrtype )StrVal, Size);
                  StrVal[Size-1] = '\0';
                  sprintf (Msg, "%s  ", StrVal);
                  break;

      case aChunk     :
#ifdef ENV01
                  VchunkTkn.DbId   = RenvId;
                  VchunkTkn.Handle = *(handletype *)FieldAddr;

                  sprintf (Msg, "%u\n", *(handletype *)FieldAddr);
                  TraceMsg (0, Msg);

                  if (VchunkTkn.Handle)
                  {
                     OaDumpVchunkVchunk (&VchunkTkn, Dump_Status);
                     if (DumpStatus == Oa_VchunkNotExist)
                        DumpStatus = Env_Normal;
                  }
#endif
                  break;

      case aInteger   :
                  if (Size == 1)
                  {
                     sprintf (Msg, "%u  ", *(byte  *)FieldAddr);

                  } else if (Size == 2) {
                     sprintf (Msg, "%d  ", *(short *)FieldAddr);

                  } else if (Size == 4) {
                     sprintf (Msg, "%d",
                              *(char **)FieldAddr);
                  } else {
                     Bytes = 0;
                     BytePtr  = (bytetype *)FieldAddr;
                     TraceMsg  (0, "\n          ");
                     while (++Bytes <= Size)
                     {
                        sprintf   (Msg, "%4u ", *BytePtr);
                        TraceMsg  (0,   Msg);
                        if (Bytes  % 10 == 0
                        &&  Bytes       != Size)
                           TraceMsg  (0, "\n          ");
                        BytePtr++;
                     }
                     if (!Bytes) sprintf (Msg, " NULL_SIZE");

                  }
                  break;

      case aReal   :
                  if (Size == sizeof (float))
                     sprintf (Msg, "%f  ", *(float  *)FieldAddr);

                  else if (Size == sizeof (double))
                     sprintf (Msg, "%g  ", *(double *)FieldAddr);
                  break;

      case aLink       :
                  if (Size == sizeof (handletype))
                     sprintf (Msg, "[XX:%4u] ", *(handletype *)FieldAddr);

                  else
                     sprintf (Msg, "[%3u:%4u] ",
                              ((tokentype *)FieldAddr)->DbId,
                              ((tokentype *)FieldAddr)->Handle);
                  break;

      case aEnum       :
                  sprintf (Msg, "%u  ", *(word *)FieldAddr);
                  break;

      case aBoolean     :
                  if (! *(boolean *)FieldAddr)
                     strcpy (Msg, " True ");

                  else
                     strcpy (Msg, " False ");
                  break;

      case aDbmsString  :
      case aValue       :
                  Bytes = 0;
                  BytePtr  = (bytetype *)FieldAddr;
                  TraceMsg  (0, "\n          ");
                  while (++Bytes <= Size)
                  {
                     sprintf   (Msg, "%4x ", *BytePtr);
                     TraceMsg  (0,   Msg);
                     if (Bytes  % 10 == 0
                     &&  Bytes       != Size)
                        TraceMsg  (0, "\n          ");
                     BytePtr++;
                  }
                  if (!Bytes) sprintf (Msg, " NULL_SIZE");
                  break;

      case aAddr        :
                  sprintf (Msg, "%x ",  *(addrtype **)FieldAddr);
                  break;
      default           :
                  sprintf (Msg, " NULL_TYPE");
   }
   if (Type != aChunk)
      TraceMsg  (0, Msg);

return(True);
}


boolean    Ut_TraceValue (int           TraceLevel,addrtype      Value,
                          valueclass    ValueClass,sizetype      ValueSize,
                          numtype       ValueObj)
{
counttype      Bytes        = 0;
char           StrVal[32];
bytetype      *BytePtr      = NullPtr;

#ifdef ENV01
tokentype      VchunkTkn;
#endif

/*
struct  EnumValueType {
   byte   Low;
   byte   High;
}             EnumValue;
*/
/*
typedef enum              TypeValueClassType
        {aVoid,
         aBitField,  aChar,         aUnChar,
         aShort,     aUnShort,      aInt,          aUnInt,
         aLong,      aUnLong,       aQuad,         aOctet,
         aVoidPtr,   aFloat,        aDouble,
         aEnumType,  aStruct,       aUnion,
         aComplex,   aObject,       aOid,   aNonValueClass=EXT_ENUM}
                          valueclasstype;
typedef valueclasstype    valueclass;
*/
 if (Value  !=  NullPtr)
 {
   switch (ValueClass)
   {
      case aOid       :
                  VchunkTkn = *(tokentype *)Value;
                  sprintf (Msg, "[%3u:%4u]",
                           VchunkTkn.DbId, VchunkTkn.Handle);
                  break;

      case aVoid        :
      case aVoidPtr     :
                  sprintf (Msg, "%x ",  *(addrtype **)Value);
                  break;

      case aBitField    :
                  sprintf (Msg, "%8o ",  *(addrtype **)Value);
                  break;

      case aChar      :
                  if (ValueSize != 1)
                     sprintf (Msg, "%c  ", *(char            *)Value);
                  else
                     sprintf (Msg, "%s  ", *(char            *)Value);
                  break;

      case aUnChar    :
                  if (ValueSize != 1)
                     sprintf (Msg, "%c  ", *(char            *)Value);
                  else {
                    MoveBytes ((addrtype )Value, (addrtype )StrVal, ValueSize);
                    StrVal[ValueSize-1] = '\0';
                    sprintf (Msg, "%s  ", StrVal);
                  }
                  break;

      case aShort     :
                  sprintf (Msg, "%d  ", *(short           *)Value);
                  break;

      case aUnShort   :
                  sprintf (Msg, "%u  ", *(unsigned short  *)Value);
                  break;

      case aInt       :
                  sprintf (Msg, "%d  ", *(int             *)Value);
                  break;

      case aUnInt     :
                  sprintf (Msg, "%u  ", *(unsigned int    *)Value);
                  break;

      case aLong      :
                  sprintf (Msg, "%d  ", *(lt64            *)Value);
                  break;

      case aUnLong    :
                  sprintf (Msg, "%u  ", *(farlongtype     *)Value);
                  break;

      case aQuad      :
                  Bytes = 0;
                  BytePtr  = (bytetype *)Value;
                  TraceMsg  (0, "\n          ");
                  while (++Bytes <= ValueSize)
                  {
                     sprintf   (Msg, "%4u ", *BytePtr);
                     TraceMsg  (0,   Msg);
                     if (Bytes  % 10 == 0
                     &&  Bytes       != ValueSize)
                        TraceMsg  (0, "\n          ");
                     BytePtr++;
                  }
                  if (!Bytes)
                     sprintf (Msg, " NULL_SIZE");

                  break;

      case aFloat  :
                  sprintf (Msg, "%f  ", *(float  *)Value);
                  break;

      case aDouble :
                  sprintf (Msg, "%g  ", *(double *)Value);
                  break;

      case aEnumType   :
                  sprintf (Msg, "%u  ", *(word *)Value);
                  break;

      case aOctet       :
                  Bytes = 0;
                  BytePtr  = (bytetype *)Value;
                  TraceMsg  (0, "\n          ");
                  while (++Bytes <= ValueSize)
                  {
                     sprintf   (Msg, "%4x ", *BytePtr);
                     TraceMsg  (0,   Msg);
                     if (Bytes  % 10 == 0
                     &&  Bytes       != ValueSize)
                        TraceMsg  (0, "\n          ");
                     BytePtr++;
                  }
                  if (!Bytes) sprintf (Msg, " NULL_SIZE");
                  break;

      default           :
                  sprintf (Msg, " NULL_TYPE");
   }

   if (ValueClass != aObject)
   {
      TraceMsg  (0, Msg);
      TraceMsg  (0, "\n");
   }
 } else {
   TraceMsg  (0, "(NULL)\n");

 }
return(True);
}


void       AlignMember   (typetype     Type_Type,  sizetype     Size,
                          sizetype    *Offset)
{ /* Offset    :: Current Size of Struct... [Returns with Aligned Position]
     Type      :: Type of New Member
     Size      :: Size of New Member
     ByteAlign :: Byte Alignment                                           */

sizetype  ByteAlign  = 0;
sizetype  Align      = 0;

 if (DeBug)
 if (sprintf (Msg,  "    Type[%u] Size(%3u) @ Offset = %2u ",
              Type_Type, Size, *Offset))
    TraceMsg (0, Msg);

 if (*Offset)
 {
   switch (Type_Type)
   {
      case aString    :
                  ByteAlign = Byte_Alignment;
                  break;

      case aLink      :
#ifdef LARGE
                  ByteAlign = Byte4Alignment;
#else
                  ByteAlign = Byte2Alignment;
#endif
                  break;

      case aInteger   :
                  if (Size == 1)
                  {
                     ByteAlign = Byte_Alignment;
 
                  } else if (Size == 2) {
                     ByteAlign = Byte2Alignment;

                  } else if (Size == 4) {
                     ByteAlign = Byte4Alignment;

                  } else { 
                     ByteAlign = Byte4Alignment;
                  }
                  break;

      case aReal       :
                  if (Size == sizeof (float)) 
                     ByteAlign = Byte4Alignment;

                  else if (Size == sizeof (double)) 
                     ByteAlign = Byte8Alignment;
                  break;

      case aEnum        :
      case aBoolean     :
                  ByteAlign = Byte4Alignment;
                  break;

      case aChunk       :
                  ByteAlign = Byte2Alignment;
                  break;

      case aAddr        :
                  ByteAlign = BytePtrAlignment;
                  break;

      case aRefObj      :
      case aDbmsString  :
      case aValue       :
                  ByteAlign = Byte4Alignment;
                  break;

   }
   Align      = *Offset   %  ByteAlign;

   if (Align)
      Align   = ByteAlign - (*Offset %  ByteAlign);

  *Offset    +=  Align;

 }
 if (DeBug)
 if (sprintf (Msg, "  + ByteAlignment(= %2u)\n", ByteAlign))
     TraceMsg (0, Msg);

}


void       AlignStruc    (sizetype     *Offset)
{ /* Offset    :: Current Size of Struct 
     ByteAlign :: Byte Alignment for structs [compiler specific]           */

sizetype  ByteAlign  = 0;

     ByteAlign  = *Offset %  StrucAlignment;
     if (ByteAlign)
        ByteAlign  = StrucAlignment - ByteAlign;

    *Offset    +=  ByteAlign;

     if (DeBug || ClassBug)
     if (sprintf (Msg, "  StrucAlignment = %2u\n", ByteAlign))
         TraceMsg (0, Msg);

}


char      *StrToLower (char *pc)
{
 char *pcopy = pc;
 while ((*pc = tolower(*pc)) != '\0')
          pc++;
 *pc = '\0';
 return (pcopy);
}
 

char      *StrToUpper (char *pc)
{
 char *pcopy = pc;

 while ((*pc = toupper(*pc)) != '\0')
          pc++;
 *pc = '\0';

 return (pcopy);
}
 

void    Ut_ReverseStr    (char          Str[])
{
/* Reverse string in place.                                               */
int     c  = 0;
int     i  = 0;
int     j  = 0;

   for (j = strlen(Str) - 1;  i < j; i++, j--)
   {
      c      = Str[i];
      Str[i] = Str[j];
      Str[j] = c;
   }
}

void    Ut_IntToStr      (int           IntNum,    char          Str[])
{
/* Convert the integer IntNum to characters in s                          */
int     i     = 0;
int     sign  = 0;

  if ((sign = IntNum)  < 0)            /* record sign                     */
      IntNum    = -IntNum;             /* make int positive               */

  do {                                 /* generate digits reverse order   */
     Str[i++]  = IntNum  % 10  + '0';  /* get next digit                  */
  }  while ((IntNum /= 10)  > 0);      /* delete it                       */

  if (sign  < 0)
    Str[i++]  = '-';
  Str[i]  = '\0';

  Ut_ReverseStr  (Str);
}
 

boolean Ut_FindInList    (char         *Word,     char        **List,
                          numtype       MaxList,  int          *Index)
{
int   i = 0;

   for (i=0; i < MaxList &&  strcmp (Word, List[i]); i++)
   {   }
   if (i < MaxList) 
      *Index = i;
   else
      *Index = 0;

   if (DeBug)
   if (sprintf (Msg, "  Ut_FindInList [%s] Index = %u (i= %u) \n", 
                Word,  *Index, i))
       TraceMsg (0, Msg);

 StackTrack ("Ut_FindInList\n");

return ((boolean )(i != (int )MaxList));
}

/*     0.0.14                                                              */
boolean Ut_CompareWild   (char         *VarValue,  compareop     CompOp,
                          char         *WildSpec,  ft F,lt Z,zz *Status)
{
static char *WildOps    = "?*";
sizetype     StrSize    = 0;
int          i          = 0;
int          j          = 0;
numtype      VarIndex        = 0;
int          VarLen          = 0;
char         CompValue[MAXSTRING];
char        *WildSeg         = NullPtr;
char        *VarSeg          = VarValue;
char         CharSpec[2];

  strcpy (CompValue, "\0");
  strcpy (CharSpec,  "\0");

  if (ClassBug)
  if (sprintf  (Msg, "    Ut_CompareWild:: '%-24s'  %6s on spec %s..\n", 
                VarValue, CompareOpName[CompOp],  WildSpec))
      TraceMsg (0, Msg);

#ifndef __NOSTR__
  VarLen                  = strlen    (VarValue) + 1;
  VarIndex                = 0;
  while (Normal(*Status)
  &&     i               == 0
  &&     VarIndex         < VarLen
  &&     VarValue[0]     != '\0')
  {
      /* Compare on the word segment up to the wild char specifier         */
      if ((WildSeg    = strpbrk (WildSpec, WildOps)) != NULL)
      {
         StrSize              = (sizetype )(WildSeg - WildSpec);
         strncpy  (CompValue,    WildSpec,  StrSize);
         CompValue[StrSize]   = '\0';
         WildSpec             = &WildSeg[0];
         if ((i   = strncmp (VarValue,  CompValue, StrSize)) != 0)
            break;

         VarIndex            += StrSize;
         VarValue            += StrSize;

      } else {
         VarIndex            += strlen (WildSpec);
         i                    = strcmp (VarValue,  WildSpec);
         break;
      }

      /* If the spec is a series of ? marks                                */
      if (WildSpec[0] == '?')
      {
      /*   advance position of VarValue                                    */
         for (j = 0; WildSpec[0] == '?'; ++WildSpec)
         {
            ++j; ++VarValue;
         }
         if ((VarIndex  += j)     >= VarLen)
         {
            i = -1;
            break;
         }
         if (VarValue[0] == '\0')
         if (WildSpec[0] != '\0')
         if (WildSpec[0] != '*')
            i = -1;

      } else if (WildSpec[0] == '*') {
         /* The spec is an asteric '*'                                     */
       ++WildSpec;
         if ((CharSpec[0]     = WildSpec[0]) == '\0')
            VarIndex          = VarLen;

         else if ((VarSeg     = strpbrk (VarValue, CharSpec)) != NULL)
         {
           StrSize            = (sizetype )(VarSeg - VarValue);
           VarValue           = &VarSeg[0];
           VarIndex          += StrSize;

         } else {
           i = -1;
           VarIndex           = VarLen;
         }

      } else
        *Status  = Lex_BadSyntax;
  }
#endif

  if (ClassBug)
  if (sprintf  (Msg, "    Ut_CompareWild:: i = %4d\n", 
                i))
      TraceMsg (0, Msg);

  if (Normal (*Status))
  switch (CompOp)
  {
     case aEQ  :
         if (i   == 0)
            return(True); 
         else 
            return (False);

     case aLT  :
         if (i    < 0)
            return(True); 
         else
            return (False);

     case aGT  :
         if (i    > 0)
            return(True);
         else
            return (False);

     case aGTE :
         if (i   == 0
         ||  i    > 0)
            return(True);
         else
            return (False);

     case aLTE :
         if (i   == 0
         ||  i    < 0)
            return(True);
         else
            return (False);

     case aNEQ :
         if (i   != 0)
            return(True);
         else
            return (False);

     case aModEQ0 :
         break;
  }

TRACK(TrackBak,"Ut_CompareWild\n");
return(STAT);
}


/*     0.0.17                                                              */
int     Ut_CompareString (char         *CompValue, char         *ValuePtr)
{
int              i           = 0;
unsigned char   *p0          = (unsigned char *)CompValue;
unsigned char   *p1          = (unsigned char *)ValuePtr;

/*  Compare the string pointed to by CompValue
     against    string pointed to by ValuePtr                              */

  if (DeBug)
  if (sprintf  (Msg, "    Ut_CompareString:: <%-24s> To <%-24s>;\n", 
                p0, p1))
      TraceMsg (0, Msg);

  for (i = 0; i == 0 && *p0 != NULL && *p1 != NULL; p0++, p1++) 
  { 
     if (*p0 != *p1)
     if (*p0  < *p1)
       i      = -1;
     else
       i      = 1;
  }

  if (i    == 0)
  if (*p0  != *p1)
  if (*p0  == NULL)
     i      = -1;
  else
     i      = 1;

  if (ClassBug && !SetBug)
  if (sprintf  (Msg,  "  Ut_Compare::      <%-16s> To <%-16s>; i = %d\n", 
                CompValue, ValuePtr, i))
      TraceMsg (0, Msg);

return(i);
}

/*     0.0.18                                                              */
int     Ut_Random ( void )
{
   static unsigned int jran = 19381;

   jran=((314157*jran)+19)&0xffffff;
   return( (int) jran );
}

/*+-----------------------------------------------------------------------+
~P                         END OF UT.C                                    !
  +-----------------------------------------------------------------------+*/
