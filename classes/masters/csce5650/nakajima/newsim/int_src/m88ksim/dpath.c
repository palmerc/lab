/* @(#) File name: dpath.c   Ver: 3.5   Cntl date: 4/4/89 14:51:58 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
#ifdef m88000
#undef m88000
#endif


# include "functions.h"
# include "float.h"
# include "trans.h"
# define CARRYMASK  0x10000000

/*
 * The Processor structure, found in sim.h is where the registers, and
 * word are defined.
 */

extern struct mem_wrd  *getmemptr ();
extern struct cmmu Icmmu, Dcmmu;
extern INSTAB simdata;
extern char null[];
extern int usecmmu;
extern FILE *fpin, *fplog, *pipe_strm;
extern int instr_cnt;
extern int debugflag;
extern int vecyes;
extern int pipe_open;
extern struct IR_FIELDS  *fp_err_ir;


int cmmutime = 0;
int Dcmmutime = 0;
int mbus_latency = 0;
int mbus_arbcnt;
int pbus_arbcnt;
int prev_extime;
int outputflag = 0;
int trace_flg = 0;


/*
*******************************************************************************
*
*       Module:     Data_path()
*
*       File:       dpath.c
*
*
*******************************************************************************
*
*  Functional Description:
*	This is the source file for the M88000 data path simulator.
*	It is the heart of the simulator, performing the actual instruction
*	execution for all instructions that don't modify the instruction
*	pointer.
*
*	The main_mem structure, also found in sim.h is where the memory is
*	defined.  Memory is an array with a fixed length (fixed via a define
*	in sim.h).  The array is structured as an array of 32 bit words, 
*	with word addresses.
*
*
*  Globals:
*
*  Parameters:
*
*  Output:
*	returns -1	If there is an error condition or breakpoint and
*			program execution must stop.
*	returns 0	If everything is OK.
*
*  Revision History:
*
*
*******************************************************************************/

int Data_path (void)
{
	register struct mem_wrd *memaddr;
	register struct IR_FIELDS   *ir;
	register struct SIM_FLAGS   *f;
	int     retval,retval_cmmu;				/* return value */
	unsigned int	issue_latency = 0;	/* max time to issue instr(s) */


	if (ckbrkpts(IP, BRK_EXEC))		/* Check for breakpoint */
	{
		retval = -1;
		return(retval);
	}

	retval_cmmu = 0;
	if(usecmmu && (retval = cmmu_function1()))
		return(retval);

	if ((memaddr = getmemptr (IP, M_INSTR)) == 0)
		return (exception(E_TCACC, "Code Access Bus Error"));

	ir = &memaddr -> opcode;
	f = ((ir->p) ? &ir->p->flgs: &simdata.flgs);


	/* see if we can issue the instruction in this clock */

	if ( (retval = test_issue( ir, f )) == -1 )
	{
		return ( retval );
	}
	else
	{

		Statistics (ir);

		/* Issue the instruction to the appropriate FU */

		do_issue();
		issue_latency = f->is_latency;
		issue_latency += retval;
		prev_extime = issue_latency;

		if(debugflag)
			PPrintf(" after chk_SB : Dcmmutime = %d \n", Dcmmutime);

		if (usecmmu)
			cmmu_function2(ir);

		/*
		 * The data from the source 1 register is put onto the 
		 * source 1 bus, as an input to the ALU.
		 */

		m88000.S1bus = m88000.Regs[ir -> src1];

		/*
		 * The data from the source 2 register, or an immediate 
		 * value, is put on the source 2 bus, which is also an 
		 * input to the ALU.
		 */

		if (!f -> imm_flags)	/* if not immediate */
			m88000.S2bus = m88000.Regs[ir -> src2];
		else if (f -> imm_flags == i26bit)
			m88000.S2bus = sext (opword (IP), 0, 26);
		else if ((f -> imm_flags == i16bit) &&
				((ir -> op < (unsigned)JSR) || (ir -> op > (unsigned)BCND)))
			m88000.S2bus = uext (opword (IP), 0, 16);
		else if ((f -> imm_flags == i16bit) &&
				((ir -> op >= (unsigned)JSR) && (ir -> op <= (unsigned)BCND)))
			m88000.S2bus = sext (opword (IP), 0, 16);
		else if (f -> imm_flags == i10bit)
			m88000.S2bus = uext (opword (IP), 0, 10);
		else
		{
			Eprintf ("SYSTEM ERROR in dpath, funky sized immediate\n");
			return(-1);
		}

		/*
		 * The instruction has been issued and the busses have 
		 * been driven.  Now execute the instruction.
		 */

		if( retval = execute(ir, f, memaddr) ) return(retval);

   		if (usecmmu && debugflag)
			PPrintf(" Dcmmutime (total after store) = %d \n",Dcmmutime);

		/*
		 *	Adjust the program counter
		 */

		killtime ( issue_latency );
		if ( retval = Pc(memaddr, ir, f) )
			return ( retval );

	}


	return ( retval );
}


