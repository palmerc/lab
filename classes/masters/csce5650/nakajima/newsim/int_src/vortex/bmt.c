/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ******************************  Edb0.C   *****************************  */
/* *  Entry to the BMT test module.                                        */
/* *                               Recipies in the Developers CookBook     */
/* *                                                                       */
/* *         The BMT (Engineering Database Benchmark) Test Module          */
/* *                                                                       */
/* *********************************************************************** */

#ifdef    DEBUG
#define  __DISPLAY_TIME__
#define  __DISPLAY_CORE__
#endif /* DEBUG */

/* BMT_C.c */
#define BMT_C
#include "bmt01.h"
#ifdef __DISPLAY_TIME__
#include <time.h>
#endif /* __DISPLAY_TIME__ */
#include "files.h"
#include "grp.h"
#include "vchunk.h"
#include "rects.h"
#include "emplib.h"
#include "empobj.h"
#include "list01.h"

typedef struct TypeBlkHdr 
{
         filetokentype BlkFile;
         idtype        DbId;
         idtype        CoreDbId;
         numtype       HeadFreeVbns;
         numtype       BlkDirChunk;
         numtype       HeadFreeBlks;
         numtype       HeadNullBlks;
         numtype       BlkTknChunk;
         numtype       BlkSizeChunk;
         farlongtype   EofMark;

} blkhdrtype;

typedef struct TypeObjHeader
{ 
     /* This portion of struct is common to both GRP and ObjHdr           */

     wordtype    EnvObjType;      
                                  
     accesstype  LocalAccess;     
     boolean     DiskCache;       

     indextype   LocalObjNum;     

     valueclass  UnitType;
     numtype     UnitObj;
     sizeinbytes ObjSize;         
     indextype   ObjectCount;     

     numtype     RegionObjects;   
     numtype     RegionCount;     
     numtype     RegionsAllocd;   

     indextype   RegionIndex;     
     boolean     NoRegionsInCore; 
     numtype     FreezeClass;     

                                  
     numtype     RegionChunkDir;  
     numtype     RegionSwapDir;   
     numtype     DirtyRegionDir;  
     numtype     RegionFreezeDir; 
     numtype     RgnBuffNumDir;   
     numtype     RgnAccessedDir;  

     numtype     RegionVbnDir;    
     vbntype     RegionVbnDirVbn;

     indextype   ExObjectCount;
     numtype     ExObjectTbl;     
     vbntype     ExObjectTblVbn;
     numtype     NextEx;          

     indextype   NextEntry;       
     indextype   BaseIndex;       
                                  
     numtype     BaseRegion;      

     indextype   CacheLimit;      
     indextype   CacheLoad;       

     numtype     NumBuffRgns;     
     indextype   RgnBuffCount;    

     numtype     BuffRgnsDir;     
     numtype     BuffNodesDir;    

     indextype   HeadRgnBuffs;    
     indextype   LastRgnBuff;     

     indextype   RgnStatusDir;    
     indextype   RgnStatusDirVbn; 

     numtype     RgnBuffIdsDir;

     indextype   GrpIndexDir;     
     indextype   GrpIndexDirVbn;  
     indextype   GrpDirCount;     
     lt64        HeadExIndexs;    

     numtype    *FrozenRgns;      
     numtype    *RgnBuffNums;     
     numtype    *RgnAccessArray;  
     indextype  *RgnBuffRgns;     
     indextype  *RgnBuffNodes;    
     indextype  *RgnStats;        

     indextype  *RgnBuffIds;

     lt64       *GrpDirAddr;      

#ifndef __RISC_64__
     numtype    *FrozenRgns_64;
     numtype    *RgnBuffNums_64;
     numtype    *RgnAccessArray_64;

     indextype  *RgnBuffRgns_64;
     indextype  *RgnBuffNodes_64;

     indextype  *RgnStats_64;

     indextype  *RgnBuffIds_64;

     lt64       *GrpDirAddr_64;
#endif /* __RISC_64__ */

     indextype   ObjectLockDir;   
     indextype   ObjectLockDirVbn;

     sizeinbytes PsudoSize;       

     numtype     ObjNum;          

     indextype   NextObject;      
                                  

     SWordType   HandleOffset;    

     numtype     FieldNumMap;     
     vbntype     FieldNumMapVbn;
     numtype     LocalAttrNumMap; 
     vbntype     LocalAttrNumMapVbn;
 
     indextype   FieldCount;      
     numtype     FieldOffsetDir;  
     vbntype     FieldOffsetDirVbn;
     numtype     ClassOffsetDir;  
     vbntype     ClassOffsetDirVbn;

     indextype   FieldRefCount;   
     numtype     FieldRefDir;     
     vbntype     FieldRefDirVbn;

     indextype   FieldMapCount;   
     numtype     FieldMapDir;     
     vbntype     FieldMapDirVbn;

     indextype   FieldArrayCount; 
     numtype     FieldArrayDir;   
     vbntype     FieldArrayDirVbn;
 
     indextype   StringCount;     
     indextype   TextChunk;       
     vbntype     TextChunkVbn;
 
}                         objheader;

