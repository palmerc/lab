/* @(#) File name: dmem.c   Ver: 3.2   Cntl date: 4/4/89 14:48:34 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
#ifdef m88000
#undef m88000
#endif
# include "functions.h"
# include "trans.h"


extern int usecmmu;
extern FILE *fpin, *fplog;
extern	int outputflag;


static	int loc_access;

int ldfrommem(struct IR_FIELDS *ir,int bytes,int signflag)
{
	union MEMDATA memdata;
	int retval;
	register struct SIM_FLAGS *f = &ir->p->flgs;
	unsigned short tempshort;

	memdata.d[0] = memdata.d[1] = 0;
	loc_access++;
	/*printf("loc_access=%d \n",loc_access);*/

	
	if(f->scale_flag && !f->imm_flags)
		m88000.S2bus *= bytes;
	trans.adr2 = trans.adr1 = m88000.S1bus + m88000.S2bus;
	if(f->user && !(PSR & 0x80000000))
		return(exception(E_TPRV, "Supervisor Privlege Violation"));
	
	if(usecmmu)
		Dcmmu.S_U = (f->user || !(PSR & 0x80000000)) ? 0: 1;

	if(bytes == 8)
	{
		if ((trans.adr1 & 7) != 0)
			return(exception(E_TMA, "Misaligned Load Access"));

		if(usecmmu)
		{
			out_to_in(ir, Dcmmu.S_U, 4, 0, 0,trans.adr1);
			retval = load_data (&Dcmmu, &memdata.d[0], &trans.adr1, 0,0);
			out_to_log(ir, Dcmmu.S_U, 4, 0, memdata.d[0], ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 1, 8, 0, &memdata));
				}

			trans.adr1 += 4;
			trans.adr2 += 4;
			out_to_in(ir, Dcmmu.S_U, 4, 0, 0,trans.adr1);
			retval = load_data (&Dcmmu, &memdata.d[1], &trans.adr1, 0,0);
			out_to_log(ir, Dcmmu.S_U, 4, 0, memdata.d[1], ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 1, 4, 0, &memdata));
				}
		}
		else
		{
			if (rdwr ((M_DATA | M_RD), trans.adr1, &memdata.d[0], 4) == -1)
				return(dacc(ir, 1, 8, 0, &memdata));

			trans.adr1 += 4;
			if (rdwr ((M_DATA | M_RD), trans.adr1, &memdata.d[1], 4) == -1)
				return(dacc(ir, 1, 4, 0, &memdata));
		}
	}
	else
	{
		if ((trans.adr1 & (bytes - 1)) != 0)
			return(exception(E_TMA, "Misaligned Load Access"));

		if(usecmmu)
		{
			out_to_in(ir, Dcmmu.S_U, bytes, 0, 0,trans.adr1);
			retval = load_data (&Dcmmu, &memdata.l, &trans.adr1, 0,0);
			out_to_log(ir, Dcmmu.S_U, bytes, 0, memdata.l, ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 1, bytes, signflag, &memdata));
				}

			memdata.l <<= ((trans.adr2 & 0x3) * 8);
		}
		else
		{
			if (rdwr ((M_DATA | M_RD), trans.adr1, &memdata.l, bytes) == -1)
				return(dacc(ir, 1, bytes, signflag, &memdata));
		}
	}
	switch(bytes)
	{
		case 1:
			m88000.ALU = memdata.c;
			if(signflag)
				m88000.ALU |= ((memdata.c & 0x80) ? 0xffffff00: 0);
			break;
		case 2:
			shortswap(&tempshort,&memdata.s);
			m88000.ALU = tempshort;
			if(signflag)
				m88000.ALU |= ((memdata.s & 0x8000) ? 0xffff0000: 0);
			break;
		case 4:
			intswap((unsigned *)&m88000.ALU,&memdata.l);
			break;
		case 8:
			intswap((unsigned *)&m88000.ALU,&memdata.d[0]);
			intswap((unsigned *)&m88000.Regs[(ir->dest + 1) & 0x1f],&memdata.d[1]);
			break;
	}
	return(0);
}


