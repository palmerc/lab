/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef DEFINES_H
#define DEFINES_H

/* DEFINES.H  */
/*+-----------------------------------------------------------------------+
~T                            INCLUDES                                    !
*+-----------------------------------------------------------------------+*/
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define    __UNIX__
#define    __BMT01__                     /* special for BMT  ONLY!         */
#define      OPTIMIZE

#define    __SWAP_GRP_POS__
#undef     __INCLUDE_MTRX__

/*+-----------------------------------------------------------------------+
~D                             DEFINES                                    !
  +-----------------------------------------------------------------------+*/

/* ************************** DIRECTORY ********************************** */                        
/*#define Virtual_Domain */
#define  ENV01                           /* required for all except MMI    */
                                         /* For Inclusion of Schema-Driven */

#define  EXT_ENUM          999999999L    /* extends all enum's to long     */

#define  CHUNK_CONSTANT    0x55555555    /* Universal_Chunk_Constant       */
#define  CORE_CONSTANT     0x55555555    /* Universal_Core_Constant        */

#define  CORE_LIMIT        6000000L      /* Limit max size of a Core Block */
#undef   CORE_LIMIT
#undef   ALIGN_BYTES                     /* force byte alignment           */
#define  CORE_BLOCK_ALIGN  4             /* VOID blocks alignment          */
#undef   CORE_BLOCK_ALIGN

/* #undef   __RISC_64__  */              /* a long is 8 bytes              */

#undef   SYSTEM_ALLOC                    /* This switch causes all calls to
                                            alloc memory via memalloc
                                            OverRides the Core Manager     */
                           /* Initially inactive                           */
#undef   SYSTEM_FREESTORE                /* This switch causes all calls to
                                            alloc memory for Core0   
                                            (images on the free store)
                                            via memalloc                   */
                           /* Initially inactive                           */

#undef   DEBUG_CORE0                     /*                                */
                                         /* Initially inactive             */
                           /* Initially inactive                           */
#define  CORE_CRC_CHECK                  /* Put wrapper on all core blks   */
                                         /* Initially inactive             */

#define  MCSTAT                          /* sets status for traceing       */        
#define  DEBUG             True_Status   /* complete debug of runs         */
#define  __STDOUT__                      /* send output to spo             */

#undef   __MACROIZE_MEM__                /* Chrcks  Chunk_Exists(ChunkNum) */
                           /* Initially ACTIVE                             */
                           /* CHUNK SAFE when used                         */
#undef   VALIDATE_MEM_CHUNKS             /* validate end of Mem Chunk      */
                           /* Initially inactive (DEEP_DoDo)               */
                                         /* Dependent on __MACROIZE_MEM__  */

#define  __FREEZE_VCHUNKS__              /* override disk swaps for VCHUNK */

#undef   __FREEZE_GRP_PACKETS__          /* override disk swaps for GRP    */
                           /* Initially inactive                           */

#define  FLUSH_FILES                     /* use if for aborts and debug    */

#undef   __TRACK_FILE_READS__            /* trace files read count (fm)    */

#define  INCLUDE_ALL_CODE                /* Include all code               */
#define  INCLUDE_DELETE_CODE             /* Include all code for deletes   */
#undef   API_INCLUDE                     /* exclude API code               */
#define  BE_CAREFUL                      /* use UNTESTED code              */
                                         /* trans20.c                      */
#undef   OLDWAY                          /* do NOT include old way of it   */
                                         /* core.c                         */
#undef   NOTUSED                         /* do NOT include text in code    */
                                         /* core.h                         */
#undef   PAGE_SPACE                      /* do NOT include text in code    */
                                         /* core.c                         */
#undef   __NOSTR__                       /* For BCC ONLY! has no strpbrk   */

#undef   LEAVE_NO_TRACE                  /* include all trace messages     */
#undef   NULL_TRACE_STRS                 /* include trace strings          */

#undef   CLOCK_IS_LONG                   /* all except BCC                 */
#undef   __DISPLAY_TIME__                /* suppress output of clock times */

#undef   NULL                            /* redefine NULL to exact zero    */
#ifdef   LPTR
#define  NULL              0L            /* 16 BIT                         */
#else
#define  NULL              0             /* 32 BIT                         */
#endif /* LPTR  */

