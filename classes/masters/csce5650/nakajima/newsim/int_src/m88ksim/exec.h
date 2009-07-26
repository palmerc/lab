/* @(#) File name: exec.h   Ver: 3.1   Cntl date: 1/20/89 14:38:59 */
/*
 * format of the exec header
 */
struct exec {
	unsigned int	a_magic;	/* magic number */
	unsigned int	a_text;		/* size of text segment */
	unsigned int	a_data;		/* size of initialized data */
	unsigned int	a_bss;		/* size of uninitialized data */
	unsigned int	a_syms;		/* size of symbol table */
	unsigned int	a_entry;	/* entry point */
	unsigned int	a_trsize;	/* size of text relocation */
	unsigned int	a_drsize;	/* size of data relocation */
};

#define	OMAGIC	0407		/* old impure format */
#define	NMAGIC	0410		/* read-only text */
#define	ZMAGIC	0413		/* demand load format */

