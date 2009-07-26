/* $RCSfile: doio.c,v $$Revision: 4.0.1.6 $$Date: 92/06/11 21:08:16 $
 *
 *    Copyright (c) 1991, Larry Wall
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log:	doio.c,v $
 * Revision 4.0.1.6  92/06/11  21:08:16  lwall
 * patch34: some systems don't declare h_errno extern in header files
 * 
 * Revision 4.0.1.5  92/06/08  13:00:21  lwall
 * patch20: some machines don't define ENOTSOCK in errno.h
 * patch20: new warnings for failed use of stat operators on filenames with \n
 * patch20: wait failed when STDOUT or STDERR reopened to a pipe
 * patch20: end of file latch not reset on reopen of STDIN
 * patch20: seek(HANDLE, 0, 1) went to eof because of ancient Ultrix workaround
 * patch20: fixed memory leak on system() for vfork() machines
 * patch20: get*by* routines now return something useful in a scalar context
 * patch20: h_errno now accessible via $?
 * 
 * Revision 4.0.1.4  91/11/05  16:51:43  lwall
 * patch11: prepared for ctype implementations that don't define isascii()
 * patch11: perl mistook some streams for sockets because they return mode 0 too
 * patch11: reopening STDIN, STDOUT and STDERR failed on some machines
 * patch11: certain perl errors should set EBADF so that $! looks better
 * patch11: truncate on a closed filehandle could dump
 * patch11: stats of _ forgot whether prior stat was actually lstat
 * patch11: -T returned true on NFS directory
 * 
 * Revision 4.0.1.3  91/06/10  01:21:19  lwall
 * patch10: read didn't work from character special files open for writing
 * patch10: close-on-exec wrongly set on system file descriptors
 * 
 * Revision 4.0.1.2  91/06/07  10:53:39  lwall
 * patch4: new copyright notice
 * patch4: system fd's are now treated specially
 * patch4: added $^F variable to specify maximum system fd, default 2
 * patch4: character special files now opened with bidirectional stdio buffers
 * patch4: taintchecks could improperly modify parent in vfork()
 * patch4: many, many itty-bitty portability fixes
 * 
 * Revision 4.0.1.1  91/04/11  17:41:06  lwall
 * patch1: hopefully straightened out some of the Xenix mess
 * 
 * Revision 4.0  91/03/20  01:07:06  lwall
 * 4.0 baseline.
 * 
 */

#include "EXTERN.h"
#include "perl.h"






int laststatval = -1;
int laststype = O_STAT;

static char* warn_nl = "Unsuccessful %s on filename containing newline";