/*
*******************************************************************************
*
*       Module:     sttomem()
*
*       File:       dmem.c
*
*
*******************************************************************************
*
*  Functional Description:
*	sttomem() is used by the "st" instructions to store data into memory
*	or cache memory.
*
*  Globals:
*
*  Parameters:
*
*  Output:
*
*  Revision History:
*
*******************************************************************************/


/* MJP added unsigned to decl below to reduce warnings for SPEC */
unsigned int msktbl[] = { 0x00FFFFFF, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFF00 };

int sttomem(struct IR_FIELDS *ir, int bytes)
{
	union MEMDATA memdata;
	register struct SIM_FLAGS *f = &ir->p->flgs;
	int retval;
	unsigned short tempshort;

	memdata.d[0] = memdata.d[1] = memdata.d[2] = 0;
	loc_access++;
	/*printf("loc_access=%d \n",loc_access);*/

	if(f->scale_flag && !f->imm_flags)
		m88000.S2bus *= bytes;
	trans.adr2 = trans.adr1 = m88000.S1bus + m88000.S2bus;
	if(f->user && !(PSR & 0x80000000))
		return(exception(E_TPRV, "Supervisor Privlege Violation"));
	
	switch(bytes)
	{
		case 1:
			memdata.c = m88000.Regs[ir->dest];
			break;
		case 2:
			tempshort = m88000.Regs[ir->dest];
			shortswap(&memdata.s,&tempshort);
			break;
		case 4:
			intswap(&memdata.l,(unsigned *)&m88000.Regs[ir->dest]);
			break;
		case 8:
			intswap(&memdata.d[0],(unsigned *)&m88000.Regs[ir->dest]);
			intswap(&memdata.d[1],(unsigned *)&m88000.Regs[(ir->dest + 1)  & 0x1f]);
			break;
	}
	if(usecmmu)
		Dcmmu.S_U = (f->user || !(PSR & 0x80000000)) ? 0: 1;

	if(bytes == 8)
	{
		if ((trans.adr1 & 7) != 0)
			return(exception(E_TMA, "Misaligned Load Access"));

		if(usecmmu)
		{
			out_to_in(ir, Dcmmu.S_U, 4, 1, memdata.d[0],trans.adr1);
			retval = load_data (&Dcmmu, &memdata.d[2], &trans.adr1, 1,0);
			out_to_log(ir, Dcmmu.S_U, 4, 1, memdata.d[0], ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 0, 8, 0, &memdata));
				}

			if(store_data (&Dcmmu, trans.adr1, memdata.d[0]))
				return(dacc(ir, 0, 8, 0, &memdata));

			trans.adr1 += 4;
			trans.adr2 += 4;
			out_to_in(ir, Dcmmu.S_U, 4, 1, memdata.d[1],trans.adr1);
			retval = load_data (&Dcmmu, &memdata.d[2], &trans.adr1, 1,0);
			out_to_log(ir, Dcmmu.S_U, 4, 1, memdata.d[1], ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 0, 4, 0,(union MEMDATA *) &memdata.d[1]));
				}

			if(store_data (&Dcmmu, trans.adr1, memdata.d[1]))
				return(dacc(ir, 0, 4, 0,(union MEMDATA *) &memdata.d[1]));
		}
		else
		{
			if (rdwr ((M_DATA | M_WR), trans.adr1, &memdata.d[0], 4) == -1)
				return(dacc(ir, 0, 8, 0, &memdata));

			trans.adr1 += 4;
			if (rdwr ((M_DATA | M_WR), trans.adr1, &memdata.d[1], 4) == -1)
				return(dacc(ir, 0, 4, 0,(union MEMDATA *) &memdata.d[1]));
		}
	}
	else
	{
		if ((trans.adr1 & (bytes - 1)) != 0)
			return(exception(E_TMA, "Misaligned Load Access"));

		if(usecmmu)
		{
			if(bytes != 4)
				memdata.l >>= ((trans.adr2 & 0x3) * 8);

			out_to_in(ir, Dcmmu.S_U, bytes, 1, memdata.l,trans.adr1);
			retval = load_data (&Dcmmu, &memdata.d[1], &trans.adr1, 1,0);
			out_to_log(ir, Dcmmu.S_U, bytes, 1, memdata.l, ((retval == -1) ? 1: 0),trans.adr2);
			if(retval == -1)
				{
				out_to_in(ir,Dcmmu.S_U,0,0,0,0);
				out_to_log(ir, Dcmmu.S_U,0,0,0,1,0);
				return(dacc(ir, 1, bytes, 0, &memdata));
				}

			switch(bytes)
			{
				case 2:
					memdata.d[1] &= msktbl[(trans.adr2 & 0x2)];
					memdata.l |= (memdata.d[1] & msktbl[(trans.adr2 & 0x2) + 1]);
					break;
				case 1:
					memdata.l |= (memdata.d[1] & msktbl[trans.adr2 & 0x3]);
					break;
				default:
					break;
			}
			if(store_data (&Dcmmu, (trans.adr1 & ~0x3), memdata.l))
				return(dacc(ir, 0, bytes, 0, &memdata));
		}
		else
		{
			if (rdwr ((M_DATA | M_WR), trans.adr1, &memdata.l, bytes) == -1)
				return(dacc(ir, 0, bytes, 0, &memdata));
		}
	}
	return(0);
}

