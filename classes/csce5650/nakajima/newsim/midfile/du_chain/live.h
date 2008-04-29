// -*- C++ -*-
//
// live.h
//


#ifndef LIVE_H
#define LIVE_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>

#include "bb.h"
#include "def_use.h"
#include "main.h"

// define live/dead
const bool LIVE = false;
const bool DEAD = true;

//
// class Live_Reg
//
class Live_Reg{
  // define
  typedef bitset< bitset_size > BITSET;

  // current function number/bb size
  int func;
  int bb_size;
  string fname;

  // in/out
  BITSET *in;
  BITSET *out;

public:
  // const/destructor
  Live_Reg(Program_Info &, const int &);
  ~Live_Reg();

  //analysisi live reg
  void analysis(Program_Info &, Def_Use &);
  void print(ofstream &);
};

#endif