bool
do_open(stab,name,len)
STAB *stab;
register char *name;
int len;
{
    FILE *fp;
    register STIO *stio = stab_io(stab);
    char *myname = savestr(name);
    int result;
    int fd;
    int writing = 0;
    char mode[3];		/* stdio file mode ("r\0" or "r+\0") */
    FILE *saveifp = Nullfp;
    FILE *saveofp = Nullfp;
    char savetype = ' ';

    mode[0] = mode[1] = mode[2] = '\0';
    name = myname;
    forkprocess = 1;		/* assume true if no fork */
    while (len && isSPACE(name[len-1]))
	name[--len] = '\0';
    if (!stio)
	stio = stab_io(stab) = stio_new();
    else if (stio->ifp) {
	fd = fileno(stio->ifp);
	if (stio->type == '-')
	    result = 0;
	else if (fd <= maxsysfd) {
	    saveifp = stio->ifp;
	    saveofp = stio->ofp;
	    savetype = stio->type;
	    result = 0;
	}
	else if (stio->type == '|')
	    result = mypclose(stio->ifp);
	else if (stio->ifp != stio->ofp) {
	    if (stio->ofp) {
		result = fclose(stio->ofp);
		fclose(stio->ifp);	/* clear stdio, fd already closed */
	    }
	    else
		result = fclose(stio->ifp);
	}
	else
	    result = fclose(stio->ifp);
	if (result == EOF && fd > maxsysfd)
	    fprintf(stderr,"Warning: unable to close filehandle %s properly.\n",
	      stab_ename(stab));
	stio->ofp = stio->ifp = Nullfp;
    }
    if (*name == '+' && len > 1 && name[len-1] != '|') {	/* scary */
	mode[1] = *name++;
	mode[2] = '\0';
	--len;
	writing = 1;
    }
    else  {
	mode[1] = '\0';
    }
    stio->type = *name;
    if (*name == '|') {
	/*SUPPRESS 530*/
	for (name++; isSPACE(*name); name++) ;
	fp = mypopen(name,"w");
	writing = 1;
    }
    else if (*name == '>') {
	name++;
	if (*name == '>') {
	    mode[0] = stio->type = 'a';
	    name++;
	}
	else
	    mode[0] = 'w';
	writing = 1;
	if (*name == '&') {
	  duplicity:
	    name++;
	    while (isSPACE(*name))
		name++;
	    if (isDIGIT(*name))
		fd = atoi(name);
	    else {
		stab = stabent(name,FALSE);
		if (!stab || !stab_io(stab)) {
#ifdef EINVAL
		    errno = EINVAL;
#endif
		    goto say_false;
		}
		if (stab_io(stab) && stab_io(stab)->ifp) {
		    fd = fileno(stab_io(stab)->ifp);
		    if (stab_io(stab)->type == 's')
			stio->type = 's';
		}
		else
		    fd = -1;
	    }
	    if (!(fp = fdopen(fd = dup(fd),mode))) {
		close(fd);
	    }
	}
	else {
	    while (isSPACE(*name))
		name++;
	    if (strEQ(name,"-")) {
		fp = stdout;
		stio->type = '-';
	    }
	    else  {
		fp = fopen(name,mode);
	    }
	}
    }
    else {
	if (*name == '<') {
	    mode[0] = 'r';
	    name++;
	    while (isSPACE(*name))
		name++;
	    if (*name == '&')
		goto duplicity;
	    if (strEQ(name,"-")) {
		fp = stdin;
		stio->type = '-';
	    }
	    else
		fp = fopen(name,mode);
	}
	else if (name[len-1] == '|') {
	    name[--len] = '\0';
	    while (len && isSPACE(name[len-1]))
		name[--len] = '\0';
	    /*SUPPRESS 530*/
	    for (; isSPACE(*name); name++) ;
	    fp = mypopen(name,"r");
	    stio->type = '|';
	}
	else {
	    stio->type = '<';
	    /*SUPPRESS 530*/
	    for (; isSPACE(*name); name++) ;
	    if (strEQ(name,"-")) {
		fp = stdin;
		stio->type = '-';
	    }
	    else
		fp = fopen(name,"r");
	}
    }
    if (!fp) {
	if (dowarn && stio->type == '<' && index(name, '\n'))
	    warn(warn_nl, "open");
	Safefree(myname);
	goto say_false;
    }
    Safefree(myname);
    if (stio->type &&
      stio->type != '|' && stio->type != '-') {
	if (fstat(fileno(fp),&statbuf) < 0) {
	    (void)fclose(fp);
	    goto say_false;
	}
	if (S_ISSOCK(statbuf.st_mode))
	    stio->type = 's';	/* in case a socket was passed in to us */
    }
    if (saveifp) {		/* must use old fp? */
	fd = fileno(saveifp);
	if (saveofp) {
	    fflush(saveofp);		/* emulate fclose() */
	    if (saveofp != saveifp) {	/* was a socket? */
		fclose(saveofp);
		if (fd > 2)
		    Safefree(saveofp);
	    }
	}
	if (fd != fileno(fp)) {
	    int pid;
	    STR *str;

	    dup2(fileno(fp), fd);
	    str = afetch(fdpid,fileno(fp),TRUE);
	    pid = str->str_u.str_useful;
	    str->str_u.str_useful = 0;
	    str = afetch(fdpid,fd,TRUE);
	    str->str_u.str_useful = pid;
	    fclose(fp);

	}
	fp = saveifp;
	clearerr(fp);
    }
    stio->ifp = fp;
    if (writing) {
	if (stio->type == 's'
	  || (stio->type == '>' && S_ISCHR(statbuf.st_mode)) ) {
	    if (!(stio->ofp = fdopen(fileno(fp),"w"))) {
		fclose(fp);
		stio->ifp = Nullfp;
		goto say_false;
	    }
	}
	else
	    stio->ofp = fp;
    }
    return TRUE;

say_false:
    stio->ifp = saveifp;
    stio->ofp = saveofp;
    stio->type = savetype;
    return FALSE;
}

