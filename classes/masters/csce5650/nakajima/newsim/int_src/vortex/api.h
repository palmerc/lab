/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ENV - API */
/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
typedef struct ApiPortHead
{
   handletype        Handle;
   numtype           ShellNum;
   char              Nam[MAXNAME];
   envclass          EnvType;
   numtype           DescNum;

   handletype        MemberRenvShells;
   handletype        MemberRenvDescs;
   handletype        OwnerShellSchemas;
   handletype        OwnerApiPortObjs;
   handletype        CodesInShell;
   handletype        ObjsInShell;

} portheadtype;

typedef struct TransPortHead
{
   handletype        Handle;
   numtype           ShellNum;
   char              Nam[MAXNAME];
   envclass          EnvType;
   numtype           DescNum;
   handletype        MemberRenvShells;
   handletype        MemberRenvDescs;
   handletype        OwnerShellSchemas;
   handletype        OwnerApiPortObjs;
   handletype        CodesInShell;
   handletype        ObjsInShell;

   numtype           MapCount;
   numtype           FieldCount;
   handletype        OwnerApiPortAttrs;
   handletype        OwnerTransMapNames;
   handletype        OwnerTransFieldNames;
   handletype        OwnerTransPortMaps;
   handletype        OwnerTransPortFields;

} transporthead;

typedef struct TypeApiPortObj
{
   handletype        Handle;
   numtype           ObjNum;
   char              Nam[MAXNAME];
   numtype           AttrCount;
   handletype        PortFields;
   handletype        PortOffsets;
   handletype        MemberApiPortObjs;
   numtype           ExportCode;
   numtype           ImportCode;

} portobjtype;

typedef struct TransPortMap
{
   handletype        Handle;
   numtype           ObjNum;
   char              Nam[MAXNAME];
   numtype           AttrCount;
   handletype        PortFields;
   handletype        PortOffsets;
   handletype        MemberApiPortObjs;
   numtype           ExportCode;
   numtype           ImportCode;

   numtype           MapNum;
   sizetype          MapImageSize;
   sizetype          MapTokenSize;
   handletype        PortAttrs;       
   handletype        PortMapTkns;       

   handletype        MemberTransMapNames;
   handletype        MemberTransPortMaps;
   handletype        OwnerPortMapFields;
   handletype        MemberPortFieldMaps;

} transportmap;


typedef struct TransPortField
{
   handletype        Handle;
   numtype           FieldNum;
   char              Nam[MAXNAME];
   numtype           AttrNum;
   numtype           MapIndex;
   numtype           FieldExportCode;
   numtype           FieldImportCode;

   handletype        MemberApiPortAttrs;
   handletype        MemberTransFieldNames;
   handletype        MemberTransPortFields;
   handletype        MemberPortMapFields;
   handletype        OwnerPortFieldMaps;

} transportfield;


/*
SET ApiPortObjs     LocalKeyedSet 4  ApiPortObj     ApiPortObj      ObjNum 
SET ApiPortAttrs    LocalKeyedSet 4  TrnasPortHead  TransPortField  AttrNum
SET TransMapNames   LocalKeyedSet 4  TransPortHead  TransPortMap    Nam
SET TransFieldNames LocalKeyedSet 4  TransPortHead  TransPortField  Nam
SET TransPortMaps   LocalKeyedSet 4  TransPortHead  TransPortMap    MapNum
SET TransPortFields LocalKeyedSet 4  TransPortHead  TransPortField  FieldNum
SET PortMapFields   LocalKeyedSet 4  TransPortMap   TransPortField  FieldNum
SET PortFieldMaps   LocalKeyedSet 4  TransPortField TransPortMap    MapNum
*/

/*
 This is the Object at the Subject Level 
 that is used to write the Application Interface TransPort Procedures
 An instance of this object is passed in the PortCode Argument List
*/
/*
class portmaptype
{
  handletype            Handle;
  numtype               ObjNum;

  sizetype              ImageSize;

  indextype             FieldCount;
  numtype              *FieldAttrIds;      
  attrtype             *FieldAttrTypes;
  typetype             *FieldTypeTypes;
  sizetype             *FieldSizes;
  numtype              *FieldRefObjNums;      

  sizetype             *VfieldUnitSizes;      
  indextype            *VfieldUnitCounts;

  sizetype              PortImageSize;
  sizetype              MapTokenSize;

  indextype             AttrCount;
  numtype              *PortFields;
  sizetype             *PortOffsets;
  addrtype              PortMapTkns;       

  funcptrtype           ExportCode;
  funcptrtype           ImportCode;

};
*/
/*+-----------------------------------------------------------------------+
~P                         END OF API.H                                   !
  +-----------------------------------------------------------------------+*/
