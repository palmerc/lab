// -*- C++ -*-
//
// main.h
//

#ifndef MAIN_H
#define MAIN_H

#include <string>

//
// const
//

// posdom flag bitset  size
const int bitset_size = 0x0D00;// 3328bit -> 104 * 32bit


//
// class Model
//
struct Sim_Model{
  // filename (full path)
  std::string bb_info;
  std::string posdom;
  std::string ctrleq;

  // file stream
  std::ofstream fout_posdom;
  std::ofstream fout_ctrleq;

public:
  // file_open
  void file_open();
};

//
// extern
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
  exit(0);
}

#endif
