//
//  re.cc reexec mode
//
//  Time-stamp: <04/03/03 17:39:31 nakajima>
//

#include <unistd.h>
#include <iostream>
#include "re.h"

#include "bb.h"
#include "data.h"
#include "fork.h"
#include "icd.h"
#include "loop.h"
#include "pred.h"
#include "print.h"
#include "sim.h"

Re_Exec reexec;

//
// class Re_Exec
//

Re_Exec::Re_Exec(){
  // 初期状態 Normal
  re = Normal;

  trace_count = re_trace_count = 0;
  spool_size = model.spool_size;
  no_max_exec = fork_max_exec = 0;

  fork_exec_time = 0;

  re_count = 0;
  gain_fork = no_gain_no_fork = no_gain_fork = eq_no_fork = eq_fork = 0;

  // state check
  re_state = ch_state = false;
}

Re_Exec::~Re_Exec(){
  spool.clear();
}

// mode check and new instructon
const bool Re_Exec::check_mode(Pipe_Inst &inst){
  switch( re ){
  case Normal:
    // trace limit, spool check
    if( model.trace_limit <= trace_count ){
      cerr << "# Re_Exec::check_mode() trace lim :" << trace_count << endl;
      return false; // exit main loop
    }else if( spool.empty() ){
      cerr << "# Re_Exec::check_mode() trace end :" << trace_count << endl;
      return false;// exit main loop
    }else{
      // next inst
      inst = spool.front();
      trace_count ++;
    }
    break;

  case No:
    // model.sool_size check
    if( trace_count + model.spool_size - 1 <= re_trace_count ){
      // ---- 測定時間の都合
//        error("Re_Exec::check_mode() enlarge default spool size");

      //
      // ---- 測定時間の都合
      //

      // state check
      if( ch_state ){
	state();
	error("Re_Exec::check_mode() change state loop, STOP");
      }
      ch_state = true;

      // No -> Changeに遷移
      re = Change;

      // first reexec inst
      inst = spool.front();
      // first iterator
      spool_i = spool.begin();
      // init reexec mode (Fork)
      re_trace_count = trace_count;

      // restore data
      restore_data();
      // init func stack
      func_data.re_start();

      //
      // ---- 測定時間の都合
      //
    }else{
      // next reexec inst
      spool_i ++;

      if( spool_i == spool.end() ){
	state();
	error("Re_Exec::check_mode() spool_i == spool.end()");
      }

      inst = *spool_i;
      re_trace_count ++;
    }
    break;

  case Fork:
    if( trace_count + spool_size - 1 <= re_trace_count ){
      // Fork -> Finishに遷移
      re = Finish;

      if( model.debug(1) ){
	cerr << "Re_Exec::check_mode() Fork -> Finish" << endl
	     << "Re_Exec::check_mode() restore data" << endl;
      }

      // restore data
      restore_data();
      // first inst
      inst = spool.front();

      if( model.debug(1) ){
	cerr << "Re_Exec::check_mode() Finish (-> Normal)" << endl;
      }
    }else{
      // next reexec inst
      spool_i ++;

      if( spool_i == spool.end() ){
	state();
	error("Re_Exec::check_mode() spool_i == spool.end()");
      }

      inst = *spool_i;
      re_trace_count ++;
    }
    break;

  case Change:
    // first reexec inst
    inst = spool.front();
    // first iterator
    spool_i = spool.begin();
    // init reexec mode (Fork)
    re_trace_count = trace_count;

    // restore data
    restore_data();
    // init func stack
    func_data.re_start();

    if( model.debug(1) ){
      cerr << "Re_Exec::check_mode() state Change (-> Fork)" << endl;
    }
    break;

  case Finish:
    state();
    error("Re_Exec::check_mode() Finish");

  default:
    state();
    error("Re_Exec::check_mode() default switch");
  }

  return true;
}

