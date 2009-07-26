// -*- C++ -*-
//
// bb.h
//

#ifndef BB_H
#define BB_H

#include <set>
#include <string>
#include "main.h"

//
// class Func_Bb
//

// search func, bb
class Func_Bb{
public:
  int func;
  int bb;

public:
  // constructor
  Func_Bb(const int &f = 0, const int &b = 0) { func = f; bb = b; }
};


//
// class Progranm_Info (function and bb number, bb info)
//

// bb start/end pc and br/in bb num set
class Bb_Info{
public:
  // bb start/end pc
  int start;
  int end;

public:
  // const/destructor
  Bb_Info() { start = end = 0; }
};

class Program_Info{
  // define
  typedef set< int > SET;
  typedef SET::iterator SI;

  // function size
  int func_size;
  // bb size
  int *bb_size;
  // function name
  string *fname;

  // succeesor bb
  SET **succ;
  // predecessor bb
  SET **pred;
  // bb start/end pc
  Bb_Info **info;

public:
  // const/destructor
  Program_Info();
  ~Program_Info();

  // function num, bb num
  const int size() { return func_size; }
  const int size(const int &func) { return bb_size[func]; }
  string funcname(const int &func) { return fname[func]; }

  // get start/end pc
  Bb_Info get_info(const Func_Bb &fbb) { return info[fbb.func][fbb.bb]; }

  // search bb
  const int search_bb(const int &, const int &);

  // get succeesor bb
  SET get_succ(const Func_Bb &fbb) { return succ[fbb.func][fbb.bb]; }
  // get predecessor bb
  SET get_pred(const Func_Bb &fbb) { return pred[fbb.func][fbb.bb]; }

private:
  // check code
  void print();
};

#endif
