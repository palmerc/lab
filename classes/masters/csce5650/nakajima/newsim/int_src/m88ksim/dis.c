/* @(#) File name: dis.c   Ver: 3.3   Cntl date: 3/10/89 10:37:37 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

/*****************************************************************************
*
*
*
*/
#ifdef m88000
#undef m88000
#endif



        /**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/
#include "functions.h"


	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	int   sym_addr;

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

static char	disbuf[100] = { "\0" };
static char	mnemonic[15] = { "\0" };
static char	operands[50] = { "\0" };
int	memscaleu[] = { 1, 4, 2, 1 };
int	memscale[] = { 8, 4, 2, 1 };
union	opcode opcode;
static int dest, src1, src2;
static unsigned int Rsrc1, Rsrc2, Disaddr;


	/**********************************************************
	*
	*      Messages
	*/

char null[] = { "" };
char *memnames[] = { "xmem", "ld", "st", "lda" };
char *memuser[] = { null, ".usr" };
char *memszu[] = { ".bu", null, ".hu", ".bu" };
char *memsz[] = { ".d", null, ".h", ".b" };
char *lognames[] = { "and", "mask", "xor", "or" };
char *intnames[] = { "addu", "subu", "divu", "mul", "add", "sub", "div", "cmp" };
char *intsuffix[] = { null, ".co", ".ci", ".cio" };
char *bitnames[] = { "clr", "set", "ext", "extu", "mak", "rot", NULL, NULL };
char *trpnames[] = { NULL, NULL, "tb0", "tb1", NULL, "tcnd" };
char *xfrnames[] = { "br", "bsr", "bb0", "bb1" };
char *ctlnames[] = { NULL, "ldcr", "stcr", "xcr" };
char *sfu1names[] = { "fmul", 0, 0, 0, "flt", "fadd", "fsub", "fcmp",
		      0, "int", "nint", "trnc", 0, 0, "fdiv", "fsrt",
		      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char *tynames[] = { null, "(eq)", "(ne)", "(gt)", "(le)", "(lt)",
		    "(ge)", "(hi)", "(ls)", "(lo)", "(hs)" };
char *bitpats[] = { null, "never", "gt0", "eq0", "ge0", "0100", "0101", "0110", "0111",
		    "1000", "1001", "1010", "1011", "lt0", "ne0", "le0", "always" };

	/**********************************************************
	*
	*      dis()
	*/

/*****************************************************************************
*	Modified:
*	Reduced the size of the operand field to 30 characters to allow for
*	mm x;di assembly to occur on one line.
*****************************************************************************/

char *dis(unsigned int opc,unsigned int  disaddr)
{
	int retvalue;

	opcode.opc = opc;
	dest = opcode.rrr.dest;
	src1 = opcode.rrr.src1;
	src2 = opcode.rrr.src2;
	Rsrc1 = m88000.Regs[src1];
	Rsrc2 = m88000.Regs[src2];
	Disaddr = disaddr;

	switch(opcode.rrr.maj)
	{
		case	0:
		case	1:
			retvalue = memi();
			break;

		case	2:
			retvalue = logi();
			break;

		case	3:
			retvalue = inti();
			break;

		case	4:
			retvalue = sfu();
			break;

		case	6:
			retvalue = xfr();
			break;

		case	7:
			retvalue = rrr();
			break;

		default:
			retvalue = -1;
			break;
	}
	if(retvalue)
	{
		sprintf(mnemonic, "word");
		sprintf(operands, "$%08X", opcode.opc);
	}
	sprintf(disbuf, "%-10.9s%-30.29s", mnemonic, operands);
	return(disbuf);
}


int memi(void)
{
	char	*memsize = memsz[opcode.memi.ty];

	switch(opcode.memi.p)
	{
		case	0:
			if(opcode.memi.ty & 0x2)
				opcode.memi.p = 1;
			memsize = memszu[opcode.memi.ty];
			break;

		case	1:
		case	2:
			break;

		case	3:
			return(-1);
	}

	sprintf(mnemonic, "%s%s", memnames[opcode.memi.p], memsize);

	if (sym_addr)
		sprintf(operands, "r%d,r%d,$%04X (%s)", dest, src1, opcode.imm16.imm16, 
			symbol(M_DATA, Rsrc1 + opcode.imm16.imm16));
	else
		sprintf(operands, "r%d,r%d,$%04X", dest, src1, opcode.imm16.imm16);

	return(0);
}


int imm16(void)
{
	sprintf(operands, "r%d,r%d,$%04X", dest, src1, opcode.imm16.imm16);
	return(0);
}

int logi(void)
{
	sprintf(mnemonic, "%s%s", lognames[opcode.imm16.min >> 1], ((opcode.imm16.min & 0x1) ? ".u": null));
	return(imm16());
}

int inti(void)
{
	sprintf(mnemonic, intnames[opcode.imm16.min]);
	return(imm16());
}

int sfu(void)
{
	switch(opcode.rrr.min)
	{
		case	0:
			return(ctl());

		case	1:
			return(sfu1());

		case	2:
		case	3:
		case	4:
		case	5:
		case	6:
		case	7:
			break;
	}
	return(-1);
}

int xfr(void)
{
	int immvalue;

	sprintf(mnemonic, "%s%s", xfrnames[opcode.imm16.min >> 1], ((opcode.imm16.min & 0x1) ? ".n": null));

	if(opcode.imm16.min & 0x4)
	{
		immvalue = ((opcode.imm16.imm16 & 0x8000) ? 0xffff0000: 0) | opcode.imm16.imm16;
		if (sym_addr)
			sprintf(operands, "%d,r%d,%s %s", dest, src1, symbol(M_INSTR, Disaddr + (immvalue << 2)),
				nameofbit(dest));
		else
			sprintf(operands, "%d,r%d,$%08X", dest, src1, (Disaddr + (immvalue << 2)));
	}
	else
	{
		immvalue = ((opcode.imm26.imm26 & 0x02000000) ? 0xfc000000: 0) | opcode.imm26.imm26;
		if (sym_addr)
			sprintf(operands, "%s", symbol(M_INSTR, Disaddr + (immvalue << 2)));
		else
			sprintf(operands, "$%08X", Disaddr + (immvalue << 2));
	}

	return(0);
}

int rrr(void)
{
	int immvalue;

	switch(opcode.rrr.min)
	{
		case	2:
		case	3:
			sprintf(mnemonic, "%s%s", "bcnd", ((opcode.imm16.min & 0x1) ? ".n": null));
			immvalue = ((opcode.imm16.imm16 & 0x8000) ? 0xffff0000: 0) | opcode.imm16.imm16;

			if (sym_addr)
				sprintf(operands, "%d,r%d,%s (%s)", dest, src1,
					symbol(M_INSTR, Disaddr + (immvalue << 2)), cndtype(dest));
			else
				sprintf(operands, "%s,r%d,$%08X", cndtype(dest), src1, Disaddr + (immvalue << 2));
			return(0);

		case	4:
			return(gen1());

		case	5:
			return(gen2());

		case	6:
			if(dest)
				return(-1);
			sprintf(mnemonic, "tbnd");
			sprintf(operands, "r%d,$%04X", src1, opcode.imm16.imm16);
			return(0);

		default:
			break;
	}
	return(-1);
}

int ctl(void)
{
	char modifier;

	modifier = "\0f######"[opcode.ctl.sfu];
	if((opcode.ctl.opc == 0) || (modifier == '#'))
		return(-1);
	
	if(modifier)
		sprintf(mnemonic, "%c%s", modifier, ctlnames[opcode.ctl.opc]);
	else
		sprintf(mnemonic, ctlnames[opcode.ctl.opc]);

	switch(opcode.ctl.opc)
	{
		case	1:
			if(src1 || src2)
				return(-1);
			sprintf(operands, "r%d,cr%d", dest, opcode.ctl.crsd);
			break;

		case	2:
			if(dest)
				return(-1);
			sprintf(operands, "r%d,cr%d", src1, opcode.ctl.crsd);
			break;

		case	3:
			sprintf(operands, "r%d,r%d,cr%d", dest, src1, opcode.ctl.crsd);
			break;
	}
	return(0);
}

int sfu1(void)
{
	char	td, t1, t2;

	td = "sd##"[opcode.sfu1.td];			/* td */
	t1 = "sd##"[opcode.sfu1.t1];			/* t1 */
	t2 = "sd##"[opcode.sfu1.t2];			/* t2 */

	if((td == '#') || (t1 == '#') || (t2 == '#') 
	   || (sfu1names[opcode.sfu1.opc] == 0))
		return(-1);

	switch(opcode.sfu1.opc)
	{
		case	0x7:
			if(td != 's')
				return(-1);
		case	0x0:
		case	0x5:
		case	0x6:
		case	0xe:
			sprintf(mnemonic, "%s.%c%c%c", sfu1names[opcode.sfu1.opc], td, t1, t2);
			sprintf(operands, "r%d,r%d,r%d", dest, src1, src2);
			return(0);

		case	0x4:
			if(t2 != 's')
				return(-1);
			break;

		case	0x9:
		case	0xa:
		case	0xb:
			if(td != 's')
				return(-1);
			break;

		default:
			return(-1);
	}
	if(src1 || (t1 != 's'))
		return(-1);
	sprintf(mnemonic, "%s.%c%c", sfu1names[opcode.sfu1.opc], td, t2);
	sprintf(operands, "r%d,r%d", dest, src2);
	return(0);
}

int gen1(void)
{
	union rrropc opc;

	opc.opc = opcode.trpbfi.opc;

	if(opc.bfi.zero)
		return(-1);

	switch(opc.bfi.maj)
	{
		case	2:
			if(!bitnames[opc.bfi.opc])
				return(-1);
			sprintf(mnemonic, bitnames[opc.bfi.opc]);
			sprintf(operands, "r%d,r%d,%d<%d>", dest, src1, opc.bfi.w, opc.bfi.o);
			return(0);
			
		case	3:

			if(!trpnames[opc.trp.opc])
			{
				printf("gen1: !trpnames[opc.trp.opc]\n");
				return(-1);
			}

			sprintf(mnemonic, trpnames[opc.trp.opc]);
			sprintf(operands, "%d,r%d,$%03X %s", dest, src1, opc.trp.vec,
				(opc.trp.opc == 5) ? cndtype(dest): nameofbit(dest));
			return(0);

		default:
			break;
	}
	return(-1);
}

int gen2(void)
{
	char	*ptr;
	int	scale;
	union rrropc opc;

	opc.opc = opcode.rrr.opc;

	switch(opc.gen.maj)
	{
		case	0:				/* ld.xx & xmem.xx */
		case	1:				/* st.xx & lda.xx */
			ptr = memsz[opc.mem.ty];
			scale = memscale[opc.mem.ty];
			switch(opc.mem.p)
			{
				case	0:
					if((opc.mem.idx) && ((opc.mem.ty == 3) || (opc.mem.ty == 0)))
						return(-1);
					if(opc.mem.ty & 0x2)
						opc.mem.p = 1;
					ptr = memszu[opc.mem.ty];
					scale = memscaleu[opc.mem.ty];
					break;
		
				case	1:
				case	2:
					if((opc.mem.idx) && (opc.mem.ty == 3))
						return(-1);
					break;

				case	3:
					if((!opc.mem.idx) || (opc.mem.user))
						return(-1);
					break;
			}
			sprintf(mnemonic, "%s%s%s", memnames[opc.mem.p], ptr, memuser[opc.mem.user]);
			return(rrrmdisp(scale));

		case	2:
			if((opc.log.zero) || (opc.log.opc == 1))
				return(-1);
			sprintf(mnemonic, "%s%s", lognames[opc.log.opc], ((opc.log.cp) ? ".c": null));
			rrrdisp();
			return 0;

		case	3:
			if(opc.intg.zero)
				return(-1);
			switch(opc.intg.opc)
			{
				case	2:
				case	3:
				case	6:
				case	7:
					if((opc.intg.cbin) || (opc.intg.cbout))
						return(-1);
					ptr = "";
					break;

				default:
					ptr = intsuffix[(opc.intg.cbin << 1) + opc.intg.cbout];
					break;
			}
			sprintf(mnemonic, "%s%s", intnames[opc.intg.opc], ptr);
			rrrdisp();
			return 0;

		case	4:
		case	5:
			if(opc.bf.zero || !bitnames[opc.bf.opc])
				return(-1);
			sprintf(mnemonic, bitnames[opc.bf.opc]);
			rrrdisp();
			return 0;

		case	6:
			if(opcode.xfra.zero1 || opcode.xfra.zero2)
				return(-1);
			sprintf(mnemonic, "%s%s", (opcode.xfra.opc) ? "jsr": "jmp", (opcode.xfra.nxt) ? ".n": null);

			if(sym_addr)
				sprintf(operands, "r%d (%s)", src2, symbol(M_INSTR, Rsrc2));
			else
				sprintf(operands, "r%d", src2);

			return(0);

		case	7:
			if(opc.gen.zero)
				return(-1);
			switch(opc.gen.min)
			{
				case	2:
				case	3:
					if(src1)
						return(-1);
					sprintf(mnemonic, (opc.log.cp) ? "ff0": "ff1");
					sprintf(operands, "r%d,r%d", dest, src2);
					return(0);

				case	6:
					if(dest)
						return(-1);
					sprintf(mnemonic, "tbnd");
					sprintf(operands, "r%d,r%d", src1, src2);
					return(0);
					
				case	7:
					if(opcode.xfra.zero1 || src2)
						return(-1);
					sprintf(mnemonic, "rte");
					return(operands[0] = '\0');

				default:
					break;
			}
		default:
			break;
	}
	return(-1);
}


void rrrdisp(void)
{
	sprintf(operands, "r%d,r%d,r%d", dest, src1, src2);
	return;
}

int rrrmdisp(int scale)
{
	union rrropc opc;

	opc.opc = opcode.rrr.opc;
	if(opc.mem.zero)
		return(-1);

	if(!opc.mem.idx)
	{
		if (sym_addr)
			sprintf(operands, "r%d,r%d,r%d (%s)", dest, src1, src2,
				symbol(M_DATA, Rsrc1 + Rsrc2));
		else
			sprintf(operands, "r%d,r%d,r%d", dest, src1, src2);
	}
	else
	{
		if (sym_addr)
			sprintf(operands, "r%d,r%d[r%d] (%s)", dest, src1, src2,
				symbol(M_DATA, Rsrc1 + (Rsrc2 * scale)));
		else
			sprintf(operands, "r%d,r%d[r%d]", dest, src1, src2);
	}

	return(0);
}

char *nameofbit(int code)
{
	if((code >= 2) && (code <= 11))
		return(tynames[code-1]);

	return(tynames[0]);
}

char *cndtype(int code)
{
	if(code < 16)
		return(bitpats[code+1]);

	return(bitpats[0]);
}
