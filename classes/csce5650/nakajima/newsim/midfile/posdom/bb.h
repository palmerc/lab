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
// class Progranm_Info (bb info)
//

class Program_Info{
public:
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;

  // succeesor bb
  SET **succ;
  // predecessor bb
  SET **pred;

  // function size
  int func_size;
  // bb size
  int *bb_size;
  // function name
  std::string *fname;

  // const/destructor
  Program_Info();
  ~Program_Info();

  // function num, bb num
  const int size() { return func_size; }
  const int size(const int &func) { return bb_size[func]; }
  std::string funcname(const int &func) { return fname[func]; }

  // get succeesor bb
  SET get_succ(const Func_Bb &fbb) { return succ[fbb.func][fbb.bb]; }
  // get predecessor bb
  SET get_pred(const Func_Bb &fbb) { return pred[fbb.func][fbb.bb]; }

private:
  void print();
};

#endif
