/*
 * static char ID_aouth[] = "@(#)aouthdr.h	6.1	";
 */

typedef	struct aouthdr {
	short	magic;		/* see magic.h				*/
	short	vstamp;		/* version stamp			*/
	int	tsize;		/* text size in bytes, padded to FW
				   bdry					*/
	int	dsize;		/* initialized data "  "		*/
	int	bsize;		/* uninitialized data "   "		*/
#if u3b
	int	dum1;
	int	dum2;		/* pad to entry point	*/
#endif
	int	entry;		/* entry pt.				*/
	int	text_start;	/* base of text used for this file	*/
	int	data_start;	/* base of data used for this file	*/
} AOUTHDR;
