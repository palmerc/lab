/* @(#) File name: sim_io.c   Ver: 3.1   Cntl date: 1/20/89 14:41:01 */

static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


# ifdef m88000
# undef m88000
# endif
#include "functions.h"
#include <math.h>
#include "sim_io.h"
#include "trans.h"
#include <stdio.h>
#include <string.h>


#define RDWRBUF	(2048)

extern  int local_flag;
char progname[80];




#ifdef NOT_SPEC95
int fake_io(int vec)
{
	char ch;
	static char *head = inbuf;
	static char *tail = inbuf;
	static char inbuf[500] = { 0 };

	switch(vec)
	{
		case 0x1FC:	/*print*/

			trans.adr1 = m88000.Regs[2];   /* address of print buffer is in r2 */

			while(1)
			{
				if(rdwr((M_DATA | M_RD), trans.adr1, &ch, 1) == -1)
					return(-1);
				if(ch == '\0')
					break;
				putchar(ch);
				trans.adr1++;
			}
			break;

		case 0x1FD:	/* putchar - char sent from reg #2 */
			putchar(m88000.Regs[2]);
			break;	

		case 0x1FE:	/* getchar - char returned in reg #2 */
			if(head == tail)
			{
				do
				{
					*head = (ch = getchar());
					if(head++ == (inbuf+499))
						head = inbuf;
				}
				while(ch != '\n');
			}
			m88000.Regs[2] = *tail++;
			if(tail == &inbuf[499])
				tail = inbuf;
			break;	

		case 0x1ff:			/* exit */
			return(-1);

		default: 			/* unsupported trap */
			 return(1);
	}
	IP += 4;
	return(0);
}
#endif