typedef struct TypeDbHeader
{
     idtype      DbId;            
     idtype      CoreDbId;

     indextype   NextHandle;      
     indextype   HandleExQty;     
     indextype   HandleUnits;     
 
     filename    DbFileName;      
     filename    ActiveFileName;  
     accesstype  LocalAccess;     
     boolean     DiskCache;       

     blkhdrtype  BlkHdr;          
                                  

     indextype   GrpCount;        
     numtype     GrpHdrDir;       
     vbntype     GrpHdrDirVbn;
     vbntype     GrpVbnDir;       
     vbntype     GrpVbnDirVbn;
     indextype   HeadFreeGrps;    

     numtype     VchunkCommitDir; 
     numtype     VchunkSwapDir;   

     vbntype     VbnDeleteDir;    
     vbntype     FreeVchunkVbn;   

     indextype   VchunkGrpNum;    
     indextype   VchunkBaseIndex; 

     indextype   HandleCount;     
     indextype   HandleBaseIndex; 

     numtype     ObjectAddrDir;   
     vbntype     ObjectAddrDirVbn;
     farlongtype ObjectAddrDirOffset;
     indextype   ObjectAddrGrpNum;

     numtype     ObjectMemLocDir; 

     numtype     ObjectMemRefDir; 

     numtype     ObjectIndexDir;  
     vbntype     ObjectIndexDirVbn; 
     farlongtype ObjectIndexDirOffset;
     indextype   ObjectIndexGrpNum;

     numtype     LocalObjNumDir;
     vbntype     LocalObjNumDirVbn;
     farlongtype LocalObjNumDirOffset;
     indextype   LocalObjNumGrpNum;

     indextype   LastEnvObjCount; 

     numtype     LocalObjNumMap;
     vbntype     LocalObjNumMapVbn;

     numtype     ObjHandleDir;    
     numtype     ObjHandleDirVbn; 

     indextype   LocalObjCount;   
     numtype     ObjNumMap;       
     vbntype     ObjNumMapDirVbn;
 
     numtype     ObjTimeStampDir; 
                                  
                                  
                                  
     numtype     ObjTimeStampVbn;

     numtype     ObjHdrDir;       
     vbntype     ObjHdrDirVbn;
     numtype     ObjVbnDir;       
     vbntype     ObjVbnDirVbn;

     indextype   LastEnvAttrCount;
     numtype     LocalAttrNumMap; 
     vbntype     LocalAttrMapVbn;

     numtype     AttrHandleDir;   
                                  
     vbntype     AttrHandleDirVbn;

     indextype   LocalAttrCount;  
     numtype     AttrNumMap;      
     vbntype     AttrNumMapVbn;
 
     numtype     LocalAttrSizeTbl;
     vbntype     LocalAttrSizeVbn;
 

     numtype     TextTokenDir;    
     numtype     TextTknDirVbn;   

     numtype     FreeTxtTknStack; 
     vbntype     FreeTxtTknVbn;   
 
     indextype   FutureHandles;   

     indextype   CurrentHandles;  

     indextype   AvailHandles;    
     numtype     NewHandles;      
     vbntype     NewHandlesVbn;
     indextype   NextFree;        
 

     numtype     StackHeadDir;    
     handletype  StackNames;      

     numtype     ListHeadDir;     
     handletype  ListNames;       

     numtype     TblHeadDir;      
     handletype  TblNames;        

     numtype     TreeHeadDir;     
     handletype  TreeNames;       

     numtype     lKeyHeadDir;     
     handletype  lKeyNames;       

     numtype     MtrxHeadDir;     
     handletype  MtrxNames;       


     addrtype    PrimalHdr;       
     sizetype    PrimalHdrSize;   
     vbntype     PrimalHdrVbn;    

     vbntype     EOFVbn;          

     indextype   BlkBlksBaseIndex;
     indextype   BlkBlksExtendQty;

     vbntype     BlkDirVbn;       
     indextype   BlkDirUnits;     
     farlongtype BlkDirOffset;    
     indextype   BlkDirGrpNum;    

     vbntype     BlkTknVbn;
     indextype   BlkTknUnits;
     farlongtype BlkTknOffset;
     indextype   BlkTknGrpNum;    

     vbntype     BlkSizeVbn;
     farlongtype BlkSizeOffset;
     indextype   BlkSizeGrpNum;   


     indextype   FreeRegionCount; 
     numtype     FreeRegionDir;   
     numtype     FreeRegionLengths;

     handletype  SchemaHndl;

     grpheader **GrpHdrs;         
     grpheader  *VchunkGrpHdr;    

#ifndef __RISC_64__
     grpheader **GrpHdrs_64;
     grpheader  *VchunkGrpHdr_64;
#endif /* __RISC_64__ */

}                          dbheader;

extern numtype TestObj_Class;

extern boolean EnvFetchObjSize
                         (numtype       ObjNum,
                          ft F,lt Z,zz *Status,    bytessize    *SizeInBytes);

extern boolean Env_ReclaimHandles
			 (tokentype    *Token,     ft F,lt Z,zz *Status);

extern boolean Dbm_DeCacheDbs 
                         (ft F,lt Z,zz *Status);

typedef Part2  Part;

/*+-----------------------------------------------------------------------+*/

#ifdef __DISPLAY_CORE__
extern void    get_proc_mem_status( char* header );
extern void    Core_ShowStats (void);
#endif /* __DISPLAY_CORE__ */

extern tokentype        FirstRect;
/*+-----------------------------------------------------------------------+
~C                            BMT TEST                                    !
  +-----------------------------------------------------------------------+*/
