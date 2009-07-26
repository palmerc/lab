/* @(#) File name: cmmu_debug.c   Ver: 3.3   Cntl date: 3/10/89 10:30:17 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

extern int	cmdcount;
extern char	*cmdptrs[];


/*************************************************************************/
/* cmmu cache display                                                    */
/**                                                                      */
/**                                                                      */
/*  cd                                                                   */
/**            parameters: 1) choice of cmmu to display                  */
/**                        2) set number                                 */
/**            default: Data cmmu cache set #0 is displayed              */
/*************************************************************************/
int cd(void)		/* cmmu cache display */
{
    struct cmmu *cmmu_ptr;
    int     setnum, i;
    char    choice, *errptr;
    static int  save_set;

    if (cmdcount == 3)
    {
	if ((strcmp ("I", cmdptrs[1]) == 0) || (strcmp ("i", cmdptrs[1]) == 0))
	{
	    cmmu_ptr = &Icmmu;
	    choice = 'I';
	}
	else
	{
	    cmmu_ptr = &Dcmmu;
	    choice = 'D';
	}

	if(getexpr(cmdptrs[2], &errptr, (unsigned int *)&setnum) != -1)
	{
	    save_set = setnum;
	    if (save_set <= 255)
	    {
		PPrintf (" %c CACHE (SET # %d) :\n", choice, save_set);
		PPrintf (" vv  kill  address tag  data0      data1      data2      data3      order\n");
		PPrintf (" --- ----  -----------  ---------- ---------- ---------- ---------- -----\n");
		for (i = 0; i < LPS; i++)
		{
			if(choice == 'I'){
		    PPrintf ("  %d   %1d    0x%08x   0x%08x 0x%08x 0x%08x 0x%08x   %d \n",
			    cmmu_ptr -> cache.cache_entry[save_set][i].vv,
			    cmmu_ptr -> cache.cache_entry[save_set][i].kill,
			    cmmu_ptr -> cache.cache_entry[save_set][i].tag,
			    cmmu_ptr -> cache.cache_entry[save_set][i].data[0],
			    cmmu_ptr -> cache.cache_entry[save_set][i].data[1],
			    cmmu_ptr -> cache.cache_entry[save_set][i].data[2],
			    cmmu_ptr -> cache.cache_entry[save_set][i].data[3],
			    cmmu_ptr -> cache.cache_entry[save_set][i].order);
			}else{
				unsigned int swap[4];
			    intswap(swap,(unsigned int *)cmmu_ptr->cache.cache_entry[save_set][i].data);
			    intswap(swap+1,(unsigned int *)cmmu_ptr->cache.cache_entry[save_set][i].data+1);
			    intswap(swap+2,(unsigned int *)cmmu_ptr->cache.cache_entry[save_set][i].data+2);
			    intswap(swap+3,(unsigned int *)cmmu_ptr->cache.cache_entry[save_set][i].data+3);

		    PPrintf ("  %d   %1d    0x%08x   0x%08x 0x%08x 0x%08x 0x%08x   %d \n",
			    cmmu_ptr -> cache.cache_entry[save_set][i].vv,
			    cmmu_ptr -> cache.cache_entry[save_set][i].kill,
			    cmmu_ptr -> cache.cache_entry[save_set][i].tag,
			    swap[0],
			    swap[1],
			    swap[2],
			    swap[3],
			    cmmu_ptr -> cache.cache_entry[save_set][i].order);
			}
		}
	    }
	}
	else
	{
	    PPrintf ("undefined symbol: %s\n", cmdptrs[2]);
	}
    }
    else
	PPrintf ("Usage: cd {I/D} {set number} \n");

	return 0;
}



