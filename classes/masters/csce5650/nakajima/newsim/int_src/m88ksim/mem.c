/* @(#) File name: mem.c   Ver: 3.1   Cntl date: 1/20/89 14:19:42 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

# ifdef m88000
# undef m88000
# endif

# include "functions.h"


/* Declare the main memory segment list */

struct mem_segs memory[MAXSEGS] = { 0 };

/* Declare the MC88000 Processor model */

struct PROCESSOR m88000 = { 0 };

/* simulator execution state and error flags */

int memerr,
    unixvec = 1,
    dis_word_align = 0,
    vecyes = 0,
    usecmmu = 0,
    debugflag = 0,
    stdio_en = 0,
    brkenabled,
    brkpend,
    sym_addr = 1,
    pipe_open = 0;
