// -*- C++ -*-
//
// bb.h
//
//  Time-stamp: <04/01/26 12:04:13 nakajima>
//

#ifndef BB_H
#define BB_H

#include <bitset>
#include <string>

//
// class Func_Bb
//

// current func, bb
class Func_Bb{
public:
  int func;
  int bb;

public:
  // constractior
  Func_Bb( const int &f = 0, const int &b = 0 ) { func = f; bb = b; }
};

class Bb_Statistic{
  // function size
  int func_size;

  // それぞれの出現回数
  int *func;
  int **bb;
  int **inst;

public:
  // const/destructor
  Bb_Statistic();
  ~Bb_Statistic();

  void allocate();
  void print();

  void count_func(const Func_Bb &fbb){ func[fbb.func] ++; count_bb(fbb); }
  void count_bb(const Func_Bb &fbb) { bb[fbb.func][fbb.bb] ++; }
  void count_inst(const Func_Bb &fbb) { inst[fbb.func][fbb.bb] ++; }

  void uncount_inst(const Func_Bb &fbb) { inst[fbb.func][fbb.bb] --; }
};

//
// class Progranm_Info (function and bb number, bb info)
//

class Program_Info{
  // bb start/end pc
  class Bb_Info{
  public:
    int start;
    int end;

  public:
    // constractior
    Bb_Info() { start = end = 0; }
  };

  // function size
  int func_size;
  // bb size
  int *bb_size;
  // function name
  std::string *fname;

  // bb start/end pc
  Bb_Info **info;

  // current fbb, last fbb
  Func_Bb fbb_last;
  Func_Bb fbb_now;

  // backup for reexec mode
  Func_Bb fbb_last_bak;

public:
  // const/destructor
  Program_Info();
  ~Program_Info();

  // read bb_info
  void file_read();

  // change fbb check and search fbb_now
  const bool change_fbb(const int &, const int &);

  // function num, bb num
  const int size() { return func_size; }
  const int size(const int &func) { return bb_size[func]; }
  const std::string funcname(const int &func) { return fname[func]; }

  // read fbb data
  const Func_Bb & fbb() { return fbb_now; }
  const Func_Bb & last_fbb() { return fbb_last; }

  // for reexec mode
  void backup();
  void restore();

private:
  // search fbb
  const int search_func(const int &);
  const int search_bb(const int &, const int &);

private:
  // check code
  void print();
};


//
// class PosDom post diminate or ctrl eq info
//

// read post dominate or ctrl eq info
class PosDom{
  // posdom flag bitset  size
  static const int bitset_size = 0x0D00;// 104 * 32bit
  // define
  typedef std::bitset< bitset_size > BITSET;

  // function size
  int func_size;

  // post dominate flag
  BITSET **pd;

public:
  // const/destructor
  PosDom();
  ~PosDom();

  // file read
  void file_read();

  // read post dominate or ctrl eq
  const bool flag(const Func_Bb &fbb, const int &bb){
    return( pd[fbb.func][fbb.bb][bb] );
  }

private:
  // check code
  void print();
};

//
// extern
//

extern Program_Info program;
extern PosDom pd;
extern Bb_Statistic bb_statistic;

#endif
