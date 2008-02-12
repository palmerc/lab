/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef  ENVDEFS_H
#define ENVDEFS_H

/* ENV- Envdefs.H  */
/*+-----------------------------------------------------------------------+
~I                            INCLUDES                                    !
*+-----------------------------------------------------------------------+*/


/*+-----------------------------------------------------------------------+
~D                            DEFINES                                     !
  +-----------------------------------------------------------------------+*/
#define  Extend_Codes      (indextype )101

#ifndef BLDDEFS_H
#define        Image01_ObjNum      (objnumtype )149

#define        That_AttrNum           (numtype )1

#define        Token_Attr             (numtype )2

#define        Handle_AttrNum         (numtype )2
#define        DbId_AttrNum           (numtype )3
#define        CoreDbId_AttrNum       (numtype )4

#define        ObjNum_AttrNum         (numtype )5

#define        Id_AttrNum             (numtype )144
 
#define        Renv0_Hndl             (handletype )1
#define        Renv_Hndl              (handletype )2

#define        RenvTypes_Hndl         (handletype )702

#endif

/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
*+-----------------------------------------------------------------------+*/
/* these are general typedefs as defined during generation of Class Types */
typedef handletype   memberhandle;
typedef handletype   ownerhandle;
 
typedef tokentype    membertoken;
typedef tokentype    ownertoken;
 
/* the C++ TOKEN */
typedef tokentype    slottokentype;
typedef tokentype    slottoken;


typedef enum   TypeEnvClass
               {aEnvDb,   aEnvObj,    aEnvType,   aEnvStruc,  aEnvAttr,
                aEnvSet,  aEnvMatrix, aEnvPair,   aEnvTuple,  aEnvSchema,
                aEnvView, aEnvCode,   aEnvHead,   aEnvNode,   aEnvSlot,
                aEnvValue,aEnvArg,    aEnvPort,   aEnvShell,  
                aNonEnvClass=EXT_ENUM                           } envclass;

#ifndef TYPEDEFS_01
typedef enum              TypeTypeTypeDesc
               {aString,      aInteger,      aReal,         aLink,
                aEnum,        aBoolean,      aChunk,        aDbmsString,   
                aValue,       aAddr,         aRefObj, 
                aNonTypeType=EXT_ENUM } 
                          typetype;
 
typedef enum              TypeValueClassType
                 {aVoid,        
                  aBitField,  aChar,         aUnChar,
                  aShort,     aUnShort,      aInt,          aUnInt,
                  aLong,      aUnLong,       aQuad,         aOctet,
                  aVoidPtr,   aFloat,        aDouble,
                  aEnumType,  aStruct,       aUnion,
                  aComplex,   aObject,       aOid,   aNonValueClass=EXT_ENUM}  
                          valueclasstype;
typedef valueclasstype    valueclass;                                                              

typedef enum              TypeAttrType
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

typedef enum              TypeCppAttrType
               {aPublic,      aPrivate,      aProtected, aNonCppAttr=EXT_ENUM}
                          cppattrtype;
#endif


typedef enum   TypeDescTypeDesc
               {aDesc,      aObjDesc,    aPrimalDesc, aMasterDesc,
                aHeadDesc,  aNodeDesc,   axNodeDesc,  aPointDesc ,
                aSlotDesc,  aPortDesc,   aRefObjDesc,
                aNonDesc=EXT_ENUM                            } desctype;

typedef enum   TypeSetType
               {aLocalSet,          aGlobalSet, 
                aLocalKeyedSet,     aGlobalKeyedSet,
                aLocalKeyed2arySet, aGlobalKeyed2arySet,
                aLocalTableSet,     aGlobalTableSet,
                aLocalTreeSet,      aGlobalTreeSet,
                aIndexedSet,        aSpecialSet,    aNonSet=EXT_ENUM
                                                             } settype;


#ifndef TYPEDEFS_01
typedef enum   KeyedSetType
               {aKeyedSet,    aMtrxHeadSet,  aNonKeyedSet=EXT_ENUM       
                                                             } keyedsettype;
#endif


