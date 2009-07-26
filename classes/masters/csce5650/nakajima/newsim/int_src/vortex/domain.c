/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* c Shell - Domain.c */
#define  DOMAIN01

#define  __DISPLAY_TIME__
#ifdef    DEBUG
#define  __DISPLAY_TIME__
#endif /* DEBUG */

#include "defines.h"
#include "typedefs.h"
#ifdef   __DISPLAY_TIME__
#include <time.h>
#endif /* __DISPLAY_TIME__  */
#include "core01.h"
#include "hm.h"
#include "env0.h"
#include "dbm.h"
#include "odbm.h"
#include "env1.h"
#include "domain.h"
#include "files.h"

extern boolean Mem_AssignXmemFile
               (char         *FileName,         ft F,lt Z,zz *Status);

extern void    Mem_CloseXmemFile
               (void);

extern void    Mem_Init
               (indextype     Alloc_FreeAddrs,  indextype  Extend_FreeAddrs,
                char         *Xmem_File,
                indextype     Alloc_MemChunks,  indextype  Extend_MemChunks,
                indextype     Alloc_XmemBlks,   indextype  Extend_XmemBlks);

extern void    Mem_ShowStats (void);
extern void    Mem_DumpStats (void);

/* these Constants are defined in the initial BuildEnv run.                */
#ifndef INLINE_OMIDEFS
#include "omidefs.h"

#else
#define        WenvDbs_Hndl           (handletype )692

#endif

extern  tokentype    RenvTkn;
extern  tokentype    WenvTkn;

extern  boolean      SWAP;

/*+-----------------------------------------------------------------------+
                         RUN SPECIFIC
  +-----------------------------------------------------------------------+*/
#define DbmSetDefaults     Dbm_SetDefaults
#define DbmLoadDbHdr       Dbm_LoadDbHdr
#define DbmCommitDb        Dbm_CommitDb

extern boolean VORTEx        (ft F,lt Z,zz *Status);

extern void    Env_ShowStats (void);

extern boolean KernelCreateDb
                             (char         *SchemaNam,
                              char         *DbName,   char       *NewFileName,
                              dbaccesstype  DbAccess,
                              ft F,lt Z,zz *Status,   tokentype  *PrimalTkn);

extern boolean KernelFindIn  (handletype    SetDesc,  tokentype  *OwnerTkn,
                              addrtype      KeyValue,
                              ft F,lt Z,zz *Status,
                              tokentype    *MemberTkn);

extern boolean DbmCommitDb   (tokentype    *Anchor,   boolean    RollOut,
                              ft F,lt Z,zz *Status);
extern boolean DbmLoadDbHdr  (tokentype    *DbToken,
                              char         *LoadFileName,
                              ft F,lt Z,zz *Status,   dbheader    **CoreDb);

extern void    DbmSetDefaults
                         (indextype     EnvObjs,   indextype     EnvAttrs,
                          indextype     AllocObjs, indextype     ExtendObjs,
                          indextype     AllocAttrs,indextype     ExtendAttrs,
                          indextype     AllocHndls,indextype     ExtendHndls,
                          indextype     AllocVchunks,indextype   ExtendVchunks,
                          indextype     AllocGrps, indextype     ExtendGrps,
                          indextype     AllocTexts,indextype     ExtendTexts,
                          indextype     AllocXblks,indextype     ExtendXblks);

extern boolean OaGetObject   (tokentype    *Anchor,   
                              ft F,lt Z,zz *Status, addrtype *Object);

typedef struct AlignType11 {
      bytetype       Byte0;
      bytetype       ByteWord0;
}              align11type;

typedef struct AlignType12 {
      bytetype       Byte0;
      short          ShortWord0;
}              align12type;

typedef struct AlignType14 {
      bytetype       Byte0;
      longwordtype   LongWord0;
}              align14type;

typedef struct AlignType141 {
      bytetype       Byte0;
      longwordtype   LongWord0;
      longwordtype   Byte1;
}              align141type;

typedef struct AlignType18 {
      bytetype       Byte0;
      double         DblWord0;
}              align18type;

typedef struct AlignType482 {
      longwordtype   LongWord0;
      double         DblWord;
      short          Word1;
}              align482type;

typedef struct AlignType842 {
      double         DblWord;
      longwordtype   LongWord0;
      short          Word1;
}              align842type;

/*+-----------------------------------------------------------------------+
~C                           DOMAIN                                       !
  +-----------------------------------------------------------------------+*/
boolean  Large                   = False;

#ifdef   __DISPLAY_TIME__
static   time_t   Implode_Time  = 0;
#endif /* __DISPLAY_TIME__  */

/* .......................................................................
                    ....     Protected      ....
   .......................................................................*/

