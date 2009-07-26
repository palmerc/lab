/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* C_Shell - DOMAIN.h */
#ifndef DOMAIN01_H
#define DOMAIN01_H

#ifdef  Alloc_Grps
#undef  Alloc_Grps
#undef  Extend_Grps     
#endif

#ifdef  Alloc_TxtTkns
#undef  Alloc_TxtTkns
#undef  Extend_TxtTkns
#endif
/*+-----------------------------------------------------------------------+
  !                          ............                                 !
  !                 ....       IMAGE01      ....                          !
  !                          ............                                 !
  +-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/
/* General Definitions required by all application modules                 */

extern tokentype     NullToken;

#ifdef DOMAIN01
       indextype    Alloc_Vchunks          = 1801;
       indextype    Extend_Vchunks         = 200;

static indextype    Alloc_Grps             = 100;
static indextype    Extend_Grps            = 20;

static indextype    Alloc_TxtTkns          = 0;
static indextype    Extend_TxtTkns         = 0;

       indextype    Alloc_Handles          = 10000;
       indextype    Extend_Handles         = 1000;
       indextype    Alloc_SwapHandles      = 20;

       indextype    DbXmemBlocks_AllocQty  = 6000;
       indextype    DbXmemBlocks_ExtendQty = 1000; 

       boolean      Align_Bytes            = False;
       boolean      Align_CppStruc         = False;

#else
extern indextype    Alloc_Vchunks;
extern indextype    Extend_Vchunks;

extern indextype    Alloc_Handles;
extern indextype    Alloc_SwapHandles;
extern indextype    Extend_Handles;

extern indextype    DbXmemBlocks_AllocQty;
extern indextype    DbXmemBlocks_ExtendQty; 

extern boolean      Align_Bytes;
extern boolean      Align_CppStruc;
#endif

/*+-----------------------------------------------------------------------+
~C                           DBMS OBJECTS                                 !
  +-----------------------------------------------------------------------+*/

 typedef     enum    {               Domaya,         Shelldom,
                     Objdom,         Coredom,        Kingdom,
                     EmminentDomain, VirtualDomain,  VirtualImage,
                     III_View,       ManifestView,   aNonKeydom=EXT_ENUM }
                                                                 keydomtype;

 typedef     enum    {               IOdomdom,       Knight,
                     Bishop,         CastleKeep,     PrimeMinister,
                     InvisibleKing,  Sourceeror,     Artesian,
                     Manifest,       UnManifest,     aNonPatronage=EXT_ENUM }
                                                                 patronage;

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/

#define  EnvImplodeVoid            Domain_ImplodeMemory
#define  ImplodeMemory             Domain_ImplodeMemory
#define  EnvBeHereNow              Domain_BeHereNow
#define  BeHereNow                 Domain_BeHereNow
#define  EnterDomain               Domain_Enter
#define  ExitDomain                Domain_Exit

/*+-----------------------------------------------------------------------+
  !                  ....       DOMAIN       ....                         !
  +-----------------------------------------------------------------------+*/
#ifdef DOMAIN01
/* .......................................................................
                    ....     Protected      ....
   .......................................................................*/
/*     1.0.0  Implode Memory

       Manifest a 'Domain' within the bounds of Time, Space and Hardware. */

       boolean Domain_ImplodeMemory
                         (ft F,lt Z,zz *Status);


/*     1.0.1  Be Here Now

       Establish the Environment.                                         */

       boolean Domain_BeHereNow
                         (ft F,lt Z,zz *Status);

#endif

/* .......................................................................
                    ....     Privilaged     ....
   .......................................................................*/
/*     1.1.0.0  Enter the Domain...  Object Manager Interface                                          
       Request an entre' to a Realm of the Domain.                        */

       boolean Domain_MmiInvoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Xmem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          ft F,lt Z,zz *Status);

       boolean Domain_Omi0Invoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Mem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          indextype     AllocHandles,     indextype     ExtendHandles,
          indextype     AllocVchunks,     indextype     ExtendVchunks,
          indextype     AllocGrps,        indextype     ExtendGrps,
          indextype     AllocDbBlocks,    indextype     ExtendDbBlocks,
          char         *ParmsBuf,         ft F,lt Z,zz *Status);

       boolean Domain_OmiInvoke
         (char         *Dat_File,         char         *Msg_File,  
          char         *Out_File,         char         *Mem_File,  
          farlongtype   CorePageSize,     farlongtype   CoreMaxOnPage,
          indextype     AllocFreeAddrs,   indextype     ExtendFreeAddrs,
          indextype     AllocChunks,      indextype     ExtendChunks,
          indextype     AllocXmemBlocks,  indextype     ExtendXmemBlocks,
          indextype     AllocHandles,     indextype     ExtendHandles,
          indextype     AllocVchunks,     indextype     ExtendVchunks,
          indextype     AllocGrps,        indextype     ExtendGrps,
          indextype     AllocDbBlocks,    indextype     ExtendDbBlocks,
          ft F,lt Z,zz *Status);

/*     1.1.0.1  Enter the Domain

       Request an entre' to a Realm of the Domain.                        */

       boolean Domain_Enter
                         (char         *Subject,   keydomtype    KeyToRealm,
                          ft F,lt Z,zz *Status);


/*     1.1.1  Exit the Domain

       Demanifest the Domain.                                             */

       boolean Domain_Exit
                         (ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF DOMAIN.H                                !
  +-----------------------------------------------------------------------+*/
#endif
