/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef FILES_H
#define FILES_H


/* FILES.h  */
/*+-----------------------------------------------------------------------+
~G                           GLOBAL DECLARATIONS                          !
  +-----------------------------------------------------------------------+*/
 
#ifdef UT
     char           UserName[MAXNAME]      = "SCAN";
 
     char           InFileName[MAXNAME]    = " ";
     FilePtrType    InFilePtr              = NullPtr;
     DbeFileType    InFileType             = aHistoryType;

     FilePtrType    MsgFilePtr             = NullPtr;
     FilePtrType    OutFilePtr             = NullPtr;
     FilePtrType    XmemFilePtr            = NullPtr;

     char           MsgFile[MAXNAME]       = " ";
     char           OutFile[MAXNAME]       = " ";
     char           XmemFile[MAXNAME]      = " ";

     FilePtrType    LogFilePtr             = NullPtr;
     FilePtrType    DiskFilePtr            = NULL;

     FilePtrType    TmpFilePtr             = NullPtr;
     FilePtrType    EnvFilePtr             = NullPtr;
 
     char           LogFile[MAXNAME]       = " ";
     char           DiskFile[MAXNAME]      = " ";

     char           DbeDbName[MAXNAME]     = " ";
     char           DbFileName[MAXNAME]    = "Renv.db";

#else
extern       char           UserName[MAXNAME];
 
extern       char           InFileName[MAXNAME];
extern       DbeFileType    InFileType;
extern       FilePtrType    InFilePtr;

extern       FilePtrType    MsgFilePtr;
extern       FilePtrType    OutFilePtr;
extern       FilePtrType    LogFilePtr;
extern       FilePtrType    DiskFilePtr;

extern       FilePtrType    XmemFilePtr;
extern       FilePtrType    TmpFilePtr;
extern       FilePtrType    EnvFilePtr;
 
extern       char           MsgFile[];
extern       char           OutFile[];
extern       char           XmemFile[];
extern       char           LogFile[];
extern       char           DiskFile[];
extern       char           DbeDbName[];
extern       char           DbFileName[];
#endif
 
/*+-----------------------------------------------------------------------+
~M                     MACRO DEFINES                                      !
  +-----------------------------------------------------------------------+*/
#define  SetInFile(Iread) (InFilePtr = Iread)

/*+-----------------------------------------------------------------------+
~P                         END OF FILES.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
