static char copyright16[] = "Copyright (c) Motorola, Inc. 1986";


#ifdef m88000
#undef m88000
#endif
#define RNDMODE()   (((unsigned int)(READ_FPCR())>>14)&3)

#define	FLPT_UNIMP_CHK	0x84000000

#define RN   0
#define RZ   1
#define RM   2
#define RP   3

#define SSS  0
#define SSD  1
#define SDS  2
#define SDD  3
#define DSS  4
#define DSD  5
#define DDS  6
#define DDD  7

#define SINGLE  1

#define AFINX   1
#define EFINX   1

#define BEFORE_EXC(temp)	printf("\nBefore error returned %08x\n",temp); 
#define AFT_EXC(temp)		printf("\nAfter error returned %08x\n",temp); 
#define INX_EXC()		printf("\nInexact error returned\n"); 
#define READ_FPCR()             (unsigned int)m88000.SFU1_regs[63]
#define WRITE_FPCR(var)         m88000.SFU1_regs[63] = (unsigned int) var
#define READ_FPSR()             (unsigned int)m88000.SFU1_regs[62]
#define WRITE_FPSR(var)         m88000.SFU1_regs[62] = (unsigned int) var

#define FPSIZE(ir)	((((ir->p->flgs.dest_64)?1:0)<<2) | \
			(((ir->p->flgs.s1_64)?1:0)<<1) | \
			((ir->p->flgs.s2_64)?1:0)) 

#define BFEXTU(src,off,wid)   (((unsigned int)src>>off)&((1<<wid)-1))
#define BFEXTS(src,off,wid)   (((int)src>>off)&((1<<wid)-1))
#define CARRY(a,b)   ((((unsigned int)a+(unsigned int)b)<(unsigned int)a) || (((unsigned int)a+(unsigned int)b)<(unsigned int)b))
#define DZERO(ahi,alo)        ((ahi == 0) && (alo == 0))
#define DGE(ahi,alo,bhi,blo)  ((ahi>bhi) || ((ahi == bhi) && (alo>=blo)))
#define DSUB(dhi,dlo,ahi,alo,bhi,blo) \
	 if (CARRY(alo,1)) {             \
	    dlo = alo + 1;               \
	    dlo += ~blo;                 \
	    dhi = ahi + ~bhi + 1;        \
	 }                               \
	 else {                          \
	    dlo = alo + 1;               \
	    if (CARRY(dlo,~blo)) {       \
	       dlo += ~blo;              \
	       dhi = ahi + ~bhi + 1;     \
	    }                            \
	    else {                       \
	       dlo = alo + ~blo;         \
	       dhi = ahi + ~bhi;         \
	    }                            \
	 }                               \

#define DRSH(ahi,alo,cnt) alo >>= cnt; alo |= ahi<<(32-cnt); ahi >>= cnt;
#define DLSH(ahi,alo,cnt) ahi <<= cnt; ahi |= alo>>(32-cnt); alo <<= cnt;


typedef struct {
   unsigned int sign;
   int  exp;
   unsigned int mant;
} FPNUM;

typedef struct {
   unsigned int sign;
   int  exp;
   unsigned int manthi;
   unsigned int mantlo;
} FPLONG;

#define MINEXP       0
#define MAXEXP       0xff
#define MINEXPD      0
#define MAXEXPD      0x7ff
#define SIGNMASK     0x80000000
#define VBITMASK     0x01000000
#define VBITMASKD    0x00200000
#define HBITMASK     0x00800000
#define HBITMASKD    0x00100000
#define MANTMASK     0x007fffff
#define MANTMASKD    0x000fffff

#define NORMAL       0
#define INF          1
#define NAN          2
#define DENORM       4
#define	UNIMP	     5
#define CONVOVF      8
#define DIVZERO      16
#define NEGSQRT      32

#define INEXACT      1<<16
#define OVERFLOW     2<<16
#define UNDERFLOW    4<<16

#define FLAGWIDTH    8


/* precise control register masks */

#define PCR_RESMASK    0x0000FFFF
#define PCR_OPERMASK   0x0000FFE0
#define PCR_DESTMASK   0x0000001F
#define HIGH20BITS     0x007FFFF8

#define	SRC1SINGLE	0x00000200
#define	SRC2SINGLE	0x00000080
#define	DESTSINGLE	0x00000020


/* imprecise control register masks */

#define IMPCR_RESMASK    0xFFF0FFFF
#define IMPCR_OPERMASK   0x0000F800
#define IMPCR_DESTMASK   0x0000001F
#define IMPCR_DSIZEMASK  0x00000020

/* floating point exception cause register masks */

#define FINX  0x00000001
#define FOVF    0x00000002
#define FUNF    0x00000004
#define FIOV    0x00000080
#define FDVZ    0x00000008
#define FPRV    0x00000020
#define FINV    0x00000010
#define FUNIMP  0x00000040