boolean BMT_Test         (char         *EdbName, char         *EdbFileName,
                          char         *DrwName, char         *DrwFileName,
                          char         *EmpName, char         *EmpFileName,
                          int             Count, int           MaxOuterLoop,
                          int     MaxInnerLoops,
                          int           LookUps, int           Deletes,
                          int        StuffParts, int           TraverseLimit,
                          int        NewPartPct, int           LookUpPct,
                          int         DeletePct, int           StuffPct,
                          int       DeleteDraws, int           DeleteParts, 
                          boolean     CommitDbs, ft F,lt Z,zz *Status)
{
#ifdef __DISPLAY_TIME__
time_t         NullTime     = 0;
time_t         Elapsed      = 0;
float          Seconds      = 0;
time_t         InitStartTime  = 0;
time_t         OuterStartTime = 0;
time_t         InnerStartTime = 0;
time_t         InitEndTime    = 0;
time_t         OuterEndTime   = 0;
time_t         InnerEndTime   = 0;
#endif /* __DISPLAY_TIME__ */
longtype       Level        = 0;
boolean        Release      = False;
unsigned       Seed         = 1008;
numtype        NewParts     = 0;
numtype        RgnEntries   = 0;
numtype        RgnEntrys    = 0;
numtype        AllocRgns    = 0;
numtype        ExtendRgns   = 0;
numtype        AllocXs      = 0;
numtype        ExtendXs     = 0;
numtype        CacheLimit   = 0;
numtype        CacheLoad    = 0;
int            OuterLoop    = 0;
int            InnerLoop    = 0;
int            FreeCount    = 0;
tokentype      PartTkn      = NullToken;
partidtype     FromPart     = 0;
parttkntype    PartToken;
tokentype      ItorTkn      = NullToken;
boolean        CommitOnExit = False;
numtype        NumObjects   = 0;
numtype        StackPtr     = 0;
static tokentype WatchTkn   = {0, 6, 6};

#ifdef __DISPLAY_TIME__
  InitStartTime = clock();
#endif /* __DISPLAY_TIME__ */

  if (CommitDbs)
  {
     fprintf(stderr,"CommitDbs = True\n");
     fflush(stderr);
  } else
     DrawBug   = False;

  if (DeleteDraws       >= 1)
     DeleteDrawObjects   = True;
  else
     DeleteDrawObjects   = False;

  if (DeleteParts       >= 1)
     DeletePartObjects   = True;
  else
     DeletePartObjects   = False;

  Traverse_Limit = TraverseLimit;

  TraceMsg (0, "\n\n BMT TEST :: files...\n");
  sprintf (Msg, "      EdbName           := %s\n", EdbName);
  TraceMsg (0, Msg);
  sprintf (Msg, "      EdbFileName       := %s\n", EdbFileName);
  TraceMsg (0, Msg);
  sprintf (Msg, "      DrwName           := %s\n", DrwName);
  TraceMsg (0, Msg);
  sprintf (Msg, "      DrwFileName       := %s\n", DrwFileName);
  TraceMsg (0, Msg);
  sprintf (Msg, "      EmpName           := %s\n", EmpName);
  TraceMsg (0, Msg);
  sprintf (Msg, "      EmpFileName       := %s\n", EmpFileName);
  TraceMsg (0, Msg);
  sprintf (Msg, "\n      Swap to DiskCache := %s\n", BoolStr[SWAP]);
  TraceMsg (0, Msg);

  sprintf (Msg, "      Freeze the cache  := %s\n", BoolStr[FREEZE_GRP_PACKETS]);
  TraceMsg (0, Msg);

  TraceMsg (0, "\n\n BMT TEST :: parms...\n");
  sprintf (Msg, "      DeBug modulo      := %-8u\n", QueBug);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Create Parts count:= %-8u\n", Count);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Outer Loops       := %-8u\n", MaxOuterLoop);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Inner Loops       := %-8u\n", MaxInnerLoops);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Look Ups          := %-8u\n", LookUps);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Delete Parts      := %-8u\n", Deletes);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Stuff Parts       := %-8u\n", StuffParts);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Traverse Limit    := %-8u\n", Traverse_Limit);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Delete Draws      := %s\n", BoolStr[DeleteDrawObjects]);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Delete Parts      := %s\n", BoolStr[DeletePartObjects]);
  TraceMsg (0, Msg);
  sprintf (Msg, "      Delete ALL Parts  := after every <mod %2u>Outer Loop\n",
           DeleteParts);
  TraceMsg (0, Msg);

#ifdef __INCLUDE_RANDOM__
  Env_GenerateRandomNumbers  (Count * 3 * MaxOuterLoop * MaxInnerLoops * LookUps, McStat);
  sprintf (Msg, "\n      Random Numbers    := %lu\n", Random_Limit);
  TraceMsg (0, Msg);
#else
  srand( 1008 );
#endif /* __INCLUDE_RANDOM__ */

#ifdef __DISPLAY_CORE__
  get_proc_mem_status("INITIALIZE LIBRARY ::\n");
#endif /* __DISPLAY_CORE__ */

  TraceMsg (0, "\n INITIALIZE LIBRARY ::\n");
  if (Normal(*Status))
      BMT_Init          (EdbName, EdbFileName,
                         DrwName, DrwFileName,
                         EmpName, EmpFileName, Count,   McStat);

  Seed          = 1008;

  MaxParts      = OldParts;
  RgnEntries    = OldParts + (Count * MaxOuterLoop)
                + (MaxOuterLoop * (MaxInnerLoops * (LookUps + StuffParts)));

#ifdef     __INCLUDE_MTRX__
  if (RgnEntries > 65535)
  {
    fprintf(stderr,"ERROR: RgnEntries (%d) > 65535 : ", RgnEntries);
    fprintf(stderr,"%d + (%d * %d) + (%d * (%d * (%d + %d)))\n",
                    OldParts, Count, MaxOuterLoop, MaxOuterLoop,
                    MaxInnerLoops, LookUps, StuffParts);
    exit(1);
  }
