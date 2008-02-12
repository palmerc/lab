/* @(#) File name: cmmu_cache.c   Ver: 3.1   Cntl date: 1/20/89 14:17:25 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"

extern int IChit, DChit;
extern int  wrapflag;		/* memory wrap-around option flag */
extern int  debugflag;		/* debug option flag */
extern int  write_through;
extern int  cache_inhibit;
extern int  global;
extern int  cmmutime;


int load_from_cache(struct cmmu *cmmu_ptr,unsigned int *data,
unsigned int address,int writeflag)
{
    unsigned int   setnum, comparetag;
    unsigned int   datawrd;

    /* decode bits (11-4) to select 1 of 256 cache sets */

    setnum = (address & SETMASK) >> 4;

    /* decode bits (3-2) to select 1 of 4 data words */

    datawrd = (address & DATAWRDMASK) >> 2;

    /* mask off log. addr. bits (31-12) to compare to phy. addr. tag in cache */

    comparetag = (address & ADDRTAGMASK);
    if(debugflag) PPrintf("comparetag = 0x%08X \n", comparetag);

    if (check_cache (cmmu_ptr, setnum, comparetag))
    {
	if (debugflag) PPrintf ("cache HIT \n");
	*data = cache_hit (cmmu_ptr, setnum, comparetag, datawrd);
    if ((cmmu_ptr == &Icmmu) && (!cache_inhibit))
	   IChit++;
    else
       if ((cmmu_ptr == &Dcmmu) && (!cache_inhibit))
          {
	       DChit++;
	       if(writeflag)
		      SDChit++;
		   else
		      LDChit++;
		  }
    }
    else
    {
	if (debugflag)
	    PPrintf ("cache MISS \n");
        if (cache_inhibit) return (0);
        if (debugflag) printf(" after load from cache :cmmutime = %d \n",cmmutime);
        return (cache_miss (cmmu_ptr, data, setnum, comparetag, datawrd, address, writeflag));
    }
    return (0);
}



int check_cache(struct cmmu *cmmu_ptr,unsigned int setnum,
unsigned int comparetag)
{
    int     i;
    /* check if any lines in set are valid */
    if (((cmmu_ptr -> cache.cache_entry[setnum][0].vv) != INV) ||
          ((cmmu_ptr -> cache.cache_entry[setnum][1].vv) != INV) ||
          ((cmmu_ptr -> cache.cache_entry[setnum][2].vv) != INV) ||
          ((cmmu_ptr -> cache.cache_entry[setnum][3].vv) != INV))
    {				/* there is some valid data in this set */
	for (i = 0; i < LPS; i++)
	{
	    if ((comparetag == cmmu_ptr -> cache.cache_entry[setnum][i].tag) &&
		    ((cmmu_ptr -> cache.cache_entry[setnum][i].vv) != INV) &&
		    !(cmmu_ptr -> cache.cache_entry[setnum][i].kill))
		return (1);
	}
    }
    return (0);
}



int cache_hit(struct cmmu *cmmu_ptr,unsigned int setnum,
unsigned int comparetag,unsigned int datawrd)
{
    int     ord, line;

    line = 0;
    while (comparetag != cmmu_ptr -> cache.cache_entry[setnum][line].tag)
	line++;
    ord = cmmu_ptr -> cache.cache_entry[setnum][line].order;
    mark_lru (ord, cmmu_ptr, setnum, line);
    if(cache_inhibit)
	cmmu_ptr->cache.cache_entry[setnum][line].vv = INV;
	/* mark set as having been accessed */
	cmmu_ptr->cache.cache_accessed[setnum] = 1;
    return (cmmu_ptr -> cache.cache_entry[setnum][line].data[datawrd]);
}




