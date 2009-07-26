/* @(#) File name: run.c   Ver: 3.2   Cntl date: 3/10/89 10:40:38 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

#include "functions.h"
#include "trans.h"
#include "defines.h"
#include "symbols.h"
#include "br.h"
#include "core88.h"
#include <string.h>


	/**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/



	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	char	*cmdptrs[];

extern char *load_program;

extern	int	cmdcount;
extern	int	cmdflags;
extern	struct	brkpoints brktable[TMPBRK];
extern	unsigned int curbrkaddr;
extern	int	debug;
extern	int	unixvec;
extern	int	vecyes;
extern	int	usecmmu;
extern	struct	cmmu Dcmmu, Icmmu;
extern  int	sID;

	/**********************************************************
	*
	*       Local Defines
	*
	*/


	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/

int	argc = 0;
char	*argv[MAXCMD] = {0};
char	args[BSIZE+10] = {'\0'};
unsigned int startadr;

static char	*cmd_argv[MAXCMD] = {0};
static int	cmd_argc;

extern int prog_loaded;

	/**********************************************************
	*
	*      Messages
	*/


	/**********************************************************
	*
	*      run()
	*/



int run(void)
{
	unsigned int	r31;
	unsigned int 	argvptrs[MAXCMD];
	int 		strsize, cnt, idx;

	extern char		cmdbuf[];
	extern char		ccmdbuf[];
	extern struct dirtree	dir[];

	r31 = STKBASE+STKSIZE;

	if(argc == 0)
	{
		PPrintf("No program loaded\n");
		return(-1);
	}

	if(cmdcount != 1)
	{
#ifdef NOT_SPEC95
				/* Expand the cmdbuf line using the
				 * shell (echo) and reparse.
				 */
		expand(ccmdbuf, cmdbuf);
		parse(cmdbuf, dir);

		argv[1] = strchr(args, '\0') + 1;
		cmd_argv[1] = strchr(args, '\0') + 1;
		for(argc = 1; argc <= cmdcount; argc++)
		{
			strcpy(argv[argc], cmdptrs[argc]);
			strcpy(cmd_argv[argc], cmdptrs[argc]);
			argv[argc+1] = strchr(argv[argc], '\0') + 1;
			cmd_argv[argc+1] = strchr(cmd_argv[argc], '\0') + 1;
		}
		argc--;
#else
		PPrintf("run command not accepted in SPEC95\n");
		return(-1);
#endif
	}

	cmd_argc = argc - 1;
	for(idx = (cnt = argc) - 1; cnt; cnt--, idx--)
	{
		strsize = strlen(argv[idx]) + 1;
		r31 = (r31 - strsize) & 0xFFFFFFFC;
		argvptrs[idx] = r31;
		if(rdwr((M_SEGANY | M_WR), r31, argv[idx], strsize) == -1)
			return(-1);
	}

	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &cnt, 4) == -1)  /*write NULL to stack */
		return(-1);


	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &cnt, 4) == -1)  /*write NULL to stack */
		return(-1);

	for(idx = (cnt = argc) - 1; cnt; cnt--, idx--)
	{
		if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &argvptrs[idx], 4) == -1)
			return(-1);
	}

	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &argc, 4) == -1)
		return(-1);

	sysVclose();			/* close all open fildes */
	m88000.Regs[31] = r31;

	m88000.Regs[2] = STKBASE;

	m88000.Regs[2] = argc;

/*
** argv is located just above argc (hence the + 4)
*/

	m88000.Regs[3] = r31 + 4;

/*
** envp is located past the NULL after the last argv pointer
** hence the argc * 4 (number of argv pointers) + 4 (the NULL)
** the firt + 4 is to bump past argc
*/

	m88000.Regs[4] = r31 + 4 + (argc * 4) + 4;

	PPrintf("Effective address: %08X\n", IP = startadr);
	brkptenb();
	return(goexec());
}


void setargs(char *ptr,int execaddr)
{
	strcpy(args, ptr);
	argv[0] = args;
	startadr = execaddr;
	argc = 1;
}


int runsilent(int argc,char **argv,int curargc)
{
	if(argc > MAXCMD-1)
	{
		printf("Simulator run program, invoked with to many args (%d limit)\n", MAXCMD);
		return(1);
	}

	for(cmdcount = 1; curargc < argc; curargc++, cmdcount++)
	{
		cmdptrs[cmdcount] = argv[curargc];
	}

	cmdptrs[cmdcount] = "";

	if(run() == -1)
	{
		(void)dumpcore();
		exit(1);
	}

	return(m88000.Regs[2]);
}

