// -*- C++ -*-
//
// loop.h
//

#ifndef LOOP_H
#define LOOP_H

#include <fstream>
#include <set>
#include <stack>
#include "bb.h"
#include "du_chain.h"
#include "inst.h"

//
// class Loop
//

class Loop{
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::stack< int > STACK;
  typedef std::map< int, int > MAP;
  typedef MAP::iterator MI;

  // current function number/bb size
  int func;
  int bb_size;
  std::string fname;

  // backward edge
  Program_Info::MMAP bedge;
  Program_Info::MMI bedge_i;
  Program_Info::MMI_PAIR bedge_pair;

  // natural loop
  SET loop_bbs;

  // exit pc
  SET exit_pcs;

  // constant variable
  SET const_pcs;

  // unify substitution
  SET unify_pcs;
  // basic induction variable
  SET basic_induct_pcs;
  // induction variable
  MAP induct_pcs;

public:
  // const/destructor
  Loop(Program_Info &, const int &);
  ~Loop();

  // analsis all loop of a function
  void analysis(std::ofstream &, Program_Info &, DU_Chain &, Inst_Type &);

private:
  // search natural loop
  bool natural_loop(std::ofstream &, Program_Info &);
  // natural loop subroutine
  void sub_insert(SET &, STACK &, const int &);

  //search exit branch pcs
  void search_exit_branch(std::ofstream &, Program_Info &, Inst_Type &);

  // search constant variable
  void const_variable(std::ofstream &, Program_Info &, DU_Chain &, Inst_Type &);

  // search candidate pcs
  void candidate_pcs(Program_Info &, DU_Chain &, Inst_Type &);
  // search basic induction variable
  void basic_induct_variable(std::ofstream&, Program_Info&, DU_Chain&, Inst_Type&);
  // search induction variable
  void induct_variable(std::ofstream &, Program_Info &, DU_Chain&, Inst_Type &);

  // check source register is constant variable
  bool check_const(DU_Chain::MI_PAIR, const int &);
  // check source register is induction variable
  bool basic_induct_pc(DU_Chain::MI_PAIR, const int &);

private:
  // clear
  void clear();
};

#endif
