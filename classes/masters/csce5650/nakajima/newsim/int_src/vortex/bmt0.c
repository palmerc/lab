/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* ******************************  BMT0.C   *****************************  */
/* *  Main program for the Bench Mark Test                                 */
/* *                               Recipies in the Developers CookBook     */
/* *  Executable entry to the BMT (BenchMark Test)                         */
/* *                                                                       */
/* *********************************************************************** */

#define  __DISPLAY_TIME__

/* BMT0.c */
#define  MAIN
#define  BMT0
#include <ctype.h>
#ifdef __DISPLAY_TIME__
#include <time.h>
#endif /* __DISPLAY_TIME__ */
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "files.h"
#include "core01.h"
#include "domain.h"
#include "env0.h"
#include "primal.h"
#include "bmt01.h"

/*+-----------------------------------------------------------------------+*/

/*+-----------------------------------------------------------------------+*/
#define null 0

extern numtype      Global_Apl;

extern  char        WenvFileName[];
extern  char        PrimalFileName[];

/*+-----------------------------------------------------------------------+
                         RUN SPECIFIC
  +-----------------------------------------------------------------------+*/

extern boolean BMT_Test  (char         *EdbName, char         *EdbFileName,
                          char         *DrwName, char         *DrwFileName,
                          char         *EmpName, char         *EmpFileName,
                          int           Count,   int           Loop,
                          int           Samples, 
                          int           LookUps, int           Deletes,
                          int           Stuffs,  int           TraverseLimit,
                          int         NewPartPct,int           LookUpPct, 
                          int          DeletePct,int           StuffPct,
                          int        DeleteDraws,int           DeleteParts,
                          boolean      CommitDbs,ft F,lt Z,zz *Status);

       longtype  Ctest_Count    = 0;

/*+-----------------------------------------------------------------------+
~C                            MAIN                                        !
  +-----------------------------------------------------------------------+*/

#define  PARMS_EXPECTED  30

