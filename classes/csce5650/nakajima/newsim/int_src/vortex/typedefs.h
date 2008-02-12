/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef TYPEDEFS_01
#define TYPEDEFS_01

/* VORTEx - TYPEDEFS.h  */
/*+-----------------------------------------------------------------------+
~T                            CONDITIONAL                                 !
*+-----------------------------------------------------------------------+*/

typedef  void              *AddrType;
typedef  void              *voidptr;
 
#ifdef __RISC_64__
typedef  int                lt64;
typedef  unsigned int       FarLongType;
#else
typedef  long               lt64;
typedef  unsigned long      FarLongType;
#endif
 
typedef  unsigned long      longaddr;
typedef  FarLongType        farlongtype;

/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
*+-----------------------------------------------------------------------+*/
typedef  char            *cstringtype;
typedef  cstringtype      cstring;
typedef  char             nametype[MAXNAME];
 
typedef  unsigned char    bitfieldtype;
typedef  unsigned char    bittype;
typedef  bittype          BitType;
typedef  bittype          BitArray[];
 
typedef  unsigned char    bytetype;
typedef  bytetype         ByteType;
typedef  ByteType         byte;
typedef  ByteType         ByteArray[];

#ifdef TWOBYTE_BOOL
#ifdef ONEBYTE_BOOL
   typedef  unsigned char    twobytetype[3];
#else
   typedef  unsigned char    twobytetype[2];
#endif
#endif

typedef  char             sbytetype;

typedef           short   shorttype;
typedef  unsigned short   unshorttype;

#ifdef LARGE
typedef  unshorttype      idtype;
typedef  unshorttype      id;

typedef  unsigned int     wordtype;
#define MAX_UNIT_LENGTH   999999L

#else
typedef  bytetype         idtype;
typedef  bytetype         id;

typedef  unsigned short   wordtype;
#define MAX_UNIT_LENGTH   64000
#endif 
 

typedef  wordtype         word;
typedef  wordtype         WordType;
typedef  wordtype         numtype;

typedef  wordtype         bytessizetype;
typedef  wordtype         bytessize;
typedef  wordtype         sizeinbytestype;
typedef  wordtype         sizeinbytes;
typedef  wordtype         sizeinblockstype;
typedef  wordtype         sizeinblocks;

typedef  wordtype         sizetype;
typedef  wordtype         size;
typedef  wordtype         typesizetype;
typedef  wordtype         typesize;

#ifdef LARGE
typedef  int              typeunsize;

#else
typedef  short            typeunsize;

#endif 

typedef  wordtype         regionoffsettype;
typedef  wordtype         regionoffset;
 
typedef  numtype          num;
typedef  numtype          objnumtype;
typedef  numtype          objnum;
typedef  numtype          vbntype;
typedef  numtype          vbn;
typedef  numtype          localnumtype;
typedef  numtype          localobjnum;
typedef  numtype          localattrtype;
typedef  numtype          localattrnum;
 
#ifdef LARGE
typedef  int              swordtype;

#else
typedef  short            swordtype;

#endif 
typedef  swordtype        SWordType;
typedef  swordtype        counttype;
typedef  swordtype        CountType;
typedef  counttype        count;
typedef  counttype        CountArray[];
typedef  swordtype        sword;
 
typedef  wordtype         indextype;
typedef  indextype        IndexType;
typedef  indextype        IndexArray[];

typedef  swordtype        sindextype;

typedef  int              inttype;
 
typedef  farlongtype      longtype;
typedef  longtype         LongType;
 
typedef  farlongtype      longwordtype;
 
typedef  double           doubletype;
 
typedef  AddrType         addrtype;
typedef  addrtype         addr;
typedef  addrtype         AddrArray[];

typedef  enum             TypeBoolean 
              {False,         True,   aNonBool=EXT_ENUM }
                          booleantype;
typedef  booleantype      boolean;
 
typedef  int              statustype;
#ifdef MCSTAT
typedef  char            *ft;
#else
typedef  unsigned int     ft;
#endif
typedef  unsigned int     lt;
typedef  statustype       zz;
 
typedef  numtype          chunknumtype;
typedef  chunknumtype     chunknum;
typedef  vbntype          regionnumtype;
typedef  regionnumtype    regionnum;
typedef  indextype        chunkindextype;
typedef  indextype        chunkindex;
typedef  indextype        regionindextype;
typedef  regionindextype  regionindex;
 
typedef  chunkindextype   handletype;
 
