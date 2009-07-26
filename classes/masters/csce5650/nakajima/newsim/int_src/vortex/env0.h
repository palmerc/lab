/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef ENV0_H
#define ENV0_H

/* ENV - Env0.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
typedef struct PrimalImage
{
      addrtype       InTheOry;
      addrtype       InCore;
}              imagetype;
 
 
typedef struct BaseObjTypeDesc
{
      handletype     Handle;
      numtype        ObjNum;
}              objtype;
 

typedef struct CppObjTypeDesc 
{
#ifdef IMAGE_WITH_THAT
      addrtype        That;
#endif /* IMAGE_WITH_THAT  */

      handletype      Handle;
      idtype          DbId;
      idtype      CoreDbId;
}              cppobjtype;
 

typedef struct CppObjectTypeDesc 
{
      addrtype        That;
      tokentype       Token;
}              cppobjecttype;

 
/*+-----------------------------------------------------------------------+
~V                          VARIABLES                                     !
  +-----------------------------------------------------------------------+*/
#ifdef ENV0

static  imagetype   *Image01                  = NullPtr;

static  prtaddr      SysBaseAddr;

        idtype       RenvId                   = 1;
        char         RenvNam[MAXNAME]         = "Renv0";
        char         RenvFileName[MAXNAME]    = "f:\\renv.hic";
        char         RenvSchemaNam[MAXNAME]   = "RenvSchema";
        dbaccesstype RenvDbAccess             = Read_Write;
        tokentype    RenvTkn                  = {1, 1, 1};
        tokentype    PrimeToken               = {1, 1, 1};
        tokentype   *RenvTknPtr               = &PrimeToken;

        idtype       WenvId                   = 2;
        char         WenvNam[MAXNAME]         = "Wenv1";
        char         WenvSchemaNam[MAXNAME]   = "WenvSchema";
        char         WenvFileName[MAXNAME]    = "f:\\wenv.hic";
        dbaccesstype WenvDbAccess             = Read_Write;
        tokentype    WenvTkn                  = {1, 2, 2};

        idtype       PrimalId                 = 3;
        char         PrimalNam[MAXNAME]       = "Primal01";
        char         PrimalFileName[MAXNAME]  = "f:\\primal.hic";
        char         PrimalSchemaNam[MAXNAME] = "PrimalSchema";
        dbaccesstype PrimalDbAccess           = Read_Write;
        tokentype    PrimalTkn                = {1, 3, 3};

        char        *TypeTypeStrs[]           =
               {"aString",  "aInteger",  "aReal",       "aLink",  "aEnum",
                "aBoolean", "aChunk",    "aDbmsString", "aValue", "aAddr",
                "aRefObj"};
 
#else
extern  idtype       RenvId;
extern  char         RenvNam[];
extern  char         RenvFileName[];
extern  char         RenvSchemaNam[];
extern  dbaccesstype RenvDbAccess;
extern  tokentype    RenvTkn;
extern  tokentype    PrimeToken;
extern  tokentype   *RenvTknPtr;
extern  idtype       WenvId;
extern  char         WenvNam[];
extern  char         WenvSchemaNam[];
extern  char         WenvFileName[];
extern  tokentype    WenvTkn;
extern  idtype       PrimalId;
extern  char         PrimalNam[];
extern  char         PrimalFileName[];
extern  char         PrimalSchemaNam[];
extern  dbaccesstype PrimalDbAccess;
extern  tokentype    PrimalTkn;
extern  char        *TypeTypeStrs[];

#endif
 
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/

#ifdef ENV0
    void    CoreSetPageSize
                          (farlongtype   Size);

    boolean VORTEx       (ft F,lt Z,zz *Status);

    boolean CreateImage   (ft F,lt Z,zz *Status,    imagetype   **Image);

    boolean ImplodeTheOry (imagetype    *Image,     ft F,lt Z,zz *Status);

    boolean CreateKernel  (imagetype    *Image,     ft F,lt Z,zz *Status,
                           dbheader    **Image0,    dbheader    **Image1);

    boolean LoadKernel    (imagetype    *Image,     ft F,lt Z,zz *Status,
                           dbheader    **Image0,    dbheader    **Image1);


    boolean LoadEnv0      (ft F,lt Z,zz *Status);
    boolean LoadObj0      (ft F,lt Z,zz *Status);

#endif

/*+-----------------------------------------------------------------------+
~P                         END OF ENV0.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
