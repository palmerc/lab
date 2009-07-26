/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef ENV1_H
#define ENV1_H

#include "envdefs.h"

/* ENV - Env1.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
typedef handletype   localobj;
typedef numtype      typenum;
typedef numtype      valuenum;
typedef numtype      attrnum;
typedef numtype      setnum;
typedef numtype      mtrxnum;
typedef numtype      nodenum;
typedef numtype      tuplenum;
typedef numtype      codenum;
typedef handletype   sethead;
typedef handletype   stackhead;
typedef handletype   setnode;
typedef tokentype    rowhead;
typedef tokentype    colhead;
typedef handletype   mtrxnode;
typedef handletype   envobj;      
typedef handletype   typeobj;      
typedef handletype   attrobj;      
typedef handletype   codetype;
typedef handletype   slothandle;
typedef handletype   chunkhandle;


typedef struct EnvTypeDesc 
{
      handletype     Handle;
      numtype        ObjNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      wordtype       FieldExtender01;
#endif
}              envdesc;
typedef  envdesc      envdesctype;
 
 
typedef struct Env0TypeDesc 
{
      handletype     Handle;
      numtype        DbNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      numtype        Objects;
      numtype        ObjCount;
      numtype        TypeCount;
      numtype        ValueCount;
      numtype        AttrCount;
      numtype        TraitCount;
      numtype        SetCount;
      numtype        MatrixCount;
      numtype        TupleCount;
      numtype        SlotCount;
      numtype        SchemaCount;
      numtype        ViewCount;
      numtype        CodeCount;
      numtype        ShellCount;
      idtype         DbCount;
      idtype         EnvId;
      slottoken      PrimalSlotAttr;
      codetype       SpclCreateCode;
      codetype       SpclDeleteCode;
      handletype     OwnerTraitsObjsFields;
      handletype     OwnerRootTraitsObjs;
      tokentype      OwnerParentsChilds;
      handletype     OwnerCestorsCendents;
      handletype     OwnerSchemasObjs;
      handletype     OwnerRowsColsMatrices;
      handletype     OwnerMembersOwnersSets;
      sethead        RenvObjs;
      sethead        RenvTraits;
      sethead        RenvStrucs;
      sethead        RenvAttrs;
      sethead        RenvSets;
      sethead        RenvTypes;
      sethead        RenvValues;
      sethead        RenvTuples;
      sethead        RenvSlots;
      sethead        RenvMatrices;
      sethead        RenvSchemas;
      sethead        RenvViews;
      sethead        RenvShells;
      sethead        RenvCodes;
      sethead        RenvDescs;
      sethead        RenvCodeDescs;
      sethead        RenvNames;
}              renvobj;
 
/* ........................................................................
typedef    enum   TypeDescTypeDesc
               {aDesc,      aObjDesc,    aPrimalDesc, aMasterDesc,
                aHeadDesc,  aNodeDesc,   axNodeDesc,  aPointDesc ,
                aSlotDesc,  aPortDesc,   aRefObjDesc              } desctype;
 ..........................................................................*/

typedef struct ObjTypeDesc 
{
      handletype     Handle;
      numtype        ObjNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      desctype       DescType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      ObjSize;
      bytessize      PsudoSize;
      numtype        CreateCodeCount;
      handletype     BaseCreateCode;
      chunkhandle    OvrlyCreateCodes;
      codetype       DeleteCode;
      codetype       GetCode;
      codetype       PutCode;
      chunkhandle    BasesAccess;
      chunkhandle    RootsAccess;
      chunkhandle    ClassFields;   /* Modified In Trans_BuildMap */
      chunkhandle    ClassOffsets;  /* Modified In Trans_BuildMap */
      chunkhandle    Fields;
      chunkhandle    RootFields;
      chunkhandle    Cestrs;
      chunkhandle    RootCestrs;
      chunkhandle    CestorsOfObj;
      chunkhandle    CendentsOfObj;
      chunkhandle    FriendsOfObj;
      rowhead        RowHeadParentsChilds;
      colhead        ColHeadParentsChilds;
      sethead        OwnerObjParents;
      setnode        MemberObjParents;
      sethead        OwnerObjChilds;
      setnode        MemberObjChilds;
      sethead        OwnerObjAncestors;
      setnode        MemberObjAncestors;
      sethead        OwnerObjCendents;
      setnode        MemberObjCendents;
      sethead        OwnerObjSchemas;
      sethead        OwnerRootTraits;
      sethead        OwnerObjTraits;
      sethead        OwnerObjRowHeads;
      sethead        OwnerObjColHeads;
      sethead        OwnerOwnerSets;
      setnode        OwnerMemberSets;
      setnode        OwnerObjSlots;
      setnode        MemberTraitRootObjs;
      setnode        MemberTraitObjs;
      setnode        MemberSchemaObjs;
      setnode        MemberViewObjs;
      setnode        MemberRenvObjs;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
      sethead        OwnerObjCodes;
      setnode        MemberPortComponents;
      sethead        OwnerComponentOfPorts;
}              objdesctype;

