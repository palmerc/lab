// -*- C++ -*-
//
// inst.h
//

#ifndef INST_H
#define INST_H

#include <fstream>
#include <map>
#include "bb.h"

//
// class Inst_Type
//

enum Operand{
  Add    = 0x00,
  Sub    = 0x01,
  Mult   = 0x02,
  Div    = 0x04,
  Call   = 0x10,
  Jump   = 0x11,
  Branch = 0x12,
  Ctrl   = 0x10,
  Other  = 0x20
};

class Inst_Data{
public:
  Operand op;
  int dest;
  int src1;
  int src2;

  // const/destructor
  Inst_Data(Operand o = Other, const int &d = -1,
	    const int &s1 = -1, const int &s2 = -1){ 
    op = o;
    dest = d;
    src1 = s1;
    src2 = s2;
  }
};

class Inst_Type{
public:
  // define
  typedef std::map< int, Inst_Data > MAP;
  typedef MAP::iterator MI;
  typedef std::pair< MI, MI > MI_PAIR;

  // add. sub, mult, div
  MAP insts;

  // const/destructor
  Inst_Type(Program_Info &, const int &);
  ~Inst_Type();

  // find instruction operand
  Inst_Data operand(const int &);

private:
  // check code
  void print();
};

#endif