int bytestrobes[2][4] = {{ 0x04, 0x08, 0x10, 0x20 },
			 { 0x44, 0x48, 0x50, 0x60 }};

int halfstrobes[2][2] = {{ 0x0c, 0x30 },
			 { 0x4c, 0x70 }};

int dacc(struct IR_FIELDS *ir,int rdwr,int signflag,
int size,union MEMDATA *data)
{
	register int temp;

	m88000.SFU0_regs[11] = 0;
	m88000.SFU0_regs[14] = 0;
	PSR |= 0x08000000;
	temp = (!(PSR & 0x8000)) ? 0: 0x8000;			/* get byte ordering bit */
	temp |= (ir->p->flgs.user || !(PSR & 0x80000000)) ? 0: 0x4000;	/* get super bit */
	temp |= (rdwr << 1);					/* get rdwr */
	switch(size)
	{
		case 8:
			m88000.SFU0_regs[11] = temp | 0x3d | (((ir->dest + 1) & 0x1f) << 7);
			m88000.SFU0_regs[12] = data->d[1];
			m88000.SFU0_regs[13] = (trans.adr1 & 0xFFFFFFFC) + 4;
			temp |= 0x00002000;
		case 4:
			temp |= 0x3c;
			m88000.SFU0_regs[9] = data->l;
			break;
		case 2:
			temp |= halfstrobes[signflag][trans.adr1 & 0x02];
			m88000.SFU0_regs[9] = data->s;
			break;

		case 1:
			temp |= bytestrobes[signflag][trans.adr1 & 0x03];
			m88000.SFU0_regs[9] = data->c;
			break;
	}
	temp |= (ir->dest << 7) | 1;		/* get dest reg and set valid bit*/
	m88000.SFU0_regs[10] = trans.adr1 & 0xFFFFFFFC;
	if((ir->op >= (unsigned)XMEMBU) && (ir->op <= (unsigned)XMEM))
	{
		temp |= 0x1000;
		if(rdwr)
		{
			m88000.SFU0_regs[11] = temp & 0xFFFFFFFD;
			m88000.SFU0_regs[12] = m88000.Regs[ir->dest];
			m88000.SFU0_regs[13] = m88000.SFU0_regs[10];
		}
	}
	m88000.SFU0_regs[8] = temp;
	return(exception(E_TDACC, "Data Access Bus Error"));
}

char *charstrb[] = { "0 0 0 1", "0 0 1 0", "0 1 0 0", "1 0 0 0" };
char *halfstrb[] = { "0 0 1 1", "", "1 1 0 0", "" };
char *wordstrb[] = { "1 1 1 1" };
char *noopstrb[] = { "0 0 0 0" };

