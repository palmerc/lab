/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



#ifndef OMIDEFS_H
#define OMIDEFS_H

/* VORTEx - BUILD DEFS.H  (limited Sub-Set)                               */
/*+-----------------------------------------------------------------------+
~D                           DEFINES                                      !
  +-----------------------------------------------------------------------+*/
#define        Alloc_RootSlots        (indextype  )50

#define        Alloc_Primals          (indextype  )1
#define        Alloc_Descs            (indextype  )600


#define        ObjDesc_ObjNum         (objnumtype )1
#define        Primal_ObjNum          (objnumtype )25
#define        Renv0_ObjNum           (objnumtype )26
#define        SetHead_ObjNum         (objnumtype )69
#define        lSetHead_ObjNum        (objnumtype )70     
#define        gSetHead_ObjNum        (objnumtype )71     
#define        lKeyedSetHead_ObjNum   (objnumtype )73
#define        gKeyedSetHead_ObjNum   (objnumtype )74
#define        lKeyed2arySetHead_ObjNum   (objnumtype )76
#define        gKeyed2arySetHead_ObjNum   (objnumtype )77
#define        lTableSetHead_ObjNum   (objnumtype )79
#define        gTableSetHead_ObjNum   (objnumtype )80
#define        lTreeSetHead_ObjNum    (objnumtype )81
#define        gTreeSetHead_ObjNum    (objnumtype )82
#define        Node_ObjNum            (objnumtype )83 
#define        lSetNode_ObjNum        (objnumtype )85
#define        gSetNode_ObjNum        (objnumtype )86
#define        lTableNode_ObjNum      (objnumtype )111  
#define        LsMatrix_ObjNum        (objnumtype )120
#define        Wschema_ObjNum         (objnumtype )139
#define        Db_ObjNum              (objnumtype )141
#define        CodeDesc_ObjNum        (objnumtype )145
#ifndef ENVDEFS_H
#define        Image01_ObjNum         (objnumtype )149
#endif

/* as defined in lr01.dat                                                  */
#define        lKeyHead_ObjNum        (objnumtype )157
#define        MtrxHead_ObjNum        (objnumtype )158
/* NOTE ::
   additional _ObjNum declarations are defined in 'value.h'
*/

#ifndef ENVDEFS_H
#define        That_AttrNum               (numtype )1
#define        Token_Attr                 (numtype )2
#define        Handle_AttrNum             (numtype )2
#define        DbId_AttrNum               (numtype )3
#define        CoreDbId_AttrNum           (numtype )4
#define        ObjNum_AttrNum             (numtype )5
#endif

#define        AttrNum_AttrNum            (numtype )9

#define        CodeNum_AttrNum            (numtype )17
#define        DbNum_AttrNum              (numtype )18
#define        Nam_AttrNum                (numtype )19
#define        EnvType_AttrNum            (numtype )20

#define        DescNum_AttrNum            (numtype )22

#define        DbCount_AttrNum            (numtype )38

#define        SetHeadType_AttrNum        (numtype )44

#define        OwnerAttrNum_AttrNum       (numtype )61
#define        MemberAttrNum_AttrNum      (numtype )62

#define        PrimaryKey_AttrNum         (numtype )64

#define        AccessRights_AttrNum       (numtype )70

#define        BaseCreateCode_AttrNum     (numtype )72

#define        lOwnerObject_AttrNum       (numtype )88

#define        lFirstLink_AttrNum         (numtype )89
#define        gFirstLink_AttrNum         (numtype )90

#define        lLastLink_AttrNum          (numtype )91
#define        gLastLink_AttrNum          (numtype )92

#define        lPrevLink_AttrNum          (numtype )93
#define        gPrevLink_AttrNum          (numtype )94

#define        lNextLink_AttrNum          (numtype )95
#define        gNextLink_AttrNum          (numtype )96

#define        lMemberObject_AttrNum      (numtype )97
#define        gMemberObject_AttrNum      (numtype )98

#ifndef ENVDEFS_H
#define        Id_AttrNum                 (numtype )144
#endif

#define        RootNodes_AttrNum          (numtype )156
#define        DbAccessType_AttrNum       (numtype )157
#define        SchemaPrimalDesc_AttrNum   (numtype )158
#define        DbFileName_AttrNum         (numtype )159
#define        ObjsOfSchema_AttrNum       (numtype )160

#define        OwnerWenvDbs_AttrNum       (numtype )253

#define        DbSlotAttr_AttrNum         (numtype )267
#define        PrimalSlotAttr_AttrNum     (numtype )268
#define        RschemaSlotAttr_AttrNum    (numtype )269
#define        WschemaSlotAttr_AttrNum    (numtype )270


#define        DbPrimal_Index         (indextype  )0
#define        ObjDesc_Index          (indextype  )1

#define        DbPrimal_Class         (word       )0
#define        ObjDesc_Class          (numtype    )1


#define        ObjDesc_Hndl           (handletype )3

#define        ArgDesc_Hndl           (handletype )149

#define        StatusType_Hndl        (handletype )181

#define        AddrType_Hndl          (handletype )184

#define        ReferenceType_Hndl     (handletype )197

#define        ObjTraits_Hndl         (handletype )681

#define        EnumValues_Hndl        (handletype )688

#define        TraitObjs_Hndl         (handletype )691

#define        RenvObjs_Hndl          (handletype )697
#define        RenvTraits_Hndl        (handletype )698
#define        RenvStrucs_Hndl        (handletype )699
#define        RenvAttrs_Hndl         (handletype )700
#define        RenvSets_Hndl          (handletype )701
#ifndef ENVDEFS_H
#define        RenvTypes_Hndl         (handletype )702
#endif
#define        RenvValues_Hndl        (handletype )703
#define        RenvTuples_Hndl        (handletype )704
#define        RenvSlots_Hndl         (handletype )705
#define        RenvMatrices_Hndl      (handletype )706
#define        RenvSchemas_Hndl       (handletype )707
#define        RenvViews_Hndl         (handletype )708
#define        RenvShells_Hndl        (handletype )709
#define        RenvCodes_Hndl         (handletype )710
#define        RenvDescs_Hndl         (handletype )711
#define        RenvCodeDescs_Hndl     (handletype )712
#define        RenvNames_Hndl         (handletype )713

#define        ObjCodes_Hndl          (handletype )714
#define        WenvDbs_Hndl           (handletype )715
#define        SchemaDbs_Hndl         (handletype )716

#define        ApiPortObjs_Hndl       (handletype )718


/* STATUS VALUES                                                          */
#define        Env01_BadDescTypeType                2005
#define        Code_ForeignNotCreate               14000

/*+-----------------------------------------------------------------------+
~P                         END OF OMIDEFS.H                               !
  +-----------------------------------------------------------------------+*/
#endif
