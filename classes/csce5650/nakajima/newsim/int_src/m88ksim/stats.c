/* @(#) File name: stats.c   Ver: 3.2   Cntl date: 4/25/89 12:40:54 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# include "functions.h"


int			instr_cnt;
int			branch_taken_cnt;
extern unsigned int	Clock;
extern int		usecmmu, mbus_arbcnt, pbus_arbcnt;

int	mathcount,
	logicalcount,
	ldacount,
	loadcount,
	storecount,
	xmemcount,
	crcount,
	bitcount,
	jumpcount,
	trapcount,
	floatcount,
	errorcount;

void statreset(void)
{
	instr_cnt =
	branch_taken_cnt =
	mathcount =
	logicalcount =
	ldacount =
	loadcount =
	storecount =
	xmemcount=
	crcount =
	bitcount =
	jumpcount =
	trapcount =
	floatcount =
	errorcount =
	Clock =
	mbus_arbcnt =
	pbus_arbcnt = 0;
}

void printstats(void)
{

	PPrintf ("Instructions: %d\n", instr_cnt);
	Clock = readtime ();
	PPrintf ("Clocks:       %d\n", Clock);

	if (instr_cnt != 0)
	{
		PPrintf ("arithmetic       %10d   %5.2f percent\n", mathcount, (float) mathcount / instr_cnt * 100);
		PPrintf ("logical          %10d   %5.2f percent\n", logicalcount, (float) logicalcount / instr_cnt * 100);
		PPrintf ("load address     %10d   %5.2f percent\n", ldacount, (float) ldacount / instr_cnt * 100);
		PPrintf ("load             %10d   %5.2f percent\n", loadcount, (float) loadcount / instr_cnt * 100);
		PPrintf ("store            %10d   %5.2f percent\n", storecount, (float) storecount / instr_cnt * 100);
		PPrintf ("xmem             %10d   %5.2f percent\n", xmemcount, (float) xmemcount / instr_cnt * 100);
		PPrintf ("control register %10d   %5.2f percent\n", crcount, (float) crcount / instr_cnt * 100);
		PPrintf ("bit field        %10d   %5.2f percent\n", bitcount, (float) bitcount / instr_cnt * 100);
		PPrintf ("jumps not taken  %10d   %5.2f percent\n", jumpcount - branch_taken_cnt, (float) (jumpcount - branch_taken_cnt) / instr_cnt * 100);
		PPrintf ("jumps taken      %10d   %5.2f percent\n", branch_taken_cnt, (float) branch_taken_cnt / instr_cnt * 100);
		PPrintf ("traps            %10d   %5.2f percent\n", trapcount, (float) trapcount / instr_cnt * 100);
		PPrintf ("floating point   %10d   %5.2f percent\n", floatcount, (float) floatcount / instr_cnt * 100);
		PPrintf ("error count      %10d   %5.2f percent\n", errorcount, (float) errorcount / instr_cnt * 100);

		if(usecmmu)
		{
			printf(" NON-Execution Clocks: \n");
			printf("  Clocks due to Mbus Arbitration = %d \n",mbus_arbcnt);
			printf("  Clocks due to Pbus Arbitration = %d \n",pbus_arbcnt);
		}
	}
}


void Statistics (struct IR_FIELDS *ir)
{
	++instr_cnt;
	switch (ir -> op)		/* operate on current instruction */
	{
					/* integer and logical instructions */

		case ADDU:
		case ADDUCO:
		case ADDUCI:
		case ADDUCIO:
		case ADD:
		case ADDCO:
		case ADDCI:
		case ADDCIO:
		case MUL: 
		case DIV: 
		case DIVU: 
			++mathcount;
			break;
		case AND: 
		case OR: 
		case XOR: 
		case MASK: 
		case CMP: 
			++logicalcount;
			break;

		/* load address instructions */

		case LDAB: 
		case LDAH: 
		case LDA: 
		case LDAD:
			++ldacount;
			break;

		/* load and store instructions */

		case LDB: 
		case LDBU: 
		case LDH: 
		case LDHU: 
		case LD: 
		case LDD: 
			++loadcount;
			break;
		case STB: 
		case STH: 
		case ST: 
		case STD: 
			++storecount;
			break;

		case XMEM:
		case XMEMBU:
			++xmemcount;
			break;

		case LDCR: 
		case STCR: 
		case XCR: 
			++crcount;
			break;

		/* control instructions */

		case JSR:		/* absolute */
		case JMP:		/* absolute */
		case RTN:		/* absolute */
		case BSR:		/* pc relative */
		case BR: 		/* pc relative */
		case BB1:		/* pc relative */
		case BB0:		/* pc relative */
		case BCND:		/* pc relative */
			++jumpcount;
			break;

		/* trap instructions */

		case TB1: 
		case TB0: 
		case TCND: 
		case TBND: 
		case RTE: 
			++trapcount;
			break;

		/* bit field instructions */

		case CLR: 
		case EXT: 
		case EXTU: 
		case MAK: 
		case SET: 
		case ROT: 
		case FF_ONE: 
		case FFZERO: 
			++bitcount;
			break;

		/* floating point instructions */

		case FADD: 
		case FSUB: 
		case FCMP: 
		case FMUL: 
		case FDIV: 
		case FSQRT: 
		case FLT: 
		case INT: 
		case NINT: 
		case TRNC: 
		case FLDC: 
		case FSTC: 
		case FXC: 
			++floatcount;
			break;
		default: 
			++errorcount;
			return;
	}
}