#define  False_Status      1             /* used for type boolean          */
#define  True_Status       0             /* used for type boolean          */

#define  LARGE                           /* for 32/64 bit compilers        */
#define  MSGFILE          -1             /* < 0 sends ouput only to MsgFile*/

#undef     __SWAP_VCHUNKS__            /* disk swaps for Vchunks         */
#undef     INCLUDE_KEYEDSET_CODE
#undef     INCLUDE_TBL_CODE
#undef     INCLUDE_DBE_CODE
#undef     FREEZE_SETNODE_CLASS
#define  UPPER_VALUE        16777215L    /* Module to seperate Grp num     */


/* ************************** END OF DIRECTORY *************************** */                        
/*+-----------------------------------------------------------------------+*/
/*                  STD     CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#undef   BE_CAREFUL                      /* do NOT include UNTESTED code   */

#define  __FREEZE_GRP_PACKETS__          /* override disk swaps for GRP    */
#define  __MINIMIZE_TREE_CACHE__         /* avert disck cacheing (tree01)  */
#undef   __TRACK_FILE_READS__            /* trace files read count (fm)    */

#undef   DEBUG_CORE0

#undef   NO_DISKCACHE                    /* does not use setvbuf()         */

#define USE_HPP_STYLE_HDRS

/*+-----------------------------------------------------------------------+*/
/*                  UNIX    CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/

#ifdef    __UNIX__                  /* For RISC systems running UNIX or NT */

#define  TIME(t1)  time((time_t *)t1)

#undef   __STDOUT__                      /* abort output to tty            */
#define  USE_H_STYLE_HDRS
#define  ALIGN_BYTES                     /* force byte alignment           */
#define  CORE_BLOCK_ALIGN  8             /* CORE blocks alignment (BYTES)  */
#define  SYSTEM_ALLOC
#define  SYSTEM_FREESTORE
#undef   NO_DISKCACHE
#define  __FREEZE_GRP_PACKETS__     /* override disk swaps for GRP & CLASS */
#define  CORE_LIMIT     10485760L   /* Limit max size of a Core Block      */

#endif /* __UNIX__ */

/*+-----------------------------------------------------------------------+*/
/*                  RISC    CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/

#define    __RISC__

#ifdef     __HP__                         /* Unique to risc architecture   */
#define ALIGN_BYTES
#endif  /* __HP__  */                     /* Unique to risc architecture   */

typedef size_t    SIZE_T;

#ifdef     __RISC_64__                    /* a long is 8 bytes             */
#define size_t    sizetype
#undef  STRUC_ALIGN_BY8
#undef  STRUC_ALIGN_BY64
#undef  IMAGE_WITH_THAT   
#else
#define STRUC_ALIGN_BY8
#undef  STRUC_ALIGN_BY64
#undef  IMAGE_WITH_THAT   
#endif /*  __RISC_64__  */                /* a long is 8 bytes             */

/*+-----------------------------------------------------------------------+*/
/*                  BMT01   CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#ifdef    __BMT01__                      /* special for BMT                */
                                         /* trans20                        */
#undef   MCSTAT
#undef   DEBUG
#define  TRACKBACK
#define  LEAVE_NO_TRACE

#undef   VALIDATE_MEM_CHUNKS             /* Ignores Vchunk Validation      */
                                         /* Dependent on __MACROIZE_MEM__  */
      /* MACROIZE must be defined for the mem chunk validion to be checked */
      /* validation occurs in Cunk_Exists().                               */

#undef   INCLUDE_ALL_CODE                /* undef code not used in BMT     */
#undef   __USER_SUPPLIED__     /* user implemented random number generator */

#endif /* __BMT01__  */

#define  RANDOM_FUNC     Ut_Random 

/*+-----------------------------------------------------------------------+*/
/*                 OPTIMIZATION CONTROLL PARAMETERS                        */
/*+-----------------------------------------------------------------------+*/

#ifdef  OPTIMIZE
#undef   DEBUG
#undef   DEBUG_CORE0
#undef   DEBUG_RISC
#undef   MCSTAT
#undef   FLUSH_FILES
#undef   __TRACK_FILE_READS__            /* do not trace files read count  */

#undef   CORE_CRC_CHECK                  /* Put wrapper on all core blks   */
#define  __MACROIZE_MEM__                /* replaces Chunk_Exists w/ macro */
#undef   VALIDATE_MEM_CHUNKS             /* validate end of Mem Chunk      */

