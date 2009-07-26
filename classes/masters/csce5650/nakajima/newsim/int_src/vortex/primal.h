/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C_Shell - Primal.h */

/*+-----------------------------------------------------------------------+
  !                 ....       PRIMAL       ....                          !
  +-----------------------------------------------------------------------+*/
/*     3.0  Primal <csh.h>                                                 */

#ifndef PRIMAL01_H
#define PRIMAL01_H

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


/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

/* ~   3.1.1                                                              */
       boolean Primal_CreateDb
                         (char         *SchemaName,char         *DbName,
                          char         *DbFileName,dbaccesstype  AccessType,
                          numtype       HndlRgns,  numtype       RgnHndls,
                          numtype       VhndlRgns, numtype       RgnVhndls,
                          numtype       XmemRgns,  numtype       RgnXmems,
                          ft F,lt Z,zz *Status,    tokentype    *PrimalTkn);

/* ~   3.1.2                                                              */
       boolean Primal_ActivateDbByName
                         (char         *DbName,    char         *DbFileName,
                          dbaccesstype  AccessType,
                          ft F,lt Z,zz *Status,    tokentype    *PrimalTkn);

/* ~   3.1.3                                                              */
       boolean Primal_ActivateDbByToken
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          dbaccesstype  AccessType,ft F,lt Z,zz *Status);


/* ~   3.1.9                                                              */
       boolean Primal_FreeDb
                         (tokentype    *PrimalTkn, ft F,lt Z,zz *Status);

/* ~   3.1.10                                                             */
       boolean Primal_CommitDb 
                         (tokentype    *PrimalTkn, char         *DbFileName,
                          boolean       Release,   ft F,lt Z,zz *Status);

/*     3.1.11                                                             */
       boolean Primal_DeleteDb 
                         (tokentype    *PrimalTkn, ft F,lt Z,zz *Status);


/* .......................................................................
                    ....  Class Activity   ....
   .......................................................................*/

/* ~   3.2.1                                                              */
       boolean Primal_ConfigYourClass
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs, 
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          ft F,lt Z,zz *Status);


/* ~   3.2.2                                                              */
       boolean Primal_GetClassObjectCount
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status,    numtype      *Count);

/* ~   3.2.9                                                              */
       boolean Primal_IterateOnClassObjects
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status,
                          tokentype    *ItorTkn,   tokentype    *Token);


/* ~   3.2.10                                                             */
       boolean Primal_FreezeClass
                         (tokentype    *PrimalTkn, numtype       ClassId,
                          ft F,lt Z,zz *Status);


/* ~   3.2.13                                                             */
       boolean Primal_FreeClass
                         (tokentype    *Token,     numtype       ClassId,
                          ft F,lt Z,zz *Status);



/*+-----------------------------------------------------------------------+
~P                         END OF PRIMAL.H                                !
  +-----------------------------------------------------------------------+*/
#endif