main                    (int           argc,     char          *argv[])
{
int          argcount           = 0;
FILE        *arglistfile;
char         buffer[MAXSTRING];
char         ArgName[MAXPATHNAME];
statustype   Status             = 0;
boolean      Large              = False;

int          PartCount          = 1000;
int          OuterLoop          = 1;
int          InnerLoop          = 2;
int          LookUps            = 1000;
int          Deletes            = 0;
int          StuffParts         = 100;
int          TraverseLimit      = 7;
int          DeleteDraws        = 0;
int          DeleteParts        = 0;

int          NewPartPct         = 0;
int          LookUpPct          = 0;
int          DeletePct          = 0;
int          StuffPct           = 0;
boolean      CommitDbs          = False;

#ifdef __DISPLAY_TIME__
time_t       NullTime           = 0;
time_t       CurrTime           = 0;
double       Elapsed            = 0.0;
#endif /* __DISPLAY_TIME__ */

static char  Parms[MAXSTRING *  2];
numtype      ParmLen                       = 0;
static char  Edb_DbName[MAXPATHNAME]       = "PartLib";
static char  Edb_DbFileName[MAXPATHNAME]   = "Part.db";

static char  Drw_DbName[MAXPATHNAME]       = "DrawLib";
static char  Drw_DbFileName[MAXPATHNAME]   = "Draw.db";

static char  Emp_DbName[MAXPATHNAME]       = "PersonLib";
static char  Emp_DbFileName[MAXPATHNAME]   = "Persom.db";

static char  Dat_File[MAXPATHNAME]         = "";
static char  Msg_File[MAXPATHNAME]         = "";
static char  Out_File[MAXPATHNAME]         = "";
static char  Mem_File[MAXPATHNAME]         = "";

farlongtype  Core_PageSize      = 64000L;
indextype    Alloc_FreeAddrs    = 2806;
indextype    Extend_FreeAddrs   = 1806;

indextype    Alloc_Chunks       = 10402;
indextype    Extend_Chunks      = 10002;

indextype    Alloc_XmemBlks     = 1004;
indextype    Extend_XmemBlks    = 1004;

indextype    Alloc_DbBlks       = 1004;
indextype    Extend_DbBlks      = 1004;

indextype    Alloc_Handles      = 1004;
indextype    Extend_Handles     = 1004;

indextype    Alloc_Vchunks      = 10402;
indextype    Extend_Vchunks     = 10002;

indextype    Alloc_Grps         = 100;
indextype    Extend_Grps        = 20;

sizetype     BoolSize           = sizeof (boolean);


/*+-----------------------------------------------------------------------+*/
   FileBug        = False;
   Global_Apl     = Cpp_Apl;

   Large          = True;

   Align_Bytes    = False;
   Align_CppStruc = False;

   Print_Msg      = True;
   WriteHdr       = True;

   strcpy  (Msg_File,     "stdout");
   MsgFilePtr     = stdout;

/* no argS                                                                 */
   if(--argc     == 0)
   {
/* no args; Display Syntax description                                    */
   TraceMsg (0, "\n\n  VORTEX Help File::\n");
   TraceMsg (0, "\n  Create an input file with the following arguments\n");
   TraceMsg (0, "  listed one per line.  Then use the input file name\n");
   TraceMsg (0, "  as the argument to the program when invoked!\n");
   TraceMsg (0, "  arg[ 1]... MESSAGE        FileName\n");
   TraceMsg (0, "  arg[ 2]... OUTPUT         FileName\n");
   TraceMsg (0, "  arg[ 3]... DISK CACHE     FileName\n");
   TraceMsg (0, "  arg[ 4]... RENV           FileName\n");
   TraceMsg (0, "  arg[ 5]... WENV           FileName\n");
   TraceMsg (0, "  arg[ 6]... PRIMAL         FileName\n");
   TraceMsg (0, "  arg[ 7]... PART DB        FileName\n");
   TraceMsg (0, "  arg[ 8]... DRAW DB        FileName\n");
   TraceMsg (0, "  arg[ 9]... PERSON DB      FileName\n");
   TraceMsg (0, "  arg[10]... PERSONS Input  FileName\n");
   TraceMsg (0, "  arg[11]... # of PARTS            to Create.\n");
   TraceMsg (0, "  arg[12]... # of OUTER     Loops\n");
   TraceMsg (0, "  arg[13]... # of INNER     Loops \n");
   TraceMsg (0, "  arg[14]... # of LOOKUP    Parts         per inner loop\n");
   TraceMsg (0, "  arg[15]... # of DELETE    Parts         per inner loop\n");
   TraceMsg (0, "  arg[16]... # of STUFF     Parts  to add per inner loop\n");
   TraceMsg (0, "  arg[17]... % to DECREASE  new    Parts created.\n");
   TraceMsg (0, "  arg[18]... % to INCREASE  Lookup Parts per inner loop.\n");
   TraceMsg (0, "  arg[19]... % to INCREASE  Delete Parts per inner loop.\n");
   TraceMsg (0, "  arg[20]... % to INCREASE  Stuff  Parts per inner loop.\n");
   TraceMsg (0, "  arg[21]... DEPTH          of Level in Traverse/Reverse\n");
   TraceMsg (0, "  arg[22]... FREEZE_GRP_PACKETS    always true.\n");
   TraceMsg (0, "  arg[23]... ALLOC_CHUNKS   low value will flush data cache\n");
   TraceMsg (0, "  arg[24]... EXTEND_CHUNKS  on each extension\n");
   TraceMsg (0, "  arg[25]... DELETE Draw    objects during Traverse/Reverse\n");
   TraceMsg (0, "  arg[26]... DELETE Part    objects at end of i'th Outer Loop\n");
   TraceMsg (0, "  arg[27]... QueBug         modulo  for traceing process\n");
   TraceMsg (0, "  arg[28]... Void Boundary  MAX Core allocatable.\n");
   TraceMsg (0, "  arg[29]... Void Reserve   RESERVE Core allocatable.\n");
   TraceMsg (0, "  arg[30]... Commit Dbs     Commit Dbs during run.\n");
   return (0);
   }
   else
   {
      strcpy  (ArgName,  *++argv);
      arglistfile  = fopen (ArgName,  "r");

      if  (!arglistfile)
      {
         fprintf(stderr,"ERROR on fopen() of VORTEX input parameter file %s\n",
                 ArgName);
         exit (1);
      }

      while (fgets (buffer,  MAXSTRING,  arglistfile))
      {
         switch (argcount)
         {
           case 0:
               sscanf (buffer, "%*s %s", Msg_File);
               break;
           case 1:
               sscanf (buffer, "%*s %s", Out_File);
               break;
           case 2:
               sscanf (buffer, "%*s %s", Mem_File);
               break;
           case 3:
               sscanf (buffer, "%*s %s", DbFileName);
               break;
           case 4:
               sscanf (buffer, "%*s %s", WenvFileName);
               break;
           case 5:
               sscanf (buffer, "%*s %s", PrimalFileName);
               break;
           case 6:
               sscanf (buffer, "%*s %s", Edb_DbFileName);
               break;
           case 7:
               sscanf (buffer, "%*s %s", Drw_DbFileName);
               break;
           case 8:
               sscanf (buffer, "%*s %s", Emp_DbFileName);
               break;
           case 9:
               sscanf (buffer, "%*s %s", Dat_File);
               break;
           case 10:
               sscanf (buffer, "%*s %d", &PartCount);
               break;
           case 11:
               sscanf (buffer, "%*s %d", &OuterLoop);
               break;
           case 12:
               sscanf (buffer, "%*s %d", &InnerLoop);
               break;
           case 13:
               sscanf (buffer, "%*s %d", &LookUps);
               break;
           case 14:
               sscanf (buffer, "%*s %d", &Deletes);
               break;
           case 15:
               sscanf (buffer, "%*s %d", &StuffParts);
               break;
           case 16:
               sscanf (buffer, "%*s %d", &NewPartPct);
               break;
           case 17:
               sscanf (buffer, "%*s %d", &LookUpPct);
               break;
           case 18:
               sscanf (buffer, "%*s %d", &DeletePct);
               break;
           case 19:
               sscanf (buffer, "%*s %d", &StuffPct);
               break;
           case 20:
               sscanf (buffer, "%*s %d", &TraverseLimit);
               break;
           case 21:
               sscanf (buffer, "%*s %d", &FREEZE_GRP_PACKETS);
               break;
           case 22:
               sscanf (buffer, "%*s %d", &Alloc_Chunks);
               break;
           case 23:
               sscanf (buffer, "%*s %d", &Extend_Chunks);
               break;
           case 24:
               sscanf (buffer, "%*s %d", &DeleteDraws);
               break;
           case 25:
               sscanf (buffer, "%*s %d", &DeleteParts);
               break;
           case 26:
               sscanf (buffer, "%*s %d", &QueBug);
               break;
           case 27:
               sscanf (buffer, "%*s %d", &VOID_BOUNDARY);
               break;
           case 28:
               sscanf (buffer, "%*s %d", &VOID_RESERVE);
               break;
           case 29:
               sscanf (buffer, "%*s %d", &CommitDbs);
               break;
           default:
               break;
         }
         argcount++;
      }

      if (FREEZE_GRP_PACKETS == True)
         strcpy (Mem_File, "NULL");

      if  (argcount < PARMS_EXPECTED - 3)
      {
         fprintf (stderr,"ERROR in VORTEX input parameter file %s: ",
                  ArgName);
         fprintf (stderr,"%d of the expected %d arguments were found.\n",
                  argcount,  PARMS_EXPECTED);
         exit (1);
      }
      else if  (argcount > PARMS_EXPECTED)
      {
         fprintf(stderr,"ERROR in VORTEX input parameter file %s: ",
               ArgName);
         fprintf(stderr,"%d arguments exceeds the %d required parameters.\n",
               argcount,  PARMS_EXPECTED);
         exit (1);
      }
      else
      {
#ifdef __BMT01__
      sprintf (Parms, "\nVORTEX INPUT PARAMETERS::\n");
      strcat  (Parms, Msg );
      sprintf (Msg, "\tMESSAGE       FileName:\t %-21s\n", Msg_File);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tOUTPUT        FileName:\t %-21s\n", Out_File);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tDISK CACHE    FileName:\t %-21s\n", Mem_File);
      strcat  (Parms, Msg );
      /* DCH - 01/13/95 - rem'd for purpose of diffs to standard out files ----
      sprintf (Msg, "\tRENV          FileName:\t %-21s\n", DbFileName);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tWENV          FileName:\t %-21s\n", WenvFileName);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tPRIMAL        FileName:\t %-21s\n", PrimalFileName);
      strcat  (Parms, Msg );
      -- DCH - 01/13/95 - rem'd for purpose of diffs to standard out files - */
      sprintf (Msg, "\tPART DB       FileName:\t %-21s\n", Edb_DbFileName);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tDRAW DB       FileName:\t %-21s\n", Drw_DbFileName);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tPERSON DB     FileName:\t %-21s\n", Emp_DbFileName);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tPERSONS Data  FileName:\t %-21s\n", Dat_File);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tPARTS         Count   :\t %-8d\n",  PartCount);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tOUTER         Loops   :\t %-8d\n",  OuterLoop);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tINNER         Loops   :\t %-8d\n",  InnerLoop);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tLOOKUP        Parts   :\t %-8d\n",  LookUps);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tDELETE        Parts   :\t %-8d\n",  Deletes);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tSTUFF         Parts   :\t %-8d\n",  StuffParts);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tDEPTH         Traverse:\t %-8d\n",  TraverseLimit);
      strcat  (Parms, Msg );
      sprintf (Msg, "\t%% DECREASE    Parts   :\t %-8d\n",  NewPartPct);
      strcat  (Parms, Msg );
      sprintf (Msg, "\t%% INCREASE    LookUps :\t %-8d\n",  LookUpPct);
      strcat  (Parms, Msg );
      sprintf (Msg, "\t%% INCREASE    Deletes :\t %-8d\n",  DeletePct);
      strcat  (Parms, Msg );
      sprintf (Msg, "\t%% INCREASE    Stuffs  :\t %-8d\n",  StuffPct);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tFREEZE_PACKETS        :\t %-8d\n",  FREEZE_GRP_PACKETS);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tALLOC_CHUNKS          :\t %-8d\n",  Alloc_Chunks);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tEXTEND_CHUNKS         :\t %-8d\n",  Extend_Chunks);
      strcat  (Parms, Msg );
      sprintf (Msg, "\tDELETE Draw objects   :\t %-21s\n",
                                                        BoolStr[DeleteDraws]);
      strcat  (Parms, Msg );
      if (DeleteParts > 1)
      sprintf (Msg, "\tDELETE Part objects   :\t %-21s\n", BoolStr[True]);
      else
      sprintf (Msg, "\tDELETE Part objects   :\t %-21s\n", BoolStr[DeleteParts]);
      strcat  (Parms, Msg );

      sprintf (Msg, "\tQUE_BUG               :\t %d\n", QueBug);
      strcat  (Parms, Msg );

      sprintf (Msg, "\tVOID_BOUNDARY         :\t %d\n", VOID_BOUNDARY);
      strcat  (Parms, Msg );

      sprintf (Msg, "\tVOID_RESERVE          :\t %d\n\n", VOID_RESERVE);
      strcat  (Parms, Msg );

      sprintf (Msg, "\tCOMMIT_DBS            :\t %s\n\n", BoolStr[CommitDbs]);
      strcat  (Parms, Msg );

