/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* TRANS */
#ifndef TRANS010_H
#define  TRANS010_H

#ifndef OBJ01_H
#ifndef ENV1_H
typedef struct TypeObjRef
{
  addrtype             *ObjAddr;
  tokentype             ObjTkn;

}  RefObj;

typedef struct TypeVarray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  void                 *Varray;

}  varraytype;

typedef struct TypeVchunk
{
 tokentype              VchunkTkn;
 indextype              Vsize;
 void                  *Varray;
 indextype              VstackPtr;

}  vchunktype;
#endif /* ENV1_H   */

typedef struct IntArray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  int                  *Varray;

}  intarraytype;

typedef struct IntChunk
{
 tokentype              VchunkTkn;
 indextype              Vsize;
 int                   *Varray;
 indextype              VstackPtr;

}  intchunktype;

#endif /* OBJ01_H   */

/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
#define V(ObjNum)  (ObjNum - Image01_ObjNum)

typedef handletype      addrchunktype;
typedef handletype      mapchunktype;

typedef struct TypeWenv           {
  handletype            Handle;
  numtype               DbNum;
  nametype              Nam[32];
  envclass              EnvType;
  numtype               DescNum;
  idtype                DbCount;
  slottoken             PrimalSlotAttr;
  handletype            SpclCreateCode;
  handletype            SpclDeleteCode;
  ownerhandle           OwnerWenvDbs;
  addrchunktype         VcodeAddrs;
  ownerhandle           OwnerWenvGroups;
  ownerhandle           OwnerWenvValues;
  mapchunktype          C_ImageMapDir;

} wenvdbprimal;

typedef struct TypeImageMap
{
/* public:                                                                  */
  handletype            DbOffsetsHndl;
  handletype            CppOffsetsHndl;
  handletype            FieldAttrIdsHndl;      
  handletype            FieldAttrTypesHndl;
  handletype            FieldTypeTypesHndl;
  handletype            FieldSizesHndl;
  handletype            FieldRefObjNumsHndl;      
  handletype            ArrayAttrsHndl;

} imagemaptype;

typedef struct TypeMapArrays
{
  typesize             *DbOffsets;
  typesize             *CppOffsets;
  typesize             *FieldSizes;
  attrtype             *FieldAttrTypes;
  numtype              *FieldAttrIds;      
  numtype              *RefObjNums;      
  numtype              *ArrayAttrs;      
  typetype             *FieldTypeTypes;

} maparraystype;
typedef  maparraystype  maparrays;

#ifdef TRANS00
static tokentype ImageMapTkn           = Null_Token;

       boolean   OverRideInitClass     = False;

       numtype   C_ImageMapDir_AttrNum = 0;

       boolean   Build_ByCpp           = False;
 
       sizetype  AttrTypeSizes[22]     = {1,          sizeof(tokentype),
                                          1,          sizeof(tokentype),
                                  sizeof(tokentype),  sizeof(handletype),
                                  sizeof(tokentype),  sizeof(handletype),
                                  sizeof(tokentype),  sizeof(tokentype), 
                                  sizeof(tokentype), 
                                  sizeof(tokentype),  sizeof(tokentype), 
                                  sizeof(handletype), sizeof(tokentype),
                                  sizeof(handletype), sizeof(tokentype),
                                  sizeof(handletype), sizeof(handletype),
                                  sizeof(handletype),
                                  sizeof(tokentype),  sizeof(tokentype) };


#else
extern boolean   OverRideInitClass;
extern numtype   C_ImageMapDir_AttrNum;
extern boolean   Build_ByCpp;

extern sizetype  AttrTypeSizes[];

#endif