void mark_lru(int ord, struct cmmu *cmmu_ptr,unsigned int setnum,int line)
{
    switch (ord)
    {
	case 3: 		/* do nothing */
	    break;

	case 2: 
	    find_change_order (cmmu_ptr, setnum, 3, 2);
	    break;

	case 1: 
	    find_change_order (cmmu_ptr, setnum, 2, 1);
	    find_change_order (cmmu_ptr, setnum, 3, 2);
	    break;
	case 0: 
	    find_change_order (cmmu_ptr, setnum, 1, 0);
	    find_change_order (cmmu_ptr, setnum, 2, 1);
	    find_change_order (cmmu_ptr, setnum, 3, 2);
	    break;
    }
    cmmu_ptr -> cache.cache_entry[setnum][line].order = 3;
}



void find_change_order(struct cmmu *cmmu_ptr,
unsigned int setnum,int oldord,int neword)
{
    int     i;
    i = 0;
    while ((cmmu_ptr -> cache.cache_entry[setnum][i].order != oldord) && (i < 4))
	i++;
    if (i > 4)
	PPrintf ("error: line index = %d in function find_change_order \n", i);
    else
	cmmu_ptr -> cache.cache_entry[setnum][i].order = neword;
}



int cache_miss (struct cmmu *cmmu_ptr,unsigned int *data,unsigned int setnum,
unsigned int comparetag,unsigned int datawrd,unsigned int address,int writeflag)
{
    int     line, ord, i, j, wordpos, wordstrt=0, line_selected, lru_val;
    int    retr_data[WPL];

    /* Find the cache entry to be replaced by finding the  */
    /* least recently used line in set */

    line_selected = FALSE;
    lru_val = 0;
    do
    {
	line = 0;
	while (cmmu_ptr -> cache.cache_entry[setnum][line].order != lru_val)
	    line++;
	if (cmmu_ptr -> cache.cache_entry[setnum][line].kill)
	{
	    if (lru_val >= 3)
	    {
		PPrintf (" error - all lines in this set have been killed \n");
		exit (0);
	    }
	    else
		lru_val++;
	}
	else
	    line_selected = TRUE;
    }
    while (!(line_selected));

    /* Does the entry have valid data ? */

    if (cmmu_ptr -> cache.cache_entry[setnum][line].vv != INV)
    {				/*  yes  */
	    /* Is the old entry "em" exclusive modified ? */
	    if (cmmu_ptr -> cache.cache_entry[setnum][line].vv == EM)
	    {			/*  yes  */
		/* write entire cache line back to memory */
		cmmutime += ((writeflag) ? TIME_WCMWCB: TIME_RCMWCB);
		if (writeflag)
			cmmu_ptr->stats.num_wcb++; /* increment WCM with copy-back */
        else
			cmmu_ptr->stats.num_rcb++; /* increment RCM with copy-back */
		j = cmmu_ptr -> cache.cache_entry[setnum][line].tag | (setnum << 4);
		for (i = 0; i < 4; i++, j += 4)
		    if (rdwr ((cmmu_ptr->IorD | M_WR), j, &cmmu_ptr->cache.cache_entry[setnum][line].data[i], 4) == -1)
			return (-1);
	    }
    }

    /* align for four word boundary */

    wordpos = ((address & 0x0000000c) >> 2) & 0x3;/* wordpos should be 0,1,2, or 3 */
    switch (wordpos)
    {
	case 0: 
	    wordstrt = 0;
	    break;
	case 1: 
	    wordstrt = -4;
	    break;
	case 2: 
	    wordstrt = -8;
	    break;
	case 3: 
	    wordstrt = -12;
	    break;
    }

	if (debugflag) printf("before rdwr\n");
	if (debugflag) printf("(address & 0xfffffffc) + wordstrt = 0x%08x \n",(address & 0xfffffffc) + wordstrt);
    if (rdwr ((cmmu_ptr->IorD | M_RD), ((address & 0xFFFFFFFC) + wordstrt), retr_data, 16) == -1)
	return (-1);

    /* update all other lines */
    ord = lru_val;
    mark_lru (ord, cmmu_ptr, setnum, line);

	/* mark set as having been accessed */
	cmmu_ptr->cache.cache_accessed[setnum] = 1;

    /* write to cache line */
    cmmu_ptr -> cache.cache_entry[setnum][line].vv = SU;
    cmmu_ptr -> cache.cache_entry[setnum][line].tag = comparetag;

    for (i = 0; i < WPL; i++)
    {
	cmmu_ptr -> cache.cache_entry[setnum][line].data[i] = retr_data[i];
    }

    cmmutime += ((writeflag) ? TIME_WCMWOCB: TIME_RCMWOCB);
    if (debugflag) printf(" after cache miss :cmmutime = %d \n",cmmutime);
    *data = cmmu_ptr -> cache.cache_entry[setnum][line].data[datawrd];
    return (0);

}




