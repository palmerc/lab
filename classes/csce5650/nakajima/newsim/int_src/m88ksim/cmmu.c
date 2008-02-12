/* @(#) File name: cmmu.c   Ver: 3.1   Cntl date: 1/20/89 14:17:21 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

	/******************************
	* Local Definitions
	*/
#define BWPSBITMASK 0x00000020
#define LRUMASK     0x3f000000

	/******************************
	* External Declarations
	*/
extern int IChit, DChit;
extern int  debugflag;
extern int cmmutime;
extern int probetype;

	/******************************
	* Global Definitions
	*/
int     cache_inhibit;
int     write_through = 0;
int	global;


/***********************************************************************
*	load_data
*/
int load_data(struct cmmu *cmmu_ptr, unsigned int *data,
unsigned int *log_addr_ptr,int writeflag,int probeflag)
{
	unsigned int   phy_address;/* physical address */
	unsigned int   batc_log_entry, batc_phy_entry, batc_offset;
	unsigned int   patc_log_entry, patc_phy_entry, patc_offset;
	unsigned int   APR, inv_centry;

				/* supervisor/user bit set to 'supervisor' and it is a read from cmmu control space */
	if ((cmmu_ptr -> S_U) && ((*log_addr_ptr & CTRL_SPACE) == CTRL_SPACE))
	{
		if (writeflag)
			return(0);
		else
		{
			cmmutime += TIME_RCTL;
					/* if this is a probe simply install the info that would
					 * have been installed if the hardwired BATC entries were
					 * being used for the control memory accesses.
					 */
			if ((probeflag) && (cmmu_ptr->control.SAPR & V_BMASK))
			{
				cmmu_ptr->control.SADR = *log_addr_ptr;
				cmmu_ptr->control.SSR |= BH_BMASK;
				cmmu_ptr->control.SSR |= WT_BMASK;
				cmmu_ptr->control.SSR |= CI_BMASK;
				cmmu_ptr->control.SSR |= V_BMASK;
				return(0);
			}
			else
				return (ctrl_space_read (*log_addr_ptr, data));
		}
	}
	else
	{
		if (probeflag)
			if (probetype)				/* SUPERVISOR */
				APR = cmmu_ptr->control.SAPR;
			else					/* USER */
				APR = cmmu_ptr->control.UAPR;
		else
			if (cmmu_ptr -> S_U)			/* set (temporary) Area Pointer */
				APR = cmmu_ptr -> control.SAPR;
			else
				APR = cmmu_ptr -> control.UAPR;


		if (APR & V_BMASK)
		{						/* address translation mode */

			batc_log_entry = ((*log_addr_ptr & 0xfff80000) >> 19) | ((cmmu_ptr -> S_U) << 13);

			if (probeflag)
				batc_log_entry = ((*log_addr_ptr & 0xfff80000) >> 19) | ((probetype) << 13);

			if (debugflag)
				PPrintf ("batc_log_entry = 0x%08x \n", batc_log_entry);

			cmmu_ptr->stats.num_BATC_chk++;    /* increment number of BATC checks */

			if (check_BATC (cmmu_ptr, batc_log_entry))
			{			/* in BATC */
				if (debugflag)
					PPrintf (" BATC HIT \n");

				cmmu_ptr->stats.num_BATC_hits++;   /* increment number of BATC hits */
				if(probeflag) cmmu_ptr->control.SSR |= BH_BMASK;
					batc_offset = (*log_addr_ptr & 0x0007ffff);

				if (debugflag)
					PPrintf ("batc_offset = 0x%08x \n", batc_offset);

				if (get_from_BATC (cmmu_ptr, &batc_phy_entry, batc_log_entry, writeflag, probeflag) == -1)
					return((probeflag) ? 0: -1);

				if (debugflag)
					PPrintf ("batc_phy_entry = 0x%08x \n", batc_phy_entry);

				phy_address = batc_phy_entry | batc_offset;
				if (debugflag)
					PPrintf ("phy_address = 0x%08x \n", phy_address);

				/* if this is for an eventual store send back translated address */
				if (writeflag)
					*log_addr_ptr = phy_address;
 
				/* write physical address to address control register if a probe */
				if (probeflag)
					cmmu_ptr->control.SADR = phy_address;
 
				/* check global variable 'cache_inhibit' to see if 'c' bit was set in the batc entry */
				if (cache_inhibit)
				{
					if(!(writeflag))
						cmmutime += TIME_RMEMACC;
					inv_centry = load_from_cache(cmmu_ptr, data, phy_address, writeflag);
					return (rdwr ((cmmu_ptr->IorD | M_RD), phy_address, data, 4));
				}
				else
				{
					if (cmmu_ptr == &Icmmu)
					ICcnt++;
					else
					{
						DCcnt++;
						if (debugflag)
							printf(" DCcnt = %d \n",DCcnt);

						if (writeflag)
						{
							SDCcnt++;
							if (debugflag)
								printf(" SDCcnt = %d \n",SDCcnt);
						}
						else
						{
							LDCcnt++;
							if (debugflag)
								printf(" LDCcnt = %d \n",LDCcnt);
						}
					}
								/* batc 'c' bit was not set */
					return (load_from_cache (cmmu_ptr, data, phy_address, writeflag));
				}
			}
			else
			{			/* not in BATC so check PATC */

				patc_log_entry = ((*log_addr_ptr & 0xfffff000) >> 12) | ((cmmu_ptr -> S_U) << 20);

				if (probeflag)
					patc_log_entry = ((*log_addr_ptr & 0xfffff000) >> 12) | ((probetype) << 20);

				if (debugflag)
					PPrintf ("cmmu_ptr->S_U = %d \n", cmmu_ptr -> S_U);

				if (debugflag)
					PPrintf ("patc_log_entry = 0x%08x \n", patc_log_entry);

				cmmu_ptr->stats.num_PATC_chk++;   /* increment number of PATC checks */

				if (check_PATC (cmmu_ptr, patc_log_entry))
				{
					/* in PATC */
					if (debugflag)
						PPrintf (" PATC HIT \n");

					cmmu_ptr->stats.num_PATC_hits++;	/* increment number of PATC hits */
					patc_offset = (*log_addr_ptr & 0x00000fff);
					if (debugflag)
						PPrintf ("patc_offset = 0x%08x \n", patc_offset);

					if (get_from_PATC (cmmu_ptr, &patc_phy_entry, patc_log_entry,
					                   writeflag, *log_addr_ptr, probeflag) == -1)
						return ((probeflag) ? 0: -1);

					if (debugflag)
						PPrintf ("patc_phy_entry = 0x%08x \n", patc_phy_entry);

					phy_address = (patc_phy_entry << 12) | patc_offset;
					if (debugflag)
						PPrintf ("phy_address = 0x%08x \n", phy_address);

					/* if this is for an eventual store send back translated address */
					if (writeflag)
						*log_addr_ptr = phy_address;

					/* write physical address to address control register if a probe */
					if (probeflag)
						cmmu_ptr->control.SADR = phy_address;

					/* check global variable 'cache_inhibit' to see if 'c' bit
					 * was set in the patc entry
					 */
					if (cache_inhibit)
					{
						if(!(writeflag))
							cmmutime += TIME_RMEMACC;

						inv_centry = load_from_cache(cmmu_ptr, data, phy_address, writeflag);
						return (rdwr ((cmmu_ptr->IorD | M_RD), phy_address, data, 4));
					}
					else
					{
						if (cmmu_ptr == &Icmmu)
							ICcnt++;
						else
						{
							DCcnt++;
							if (debugflag)
								printf(" DCcnt = %d \n",DCcnt);
							if (writeflag)
							{
								SDCcnt++;
								if (debugflag)
									printf(" SDCcnt = %d \n",SDCcnt);
							}
							else
							{
								LDCcnt++;
								if (debugflag)
									printf(" LDCcnt = %d \n",LDCcnt);
							}
						}
										/* batc 'c' bit was not set */
						return (load_from_cache (cmmu_ptr, data, phy_address, writeflag));
					}
				}
				else
				{
									/* not in PATC or BATC so do tablewalk */
					cmmutime += TIME_TBLWK;
					cmmu_ptr->stats.num_tblwks++;	/*increment number of tblwlks ctr */

					if (tablewalk(cmmu_ptr, &phy_address, *log_addr_ptr, patc_log_entry,
					              writeflag, 0, probeflag))
						return ((probeflag) ? 0: -1);

					if (debugflag)
						PPrintf ("phy_address = 0x%08x \n", phy_address);

					/* if this is for an eventual store send back translated address */
					if (writeflag)
						*log_addr_ptr = phy_address;

					/* write physical address to address control register if a probe */
					if (probeflag)
						cmmu_ptr->control.SADR = phy_address;

					/* check global variable 'cache_inhibit' to see if any descriptors c bit was set */
					if (debugflag)
						PPrintf ("cache_inhibit = %d \n", cache_inhibit);

					if (cache_inhibit)
					{
						if(!(writeflag)) cmmutime += TIME_RMEMACC;
							inv_centry =
								load_from_cache(cmmu_ptr, data, phy_address, writeflag);
							return (rdwr ((cmmu_ptr->IorD | M_RD), phy_address, data, 4));
					}
					else
					{
						if (cmmu_ptr == &Icmmu)
							ICcnt++;
						else
						{
							DCcnt++;

							if (debugflag)
								printf(" DCcnt = %d \n",DCcnt);

							if (writeflag)
							{
								SDCcnt++;
								if (debugflag)
									printf(" SDCcnt = %d \n",SDCcnt);
							}
							else
							{
								LDCcnt++;
								if (debugflag)
									printf(" LDCcnt = %d \n",LDCcnt);
							}
						}
										/* batc 'c' bit was not set */
						return (load_from_cache (cmmu_ptr, data, phy_address, writeflag));
					}
				}
			}
		}
		else
		{			/* Identity mapping/cache mode */
			if (APR & CI_BMASK)
			{			/* if cache inhibit */
				if(!(writeflag))
					cmmutime += TIME_RMEMACC;

				cache_inhibit = 1;
				inv_centry = load_from_cache(cmmu_ptr, data, *log_addr_ptr, writeflag);
				return (rdwr ((cmmu_ptr->IorD | M_RD), (*log_addr_ptr & 0xFFFFFFFC), data, 4));
			}
			else
			{
				cache_inhibit = 0;

				if (APR & WT_BMASK)
					write_through = 1;

				if (cmmu_ptr == &Icmmu)
					ICcnt++;
				else
				{
					DCcnt++;
					if (debugflag)
						printf(" DCcnt = %d \n",DCcnt);
					if (writeflag)
					{
						SDCcnt++;
						if (debugflag)
							printf(" SDCcnt = %d \n",SDCcnt);
					}
					else
					{
						LDCcnt++;
						if (debugflag)
							printf(" LDCcnt = %d \n",LDCcnt);
					}
				}
				return (load_from_cache (cmmu_ptr, data, *log_addr_ptr, writeflag));
			}
		}
	}
}