FILE *
nextargv(stab)
register STAB *stab;
{
    register STR *str;
    int fileuid;
    int filegid;
    static int filemode = 0;
    static int lastfd;
    static char *oldname;

    if (!argvoutstab)
	argvoutstab = stabent("ARGVOUT",TRUE);
    if (filemode & (S_ISUID|S_ISGID)) {
	fflush(stab_io(argvoutstab)->ifp);  /* chmod must follow last write */
	(void)chmod(oldname,filemode);
    }
    filemode = 0;
    while (alen(stab_xarray(stab)) >= 0) {
	str = ashift(stab_xarray(stab));
	str_sset(stab_val(stab),str);
	STABSET(stab_val(stab));
	oldname = str_get(stab_val(stab));
	if (do_open(stab,oldname,stab_val(stab)->str_cur)) {
	    if (inplace) {
		if (strEQ(oldname,"-")) {
		    str_free(str);
		    defoutstab = stabent("STDOUT",TRUE);
		    return stab_io(stab)->ifp;
		}
		filemode = statbuf.st_mode;
		fileuid = statbuf.st_uid;
		filegid = statbuf.st_gid;
		if (!S_ISREG(filemode)) {
		    warn("Can't do inplace edit: %s is not a regular file",
		      oldname );
		    do_close(stab,FALSE);
		    str_free(str);
		    continue;
		}
		if (*inplace) {
		    str_cat(str,inplace);
		}
		else {
		    fatal("Can't do inplace edit without backup");
		}

		str_nset(str,">",1);
		str_cat(str,oldname);
		errno = 0;		/* in case sprintf set errno */
		if (!do_open(argvoutstab,str->str_ptr,str->str_cur)) {
		    warn("Can't do inplace edit on %s: %s",
		      oldname, strerror(errno) );
		    do_close(stab,FALSE);
		    str_free(str);
		    continue;
		}
		defoutstab = argvoutstab;
		lastfd = fileno(stab_io(argvoutstab)->ifp);
		(void)fstat(lastfd,&statbuf);
		(void)chmod(oldname,filemode);
	    }
	    str_free(str);
	    return stab_io(stab)->ifp;
	}
	else
	    fprintf(stderr,"Can't open %s: %s\n",str_get(str), strerror(errno));
	str_free(str);
    }
    if (inplace) {
	(void)do_close(argvoutstab,FALSE);
	defoutstab = stabent("STDOUT",TRUE);
    }
    return Nullfp;
}


bool
do_close(stab,explicit)
STAB *stab;
bool explicit;
{
    bool retval = FALSE;
    register STIO *stio;
    int status;

    if (!stab)
	stab = argvstab;
    if (!stab) {
	errno = EBADF;
	return FALSE;
    }
    stio = stab_io(stab);
    if (!stio) {		/* never opened */
	if (dowarn && explicit)
	    warn("Close on unopened file <%s>",stab_ename(stab));
	return FALSE;
    }
    if (stio->ifp) {
	if (stio->type == '|') {
	    status = mypclose(stio->ifp);
	    retval = (status == 0);
	    statusvalue = (unsigned short)status & 0xffff;
	}
	else if (stio->type == '-')
	    retval = TRUE;
	else {
	    if (stio->ofp && stio->ofp != stio->ifp) {		/* a socket */
		retval = (fclose(stio->ofp) != EOF);
		fclose(stio->ifp);	/* clear stdio, fd already closed */
	    }
	    else
		retval = (fclose(stio->ifp) != EOF);
	}
	stio->ofp = stio->ifp = Nullfp;
    }
    if (explicit)
	stio->lines = 0;
    stio->type = ' ';
    return retval;
}

