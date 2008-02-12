#ifndef config_h
#define config_h
/* config.h
 * This file was NOT produced by running the config.h.SH script
 * This file was created by SPEC to define the standard SPEC configuration
 *  use -Dfoo or -Ufoo in EXTRA_CFLAGS to override these values
 */
 /*SUPPRESS 460*/


/* ALIGNBYTES
 *	This symbol contains the number of bytes required to align a double.
 *	Usual values are 2, 4, and 8.
 */
#ifndef ALIGNBYTES
#define ALIGNBYTES ?		/**/
#endif

/* BYTEORDER
 *	This symbol contains an encoding of the order of bytes in a long.
 *	Usual values (in hex) are 0x1234, 0x4321, 0x2143, 0x3412...
 */
#ifndef BYTEORDER
#define BYTEORDER 0x1234		/**/
#endif

/* CASTNEGFLOAT
 *	This symbol, if defined, indicates that this C compiler knows how to
 *	cast negative or large floating point numbers to unsigned longs, ints
 *	and shorts.
 */
/* CASTFLAGS
 *	This symbol contains flags that say what difficulties the compiler
 *	has casting odd floating values to unsigned long:
 *		1 = couldn't cast < 0
 *		2 = couldn't cast >= 0x80000000
 */
/*#undef	CASTNEGFLOAT	*/
#ifndef CASTFLAGS
#define	CASTFLAGS 3	/**/
#endif

/* CHARSPRINTF
 *	This symbol is defined if this system declares "char *sprintf()" in
 *	stdio.h.  The trend seems to be to declare it as "int sprintf()".  It
 *	is up to the package author to declare sprintf correctly based on the
 *	symbol.
 */
#undef	CHARSPRINTF 	/**/

/* index
 *	This preprocessor symbol is defined, along with rindex, if the system
 *	uses the strchr and strrchr routines instead.
 */
/* rindex
 *	This preprocessor symbol is defined, along with index, if the system
 *	uses the strchr and strrchr routines instead.
 */
#ifndef BSD_INDEX
#define	index strchr	/* cultural */
#define	rindex strrchr	/*  differences? */
#endif

/* HAS_MEMCMP
 *	This symbol, if defined, indicates that the memcmp routine is available
 *	to compare blocks of memory.  If undefined, roll your own.
 */
#define	HAS_MEMCMP		/**/

/* HAS_MEMCPY
 *	This symbol, if defined, indicates that the memcpy routine is available
 *	to copy blocks of memory.  Otherwise you should probably use bcopy().
 *	If neither is defined, roll your own.
 */
/* SAFE_MEMCPY
 *	This symbol, if defined, indicates that the memcpy routine is available
 *	to copy potentially overlapping copy blocks of memory.  Otherwise you
 *	should probably use memmove() or bcopy().  If neither is defined,
 *	roll your own.
 */
#define	HAS_MEMCPY		/**/
#undef	SAFE_MEMCPY		/**/

/* HAS_MEMMOVE
 *	This symbol, if defined, indicates that the memmove routine is available
 *	to move potentially overlapping blocks of memory.  Otherwise you
 *	should use bcopy() or roll your own.
 */
#define	HAS_MEMMOVE		/**/

/* HAS_MEMSET
 *	This symbol, if defined, indicates that the memset routine is available
 *	to set a block of memory to a character.  If undefined, roll your own.
 */
#define	HAS_MEMSET		/**/

/* STRUCTCOPY
 *	This symbol, if defined, indicates that this C compiler knows how
 *	to copy structures.  If undefined, you'll need to use a block copy
 *	routine of some sort instead.
 */
#define	STRUCTCOPY	/**/

/* HAS_STRERROR
 *	This symbol, if defined, indicates that the strerror() routine is
 *	available to translate error numbers to strings.
 */
#define	HAS_STRERROR		*/

/* HAS_VFORK
 *	This symbol, if defined, indicates that vfork() exists.
 */