union wrdsign {
	int w;
#ifdef LEHOST
	struct { unsigned int w:31, s:1; } sw;
#else
	struct { unsigned int s:1, w:31; } sw;
#endif
};

int addunsigned(unsigned int l1,unsigned int l2,struct IR_FIELDS *ir,
int sub,int overflow)
{
	union wrdsign temp;
	union wrdsign s1, s2;
	int carryin = 0;
	int carryout = 0;

	s1.w = l1;
	s2.w = l2;

	switch(ir->op)
	{
		case ADDCIO:
		case ADDUCIO:
			carryout = 1;

		case ADDCI:
		case ADDUCI:
			if ((PSR & CARRYMASK) != 0)
				carryin = 1;
			break;

		case ADDCO:
		case ADDUCO:
			carryout = 1;

		default:	/* case ADDU: and case ADD: */
			if(sub)
				carryin = 1;
			break;
	}

	if (sub)		/* if subtract */
		s2.w = ~s2.w;

	temp.w = m88000.ALU = s1.w + s2.w + carryin;

/***********************************************************************
*
*	ADD:
*	   overflow = (s1.sign * s2.sign * !ans.sign)
*		      + (!s1.sign * !s2.sign * ans.sign)
*
*	SUB:
*	   overflow = (!s1.sign * !s2.sign * ans.sign)
*		      + (s1.sign * s2.sign * !ans.sign)
*
***********************************************************************/

	if(overflow)
	{
		if((s1.sw.s && s2.sw.s && !temp.sw.s)
		   || (!s1.sw.s && !s2.sw.s && temp.sw.s))
			return(exception(E_TIOV, "Signed Overflow Exception"));
	}

	if(carryout)
	{
                if ((s1.sw.s && s2.sw.s) || (!temp.sw.s && (s1.sw.s || s2.sw.s)))
			PSR |= CARRYMASK;	/* set carry/borrow bit of psr */
		else
			PSR &= ~CARRYMASK;	/* clear carry/borrow bit of psr */
	}

	return(0);
}


/**** sext *******************************************************************
*	Implements the Extract signed bit field instruction
*****************************************************************************/
int sext(int src,int off,int wid)
{
	int res;

	if (wid == 0)
		wid = 32;

	if ((off+wid) < 32)
		res = ((int)src <<  (32-(off+wid))) >> (32-wid);
	else
		res = (int)src >> (off);

	return(res);
}


/**** uext *******************************************************************
*	Implements the Extract unsigned bit field instruction
*****************************************************************************/
int uext(int src,int off,int wid)
{
	int res;

	if (wid == 0)
		wid = 32;

	if ((off+wid) < 32)
		res = (((unsigned int)src >> (off)) & ((1<<wid) - 1));
	else
		res = (((unsigned int)src >> (off)) & ((1<<(32-off)) - 1));

	return(res);
}


/**** make *******************************************************************
*	Implements the Make bit field instruction
*****************************************************************************/
int make(int src,int off,int wid)
{
	int res;

	if ((wid == 0) || (wid == 32))
		res = (((unsigned int)src) << off);
	else
		res = (((unsigned int)src & ((1 << wid) - 1)) << off);

	return(res);
}

