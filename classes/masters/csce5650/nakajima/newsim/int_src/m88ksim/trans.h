/* @(#) File name: trans.h   Ver: 3.1   Cntl date: 1/20/89 14:39:12 */
/*****************************************************************************
*
*       This is the transaction include file for the VBED debugger
*
*                                          Date Last Modified:
*/

	/* Defines and Typedefs */

	/* Transaction packet structure */

struct transaction
{
	unsigned int	adr1;
	unsigned int	adr2;
	unsigned int	size;
	unsigned int	cnt;
	unsigned int	expr;
	unsigned int	type;
	unsigned int	page;
	unsigned int	am;
};

extern struct transaction trans;