typedef  struct           TypeToken
{
   handletype      Handle;
   idtype          DbId;
   idtype          CoreDbId;
}                         tokentype;
typedef  tokentype        dbchunktkntype;
typedef  tokentype       *tokenptr;
 
typedef  struct           TypeQuad
{
   handletype      Lower;
   handletype      Upper;
}                         QuadType;
 

typedef  handletype       strhandletype;
typedef  strhandletype    strhandle;

typedef  struct           TypeStrToken
{
   tokentype       Token;
   strhandletype   StrHandle;
}                         strtokentype;
typedef  strtokentype     strtoken;
 

typedef  regionindextype  fieldindextype;
typedef  fieldindextype   fieldindex;
typedef  regionoffsettype fieldoffsettype;
typedef  fieldoffsettype  fieldoffset;

typedef  struct           TypeFldToken
{
   fieldindextype  Index;
   fieldoffset     Offset;
}                         FldTokenType;
typedef  FldTokenType     fieldtokentype;
typedef  fieldtokentype   fieldtoken;

 
typedef  struct           TypeFieldSpec
{
   indextype       AttrId;
   indextype       Index;
}                         fieldspectype;
typedef  fieldspectype    fieldspec;
 
typedef  struct           TypeFieldStruc
{
   char            FieldName[128];
   fieldspec      *FieldTkns;
   indextype       SubLevel;
   boolean         IsIndexed;
}                         fieldstructype;
typedef  fieldstructype   fieldstruc;
 
typedef struct            TypeBlkTkn 
{
   farlongtype     Offset;
   indextype       Owner;
}                         blktkntype;
 

typedef  struct           TypeDbmsString 
{
   bytessizetype   Size;
   booleantype     IsaString;
#ifdef TWOBYTE_BOOL
   twobytetype     FieldExtender01;
#endif
	union           string_or_int 
   {
      handletype   StringValue;
      longtype     IntValue;
	}                      Value;
}                         dbmsstringtype;
typedef  dbmsstringtype   dbmsstring;

typedef char             *varyingtype;

typedef struct            TypeRefTkn
{
   farlongtype     FieldNum;
   indextype       CppAlign;
}                         reftkntype;

typedef struct            TypeItrator
{
   numtype         SeqNum;
   numtype         LocalObjNum;
   tokentype       CurrentTkn;
}                         itrator;

/* This is the generic pointer to a function for Dynamic access           */
typedef  void   *(*funcptrtype)    (void);

 
typedef void     (*dumpentryptr)   (addrtype      Entry);
                                                
typedef boolean  (*ptr_func)       (addrtype      Object);

typedef int      (*ptr_cmp )       (addrtype      Value,    addrtype   Object);

typedef boolean  (*iteratetype )   (tokentype    *MemberTkn,indextype  ItNum,
                                    ft F,lt Z,zz *Status);

typedef boolean  (*treeiterfunc)   (tokentype    *Token,    indextype  ItNum,
                                    ft F,lt Z,zz *Status);

typedef int      (*ptr_TblCompare) (char         *Value1,   char      *Value2);
typedef ptr_TblCompare    tblcompfunc;
 
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

typedef enum              TypeDeleteType
               {dbFree,       dbCommit,      dbDelete,   aNonDelete=EXT_ENUM}   
                          deletetype;

typedef enum              TypeDbPutType
               {Put_NoFree,   Put_Release,   Put_Deep,   aNonDbPut=EXT_ENUM}
                          dbputtype;


typedef enum              KeyedSetType
               {aKeyedSet,    aMtrxHeadSet,  aNonKeyedSet=EXT_ENUM}       
                          keyedsettype;

typedef enum              ComparisonOpType
               {aEQ,                aLT,           aGT,
                aGTE,               aLTE,          aNEQ,
                aModEQ0,            aNonCompareOp=EXT_ENUM   } compareoptype;
typedef compareoptype     compareop;

typedef struct            TypeClassMapType
{
  handletype        Handle;
  numtype           ObjNum;
  indextype         NumMapFields;
  indextype         NumArrayAttrs;
  typesize          ClassSize;
  typesize          ImageSize;
  boolean           PureDbStruc;
  indextype         ThatIndex;
  typesize         *DbOffsets;
  typesize         *CppOffsets;
  typesize         *FieldSizes;
  attrtype         *FieldAttrTypes;
  numtype          *FieldAttrIds;      
  numtype          *RefObjNums;      
  numtype          *ArrayAttrs;      
  typetype         *FieldTypeTypes;

}                         classmaptype;
typedef classmaptype      classmap;