/*************************************************************************/
/* cmmu statistics                                                       */
/**                                                                      */
/**                                                                      */
/*  cs                                                                   */
/*          statistics displayed:                                        */
/*          =====================                                        */
/*                 cmmu function statistics:                             */
/*                 cmmu ATC      statistics:                             */
/*                 cache statistics:                                     */
/*                   -  Instruction cache Hit/Miss Ratio                 */
/*                   -  Data cache Hit/Miss Ratio                        */
/*                      *  Load data Hit/Miss Ratio                      */
/*                      *  Store data Hit/Miss Ratio                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

int cs(void)		/* cmmu statistics */
{
    char  ch;
    float   IFhitrat = 0.0;	/* Instruction fetches hit ratio */
    float   IFmissrat = 0.0;	/* Instruction fetches miss ratio */
    float   DLhitrat = 0.0;	/* Data loads hit ratio */
    float   DLmissrat = 0.0;	/* Data loads miss ratio */
    float   DShitrat = 0.0;	/* Data stores hit ratio */
    float   DSmissrat = 0.0;	/* Data stores miss ratio */

	float   I_BATC_HR = 0.0;
	float   I_BATC_MR = 0.0;
	float   D_BATC_HR = 0.0;
	float   D_BATC_MR = 0.0;

	float   I_PATC_HR = 0.0;
	float   I_PATC_MR = 0.0;
	float   D_PATC_HR = 0.0;
	float   D_PATC_MR = 0.0;

     /* display cmmu function usage */

	 printf(" \ncmmu function statistics: \n");
	 printf("                         -----------------\n");
	 printf("                         | Icmmu | Dcmmu |\n");
	 printf(" ----------------------------------------|\n");
	 printf("|tablewalks              | %4d  | %4d  |\n",Icmmu.stats.num_tblwks,Dcmmu.stats.num_tblwks);
	 printf("-----------------------------------------|\n");
	 printf("|tablewalks w/U,M bit set| %4d  | %4d  |\n",Icmmu.stats.num_tblwks_UM,Dcmmu.stats.num_tblwks_UM);
	 printf("-----------------------------------------|\n");
	 printf("|write once              | %4d  | %4d  |\n",Icmmu.stats.num_wchwr1,Dcmmu.stats.num_wchwr1);
	 printf("-----------------------------------------|\n");
	 printf("|cache writes w/ copyback| %4d  | %4d  |\n",Icmmu.stats.num_wcb,Dcmmu.stats.num_wcb);
	 printf("-----------------------------------------|\n");
	 printf("|cache reads w/ copyback | %4d  | %4d  |\n",Icmmu.stats.num_rcb,Dcmmu.stats.num_rcb);
	 printf("-----------------------------------------|\n");
	 printf("|probes                  | %4d  | %4d  |\n",Icmmu.stats.num_probes,Dcmmu.stats.num_probes);
	 printf("-----------------------------------------|\n");

   PPrintf ("\n CONTINUE  -  PRESS  <return> \n");
   ch = getchar ();


	/* calculate ATC statistics */

if (Icmmu.stats.num_BATC_chk != 0)
	{
	I_BATC_HR = 100.00 * ((float) Icmmu.stats.num_BATC_hits / Icmmu.stats.num_BATC_chk); /* Icmmu BATC hit ratio */ 
	I_BATC_MR = 100.00 - I_BATC_HR;
	}
if (Dcmmu.stats.num_BATC_chk != 0)
	{
	D_BATC_HR = 100.00 * ((float) Dcmmu.stats.num_BATC_hits / Dcmmu.stats.num_BATC_chk); /* Dcmmu BATC hit ratio */
	D_BATC_MR = 100.00 - D_BATC_HR;
	}
if (Icmmu.stats.num_PATC_chk != 0)
	{
	I_PATC_HR = 100.00 * ((float) Icmmu.stats.num_PATC_hits / Icmmu.stats.num_PATC_chk); /* Icmmu PATC hit ratio */ 
	I_PATC_MR = 100.00 - I_PATC_HR;
	}
if (Dcmmu.stats.num_PATC_chk != 0)
	{
	D_PATC_HR = 100.00 * ((float) Dcmmu.stats.num_PATC_hits / Dcmmu.stats.num_PATC_chk); /* Dcmmu PATC hit ratio */
	D_PATC_MR = 100.00 - D_PATC_HR;
	}

	/* print out ATC statistics */

    PPrintf ("                 ----------------------------------------------\n");
    PPrintf ("                 | # of      | # of     | Hit      | Miss     |\n");
    PPrintf ("                 | checks    | hits     | Ratio    | Ratio    |\n");
    PPrintf ("---------------------------------------------------------------\n");
    PPrintf ("| BATC - ICMMU   |%9d  |%9d |  %6.2f  |  %6.2f  |\n",Icmmu.stats.num_BATC_chk,Icmmu.stats.num_BATC_hits,I_BATC_HR,I_BATC_MR);
    PPrintf ("|----------------|-----------|----------|----------|----------|\n");
    PPrintf ("| PATC - ICMMU   |%9d  |%9d |  %6.2f  |  %6.2f  |\n", Icmmu.stats.num_PATC_chk,Icmmu.stats.num_PATC_hits, I_PATC_HR, I_PATC_MR);
    PPrintf ("|----------------|-----------|----------|----------|----------|\n");
    PPrintf ("| BATC - DCMMU   |%9d  |%9d |  %6.2f  |  %6.2f  |\n", Dcmmu.stats.num_BATC_chk,Dcmmu.stats.num_BATC_hits, D_BATC_HR, D_BATC_MR);
    PPrintf ("|----------------|-----------|----------|----------|----------|\n");
    PPrintf ("| PATC - DCMMU   |%9d  |%9d |  %6.2f  |  %6.2f  |\n", Dcmmu.stats.num_PATC_chk,Dcmmu.stats.num_PATC_hits, D_PATC_HR, D_PATC_MR);
    PPrintf ("---------------------------------------------------------------\n");

	/* gather data cache statistics */

    /* calculate hit ratios */

    if (ICcnt != 0)
    {
	IFhitrat = 100.00 * ((float) IChit / ICcnt);/* Instruction fetches hit ratio */
	IFmissrat = 100.00 - IFhitrat;
    }
    if (LDCcnt != 0)
    {
	DLhitrat = 100.00 * ((float) LDChit / LDCcnt);/* Data loads hit ratio */
	DLmissrat = 100.00 - DLhitrat;
    }
    if (SDCcnt != 0)
    {
	DShitrat = 100.00 * ((float) SDChit / SDCcnt);/* Data stores hit ratio */
	DSmissrat = 100.00 - DShitrat;
    }

    PPrintf ("                 ----------------------------------------------\n");
    PPrintf ("                 | # of      | # of     | Hit      | Miss     |\n");
    PPrintf ("                 | accesses  | hits     | Ratio    | Ratio    |\n");
    PPrintf ("---------------------------------------------------------------\n");
    PPrintf ("| Instr. fetches |%9d  |%9d |  %6.2f  |  %6.2f  |\n", ICcnt, IChit, IFhitrat, IFmissrat);
    PPrintf ("|----------------|-----------|----------|----------|----------|\n");
    PPrintf ("| Data loads     |%9d  |%9d |  %6.2f  |  %6.2f  |\n", LDCcnt, LDChit, DLhitrat, DLmissrat);
    PPrintf ("|----------------|-----------|----------|----------|----------|\n");
    PPrintf ("| Data stores    |%9d  |%9d |  %6.2f  |  %6.2f  |\n", SDCcnt, SDChit, DShitrat, DSmissrat);
    PPrintf ("---------------------------------------------------------------\n");
	
	return 0;
}