typedef enum   TypeSetHeadType
               {aLSetHead,          aGSetHead,  
                aLKeyedSetHead,     aGKeyedSetHead,
                aLKeyed2arySetHead, aGKeyed2arySetHead,
                aLTableSetHead,     aGTableSetHead,
                aLTreeSetHead,      aGTreeSetHead,
                aIndexedSetHead,    aSpclSetHead,
                aLSetNode,          aGSetNode,      aNonSetHead=EXT_ENUM
                                                             } setheadtype;
 
typedef enum   TypeRowColType
               {aRowType,           aColType,       aNonRowCol=EXT_ENUM
                                                             } rowcoltype;

typedef enum   EnvNodeEnumType
               {alSetNode,          agSetNode,
                aBaseNode,          aRenvNode,
                alSetPointNode,     agSetPointNode,
                alSetPairedNode,    agSetPairedNode,
                alSetLinkedNode,    agSetLinkedNode,
                alTableNode,        agTableNode,
                alTreeNode,         agTreeNode,     aNonEnvNode=EXT_ENUM
                                                             } envnodetype;

typedef enum   LinkNodeEnumType
               {aNodeLink,          aPointLink,
                aEnvNodeLink,       aAppNodeLink,   aNonLinkNode=EXT_ENUM
                                                             } linknodetype;

typedef enum   {aMatrix,            aLocalMatrix,
                aGlobalMatrix,      aSingularMatrix, aRadicalMatrix,
                aLsMatrix,          aLrMatrix,      
                aGsMatrix,          aGrMatrix,      aNonMatrix=EXT_ENUM
                                                             } matrixtype;

typedef enum    
               {aLMatrixHead,       aGMatrixHead,   aNonMatrixHead=EXT_ENUM
                                                             } matrixheadtype;

typedef enum   TypeTupleType
               {aLocalTuple,        aGlobalTuple, 
                aLocalPair,         aGlobalPair,    aNonTuple=EXT_ENUM
                                                             } tupletype;

typedef enum   TypeSlotType 
               {aLTupleSlot,        aGTupleSlot,   
                aLPairSlot,         aGPairSlot,     aNonSlot=EXT_ENUM
                                                             } slottype;

typedef enum   TypeDependType 
               {aIndependent,       aMaster,
                aSlave,             aDependent,     aNonDepend=EXT_ENUM
                                                             } dependtype;

typedef enum   TypeCodeType
               {aCreateCode,        aDeleteCode,  
                aPutObjCode,        aGetObjCode,
                aPutAttrCode,       aGetAttrCode,
                aCompAttrCode,      aSpclCode,
                aObjCode,           aKernelCode,
                aShellCode,         aVirtualCode,
                aOvrloadCode,       aDbeCode,     
                aDbmsCode,          aNonCodeType=EXT_ENUM    } codetypetype;

typedef enum   ClassArgType
               {aVoidClass,         aObjClass,  
                aTypeClass,         aFuncClass,     aNonClassArg=EXT_ENUM
                                                             } argclasstype;

typedef enum   QualArgType
               {aOfThisType,        aPtrToType,   
                aDblPtrToType,      aRefToType,     aNonQualArg=EXT_ENUM
                                                             } argqualtype;


typedef enum   TypeF77DataType
               {aNonF77type,
                aF77String,         aF77Integer,
                aF77Real,           aF77Double,     aNonF77DataType=EXT_ENUM
                                                             } f77datatype;

typedef enum   TypeF77ClassType
               {aNonF77Class,       aF77Sclar,
                aF77NumericArray,   aF77CharArray,  aNonF77ClassType=EXT_ENUM
                                                             } f77classtype;


#ifndef TYPEDEFS_01
typedef enum   ComparisonOpType
               {aEQ,                aLT,           aGT,
                aGTE,               aLTE,          aNEQ,
                aModEQ0,            aNonCompareOp=EXT_ENUM   } compareoptype;
typedef compareoptype      compareop;
#endif

typedef enum   SortOrderType
               {aAscending,         aDescending,   aNonSortOIrder=EXT_ENUM
                                                             } sortordertype;
typedef sortordertype      sortorder;


typedef enum   BooleanOpsType
               {aNullBoolOp,        aAND,     aOR,   aExOR,
                aNOT,               aNAND,    aNOR,  aNxOR,
                aNonBoolOp=EXT_ENUM                          } booleanoptype;
