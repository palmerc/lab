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
  std::string asm_loop;
  std::string bb_info;
  std::string du_chain;
  std::string posdom;
  std::string loop_del_pcs;// output filename

  // file stream
  std::ifstream fin_du_chain;
  std::ifstream fin_asm_loop;
  std::ifstream fin_posdom;
  std::ofstream fout_loop;

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
inline void error(std::string msg){
  std::cerr << "=== ERROR: " << msg << " ===\n";
  exit(1);
}

// usage
inline void usage(char *arg0){
  std::cerr << "usage: " << arg0 << " -dir DIR" << std::endl;
  exit(2);
}

#endif