/***********************************************************************
*	tablewalk
*/
int tablewalk (struct cmmu *cmmu_ptr,unsigned int *phy_address,
unsigned int log_addr,unsigned int patc_log_entry,int writeflag,
int updateM,int probeflag)
{

	unsigned int	seg_num, page_num, page_offset;
	unsigned int	area_desc, seg_desc, page_desc;
	unsigned int	STBaddr, PTPaddr;
	unsigned int	seg_table_entry, page_table_entry;
	unsigned int	page_frame;
	int		Mflag = 0;

	/* extract segment number, page number, and page offset */

	seg_num = log_addr & 0xffc00000;
	page_num = log_addr & 0x003ff000;
	page_offset = log_addr & 0x00000fff;

	if (debugflag)
		PPrintf ("seg_num = 0x%08x page_num = 0x%08x page_offset = 0x%08x\n", seg_num, page_num, page_offset);

	/* set physical address equal to incoming logical address by default */
	/* in case any of the descriptors are invalid */

	*phy_address = log_addr;

	/* retrieve area descriptor */

	if (probeflag)
	{
		if (probetype)					/* SUPERVISOR */
			area_desc = cmmu_ptr->control.SAPR;
		else						/* USER */
			area_desc = cmmu_ptr->control.UAPR;
	}
	else if (!(cmmu_ptr -> S_U))
	{
		area_desc = cmmu_ptr -> control.UAPR;		/* use 'user' area pointer */
		if(probeflag) cmmu_ptr->control.SSR |= SP_BMASK;
	}
	else
		area_desc = cmmu_ptr -> control.SAPR;		/* use 'supervisor' area pointer */

	if (debugflag)
		PPrintf ("area_desc = 0x%08x \n", area_desc);

	/* check 'c' bit of area descriptor and set/clear global variable 'cache_inhibit' */

	if (area_desc & CI_BMASK)
	{
		cache_inhibit = 1;
		if(probeflag)
			cmmu_ptr->control.SSR |= CI_BMASK;
	}
	else
		cache_inhibit = 0;

	/* check 'g' bit of area descriptor and set/clear variable 'global' */

	if (area_desc & G_BMASK)
	{
		global = 1;
		if (probeflag)
			cmmu_ptr->control.SSR |= G_BMASK;
	}
	else
		global = 0;

	/* check 't' bit of area descriptor and set/clear global variable 'write through' */

	if (area_desc & WT_BMASK)
	{
		write_through = 1;
		if(probeflag) cmmu_ptr->control.SSR |= WT_BMASK;
	}
	else
		write_through = 0;

	/* obtain segment table base address from area descriptor */

	STBaddr = area_desc & 0xfffff000;

	if (debugflag)
		PPrintf ("STBaddr = 0x%08x \n", STBaddr);

	seg_table_entry = (STBaddr & 0xfffff000) | ((seg_num >> 20) & 0xfffffffc);

	if (debugflag)
		PPrintf ("seg_table_entry = 0x%08x \n", seg_table_entry);

	/* retrieve segment descriptor from segment table entry in memory */

	if (rdwr((cmmu_ptr->IorD | M_RD), seg_table_entry, &seg_desc, 4) == -1)
		return (-1);
	if (debugflag)
		PPrintf ("seg_desc = 0x%08x \n", seg_desc);

	if (seg_desc & 0x00000001)	/* check if valid segment descriptor */
	{

	/* If current access mode is 'user' make sure descriptor's Supervisor bit is not set. */

		if (!(cmmu_ptr -> S_U) && (seg_desc & 0x00000100))
		{
			PPrintf ("Supervisor privilege violation <<trapping>> \n");
			cmmu_ptr->control.LSR = SUPER_VIOLATION;
/*
			cmmu_ptr -> control.SSR &= EXCEPT_CLR;
			cmmu_ptr -> control.SSR |= INV_SD_SV;
*/
			return (-1);
		}

		if ((probeflag) && (seg_desc & SP_BMASK))
			cmmu_ptr->control.SSR |= SP_BMASK;

		if ((probeflag) && (seg_desc & WP_BMASK))
			cmmu_ptr->control.SSR |= WP_BMASK;

		/* If write access, check to see if write protection bit is set */

		if ((writeflag) && (seg_desc & WP_BMASK))
		{
			PPrintf ("error: (segment descriptor) write protection violation <<trapping>> \n");
			cmmu_ptr->control.LSR = WRITE_VIOLATION;
/*
			cmmu_ptr -> control.SSR &= EXCEPT_CLR;
			cmmu_ptr -> control.SSR |= WR_PROTECT_VIO;
*/
			return (-1);
		}

		/* check 'c' bit of segment descriptor and set/clear global variable 'cache_inhibit' */

		if ((seg_desc & CI_BMASK) || (cache_inhibit))
		{
			cache_inhibit = 1;
			if(probeflag)
				cmmu_ptr->control.SSR |= CI_BMASK;
		}
		else
			cache_inhibit = 0;

		/* check 'g' bit of area descriptor and set/clear variable 'global' */

		if ((seg_desc & G_BMASK) || (global))
		{
			global = 1;
			if (probeflag)
			cmmu_ptr->control.SSR |= G_BMASK;
		}
		else
			global = 0;

		/* check 't' bit of segment descriptor and set/clear global variable 'write through' */

		if ((seg_desc & WT_BMASK) || (write_through))
		{
			write_through = 1;
			if(probeflag)
				cmmu_ptr->control.SSR |= WT_BMASK;
		}
		else
			write_through = 0;

		/* obtain page table page address from segment descriptor */

		PTPaddr = seg_desc & 0xfffff000;

		if (debugflag)
			PPrintf ("PTPaddr = 0x%08x \n", PTPaddr);

		page_table_entry = (PTPaddr & 0xfffff000) | ((page_num >> 10) & 0xfffffffc);

		if (debugflag)
			PPrintf ("page_table_entry = 0x%08x \n", page_table_entry);

		/* retrieve page descriptor from page table entry in memory */

		if (rdwr((cmmu_ptr->IorD | M_RD), page_table_entry, &page_desc, 4) == -1)
			return (-1);

		if (debugflag)
			PPrintf ("page_desc = 0x%08x \n", page_desc);

		/* check if valid page descriptor valid descriptor */
		if (page_desc & 0x00000001)
		{

			/* If current access mode is 'user' make sure descriptor's Supervisor bit is not set. */

			if (!(cmmu_ptr -> S_U) && (page_desc & 0x00000100))
			{
				PPrintf ("Supervisor privilege violation <<trapping>> \n");
				cmmu_ptr->control.LSR = SUPER_VIOLATION;
/*
				cmmu_ptr -> control.SSR &= EXCEPT_CLR;
				cmmu_ptr -> control.SSR |= INV_PD_SV;
*/
				return (-1);
			}

			if ((probeflag) && (page_desc & M_BMASK))
				cmmu_ptr->control.SSR |= M_BMASK;

			if ((probeflag) && (page_desc & SP_BMASK))
				cmmu_ptr->control.SSR |= SP_BMASK;

			if ((probeflag) && (page_desc & WP_BMASK))
				cmmu_ptr->control.SSR |= WP_BMASK;

			/* If write access, check to see if write protection bit is set */

			if ((writeflag) && (page_desc & WP_BMASK))
			{
				PPrintf ("error: (page descriptor) write protection violation <<trapping>> \n");
				cmmu_ptr->control.LSR = WRITE_VIOLATION;
/*
				cmmu_ptr -> control.SSR &= EXCEPT_CLR;
				cmmu_ptr -> control.SSR |= WR_PROTECT_VIO;
*/
				return (-1);
			}

			/* Whenever a write to main memory is mapped through a page descriptor */
			/* and the 'Modified' bit is not set, it is set */

			if ((writeflag) && !(page_desc & M_BMASK))
			{
				Mflag = 1;
				cmmutime += TIME_TBLWKWUM;
				cmmu_ptr->stats.num_tblwks_UM++; /*increment # of tblwlks w/U&M update*/
				page_desc |= M_BMASK;

				/* write_memory (page_desc, page_table_entry);*/
				rdwr ((cmmu_ptr->IorD | M_WR), page_table_entry, &page_desc, 4);
			}

			/* This table walk for purpose of setting 'Modified' bit in page_descriptor only */
			if (updateM)
				return (0);

			/* check 'c' bit of page descriptor and set/clear global variable 'cache_inhibit' */

			if ((page_desc & CI_BMASK) || (cache_inhibit))
			{
				cache_inhibit = 1;
				if(probeflag)
					cmmu_ptr->control.SSR |= CI_BMASK;
			}
			else
				cache_inhibit = 0;

			/* check 'g' bit of area descriptor and set/clear variable 'global' */

			if ((page_desc & G_BMASK) || (global))
			{
				global = 1;
				if (probeflag)
					cmmu_ptr->control.SSR |= G_BMASK;
			}
			else
				global = 0;

			/* check 't' bit of page descriptor and set/clear global variable 'write through' */

			if ((page_desc & WT_BMASK) || (write_through))
			{
				write_through = 1;
				if(probeflag)
					cmmu_ptr->control.SSR |= WT_BMASK;
			}
			else
				write_through = 0;


			/* set 'Used bit' of the page descriptor */

			if(!(page_desc & U_BMASK))
			{
				if(!(Mflag))
					cmmutime += TIME_TBLWKWUM;

				page_desc |= U_BMASK;
				cmmu_ptr->stats.num_tblwks_UM++; /*increment # of tblwlks w/U&M update*/

				/* write_memory (page_desc, page_table_entry); */
				rdwr ((cmmu_ptr->IorD | M_WR), page_table_entry, &page_desc, 4);

			}
			/* obtain page frame address from page descriptor */
			page_frame = page_desc & 0xfffff000;

			if (debugflag)
				PPrintf ("page_frame = 0x%08x \n", page_frame);

			/* create new physical address */
			*phy_address = page_frame | page_offset;
			if(probeflag) cmmu_ptr->control.SSR |= V_BMASK;

			/* update the PATC with the new translation information */
			update_PATC (cmmu_ptr, patc_log_entry, *phy_address, page_desc, seg_desc, area_desc, writeflag);


		}
		else			/* invalid page descriptor */
		{
			PPrintf (" error: invalid page descriptor <<trapping>> \n");

			/* inform status register */
			cmmu_ptr->control.LSR = PAGE_FAULT;
/*
			cmmu_ptr -> control.SSR &= 0xfffffffe;
			cmmu_ptr -> control.SSR &= EXCEPT_CLR;
			cmmu_ptr -> control.SSR |= INV_PD_SV;
*/

			if (debugflag)
				printf("control.LSR = 0x%08x \n",cmmu_ptr->control.LSR);

			cmmu_ptr -> control.LADR = page_table_entry;

			if (debugflag)
				printf("control.LADR = 0x%08x \n",cmmu_ptr->control.LADR);

			return (-1);
		}

	}
	else			/* invalid segment descriptor */
	{
		PPrintf (" error: invalid segment descriptor <<trapping>> \n");

		/* inform status register */
		cmmu_ptr->control.LSR = SEGMENT_FAULT;
/*
		cmmu_ptr -> control.SSR &= 0xfffffffe;
		cmmu_ptr -> control.SSR &= EXCEPT_CLR;
		cmmu_ptr -> control.SSR |= INV_SD_SV;
*/
		if (debugflag)
			printf("control.LSR = 0x%08x \n",cmmu_ptr->control.LSR);

		cmmu_ptr -> control.LADR = seg_table_entry;

		if (debugflag)
			printf("control.LADR = 0x%08x \n",cmmu_ptr->control.LADR);

		return (-1);
	}
	return (0);
}
