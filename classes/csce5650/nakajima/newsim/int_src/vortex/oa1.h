/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - OA1.h  */
#ifndef OA1_H
#define OA1_H

/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
 
typedef boolean (*p_create) (addrtype Object, addrtype *Foreign);
 
/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/

/* Attribute Cognisant                                                     */
       boolean OaIamA     (tokentype    *Token,     numtype       EnvObj,
                           ft F,lt Z,zz *Status);
 
 
       boolean OaInitObject
                          (tokentype    *Token,     numtype       NumInits, 
                           numtype      *AttrNums,  addrtype     *Values,
                           ft F,lt Z,zz *Status);

       boolean OaCopy     (tokentype    *TokenA,    tokentype    *TokenB,
                           ft F,lt Z,zz *Status);

       boolean OaGetDbObjNums 
                          (int           TraceSwi,  tokentype    *Anchor,
                           ft F,lt Z,zz *Status);

       boolean OaDelete   (tokentype    *Token,     ft F,lt Z,zz *Status);

       boolean OaDeleteFields
                          (tokentype    *Token,     numtype       LocalObj,
                           objheader    *ObjHdr,    addrtype      Object,
                           ft F,lt Z,zz *Status);

       boolean OaDeleteEmbedded 
                          (tokentype    *Token,     numtype       LocalObj,
                           addrtype      Object,    ft F,lt Z,zz *Status);

       boolean OaCreateArray 
                          (tokentype    *DbmsToken, numtype       AttrNum,
                           indextype     UnitCount, sizetype      UnitSize,
                           ft F,lt Z,zz *Status, 
                           tokentype    *VchunkTkn, addrtype     *ArrayAddr);

       boolean OaCreateVarray
                          (tokentype    *DbmsToken, numtype       AttrNum,
                           indextype     UnitCount, sizetype      UnitSize,
                           ft F,lt Z,zz *Status, 
                           tokentype    *VchunkTkn, addrtype     *ArrayAddr);


/* Attribute Specific                                                      */

/* Put's                                                                   */
       boolean OaPut      (boolean       CppType,   tokentype    *Token,
                           addrtype      Object,    numtype       EnvAttrNum,
                           addrtype      Value,     ft F,lt Z,zz *Status);

       boolean OaPutString
                          (idtype        DbId,      objheader    *ObjHdr,
                           char         *String,
                           ft F,lt Z,zz *Status,    indextype    *StrIndex);

       boolean OaPutToEmbedded 
                          (tokentype    *Token,     numtype       SubClass,
                           numtype       AttrNum,   addrtype      Value,
                           ft F,lt Z,zz *Status);

       boolean OaPutField
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status);

/* Get's                                                                   */
       boolean OaGet      (boolean       CppType,   tokentype    *Token,
                           addrtype      Object,    numtype       EnvAttrNum,
                           ft F,lt Z,zz *Status,    addrtype      AttrValue);

       boolean OaGetString
                          (idtype        DbId,      objheader    *ObjHdr,
                           indextype     StrIndex,
                           ft F,lt Z,zz *Status,    char        **String);

       boolean OaGetField
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value);

/* Utility                                                                 */
       boolean OaChkForAttr
                          (tokentype    *Token,     numtype       EnvAttrNum,
                           ft F,lt Z,zz *Status,
                           sizetype     *Size,      sizetype     *Offset);

       boolean OaGetAttrSize
                          (tokentype    *Anchor,    numtype       EnvAttrNum,
                           ft F,lt Z,zz *Status,    addrtype      AttrSize);

       boolean OaCompare  (boolean       CppType,   tokentype    *Token,
                           addrtype      Object,    numtype       EnvAttrNum,
                           addrtype      CompValue, ptr_cmp       CmpCode,
                           ft F,lt Z,zz *Status);

       boolean OaCompareField  
                          (boolean       CppType,   tokentype    *Token,
                           addrtype      Object,    fieldstruc   *FieldStruc,
                           addrtype      CompValue, ptr_cmp       CmpCode,
                           ft F,lt Z,zz *Status);

/* DUMP                                                                    */
       boolean OaDumpObjHdr
                          (int           TraceSwi,  tokentype    *Anchor,
                           num           ObjNum,    ft F,lt Z,zz *Status);

       boolean OaDumpObjHndls 
                          (tokentype    *Anchor,    numtype       ObjNum,
                           ft F,lt Z,zz *Status);

       boolean OaDumpObject
                          (int           TraceSwi,  tokentype    *Token,
                           ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF OA1.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
