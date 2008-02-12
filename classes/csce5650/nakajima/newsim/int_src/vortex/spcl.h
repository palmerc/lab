/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/*                              SPCL.H                                      */
#ifndef SPCL_01
#define SPCL_01

/*+-----------------------------------------------------------------------+
                        SPCL    Typedefs                                  !
  +-----------------------------------------------------------------------+*/
#ifdef SPCL

typedef struct lTableSetHeadType
{
   handletype     Handle;
   handletype     BuildDesc;
   handletype     lOwnerObject;
   handletype     BucketDir;
   indextype      MaxBuckets;
   numtype        Entries;

}             ltablesethead;
 
typedef struct lTableNodeType
{
   handletype     Handle;
   inttype        lPrevLink;
   inttype        lNextLink;
   handletype     lMemberObject;

}             ltablenode;

#endif
/* ------------------------------------------------------------------------*/
/*                        SPCL     Procedures                              */
/* ------------------------------------------------------------------------*/

       boolean    SpclAddIntoTable
                      (handletype    SetDescHndl,
                       tokentype    *OwnerTkn,     tokentype     *MemberTkn,
                       ft F,lt Z,zz *Status,       tokentype     *NodeTkn);

       boolean    SpclFindInTable
                      (handletype     SetDescHndl,
                       tokentype     *OwnerTkn,    addrtype       KeyValue,
                       ft F,lt Z,zz  *Status,
                       tokentype     *NodeTkn,     tokentype     *MemberTkn);

       boolean    SpclDeleteFromTable
                      (handletype     SetDescHndl, 
                       tokentype     *MemberTkn,   ft F,lt Z,zz  *Status);


       boolean    SpclDeleteTableSet
                      (handletype     SetDescHndl, tokentype     *OwnerTkn,
                       ft F,lt Z,zz  *Status);

/*  LOCAL */
#ifdef SPCL
       numtype    SpclTableGetBucket
                      (char          *KeyName,     numtype        MaxBuckets,
                       ft F,lt Z,zz *Status);
#endif

/* ------------------------------------------------------------------------*/
/*                           END SPCL.H                                    */
/* ------------------------------------------------------------------------*/
#endif