#endif /* __INCLUDE_MTRX__ */

  sprintf (Msg, "      SEED          :=%8u; Swap     = %s; RgnEntries =%6u\n",
           Seed, BoolStr[SwapType], RgnEntries);
  TraceMsg (0, Msg);



  if (Normal(*Status))
  if (MaxParts == 0)
  {
     if (RgnEntries   < 20000)
         RgnEntries   =  20000;

     if ((RgnEntrys   = RgnEntries / 100)       <  100)
     RgnEntrys        = 100;

/*   .......................  HARD WIRED SIZE OF GRP REGIONS ............. */
     if ((AllocRgns   = RgnEntries / RgnEntrys) <  100)
     AllocRgns        = 100;


     ExtendRgns       = 100;

     AllocXs          = 1000;
     ExtendXs         = 1000;

     if ((CacheLimit  = AllocRgns  / 10)        <  7)
     CacheLimit       = 10;

     CacheLoad        = CacheLimit;

     BMT_ConfigAllClasses (RgnEntrys,   AllocRgns,   ExtendRgns,
                           AllocXs,     ExtendXs,    Read_Write, False,
                           CacheLimit,  CacheLoad,   McStat);
  }

#ifdef __DISPLAY_CORE__
  get_proc_mem_status("CORE Specs for INITIAL load ::\n");
  TraceMsg (MSGFILE, "\n CORE Specs for INITIAL load ::\n");
  Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */

#ifdef __DISPLAY_TIME__
  NullTime     = clock();
