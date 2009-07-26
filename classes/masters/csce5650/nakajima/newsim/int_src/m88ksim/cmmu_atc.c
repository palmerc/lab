/* @(#) File name: cmmu_atc.c   Ver: 3.1   Cntl date: 1/20/89 14:17:18 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

extern int IChit, DChit;
extern int  debugflag;
extern int  cache_inhibit;
extern int  write_through;
extern int  global;
extern int  cmmutime;


/**********************************************************************
*	check_BATC
*/
int check_BATC(struct cmmu *cmmu_ptr, unsigned int batc_log_entry)
{
	int     i;

	for (i = 0; i < BATC_SIZE; i++)
	{
		if ((cmmu_ptr -> batc.batc_entry[i].LBA == batc_log_entry) &&
		    (cmmu_ptr -> batc.batc_entry[i].V))				/* and it's a valid entry */
			return (1);
	}
	return (0);
}



/**********************************************************************
*	check_PATC
*/
int check_PATC(struct cmmu *cmmu_ptr, unsigned int patc_log_entry)
{
	int     i;

	for (i = 0; i < PATC_SIZE; i++)
	{
		if ((cmmu_ptr -> patc.patc_entry[i].LPA == patc_log_entry) &&
		    (cmmu_ptr -> patc.patc_entry[i].V))				/* and it's a valid entry */
			return (1);
	}
	return (0);
}



/**********************************************************************
*	get_from_BATC
*/
int get_from_BATC (struct cmmu *cmmu_ptr,unsigned int *entry,
unsigned int batc_log_entry,int writeflag,int probeflag)
{
	int     Index;

	/* find index */
	Index = 0;
	while (cmmu_ptr -> batc.batc_entry[Index].LBA != batc_log_entry)
		Index++;

	/* if write access, check write protect bit */
	if ((writeflag) && (cmmu_ptr -> batc.batc_entry[Index].W))
	{
		PPrintf ("error: (BATC) write protect violation <<trapping>>\n");
		cmmu_ptr->control.LSR = WRITE_VIOLATION;
/*
		cmmu_ptr->control.SSR &= EXCEPT_CLR;
		cmmu_ptr->control.SSR |= WR_PROTECT_VIO;
*/
		return (-1);
	}

	/* check 'c' bit and set/clear global 'cache inhibit' variable */
	if (cmmu_ptr -> batc.batc_entry[Index].C)
	{
		cmmutime += TIME_XBATC;
		cache_inhibit = 1;
		if(probeflag)
			cmmu_ptr->control.SSR |= CI_BMASK;
	}
	else
		cache_inhibit = 0;

	/* check 'T' bit and set/clear global 'write through' variable */
	if (cmmu_ptr -> batc.batc_entry[Index].T)
	{
		write_through = 1;
		if(probeflag)
			cmmu_ptr->control.SSR |= WT_BMASK;
	}
	else
		write_through = 0;

	/* check 'G' bit and set/clear 'global' variable */
	if (cmmu_ptr -> batc.batc_entry[Index].G)
	{
		global = 1;
		if (probeflag)
			cmmu_ptr->control.SSR |= G_BMASK;
	}
	else
		global = 0;

	if ((probeflag) && (cmmu_ptr->batc.batc_entry[Index].W))
		cmmu_ptr->control.SSR |= WP_BMASK;

	if (probeflag)
		cmmu_ptr->control.SSR |= V_BMASK;

	*entry = (cmmu_ptr -> batc.batc_entry[Index].PBA << 19);
	return (0);
}