#ifdef NOT_SPEC95
int stdio_io(int vec)
{
	union {
		double d;
		int    i[2];
	} db1, db2, db3;
	register int arg1, arg2, arg3, arg4;
	int i, j, k;
	char str1[150], str2[150], rdwrbuf[RDWRBUF];
	FILE *fp_out = stdout;

	arg1 = getarg(1);
	arg2 = getarg(2);
	arg3 = getarg(3);
	arg4 = getarg(4);

	switch(vec)
	{			/* unsupported trap */
		default: 
			 return(1);

		case 0x1fb:   /* Unix times */
/*			addr = m88000.Regs[2];			*/
			i = readtime();
			PPrintf("at clock = %d\n",i);
/*			membyte(addr+0) = (char) uext(i,0,8);	*/
/*			membyte(addr+1) = (char) uext(i,8,8);	*/
/*			membyte(addr+2) = (char) uext(i,16,8);	*/
/*			membyte(addr+3) = (char) uext(i,24,8);	*/
			break;	

		case S_PRINTF:        /* printf     */
			if(!copystr(1, str1))
			    return(-1);
			sim_printf(fp_out, str1, 2);
			break;	

		case S_FPRINTF:       /* fprintf:   */
			if(!copystr(2, str1))
			    return(-1);
			sim_printf(ckiob((TARGETFILE *)arg1), str1, 3);
			break;

		case S_SPRINTF:       /* _sprintf:  */
			Eprintf("sorry, sprintf not yet supported\n");
			break;

		case S_SCANF:         /* _scanf:    */
		case S_FSCANF:     /* _fscanf:   */
		case S_SSCANF:     /* _sscanf:   */
		    Eprintf("sorry, scanf not yet supported\n");
		    break;

		case S_FOPEN:
		    if(!copystr(1, str1) || !copystr(2, str2))
			return(-1);
		    m88000.Regs[2] = (int)fopen(str1, str2);
                           break;

		case S_FREOPEN:
		    if(!copystr(1, str1) || !copystr(2, str2))
			return(-1);
		    m88000.Regs[2] = (int)freopen(str1, str2, ckiob((TARGETFILE *)arg3));
                           break;

		case S_FDOPEN:
		    if(!copystr(2, str1))
			return(-1);
		    m88000.Regs[2] = (int)fdopen(arg1, str1);
		    break;

		case S_FREAD:
		    k = arg2 * arg3;
		    j = 0;
		    do
		    {
		 	i = fread(rdwrbuf, sizeof(char), RDWRBUF, ckiob((TARGETFILE *)getarg(4)));
		 	if(rdwr((M_DATA | M_RD), arg1, rdwrbuf, (i < k) ? k: i) == -1)
					return(-1);
			j += (i < k) ? k: i;
			arg1 += i;
		    }
		    while((k -= i) < 0);
		    m88000.Regs[2] = j;
		    break;

		case S_FWRITE:
		    k = arg2 * arg3;
		    j = 0;
		    do
		    {
			if((i = k / RDWRBUF) == 0)
				i = k % RDWRBUF;
		 	if(rdwr((M_DATA | M_WR), arg1, rdwrbuf, i) == -1)
				return(-1);
		 	if(i != fwrite(rdwrbuf, sizeof(char), i, ckiob((TARGETFILE *)getarg(4))))
			{
				j = 0;
				break;
			}
			j += i;
			arg1 += i;
		    }
		    while((k -= i) < 0);
		    m88000.Regs[2] = j;
		    break;

		case S_FCLOSE:
		    m88000.Regs[2] = (int)fclose(ckiob((TARGETFILE *)arg1));
		    break;

		case S_FFLUSH:
		    m88000.Regs[2] = (int)fflush(ckiob((TARGETFILE *)arg1));
		    break;

		case 0x203:
		case 0x204:
		case S__FILBUF:    /* __filbuf:  */
		case S__FLSBUF:    /* __flsbuf:  */
		case S_MALLOC:     /* _malloc:   */
		case S_SETBUF:
		case S_SETBUFFER:
		case S_SETLINEBUF:
		    Eprintf("unsupported trap\n");
		    break;

		case S_FSEEK:
		    m88000.Regs[2] = (int)fseek(ckiob((TARGETFILE *)arg1), arg2, arg3);
		    break;

		case S_GETS:
		    if(fgets(rdwrbuf, sizeof(rdwrbuf), stdin) == 0)
			m88000.Regs[2] = 0;
		    else
		    {
			i = strchr(rdwrbuf, '\0') - rdwrbuf + 1;
		 	if(rdwr((M_DATA | M_WR), arg1, rdwrbuf, i) == -1)
				return(-1);
		    }
		    break;

		case S_FGETS:
		    i = (arg2 < sizeof(rdwrbuf)) ? arg2: sizeof(rdwrbuf);
		    if(fgets(rdwrbuf, i, ckiob((TARGETFILE *)arg3)) == 0)
			m88000.Regs[2] = 0;
		    else
		    {
			i = strchr(rdwrbuf, '\0') - rdwrbuf + 1;
		 	if(rdwr((M_DATA | M_WR), arg1, rdwrbuf, i) == -1)
				return(-1);
		    }
		    break;

		case S_REWIND:
		    rewind(ckiob((TARGETFILE *)arg1));
			    break;

		case S_FTELL:
		    m88000.Regs[2] = (int)ftell(ckiob((TARGETFILE *)arg1));
		    break;

		case S_FEOF:
		    m88000.Regs[2] = feof((FILE *)ckiob((TARGETFILE *)arg1));
		    break;

		case S_FERROR:
		    m88000.Regs[2] = ferror((FILE *)ckiob((TARGETFILE *)arg1));
		    break;

		case S_CLEARERR:
		    clearerr((FILE *)ckiob((TARGETFILE *)arg1));
		    break;

		case S_FILENO:
		    m88000.Regs[2] = (int)fileno((FILE *)ckiob((TARGETFILE *)arg1));
		    break;

		case S_GETC:
		case S_FGETC:
		    m88000.Regs[2] = (int)getc((FILE *)ckiob((TARGETFILE *)arg1));
		    break;

		case S_GETCHAR:
		    m88000.Regs[2] = (int)getchar();
		    break;

		case S_GETW:
		    m88000.Regs[2] = (int)getw(ckiob((TARGETFILE *)arg1));
		    break;

		case S_PUTC:
		case S_FPUTC:
		    m88000.Regs[2] = (int)fputc(arg1,ckiob((TARGETFILE *)arg2));
		    break;

		case S_PUTCHAR:
		    m88000.Regs[2] = (int)putchar(arg1);
		    break;


		case S_PUTW:
		    m88000.Regs[2] = (int)putw(arg1,ckiob((TARGETFILE *)arg2));
		    break;

		case S_PUTS:
		    if(!copystr(1, str1))
			return(-1);
		    m88000.Regs[2] = (int)puts(str1);
		    break;

		case S_FPUTS:
		    if(!copystr(1, str1))
			return(-1);
		    m88000.Regs[2] = (int)fputs(str1, ckiob((TARGETFILE *)arg2));
		    break;

		case S_UNGETC:
		    m88000.Regs[2] = (int)ungetc(arg1, ckiob((TARGETFILE *)arg2));
		    break;
		case S_GETTIME:
			m88000.Regs[2] = (int)readtime();
			break;
		case S_SYSTEM:
			if(!copystr(1, str1))
				return(-1);
			m88000.Regs[2] = (int)system(str1);
			break;
		case S_EXP:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = exp(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_LOG:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = log(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_LOG10:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = log10(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_SQRT:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = sqrt(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_SINH:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = sinh(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_COSH:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = cosh(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_TANH:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = tanh(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
     			break;	
		case S_FLOOR:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = floor(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_CEIL:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.d = ceil(db1.d);
			m88000.Regs[2] = db2.i[0];
			m88000.Regs[3] = db2.i[1];
			break;
		case S_POW:
			db1.i[0] = arg1;
			db1.i[1] = arg2;
			db2.i[0] = arg3;
			db2.i[1] = arg4;
			db3.d = pow(db1.d,db2.d);
			m88000.Regs[2] = db3.i[0];
			m88000.Regs[3] = db3.i[1];
			break;
	}

	IP += 4;
	return(0);
}
#endif


int getarg(int n)       /* return the nth argument after a subroutine call */
{
	if (n < 8)
		return(m88000.Regs[n+1]);    /* base of arg list is register r2 */
	else
		Eprintf("too many call arguments passed in simulator i/o\n");
	return 0xdeadbeef;
}

char *copystr(int argc,char *ptr)
{
	char *saveptr = ptr;
	int i;
	union {
		char c[4];
		int l;
	} buf;

	trans.adr1 = getarg(argc);

	while(1)
	{
		if(rdwr((M_DATA | M_RD), trans.adr1, &buf.l, 4) == -1)
			return(0);

		for(i = 0; i < 4; i++)
		{
			*ptr++ = buf.c[i];
			if(!buf.c[i])
				return(saveptr);
		}
		trans.adr1 += 4;
	}
}




	/* This function tests for the presence of I/O performed
	at a stdio level by means of traps. This is presumed to mean
	that trap vectors should be interpreted as being for
	STDIO. The default is otherwise, namely that trap vectors
	are to perform Unix System V system calls. The method is
	to identify the presence or otherwise of "_printf" in the
	symbol table of the executable. If it does not exist, then
	traps are assumed to be System V calls. If it does exist,
	and it is a "tb0 0,r0,0x104" then stdio vectors are assumed.
	Otherwise, System V calls assumed.	

	A value of 0 is returned if System V calls are to be assumed,
	and 1 if sydio calls are assumed. */

int stdio_enable(void)
{
	char *err = NULL;
	unsigned int value = 0;
	unsigned instruct = 0;


	if (getexpr("_printf",&err,&value) != 0) return 0;
	/* If we get here, _printf was found */

	if(rdwr((M_INSTR|M_RD),value,&instruct,sizeof(instruct)) == -1)
		return 0;

	if(instruct != 0xF000D104)
		return 0;

	return 1;
}