#undef	HAS_VFORK	/**/

/* VOIDSIG
 *	This symbol is defined if this system declares "void (*signal())()" in
 *	signal.h.  The old way was to declare it as "int (*signal())()".  It
 *	is up to the package author to declare things correctly based on the
 *	symbol.
 */
/* TO_SIGNAL
 *	This symbol's value is either "void" or "int", corresponding to the
 *	appropriate return "type" of a signal handler.  Thus, one can declare
 *	a signal handler using "TO_SIGNAL (*handler())()", and define the
 *	handler using "TO_SIGNAL handler(sig)".
 */
#define	VOIDSIG 	/**/
#define	TO_SIGNAL	int 	/**/

/* HASVOLATILE
 *	This symbol, if defined, indicates that this C compiler knows about
 *	the volatile declaration.
 */
#undef	HASVOLATILE	/**/

/* HAS_VPRINTF
 *	This symbol, if defined, indicates that the vprintf routine is available
 *	to printf with a pointer to an argument list.  If unavailable, you
 *	may need to write your own, probably in terms of _doprnt().
 */
/* CHARVSPRINTF
 *	This symbol is defined if this system has vsprintf() returning type
 *	(char*).  The trend seems to be to declare it as "int vsprintf()".  It
 *	is up to the package author to declare vsprintf correctly based on the
 *	symbol.
 */
#define	HAS_VPRINTF	/**/
#undef	CHARVSPRINTF 	/**/

/* MYMALLOC
 *	This symbol, if defined, indicates that we're using our own malloc.
 */
/* MALLOCPTRTYPE
 *	This symbol defines the kind of ptr returned by malloc and realloc.
 */
/*#undef MYMALLOC			*/

#ifndef MALLOCPTRTYPE
#define MALLOCPTRTYPE void         /**/
#endif


/* RANDBITS
 *	This symbol contains the number of bits of random number the rand()
 *	function produces.  Usual values are 15, 16, and 31.
 */
#ifndef RANDBITS
#define RANDBITS 15		/**/
#endif

/* STDCHAR
 *	This symbol is defined to be the type of char used in stdio.h.
 *	It has the values "unsigned char" or "char".
 */
#ifndef STDCHAR
#define STDCHAR unsigned char	/**/
#endif

/* VOIDHAVE
 *	This symbol indicates how much support of the void type is given by this
 *	compiler.  What various bits mean:
 *
 *	    1 = supports declaration of void
 *	    2 = supports arrays of pointers to functions returning void
 *	    4 = supports comparisons between pointers to void functions and
 *		    addresses of void functions
 *
 *	The package designer should define VOIDWANT to indicate the requirements
 *	of the package.  This can be done either by #defining VOIDWANT before
 *	including config.h, or by defining voidwant in Myinit.U.  If the level
 *	of void support necessary is not present, config.h defines void to "int",
 *	VOID to the empty string, and VOIDP to "char *".
 */
/* void
 *	This symbol is used for void casts.  On implementations which support
 *	void appropriately, its value is "void".  Otherwise, its value maps
 *	to "int".
 */
/* VOID
 *	This symbol's value is "void" if the implementation supports void
 *	appropriately.  Otherwise, its value is the empty string.  The primary
 *	use of this symbol is in specifying void parameter lists for function
 *	prototypes.
 */
/* VOIDP
 *	This symbol is used for casting generic pointers.  On implementations
 *	which support void appropriately, its value is "void *".  Otherwise,
 *	its value is "char *".
 */
#ifndef VOIDWANT
#define VOIDWANT 7
#endif
#define VOIDHAVE 7
#if (VOIDHAVE & VOIDWANT) != VOIDWANT
#define void int		/* is void to be avoided? */
#define VOID
#define VOIDP (char *)
#define M_VOID		/* Xenix strikes again */
#else
#define VOID void
#define VOIDP (void *)
#endif

#endif
