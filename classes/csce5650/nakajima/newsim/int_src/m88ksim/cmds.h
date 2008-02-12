/* @(#) File name: cmds.h   Ver: 3.1   Cntl date: 1/20/89 14:38:49 */
/*****************************************************************************
*
*       This is the commands include file for the VBED debugger
*
*                                          Date Last Modified:
*/

/****************************************************************************
*
*
*	COMMANDS SYNOPSIS
*
*
*	Block Fill:
*		bf <RANGE> <DATA> [;<TYPE>]
*
*	Block Move:
*		bm <RANGE> <ADDR> [;<TYPE>]
*
*	Block Search:
*		bs <RANGE> <DATA> [;<TYPE>]
*
*	Data Conversion:
*		dc <EXPR> | <ADDR>
*
*	Exit:
*		ex
*
*	Help:
*		he
*
*	Load a COFF:
*		lo <filename>
*
*	Memory Display:
*		md[s] <ADDR>[:<COUNT> | <ADDR>] [;<TYPE>]
*
*	Memory Display and dissaemble:
*		id[s] <ADDR>[:<COUNT> | <ADDR>] [;<TYPE>]
*
*	Memory Modify:
*		mm <ADDR> [;<TYPE>]
*
*	Quit:
*		q
*
*
*	OPTIONS
*
*	<EXPR>:	is an equation of the form
*		num+num
*		num-num
*
*		and the number is of the form
*		3f	for HEX
*		$03f	for HEX
*		&44	for DECIMAL
*		@34	for OCTAL
*		%011	for BINARY
*
*	<ADDR>: is an EXPR
*
*	<COUNT>: is an EXPR
*
*	<RANGE>: is one of the following forms
*		<ADDR> <ADDR>
*		<ADDR>:<COUNT>
*
*	<TEXT>: is an ASCII string in the form
*		'abcdefg'
*
*/
	/* Defines and Typedefs */

#define	VALID	(1)
#define	INVALID	(2)

#define	NONE		(0x0)
#define	EXPR		(0x1)
#define	ADDR		(0x2)
#define	DATA		(0x4)
#define	COUNT		(0x8)
#define	RANGE		(0x10)
#define	TEXT		(0x20)
#define	TYPE		(0x40)
#define	MSK		(0x80)
#define	COMMAND		(0x100)
#define	AM		(0x200)
#define	RERUN		(0x400)
#define	REGISTER	(0x800)

#define	BYTE	(0x1)
#define	WRD	(0x2)
#define	LONG	(0x4)
#define	DI	(0x100)
#define	DF	(0x200)
#define	SF	(0x400)

#define DEFWRDSZE	(LONG)		/* default word size for data transfers */

typedef void (*FUNCT)();                /* funct is a function pointer */
typedef struct cmdstruct *CMD;          /* cmd is a cmdstruct pointer */


	/* directory tree structure */

struct dirtree
{
	int	flags;
	struct cmdstruct *dir;
};

	/* command tree node structure */

struct cmdstruct
{
	char    *cmd;
	int	flags;
	struct options *opts;
	int	(*funct)();
	char	*help;
};


struct options
{
	char	*name;
	int	flags;
};

