// -*- C++ -*-
//
// bb.h
//

#ifndef BB_H
#define BB_H

#include <string>

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
  // bb start/end pc and br/in bb num set
  Bb_Info **info;

  // function size
  int func_size;
  // bb size
  int *bb_size;
  // function name
  string *fname;

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

  // search fbb
  const int search_func(const int &);
  const int search_bb(const int &, const int &);

private:
  // check code
  void print();
};

#endif