typedef booleanoptype      booleanop;


typedef struct TypeAttrObj
{
  char                  AttrName[MAXNAME];
  attrtype              AttrType;
  cppattrtype           CppType;
  char                  TypeName[MAXNAME];
  typetype              TypeType;
  bytessize             Size;
  boolean               aSigned;
  numtype               EnumValue;
  bytessize             ChunkUnitSize;
  tokentype             ObjRefTkn;

}  attrobjtype;
typedef attrobjtype     attrinfotype;


typedef struct    KrnlObjectType
{
   addrtype    That;
   tokentype   Token;
}                       krnlobjtype;
typedef krnlobjtype    *referencetype;   
 

typedef struct TypeF77TokenType
{
   f77datatype          F77Type;
   f77classtype         F77Class;
   numtype              ArrayNum;
   numtype              ArrayIndex;

}  f77tokentype;

/*+-----------------------------------------------------------------------+
~V                             VARIABLES                                  !
  +-----------------------------------------------------------------------+*/
#ifdef ENV1_C
       numtype      Global_Apl             = C_Apl;

       numtype      EOS_ObjNum             = 131;   /* in InstallObjChunks */

    /* Oa  :: Used In Build for the ObjDesc Class                          */
       indextype    Alloc_ObjRegions       = 80;
       indextype    Alloc_RegionObjs       = 5;

    /* ObjClass region chunks for storage of object struct                 */
       indextype    Alloc_Regions          = 10;
       indextype    Extend_Regions         = 10;
       indextype    Region_Objects         = 100;

       indextype    Alloc_ObjDescRgns      = 10;
       indextype    Alloc_ObjDescSectors   = 40;
       indextype    Alloc_ObjDescRgnObjs   = 10;

       numtype      C_ObjMapDir            = 0;
       numtype      C_ImageMapDir          = 0;
       numtype      CppObjMapDir           = 0;
       numtype      F77ObjMapDir           = 0;
       numtype      F77StructDir           = 0;   
       numtype      ApiStructDir           = 0;   
#else
extern numtype      Global_Apl;
extern numtype      EOS_ObjNum;

extern indextype    Alloc_ObjRegions;
extern indextype    Alloc_RegionObjs;

extern indextype    Alloc_Regions;
extern indextype    Extend_Regions;
extern indextype    Region_Objects;

extern indextype    Alloc_ObjDescRgns;
extern indextype    Alloc_ObjDescSectors;
extern indextype    Alloc_ObjDescRgnObjs;

extern numtype      C_ObjMapDir;
extern numtype      C_ImageMapDir;
extern numtype      CppObjMapDir;
extern numtype      F77ObjMapDir;
extern numtype      F77StructDir;   
extern numtype      ApiStructDir;   
#endif


#ifdef ENV1_C
        char        *DescNam[11]  = {"Desc",        "ObjDesc",    "PrimalDesc",
                                    "MasterDesc",  "HeadDesc",   "NodeDesc",
                                    "xNodeDesc",   "PointDesc",  "SlotDesc",
                                    "PortDesc",    "RefObjDesc"   };

        char        *LowDescNam[11]
                                 = {"desc",        "objdesc",    "primaldesc",
                                    "masterdesc",  "headdesc",   "nodedesc",
                                    "xnodedesc",   "pointdesc",  "slotdesc",
                                    "portdesc",    "refobjdesc"             };

        char        *LowTypeNam[11]
                                 = {"stringdesc",  "integerdesc",
                                    "realdesc",    "linkdesc",
                                    "enumdesc",    "booleandesc",
                                    "chunkdesc",   "dbmsstringdesc",
                                    "valuedesc",   "addrdesc",
                                    "refobjdesc"                     };
 
        char        *CppQualName[3]     = {
                        "public",      "private",     "protected" };

        char        *ArgClassName[4]    = {
                        "aVoidClass",    "aObjClass",   "aTypeClass",
                        "aFuncClass" };

        char        *ArgQualName[4]     = {
                         "aOfThisType",  "aPtrToType",  "aDblPtrToType",
                         "aRefToType"                                    };

        char        *ArgQualLex[4]      = {   "  ",  " *",  "**", " &"   };


