// -*- C++ -*-
//
// def_use.h
//

#ifndef DEF_USE_H
#define DEF_USE_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <set>
#include <map>
#include <vector>

#include "bb.h"
#include "main.h"

//
// class DU_Data
//
class DU_Data{
public:
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;

  // pc/def id num
  int pc;
  int id;
  // destination, source reg number
  SET use;
  int def;
  // jal, jalr flag
  bool jal;

public:
  // const/destructor
  DU_Data(const int &p, const int &d, SET &use_reg, bool j) {
    pc = p;
    def = d;
    use = use_reg;
    jal = j;
  }
  ~DU_Data(){ use.clear(); }

  // set pc/def id num
  void set_id(const int &i) { id = i; }
  // get
  const int get_pc() { return pc; }
  const int get_id() { return id; }
  const int get_def() { return def; }
  SET get_use() { return use; }
  bool get_jal() { return jal; }
};

//
// class Def_Use
//
class Def_Use{
public:
  // define
  typedef std::bitset< bitset_size > BITSET;
  typedef std::map< int, int > HMAP;
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::vector< DU_Data > VEC;

  // current function number/bb size
  int func;
  int bb_size;
  std::string fname;

  // read data
  VEC du_data;
  // correspondence pc/def id number
  HMAP pc_id_num;

  // identifire
  SET def_id[REG];

  // kill/gen for analysis reach definiton and make du-chain
  BITSET *kill;
  BITSET *gen;

  // def/use for analysis livereg
  BITSET *def;
  BITSET *use;

  // const/destructor
  Def_Use(Program_Info &, const int &);
  ~Def_Use();

  // get correspondence pc/def id number
  DU_Data get_data(const int &id){ return du_data[id]; }
  const int get_id(const int &pc) { return pc_id_num[pc]; }

  // get identifire
  SET get_def_id(const int &reg) { return def_id[reg]; }

  // get kill/gen for analysis reach definiton and make du-chain (reach.cc)
  BITSET get_kill(const int &bb) { return kill[bb]; }
  BITSET get_gen(const int &bb) { return gen[bb]; }
  // make kill/gen for analysis reach definiton and make du-chain
  void make_kill_gen(Program_Info &);

  // get def/use for analysis livereg (live.cc)
  BITSET get_def(const int &bb) { return def[bb]; }
  BITSET get_use(const int &bb) { return use[bb]; }
  // make get def/use for analysis livereg
  void make_def_use(Program_Info &);

private:
  // insert readdata and correspondence pc/def id number
  void insert_data(DU_Data &);
  // test code
  void print();
};

#endif