typedef struct PortTypeDesc 
{
      handletype     Handle;
      numtype        ObjNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      desctype       DescType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      ObjSize;
      bytessize      PsudoSize;
      numtype        CreateCodeCount;
      handletype     BaseCreateCode;
      chunkhandle    OvrlyCreateCodes;
      codetype       DeleteCode;
      codetype       GetCode;
      codetype       PutCode;
      chunkhandle    BasesAccess;
      chunkhandle    RootsAccess;
      chunkhandle    ClassFields;
      chunkhandle    ClassOffsets;
      chunkhandle    Fields;
      chunkhandle    RootFields;
      chunkhandle    Cestrs;
      chunkhandle    RootCestrs;
      chunkhandle    CestorsOfObj;
      chunkhandle    CendentsOfObj;
      chunkhandle    FriendsOfObj;
      rowhead        RowHeadParentsChilds;
      colhead        ColHeadParentsChilds;
      sethead        OwnerObjParents;
      setnode        MemberObjParents;
      sethead        OwnerObjChilds;
      setnode        MemberObjChilds;
      sethead        OwnerObjAncestors;
      setnode        MemberObjAncestors;
      sethead        OwnerObjCendents;
      setnode        MemberObjCendents;
      sethead        OwnerObjSchemas;
      sethead        OwnerRootTraits;
      sethead        OwnerObjTraits;
      sethead        OwnerObjRowHeads;
      sethead        OwnerObjColHeads;
      sethead        OwnerOwnerSets;
      setnode        OwnerMemberSets;
      setnode        OwnerObjSlots;
      setnode        MemberTraitRootObjs;
      setnode        MemberTraitObjs;
      setnode        MemberSchemaObjs;
      setnode        MemberViewObjs;
      setnode        MemberRenvObjs;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
      sethead        OwnerObjCodes;
      sethead        OwnerPortComponents;
      setnode        MemberPortComponents;
      sethead        OwnerComponentOfPorts;
      setnode        MemberComponentOfPorts;
      sethead        OwnerPortRefAttrs;
}              portdesctype;
                                                                             

/* ........................................................................
typedef enum   TypeTypeType
               {aString,  aInteger,  aReal,       aLink,  aEnum,
                aBoolean, aChunk,    aDbmsString, aValue, aAddr,
                aRefObj                                         } typetype;
 ..........................................................................*/

typedef struct BaseTypeTypeDesc 
{
      handletype     Handle;
      typenum        TypeNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      char           AliasNam[MAXNAME];
      numtype        DescNum;
      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender08;
#endif /* STRUC_ALIGN_BY64  */

}              typebasetype;
 

typedef struct TypeTypeDesc 
{
      handletype     Handle;
      typenum        TypeNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      char           AliasNam[MAXNAME];
      numtype        DescNum;
      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif

      codetype       GetCode;
      codetype       PutCode;
      codetype       CompareCode;
      sethead        OwnerTypeAttrs;
      setnode        MemberRenvTypes;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;

}              typedesctype;
 
