/* @(#) File name: lo.c   Ver: 3.3   Cntl date: 2/19/89 12:49:55 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

#include "functions.h"
#include "exec.h"
#include "filehdr.h"
#include "aouthdr.h"
#include "trans.h"
#include "defines.h"
#include <string.h>


	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	char	*cmdptrs[];
extern	int	cmdcount;
extern	int	sID;
extern	int	stdio_en;
extern	int	unixvec;


	/**********************************************************
	*
	*       Local Defines
	*
	*/

#define	BUFSIZE	(1024)
static char load_program[BUFSIZE];	/* Contains the coff file name*/


/*
*******************************************************************************
*
*       Module:     lo()
*
*       File:       lo.c
*
*******************************************************************************
*
*  Functional Description:
*	Loads in the specified coff or a.out format file.  Allocates memory
*	for the program and creates a symbol table.  
*
*	The filename is stored in the local static character array 
*	"load_program".  If a subsequent "lo" instruction is issued without
*	a filename, the previous name will be used.
*
*  Globals:
*	cmdcount	Indicates the number of arguments entered from the
*			simulator command line.  Cmdcount is usually equal
*			to 2, the "lo" command and the filename.
*	cmdptrs[]	Contains pointers to the arguments entered from the
*			command line.  cmdptrs[0] will point to the "lo"
*			command and cmdptrs[1] will point to the filename
*			string.
*	sID		True is sections were loaded from the coff.
*	stdio_en	Flag set by the lo() function.  True if Sim IO
*			type exception vectors are enabled.
*	unixvec		Command line switch (-u).  Specifying the (-u) option
*			makes this flag false.  If true, the simulator will
*			interpret exception vectors in the range of 128-507 as
*			Unix calls (Sim IO or SysV IO).  If false, the user
*			may use this range of vectors as he pleases.
*
*  Parameters:
*
*  Output:
*	returns a (-1) if a serious error has occured.
*	returns a (0) otherwise
*
*
*******************************************************************************/

