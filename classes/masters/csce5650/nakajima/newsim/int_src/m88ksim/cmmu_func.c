/* @(#) File name: cmmu_func.c   Ver: 3.1   Cntl date: 1/20/89 14:17:15 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"

#define FUNC_CODE_MASK	0x0000003f
#define GG_MASK		0x00000003
#define UPPER20		0xfffff000
#define SEG_MASK	0x000ffc00

#define SUP_USER_MASK	0x00000004
#define CACHE_CODE_MASK	0x0000000c
#define SECT_CODE_MASK	0x00000030
#define INVALIDATE_PATC	0x00000030
#define PROBE_ADDRESS	0x00000020
#define CACHE_NOOP	0x00000010

#define NO_OP		0x00000000
#define ATC_PROBE_U	0x00000020
#define ATC_PROBE_S	0x00000024
#define FLUSH_UATC	0x00000030
#define FLUSH_SATC	0x00000034
#define DATA_CBACK_INV	0x0000000c
#define DATA_CBACK	0x00000008
#define DATA_INV	0x00000004

#define GG_LINE		0x0
#define GG_PAGE		0x1
#define GG_SEGMENT	0x2
#define GG_ALL		0x3

#define PATC_SMASK	0x00100000
#define SEGONLYMASK	0xffc00000

int		probetype;	/* user = 0 supervisor = 1 */
extern int	debugflag;


