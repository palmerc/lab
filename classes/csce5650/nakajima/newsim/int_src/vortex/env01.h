/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C_Shell - CSH.h */
#ifndef CSH01_H
#define CSH01_H

#ifndef IMAGE01_HPP

#include "envdefs.h"
/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/

/* ........................................................................
#ifndef ENV01_H
#ifndef CTRANS01_H

typedef enum   TypeEnvClass
               {aEnvDb,   aEnvObj,    aEnvType,   aEnvStruc,  aEnvAttr,
                aEnvSet,  aEnvMatrix, aEnvPair,   aEnvTuple,  aEnvSchema,
                aEnvView, aEnvCode,   aEnvHead,   aEnvNode,   aEnvSlot,
                aEnvValue,aEnvArg,    aEnvPort,   aEnvShell     } envclass;

#endif
#endif

 ..........................................................................*/

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/
#define  EnvShowStats              Env_ShowStats
#define  EnvGetClassId             Env_GetClassId 

#define  EnvGetMemberId            Env_GetAttrId 

#define  EnvGetSetId               Env_GetSetId 
#define  EnvInitClassMap           Env_InitClassMap 
#define  EnvAppendToMap            Env_AppendToMap 
#define  EnvInvokeMap              Env_InvokeMap
#define  EnvGetClassMap            Env_GetClassMap
#define  EnvGetApiToken            Env_GetApiToken
#define  EnvGetMapId               Env_GetApiMapId 
#define  EnvGetFieldId             Env_GetApiFieldId 
#define  EnvFetchTransPortLib      Env_FetchApiPortLib
#define  EnvLoadPortCode           Env_LoadApiPortCode
#define  EnvImportDb               Env_ImportDb
#define  EnvExportDb               Env_ExportDb

#ifdef ENV01_CSH
       tokentype      Random_Tkn            = {31, 2, 0};
       int           *Random_Dir            = NullPtr;
       indextype      Random_Limit          = 0;
       indextype      Random_ItNum          = 0;

       numtype      WenvGroups_Hndl         = 0;
       numtype      WenvSubjects_Hndl       = 0;
       numtype      WenvCodes_Hndl          = 0;

       numtype      GroupId_AttrNum         = 0;
       numtype      SubjectId_AttrNum       = 0;
       numtype      CodeId_AttrNum          = 0;
       numtype      AplNum_AttrNum          = 0;
       numtype      ApiNum_AttrNum          = 0;
       numtype      MapNum_AttrNum          = 0;
       numtype      FieldNum_AttrNum        = 0;
       numtype      MapIndex_AttrNum        = 0;

       numtype      TransMapNames_Hndl      = 0;
       numtype      TransPortMaps_Hndl      = 0;
       numtype      TransFieldNames_Hndl    = 0;
       numtype      TransPortFields_Hndl    = 0;
       numtype      PortMapFields_Hndl      = 0;
       numtype      PrimalGroupSlotAttr_Attr = 0;
#else
extern tokentype      Random_Tkn;
extern int           *Random_Dir;
extern indextype      Random_Limit;
extern indextype      Random_ItNum;

extern numtype      WenvGroups_Hndl;
extern numtype      WenvSubjects_Hndl;
extern numtype      WenvCodes_Hndl;

extern numtype      GroupId_AttrNum;
extern numtype      SubjectId_AttrNum;
extern numtype      CodeId_AttrNum;
extern numtype      AplNum_AttrNum;
extern numtype      ApiNum_AttrNum;
extern numtype      MapNum_AttrNum;
extern numtype      FieldNum_AttrNum;
extern numtype      MapIndex_AttrNum;

extern numtype      TransMapNames_Hndl;
extern numtype      TransPortMaps_Hndl;
extern numtype      TransFieldNames_Hndl;
extern numtype      TransPortFields_Hndl;
extern numtype      PortMapFields_Hndl;
extern numtype      PrimalGroupSlotAttr_Attr;

#endif


#endif /* IMAGE01_HPP  */

/*+-----------------------------------------------------------------------+
                    ....        ENV01        ....
  +-----------------------------------------------------------------------+*/

