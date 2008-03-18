// -*- C++ -*-
//
// main.h
//

#ifndef MAIN_H
#define MAIN_H

#include <string>
using namespace std;

//
// class GnuPlot
//

class GnuPlot{
public:
  void print_header(){;}
  void print_command(){;}
};

//
// class Sim_Model simulation model
//

// simulation model
class Sim_Model{
  string argv0;

public:
  // filename
  string mp_analysis;

public:
  // argment check
  void arg_check(const int &, char **);

private:
  const int check_atoi(const string);
  void usage();
};


//
// extern
//

extern Sim_Model model;




// error
inline void error(const string &msg){
  cerr << "#### ERROR: " << msg << " ####" << endl;
  exit(1);
}

// warning
inline void warning(const string &msg){
  cerr << "#### Warning: " << msg << " ####" << endl;
}

#endif