bool
do_eof(stab)
STAB *stab;
{
    register STIO *stio;
    int ch;

    if (!stab) {			/* eof() */
	if (argvstab)
	    stio = stab_io(argvstab);
	else
	    return TRUE;
    }
    else
	stio = stab_io(stab);

    if (!stio)
	return TRUE;

    while (stio->ifp) {


	ch = getc(stio->ifp);
	if (ch != EOF) {
	    (void)ungetc(ch, stio->ifp);
	    return FALSE;
	}
	if (!stab) {			/* not necessarily a real EOF yet? */
	    if (!nextargv(argvstab))	/* get another fp handy */
		return TRUE;
	}
	else
	    return TRUE;		/* normal fp, definitely end of file */
    }
    return TRUE;
}

long
do_tell(stab)
STAB *stab;
{
    register STIO *stio;

    if (!stab)
	goto phooey;

    stio = stab_io(stab);
    if (!stio || !stio->ifp)
	goto phooey;

#ifdef ULTRIX_STDIO_BOTCH
    if (feof(stio->ifp))
	(void)fseek (stio->ifp, 0L, 2);		/* ultrix 1.2 workaround */
#endif

    return ftell(stio->ifp);

phooey:
    if (dowarn)
	warn("tell() on unopened file");
    errno = EBADF;
    return -1L;
}

bool
do_seek(stab, pos, whence)
STAB *stab;
long pos;
int whence;
{
    register STIO *stio;

    if (!stab)
	goto nuts;

    stio = stab_io(stab);
    if (!stio || !stio->ifp)
	goto nuts;

#ifdef ULTRIX_STDIO_BOTCH
    if (feof(stio->ifp))
	(void)fseek (stio->ifp, 0L, 2);		/* ultrix 1.2 workaround */
#endif

    return fseek(stio->ifp, pos, whence) >= 0;

nuts:
    if (dowarn)
	warn("seek() on unopened file");
    errno = EBADF;
    return FALSE;
}

int
do_ctl(optype,stab,func,argstr)
int optype;
STAB *stab;
int func;
STR *argstr;
{
    register STIO *stio;
    register char *s;
    int retval;

    if (!stab || !argstr || !(stio = stab_io(stab)) || !stio->ifp) {
	errno = EBADF;	/* well, sort of... */
	return -1;
    }

    if (argstr->str_pok || !argstr->str_nok) {
	if (!argstr->str_pok)
	    s = str_get(argstr);

	retval = 256;			/* otherwise guess at what's safe */
	if (argstr->str_cur < retval) {
	    Str_Grow(argstr,retval+1);
	    argstr->str_cur = retval;
	}

	s = argstr->str_ptr;
	s[argstr->str_cur] = 17;	/* a little sanity check here */
    }
    else {
	retval = (int)str_gnum(argstr);
#ifdef DOSISH
	s = (char*)(long)retval;		/* ouch */
#else
	s = (char*)retval;		/* ouch */
#endif
    }

#ifndef lint
	fatal("fcntl is not implemented");
#else /* lint */
    retval = 0;
#endif /* lint */

    if (argstr->str_pok) {
	if (s[argstr->str_cur] != 17)
	    fatal("Return value overflowed string");
	s[argstr->str_cur] = 0;		/* put our null back */
    }
    return retval;
}