#endif /* __DISPLAY_TIME__ */

  PartTkn.DbId = LibTkn.DbId;

  NewParts     = Count;
  OuterLoop    = 0;

  if (Env_IsValidToken (&WatchTkn,  McStat))
     Object_Dump    (&WatchTkn);

  while (++OuterLoop  <= MaxOuterLoop
  &&     Normal(*Status))
  {
#ifdef __DISPLAY_TIME__
     OuterStartTime = clock();
#endif /* __DISPLAY_TIME__ */

  /* calculate a progressive decrease in New Parts                        */
     if (OuterLoop  > 1)
     {
        if (NewParts     > 1000)
        {
           NewParts       = (NewParts   * NewPartPct)  / 100;
           LookUps       += (LookUps    * LookUpPct)  / 100;
           Deletes       += (Deletes    * DeletePct) / 100;
           StuffParts    += (StuffParts * StuffPct) / 100;

        } else if (Count  != 1000)
          NewParts       = StuffParts;
     }

     sprintf (Msg, "\n OUTER LOOP [%3u] :  NewParts = %d LookUps = %d StuffParts = %d.\n", OuterLoop, NewParts, LookUps, StuffParts);
     TraceMsg (0, Msg);

     if (StuffParts
     ||  MaxParts   == 0)
     {
        MaxParts += Count;

#ifdef __DISPLAY_TIME__
        NullTime = clock();
#endif /* __DISPLAY_TIME__ */

        BMT_CreateParts    (NewParts,                McStat);

#ifdef __DISPLAY_CORE__
        get_proc_mem_status("CORE Specs after Outer Loop CreateParts ::\n");
        TraceMsg (MSGFILE, "\n CORE Specs after Outer Loop CreateParts::\n");
        Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */

        if (Normal(*Status))
           BMT_TraverseSets   (McStat);

        if (Normal(*Status))
        if (MaxInnerLoops  == 0
        &&  MaxOuterLoop     == 1)
        {
           CommitOnExit = True;
           sprintf (Msg, "\n COMMIT %6u NewParts and FREE all Image's.\n",
                    NewParts);
           TraceMsg (0, Msg);

#ifdef __DISPLAY_TIME__
           NullTime = clock();
#endif /* __DISPLAY_TIME__ */

           if (!Primal_CommitDb    (&LibTkn,     "\0",  True,     McStat))
              *Status = 1;

           if (Normal(*Status))
           if (!Primal_CommitDb    (&EmpTkn010,  "\0",  True,     McStat))
              *Status = 1;

           if (Normal(*Status))
           if (!Primal_CommitDb    (&LibTkn010,  "\0",  True,     McStat))
              *Status = 1;

#ifdef __DISPLAY_TIME__
           Elapsed  = clock() - NullTime;
           Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
           sprintf  (Msg,
                    "\n COMMIT Parts DB (release)... Time = %4.2f sec\n",
                     Seconds);
           TraceMsg (MSGFILE,   Msg);
#else
#endif /* __DISPLAY_TIME__ */

           sprintf  (Msg, "\n COMMIT Parts DB (release)... \n");
           TraceMsg (0,   Msg);

        } else if (StuffParts)
           BMT_CommitParts  (True,  McStat);

#ifdef __DISPLAY_CORE__
        get_proc_mem_status("CORE Specs after Outer Loop commit image's::\n");
        TraceMsg (MSGFILE, "\n CORE Specs after Outer Loop commit image's::\n");
        Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */
     }
     StartCommit  = CurrentId;

#ifdef __DISPLAY_TIME__
     InitEndTime = clock();
     Elapsed  = InitEndTime - InitStartTime;
     Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
     sprintf  (Msg,"\n INITIAL STARTUP RUN TIME = %4.2f sec\n", Seconds);
     TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__ */

/*   Do The Inner Test Loop  10 Times                                     */
     InnerLoop      = 0;

     if (Normal(*Status))
     if (MaxInnerLoops)
     if (TraceMsg (0, "\n BEGIN  Inner Loop Sequence::.\n"))

     while (Normal(*Status)
     &&   ++InnerLoop <= MaxInnerLoops)
     {
#ifdef __DISPLAY_TIME__
       InnerStartTime = clock();
#endif /* __DISPLAY_TIME__ */

       sprintf  (Msg, "\n INNER LOOP [%4u:%4u] :\n", OuterLoop, InnerLoop);
       TraceMsg (0, Msg);

       if (Normal(*Status))
       BMT_LookUpParts                (LookUps,   McStat);

       if (Normal(*Status))
       if (Deletes)
       {
#ifdef __DISPLAY_TIME__
          NullTime     = clock();
#endif /* __DISPLAY_TIME__ */

          BMT_DeleteParts   (Deletes,   McStat);

#ifdef __DISPLAY_TIME__
          Elapsed      = clock() - NullTime;
          Seconds      = (float )Elapsed / CLOCKS_PER_SEC;
          sprintf    (Msg, "\n Delete Parts Time = %4.2f sec, for Count=%6u\n",
                      Seconds, Deletes);
          TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */

          sprintf  (Msg, "\n Traverse Count=%6u\n", PartCount);
          TraceMsg (0, Msg);
       }

       PartCount    = 0;
       FromPart     = Random(NumVlists);
       FromPart     = Vlists[FromPart];

       sprintf (Msg,
                "\n TRAVERSE PartId[%6u] and all Connections to %2u Levels\n",
                FromPart, Traverse_Limit);
       TraceMsg (0,   Msg);

       Level        = 0;
#ifdef __DISPLAY_TIME__
       NullTime     = clock();
#endif /* __DISPLAY_TIME__ */

       if (Normal(*Status))
       if (Part_GetToken  (&LibTkn,               FromPart,
                           McStat,               &PartTkn))

          Part_Traverse   (&PartTkn,  Level,      McStat);

#ifdef __DISPLAY_TIME__
       Elapsed      = clock() - NullTime;
       Seconds      = (float )Elapsed / CLOCKS_PER_SEC;
#endif /* __DISPLAY_TIME__ */

       if (Normal(*Status))
       if (DrawBug)
          Object_Dump       (&PartTkn);

#ifdef __DISPLAY_TIME__
       sprintf  (Msg, "\n Traverse Time = %4.2f sec, for Count=%6u\n",
                 Seconds, PartCount);
       TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */

       sprintf  (Msg, "\n Traverse Count=%6u\n", PartCount);
       TraceMsg (0, Msg);

       sprintf  (Msg, "       Traverse    Asserts =%6lu. True Tests =%6lu\n",
                 LookUp_Asserts + LookUp_NullAsserts,  LookUp_Asserts);
       TraceMsg (0,   Msg);

       sprintf (Msg,  " <%6u> DrawObj         objects  DELETED.\n", DrawObjs);
       TraceMsg (0, Msg);
       sprintf (Msg,  "                 <%6u> are Named.\n", NamedObjs);
       TraceMsg (0, Msg);
       sprintf (Msg,  " <%6u> Point           objects  DELETED.\n", Points);
       TraceMsg (0, Msg);

       LookUp_Asserts      = 0;
       LookUp_NullAsserts  = 0;
       for (StackPtr       = 0;          /* valid only for BMT01 config    */
            StackPtr       < 2  && Normal(*Status); )
       {
          FromPart     = Random(NumVlists);
          FromPart     = Vlists[FromPart];
          if (Grp_GetEntry             (&LibTkn,   VpartsDir,  FromPart, 
                                        McStat,   (addrtype  )&PartToken))
          {
              PartTkn.Handle          = PartToken.Handle;
              ItorTkn.DbId            = LibTkn.DbId;
              ItorTkn.Handle          = PartToken.FromList;
              if (Vchunk_IsValidToken  (&ItorTkn,     McStat))
                  Vchunk_GetStackPtr   (&ItorTkn,     McStat, &StackPtr);
          }
       }

#ifdef __REVERSE_TRAVERSE__
       sprintf (Msg,
                "\n REVERSE Traverse FROM PartId[%6u] Chain to %2u Levels\n",
                FromPart, Traverse_Limit);
       TraceMsg (0,   Msg);

       PartCount    = 0;
       Level        = 0;
#ifdef __DISPLAY_TIME__
       NullTime     = clock();
#endif /* __DISPLAY_TIME__ */

       if (Normal(*Status))
          Part_Reverse    (&PartTkn,     Level,   McStat);

#ifdef __DISPLAY_TIME__
       Elapsed      = clock() - NullTime;
       Seconds      = (float )Elapsed / CLOCKS_PER_SEC;
#endif /* __DISPLAY_TIME__ */

       if (Normal(*Status))
       if (DrawBug)
         Object_Dump      (&PartTkn);

#ifdef __DISPLAY_TIME__
       sprintf  (Msg, "\n Reverse Time = %4.2f sec, for Count=%6u\n",
                 Seconds, PartCount);
       TraceMsg (MSGFILE, Msg);
#else
#endif /* __DISPLAY_TIME__ */

       sprintf  (Msg, "\n Reverse Count=%6u\n", PartCount);
       TraceMsg (0, Msg);
       sprintf  (Msg, "       Reverse     Asserts =%6lu. True Tests =%6lu\n",
                 LookUp_Asserts + LookUp_NullAsserts,  LookUp_Asserts);
       TraceMsg (0,   Msg);

       sprintf (Msg,  " <%6u> DrawObj         objects  Deleted.\n", DrawObjs);
       TraceMsg (0, Msg);
       sprintf (Msg,  "                 <%6u> are Named.\n", NamedObjs);
       TraceMsg (0, Msg);
       sprintf (Msg,  " <%6u> Point           objects  Deleted.\n", Points);
       TraceMsg (0, Msg);
#endif /* __REVERSE_TRAVERSE__ */

       LookUp_Asserts  = 0;
       LookUp_NullAsserts  = 0;

       if (StuffParts)
       {

          sprintf (Msg, "\n CREATE %u Additional Parts\n",
                   StuffParts);
          TraceMsg (0,   Msg);

          if (Normal(*Status))
             BMT_CreateParts  (StuffParts,  McStat);

          if (Normal(*Status))
          if (CommitDbs)
          {
             CommitOnExit = True;
             TraceMsg (0, "\n COMMIT Parts and FREE all Image's.\n");
             sprintf (Msg, "\n COMMIT %6u Parts to temp DB; Release=%s\n",
                      MaxParts - StartCommit, BoolStr[Release]);
             TraceMsg (0,   Msg);

#ifdef __DISPLAY_TIME__
             NullTime = clock();
#endif /* __DISPLAY_TIME__ */
             if (Normal(*Status))
             if (Object_CommitImage  (&LibTkn,    False,            McStat))
             if (!Primal_CommitDb    (&LibTkn,     "\0",  Release,  McStat))
                *Status = 1;

             if (Normal(*Status))
             if (Object_CommitImage  (&EmpTkn010, False,            McStat))
             if (!Primal_CommitDb    (&EmpTkn010,  "\0",  Release,  McStat))
                *Status = 1;

             if (Normal(*Status))
             if (Object_CommitImage  (&LibTkn010, False,            McStat))
             if (!Primal_CommitDb    (&LibTkn010,  "\0",  Release,  McStat))
                *Status = 1;

#ifdef __DISPLAY_TIME__
             Elapsed  = clock() - NullTime;
             Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
             sprintf  (Msg, "\n Commit Parts to DB... Time = %4.2f sec\n",
                       Seconds);
             TraceMsg (MSGFILE,   Msg);
#else
#endif /* __DISPLAY_TIME__ */

             sprintf  (Msg, "\n Commit Parts to DB...\n");
             TraceMsg (0,   Msg);

             Object_Dump    (&FirstRect);

          } else if (DrawBug)
             Object_Dump    (&FirstRect);

          if (Release)
          if (InnerLoop  <  MaxInnerLoops
          &&  OuterLoop  >  1)
          if (Normal(*Status))
          if (PartLib_ActivateDb (&LibTkn,        EdbFileName,
                                  McStat,        &PartLib01))

          if (PersonLib_ActivateDbByToken
                                 (&EmpTkn010,     EmpFileName, Read_Write,
                                  McStat,        &PersonLib01))

          if (DrawLib_ActivateDbByToken
                                 (&LibTkn010,     DrwFileName, Read_Write,
                                  McStat,        &DrawLib01))
             PartsTbl = PartLib01->PartsListHead.Varray;

       }

       if (Normal(*Status))
       if (!CommitDbs)
          BMT_CommitParts        (True,           McStat);

       if (Normal(*Status))
       if (Primal_GetClassObjectCount
                                 (&PrimalTkn,     TestObj_Class,
                                  McStat,        &NumObjects))
       if (NumObjects >= 10)
       {
          Release      = True;
          BMT_DeleteTestObjs  (McStat);
       }

#ifdef __DISPLAY_CORE__
       if (Normal(*Status))
       {
         sprintf (Msg, "\n CORE Specs After INNER Loop[%4u] BEFORE Dbm_DeCacheDbs::\n", InnerLoop);
         get_proc_mem_status( Msg );
         TraceMsg (MSGFILE, Msg);
         Core_ShowStats ();
       }
#endif /* __DISPLAY_CORE__ */

       /* DCH 01/24/95 */
       if ( Normal(*Status) )
       {
	  if ( Env_ReclaimHandles( &EmpTkn010, McStat ) )
	  if ( Env_ReclaimHandles( &LibTkn010, McStat ) )
	     Env_ReclaimHandles( &PrimalTkn, McStat ) ;
       }

#ifndef __DECACHE_DBS__ 
       if (Normal(*Status))
          Dbm_DeCacheDbs     (McStat);
#endif /* __DECACHE_DBS__  */

#ifdef __DISPLAY_CORE__
       if (Normal(*Status))
       {
         sprintf (Msg, "\n CORE Specs After INNER Loop[%4u] AFTER Dbm_DeCacheDbs::\n", InnerLoop);
         get_proc_mem_status( Msg );
         TraceMsg (MSGFILE, Msg);
         Core_ShowStats ();
       }
#endif /* __DISPLAY_CORE__ */

#ifdef __DISPLAY_TIME__
       InnerEndTime = clock();
       Elapsed  = InnerEndTime - InnerStartTime;
       Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
       sprintf  (Msg,"\n INNER LOOP RUN TIME = %4.2f sec\n", Seconds);
       TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__ */

       FreeCount           = 0;
       sprintf  (Msg, "\n END INNER LOOP [%4u:%4u].\n", OuterLoop, InnerLoop);
       TraceMsg (0, Msg);
     }

     /* END of INNER LOOP                                                 */
#ifndef __BMT01__
     if (Normal(*Status))
        BMT_TraverseSets   (McStat);
#endif

     if (*Status  == Env_CoreNoMore)
     {
         *Status   = Env_Normal;
          TraceMsg (0, "\n  ***WARNING*** CORE LIMIT EXCEEDED!!\n");
          TraceMsg (0, "  ...  NOTE ... Well attempt to recover!!\n");
     }

     if (Normal(*Status))
     {
        BMT_DeleteTestObjs  (McStat);
#ifdef __DISPLAY_CORE__
        sprintf (Msg, "\n CORE Specs after DELETE of ALL TestObjs ::\n");
        get_proc_mem_status( Msg );
        TraceMsg (MSGFILE, "\n CORE specs after DELETE of ALL TestObjs ::\n");
        Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */
     }

     Release               = False;
#ifdef __DISPLAY_TIME__
     NullTime              = clock();
#endif /* __DISPLAY_TIME__ */

     if (Normal(*Status))
     if (MaxInnerLoops)
     if (CommitDbs)
     if (TraceMsg (0, "\n FREE ALL databases...\n"))
     if (TraceMsg (0, "\n FREE the Part   DB...\n"))
     if (Primal_FreeDb     (&LibTkn,      McStat))
     if (TraceMsg (0, "\n FREE the Person DB...\n"))
     if (Primal_FreeDb     (&EmpTkn010,   McStat))
     if (TraceMsg (0, "\n FREE the Draw   DB...\n"))
     if (Primal_FreeDb     (&LibTkn010,   McStat))
     {
#ifdef __DISPLAY_TIME__
        Elapsed  = clock() - NullTime;
        Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
        sprintf  (Msg,
                "\n Free DB... Time = %4.2f sec\n",
                  Seconds);
        TraceMsg (MSGFILE,   Msg);
#else
#endif /* __DISPLAY_TIME__ */

        sprintf  (Msg, "\n Free DB...\n");
        TraceMsg (0,   Msg);
     }

     if (Normal(*Status))
     if (OuterLoop       != MaxOuterLoop)
     if (CommitDbs)
     {
        if (PartLib_ActivateDb (&LibTkn,        EdbFileName,
                                McStat,        &PartLib01))

        if (PersonLib_ActivateDbByToken
                               (&EmpTkn010,     EmpFileName, Read_Write,
                                McStat,        &PersonLib01))

        if (DrawLib_ActivateDbByToken
                               (&LibTkn010,     DrwFileName, Read_Write,
                                McStat,        &DrawLib01))
           PartsTbl = PartLib01->PartsListHead.Varray;

     }

     if (Normal(*Status))
     if (DeleteParts && OuterLoop % DeleteParts == 0)
     {
        BMT_DeleteAllObjects   (McStat);
#ifdef __DISPLAY_CORE__
        sprintf(Msg," Outer Loop %2d CORE after DELETE OBJECTS\n", OuterLoop );
        get_proc_mem_status( Msg );
        TraceMsg (MSGFILE, "\n CORE Specs after DELETE ALL OBJECTS!! ::\n");
        Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */
     }

#ifdef __DISPLAY_TIME__
       OuterEndTime = clock();
       Elapsed  = OuterEndTime - OuterStartTime;
       Seconds  = (float )Elapsed / CLOCKS_PER_SEC;
       sprintf  (Msg,"\n OUTER LOOP RUN TIME = %4.2f sec\n", Seconds);
       TraceMsg (MSGFILE, Msg);
#endif /* __DISPLAY_TIME__ */
  }

  PageBug = True;       /* DCH DEBUG */

