/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


#ifndef UT_01
#define UT_01

#ifdef UT
static char         III[9]                 = "VORTEx ";
       voidptr      PtrToVoid              = (voidptr )&III;

static char         TrackBack              = 1;

static boolean      Watch_Set              = False;
static boolean      Break_Set              = False;
static char        *WatchAddr              = NullPtr;
static sizetype     WatchSize              = NullPtr;
static char         WatchWord[MAXLINE]     = " ";

#else
extern voidptr      PtrToVoid;

#endif

/*    I.   Utility Procedures                                             */
#define        PrintErr            Ut_PrintErr   
#define        SetBreak            Ut_SetBreak   
#define        SetWatch            Ut_SetWatch   
#define        StopWatch           Ut_StopWatch  
#define        VoidTrack           Ut_VoidTrack  
#define        PrintTrack          Ut_PrintTrack 
#define        StackTrack          Ut_StackTrack 
#define        TraceMsg            Ut_TraceMsg   
#define        SendMsg             Ut_SendMsg    
#define        TraceBytes          Ut_TraceBytes 
#define        TraceField          Ut_TraceField 
#define        AlignMember         Ut_AlignMember
#define        AlignStruc          Ut_AlignStruc 
#define        StrToUpper          Ut_StrToUpper 
#define        StrToLower          Ut_StrToLower 

/*     0.0.1                                                               */
       void    Ut_SetBreak 
                         (boolean       Break);

/*     0.0.2                                                               */
       boolean Ut_SetWatch
                         (addrtype      VoidAddr,  sizetype      WordSize);

/*     0.0.3                                                               */
       boolean Ut_StopWatch 
                         (addrtype      VoidAddr);

/*     0.0.4                                                               */
       boolean Ut_VoidTrack 
                         (ft F,lt Z,zz *Status);

/*     0.0.5                                                               */
       void    Ut_StackTrack 
                         (char         *ProgNam);

/*     0.0.6                                                               */
       boolean Ut_PrintErr
                         (ft F,lt Z,zz  Status);

/*     0.0.7                                                               */
       boolean Ut_TraceMsg
                         (int           TraceLevel,char         *Line);

/*     0.0.8                                                               */
       boolean Ut_SendMsg  
                         (int           MsgLevel,  char         *Line);

/*     0.0.9                                                               */
       boolean Ut_TraceBytes
                         (int           TraceLevel,addrtype      SomeBytes,
                          sizetype      Size);

/*     0.0.10                                                              */
       boolean Ut_TraceField
                         (int           TraceLevel,addrtype      FieldAddr, 
                          sizetype      Size,      typetype      Type);

/*     0.0.9                                                               */
       boolean Ut_TraceValue
                         (int           TraceLevel,addrtype      Value,
                          valueclass    ValueClass,sizetype      ValueSize,
                          numtype       ValueObj);

/*     0.0.11                                                              */
       char   *Ut_StrToUpper 
                         (char         *pc);

/*     0.0.12                                                              */
       char   *Ut_StrToLower 
                         (char         *pc);

/*     0.0.13                                                              */
       void    Ut_ReverseStr 
                         (char          Str[]);

/*     0.0.14                                                              */
       void    Ut_IntToStr
                         (int           IntNum,    char          Str[]);

/*     0.0.15                                                              */
       boolean Ut_FindInList
                         (char         *Word,      char        **List,
                          numtype       MaxList,   int          *Index);

/*     0.0.16                                                              */
       boolean Ut_CompareWild
                         (char         *VarValue,  compareop     CompOp,
                          char         *WildSpec,  ft F,lt Z,zz *Status);

/*     0.0.17                                                              */
       int     Ut_CompareString 
                         (char         *CompValue, char         *ValuePtr);

/*     0.0.18                                                              */
       int     Ut_Random ( void );

#ifndef CORE_01
#define        MoveBytes           Ut_MoveBytes      
#define        CompareBytes        Ut_CompareBytes      
/*     0.0.13                                                              */
       void    Ut_MoveBytes
                         (addrtype      Source,    addrtype      Target, 
                          bytessize     ByteCount);

/*     0.0.14                                                              */
       int     Ut_CompareBytes
                         (addrtype      Source,    addrtype      BytesAddr,
                          bytessizetype ByteCount, ft F,lt Z,zz *Status);
#endif

/*     0.0.15                                                              */
       boolean  Ut_PrintTrack 
                         (ft F,lt Z);


/*     0.0.16                                                              */
       void     Ut_AlignMember
                         (typetype      Type_Type,
                          sizetype      Size,      sizetype     *Offset);

/*     0.0.17                                                              */
       void     Ut_AlignStruc 
                         (sizetype     *Offset);

#ifndef CORE_01
#define        VoidExtendCore      Void_ExtendCore
#define        VoidFreeCore        Void_FreeCore

       addrtype
               VoidExtendCore 
                         (farlongtype  *SizeInBytes);

       void    VoidFreeCore 
                         (addrtype     *CoreAddr,  farlongtype   SizeInBytes);

#define        Core0_FreeCore      Core0_FreeCoreSpace

       boolean Core0_MoreCore  
                         (bytessizetype Size,
                          ft F,lt Z,zz *Status,    addrtype     *Addr);

       boolean Core0_FreeCoreSpace
                         (addrtype     *CoreAddr,  bytessize     SizeInBytes,
                          ft F,lt Z,zz *Status);

#endif

/*+-----------------------------------------------------------------------+
~P                         END OF UT.H                                    !
  +-----------------------------------------------------------------------+*/
#endif
