/* @(#) File name: cmmu_ctl.c   Ver: 3.2   Cntl date: 2/9/89 14:42:35 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"

#define BWPSBITMASK	0x00000020
#define LRUMASK		0x3f000000

extern int			IChit, DChit;
extern int			debugflag;
extern int			cmmutime;

extern int			cache_inhibit;
extern int			write_through;
extern int			global;
unsigned int			Dcmmu_devnum, Icmmu_devnum;


/**********************************************************************
*	ctrl_space_read
*/
int ctrl_space_read (unsigned int log_addr,unsigned int *data)
{
	int     Index, i;
	unsigned int   device_num, reg_offset, oldreg_offset=0, setnum, datawrd;
	struct cmmu *cmmu_ptr;
	unsigned int   VN = 0;
	unsigned int   KN = 0;
	unsigned int   L[6];

	if (debugflag)
		PPrintf ("log_addr = 0x%08x \n", log_addr);

	/* pull out device number from incoming logical address */

	device_num = (log_addr & 0x000ff000) >> 12;
	device_num &= 0x000000ff;			/* a guard against sign extension error */

	/* pull out register offset from incoming logical address */

	reg_offset = (log_addr & 0x00000fff);

	if (debugflag)
		PPrintf ("device_num = %d    register offset = 0x%04x \n", device_num, reg_offset);

	/* check reg_offset to find out if it is a Cache Control Register Access */

	if ((reg_offset & CCR_OFFSET_MASK) == CCR_OFFSET_MASK)
	{
		oldreg_offset = reg_offset;
		reg_offset &= SELECT_CCR_MASK;
	}

	/* select device (cmmu) */
	if (debugflag)
		PPrintf ("Dcmmu_devnum = 0x%08x \n", Dcmmu_devnum);

	if (device_num == Dcmmu_devnum)
		cmmu_ptr = &Dcmmu;
	else if (device_num == Icmmu_devnum)
		cmmu_ptr = &Icmmu;
	else
	{
		PPrintf (" error: incorrect device number <<trapping>>\n");
		return (-1);
	}

	/* select register */

	switch (reg_offset)
	{
		case IDR_ro: 		/* cmmu ID register  */
			*data = cmmu_ptr -> control.IDR;
			break;

		case SCMR_ro: 		/* command register  */
			*data = cmmu_ptr -> control.SCMR;
			break;

		case SSR_ro: 		/* status register  */
			*data = cmmu_ptr -> control.SSR;
			break;

		case SADR_ro: 		/* address register  */
			*data = cmmu_ptr -> control.SADR;
			break;

		case SCTR_ro: 		/* control register  */
			*data = cmmu_ptr -> control.SCTR;
			break;

		case LSR_ro:		/* Local Status register */
			*data = cmmu_ptr -> control.LSR;
			break;

		case LADR_ro:		/* Local Address register */
			*data = cmmu_ptr -> control.LADR;
			break;

		case SAPR_ro: 		/* Supervisor area pointer register  */
			*data = cmmu_ptr -> control.SAPR;
			break;

		case UAPR_ro: 		/* User area pointer register  */
			*data = cmmu_ptr -> control.UAPR;
			break;

		case BWP0_ro: 		/* batc write port 0  */
		case BWP1_ro: 		/* batc write port 1  */
		case BWP2_ro: 		/* batc write port 2  */
		case BWP3_ro: 		/* batc write port 3  */
		case BWP4_ro: 		/* batc write port 4  */
		case BWP5_ro: 		/* batc write port 5  */
		case BWP6_ro: 		/* batc write port 6  */
		case BWP7_ro: 		/* batc write port 7  */
			Index = (reg_offset & 0x000000ff);
			Index = Index / 4;
			if (debugflag)
				PPrintf ("Index = %d \n", Index);

			*data = cmmu_ptr -> control.BWP[Index];
			break;

		case CACHE_ro: 		/* cache control register */
					/* decode set number from bits 11-4 of SADR */
			setnum = ((cmmu_ptr -> control.SADR & SETMASK) >> 4) & 0xff;
					/* decode datawrd position from bits 3 & 2 of SADR */
			datawrd = ((cmmu_ptr -> control.SADR & DATAWRDMASK) >> 2) & 0x3;
					/* retrieve cache line requested */
			switch (oldreg_offset - reg_offset)
			{
				case 0x0: 	/* cache line 0 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][0].data[datawrd];
					break;

				case 0x4: 	/* cache line 1 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][1].data[datawrd];
					break;

				case 0x8: 	/* cache line 2 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][2].data[datawrd];
					break;

				case 0xc: 	/* cache line 3 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][3].data[datawrd];
					break;
			}
			break;

		case TAG_ro: 		/* cache tag control access */
						/* decode set number from bits 11-4 of SADR */
			setnum = ((cmmu_ptr -> control.SADR & SETMASK) >> 4) & 0xff;
						/* retrieve cache tag line requested */
			switch (oldreg_offset - reg_offset)
			{
				case 0x0: 	/* cache tag for line 0 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][0].tag;
					break;

				case 0x4: 	/* cache tag for  line 1 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][1].tag;
					break;

				case 0x8: 	/* cache tag for line 2 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][2].tag;
					break;

				case 0xc: 	/* cache tag for line 3 */
					*data = cmmu_ptr -> cache.cache_entry[setnum][3].tag;
					break;
			}
			break;

		case CSSP_ro: 	/* retrieve state of requested cache set */
					/* decode set number from bits 11-4 of SADR */
			setnum = ((cmmu_ptr -> control.SADR & SETMASK) >> 4) & 0xff;
/*
			printf("setnum=%d\n",setnum);
			printf("cmmu_ptr->cache.cache_accessed[setnum]=%d\n",cmmu_ptr->cache.cache_accessed[setnum]);
*/

			if (cmmu_ptr->cache.cache_accessed[setnum] == 0)
			{
				*data = cmmu_ptr->control.CSSP;
						/* fill in KILL and VALID information on the Set */
				*data &= ZEROKILLVALID; /* zero out kill and valid bits to ready */
				for (i = 0; i < 4; i++)
				{
					VN = cmmu_ptr -> cache.cache_entry[setnum][i].vv;
					VN <<= (12 + (i * 2));
					*data |= VN;
					KN = cmmu_ptr -> cache.cache_entry[setnum][i].kill;
					KN <<= (20 + i);
					*data |= KN;
				}
			}
			else
			{
						/* since the cache has been accessed recompute */
						/* the lru into the CSSP register */
				cmmu_ptr->cache.cache_accessed[setnum] = 0;
						/* fill in KILL and VALID information on the Set */
				*data = 0;
				for (i = 0; i < 4; i++)
				{
					VN = cmmu_ptr -> cache.cache_entry[setnum][i].vv;
					VN <<= (12 + (i * 2));
					*data |= VN;
					KN = cmmu_ptr -> cache.cache_entry[setnum][i].kill;
					KN <<= (20 + i);
					*data |= KN;
				}
				/* fill in LRU information on the set */

				/* first initialize array */
				for(i=0;i<6;i++)
					L[i]=0;

				if (cmmu_ptr -> cache.cache_entry[setnum][0].order <
				    cmmu_ptr -> cache.cache_entry[setnum][1].order)
				L[0] = 1;

				if (cmmu_ptr -> cache.cache_entry[setnum][0].order <
				    cmmu_ptr -> cache.cache_entry[setnum][2].order)
				L[1] = 1;

				if (cmmu_ptr -> cache.cache_entry[setnum][1].order <
				    cmmu_ptr -> cache.cache_entry[setnum][2].order)
				L[2] = 1;

				if (cmmu_ptr -> cache.cache_entry[setnum][0].order <
				    cmmu_ptr -> cache.cache_entry[setnum][3].order)
				L[3] = 1;

				if (cmmu_ptr -> cache.cache_entry[setnum][1].order <
				    cmmu_ptr -> cache.cache_entry[setnum][3].order)
				L[4] = 1;

				if (cmmu_ptr -> cache.cache_entry[setnum][2].order <
				    cmmu_ptr -> cache.cache_entry[setnum][3].order)
				L[5] = 1;

/*
				printf("L[5]=%d L[4]=%d L[3]=%d L[2]=%d L[1]=%d L[0]=%d \n",L[5],L[4],L[3],L[2],L[1],L[0]);
*/
				for (i = 0; i < 6; i++)
				{
					L[i] <<= (24 + i);
					*data |= L[i];
				}
				/* printf("*data = 0x%08x \n",*data); */
				cmmu_ptr->control.CSSP = *data;
			}
			break;

		default: 
			*data = 0x0;
			PPrintf ("error - bad cmmu register offset \n");
			break;
	}
	return (0);
}



/**********************************************************************
*	ctrl_space_write
*/
int ctrl_space_write (unsigned int log_addr,int  storedata)
{
	int     Index, bwp_sbit;
	unsigned int   device_num, oldreg_offset=0, reg_offset;
	unsigned int	 newdevice_num, datawrd;
	unsigned int   setnum;
	struct cmmu *cmmu_ptr;

	/* pull out device number from incoming logical address */

	device_num = (log_addr & 0x000ff000) >> 12;
	device_num &= 0x000000ff;	/* a guard against sign extension error */

	/* pull out register offset from incoming logical address */

	reg_offset = (log_addr & 0x00000fff);

	if (debugflag)
		PPrintf ("device_num = %d    register offset = 0x%04x \n", device_num, reg_offset);

	/* check reg_offset to find out if it is a Cache Control Register Access */

	if ((reg_offset & CCR_OFFSET_MASK) == CCR_OFFSET_MASK)
	{
		oldreg_offset = reg_offset;
		reg_offset &= SELECT_CCR_MASK;
	}

	/* select device (cmmu) */

	if (device_num == Dcmmu_devnum)
		cmmu_ptr = &Dcmmu;
	else if (device_num == Icmmu_devnum)
		cmmu_ptr = &Icmmu;
	else
	{
		PPrintf (" error: incorrect device number <<trapping>>\n");
		return (-1);
	}

	/* select register */

	switch (reg_offset)
	{
		case IDR_ro:		/* cmmu ID register  */
			newdevice_num = storedata & 0xff000000;
			cmmu_ptr->control.IDR = ((cmmu_ptr->control.IDR & 0x00ffffff) | newdevice_num);

			newdevice_num = newdevice_num >> 24;
			newdevice_num &= 0x000000ff;

			if (debugflag)
				PPrintf ("newdevice_num = %d \n", newdevice_num);

			if (cmmu_ptr == &Dcmmu)
			{
				Dcmmu_devnum = newdevice_num;
				if (debugflag)
					PPrintf ("Dcmmu_devnum = 0x%08x \n", Dcmmu_devnum);
			}
			else
				Icmmu_devnum = newdevice_num;
				if (debugflag)
					PPrintf ("Icmmu_devnum = 0x%08x \n", Icmmu_devnum);
			break;

		case SCMR_ro:		/* system command register  */
			cmmu_ptr->control.SCMR = (storedata & 0x0000003f);

			if (debugflag)
				PPrintf (" In write command register \n");

			cmmu_ctrl_func (cmmu_ptr);
			break;

		case SSR_ro: 		/* system status register  */
			cmmu_ptr->control.SSR = (storedata & 0x0000c3ff);
			break;

		case SADR_ro: 		/* system address register  */
			cmmu_ptr->control.SADR = storedata;
			break;

		case SCTR_ro:		/* system control register  */
			cmmu_ptr->control.SCTR = (storedata & 0x0000e000);

			if (debugflag)
				PPrintf (" In write control register \n");

			break;

		case LSR_ro:		/* Local Status register */
			cmmu_ptr->control.LSR = (storedata & 0x00070000);
			break;

		case LADR_ro:		/* Local Address register */
			cmmu_ptr->control.LADR = storedata;
			break;

		case SAPR_ro: 		/* Supervisor area pointer register  */
			cmmu_ptr->control.SAPR = (storedata & 0xffffffc1);
			break;

		case UAPR_ro: 		/* User area pointer register  */
			cmmu_ptr->control.UAPR = (storedata & 0xffffffc1);
			break;

		case BWP0_ro: 		/* batc write port 0  */
		case BWP1_ro: 		/* batc write port 1  */
		case BWP2_ro: 		/* batc write port 2  */
		case BWP3_ro: 		/* batc write port 3  */
		case BWP4_ro: 		/* batc write port 4  */
		case BWP5_ro: 		/* batc write port 5  */
		case BWP6_ro: 		/* batc write port 6  */
		case BWP7_ro: 		/* batc write port 7  */
			Index = (reg_offset & 0x000000ff);
			Index = Index / 4;
			cmmu_ptr->control.BWP[Index] = storedata;
			cmmu_ptr->batc.batc_entry[Index].LBA =
				((cmmu_ptr -> control.BWP[Index] & 0xfff80000) >> 19) & 0x00001fff;
			bwp_sbit = ((cmmu_ptr -> control.BWP[Index] & BWPSBITMASK) >> 5) & 0x1;

			if (bwp_sbit)
				cmmu_ptr -> batc.batc_entry[Index].LBA |= 0x2000;

			cmmu_ptr->batc.batc_entry[Index].PBA =
				((cmmu_ptr -> control.BWP[Index] & 0x0007ffc0) >> 6) & 0x00001fff;
			cmmu_ptr->batc.batc_entry[Index].T =
				(int) (((cmmu_ptr -> control.BWP[Index] & 0x00000010) >> 4) & 0x00000001);
			cmmu_ptr->batc.batc_entry[Index].G =
				(int) (((cmmu_ptr -> control.BWP[Index] & 0x00000008) >> 3) & 0x00000001);
			cmmu_ptr->batc.batc_entry[Index].C =
				(int) (((cmmu_ptr -> control.BWP[Index] & 0x00000004) >> 2) & 0x00000001);
			cmmu_ptr->batc.batc_entry[Index].W =
				(int) (((cmmu_ptr -> control.BWP[Index] & 0x00000002) >> 1) & 0x00000001);
			cmmu_ptr->batc.batc_entry[Index].V =
				(int) (cmmu_ptr -> control.BWP[Index] & 0x00000001);
			break;


		case CACHE_ro: 		/* cache control register */
					/* decode set number from bits 11-4 of SADR */
			setnum = ((cmmu_ptr->control.SADR & SETMASK) >> 4) & 0xff;

					/* decode datawrd position from bits 3 & 2 of SADR */
			datawrd = ((cmmu_ptr -> control.SADR & DATAWRDMASK) >> 2) & 0x3;

					/* retrieve cache line requested */
			switch (oldreg_offset - reg_offset)
			{
				case 0x0: 	/* cache line 0 */
					cmmu_ptr -> cache.cache_entry[setnum][0].data[datawrd] = storedata;
					break;

				case 0x4: 	/* cache line 1 */
					cmmu_ptr -> cache.cache_entry[setnum][1].data[datawrd] = storedata;
					break;

				case 0x8: 	/* cache line 2 */
					cmmu_ptr -> cache.cache_entry[setnum][2].data[datawrd] = storedata;
					break;

				case 0xc: 	/* cache line 3 */
					cmmu_ptr -> cache.cache_entry[setnum][3].data[datawrd] = storedata;
					break;
			}
			break;

		case TAG_ro: 		/* cache tag control access */

					/* decode set number from bits 11-4 of SADR */
			setnum = ((cmmu_ptr -> control.SADR & SETMASK) >> 4) & 0xff;

					/* retrieve cache tag line requested */
			switch (oldreg_offset - reg_offset)
			{
				case 0x0: 	/* cache tag for line 0 */
					cmmu_ptr->cache.cache_entry[setnum][0].tag = (storedata & 0xfffff000);
					break;

				case 0x4: 	/* cache tag for  line 1 */
					cmmu_ptr->cache.cache_entry[setnum][1].tag = (storedata & 0xfffff000);
					break;

				case 0x8: 	/* cache tag for line 2 */
					cmmu_ptr->cache.cache_entry[setnum][2].tag = (storedata & 0xfffff000);
					break;

				case 0xc: 	/* cache tag for line 3 */
					cmmu_ptr->cache.cache_entry[setnum][3].tag = (storedata & 0xfffff000);
					break;
			}
			break;

		case CSSP_ro: 	/* change state of requested cache set */

			cmmu_ptr->control.CSSP = (storedata & CSSPZEROBITS);	/* save input state in control register
										 * CSSP  while masking off zero bits
										 */
			setnum = ((cmmu_ptr->control.SADR & SETMASK) >> 4) & 0xff;	/* decode set number from
											 * bits 11-4 of SADR
											 */
			change_cache_set (cmmu_ptr, setnum, storedata);		/* make changes to requested cache line */
			break;

		default: 
			PPrintf ("error - bad cmmu register offset \n");
			break;
	}
	return (0);
}






/**********************************************************************
*	change_cache_set
*/
int change_cache_set(struct cmmu *cmmu_ptr,unsigned int setnum,int storedata)
{
	int	i, linelru[4];

	/* set vv bits accordingly */

	cmmu_ptr->cache.cache_entry[setnum][0].vv =((storedata & VVLINE0) >> 12) & 0x3;
	cmmu_ptr->cache.cache_entry[setnum][1].vv =((storedata & VVLINE1) >> 14) & 0x3;
	cmmu_ptr->cache.cache_entry[setnum][2].vv =((storedata & VVLINE2) >> 16) & 0x3;
	cmmu_ptr->cache.cache_entry[setnum][3].vv =((storedata & VVLINE3) >> 18) & 0x3;

	/* set kill bits accordingly */

	cmmu_ptr->cache.cache_entry[setnum][0].kill = ((storedata & KILLLINE0) >> 20) & 0x1;
	cmmu_ptr -> cache.cache_entry[setnum][1].kill = ((storedata & KILLLINE1) >> 21) & 0x1;
	cmmu_ptr -> cache.cache_entry[setnum][2].kill = ((storedata & KILLLINE2) >> 22) & 0x1;
	cmmu_ptr -> cache.cache_entry[setnum][3].kill = ((storedata & KILLLINE3) >> 23) & 0x1;

	/* set lru scheme accordingly */

	switch (((storedata & LRUMASK) >> 24) & 0x3f)
	{
	case 0x0: 
		linelru[0] = 3;
		linelru[1] = 2;
		linelru[2] = 1;
		linelru[3] = 0;
		break;

	case 0x1: 
		linelru[0] = 2;
		linelru[1] = 3;
		linelru[2] = 1;
		linelru[3] = 0;
		break;

	case 0x3: 
		linelru[0] = 1;
		linelru[1] = 3;
		linelru[2] = 2;
		linelru[3] = 0;
		break;

	case 0x4: 
		linelru[0] = 3;
		linelru[1] = 1;
		linelru[2] = 2;
		linelru[3] = 0;
		break;

	case 0x6: 
		linelru[0] = 2;
		linelru[1] = 1;
		linelru[2] = 3;
		linelru[3] = 0;
		break;

	case 0x7: 
		linelru[0] = 1;
		linelru[1] = 2;
		linelru[2] = 3;
		linelru[3] = 0;
		break;

	case 0xb: 
		linelru[0] = 0;
		linelru[1] = 3;
		linelru[2] = 2;
		linelru[3] = 1;
		break;

	case 0xf: 
		linelru[0] = 0;
		linelru[1] = 2;
		linelru[2] = 3;
		linelru[3] = 1;
		break;

	case 0x14: 
		linelru[0] = 3;
		linelru[1] = 0;
		linelru[2] = 2;
		linelru[3] = 1;
		break;

	case 0x16: 
		linelru[0] = 2;
		linelru[1] = 0;
		linelru[2] = 3;
		linelru[3] = 1;
		break;

	case 0x1e: 
		linelru[0] = 1;
		linelru[1] = 0;
		linelru[2] = 3;
		linelru[3] = 2;
		break;

	case 0x1f: 
		linelru[0] = 0;
		linelru[1] = 1;
		linelru[2] = 3;
		linelru[3] = 2;
		break;

	case 0x20: 
		linelru[0] = 3;
		linelru[1] = 2;
		linelru[2] = 0;
		linelru[3] = 1;
		break;

	case 0x21: 
		linelru[0] = 2;
		linelru[1] = 3;
		linelru[2] = 0;
		linelru[3] = 1;
		break;

	case 0x29: 
		linelru[0] = 1;
		linelru[1] = 3;
		linelru[2] = 0;
		linelru[3] = 2;
		break;

	case 0x2b: 
		linelru[0] = 0;
		linelru[1] = 3;
		linelru[2] = 1;
		linelru[3] = 2;
		break;

	case 0x30: 
		linelru[0] = 3;
		linelru[1] = 1;
		linelru[2] = 0;
		linelru[3] = 2;
		break;

	case 0x34: 
		linelru[0] = 3;
		linelru[1] = 0;
		linelru[2] = 1;
		linelru[3] = 2;
		break;

	case 0x38: 
		linelru[0] = 2;
		linelru[1] = 1;
		linelru[2] = 0;
		linelru[3] = 3;
		break;

	case 0x39: 
		linelru[0] = 1;
		linelru[1] = 2;
		linelru[2] = 0;
		linelru[3] = 3;
		break;

	case 0x3b: 
		linelru[0] = 0;
		linelru[1] = 2;
		linelru[2] = 1;
		linelru[3] = 3;
		break;

	case 0x3c: 
		linelru[0] = 2;
		linelru[1] = 0;
		linelru[2] = 1;
		linelru[3] = 3;
		break;

	case 0x3e: 
		linelru[0] = 1;
		linelru[1] = 0;
		linelru[2] = 2;
		linelru[3] = 3;
		break;

	case 0x3f: 
		linelru[0] = 0;
		linelru[1] = 1;
		linelru[2] = 2;
		linelru[3] = 3;
		break;
	}

	for (i = 0; i < 4; i++)
		cmmu_ptr -> cache.cache_entry[setnum][i].order = linelru[i];

	redo_set_lru(cmmu_ptr,setnum);

	return 0;
}