#ifndef __BMT01__
  if (Normal(*Status))
  if (!CommitOnExit)
  {
     if (Primal_DeleteDb     (&LibTkn,         McStat))
     if (Primal_DeleteDb     (&EmpTkn010,      McStat))
         Primal_DeleteDb     (&LibTkn010,      McStat);
     if (*Status  == Err_NotImplemented)
         *Status   = Env_Normal;
  }
#endif /* __BMT01__   */

  PageBug = True;  /* DCH 12/22 */

  if (Normal(*Status))
  if (CommitOnExit)
     BMT_DeleteTestObjs      (McStat);

  if (!CommitOnExit)
  {
#ifdef __DISPLAY_CORE__
    get_proc_mem_status("CORE Specs after FINAL COMMIT\n") ;
    TraceMsg (MSGFILE, "\n CORE Specs after FINAL COMMIT ::\n");
    Core_ShowStats ();
#endif /* __DISPLAY_CORE__ */
    *Status  = Dbe_Quit;
  }

  LexBug = False;  /* DCH 12/22 */

TRACK(TrackBak,"BMT_Test\n");
return (STAT);
}


boolean BMT_Init         (char         *EdbName, char         *EdbFileName,
                          char         *DrwName, char         *DrwFileName,
                          char         *EmpName, char         *EmpFileName,
                          longtype      Count,   ft F,lt Z,zz *Status)
{
static numtype       HndlRgns           = 0;
static numtype       RgnHndls           = 0;
static numtype       VhndlRgns          = 0;
static numtype       RgnVhndls          = 0;
static numtype       XmemRgns           = 0;
static numtype       RgnXmems           = 0;
static boolean       UseVparts          = True;
grpheader           *GrpHdr             = NullPtr;
numtype              MaxPersons         = 131072;

  TraceMsg (0, "\n INITIALIZE SCHEMA ::\n");
  PartLib_Init         (McStat);

  if (Normal(*Status))
  {
     AllocParts   = Count  + 1001;

     if (AllocParts > 10000) {
         RgnHndls       = AllocParts + 1000;
         HndlRgns       = 10001;

         RgnVhndls      = 24003;
         VhndlRgns      = 10003;

         RgnXmems       = AllocParts + 5009;
         XmemRgns       = 10010;

     } else {
         RgnHndls       = 10401;
         HndlRgns       = 10001;

         RgnVhndls      = 15403;
         VhndlRgns      = 10003;

         RgnXmems       = 24010;
         XmemRgns       = 10010;

     }
  }
  RgnHndls              = AllocParts + 1000;
  RgnXmems              = AllocParts + 5009;
  RgnVhndls             = 1;
  VhndlRgns             = 1;

/* Establish the Primal Object Of a DataBase to hold the parts           */
  if (PartLib_Create  (EdbName,      EdbFileName,Read_Write,    UseVparts,
                       HndlRgns,     RgnHndls,   VhndlRgns,
                       RgnVhndls,    XmemRgns,   RgnXmems,
                       McStat,      &LibTkn,    &PartLib01))

/* Get the current Parts count                                           */
  if (Grp_GetPacket   (&LibTkn,                  VpartsDir,
                       McStat,                  &GrpHdr))
     OldParts  = GrpHdr->EntryCount;

  sprintf (Msg, "\n Part Count=%8u\n", OldParts);
  TraceMsg (0, Msg);

  if (OldParts > 0)
    OldParts--;
  else
    OldParts   = 0;

  MaxParts     = Count + OldParts;
  CurrentId    = OldParts;
  StartCommit  = OldParts;

  OneCent      = MaxParts / 128;
  if (OneCent == 0)
    OneCent = 2;

  HalfCent     = MaxParts / 256;
  if (HalfCent == 0)
    HalfCent = 2;

  if (Normal(*Status))
  if (DrawLib_InitDb   (DrwName,  DrwFileName,  Count,  McStat, &LibTkn010))
      List01_FindListHead
                       (&LibTkn010, "DbRectangles",    McStat, &RectListTkn);
/* Create List for Rectangles                                              */
  if (*Status  == List01_NotFound)
  {
    *Status      = Env_Normal;
     List01_Create  (&LibTkn010, &NullToken,    "DbRectangles",  0, 0,
                     McStat,     &RectListTkn);
  }

  if (Normal(*Status))
      Person_InitDb     (EmpName,     EmpFileName,            Count,
                         McStat,                             &EmpTkn010);


  if (Normal(*Status))
  if (Primal_GetClassObjectCount
                        (&EmpTkn010,  Person_Class, McStat,  &NumPersons))
  if (NumPersons == 0)
  {
 /* ......  CREATE Option1 Person's                             .......... */

     BMT_BuildPersonLib (&EmpTkn010,  MaxPersons,   McStat);
  }

  if (Normal(*Status))
     BMT_InitQuerys     (McStat);

TRACK(TrackBak,"BMT_Init\n");
return (STAT);
}