int cmmu_function1(void)
{
	int     mbuswait;
	unsigned int	temp;

	cmmutime = 0;
	Icmmu.S_U = (PSR & 0x80000000) ? 1: 0;

	if(debugflag)
		PPrintf("mbus latency(before this fetch)=%d \n", mbus_latency);

	if(load_data (&Icmmu, &temp, &IP, 0, 0) == -1)
		return (exception(E_TCACC, "Code Access Bus Error"));

	if (mbus_latency == 0)
		mbuswait = 0;
	else
		mbuswait = 1;
 
	/*
	** kill mbus latency iff cache miss (where cmmutime != 0)
	*/

	if ((mbus_latency != 0) && (cmmutime != 0))
	{
		mbus_arbcnt++;
		killtime(mbus_latency);
		if (debugflag)
			PPrintf("mbus latency(after killtime)=%d \n",mbus_latency);
	}


	/*
	** take concurrency into effect if standard
	** memory access only
	*/

	if (cmmutime == TIME_RMEMACC)
	{
		if (debugflag)
			PPrintf(" prev_extime = %d \n",prev_extime);

		cmmutime = (TIME_RMEMACC + ADDEDLDTIME - CONCURRENTFACTOR - prev_extime);

		if (debugflag)
			PPrintf(" code access time(after recalc.) = %d \n",cmmutime);

		if (cmmutime < 0) cmmutime = 0;
	}

	else if ((cmmutime != 0) && (!mbuswait))	/* cache miss fetch */
			cmmutime -= prev_extime;	/* to account for concurrency */

	if (cmmutime < 0)
		cmmutime = 0;

	if (cmmutime != 0)
		mbus_latency = cmmutime;

	if (debugflag)
		PPrintf(" code access time = %d \n",cmmutime);

	cmmutime = 0;

	return 0;
}

/* If using the cmmu, check to see if it is still busy. If it is and the
 * next instruction is another load or store then wait until the cmmu
 * has totally completed it's previous load or store instruction.
 */

void cmmu_function2(struct IR_FIELDS   *ir)
{

	if (Dcmmutime != 0)
		if(((ir->op >= (unsigned)LDB) && (ir->op <= (unsigned)LDHU)) ||
		   ((ir->op >= (unsigned)STB) && (ir->op <= (unsigned)STD)))
		{
			pbus_arbcnt++;
			killtime(Dcmmutime);
		}

	if (debugflag)
		printf(" after cmmubusy chk : Dcmmutime = %d \n",Dcmmutime);
}


/*
*******************************************************************************
*
*       Module:     execute()
*
*       File:       dpath.c
*
*******************************************************************************
*
*  Functional Description:
*	execute() implememts the operands of the 88100.  It accepts the sources
*	and computes the result.  The result is placed on the m88000.alu bus.
*	Run time exceptions are recognized here.
*
*  Globals:
*
*  Parameters:
*
*  Output:
*	returns 0	If the execution orrured without error or exception.
*	returns 1	If a precises exception occured.
*	other		If an error occured.
*
*  Revision History:
*
*
*******************************************************************************/