#else
      fprintf (stdout, "\nVORTEX INPUT PARAMETERS::\n");
      fprintf (stdout, "\tMESSAGE       FileName:\t %-21s\n", Msg_File);
      fprintf (stdout, "\tOUTPUT        FileName:\t %-21s\n", Out_File);
      fprintf (stdout, "\tDISK CACHE    FileName:\t %-21s\n", Mem_File);
      fprintf (stdout, "\tRENV          FileName:\t %-21s\n", DbFileName);
      fprintf (stdout, "\tWENV          FileName:\t %-21s\n", WenvFileName);
      fprintf (stdout, "\tPRIMAL        FileName:\t %-21s\n", PrimalFileName);
      fprintf (stdout, "\tPART DB       FileName:\t %-21s\n", Edb_DbFileName);
      fprintf (stdout, "\tDRAW DB       FileName:\t %-21s\n", Drw_DbFileName);
      fprintf (stdout, "\tPERSON DB     FileName:\t %-21s\n", Emp_DbFileName);
      fprintf (stdout, "\tPERSONS Data  FileName:\t %-21s\n", Dat_File);
      fprintf (stdout, "\tPARTS         Count   :\t %-8d\n",  PartCount);
      fprintf (stdout, "\tOUTER         Loops   :\t %-8d\n",  OuterLoop);
      fprintf (stdout, "\tINNER         Loops   :\t %-8d\n",  InnerLoop);
      fprintf (stdout, "\tLOOKUP        Parts   :\t %-8d\n",  LookUps);
      fprintf (stdout, "\tDELETE        Parts   :\t %-8d\n",  Deletes);
      fprintf (stdout, "\tSTUFF         Parts   :\t %-8d\n",  StuffParts);
      fprintf (stdout, "\t% DECREASE    Parts   :\t %-8d\n",  NewPartPct);
      fprintf (stdout, "\t% INCREASE    LookUps :\t %-8d\n",  LookUpPct);
      fprintf (stdout, "\t% INCREASE    Deletes :\t %-8d\n",  DeletePct);
      fprintf (stdout, "\t% INCREASE    Stuffs  :\t %-8d\n",  StuffPct);
      fprintf (stdout, "\tDEPTH         Traverse:\t %-8d\n",  TraverseLimit);
      fprintf (stdout, "\tFREEZE_PACKETS        :\t %-8d\n",  FREEZE_GRP_PACKETS);
      fprintf (stdout, "\tALLOC_CHUNKS          :\t %-8d\n",  Alloc_Chunks);
      fprintf (stdout, "\tEXTEND_CHUNKS         :\t %-8d\n",  Extend_Chunks);
      fprintf (stdout, "\tDELETE Draw objects   :\t %-21s\n",
                                                        BoolStr[DeleteDraws]);
      if (DeleteParts > 1)
      fprintf (stdout, "\tDELETE Part objects   :\t %s\n", BoolStr[True]);
      else
      fprintf (stdout, "\tDELETE Part objects   :\t %s\n", BoolStr[DeleteParts]);

      fprintf (stdout, "\tQUE_BUG               :\t %d\n", QueBug);

      fprintf (stdout, "\tVOID_BOUNDARY         :\t %d\n", VOID_BOUNDARY);
      fprintf (stdout, "\tVOID_RESERVE          :\t %d\n\n", VOID_RESERVE);

      fprintf (stdout, "\tCOMMIT DBS            :\t %s\n\n",BoolStr[CommitDbs]);

      fflush  (stdout);
