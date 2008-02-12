/* @(#) File name: cmmu.h   Ver: 3.1   Cntl date: 1/20/89 14:38:51 */
/********************************************************************/
/*           DCMMU and ICMMU CACHE structures                       */
/********************************************************************/

#define WPL		4	/* DATA WORDS PER LINE  */
#define SETS		256	/* NO. OF SETS IN CACHE */
#define LPS		4	/* LINES PER SET        */
#define BATC_SIZE	10	/* Number of entries in BATC */
#define PATC_SIZE	56	/* Number of entries in PATC */

	/* cache masks */

#define SETMASK		0x00000ff0
#define DATAWRDMASK	0x0000000c
#define ADDRTAGMASK	0xfffff000


	/* descriptor masks */

#define V_BMASK		0x00000001	/*  V bit (Valid) mask */
#define BH_BMASK	0x00000002	/*  BH bit (BATC hit) mask */
#define WP_BMASK	0x00000004	/*  WP bit (write protection) mask */
#define U_BMASK		0x00000008	/*  U bit (Used) mask */
#define M_BMASK		0x00000010	/*  M bit (modified) mask */
#define CI_BMASK	0x00000040	/*  CI bit (cache inhibit) mask */
#define G_BMASK		0x00000080	/*  G bit (snoop enable) mask */
#define SP_BMASK	0x00000100	/*  SP bit (supervisor protect) mask */
#define WT_BMASK	0x00000200	/*  WT bit (write through) mask */
#define BE_BMASK	0x00004000	/*  BE bit (M bus error) mask */
#define CE_BMASK	0x00008000	/*  CE bit (copy back error) mask */
/*
#define GBITMASK	0x00000080
#define SBITMASK	0x00000100
#define WBITMASK	0x00000004
#define CBITMASK	0x00000040
#define MBITMASK	0x00000010
#define UBITMASK	0x00000008
#define VALIDMASK	0x00000001
#define TBITMASK	0x00000200
#define BHBITMASK	0x00000002
*/
	/* Status Register Exception Status Encodings */

#define EXCEPT_CLR	0xffff0fff
#define BUS_ERROR	0x00030000	/* Bus Error */
#define SEGMENT_FAULT	0x00040000	/* Segment Fault */
#define PAGE_FAULT	0x00050000	/* Page Fault */
#define SUPER_VIOLATION	0x00060000	/* Supervisor Violation */
#define WRITE_VIOLATION	0x00070000	/* Write Viloation */
/*
#define INV_PD_SV	0x0000a000
#define INV_SD_SV	0x0000b000
#define WR_PROTECT_VIO	0x0000f000
*/

	/* Control Register Offsets */

#define IDR_ro		0x0	/* CMMU ID Register */
#define SCMR_ro		0x4	/* CMMU Command Register */
#define SSR_ro		0x8	/* System Satus Register */
#define SADR_ro		0xc	/* System Address Register */
#define SCTR_ro		0x104	/* System Control Register */

#define LSR_ro		0x108	/* Local Status Register */
#define LADR_ro		0x10c	/* Local Address Register */

#define SAPR_ro		0x200	/* Supervisor Are Pointer Register */
#define UAPR_ro		0x204	/* User Area Pointer Register */

#define BWP0_ro		0x400	/* Block ATC Write Port 0 */
#define BWP1_ro		0x404	/* Block ATC Write Port 1 */
#define BWP2_ro		0x408	/* Block ATC Write Port 2 */
#define BWP3_ro		0x40c	/* Block ATC Write Port 3 */
#define BWP4_ro		0x410	/* Block ATC Write Port 4 */
#define BWP5_ro		0x414	/* Block ATC Write Port 5 */
#define BWP6_ro		0x418	/* Block ATC Write Port 6 */
#define BWP7_ro		0x41c	/* Block ATC Write Port 7 */

#define CACHE_ro	0x800	/* Start of CACHE DATA Ports */
#define CDP0_ro		0x800	/* CACHE Data Port 0 */
#define CDP1_ro		0x804	/* CACHE Data Port 1 */
#define CDP2_ro		0x808	/* CACHE Data Port 2 */
#define CDP3_ro		0x80c	/* CACHE Data Port 3 */

