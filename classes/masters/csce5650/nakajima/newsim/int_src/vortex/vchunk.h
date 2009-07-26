/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C_Shell - VCHUNK.h */
#ifndef VCHUNK01_H
#define VCHUNK01_H

/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
/* General Definitions required by all application modules                 */

/* Tokens  to Primal and Null Dbs                                          */
extern tokentype     PrimalTkn;
extern tokentype     NullToken;

 
/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/
/* .......................................................................
                   ....   VIRTUAL CHUNK'S  ....
   .......................................................................*/

typedef struct TypeVchunk01
{
  addrtype              That;
  tokentype             Token;
  typetype              TypeType;
  handletype            TypeObj;
  sizetype              UnitSize;
  handletype            UnitChunk;
  handletype            lOwnerObject;

}  Vchunk01;


/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

#define VchunkCreate       Vchunk_Create

#define VchunkGetVstruc    Vchunk_GetVstruc

#define VchunkDumpVstruc   Vchunk_DumpVstruc

#define VchunkFreeze       Vchunk_Freeze

#define VchunkThaw         Vchunk_Thaw

#define VchunkDefrost      Vchunk_Defrost

#define VchunkSetDirty     Vchunk_SetDirty

#define VchunkSetClean     Vchunk_SetClean

#define VchunkSwapOut      Vchunk_SwapOut

#define VchunkSwapIn       Vchunk_SwapIn

#define VchunkGetAddress   Vchunk_GetAddress

#define VchunkPutAddress   Vchunk_PutAddress

#define VchunkGetStackPtr  Vchunk_GetStackPtr
#define VchunkPutStackPtr  Vchunk_PutStackPtr

#define VchunkPushUnit     Vchunk_PushUnit

#define VchunkPutUnit      Vchunk_PutUnit

#define VchunkPopUnit      Vchunk_PopUnit
#define VchunkGetUnit      Vchunk_GetUnit

#define VchunkInsertUnit   Vchunk_InsertUnit

#define VchunkExtractUnit  Vchunk_ExtractUnit

#define VchunkFindIn       Vchunk_FindIn
#define VchunkAddInto      Vchunk_AddInto

#define VchunkCopy         Vchunk_Copy

#define VchunkDump         Vchunk_Dump

#define VchunkCommit       Vchunk_Commit

#define VchunkDelete       Vchunk_Delete


#define BitFieldCreate     BitField_Create

#define BitFieldDelete     BitField_Delete

#define BitFieldPut        BitField_Put

#define BitFieldGet        BitField_Get

#define BitFieldFirstOf    BitField_FirstOf

#define BitFieldNextOf     BitField_NextOf


/*+-----------------------------------------------------------------------+
  !                 ....       VCHUNK       ....                          !
  +-----------------------------------------------------------------------+*/
/* ~   10.1.1                                                              */
       boolean Vchunk_Create
                         (tokentype    *Anchor,    sizetype      UnitSize,
                          indextype     UnitCount, indextype     ExtendQty,
                          ft F,lt Z,zz *Status,    tokentype    *Vtoken);

/* ~   10.1.1.1                                                            */
       boolean Vchunk_IsValidToken
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);

/* ~   10.1.2                                                              */
       boolean Vchunk_GetVstruc
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status, 
                          chunkstruc   *VchunkStruc);

/* ~   10.1.3                                                              */
       boolean Vchunk_DumpVstruc
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);

/* ~   10.1.4                                                              */
       boolean Vchunk_Freeze
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);


/* ~   10.1.11                                                             */
       boolean Vchunk_GetAddress
                         (tokentype    *Vtoken,    
                          ft F,lt Z,zz *Status,    addrtype     *Chunk);

/* ~   10.1.12                                                             */
       boolean Vchunk_PutAddress
                         (tokentype    *Vtoken,    addrtype      ChunkAddr,    
                          ft F,lt Z,zz *Status);

/* ~   10.1.13                                                             */
       boolean Vchunk_GetStackPtr
                         (tokentype    *Vtoken,
                          ft F,lt Z,zz *Status,    indextype    *StackPtr);

/* ~   10.1.14                                                             */
       boolean Vchunk_PutStackPtr
                         (tokentype    *Vtoken,    indextype     StackPtr,    
                          ft F,lt Z,zz *Status,    addrtype     *Varray);

/* ~   10.1.14.1                                                           */
       boolean Vchunk_GetChunkSize
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status,
                          indextype    *UnitCount, sizetype     *UnitSize);

/* ~   10.1.15                                                             */
       boolean Vchunk_Copy
                         (tokentype    *Vtoken0,   tokentype    *Vtoken1, 
                          ft F,lt Z,zz *Status);

/* ~   10.1.16.0                                                           */
       boolean Vchunk_Dump
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);

/* ~   10.1.16.1                                                           */
       boolean Vchunk_DumpPartial
                         (tokentype    *Vtoken,    
                          numtype       Ilow,      numtype       Ihigh,
                          ft F,lt Z,zz *Status);

/* ~   10.1.17                                                             */
       boolean Vchunk_Free
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);

/* ~   10.1.18                                                             */
       boolean Vchunk_Commit
                         (tokentype    *Vtoken,    boolean       Release,
                         ft F,lt Z,zz *Status);

/* ~   10.1.19                                                             */
       boolean Vchunk_Delete
                         (tokentype    *Vtoken,    ft F,lt Z,zz *Status);


/* ........................................................................
                    ....        unit        ....
   ........................................................................*/

/* ~   10.2.1                                                              */
       boolean Vchunk_PushUnit
                         (tokentype    *Vtoken,    addrtype      SomeBytes,
                          ft F,lt Z,zz *Status,    indextype    *Index);

/* ~   10.2.2                                                              */
       boolean Vchunk_PutUnit
                         (tokentype    *Vtoken,    indextype     Index,    
                          addrtype      SomeBytes, ft F,lt Z,zz *Status);

/* ~   10.2.3                                                              */
       boolean Vchunk_PopUnit
                         (tokentype    *Vtoken,
                          ft F,lt Z,zz *Status,    addrtype      SomeBytes,
                          indextype    *StackPtr);

/* ~   10.2.4                                                              */
       boolean Vchunk_GetUnit
                         (tokentype    *Vtoken,    indextype     Index,    
                          ft F,lt Z,zz *Status,    addrtype      SomeBytes);


/*+-----------------------------------------------------------------------+
  !                 ....     BIT FIELD      ....                          !
  +-----------------------------------------------------------------------+*/

/* ~   11.1.1                                                             */
       boolean BitField_Create
                         (tokentype    *Anchor,    sizetype      AllocBits,
                          ft F,lt Z,zz *Status,    tokentype    *BitFldTkn);


/* ~   11.1.3                                                             */
       boolean BitField_Put
                         (tokentype    *BitFldTkn, indextype     BitNum,
                          bytetype      BitValue,  ft F,lt Z,zz *Status);

/* ~   11.1.4                                                             */
       boolean BitField_Get
                         (tokentype    *BitFldTkn, indextype     BitNum,
                          ft F,lt Z,zz *Status,    bytetype     *BitValue);


/*+-----------------------------------------------------------------------+
~P                         END OF VCHUNK.H                                !
  +-----------------------------------------------------------------------+*/
#endif