// reexec start: reexec mode change: Normal -> No
void Re_Exec::change_no(){
  if( !model.reexec ){
    return;
  }

  if( re != Normal ){
    state();
    error("Re_Exec::change_no() re != Normal");
  }

  // state check
  if( re_state ){
    state();
    error("Re_Exec::change_no() re_state");
  }
  re_state = true;

  // 状態遷移 Normal -> No
  re = No;

  if( model.debug(10) ){
    cerr << "Re_Exec::change_no() init reexec mode" << endl;
  }

  // init reexec mode
  re_trace_count = trace_count;
  spool_i = spool.begin();
  spool_size = model.spool_size;
  no_max_exec = fork_max_exec = 0;

  // init func stack
  func_data.re_start();
  // backup
  backup_data();

  re_count ++;
}

// check fork_child_process
const bool Re_Exec::check_fork_process(const int &fork_exec_time){
  bool fork = true;// fork

  switch( re ){
  case Normal:
    // 通常は、forkしない
    fork = false;// not fork
    break;

  case No:
    // 仮実行の終了チェック
//      if( t.cd_limit > fork_exec_time ){
    if( fork_exec_time != null_data ){
      // 次のfork候補で分割可能な地点までの命令数について仮実行する
      // forkする可能性があるので、今回仮実行する命令数を記録

      // reexec mode change: No -> Change
      change();
    }
    fork = false;// not fork
    break;

  case Change:
    // reexec mode change: Change -> Fork
    change_fork();
    break;

  case Fork:
    // 仮実行中
    fork = false;// not fork
    break;

  case Finish:
    // fork or no fork
    if( no_max_exec > fork_max_exec ){
      if( model.debug(1) ){
	cerr << "Re_Exec::check_fork_process() gain Fork" << endl;
      }

      gain_fork ++;

      // reexec mode change: Finish -> Normal
      change_normal();
    }else if( no_max_exec == fork_max_exec ){
      if( model.debug(1) ){
	cerr << "Re_Exec::check_fork_process() gain EQ" << endl;
      }

      // 仮実行の結果では判別できない	
      if( ( model.sp_exec != SP_None && t.cd_limit > fork_exec_time )
	  || ( model.sp_exec == SP_Fork && t.cd_limit == fork_exec_time )
	  || ( ( model.mp == MP_Analysis && model.mp == MP_Predict )
	       && mem_mp.get_last_limit() > fork_exec_time )
	  || ( model.reg == Reg_Finite
	       && reg_finite.get_finite_limit() > fork_exec_time )
  	  ){
	if( model.debug(1) ){
	  cerr << "  Re_Exec::check_fork_process() Ex_EQ -> Fork" << endl;
	}

	eq_fork ++;

	// reexec mode change: Finish -> Normal
	change_normal();
      }else{
	if( model.debug(1) ){
	  cerr << "  Re_Exec::check_fork_process() Ex_EQ -> NO" << endl;
	}

	eq_no_fork ++;

	// reexec mode change: Finish -> Normal
	change_normal();
	fork = false;// not fork
      }
    }else{
      if( model.debug(1) ){
	cerr << "Re_Exec::check_fork_process() gain NO" << endl;
      }

      no_gain_no_fork ++;

      // reexec mode change: Finish -> Normal
      change_normal();
      fork = false;// not fork
    }
    break;

  default:
    state();
    error("Re_Exec::check_fork_process() re default");
  }

  // return to Func_Stack::fork_child_process();
  // exec fork or not fork
  return fork;
}

// reexec mode change: No -> Change
void Re_Exec::change(){
  if( re != No ){
    state();
    error("Re_Exec::change() re != No");
  }

  if( re_trace_count == trace_count ){
    // 状態NOで最低1命令は実行しなければならない。
    // 0命令しか実行していないため、状態遷移してはいけない。
    return;
  }

  // state check
  if( ch_state ){
    state();
    error("Re_Exec::change() change state loop");
  }
  ch_state = true;

  // No -> Forkに状態遷移させる準備
  re = Change;

  // set spool_size
  spool_size = re_trace_count - trace_count;

  if( model.debug(10) ){
    cerr << "Re_Exec::change() " << spool_size
	 << " mode: No -> Change" << endl;
  }
}

