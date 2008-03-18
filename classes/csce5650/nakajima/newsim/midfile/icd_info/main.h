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

// byte
const int byte = 8;

//
// class Sim_Model simulation model
//
class Sim_Model{
public:
  // filename (full path)
  std::string bb_info;
  std::string du_chain;
  std::string icd_info;// output filename

  // file stream
  std::ifstream fin_du_chain;
  std::ofstream fout_icd;

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
