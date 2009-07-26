// -*- C++ -*-
//
// reach.h
//


#ifndef REACH_H
#define REACH_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
#include <string>

#include "bb.h"
#include "def_use.h"
#include "main.h"

//
// class Chain
//
class Chain{
  // use pc
  int pc;
  // def/use reg
  int reg;

public:
  // constructor
  Chain(const int &p, const int &r){
    pc = p;
    reg = r;
  }

  // get
  const int get_pc() { return pc; }
  const int get_reg() { return reg; }
};


//
// class Reach_Def
//
class Reach_Def{
  // define
  typedef bitset< bitset_size > BITSET;
  typedef multimap< int, Chain > MMAP;
  typedef MMAP::iterator MI;
  typedef pair< MI, MI > MI_PAIR;

  // current function number/bb size
  int func;
  int bb_size;
  string fname;

  // in/out
  BITSET *in;
  BITSET *out;

  // du chain
  MMAP du_chain;

public:
  // const/destructor
  Reach_Def(Program_Info &, const int &);// make kill/gen
  ~Reach_Def();

  // data_flow equations
  void calc_dataflow_eq(Program_Info &, Def_Use &);
  // make du-chain
  void make_du_chain(Program_Info &, Def_Use &);
  // file out
  void print(ofstream &);
};

#endif