#define TAG_ro		0x840	/* Start of CACHE TAG Ports */
#define CTP0_ro		0x840	/* CACHE Tap Port 0 */
#define CTP1_ro		0x844	/* CACHE Tap Port 1 */
#define CTP2_ro		0x848	/* CACHE Tap Port 2 */
#define CTP3_ro		0x84c	/* CACHE Tap Port 3 */

#define CSSP_ro	0x880	/* CACHE Set Status Port */

/*	Old CMMU Register MAP (Rev. 2.0 )
#define ID_ro		0x0
#define CR_ro		0x4
#define SR_ro		0x8
#define ADR_ro		0xc
#define PBR_ro		0x100
#define PMR_ro		0x104
#define SAPR_ro		0x200
#define UAPR_ro		0x204
#define BWP0_ro		0x400
#define BWP1_ro		0x404
#define BWP2_ro		0x408
#define BWP3_ro		0x40c
#define BWP4_ro		0x410
#define BWP5_ro		0x414
#define BWP6_ro		0x418
#define BWP7_ro		0x41c
#define CACHE_ro	0x800
#define TAG_ro		0x840
#define LRU_KILL_VAL_ro	0x880
*/

	/* Cache Control Register Offset Mask */

#define CCR_OFFSET_MASK	0x800
#define SELECT_CCR_MASK	0x8c0

	/* Other Cache Control Register Masks */

#define CSSPZEROBITS	0x3ffff000
#define ZEROKILLVALID	0x3f000000

#define VVLINE0		0x00003000
#define VVLINE1		0x0000c000
#define VVLINE2		0x00030000
#define VVLINE3		0x000c0000

#define KILLLINE0	0x00100000
#define KILLLINE1	0x00200000
#define KILLLINE2	0x00400000
#define KILLLINE3	0x00800000

	/* CMMU status register masks */

#define SR_GBIT		0x00000080    /*  G bit mask */


	/* CMMU control space masks */

#define CTRL_SPACE	0xfff00000


	/* cache valid bits */

#define INV	3	/* invalid  */
#define SU	2	/* shared unmodified */
#define EU	0	/* exclusive unmodified */
#define EM	1	/* exclusive modified */
#define UNMOD	2	/* unmodified */
#define MOD	0	/* modified */

struct cache_line
{
	unsigned char	vv;		/* valid bits  */
	unsigned int	tag;		/* address tag */
	int		data[WPL];	/* data words  */
	int		order;		/* lru order   */
	int		kill;		/* kill status */
};

struct cache_table
{
	struct cache_line	cache_entry [SETS] [LPS];
	int			cache_accessed [SETS];
};

struct BATC_entry
{
		   unsigned int LBA;	/* logical block address */
		   unsigned int PBA;	/* physical block address */
		   int T, G, C, W, V;	/* T = write through bit,
					 * G = snoop enable bit,
					 * C = cache inhibit bit,
					 * W = write protect bit,
					 * V = valid bit
					 */
};

struct BATC_table
{
	struct BATC_entry batc_entry[BATC_SIZE];
};


struct PATC_entry
{
	unsigned int	LPA;			/* logical page address */
	unsigned int	PFA;			/* physical frame address */
	int		S,			/* S  - Supervisor Mode */
			WT,			/* WT - Writethrough */
			G,			/* G  - Global */
			CI,			/* CI - Cache Inhibit */
			M,			/* M  - Modified */
			WP,			/* WP - Write Protect */
			V;			/* V  - Valid */
	int		fifo_cnt;		/* entry's fifo counter */
};

struct PATC_table
{
	struct PATC_entry	patc_entry[PATC_SIZE];
};


struct cmmu_regs
{
	unsigned int	IDR;	/* CMMU ID register */
	unsigned int	SCMR;	/* System command register */
	unsigned int	SSR;	/* System status register */
	unsigned int	SADR;	/* System address register */
	unsigned int	SCTR;	/* System control register */
	unsigned int	LSR;	/* Local status register */
	unsigned int	LADR;	/* Local address register */
	unsigned int	SAPR;	/* Supervisor area pointer register */
	unsigned int	UAPR;	/* User area pointer register */
	unsigned int	BWP[8];	/* BATC write ports (0-7) */
	unsigned int	CSSP;	/* Cache set status port */
};