/*************************************************************************/
/* Page Address Translation Cache display                                */
/**                                                                      */
/**                                                                      */
/*  pd                                                                   */
/*                                                                       */
/**            parameters: 1) choice of cmmu to display                  */
/**            default: Data cmmu PATC is displayed                      */
/*                                                                       */
/*************************************************************************/
int pd(void)		/* page address translation cache display */
{
	struct cmmu	*cmmu_ptr;
	char		choice, ch;
	int		i, Index;

	if (cmdcount == 2)
	{
		if ((strcmp ("I", cmdptrs[1]) == 0) || (strcmp ("i", cmdptrs[1]) == 0))
		{
			cmmu_ptr = &Icmmu;
			choice = 'I';
		}
		else
		{
			cmmu_ptr = &Dcmmu;
			choice = 'D';
		}
	}
	else if (cmdcount == 1)
	{
		cmmu_ptr = &Dcmmu;
		choice = 'D';
	}

	else
	{
		PPrintf ("Usage: pd {I/D} \n");
		return (0);
	}

	Index = 0;
	while (Index <= (PATC_SIZE - 1))
	{

		if (choice == 'I')
			PPrintf ("\n             ICMMU     PATC   \n");
		else
			PPrintf ("\n             DCMMU     PATC   \n");

		PPrintf ("-------------------------------------------------------------------\n");
		PPrintf ("| entry |      LPA     |      PFA     | S | T | G | C | M | W | V |\n");
		PPrintf ("-------------------------------------------------------------------\n");

		for (i = 0; ((i <= 15) && (Index <= PATC_SIZE -1)) ; Index++, i++)
			PPrintf ("  %2d         %06x          %05x      %1d   %1d   %1d   %1d   %1d   %1d   %1d \n", Index,
			        cmmu_ptr -> patc.patc_entry[Index].LPA,
			        cmmu_ptr -> patc.patc_entry[Index].PFA,
			        cmmu_ptr -> patc.patc_entry[Index].S,
			        cmmu_ptr -> patc.patc_entry[Index].WT,
			        cmmu_ptr -> patc.patc_entry[Index].G,
			        cmmu_ptr -> patc.patc_entry[Index].CI,
			        cmmu_ptr -> patc.patc_entry[Index].M,
			        cmmu_ptr -> patc.patc_entry[Index].WP,
			        cmmu_ptr -> patc.patc_entry[Index].V);

		if (Index < 53)		/* don't need after 3rd listing */
		{
			PPrintf ("\n CONTINUE  -  PRESS  <return> \n");
			ch = getchar ();
		}
	}
	
	return 0;
}