/* TRANS00.C                                                              */
       boolean TransInitMap
                        (numtype       ClassId,     size_t        ObjSize,
                         numtype       AplId,       ft F,lt Z,zz *Status);

       boolean TransAppendToMap
                        (numtype       AttrId,      size_t        Offset,
                         numtype       AplId,       ft F,lt Z,zz *Status);

       boolean TransInvokeMap
                        (numtype       AplId,       ft F,lt Z,zz *Status);

       boolean TransBuildMap
                        (numtype       AplId,       ft F,lt Z,zz *Status);

       boolean TransGetMap
                        (numtype       ObjNum,      numtype       AplId,
                         ft F,lt Z,zz *Status,      classmaptype *ClassMap);

       boolean Trans_DumpMap
                        (numtype       ObjNum,      numtype       AplId,
                         ft F,lt Z,zz *Status);

       boolean Trans_MapIsActive
                        (numtype       ObjNum,      ft F,lt Z,zz *Status);

       boolean TransCreateMapDirs
                        (numtype       AplId,       ft F,lt Z,zz *Status,
                         numtype      *ObjMapDir,   numtype      *ImageMapDir);

       boolean TransNewImageMap
                        (numtype       ObjMapDir,   numtype       ImageMapDir,
                         numtype       ObjNum,      numtype       FieldCount,
                         ft F,lt Z,zz *Status,      boolean      *MapExists,
                         imagemaptype *ImageMap,maparraystype   **FieldsDir);

       boolean TransBuildFields 
                        (handletype    MapAttrCount,fieldtoken   *FieldTknMap,
                         objnumtype   *Vfields,     indextype     FieldCount,
                         char         *ObjName,
                         imagemaptype *ImageMap,   maparraystype *ClassMap,
                         ft F,lt Z,zz *Status);

       boolean TransBuildMapField
                        (objdesctype  *BuildDesc,   typesize     *DbOffsets,
                         typesize     *FieldSizes,  ft F,lt Z,zz *Status,
                         indextype    *NumMapFields,indextype  *NumArrayAttrs);


       boolean Trans_FetchAttrOffset
                        (numtype       ObjNum,      numtype       AplId,
                         numtype       FieldNum,    ft F,lt Z,zz *Status,
                         sizetype     *CppOffset);

       boolean Trans_FetchAttrOffsets
                        (numtype       ObjNum,      numtype       AplId,
                         numtype       AttrId,      ft F,lt Z,zz *Status,
                         sizetype     *DbOffset,    sizetype     *CppOffset);

       boolean Trans_FetchFieldOffsets
                        (tokentype    *Token,       numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         sizetype     *DbOffset,    sizetype     *CppOffset);

       boolean Trans_FetchFieldOffset
                        (tokentype    *Token,       numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         numtype      *AttrId,      attrtype     *AttrType,    
                         numtype      *RefObjNum,   typetype     *UnitType,
                         sizetype     *DbOffset,    sizetype     *CppOffset,
                         sizetype     *DbBase,      sizetype     *CppBase);


       boolean Trans_FetchObjFieldSpec
                        (numtype       ObjNum,      numtype       AplId,
                         fieldspec     FieldTkns[], numtype       SubLevel,
                         ft F,lt Z,zz *Status,      sizetype     *FieldSize,
                         numtype      *AttrId,      attrtype     *AttrType,    
                         numtype      *RefObjNum,   typetype     *UnitType,
                         sizetype     *DbOffset,    sizetype     *CppOffset,
                         sizetype     *DbBase,      sizetype     *CppBase);

