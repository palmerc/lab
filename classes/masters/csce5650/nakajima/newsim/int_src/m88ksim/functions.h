#include "instab.h"
#include "cmds.h"
#include "cmmu.h"
#include "syms.h"
#if 0
#include <unistd.h>
/* unistd is supposed to provide POSIX std definitions only */
#endif
#include <stdlib.h>
#include <stdio.h>
#include "sim.h"
#include "targstdio.h"
#include "scnhdr.h"
#include "ld_coff.h"
union MEMDATA {
        unsigned int    d[3];
        unsigned int   l;
        unsigned short  s;
        unsigned char   c;
};
void addd(ULONG asign,ULONG amanthi,ULONG amantlo,ULONG bsign,ULONG bmanthi,
ULONG bmantlo,ULONG *resign,ULONG *resmanthi,ULONG *resmantlo,ULONG s);
void adds(ULONG asign,ULONG amant,ULONG bsign,ULONG bmant,
ULONG *resign,ULONG *resmant,ULONG s);
void alignd(int aexp,ULONG *amanthi,ULONG *amantlo,int bexp,
ULONG *bmanthi,ULONG *bmantlo,int *resexp,ULONG *s);
void aligns(int aexp,ULONG *amant,int bexp,ULONG *bmant,int *resexp,ULONG *s);
int assembler( char *buffer, union opcode *ptr, unsigned int addr);
int reterr(char *ptr);
void a_pname(char *ptr);
struct instruction *a_choice(struct instruction *cmdptr, char *cmdbuf);
int procopers(char **opers);
int mkwrd(struct instruction *cmd, union opcode *ptr);
int a_sfu1(struct instruction *cmd, union opcode *ptr);
int cksfu1(union opcode *ptr, char *sufstr);
int a_ctl(struct instruction *cmd, union opcode *ptr);
int integer(struct instruction *cmd, union opcode *ptr);
int trpbci(struct instruction *cmd, union opcode *ptr);
int tbnd(struct instruction *cmd, union opcode *ptr);
int mem(struct instruction *cmd, union opcode *ptr);
int ckmem(union opcode *ptr, int flag);
int xfrbci(struct instruction *cmd, union opcode *ptr);
int logical(struct instruction *cmd, union opcode *ptr);
int xfrri(struct instruction *cmd, union opcode *ptr);
int ffirst(struct instruction *cmd, union opcode *ptr);
int rte(struct instruction *cmd, union opcode *ptr);
int xfra(struct instruction *cmd, union opcode *ptr);
int bits(struct instruction *cmd, union opcode *ptr);
int validreg(int code1);
int dbrks(void);
int ckbrkpts(unsigned int addr, int brktype);
int brkptenb(void);
void settmpbrk(int addr);
void rsttmpbrk(void);
FILE *ckiob(TARGETFILE *fp);
UINT classify(UINT inst);
struct cmdstruct *parse(char *ptr, struct dirtree *list);
void pname(char *ptr);
struct cmdstruct *choice(struct cmdstruct *cmdptr,char *cmdbuf);
int load_data (struct cmmu *cmmu_ptr, unsigned int *data,
unsigned int *log_addr_ptr,int writeflag,int probeflag);
int tablewalk (struct cmmu *cmmu_ptr,unsigned int *phy_address,
unsigned int log_addr,unsigned int patc_log_entry,int writeflag,
int updateM,int probeflag);
int check_BATC(struct cmmu *cmmu_ptr, unsigned int batc_log_entry);
int check_PATC(struct cmmu *cmmu_ptr, unsigned int patc_log_entry);
int get_from_BATC (struct cmmu *cmmu_ptr,unsigned int *entry,
unsigned int batc_log_entry,int writeflag,int probeflag);
int get_from_PATC(struct cmmu *cmmu_ptr,unsigned int *entry,
unsigned int patc_log_entry,int writeflag,unsigned int log_addr,
int probeflag);
int update_PATC(struct cmmu *cmmu_ptr,unsigned int patc_log_entry,
unsigned int phy_address,unsigned int page_desc,unsigned int seg_desc,
unsigned int area_desc,int writeflag);
int load_from_cache(struct cmmu *cmmu_ptr,unsigned int *data,
unsigned int address,int writeflag);
int check_cache(struct cmmu *cmmu_ptr,unsigned int setnum,
unsigned int comparetag);
int cache_hit(struct cmmu *cmmu_ptr,unsigned int setnum,
unsigned int comparetag,unsigned int datawrd);
void mark_lru(int ord, struct cmmu *cmmu_ptr,unsigned int setnum,int line);
void find_change_order(struct cmmu *cmmu_ptr,
unsigned int setnum,int oldord,int neword);
int cache_miss (struct cmmu *cmmu_ptr,unsigned int *data,unsigned int setnum,
unsigned int comparetag,unsigned int datawrd,
unsigned int address,int writeflag);
int store_data (struct cmmu *cmmu_ptr,unsigned int address,int storeword);
int ctrl_space_read (unsigned int log_addr,unsigned int *data);
int ctrl_space_write (unsigned int log_addr,int  storedata);
int change_cache_set(struct cmmu *cmmu_ptr,unsigned int setnum,int storedata);
void cmmu_ctrl_func(struct cmmu *cmmu_ptr);
void flush_all (struct cmmu *cmmu_ptr, int super_atc);
void flush_byPandS(struct cmmu *cmmu_ptr, unsigned int comp_LPA, int seg_flag);
int write_line(struct cmmu *cmmu_ptr, unsigned int setnum,unsigned int line);
void redo_set_lru(struct cmmu *cmmu_ptr, unsigned int setnum);
void cmmu_init(void);
void cache_reset(struct cmmu *cmmu_ptr);
void write_memory(int word,unsigned int address);
void init_batc_HDWRD(struct cmmu *cmmu_ptr,int index1,int index2);
int getexpr(char *ptr,char **eptr,unsigned int *value);
int getrange(void);
int getdata(char *ptr,char **eptr,unsigned int *value);
char *simatoi(char *ptr,unsigned int *value);
unsigned int atosf(char *str);
void str_toupper(char *str);
int wrctlregs(int sfunum,int regnum,int value,int ctlwr);
void init_processor(void);
char *dis(unsigned int opc,unsigned int  disaddr);
int memi(void);
int imm16(void);
int logi(void);
int inti(void);
int sfu(void);
int xfr(void);
int ctl(void);
int sfu1(void);
int gen1(void);
int gen2(void);
void rrrdisp(void);
int rrrmdisp(int scale);
int rrr(void);
char *nameofbit(int code);
char *cndtype(int code);
void divd(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *reshi,ULONG *reslo,ULONG *s);
void divs(ULONG a,ULONG b,ULONG *res,ULONG *s);
int ldfrommem(struct IR_FIELDS *ir,int bytes,int signflag);
int sttomem(struct IR_FIELDS *ir, int bytes);
int dacc(struct IR_FIELDS *ir,int rdwr,int signflag,
int size,union MEMDATA * data);
void out_to_in(struct IR_FIELDS *ir,int s_u,int bytes,int rdwr,
unsigned int data,unsigned int addr);
void out_to_log(struct IR_FIELDS *ir,int s_u,int bytes,int rdwr,
unsigned int data,unsigned int reply,unsigned int addr);
int Data_path (void);
int addunsigned(unsigned int l1,unsigned int l2,struct IR_FIELDS *ir,
int sub,int overflow);
int sext(int src,int off,int wid);
int uext(int src,int off,int wid);
int make(int src,int off,int wid);
int cmmu_function1(void);
void cmmu_function2(struct IR_FIELDS *ir);
int execute(struct IR_FIELDS *ir,struct SIM_FLAGS *f,struct mem_wrd *memaddr);
void display_trace(struct IR_FIELDS *ir,struct mem_wrd  *memaddr);
int fadd(struct IR_FIELDS *ir);
ULONG fadd64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,ULONG *desthi,
ULONG *destlo,ULONG rnd,ULONG rndprec);
ULONG fadds(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd);
ULONG fcdi(ULONG ahi,ULONG alo,int *dest,ULONG rnd);
ULONG fcds(ULONG ahi,ULONG alo,ULONG *dest,ULONG rnd);
ULONG fcid(int a,ULONG *desthi,ULONG *destlo);
ULONG fcis(int a,ULONG *dest,ULONG rnd);
int fcmp(struct IR_FIELDS *ir);
ULONG fcmp64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,ULONG *dest);
ULONG fcmps(ULONG aa,ULONG bb,ULONG *dest);
ULONG fcsd(ULONG a,ULONG *desthi,ULONG *destlo);
ULONG fcsi(ULONG aa,int *dest,ULONG rnd);
int fdiv(struct IR_FIELDS *ir);
ULONG fdiv64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *desthi,ULONG *destlo,ULONG rnd);
ULONG fdivs(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd,int singleflag);
void floaterr(char *msg,ULONG a_sign,ULONG b_sign,
int a_exp,int b_exp, ULONG a_manthi,ULONG b_manthi,ULONG a_mantlo,
ULONG b_mantlo,int singleflag, ULONG rnd, ULONG grd, ULONG rd, ULONG stky);
void pre_except(ULONG a_sign,ULONG b_sign,int a_exp,int b_exp,ULONG a_manthi,
ULONG b_manthi,ULONG a_mantlo,ULONG b_mantlo,int singleflag);
void impre_except(ULONG a_sign,int a_exp,ULONG a_manthi,ULONG a_mantlo,
int singleflag,ULONG rnd,ULONG grd,ULONG rd,ULONG stky);
void flt(struct IR_FIELDS *ir);
int fmul(struct IR_FIELDS *ir);
ULONG fmul64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,
ULONG *desthi,ULONG *destlo,ULONG rnd,ULONG prec);
ULONG fmuls(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd);
int fpunimp(unsigned int instr_encode);
int fsub(struct IR_FIELDS *ir);
ULONG fsub64(ULONG ahi,ULONG alo,ULONG bhi,ULONG blo,ULONG *desthi,
ULONG *destlo,ULONG rnd,ULONG rndprec);
ULONG fsubs(ULONG aa,ULONG bb,ULONG *dest,ULONG rnd);
int goexec(void);
int trexec(unsigned int cnt);
int convert_int(struct IR_FIELDS *ir, unsigned int rnd);
int transinit(char *errmsg,int size);
int rdwr(int rdwrflg,unsigned int memadr,void *srcdesta,int size);
int ckquit(void);
int checklmt(unsigned int addr1,int segtype);
struct mem_wrd *getmemptr(unsigned int addr1,int segtype);
int getpmem(unsigned int addr);
int getmem(int base, int size, int segtype,unsigned int  phyaddr);
void releasemem(void);
void releasepmem(void);
void releaseseg(int i);
void intswap(unsigned int *destptr,unsigned int *srcptr);
void shortswap(unsigned short *destptr,unsigned short *srcptr);
int loadmem(int rdwrflg,unsigned int memadr,void *srcdesta,int size);
int main(int argc, char **argv);
int presetsim(char *file);
void multd(ULONG asign,int aexp,ULONG amanthi,ULONG amantlo,ULONG bsign,
int bexp,ULONG bmanthi,ULONG bmantlo,ULONG *resign,int *resexp,
ULONG *resmanthi,ULONG *resmantlo,ULONG *s);
void mults(ULONG asign,int aexp,ULONG amant,ULONG bsign,int bexp,ULONG bmant,
ULONG *resign,int *resexp,ULONG *resmant,ULONG *s);
void normalized(int *resexp,ULONG *resmanthi,ULONG *resmantlo,ULONG *s);
void normalizes(int *resexp,ULONG *resmant,ULONG *s);
void open_output (char *filename);
int Pc(struct mem_wrd *memaddr,struct IR_FIELDS *ir,struct SIM_FLAGS *f);
int checkforjump (struct IR_FIELDS *ir,struct mem_wrd *memaddr);
int checkfortrap (struct IR_FIELDS *ir,struct mem_wrd *memaddr);
void rdexec(int format);
void dispSFU(int *p);
ULONG reserved(ULONG src1hi,ULONG src1lo,ULONG src2hi,ULONG src2lo,
int opchoice);
ULONG reserves(ULONG src1,ULONG src2,int opchoice);
ULONG return_double(ULONG sign,int exp,ULONG manthi,ULONG mantlo,ULONG *desthi,
ULONG *destlo,ULONG rnd,ULONG grd,ULONG rd,ULONG stky);
ULONG return_single(ULONG sign,int exp,ULONG mant,ULONG *dest,ULONG rnd,
ULONG grd,ULONG rd,ULONG stky);
int rmSFU(int num,int i);
char *getregval(char *ptr,int *value);
ULONG round(ULONG sign,ULONG l,ULONG g,ULONG r,ULONG s,ULONG rnd);
ULONG roundd(int resexp,ULONG *resign,ULONG *resmanthi,ULONG *resmantlo,
ULONG g,ULONG r,ULONG s,ULONG rnd);
ULONG rounds(int *resexp,ULONG *resign,ULONG *resmant,
ULONG g,ULONG r,ULONG s,ULONG rnd);
void setargs(char *ptr,int execaddr);
int runsilent(int argc,char **argv,int curargc);
void loadcore(void);
void cp_ptrs(char **p1, char **p2,int count);
void sig_handler(int signo);
void sig_set(void);
void catch_fpe(void);
int getarg(int n);
char *copystr(int argc,char *ptr);
int stdio_enable(void);
int sim_printf(FILE *fp, char *ptr, int regno);
void makesim(unsigned int instr, struct IR_FIELDS *opcode);
int test_issue(struct IR_FIELDS *ir, struct SIM_FLAGS *f);
void do_issue(void);
void killtime (unsigned int time_to_kill);
int readtime (void);
int check_scoreboard (struct IR_FIELDS *ir, struct SIM_FLAGS *f);
void statreset (void);
void printstats(void);
void Statistics (struct IR_FIELDS *ir);
char *symbol(int sect,unsigned int value);
char *findsym(char *ptr,int *value);
int symcreate(struct syment *symptr,int nsyms,struct string_table *flexname,
struct scnhdr *shdr);
char *symcopy(char *ptr1, char *ptr2, int cnt);
void symfree(void);
void initsymptrs(int sect);
struct symbols *find_next_symbol(int sect, struct symbols *cptr);
void sysVclose(void);
int sysVupfil(int fildes);
int dosysVstat(char *path, void *buf);
int sysVbcs(int sys_call_index);
int PPrintf(char *,...);
int Eprintf(char *,...);
char * FFgets(char *buf,int size, FILE *iob);
INSTAB *lookupdisasm(UINT key);
void init_disasm(void);
void install(INSTAB *instptr);
int vector(struct IR_FIELDS *ir,int vec);
int exception(int vec, char *message);
void upd_status(unsigned int status);

/* functions not needed for SPEC95 workloads */

#ifdef NOT_SPEC95
int fake_io(int vec);
int stdio_io(int vec);
void expand(char *line1,char *line2);
#endif

/*

	The command functions

*/
int bf(void);
int bm(void);
int br(void);
int nobr(void);
int bs(void);
int cm(void);
int simexit(void);
int cd(void);
int cs(void);
int pd(void);
int bd(void);
int cr(void);
int dc(void);
int go(void);
int gd(void);
int gn(void);
int gt(void);
int tr(void);
int tv(void);
int tx(void);
int tz(void);
int tt(void);
int he(void);
int ids(void);
int id(void);
int dm(void);
int lo(void);
int rlo(void);
int map(void);
int mds(void);
int md(void);
int mm(void);
int rd(void);
int rm(void);
int run(void);
int rrn(void);
int sr(void);
int sd(void);
int show(void);
int pa(void);
int nopa(void);
int cwd(void);
int pwd(void);
int rstsys(void);
int dumpcore(void);
int prtsym(void);
int cacheoff(void);

/* String stuff */
char *strtolower(char *p);
