/* @(#) File name: floaterr.c   Ver: 3.2   Cntl date: 2/10/89 09:53:02 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
 
#ifdef m88000
#undef m88000
#endif

#include  "functions.h"
#include  "float.h"

extern ULONG addone;

char	caseerr[] = { "in Case Statement" };
char	intover[] = { "- Conversion to integer overflow" };
char	unimpfp[] = { "- Unimplemented Floating Point Instruction" };
char	convovf[] = { "- Conversion overflow" };
char	convunf[] = { "- Conversion underflow" };
char	divzero[] = { "- Floating point division by zero" };
char	naninf[] = { "- Reserved Operand: Nan or Infinity Detected" };
char	denorm[] = { "- Reserved Operand: Denormalized number detected" };
char	overflow[] = { "- Overflow" };
char	underflow[] = { "- Underflow" };
char	inexact[] = { "- Inexact Condition exists" };

struct IR_FIELDS  *fp_err_ir;

void floaterr(char *msg,
ULONG a_sign,
ULONG b_sign,
int a_exp,
int b_exp,
ULONG a_manthi,
ULONG b_manthi,
ULONG a_mantlo,
ULONG b_mantlo,
int singleflag,
ULONG rnd,
ULONG grd,
ULONG rd,
ULONG stky)
{

	Eprintf("Floating point unit: ERROR %s\n", msg);
	if ((msg == overflow) || (msg == convovf))
		{
		FPECR = FOVF;
		/* check for inexactness */
 		if (grd || rd || stky)
			if (FPCR & EFINX)
				{
				FPECR |= FINX;
				FPSR  |= AFINX;
				}
			else
				FPSR  |= AFINX;

		impre_except(a_sign,a_exp,a_manthi,a_mantlo,singleflag,rnd,grd,rd,stky);
		exception(E_SFU1I,"");
		}
	else if ((msg == underflow) || (msg == convunf))
			{
			FPECR = FUNF;
			/* check for inexactness */
 			if (grd || rd || stky)
				if (FPCR & EFINX)
					{
					FPECR |= FINX;
					FPSR  |= AFINX;
					}
				else
					FPSR  |= AFINX;
			impre_except(a_sign,a_exp,a_manthi,a_mantlo,singleflag,rnd,grd,rd,stky);
			exception(E_SFU1I,"");
			}
		else if (msg == intover)
				{
				FPECR = FIOV;
				/* update precise exception control registers */
				pre_except(a_sign,b_sign,a_exp,b_exp,a_manthi,b_manthi,a_mantlo,b_mantlo,singleflag);
				exception(E_SFU1P,"");
				}
			else if (msg == divzero)
					{
					FPECR = FDVZ;
					/* update precise exception control registers */
					pre_except(a_sign,b_sign,a_exp,b_exp,a_manthi,b_manthi,a_mantlo,b_mantlo,singleflag);
					exception(E_SFU1P,"");
					}
				else if ((msg == naninf) || (msg == denorm))
					{
					FPECR = FINV;
					/* update precise exception control registers */
					pre_except(a_sign,b_sign,a_exp,b_exp,a_manthi,b_manthi,a_mantlo,b_mantlo,singleflag);
					exception(E_SFU1P,"");
					}
					else if (msg == unimpfp)
						{
						FPECR = FUNIMP;
						/* update precise exception control registers */
						pre_except(a_sign,b_sign,a_exp,b_exp,a_manthi,b_manthi,a_mantlo,b_mantlo,singleflag);
						exception(E_SFU1P,"");
						}
						else if (msg == inexact)
							{
							FPECR = FINX; 
							impre_except(a_sign,a_exp,a_manthi,a_mantlo,singleflag,rnd,grd,rd,stky);
							exception(E_SFU1I,"");
							}
}




void pre_except(
ULONG a_sign,
ULONG b_sign,
int a_exp,
int b_exp,
ULONG a_manthi,
ULONG b_manthi,
ULONG a_mantlo,
ULONG b_mantlo,
int singleflag)

