/* @(#) File name: interface.c   Ver: 3.2   Cntl date: 2/16/89 11:00:18 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

/*****************************************************************************
*
*		This module contains all memory support functions
*
*/

#include "functions.h"
#include "trans.h"



	/**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/
#define CACHE_WPL	(4)

	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	int	memerr;
extern	struct mem_segs memory[];
extern	int	interrupt;
extern	int	quit;
extern	int	buserr;
extern	int	segviol;
extern	int	vecyes;


	/**********************************************************
	*
	*       Local Defines
	*
	*/


	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/

struct transaction trans;
int	sID;



	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      transinit()
	*
	*	Initialize transactions perameters
	*/


int transinit(char *errmsg,int size)
{
	if(trans.size > size)
	{
		PPrintf("%s: unknown size for function\n", errmsg);
		return(-1);
	}
	return(0);
}

        /**********************************************************
        *
        *      rdwr()
        *
        *       read or write to memory
        */

union allptrs {void *v; unsigned char *c; unsigned short *s; unsigned int *i;};

int rdwr(int rdwrflg,unsigned int memadr,void *srcdesta,int size)
{
	struct mem_wrd *ptr, *end;
	union allptrs srcdest;
	int cnt, seg;
	int addr;

	srcdest.v = srcdesta;

	if((seg = checklmt(memadr, (rdwrflg & M_SEGMSK))) == -1)
		return(-1);

	if((cnt = size) == 0)
		return(size);

	ckbrkpts(memadr, (rdwrflg & (M_RD | M_WR)));

	if(sID && ((rdwrflg & M_SEGMSK) == M_SEGANY))
		addr = memadr - memory[seg].physaddr;
	else
		addr = memadr - memory[seg].baseaddr;
	ptr = memory[seg].seg + (addr / 4);
	end = memory[seg].seg + ((memory[seg].endaddr - memory[seg].baseaddr)/4);

	if(rdwrflg & M_RD)
	{
		while((addr % 4) && (cnt))
		{
			*srcdest.c++ = ptr->mem.c[addr++ % 4];
			cnt--;
			if(!(addr % 4))
				ptr++;
		}
		while(!((int)srcdest.i & 3) && (cnt / 4) && (ptr <= end) && !ckquit())
		{
			*(srcdest.i)++ = ptr++->mem.l;
			addr += 4;
			cnt -= 4;
		}
		while(cnt && (ptr <= end) && !ckquit())
		{
			*srcdest.c++ = ptr->mem.c[addr++ % 4];
			cnt--;
			if(!(addr % 4))
				ptr++;
		}
	}
	if(rdwrflg & M_WR)
	{
		while((addr % 4) && (cnt))
		{
			ptr->mem.c[addr++ % 4] = *srcdest.c++;
			cnt--;
			if(!(addr % 4) || !(cnt))
			{
				makesim(ptr->mem.l, &ptr->opcode);
				ptr++;
			}
		}
		while(!((int)srcdest.i & 3) && (cnt / 4) && (ptr <= end) && !ckquit())
		{
			ptr->mem.l = *srcdest.i++;
			makesim(ptr->mem.l, &ptr->opcode);
			ptr++;
			addr += 4;
			cnt -= 4;
		}
		while(cnt && (ptr <= end) && !ckquit())
		{
			ptr->mem.c[addr++ % 4] = *srcdest.c++;
			cnt--;
			if(!(addr % 4) || !(cnt))
			{
				makesim(ptr->mem.l, &ptr->opcode);
				ptr++;
			}
		}
	}
	if(ckquit())
		return(-1);

	return(size - cnt);
}

int ckquit(void)
{
	return(interrupt | quit | buserr | segviol);
}


int checklmt(unsigned int addr1,int segtype)
{
	int i;

	for(i = 0; i < MAXSEGS; i++)
	{
		if(!memory[i].seg)
			continue;

		if(sID && (segtype == M_SEGANY))
		{
			if((addr1 >= memory[i].physaddr) && (addr1 <= memory[i].phyeaddr))
				return(i);
		}
		else
		{
			/*
			** The check for endaddr needs to be < not <= otherwise
			** if addr1 happend to be = to endaddr the wrong seg
			** number will be returned
			*/

			if((addr1 >= memory[i].baseaddr) && (addr1 < memory[i].endaddr)
			  && (memory[i].flags & segtype))
				return(i);
		}
	}
	memerr = 1;
	if(!vecyes)
		Eprintf("\nMemory not found at address %08X\n", addr1);
	return(-1);
}

struct mem_wrd *getmemptr(unsigned int addr1,int segtype)
{
	unsigned int addr;
	int seg;

	if((seg = checklmt(addr1, segtype)) == -1)
		return(0);

	addr = addr1 - memory[seg].baseaddr;
	return(memory[seg].seg + (addr / 4));
}


/*
*******************************************************************************
*
*       Module:     getpmem()
*
*       File:       interface.c
*
*******************************************************************************
*
*  Functional Description:
*	This function is called to adjust the size of existing memory
*	segments.
*
*  Globals:
*
*  Parameters:
*	addr	New end address of segment.
*
*  Output:
*
*  Revision History:
*	If the cmmu in being used (-c) flag, then all memory segments should
*	be multiples of cache lines in size.  This should eliminate error
*	messages when unmapped memory is read by the cache when accessing
*	data at the extremes of the mapped segment.
*
*******************************************************************************/