int dumpcore(void)
{
	extern struct symbols *symtable;
	extern void *nametable;

	struct core88 core;
	register int i = 0, x, y, z;
	FILE *core88;
	char outbuf[0x4000];
	struct symbols *sym = symtable;

	fprintf(stderr, "Dumping core into file \"core88\"\n");

	core.pstate = m88000;	/* get registers */
	core.Icmmu = Icmmu;	/* get instruction CMMU */
	core.Dcmmu = Dcmmu;	/* get data CMMU */

	for(i = 0; i < MAXSEGS; i++)
		core.mem[i] = memory[i];

	core.vecyes = vecyes;
	core.unixvec = unixvec;
	core.usecmmu = usecmmu;
	core.argc = argc;

	for(i = 0; i < MAXCMD; i++)
		core.argv[i] = argv[i];

	for(i = 0; i < BSIZE+10; i++)
		core.args[i] = args[i];

	for(i = 0; i < TMPBRK; i++)
		core.brktable[i] = brktable[i];

	core.curbrkaddr = curbrkaddr;
	core.startadr = startadr;
	core.nmtbsz = core.syms = 0;

	while(sym && ++core.syms && sym->sym)
	{
		core.nmtbsz += (strlen(sym->sym) + 1);
		sym->sym -= (int)nametable;
		sym++;
	}

	if((core88 = fopen("core88", "w")) == NULL)
	{
		fprintf(stderr, "Can not open \"core88\" for writing\n");
		return 0;
	}

	if(fwrite(&core, sizeof(struct core88), 1, core88) == 0)
	{
		fprintf(stderr, "Can not write to \"core88\"\n");
		fclose(core88);
		return 0;
	}

	core.sID = sID;

	for(i = 0; i < MAXSEGS; i++)
	{
		if(memory[i].seg == NULL)
			continue;
		trans.adr1 = memory[i].physaddr;
		x = memory[i].phyeaddr - memory[i].physaddr;
		do
		{
			y = (x < 0x4000) ? x : 0x4000;		/* size for buffer */
 			if(rdwr((M_SEGANY | M_RD), trans.adr1, outbuf, y) == -1)	/* move */
				return 0;
			if((z = fwrite(outbuf, sizeof(char), y, core88)) == -1)
			{
				fprintf(stderr, "Can not write to \"core88\"\n");
				return 0;			/* if error return error */
			}
			trans.adr1 += z;			/* update memory pointer */
		}
		while((z == y) && ((x -= z) > 0));		/* go until done */
	}

	if(core.syms && (fwrite(symtable, sizeof(struct symbols), core.syms, core88) == 0))
	{
		fprintf(stderr, "Can not write to \"core88\"\n");
		return 0;			/* if error return error */
	}

	if(core.syms && (fwrite(nametable, sizeof(char), core.nmtbsz, core88) == 0))
	{
		fprintf(stderr, "Can not write to \"core88\"\n");
		return 0;			/* if error return error */
	}

	fclose(core88);
	return 0;
}


void loadcore(void)
{
	extern struct symbols *symtable;
	extern void *nametable;
	struct core88 core;
	register int i = 0, x, y, z;
	FILE *core88;
	char outbuf[0x4000];
	struct symbols *sym;

	if((core88 = fopen("core88", "r")) == NULL)
	{
		fprintf(stderr, "Can not open \"core88\" for reading\n");
		return;
	}

	if(fread(&core, sizeof(struct core88), 1, core88) == 0)
	{
		fprintf(stderr, "Can not read \"core88\"\n");
		fclose(core88);
		return;
	}

	printf("Loading core image for \"core88\"\n");

	m88000 = core.pstate;	/* get registers */
	Icmmu = core.Icmmu;	/* get instruction CMMU */
	Dcmmu = core.Dcmmu;	/* get data CMMU */
	vecyes = core.vecyes;
	unixvec = core.unixvec;
	usecmmu = core.usecmmu;
	argc = core.argc;

	for(i = 0; i < MAXCMD; i++)
		argv[i] = core.argv[i];

	for(i = 0; i < BSIZE+10; i++)
		args[i] = core.args[i];

	for(i = 0; i < TMPBRK; i++)
		brktable[i] = core.brktable[i];

	curbrkaddr = core.curbrkaddr;
	startadr = core.startadr;
	releasemem();
	sID = core.sID;

	for(i = 0; i < MAXSEGS; i++)
	{
		if(core.mem[i].seg == NULL)
			continue;
		x = core.mem[i].endaddr - core.mem[i].baseaddr;
		if(getmem(core.mem[i].baseaddr, x, core.mem[i].flags, core.mem[i].physaddr) != 0)
		{
			printf("Can not open memory\n");
			perror("loadcore");
			return;
		}
		trans.adr1 = core.mem[i].physaddr;
		do
		{
			y = (x < 0x4000) ? x : 0x4000;		/* size for buffer */
			if((z = fread(outbuf, sizeof(char), y, core88)) == 0)
			{
				fprintf(stderr, "Can not read \"core88\"\n");
				return;
			}
 			if(rdwr((M_SEGANY | M_WR), trans.adr1, outbuf, y) == -1)	/* move */
				return;
			trans.adr1 += z;			/* update memory pointer */
		}
		while((z == y) && ((x -= z) > 0));		/* go until done */
	}

	if(core.syms == 0)
		return;

	if((symtable = (struct symbols *)malloc(core.syms * sizeof(struct symbols))) == 0)
	{
		perror("loadcore:");
		return;
	}

	if((nametable = calloc(core.nmtbsz, sizeof(char)))== 0)
	{
		perror("loadcore:");
		symtable = 0;
		return;
	}

	if(fread(symtable, sizeof(struct symbols), core.syms, core88) != core.syms)
	{
		fprintf(stderr, "Can not read \"core88\"\n");
		perror("loadcore:");
		symtable = 0;
		return;
	}

	if(fread(nametable, sizeof(char), core.nmtbsz, core88) == 0)
	{
		fprintf(stderr, "Can not read \"core88\"\n");
		perror("loadcore:");
		symtable = 0;
		return;
	}

	sym = symtable;

	for(i = 0; i < (core.syms - 1); i++, sym++)
		sym->sym += (int)nametable;

	dm();

	if(debug)
	{
		Data_path();
		rdexec(0);
	}

	fclose(core88);
	return;
}


