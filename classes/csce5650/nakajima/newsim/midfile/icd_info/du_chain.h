// -*- C++ -*-
//
// du_chain.h
//

#ifndef DU_CHAIN_H
#define DU_CHAIN_H

#include <map>
#include "bb.h"

//
// class DU_Chain
//

class DU_Data{
public:
  int pc;
  int bb;
  int reg;

  DU_Data(const int &p = 0, const int b = 0, const int r = 0){
    pc = p;
    bb = b;
    reg = r;
  }
};

class DU_Chain{
public:
  // define
  typedef std::multimap< int, DU_Data > MMAP;
  typedef MMAP::iterator MI;
  typedef std::pair< MI, MI > MI_PAIR;

  // current function number/bb size
  int func;
  int bb_size;
  std::string fname;

  // used-defined chain
  MMAP *use_chain;

  // const/destructor
  DU_Chain(Program_Info &, const int &);
  ~DU_Chain();

  // get du-chain data
  MMAP get_use(const int &bb) { return use_chain[bb]; }
};

#endif
