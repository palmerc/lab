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
  string bb_info;
  string du_chain;
  string icd_info;// output filename

  // file stream
  ifstream fin_du_chain;
  ofstream fout_icd;

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
