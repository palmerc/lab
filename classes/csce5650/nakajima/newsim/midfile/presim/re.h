// -*- C++ -*-
//
//  re.h reexec mode
//

#ifndef RE_H
#define RE_H

#include "trace.h"

//
// class Re_Exec (pipe instruction spool)
//

class Re_Exec{
  // trace count
  int trace_count;

public:
  // constructor
  Re_Exec() { trace_count = 0; }

  // check next instruction
  const bool check(Pipe_Inst &);
  // init trace skip
  void init_trace_skip();
  // current trace counter
  const int tc() { return trace_count; }
};

#endif
