/* @(#) File name: sim.h   Ver: 3.2   Cntl date: 2/16/89 10:43:40 */

static char copyright15[] = "Copyright (c) Motorola, Inc. 1986";


/*
 * This file contains the structures and constants needed to build the M88000
 * M88000 simulator.  It is the main include file, containing all the
 * structures, macros and definitions except for the floating point
 * instruction set.
 */

/* define the number of bits in the primary opcode field of the instruction,
 * the destination field, the source 1 and source 2 fields.
 */
# ifdef m88000
# undef m88000
# endif
# define    OP       8                        /* size of opcode field */ 
# define    DEST     6                        /* size of destination  */
# define    SOURCE1  6                        /* size of source1      */
# define    SOURCE2  6                        /* size of source2      */

# define    REGs    32                        /* number of registers  */

# define    WORD    int
# define    FLAG    unsigned
# define    STATE   short 

# define    TRUE     1
# define    FALSE    0

# define    READ     0
# define    WRITE    1

/* The next four equates define the priorities that the various classes
 * of instructions have regarding writing results back into registers and
 * signalling exceptions.
 */

# define    PINT  0   /* Integer Priority */
# define    PFLT  1   /* Floating Point Priority */
# define    PMEM  2   /* Memory Priority */
# define    NA    3   /* Not Applicable, instruction doesnt write to regs */
# define    HIPRI 3   /* highest of these priorities */

/* The instruction registers are an artificial mechanism to speed up
 * simulator execution.  In the real processor, an instruction register
 * is 32 bits wide.  In the simulator, the 32 bit instruction is kept in
 * a structure field called rawop, and the instruction is partially decoded,
 * and split into various fields and flags which make up the other fields
 * of the structure.
 * The partial decode is done when the instructions are initially loaded
 * into simulator memory.  The simulator code memory is not an array of
 * 32 bit words, but is an array of instruction register structures.
 * Yes this wastes memory, but it executes much quicker.
 */

struct IR_FIELDS {
		    unsigned int    op:OP,
				    dest: DEST,
				    src1: SOURCE1,
				    src2: SOURCE2;
		    struct INSTRUCTAB *p;
                 };

struct	mem_segs {
	struct mem_wrd *seg;			/* pointer (returned by calloc) to segment */
	unsigned int baseaddr;			/* base virtual load address from file headers */
	unsigned int endaddr;			/* Ending address of segment */
	unsigned int physaddr;			/* base phyisical load address */
	unsigned int phyeaddr;			/* end phyisical load address */
	int	      flags;			/* segment control flags */
};

#define	M_DATA		(0xC0)			/* segment control flag for data seg */
#define	M_INSTR		(0x20)			/* segment control flag for instr seg */
#define	M_PMAP		(0x400)			/* segment control flag for simulated program requested memory */
#define	M_SEGMSK	(M_DATA|M_INSTR|M_PMAP)	/* segment mask */
#define	M_SEGANY	(M_SEGMSK)		/* any segment */

#define	M_RD		(1)			/* rdwr read request */
#define	M_WR		(2)			/* rdwr write request */

#define	MAXSEGS		(20)			/* max number of segment allowed */
#define	MEMSEGSIZE	(sizeof(struct mem_segs))/* size of mem_segs structure */


#define BRK_RD		(0x01)			/* break on memory read */
#define BRK_WR		(0x02)			/* break on memory write */
#define BRK_EXEC	(0x04)			/* break on execution */
#define	BRK_CNT		(0x08)			/* break on terminal count */


struct	mem_wrd {
	struct IR_FIELDS opcode;		/* simulator instruction break down */
	union {
		unsigned int  l;		/* memory element break down */
		unsigned short s[2];
		unsigned char  c[4];
	} mem;
};

#define	MEMWRDSIZE	(sizeof(struct mem_wrd))	/* size of each 32 bit memory model */

/* External declarations */

extern	struct mem_segs memory[];

