/* @(#) File name: main.c   Ver: 3.1   Cntl date: 1/20/89 14:19:55 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";
/*****************************************************************************
*
*
*       Functions contain in this module are:
*
*               main()          Is the controlling function
*
*/



        /**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/
#include "functions.h"


	/**********************************************************
	*
	*       External Variables Definition
	*
	*/
#include "defines.h"
#include <string.h>
#include <stdio.h>

extern	int 	cmdcount;
extern	int	interrupt;
extern	int	quit;
extern	int	buserr;
extern	int	segviol;
extern	int	silent;
extern	int	vecyes;
extern	int	unixvec;
extern	int	sym_addr;
extern	int	pipe_open;
extern	char	Vers[];
extern	char	*cmdptrs[];
extern	struct dirtree	dir[];

/*
** disable BSD - system call vectors
**
*/

extern	int	dis_word_align;
extern	int	usecmmu;
extern	int	outputflag;
extern	int	debugflag;	/* cmmu debug flag */


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

char	cmdbuf[BSIZE] = {'\0'};
char	ccmdbuf[BSIZE] = {'\0'};
int	debug = 0;
int	bigendian = 1;
FILE	*pipe_strm;

	/**********************************************************
	*
	*      Messages
	*/

char    prompt[]  = { "sim>> " };


	/*********************************************************************
	*
	*       This function is where all other function are run from.
	*
	*	command line options:
	*	c		Enable CMMU simulation.
	*	d		Read a "core88" file and restore the simulator
	*			to the point of error.
	*	j		Enable debugging messages. (intended for
	*			internal use only)
	*	r <prog> [args]	Load and run <prog>.  Optionally pass [args]
	*			to simulated program.
	*	u
	*	v
	*	H		The the address portion of the tz trace
	*			in HEX instead of using symbolic addressing.
	*	P <filter>	Pipe the tz trace output through <filter>.
	*			ex. -P "cat > tz.out" will save the trace
	*			output to the file tz.out.
	*	Z		Assume that all character strings are aligned
	*			on word boundaries.  The default is to assume
	*			that all character strings are not aligned.
	*	O
	*	s <file>	Execute the simulator commands in <file>
	*			on startup.
	*/

int main(int argc, char **argv)
{

	struct cmdstruct *func;
	int c, errflg = 0;
	extern	int	optind;
	extern	char	*optarg;
	char *setupfile = 0;

	while((c = getopt(argc, argv, "cdljruvHP:ZOps:")) != EOF)
	{
		switch(c)
		{
			case 'c':
				usecmmu = 1;
				break;

			case 'l':
				bigendian = 0;
				break;

			case 'H':
				sym_addr = 0;
				break;

			case 'j':
				debugflag = 1;
				break;

			case 'u':
				unixvec = 0;
				break;

			case 'v':
				vecyes = 1;
				break;

			/*
			** character strings word aligned
			*/
			case 'Z':
				dis_word_align = 1;
				break;

			case 'r':
				silent = 1;
				break;

			case 's':
				setupfile = optarg;
				break;

			case 'd':
				debug = 1;
				break;

			case 'O':
				outputflag = 1;
				break;

			case '?':
			default:
				errflg++;
				break;
		}
	}

	if(errflg)
	{
		fprintf(stderr, "usage: %s [-vu] [-s file] [file]\n", argv[0]);
		fprintf(stderr, "usage: %s [-vu] [-s file] -r file [arg1...arg18]\n", argv[0]);
		fprintf(stderr, "usage: %s -d\n", argv[0]);
		fflush (stderr);
		exit(2);
	}

	init_disasm();
	init_processor();		/* in ctlregs.c */
	cmmu_init();

/* PPrintf("M88000 Simulator, %s\n", Vers); */

	if(getmem(0, 0x3fff, M_SEGANY, 0) != 0)
	{
		printf("Can not open memory\n");
		exit(2);
	}

	if(setupfile)
		presetsim(setupfile);

	if(optind < argc)
	{
		cmdptrs[1] = argv[optind];	/* set up to load file */
		if((lo() != -1) && silent)
		{
			errflg = runsilent(argc, argv, ++optind);
			exit(0); /* Pre-SPEC, this was exit(errflg); */
		}
	}

	if(outputflag)
	{
		if(!usecmmu)
			outputflag = 0;
		else
		{
			if(optind < argc)
				open_output(cmdptrs[1]);
			else
				open_output("m88k");
		}
	}

	silent = 0;

	if(debug)
	{
		loadcore();
		debug = 0;
	}

	sig_set();

	while(1)                           /* Stand Alone main command loop */
	{
		char *fred;
		interrupt = quit = buserr = segviol = 0;

		PPrintf(prompt);

		fred=FFgets(cmdbuf, BSIZE, stdin);

		if(fred != NULL)
						/* Get users response */
		{
			strcpy(ccmdbuf, cmdbuf);		
			/* Save cmdbuf contents in ccmdbuf for the case when
			 * func = run.  The command line will then be expanded
			 * by the shell before further processing.  */

			if((func = parse(cmdbuf, dir)) != '\0')   
				/* Check for valid command */
			{
				if(func->funct != 0)
					(func->funct)();
				else if((strcmp(func->cmd, "sh") == 0) || (strcmp(func->cmd, "!") == 0))
				{
					c = 0;
					if(*func->cmd == '!')
						c++;
					while(*cmdptrs[++c] != '\0')
						*(cmdptrs[c]-1) = ' ';
					cmdptrs[c] = 0;
					if(*func->cmd == '!')
						c = 1;
					else
						c = 0;
					if(system(cmdptrs[c]) != 0)
						PPrintf("Unable to perform command\n");
				}
				PPrintf("\n");
			}
		}
		else
			clearerr(stdin);
	}
	return 0;
}


int presetsim(char *file)
{
	struct cmdstruct *func;
	FILE *setfile;
	int silentsave = silent;

	if((setfile = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "Unable to open set up file \"%s\"\n", file);
		fflush (stderr);
		return(-1);
	}

	silent = 1;

	while(1)  
	{
		if(fgets(cmdbuf, BSIZE, setfile) != '\0')
		{
			if((func = parse(cmdbuf, dir)) != '\0')   /* Check for valid command */
			{
				if(func->funct != 0)
					(func->funct)();
			}
		}
		else
			break;
	}

	silent = silentsave;
	return(0);
}
