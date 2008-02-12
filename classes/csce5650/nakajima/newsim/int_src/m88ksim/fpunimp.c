/* @(#) File name: fpunimp.c   Ver: 3.2   Cntl date: 2/10/89 09:53:30 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

#include "functions.h"
#include "float.h"

extern char unimpfp[];
extern struct IR_FIELDS  *fp_err_ir;


int fpunimp(unsigned int instr_encode)
{


    WORD        *src1 = &m88000.Regs[(instr_encode & 0x001f0000)>>16];
    WORD        *src2 = &m88000.Regs[(instr_encode & 0x0000001f)>> 0];

	ULONG aa_sign,aa_exp,aa_manthi,aa_mantlo;
	ULONG bb_sign,bb_exp,bb_manthi,bb_mantlo;

    unsigned int status,destsingle;

    status = 0;
    destsingle = 0;

/* Extract fields */

	if ((instr_encode & SRC1SINGLE) == 0)  /* source1 is single */
		{
   		aa_sign = BFEXTU(*src1,31,1);
   		aa_exp  = BFEXTU(*src1,23,8);
   		aa_manthi = BFEXTU(*src1,0,23);
		aa_mantlo = 0;
		}
	else											/* source1 is double */
		{
   		aa_sign = BFEXTU(*src1,31,1);
   		aa_exp  = BFEXTU(*src1,20,11);
   		aa_manthi = BFEXTU(*src1,0,20);
   		aa_mantlo = *(src1 +1);
		}

	if ((instr_encode & SRC2SINGLE) == 0)  /* source2 is single */
		{
   		bb_sign = BFEXTU(*src2,31,1);
   		bb_exp  = BFEXTU(*src2,23,8);
   		bb_manthi = BFEXTU(*src2,0,23);
		bb_mantlo = 0;
		}
	else											/* source2 is double */
		{
   		bb_sign = BFEXTU(*src2,31,1);
   		bb_exp  = BFEXTU(*src2,20,11);
   		bb_manthi = BFEXTU(*src2,0,20);
   		bb_mantlo = *(src2 +1);
		}

	if ((instr_encode & DESTSINGLE) == 0)  /* destination is single */
		destsingle++;

	/* before call to floaterr() set global fp_err_ir variables */

	fp_err_ir->p->opcode = instr_encode;
	fp_err_ir->dest = ((instr_encode & 0x03e00000) >> 21); 


	floaterr(unimpfp,aa_sign,bb_sign,aa_exp,bb_exp,aa_manthi,bb_manthi,aa_mantlo,bb_mantlo,destsingle,0,0,0,0);
	return(1);
}
