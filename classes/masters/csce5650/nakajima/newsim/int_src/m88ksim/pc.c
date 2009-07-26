/* @(#) File name: pc.c   Ver: 3.1   Cntl date: 1/20/89 14:40:57 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


/* Sequencing Control Logic for M88000 Simulator
 *
 *     The straight-forward case is inline code, where we merely increment
 *     the instruction pointer.
 *
 *     For branches and traps we calculate a new instruction pointer.
 *     The branch destination may be absolute or calculated.
 */

# ifdef m88000
# undef m88000
# endif

# include "functions.h"

extern int branch_taken_cnt;


int Pc(struct mem_wrd *memaddr,struct IR_FIELDS *ir,struct SIM_FLAGS *f)
{
    int retval;

    /*
    static double   bugfix;	 if these arnt here, dot_n_flag gets clobbered 
    static double   bugfix2;
    unsigned int  *bufptr;
    */

    static int  dot_n_flag;
    static unsigned int    pc_dot_n;

    m88000.jump_pending = 0;

    if ((ir -> op >= (unsigned)TB0) && (ir -> op <= (unsigned)RTE))
    {
	if (checkfortrap (ir, memaddr))
	{
	    if(retval = vector (ir, uext(opword(IP),0,10)))
		return(retval);
	    return(0);
	}
    }
    if (dot_n_flag)		/* if last instruction was a branch.n */
    {
	IP = pc_dot_n;
	dot_n_flag = FALSE;
	return(0);
    }


    /* If taking any type of jump, calculate new instruction pointer. */

    if ((ir -> op >= (unsigned)JSR) && (ir -> op <= (unsigned)BCND))
    {
	if(m88000.jump_pending = checkforjump (ir, memaddr))
	    ++branch_taken_cnt;

	switch (ir -> op)
	{
	    case JSR: 
		if (f -> n_flag)
		    m88000.Regs[1] = IP + 8;
		else
		    m88000.Regs[1] = IP + 4;

	    case JMP: 

		if (f -> n_flag)
		{
		    pc_dot_n = m88000.Regs[ir -> src2];
		    dot_n_flag = TRUE;
		}
		else
		{
		    IP = m88000.Regs[ir -> src2];
		}
		break;
	    case BSR: 
		if (f -> n_flag)
		{
		    m88000.Regs[1] = IP + 8;
		}
		else
		{
		    m88000.Regs[1] = IP + 4;
		}
	    case BR: 
	    case BCND: 
	    case BB0: 
	    case BB1: 
		if (!m88000.jump_pending)
		    break;

		if (f -> n_flag)
		{
		    pc_dot_n = IP + (m88000.S2bus << 2);
		    dot_n_flag = TRUE;
		}
		else
		    IP = IP + (m88000.S2bus << 2);

		break;
	}
    }

    if ((!m88000.jump_pending) || (dot_n_flag))/* increment IP */
		IP = IP + 4;
    return(0);
}

/* check the branch condition to see if the branch should be taken
 */

int checkforjump (struct IR_FIELDS *ir,struct mem_wrd *memaddr)
{
    unsigned int    data,
                    temp;

    data = m88000.Regs[ir -> src1];
    if ((ir -> op == BR) || (ir -> op == JMP) ||
	(ir -> op == JSR) || (ir -> op == BSR) ||
	((ir -> op == BB1) && (((data >> ir -> dest) & 1) == 1)) ||
	((ir -> op == BB0) && (((data >> ir -> dest) & 1) == 0)))

	return(TRUE);

    else if (ir -> op == BCND)
    {
	temp = (data & 0x7fffffff) ? 0 : 1;
	/* concatenate with sign bit */
	if (data & 0x80000000)
	    (temp |= 0x02);

	if (uext (opword (IP), temp + 21, 1) == 1)
	    return(TRUE);
    }
    return (FALSE);
}

int checkfortrap (struct IR_FIELDS *ir,struct mem_wrd *memaddr)
{
    unsigned int    data,
                    temp,
                    temp2;

    data = m88000.Regs[ir -> src1];

    if (((ir -> op == TB1) && (((data >> (int) ir -> dest) & 1) == 1)) ||
	  ((ir -> op == TB0) && (((data >> (int) ir -> dest) & 1) == 0)))
		return(TRUE);

    else if (ir -> op == TCND)
    {
	temp = (data & 0x7fffffff) ? 0 : 1;
	/* concatenate with sign bit */
	if (data & 0x80000000)
	    (temp |= 0x02);

	if (uext (opword (IP), temp + 21, 1) == 1)
	    return(TRUE);
    }

    else if (ir -> op == TBND)
    {
	temp = m88000.Regs[ir -> src1];

	if (!ir->p->flgs.imm_flags)
	    temp2 = m88000.Regs[ir -> src2];
	else
	    temp2 = uext (opword (IP), 0, 16);

	if (temp > temp2)
	    return(TRUE);
    }

    if(ir -> op == RTE)
	return(TRUE);

    return (FALSE);
}