typedef struct            TypePortMapType
{
/* This is the Object at the Subject Level                                */
/* that is used to write the Application Interface TransPort Procedures   */
/* An instance of this object is passed in the PortCode Argument List     */

  handletype        Handle;
  numtype           ObjNum;

  typesize          ImageSize;
  indextype         FieldCount;
  numtype          *FieldAttrIds;      
  attrtype         *FieldAttrTypes;
  typetype         *FieldTypeTypes;
  sizetype         *FieldSizes;
  numtype          *FieldRefObjNums;      

  sizetype         *VfieldUnitSizes;      
  indextype        *VfieldUnitCounts;

  sizetype          PortImageSize;
  sizetype          MapTokenSize;

  indextype         AttrCount;
  numtype          *PortFields;
  sizetype         *PortOffsets;
  addrtype          PortMapTkns;       

  funcptrtype       ExportCode;
  funcptrtype       ImportCode;

}                         portmaptype;


typedef void             *foidtype;

typedef struct            TypeChunkStruc
{
  numtype           CRC;
  bytessize         UnitSize;
  indextype         UnitCount;
  indextype         StackPtr;
  numtype           StringText;
  sizeinbytes       MaxUnitLength;
  indextype         ExtendQty;
  valueclass        UnitType;
#ifdef TWOBYTE_BOOL
      twobytetype    FieldExtender01;
#endif
}                         chunkstruc;
typedef chunkstruc        vchunkstruc;
typedef tokentype         vchunktoken;

typedef tokentype         grptype;
typedef tokentype         vgrptype;

typedef numtype           vchunknum;
typedef numtype           grpnum;
typedef numtype           vgrpnum;

/*+-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*/
typedef struct            prttypetype
{
		unsigned int  Low;
		unsigned int  High;
}                         prttype;
 
typedef	 union prtaddrtype
{
		prttype       Parts;
      addrtype      Address;
}                         prtaddr;

#define Prt_Addr(Addr) (printf("%X:%X", Addr.Parts.High, Addr.Parts.Low))
 
/*+-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*/
typedef enum              TypeAccessType
               {Read_Write,     Shared_Write,
                Read_Only,      No_Access,       Unknown_Access=EXT_ENUM } 
                          accesstype;
typedef accesstype        dbaccesstype;

typedef  enum             TypeFileAccess  
               {R,              W,               B,   aNonFileAccess=EXT_ENUM}              
                          fileaccesstype;
typedef  FILE            *FilePtrType;
typedef  FilePtrType      filetokentype;

typedef  enum             TypeDbeFile
               {aHistoryType,   aInputType,      aNonDbeFile=EXT_ENUM}
                          DbeFileType;
typedef  char             filename[MAXPATHNAME];

typedef struct            TypeFileIndex
{
   count            SeqNum;
   count            OffSet;
   count            Size;
}                         FileIndexType;
 
typedef struct            TypeRecIndex
{
   count            SeqNum;
   count            Size;
}                         RecIndexType;


/*+-----------------------------------------------------------------------+
~T                        GLOBAL VARIABLESS                               !
*+-----------------------------------------------------------------------+*/
#ifdef UT
#ifdef LARGE
#ifdef __ZTC__
      extern        int  _okbigbuf;
#endif
#endif

       boolean      SWAP                   = False;

       boolean      DO_NOT_CREATE          = False;

       boolean      FREEZE_GRP_PACKETS     = False;

       longtype     VOID_BOUNDARY          = 20000000L;
       longtype     VOID_RESERVE           =  1000000L;

       char        *BoolStr[2]             = { "False",      "True" };

       char        *DbAccessStr[5]         = { "Read_Write", "Shared_Write",
                                               "Read_Only",  "No_Access",
                                               "Unknown_Access" };

       char        *AplStr[3]              = { "ASM", "'C'", "'C++'" };

       char        *DeleteStr[3]           = { "dbFree",     "dbCommit",
                                               "dbDelete" };

       tokentype    NullToken              = {0, 0, 0};
       handletype   NullHandle             = 0;
       idtype       NullDbId               = 0;
       numtype      NullObjNum             = 0;
       char        *NullStr                = {"\0"};
       addrtype     NullAddr               = NULL;

       boolean      Test                   = True;
       count        Trace1                 = 0;

       boolean      CppSyntax              = False;

       boolean      CoreBug                = False;
       boolean      Core0Bug               = False;
       boolean      Core1Bug               = False;
       boolean      PageBug                = False;
       boolean      SwapBug                = False;
       boolean      ChunkBug               = False;
       boolean      MemBug                 = False;
       boolean      GrpBug                 = False;
       boolean      FileBug                = False;
       boolean      DbmBug                 = False;
       boolean      LexBug                 = False;
       boolean      StackBug               = False;

       boolean      OaBug                  = False;
       boolean      SetBug                 = False;
       boolean      MtrxBug                = False;
       boolean      ClassBug               = False;
       boolean      AdminBug               = False;
       boolean      ListBug                = False;
       boolean      LibBug                 = False;

       numtype      QueBug                 = 100;


       int          DeBug                  = 0;
       int          DeBug0                 = 0;
       int          DeBug1                 = 0;

       char         TrackBak[MAXLINE]      = "Env_Normal\n";
       char         Msg[256]               = " ";
