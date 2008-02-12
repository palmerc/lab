// -*- C++ -*-
//
// main.h
//
//  Time-stamp: <04/02/06 19:52:00 nakajima>
//

#ifndef MAIN_H
#define MAIN_H

using namespace std;

#include "trace.h"

//
// const
//

// define reg number
const int REG = 67;// reg size
const int ZERO_REG = 0;
const int RET_REG = 2;// 2,3 return register
const int ARG_REG = 4;// 4,5,6,7 argment register
const int GP_REG = 28;// global pointer
const int SP_REG = 29;// stack pointer
const int FP_REG = 30;// frame pointer
const int RA_REG = 31;// return address
const int HI_REG = 64;// HI/LO reg
const int FCC_REG = 66;// fcc

// SimpleScalar Architecture register definition
// Hard    Soft    
// 0       zero    zero
// 1       at      reserved for assembler
// 2-3     v0-v1   function result , evaluation
// 4-7     a0-a3   argument
// 8-15    t0-t7   temporary
// 16-23   s0-s7   static
// 24-25   t8-t9   temporary
// 26-27   k0-k1   reserved for kernel
// 28      gp      global pointer
// 29      sp      stack pointer
// 30      s8      frame pointer
// 31      ra      return address
// 32-63   f0-f31  fp register
// 64      hi      ?
// 65      lo      ?
// 66      fcc     ?

//
// class Reg_Data and vector reg
//
enum Reg_Flag{
  UNDEF_UNUSE = 0x00,// undefined and unused in this thread
  DEFINE      = 0x01,// already defined in this thread
  USE         = 0x02,// undefined but already used in this thread
  DEF_USE     = 0x03,
  // last defined by induction or loop invariant variable
  // But Potentially undefined this thread
  LAST_DEF_I  = 0x08,
  DEF_I       = 0x09,// defined induc. or loop inva. vari. in this thread
  PREDICT     = 0x10
};

// register set
class Reg_Data{
  // for reexec mode
  Reg_Flag flag_bak;
  int write_bak;
  int read_bak;
  int commit_bak;
  int def_tc_bak;

public:
  // orig data
  Reg_Flag flag;// reg state
  int write;    // reg write time
  int read;     // reg read time
  int commit;   // reg commit time
  int def_tc;   // reg define tc

public:
  // constructor
  Reg_Data(const Reg_Flag f = DEFINE){
    flag = f;
    write = read = commit = def_tc = 0;
  }

  // for reexec mode
  void backup(){
    flag_bak = flag;
    write_bak = write;
    read_bak = read;
    commit_bak = commit;
    def_tc_bak = def_tc;
  }
  void restore(){
    flag = flag_bak;
    write = write_bak;
    read = read_bak;
    commit = commit_bak;
    def_tc = def_tc_bak;
  }
};

//
// class counter
//
class Counter{
  // instruction
  int inst_nop;
  int inst_procedure;   // call/return
  int inst_con_branch;  // conditional branch
  int inst_other_branch;// unconditional branch, call/return
  int inst_load;
  int inst_store;
  int inst_other;

  // Model func_inline
  int inline_call_c;  // func inline jall call
  int inline_return_c;// func inline jr reutrn
  int inline_other_c; // func inline other (addi)

  // perfect memory disamiguation
  int sp_load_c; // sp load
  int sp_store_c;// sp store
  int gp_load_c; // gp load
  int gp_store_c;// gp store

  // last fork trace count
  int last_fork_tc;

  // branch trace count, last branch type
  int branch_tc;
  Branch_Type brn_type;

  // reexec mode
  int last_fork_tc_bak;
  int branch_tc_bak;
  int thread_bak;
  Branch_Type brn_type_bak;

public:
  // thread count
  int thread;

public:
  // constructor
  Counter();

  // if function change (for class Program_Info)
  void set_branch(const int &, const Branch_Type &);
  const bool last_call_tc(const int &);
  const bool last_j_call_tc(const int &);
  const bool last_return_tc(const int &);
  const bool last_branch_tc(const int &);
  const bool last_con_tc(const int &);

  // defined last thread
  void set_last_fork_tc(const int &);
  const bool last_thread_defined(const int &);

  // instance
  const int instance();
  void print_result();

  // instruction
  void count_inst(const Pipe_Inst &);
  // func inline
  void count_inline(const Pipe_Inst &);

  // perfect dismabiguate
  void count_sp(const Funit_Type &);
  void count_gp(const Funit_Type &);

  const int load() { return inst_load; }

  // for reexec mode
  void backup(){
    last_fork_tc_bak = last_fork_tc;
    branch_tc_bak = branch_tc;
    brn_type_bak = brn_type;
    thread_bak = thread;
  }
  void restore(){
    last_fork_tc = last_fork_tc_bak;
    branch_tc = branch_tc_bak;
    brn_type = brn_type_bak;
    thread = thread_bak;
  }
};

//
// class Time
//
class Time{
  // for reexec mode
  int commit_bak;
  int cd_limit_bak;
  int max_cd_limit_bak;

  int branch_bak;
  int last_branch_bak;

  int exec_max_bak;
  int last_exec_max_bak;

public:
  // commit time
  int commit;
  // control dependence limit time
  int cd_limit;
  // max cd_limit
  int max_cd_limit;

  // (last) branch miss prediction time
  int branch;
  int last_branch;

  // max exec time
  int exec_max;

public:
  // constructor
  Time(){
    commit = cd_limit = max_cd_limit = branch = last_branch = exec_max = 0;
  }

  void set_fork_send_time(int &, int &);

  // for reexec mode
  void backup(){
    commit_bak = commit;
    cd_limit_bak = cd_limit;
    max_cd_limit_bak = max_cd_limit;
    branch_bak = branch;
    last_branch_bak = last_branch;
    exec_max_bak = exec_max;
  }
  void restore(){
    commit = commit_bak;
    cd_limit = cd_limit_bak;
    max_cd_limit = max_cd_limit_bak;
    branch = branch_bak;
    last_branch  = last_branch_bak;
    exec_max = exec_max_bak;
  }
};

// extern
extern Reg_Data reg[REG];
extern Counter c;
extern Time t;

#endif
