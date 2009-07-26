/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef DBA_H
#define DBA_H

/* ENV - Dba.h  */
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
    boolean Dbm_CreateDb (tokentype    *SchemaTkn, numtype       PrimalObj,
                          char         *DbName,    char         *NewFileName,
                          dbaccesstype  DbAccess,  indextype     AllocObjs,
                          indextype     AllocAttrs,
                          ft F,lt Z,zz *Status,    tokentype    *DbPrimal);

    boolean Dbm_DeCacheDbs
                         (ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF DBA.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