typedef struct TypeIntegerTypeDesc 
{
      handletype     Handle;
      typenum        TypeNum;
      char           Nam[MAXNAME];

      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif

      char           AliasNam[MAXNAME];
      numtype        DescNum;

      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif

      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif

      boolean        aSigned;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender04;
#endif

      codetype       GetCode;
      codetype       PutCode;
      codetype       CompareCode;
      sethead        OwnerTypeAttrs;
      setnode        MemberRenvTypes;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */
}              integerdesctype;
 

typedef struct TypeChunkTypeDesc 
{
      handletype     Handle;
      typenum        TypeNum;
      char           Nam[MAXNAME];

      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif

      char           AliasNam[MAXNAME];
      numtype        DescNum;

      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif

      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif

      typeobj        TypeObj;
      bytessize      UnitSize;
      codetype       GetCode;
      codetype       PutCode;
      codetype       CompareCode;
      sethead        OwnerTypeAttrs;
      setnode        MemberRenvTypes;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
}              chunkdesctype;
 

typedef struct TypeEnumTypeDesc
{
      handletype     Handle;
      typenum        TypeNum;
      char           Nam[MAXNAME];

      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif

      char           AliasNam[MAXNAME];
      numtype        DescNum;

      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif

      bytessize      Size;

      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif

      numtype        MaxValue;
      slothandle     EnumSlotAttr;

      codetype       GetCode;
      codetype       PutCode;
      codetype       CompareCode;
      sethead        OwnerTypeAttrs;
      sethead        OwnerEnumValues;
      setnode        MemberRenvTypes;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */
}              enumdesctype;
 
typedef struct TypeValueTypeDesc 
{
      handletype     Handle;
      valuenum       ValueNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      numtype        Value;
      setnode        MemberEnumValues;
      setnode        MemberRenvValues;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */
}              valuedesctype;
 
/* ........................................................................
typedef enum   TypeAttrType
               {aAttr,         aObjRefAttr,   aMapRefAttr,   aPortRefAttr,
                aTknRefAttr,   aStrHndlAttr,
                aSetHeadAttr,  aSetNodeAttr,  
                aMtrxHeadAttr, aRowHeadAttr,  aColHeadAttr,
                aMateNodeAttr, aPointNodeAttr,
                aSlotHndlAttr, aSlotTokenAttr,
                aChunkAttr,    aVchunkAttr,   
                aArrayAttr,    aVarrayAttr,   aDblPtrAttr,
                aGrpAttr,      aVgrpAttr,
                aNonTypeAttr=EXT_ENUM      }             attrtype;

typedef enum   TypeCppAttrType
               {aPublic,    aPrivate, aProtected }              cppattrtype;
 ..........................................................................*/

typedef struct AttrTypeDesc 
{
      handletype     Handle;
      attrnum        AttrNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      numtype        TraitNum;
      typetype       TypeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      attrtype       AttrType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
      cppattrtype    CppType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender04;
#endif
      valueclasstype ValueClass;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender05;
#endif

      dbmsstring     DefaultValue;
      typeobj        TypeObj;
      sizetype       UnitSize;

      codetype       GetCode;
      codetype       PutCode;
      codetype       SpclCompareCode;

      handletype     BuildDesc;

      setnode        MemberRootTraits;
      setnode        MemberObjTraits;
      setnode        MemberTypeAttrs;
      sethead        OwnerKeySets;
      sethead        OwnerTraitRootObjs;
      sethead        OwnerTraitObjs;
      setnode        MemberStrucAttrs;
      sethead        OwnerAttrStrucs;
      setnode        MemberRenvTraits;
      setnode        MemberRenvAttrs;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
}              attrdesctype;
 


typedef struct SetTypeDesc 
{
      handletype     Handle;
      setnum         SetNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      settype        SetType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      setheadtype    SetHeadType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
      attrnum        OwnerAttrNum;        
      attrnum        MemberAttrNum;        
      bytessize      LinkSize;
      numtype        NodeObjNum;
      envobj         Owner;
      envobj         Member;
      attrnum        PrimaryKey;
      attrnum        Key2ary;
      handletype     OwnersMembersNodeLink;
      codetype       SpclCompareCode;
      setnode        MemberOwnerSets;
      setnode        MemberMemberSets;
      setnode        MemberKeySets;
      setnode        MemberRenvSets;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
}             setdesctype;
 
