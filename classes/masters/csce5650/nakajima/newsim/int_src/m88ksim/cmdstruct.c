/* @(#) File name: cmdstruct.c   Ver: 3.3   Cntl date: 3/10/89 10:33:05 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

/*****************************************************************************
*
*
*       This module contains the command tree structures.
*
*
*                                          Date Last Modified: 
*/


	/**********************************************************
	*
	*       Function Definition
	*
	*/

#include "functions.h"


	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	struct cmdstruct bugtree[];
extern	struct cmdstruct diagtree[];
extern	struct options mem1[], mem2[], mem3[], mapopts[], regopts[];


	/**********************************************************
	*
	*       Local Variables and Strings
	*
	*/


	/**********************************************************
	*
	*       Directory Structure Tree
	*
	*/

struct dirtree dir[] =
{
	{ VALID,	bugtree		},
	{ VALID,	diagtree	},
	{ 0,		NULL,		},
};


	/**********************************************************
	*
	*       Command Structure Tree
	*
	*/

struct cmdstruct bugtree[] =		/* debugger command list */
{
/*0 */	{ "bd"	, COMMAND,				NULL,	bd,	"CMMU BATC Display"			},
/*1 */	{ "br"	, COMMAND | TYPE,			NULL,	br,	"Break points"				},
/*2 */	{ "bf"	, RANGE | DATA | TYPE,			mem2,	bf,	"Block Fill Memory"			},
/*3 */	{ "bm"	, RANGE | DATA | TYPE,			mem2,	bm,	"Block Move Memory"			},
/*4 */	{ "bs"	, RANGE | DATA | TEXT | MSK | TYPE,	mem2,	bs,	"Block Search Memory"			},
/*5 */	{ "cd"	, COMMAND,				NULL,	cd,	"CMMU Cache Display"			},
/*6 */	{ "cm"	, COMMAND | TYPE,			NULL	,cm,	"CMMU Register Modify"			},
/*7 */	{ "cr"	, COMMAND,				NULL,	cr,	"CMMU Control Display"			},
/*8 */	{ "cs"	, NONE,					NULL,	cs,	"CMMU Display stats"			},
/*9 */	{ "dc"	, EXPR,					NULL,	dc,	"Data Conversion"			},
/*
	{ "dm"	, NONE,					NULL,	dm,	"Display Memory Map"			},
*/
/*10*/	{ "ex"	, NONE,					NULL,	simexit,"Exit"					},
/*11*/	{ "gd"	, ADDR,					NULL,	gd,	"Execute and ignore breakpoints" 	},
/*12*/	{ "gn"	, NONE,					NULL,	gn,	"Execute and break at next instruction" },
/*13*/	{ "go"	, ADDR,					NULL,	go,	"Execute program"			},
/*14*/	{ "gt"	, ADDR,					NULL,	gt,	"Execute and set temporary breakpoint"	},
/*15*/	{ "g"	, ADDR,					NULL,	go,	"alias for \"go\""			},
/*16*/	{ "he"	, COMMAND,				NULL,	he,	"This help function"			},
/*17*/	{ "lo"	, COMMAND,				NULL,	lo,	"Load a coff file"			},
/*
	{ "rlo"	, COMMAND,				NULL,	rlo,	"reload previous coff file"		},
*/
/*18*/	{ "map"	, RANGE | TYPE,				mapopts,map,	"Map more memory"			},
/*19*/	{ "mds"	, RANGE | TYPE | RERUN,			mem1,	mds,	"Memory Display"			},
/*20*/	{ "md"	, RANGE | TYPE | RERUN,			mem1,	md,	"Memory Display"			},
/*
	{ "ids"	, RANGE | TYPE | RERUN,			mem3,	ids,	"Memory Display and disassemble"	},
	{ "id"	, RANGE | TYPE | RERUN,			mem3,	id,	"Memory Display and disassemble"	},
*/
/*21*/	{ "mm"	, ADDR | TYPE,				mem1,	mm,	"Memory Modify"				},
/*22*/	{ "m"	, ADDR | TYPE,				mem1,	mm,	"alias for \"mm\""			},
/*23*/	{ "nobr", COMMAND,				NULL,	nobr,	"Delete break points"			},
/*24*/	{ "nopa", NONE,					NULL,	nopa,	"Stop copying stdout"			},
/*25*/	{ "pa"	, COMMAND,				NULL,	pa,	"Copy all stdout to named file"		},
/*26*/	{ "pd"	, COMMAND,				NULL,	pd,	"CMMU PATC Display"			},
/*27*/	{ "q"	, NONE,					NULL,	simexit,"Exit"					},
/*28*/	{ "rd"	, COMMAND | TYPE,			regopts,rd,	"Register Display"			},
/*29*/	{ "rst"	, NONE,					NULL,	rstsys,	"Reset processor and CMMU" 		},
/*30*/	{ "rm"	, COMMAND | TYPE,			regopts,rm,	"Register Modify"			},
/*31*/	{ "run"	, COMMAND,				NULL,	run,	"Run program with argc, argv"		},
/*32*/	{ "rrn"	, COMMAND,				NULL,	rrn,	"Rerun program with previous argc, argv"},
/*33*/	{ "cwd"	, COMMAND,				NULL,	cwd,	"Change Current Working Directory" 	},
/*34*/	{ "pwd"	, COMMAND,				NULL,	pwd,	"Print Current Working Directory" 	},
/*35*/	{ "save", NONE,					NULL,	dumpcore,"Save current state to disk" 		},
/*36*/	{ "sh"	, NONE,					NULL,	NULL,	"Escape to shell"			},
/*37*/	{ "show", COMMAND,				NULL,	show,	"Display/Set/Remove Trace Data"		},
/*38*/	{ "sd"	, NONE,					NULL,	sd,	"Display execution stats" 		},
/*39*/	{ "sr"	, NONE,					NULL,	sr,	"Reset execution stats" 		},
/*40*/	{ "sym"	, COMMAND,				NULL,	prtsym,	"Display symbol table"			},
/*41*/	{ "tt"	, ADDR,					NULL,	tt,	"Trace to temporary breakpoint" 	},
/*42*/	{ "tv"	, ADDR | RERUN,				NULL,	tv,	"Trace being verbose"			},
/*43*/	{ "tx"	, ADDR,					NULL,	tx,	"Trace displaying flow changes" 	},
/*44*/	{ "t"	, EXPR | RERUN,				NULL,	tr,	"Trace instructions"			},
/*
	{ "tz"	, ADDR,					NULL,	tz,	"Special Trace instruction"		},
*/
/*45*/	{ "!"	, COMMAND,				NULL,	NULL,	"Execute an shell and return" 		},
/*46*/	{ "\r"	, RERUN,				NULL,	NULL,	"\0"					},
/*47*/	{ "cacheoff"	, COMMAND,		NULL,	cacheoff,	"turn off usecmmu flag"			},
	{ NULL	}
};