#endif /* __BMT01__ */

      if ((ParmLen = strlen (Parms)) > MAXSTRING * 2)
      {
      fprintf (stdout, "\nPARAMETER STRING EXCEEDED (*%6u) !!!\n", MAXSTRING);
      fprintf (stdout, "          STRING Size   :=  %6u\n", ParmLen);
      }

      }
   }

   WriteHdr       = False;
   Print_Msg      = True;

   Core_PageSize  = 65536 ; /* 320000L; */

/* ENTER THE DOMAIN OF THE REALM                                           */
   if (BoolSize != 2 || Large)
   {

      Alloc_DbBlks          = 1500;
      Extend_DbBlks         = 1005;

      Alloc_Handles         = 1000;
      Extend_Handles        = 1000;

      Alloc_Vchunks         = 3001;
      Extend_Vchunks        = 1001;

      Alloc_XmemBlks        = 1004;
      Extend_XmemBlks       = 1004;

      Alloc_FreeAddrs       = 2806;
      Extend_FreeAddrs      = 1806;

   } else {
      Alloc_DbBlks          = 1000;
      Extend_DbBlks         = 1005;

      Alloc_Handles         = 1000;
      Extend_Handles        = 1000;

      Alloc_Vchunks         = 2000;
      Extend_Vchunks        = 1002;

      Alloc_XmemBlks        = 99;
      Extend_XmemBlks       = 50;

      Alloc_FreeAddrs       = 84;
      Extend_FreeAddrs      = 100;

   }

   WriteHdr       = False;
   Print_Msg      = True;

   Reload_Env     = True;
   Install_Env    = False;

   Ctest_Count    = PartCount;

