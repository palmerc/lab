/* @(#) File name: core88.h   Ver: 3.1   Cntl date: 1/20/89 14:38:56 */
/*****************************************************************************
*
*       This is the core dump structure for the simulator in the run silent mode
*
*                                          Date Last Modified:
*/

struct core88 
{
	struct PROCESSOR pstate;
	struct cmmu Icmmu, Dcmmu;
	struct mem_segs mem[MAXSEGS];
	int	sID;
	int	vecyes;
	int	unixvec;
	int	usecmmu;
	int	argc;
	char	*argv[MAXCMD];
	char	args[BSIZE+10];
	int	startadr;
	struct brkpoints brktable[TMPBRK];
	unsigned int curbrkaddr;
	int	syms;
	int	nmtbsz;
};
	