void cmmu_ctrl_func( struct cmmu *cmmu_ptr)
{
	unsigned int	cache_code, sect_code, su_code, comp_LPA;
	unsigned int	setnum, comparetag, compmask;
	unsigned int	prdata1, prdata2, pr_address;
	int		GG, line, i, j;
	int		dcopyback = 0;
	int		invalidate = 0;
	int		seg_flag;

	/* pull out function code */
	sect_code = cmmu_ptr -> control.SCMR & SECT_CODE_MASK;

	if (debugflag)
		PPrintf (" Function code = 0x%08x \n", cmmu_ptr->control.SCMR & FUNC_CODE_MASK);

	/* pull out GG code */
	GG = cmmu_ptr -> control.SCMR & GG_MASK;

	if (debugflag)
		PPrintf (" GG = 0x%08x \n", GG);

	switch(sect_code)
	{
		case INVALIDATE_PATC:
			su_code = (cmmu_ptr->control.SCMR & SUP_USER_MASK);

			switch (GG)
			{
				case GG_LINE:		/* error-only for data cache */
					PPrintf (" error - incorrect function code in control register \n");
					return;

				case GG_SEGMENT: 
				case GG_PAGE: 
					/* create comparison mask */
					comp_LPA = (cmmu_ptr -> control.SADR & UPPER20) >> 12;
					comp_LPA &= 0x000fffff;
					seg_flag = 0;

					if (GG == GG_SEGMENT)
					{
						comp_LPA &= SEG_MASK;
						seg_flag = 1;
					}

					if (su_code)
						comp_LPA |= PATC_SMASK;

					if (debugflag)
						PPrintf (" comp_LPA= 0x%08x \n", comp_LPA);

					flush_byPandS (cmmu_ptr, comp_LPA, seg_flag);
					break;

				case GG_ALL: 
					if(su_code)
						flush_all (cmmu_ptr, 1);
					else
						flush_all (cmmu_ptr, 0);
					break;
			}
			break;
		case PROBE_ADDRESS:
			if((cmmu_ptr->control.SCMR & SUP_USER_MASK) == 0)
				probetype = 0;
			else
				probetype = 1;
							/* logical address for probe is in address register */
			pr_address = cmmu_ptr->control.SADR;
			cmmu_ptr->control.SSR = 0;	/* clear status register for upcoming probe */
			cmmu_ptr->stats.num_probes++;	/* increment probe counter */
							/* turn probe flag on (1) */
			prdata1 = load_data(cmmu_ptr,&prdata2,&pr_address,0,1);
			break;
		case CACHE_NOOP:
			cache_code = (cmmu_ptr->control.SCMR & CACHE_CODE_MASK);
			if (cache_code == DATA_CBACK_INV)
			{
				invalidate = 1;
				dcopyback = 1;
			}
			else if (cache_code == DATA_CBACK)
			{
				invalidate = 0;
				dcopyback = 1;
			}
			else if (cache_code == DATA_INV)
			{
				invalidate = 1;
				dcopyback = 0;
			}
			else
				break;

			if (debugflag)
				printf("invalidate=%d dcopyback=%d \n",invalidate,dcopyback);

			switch (GG)
			{
				case GG_LINE: 
					/* decode bits (11-4) to select 1 of 256 cache sets */
					setnum = (cmmu_ptr -> control.SADR & SETMASK) >> 4;

					/* Pull out address tag */
					comparetag = (cmmu_ptr -> control.SADR & ADDRTAGMASK);

					/* compare and flush as required */
					line = 0;
					while ((comparetag != cmmu_ptr -> cache.cache_entry[setnum][line].tag) &&
					       (line < LPS))
						line++;

					if (line < LPS)
					{
						if ((dcopyback) && (cmmu_ptr -> cache.cache_entry[setnum][line].vv == EM))
						{
							cmmu_ptr -> cache.cache_entry[setnum][line].vv = EU;
							write_line (cmmu_ptr, setnum, line);
						}

						if (debugflag)
							printf("invalidate = %d \n",invalidate);

						if (debugflag)
							printf("setnum = 0x%08x line = 0x%08x \n",setnum,line);

						if (invalidate)
						{
							cmmu_ptr -> cache.cache_entry[setnum][line].vv = INV;
							redo_set_lru(cmmu_ptr, setnum);
						}
					}
				break;

			case GG_PAGE: 
			case GG_SEGMENT: 
				/* setup proper mask for comparison */
				if (GG == GG_PAGE)
					compmask = UPPER20;
				else
					compmask = SEGONLYMASK;

				/* Pull out address tag */
				comparetag = (cmmu_ptr -> control.SADR & ADDRTAGMASK);
				comparetag &= compmask;

				/* search cache for entry to flush and flush as required */
				for (i = 0; i < SETS; i++)
				{
					for (j = 0; j < LPS; j++)
					{
						if ((cmmu_ptr -> cache.cache_entry[i][j].tag & compmask) == comparetag)
						{
							if (dcopyback)
								write_line (cmmu_ptr, i, j);

							if (invalidate)
							{
								cmmu_ptr -> cache.cache_entry[i][j].vv = INV;
								redo_set_lru(cmmu_ptr, i);
							}
						}
					}
				}
				break;

			case GG_ALL: 
				for (i = 0; i < SETS; i++)
				{
					for (j = 0; j < LPS; j++)
					{
						if (cmmu_ptr->cache.cache_entry[i][j].vv != INV)
						{
							if (dcopyback)
								write_line (cmmu_ptr, i, j);
							if (invalidate)
								cmmu_ptr->cache.cache_entry[i][j].vv = INV;
						}
					}
				}
				break;
			}
			break;
		case NO_OP:
			break;
		default:
			PPrintf (" error - incorrect function code \n");
			break;
	}
	return;
}




void flush_all (struct cmmu *cmmu_ptr, int super_atc)
{
	int     i;

	for (i = 0; i < PATC_SIZE; i++)
	{
		if (((super_atc) && (cmmu_ptr -> patc.patc_entry[i].LPA >= PATC_SMASK)) ||
		    ((!super_atc) && (cmmu_ptr -> patc.patc_entry[i].LPA < PATC_SMASK)))
		{
/*			cmmu_ptr -> patc.patc_entry[i].LPA = 0; */
/*			cmmu_ptr -> patc.patc_entry[i].PFA = 0; */
			cmmu_ptr -> patc.patc_entry[i].WT = 0;
			cmmu_ptr -> patc.patc_entry[i].G = 0;
			cmmu_ptr -> patc.patc_entry[i].CI = 0;
			cmmu_ptr -> patc.patc_entry[i].M = 0;
			cmmu_ptr -> patc.patc_entry[i].WP = 0;
			cmmu_ptr -> patc.patc_entry[i].V = 0;
		}
	}
	return;
}




