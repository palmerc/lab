/* @(#) File name: int.c   Ver: 3.1   Cntl date: 1/20/89 14:25:50 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

# include "functions.h"
# include "float.h"

extern char caseerr[];
extern struct IR_FIELDS  *fp_err_ir;


int convert_int(struct IR_FIELDS *ir, unsigned int rnd)
{

    WORD        *src2 = &m88000.Regs[ir->src2];
    WORD        *dest = &m88000.Regs[ir->dest];

    unsigned int status;

    status = 0;
	fp_err_ir = ir;
    switch(FPSIZE(ir)) {
	case SSS:
	    status = fcsi(*src2,dest,rnd);
	    if(status != 0)
		return(1);
	    m88000.ALU = *dest;
	    break;

	case SSD:
	    status = fcdi(*src2,*(src2+1),dest,rnd);
	    m88000.ALU = *dest;
	    if(status != 0)
		return(1);
	    break;
	
	default:
	    floaterr(caseerr,0,0,0,0,0,0,0,0,0,0,0,0,0);
    }
    return(0);
	
}