{

ULONG value;
struct IR_FIELDS  *ir = fp_err_ir;
unsigned int manthi;

/* update precise exception control register (SFU1 - register 5) */

value = ((ir->p->opcode & PCR_OPERMASK) | (ir->dest & PCR_DESTMASK)) & PCR_RESMASK;
wrctlregs(1,5,value,2);

/* update Source 1 Operand High register (SFU1 - register 1) */

/* first sign extend exponent (for m88000 type representation)*/
if (singleflag)                 /* exponent was in single format */ 
	{
	if ((a_exp & 0x80) == 0x80)  /* 8th bit of exponent was 1 */
		a_exp |= 0x00000700;     /* sign extend to 11 bits */
	else
		a_exp &= 0x000000ff;     /* 8th bit was 0 */
	manthi = (a_manthi & HIGH20BITS) >> 3;
	}
else                            /* exponent was in double format */ 
	manthi = a_manthi;
	

value = ((a_sign & 0x1) << 31) | (a_exp << 20) | (manthi);
wrctlregs(1,1,value,2);

/* update Source 1 Operand Low register (SFU1 - register 2) */

if (singleflag)
	{
	value = ((a_manthi & 0x7) << 29) & 0xE0000000;
	/* printf("a_manthi=0x%08x   value=0x%08x \n",a_manthi,value); */
	}
else
	value = a_mantlo;

wrctlregs(1,2,value,2);

/* update Source 2 Operand High register (SFU1 - register 3) */

/* first sign extend exponent (for m88000 type representation)*/

if (singleflag)                 /* exponent was in single format */ 
	{
	if ((b_exp & 0x80) == 0x80)  /* 8th bit of exponent was 1 */
		b_exp |= 0x00000700;     /* sign extend to 11 bits */
	else
		b_exp &= 0x000000ff;     /* 8th bit was 0 */
	manthi = (b_manthi & HIGH20BITS) >> 3;
	}
else                            /* exponent was in double format */ 
	manthi = b_manthi;

value = ((b_sign & 0x1) << 31) | (b_exp << 20) | (manthi);
wrctlregs(1,3,value,2);

/* update Source 2 Operand Low register (SFU1 - register 4) */

if (singleflag)
	value = ((b_manthi & 0x7) << 29) & 0xE0000000;
else
	value = b_mantlo;

wrctlregs(1,4,value,2);

}





void impre_except(ULONG a_sign,int a_exp,ULONG a_manthi,ULONG a_mantlo,
int singleflag,ULONG rnd,ULONG grd,ULONG rd,ULONG stky)
{

ULONG value;
struct IR_FIELDS  *ir = fp_err_ir;

/* update imprecise exception control register (SFU1 - register 8) */

/* first sign extend exponent (for m88000 type representation)*/

	if (singleflag)                 /* exponent was in single format */ 
		{
		a_exp += -128;
		a_exp &= 0xfff;
		}
	else                            /* exponent was in double format */
		{
		a_exp += -1024;
		a_exp &= 0xfff;
		}


/* shift 12-bit result exponent to bits 31-20 of register */

value  = (a_exp << 20) & 0xFFFFF000;               /* into bits 31-20 */	
value |= (ir->p->opcode & IMPCR_OPERMASK);         /* into bits 15-11 */
value |= (ir->dest & IMPCR_DESTMASK);              /* into bits 4-0   */
value |= ((ir->p->opcode & IMPCR_DSIZEMASK) << 5); /* into bit 10     */ 
value |= (((FPCR & 0x1f)<<5) & 0x3e0);
value &= IMPCR_RESMASK;                            /* into bits 19-16 */
wrctlregs(1,8,value,2);

/* update Mantissa High Register (SFU1 - register 6) */

value  = (a_sign & 0x1) << 31;
value |= (rnd & 0x3)    << 29;
value |= (grd & 0x1)    << 28;
value |= (rd  & 0x1)    << 27;
value |= (stky & 0x1)   << 26;
value |= (addone  & 0x1)<< 25; 
if (singleflag)
	value |= (((a_manthi & HIGH20BITS) >> 3) | 0x00100000);
else
	value |= (a_manthi | 0x00100000);
value &= 0xFE1FFFFF;

wrctlregs(1,6,value,2);

/* update Mantissa low Register (SFU1 - register 7) */

if (singleflag)
	value = ((a_manthi & 0x7) << 29) & 0xE0000000;
else
	value = a_mantlo;

wrctlregs(1,7,value,2);

}
