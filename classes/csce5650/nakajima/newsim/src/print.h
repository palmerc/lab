// -*- C++ -*-
//
// print.h
//
//  Time-stamp: <03/12/05 02:14:12 nakajima>
//

#ifndef PRINT_H
#define PRINT_H

using namespace std;

#include <string>
#include "trace.h"

void print_result_data();
void print_progress_report();
void print_debug_header(const Pipe_Inst &);

// error
inline void error(const string &msg){
  cerr << "#### ERROR: " << msg << " ####" << endl;
  print_result_data();
  exit(1);
}

// warning
inline void warning(const string &msg){
  cerr << "#### Warning: " << msg << " ####" << endl;
}

#endif
