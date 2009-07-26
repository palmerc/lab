/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/*+-----------------------------------------------------------------------+
~AM                       ATTRIBUTE MANAGER                               !
  +-----------------------------------------------------------------------+*/

/* Shell - IMAGE.h  */
#ifndef IAM_H
#define IAM_H


/*+-----------------------------------------------------------------------+*
~t                            PROCEDURES
 *+-----------------------------------------------------------------------+*/

/* .......................................................................
                   ....       ATTRIBUTE    ....
   .......................................................................*/
/* C; C++                                                                 */      
       boolean ImagePutAttrValue 
                         (tokentype    *Token,     numtype       AttrNum,
                          addrtype      Value,     ft F,lt Z,zz *Status);


/* C; C++                                                                 */      
       boolean ImageGetAttrValue 
                         (tokentype    *DbmsToken, numtype       AttrNum,
                          ft F,lt Z,zz *Status,    addrtype      Value);

/* C; C++                                                                 */      
       int     ImageCompareAttr 
                         (tokentype    *Token,     numtype       AttrNum,
                          addrtype      CompValue, ptr_cmp       CmpCode,
                          ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImagePutObjTkn   
                         (tokentype    *Token,     numtype       AttrId,
                          tokentype    *ObjTkn,    ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageGetObjTkn
                         (tokentype    *Token,     numtype       AttrId,
                          ft F,lt Z,zz *Status,    tokentype    *ObjTkn);

/* C; C++                                                                 */      
       boolean ImageRefTknPut  
                         (tokentype    *Token,     numtype       AttrId,
                          RefObj       *RefObjVal, ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
       boolean ImageTokenToRef   
                         (tokentype    *DbmsToken, numtype       AttrNum,
                          ft F,lt Z,zz *Status,    addrtype     *Image);

/* C; C++                                                                 */      
       boolean ImageNewString    
                          (tokentype    *Token,      numtype       AttrNum,
                           sizetype      AllocSize,
                           ft F,lt Z,zz *Status,     cstring      *StrValue);

/* C; C++                                                                 */      
       boolean ImageGetString    
                          (tokentype    *Token,      numtype       AttrNum,
                           ft F,lt Z,zz *Status,  
                           boolean      *StrCpyIsA,  cstring      *StrValue);

/* .......................................................................
                   ....       ARRAY        ....
   .......................................................................*/
/*    C++                                                                 */      
       boolean ImageGetAttrUnitSize
                          (tokentype    *Token,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,   sizetype     *UnitSize);

/* C; C++                                                                 */      
       boolean ImageCreateArray
                          (tokentype    *DbmsToken, numtype      AttrNum,
                           indextype     UnitCount, sizetype     UnitSize,
                           ft F,lt Z,zz *Status,
                           tokentype    *VchunkTkn, addrtype    *Varray);

/* C; C++                                                                 */      
       boolean ImageCreateSubArray
                          (tokentype    *Token,     
                           numtype       AttrNum,   indextype ArrayIndex,
                           indextype     UnitCount, sizetype  UnitSize, 
                           ft F,lt Z,zz *Status,
                           tokentype    *VchunkTkn, addrtype *Varray);

/* C; C++                                                                 */      
        boolean ImageArrayActivate
                          (tokentype    *Token,     numtype       AttrNum,
                           ft F,lt Z,zz *Status,    tokentype    *Vtoken,
                           numtype      *Vsize,     sizetype     *UnitSize,
                           addrtype     *Varray);

/* C; C++                                                                 */      
        boolean ImageArrayDeActivate
                          (tokentype    *Token,     numtype       AttrNum,
                           ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
        boolean ImageArrayGetSize
                          (tokentype    *Token,     numtype       AttrNum,
                           ft F,lt Z,zz *Status,    
                           numtype      *UnitCount, sizetype     *UnitSize);

/* C; C++                                                                 */      
        boolean ImageSubArrayGetSize
                          (tokentype    *Token,     numtype       AttrId,
                           indextype     Index,
                           ft F,lt Z,zz *Status,    numtype      *UnitCount);

/* C; C++                                                                 */      
        boolean ImageArrayAssertSize
                          (tokentype    *Token,     numtype       AttrNum,
                           numtype       UnitCount, 
                           ft F,lt Z,zz *Status,    addrtype     *Varray);

/* C; C++                                                                 */      
        boolean ImageSubArrayAssertSize
                          (tokentype    *Token,     numtype       AttrId,
                           indextype     Index,     numtype       UnitCount,
                           ft F,lt Z,zz *Status,    addrtype     *Varray);

/* C; C++                                                                 */      
        boolean ImageArrayDelete
                          (tokentype    *Token,     numtype       AttrNum,
                           ft F,lt Z,zz *Status);

/* C; C++                                                                 */      
        boolean ImageSubArrayDelete
                          (tokentype    *Token,     numtype       AttrNum,
                           indextype     Index,     ft F,lt Z,zz *Status);


/* .......................................................................
                   ....      VARRAY       ....
   .......................................................................*/
/* C; C++                                                                 */      
       boolean ImageCreateVarray (tokentype    *DbmsToken, numtype   AttrNum,
                                  indextype     UnitCount, sizetype  UnitSize,
                                  ft F,lt Z,zz *Status,
                                 tokentype     *VchunkTkn, addrtype *Varray);

/* C; C++                                                                 */      
       boolean ImageGetVarray    (tokentype    *Token,
                                  ft F,lt Z,zz *Status,   indextype *Vsize,  
                                  indextype    *VstackPtr,addrtype  *Varray);

/* C; C++                                                                 */      
       boolean ImagePutVarrayStackPtr
                                 (tokentype    *Token,    indextype  StackPtr,
                                  ft F,lt Z,zz *Status,
                                  indextype    *Vsize,    addrtype  *Varray);

/* C; C++                                                                 */      
       boolean ImageGetVarrayStackPtr
                                 (tokentype    *Token,
                                  ft F,lt Z,zz *Status,   indextype *StackPtr);



/* -----------------------------------------------------------------------
                   ....     FIELD ATTRIBUTE   ....
   -----------------------------------------------------------------------*/

/* C; C++                                                                 */      
       boolean ImagePutFieldValue 
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status);


/* C; C++                                                                 */      
       boolean ImageGetFieldValue 
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value);



/*+-----------------------------------------------------------------------+
~P                         END OF IAM.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