int lo(void)
{

	FILE *fp;				/* file descriptor pointer */


	/* COFF pointers and buffers */

	FILHDR	fhdr;			/* will contain the file header */
	SCNHDR	*shdr, *sptr;		/* holder buffer pointer and work */
					/*   pointer for section headers */


	struct syment	*symptr;	/* pointer to symbol table buffer */
	AOUTHDR	*aptr = 0, aoutheader;	/* holding buffer for the a.out */
					/*   section header */


	/* a.out format pointers and buffers */

	struct exec	*aout;


	/* general variables */

	unsigned int		fileoffset;
	unsigned int		sectcnt, ldcnt, ldtot;
	char			ldbuf[BUFSIZE];	/* download buffer */
	struct string_table	flexname;


	/* Do we have a filename to load from */
	if(cmdcount == 1)
	{
		if (load_program[0] == '\0')
		{
			Eprintf("lo: needs a file name to load\n");
			return(-1);
		}

		if((fp = fopen(load_program, "rb")) == NULL)
		{
			Eprintf("lo: can not open file \"%s\"\n", cmdptrs[1]);
			return(-1);
		}
	}
	else
	{
		if(strcmp(cmdptrs[1], "core88") == 0)
		{
			loadcore();
			return(0);
		}

		if((fp = fopen(cmdptrs[1], "rb")) == NULL)
		{
			Eprintf("lo: can not open file \"%s\"\n", cmdptrs[1]);
			return(-1);
		}
		else
		{
			/* Save name of program to load */
			strcpy(load_program, cmdptrs[1]);
		}
	}


	/* Read file header */
	if(fread(&fhdr, sizeof(FILHDR), 1, fp) == 0)
	{
		Eprintf("lo: could not read file header\n");
		return(-1);
	}

	/* Check for coff m88k magic */
	if(fhdr.f_magic == MC88MAGIC)
	{
/*
		PPrintf("COFF loader: Loading file \"%s\" for processor type MC88k\n", load_program);
*/

		releasemem();

		fileoffset = sizeof(FILHDR);

		/* Read the optional header if present */
		if(fhdr.f_opthdr)
		{
			if((fseek(fp, fileoffset, 0) == -1) ||
			   (fread(&aoutheader, sizeof(AOUTHDR), 1, fp) == 0))
			{
				Eprintf("lo: could not read a.out header\n");
				return(-1);
			}

			aptr = &aoutheader;
			fileoffset += sizeof(AOUTHDR);
		}

		/* Get room for the section headers */
		if((shdr = (SCNHDR *)malloc(sizeof(SCNHDR) * fhdr.f_nscns)) == 0)
		{
			Eprintf("lo: Could not get memory for section header buffer\n");
			return(-1);
		}

		/* Read the section headers */
		if((fseek(fp, fileoffset, 0) == -1) ||
		   (fread(shdr, sizeof(SCNHDR) * fhdr.f_nscns, 1, fp) == 0))
		{
			Eprintf("lo: Could not read section headers\n");
			free(shdr);
			return(-1);
		}

		for(sID = sectcnt = 0; sectcnt < fhdr.f_nscns; sectcnt++)
		{
			for(ldcnt = sectcnt + 1; ldcnt < fhdr.f_nscns; ldcnt++)
			{
				if(((shdr[ldcnt].s_vaddr + shdr[ldcnt].s_size) <= shdr[sectcnt].s_vaddr)
				   || (shdr[ldcnt].s_vaddr >= (shdr[sectcnt].s_vaddr + shdr[sectcnt].s_size)))
					continue;

				sID = 1;
				ldcnt = sectcnt = fhdr.f_nscns;
			}
		}

		for(sectcnt = 0; sectcnt < fhdr.f_nscns; sectcnt++)
		{
			sptr = shdr + sectcnt;
/*
** Ignore any section header type which is a no load section
**
*/

			switch (sptr->s_flags)
			{	
				case STYP_DSECT  :
				case STYP_NOLOAD :
				case STYP_INFO   :
				case STYP_OVER   :
				case STYP_LIB    :
					continue;
			}

			if(sID && (sptr->s_flags & M_DATA))
				sptr->s_paddr += 0x40000;

			trans.adr1 = sptr->s_vaddr;		/* get target address */

			if(sID)
			{
				PPrintf("Loading section \"%.8s\" ", sptr->s_name);
				PPrintf("at\nphysical address $%08X, ", sptr->s_paddr);
				PPrintf("bytes to load $%08X, ", sptr->s_size);
				PPrintf("Virtual address $%08X\n", trans.adr1);
			}
			else
			{
				PPrintf("Loading section \"%.8s\" ", sptr->s_name);
				PPrintf("at address $%08X, bytes to load $%08X\n",
					trans.adr1, sptr->s_size);
			}

			if(getmem(sptr->s_vaddr, sptr->s_size, (sptr->s_flags &= M_SEGMSK), sptr->s_paddr) != 0)
				return(-1);

			if(sptr->s_flags & STYP_BSS)		/* if bss add break memory */
				if(getmem(sptr->s_vaddr+sptr->s_size, 0, M_PMAP, sptr->s_paddr+sptr->s_size) != 0)
					return(-1);

				/* get section size */
				/* if size is zero all done goto next section */
			if(!(ldtot = sptr->s_size))
				continue;

				/* Generate a null section for null sections */
			if(sptr->s_scnptr == 0)
			{
				for(ldcnt = 0; ldcnt < BUFSIZE; ldcnt++)
					ldbuf[ldcnt] = 0;
			}
			else
			{
						/* move system file pointer */
				if(fseek(fp, (int)sptr->s_scnptr, 0) == -1)
				{
					Eprintf("lo: File seek error - file partialy loaded\n");
					break;
				}
			}

			do			/* load section */
			{
				ldtot -= (ldcnt = (ldtot > BUFSIZE) ? BUFSIZE: ldtot);
						/* don't read null sections */
				if(sptr->s_scnptr != 0)
				{
					if(fread(ldbuf, (int)ldcnt, 1, fp) == 0)
					{
						Eprintf("lo: File read error - file partialy loaded\n");
						break;
					}
				}

				if(loadmem(((sptr->s_flags & M_SEGMSK) | M_WR), trans.adr1, ldbuf, ldcnt) == -1)
				{
					Eprintf("lo: Memory write error - file partialy loaded\n");
					break;
				}
				trans.adr1 += ldcnt;
			}
			while(ldtot);
		}

		/* Load symbol table */
		if(fhdr.f_symptr && (fseek(fp, fhdr.f_symptr, 0) != -1))
		{
			if((symptr = (struct syment *)malloc(fhdr.f_nsyms * sizeof(struct syment))) == 0)
			{
				Eprintf("Can't get memory for symtable\n");
				perror("lo:");
			}
			else if(fread(symptr, fhdr.f_nsyms, sizeof(struct syment), fp) == 0)
			{
				symptr = 0;
				Eprintf("Can't read symtable\n");
				perror("lo:");
			}
			else if(fread(&ldcnt, 1, 4, fp) != 0)
			{
				if((flexname.string_ptr = malloc(ldcnt)) == 0)
				{
					flexname.number_of_entries = 0;
					Eprintf("Can't get memory for flexnames\n");
					perror("lo:");
					return(-1);
				}

				flexname.number_of_entries = ldcnt;
				if(fread(flexname.string_ptr, ldcnt - 4, 1, fp) == 0)
				{
					Eprintf("Can't read flexnames\n");
					perror("lo:");
					return(-1);
				}
			}
			else
				flexname.number_of_entries = 0;

			/* Create local symbol table */
			if(symptr)
			{
				symcreate(symptr, fhdr.f_nsyms, &flexname, shdr);
				free(symptr);

				if(flexname.number_of_entries != 0)
					free(flexname.string_ptr);
			}
		}
		free(shdr);

		if(getmem(STKBASE, STKSIZE, M_DATA, STKBASE) != 0)
			return(-1);

		PPrintf("Setting stack space at %08X to %08X\n", STKBASE, STKBASE+STKSIZE);
		m88000.Regs[31] = (STKBASE+STKSIZE) & 0xFFFFFFF0;

		if(!aptr || (aptr->entry < aptr->text_start) || (aptr->entry > (aptr->text_start + aptr->tsize)))
		{
			if(!aptr)
			{
				Eprintf("Warning this program has no execution header (aouthdr), entry address not set\n");
			}
			else
			{
				Eprintf("Warning entry address ($%08X) out side of text space\n", aptr->entry);
				IP = aptr->text_start;
			}
		}
		else
			IP = aptr->entry;

		PPrintf("Execution address = $%08X\n", IP);
		setargs(cmdptrs[1], IP);

				/* Check if the loaded COFF file is using
				 * stdio calls or sysV calls.  Determined
				 * by the crt0 file. Only if not -u option.
				 */
		if (unixvec)
		{
			if ((stdio_en = stdio_enable()))
			{
				PPrintf("Sim IO vectors: Enabled\n");
			}
			else
			{
				PPrintf("SysV IO vectors: Enabled\n");
			}
		}
		else
			PPrintf("SysV IO and Sim IO vectors: Disabled\n");
	}

	/* Load a.out format file */
	else if(((aout = (struct exec *)&(fhdr))->a_magic == 0407) || (aout->a_magic == 0410) || (aout->a_magic == 0413))
	{
		PPrintf("a.out loader: Loading a file for processor type MC88k\n");

		releasemem();

		if(getmem(0, (ldtot = aout->a_text + aout->a_data + aout->a_bss), M_SEGANY, 0) != 0)
			return(-1);

		trans.adr1 = 0;		/* get target address */
		PPrintf("Loading program at address $00000000, bytes to load $%08X\n", ldtot);

		if(fseek(fp, sizeof(struct exec), 0) == -1) /* move system file pointer */
		{
			Eprintf("lo: File seek error - file not loaded\n");
			return(-1);
		}

		do						/* load section */
		{
			ldtot -= (ldcnt = (ldtot > BUFSIZE) ? BUFSIZE: ldtot);
			if(fread(ldbuf, (int)ldcnt, 1, fp) == 0)
			{
				Eprintf("lo: File read error - file partialy loaded\n");
				break;
			}

			if(loadmem((M_SEGANY | M_WR), trans.adr1, ldbuf, ldcnt) == -1)
			{
				Eprintf("lo: Memory write error - file partialy loaded\n");
				break;
			}

			trans.adr1 += ldcnt;
		} while(ldtot);

		if(getmem(STKBASE, STKSIZE, M_DATA, STKBASE) != 0)
			return(-1);

		PPrintf("Setting stack space at %08X to %08X\n", STKBASE, STKBASE+STKSIZE);
		m88000.Regs[31] = (STKBASE+STKSIZE) & 0xFFFFFFF0;

		if(aout->a_entry > aout->a_text)
		{
			Eprintf("Warning entry address ($%08X) out side of text space\n", aout->a_entry);
			IP = 0;
		}
		else
			IP = aout->a_entry;

		PPrintf("Execution address = $%08X\n", IP);
		setargs(cmdptrs[1], IP);
	}
	else
	{
		Eprintf("lo: file \"%s\" has bad magic - file not loaded\n", cmdptrs[1]);
	}

	fclose(fp);
	return(0);
}




	/**********************************************************
	*
	*      rlo()
	*/