#ifndef TYPEDEFS_01
        char        *CompareOpName[7]   = {"aEQ",      "aLT",   "aGT",
                                           "aGTE",     "aLTE",  "aNEQ",
                                           "aModEQ0" };
#endif

        char        *CompareOpLex[7]    = {"==",    "<",     ">",
                                           ">=",    "<=",    "!=",
                                           "%"                     };


        char        *SortOrderName[2]   = {"aAscending", "aDescending" };


        char        *BooleanOpName[8]   = {"aNullBoolOp",
                                           "aAND",  "aOR",   "aExOR",
                                           "aNOT",  "aNAND", "aNOR",
                                           "aNxOR"  };

        char        *BooleanOpLex[8]    = {"??", "&&",    "||",    "^",
                                           "!",  "^&",    "|",     "^|"  };


        char        *DbPutStr[3]        = { "Put_NoFree",
                                            "Put_Release", "Put_Deep"};

        char        *HeapStr[2]         = { "FreeStore",   "HeapSpace"};


        char        *TypeTypeNam[11]    = {"aString",  "aInteger",  "aReal",
                                           "aLink",    "aEnum",     "aBoolean", 
                                           "aChunk",
                                           "aDbmsString",           "aValue",
                                           "aAddr",    "aRefObj"};

        char        *ValueClassNam[21]  = {"aVoid",        
                                           "aBitField", "aChar",    "aUnChar",
                                "aShort",  "aUnShort",  "aInt",     "aUnInt",
                                "aLong",   "aUnLong",   "aQuad",    "aOctet",
                                           "aVoidPtr",  "aFloat",   "aDouble",
                                           "aEnumType", "aStruct",  "aUnion",
                                           "aComplex",  "aObject",  "aOid"  };

        char        *AttrTypeNam[22]    = {"Attr",
                                           "ObjRefAttr",     "MapRefAttr",
                                           "PortRefAttr",    "TknRefAttr",
                                           "StrHndlAttr",
                                           "SetHeadAttr",    "SetNodeAttr",    
                                           "MtrxHeadAttr",   
                                           "RowHeadAttr",    "ColHeadAttr",
                                           "MateNodeAttr",   "PointNodeAttr",
                                           "SlotHndlAttr",   "SlotTokenAttr",
                                           "ChunkAttr",
                                           "VchunkAttr",     "ArrayAttr",
                                           "VarrayAttr",     "DblPtrAttr",
                                           "GrpAttr",        "VgrpAttr"     };

        char        *CodeEnumName[15]   = {"aCreateCode",    "aDeleteCode", 
                                           "aPutObjCode",    "aGetObjCode",
                                           "aPutAttrCode",   "aGetAttrCode",
                                           "aCompAttrCode",  "aSpclCode",
                                           "aObjCode",       "aKernelCode", 
                                           "aShellCode",     "aVirtualCode",
                                           "aOvrloadCode",   "aDbeCode",
                                           "aDbmsCode" };

        char        *SetTypeNam[11]     = {"LocalSet",          "GlobalSet", 
                                   "LocalKeyedSet",     "GlobalKeyedSet",
                                   "LocalKeyed2arySet", "GlobalKeyed2arySet",
                                   "LocalTableSet",     "GlobalTableSet",
                                   "LocalTreeSet",      "GlobalTreeSet",
                                   "IndexedSet"};

        char        *TupleTypeNam[4]     = {"LocalTuple",  "GlobalTuple",
                                           "LocalPair",   "GlobalPair"};

        char        *SlotTypeNam[4]      = {"lTupleSlot",  "gTupleSlot", 
                                           "lPairSlot",   "gPairSlot" };

        char        *DependTypeNam[4]    = {"aIndependent", "aMaster",
                                            "aSlave",       "aDependent"};
#else

extern  char        *DescNam[];
extern  char        *LowDescNam[];
extern  char        *LowTypeNam[];
extern  char        *CppQualName[];

#ifndef TYPEDEFS_01
extern  char        *CompareOpName[];
#endif
extern  char        *CompareOpLex[];

