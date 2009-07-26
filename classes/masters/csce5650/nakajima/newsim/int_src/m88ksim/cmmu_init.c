/* @(#) File name: cmmu_init.c   Ver: 3.1   Cntl date: 1/20/89 14:17:08 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

extern int ICcnt, DCcnt, IChit, DChit, LDCcnt, SDCcnt, LDChit, SDChit;
extern int cache_inhibit;
extern int global;
extern int prev_extime;
extern unsigned int    Dcmmu_devnum, Icmmu_devnum;


void cmmu_init(void)
{
    int     i;
    struct cmmu *cmmu_ptr;


    PPrintf ("  DCMMU & ICMMU RESET   \n");

	/* set up cmmu types */

	Dcmmu.IorD = M_DATA;
	Icmmu.IorD = M_INSTR;

    /* set both cmmu's logical extension bit (s/u) for Supervisor mode */
    Dcmmu.S_U = 1;
    Icmmu.S_U = 1;

    /* set UAPR and SAPR cache inhibit bit (c=1) and clear the valid bit (invalidate) (v=0) on both cmmu's */
    Dcmmu.control.UAPR = CI_BMASK;
    Dcmmu.control.SAPR = CI_BMASK;

    Icmmu.control.UAPR = CI_BMASK;
    Icmmu.control.SAPR = CI_BMASK;

    /* disable caches */

    cache_inhibit = 1;

    /* initialize for global coherency protocal state changes */

    global = 1;

    /* initialize other cmmu control registers */
    Dcmmu.control.IDR = 0x00A00000;	/* DCMMU: ID=0, TYPE=101B @ Reset */
    Dcmmu_devnum = 0;			/* set global variable for dev. num */
    Dcmmu.control.SSR = G_BMASK;	/* set status register global bit */
    Dcmmu.control.SCMR = 0x0;		/* Command register */
    Dcmmu.control.SCTR = 0x0;		/* Control register */
    Dcmmu.control.SADR = 0x0;
    Dcmmu.control.LSR = 0x0;
    Dcmmu.control.LADR = 0x0;
    for (i = 0; i <= 7; i++)
	Dcmmu.control.BWP[i] = 0x0;

    Icmmu.control.IDR = 0x01A00000;	/* ICMMU: ID=1, TYPE=101B @ reset */
    Icmmu_devnum = 1;			/* set global variable for dev. num */
    Icmmu.control.SSR = G_BMASK;	/* set status register global bit */
    Icmmu.control.SCMR = 0x0;		/* Command register */
    Icmmu.control.SCTR = 0x0;		/* Control register */
    Icmmu.control.SADR = 0x0;
    Icmmu.control.LSR = 0x0;
    Icmmu.control.LADR = 0x0;
    for (i = 0; i <= 7; i++)
	Icmmu.control.BWP[i] = 0x0;


    /* initialize BATC's */
    for (i = 0; i < BATC_SIZE; i++)/* invalidate batc entries */
    {
	Dcmmu.batc.batc_entry[i].V = 0;
	Icmmu.batc.batc_entry[i].V = 0;
    }

    /* initialize two "hardwired" batc entries */
    cmmu_ptr = &Dcmmu;
    init_batc_HDWRD (cmmu_ptr, 8, 9);

    /* initialize PATC's */
    for (i = 0; i < PATC_SIZE; i++)/* invalidate patc entries */
    {
	Dcmmu.patc.patc_entry[i].V = 0;
	Icmmu.patc.patc_entry[i].V = 0;
    }

    /* reset both caches */

    cmmu_ptr = &Dcmmu;
    cache_reset (cmmu_ptr);
    cmmu_ptr = &Icmmu;
    cache_reset (cmmu_ptr);

    /* initialize all cache statistic variables */
    ICcnt = 0L;
    DCcnt = 0L;
    IChit = 0L;
    DChit = 0L;
    LDCcnt = 0L;
    SDCcnt = 0L;
    LDChit = 0L;
    SDChit = 0L;

	Icmmu.stats.num_wchwr1 = 0;  
	Icmmu.stats.num_wcb = 0;   
	Icmmu.stats.num_rcb = 0;  
	Icmmu.stats.num_tblwks = 0;    
	Icmmu.stats.num_tblwks_UM = 0;
	Icmmu.stats.num_BATC_chk = 0; 
	Icmmu.stats.num_BATC_hits = 0;
	Icmmu.stats.num_PATC_chk = 0;   
	Icmmu.stats.num_PATC_hits = 0;

	Dcmmu.stats.num_wchwr1 = 0;  
	Dcmmu.stats.num_wcb = 0;   
	Dcmmu.stats.num_rcb = 0;  
	Dcmmu.stats.num_tblwks = 0;    
	Dcmmu.stats.num_tblwks_UM = 0;
	Dcmmu.stats.num_BATC_chk = 0; 
	Dcmmu.stats.num_BATC_hits = 0;
	Dcmmu.stats.num_PATC_chk = 0;   
	Dcmmu.stats.num_PATC_hits = 0;

    /* initialize previous execution time variable to zero */
    prev_extime = 0;

}


void cache_reset(struct cmmu *cmmu_ptr)
{
    int     i, j;

    for (i = 0; i < SETS; i++)
	for (j = 0; j < LPS; j++)
	{
	    cmmu_ptr -> cache.cache_entry[i][j].vv = INV;/* invalidate data */
	    /* cmmu_ptr->cache.cache_entry[i][j].tag = 0L;     clear address of line */
	    cmmu_ptr -> cache.cache_entry[i][j].order = j;/* force a startup order          */
	    cmmu_ptr -> cache.cache_entry[i][j].kill = 0;/* clear kill status */
	}
    for (i = 0; i < SETS; i++)
		cmmu_ptr->cache.cache_accessed[i] = 0; /* all cache sets have not been*/
											   /* accessed at initialization */
}




void write_memory(int word,unsigned int address)
{
    rdwr ((cmmu_ptr->IorD | M_WR), address, &word, 4);
}



void init_batc_HDWRD(struct cmmu *cmmu_ptr,int index1,int index2)
{

    cmmu_ptr -> batc.batc_entry[index1].LBA = 0x3ffe;
    cmmu_ptr -> batc.batc_entry[index1].PBA = 0x1ffe;
    cmmu_ptr -> batc.batc_entry[index1].T = 1;
    cmmu_ptr -> batc.batc_entry[index1].G = 0;
    cmmu_ptr -> batc.batc_entry[index1].C = 1;
    cmmu_ptr -> batc.batc_entry[index1].W = 0;
    cmmu_ptr -> batc.batc_entry[index1].V = 1;

    cmmu_ptr -> batc.batc_entry[index2].LBA = 0x3fff;
    cmmu_ptr -> batc.batc_entry[index2].PBA = 0x1fff;
    cmmu_ptr -> batc.batc_entry[index2].T = 1;
    cmmu_ptr -> batc.batc_entry[index2].G = 0;
    cmmu_ptr -> batc.batc_entry[index2].C = 1;
    cmmu_ptr -> batc.batc_entry[index2].W = 0;
    cmmu_ptr -> batc.batc_entry[index2].V = 1;

}