int execute(struct IR_FIELDS *ir,struct SIM_FLAGS *f,struct mem_wrd *memaddr)
{
	int     retval;
	int     width;
	int     offset;
	unsigned int uS1bus, uS2bus;
	struct IR_FIELDS ir_buf;
	char    emsg[50];
	int	i=0;

/* Now we figure out which instruction we are to execute, execute it, and
 * place the result in an output latch of the ALU.
 */

	switch (ir -> op)		/* operate on current instruction */
	{
	   	/* integer and logical instructions */

		case ADDU:
		case ADDUCI:
		case ADDUCO:
		case ADDUCIO:
	   		addunsigned(m88000.S1bus, m88000.S2bus, ir, f->c_flag, 0);
	   		break;

		case ADD:
		case ADDCI:
		case ADDCO:
		case ADDCIO:
	   		if(retval = addunsigned(m88000.S1bus, m88000.S2bus, ir, f->c_flag, 1))
			return(retval);
	   		break;

		case MUL: 
			m88000.ALU = m88000.S1bus * m88000.S2bus;
	   		break;

		case DIV: 
		case DIVU: 
	    		if (m88000.S2bus == 0)
				return(exception(E_TIDE, "Divide by Zero"));

    			if(ir->op == DIVU)
				m88000.ALU = (unsigned) m88000.S1bus / (unsigned) m88000.S2bus;
    			else
    			{
				if((m88000.S1bus | m88000.S2bus) & 0x80000000)
				{
					return(exception(E_TIDE, "Division with a Negative Number"));

				}
				m88000.ALU = m88000.S1bus / m88000.S2bus;
    			}
	    		break;

		case AND: 
	    		if(f->imm_flags)
				m88000.S2bus |= 0xffff0000;
    			if (f -> c_flag)
				m88000.ALU = m88000.S1bus & ~m88000.S2bus;
    			else if (f -> u_flag)
				m88000.ALU = m88000.S1bus & ((m88000.S2bus << 16) | 0xffff);
    			else
				m88000.ALU = m88000.S1bus & m88000.S2bus;
    			break;

		case OR: 
	    		if (f -> c_flag)
				m88000.ALU = m88000.S1bus | ~m88000.S2bus;
    			else if (f -> u_flag)
				m88000.ALU = m88000.S1bus | (m88000.S2bus << 16);
    			else
				m88000.ALU = m88000.S1bus | m88000.S2bus;
    			break;

		case XOR: 
    			if (f -> c_flag)
				m88000.ALU = m88000.S1bus ^ ~m88000.S2bus;
    			else if (f -> u_flag)
				m88000.ALU = m88000.S1bus ^ (m88000.S2bus << 16);
    			else
				m88000.ALU = m88000.S1bus ^ m88000.S2bus;
    			break;

		case MASK: 
    			if (f -> u_flag)
				m88000.ALU = m88000.S1bus & (m88000.S2bus << 16);
    			else
				m88000.ALU = m88000.S1bus & m88000.S2bus;
	    		break;

		case CMP: 
    			m88000.ALU = 0;

    			if (m88000.S1bus == m88000.S2bus)
				m88000.ALU |= 4 + 32 + 128;	/* eq le ge */
    			else if(m88000.S1bus > m88000.S2bus)
				m88000.ALU |= 8 + 16 + 128;	/* ne gt ge */
    			else
				m88000.ALU |= 8 + 32 + 64;	/* ne le lt */

	    		uS1bus = (unsigned)m88000.S1bus;
    			uS2bus = (unsigned)m88000.S2bus;

    			if (uS1bus == uS2bus)
				m88000.ALU |= 512 + 2048;	/* ls hs */
	    		else if(uS1bus > uS2bus)
				m88000.ALU |= 256 + 2048;	/* hi hs */
	    		else
				m88000.ALU |= 512 + 1024;	/* ls lo */
	    		break;

    		/* load address instructions */

		case LDAB:
		case LDAH:
		case LDA:
		case LDAD:
	    		switch(ir->op)
	    		{
				case LDAB:
					i = 1;
					break;

				case LDAH:
					i = 2;
					break;

				case LDA:
					i = 4;
					break;

				case LDAD:
					i = 8;
					break;
			}

	    		m88000.ALU = m88000.S1bus + (m88000.S2bus * i);
	    		break;

    		/* load and store instructions */

		case LDB: 
	    		if(retval = ldfrommem(ir, 1, 1))
				return(retval);
	    		break;
		case LDBU: 
	    		if(retval = ldfrommem(ir, 1, 0))
				return(retval);
	    		break;
		case LDH: 
	    		if(retval = ldfrommem(ir, 2, 1))
				return(retval);
	    		break;
		case LDHU: 
	    		if(retval = ldfrommem(ir, 2, 0))
				return(retval);
	    		break;
		case LD: 
	    		if(retval = ldfrommem(ir, 4, 0))
				return(retval);
	    		break;
		case LDD: 
	    		if(retval = ldfrommem(ir, 8, 0))
				return(retval);
	    		break;
		case STB: 
	    		if(retval = sttomem(ir, 1))
				return(retval);
	    		break;
		case STH: 
	    		if(retval = sttomem(ir, 2))
				return(retval);
	    		break;
		case ST: 
	    		if(retval = sttomem(ir, 4))
				return(retval);
	    		break;
		case STD: 
	    		if(retval = sttomem(ir, 8))
				return(retval);
	    		break;
		case XMEMBU:
	    		ir_buf = *ir;
	    		ir = &ir_buf;
	    		if(retval = ldfrommem(ir, 1, 0))
				return(retval);
	    		if(retval = sttomem(ir, 1))
				return(retval);
	    		break;
		case XMEM:
	    		ir_buf = *ir;
	    		ir = &ir_buf;
	    		if(retval = ldfrommem(ir, 4, 0))
				return(retval);
	    		if(retval = sttomem(ir, 4))
				return(retval);
	    		break;

		case LDCR: 
		case STCR: 
		case XCR: 
	    		if (!(uext (PSR, psrmode, 1)))		/* not if supervisor mode */
				return(exception(E_TPRV, "Supervisor Privilege Violation"));

	    		switch(ir->op)
	    		{
				case LDCR:
     				m88000.ALU = wrctlregs(0, ir->src1, 0, 0);
     				break;

				case STCR:
     				wrctlregs(0, ir->src1, m88000.Regs[ir->src2], 2);
     				break;

				case XCR:
     				m88000.ALU = wrctlregs(0, ir->src1, 0, 0);
     				wrctlregs(0, ir->src1, m88000.Regs[ir->src2], 2);
     				break;
	    		}
	    		break;

    /* control instructions */

		case JSR: 		/* absolute */
		case JMP: 		/* absolute */
		case RTN: 		/* absolute */
		case BSR: 		/* pc relative */
		case BR: 		/* pc relative */
		case BB1: 		/* pc relative */
		case BB0: 		/* pc relative */
		case BCND: 		/* pc relative */

    /* trap instructions */

		case TB1: 
		case TB0: 
		case TCND: 
		case TBND: 
		case RTE: 
    		break;

    /* bit field instructions */

		case CLR: 
		case EXT: 
		case EXTU: 
		case MAK: 
		case SET: 
		case ROT: 
	    		offset = uext (m88000.S2bus, 0, 5);
	    		width = uext (m88000.S2bus, 5, 5);
	    		if (width == 0)
				width = 32 - offset;
	    		switch(ir->op)
			{
				case CLR: 
	    				m88000.ALU = m88000.S1bus & (~(make ((-1), offset, width)));
	    				break;

				case EXT: 
	    				m88000.ALU = sext (m88000.S1bus, offset, width);
	    				break;

				case EXTU: 
	    				m88000.ALU = uext (m88000.S1bus, offset, width);
	    				break;

				case MAK: 
	    				m88000.ALU = make (m88000.S1bus, offset, width);
					break;

				case SET: 
					m88000.ALU = m88000.S1bus | (make ((-1), offset, width));
					break;

				case ROT: 
					m88000.ALU = (m88000.S1bus << (32 - offset)) | uext (m88000.S1bus, offset, 32 - offset);
					break;
			}
			break;

		case FF_ONE: 
	    		if (f -> c_flag)
				m88000.S2bus = ~m88000.S2bus;
		case FFZERO: 
			for (i = 31; (i != -1) && (m88000.S2bus & 0x80000000); m88000.S2bus <<= 1, i--);
	    			m88000.ALU = ((i == -1) ? 32: i);
	    		break;

    /* floating point instructions */

		case FADD: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		if(retval = fadd (ir))
				return(retval);
	    		break;
		case FSUB: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		if(retval = fsub (ir))
				return(retval);
	    		break;
		case FCMP: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		if(retval = fcmp (ir))
				return(retval);
	    		break;
		case FMUL: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		if(retval = fmul (ir))
				return(retval);
	    		break;
		case FDIV: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		if(retval = fdiv (ir))
				return(retval);
	    		break;
		case FLT: 
			if (((m88000.SFU0_regs[1] & 0x8) == 0x8) && (vecyes))
				/* FPU is Disabled */
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - FPU is Disabled"));
	    		flt (ir);
	    		break;
		case INT: 
	    		if(retval = convert_int (ir, RNDMODE ()))
				return(retval);
	    		break;
		case NINT: 
	    		if(retval = convert_int (ir, RN))
				return(retval);
	    		break;
		case TRNC: 
	    		if(retval = convert_int (ir, RZ))
				return(retval);
	    		break;

		case FLDC: 
		case FSTC: 
		case FXC: 
	    		if (!(uext (PSR, psrmode, 1)) && (ir->src1 < 62))	/* not if supervisor mode */
			{
				FPECR |= FPRV;
				return(exception(E_SFU1P, "Floating Point Unit: ERROR - Privilege Violation"));
			}

	    		switch(ir->op)
	    		{
				case FLDC:
	     				m88000.ALU = wrctlregs(1, ir->src1, 0, 0);
	     				break;

				case FSTC:
	     				wrctlregs(1, ir->src1, m88000.Regs[ir->src2], 2);
	     				break;

				case FXC:
	     				m88000.ALU = wrctlregs(1, ir->src1, 0, 0);
	     				wrctlregs(1, ir->src1, m88000.Regs[ir->src2], 2);
	     				break;
	    		}
	    		break;

		default: 
			fp_err_ir = ir;
			if (retval = fpunimp(memaddr->mem.l))
				return(retval);

	    		sprintf(emsg, "Unknown Opcode, $%08X (%d) at address $%08X", memaddr->mem.l, ir -> op, IP);
	    		--instr_cnt;	/* this instruction doesnt count */
	    		return(exception(E_TOPC, emsg));
	}

/* if the instruction was a store or a load save the contents of cmmutime */
/* and update mbus latency flag */

   	if( ((ir->op >= (unsigned)LDB) && (ir->op <= (unsigned)LDHU)) ||
		((ir->op >= (unsigned)STB) && (ir->op <= (unsigned)STD)))
   	{
		mbus_arbcnt++;

		if (mbus_latency != 0)
			killtime(mbus_latency);

		Dcmmutime = cmmutime;
		mbus_latency = cmmutime;
   	}

/* Now that the instruction has been executed, copy the result from the
 * ALU to the destination register, if not a store or a branch.
 */
	/* if instruction uses destination register */
   	if ((f -> rsd_used) && (ir -> dest != 0))	/* and not r0 */
   	{
		m88000.Regs[ir -> dest] = m88000.ALU;

		/* 
		 * store execution latency, and writeback priority in
		 * register file for the current instruction.
		 */

	       	if ((usecmmu) && (ir->op >= (unsigned)LDB) && (ir->op <= (unsigned)LDHU))
		{
			m88000.time_left[ir -> dest] = f -> fu_latency + Dcmmutime;
			if ((ir->op == LDD) && ((ir->dest) <= 30))
			{
				m88000.time_left[(ir -> dest)+1] = f->fu_latency + Dcmmutime + 1;
			}
		}
		else
		{
			m88000.time_left[ir -> dest] = f -> fu_latency;
			if ((ir->op == LDD) && ((ir->dest) <= 30))
			{
				m88000.time_left[(ir -> dest)+1] = f->fu_latency + 1;
			}
		}

		m88000.wb_pri[ir -> dest] = f -> wb_pri;
   	}

	display_trace(ir, memaddr);

	return(0);
}