int  prog_loaded = 0;

int rlo(void)
{
	/* file descriptor pointer */

	FILE *fp;

	/* COFF pointers and buffers */

	FILHDR fhdr;	/* file haeder holding buffer */
	SCNHDR *shdr, *sptr;	/* holder buffer pointer and work pointer for section headers */
	struct syment *symptr;		/* pointer to symbol table buffer */
	AOUTHDR *aptr = 0, aoutheader;	/* holding buffer for the a.out section header */

	/* a.out format pointers and buffers */

	struct exec *aout;

	/* general variables */

	unsigned int	 fileoffset;
	unsigned int    sectcnt, ldcnt, ldtot;
	char   ldbuf[BUFSIZE];	/* download buffer */
	struct string_table	flexname;

	if(strlen(load_program) == 0)
	{
		Eprintf("rlo: no program has been previously loaded, use lo\n");
		return(-1);
	}
	else
	{
		Eprintf("Loading program \"%s\"\n", load_program);
		prog_loaded = 1;
	}
	if(strcmp(load_program, "core88") == 0)
	{
		loadcore();
		return(0);
	}
	if((fp = fopen(load_program, "rb")) == NULL)
	{
		Eprintf("rlo: can not open file \"%s\"\n", load_program);
		return(-1);
	}
	if(fread(&fhdr, sizeof(FILHDR), 1, fp) == 0)
	{
		Eprintf("lo: could not read file header\n");
		return(-1);
	}
	if(fhdr.f_magic == MC88MAGIC)
	{
		PPrintf("COFF loader: Loading a file for processor type MC88k\n");

		releasemem();

		fileoffset = sizeof(FILHDR);

		if(fhdr.f_opthdr)
		{
			if((fseek(fp, fileoffset, 0) == -1)
			   || (fread(&aoutheader, sizeof(AOUTHDR), 1, fp) == 0))
			{
				Eprintf("lo: could not read a.out header\n");
				return(-1);
			}
			aptr = &aoutheader;
			fileoffset += sizeof(AOUTHDR);
		}
		if((shdr = (SCNHDR *)malloc(sizeof(SCNHDR) * fhdr.f_nscns)) == 0)
		{
			Eprintf("rlo: Could not get memory for section header buffer\n");
			return(-1);
		}
		if((fseek(fp, fileoffset, 0) == -1) || (fread(shdr, sizeof(SCNHDR) * fhdr.f_nscns, 1, fp) == 0))
		{
			Eprintf("rlo: Could not read section headers\n");
			free(shdr);
			return(-1);
		}
		for(sID = sectcnt = 0; sectcnt < fhdr.f_nscns; sectcnt++)
		{
			for(ldcnt = sectcnt + 1; ldcnt < fhdr.f_nscns; ldcnt++)
			{
				if(((shdr[ldcnt].s_vaddr + shdr[ldcnt].s_size) <= shdr[sectcnt].s_vaddr)
				   || (shdr[ldcnt].s_vaddr >= (shdr[sectcnt].s_vaddr + shdr[sectcnt].s_size)))
					continue;

				sID = 1;
				ldcnt = sectcnt = fhdr.f_nscns;
			}
		}
		for(sectcnt = 0; sectcnt < fhdr.f_nscns; sectcnt++)
		{
			sptr = shdr + sectcnt;
/*
** Ignore any section header type which is a no load section
**
*/

			switch (sptr->s_flags)
			{	
				case STYP_DSECT  :
				case STYP_NOLOAD :
				case STYP_INFO   :
				case STYP_OVER   :
				case STYP_LIB    :
					continue;
			}
			if(sID && (sptr->s_flags & M_DATA))
				sptr->s_paddr += 0x40000;
			if(getmem(sptr->s_vaddr, sptr->s_size, (sptr->s_flags &= M_SEGMSK), sptr->s_paddr) != 0)
				return(-1);
			if(sptr->s_flags & STYP_BSS)		/* if bss add break memory */
				if(getmem(sptr->s_vaddr+sptr->s_size, 0, M_PMAP, sptr->s_paddr+sptr->s_size) != 0)
					return(-1);
			trans.adr1 = sptr->s_vaddr;		/* get target address */
			if(sID)
			{
				PPrintf("Loading section \"%.8s\" ", sptr->s_name);
				PPrintf("at\nphysical address $%08X, ", sptr->s_paddr);
				PPrintf("bytes to load $%08X, ", sptr->s_size);
				PPrintf("Virtual address $%08X\n", trans.adr1);
			}
			else
			{
				PPrintf("Loading section \"%.8s\" at address $%08X, bytes to load $%08X\n",
					sptr->s_name, trans.adr1, sptr->s_size);
			}
			if(!(ldtot = sptr->s_size))	/* get section size */
				continue;		/* if size is zero all done goto next section */
			if(sptr->s_scnptr == 0)		/* Generate a null section for null sections */
			{
				for(ldcnt = 0; ldcnt < BUFSIZE; ldcnt++)
					ldbuf[ldcnt] = 0;
			}
			else
			{
				if(fseek(fp, (int)sptr->s_scnptr, 0) == -1) /* move system file pointer */
				{
					Eprintf("rlo: File seek error - file partialy loaded\n");
					break;
				}
			}
			do						/* load section */
			{
				ldtot -= (ldcnt = (ldtot > BUFSIZE) ? BUFSIZE: ldtot);
				if(sptr->s_scnptr != 0)			/* don't read null sections */
				{
					if(fread(ldbuf, (int)ldcnt, 1, fp) == 0)
					{
						Eprintf("rlo: File read error - file partialy loaded\n");
						break;
					}
				}
				if(loadmem(((sptr->s_flags & M_SEGMSK) | M_WR), trans.adr1, ldbuf, ldcnt) == -1)
				{
					Eprintf("rlo: Memory write error - file partialy loaded\n");
					break;
				}
				trans.adr1 += ldcnt;
			}
			while(ldtot);
		}
		if(fhdr.f_symptr && (fseek(fp, fhdr.f_symptr, 0) != -1))
		{
			if((symptr = (struct syment *)malloc(fhdr.f_nsyms * sizeof(struct syment))) == 0)
			{
				Eprintf("Can't get memory for symtable\n");
				perror("rlo:");
			}
			else if(fread(symptr, fhdr.f_nsyms, sizeof(struct syment), fp) == 0)
			{
				symptr = 0;
				Eprintf("Can't read symtable\n");
				perror("rlo:");
			}
			else if(fread(&ldcnt, 1, 4, fp) != 0)
			{
				if((flexname.string_ptr =(char *)malloc(ldcnt)) == 0)
				{
					flexname.number_of_entries = 0;
					Eprintf("Can't get memory for flexnames\n");
					perror("rlo:");
				}
				else
				{
					flexname.number_of_entries = ldcnt;
					if(fread(flexname.string_ptr, ldcnt - 4, 1, fp) == 0)
					{
						flexname.number_of_entries = 0;
						Eprintf("Can't read flexnames\n");
						perror("rlo:");
					}
				}
			}
			if(symptr && (flexname.number_of_entries != 0))
			{
				symcreate(symptr, fhdr.f_nsyms, &flexname, shdr);
				free(symptr);
				if(flexname.number_of_entries != 0)
					free(flexname.string_ptr);
			}
		}
		free(shdr);

		if(getmem(STKBASE, STKSIZE, M_DATA, STKBASE) != 0)
			return(-1);

		PPrintf("Setting stack space at %08X to %08X\n", STKBASE, STKBASE+STKSIZE);
		m88000.Regs[31] = (STKBASE+STKSIZE) & 0xFFFFFFF0;

		if(!aptr || (aptr->entry < aptr->text_start)
		   || (aptr->entry > (aptr->text_start + aptr->tsize)))
		{
			if(!aptr)
			{
				Eprintf("Warning this program has no execution header (aouthdr), entry address not set\n");
			}
			else
			{
				Eprintf("Warning entry address ($%08X) out side of text space\n", aptr->entry);
				IP = aptr->text_start;
			}
		}
		else
			IP = aptr->entry;
		PPrintf("Execution address = $%08X\n", IP);
		setargs(load_program, IP);

				/* Check if the loaded COFF file is using
				 * stdio calls or sysV calls.  Determined
				 * by the crt0 file. Only if not -u option.
				 */
		if (unixvec)
		{
			if ((stdio_en = stdio_enable()))
			{
				printf("stdio trap vectors enabled\n");
			}
			else
			{
				printf("System V library enabled\n");
			}
		}
	}
	else if(((aout = (struct exec *)&(fhdr))->a_magic == 0407) || (aout->a_magic == 0410) || (aout->a_magic == 0413))
	{
		PPrintf("a.out loader: Loading a file for processor type MC88k\n");

		releasemem();

		if(getmem(0, (ldtot = aout->a_text + aout->a_data + aout->a_bss), M_SEGANY, 0) != 0)
			return(-1);
		trans.adr1 = 0;		/* get target address */
		PPrintf("Loading program at address $00000000, bytes to load $%08X\n", ldtot);
		if(fseek(fp, sizeof(struct exec), 0) == -1) /* move system file pointer */
		{
			Eprintf("rlo: File seek error - file not loaded\n");
			return(-1);
		}
		do						/* load section */
		{
			ldtot -= (ldcnt = (ldtot > BUFSIZE) ? BUFSIZE: ldtot);
			if(fread(ldbuf, (int)ldcnt, 1, fp) == 0)
			{
				Eprintf("rlo: File read error - file partialy loaded\n");
				break;
			}
			if(loadmem((M_SEGANY | M_WR), trans.adr1, ldbuf, ldcnt) == -1)
			{
				Eprintf("rlo: Memory write error - file partialy loaded\n");
				break;
			}
			trans.adr1 += ldcnt;
		}
		while(ldtot);
		if(getmem(STKBASE, STKSIZE, M_DATA, STKBASE) != 0)
			return(-1);
		PPrintf("Setting stack space at %08X to %08X\n", STKBASE, STKBASE+STKSIZE);
		m88000.Regs[31] = (STKBASE+STKSIZE) & 0xFFFFFFF0;
		if(aout->a_entry > aout->a_text)
		{
			Eprintf("Warning entry address ($%08X) out side of text space\n", aout->a_entry);
			IP = 0;
		}
		else
			IP = aout->a_entry;
		PPrintf("Execution address = $%08X\n", IP);
		setargs(load_program, IP);
	}
	else
	{
		Eprintf("rlo: file \"%s\" has bad magic - file not loaded\n", load_program);
	}
	fclose(fp);
	return(0);
}