#define __MACROIZE_HM__                /* replace 16 Hm_ procedures      */

#undef   TRACKBACK                       /* used for debugging only        */
#define  LEAVE_NO_TRACE                  /* do not include trace messages  */
#undef   NULL_TRACE_STRS                 /* used for debugging only        */
#undef   TREE_TIME
#undef   TREE_BUG
#undef   __DISPLAY_CORE__
#undef   __DISPLAY_ERRORS__
#undef   __DISPLAY_TIME__              /* do NOT output run times for SPEC */

#else

#define  DEBUG             True_Status   /* complete debug of runs         */
#undef   DEBUG_CORE0
#undef   DEBUG_RISC
#define  MCSTAT                          /* sets status for traceing       */
#define  FLUSH_FILES                     /* use if for aborts and debug    */

#define __TRACK_FILE_READS__             /* trace files read count (fm)    */


#define  CORE_CRC_CHECK                  /* Put wrapper on all core blks   */
#define  __MACROIZE_MEM__                /* calls  Chunk_Exists(ChunkNum)  */
#define  VALIDATE_MEM_CHUNKS             /* validate end of Mem Chunk      */

#define  TRACKBACK
#undef   LEAVE_NO_TRACE                  /* include all trace messages     */
#undef   NULL_TRACE_STRS                 /* include trace strings          */
#undef   TREE_TIME
#undef   TREE_BUG
#define  INCLUDE_ALL_CODE                /* Include all code               */
#undef   __DISPLAY_CORE__
#define  __DISPLAY_ERRORS__
#define  __DISPLAY_TIME__              /* do NOT output run times for SPEC */

#endif /*  __OPTIMIZE__ */

/*+-----------------------------------------------------------------------+*/
/*                  WTC     CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#ifdef    __WATCOMC__                    /* special for WATCOM compiler    */
#define __WTC__
#endif /* __WATCOMC__ */

#ifdef    __WTC__                        /* not required when EXT_ENUM     */
#define TWOBYTE_BOOL
#define ONEBYTE_BOOL
#endif /* __WTC__  */

/*+-----------------------------------------------------------------------+*/
/*                  BCC     CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#ifdef    __BORLANDC__                   /* varsys from BCC3.0 to BCC4.1   */
#define __BCC__
#endif /* __BORLANDC__ */

#ifdef   __BCC__                         /* unique to borland compiler     */
#undef  LARGE
/*
//#undef  MCSTAT
*/
#define USE_H_STYLE_HDRS
#define NULL_TRACE_STRS
#define CLOCK_IS_LONG
/*
//#define  FAR_MEM
//#define TWOBYTE_BOOL
//#undef   EXT_ENUM
//#define  EXT_ENUM         513
*/
#endif /* __BCC__ */

#ifdef    FAR_MEM                        /* required for 16 bit compiler   */
#define  GET_MEM           farcalloc
#define  FREE_MEM          farfree  
#else                                    /* USE normal  32 bit calls       */
#define  GET_MEM           calloc
#define  FREE_MEM          free  
#endif /* FAR_MEM  */

/*+-----------------------------------------------------------------------+*/
/*                  MAC     CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
/*
#define __MAC__
*/
#ifdef    __MAC__                         /* Unique to the system7.0 OS   */
#define  __GUI__
#define  __ZTC__
#define clock(void)   time(&NullTime)
#endif /* __MAC__ */

#ifdef __ZTC__
#define NO_DISKCACHE
#endif

/*+-----------------------------------------------------------------------+*/
/*                  CORE    CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#ifdef    LARGE                    /* sets page size of core alloc blocks  */
#define             CorePage_Size            384000L 
#else
#define             CorePage_Size            64000L 
#endif /* LARGE   */

 
/*+-----------------------------------------------------------------------+*/
/*                  SYSTEM  CONTROLL PARAMETERS                            */
/*+-----------------------------------------------------------------------+*/
#define  NullPtr           NULL
#define  Null_DbId         (idtype )0
#define  Null_Handle       (handletype )0
#define  Null_ObjNum       (numtype )0xFFFF
#define  Null_Token        {0,0,0}
 