void display_trace(struct IR_FIELDS *ir,struct mem_wrd  *memaddr)
{
	extern int	sym_addr;


	if((trace_flg == 1) || ((trace_flg == 2) && 
	   (ir->op >= (unsigned)JSR) && (ir->op <= (unsigned)BCND)))
	{
  		trans.adr1 = IP;
  		PPrintf ("\t\t%08X %08X %08X\n", m88000.ALU, m88000.S1bus, 
		        m88000.S2bus);

		if (sym_addr)
  			PPrintf("%12s %08X %08X %s\n", symbol(M_INSTR, IP), 
			       IP, memaddr->mem.l, dis(memaddr->mem.l, IP));
		else
  			PPrintf("%08X %08X %s\n", IP, memaddr->mem.l, 
			       dis(memaddr->mem.l, IP));
	}
	else if (trace_flg == 3)
	{
		trans.adr1 = IP;
		PPrintf("%12s %08X %08X %s\n", symbol(M_INSTR, IP), IP, 
		       memaddr->mem.l, dis(memaddr->mem.l, IP));
	}
	else if (trace_flg == 4)
	{
		trans.adr1 = IP;

		if (pipe_open)
		{
			if (sym_addr)
				fprintf(pipe_strm, "%10d %16s  %s\n", 
				        readtime(), symbol(M_INSTR, IP), 
				        dis(memaddr->mem.l, IP));
			else
				fprintf(pipe_strm, "%10d  %08X  %s\n", 
				        readtime(), IP, 
				        dis(memaddr->mem.l, IP));
		}
		else
		{
			if (sym_addr)
				PPrintf("%10d %16s  %s\n", readtime(), 
				       symbol(M_INSTR, IP),
				       dis(memaddr->mem.l, IP));
			else
				PPrintf("%10d  %08X  %s\n", readtime(), IP, 
				       dis(memaddr->mem.l, IP));
		}
	}
}