extern  char        *SortOrderName[];
extern  char        *BooleanOpName[];

extern  char        *ArgClassName[];
extern  char        *ArgQualName[];
extern  char        *ArgQualLex[];
extern  char        *DbPutStr[];
extern  char        *HeapStr[];
extern  char        *TypeTypeNam[];
extern  char        *ValueClassNam[];
extern  char        *AttrTypeNam[];
extern  char        *SetTypeNam[];
extern  char        *TupleTypeNam[];
extern  char        *SlotTypeNam[];
extern  char        *DependTypeNam[];
extern  char        *CodeEnumName[];
#endif
 
/*+-----------------------------------------------------------------------+
~S                            STATUS                                      !
  +-----------------------------------------------------------------------+*/
#define  Env_AttrNotInAplClassDef  -21
#define  Env_AttrNotInDbmsClassDef -22

#define  Dbe_Exit                -200
#define  Dbe_InvalidType          201
#define  Dbe_VarNameReq           202
#define  Dbe_SemiReq              203
#define  Dbe_BadSyntax            204
#define  Dbe_UnMatchedBrace       205
#define  Dbe_NotFound            -206
#define  Dbe_EndOfExpression     -207
#define  Dbe_EndOfStatement      -208
#define  Dbe_EndOfDeclaration    -209
#define  Dbe_EndOfFunction       -210
#define  Dbe_EndOfArgument       -211
#define  Dbe_EndOfElement        -212
#define  Dbe_UnmatchedArgType     213
#define  Dbe_NotAnObjDesc         214
#define  Dbe_NullHandle           215
#define  Dbe_BadDbId              216
#define  Dbe_UnmatchedBracket     217
#define  Dbe_BadArraySyntax       218

#define Env01_BadDescTypeType    2005

/*               defined in defines.h
#define Oa_VchunkNotExist        5053
#define Oa_VchunkIndexTooLarge   5054
#define Oa_ValueNoFindInVchunk   5055
#define Oa_ValueAlreadyInVchunk  5056
#define Oa_NullObjHdr            5059
#define Oa_NewObjHdr             5060
#define Oa_ObjClassNotLoaded     5063
#define Oa_NotInCreateMode       5064
#define Oa_VchunkNoAccess        5065
#define Oa_ObjNotInImageForm     5066
#define Oa_BadObjStacks          5067
#define Oa_ObjectHasBeenDeleted  5069
*/
#define Oa_ObjNoThisAttr         5050
#define Oa_LessThen             -5051
#define Oa_GtrThen              -5052

#define Oa_ObjNoThisField        5057
#define Oa_DbNoThisAttr          5058
#define Oa_AttrNotPresent        5061
#define Oa_NullStringIndex       5062
#define Oa_BadDbToken            5068
 
/*
#define  Set_EmptySet           -10008
#define  Set_EndOfSet           -10009
#define  Set_Found              -10010
#define  Set_NotFound            10011
#define  Set_BadSet              10012
#define  Set_AlreadyInSet        10013
#define  Set_LessThen           -10014
#define  Set_GtrThen            -10015
#define  Set_InSerted           -10016
#define  Set_IndexGtrAllocRoots  10017
#define  Set_StartOfSet         -10020
#define  Set_AddInto             10025
#define  Set_KeyFound            10026
#define  Set_NullKeyName         10027
#define  Set_OutOfRange          10028
#define  Set_IllegalKeyType      10029
#define  Set_IllegalKeyString    10030
*/
#define Set_NotAnObjDesc        10001
#define Set_SetNameReq          10002
#define Set_DupObjName          10003
#define Set_DupSetName          10003
#define Set_BadDescName         10004
#define Set_DescNameReq         10005
#define Set_SetLinkSizeReq      10006
#define Set_NotAnAttrDesc       10007

#define Set_IsAnIndexedSet     -10018
#define Set_BadQuickPickSet     10019

#define Set_SetNodeNameReq      10021
#define Set_SetNodeDescReq      10022
#define Set_SetNotOfObjCestrs   10023 
#define Set_KeyNotOfObjCestrs   10024 

#define Set_NotAnOwnerClass     10025 

#define Spcl_NullKeyName        10050

