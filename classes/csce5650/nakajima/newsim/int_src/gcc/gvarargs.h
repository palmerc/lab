#ifndef _gvarargs_h_
#define _gvarargs_h_

/* changed by kanou & hasimoto */
#include "/usr/include/stdarg.h"
/* #include "src.alt/stdarg.h" */
#undef  va_dcl
#define va_dcl
#define VARGLIST(arg)	arg, ...
#define FIRST_ARG(arg)
#define VA_START(p, f, t)  va_start(p, f)

#endif /* _gvarargs_h_ */