/* TRANS01.C                                                              */
       boolean C_GetObjectImage
                              (tokentype    *Token,    numtype       AplId,
                               ft F,lt Z,zz *Status,   addrtype     *This);

       boolean C_ObjectNewImage 
                              (tokentype    *Token,    numtype       ObjNum,
                               numtype       AplId,
                               ft F,lt Z,zz *Status,   addrtype     *This);

       boolean C_InvokeAtThis
                              (tokentype    *Token,    addrtype      This,
                               numtype       AplId,    ft F,lt Z,zz *Status);

       boolean C_FaxToThis    (tokentype    *Token,    addrtype      This,
                               numtype       AplId,    ft F,lt Z,zz *Status);

       boolean C_FaxToClassFields
                              (idtype        DbId,     numtype       LocalObj,
                               addrtype      DbObject, addrtype      Object,
                               numtype       ObjNum,   maparrays    *FieldMap,
                               numtype       AplId,    ft F,lt Z,zz *Status);

       boolean C_MapRefToAddr (idtype        DbId,     numtype       LocalObj,
                               addrtype      DbObject, addrtype      Object,
                               numtype       ObjNum,   sizetype      MapSize,
                               numtype       AplId,    ft F,lt Z,zz *Status);

       boolean C_RefToAddr    (tokentype    *Token,    numtype       AplId,
                               ft F,lt Z,zz *Status,   addrtype     *This);

       boolean C_CreateArray  (tokentype    *Token,    numtype       AttrNum, 
                               indextype     UnitCount,sizetype      UnitSize,
                               ft F,lt Z,zz *Status,    
                               tokentype    *VchunkTkn,addrtype     *Array);

       boolean C_CreateSubArray
                              (tokentype    *Token,     
                               numtype       AttrNum,  indextype    ArrayIndex,
                               indextype     UnitCount,sizetype     UnitSize, 
                               ft F,lt Z,zz *Status,
                               tokentype    *VchunkTkn,addrtype    *Array);


       boolean C_CreateFieldArray
                         (tokentype    *Token,     numtype       AplId,
                          fieldspec    *FieldTkns, numtype       SubLevel,
                          indextype     UnitCount, sizetype      UnitSize,
                          ft F,lt Z,zz *Status,    
                          tokentype    *VchunkTkn, addrtype     *Varray);

       boolean C_CreateFieldSubArray
                         (tokentype    *Token,     numtype       AplId,
                          fieldspec    *FieldTkns, numtype       SubLevel,
                          indextype     ArrayIndex,
                          indextype     UnitCount, sizetype      UnitSize, 
                          ft F,lt Z,zz *Status,
                          tokentype    *VchunkTkn, addrtype     *Varray);



       boolean C_CreateObject (numtype       ObjNum,   numtype       AplId,
                               ft F,lt Z,zz *Status,   addrtype    *CppObject);

 

/* TRANS10.C                                                              */
       boolean C_ReFaxToDb    (tokentype    *Token,   addrtype      Object,
                               boolean       Release, boolean       FreeImage,
                               numtype       AplId,
                               ft F,lt Z,zz *Status,  bytessize    *ImageSize);

       boolean C_GroupArrays  (tokentype    *Token,   addrtype      Object,
                               boolean       Release, numtype       AplId,
                               ft F,lt Z,zz *Status);

       boolean C_MapRefToDb   (idtype        DbId,    numtype       ClassId, 
                               sizetype      MapSize, addrtype      DbObject,
                               addrtype      Object,  numtype       AplId,
                               boolean       Release, ft F,lt Z,zz *Status);

       boolean C_RefToTkn     (addrtype      ThisObj, numtype       ObjNum, 
                               numtype       AplId,
                               ft F,lt Z,zz *Status,  tokentype    *ObjTkn);

       boolean C_CommitClass  (tokentype    *Token,   numtype       ClassId,
                               numtype       AplId,   ft F,lt Z,zz *Status);


/* TRANS20.C                                                              */
       boolean C_FreeObject   (tokentype    *Token,   
                               numtype       AplId,   boolean       FreeImage, 
                               ft F,lt Z,zz *Status);

       boolean C_FreeEmbeded  (idtype        DbId,    numtype       ObjNum, 
                               sizetype      MapSize, addrtype      DbObject,
                               addrtype      Object,  numtype       AplId,
                               ft F,lt Z,zz *Status);

       boolean C_FreeClass    (idtype        DbId,    numtype       ClassId,
                               numtype       AplId,   ft F,lt Z,zz *Status);

       boolean C_DeleteObject (tokentype    *Token,   numtype       AplId,
                               ft F,lt Z,zz *Status);

/*+-----------------------------------------------------------------------+
~P                         END OF TRANS.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
