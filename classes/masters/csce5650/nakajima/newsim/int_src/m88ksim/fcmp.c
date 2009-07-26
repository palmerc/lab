/* @(#) File name: fcmp.c   Ver: 3.1   Cntl date: 1/20/89 14:25:19 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#ifdef m88000
#undef m88000
#endif

# include "functions.h"
# include "float.h"

extern char caseerr[];
extern struct IR_FIELDS  *fp_err_ir;


int fcmp(struct IR_FIELDS *ir)
{

    WORD        *src1 = &m88000.Regs[ir->src1];
    WORD        *src2 = &m88000.Regs[ir->src2];
    WORD        *dest = &m88000.Regs[ir->dest];

    unsigned int status;
    unsigned int temp1, temp2;
	fp_err_ir = ir;

    switch(FPSIZE(ir)) {
	case SSS:
	    status = fcmps(*src1,*src2,(unsigned int *)dest);
	    if(status != 0)
		return(1);
	    m88000.ALU = *dest;
	    break;

	case SDS:
	    status = fcsd(*src2,&temp1,&temp2);
	    if (status==0) {
	       status = fcmp64(*src1,*(src1+1),temp1,temp2,(unsigned int *)dest);
	       m88000.ALU = *dest;
            }
	    if(status != 0)
		return(1);
	    break;
	
	case SSD:
	    status = fcsd(*src1,&temp1,&temp2);
	    if (status==0) {
	       status = fcmp64(temp1,temp2,*src2,*(src2+1),(unsigned int *)dest);
	       m88000.ALU = *dest;
            }
	    if(status != 0)
		return(1);
	    break;
	
	case SDD:
	    status = fcmp64(*src1,*(src1+1),*src2,*(src2+1),(unsigned int *)dest);
	    m88000.ALU = *dest;
	    if(status != 0)
		return(1);
	    break;
	
	default:
	    floaterr(caseerr,0,0,0,0,0,0,0,0,0,0,0,0,0);
    }

	return(0);
}

