/* @(#) File name: br.h   Ver: 3.1   Cntl date: 1/20/89 14:38:48 */
/*****************************************************************************
*
*
*/

#define	MAXBRK	(15)
#define	TMPBRK	(MAXBRK + 1)

struct	brkpoints {
	unsigned int adr;
	unsigned short limit;
	unsigned short cnt;
	unsigned char code;
};

