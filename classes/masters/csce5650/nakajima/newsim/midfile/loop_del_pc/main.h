// -*- C++ -*-
//
// main.h
//

#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>

//
// const
//

// reg size
const int REG = 67;

// byte size
const int byte = 8;

// Architecture register definition
const int HI_REG = 64;

//
// class Sim_Model simulation model
//
class Sim_Model{
public:
  // filename (full path)
  string asm_loop;
  string bb_info;
  string du_chain;
  string posdom;
  string loop_del_pcs;// output filename

  // file stream
  ifstream fin_du_chain;
  ifstream fin_asm_loop;
  ifstream fin_posdom;
  ofstream fout_loop;

public:
  // file open
  void file_open();
};


//
// extern (simulation model)
//

extern Sim_Model model;


//
// error, usage print function
//

// error
inline void error(string msg){
  cerr << "=== ERROR: " << msg << " ===\n";
  exit(1);
}

// usage
inline void usage(char *arg0){
  cerr << "usage: " << arg0 << " -dir DIR" << endl;
  exit(2);
}

#endif
