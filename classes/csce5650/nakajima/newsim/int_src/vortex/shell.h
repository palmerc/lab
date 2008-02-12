/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Shell - SCHELL.h */
#ifndef SCHELL_H
#define SCHELL_H


/*+-----------------------------------------------------------------------+*
~t                          PROCS / TYPEDEFS
 *+-----------------------------------------------------------------------+*/

#ifdef SHELL

typedef void*     (*new_code)      (size_t   ClassSize);

typedef tokentype (*GetToken_code) (addrtype This);


typedef boolean (*shell_code00) (void);
typedef struct Code04StackBLk {
  char   StackBlk[4];
}       code04type;
typedef boolean      (*shell_code04) (code04type    ArgStack);

typedef struct Code08StackBLk {
  char   StackBlk[8];
}       code08type;
typedef boolean      (*shell_code08) (code08type    ArgStack);

typedef struct Code16StackBLk {
  char   StackBlk[16];
}       code16type;
typedef boolean       (*shell_code16) (code16type    ArgStack);

typedef struct Code20StackBLk {
  char   StackBlk[20];
}       code20type;
typedef boolean       (*shell_code20) (code20type    ArgStack);

typedef struct Code24StackBLk {
  char   StackBlk[24];
}       code24type;
typedef boolean       (*shell_code24) (code24type    ArgStack);

#endif

/*+-----------------------------------------------------------------------+
~M                           DBMS METHODS                                 !
  +-----------------------------------------------------------------------+*/


       boolean ShellLoadObjCode  (numtype       ObjNum,  funcptrtype CodeAddr,
                                  ft F,lt Z,zz *Status);

       boolean ShellGetObjCode   (numtype       ObjNum,
                                  ft F,lt Z,zz *Status,  
                                  funcptrtype  *CodeAddr, size_t   *Size);



       boolean ShellLoadTokenCode
                                 (numtype       ObjNum,  funcptrtype CodeAddr,
                                  ft F,lt Z,zz *Status);

       boolean ShellGetTokenCode (numtype       ObjNum,
                                  ft F,lt Z,zz *Status, funcptrtype *CodeAddr);


/*+-----------------------------------------------------------------------+
~P                         END OF SHELL.H                                 !
  +-----------------------------------------------------------------------+*/
#endif
