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
  string bb_info;
  string posdom;
  string ctrleq;

  // file stream
  ofstream fout_posdom;
  ofstream fout_ctrleq;

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
inline void error(string msg){
  cerr << "=== ERROR: " << msg << " ===\n";
  exit(1);
}

// usage
inline void usage(char *arg0){
  cerr << "usage: " << arg0 << " -dir DIR" << endl;
  exit(0);
}

#endif
