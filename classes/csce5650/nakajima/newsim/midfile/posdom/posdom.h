// -*- C++ -*-
//
// posdom.h
//

#ifndef POSDOM_H
#define POSDOM_H

#include <bitset>
#include <string>
#include "bb.h"
#include "main.h"

//
// class PosDom post diminate or ctrl eq info
//

class PosDom{
  // define
  typedef bitset< bitset_size > BITSET;
  // post dominate flag (PD)
  BITSET *pos;
  // dominate flag (CE)
  BITSET *dom;

  // current function number/bb size
  int func;
  int bb_size;
  string fname;

public:
  // const/destructor
  PosDom(Program_Info &, const int &);
  ~PosDom();

  // post dominate (PD)
  void post_dominate(Program_Info &);
  // dominate (CE)
  void dominate(Program_Info &);
  // ctrl equivalence (CE)
  void equivalence();

  // output (PD/CE)
  void print(ofstream &);
  const int calc_bb_num();
};

#endif
