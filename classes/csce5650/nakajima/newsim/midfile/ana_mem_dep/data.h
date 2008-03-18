// -*- C++ -*-
//
// data.h (mem profile)
//

#ifndef DATA_H
#define DATA_H

#include <fstream>
#include <map>
#include <vector>

//
// class Mem_Profile
//

class Mem_Profile{
  // define
  typedef std::vector< double > VEC;
  typedef std::map< int, VEC > MAP;
  typedef MAP::iterator MI;

  static const int size = 101;

  MAP profile;
  VEC total_freq;

  int total_load;

public:
  // const/destructor
  Mem_Profile();
  ~Mem_Profile();

  void file_read();

private:
  void print();
  void calc_total_load();
};

#endif