#define  MAXNAME           32
#define  MAXLINE           132
#define  MAXSTRING         1024
#define  MAXPATHNAME       1024
 
#define  RECORDLENGTH      512
#define  FILENAMELENGTH    1024
 
#define  AsciiEof          NULL
#define  AsciiEob          '\n'
#define  AsciiEow          '\0'
#define  Eow               '\0'
 
/* env numtype to local numtype chunk size ::  NumEnvObjs */
#define  Env_Objs          (indextype )286
#define  Extend_DbObjNums  (indextype )50

/* local numtype to env numtype chunk size */
#define  Alloc_DbObjs      (indextype )286
#define  Extend_DbObjs     (indextype )50

/* map from Env attrnum to Local attrnum :: NumEnvAttrs */
#define  Env_Attrs         (indextype )600
#define  Alloc_Attrs       (indextype )600
#define  Extend_DbAttrNums (indextype )100
 
/* chunk of Text Tokens  */
#define  Alloc_TxtTkns     (indextype )108
#define  Extend_TxtTkns    (indextype )108
 
#define  Alloc_ObjBitFields (indextype)36
#define  Extend_BitField    (indextype)10

#define  Alloc_Grps        (indextype )64
#define  Extend_Grps       (indextype )64

/* ObjClass chunk size  :: AllocObjects */
#define  Alloc_ObjectObjs  (indextype )286
#define  Extend_Objects    (indextype )50

/* ObjClass ex objects chunk space */
#define  Alloc_ExObjects   (indextype )1001
#define  Extend_ExObjects  (indextype )1000

/* map from Env attrnum to Local attrnum :: NumEnvAttrs */
#define  Alloc_DbAttrs     (indextype )600

/* map from LocalAttrNum to ObjFieldNum */
#define  Alloc_Fields      (chunkindextype )42
#define  Extend_Fields     (chunkindextype )16

#define  Alloc_RootFields  (chunkindextype )11

/* dir of FieldNums that are RefPtrTypes */
#define  Alloc_FieldRefs   (chunkindextype )108
#define  Extend_FieldRefs  (chunkindextype )20

/*+-----------------------------------------------------------------------+*/
#define        That_AttrNum           (numtype )1
#define        Handle_AttrNum         (numtype )2
#define        DbId_AttrNum           (numtype )3
#define        CoreDbId_AttrNum       (numtype )4
#define        ObjNum_AttrNum         (numtype )5
#define        TypeNum_AttrNum        (numtype )6
#define        ValueNum_AttrNum       (numtype )7
#define        StrucNum_AttrNum       (numtype )8
#define        AttrNum_AttrNum        (numtype )9
#define        SetNum_SetNum          (numtype )10
#define        SetNum_AttrNum         (numtype )10
#define        MatrixNum_AttrNum      (numtype )11
#define        TupleNum_AttrNum       (numtype )12
#define        SlotNum_AttrNum        (numtype )13
#define        SchemaNum_AttrNum      (numtype )14
#define        ViewNum_AttrNum        (numtype )15
#define        ShellNum_AttrNum       (numtype )16
#define        CodeNum_AttrNum        (numtype )17
#define        DbNum_AttrNum          (numtype )18
#define        Nam_AttrNum            (numtype )19
#define        EnvType_AttrNum        (numtype )20

#define        UnitChunk_AttrNum      (numtype )59
 
#define        Asm_Apl                (numtype )0
#define        C_Apl                  (numtype )1
#define        Cpp_Apl                (numtype )2
#define        F77_Apl                (numtype )3
#define        Pas_Apl                (numtype )4
#define        Api_Apl                (numtype )5

/*+----------------------------------------------------------------------+*/
 
#define  Env_Normal                 0
#define  Env_CoreNoMore             1
#define  Env_OutOfMem               2
#define  Env_OryExists              3
#define  Env_BadDbPrimal            4
#define  Env_ZeroAllocQty           5
#define  Env_Less                   6
#define  Env_Gtr                    7
#define  Env_Installed             -8

#define  Env_BlackIII               9
#define  Env_EnumSizeConflict      10
#define  Env_WatchChangedValue     11
#define  Env_Break                -12

#define  Env_ObjNoHdr              13
#define  Env_BadDbId               14
#define  Env_SuppliedByShellApp    14
#define  Env_IdErrata              15

#define  Env_NullPtr               16
#define  Nuc_NullOp                17

