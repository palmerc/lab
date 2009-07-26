
struct filehdr {
	unsigned short	f_magic;	/* magic number */
	unsigned short	f_nscns;	/* number of sections */
	int		f_timdat;	/* time & date stamp */
	int		f_symptr;	/* file pointer to symtab */
	int		f_nsyms;	/* number of symtab entries */
	unsigned short	f_opthdr;	/* sizeof(optional hdr) */
	unsigned short	f_flags;	/* flags */
	};


	/* Motorola 88000 */
#define MC88MAGIC	0555	/* M88000 OCS_BCS normal file */

#define	FILHDR	struct filehdr
#define	FILHSZ	sizeof(FILHDR)