int store_data (struct cmmu *cmmu_ptr,unsigned int address,int storeword)
{
    unsigned int   setnum, comparetag;
    unsigned int   datawrd;
    int     line;



	/*printf("storeword = 0x%08x \n",storeword);*/
    if ((cmmu_ptr -> S_U) && ((address & CTRL_SPACE) == CTRL_SPACE))
    {				/* supervisor/user bit set to 'supervisor' */
	cmmutime += TIME_WCTL;
	return (ctrl_space_write (address, storeword));
    }
    if(cache_inhibit)
    {
	cmmutime += TIME_WMEMACC;
	if (rdwr ((cmmu_ptr->IorD | M_WR), address, &storeword, 4) == -1)
	    return (-1);
	return(0);
    }
	
    /* decode bits (11-4) to select 1 of 256 cache sets */

    setnum = (address & SETMASK) >> 4;

    /* decode bits (3-2) to select 1 of 4 data words */

    datawrd = (address & DATAWRDMASK) >> 2;

    /* mask off log. addr. bits (31-12) to compare to phy. addr. tag in cache */

    comparetag = (address & ADDRTAGMASK);

    /* Find particular line in cache set */

    line = 0;
    while ((comparetag != cmmu_ptr -> cache.cache_entry[setnum][line].tag) || 
           ((comparetag == cmmu_ptr -> cache.cache_entry[setnum][line].tag) && 
	       (cmmu_ptr-> cache.cache_entry[setnum][line].vv == INV)))
	line++;

    /* Write to the cache line  */

	/*printf("storeword = 0x%08x \n",storeword);*/
    cmmu_ptr -> cache.cache_entry[setnum][line].data[datawrd] = storeword;

    /* if write through requested, also write directly to memory address */

    if (write_through)
    {
	cmmutime += TIME_WMEMACC;
	if (rdwr ((cmmu_ptr->IorD | M_WR), address, &storeword, 4) == -1)
	    return (-1);
	cmmu_ptr -> cache.cache_entry[setnum][line].vv = SU;
    }

    else
    {
	switch (cmmu_ptr -> cache.cache_entry[setnum][line].vv)
	{
	    case INV: 
				cmmutime += TIME_WCHWR1;
				cmmu_ptr->stats.num_wchwr1++; /* increment write once ctr */
				if (rdwr ((cmmu_ptr->IorD | M_WR), address, &storeword, 4) == -1)
					return (-1);
				/*printf("setnum = 0x%08x line = 0x%08x \n",setnum,line);*/
				cmmu_ptr -> cache.cache_entry[setnum][line].vv = EU;
				break;
	    case SU: 
				if (global)
				{
				cmmutime += TIME_WCHWR1;
				cmmu_ptr->stats.num_wchwr1++; /* increment write once ctr */
				if (rdwr ((cmmu_ptr->IorD | M_WR), address, &storeword, 4) == -1)
					return (-1);
				cmmu_ptr -> cache.cache_entry[setnum][line].vv = EU;
				}
				else
				cmmu_ptr -> cache.cache_entry[setnum][line].vv = EM;
				break;
	    case EU: 
				cmmu_ptr -> cache.cache_entry[setnum][line].vv = EM;
				break;
	    case EM: 
				break;
	}
    }
    if (debugflag) printf(" after store data :cmmutime = %d \n",cmmutime);

    return(0);
}


