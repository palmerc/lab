/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef DBM01_H
#define  DBM01_H

/* ENV - DBM.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
extern   boolean             FileBug;
#ifdef DBM_C
#ifdef DBM0
             idtype   DbmDbId   = 0;
             boolean  NewDbFile = False;
#else
    extern   idtype   DbmDbId;
    extern   boolean  NewDbFile;
#endif
#endif
 
#define DbmSetDefaults       Dbm_SetDefaults 
#define DbmGetDefaults       Dbm_GetDefaults
#define DbmCommitDb          Dbm_CommitDb    
#define DbmFreeDb            Dbm_FreeDb      
#define DbmFreeDbHdr         Dbm_FreeDbHdr   
#define DbmLoadDb            Dbm_LoadDb      
#define DbmLoadDbHdr         Dbm_LoadDbHdr   
#define DbmFileInDbHdr       Dbm_FileInDbHdr 
#define DbmFileInBlkHdr      Dbm_FileInBlkHdr

/*+-----------------------------------------------------------------------+
~P                          PROCEDURES                                    !
  +-----------------------------------------------------------------------+*/
/* PUBLIC  */
 
       void    Dbm_SetDefaults 
                         (indextype     EnvObjs,   indextype     EnvAttrs,  
                          indextype     AllocObjs, indextype     ExtendObjs,
                          indextype     AllocAttrs,indextype     ExtendAttrs,
                          indextype     AllocHndls,indextype     ExtendHndls,
                          indextype     AllocVchunks,indextype   ExtendVchunks, 
                          indextype     AllocGrps, indextype     ExtendGrps, 
                          indextype     AllocTexts,indextype     ExtendTexts, 
                          indextype     AllocXblks,indextype     ExtendXblks);

       void    Dbm_GetDefaults
                         (indextype    *EnvObjs,   indextype    *EnvAttrs,  
                          indextype    *AllocObjs, indextype    *ExtendObjs,
                          indextype    *AllocAttrs,indextype    *ExtendAttrs,
                          indextype    *AllocHndls,indextype    *ExtendHndls,
                          indextype    *AllocVchunks,indextype  *ExtendVchunks, 
                          indextype    *AllocGrps, indextype    *ExtendGrps, 
                          indextype    *AllocTexts,indextype    *ExtendTexts, 
                          indextype    *AllocXblks,indextype    *ExtendXblks);


       boolean Dbm_CommitDb
                         (tokentype    *DbToken,   boolean       RollOut,
                          ft F,lt Z,zz *Status);



       boolean Dbm_LoadDb 
                         (tokentype    *Anchor,    ft F,lt Z,zz *Status,
                          char         *DbName,    dbheader    **CoreDb);

       boolean Dbm_LoadDbHdr
                         (tokentype    *DbToken,   char         *LoadFile,
                          ft F,lt Z,zz *Status,    dbheader    **DbHdr);

       boolean Dbm_FileInDbHdr 
                         (dbheader     *DbHdr,     boolean       Release,
                          ft F,lt Z,zz *Status);

       boolean Dbm_FileInBlkHdr 
                         (dbheader     *DbHdr,     ft F,lt Z,zz *Status);


       boolean Dbm_FreeDb
                         (tokentype    *DbToken,   ft F,lt Z,zz *Status);


/*+-----------------------------------------------------------------------+
~P                         END OF DBM.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