int
do_stat(str,arg,gimme,arglast)
STR *str;
register ARG *arg;
int gimme;
int *arglast;
{
    register ARRAY *ary = stack;
    register int sp = arglast[0] + 1;
    int max = 13;

    if ((arg[1].arg_type & A_MASK) == A_WORD) {
	tmpstab = arg[1].arg_ptr.arg_stab;
	if (tmpstab != defstab) {
	    laststype = O_STAT;
	    statstab = tmpstab;
	    str_set(statname,"");
	    if (!stab_io(tmpstab) || !stab_io(tmpstab)->ifp ||
	      fstat(fileno(stab_io(tmpstab)->ifp),&statcache) < 0) {
		max = 0;
		laststatval = -1;
	    }
	}
	else if (laststatval < 0)
	    max = 0;
    }
    else {
	str_set(statname,str_get(ary->ary_array[sp]));
	statstab = Nullstab;
	    laststatval = stat(str_get(statname),&statcache);
	if (laststatval < 0) {
	    if (dowarn && index(str_get(statname), '\n'))
		warn(warn_nl, "stat");
	    max = 0;
	}
    }

    if (gimme != G_ARRAY) {
	if (max)
	    str_sset(str,&str_yes);
	else
	    str_sset(str,&str_undef);
	STABSET(str);
	ary->ary_array[sp] = str;
	return sp;
    }
    sp--;
    if (max) {
#ifndef lint
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)(int)statcache.st_dev)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)(int)statcache.st_ino)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_mode)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_nlink)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_uid)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_gid)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)(int)statcache.st_rdev)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_size)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_atime)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_mtime)));
	(void)astore(ary,++sp,
	  str_2mortal(str_nmake((double)statcache.st_ctime)));
	(void)astore(ary,++sp,
	  str_2mortal(str_make("",0)));
	(void)astore(ary,++sp,
	  str_2mortal(str_make("",0)));
#else /* lint */
	(void)astore(ary,++sp,str_nmake(0.0));
#endif /* lint */
    }
    return sp;
}


int					/*SUPPRESS 590*/
do_truncate(str,arg,gimme,arglast)
STR *str;
register ARG *arg;
int gimme;
int *arglast;
{
    register ARRAY *ary = stack;
    register int sp = arglast[0] + 1;
    off_t len = (off_t)str_gnum(ary->ary_array[sp+1]);
    int result = 1;
    STAB *tmpstab;

    fatal("truncate not implemented");
    return -1;
}

int
looks_like_number(str)
STR *str;
{
    register char *s;
    register char *send;

    if (!str->str_pok)
	return TRUE;
    s = str->str_ptr; 
    send = s + str->str_cur;
    while (isSPACE(*s))
	s++;
    if (s >= send)
	return FALSE;
    if (*s == '+' || *s == '-')
	s++;
    while (isDIGIT(*s))
	s++;
    if (s == send)
	return TRUE;
    if (*s == '.') 
	s++;
    else if (s == str->str_ptr)
	return FALSE;
    while (isDIGIT(*s))
	s++;
    if (s == send)
	return TRUE;
    if (*s == 'e' || *s == 'E') {
	s++;
	if (*s == '+' || *s == '-')
	    s++;
	while (isDIGIT(*s))
	    s++;
    }
    while (isSPACE(*s))
	s++;
    if (s >= send)
	return TRUE;
    return FALSE;
}

bool
do_print(str,fp)
register STR *str;
FILE *fp;
{
    register char *tmps;

    if (!fp) {
	if (dowarn)
	    warn("print to unopened file");
	errno = EBADF;
	return FALSE;
    }
    if (!str)
	return TRUE;
    if (ofmt &&
      ((str->str_nok && str->str_u.str_nval != 0.0)
       || (looks_like_number(str) && str_gnum(str) != 0.0) ) ) {
	fprintf(fp, ofmt, str->str_u.str_nval);
	return !ferror(fp);
    }
    else {
	tmps = str_get(str);
	if (*tmps == 'S' && tmps[1] == 't' && tmps[2] == 'B' && tmps[3] == '\0'
	  && str->str_cur == sizeof(STBP) && strlen(tmps) < str->str_cur) {
	    STR *tmpstr = str_mortal(&str_undef);
	    stab_efullname(tmpstr,((STAB*)str));/* a stab value, be nice */
	    str = tmpstr;
	    tmps = str->str_ptr;
	    putc('*',fp);
	}
	if (str->str_cur && (fwrite(tmps,1,str->str_cur,fp) == 0 || ferror(fp)))
	    return FALSE;
    }
    return TRUE;
}

