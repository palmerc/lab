
struct syment
{
	union
	{
		char		_n_name[8];	/* old COFF version */
		struct
		{
			int	_n_zeroes;	/* new == 0 */
			int	_n_offset;	/* offset into string table */
		} _n_n;
	} _n;
	int			n_value;	/* value of symbol */
	short			n_scnum;	/* section number */
	unsigned short		n_type;		/* type and derived type */
	char			n_sclass;	/* storage class */
	char			n_numaux;	/* number of aux. entries */
	short			n_pad;		/* pad to 4 byte multiple */
};
