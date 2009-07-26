/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - TOKEN.h  */

#ifndef TM01
#define TM01

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/

   boolean TmNewCoreDb   (tokentype    *Anchor,   
                          ft F,lt Z,zz *Status,   dbheader    **CoreDb); 

   boolean TmRenvToken   (ft F,lt Z,zz *Status,   tokentype    *RenvToken);

   boolean TmWenvToken   (ft F,lt Z,zz *Status,   tokentype    *WenvToken);

   boolean TmFetchCoreDb (tokentype    *Anchor,   
                          ft F,lt Z,zz *Status,   dbheader    **CoreDb);

   boolean TmMakeToken   (tokentype    *Anchor,   indextype     HandleIndex,
                          ft F,lt Z,zz *Status,   tokentype    *Token);

   boolean TmIsValid     (tokentype    *Token,    ft F,lt Z,zz *Status);

   boolean TmGetObject   (tokentype    *Token,
                          ft F,lt Z,zz *Status,   addrtype     *Object);

   boolean TmFreeToken   (ft F,lt Z,zz *Status,   tokentype    *Token);

   boolean TmReclaimHandles  ( tokentype    *Token,    ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF TM.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
