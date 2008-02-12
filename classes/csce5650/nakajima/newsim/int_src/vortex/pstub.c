/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* PSTUB.c */
#define  PSTUB
#include "defines.h"
#include "typedefs.h"
#include "envdefs.h"

   boolean CppCreateObject 
                       (numtype    ObjNum, 
                        ft F,lt Z,zz *Status, addrtype *CppObject);

   boolean CppRefToTkn (addrtype      ThisObj, numtype      ObjNum, 
                        ft F,lt Z,zz *Status,  tokentype   *ObjTkn);

   boolean CppCastObject  
                       (numtype       ObjNum,  numtype      BaseObjNum,
                        ft F,lt Z,zz *Status,  addrtype    *Object);

   addrtype Image01_GetFreeStoreAddr 
                       (void);

/*+-----------------------------------------------------------------------+
~C++I                    C++ OBJECT  INTERFACE                            !
  +-----------------------------------------------------------------------+*/
boolean CppCreateObject (numtype    ObjNum, 
                        ft F,lt Z,zz *Status, addrtype *CppObject)
{
/* Dummy it up for LINT                                                    */

  *CppObject = NullPtr;
   if (ClassBug || DeBug || OaBug)
   if (sprintf (Msg, "  Alloc Object in OuterRealm... Class= %2u\n", ObjNum))
      TraceMsg (0, Msg);

TRACK(TrackBak,"CppCreateObject\n");
return (STAT);
}

boolean CppRefToTkn     (addrtype      ThisObj, numtype      ObjNum, 
                         ft F,lt Z,zz *Status,  tokentype   *ObjTkn)
{

  *ObjTkn    = NullToken;
   if (ClassBug || DeBug || OaBug)
   if (sprintf (Msg, "  CppRefToTkn... Class= %2u  @%x\n", 
                ObjNum,  ThisObj))
      TraceMsg (0, Msg);

  *Status    = Trans_CppShellNotInvoked;

strcpy (TrackBak, "CppRefToTkn\n");
return (False);
}


boolean CppCastObject   (numtype       ObjNum,  numtype      BaseObjNum,
                         ft F,lt Z,zz *Status,  addrtype    *Object)
{
  *Object = NullPtr;

   if (ClassBug || DeBug || OaBug)
   if (sprintf (Msg, " Cast Object of class= %4u To Base Class= %4u\n",
                ObjNum, BaseObjNum))
      TraceMsg (0, Msg);

strcpy (TrackBak, "CppCastObject\n");
return (STAT);
}

addrtype Image01_GetFreeStoreAddr   (void)
{

return (NullPtr);
}
/*+-----------------------------------------------------------------------+
~M                         END OF PSTUB.C                                 !
  +-----------------------------------------------------------------------+*/
