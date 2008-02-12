// -*- C++ -*-
//
// pred.h
//
//  Time-stamp: <04/02/10 13:28:00 nakajima>
//

#ifndef PRED_H
#define PRED_H

using namespace std;

#include <bitset>
#include <hash_map>
#include <set>
#include "bb.h"
#include "trace.h"

//
// class Value_Predict (static value predictor)
//

// value predict result
enum Value_Result{
  NO_VP,
  HIT_VP,
  MISS_VP
};

class Value_Predict{
  // define
  typedef set< int > SET;
  typedef SET::iterator SI;
  typedef hash_map< int, SET > MAP;
  typedef MAP::iterator MI;

  // function size
  int func_size;

  MAP *val_table;

  // counter
  int total;
  int hit;

public:
  Value_Predict();
  ~Value_Predict();

  const Value_Result predict(const Pipe_Inst &);
  void print_result();
  void file_read();

private:
  void print();
};

//
// class Branch_Pre static branch prediction
//

// branch predict result
enum Branch_Result{
  MISS   = 0x00,
  HIT    = 0x01,
  UNROLL = 0x02
};

class Branch_Predict{
  // define
  typedef hash_map< int, int > MAP;
  typedef MAP::iterator MI;

  // function size
  int func_size;

  // static branch prediction table
  MAP *brn_table;

  // bitset size
  static const int bitset_size = 17;// 2^x
  // define
  typedef bitset< (1 << bitset_size) > BITSET;// 2^bitset_size bit

  int pa_i;// pc
  int pa_j;// pc (pa_i > pa_j)
  int pa_k;// BHT k-bit

  // branch history table, pattern history table
  BITSET *bh_table;// k bit shift reg
  BITSET *ph_table;// 2bc

  // predict result
  Branch_Result sp;

  // counter
  int pred;
  int pred_hit;

  // reexec mode
  BITSET *bh_bak;// k bit shift reg
  BITSET *ph_bak;// 2bc

public:
  // const/destructor
  Branch_Predict();
  ~Branch_Predict();

  // file read
  void file_read();

  // allocate 2lev (BHT, PHT)
  void allocate();

  // static branch prediction
  void predict(const Pipe_Inst &);

  const bool hit() { return(sp & HIT); }

  void set_hit() { sp = HIT; }
  void change_miss();
  void change_unroll() { sp = Branch_Result(sp | UNROLL); }

  // loop_unroll
  void change_pred_hit();

  // for reexec mode
  void backup();
  void restore();

  void print_result();
};


//
// extern
//
extern Value_Predict value_pred;
extern Branch_Predict branch_pred;

#endif