#define  Core_SegmentSizeExceeded  18
#define  Core_RequestGtrPageSize   19
#define  Core_NotInAlignment       20
#define  Core_DuplicateFree        21
#define  Core_NULLCoreAddrOfFree   22
#define  Core_CRCoverWritten       23


#define  Mem_BitIndexGtrByteCount  30

#define  Ut_TraceLevelOutOfRange   41

#define  Lex_BadSyntax            101
#define  Lex_Eof                 -102

#define  Dbe_Exit                -200
#define  Dbe_Quit                -201

#define  Stack_EndOfStack        -300
#define  Stack_OutOfRange         301

#define  Err_BadDesign           1000
#define  Err_EmptyChunk          1001
#define  Err_NotEnufOnStack      1002
#define  Err_BadUnitType         1003
#define  Err_BadChunk            1004
#define  Err_IndexOutOfRange     1005
#define  Err_NoBytesMoved        1006
#define  Err_BadUnitSize         1007
#define  Err_BadStackMath        1008
#define  Err_NotImplemented      1009
 
#define  Mem_FoundGap           -1010
#define  Mem_NoFreeSpace         1011
#define  Info_NotExpanded        1012
#define  Mem_ChunkWasExpanded   -1013
#define  Mem_NullSwapVbn         1014
#define  Mem_ChunkCRCisBad       1015
 
#define  Core_GapFound          -1022
#define  Core_NullFreeSpacePut   1023
#define  Core_BadGap             1024
 
#define  Dump_NullObjectAddr     1030

#define  File_BadXblkWrite       1050
#define  File_BadXblkRead        1051
#define  File_EndOfFile          1052
#define  File_ChunkNotLoaded    -1053
#define  File_EmptyOutChunk      1054
#define  File_BadXblkSeek        1055
 
#define  Env_NullObjHdr          2000
#define  Env0_Corrupted          2001
#define  Env0_NullRegionQty      2002
#define  Env0_NoPrimalIntegrity  2003
#define  Env01_BadObjNumMap      2004

#define  Hm_BadHandlesStack      3000
#define  Hm_NoRoomForObject      3001
#define  Hm_ObjNotInSchema       3002
#define  Hm_BadHandle            3003
#define  Hm_ObjectNotPaired     -3004
#define  Hm_ObjectWasDeleted    -3005
#define  Hm_NoFreeHandles       -3006
#define  Hm_GtrHandleUnits       3007
 
#define  Tm_ObjectNotPaired     -4001
#define  Tm_NullDb               4002
#define  Tm_DbIdOutOfRange       4003

#define  Grp_BadGrpCount         4100
#define  Grp_GrpHdrNotExist      4101
#define  Grp_PacketNotExist      4102
#define  Grp_PacketNotLoaded     4103
#define  Grp_BaseRgnNotNew       4104
#define  Grp_BadSwapChunks       4105
#define  Grp_NullRegionChunk     4106
#define  Grp_RgnNotResident      4107
#define  Grp_IndexOutOfRange     4108
#define  Grp_NoBaseRegion        4109
#define  Grp_AllRgnsAreFrozen   -4110
#define  Grp_NullDb              4111
#define  Grp_BadExEntryTable     4112
#define  Grp_CanNotSwapBaseRgn  -4113
#define  Grp_PermaFrostReached   4114
#define  Grp_RgnNotMatched       4114

#define  Vgrp_DeletedIndex       4200
#define  Vgrp_IndexGtrCount      4201

#define  Om_ObjNotInSchema       5001
#define  Om_ObjHdrExists         5002
#define  Om_ObjHdrNotExist       5003
#define  Om_NewIndex            -5004
#define  Om_BadRegion            5005
#define  Om_NullRegionChunk      5006
#define  Om_BadAddr              5007
#define  Om_IndexOutOfRange     -5008
#define  Om_BadSwapChunks        5009
#define  Om_BadHandleObjPair     5010
#define  Om_ObjectHasBeenDeleted 5011