bool
do_aprint(arg,fp,arglast)
register ARG *arg;
register FILE *fp;
int *arglast;
{
    register STR **st = stack->ary_array;
    register int sp = arglast[1];
    register int retval;
    register int items = arglast[2] - sp;

    if (!fp) {
	if (dowarn)
	    warn("print to unopened file");
	errno = EBADF;
	return FALSE;
    }
    st += ++sp;
    if (arg->arg_type == O_PRTF) {
	do_sprintf(arg->arg_ptr.arg_str,items,st);
	retval = do_print(arg->arg_ptr.arg_str,fp);
    }
    else {
	retval = (items <= 0);
	for (; items > 0; items--,st++) {
	    if (retval && ofslen) {
		if (fwrite(ofs, 1, ofslen, fp) == 0 || ferror(fp)) {
		    retval = FALSE;
		    break;
		}
	    }
	    if (!(retval = do_print(*st, fp)))
		break;
	}
	if (retval && orslen)
	    if (fwrite(ors, 1, orslen, fp) == 0 || ferror(fp))
		retval = FALSE;
    }
    return retval;
}

int
mystat(arg,str)
ARG *arg;
STR *str;
{
    STIO *stio;

    if (arg[1].arg_type & A_DONT) {
	stio = stab_io(arg[1].arg_ptr.arg_stab);
	if (stio && stio->ifp) {
	    statstab = arg[1].arg_ptr.arg_stab;
	    str_set(statname,"");
	    laststype = O_STAT;
	    return (laststatval = fstat(fileno(stio->ifp), &statcache));
	}
	else {
	    if (arg[1].arg_ptr.arg_stab == defstab)
		return laststatval;
	    if (dowarn)
		warn("Stat on unopened file <%s>",
		  stab_ename(arg[1].arg_ptr.arg_stab));
	    statstab = Nullstab;
	    str_set(statname,"");
	    return (laststatval = -1);
	}
    }
    else {
	statstab = Nullstab;
	str_set(statname,str_get(str));
	laststype = O_STAT;
	laststatval = stat(str_get(str),&statcache);
	if (laststatval < 0 && dowarn && index(str_get(str), '\n'))
	    warn(warn_nl, "stat");
	return laststatval;
    }
}

int
mylstat(arg,str)
ARG *arg;
STR *str;
{
    if (arg[1].arg_type & A_DONT) {
	if (arg[1].arg_ptr.arg_stab == defstab) {
	    if (laststype != O_LSTAT)
		fatal("The stat preceding -l _ wasn't an lstat");
	    return laststatval;
	}
	fatal("You can't use -l on a filehandle");
    }

    laststype = O_LSTAT;
    statstab = Nullstab;
    str_set(statname,str_get(str));
    laststatval = stat(str_get(str),&statcache);
    if (laststatval < 0 && dowarn && index(str_get(str), '\n'))
	warn(warn_nl, "lstat");
    return laststatval;
}