struct cmdstruct diagtree[] =		/* Diagnostic command tree */
{
	{ NULL	, 0,	NULL	}
};



struct options mem1[] =		/* Size type command tree */
{
	{ "d"	, LONG*2 | DF	},
	{ "s"	, LONG | SF,	},
	{ "w"	, LONG		},
	{ "h"	, WRD		},
	{ "b"	, BYTE		},
	{ "di"	, LONG | DI	},
	{ NULL	}
};


struct options mem2[] =		/* Size type command tree */
{
	{ "w"	, LONG		},
	{ "h"	, WRD		},
	{ "b"	, BYTE		},
	{ NULL	}
};

struct options mem3[] =		/* Size type command tree */
{
	{ "d"	, LONG*2 | DF	},
	{ "s"	, LONG | SF,	},
	{ "w"	, LONG		},
	{ "h"	, WRD		},
	{ "b"	, BYTE		},
	{ NULL	}
};


struct options mapopts[] =		/* Size type command tree */
{
	{ "r"	, 0x1E0		},	/* retain throught loads */
	{ "u"	, 0x200		},	/* unmap */
	{ "t"	, 0x20		},	/* map as text */
	{ "d"	, 0x40		},	/* map as data */
	{ "b"	, 0x80		},	/* map as bss */
	{ "a"	, 0xE0		},	/* map as all sections */
	{ NULL	}
};

struct options regopts[] =		/* Size type command tree */
{
	{ "l"	, DEFWRDSZE	},
	{ "d"	, DF		},
	{ "s"	, SF		},
	{ NULL	}
};

int simexit(void){ (void)PPrintf("\n"); exit(0); return 0;}
