/* @(#) File name: asmcmdstr.c   Ver: 3.1   Cntl date: 1/20/89 14:20:06 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*       This module contains the assembler tree structures.
*
*
*                                          Date Last Modified: 
*/

#include "functions.h"


	/**********************************************************
	*
	*       Function Definition
	*
	*/



	/**********************************************************
	*
	*       External Variables Definition
	*
	*/


	/**********************************************************
	*
	*       Local Variables and Strings
	*
	*/


	/**********************************************************
	*
	*       Assembler Structure Tree
	*
	*/

struct instruction mnemonics[] =		/* assembler command list */
{
	{ "jsr",	{ 0, 0, 0x640 },	xfra	},
	{ "jmp",	{ 0, 0, 0x600 },	xfra	},
	{ "bsr",	{ 0x32, 0, 0 },		xfrri	},
	{ "br",		{ 0x30, 0, 0 },		xfrri	},
	{ "bcnd",	{ 0x3A, 0, 0 },		xfrbci	},
	{ "bb0",	{ 0x34, 0, 0 },		xfrbci	},
	{ "bb1",	{ 0x36, 0, 0 },		xfrbci	},
	{ "tcnd",	{ 0, 0x3A, 0 },		trpbci	},
	{ "tb0",	{ 0, 0x34, 0 },		trpbci	},
	{ "tb1",	{ 0, 0x36, 0 },		trpbci	},
	{ "tbnd",	{ 0x3E, 0, 0x7C0 },	tbnd	},
	{ "rte",	{ 0, 0, 0x7e0 },	rte	},
	{ "ldcr",	{ 0, 0, 0x200 },	a_ctl	},
	{ "stcr",	{ 0, 0, 0x400 },	a_ctl	},
	{ "xcr",	{ 0, 0, 0x600 },	a_ctl	},
	{ "ld",		{ 0x04, 0, 0x080 },	mem	},
	{ "st",		{ 0x08, 0, 0x100 },	mem	},
	{ "xmem",	{ 0x00, 0, 0x000 },	mem	},
	{ "lda",	{ 0x0C, 0, 0x190 },	mem	},
	{ "addu",	{ 0x18, 0, 0x300 },	integer	},
	{ "subu",	{ 0x19, 0, 0x320 },	integer	},
	{ "divu",	{ 0x1A, 0, 0x348 },	integer	},
	{ "mul",	{ 0x1B, 0, 0x368 },	integer	},
	{ "add",	{ 0x1C, 0, 0x380 },	integer	},
	{ "sub",	{ 0x1D, 0, 0x3A0 },	integer	},
	{ "div",	{ 0x1E, 0, 0x3C8 },	integer	},
	{ "cmp",	{ 0x1F, 0, 0x3E8 },	integer	},
	{ "and",	{ 0x10, 0, 0x200 },	logical	},
	{ "or",		{ 0x16, 0, 0x2C0 },	logical	},
	{ "xor",	{ 0x14, 0, 0x280 },	logical	},
	{ "mask",	{ 0x12, 0, 0 },		logical	},
	{ "clr",	{ 0, 0x20, 0x400 },	bits	},
	{ "set",	{ 0, 0x22, 0x440 },	bits	},
	{ "ext",	{ 0, 0x24, 0x480 },	bits	},
	{ "extu",	{ 0, 0x26, 0x4C0 },	bits	},
	{ "mak",	{ 0, 0x28, 0x500 },	bits	},
	{ "rot",	{ 0, 0x2A, 0x540 },	bits	},
	{ "ff0",	{ 0, 0, 0x760 },	ffirst	},
	{ "ff1",	{ 0, 0, 0x740 },	ffirst	},
	{ "fadd",	{ 0x21, 0, 0x140 },	a_sfu1	},
	{ "fsub",	{ 0x21, 0, 0x180 },	a_sfu1	},
	{ "fcmp",	{ 0x21, 0, 0x1C0 },	a_sfu1	},
	{ "fmul",	{ 0x21, 0, 0x000 },	a_sfu1	},
	{ "fdiv",	{ 0x21, 0, 0x380 },	a_sfu1	},
	{ "flt",	{ 0x21, 0, 0x100 },	a_sfu1	},
	{ "int",	{ 0x21, 0, 0x240 },	a_sfu1	},
	{ "nint",	{ 0x21, 0, 0x280 },	a_sfu1	},
	{ "trnc",	{ 0x21, 0, 0x2C0 },	a_sfu1	},
	{ "fldcr",	{ 0, 0, 0x240 },	a_ctl	},
	{ "fstcr",	{ 0, 0, 0x440 },	a_ctl	},
	{ "fxcr",	{ 0, 0, 0x640 },	a_ctl	},
	{ "word",	{ 0, 0, 0 },		mkwrd	},
	{ NULL,		{ 0},			0	},
};


struct instruction memsuffixes[] =		/* Memory type suffixes */
{
	{ "b",		{ 0x03, 0x2E, 0x060 },	NULL	},
	{ "bu",		{ 0x03, 0x31, 0x060 },	NULL	},
	{ "b.usr",	{ 0x00, 0x06, 0x068 },	NULL	},
	{ "bu.usr",	{ 0x00, 0x11, 0x068 },	NULL	},
	{ "h",		{ 0x02, 0x2E, 0x040 },	NULL	},
	{ "hu",		{ 0x02, 0x30, 0x040 },	NULL	},
	{ "h.usr",	{ 0x00, 0x06, 0x048 },	NULL	},
	{ "hu.usr",	{ 0x00, 0x10, 0x048 },	NULL	},
	{ "",		{ 0x01, 0x2F, 0x020 },	NULL	},
	{ "usr",	{ 0x00, 0x07, 0x028 },	NULL	},
	{ "d",		{ 0x00, 0x2E, 0x000 },	NULL	},
	{ "d.usr",	{ 0x00, 0x06, 0x008 },	NULL	},
	{ NULL,		{ 0},			0	},
};

struct instruction xfrsymbols[] =		/* branch condision symbols */
{
        { "eq",		{ 0x2, 0, 0 },		NULL	},
        { "ne",		{ 0x3, 0, 0 },		NULL	},
        { "gt",		{ 0x4, 0, 0 },		NULL	},
        { "le",		{ 0x5, 0, 0 },		NULL	},
        { "lt",		{ 0x6, 0, 0 },		NULL	},
        { "ge",		{ 0x7, 0, 0 },		NULL	},
        { "hi",		{ 0x8, 0, 0 },		NULL	},
        { "ls",		{ 0x9, 0, 0 },		NULL	},
        { "lo",		{ 0xa, 0, 0 },		NULL	},
        { "hs",		{ 0xb, 0, 0 },		NULL	},
        { "eq0",	{ 0x2, 0, 0 },		NULL	},
        { "ne0",	{ 0xd, 0, 0 },		NULL	},
        { "le0",	{ 0xe, 0, 0 },		NULL	},
        { "lt0",	{ 0xc, 0, 0 },		NULL	},
        { "ge0",	{ 0x3, 0, 0 },		NULL	},
        { "gt0",	{ 0x1, 0, 0 },		NULL	},
        { "s0z0",	{ 0x1, 0, 0 },		NULL	},
        { "s0z1",	{ 0x2, 0, 0 },		NULL	},
        { "s1z0",	{ 0x4, 0, 0 },		NULL	},
        { "s1z1",	{ 0x8, 0, 0 },		NULL	},
	{ NULL,		{ 0},			0	},
};
