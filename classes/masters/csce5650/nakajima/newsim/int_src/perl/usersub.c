/* $RCSfile: usersub.c,v $$Revision: 4.0.1.2 $$Date: 92/06/08 16:04:24 $
 *
 *  This file contains stubs for routines that the user may define to
 *  set up glue routines for C libraries or to decrypt encrypted scripts
 *  for execution.
 *
 * $Log:	usersub.c,v $
 * Revision 4.0.1.2  92/06/08  16:04:24  lwall
 * patch20: removed implicit int declarations on functions
 * 
 * Revision 4.0.1.1  91/11/11  16:47:17  lwall
 * patch19: deleted some unused functions from usersub.c
 * 
 * Revision 4.0  91/03/20  01:55:56  lwall
 * 4.0 baseline.
 * 
 */

#include "EXTERN.h"
#include "perl.h"

int
userinit()
{
    return 0;
}

/*
 * The following is supplied by John Macdonald as a means of decrypting
 * and executing (presumably proprietary) scripts that have been encrypted
 * by a (presumably secret) method.  The idea is that you supply your own
 * routine in place of cryptfilter (which is purposefully a very weak
 * encryption).  If an encrypted script is detected, a process is forked
 * off to run the cryptfilter routine as input to perl.
 */