struct  PROCESSOR   {
	    unsigned WORD   ip;
		    WORD    S1bus, /* source 1 */
                            S2bus, /* source 2 */
                            Dbus,  /* destination */
			    DAbus, /* data address bus */
                            ALU,
			    Regs[REGs],       /* data registers */
			    time_left[REGs],  /* max clocks before reg is available */
			    wb_pri[REGs],     /* writeback priority of reg */
			    SFU0_regs[REGs*2],/* integer unit control regs */
			    SFU1_regs[REGs*2];/* floating point control regs */
	    unsigned WORD   scoreboard;
		    FLAG    jump_pending;     /* waiting for a jump instr. */
                    };

extern	struct PROCESSOR m88000;

#define	IP	m88000.ip
#define	PID	m88000.SFU0_regs[0]
#define	PSR	m88000.SFU0_regs[1]
#define	TPSR	m88000.SFU0_regs[2]
#define	TSB	m88000.SFU0_regs[3]
#define	TCIP	m88000.SFU0_regs[4]
#define	TNIP	m88000.SFU0_regs[5]
#define	TFIP	m88000.SFU0_regs[6]
#define	VBR	m88000.SFU0_regs[7]

#define	FPSR	m88000.SFU1_regs[62]
#define	FPCR	m88000.SFU1_regs[63]
#define	FPECR	m88000.SFU1_regs[0]

# define    i26bit      1    /* size of immediate field */
# define    i16bit      2
# define    i10bit      3

#define	E_TCACC	(2)	/* Code Access Fault */
#define	E_TDACC	(3)	/* Data Access Fault */
#define	E_TMA	(4)	/* Missaligned memory access */
#define	E_TOPC	(5)	/* Unimplemented Operation Code */
#define	E_TPRV	(6)	/* Privileged operation attempted in user code */
#define	E_TBND	(7)	/* Array Bounds Violation */
#define	E_TIDE	(8)	/* Integer Divide */
#define	E_TIOV	(9)	/* Integer Overflow */
#define	E_TERR	(10)	/* Error Exception */

#define	E_SFU1P	(114)	/* SFU 1 Precise */
#define	E_SFU1I	(115)	/* SFU 1 Imprecise */

/* Definitions for fields in psr */

# define psrmode  31
# define rbo   30
# define ser   29
# define carry 28
# define sf7m  11
# define sf6m  10
# define sf5m   9
# define sf4m   8
# define sf3m   7
# define sf2m   6
# define sf1m   5
# define mam    4
# define inm    3
# define exm    2
# define trm    1
# define ovfm   0

/* the 1 clock operations */

# define    ADDU        1
# define    ADDUCO      ADDU+2
# define    ADDUCI      ADDU+3
# define    ADDUCIO     ADDU+4
# define    ADD         ADDU+5
# define    ADDCO       ADDU+6
# define    ADDCI       ADDU+7
# define    ADDCIO      ADDU+8

# define    AND     ADDCIO+1
# define    OR      ADDCIO+2
# define    XOR     ADDCIO+3
# define    CMP     ADDCIO+4
                        
/* the LOADS */

# define    LDAB    CMP+1
# define    LDAH    CMP+2
# define    LDA     CMP+3
# define    LDAD    CMP+4

# define    LDB   LDAD+1
# define    LDH   LDAD+2
# define    LD    LDAD+3
# define    LDD   LDAD+4
# define    LDBU  LDAD+5
# define    LDHU  LDAD+6

/* the STORES */

# define    STB    LDHU+1
# define    STH    LDHU+2
# define    ST     LDHU+3
# define    STD    LDHU+4

/* the exchange */

# define    XMEMBU STD+1
# define    XMEM   STD+2

/* the branches */
# define    JSR    XMEM+1
# define    BSR    XMEM+2
# define    BR     XMEM+3
# define    JMP    XMEM+4
# define    BB1    XMEM+5
# define    BB0    XMEM+6
# define    RTN    XMEM+7
# define    BCND   XMEM+8

/* the TRAPS */
# define    TB1    BCND+1
# define    TB0    BCND+2
# define    TCND   BCND+3
# define    TBND   BCND+4
# define    RTE    BCND+5

