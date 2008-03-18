// -*- C++ -*-
//
// loop.h
//
//  Time-stamp: <04/01/26 15:52:53 nakajima>
//

#ifndef LOOP_H
#define LOOP_H

#include <map>
#include <set>
#include "bb.h"
#include "trace.h"

//
// class Loop
//

enum Skip{
  NOT_FOUND,
  CONST_PC,
  INDUCT_PC,
  CONST_CAND,
  INDUCT_CAND,
  EXIT_BRANCH
};


class Loop{
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::map< int, int > MAP;
  typedef MAP::iterator MI;

  // function size
  int func_size;

  // loop head bb
  // a[func].{ set of header BBs }
  SET *header_bb;

  // a[func][pc].loop header bb
  // loop exit branch pc, loop header bb
  MAP *exit_pcs;
  // loop constant variable pc, loop header bb
  MAP *const_pcs;
  // loop induction variable pc, loop header bb
  MAP *induct_pcs;

  // counter
  int induct_c;// loop induction variable
  int const_c; // loop constant variable
  int unroll_c;// loop unroll branch

public:
  // const/destructor
  Loop();
  ~Loop();

  // file read
  void file_read();

  // ignore/skip pc check
  Skip flag_check(const Pipe_Inst &);

  const int ignore_inst();

  // loop header
  bool header(const Func_Bb& fbb){
    return( header_bb[fbb.func].count(fbb.bb) );
  }

  // counter
  void count_unroll();
  void count_loop_inst(const Skip &);

  // result
  void print_result();

private:
  // ループの平均繰り返し回数
  class Loop_Count{
    int bedge_c; // loop backedge count
    int header_c;// loop header count

  public:
    // constructor
    Loop_Count() { bedge_c = header_c = 0; }

    // counting
    void bedge() { bedge_c ++; }
    void header() { header_c ++; }

    // 0÷0 チェック
    bool check(){ return( header_c ); }
    // 各ループの平均繰り返し回数
    double calc_ave(){
      // ループ回数 / ループに入った回数
      return( (double)( bedge_c + header_c ) / (double)header_c );
    }
    void print(){
      std::cout << "bedge, header, ave: " << bedge_c << ", " << header_c
  	   << ", " << calc_ave() << std::endl;
    }
    int exec() { return( header_c ); }
    int loop() { return( bedge_c + header_c ); }
  };

  // ループ実行回数と、headerに後方分岐した回数
  Loop_Count **loop_c;

  void allocate_loop_c();
  void print_count_loop();

public:
  // ループの平均繰り返し回数
  void count_bedge(const Func_Bb &);
  void count_header(const Func_Bb &);

private:
  // check code
  void print();
};


//
// extern
//

extern Loop lp;

#endif
