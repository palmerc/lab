/* $RCSfile: dump.c,v $$Revision: 4.0.1.2 $$Date: 92/06/08 13:14:22 $
 *
 *    Copyright (c) 1991, Larry Wall
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log:	dump.c,v $
 * Revision 4.0.1.2  92/06/08  13:14:22  lwall
 * patch20: removed implicit int declarations on funcions
 * patch20: fixed confusion between a *var's real name and its effective name
 * 
 * Revision 4.0.1.1  91/06/07  10:58:44  lwall
 * patch4: new copyright notice
 * 
 * Revision 4.0  91/03/20  01:08:25  lwall
 * 4.0 baseline.
 * 
 */

#include "EXTERN.h"
#include "perl.h"

