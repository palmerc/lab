
struct scnhdr {
	char		s_name[8];	/* section name */
	int		s_paddr;	/* physical address, aliased s_nlib */
	int		s_vaddr;	/* virtual address */
	int		s_size;		/* section size */
	int		s_scnptr;	/* file ptr to raw data for section */
	int		s_relptr;	/* file ptr to relocation */
	int		s_lnnoptr;	/* file ptr to line numbers */
	unsigned int	s_nreloc;	/* number of relocation entries */
	unsigned int	s_nlnno;	/* number of line number entries */
	int		s_flags;	/* flags */
	};


#define	SCNHDR	struct scnhdr

/*
 * The low 2 bytes of s_flags is used as a section "type"
 */

#define STYP_DSECT	0x01		/* "dummy" section:
						not allocated, relocated,
						not loaded */
#define STYP_NOLOAD	0x02		/* "noload" section:
						allocated, relocated,
						 not loaded */
#define STYP_INFO	0x200		/* comment section : not allocated
						not relocated, not loaded */
#define STYP_LIB	0x800		/* for .lib section : same as INFO */
#define STYP_OVER	0x400		/* overlay section : relocated
						not allocated or loaded */
#define	STYP_TEXT	0x20		/* section contains text only */
#define STYP_DATA	0x40		/* section contains data only */
#define STYP_BSS	0x80		/* section contains bss only */