#define  Oa_VchunkNotExist       5053
#define  Oa_VchunkIndexTooLarge  5054
#define  Oa_ValueNoFindInVchunk  5055
#define  Oa_ValueAlreadyInVchunk 5056
#define  Oa_NullObjHdr           5059
#define  Oa_NewObjHdr            5060
#define  Oa_ObjClassNotLoaded    5063
#define  Oa_NotInCreateMode      5064
#define  Oa_VchunkNoAccess       5065
#define  Oa_ObjNotInImageForm    5066
#define  Oa_BadObjStacks         5067
#define  Oa_ObjectHasBeenDeleted 5069
#define  Oa_AttrNotInDbmsClassDef 5070
#define  Oa_FieldSizeTooLarge    5071
 
#define List01_NullNode            7001
#define List01_NoNodeMem           7002
#define List01_NotaListNode        7003
#define List01_NotaListHead        7004
#define List01_AttrNotOfObjCestors 7005
#define List01_EndOfList          -7006
#define List01_AlreadyInList       7007
#define List01_NotFound            7008
#define List01_NullListTkn         7009
#define List01_BadPrevNode         7010
#define List01_EndOfPrevList      -7011
#define List01_EmptyList          -7012
#define List01_AttrNotOfObjCestrs  7013
#define List01_InvalidListHead     7014
#define List01_GRPisFrosted        7015
#define List01_NodeNotOfSameList   7016

#define  BitVec_CoreNoMore       7021
#define  BitVec_OutOfRange       7022

#define List00_NullNode            7001
#define List00_NoNodeMem           7002
#define List00_NotaListNode        7003
#define List00_NotaListHead        7004
#define List00_AttrNotOfObjCestors 7005
#define List00_EndOfList          -7006
#define List00_AlreadyInList       7007
#define List00_NotFound            7008
#define List00_NullListTkn         7009
#define List00_BadPrevNode         7010
#define List00_EndOfPrevList      -7011
#define List00_EmptyList          -7012
#define List00_AttrNotOfObjCestrs  7013
#define List00_InvalidListHead     7014
#define List00_GRPisFrosted        7015
#define List00_DirCorrupted        7016
#define List00_DuplicateSetName    7017

#define  Set_EmptySet           -10008
#define  Set_EndOfSet           -10009
#define  Set_Found              -10010
#define  Set_NotFound            10011
#define  Set_BadSet              10012
#define  Set_AlreadyInSet        10013
#define  Set_LessThen           -10014
#define  Set_GtrThen            -10015
#define  Set_InSerted           -10016
#define  Set_IndexGtrAllocRoots  10017
#define  Set_StartOfSet         -10020
#define  Set_AddInto             10025
#define  Set_KeyFound            10026
#define  Set_NullKeyName         10027
#define  Set_OutOfRange          10028
#define  Set_IllegalKeyType      10029
#define  Set_IllegalKeyString    10030

#define  Key_UnmatchedDirIndex   10031
#define  Key_DuplicateSetName    10032

#define Set_2aryKeyExpected      10033

#define  Tree_GRPisFrosted       10040
#define  Tree_UnmatchedDirIndex  10041
#define  Tree_DuplicateSetName   10042
#define  Tree_UnmatchedParent    10043

#define  Tbl_GRPisFrosted        10050
#define  Tbl_UnmatchedDirIndex   10051
#define  Tbl_DuplicateSetName    10052


#define  Mtrx_Mated              10100
#define  Mtrx_NullRowHead        10101
#define  Mtrx_NullColHead        10102
#define  Mtrx_NullMateNode       10103
#define  Mtrx_OutOfRange         10104
#define  Mtrx_AlreadyMated       10105
#define  Mtrx_GRPisFrosted       10106
#define  Mtrx_NotMated           10107
#define  Mtrx_NullPointNode      10108
#define  Mtrx_NotInRange        -10109
#define  Mtrx_EndOfRange        -10110
#define  Mtrx_NullFirstMate      10111
#define  Mtrx_UnmatchedDirIndex  10112
#define  Mtrx_DuplicateSetName   10113
#define  Mtrx_NullMatrix         10114

#define  Matrix_RowColSetsUnMatched 10115

#define  Stack_UnmatchedDirIndex 10121
#define  Stack_DuplicateSetName  10122

#define  Vchunk_NotSameUnitSize  11100
#define  Vchunk_NullToken        11101