typedef struct lSetHeadTypeDesc 
{
      handletype     Handle;
      handletype     BuildDesc;
      envobj         lOwnerObject;
      envobj         lFirstLink;
      envobj         lLastLink;
      chunkhandle    RootNodes;
      sizetype       KeySize;     /* Size of Key                           */
      typetype       KeyType;     /* Key declaration type                  */
      idtype         AnchorDbId;  /* DbId of GRP                           */
      numtype        SetKeysDir;  /* GRP of Keys by KeyIndex               */

}             lsetheaddesc;
typedef  lsetheaddesc lsetheaddesctype;
 

typedef struct gSetHeadTypeDesc  
{
      handletype     Handle;
      handletype     BuildDesc;
      envobj         lOwnerObject;
      envobj         lFirstLink;
      envobj         lLastLink;
      chunkhandle    RootNodes;
      sizetype       KeySize;     /* Size of Key                           */
      typetype       KeyType;     /* Key declaration type                  */
      idtype         AnchorDbId;  /* DbId of GRP                           */
      numtype        SetKeysDir;  /* GRP of Keys by KeyIndex               */

}             gsetheaddesc;
typedef  gsetheaddesc gsetheaddesctype;


typedef struct lSetNodeTypeDesc 
{
      handletype     Handle;
      envobj         lOwnerObject;
      envobj         lPrevLink;
      envobj         lNextLink;
      envobj         lMemberObject;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */

}             lsetnodedesc;
typedef  lsetnodedesc lsetnodedesctype;
 

typedef struct gSetNodeTypeDesc
{
      handletype     Handle;
      envobj         lOwnerObject;
      envobj         lPrevLink;
      envobj         lNextLink;
      tokentype      gMemberObject;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */
}             gsetnodedesc;
typedef  gsetnodedesc gsetnodedesctype;


typedef struct TypeLsMatrix       
{
  handletype            Handle;
  numtype               MatrixNum;
  char                  Nam[MAXNAME];
  envclass              EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
  numtype               DescNum;
  matrixtype            MatrixType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
  matrixheadtype        MatrixHeadType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
  numtype               OwnerAttrNum;
  handletype            Owner;
  handletype            RowsColsNodeLink;
  linknodetype          LinkNodeType;
  numtype               LinkNodeObjNum;
  boolean               LinkNodeIsDepend;
  handletype            RowHeadOwner;
  numtype               RowHeadAttrNum;
  handletype            RowHeadSetDesc;
  numtype               RowHeadKey;
  handletype            ColHeadOwner;
  numtype               ColHeadAttrNum;
  handletype            ColHeadSetDesc;
  numtype               ColHeadKey;
  handletype            RowSetOwner;
  numtype               RowSetAttrNum; /* Also LinkNodeAttrNum  */
  handletype            RowSetSetDesc;
  numtype               RowSetKey;
  handletype            ColSetOwner;
  numtype               ColSetAttrNum;
  handletype            ColSetSetDesc;
  numtype               ColSetKey;
  memberhandle          MemberObjRowHeads;
  memberhandle          MemberObjColHeads;
  handletype            MemberRenvMatrices;
  memberhandle          MemberRenvDescs;
  setnode               MemberRenvNames;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */

}  LsMatrix;
typedef  LsMatrix     lsmatrixtype;


typedef struct TypelMatrixHead    
{
  handletype            Handle;
  tokentype             OwnerObject;
  handletype            lRowHead;
  handletype            lColHead;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */

}  lMatrixHead;
typedef lMatrixHead   lmatrixheadtype;


typedef struct TupleTypeDesc 
{
      handletype     Handle;
      attrnum        TupleNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      tupletype      TupleType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      sethead        OwnerTupleSlots;
      setnode        MemberRenvTuples;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;
      slothandle     TupleSlotAttr;
}              tupledesctype;