int getpmem(unsigned int addr)
{
	extern int	usecmmu;
	unsigned int	seg, size;

	for(seg = 0; seg < MAXSEGS; seg++)
		if(memory[seg].flags & M_PMAP)
			break;

	if(seg == MAXSEGS)
		return(-1);

		/*
		 * If using the CMMU, all segments should end on
		 * cache line boundaries.
		 */
	if(usecmmu)
		addr = addr | (unsigned int)((CACHE_WPL * 4) - 1);

	if(memory[seg].baseaddr > addr)
		return(-1);

	size = addr - memory[seg].baseaddr;

	if((memory[seg].seg = (struct mem_wrd *)realloc(memory[seg].seg, (size/4)*MEMWRDSIZE)) == 0)
	{
		perror("getpmem");
		return(-1);
	}

	memory[seg].phyeaddr = memory[seg].physaddr + size;

	return(memory[seg].endaddr = memory[seg].baseaddr + size);	
}

int getmem(int base, int size, int segtype,unsigned int  phyaddr)
{
	int i, oldvecyes = vecyes;
	int seg;

	vecyes = 1;
	if((seg = checklmt(base, segtype)) == -1)
		seg = checklmt(base+size, segtype);
	vecyes = oldvecyes;

	if(seg != -1)
	{
		if(base < memory[seg].baseaddr)
		{
			fprintf(stderr, "Changing the base address of a segment not permitted");
			return(-1);
		}
		size = base + size - memory[seg].baseaddr;
		if((memory[seg].seg = (struct mem_wrd *)realloc(memory[seg].seg, ((size/4)+1)*MEMWRDSIZE)) == 0)
		{
			perror("getmem");
			return(-1);
		}
		memory[seg].endaddr = memory[seg].baseaddr + size;
		memory[seg].phyeaddr = memory[seg].physaddr + size;
	}
	else
	{
		for(i = 0; i < MAXSEGS; i++)
		{
			if(memory[i].seg == 0)
				break;
		}
		if(i == MAXSEGS)
			return(-1);
		if((memory[i].seg = (struct mem_wrd *)calloc(((size / 4) + 1), MEMWRDSIZE)) == 0)
		{
			perror("getmem");
			return(-1);
		}
		memory[i].baseaddr = base;
		memory[i].endaddr = base + size;
		memory[i].physaddr = phyaddr;
		memory[i].phyeaddr = phyaddr + size;
		memory[i].flags = segtype;
		if(segtype & M_PMAP)
			memory[i].flags |= M_DATA;
	}
	return(0);
}


void releasemem(void)
{
	int i;

	for(i = 0; i < MAXSEGS; i++)
		if(!(memory[i].flags & 0x100))
			releaseseg(i);

	return;
}

void releasepmem(void)
{
	int i;

	for(i = 0; i < MAXSEGS; i++)
		if((memory[i].flags & M_PMAP))
			releaseseg(i);

	return;
}


void releaseseg(int i)
{

	if(i < MAXSEGS)
	{
		if(memory[i].seg)
		{
			free(memory[i].seg);
			memory[i].seg = 0;
		}
	}
	return;
}


int dm(void)
{
	int i;
	char *mess;
	struct mem_segs *m = memory;

	for(i = 0; i < MAXSEGS; i++, m++)
	{
		if(!m->seg)
			continue;
		if((m->flags & M_SEGANY) == M_SEGANY)
			mess = "all ";
		else if(m->flags & 0x40)
			mess = "data";
		else if(m->flags & 0x80)
			mess = "bss ";
		else if(m->flags & M_INSTR)
			mess = "text";
		else
			mess = "unkn";
		if(sID)
		{
			PPrintf("Segment %d: %s Virtual %08X-%08X, Physical %08X-%08X\n",
				i, mess, m->baseaddr, m->endaddr, m->physaddr, m->phyeaddr);
		}
		else
		{
			PPrintf("Segment %d: %s %08X-%08X\n", i, mess, m->baseaddr, m->endaddr);
		}
	}
	return 0;
}

extern int bigendian;
union u1 {	unsigned char	c[4];
		unsigned short	s;
		unsigned int	i;};

void intswap(unsigned int *destptr,unsigned int *srcptr)
{
	union u1 u,v;


	u.i = *srcptr;
#ifdef LEHOST
		v.c[0] = u.c[3];
		v.c[1] = u.c[2];
		v.c[2] = u.c[1];
		v.c[3] = u.c[0];
#else
		v.c[0] = u.c[0];
		v.c[1] = u.c[1];
		v.c[2] = u.c[2];
		v.c[3] = u.c[3];
#endif
	*destptr = v.i;
}

void shortswap(unsigned short *destptr,unsigned short *srcptr)
{
	union u1 u,v;


	u.s = *srcptr;
#ifdef LEHOST
		v.c[0] = u.c[1];
		v.c[1] = u.c[0];
#else
		v.c[0] = u.c[0];
		v.c[1] = u.c[1];
#endif
	*destptr = v.s;
}
