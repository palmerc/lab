/*
 * sim-bpred.c - sample branch predictor simulator implementation
 *
 * This file is a part of the SimpleScalar tool suite written by
 * Todd M. Austin as a part of the Multiscalar Research Project.
 *  
 * The tool suite is currently maintained by Doug Burger and Todd M. Austin.
 * 
 * Copyright (C) 1994, 1995, 1996, 1997, 1998 by Todd M. Austin
 *
 * This source file is distributed "as is" in the hope that it will be
 * useful.  The tool set comes with no warranty, and no author or
 * distributor accepts any responsibility for the consequences of its
 * use. 
 * 
 * Everyone is granted permission to copy, modify and redistribute
 * this tool set under the following conditions:
 * 
 *    This source code is distributed for non-commercial use only. 
 *    Please contact the maintainer for restrictions applying to 
 *    commercial use.
 *
 *    Permission is granted to anyone to make or distribute copies
 *    of this source code, either as received or modified, in any
 *    medium, provided that all copyright notices, permission and
 *    nonwarranty notices are preserved, and that the distributor
 *    grants the recipient permission for further redistribution as
 *    permitted by this document.
 *
 *    Permission is granted to distribute this file in compiled
 *    or executable form under the same conditions that apply for
 *    source code, provided that either:
 *
 *    A. it is accompanied by the corresponding machine-readable
 *       source code,
 *    B. it is accompanied by a written offer, with no time limit,
 *       to give anyone a machine-readable copy of the corresponding
 *       source code in return for reimbursement of the cost of
 *       distribution.  This written offer must permit verbatim
 *       duplication by anyone, or
 *    C. it is distributed by someone who received only the
 *       executable form, and is accompanied by a copy of the
 *       written offer of source code that they received concurrently.
 *
 * In other words, you are welcome to use, share and improve this
 * source file.  You are forbidden to forbid anyone else to use, share
 * and improve what you give them.
 *
 * INTERNET: dburger@cs.wisc.edu
 * US Mail:  1210 W. Dayton Street, Madison, WI 53706
 *
 * $Id: sim-bpred.c,v 1.1 1998/08/27 15:54:35 taustin Exp taustin $
 *
 * $Log: sim-bpred.c,v $
 * Revision 1.1  1998/08/27 15:54:35  taustin
 * Initial revision
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "regs.h"
#include "memory.h"
#include "loader.h"
#include "syscall.h"
#include "dlite.h"
#include "options.h"
#include "stats.h"
#include "bpred.h"
#include "sim.h"

/*
 * This file implements a branch predictor analyzer.
 */

/* simulated registers */
static struct regs_t regs;

/* simulated memory */
static struct mem_t *mem = NULL;

/* maximum number of inst's to execute */
static unsigned int max_insts;

/* branch predictor type {nottaken|taken|perfect|bimod|2lev} */
static char *pred_type;

/* bimodal predictor config (<table_size>) */
static int bimod_nelt = 1;
static int bimod_config[1] =
  { /* bimod tbl size */2048 };

/* 2-level predictor config (<l1size> <l2size> <hist_size> <xor>) */
static int twolev_nelt = 4;
static int twolev_config[4] =
  { /* l1size */1, /* l2size */1024, /* hist */8, /* xor */FALSE};

/* combining predictor config (<meta_table_size> */
static int comb_nelt = 1;
static int comb_config[1] =
  { /* meta_table_size */1024 };

/* return address stack (RAS) size */
static int ras_size = 8;

/* BTB predictor config (<num_sets> <associativity>) */
static int btb_nelt = 2;
static int btb_config[2] =
  { /* nsets */512, /* assoc */4 };

/* branch predictor */
static struct bpred_t *pred;

/* track number of insn and refs */
static counter_t sim_num_refs = 0;

/* total number of branches executed */
static counter_t sim_num_branches = 0;


/* add start */

#define DNA			(0)
#if defined(TARGET_PISA)

/* general register dependence decoders */
#define DGPR(N)			(N)
#define DGPR_D(N)		((N) &~1)