STR *
do_fttext(arg,str)
register ARG *arg;
STR *str;
{
    int i;
    int len;
    int odd = 0;
    STDCHAR tbuf[512];
    register STDCHAR *s;
    register STIO *stio;

    if (arg[1].arg_type & A_DONT) {
	if (arg[1].arg_ptr.arg_stab == defstab) {
	    if (statstab)
		stio = stab_io(statstab);
	    else {
		str = statname;
		goto really_filename;
	    }
	}
	else {
	    statstab = arg[1].arg_ptr.arg_stab;
	    str_set(statname,"");
	    stio = stab_io(statstab);
	}
	if (stio && stio->ifp) {
	    fatal("-T and -B not implemented on filehandles");
	}
	else {
	    if (dowarn)
		warn("Test on unopened file <%s>",
		  stab_ename(arg[1].arg_ptr.arg_stab));
	    errno = EBADF;
	    return &str_undef;
	}
    }
    else {
	statstab = Nullstab;
	str_set(statname,str_get(str));
      really_filename:
	i = open(str_get(str),0);
	if (i < 0) {
	    if (dowarn && index(str_get(str), '\n'))
		warn(warn_nl, "open");
	    return &str_undef;
	}
	fstat(i,&statcache);
	len = read(i,tbuf,512);
	(void)close(i);
	if (len <= 0) {
	    if (S_ISDIR(statcache.st_mode) && arg->arg_type == O_FTTEXT)
		return &str_no;		/* special case NFS directories */
	    return &str_yes;		/* null file is anything */
	}
	s = tbuf;
    }

    /* now scan s to look for textiness */

    for (i = 0; i < len; i++,s++) {
	if (!*s) {			/* null never allowed in text */
	    odd += len;
	    break;
	}
	else if (*s & 128)
	    odd++;
	else if (*s < 32 &&
	  *s != '\n' && *s != '\r' && *s != '\b' &&
	  *s != '\t' && *s != '\f' && *s != 27)
	    odd++;
    }

    if ((odd * 10 > len) == (arg->arg_type == O_FTTEXT)) /* allow 10% odd */
	return &str_no;
    else
	return &str_yes;
}

static char **Argv = Null(char **);
static char *Cmd = Nullch;

bool
do_aexec(really,arglast)
STR *really;
int *arglast;
{
    register STR **st = stack->ary_array;
    register int sp = arglast[1];
    register int items = arglast[2] - sp;
    register char **a;
    char *tmps;

    if (items) {
	New(401,Argv, items+1, char*);
	a = Argv;
	for (st += ++sp; items > 0; items--,st++) {
	    if (*st)
		*a++ = str_get(*st);
	    else
		*a++ = "";
	}
	*a = Nullch;
	if (really && *(tmps = str_get(really)))
	    execvp(tmps,Argv);
	else
	    execvp(Argv[0],Argv);
    }
    do_execfree();
    return FALSE;
}

void
do_execfree()
{
    if (Argv) {
	Safefree(Argv);
	Argv = Null(char **);
    }
    if (Cmd) {
	Safefree(Cmd);
	Cmd = Nullch;
    }
}

bool
do_exec(cmd)
char *cmd;
{
    register char **a;
    register char *s;
    char flags[10];

    /* save an extra exec if possible */


    /* see if there are shell metacharacters in it */

    /*SUPPRESS 530*/
    for (s = cmd; *s && isALPHA(*s); s++) ;	/* catch VAR=val gizmo */
    if (*s == '=')
	goto doshell;
    for (s = cmd; *s; s++) {
	if (*s != ' ' && !isALPHA(*s) && index("$&*(){}[]'\";\\|?<>~`\n",*s)) {
	    if (*s == '\n' && !s[1]) {
		*s = '\0';
		break;
	    }
	  doshell:
	    execl("/bin/sh","sh","-c",cmd,(char*)0);
	    return FALSE;
	}
    }
    New(402,Argv, (s - cmd) / 2 + 2, char*);
    Cmd = nsavestr(cmd, s-cmd);
    a = Argv;
    for (s = Cmd; *s;) {
	while (*s && isSPACE(*s)) s++;
	if (*s)
	    *(a++) = s;
	while (*s && !isSPACE(*s)) s++;
	if (*s)
	    *s++ = '\0';
    }
    *a = Nullch;
    if (Argv[0]) {
	execvp(Argv[0],Argv);
	if (errno == ENOEXEC) {		/* for system V NIH syndrome */
	    do_execfree();
	    goto doshell;
	}
    }
    do_execfree();
    return FALSE;
}


int
do_gpwent(which,gimme,arglast)
int which;
int gimme;
int *arglast;
{
    fatal("password routines not implemented");
    return -1;
}

