/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - DBM0.c */
#define DBM_C
#define DBM0
#include <stdio.h>
#include "defines.h"
#include "typedefs.h"
#include "hm.h"
#include "tm.h"
#include "grp.h"
#include "core01.h"
#include "mem010.h"
#include "fm.h"
#include "dbm.h"
#include "vdbm.h"
#include "gdbm.h"
#include "odbm.h"
#include "files.h"
#include "env0.h"
#ifdef ENV01 
extern boolean OaDumpObject
                          (int           TraceSwi,  tokentype    *Token,
                           ft F,lt Z,zz *Status);

extern boolean C_ReFaxToDb
                          (tokentype    *Token,   addrtype      Object,
                           boolean       Release, boolean       FreeImage,
                           numtype       AplId,
                           ft F,lt Z,zz *Status,  bytessize    *ImageSize);


#endif

#define TmGetCoreDb(Anchor,Stat,DbInCore) \
 (*DbInCore  = CoreDbs[Anchor->DbId]) != NullPtr ? True : \
  TmFetchCoreDb(Anchor,Stat,DbInCore)

 
#define    MemGetVbn          MemGetWord
#define    MemPutVbn          MemPutWord

#define    MemGetChunkNum     MemGetWord
#define    MemPutChunkNum     MemPutWord

#define    MemGetIndex        MemGetWord

extern boolean DbmCommitObjHdr 
                          (dbheader     *DbHdr,
                           boolean       Release,   ft F,lt Z,zz *Status);


farlongtype   FileBlkOffset   = 0;

/*+-----------------------------------------------------------------------+*
~DBM                           DBM10
 *+-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+*
~DBM                           1->0  
 *+-----------------------------------------------------------------------+*/
boolean DbmCommitDb     (tokentype    *DbToken,  boolean     ReleaseDb,
                         ft F,lt Z,zz *Status)
{
     *Status   = Err_NotImplemented;

TRACK(TrackBak,"DbmCommitDb\n");
return(STAT);
}
 
 


void    DbmSetDefaults   (indextype     EnvObjs,   indextype     EnvAttrs,  
                          indextype     AllocObjs, indextype     ExtendObjs,
                          indextype     AllocAttrs,indextype     ExtendAttrs,
                          indextype     AllocHndls,indextype     ExtendHndls,
                          indextype     AllocVchunks,indextype   ExtendVchunks, 
                          indextype     AllocGrps, indextype     ExtendGrps, 
                          indextype     AllocTexts,indextype     ExtendTexts, 
                          indextype     AllocXblks,indextype     ExtendXblks)
{
   HmSetDefaults   (EnvObjs,       EnvAttrs,      AllocObjs,     ExtendObjs,
                    AllocAttrs,    ExtendAttrs,   AllocHndls,    ExtendHndls,
                    AllocVchunks,  ExtendVchunks, AllocGrps,     ExtendGrps, 
                    AllocTexts,    ExtendTexts,   AllocXblks,    ExtendXblks);

return;
}


void    DbmGetDefaults   (indextype    *EnvObjs,   indextype    *EnvAttrs,  
                          indextype    *AllocObjs, indextype    *ExtendObjs,
                          indextype    *AllocAttrs,indextype    *ExtendAttrs,
                          indextype    *AllocHndls,indextype    *ExtendHndls,
                          indextype    *AllocVchunks,indextype  *ExtendVchunks, 
                          indextype    *AllocGrps, indextype    *ExtendGrps, 
                          indextype    *AllocTexts,indextype    *ExtendTexts, 
                          indextype    *AllocXblks,indextype    *ExtendXblks)
{

   HmGetDefaults   (EnvObjs,       EnvAttrs,      AllocObjs,     ExtendObjs,
                    AllocAttrs,    ExtendAttrs,   AllocHndls,    ExtendHndls,
                    AllocVchunks,  ExtendVchunks, AllocGrps,     ExtendGrps, 
                    AllocTexts,    ExtendTexts,   AllocXblks,    ExtendXblks);

return;
}

/*+-----------------------------------------------------------------------+
~M                         END OF DBM0.C                                  !
  +-----------------------------------------------------------------------+*/