/*************************************************************************/
/* Block Address Translation Cache display                               */
/**                                                                      */
/**                                                                      */
/*  bd                                                                   */
/*                                                                       */
/**            parameters: 1) choice of cmmu to display                  */
/**            default: Data cmmu BATC is displayed                      */
/*                                                                       */
/*************************************************************************/
int bd (void)		/* block address translation cache display */
{
    struct cmmu *cmmu_ptr;
    char    choice;
    int     Index;

    if (cmdcount == 2)
    {
	if ((strcmp ("I", cmdptrs[1]) == 0) || (strcmp ("i", cmdptrs[1]) == 0))
	{
	    cmmu_ptr = &Icmmu;
	    choice = 'I';
	}
	else
	{
	    cmmu_ptr = &Dcmmu;
	    choice = 'D';
	}
    }
    else if (cmdcount == 1)
    {
	cmmu_ptr = &Dcmmu;
	choice = 'D';
    }

    else
    {
	PPrintf ("Usage: bd {I/D} \n");
	return (0);
    }


    if (choice == 'I')
	PPrintf ("\n             ICMMU     BATC   \n");
    else
	PPrintf ("\n             DCMMU     BATC   \n");
    PPrintf ("-----------------------------------------------------------\n");
    PPrintf ("| entry |      LBA     |      PBA     | T | G | C | W | V |\n");
    PPrintf ("-----------------------------------------------------------\n");
    for (Index = 0; Index < BATC_SIZE; Index++)
	PPrintf ("  %2d          %04x           %04x       %1d   %1d   %1d   %1d   %1d \n", Index,
		cmmu_ptr -> batc.batc_entry[Index].LBA,
		cmmu_ptr -> batc.batc_entry[Index].PBA,
		cmmu_ptr -> batc.batc_entry[Index].T,
		cmmu_ptr -> batc.batc_entry[Index].G,
		cmmu_ptr -> batc.batc_entry[Index].C,
		cmmu_ptr -> batc.batc_entry[Index].W,
		cmmu_ptr -> batc.batc_entry[Index].V);
	
	return 0;
}