/* the MISC instructions */
# define    MUL     RTE  +1
# define    DIV     MUL  +2
# define    DIVU    MUL  +3
# define    MASK    MUL  +4
# define    FFZERO  MUL  +5
# define    FF_ONE  MUL  +6
# define    CLR     MUL  +7
# define    SET     MUL  +8
# define    EXT     MUL  +9
# define    EXTU    MUL  +10
# define    MAK     MUL  +11
# define    ROT     MUL  +12

/* control register manipulations */

# define    LDCR    ROT  +1
# define    STCR    ROT  +2
# define    XCR     ROT  +3

# define    FLDCR    ROT  +4
# define    FSTCR    ROT  +5
# define    FXCR     ROT  +6


# define    NOP     XCR +1

/* floating point instructions */

# define    FADD    NOP +1
# define    FSUB    NOP +2
# define    FMUL    NOP +3
# define    FDIV    NOP +4
# define    FSQRT   NOP +5
# define    FCMP    NOP +6
# define    FIP     NOP +7
# define    FLT     NOP +8
# define    INT     NOP +9
# define    NINT    NOP +10
# define    TRNC    NOP +11
# define    FLDC   NOP +12
# define    FSTC   NOP +13
# define    FXC    NOP +14

/*
# define    UEXT(src,off,wid)  ((((unsigned int)src)>>off) & ((1<<wid) - 1))
# define    SEXT(src,off,wid)  (((((int)src)<<(32-(off+wid))) >>(32-wid)) )
# define    MAKE(src,off,wid)  ((((unsigned int)src) & ((1<<wid) - 1)) << off)
*/

# define opword(n) (unsigned int) (memaddr->mem.l)



/* OLD DISASM.H file */


/* Constants and Masks */

#define SFU0       0x80000000
#define SFU1       0x84000000
#define SFU7       0x9c000000
#define RRI10      0xf0000000
#define RRR        0xf4000000
#define SFUMASK    0xfc00ffe0
#define RRRMASK    0xfc00ffe0
#define RRI10MASK  0xfc00fc00
#define DEFMASK    0xfc000000
#define CTRLMASK   0xfc00f800

/* Operands types */

#define HEX          1
#define REG          2
#define IND          3
#define CONT         3
#define IND          3
#define BF           4
#define REGSC        5    /* scaled register */
#define CRREG        6    /* control register */
#define FCRREG       7    /* floating point control register */

/* Hashing Specification */

#define HASHVAL     79

/* Type definitions */

#include "types.h"

/* Structure templates */

typedef struct {
   unsigned int offset:5,
   		width:6,
   		type:5;
} OPSPEC;

struct SIM_FLAGS {
	 int  fu_latency,   /* FU latency (clocks to complete execution) */
	      is_latency,   /* issue latency (clocks to complete issue) */
		 fu_busy,   /* FU busy (clocks that an FU is busy) */
		  wb_pri;   /* writeback slot priority */
   unsigned int	   op:OP,   /* simulator version of opcode */
	     imm_flags:2,   /* 10,16 or 26 bit immediate flags */
	      rs1_used:1,   /* register source 1 used */
	      rs2_used:1,   /* register source 2 used */
	      rsd_used:1,   /* register source/dest used */
		c_flag:1,   /* complement */
		u_flag:1,   /* upper half word */
		n_flag:1,   /* execute next */
	       wb_flag:1,   /* uses writeback slot */
	       dest_64:1,   /* double precision dest */
		 s1_64:1,   /* double precision source 1 */
		 s2_64:1,   /* double precision source 2 */
	    scale_flag:1,   /* register is scaled */
	          user:1,   /* ld, st, xmem to user space */
		 class:5;   /* function unit code */
};

/* SIM_FLAGS.class types */
#define	ILLEGAL_CLASS	0
#define	INT_CLASS	1
#define	LDST_CLASS	2
#define	FMUL_CLASS	3
#define	FADD_CLASS	4
#define	BR_CLASS	5

typedef struct INSTRUCTAB {
   unsigned int  opcode;
   char          *mnemonic;
   OPSPEC        op1,op2,op3;
   struct SIM_FLAGS flgs;
   struct INSTRUCTAB    *next;
} INSTAB;