static sizetype     MsgMaxIndex            = 255;

       boolean      WriteHdr               = False;
       boolean      Print_Msg              = False;

    /* Boundary Alignment Byte Size                */
       sizetype     Byte_Alignment          = 1;     
       sizetype     Byte2Alignment         = 1;     
       sizetype     Byte4Alignment         = 1;     
       sizetype     Byte8Alignment         = 1;     
       sizetype     BytePtrAlignment       = 1;     
       sizetype     StrucAlignment         = 1;     
       sizetype     CppAlignment           = 1;     

       char        *AccessStr[5]           = { "Read_Write", "Shared_Write", 
                                               "Read_Only",  "No_Access", 
                                               "Unknown"                    };

       char        *CompareOpName[7]       = {"aEQ",      "aLT",   "aGT",
                                              "aGTE",     "aLTE",  "aNEQ",
                                              "aModEQ0" };

#else

extern boolean      SWAP;
extern boolean      DO_NOT_CREATE;
extern boolean      FREEZE_GRP_PACKETS;
extern longtype     VOID_BOUNDARY;
extern longtype     VOID_RESERVE;

extern char        *BoolStr[];
extern char        *DbAccessStr[];
extern char        *AplStr[];
extern char        *DeleteStr[];

extern tokentype    NullToken;
extern handletype   NullHandle;
extern idtype       NullDbId;
extern numtype      NullObjNum;
extern char        *NullStr;
extern addrtype     NullAddr;

extern boolean      Test;
extern count        Trace1;

extern boolean      CppSyntax;

extern boolean      CoreBug;
extern boolean      Core0Bug;
extern boolean      Core1Bug;
extern boolean      PageBug;
extern boolean      ChunkBug;
extern boolean      MemBug;
extern boolean      GrpBug;
extern boolean      DbmBug;
extern boolean      FileBug;
extern boolean      SwapBug;
extern boolean      LexBug;
extern boolean      StackBug;
extern boolean      OaBug;
extern boolean      SetBug;
extern boolean      MtrxBug;
extern boolean      ClassBug;
extern boolean      AdminBug;
extern boolean      ListBug;
extern boolean      LibBug;

extern numtype      QueBug;


extern int          DeBug;
extern int          DeBug0;
extern int          DeBug1;

extern char         TrackBak[];
extern char         Msg[];

extern boolean      WriteHdr;
extern boolean      Print_Msg;

extern sizetype     Byte_Alignment;     
extern sizetype     Byte2Alignment;     
extern sizetype     Byte4Alignment;     
extern sizetype     Byte8Alignment;     
extern sizetype     BytePtrAlignment;     
extern sizetype     CppAlignment;     
extern sizetype     StrucAlignment;     

extern char        *AccessStr[];
extern char        *CompareOpName[];

#endif
 
#ifdef UT
       boolean      Renv_Modified          = False;

       boolean      Build_Env              = False;
       boolean      Admin_Run              = False;
       boolean      Dbe_Run                = False;
       boolean      Dri_Env                = False;
       boolean      Install_Env            = False;
       boolean      Reload_Env             = False;
       boolean      Reload_Wenv            = False;
       boolean      Commit_Env             = False;
       boolean      Create_Wenv            = False;
       boolean      Gen_Shell              = False;
#else
extern boolean      Renv_Modified;

extern boolean      Build_Env;
extern boolean      Admin_Run;
extern boolean      Dbe_Run;
extern boolean      Dri_Env;
extern boolean      Install_Env;
extern boolean      Reload_Env;
extern boolean      Reload_Wenv;
extern boolean      Commit_Env;
extern boolean      Create_Wenv;
extern boolean      Gen_Shell;

#endif


#include "ut.h"

/*+-----------------------------------------------------------------------+
~P                         END OF TYPEDEFS.H                              !
  +-----------------------------------------------------------------------+*/
#endif