int rstsys(void)
{
	fflush(stdout);fflush(stderr);
	init_processor();


	if(usecmmu)
		cmmu_init();
	return 0;
}

int cacheoff(void)
{
/* Spec special to set usecmmu off to combine workloads */
	usecmmu = 0;
	return 0;
}

	/**********************************************************
	*
	*      rrn()
	*/


int rrn(void)
{
	unsigned int r31;
	int strsize, cnt, idx;
	unsigned int argvptrs[MAXCMD];

	r31 = STKBASE+STKSIZE;

	if (cmd_argc == 0 && !prog_loaded)
	{
		PPrintf("No program loaded\n");
		return(-1);
	}

	PPrintf("Argcnt = %d, Args:", cmd_argc);

	for (cnt = 1; cnt < cmd_argc; cnt++)
	{
		PPrintf(" %s", cmd_argv[cnt]);
	}

	PPrintf("\n");

	for(idx = (cnt = cmd_argc) - 1; cnt; cnt--, idx--)
	{
		strsize = strlen(argv[idx]) + 1;
		r31 = (r31 - strsize) & 0xFFFFFFFC;
		argvptrs[idx] = r31;
		if(rdwr((M_SEGANY | M_WR), r31, argv[idx], strsize) == -1)
			return(-1);
	}

	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &cnt, 4) == -1)  /*write NULL to stack */
		return(-1);


	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &cnt, 4) == -1)  /*write NULL to stack */
		return(-1);

	for(idx = (cnt = cmd_argc) - 1; cnt; cnt--, idx--)
	{
		if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &argvptrs[idx], 4) == -1)
			return(-1);
	}

	if(rdwr((M_SEGANY | M_WR), (r31 -= 4), &cmd_argc, 4) == -1)
		return(-1);

	rstsys();			/* reset processor and CMMU */
	sysVclose();			/* close all open fildes */
	m88000.Regs[31] = r31;

	m88000.Regs[2] = STKBASE;

	m88000.Regs[2] = cmd_argc;

/*
** argv is located just above argc (hence the + 4)
*/

	m88000.Regs[3] = r31 + 4;

/*
** envp is located past the NULL after the last argv pointer
** hence the argc * 4 (number of argv pointers) + 4 (the NULL)
** the firt + 4 is to bump past argc
*/

	m88000.Regs[4] = r31 + 4 + (cmd_argc * 4) + 4;

	PPrintf("Effective address: %08X\n", IP = startadr);
	brkptenb();
	return(goexec());
}

void cp_ptrs(char **p1, char **p2,int count)
{
	int	i;
	for (i=0; i <= count; i++)
		p1[i] = p2[i];
}


#ifdef NOT_SPEC95

void expand(char *line1,char *line2)
{
/* the 3 lines below really need inclusions of stdio.h and unistd.h */
	FILE	*popen();
	int	pclose();
	FILE	*st;
	char	eln[BSIZE];

	strcpy(eln, "echo ");
	strcat(eln, line1);

	if ((st = popen(eln, "r")) == NULL)
	{
		perror("expand: popen");
		exit(0);
	}

	fgets(line2, BSIZE, st);
	pclose(st);
} 
#endif
