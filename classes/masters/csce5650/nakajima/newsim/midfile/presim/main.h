// -*- C++ -*-
//
// main.h
//

#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>


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
// simulation model
//

// simulation model
class Sim_Model{
  string argv0;

public:
  // fin filename
  string bb_info;
  // fout filename
  string brn_pred;
  string val_pred;
  string mem_region;

  // value
  int trace_limit;
  int print_freq;
  int print_freq_def;
  int fastfwd_tc;

public:
  // constructor
  Sim_Model();

  // argment check
  void arg_check(const int &, char **);

private:
  void usage();
  const int check_atoi(const string&);
};

//
// extern
//

extern Sim_Model model;

//
// error print function
//

// error
inline void error(string msg){
  cerr << "=== ERROR: " << msg << " ===\n";
  exit(1);
}

#endif
