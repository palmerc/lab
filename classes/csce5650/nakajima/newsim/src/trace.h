// -*- C++ -*-
//
// trace.h
//
//  Time-stamp: <03/12/05 02:14:55 nakajima>
//

#ifndef TRACE_H
#define TRACE_H

// FD
const int sim_fd = 18;

// function type
enum Funit_Type{
  NOOP,
  BRANCH,
  LOAD,
  STORE,
  OTHER
};

// branch type
enum Branch_Type{
  CR_MISSMATCH,
  JALL_CALL,   // Function Call branch
  JR_RETURN,   // Function Return branch
  UNCON_DIRE,  // uncondition direct
  UNCON_INDIRE,// uncondition indirect
  CON_DIRE     // Condition Direct
};

// pipe instruction data from SimpleScalar
struct Pipe_Inst{
  // function type
  Funit_Type func;
  // source/destination reg number
  int s_A;
  int s_B;
  int s_C;
  int dest;
  // pc (word)
  int pc;
  // branch target address or load/store address
  int address;
  // branch type
  Branch_Type brn;
  // mem size
  int m_size;
  // source/destination reg value
  int v_A;
  int v_B;
  int v_C;
  int v_dest;
  // double reg flag
  char double_reg;
};

void child_process(char **);

#endif