/* ~   2.1.2                                                               */
       boolean Env_GetTypeId 
                         (char         *TypeName,  
                          ft F,lt Z,zz *Status,    numtype      *TypeId);


/* ~   2.1.3                                                               */
       boolean Env_GetClassId 
                         (char         *ClassName, 
                          ft F,lt Z,zz *Status,    numtype      *ClassId);


/* ~   2.1.4                                                               */
       boolean Env_GetAttrId 
                         (char         *AttrName,  
                          ft F,lt Z,zz *Status,    numtype      *AttrId);

/* ~   2.1.5                                                               */
       boolean Env_NewFieldStruc
                         (char         *FieldName, 
                          ft F,lt Z,zz *Status,    fieldstruc  **FieldStruc);
/* ~   2.1.5                                                               */
       boolean Env_GetFieldStruc
                         (char         *FieldName, 
                          ft F,lt Z,zz *Status,    fieldstruc   *FieldStruc);

/* ~   2.1.5                                                               */
       boolean Env_DeleteFieldStruc
                         (ft F,lt Z,zz *Status,    fieldstruc  **FieldStruc);

/* ~   2.1.5                                                               */
       boolean Env_GetSetId 
                         (char         *SetName,   
                          ft F,lt Z,zz *Status,    numtype      *SetId);

/*     2.1.6                                                               */
       boolean Env_GetTupleId 
                         (char         *TupleName, 
                          ft F,lt Z,zz *Status,    numtype      *TupleId);

/*     2.1.7                                                               */
       boolean Env_GetMatrixId 
                         (char         *MatrixName,
                          ft F,lt Z,zz *Status,    numtype      *MatrixId);


/*     2.1.15                                                              */
       boolean Env_LoadCreateCode
                         (numtype       ClassId,    funcptrtype   CodePtr,
                          ft F,lt Z,zz *Status);

/*     2.1.15                                                              */
       boolean Env_LoadGetTknCode
                         (numtype       ClassId,    funcptrtype   CodePtr,
                          ft F,lt Z,zz *Status);

/* ........................................................................
                    ....     Meta Class     ....
   ........................................................................*/

/* ~   2.2.1                                                               */
       boolean Env_InitClassMap 
                         (numtype       ClassId,   size_t        ObjSize,
                          numtype       AplId,     ft F,lt Z,zz *Status);

/* ~   2.2.2                                                               */
       boolean Env_AppendToMap 
                         (numtype       AttrId,    size_t        Offset,
                          ft F,lt Z,zz *Status);

/* ~   2.2.3                                                               */
       boolean Env_InvokeMap
                         (numtype       AplId,     ft F,lt Z,zz *Status);

/* ~   2.2.4                                                               */
       boolean Env_GetClassMap                                             
                         (numtype       ClassId,   numtype       AplId,
                          ft F,lt Z,zz *Status,    classmaptype *ClassMap);


/* ~   2.2.6                                                               */
       boolean Env_GetAttrInfo
                         (numtype       AttrId,
                          ft F,lt Z,zz *Status,    attrinfotype *AttrInfo);



/* ~   2.2.11                                                              */
       boolean Env_IsValidToken
                         (tokentype    *Token,     ft F,lt Z,zz *Status);


/* ~   2.2.12                                                              */
       boolean Env_TokenIsEquiv
                         (tokentype    *Token0,    tokentype    *Token1);

/* ~   2.2.13                                                              */
       boolean Env_ReclaimHandles
                         (tokentype    *Token,     ft F,lt Z,zz *Status);

/* ........................................................................
                    ....     Itterate       ....
   ........................................................................*/


/* ........................................................................
                    ....     Statistics     ....
   ........................................................................*/

/* ~   2.4                                                                 */
       void    Env_ShowStats
                         (void);

/* ........................................................................
                    ....    API Library     ....
   ........................................................................*/


/*     2.7.0                                                               */
       boolean Env_GenerateRandomNumbers
                         (indextype     Count,     ft F,lt Z,zz *Status);

/*     2.7.1                                                               */
       int     Env_Random
                         (numtype       Limit);

/*+-----------------------------------------------------------------------+
~P                         END OF ENV01.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