#define Mtrx_DupMtrxName             10061
#define Mtrx_MtrxNameReq             10062
#define Mtrx_BadDescName             10063
#define Mtrx_DescNameReq             10064
#define Mtrx_OwnerObjReq             10065
#define Mtrx_RowHeadNameNotFound     10066
#define Mtrx_RowHeadNameReq          10067
#define Mtrx_RowHeadKeyNotAnAttrDesc 10068
#define Mtrx_RowHeadKeyNameReq       10069
#define Mtrx_ColHeadNameNotFound     10070
#define Mtrx_ColHeadNameReq          10071
#define Mtrx_ColHeadKeyNotAnAttrDesc 10072
#define Mtrx_ColHeadKeyNameReq       10073
#define Mtrx_PointNodeNameNotFound   10074
#define Mtrx_PointNodeNameReq        10075
#define Mtrx_InvalidAttrField        10076

#define Mtrx_AlreadyExists           10077
#define Mtrx_RowColNotOfSameMatrix   10080

#define Map_NoPortForThisObj    15011

#define Trans_NullMap           16000
#define Trans_NullAttrNum       16001
#define Trans_ArrayNotOfClass   16002
#define Trans_MixedArraySize    16003
#define Trans_NullBaseArray     16004
#define Trans_NullBaseArraySize 16005
#define Trans_AttrNotOfClass    16006
#define Trans_ThatNotOfClass    16007
#define Trans_NullObjSize       16008
#define Trans_CoreNoMore        16009
#define Trans_IndexOutOfRange   16010
#define Trans_AttrNotInClassDef 16011
#define Trans_CppShellNotInvoked 16012

#define Image_AttrNotArrayType      16020
#define Image_AttrNotDblPtrType     16021
#define Image_ArrayIndexOutOfRange  16022
#define Image_ArrayWasNotCreated    16023
#define Image_AttrNotRefTknType     16024
#define Image_AttrNotObjRefType     16025
#define Image_AttrNotStringType     16026
#define Image_AttrNotPutArrayType   16027


#define Attr_TransientNotPut        16040
#define Attr_TransientNotCreate     16041
#define Attr_ArrayIsActive          16042
#define Attr_NullArray              16043
#define Attr_NullSubArray           16044
#define Attr_ImageIsActive         -16045


#define Query_NotAnQueryOnArray     16050
#define Query_AttrNotArrayType      16051
#define Query_ArrayNotOfValueType   16052
#define Query_NotAnValueQuery       16053
#define Query_ArrayNotApplicable    16054
#define Query_SetOrTupleNotAllowed  16055
#define Query_InvalidTknCompare     16056
#define Query_MustBeValueQuery      16057
#define Query_UnitTypeNotHandle     16058
#define Query_AttrNotReference      16059
#define Query_BadSubQueryClassId    16060
#define Query_AttrMustBeSetOrArray  16061
#define Query_BadArrayQueryClass    16062

#define Cpp_ObjectNotActivated  17000
#define Cpp_NotOfThisClass      17001
#define Cpp_DbNotActivated      17002
#define Cpp_InvalidToken        17003

#define pShell_NullOffsetDir    17010
#define pShell_NullClassSizeDir 17011
#define pShell_NullTransImageDir    17012
#define pShell_NullTknCodeDir   17013
#define pShell_NullCastCodeDir  17014
#define pShell_NullNewCodeDir   17014
#define pShell_NewCodeMissing   17015

#define Image01_ThatUnassigned       17100
#define Image01_NullAddr             17101
#define Image01_CanNotDownCast       17102
#define Image01_ConstructorMissMatch 17103
#define Image01_NullTokenPtr         17104
#define Image01_NullToken            17105
#define Image01_BadCastInCode        17106
#define Image01_MixedTokensInNew     17107


#define Csh_DbAlreadyExists    -18000

#define Primal_NullDbName       18010
#define Primal_NullDbFileName   18011
#define Primal_NotOfSameDb      18012

#define IntChunk_NullVtoken     19000
#define IntChunk_IndexGtrStack  19001

/*+-----------------------------------------------------------------------+
~P                         END OF ENVDEFS.H                               !
  +-----------------------------------------------------------------------+*/
#endif