boolean BMT_ConfigAllClasses
                         (numtype       RgnEntrys,
                          numtype       AllocRgns, numtype       ExtendRgns,
                          numtype       AllocXs,   numtype       ExtendXs,
                          accesstype    AccessType,boolean       DiskCache,
                          numtype       CacheLimit,numtype       CacheLoad,
                          ft F,lt Z,zz *Status)
{
numtype      MediumSize  = 4096;
numtype      LargeSize   = 8192;
bytessize    ObjSize     = 0;
numtype      AllocEntrys = RgnEntrys * 100;

     if (EnvFetchObjSize (Part_Class,          McStat,     &ObjSize))
     {
         RgnEntrys      = MediumSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = CacheLoad;
         if (CacheLimit > 10)
         CacheLimit     = 10;
         if (CacheLoad  > 10)
         CacheLoad      = 10;
         Primal_ConfigYourClass
                         (&LibTkn,     Part_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (Rectangle_Class,      McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         if (CacheLimit > 10)
         CacheLimit     = 10;
         if (CacheLoad  > 10)
         CacheLoad      = 10;

         Primal_ConfigYourClass
                         (&LibTkn010,  Rectangle_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (XyRect_Class,         McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  XyRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (NamedXyRect_Class,    McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  NamedXyRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (ArrayRect_Class,      McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  ArrayRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (DblPtrRect_Class,     McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  DblPtrRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (VarrayRect_Class,     McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  VarrayRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (IntChunkRect_Class,       McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  IntChunkRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (VchunkRect_Class,         McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  VchunkRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);

     }

     if (Normal(*Status))
     if (EnvFetchObjSize (RefRect_Class,            McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  RefRect_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (CartesianPoint_Class,     McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;
         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         Primal_ConfigYourClass
                         (&LibTkn010,  CartesianPoint_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }

     if (Normal(*Status))
     if (EnvFetchObjSize (TestObj_Class,            McStat,     &ObjSize))
     {
         RgnEntrys      = LargeSize   / ObjSize;
         AllocRgns      = AllocEntrys / RgnEntrys;

         CacheLoad      = AllocRgns * 6 / 10;
         CacheLimit     = AllocRgns;
         if (CacheLimit > 10)
         CacheLimit     = 10;
         if (CacheLoad  > 10)
         CacheLoad      = 10;
         Primal_ConfigYourClass
                         (&PrimalTkn,  TestObj_Class,
                          RgnEntrys,   AllocRgns,   ExtendRgns,
                          AllocXs,     ExtendXs,    Read_Write, True,
                          CacheLimit,  CacheLoad,   McStat);
     }
  LibBug = False;

TRACK(TrackBak,"BMT_ConfigAllClasses\n");
return (STAT);
}
/*+-----------------------------------------------------------------------+
~M                         END OF BMT.C                                   !
  +-----------------------------------------------------------------------+*/