#ifdef __DISPLAY_TIME__
   time (&NullTime);
#endif /* __DISPLAY_TIME__ */

   if (Domain_Omi0Invoke (Dat_File,        Msg_File,     
                          Out_File,        Mem_File,
                          Core_PageSize,   Core_PageSize/2,
                          Alloc_FreeAddrs, Extend_FreeAddrs,
                          Alloc_Chunks,    Extend_Chunks, 
                          Alloc_XmemBlks,  Extend_XmemBlks,
                          Alloc_Handles,   Extend_Handles,  
                          Alloc_Vchunks,   Extend_Vchunks, 
                          Alloc_Grps,      Extend_Grps, 
                          Alloc_DbBlks,    Extend_DbBlks,
                          Parms,           McStatPtr))
   {
/*    Access the KingDom from  Command Line Mode                           */
      BMT_Test           (Edb_DbName,      Edb_DbFileName,
                          Drw_DbName,      Drw_DbFileName,
                          Emp_DbName,      Emp_DbFileName,
                          PartCount,       OuterLoop,      InnerLoop,
                          LookUps,         Deletes,        StuffParts,     
                          TraverseLimit,   NewPartPct,     LookUpPct,
                          DeletePct,       StuffPct,
                          DeleteDraws,     DeleteParts,    CommitDbs,
                          McStatPtr);
   }

#ifdef __DISPLAY_TIME__
   Elapsed    = difftime (time(&CurrTime), NullTime);
   sprintf  (Msg, "\n Run Time = %.0f sec\n", Elapsed);
   TraceMsg (MSGFILE,   Msg);
#endif /* __DISPLAY_TIME__ */

   ClassBug = False;

/* EXIT THE DOMAIN OF THE REALM                                            */
   Domain_Exit           (McStatPtr);

   TraceMsg (0,"\n");
   StackTrack ("Main");

#if 0
/* runspec does not like non-zero return codes */
   exit(1);
#endif

return(0);
}

/*+-----------------------------------------------------------------------+
~M                         END OF BMT0.C                                  !
  +-----------------------------------------------------------------------+*/
