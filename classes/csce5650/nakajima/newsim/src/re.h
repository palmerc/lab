// -*- C++ -*-
//
//  re.h reexec mode
//
//  Time-stamp: <04/02/04 16:11:12 nakajima>
//

#ifndef RE_H
#define RE_H

using namespace std;

#include <list>
#include "trace.h"

//
// class Re_Exec (reexec mode, pipe instruction spool)
//

// 実行の状態遷移
enum Re_Mode{
  Normal, // 通常実行
  No,     // 仮実行: forkせずにss実行の状態、仮実行命令数(次のfork点まで)の計算
  Change, // No -> Forkに状態遷移させる準備。仮実行forkしたら、Forkに遷移
  Fork,   // 仮実行: fork後のss実行の状態
  Finish  // 仮実行モード終了、forkするしないを決定し、Normalに遷移
};

class Re_Exec{
  // define
  typedef list< Pipe_Inst > LIST;
  typedef LIST::iterator LI;

  // reexec mode instruction spool
  LIST spool;
  LI spool_i;

  // reexec mode
  Re_Mode re;// for reexec mode

  // check state
  bool re_state;// "Normal" or other mode
  bool ch_state;// "Change" or other mode

  // trace counter
  int trace_count;
  int re_trace_count;// for reexec mode
  int spool_size;// 実際に仮実行する命令

  // reexec exec_time
  int no_max_exec;  // for reexec mode
  int fork_max_exec;// for reexec mode

  // for Ex_EQ
  int fork_exec_time;

  // counter
  int re_count;
  int gain_fork;
  int no_gain_no_fork;
  int no_gain_fork;
  int eq_no_fork;
  int eq_fork;

public:
  // const/destructor
  Re_Exec();
  ~Re_Exec();

  // mode check and fetch new instructon
  const bool check_mode(Pipe_Inst &);
  // change Normal -> No
  void change_no();
  // check fork_child_process
  const bool check_fork_process(const int&);

private:
  // change No -> Change
  void change();
  // change Change -> Fork
  void change_fork();
  // change Finish -> Normal
  void change_normal();

public:
  // spool instructons
  void init_inst_spool();
  // no reexec mode && lp mode trace skip
  void init_trace_skip();
  // delete current instruction, push next instructions
  void pop_and_push_inst();

public:
  // calc gain
  void gain(const int &);
  // current reexec state
  const Re_Mode mode() { return re; }
  // current trace count
  const int tc();

  // print result
  const int get_re_count() { return re_count; }
  void print_result();

private:
  // backup/restore data
  void backup_data();
  void restore_data();

private:
  // debug
  void state();
};


//
// extern
//

extern Re_Exec reexec;

#endif