void pr_opt_hdr(AOUTHDR *bptr)
{
        int i,j;
        char *a = (char *) bptr;

        printf("loaded pseudo optional part of coff header\n");
        printf("size of header is %d\n",i=sizeof(AOUTHDR));
        printf("magic number = %o\n",bptr->magic);
        printf("vstamp = %x\n",bptr->vstamp);
        printf("tsize = %x\n",bptr->tsize);
        printf("dsize = %x\n",bptr->dsize);
        printf("bsize = %x\n",bptr->bsize);
        printf("entry = %x\n",bptr->entry);
        printf("text_start = %x\n",bptr->text_start);
        printf("data_start = %x\n",bptr->data_start);

        printf("header hex dump\n");
        for (j=0;j<i;j++)printf("%02.2x ",a[j]);
        printf("\n");
}

extern	struct mem_segs memory[];
union allptrs { void *v; unsigned char *c; unsigned short *s; unsigned int *i;};

int loadmem(int rdwrflg,unsigned int memadr,void *srcdesta,int size)
{
	struct mem_wrd *ptr, *end;
	union allptrs srcdest;
	int cnt, seg;
	int addr;

	srcdest.v = srcdesta;
	if((seg = checklmt(memadr, (rdwrflg & M_SEGMSK))) == -1)
		return(-1);

	if((cnt = size) == 0)
		return(size);

	if(sID && ((rdwrflg & M_SEGMSK) == M_SEGANY))
		addr = memadr - memory[seg].physaddr;
	else
		addr = memadr - memory[seg].baseaddr;
	ptr = memory[seg].seg + (addr / 4);
	end = memory[seg].seg + ((memory[seg].endaddr - memory[seg].baseaddr)/4);

	while((addr % 4) && (cnt))
	{
		ptr->mem.c[addr++ % 4] = *srcdest.c++;
		cnt--;
		if(!(addr % 4) || !(cnt))
		{
			makesim(ptr->mem.l, &ptr->opcode);
			ptr++;
		}
	}
	while(!((int)srcdest.i & 3) && (cnt / 4) && (ptr <= end) && !ckquit())
	{
		ptr->mem.l = *srcdest.i++;
		makesim(ptr->mem.l, &ptr->opcode);
		ptr++;
		addr += 4;
		cnt -= 4;
	}
	while(cnt && (ptr <= end) && !ckquit())
	{
		ptr->mem.c[addr++ % 4] = *srcdest.c++;
		cnt--;
		if(!(addr % 4) || !(cnt))
		{
			makesim(ptr->mem.l, &ptr->opcode);
			ptr++;
		}
	}

	return(size - cnt);
}