boolean Domain_ImplodeMemory
                         (ft F,lt Z,zz *Status)
{
#ifdef   __DISPLAY_TIME__
time_t          Tod        = 0;
time_t          TimeOf     = 0;
#endif /* __DISPLAY_TIME__  */

align11type     Align11;
align12type     Align12;
align14type     Align14;
align18type     Align18;
size_t          Offset     = 0;
longaddr        StartAddr  = 0;
numtype         AlignSize  = 0;

 WriteHdr         = True;
/*#define Virtual_Domain */
/* required for all except MMI    */
   TraceMsg (0, "\n  SYSTEM TYPE...\n");
#ifdef __ZTC__ 
   sprintf (Msg,  "  __ZTC__                := True \n");
#else
   sprintf (Msg,  "  __ZTC__                := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __UNIX__
   sprintf (Msg,  "  __UNIX__               := True \n");
#else
   sprintf (Msg,  "  __UNIX__               := False \n");
#endif
   TraceMsg (0, Msg);
/* a long is 8 bytes              */
#ifdef __RISC__
   sprintf (Msg,  "  __RISC__               := True \n");
#else
   sprintf (Msg,  "  __RISC__               := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __RISC_64__
   sprintf (Msg,  "  __RISC_64__            := True \n");
#else
   sprintf (Msg,  "  __RISC_64__            := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __MAC__ 
   sprintf (Msg,  "  __MAC__                := True \n");
#else
   sprintf (Msg,  "  __MAC__                := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __BCC__ 
   sprintf (Msg,  "  __BCC__                := True \n");
#else
   sprintf (Msg,  "  __BCC__                := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __BORLANDC__ 
   sprintf (Msg,  "  __BORLANDC__           := True \n");
#else
   sprintf (Msg,  "  __BORLANDC__           := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __GUI__ 
   sprintf (Msg,  "  __GUI__                := True \n");
#else
   sprintf (Msg,  "  __GUI__                := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __WTC__ 
   sprintf (Msg,  "  __WTC__                := True \n");
#else
   sprintf (Msg,  "  __WTC__                := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __HP__
   sprintf (Msg,  "  __HP__                 := True \n");
#else
   sprintf (Msg,  "  __HP__                 := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  CODE OPTIONS...\n");
#ifdef __MACROIZE_HM__
   sprintf (Msg,  "  __MACROIZE_HM__        := True \n");
#else
   sprintf (Msg,  "  __MACROIZE_HM__        := False \n");
#endif
   TraceMsg (0, Msg);
/* Chrcks  Chunk_Exists(ChunkNum) */
#ifdef __MACROIZE_MEM__
   sprintf (Msg,  "  __MACROIZE_MEM__       := True \n");
#else
   sprintf (Msg,  "  __MACROIZE_MEM__       := False \n");
#endif
   TraceMsg (0, Msg);
/* For Inclusion of Schema-Driven */
#ifdef ENV01
   sprintf (Msg,  "  ENV01                  := True \n");
#else
   sprintf (Msg,  "  ENV01                  := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef USE_HPP_STYPE_HDRS
   sprintf (Msg,  "  USE_HPP_STYPE_HDRS     := True \n");
#else
   sprintf (Msg,  "  USE_HPP_STYPE_HDRS     := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef USE_HPP_STYPE_HDRS
   sprintf (Msg,  "  USE_H_STYPE_HDRS       := True \n");
#else
   sprintf (Msg,  "  USE_H_STYPE_HDRS       := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  CODE INCLUSION PARAMETERS...\n");
/* Include all code               */
#ifdef INCLUDE_ALL_CODE
   sprintf (Msg,  "  INCLUDE_ALL_CODE       := True \n");
#else
   sprintf (Msg,  "  INCLUDE_ALL_CODE       := False \n");
#endif
   TraceMsg (0, Msg);
/* Include all code for deletes   */
#ifdef INCLUDE_DELETE_CODE
   sprintf (Msg,  "  INCLUDE_DELETE_CODE    := True \n");
#else
   sprintf (Msg,  "  INCLUDE_DELETE_CODE    := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __SWAP_GRP_POS__
   sprintf (Msg,  "  __SWAP_GRP_POS__       := True \n");
#else
   sprintf (Msg,  "  __SWAP_GRP_POS__       := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __INCLUDE_MTRX__
   sprintf (Msg,  "  __INCLUDE_MTRX__       := True \n");
#else
   sprintf (Msg,  "  __INCLUDE_MTRX__       := False \n");
#endif
   TraceMsg (0, Msg);
/* Include bad code               */
#ifdef __BAD_CODE__
   sprintf (Msg,  "  __BAD_CODE__           := True \n");
#else
   sprintf (Msg,  "  __BAD_CODE__           := False \n");
#endif
   TraceMsg (0, Msg);
/* exclude API code               */
#ifdef API_INCLUDE
   sprintf (Msg,  "  API_INCLUDE            := True \n");
#else
   sprintf (Msg,  "  API_INCLUDE            := False \n");
#endif
   TraceMsg (0, Msg);
/* use UNTESTED code              */
#ifdef BE_CAREFUL
   sprintf (Msg,  "  BE_CAREFUL             := True \n");
#else
   sprintf (Msg,  "  BE_CAREFUL             := False \n");
#endif
   TraceMsg (0, Msg);
/* do NOT include old way of it   */
#ifdef OLDWAY
   sprintf (Msg,  "  OLDWAY                 := True \n");
#else
   sprintf (Msg,  "  OLDWAY                 := False \n");
#endif
   TraceMsg (0, Msg);
/* do NOT include text in code    */
#ifdef NOTUSED
   sprintf (Msg,  "  NOTUSED                := True \n");
#else
   sprintf (Msg,  "  NOTUSED                := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  SYSTEM PARAMETERS...\n");
/* extends all enum's to long     */
   sprintf (Msg,  "  EXT_ENUM               := %4uL \n", EXT_ENUM);
   TraceMsg (0, Msg);
/* Universal_Chunk_Constant       */
   sprintf (Msg,  "  CHUNK_CONSTANT         := %8x \n", CHUNK_CONSTANT);
   TraceMsg (0, Msg);
/* Universal_Core_Constant        */
   sprintf (Msg,  "  CORE_CONSTANT          := %8x \n", CORE_CONSTANT);
   TraceMsg (0, Msg);
/* Limit max size of a Core Block */
   sprintf (Msg,  "  CORE_LIMIT             := %4u \n", CORE_LIMIT);
   TraceMsg (0, Msg);
/* Limit max size of a Core Page  */
   sprintf (Msg,  "  CorePage_Size          := %4u \n", CorePage_Size);
   TraceMsg (0, Msg);
/* force byte alignment           */
#ifdef ALIGN_BYTES
   sprintf (Msg,  "  ALIGN_BYTES            := True \n");
#else
   sprintf (Msg,  "  ALIGN_BYTES            := False \n");
#endif
   TraceMsg (0, Msg);
/* VOID blocks alignment          */
   sprintf (Msg,  "  CORE_BLOCK_ALIGN       := %4u \n", CORE_BLOCK_ALIGN);
   TraceMsg (0, Msg);
/* required for 16 bit compiler   */
#ifdef FAR_MEM    
   sprintf (Msg,  "  FAR_MEM                := True \n");
#else
   sprintf (Msg,  "  FAR_MEM                := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  MEMORY MANAGEMENT PARAMETERS...\n");
/* This switch causes all calls to   alloc memory via memalloc
                                     OverRides the Core Manager     */
#ifdef SYSTEM_ALLOC
   sprintf (Msg,  "  SYSTEM_ALLOC           := True \n");
#else
   sprintf (Msg,  "  SYSTEM_ALLOC           := False \n");
#endif
   TraceMsg (0, Msg);
/* This switch causes all calls to   alloc memory for Core0   
                                     (images on the free store)
                                     via memalloc                   */
#ifdef SYSTEM_FREESTORE
   sprintf (Msg,  "  SYSTEM_FREESTORE       := True \n");
#else
   sprintf (Msg,  "  SYSTEM_FREESTORE       := False \n");
#endif
   TraceMsg (0, Msg);

/* Regulate the setvbuf command    */
#ifdef __NO_DISKCACHE__
   sprintf (Msg,  "  __NO_DISKCACHE__       := True \n");
#else
   sprintf (Msg,  "  __NO_DISKCACHE__       := False \n");
#endif
   TraceMsg (0, Msg);
/* override disk swaps for VCHUNK */
#ifdef __FREEZE_VCHUNKS__
   sprintf (Msg,  "  __FREEZE_VCHUNKS__     := True \n");
#else
   sprintf (Msg,  "  __FREEZE_VCHUNKS__     := False \n");
#endif
   TraceMsg (0, Msg);
/* override disk swaps for GRP    */
#ifdef __FREEZE_GRP_PACKETS__
   sprintf (Msg,  "  __FREEZE_GRP_PACKETS__ := True \n");
#else
   sprintf (Msg,  "  __FREEZE_GRP_PACKETS__ := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __MINIMIZE_TREE_CACHE__
   sprintf (Msg,  "  __MINIMIZE_TREE_CACHE__:= True \n");
#else
   sprintf (Msg,  "  __MINIMIZE_TREE_CACHE__:= False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  SYSTEM STD PARAMETERS...\n");
/* send output to spo             */
#ifdef __STDOUT__
   sprintf (Msg,  "  __STDOUT__             := True \n");
#else
   sprintf (Msg,  "  __STDOUT__             := False \n");
#endif
   TraceMsg (0, Msg);
/* redefine NULL to exact zero    */
   sprintf (Msg,  "  NULL                   := %4u \n", NULL);
   TraceMsg (0, Msg);
/* NULL Ptr                       */
#ifdef LPTR
   sprintf (Msg,  "  LPTR                   := True \n");
#else
   sprintf (Msg,  "  LPTR                   := False \n");
#endif
   TraceMsg (0, Msg);
/* used for type boolean          */
   sprintf (Msg,  "  False_Status           := %4u \n", False_Status);
   TraceMsg (0, Msg);
/* used for type boolean          */
   sprintf (Msg,  "  True_Status            := %4u \n", True_Status);
   TraceMsg (0, Msg);
/* for 32/64 bit compilers        */
#ifdef LARGE
   sprintf (Msg,  "  LARGE                  := True \n");
#else
   sprintf (Msg,  "  LARGE                  := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef TWOBYTE_BOOL
   sprintf (Msg,  "  TWOBYTE_BOOL           := True \n");
#else
   sprintf (Msg,  "  TWOBYTE_BOOL           := False \n");
#endif
   TraceMsg (0, Msg);
/* For BCC ONLY! has no strpbrk   */
#ifdef __NOSTR__
   sprintf (Msg,  "  __NOSTR__              := True \n");
#else
   sprintf (Msg,  "  __NOSTR__              := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  MEMORY VALIDATION PARAMETERS...\n");
/* Put wrapper on all core blks   */
#ifdef CORE_CRC_CHECK
   sprintf (Msg,  "  CORE_CRC_CHECK         := True \n");
#else
   sprintf (Msg,  "  CORE_CRC_CHECK         := False \n");
#endif
   TraceMsg (0, Msg);
/* validate end of Mem Chunk      */
#ifdef VALIDATE_MEM_CHUNKS
   sprintf (Msg,  "  VALIDATE_MEM_CHUNKS    := True \n");
#else
   sprintf (Msg,  "  VALIDATE_MEM_CHUNKS    := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  SYSTEM DEBUG OPTIONS...\n");
/* complete debug of runs         */
#ifdef DEBUG
   sprintf (Msg,  "  DEBUG                  := True \n");
#else
   sprintf (Msg,  "  DEBUG                  := False \n");
#endif
   TraceMsg (0, Msg);
/* sets status for traceing       */        
#ifdef MCSTAT
   sprintf (Msg,  "  MCSTAT                 := True \n");
#else
   sprintf (Msg,  "  MCSTAT                 := False \n");
#endif
   TraceMsg (0, Msg);
/* complete debug of runs         */
#ifdef TRACKBACK
   sprintf (Msg,  "  TRACKBACK              := True \n");
#else
   sprintf (Msg,  "  TRACKBACK              := False \n");
#endif
   TraceMsg (0, Msg);
/* use if for aborts and debug    */
#ifdef FLUSH_FILES
   sprintf (Msg,  "  FLUSH_FILES            := True \n");
#else
   sprintf (Msg,  "  FLUSH_FILES            := False \n");
#endif

   TraceMsg (0, Msg);
#ifdef DEBUG_CORE0
   sprintf (Msg,  "  DEBUG_CORE0            := True \n");
#else
   sprintf (Msg,  "  DEBUG_CORE0            := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef DEBUG_RISC
   sprintf (Msg,  "  DEBUG_RISC             := True \n");
#else
   sprintf (Msg,  "  DEBUG_RISC             := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __TREE_BUG__
   sprintf (Msg,  "  __TREE_BUG__           := True \n");
#else                              
   sprintf (Msg,  "  __TREE_BUG__           := False \n");
#endif
   TraceMsg (0, Msg);
/* trace files read count (fm)    */
#ifdef __TRACK_FILE_READS__
   sprintf (Msg,  "  __TRACK_FILE_READS__   := True \n");
#else
   sprintf (Msg,  "  __TRACK_FILE_READS__   := False \n");
#endif
   TraceMsg (0, Msg);
/* do NOT include text in code    */
#ifdef PAGE_SPACE
   sprintf (Msg,  "  PAGE_SPACE             := True \n");
#else
   sprintf (Msg,  "  PAGE_SPACE             := False \n");
#endif
   TraceMsg (0, Msg);
/* include all trace messages     */
#ifdef LEAVE_NO_TRACE
   sprintf (Msg,  "  LEAVE_NO_TRACE         := True \n");
#else
   sprintf (Msg,  "  LEAVE_NO_TRACE         := False \n");
#endif
   TraceMsg (0, Msg);
/* include trace strings          */
#ifdef NULL_TRACE_STRS
   sprintf (Msg,  "  NULL_TRACE_STRS        := True \n");
#else
   sprintf (Msg,  "  NULL_TRACE_STRS        := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  TIME PARAMETERS...\n");
/* all except BCC                 */
#ifdef CLOCK_IS_LONG
   sprintf (Msg,  "  CLOCK_IS_LONG          := True \n");
#else
   sprintf (Msg,  "  CLOCK_IS_LONG          := False \n");
#endif
   TraceMsg (0, Msg);
/* suppress output of clock times */
#ifdef __DISPLAY_TIME__
   sprintf (Msg,  "  __DISPLAY_TIME__       := True \n");
#else
   sprintf (Msg,  "  __DISPLAY_TIME__       := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef __TREE_TIME__
   sprintf (Msg,  "  __TREE_TIME__          := True \n");
#else
   sprintf (Msg,  "  __TREE_TIME__          := False \n");
#endif
   TraceMsg (0, Msg);
/* suppress output of clock times */
#ifdef __DISPLAY_ERRORS__
   sprintf (Msg,  "  __DISPLAY_ERRORS__     := True \n");
#else
   sprintf (Msg,  "  __DISPLAY_ERRORS__     := False \n");
#endif
   TraceMsg (0, Msg);

   TraceMsg (0, "\n  API MACROS...\n");
#ifdef __BMT01__
   sprintf (Msg,  "  __BMT01__              := True \n");
#else
   sprintf (Msg,  "  __BMT01__              := False \n");
#endif
   TraceMsg (0, Msg);
#ifdef OPTIMIZE
   sprintf (Msg,  "  OPTIMIZE               := True \n");
#else
   sprintf (Msg,  "  OPTIMIZE               := False \n");
#endif
   TraceMsg (0, Msg);
   TraceMsg (0, "\n  END OF DEFINES.\n\n");
 WriteHdr         = False;


   SendMsg  (0, "\n\n              ...   IMPLODE MEMORY ...\n");

#ifdef __DISPLAY_TIME__
   Implode_Time = clock();
   TimeOf       = time (&Tod);
   if (sprintf  (Msg, "   time stamp= [%lu] :: %s\n",
                 (lt64 )Implode_Time, ctime (&TimeOf)))
    TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__  */


   Large       = False;
   Build_Env   = False;
   Admin_Run   = False;
   Reload_Wenv = True;
   FileBug     = False;

/* Set Enviornment parameters based on 16/32 bit && noEms/Ems              */
/* Boundary Alignment Byte Size                                            */
   Align_Bytes = False;

#ifdef ALIGN_BYTES
   Align_Bytes = True;
#endif /* ALIGN_BYTES */

   StrucAlignment         = 1;     

   AlignSize = sizeof(align14type);
   if (AlignSize != (sizeof (bytetype)     + sizeof (longwordtype)))
      Align_Bytes = True;

   if (Align_Bytes) 
   {
      AlignSize   = sizeof(align141type);
      if (AlignSize      == 10)
         StrucAlignment         = 2;
      else if (AlignSize == 12)
         StrucAlignment         = 4;
      else if (AlignSize == 16)
         StrucAlignment         = 8;

      StartAddr   = (longaddr)&Align11;
      Offset      = (size_t)((longaddr )&Align11.ByteWord0  -  StartAddr);
      Byte_Alignment         = Offset;

      StartAddr   = (longaddr)&Align12;
      Offset      = (size_t)((longaddr )&Align12.ShortWord0 -  StartAddr);
      Byte2Alignment         = Offset;

      StartAddr   = (longaddr)&Align14;
      Offset      = (size_t)((longaddr )&Align14.LongWord0  -  StartAddr);
      Byte4Alignment         = Offset;

      StartAddr   = (longaddr)&Align18;
      Offset      = (size_t)((longaddr )&Align18.DblWord0   -  StartAddr);
      Byte8Alignment         = Offset;

      BytePtrAlignment       = sizeof (addrtype);

   } else {
      StrucAlignment         = 1;     

      Byte_Alignment         = 1;     
      Byte2Alignment         = 1;     
      Byte4Alignment         = 1;     
      Byte8Alignment         = 1;     

      BytePtrAlignment       = 1;
   }

   if (Align_CppStruc) 
      CppAlignment           = 4;     
   else
      CppAlignment           = 0;     

#ifdef DEBUG
   WriteHdr = True;
   sprintf (Msg, " Byte Alignment :: %u byte boundary\n", Byte_Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(1)  Alignment :: %u byte boundary\n", Byte_Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(2)  Alignment :: %u byte boundary\n", Byte2Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(4)  Alignment :: %u byte boundary\n", Byte4Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(8)  Alignment :: %u byte boundary\n", Byte8Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(*p) Alignment  :: %u byte boundary\n", BytePtrAlignment);
   SendMsg (0, Msg);
   sprintf (Msg, " Struc    Alignment :: %u byte boundary\n", StrucAlignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Add for THAT       :: %u bytes      \n\n", CppAlignment);
   TraceMsg (0, Msg);

   TraceMsg (0,   "\n STRUCT alignment with <double> as 2nd member\n");
   sprintf (Msg, " Alignment 482   :: %u\n", sizeof (align482type));
   TraceMsg (0, Msg);

   TraceMsg (0,   " STRUCT alignment with <double> as 1st member\n");
   sprintf (Msg, " Alignment 842   :: %u\n", sizeof (align482type));
   TraceMsg (0, Msg);
   WriteHdr = False;
#endif /* DEBUG */

   sprintf (Msg, "\n  SWAP to DiskCache := %s\n", BoolStr[SWAP]);
   SendMsg (0, Msg);

   sprintf (Msg, "\n  FREEZE_GRP_PACKETS:= %s\n", BoolStr[FREEZE_GRP_PACKETS]);
   SendMsg (0, Msg);

   sprintf (Msg, "\n  QueBug            := %u\n", QueBug);
   SendMsg (0, Msg);

#ifdef __RISC__                       /* OVERRIDE IF NECESSARY             */
#ifdef __RISC_64__
      StrucAlignment         = 8;     
#else
      StrucAlignment         = 4;     
#endif /* __RISC_64__ */

      Byte_Alignment         = 1;     
      Byte2Alignment         = 2;     
      Byte4Alignment         = 4;     
      Byte8Alignment         = 8;     
      BytePtrAlignment       = sizeof (addrtype);

      CppAlignment           = 0;     

   WriteHdr = True;
#ifdef DEBUG
   TraceMsg(0, "\n    *** BYTE ALIGNMENT OVERRIDE ***\n");
   sprintf (Msg, " Byte Alignment :: %u byte boundary\n", Byte_Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(1)  Alignment :: %u byte boundary\n", Byte_Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(2)  Alignment :: %u byte boundary\n", Byte2Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(4)  Alignment :: %u byte boundary\n", Byte4Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(8)  Alignment :: %u byte boundary\n", Byte8Alignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Byte(*p) Alignment  :: %u byte boundary\n", BytePtrAlignment);
   SendMsg (0, Msg);
   sprintf (Msg, " Struc    Alignment :: %u byte boundary\n", StrucAlignment);
   TraceMsg (0, Msg);
   sprintf (Msg, " Add for THAT       :: %u bytes      \n\n", CppAlignment);
   TraceMsg (0, Msg);
   WriteHdr = False;
#endif /* __RISC__ */

 sprintf (Msg, "\n  sizeof(boolean)      = %2u\n", sizeof (boolean));
   SendMsg (MSGFILE, Msg);

#ifdef TWOBYTE_BOOL
   sprintf (Msg, "  sizeof(twobytetype)  = %2u\n", sizeof (twobytetype));
   SendMsg (MSGFILE, Msg);
#endif /* TWO_BYTE_BOOL */

   sprintf (Msg, "  sizeof(sizetype)     = %2u\n", sizeof (sizetype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(chunkstruc)   = %2u\n", sizeof (chunkstruc));
   SendMsg (MSGFILE, Msg);

 sprintf (Msg, "\n  sizeof(shorttype )   = %2u\n", sizeof (shorttype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(idtype    )   = %2u\n", sizeof (idtype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(sizetype  )   = %2u\n", sizeof (sizetype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(indextype )   = %2u\n", sizeof (indextype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(numtype   )   = %2u\n", sizeof (numtype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(handletype)   = %2u\n", sizeof (handletype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(tokentype )   = %2u\n", sizeof (tokentype));
   SendMsg (MSGFILE, Msg);

 sprintf (Msg, "\n  sizeof(short     )   = %2u\n", sizeof (short));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(int       )   = %2u\n", sizeof (int));
   SendMsg (MSGFILE, Msg);

 sprintf (Msg, "\n  sizeof(lt64      )   = %2u\n", sizeof (lt64));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(farlongtype)  = %2u\n", sizeof (farlongtype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(long      )   = %2u\n", sizeof (long));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(longaddr  )   = %2u\n", sizeof (longaddr));
   SendMsg (MSGFILE, Msg);

 sprintf (Msg, "\n  sizeof(float     )   = %2u\n", sizeof (float));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(double    )   = %2u\n", sizeof (double));
   SendMsg (MSGFILE, Msg);

 sprintf (Msg, "\n  sizeof(addrtype  )   = %2u\n", sizeof (addrtype));
   SendMsg (MSGFILE, Msg);
   sprintf (Msg, "  sizeof(char *    )   = %2u\n", sizeof (char *));
   SendMsg (MSGFILE, Msg);
#endif /* DEBUG */

   VORTEx (McStat);

TRACK(TrackBak,"Domain_ImplodeMemory\n");
return (STAT);
}


boolean Domain_BeHereNow
                         (ft F,lt Z,zz *Status)
{

indextype     EnvObjs      = Env_Objs;
indextype     EnvAttrs     = Env_Attrs; 
indextype     AllocObjs    = Alloc_DbObjs;
indextype     ExtendObjs   = Extend_DbObjs;
indextype     AllocAttrs   = Alloc_DbAttrs;
indextype     ExtendAttrs  = Extend_DbAttrNums;
indextype     AllocHndls   = Alloc_Handles;
indextype     ExtendHndls  = Extend_Handles;
indextype     AllocVchunks = Alloc_Vchunks;
indextype     ExtendVchunks= Extend_Vchunks;
indextype     AllocGrps    = Alloc_Grps;
indextype     ExtendGrps   = Extend_Grps;
indextype     AllocTexts   = Alloc_TxtTkns;
indextype     ExtendTexts  = Extend_TxtTkns;
indextype     AllocXblks   = DbXmemBlocks_AllocQty;
indextype     ExtendXblks  = DbXmemBlocks_ExtendQty;
tokentype     AddrTypeTkn  = NullToken;
typedesctype *EnvType      = NullPtr;

 TraceMsg (0, "\n    BE HERE NOW !!!\n\n");
 DbmSetDefaults  (EnvObjs,       EnvAttrs,      AllocObjs,     ExtendObjs,
                  AllocAttrs,    ExtendAttrs,   AllocHndls,    ExtendHndls,
                  AllocVchunks,  ExtendVchunks, AllocGrps,     ExtendGrps, 
                  AllocTexts,    ExtendTexts,   AllocXblks,    ExtendXblks);

/* load  RENV  and  WENV                                                   */

 if (*Status == Env_Installed)
 {
    *Status          = Env_Normal;
    if (Env0->DbCount == RenvId)
    {
       if (KernelCreateDb  
                       (WenvSchemaNam,  WenvNam,  WenvFileName,  Shared_Write, 
                        McStat,        &WenvTkn))
#ifdef   __FREEZE_VCHUNKS__
           ;
#else
           DbmCommitDb (&WenvTkn,       False,    McStat);

#endif /* __FREEZE_VCHUNKS__  */

    } else {
       if (CoreDbs[WenvId] == NullPtr)
           DbmLoadDbHdr (&WenvTkn, WenvFileName, McStat, &CoreDbs[WenvId]);

    }

 }

 WriteHdr         = False;

 if (Normal(*Status))
     SendMsg  (0, "\n\n               ... VORTEx ON LINE ...\n");

 if ( Normal(*Status) )
 {
    AddrTypeTkn = RenvTkn ;
    AddrTypeTkn.Handle = AddrType_Hndl ;
    if ( OaGetObject(&AddrTypeTkn, McStat, (addrtype *) &EnvType))
       EnvType->Size = sizeof( addrtype ) ;
 }

TRACK(TrackBak,"Domain_BeHereNow\n");
return (STAT);
}


/* .......................................................................
                    ....     Public         ....
   .......................................................................*/

boolean Domain_MmiInvoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Xmem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          ft F,lt Z,zz *Status)
{
   /* Arg  1.  InFile<.Dat file>                                            */
   strcpy      (InFileName,  Dat_File);
   if ((InFilePtr = fopen(Dat_File, "r")) == NULL)  {
      fprintf (stderr, "%s:  can't open PERSONS data file\n",  Dat_File);
      exit(1);
   }
   /* Arg  2.  MsgFile<.Msg file>                                            */
   if ( (MsgFilePtr = fopen (Msg_File, "w")) == NULL)  {
      fprintf (stderr, "***ERROR... Can't open MSG file <%s>\n", Msg_File);
      exit(1); 
   }
   /* Arg  3.  OutFile<.Out file>                                            */
   if ( (OutFilePtr = fopen (Out_File, "w")) == NULL)  {
      fprintf (stderr, "***ERROR... Can't open OUT file <%s>\n", Out_File);
      exit(1); 
   }

/* Arg  5.  Core Page Size assignment                                     */
   Core_SetPageSize      (CorePageSize,         CoreMaxOnPage);

/* Arg  4.  SwapFile                                                      */
/* Arg  7.  Core Chunks assignment                                        */
/* Arg  9.  Mem  Chunks assignment                                        */
/* Arg 11.  Xmem Chunks assignment                                        */

   Mem_Init              (AllocFreeAddrs,       ExtendFreeAddrs,
                          Xmem_File,
                          AllocChunks,          ExtendChunks,
                          AllocXmemBlocks,      ExtendXmemBlocks);

   Domain_ImplodeMemory  (McStat);

TRACK(TrackBak,"Domain_MmiInvoke\n");
return (STAT);
}



boolean Domain_Omi0Invoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Xmem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          indextype     AllocHandles,     indextype     ExtendHandles,
          indextype     AllocVchunks,     indextype     ExtendVchunks,
          indextype     AllocGrps,        indextype     ExtendGrps,
          indextype     AllocDbBlocks,    indextype     ExtendDbBlocks,
          char         *ParmsBuf,         ft F,lt Z,zz *Status)
{
   Domain_OmiInvoke
         (Dat_File,         Msg_File,  
          Out_File,         Xmem_File,  
          CorePageSize,     CoreMaxOnPage,
          AllocFreeAddrs,   ExtendFreeAddrs,
          AllocChunks,      ExtendChunks,
          AllocXmemBlocks,  ExtendXmemBlocks,
          AllocHandles,     ExtendHandles,
          AllocVchunks,     ExtendVchunks,
          AllocGrps,        ExtendGrps,
          AllocDbBlocks,    ExtendDbBlocks,
          McStat);

   TraceMsg(0, ParmsBuf);

TRACK(TrackBak,"Domain_Omi0Invoke\n");
return (STAT);
}

boolean Domain_OmiInvoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Xmem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          indextype     AllocHandles,     indextype     ExtendHandles,
          indextype     AllocVchunks,     indextype     ExtendVchunks,
          indextype     AllocGrps,        indextype     ExtendGrps,
          indextype     AllocDbBlocks,    indextype     ExtendDbBlocks,
          ft F,lt Z,zz *Status)
{
tokentype    DbTkn  = NullToken;

   /* Arg  1.  InFile<.Dat file>                                            */
   strcpy      (InFileName,  Dat_File);
   if ((InFilePtr = fopen(Dat_File, "r")) == NULL)  {
      fprintf (stderr, "%s:  can't open PERSONS data file\n",  Dat_File);
      exit(1); 
   }

   /* Arg  2.  MsgFile<.Msg file>                                            */
   if ( (MsgFilePtr = fopen (Msg_File, "w")) == NULL)  {
      fprintf (stderr, "***ERROR... Can't open MSG file <%s>\n", Msg_File);
      exit(1); 
   }

   /* Arg  3.  OutFile<.Out file>                                            */
   if ( (OutFilePtr = fopen (Out_File, "w")) == NULL)  {
      fprintf (stderr, "***ERROR... Can't open OUT file <%s>\n", Out_File);
      exit(1);
   }

/* Arg  5.  Core Page Size assignment                                     */
   Core_SetPageSize      (CorePageSize,         CoreMaxOnPage);

/* Arg  4.  SwapFile                                                      */
/* Arg  7.  Core Chunks assignment                                        */
/* Arg  9.  Mem  Chunks assignment                                        */
/* Arg 11.  Xmem Chunks assignment                                        */

   Hm_SetDRIswi          (False);

   Mem_Init              (AllocFreeAddrs,       ExtendFreeAddrs,
                          Xmem_File,
                          AllocChunks,          ExtendChunks,
                          AllocXmemBlocks,      ExtendXmemBlocks);

/* Arg 13.  Db Handels Chunk assignment                                   */
/* Arg 15.  Db Vchunks assignment                                         */
/* Arg 17.  DbBlocks assignment                                           */
   Alloc_Handles           = AllocHandles;
   Extend_Handles          = ExtendHandles;

   Alloc_Vchunks           = AllocVchunks;
   Extend_Vchunks          = ExtendVchunks;

   Alloc_Grps              = AllocGrps;
   Extend_Grps             = ExtendGrps;

   DbXmemBlocks_AllocQty   = AllocDbBlocks;
   DbXmemBlocks_ExtendQty  = ExtendDbBlocks;

   Alloc_TxtTkns           = 0;
   Extend_TxtTkns          = 0;

 if (Domain_ImplodeMemory   (McStat))

 if (Domain_BeHereNow       (McStat))
 {
/* Load/Install PRIMAL                                                     */ 
    if (KernelFindIn     (WenvDbs_Hndl,   &WenvTkn,    (addrtype )PrimalNam,
                          McStat,         &DbTkn))
    {
      PrimalTkn.DbId   = PrimalId;
      PrimalTkn.Handle = 1;
      DbmLoadDbHdr       (&PrimalTkn,      PrimalFileName, 
                          McStat,         &CoreDbs[PrimalId]);
    } else {
       *Status = Env_Normal;
        if (KernelCreateDb
                         (PrimalSchemaNam, PrimalNam,   PrimalFileName, 
                          Read_Write,
                          McStat,         &PrimalTkn))
#ifdef   __FREEZE_VCHUNKS__
           ;
#else
           DbmCommitDb   (&PrimalTkn,      False,       McStat);
#endif /* __FREEZE_VCHUNKS__  */
    }

/* Access the KingDom from  Command Line Mode                              */
    if (Normal(*Status))
        EnvMakeCodeChunks (McStat);
 }

TRACK(TrackBak,"Domain_OmiInvoke\n");
return (STAT);
}


boolean Domain_Enter
                         (char         *Subject,   keydomtype    KeyToRealm,
                          ft F,lt Z,zz *Status)
{
tokentype    DbTkn  = NullToken;

indextype    AllocFreeAddrs    = 2806;
indextype    ExtendFreeAddrs   = 1806;

indextype    AllocChunks       = 10402;
indextype    ExtendChunks      = 10002;

indextype    AllocXmemBlocks   = 1004;
indextype    ExtendXmemBlocks  = 1004;

sizetype     BoolSize          = sizeof (boolean);
    
/* ENTER THE DOMAIN OF THE REALM                                           */

 if (BoolSize != 2 || Large)
 {
      AllocChunks          = 20402;

      ExtendChunks         = 10002;

      AllocXmemBlocks      = 4004;
      ExtendXmemBlocks     = 2004;

      AllocFreeAddrs       = 2806;
      ExtendFreeAddrs      = 1806;

 } else {
      AllocChunks          = 10402;

      ExtendChunks         = 10002;

      AllocXmemBlocks      = 1004;
      ExtendXmemBlocks     = 1004;

      AllocFreeAddrs       = 2806;
      ExtendFreeAddrs      = 1806;

 }
 Core_SetPageSize      (CorePage_Size, CorePage_Size/2);

 Mem_Init              (AllocFreeAddrs,       ExtendFreeAddrs,
                        XmemFile,
                        AllocChunks,          ExtendChunks,
                        AllocXmemBlocks,      ExtendXmemBlocks);

 if (Large) 
 {
      Alloc_Handles         = 20001;
      Extend_Handles        = 10001;

      Alloc_Vchunks         = 14003;
      Extend_Vchunks        = 10003;

      DbXmemBlocks_AllocQty = 8010;
      DbXmemBlocks_ExtendQty= 10010;

 } else {
      Alloc_Handles         = 10401;
      Extend_Handles        = 10001;

      Alloc_Vchunks         = 20403;
      Extend_Vchunks        = 10003;

      DbXmemBlocks_AllocQty = 6010;
      DbXmemBlocks_ExtendQty= 10010;
 }

 if (Domain_ImplodeMemory   (McStat))

 if (Domain_BeHereNow       (McStat))
 if (sprintf  (Msg, 
    "\n\n               ... VORTEx ON LINE ... Subject <%-12s> Key=%3u\n",
               Subject, KeyToRealm)) 
 if (SendMsg  (0, Msg))
 {
/* Load/Install PRIMAL                                                     */ 
    if (KernelFindIn     (WenvDbs_Hndl,   &WenvTkn,    (addrtype )PrimalNam,
                          McStat,         &DbTkn))
    {
      PrimalTkn.DbId   = PrimalId;
      PrimalTkn.Handle = 1;
      DbmLoadDbHdr       (&PrimalTkn,      PrimalFileName, 
                          McStat,         &CoreDbs[PrimalId]);
    } else {
       *Status = Env_Normal;
        if (KernelCreateDb
                         (PrimalSchemaNam, PrimalNam,   PrimalFileName, 
                          Read_Write,
                          McStat,         &PrimalTkn))
#ifdef   __FREEZE_VCHUNKS__
           ;
#else
           DbmCommitDb   (&PrimalTkn,      False,       McStat);
#endif /* __FREEZE_VCHUNKS__  */
    }

/* Access the KingDom from  Command Line Mode                              */
    if (Normal(*Status))
        EnvMakeCodeChunks (McStat);
 }

TRACK(TrackBak,"Domain_EnterDomain\n");
return (STAT);
}



boolean Domain_Exit
                         (ft F,lt Z,zz *Status)
{
/* EXIT THE DOMAIN OF THE REALM                                            */
#ifdef   __DISPLAY_TIME__
time_t    Tod        = 0;
time_t    TimeOf     = 0;
time_t    Elapsed    = 0;
float     Seconds    = 0;

time_t    WenvTime   = 0;
time_t    CurrTime   = 0;
double    DblTime    = 0.0;
#endif /* __DISPLAY_TIME__  */

int       i          = 0;

   if (*Status == Dbe_Exit)
       *Status  = Env_Normal;

/*    Commit WENV                                                          */
   if (Normal(*Status))
   {

#ifdef   __DISPLAY_TIME__
#ifndef     __BMT01__
      time (&WenvTime);
#endif /* __BMT01__  */
#endif /* __DISPLAY_TIME__  */
  
      DbmCommitDb           (&PrimalTkn,    True,    McStat);
      DbmCommitDb           (&WenvTkn,      True,    McStat);
      DbmFreeDb             (&RenvTkn,      McStat);

#ifndef   __BMT01__
      DblTime    = difftime (TIME(&CurrTime), WenvTime);
      sprintf  (Msg, "\n Commit PRIMAL, WENV & RENV DB's: %.0f sec\n",
                DblTime);
      TraceMsg (0,   Msg);
#else
      TraceMsg (0, "\n PRIMAL, WENV & RENV DB's committed.\n");

#endif /* __BMT01__  */
   }
   if (Normal(*Status))
   {
      TraceMsg (0, "\n\n         Env01 Memory Stats\n\n");
      CoreShowStats  ();
      Mem_ShowStats  ();
      Mem_DumpStats  ();
   }
/* else
      CoreShowStats  ();
*/
   SendMsg  (0, "\n\n              ...   END OF SESSION ...\n");

#ifdef __DISPLAY_TIME__
   Elapsed    = clock() - Implode_Time;
   Seconds    = (float )Elapsed / CLOCKS_PER_SEC;
   sprintf  (Msg, "\n RUN TIME = %4.2f\n\n", 
             Seconds);
   TraceMsg (MSGFILE,   Msg);

   TimeOf = time (&Tod);

   if (sprintf  (Msg, "   time stamp= [%lu] :: %s;       STATUS= %d\n", 
                 (lt64 )TimeOf, ctime (&TimeOf), *Status))
      TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__  */

   sprintf  (Msg, "   STATUS= %d\n", *Status);
   TraceMsg   (0, Msg);

   for (i=0; i++ < 5; sprintf (Msg, "V O R T E x 0 1!"), TraceMsg(0,Msg)) {};
   TraceMsg   (0,          "\n");

   StackTrack ("Domain");

   fclose     (OutFilePtr);
   fclose     (MsgFilePtr);

   if (SWAP)
      Mem_CloseXmemFile ();

   if (DiskFilePtr)
      fclose  (DiskFilePtr);


TRACK(TrackBak,"Domain_ExitDomain\n");
return (STAT);
}
 
/*+-----------------------------------------------------------------------+
~M                         END OF DOMAIN.C                                !
  +-----------------------------------------------------------------------+*/

