/* @(#) File name: converters.c   Ver: 3.2   Cntl date: 3/6/89 12:06:20 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

/*****************************************************************************
*
*
*
*/

#include "functions.h"
#include <ctype.h>
#include "defines.h"
#include "trans.h"
#include <string.h>


        /**********************************************************
	*
	*       Function Definition  (Local and External)
	*
	*/


	/**********************************************************
	*
	*       External Variables Definition
	*
	*/

extern	char	*cmdptrs[MAXCMD];
extern	int	cmdcount;
extern	int	cmdflags;


	/**********************************************************
	*
	*       Local Defines
	*
	*/



	/**********************************************************
	*
	*       Local Variables Definition
	*
	*/


	/**********************************************************
	*
	*      Messages
	*/



int
getexpr(char *ptr,char **eptr,unsigned int *value)
{
	unsigned int work, work1;
	register char *mptr;
	if(*ptr == '-')
	{
		ptr = simatoi(ptr+1, &work);
		work = 0 - work;
	}
	else
		ptr = simatoi(ptr, &work);

	while((*ptr == '*') || (*ptr == '/'))
	{
		if(*(ptr+1) == '-')
		{
			mptr = simatoi(ptr+2, &work1);
			work1 = 0 - work1;
		}
		else
			mptr = simatoi(ptr+1 , &work1);

		if(*ptr == '*')
			work *= work1;
		else if(work1 == 0)
		{
			PPrintf("Divide by zero  ");
			*eptr = ptr;
			return(-1);
		}
		else
			work /= work1;
		ptr = mptr;
	}
	if(*ptr == '+')
	{
		if(getexpr(ptr+1, &ptr, &work1) == 0)
			work += work1;
	}
	if(*ptr == '-')
	{
		if(getexpr(ptr+1, &ptr, &work1) == 0)
			work -= work1;
	}
	*eptr = ptr;
	if((*ptr == '\0') || (*ptr == '\n'))
	{
		*value = work;
		return(0);
	}
	return(-1);
}

int getrange(void)
{
	unsigned int value1 = 0, value2 = 0;
	char *ptr1 = 0, *errptr;

	if(ptr1 = strchr(cmdptrs[1], ':'))
		*ptr1++ = '\0';

	if((cmdcount > 1) && (getexpr(cmdptrs[1], &errptr, &value1) != 0))
		return(-1);
	
	if(!ptr1)
	{
		if((cmdcount == 2) || ((cmdcount == 3) && (cmdflags & TYPE)))
			value2 = value1 + 16;
		else
		{
			if(getexpr(cmdptrs[2], &errptr, &value2) != 0)
				return(-1);
		}
	}
	else
	{
		if(getexpr(ptr1, &errptr, &value2) != 0)
			return(-1);
		cmdflags |= COUNT;
		if(cmdflags & TYPE)
			value2 = value1 + (value2 * trans.size);
		else
			value2 = value1 + (value2 * 2);
	}

	trans.adr1 = value1;
	trans.adr2 = value2;
	return(0);
}


int getdata(char *ptr,char **eptr,unsigned int *value)
{
	if((*ptr == '\'') || (*ptr == '"'))
	{
		cmdflags |= TEXT;
		*value = (int)ptr;
	}
	else
		if(getexpr(ptr, eptr, value) != 0)
			return(-1);

	return(0);
}

char *simatoi(char *ptr,unsigned int *value)
{
	register int	num, base;
	register char	*symptr;
	
	*value = 0;

	if((symptr = getregval(ptr, (int *)value)) != ptr)
		return(symptr);

	if((symptr = findsym(ptr,(int *) value)) != ptr)
		return(symptr);

	switch(*ptr)
	{
		case	'%':		/* requesting BINARY */
			ptr++;		/* bump pointer past base request */
			base = 2;	/* set base */
			break;

		case	'@':		/* requesting OCTAL */
			ptr++;		/* bump pointer past base request */
			base = 8;	/* set base */
			break;

		case	'&':		/* requesting DECIMAL */
			ptr++;		/* bump pointer past base request */
			base = 10;	/* set base */
			break;

		case	'$':		/* requesting HEXDECIMAL */
			ptr++;		/* bump pointer past base request */
		default:		/* Default is HEXDECIMAL */
			base = 16;	/* set base */
			break;
	}
	do
	{
		if((*ptr >= 'A') && (*ptr <= 'Z'))	/* check bounds */
			*ptr |= 0x20;			/* make it lower case */
		num = *ptr - '0';	/* subtract zero */
		if((base == 16) && (num >= 0xa))
		{
			if(num <= 0x10)
				break;
			num -= 0x27;
		}
		if((num < 0) || (num >= base))
			break;

		*value = (*value * base) + num;
	}
	while((*++ptr) && (*ptr != '\n'));

	return(ptr);
}


unsigned int atosf(char *str)
{
	unsigned int retval;
	union {double val;
		unsigned int ahi,alo;}u;

	u.val = atof(str);
	fcds(u.ahi,u.alo, &retval, 0);
	
	return(retval);
}


/* This should be a part of ANSI C libraries
char * strchr(char *str,char chr)
{
	while((*str != chr) && (*str))
		str++;
	if(*str == chr)
		return(str);
	return(0);
}
*/


void str_toupper(char *str)
{
	while(*str != '\0')
	{
		if(islower(*str))
			*str = toupper(*str);

		str++;
	}
}