int
do_ggrent(which,gimme,arglast)
int which;
int gimme;
int *arglast;
{
    fatal("group routines not implemented");
    return -1;
}

int
do_dirop(optype,stab,gimme,arglast)
int optype;
STAB *stab;
int gimme;
int *arglast;
{
phooey:
    fatal("Unimplemented directory operation");
    return -1;
}

int
apply(type,arglast)
int type;
int *arglast;
{
    register STR **st = stack->ary_array;
    register int sp = arglast[1];
    register int items = arglast[2] - sp;
    register int val;
    register int val2;
    register int tot = 0;
    char *s;

    switch (type) {
    case O_CHMOD:
	if (--items > 0) {
	    tot = items;
	    val = (int)str_gnum(st[++sp]);
	    while (items--) {
		if (chmod(str_get(st[++sp]),val))
		    tot--;
	    }
	}
	break;
    case O_UNLINK:
	tot = items;
	while (items--) {
	    s = str_get(st[++sp]);
	    if (euid || unsafe) {
		if (UNLINK(s))
		    tot--;
	    }
	    else {	/* don't let root wipe out directories without -U */
		if (stat(s,&statbuf) < 0 || S_ISDIR(statbuf.st_mode))
		    tot--;
		else {
		    if (UNLINK(s))
			tot--;
		}
	    }
	}
	break;
    case O_UTIME:
	if (items > 2) {
	    struct {
		long    actime;
		long	modtime;
	    } utbuf;

	    Zero(&utbuf, sizeof utbuf, char);
	    utbuf.actime = (long)str_gnum(st[++sp]);    /* time accessed */
	    utbuf.modtime = (long)str_gnum(st[++sp]);    /* time modified */
	    items -= 2;
#ifndef lint
	    tot = items;
	    while (items--) {
		if (utime(str_get(st[++sp]),&utbuf))
		    tot--;
	    }
#endif
	}
	else
	    items = 0;
	break;
    }
    return tot;
}

/* Do the permissions allow some operation?  Assumes statcache already set. */

int
cando(bit, effective, statbufp)
int bit;
int effective;
register struct stat *statbufp;
{
#ifdef DOSISH
    /* [Comments and code from Len Reed]
     * MS-DOS "user" is similar to UNIX's "superuser," but can't write
     * to write-protected files.  The execute permission bit is set
     * by the Miscrosoft C library stat() function for the following:
     *		.exe files
     *		.com files
     *		.bat files
     *		directories
     * All files and directories are readable.
     * Directories and special files, e.g. "CON", cannot be
     * write-protected.
     * [Comment by Tom Dinger -- a directory can have the write-protect
     *		bit set in the file system, but DOS permits changes to
     *		the directory anyway.  In addition, all bets are off
     *		here for networked software, such as Novell and
     *		Sun's PC-NFS.]
     */

     /* Atari stat() does pretty much the same thing. we set x_bit_set_in_stat
      * too so it will actually look into the files for magic numbers
      */
     return (bit & statbufp->st_mode) ? TRUE : FALSE;

#else /* ! MSDOS */
    if ((effective ? euid : uid) == 0) {	/* root is special */
	if (bit == S_IXUSR) {
	    if (statbufp->st_mode & 0111 || S_ISDIR(statbufp->st_mode))
		return TRUE;
	}
	else
	    return TRUE;		/* root reads and writes anything */
	return FALSE;
    }
    if (statbufp->st_uid == (effective ? euid : uid) ) {
	if (statbufp->st_mode & bit)
	    return TRUE;	/* ok as "user" */
    }
    else if (ingroup((int)statbufp->st_gid,effective)) {
	if (statbufp->st_mode & bit >> 3)
	    return TRUE;	/* ok as "group" */
    }
    else if (statbufp->st_mode & bit >> 6)
	return TRUE;	/* ok as "other" */
    return FALSE;
#endif /* ! MSDOS */
}

int
ingroup(testgid,effective)
int testgid;
int effective;
{
    if (testgid == (effective ? egid : gid))
	return TRUE;
    return FALSE;
}

