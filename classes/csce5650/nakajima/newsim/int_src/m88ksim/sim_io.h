/* @(#) File name: sim_io.h   Ver: 3.1   Cntl date: 1/20/89 14:39:07 */

static char copyright19[] = "Copyright (c) Motorola, Inc. 1986";



#define S_FCLOSE    267
#define S_FFLUSH    270
#define S_FEOF      279
#define S_FERROR    280
#define S_CLEARERR  281
#define S_FILENO    282
#define S_FOPEN     266
#define S_FREOPEN   283
#define S_FDOPEN    284
#define S_FREAD     268
#define S_FWRITE    269
#define S_FSEEK     273
#define S_FTELL     274
#define S_REWIND    278
#define S_GETC      285
#define S_GETCHAR   286
#define S_FGETC     287
#define S_GETW      288
#define S_GETS      276
#define S_FGETS     277
#define S_PRINTF    260
#define S_FPRINTF   261
#define S_SPRINTF   262
#define S_PUTC      289
#define S_PUTCHAR   290
#define S_FPUTC     291
#define S_PUTW      292
#define S_PUTS      293
#define S_FPUTS     294
#define S_SCANF     263
#define S_FSCANF    264
#define S_SSCANF    265
#define S_SETBUF    295
#define S_SETBUFFER 296
#define S_SETLINEBUF 297
#define S_UNGETC    298
#define S__FILBUF   271
#define S__FLSBUF   272
#define S_MALLOC    275

/* traps 300-399 are reserved for motorola use only */
#define S_BLENTER	300
#define S_BLEXIT	301
#define S_SEPARATE	302
#define S_ANYFAIL	303
#define S_ERRMESG	304
#define S_PEND		305
#define S_DEBUG		306
#define S_IPOSTCALL	307
#define S_FPOSTCALL	308
#define S_SPOSTCALL	309
#define S_LPOSTCALL	310
#define S_ISSU		311
#define S_PASSNAME	312
#define S_SETUP		313
#define S_OPOSTCALL	314
#define S_SYSTEM    315 

#define S_GETTIME       400 
#define S_CEIL			401
#define S_COSH			402
#define S_EXP			403
#define S_FLOOR			404
#define S_LOG			405
#define S_LOG10			406
#define S_POW			407
#define S_SINH			408
#define S_SQRT			409
#define S_TANH			410