typedef struct SlotTypeDesc
{
      handletype     Handle;
      attrnum        SlotNum;
      char           Nam[MAXNAME];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      slottype       SlotType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      dependtype     DependType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
      setnode        MemberObjSlots;
      sethead        MemberTupleSlots;
      setnode        MemberRenvSlots;
      setnode        MemberRenvDescs;
      setnode        MemberRenvNames;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender08;
#endif /* STRUC_ALIGN_BY64  */
}              slotdesctype;

 
typedef struct TypeCodeTypeDesc   
{   
      handletype     Handle;
      codenum        CodeNum;
      char           Nam[32];
      envclass       EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      numtype        DescNum;
      setnode        MemberRenvCodes;
      setnode        MemberRenvDescs;
      setnode        MemberRenvCodeDescs;
      setnode        MemberRenvNames;
      setnode        MemberObjCodes;
      char           PathName[32];
      codetypetype   CodeType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      cppattrtype    CppQual;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender03;
#endif
      handletype     ReturnTypeHndl;
      argclasstype   ReturnClass;     
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender04;
#endif
      argqualtype    ReturnQual;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender05;
#endif
      sizetype       ReturnSize;
      numtype        ArgCount;
      sizetype       ArgListSize;
      sethead        OwnerCodeArgs;
}              codedesctype;

typedef struct TypeArgTypeDesc   
{   
      handletype     Handle;
      char           Nam[32];
      numtype        ArgNum;
      handletype     ArgTypeHndl;
      argclasstype   ArgClass;     
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
      argqualtype    ArgQual;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender02;
#endif
      bytessize      ArgListOffset;
      setnode        MemberCodeArgs;
}              argdesctype;

typedef struct TypeRschema        
{
  handletype       Handle;
  numtype          SchemaNum;
  char             Nam[32];
  envclass         EnvType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
  handletype       SchemaPrimalDesc;
  chunkhandle      ObjsOfSchema;
  slottoken        RschemaSlotAttr;
  ownerhandle      OwnerSchemaObjs;
  memberhandle     MemberRenvSchemas;
  memberhandle     MemberShellSchemas;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */

}  rschematype;


typedef struct TypeWschema        
{
  handletype       Handle;
  slottoken        WschemaSlotAttr;
  memberhandle     MemberObjSchemas;
  ownerhandle      OwnerSchemaDbs;

#ifdef STRUC_ALIGN_BY64
      wordtype       FieldExtender05;
#endif /* STRUC_ALIGN_BY64  */
}  wschematype;


/* Structs To Translate To when access from Outer Realm                  */
typedef struct TypeObjRef
{
  addrtype              ObjAddr;
  tokentype             ObjTkn;

}  RefObj;


typedef struct TypeVarray
{
  tokentype             VchunkTkn;
  indextype             Vsize;
  addrtype              Varray;

}  varraytype;


typedef struct TypeVchunk
{
 tokentype              VchunkTkn;
 indextype              Vsize;
 addrtype               Varray;
 indextype              VstackPtr;

}  vchunktype;
 
/*+-----------------------------------------------------------------------+
~V                          VARIABLES                                     !
  +-----------------------------------------------------------------------+*/
#ifdef ENV1_C
       renvobj     *Env0                   = NullPtr;
       numtype      CodeDirChunk           = 0;   
       numtype      ObjClassMap            = 0;   
       numtype      PortVindexDir          = 0;
       numtype      PortMapDir             = 0;
       handletype   CodeArgs_Hndl          = 0;
       cppattrtype  CurrentCppQual         = aPublic;
#else
extern renvobj     *Env0;
extern numtype      CodeDirChunk;
extern numtype      ObjClassMap;   
extern numtype      PortVindexDir;
extern numtype      PortMapDir;
extern handletype   CodeArgs_Hndl;
extern cppattrtype  CurrentCppQual;

#endif