#define  File_BadDiskWrite       12000
#define  File_BadDiskRead        12001
#define  File_BadFilePtr         12002
#define  File_WrongAccessType    12003
#define  File_NotFound           12004
#define  File_BlksNotContiguous  12005
#define  File_WontOpen           12006
#define  File_NoName             12007
#define  File_ZeroFileBlkSize    12008
#define  File_ResolveBlkBoundary 12009
#define  File_BadDiskSeek        12010
#define  File_BytesSizeExceeded  12011
#define  File_DbWasNotCommitted  12012


#define  DBM_OvlDbHdr           -13001
#define  Dbm_ObjHdrNotExist      13002
#define  Dbm_RgnBlkNotLoaded    -13003
#define  Dbm_GrpHdrNotExist      13004
#define  Dbm_GrpNullSwapAddr     13005
#define  Dbm_NullGrpHdrDirVbn    13006
#define  Dbm_BadHandleInRegion   13007

#define  Odbm_BadSwapChunks      13011

#define  Code_NotAnObjDesc       14001
#define  Code_ClassNotMember     14013

#define  Kernl_IamNot            15000

#define  Iterate_Begin          -16000
#define  Iterate_EndOf          -16001

#define  XmemBlk_GapSpace  64
#define  Dbe_Dbe                 -202
#define  Dbe_Init                -203
#define  Oa_NullClassId          5072

/*+-----------------------------------------------------------------------+*/
 
#define  ValidAddr(Address)  (Address != PtrToVoid && Address != NullPtr)

/*+-----------------------------------------------------------------------+*/

#ifdef MCSTAT
#define  McFZ             __FILE__,__LINE__
#define  McStat           __FILE__,__LINE__,Status
#define  McStatPtr        __FILE__,__LINE__,&Status
#define  ShellMcStat      __FILE__,__LINE__,&ShellStatus
#define  ViStruc_McStat   __FILE__,__LINE__,&ViStruc->Status
#define  Dump_Status      __FILE__,__LINE__,&DumpStatus

#else
#define  McFZ             
#define  McStat           0,0,Status
#define  McStatPtr        0,0,&Status
#define  ShellMcStat      0,0,&ShellStatus
#define  ViStruc_McStat   0,0,&ViStruc->Status
#define  Dump_Status      0,0,&DumpStatus

#endif

#define  Normal(Status)   (Status == Env_Normal)

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/
#ifdef  DEBUG
#define TREE_TIME
#undef  TREE_TIME
#define TREE_BUG
#undef  __MACROIZE_MEM__ 

#endif /* DEBUG */

#ifdef MCSTAT
#ifdef DEBUG
#define  STAT (Test= *Status == 0 ? \
             Ut_VoidTrack (F, Z, Status) : Ut_PrintErr(F, Z, *Status))
#define TRACKBACK

#else
#define  STAT (Test= *Status == 0 ?  True : Ut_PrintErr(F, Z, *Status))
#endif /* DEBUG */


#ifdef TRACKBACK
#define  TRACK(string1,string2)     if(*Status) strcpy(string1,string2)

#else
#define  TRACK(string1,string2)
#endif /* TRACKBACK  */

#define  SHELL_STAT (Test= ShellStatus == 0 ? \
             True : PrintErr(__FILE__, __LINE__, ShellStatus))
#else

#ifdef  DEBUG
#define  STAT (Test= *Status == 0 ? \
               Ut_VoidTrack (NULL, 0, Status) : Ut_PrintErr(NULL, 0, *Status))
#define TRACKBACK

#else
#define  STAT (Test= *Status == 0 ?  True : Ut_PrintErr (F, Z, *Status))

#endif /* DEBUG */


#define  SHELL_STAT (Test= ShellStatus == 0 ? \
               True : False)

#endif /* MCSTAT */


#define  TEST_STAT (Test= *Status == 0 ? \
               True : Ut_PrintErr(__FILE__, __LINE__, *Status))

#ifdef TRACKBACK
#define  TRACK(string1,string2)       if(*Status) strcpy(string1,string2)

#else
#define  TRACK(string1,string2)

#endif /* TRACKBACK */

/*+-----------------------------------------------------------------------+*/
/*+-----------------------------------------------------------------------+*/

#define  NullTerminateString(str) \
 {int i=0; while((str[i]) != '@') && (str[i] != NULL)) i++; str[i] = NULL;}

/*+-----------------------------------------------------------------------+
~P                         END OF DEFINES.H                               !
  +-----------------------------------------------------------------------+*/
#endif