/* floating point register dependence decoders */
#define DFPR_L(N)		(((N)+32)&~1)
#define DFPR_F(N)		(((N)+32)&~1)
#define DFPR_D(N)		(((N)+32)&~1)

/* miscellaneous register dependence decoders */
#define DHI			(0+32+32)
#define DLO			(1+32+32)
#define DFCC			(2+32+32)
#define DTMP			(3+32+32)

#elif defined(TARGET_ALPHA)

/* general register dependence decoders, $r31 maps to DNA (0) */
#define DGPR(N)			(31 - (N)) /* was: (((N) == 31) ? DNA : (N)) */

/* floating point register dependence decoders */
#define DFPR(N)			(((N) == 31) ? DNA : ((N)+32))

/* miscellaneous register dependence decoders */
#define DFPCR			(0+32+32)
#define DUNIQ			(1+32+32)
#define DTMP			(2+32+32)

#else
#error No ISA target defined...
#endif

/* add end */

/* register simulator-specific options */
void
sim_reg_options(struct opt_odb_t *odb)
{
  opt_reg_header(odb, 
"sim-bpred: This simulator implements a branch predictor analyzer.\n"
		 );

  /* branch predictor options */
  opt_reg_note(odb,
"  Branch predictor configuration examples for 2-level predictor:\n"
"    Configurations:   N, M, W, X\n"
"      N   # entries in first level (# of shift register(s))\n"
"      W   width of shift register(s)\n"
"      M   # entries in 2nd level (# of counters, or other FSM)\n"
"      X   (yes-1/no-0) xor history and address for 2nd level index\n"
"    Sample predictors:\n"
"      GAg     : 1, W, 2^W, 0\n"
"      GAp     : 1, W, M (M > 2^W), 0\n"
"      PAg     : N, W, 2^W, 0\n"
"      PAp     : N, W, M (M == 2^(N+W)), 0\n"
"      gshare  : 1, W, 2^W, 1\n"
"  Predictor `comb' combines a bimodal and a 2-level predictor.\n"
               );

  /* instruction limit */
  opt_reg_uint(odb, "-max:inst", "maximum number of inst's to execute",
	       &max_insts, /* default */0,
	       /* print */TRUE, /* format */NULL);

  opt_reg_string(odb, "-bpred",
		 "branch predictor type {nottaken|taken|bimod|2lev|comb}",
                 &pred_type, /* default */"bimod",
                 /* print */TRUE, /* format */NULL);

  opt_reg_int_list(odb, "-bpred:bimod",
		   "bimodal predictor config (<table size>)",
		   bimod_config, bimod_nelt, &bimod_nelt,
		   /* default */bimod_config,
		   /* print */TRUE, /* format */NULL, /* !accrue */FALSE);

  opt_reg_int_list(odb, "-bpred:2lev",
                   "2-level predictor config "
		   "(<l1size> <l2size> <hist_size> <xor>)",
                   twolev_config, twolev_nelt, &twolev_nelt,
		   /* default */twolev_config,
                   /* print */TRUE, /* format */NULL, /* !accrue */FALSE);

  opt_reg_int_list(odb, "-bpred:comb",
		   "combining predictor config (<meta_table_size>)",
		   comb_config, comb_nelt, &comb_nelt,
		   /* default */comb_config,
		   /* print */TRUE, /* format */NULL, /* !accrue */FALSE);

  opt_reg_int(odb, "-bpred:ras",
              "return address stack size (0 for no return stack)",
              &ras_size, /* default */ras_size,
              /* print */TRUE, /* format */NULL);

  opt_reg_int_list(odb, "-bpred:btb",
		   "BTB config (<num_sets> <associativity>)",
		   btb_config, btb_nelt, &btb_nelt,
		   /* default */btb_config,
		   /* print */TRUE, /* format */NULL, /* !accrue */FALSE);
}

