/* @(#) File name: flt.c   Ver: 3.1   Cntl date: 1/20/89 14:25:35 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#ifdef m88000
#undef m88000
#endif

# include "functions.h"
# include "float.h"

extern char caseerr[];
extern struct IR_FIELDS  *fp_err_ir;


void flt(struct IR_FIELDS *ir)
{

    WORD        *src2 = &m88000.Regs[ir->src2];
    WORD        *dest = &m88000.Regs[ir->dest];

    unsigned int status;
	    fp_err_ir = ir;

    switch(FPSIZE(ir)) {
	case SSS:
	    status = fcis(*src2,(unsigned int *)dest,RNDMODE());
	    m88000.ALU = *dest;
	    upd_status(status);

	    break;

	case DSS:
	    status = fcid(*src2,(unsigned int *)dest,(unsigned int *)(dest+1));
	    m88000.ALU = *dest;

	    upd_status(status);

	    break;
	
	default:
	    floaterr(caseerr,0,0,0,0,0,0,0,0,0,0,0,0,0);
    }
}

