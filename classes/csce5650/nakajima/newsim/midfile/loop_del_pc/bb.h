// -*- C++ -*-
//
// bb.h
//

#ifndef BB_H
#define BB_H

#include <bitset>
#include <map>
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

  // const/destructor
  Bb_Info() { start = end = 0; }
};

class Program_Info{
public:
  // define
  typedef std::multimap< int, int > MMAP;
  typedef MMAP::iterator MMI;
  typedef std::pair< MMI, MMI > MMI_PAIR;
  typedef std::set< int > SET;
  typedef SET::iterator SI;

  // function size
  int func_size;
  // bb size
  int *bb_size;
  // function name
  std::string *fname;

  // succeesor bb
  SET **succ;
  // predecessor bb
  SET **pred;
  // bb start/end pc and br/in bb num set
  Bb_Info **info;

  // const/destructor
  Program_Info();
  ~Program_Info();

  // function num, bb num
  const int size() { return func_size; }
  const int size(const int &func) { return bb_size[func]; }
  std::string funcname(const int &func) { return fname[func]; }

  // get start/end pc
  Bb_Info get_info(const Func_Bb &fbb) { return info[fbb.func][fbb.bb]; }

  // search bb
  const int search_bb(const int &, const int &);

  // get succeesor bb
  SET get_succ(const Func_Bb &fbb) { return succ[fbb.func][fbb.bb]; }
  // get predecessor bb
  SET get_pred(const Func_Bb &fbb) { return pred[fbb.func][fbb.bb]; }

  // search backward edge
  MMAP backward_edge(const int &);

private:
  // check code
  void print();
};

#endif
