/* @(#) File name: fdiv.c   Ver: 3.1   Cntl date: 1/20/89 14:25:27 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#ifdef m88000
#undef m88000
#endif

# include "functions.h"
# include "float.h"

extern char caseerr[];
extern struct IR_FIELDS  *fp_err_ir;


int fdiv(struct IR_FIELDS *ir)
{

    WORD        *src1 = &m88000.Regs[ir->src1];
    WORD        *src2 = &m88000.Regs[ir->src2];
    WORD        *dest = &m88000.Regs[ir->dest];

    unsigned int status;
    unsigned int src1_hi, src1_lo;
    unsigned int src2_hi, src2_lo;
    unsigned int dest_hi, dest_lo;

    status = 0;
	    fp_err_ir = ir;

    switch(FPSIZE(ir)) {
	case SSS:
	    status = fdivs(*src1,*src2,(unsigned int *)dest,RNDMODE(),1);
	    m88000.ALU = *dest;
	    if(status != 0)
		return(1);

	    break;

	case DDS:
	    status = fcsd(*src2,&src2_hi,&src2_lo);

	    if (status==0) {
	       status = fdiv64(*src1,*(src1+1),src2_hi,src2_lo,&dest_hi,&dest_lo,RNDMODE());
	       m88000.ALU = dest_hi;
	       *(dest+1) = dest_lo;
            }

	    if(status != 0)
		return(1);

	    break;
	
	case DSD:
	    status = fcsd(*src1,&src1_hi,&src1_lo);

	    if (status==0) {
	       status = fdiv64(src1_hi,src1_lo,*src2,*(src2+1),&dest_hi,&dest_lo,RNDMODE());
	       m88000.ALU = dest_hi;
	       *(dest+1) = dest_lo;
            }

	    if(status != 0)
		return(1);

	    break;
	
	case DSS:
	    status = fcsd(*src1,&src1_hi,&src1_lo);

	    if (status==0) {
	        status = fcsd(*src2,&src2_hi,&src2_lo);
	        if (status==0) {
	            status = fdiv64(src1_hi,src1_lo,src2_hi,src2_lo,&dest_hi,&dest_lo,RNDMODE());
		    m88000.ALU = dest_hi;
	            *(dest+1) = dest_lo;
	        }
            }

	    if(status != 0)
		return(1);

	    break;
	
	case SDS:
	    status = fcsd(*src2,&src2_hi,&src2_lo);

	    if (status==0) {
	       status = fdiv64(*src1,*(src1+1),src2_hi,src2_lo,&dest_hi,&dest_lo,RNDMODE());
	       if (status==0 || status==INEXACT) {
	          status |= fcds(dest_hi,dest_lo,(unsigned int *)dest,RNDMODE());
		  m88000.ALU = *dest;
               }
            }

	    if(status != 0)
		return(1);

	    break;
	
	case SSD:
	    status = fcsd(*src1,&src1_hi,&src1_lo);

	    if (status==0) {
	       status = fdiv64(src1_hi,src1_lo,*src2,*(src2+1),&dest_hi,&dest_lo,RNDMODE());
	       if (status==0 || status==INEXACT) {
	          status |= fcds(dest_hi,dest_lo,(unsigned int *)dest,RNDMODE());
		  m88000.ALU = *dest;
               }
            }

	    if(status != 0)
		return(1);

	    break;
	
	case SDD:
	    status = fdiv64(*src1,*(src1+1),*src2,*(src2+1),&dest_hi,&dest_lo,RNDMODE());
	    if (status==0 || status==INEXACT) {
	       status |= fcds(dest_hi,dest_lo,(unsigned int *)dest,RNDMODE());
	       m88000.ALU = *dest;
            }

	    if(status != 0)
		return(1);

	    break;
	
	case DDD:
	    status = fdiv64(*src1,*(src1+1),*src2,*(src2+1),(unsigned int *)dest,(unsigned int *)(dest+1),RNDMODE());
	    m88000.ALU = *dest;
	    if(status != 0)
		return(1);

	    break;

	default:
	    floaterr(caseerr,0,0,0,0,0,0,0,0,0,0,0,0,0);
    }
    return(0);
}

