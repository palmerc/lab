/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/*+-----------------------------------------------------------------------+
~IM                         IMAGE MAN                                     !
  +-----------------------------------------------------------------------+*/

/* Shell - IMAGE.h  */
#ifndef IM_H
#define IM_H

/*+-----------------------------------------------------------------------+*
~t                          PROCEDURES     
 *+-----------------------------------------------------------------------+*/
/* .......................................................................
                    ....     PRIMAL        ....
   .......................................................................*/
/* C; C++                                                                 */      
       boolean ImageCreateDb     (char         *SchemaName,
                                  char         *DbName, char       *File,
                                  dbaccesstype  DbAccess,
                                  ft F,lt Z,zz *Status, tokentype  *DbTkn);


/* C; C++                                                                 */      
       boolean ImageConfigClass  
                                 (tokentype    *PrimalTkn, numtype  ClassId,
                                  numtype       RgnEntrys,
                                  numtype       AllocRgns, numtype  ExtendRgns,
                                  numtype       AllocXs,   numtype  ExtendXs, 
                                  accesstype    AccessType,boolean  DiskCache,
                                  numtype       CacheLimit,numtype  CacheLoad,
                                  ft F,lt Z,zz *Status);

/* .......................................................................
                    ....      OBJECT        ....
   .......................................................................*/
/* C                                                                      */      
       boolean ImageDbCreate     (tokentype    *Anchor, idtype        ClassId,
                                  ft F,lt Z,zz *Status, tokentype    *Token);


/* C                                                                      */      
       boolean ImageNewObject    (tokentype    *Anchor, numtype       ClassId,
                                  ft F,lt Z,zz *Status,
                                  tokentype    *Token,  addrtype     *This); 

/* C                                                                      */      
       boolean ImageGetObject    (tokentype    *DbmsTkn,
                                  ft F,lt Z,zz *Status, addrtype     *This);


/*    C++                                                                 */      
       boolean ImageCreateObject (krnlobjtype  *This,   tokentype    *Anchor, 
                                  idtype        ClassId,numtype       AplId,
                                  ft F,lt Z,zz *Status);
 
/*    C++                                                                 */      
       boolean Image_IsActive    (tokentype    *Token,
                                  ft F,lt Z,zz *Status, addrtype     *This);

/* C; C++                                                                 */      
       boolean ImageFreeObject   (krnlobjtype  *This,   numtype       AplId,
                                  ft F,lt Z,zz *Status);
 
/* C; C++                                                                 */      
       boolean ImageCommitObject (numtype       AplId,  tokentype    *Token,
                                  addrtype      This,   boolean       Release,
                                  ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageDeleteObject (tokentype    *Token,  numtype       AplId, 
                                  ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageDumpObject   (tokentype   *Token);


       void    ImageDumpPseudo   (idtype       Ref_DbId,  numtype   ClassId,
                                  addrtype     Pseudo);


/* .......................................................................
                    ....       CLASS        ....
   .......................................................................*/

/* C; C++                                                                 */      
       boolean ImageInitClassMap (numtype      ClassId, size_t        ObjSize,
                                  numtype      AplId,   ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageAppendToMap  (numtype       AttrId, size_t        Offset,
                                  numtype       AplId,  addrtype      AplStruct,
                                  ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageInvokeMap    (numtype       AplId,  ft F,lt Z,zz *Status);

/* C; C++; Trans                                                          */      
       boolean ImageGetClassMap  (numtype       ClassId,numtype       AplId,
                                  ft F,lt Z,zz *Status,
                                  classmaptype *ClassMap);


/* C; C++                                                                 */      
       boolean ImageFreeClassById
                                 (numtype       AplId,  idtype        DbId, 
                                  numtype       ClassId,ft F,lt Z,zz *Status);

       boolean ImageFreeClass    (numtype       AplId,  tokentype    *Token,
                                  ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageCommitClass  (numtype       AplId,  tokentype    *Token,
                                  numtype       ClassId,ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean Image_GetClassObjectCount
                                 (tokentype    *Token,   numtype      ClassId,
                                  ft F,lt Z,zz *Status,  numtype     *Count);

/* .......................................................................
                    ....      IMAGE01       ....
   .......................................................................*/
/* C; C++                                                                 */      
       boolean ImageFaxToThis    (numtype       AplId,  tokentype    *Token,
                                  addrtype      This,   ft F,lt Z,zz *Status);

/* (not called by anyone)                                                 */      
       boolean ImageAssertToThis (tokentype    *Token,  addrtype      Object,
                                  ft F,lt Z,zz *Status);

/*    C++ (Primal::GetActiveObject)                                       */      
       boolean ImageGetActiveObject
                                 (numtype       ClassId,tokentype    *Token,
                                  ft F,lt Z,zz *Status, addrtype     *Object);


/* C                                                                      */      
       boolean ImageReFaxToDb    (numtype       AplId,  
                                  tokentype    *Token,  addrtype      This,
                                  boolean       Release,boolean       FreeMem,
                                  ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageFreeDbObject (numtype       AplId,  tokentype    *Token,
                                  ft F,lt Z,zz *Status);
/*    C++                                                                 */      
       boolean ImageFreeCppObject
                                 (tokentype    *Token,  boolean    FreeImage,
                                  ft F,lt Z,zz *Status);

/*    C++                                                                 */      
       boolean ImageRevokeObject (numtype       AplId,  tokentype    *Token,
                                  ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF IM.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
