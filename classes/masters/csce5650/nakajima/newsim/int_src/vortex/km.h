/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Kernel - KM.h */

/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
typedef void    (*Cshell_code)  (tokentype    *Token,   addrtype    ArgStack);

#ifndef KRNL_H
#define KENL_H
       boolean KernelCreateDb
                          (char         *SchemaNam,
                           char         *DbName,    char         *NewFile,
                           dbaccesstype  DbAccess,
                           ft F,lt Z,zz *Status,    tokentype    *DbPrimal);

 
       boolean KernelLoadDbHdr    
                          (tokentype    *DbToken,  char       *DbFileName,  
                           ft F,lt Z,zz *Status);
 
 

       boolean KernelFreezeObjClass
                          (tokentype    *Anchor,    numtype       EnvObj,
                           ft F,lt Z,zz *Status);


       boolean Kernel_GetClassObjectCount
                                 (tokentype    *Token,   numtype    ClassId,
                                  ft F,lt Z,zz *Status,  numtype   *Count);

       boolean KernelNew         (tokentype    *Anchor,  numtype    ObjNum,
                                  ft F,lt Z,zz *Status,
                                  tokentype    *ObjTkn,  addrtype  *ObjAddr);

       boolean KernelCreateObject(tokentype    *Anchor,  numtype    EnvObj,
                                  ft F,lt Z,zz *Status,  tokentype *Token);

       boolean KernelNewObject   (tokentype    *Anchor,  numtype    EnvObj,
                                  numtype       NumInits, 
                                  numtype      *AttrNums,addrtype  *Values,
                                  ft F,lt Z,zz *Status,  tokentype *Token);

       boolean KernelCreateArray (tokentype   *DbmsToken, numtype   AttrNum,
                                  indextype     UnitCount,sizetype  UnitSize,
                                ft F,lt Z,zz *Status,
                                tokentype    *VchunkTkn, addrtype *ArrayAddr);

       boolean KernelCreateVarray(tokentype    *DbmsToken, numtype   AttrNum,
                                  indextype     UnitCount,sizetype  UnitSize,
                                  ft F,lt Z,zz *Status,
                                tokentype    *VchunkTkn, addrtype *ArrayAddr);


       boolean KernelIamA        (tokentype    *Token,   numtype    EnvObj,
                                  ft F,lt Z,zz *Status);
       boolean KernelWhatAmI     (tokentype    *Token,
                                  ft F,lt Z,zz *Status,  numtype   *EnvObj);
 
       boolean KernelGet         (tokentype    *ObjTkn, 
                                  ft F,lt Z,zz *Status,  addrtype  *ObjAddr);

       boolean KernelGetObject   (tokentype    *Token,   numtype    EnvObjNum,
                                  ft F,lt Z,zz *Status,  addrtype   Object);

       boolean KernelPutObject   (tokentype    *Token,   numtype    EnvObjNum,
                                  addrtype      Object,
                                  ft F,lt Z,zz *Status);
 


       boolean KernelGetAttrInfo
                          (numtype       AttrId,
                           ft F,lt Z,zz *Status,    attrobjtype  *AttrInfo);


       boolean Kernel_GetFieldStruc
                         (char         *FieldNam,   
                          ft F,lt Z,zz *Status,    fieldstruc    *FieldStruc);

       boolean KernelGetFieldInfo
                          (fieldstruc  *FieldStruc,
                           ft F,lt Z,zz *Status,     attrobjtype *AttrInfo);


       boolean KernelPutAttr  (tokentype    *Token,     numtype    EnvAttrNum,
                               addrtype      AttrValue, ft F,lt Z,zz *Status);

       boolean KernelGetAttr  (tokentype    *Token,     numtype    EnvAttrNum,
                               ft F,lt Z,zz *Status,    addrtype   Value);

       boolean KernelPutField
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          addrtype      Value,     ft F,lt Z,zz *Status);

       boolean KernelGetField
                         (tokentype    *Token,     fieldstruc   *FieldStruc,
                          ft F,lt Z,zz *Status,    addrtype      Value);


 
       int     KernelCompare     (tokentype    *Token,     numtype  AttrNum,
                                  addrtype      Value,     ptr_cmp  CmpCode,
                                  ft F,lt Z,zz *Status);

       int     KernelFieldCompare
                             (tokentype    *Token,   fieldstruc   *FieldStruc,
                              addrtype      Value,   ptr_cmp       CmpCode,
                              ft F,lt Z,zz *Status);

       boolean KernelCreateBitField
                                 (tokentype    *Anchor,    sizetype AllocBits,
                                  ft F,lt Z,zz *Status,  
                                  tokentype    *BitFieldTkn);
       boolean KernelDeleteBitField
                                 (tokentype    *BitFieldTkn, 
                                  ft F,lt Z,zz *Status);
       boolean KernelPutBit      (tokentype    *BitFieldTkn,
                                  indextype     BitNum,
                                  bytetype      BitValue,
                                  ft F,lt Z,zz *Status);
       boolean KernelGetBit      (tokentype    *BitFieldTkn,
                                  indextype     BitNum,
                                  ft F,lt Z,zz *Status,     
                                  bytetype     *BitValue);
       boolean KernelFirstBit    (tokentype    *BitFieldTkn,
                                  ft F,lt Z,zz *Status,
                                  indextype    *FirstBitNum);
       boolean KernelNextBit     (tokentype    *BitFieldTkn,
                                  indextype     CurrBitNum,
                                  ft F,lt Z,zz *Status,
                                  indextype    *NextBitNum);


       boolean KernelOwnerOf     (handletype    SetHndl, tokentype *Member,
                                  ft F,lt Z,zz *Status,  tokentype *Owner);

 
       boolean KernelFirstOf  (handletype    SetHndl,  tokentype *Owner,
                               ft F,lt Z,zz *Status,   tokentype *FirstNode,
                               tokentype    *FirstMember);

       boolean KernelNextOf   (handletype    SetHndl,  tokentype *CurrentNode,
                               ft F,lt Z,zz *Status,   tokentype *NextNode,
                               tokentype    *NextMember);
 

       boolean KernelAddInto     (handletype    SetHndl, tokentype *Owner,
                                  tokentype    *NewMember,
                                  ft F,lt Z,zz *Status);


       boolean KernelFindIn      (handletype    SetDesc, tokentype *OwnerTkn,
                                  addrtype      KeyValue,
                                  ft F,lt Z,zz *Status,  tokentype *MemberTkn);


/*+-----------------------------------------------------------------------+
~P                         END OF KM.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