void out_to_in(struct IR_FIELDS *ir,int s_u,int bytes,int rdwr,
unsigned int data,unsigned int addr)
{
	char type, **strobes;
	unsigned int datain=0;

	if(!outputflag)
		return;
	switch(bytes)
	{
		case 0:
			if(rdwr)
				datain = data;
			strobes = noopstrb;
			break;
		case 1:
			if(rdwr)
				datain = (data >> ((3-(addr & 0x3)) * 8));
			strobes = charstrb;
			break;

		case 2:
			if(rdwr)
				datain = (data >> ((2-(addr & 0x3)) * 8));
			strobes = halfstrb;
			break;

		default:
			if(rdwr)
				datain = data;
			strobes = wordstrb;
			break;
	}
	type = ((ir->op >= (unsigned)XMEMBU) && (ir->op <= (unsigned)XMEM)) ? 'L': 'l';
	
	if(rdwr)
	{
		fprintf(fpin, " W %c %c %s %08X %08X \n",
				(s_u) ? 'S': 'U', type,
				strobes[addr & 0x03],
				addr, datain);
	}
	else
	{
		fprintf(fpin, " R %c %c %s %08X \n",
				(s_u) ? 'S': 'U', type,
				strobes[addr & 0x03],
				addr);
	}
}


char *charstrb1[] = { "0001", "0010", "0100", "1000" };
char *halfstrb1[] = { "0011", "", "1100", "" };
char *wordstrb1[] = { "1111" };
char *noopstrb1[] = { "0000" };

void out_to_log(struct IR_FIELDS *ir,int s_u,int bytes,int rdwr,
unsigned int data,unsigned int reply,unsigned int addr)
{
      char **strobes;
      int type;
      unsigned int datalog;

      if(!outputflag)
              return;
      switch(bytes)
      {
              case 0:
                      strobes = noopstrb1;
                      datalog = data;
					  loc_access++;   /* increment access even for noop access */
					  /*printf("loc_access=%d \n",loc_access);*/
                      break;
              case 1:
                      if (rdwr)
                              datalog = (data >> ((3-(addr & 0x3)) * 8));
                      else
                              datalog = (((data & 0xff)<<24) | ((data & 0xff00)<<8) | ((data & 0xff0000)>>8) | ((data & 0xff000000)>>24));
                      strobes = charstrb1;
                      break;

              case 2:
                      if (rdwr)
                              datalog = (data >> ((2-(addr & 0x3)) * 8));
                      else
                              datalog = (((data & 0xff)<<24) | ((data & 0xff00)<<8) | ((data & 0xff0000)>>8) | ((data & 0xff000000)>>24));
                      strobes = halfstrb1;
                      break;

              default:
                      strobes = wordstrb1;
                      datalog = data;
                      break;
      }

      type = ((ir->op >= (unsigned)XMEMBU) && (ir->op <= (unsigned)XMEM)) ? 1: 0;

      if(rdwr)
      {
              fprintf(fplog,"             NEW CPU WRITE    ACCESS %4d  s/u=%d l=%d bms(3210)=%s a=%08x\n",
                              loc_access, s_u, type,
                              strobes[addr & 0x03],
                              addr);
              fprintf(fplog,"             DATA BUS WRITE:  ACCESS %4d   %8.8x\n", loc_access, datalog);
			  if ((reply == 1) && (bytes == 0))
              		fprintf(fplog,"             REPLY = %s    ACCESS %4d (IGNORED)\n\n","FAULT",loc_access);
			  else
              		fprintf(fplog,"             REPLY = %s    ACCESS %4d\n\n",(reply) ? "FAULT":"OK   ",loc_access);
      }
      else
      {
              fprintf(fplog,"             NEW CPU READ     ACCESS %4d  s/u=%d l=%d bms(3210)=%s a=%08x\n",
                              loc_access, s_u, type,
                              strobes[addr & 0x03],
                              addr);
              fprintf(fplog,"             DATA BUS READ:   ACCESS %4d   %8.8x\n", loc_access, datalog);
			  if ((reply == 1) && (bytes == 0))
              	fprintf(fplog,"             REPLY = %s    ACCESS %4d (IGNORED)\n\n","FAULT",loc_access);
			  else
               	fprintf(fplog,"             REPLY = %s    ACCESS %4d\n\n",(reply) ? "FAULT":"OK   ",loc_access);
      }
}
 
