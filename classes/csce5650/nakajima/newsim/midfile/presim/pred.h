// -*- C++ -*-
//
// pred.h
//

#ifndef PRED_H
#define PRED_H

#include <fstream>
#include <map>
#include "bb.h"
#include "main.h"
#include "trace.h"

//
// class VP_Data (value prediction)
//

class Value_Predict{
  class Value_Data{
    // define
    typedef std::map< int, int > MAP;
    typedef MAP::iterator MI;

    // counter
    MAP val_data;
    int total;

  public:
    // const/destructor
    Value_Data() { total = 0; }
    ~Value_Data() { val_data.clear(); }

    void add_data(const int &);
    void file_write(std::ofstream &fout);
  };

  // define
  typedef std::map< int, Value_Data > MAP;
  typedef MAP::iterator MI;

  // total function size
  int func_size;

  // value data table
  MAP *val_table;

public:
  // const/destructor
  Value_Predict(Program_Info &);
  ~Value_Predict();

  // count instruction
  void count_inst(const Pipe_Inst &, Program_Info &);
  // file out
  void file_write(Program_Info &);
};

//
// class Branch_Predict (static branch prediction)
//

class Branch_Predict{
  class Branch_Inst{
  public:
    int total_count;
    int next_count;
    int target_pc;

  public:
    // constructor
    Branch_Inst() { total_count = next_count = target_pc = 0; }
  };

  // define
  typedef std::map< int, Branch_Inst > MAP;
  typedef MAP::iterator MI;
  static const int byte = 8;

  // total function size
  int func_size;

  // static branch prediction counters
  MAP *branch_table;

public:
  // const/destructor
  Branch_Predict(Program_Info &);
  ~Branch_Predict();

  // count instruction
  void count_inst(const Pipe_Inst &, Program_Info &);
  // file out
  void file_write(Program_Info &);
};

#endif