/* check simulator-specific option values */
void
sim_check_options(struct opt_odb_t *odb, int argc, char **argv)
{
  if (!mystricmp(pred_type, "taken"))
    {
      /* static predictor, not taken */
      pred = bpred_create(BPredTaken, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  else if (!mystricmp(pred_type, "nottaken"))
    {
      /* static predictor, taken */
      pred = bpred_create(BPredNotTaken, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  else if (!mystricmp(pred_type, "bimod"))
    {
      if (bimod_nelt != 1)
	fatal("bad bimod predictor config (<table_size>)");
      if (btb_nelt != 2)
	fatal("bad btb config (<num_sets> <associativity>)");

      /* bimodal predictor, bpred_create() checks BTB_SIZE */
      pred = bpred_create(BPred2bit,
			  /* bimod table size */bimod_config[0],
			  /* 2lev l1 size */0,
			  /* 2lev l2 size */0,
			  /* meta table size */0,
			  /* history reg size */0,
			  /* history xor address */0,
			  /* btb sets */btb_config[0],
			  /* btb assoc */btb_config[1],
			  /* ret-addr stack size */ras_size);
    }
  else if (!mystricmp(pred_type, "2lev"))
    {
      /* 2-level adaptive predictor, bpred_create() checks args */
      if (twolev_nelt != 4)
	fatal("bad 2-level pred config (<l1size> <l2size> <hist_size> <xor>)");
      if (btb_nelt != 2)
	fatal("bad btb config (<num_sets> <associativity>)");

      pred = bpred_create(BPred2Level,
			  /* bimod table size */0,
			  /* 2lev l1 size */twolev_config[0],
			  /* 2lev l2 size */twolev_config[1],
			  /* meta table size */0,
			  /* history reg size */twolev_config[2],
			  /* history xor address */twolev_config[3],
			  /* btb sets */btb_config[0],
			  /* btb assoc */btb_config[1],
			  /* ret-addr stack size */ras_size);
    }
  else if (!mystricmp(pred_type, "comb"))
    {
      /* combining predictor, bpred_create() checks args */
      if (twolev_nelt != 4)
	fatal("bad 2-level pred config (<l1size> <l2size> <hist_size> <xor>)");
      if (bimod_nelt != 1)
	fatal("bad bimod predictor config (<table_size>)");
      if (comb_nelt != 1)
	fatal("bad combining predictor config (<meta_table_size>)");
      if (btb_nelt != 2)
	fatal("bad btb config (<num_sets> <associativity>)");

      pred = bpred_create(BPredComb,
			  /* bimod table size */bimod_config[0],
			  /* l1 size */twolev_config[0],
			  /* l2 size */twolev_config[1],
			  /* meta table size */comb_config[0],
			  /* history reg size */twolev_config[2],
			  /* history xor address */twolev_config[3],
			  /* btb sets */btb_config[0],
			  /* btb assoc */btb_config[1],
			  /* ret-addr stack size */ras_size);
    }
  else
    fatal("cannot parse predictor type `%s'", pred_type);
}

/* register simulator-specific statistics */
void
sim_reg_stats(struct stat_sdb_t *sdb)
{
  stat_reg_counter(sdb, "sim_num_insn",
		   "total number of instructions executed",
		   &sim_num_insn, sim_num_insn, NULL);
  stat_reg_counter(sdb, "sim_num_refs",
		   "total number of loads and stores executed",
		   &sim_num_refs, 0, NULL);
  stat_reg_int(sdb, "sim_elapsed_time",
	       "total simulation time in seconds",
	       &sim_elapsed_time, 0, NULL);
  stat_reg_formula(sdb, "sim_inst_rate",
		   "simulation speed (in insts/sec)",
		   "sim_num_insn / sim_elapsed_time", NULL);

  stat_reg_counter(sdb, "sim_num_branches",
                   "total number of branches executed",
                   &sim_num_branches, /* initial value */0, /* format */NULL);
  stat_reg_formula(sdb, "sim_IPB",
                   "instruction per branch",
                   "sim_num_insn / sim_num_branches", /* format */NULL);

  /* register predictor stats */
  if (pred)
    bpred_reg_stats(pred, sdb);
}

/* initialize the simulator */
void
sim_init(void)
{
  sim_num_refs = 0;

  /* allocate and initialize register file */
  regs_init(&regs);

  /* allocate and initialize memory space */
  mem = mem_create("mem");
  mem_init(mem);
}

/* local machine state accessor */
static char *					/* err str, NULL for no err */
bpred_mstate_obj(FILE *stream,			/* output stream */
		 char *cmd,			/* optional command string */
		 struct regs_t *regs,		/* register to access */
		 struct mem_t *mem)		/* memory to access */
{
  /* just dump intermediate stats */
  sim_print_stats(stream);

  /* no error */
  return NULL;
}

/* load program into simulated state */
void
sim_load_prog(char *fname,		/* program to load */
	      int argc, char **argv,	/* program arguments */
	      char **envp)		/* program environment */
{
  /* load program text and data, set up environment, memory, and regs */
  ld_load_prog(fname, argc, argv, envp, &regs, mem, TRUE);

  /* initialize the DLite debugger */
  dlite_init(md_reg_obj, dlite_mem_obj, bpred_mstate_obj);
}

/* print simulator-specific configuration information */
void
sim_aux_config(FILE *stream)		/* output stream */
{
  /* nothing currently */
}

/* dump simulator-specific auxiliary simulator statistics */
void
sim_aux_stats(FILE *stream)		/* output stream */
{
  /* nada */
}

/* un-initialize simulator-specific state */
void
sim_uninit(void)
{
  /* nada */
}


/*
 * configure the execution engine
 */

/*
 * precise architected register accessors
 */

/* next program counter */
#define SET_NPC(EXPR)		(regs.regs_NPC = (EXPR))

/* target program counter */
#undef  SET_TPC
#define SET_TPC(EXPR)		(target_PC = (EXPR))

/* current program counter */
#define CPC			(regs.regs_PC)

/* general purpose registers */
#define GPR(N)			(regs.regs_R[N])
#define SET_GPR(N,EXPR)		(regs.regs_R[N] = (EXPR))

#if defined(TARGET_PISA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_L(N)		(regs.regs_F.l[(N)])
#define SET_FPR_L(N,EXPR)	(regs.regs_F.l[(N)] = (EXPR))
#define FPR_F(N)		(regs.regs_F.f[(N)])
#define SET_FPR_F(N,EXPR)	(regs.regs_F.f[(N)] = (EXPR))
#define FPR_D(N)		(regs.regs_F.d[(N) >> 1])
#define SET_FPR_D(N,EXPR)	(regs.regs_F.d[(N) >> 1] = (EXPR))

/* miscellaneous register accessors */
#define SET_HI(EXPR)		(regs.regs_C.hi = (EXPR))
#define HI			(regs.regs_C.hi)
#define SET_LO(EXPR)		(regs.regs_C.lo = (EXPR))
#define LO			(regs.regs_C.lo)
#define FCC			(regs.regs_C.fcc)
#define SET_FCC(EXPR)		(regs.regs_C.fcc = (EXPR))

#elif defined(TARGET_ALPHA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_Q(N)		(regs.regs_F.q[N])
#define SET_FPR_Q(N,EXPR)	(regs.regs_F.q[N] = (EXPR))
#define FPR(N)			(regs.regs_F.d[N])
#define SET_FPR(N,EXPR)		(regs.regs_F.d[N] = (EXPR))

/* miscellaneous register accessors */
#define FPCR			(regs.regs_C.fpcr)
#define SET_FPCR(EXPR)		(regs.regs_C.fpcr = (EXPR))
#define UNIQ			(regs.regs_C.uniq)
#define SET_UNIQ(EXPR)		(regs.regs_C.uniq = (EXPR))

#else
#error No ISA target defined...
#endif

/* precise architected memory state help functions */
#define READ_BYTE(SRC, FAULT)						\
  ((FAULT) = md_fault_none, MEM_READ_BYTE(mem, addr = (SRC)))
#define READ_HALF(SRC, FAULT)						\
  ((FAULT) = md_fault_none, MEM_READ_HALF(mem, addr = (SRC)))
#define READ_WORD(SRC, FAULT)						\
  ((FAULT) = md_fault_none, MEM_READ_WORD(mem, addr = (SRC)))
#ifdef HOST_HAS_QUAD
#define READ_QUAD(SRC, FAULT)						\
  ((FAULT) = md_fault_none, MEM_READ_QUAD(mem, addr = (SRC)))
#endif /* HOST_HAS_QUAD */

#define WRITE_BYTE(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, MEM_WRITE_BYTE(mem, addr = (DST), (SRC)))
#define WRITE_HALF(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, MEM_WRITE_HALF(mem, addr = (DST), (SRC)))
#define WRITE_WORD(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, MEM_WRITE_WORD(mem, addr = (DST), (SRC)))
#ifdef HOST_HAS_QUAD
#define WRITE_QUAD(SRC, DST, FAULT)					\
  ((FAULT) = md_fault_none, MEM_WRITE_QUAD(mem, addr = (DST), (SRC)))
#endif /* HOST_HAS_QUAD */

/* system call handler macro */
#define SYSCALL(INST)	sys_syscall(&regs, mem_access, mem, INST, TRUE)

/* start simulation, program loaded, processor precise state initialized */
void
sim_main(void)
{
  md_inst_t inst;
  register md_addr_t addr, target_PC;
  enum md_opcode op;
  register int is_write;
  int stack_idx;
  enum md_fault_type fault;

  int out1, out2, in1, in2, in3;	  /* register names */
  int v_out1, v_out2, v_in1, v_in2, v_in3;/* register values */
  int vl_out1, vl_out2, vl_in1, vl_in2, vl_in3;
  md_addr_t addr_dispatch;              /* access_address */
  int m_size;                           /* access_size */
  FILE *stream;
  md_addr_t predicted_PC;

#include "readdata.h"
  struct Pip_Inst_Record pipe_data;

  fprintf(stderr, "sim: ** starting functional simulation w/ predictors **\n");

  /* set up initial default next PC */
  regs.regs_NPC = regs.regs_PC + sizeof(md_inst_t);

  /* check for DLite debugger entry condition */
  if (dlite_check_break(regs.regs_PC, /* no access */0, /* addr */0, 0, 0))
    dlite_main(regs.regs_PC - sizeof(md_inst_t), regs.regs_PC,
	       sim_num_insn, &regs, mem);

  while (TRUE)
    {
      /* maintain $r0 semantics */
      regs.regs_R[MD_REG_ZERO] = 0;
#ifdef TARGET_ALPHA
      regs.regs_F.d[MD_REG_ZERO] = 0.0;
#endif /* TARGET_ALPHA */

      /* get the next instruction to execute */
      mem_access(mem, Read, regs.regs_PC, &inst, sizeof(md_inst_t));

      /* keep an instruction count */
      sim_num_insn++;

      /* set default reference address and access mode */
      addr = 0; is_write = FALSE;

      /* set default fault - none */
      fault = md_fault_none;

      /* decode the instruction */
      MD_SET_OPCODE(op, inst);

      /* execute the instruction */
      switch (op)
	{
#define DEFINST(OP,MSK,O1,O2,I1,I2,I3,VO1,LO1,VO2,LO2,ADDR,             \
                VI1,LI1,VI2,LI2,VI3,LI3,M_Size)                         \
	case OP:							\
	  in1 = I1; in2 = I2; in3 = I3;					\
	  v_in1 = VI1; v_in2 = VI2; v_in3 = VI3;			\
	  vl_in1 = LI1; vl_in2 = LI2; vl_in3 = LI3;			\
          SYMCAT(OP,_IMPL);						\
	  out1 = O1; out2 = O2;						\
	  v_out1 = VO1; v_out2 = VO2;					\
	  vl_out1 = LO1; vl_out2 = LO2;					\
	  addr_dispatch = ADDR;						\
	  m_size = M_Size;						\
          break;
#define DEFLINK(OP,MSK,NAME,MASK,SHIFT)					\
        case OP:							\
          panic("attempted to execute a linking opcode");
#define CONNECT(OP)
#define DECLARE_FAULT(FAULT)						\
	  { fault = (FAULT); break; }
#include "operand.def"
	default:
	  panic("attempted to execute a bogus opcode");
	}

      if (fault != md_fault_none)
	fatal("fault (%d) detected @ 0x%08p", fault, regs.regs_PC);

      if (MD_OP_FLAGS(op) & F_MEM)
	{
	  sim_num_refs++;
	  if (MD_OP_FLAGS(op) & F_STORE)
	    is_write = TRUE;
	}

      if (MD_OP_FLAGS(op) & F_CTRL)
	{
	  md_addr_t pred_PC;
	  struct bpred_update_t update_rec;

	  sim_num_branches++;

	  if (pred)
	    {
	      /* get the next predicted fetch address */
	      pred_PC = bpred_lookup(pred,
				     /* branch addr */regs.regs_PC,
				     /* target */target_PC,
				     /* inst opcode */op,
				     /* call? */MD_IS_CALL(op),
				     /* return? */MD_IS_RETURN(op),
				     /* stash an update ptr */&update_rec,
				     /* stash return stack ptr */&stack_idx);

	      /* valid address returned from branch predictor? */
	      if (!pred_PC)
		{
		  /* no predicted taken target, attempt not taken target */
		  pred_PC = regs.regs_PC + sizeof(md_inst_t);
		}

	      bpred_update(pred,
			   /* branch addr */regs.regs_PC,
			   /* resolved branch target */regs.regs_NPC,
			   /* taken? */regs.regs_NPC != (regs.regs_PC +
							 sizeof(md_inst_t)),
			   /* pred taken? */pred_PC != (regs.regs_PC +
							sizeof(md_inst_t)),
			   /* correct pred? */pred_PC == regs.regs_NPC,
			   /* opcode */op,
			   /* predictor update pointer */&update_rec);
	    }
	  predicted_PC = pred_PC;
	}

      /*   $B$3$3$+$i2<$r8+$l$P$I$NJQ?t$K2?$,F~$C$F$$$k$+J,$+$k$O$:!#(B */
      stream = stdout;
      fprintf(stream, "############################################################ \n");
      fprintf(stream, " --- trace count(%d) \n", (int)sim_num_insn);
      fprintf(stream, "       opcode: %s, inst: `",
	      MD_OP_NAME(op));
      md_print_insn(inst, regs.regs_PC, stream);
      fprintf(stream, "'\n");
      myfprintf(stream, "         PC: 0x%08p, NPC: 0x%08p",
		regs.regs_PC, regs.regs_NPC);
      if(pred)
	myfprintf(stream, " (pred_PC: 0x%08p)\n",
		  regs.regs_PC, regs.regs_NPC, predicted_PC);
      else
	myfprintf(stream, "\n");
      fprintf(stream," |  in(r%02d,r%02d,r%02d),out(r%02d,r%02d),\n",
	      in1, in2, in3, out1, out2);
      fprintf(stream," |  IN(%8x,%8x,%8x),OUT(%8x,%8x)addr(%8x)|\n",
	      v_in1, v_in2, v_in3, v_out1, v_out2, addr_dispatch);
      /* $B%@%V%k%o!<%I$@$C$?>l9g(B,$B$3$NJQ?t$,%;%C%H$5$l$k(B */
      fprintf(stream," |  in(%8x,%8x,%8x),out(%8x,%8x)         |\n",
	      vl_in1, vl_in2, vl_in3, vl_out1, vl_out2);

      /* added by ibuki */
      /* $B%Q%$%W$G(Bksim$B$K%G!<%?$rEO$9(B */
      if (op == MD_NOP_OP)
	{
	  pipe_data.func_unit=NOOP;
	}
      else if (MD_OP_FLAGS(op) & F_MEM)
	{
	  if (MD_OP_FLAGS(op) & F_STORE)
	    pipe_data.func_unit=STORE;
	  else if (MD_OP_FLAGS(op) & F_LOAD)
	    pipe_data.func_unit=LOAD;
	  pipe_data.address=addr_dispatch;
	}
      else if (MD_OP_FLAGS(op) & F_CTRL)
	{
	  /* branch */
	  pipe_data.func_unit=BRANCH;
	  if (MD_IS_INDIR(op))
	    pipe_data.brn_type=UNCON_INDIRE;
	  else if ((MD_OP_FLAGS(op) & (F_CTRL|F_UNCOND|F_DIRJMP)) == (F_CTRL|F_UNCOND|F_DIRJMP))
	    pipe_data.brn_type=UNCON_DIRE;
	  else if ((MD_OP_FLAGS(op) & (F_CTRL|F_UNCOND|F_INDIRJMP)) == (F_CTRL|F_UNCOND|F_INDIRJMP))
	    pipe_data.brn_type=UNCON_INDIRE;
	  else if ((MD_OP_FLAGS(op) & (F_CTRL|F_COND|F_DIRJMP)) == (F_CTRL|F_UNCOND|F_INDIRJMP))
	    pipe_data.brn_type=CON_DIRE;
	  else if (MD_IS_CALL(op))
	    pipe_data.brn_type=JALL_CALL;
	  else if (MD_IS_RETURN(op))
	    pipe_data.brn_type=JR_RETURN;
	}
      else
	{
	    pipe_data.func_unit=OTHER;
	}
      
      pipe_data.sourceA=in1;
      pipe_data.sourceB=in2;
      pipe_data.destination=out1;
      pipe_data.pc=regs.regs_PC;       
      pipe_data.address=addr_dispatch;   
      pipe_data.m_size=m_size;
    

#ifdef DEBUG
      if (op == MD_NOP_OP)
	{
	    fprintf(stream, "     NOP instruction \n");
	}
      else if (MD_OP_FLAGS(op) & F_MEM)
	{
	  if (MD_OP_FLAGS(op) & F_STORE)
	    fprintf(stream, "   STORE instruction \n");
	  else if (MD_OP_FLAGS(op) & F_LOAD)
	    fprintf(stream, "    LOAD instruction \n");
	}
      else if (MD_OP_FLAGS(op) & F_CTRL)
	{
	  /* branch */

	  fprintf(stream, "   BRANCH instruction \n");

	  if ((MD_OP_FLAGS(op) & (F_CTRL|F_UNCOND|F_DIRJMP)) == (F_CTRL|F_UNCOND|F_DIRJMP))
	    fprintf(stream, "  uncondition   direct\n");	   
	  else if ((MD_OP_FLAGS(op) & (F_CTRL|F_UNCOND|F_INDIRJMP)) == (F_CTRL|F_UNCOND|F_INDIRJMP))
	    fprintf(stream, "  uncondition indirect\n");	   
	  else if ((MD_OP_FLAGS(op) & (F_CTRL|F_COND|F_DIRJMP)) == (F_CTRL|F_UNCOND|F_INDIRJMP))
	    fprintf(stream, "    condition   direct\n");	   
	  else if (MD_IS_CALL(op))
	    fprintf(stream, "       function  call \n");
	  else if (MD_IS_RETURN(op))
	    fprintf(stream, "       function return\n");
	}
      else
	{
	    fprintf(stream, "    other instruction \n");
	}
      fprintf(stream,"func_unit=%d\n",pipe_data.func_unit);
      fprintf(stream,"sourceA=%d\n",in1);
      fprintf(stream,"sourceB=%d\n",in2);
      fprintf(stream,"destination%d\n",out1);
      fprintf(stream,"pc=%d\n",regs.regs_PC);       
      fprintf(stream,"address=%d\n",addr_dispatch);   
      fprintf(stream,"Branch_Type=%d\n",pipe_data.brn_type);
      fprintf(stream,"m_size=%d",m_size);
#endif


      /* check for DLite debugger entry condition */
      if (dlite_check_break(regs.regs_NPC,
			    is_write ? ACCESS_WRITE : ACCESS_READ,
			    addr, sim_num_insn, sim_num_insn))
	dlite_main(regs.regs_PC, regs.regs_NPC, sim_num_insn, &regs, mem);

      /* go to the next instruction */
      regs.regs_PC = regs.regs_NPC;
      regs.regs_NPC += sizeof(md_inst_t);

      /* finish early? */
      if (max_insts && sim_num_insn >= max_insts)
	return;
    }
}