/*************************************************************************/
/* cmmu control register display                                         */
/**                                                                      */
/**                                                                      */
/*  cr                                                                   */
/*                                                                       */
/**          parameters: 1) choice of cmmu control registers to display  */
/**             default: Data cmmu control registers are displayed       */
/*                                                                       */
/*************************************************************************/

/*************************************************************************
*	Modified:
*	The function no inter accepts arguments.  When invoked, it
*	displays the control register contents of both CMMU's
**************************************************************************/

int cr(void )		/* cmmu control register display */
{
	PPrintf ("     ICMMU        Regs        DCMMU");
	PPrintf ("       CMMU Control Registers \n");
	PPrintf ("|--------------------------------------|\n");
	PPrintf ("|  0x%08x  |   IDR  |  0x%08x  |  ID register \n",
Icmmu.control.IDR, Dcmmu.control.IDR);
	PPrintf ("|  0x%08x  |   SCR  |  0x%08x  |  Command register \n",
Icmmu.control.SCMR, Dcmmu.control.SCMR);
	PPrintf ("|  0x%08x  |   SSR  |  0x%08x  |  Status register \n",
Icmmu.control.SSR, Dcmmu.control.SSR);
	PPrintf ("|  0x%08x  |  SADR  |  0x%08x  |  Address register \n",
Icmmu.control.SADR, Dcmmu.control.SADR);
	PPrintf ("|  0x%08x  |  SCTR  |  0x%08x  |  Control register \n",
Icmmu.control.SCTR, Dcmmu.control.SCTR);
	PPrintf ("|  0x%08x  |  PFSR  |  0x%08x  |  P Bus Fault Status register\n",
Icmmu.control.LSR, Dcmmu.control.LSR);
	PPrintf ("|  0x%08x  |  PFAR  |  0x%08x  |  P Bus Fault Address register\n",
Icmmu.control.LADR, Dcmmu.control.LADR);
	PPrintf ("|  0x%08x  |  SAPR  |  0x%08x  |  Supervisor Area Pointer register \n",
Icmmu.control.SAPR, Dcmmu.control.SAPR);
	PPrintf ("|  0x%08x  |  UAPR  |  0x%08x  |  User Area Pointer register \n",
Icmmu.control.UAPR, Dcmmu.control.UAPR);
	PPrintf ("|  0x%08x  |  BWP0  |  0x%08x  |  Block Write Port 0 \n",
Icmmu.control.BWP[0], Dcmmu.control.BWP[0]);
	PPrintf ("|  0x%08x  |  BWP1  |  0x%08x  |  Block Write Port 1 \n",
Icmmu.control.BWP[1], Dcmmu.control.BWP[1]);
	PPrintf ("|  0x%08x  |  BWP2  |  0x%08x  |  Block Write Port 2 \n",
Icmmu.control.BWP[2], Dcmmu.control.BWP[2]);
	PPrintf ("|  0x%08x  |  BWP3  |  0x%08x  |  Block Write Port 3 \n",
Icmmu.control.BWP[3], Dcmmu.control.BWP[3]);
	PPrintf ("|  0x%08x  |  BWP4  |  0x%08x  |  Block Write Port 4 \n",
Icmmu.control.BWP[4], Dcmmu.control.BWP[4]);
	PPrintf ("|  0x%08x  |  BWP5  |  0x%08x  |  Block Write Port 5 \n",
Icmmu.control.BWP[5], Dcmmu.control.BWP[5]);
	PPrintf ("|  0x%08x  |  BWP6  |  0x%08x  |  Block Write Port 6 \n",
Icmmu.control.BWP[6], Dcmmu.control.BWP[6]);
	PPrintf ("|  0x%08x  |  BWP7  |  0x%08x  |  Block Write Port 7 \n",
Icmmu.control.BWP[7], Dcmmu.control.BWP[7]);
	PPrintf ("|  0x%08x  |  CSSP  |  0x%08x  |  Cache Set Status Port \n",
Icmmu.control.CSSP, Dcmmu.control.CSSP);
	PPrintf ("|--------------------------------------|\n");

	
	return 0;
}