#ifdef ENV1_C
        char        *HeadNam[14] = {"lSetHead",           "gSetHead", 
                                   "lKeyedSetHead",      "gKeyedSetHead",
                                   "lKeyed2arySetHead",  "gKeyed2arySetHead",
                                   "lTableSetHead",      "gTableSetHead",
                                   "lTreeSetHead",       "gTreeSetHead",
                                   "IndexedSetHead",     "SpclSetHead",
                                   "lSetNode",           "gSetNode"     };
        num          HeadObjNum[14];
        bytessize    HeadObjSize[14];
        num          NumHeadTypes = 14;

        numtype      Obj0Count                = 84;
        varraytype   NullVarray   = {{0,0,0}, 0, NullPtr};
        vchunktype   NullVchunk   = {{0,0,0}, 0, NullPtr, 0};
#else
extern  char        *HeadNam[];
extern  num          HeadObjNum[];
extern  bytessize    HeadObjSize[];
extern  num          NumHeadTypes;
extern  numtype      Obj0Count;
extern  varraytype   NullVarray;
extern  vchunktype   NullVchunk;

#endif



/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
    boolean EnvNewDbHdr   (tokentype    *SchemaTkn, numtype       PrimalObj,
                           char         *DbName,    char         *NewFileName,
                           dbaccesstype  DbAccess,  indextype     AllocObjs,
                           indextype     AllocAttrs,
                           ft F,lt Z,zz *Status,    tokentype    *DbPrimal);


    boolean EnvNewObjDesc (tokentype    *Anchor,    desctype      ObjDescType,
                           ft F,lt Z,zz *Status,
                           objdesctype **ObjDesc,   tokentype    *Token);

    boolean EnvFetchObjNum(tokentype    *ObjTkn,
                           ft F,lt Z,zz *Status,    numtype      *ObjNum);

    boolean EnvFetchObjHandle
                          (numtype       ObjNum, 
                           ft F,lt Z,zz *Status,    handletype   *ObjHandle);

    boolean EnvPairObjHandle
                          (numtype       ObjNum,    handletype    ObjHandle,
                           ft F,lt Z,zz *Status);

    boolean EnvFetchObjSize
                          (numtype       ObjNum,
                           ft F,lt Z,zz *Status,    bytessize    *SizeInBytes);

    boolean EnvFetchObjName
                          (numtype       ObjNum,
                           ft F,lt Z,zz *Status,    char         *ObjName);

    boolean EnvFetchClassSize
                          (numtype       ObjNum,
                           ft F,lt Z,zz *Status,    size_t       *ClassSize);

  
    boolean EnvFetchLastObjCount
                          (ft F,lt Z,zz *Status,    numtype      *ObjCount);

    boolean EnvFetchObjCestrs
                          (numtype       ObjNum,
                           ft F,lt Z,zz *Status,    tokentype    *ObjCestors);

    boolean Env_FetchObjCendents
                          (numtype       ObjNum,
                           ft F,lt Z,zz *Status,    tokentype    *ObjCendents);

    boolean EnvFetchSchemaObjs
                          (numtype       ObjNum,    
                           ft F,lt Z,zz *Status,    tokentype    *SchemaObjs);

    boolean DeleteObjDesc (tokentype    *Token,     ft F,lt Z,zz *Status);



    boolean EnvNewObjHdr  (tokentype    *Anchor,    numtype       EnvObjNum,
                           indextype     AllocQty,  ft F,lt Z,zz *Status);


    boolean EnvBldObjHdr  (tokentype    *Anchor,    numtype       EnvObjNum,
                           indextype     AllocQty,  ft F,lt Z,zz *Status,
                           numtype      *FieldNumMap,  
                           chunknum     *FieldOffsetDir,
                           chunknum     *ClassOffsetDir,
                           indextype   **FieldCount,
                           chunknum     *LocalAttrNumMap,
                           indextype   **FieldRefCount,
                           chunknum     *FieldRefDir);

    boolean InstallObjChunks
                          (tokentype    *Anchor,    objheader    *ObjHdr,
                           numtype       EnvObjNum, ft F,lt Z,zz *Status);

    boolean EnvInstallObjHdr
                          (tokentype    *Anchor,    numtype       EnvObjNum,
                           ft F,lt Z,zz *Status,    numtype      *LocalObj);

    boolean EnvBldObjImage 
                          (dbheader     *CoreDb,    objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,    numtype      *FieldChunk,
                           numtype      *OffsetChnk,indextype    *FieldCount);

    boolean CppEnvGetThatTkn
                          (addrtype      CppObject, numtype       ObjNum,
                           ft F,lt Z,zz *Status,    tokentype   **CppToken);


    boolean EnvInstallAttr
                          (handletype    AttrHndl,  numtype       EnvAttrNum,
                           bytessize     Size,      ft F,lt Z,zz *Status);

    boolean EnvFetchLocalAttrNum 
                          (tokentype    *Anchor,    numtype       EnvAttrNum,
                           ft F,lt Z,zz *Status,    numtype      *LocalAttrNum);

    boolean EnvFetchAttrHandle
                          (numtype       AttrNum, 
                           ft F,lt Z,zz *Status,    handletype   *AttrHandle);

    boolean EnvFetchAttrSize
                          (numtype       AttrNum,
                           ft F,lt Z,zz *Status,    sizetype     *AttrSize);

    boolean EnvFetchAttrOffset
                          (tokentype    *ObjTkn,    numtype       AttrNum,
                           objheader    *ObjHdr,    numtype       FieldNum,
                           ft F,lt Z,zz *Status,    sizetype     *Offset);

    boolean Env_FetchObj0AttrOffset
                          (numtype       MemLoc,    numtype       AttrNum,
                           objheader    *ObjHdr,
                           ft F,lt Z,zz *Status,    sizetype     *Offset);

    boolean Env_FetchObjAttrOffset
                          (numtype       MemLoc,
                           numtype       ObjNum,    numtype       AttrNum,
                           ft F,lt Z,zz *Status,    sizetype     *Offset);

    boolean Env_FetchObjAttrSpec
                          (numtype       ObjNum,    numtype       AttrId,    
                           ft F,lt Z,zz *Status,    attrtype     *AttrType,
                           numtype      *RefObjNum, envclass     *RefEnvType,
                           sizetype     *UnitSize,  typetype     *UnitType,
                           valueclass   *ValueType, sizetype     *Offset);


    boolean Env_FetchFieldOffset
                        (tokentype    *ObjTkn,      fieldspec     FieldTkns[],
                         numtype       SubLevel,    ft F,lt Z,zz *Status,
                         sizetype     *FieldSize,   sizetype     *Offset);

    boolean Env_FetchObjFieldSpec
                          (numtype       ObjNum,    fieldspec     FieldTkns[],
                           numtype       SubLevel,
                           ft F,lt Z,zz *Status,    sizetype     *FieldSize,
                           numtype      *AttrId,    attrtype     *AttrType,    
                           numtype      *RefObjNum, typetype     *FieldType,
                           sizetype     *Offset,    sizetype     *BaseOffset);


    boolean EnvFetchOffsetMap
                          (tokentype    *ObjTkn,    objheader    *ObjHdr,  
                           ft F,lt Z,zz *Status,    typesize     *MapSize,
                           numtype      *OffsetMap, indextype    *Inset);


    boolean EnvInitCodes  (ft F,lt Z,zz *Status);

    boolean EnvMakeCodeChunks
                          (ft F,lt Z,zz *Status);

    boolean EnvPairCode   (numtype       CodeNum,   funcptrtype   CodeAddr,
                           ft F,lt Z,zz *Status);

    boolean EnvFetchCode  (numtype       CodeNum, 
                           ft F,lt Z,zz *Status,    funcptrtype  *CodeAddr);

    void    EnvAlignMember
                          (attrtype      AttrType,  typetype      TypeType,
                           sizetype      Size,      sizetype     *Offset);

    void    EnvAlignStruc
                          (boolean       DblAlign,  boolean       AddrAlign,
                           sizetype     *Offset);

#ifdef ENV1_C
    boolean TestEnv0      (ft F,lt Z,zz *Status);

    boolean CreateObjDesc (tokentype    *Anchor,    ft F,lt Z,zz *Status,  
                           objdesctype **ObjDesc,   tokentype    *Token);
#endif

/*+-----------------------------------------------------------------------+
~P                         END OF ENV1.H                                  !
  +-----------------------------------------------------------------------+*/
#endif
