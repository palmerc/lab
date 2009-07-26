/* @(#) File name: sim_printf.c   Ver: 3.1   Cntl date: 1/20/89 14:41:20 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

#include "functions.h"


int sim_printf(FILE *fp, char *ptr, int regno)
{
    int temp;
    char *s1, *s2;
    char str[100];
    union {
	double d;
	int    i[2];
    } db1;

    /* parse and print the string */

    for (s2 = s1 = ptr;*s2; s2++)
    {
	if (*s2 == '%')
	{
	    for (s1 = s2;;++s2)
	    {
		if (*s2 == 's')
		{
			++s2;         /* point to character following the 's' */
			temp = *s2;
			*s2 = 0;      /* terminate string so far */
			if(!copystr(regno, str))
			    return(-1);
			fprintf(fp,s1,str);
			*s2 = temp;
			++regno;          /* increment register counter */
			break;
		}
		else if	((*s2 == 'o') ||
				 (*s2 == 'd') ||
				 (*s2 == 'x') ||
				 (*s2 == 'c') ||
				 (*s2 == 'X'))
		{
			++s2;
			temp = *s2;
			*s2 = 0;      /* terminate string so far */
			fprintf(fp,s1,m88000.Regs[++regno]);
			*s2 = temp;
			break;
		}
		else if ((*s2 == 'f') ||
				 (*s2 == 'g') ||
				 (*s2 == 'e') ||
				 (*s2 == 'E'))
		{
			++regno;
			++s2;
			temp = *s2;
			*s2 = 0;      /* terminate string so far */
			regno = (regno + 1) & 0x1e;
			db1.i[0] = m88000.Regs[regno++];
			db1.i[1] = m88000.Regs[regno];
			fprintf(fp, s1, db1.d);
			*s2 = temp;
			break;
		}
	    }
	    s2--;
	}
	else
	    fputc(*s2, fp);
    }
	return 0;
}