/**********************************************************************
*	get_from_PATC
*/
int get_from_PATC(struct cmmu *cmmu_ptr,unsigned int *entry,
unsigned int patc_log_entry,int writeflag,unsigned int log_addr,
int probeflag)
{
	int		Index;
	unsigned int   return_addr;		/* outgoing physical logical address */

	/* find index */
	Index = 0;
	while ((cmmu_ptr -> patc.patc_entry[Index].LPA != patc_log_entry) || !(cmmu_ptr -> patc.patc_entry[Index].V))
		Index++;

	if (probeflag)
		cmmu_ptr->control.SSR |=  U_BMASK;

	if ((probeflag) && (cmmu_ptr->patc.patc_entry[Index].WP))
		cmmu_ptr->control.SSR |= WP_BMASK;

	if ((probeflag) && (cmmu_ptr->patc.patc_entry[Index].S))
		cmmu_ptr->control.SSR |= SP_BMASK;

	/* if write access, check write protect bit */
	if ((writeflag) && (cmmu_ptr -> patc.patc_entry[Index].WP))
	{
		PPrintf ("error: (PATC) write protect violation <<trapping>>\n");
		cmmu_ptr->control.LSR = WRITE_VIOLATION;
/*
		cmmu_ptr->control.SSR &= EXCEPT_CLR;
		cmmu_ptr->control.SSR |= WR_PROTECT_VIO;
*/
		return (-1);
	}

	if ((probeflag) && (cmmu_ptr->patc.patc_entry[Index].M))
		cmmu_ptr->control.SSR |= M_BMASK;

	/* if the access is a write and the PATC 'Modified' bit is clear that bit */
	/* is set and a tablewalk is initiated for the sole purpose of setting the */
	/* 'Modified' bit in the translation table */

	if ((writeflag) && !(cmmu_ptr -> patc.patc_entry[Index].M))
	{
		cmmutime += TIME_TBLWK;
		if (tablewalk (cmmu_ptr, &return_addr, log_addr, patc_log_entry, writeflag, 1,0))
			return (-1);
	}

	/* check 'c' bit and set/clear global 'cache inhibit' variable */
	if (debugflag)
		PPrintf ("patc_entry[].CI = %d \n", cmmu_ptr -> patc.patc_entry[Index].CI);
	if (cmmu_ptr -> patc.patc_entry[Index].CI)
	{
		cmmutime += TIME_XPATC;
		cache_inhibit = 1;
		if (probeflag)
			cmmu_ptr->control.SSR |= CI_BMASK;
	}
	else
		cache_inhibit = 0;

	/* check 'T' bit and set/clear global 'write through' variable */
	if (cmmu_ptr -> patc.patc_entry[Index].WT)
	{
		write_through = 1;
		if (probeflag)
			cmmu_ptr->control.SSR |= WT_BMASK;
	}
	else
		write_through = 0;

	/* check 'G' bit and set/clear 'global' variable */
	if (cmmu_ptr -> patc.patc_entry[Index].G)
	{
		global = 1;
		if (probeflag)
			cmmu_ptr->control.SSR |= G_BMASK;
	}
	else
		global = 0;


	/* if this is a store or write access then set 'Modified' bit */

	if (writeflag)
		cmmu_ptr -> patc.patc_entry[Index].M = 1;

	if(probeflag)
		cmmu_ptr->control.SSR |= V_BMASK;

	*entry = cmmu_ptr -> patc.patc_entry[Index].PFA;
	return (0);
}



/**********************************************************************
*	update_PATC
*/
int update_PATC(struct cmmu *cmmu_ptr,unsigned int patc_log_entry,
unsigned int phy_address,unsigned int page_desc,unsigned int seg_desc,
unsigned int area_desc,int writeflag)
{
	int		Index, i;
	unsigned int	patc_phy_entry;

	/* get next entry to be replaced based on FIFO */

	Index = 0;
	while (cmmu_ptr -> patc.patc_entry[Index].fifo_cnt != 0)
		Index++;

	if (debugflag)
		PPrintf (" Index(patc entry to be replaced) = %d \n", Index);

	/* replace entry */

	cmmu_ptr -> patc.patc_entry[Index].LPA = patc_log_entry;
	cmmu_ptr -> patc.patc_entry[Index].fifo_cnt = PATC_SIZE - 1;
	patc_phy_entry = ((phy_address & 0xfffff000) >> 12);
	cmmu_ptr -> patc.patc_entry[Index].PFA = patc_phy_entry;

	/* validate patc entry */
	cmmu_ptr -> patc.patc_entry[Index].V = 1;

	/* fill in 'G' bit in patc entry */
	cmmu_ptr->patc.patc_entry[Index].G =
		(((area_desc & G_BMASK) | (seg_desc & G_BMASK) | (page_desc & G_BMASK)) >> 7) & 0x1;

	/* fill in 'W' bit in patc entry */
	cmmu_ptr -> patc.patc_entry[Index].WP = (((seg_desc & WP_BMASK) | (page_desc & WP_BMASK)) >> 2) & 0x1;

	/* fill in 'C' bit in patc entry */
	cmmu_ptr->patc.patc_entry[Index].CI =
		(((area_desc & CI_BMASK) | (seg_desc & CI_BMASK) | (page_desc & CI_BMASK)) >> 6) & 0x1;

	/* fill in 'T' bit in patc entry */
	cmmu_ptr -> patc.patc_entry[Index].WT =
		(((area_desc & WT_BMASK) | (seg_desc & WT_BMASK) | (page_desc & WT_BMASK)) >> 9) & 0x1;

	/* fill in 'S' bit in patc entry */
	cmmu_ptr -> patc.patc_entry[Index].S = (((seg_desc & SP_BMASK) | (page_desc & SP_BMASK)) >> 8) & 0x1;

	/* if this is a store or write access then set 'Modified' bit */
	if ((writeflag) || (page_desc & M_BMASK))
		cmmu_ptr -> patc.patc_entry[Index].M = 1;

	/* change fifo count of all other patc entries except index just replaced */
	/* and of course those not yet touched.                                  */

	for (i = 0; i < PATC_SIZE; i++)
		if ((i != Index) && (cmmu_ptr -> patc.patc_entry[i].fifo_cnt != 0))
			cmmu_ptr -> patc.patc_entry[i].fifo_cnt--;

	return 0;
}
