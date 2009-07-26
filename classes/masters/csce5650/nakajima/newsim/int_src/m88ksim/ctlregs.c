/* @(#) File name: ctlregs.c   Ver: 3.1   Cntl date: 1/20/89 14:41:25 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*
*/
#ifdef m88000
#undef m88000
#endif

#include "functions.h"


	/**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/



	/**********************************************************
	*
	*       External Variables Definition
	*
	*/


	/**********************************************************
	*
	*       Local Defines
	*
	*/

struct SFUbits {
	unsigned int regnum;
	unsigned int write;
	unsigned int zeros;
	unsigned int ones;
	unsigned int reset;
};

	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/

struct SFUbits SFU0bits[] =
{
	{  0,	0,	0x00000000,	0x00000000,	0x00000000	},
	{  1,	1,	0xF80003FF,	0x000003F0,	0x800003FF	},
	{  2,	1,	0xF80003FF,	0x000003F0,	0x800003FF	},
	{  3,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  4,	0,	0x00000000,	0x00000000,	0x00000000	},
	{  5,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  6,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  7,	1,	0xFFFFF000,	0x00000000,	0x00000000	},
	{  8,	0,	0x00000000,	0x00000000,	0x00000000	},
	{  9,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 10,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 11,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 12,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 13,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 14,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 15,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 16,	0,	0x00000000,	0x00000000,	0x00000000	},
	{ 17,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{ 18,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{ 19,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{ 20,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{ 0xFFFFFFFF,	1,	0x00000000,	0x00000000,	0x00000000	}
};

struct SFUbits SFU1bits[] =
{
	{  0,	1,	0x000000FF,	0x00000000,	0x00000000	},
	{  1,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  2,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  3,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  4,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  5,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  6,	1,	0xFE1FFFFF,	0x00000000,	0x00000000	},
	{  7,	1,	0xFFFFFFFF,	0x00000000,	0x00000000	},
	{  8,	1,	0xFFF0FFFF,	0x00000000,	0x00000000	},
	{ 62,	1,	0x0000001F,	0x00000000,	0x00000000	},
	{ 63,	1,	0x0000FFFF,	0x00000000,	0x00000000	},
	{ 0xFFFFFFFF,	1,	0x00000000,	0x00000000,	0x00000000	}
};

	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      wrctlreg()
	*/


int wrctlregs(int sfunum,int regnum,int value,int ctlwr)
{
	int j;
	int *SFUptr = (sfunum) ? m88000.SFU1_regs: m88000.SFU0_regs;
	struct SFUbits *SFUbptr = (sfunum) ? SFU1bits: SFU0bits;

	for(j = 0; SFUbptr[j].regnum != 0xFFFFFFFF; j++)
		if(SFUbptr[j].regnum == regnum)
			break;

	if((ctlwr == 1) || (SFUbptr[j].write && (ctlwr == 2)))
		SFUptr[regnum] = (value & SFUbptr[j].zeros) | SFUbptr[j].ones;

	return(SFUptr[regnum]);
}
	


void init_processor(void)
{
	int i, j;

	for(i = 0; i < 64; i++)
	{
		for(j = 0; SFU0bits[j].regnum != 0xFFFFFFFF; j++)
			if(SFU0bits[j].regnum == i)
				break;

		m88000.SFU0_regs[i] = SFU0bits[j].reset;
	}

	for(i = 0; i < 64; i++)
	{
		for(j = 0; SFU1bits[j].regnum != 0xFFFFFFFF; j++)
			if(SFU1bits[j].regnum == i)
				break;

		m88000.SFU1_regs[i] = SFU1bits[j].reset;
	}
}
