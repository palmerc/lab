/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - TM.c */

#define TM
#include "defines.h"
#include "typedefs.h"
#ifdef __MACROIZE_HM__
#include "mem010.h"
#endif /* __MACROIZE_HM__ */
#include "hm.h"
#include "tm.h"

extern boolean Core_MoreCore (bytessize  Size,
                              ft F,lt Z,zz *Status,    addrtype  *Addr);
 
extern boolean Dbm_LoadDb     (tokentype  *Anchor,   ft F,lt Z,zz *Status,
                               char       *DbName,   dbheader    **CoreDb);

extern  tokentype    RenvTkn;
extern  tokentype    WenvTkn;
/*+-----------------------------------------------------------------------+*
~TM                            TOKEN                                      !
 *+-----------------------------------------------------------------------+*/
boolean    TmNewCoreDb  (tokentype    *Anchor,
                         ft F,lt Z,zz *Status,   dbheader  **CoreDb)
{
 if (Core_MoreCore (sizeof(dbheader), McStat, *(addrtype **)CoreDb))
     CoreDbs[Anchor->DbId] = *CoreDb;

TRACK(TrackBak,"TmNewCoreDb\n");
return (STAT);
}
 

boolean    TmRenvToken    (ft F,lt Z,zz *Status,   tokentype *RenvToken)
{
       RenvToken->Handle   = RenvTkn.Handle;
       RenvToken->DbId     = RenvTkn.DbId;
       RenvToken->CoreDbId = RenvTkn.DbId;

return (STAT);
}
 

boolean    TmWenvToken    (ft F,lt Z,zz *Status,   tokentype *WenvToken)
{
       WenvToken->Handle   = WenvTkn.Handle;
       WenvToken->DbId     = WenvTkn.DbId;
       WenvToken->CoreDbId = WenvTkn.DbId;

return (STAT);
}
 
 
boolean    TmFetchCoreDb  (tokentype    *Anchor,
                           ft F,lt Z,zz *Status,   dbheader  **CoreDb)
{
char  DbName[MAXNAME];
  DbName[0] = '\0';

  if (Anchor->DbId <= MaxCoreDbId)
  {
    if ((*CoreDb  = CoreDbs[Anchor->DbId]) == NullPtr)
    {
        sprintf  (Msg, "      TmFetchCoreDb[%2u] \n", Anchor->DbId);
        TraceMsg (0,   Msg);

        Dbm_LoadDb  (Anchor,   McStat,   DbName,  &CoreDbs[Anchor->DbId]);

       *CoreDb    = CoreDbs[Anchor->DbId];
        if (*CoreDb == NullPtr)
        {
            sprintf  (Msg, "   NULL COREDB for DbId = %2u\n", Anchor->DbId);
            TraceMsg (0, Msg);
           *Status = Tm_NullDb;
        } else
            CoreDbs[Anchor->DbId] = *CoreDb;
    }
    if (!Anchor->DbId)
       *Status = Tm_NullDb;

  } else {
   *Status = Tm_DbIdOutOfRange;

    SendMsg (0, " ***ERROR... DbId Out of Range!\n");
    sprintf (Msg, "   @TmFetchCoreDb... MaxCoreDbId=%3u; DbId=%4u\n",
             MaxCoreDbId, Anchor->DbId);
    TraceMsg (0, Msg);

  }

  if (!Normal(*Status) || DeBug >= 1)
  if (sprintf (Msg, "      TmFetchCoreDb[%2u] @ %x\n",
               Anchor->DbId, *CoreDb))
     TraceMsg (0, Msg);
 
TRACK(TrackBak,"TmFetchCoreDb\n");
return (STAT);
}
 

boolean    TmMakeToken  (tokentype    *Anchor,  indextype  Handle,
                         ft F,lt Z,zz *Status,  tokentype *Token)
{
  Token->Handle   = Handle;
  Token->DbId     = Anchor->DbId;
  Token->CoreDbId = Anchor->CoreDbId;

TRACK(TrackBak,"TmMakeToken\n");
return (STAT);
}
 

boolean    TmIsValid    (tokentype    *Token,   ft F,lt Z,zz *Status)
{
dbheader   *CoreDb     = NullPtr;
numtype     LocalObj   = 0;

  if (Token->DbId   == 0
  ||  Token->Handle == 0)
      return(False);

  if (TmFetchCoreDb    (Token,                  McStat,     &CoreDb))
  if (HmGetLocalObjNum (CoreDb, Token->Handle,  McStat,     &LocalObj))
  if (LocalObj == Null_ObjNum)
      return(False);

TRACK(TrackBak,"TmIsValid\n");
return (STAT);
}
 

boolean    TmGetObject   (tokentype    *Token,  
                          ft F,lt Z,zz *Status,   addrtype   *Object)
{
dbheader   *CoreDb  = NullPtr;

  if (TmFetchCoreDb  (Token,   McStat,        &CoreDb))
  if (Normal(*Status))   
      HmFetchDbObject (CoreDb,  Token->Handle,  McStat,  Object);

  if (*Status == Hm_ObjectNotPaired)
      *Status  = Tm_ObjectNotPaired;

TRACK(TrackBak,"TmGetObject\n");
return (STAT);
}
 

boolean    TmFreeToken  (ft F,lt Z,zz *Status,    tokentype *Token)
{
dbheader   *CoreDb  = NullPtr;

  if (TmFetchCoreDb (Token,   McStat,  &CoreDb))
      HmFreeHandle  (CoreDb, Token->Handle, McStat);

  if (!Normal(*Status) || DeBug)
  {
     sprintf  (Msg, "  TM_Free TOKEN[ %2u : %4u]\n",
               Token->DbId, Token->Handle);
     TraceMsg (0, Msg);
  }

TRACK(TrackBak,"TmFreeToken\n");
return (STAT);
}

boolean    TmReclaimHandles  (tokentype *Token, ft F,lt Z,zz *Status )
{
dbheader   *CoreDb  = NullPtr;

  if (TmFetchCoreDb (Token,   McStat,  &CoreDb))
      HmReclaimHandles  (CoreDb, McStat);

TRACK(TrackBak,"TmReclaimHandles\n");
return (STAT);
}

/*+-----------------------------------------------------------------------+
~M                         END OF TM.C                                    !
  +-----------------------------------------------------------------------+*/
