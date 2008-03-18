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

// define reg number
const int rv0 = 2;// 2,3 is return variable
const int ra0 = 4;// 4,5,6,7 is argments

// byte size
const int byte = 8;

// bitset size (definition instruction num in function)
const unsigned int bitset_size = 0x02800;

//
// class Sim_Model simulation model
//
class Sim_Model{
public:
  // filename (full path)
  std::string asm_du;
  std::string bb_info;
  std::string du_chain;// output filename

  // file stream
  std::ifstream fin_asm_du;
  std::ofstream fout_du_chain;

public:
  // file_open
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
