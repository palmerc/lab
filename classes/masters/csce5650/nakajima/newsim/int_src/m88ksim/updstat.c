/* @(#) File name: updstat.c   Ver: 3.1   Cntl date: 1/20/89 14:26:14 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

/*
 *  This is the source file for the M88000 data path simulator
 */
# include "functions.h"
# include "float.h"

extern struct PROCESSOR  *processor;
 
void upd_status(unsigned int status)
{

    if (status&0xffff != 0) {
	BEFORE_EXC(status);
	return;
    }

    if (status & INEXACT) {
	if (READ_FPCR() & EFINX) {
	    INX_EXC();
	}
	else {
	    WRITE_FPSR(READ_FPSR() | AFINX);
	}
    }

    if ((status & OVERFLOW)||(status & UNDERFLOW)) {
        AFT_EXC(status);
    } 
}

