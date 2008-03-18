// -*- C++ -*-
//
// icd.h
//

#ifndef ICD_H
#define ICD_H

#include <set>
#include <stack>
#include <string>
#include "bb.h"
#include "du_chain.h"

//
// class ICD indirect control dependence
//
class ICD{
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::stack< int > STACK;

  // current function number/bb size
  int func;
  int bb_size;
  std::string fname;

  // ある識別子の定義が確定する基本ブロックの集合
  SET **icd;

public:
  // const/destructor
  ICD(Program_Info &, const int &);
  ~ICD();

  // icd analysis
  void analysis(Program_Info &, DU_Chain &);

  // file out
  void print(std::ofstream &);

private:
  // 関数の始点から定義までに通過する基本ブロックの集合を求める
  SET pass_bb(Program_Info &, SET &);

  // stack argolism subroutine
  void sub_insert(SET &, STACK &, const int &);

  // 確定点をもとめる
  SET defined_bb(Program_Info &, SET &);
};

#endif