// reexec mode change: Change -> Fork
void Re_Exec::change_fork(){
  if( re != Change ){
    state();
    error("Re_Exec::change_fork() re != Change");
  }

  // state check
  if( !ch_state ){
    state();
    error("Re_Exec::change_fork() change state loop");
  }
  ch_state = false;

  re = Fork;

  if( model.debug(10) ){
    cerr << "Re_Exec::change_fork() mode Change -> Fork" << endl;
  }
}

// reexec mode change: Finish -> Normal
void Re_Exec::change_normal(){
  if( re != Finish ){
    state();
    error("Re_Exec::change_normal() re != Finish");
  }

  // state check
  if( !re_state ){
    state();
    error("Re_Exec::change_normal() re_state");
  }
  re_state = false;

  re = Normal;

  if( model.debug(10) ){
    cerr << "Re_Exec::change_normal() mode Finish -> Normal" << endl;
  }
}

void Re_Exec::gain(const int &time){
  switch( re ){
  case No:
    no_max_exec = max(no_max_exec, time);
    break;

  case Fork:
    fork_max_exec = max(fork_max_exec, time);
    break;

  default:
    break;
  }
}

// backup data
void Re_Exec::backup_data(){
  program.backup();
  icd.backup();
  c.backup();
  t.backup();

  branch_pred.backup();

  for( int r = 0; r < REG; r ++ ){
    reg[r].backup();
  }

  mem_sp.backup();
  mem_mp.backup();

  reg_finite.backup();
}

// restore
void Re_Exec::restore_data(){
  program.restore();
  func_data.re_end();
  icd.restore();
  c.restore();
  t.restore();

  branch_pred.restore();

  for( int r = 0; r < REG; r ++ ){
    reg[r].restore();
  }

  memory.clear_diff();
  mem_sp.restore();
  mem_mp.restore();

  reg_finite.restore();
}

// delete current instruction, push next instructions
void Re_Exec::pop_and_push_inst(){
  if( re != Normal ){
    return;
  }

  // 処理した命令の削除
  spool.pop_front();

  Pipe_Inst inst;

  // push back instructions
  if( read( sim_fd, &(inst), sizeof(inst) ) == sizeof(inst) ){
    spool.push_back(inst);
  }else{
    // サイズの変更
    model.spool_size = spool.size();
  }
}

// spool instructons
void Re_Exec::init_inst_spool(){
  Pipe_Inst inst;

  for( int i = 0; i < model.spool_size; i++ ){
    if( read( sim_fd, &(inst), sizeof(inst) ) == sizeof(inst) ){
      spool.push_back(inst);
    }else{
      error("Re_Exec::inst_spool() pipe read");
    }
  }
}

// no reexec mode && lp mode trace skip
void Re_Exec::init_trace_skip(){
  if( model.fastfwd_tc ){
    cout << "# Re_Exec::init_trace_skip() " << model.fastfwd_tc
	 << " " << flush;

    Pipe_Inst inst;

    for( int tc = 1; tc <= model.fastfwd_tc; tc ++ ){// trace skip
      if( read( sim_fd, &(inst), sizeof(inst) ) != sizeof(inst) ){
	error("Re_Exec::init_trace_skip() read Pipe_Inst");
      }

      if( !(tc % model.print_freq_def) ){
	cout << "." << flush;
      }
    }// trace skip end

    cout << " skip end" << endl;
  }
}

// trace counter
const int Re_Exec::tc(){
  switch( re ){
  case Normal:
    return trace_count;

  case No:
  case Fork:
    return re_trace_count;

  case Change:
  case Finish:
    return trace_count;

  default:
    error("Re_Exec::tc() default switch");
  }
}

void Re_Exec::state(){
  cerr << "Re_Exec::state() mode:";

  switch( re ){
  case  Normal:
    cerr << "Normal";
    break;

  case No:
    cerr << "No";
    break;

  case Change:
    cerr << "Change";
    break;

  case Fork:
    cerr << "Fork";
    break;

  case Finish:
    cerr << "Finish";
    break;

  default:
    cerr << "re " << re;
    error("Re_Exec::state()");
  }

  cerr << endl;
}
