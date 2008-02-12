/* @(#) File name: instab.h   Ver: 3.1   Cntl date: 1/20/89 14:39:03 */
/*
*	This is the include file for the one line assembler/disassembler 
*	package.
*
*/

union opcode
{
	unsigned long	opc;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, opc2:11, src1:5, dest:5, opc1:6;
#else
	 unsigned opc1:6, dest:5, src1:5, opc2:11, src2:5;
#endif
	} gen;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, res:3, user:1, idx:1, ty:2, p:2, zero:2, src1:5, dest:5, fixed:6;
#else
	 unsigned fixed:6, dest:5, src1:5, zero:2, p:2, ty:2, idx:1, user:1, res:3, src2:5;
#endif
	} mem;
	struct
	{
#ifdef LEHOST
	 unsigned imm16:16, src1:5, dest:5, ty:2, p:2, fixed:2;
#else
	 unsigned fixed:2, p:2, ty:2, dest:5, src1:5, imm16:16;
#endif
	} memi;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, crsd:6, sfu:3, opc:2, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:2, sfu:3, crsd:6, src2:5;
#endif
	} ctl;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, td:2, t2:2, t1:2, opc:5, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:5, t1:2, t2:2, td:2, src2:5;
#endif
	} sfu1;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, opc:11, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:11, src2:5;
#endif
	} rrr;
	struct
	{
#ifdef LEHOST
	 unsigned opc:16, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:16;
#endif
	} trpbfi;
	struct
	{
#ifdef LEHOST
	 unsigned src2:5, zero2:5, nxt:1, opc:1, ignore:4, zero1:10, min:3, maj:3;
#else
	 unsigned maj:3, min:3, zero1:10, ignore:4, opc:1, nxt:1, zero2:5, src2:5;
#endif
	} xfra;
	struct
	{
#ifdef LEHOST
	 unsigned vec9:9, zero:1, opc:6, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:6, zero:1, vec9:9;
#endif
	} trp;
	struct
	{
#ifdef LEHOST
	 unsigned imm10:10, opc:6, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, opc:6, imm10:10;
#endif
	} imm10;
	struct
	{
#ifdef LEHOST
	 unsigned imm16:16, src1:5, dest:5, min:3, maj:3;
#else
	 unsigned maj:3, min:3, dest:5, src1:5, imm16:16;
#endif
	} imm16;
	struct
	{
#ifdef LEHOST
	 unsigned imm26:26, min:3, maj:3;
#else
	 unsigned maj:3, min:3, imm26:26;
#endif
	} imm26;
};

union rrropc
{
	unsigned short opc;
	struct
	{
#ifdef LEHOST
	 unsigned zero:5, min:3, maj:3, ignore:5;
#else
	 unsigned ignore:5, maj:3, min:3, zero:5;
#endif
	} gen;
	struct
	{
#ifdef LEHOST
	 unsigned zero:3, user:1, idx:1, ty:2, p:2, ignore:7;
#else
	 unsigned ignore:7, p:2, ty:2, idx:1, user:1, zero:3;
#endif
	} mem;
	struct
	{
#ifdef LEHOST
	 unsigned zero:5, ignore:11;
#else
	 unsigned ignore:11, zero:5;
#endif
	} tbndrte;
	struct
	{
#ifdef LEHOST
	 unsigned zero:3, cbout:1, cbin:1, opc:3, ignore:8;
#else
	 unsigned ignore:8, opc:3, cbin:1, cbout:1, zero:3;
#endif
	} intg;
	struct
	{
#ifdef LEHOST
	 unsigned zero:5, cp:1, opc:2, ignore:8;
#else
	 unsigned ignore:8, opc:2, cp:1, zero:5;
#endif
	} log;
	struct
	{
#ifdef LEHOST
	 unsigned zero:6, opc:3, ignore:7;
#else
	 unsigned ignore:7, opc:3, zero:6;
#endif
	} bf;
	struct
	{
#ifdef LEHOST
	 unsigned  o:5, w:5, zero:1, opc:3, maj:2;
#else
	 unsigned maj:2, opc:3, zero:1, w:5, o:5;
#endif
	} bfi;
	struct
	{
#ifdef LEHOST
	 unsigned vec:9, zero:2, opc:3, maj:2;
#else
	 unsigned maj:2, opc:3, zero:2, vec:9;
#endif
	} trp;
};


struct instruction
{
	char		*name;
	struct
	{
		unsigned	imm:6, imm10:6, rrr:11;
	} opc;
	int	(*funct)();
};