void flush_byPandS(struct cmmu *cmmu_ptr, unsigned int comp_LPA, int seg_flag)
{

    int     i;
    unsigned int   segmentmsk;

    if (debugflag)
	PPrintf (" comp_LPA = 0x%08x \n", comp_LPA);
    if (debugflag)
	PPrintf (" seg_flag = %d \n", seg_flag);

    if ((comp_LPA | SEG_MASK) >= PATC_SMASK)
	segmentmsk = SEG_MASK | PATC_SMASK;
    else
	segmentmsk = SEG_MASK;

    for (i = 0; i < PATC_SIZE; i++)
    {
	if (((seg_flag) && ((cmmu_ptr -> patc.patc_entry[i].LPA & segmentmsk) == comp_LPA)) ||
		((!seg_flag) && (cmmu_ptr -> patc.patc_entry[i].LPA == comp_LPA)))
	{
/*	    cmmu_ptr -> patc.patc_entry[i].LPA = 0; */
/*	    cmmu_ptr -> patc.patc_entry[i].PFA = 0; */
	    cmmu_ptr -> patc.patc_entry[i].WT = 0;
	    cmmu_ptr -> patc.patc_entry[i].G = 0;
	    cmmu_ptr -> patc.patc_entry[i].CI = 0;
	    cmmu_ptr -> patc.patc_entry[i].M = 0;
	    cmmu_ptr -> patc.patc_entry[i].WP = 0;
	    cmmu_ptr -> patc.patc_entry[i].V = 0;
	}
    }
    return;
}



int write_line(struct cmmu *cmmu_ptr, unsigned int   setnum,unsigned int line)
{
	unsigned int     i, j;

	j = cmmu_ptr -> cache.cache_entry[setnum][line].tag | (setnum << 4);
	for (i = 0; i < 4; i++, j += 4)
	{
		if (rdwr ((cmmu_ptr->IorD | M_WR), j, &cmmu_ptr -> cache.cache_entry[setnum][line].data[i], 4) == -1)
			return (-1);
	}

	return 0;
}



void redo_set_lru(struct cmmu *cmmu_ptr, unsigned int setnum)

{
int i, j, k, l, lrucnt, oldlru;

for(i = 0, lrucnt = 0; i < 4; i++)
    {
	if (cmmu_ptr->cache.cache_entry[setnum][i].vv == INV)
	   {
	   oldlru = cmmu_ptr->cache.cache_entry[setnum][i].order;
	   cmmu_ptr->cache.cache_entry[setnum][i].order = lrucnt;
	   if (oldlru != lrucnt)  /* do nothing */
	      if (oldlru == lrucnt+1)
             {
              for(j=0; ((j < 4) && ((cmmu_ptr->cache.cache_entry[setnum][j].order != lrucnt) || (j == i))); j++);
              cmmu_ptr->cache.cache_entry[setnum][j].order = lrucnt+1;
             }
	      else if (oldlru == lrucnt+2)
                  {
                  for(j=0; ((j < 4) && ((cmmu_ptr->cache.cache_entry[setnum][j].order != lrucnt) || (j == i))); j++);
                  cmmu_ptr->cache.cache_entry[setnum][j].order = lrucnt+1;
                  for(k=0; ((k < 4) && ((cmmu_ptr->cache.cache_entry[setnum][k].order == lrucnt+1) || (k == j) || (k == i))); k++);
                  cmmu_ptr->cache.cache_entry[setnum][k].order = lrucnt+2;
                  }
	           else if (oldlru == lrucnt+3)
						{
                        for(j=0; ((j < 4) && ((cmmu_ptr->cache.cache_entry[setnum][j].order != lrucnt) || (j == i))); j++);
						if (debugflag) printf("j=%d \n",j);
                        cmmu_ptr->cache.cache_entry[setnum][j].order = lrucnt+1;
                        for(k=0; ((k < 4) && ((cmmu_ptr->cache.cache_entry[setnum][k].order != lrucnt+1) || (k == j) || (k == i))); k++);
						if (debugflag) printf("k=%d \n",k);
                        cmmu_ptr->cache.cache_entry[setnum][k].order = lrucnt+2;
                        for(l=0; ((l < 4) && ((cmmu_ptr->cache.cache_entry[setnum][l].order != lrucnt+2) || (l == k) || (l == j) || (l == i))); l++);
						if (debugflag) printf("l=%d \n",l);
                        cmmu_ptr->cache.cache_entry[setnum][l].order = lrucnt+2;
                        }
	   lrucnt++;
	   } 
    }
}