/*	Old cmmu_regs (Rev 2.0)
struct cmmu_regs
{ */
/*	unsigned int	ID;	 cmmu ID register */
/*	unsigned int	CR;	 cmmu control register */
/*	unsigned int	SR;	 cmmu status register */
/*	unsigned int	ADR;	 cmmu address register */
/*	unsigned int	UAPR;	 User Area Pointer register */
/*	unsigned int	SAPR;	 Supervisor Area Pointer register */
/*	unsigned int	BWP[8];	 Block Bypass write port 0-7 */
/*	unsigned int	LKV;	 Lru-Kill-Valid register */
/*}; */

	/* cmmu statistic variables */

struct cmmu_stats
{
	int	num_wchwr1;	/* # of write cache hits needing write once */
	int	num_wcb;	/* # of write cache hits requiring copy_back */
	int	num_rcb;	/* # of read cache hits requiring copy_back */
	int	num_tblwks;	/* # of tablewalks */
	int	num_tblwks_UM;	/* # of tablewalks with U & M bit update */
	int	num_BATC_chk;	/* # of BATC checks */
	int	num_BATC_hits;	/* # of BATC hits */
	int	num_PATC_chk;	/* # of PATC checks */
	int	num_PATC_hits;	/* # of PATC hits */
	int	num_probes;	/* # of probes */
};

struct cmmu
{
	int			S_U;	/*  supervisor/user logical ext. bit */
	struct cmmu_regs	control;/*  cmmu control space  */
	struct BATC_table	batc;	/*  block address translation cache */
	struct PATC_table	patc;	/*  page address translation cache */
	struct cache_table	cache;
	int			IorD;	/*  Flag to implement code or data */
	struct cmmu_stats	stats;	/*  cmmu statistics */
};


struct cmmu	Dcmmu;
struct cmmu	Icmmu;
struct cmmu	*cmmu_ptr;

	/* cache statistics variables */

int ICcnt;             /* number of fetches from Instruction cache */
int DCcnt;             /* number of loads and stores to the Data cache */
int IChit;             /* number of hits from Instruction cache */
int DChit;             /* number of hits from/to Data cache */
int LDCcnt;            /* number of load attempts from the Data cache */
int SDCcnt;            /* number of store attempts to the Data cache */
int LDChit;            /* number of hits from Data cache for loads */
int SDChit;            /* number of hits to Data cache for stores */

	/* cmmu timing characteristics */

#define	TIME_RCTL		6	/* read from control register = 9 */
#define	TIME_WCTL		4	/* write from control register = 5 */

#define	TIME_XBATC		1	/* translate address with BATC hit */
#define	TIME_XPATC		1	/* translate address with PATC hit */

#define	TIME_RMEMACC		3	/* read memory access = 6 */
#define	TIME_WMEMACC		5	/* write memory access = 6 */

#define	TIME_RCH		0	/* read cache hit = 1 */
#define	TIME_RCMWOCB		7	/* read cache miss w/o copyback = 10 */
#define	TIME_RCMWCB		8	/* read cache miss w copyback = 18 */

#define	TIME_WCH		0	/* write cache hit */
#define	TIME_WCHWR1		9	/* write cache hit w write once = 10 */
#define	TIME_WCMWOCB		15	/* write cache miss w/o copyback = 16 */
#define	TIME_WCMWCB		6	/* write cache miss w copyback = 22 */

#define	TIME_TBLWK		12	/* tablewalk */
#define	TIME_TBLWKWUM		4	/* tablewalk w/ U & M bit update = 16 */

#define	ADDEDLDTIME 		3	/* more time needed for inst. loads*/
#define	CONCURRENTFACTOR	1	/* constant assumed to take in to
					 * effect the concurrency of driving
					 * address and decoding past
					 * instruction.
					 */
