//
// fork.cc
//   send time, loop candidate and child thread executable time data stack
//
//   fork child thread
//
//  Time-stamp: <04/02/07 20:17:04 nakajima>
//

#include <unistd.h>
#include <iostream>
#include "fork.h"

#include "data.h"
#include "icd.h"
#include "loop.h"
#include "pred.h"
#include "print.h"
#include "sim.h"

Func_Stack func_data;

//
// class Func_Stack (function stack data)
//

// constructor
Func_Stack::Func_Stack(){
  for( int i = 0; i < stack_size; i ++ ){
    fstack[i] = 0;// NULL
    fstack_func[i] = 0;
    fstack_diff[i] = 0;// NULL
    fstack_func_diff[i] = 0;
  }

  end = end_diff = 0;
  re_state = false;

  exec_loop_fork = false;
}

// destructor
Func_Stack::~Func_Stack(){
  for( int i = stack_size - 1; i >= 0; i -- ){
    if( fstack[i] ){
      delete[] fstack[i];
    }
  }
}

// init trace skip
void Func_Stack::init_trace_skip(){
  const int func = 0, bb_size = program.size(func);

  end = 0;
  fstack_func[end] = func;

  // allocate fstack[0]
  try{
    fstack[end] = new Bb_Data[bb_size];
  }
  catch( bad_alloc ){
    error("Func_Stack::init_trace_skip() bad_alloc");
  }

  // if fast foward
  if( model.fastfwd_tc ){
    cout << "# Func_Stack::init_trace_skip() " << model.fastfwd_tc
	 << " " << flush;

    Pipe_Inst inst;
    int debug = model.debuglevel;

    model.debuglevel = 0;

    for( int tc = 1; tc <= model.fastfwd_tc; tc ++ ){// trace skip
      if( read( sim_fd, &(inst), sizeof(inst) ) != sizeof(inst) ){
	error("Func_Stack::init_trace_skip() read Pipe_Inst");
      }

      // fbbが変わったかチェック
      if( program.change_fbb(inst.pc, tc) ){
	if( c.last_return_tc(tc) ){
	  pop_stack();
	}else if( c.last_call_tc(tc) || c.last_j_call_tc(tc) ){
	  push_stack();
	}else{
	  const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();
	  Bb_Data data = data_read(fbb);

	  if( last_fbb.bb >= fbb.bb && lp.header(fbb) ){
	    data.lp_unroll = true;
	  }else{
	    data.lp_unroll = false;
	  }

	  data_write(fbb, data);
	}
      }// change fbb

      // brn_typeの更新
      if( inst.func == BRANCH ){
	c.set_branch(tc, inst.brn);
      }

      if( !(tc % model.print_freq_def) ){
	cout << "." << flush;
      }
    }// trace skip end
    cout << " skip end" << endl;

    model.debuglevel = debug;
  }
}

// lp.flag_check()
const bool Func_Stack::not_zero(const Func_Bb &fbb){
  // not zero -> not false -> true
  return( data_read(fbb).lp_unroll );
}

// read Func_Stack => Bb_Data
Func_Stack::Bb_Data & Func_Stack::data_read(const Func_Bb &fbb){
  switch( reexec.mode() ){
  case Normal:
  case Finish:
    if( fstack_func[end] == fbb.func ){
      return fstack[end][fbb.bb];
    }else{
      cerr<< "fstack:" << fstack_func[end] << " func:" << fbb.func << endl;
      error("Func_Stack::data_read() fstack_func[end] != fbb.func");
    }
    break;

  case No:
  case Fork:
  case Change:
    if( fstack_func_diff[end_diff] == fbb.func ){
      return fstack_diff[end_diff][fbb.bb];
    }else{
      cerr<< "fstack_func_diff:" << fstack_func_diff[end]
	  << " func:" << fbb.func << endl;
      error("Func_Stack::data_read() fstack_func_diff[end] != fbb.func");
    }
    break;

  default:
    error("Func_Stack::data_read() default switch");
  }
}

// write Func_Stack <= Bb_Data
void Func_Stack::data_write(const Func_Bb &fbb, const Bb_Data &data){
  switch( reexec.mode() ){
  case Normal:
  case Finish:
    if( fstack_func[end] == fbb.func ){
      fstack[end][fbb.bb] = data;
    }else{
      cerr<< "fstack:" << fstack_func[end] << " func:" << fbb.func << endl;
      error("Func_Stack::data_write() fstack_func[end] == fbb.func");
    }
    break;

  case No:
  case Fork:
  case Change:
    if( fstack_func_diff[end_diff] == fbb.func ){
      fstack_diff[end_diff][fbb.bb] = data;
    }else{
      cerr<< "fstack_func_diff:" << fstack_func_diff[end]
	  << " func:" << fbb.func << endl;
      error("Func_Stack::data_write() fstack_func_diff[end] == fbb.func");
    }
    break;

  default:
    error("Func_Stack::data_write() default switch");
  }
}

///////////////////////////////////////////////////////////////////////

// delete last function's data & restore old data (jr return)
void Func_Stack::pop_stack(){
  const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();
  const string last_fname = program.funcname(last_fbb.func);

  switch( reexec.mode() ){
  case Normal:
    if( model.debug(10) ){
      cerr << "POP fstack: ";
      for( int i = 0; i <= end; i ++ ){
	cerr << fstack_func[i] << ",";
      }
      cerr << " - " << last_fbb.func << " -> " << fbb.func << endl;
    }

    // 通常の関数return
    delete[] fstack[end];
    fstack[end] = 0;// NULL
    end --;

    // setjmp, longjmp, syscall_errorの対処
    if( fstack_func[end] != fbb.func || last_fname == "__longjmp" ){
      if( last_fname == "__setjmp_aux" ){
	// 関数を一つ飛び越えてreturnする
	delete[] fstack[end];
	fstack[end] = 0;// NULL
	end --;

	if( end < 0 ){
	  error("Func_Stack::pop_stack() __setjmp_aux not found");
	}

	// setjmpの呼び出し元の関数を記録
	fstack_setjmp.push(end);
      }else if( last_fname == "__longjmp" ){
	// setjmpを呼び出しても、longjmpしない場合がある
	while( fstack_setjmp.top() >= end ){
	  fstack_setjmp.pop();
	}

	// setjmpの呼び出し元の関数まで戻る
	while( fstack_setjmp.top() != end ){
	  delete[] fstack[end];
	  fstack[end] = 0;// NULL
	  end --;

	  if( end < 0 ){
	    error("Func_Stack::pop_stack() __longjmp not found");
	  }
	}

	fstack_setjmp.pop();
      }else if( last_fname == "syscall_error" ){
	if( false ){
	  cout << "# TC " << reexec.tc()
	       << " Func_Stack::pop_stack() syscall_error" << endl;
	}

	// どれだけ関数を飛び越えるかわからない
	while( fstack_func[end] != fbb.func ){
	  delete[] fstack[end];
	  fstack[end] = 0;// NULL
	  end --;
	}

	if( end < 0 ){
	  error("Func_Stack::pop_stack() syscall_error");
	}
      }

      // check
      if( fstack_func[end] != fbb.func ){
	cerr << " - " << last_fname
	     << " -> " << program.funcname(fbb.func) << endl;
	error("Func_Stack::pop_stack()");
      }
    }
    break;

  case No:
  case Fork:
    if( model.debug(10) ){
      cerr << "POP fstack_diff: ";
      for( int i = 0; i <= end_diff; i ++ ){
	cerr << fstack_func_diff[i] << ",";
      }
      cerr << " - " << last_fbb.func << " -> " << fbb.func << endl;
    }

    delete[] fstack_diff[end_diff];
    fstack_diff[end_diff] = 0;// NULL
    end_diff --;

    if( fstack_func_diff[end_diff] != fbb.func || last_fname == "__longjmp" ){
      if( last_fname == "__setjmp_aux" ){
	delete[] fstack_diff[end_diff];
	fstack_diff[end_diff] = 0;// NULL
	end_diff --;

	if( end_diff < 0 ){
	  error("Func_Stack::pop_stack() re __setjmp_aux not found");
	}

	fstack_setjmp_diff.push(end_diff);
      }else if( last_fname == "__longjmp" ){
	while( fstack_setjmp_diff.top() >= end_diff ){
	  fstack_setjmp_diff.pop();
	}

	while( fstack_setjmp_diff.top() != end_diff ){
	  delete[] fstack_diff[end_diff];
	  fstack_diff[end_diff] = 0;// NULL
	  end_diff --;

	  if( end_diff < 0 ){
	    error("Func_Stack::pop_stack() re __longjmp not found");
	  }
	}

	fstack_setjmp_diff.pop();
      }else if( last_fname == "syscall_error" ){
	while( fstack_func_diff[end_diff] != fbb.func ){
	  delete[] fstack_diff[end_diff];
	  fstack_diff[end_diff] = 0;// NULL
	  end_diff --;
	}

	if( end_diff < 0 ){
	  error("Func_Stack::pop_stack() re syscall_error");
	}
      }

      // check
      if( fstack_func_diff[end_diff] != fbb.func ){
	cerr << " - " << last_fname
	     << " -> " << program.funcname(fbb.func) << endl;
	error("Func_Stack::pop_stack() re");
      }
    }

    // copy original data (fstack[end_diff][]) to fstack_diff[end_diff][]
    if( !fstack_diff[end_diff] ){
      const int bb_size = program.size(fbb.func);

      // allocate fstack_diff[end_diff]
      try{
	fstack_diff[end_diff] = new Bb_Data[bb_size];
      }
      catch( bad_alloc ){
	error("Func_Stack::pop_stack() bad alloc");
      }

      // copy fstack[end_diff][] to fstack_diff[end_diff][]
      memmove( fstack_diff[end_diff], fstack[end_diff],
	       bb_size * sizeof(Bb_Data) );
    }
    break;

  default:
    break;
  }
}

// stack new function data (jall call)
void Func_Stack::push_stack(){
  const Func_Bb fbb = program.fbb();

  switch( reexec.mode() ){
  case Normal:
    if( model.debug(10) ){
      cerr << "PUSH fstack: ";
      for( int i = 0; i <= end; i ++ ){
	cerr << fstack_func[i] << ",";
      }
      cerr << " + " << fbb.func << endl;
    }

    end ++;

    if( end == stack_size ){
      error("Func_Stack::push_stack() stack_size over");
    }else{
      const int bb_size = program.size(fbb.func);

      fstack_func[end] = fbb.func;

      // allocate new function data
      try{
	fstack[end] = new Bb_Data[bb_size];
      }
      catch( bad_alloc ){
	error("Func_Stack::push_stack() bad_alloc");
      }
    }
    break;

  case No:
  case Fork:
    if( model.debug(10) ){
      cerr << "PUSH fstack_diff: ";
      for( int i = 0; i <= end_diff; i ++ ){
	cerr << fstack_func_diff[i] << ",";
      }
      cerr << " + " << fbb.func << endl;
    }

    end_diff ++;

    if( end_diff == stack_size ){
      error("Func_Stack::push_stack() stack_size over");
    }else{
      const int bb_size = program.size(fbb.func);

      fstack_func_diff[end_diff] = fbb.func;

      // allocate new function diff data
      try{
	fstack_diff[end_diff] = new Bb_Data[bb_size];
      }
      catch( bad_alloc ){
	error("Func_Stack::push_stack() bad_alloc");
      }
    }

    break;

  default:
    error("Func_Stack::push_stack() default switch");
  }
}

// for reexec mode
void Func_Stack::re_start(){
  if( re_state ){
    error("Func_Stack::re_start() re_state");
  }

  re_state = true;

  if( model.debug(10) ){
    cerr << "Func_Stack::re_start()" << endl << "fstack_func: ";
    for( int i = 0; i <= end; i ++ ){
      cerr << fstack_func[i] << ",";
    }
    cerr << endl;
  }

  end_diff = end;
  fstack_setjmp_diff = fstack_setjmp;

  // copy stack func num
  for( int i = 0; i <= end_diff; i ++ ){
    fstack_func_diff[i] = fstack_func[i];
  }

  const int func = fstack_func[end_diff], bb_size = program.size(func);

  // allocate fstack_diff[end_diff]
  try{
    fstack_diff[end_diff] = new Bb_Data[bb_size];
  }
  catch( bad_alloc ){
    error("Func_Stack::re_start() bad alloc");
  }

  // copy fstack[end_diff][] to fstack_diff[end_diff][]
  memmove( fstack_diff[end_diff], fstack[end_diff],
  	   bb_size * sizeof(Bb_Data) );
}

// for reexec mode
void Func_Stack::re_end(){
  if( !re_state ){
    error("Func_Stack::re_end() re_state");
  }

  re_state = false;

  if( model.debug(10) ){
    cerr << "Func_Stack::re_end()" << endl;
  }

  // delete all fstack_diff
  for( int i = stack_size - 1; i >= 0; i -- ){
    if( fstack_diff[i] != 0 ){
      delete[] fstack_diff[i];
      fstack_diff[i] = 0;// NULL
    }
    fstack_func_diff[i] = 0;
  }

  end_diff = 0;
}


///////////////////////////////////////////////////////////////////////

// if analyze control depnedences (model CD)
void Func_Stack::analysis(const Pipe_Inst &inst){
  const int tc = reexec.tc();

  if( !program.change_fbb(inst.pc, tc) ){
    return;
  }

  const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();

  //set new limit
  if( c.last_return_tc(tc) ){
    // RETURN

    if( reexec.mode() == Normal ){
      // return value depend
      icd.depend(last_fbb, RET_REG);
    }

    // delete last function's data & restore old data
    pop_stack();

    // START reexec mode: Normal -> No
    if( model.reexec && reexec.mode() == Normal ){
      if( sim_type == FC || sim_type == PD || sim_type == CE ){
	if( data_read(Func_Bb(fbb.func, fbb.bb-1)).exec_time != null_data ){
	  if( program.funcname(last_fbb.func) != "__longjmp" ){
	    reexec.change_no();
	  }
  	}
      }
    }

    cd_ana_jr_return();
  }else if( c.last_call_tc(tc) || c.last_j_call_tc(tc) ){
    // CALL
    cd_ana_jall_call();
  }else{
    // OTHER

    // START reexec mode: Normal -> No
    if( model.reexec && reexec.mode() == Normal && sim_type != FC ){// Normal
      if( model.loop_unroll ){
	const Bb_Data d = data_read(fbb);

	// lp unroll
	if( sim_type == LP ){
	  if( last_fbb.bb >= fbb.bb && lp.header(fbb)
	      && d.lp_exec_time != null_data ){
  	    // ループの繰り返しでフォーク
  	    reexec.change_no();
	  }
	}else if( sim_type == PD || sim_type == CE ){
	  if( last_fbb.bb >= fbb.bb && lp.header(fbb)
	      && d.lp_exec_time != null_data && d.lp_exec_time < d.exec_time ){
	    // ループの繰り返しでフォーク
	    if( exec_loop_fork ){
	      error("Func_Stack::analysis() other, exec_loop_fork true");
	    }

	    exec_loop_fork = true;

  	    reexec.change_no();
	  }else if( d.exec_time != null_data ){
	    reexec.change_no();
	  }
	}
      }else{
	// no lp unroll
	const int exec_time = data_read(fbb).exec_time;

	if( sim_type == LP ){
	  if( last_fbb.bb >= fbb.bb && lp.header(fbb)
	      && exec_time != null_data ){
	    // ループの繰り返しでフォーク
	    reexec.change_no();
	  }
	}else if( sim_type == PD || sim_type == CE ){
	  if( exec_time != null_data ){
	    reexec.change_no();
	  }
	}
      }
    }// re.mode() == Normal

    cd_ana_others();
  }

  // indirect control dependence
  icd.analysis(fbb);
}

///////////////////////////////////////////////////////////////////////

// fork child process
void Func_Stack::fork_child_process(const int &exec_time){
  if( model.debug(20) ){
    cerr << "  limit(" <<  t.cd_limit << ") > exec_time(" << exec_time
	 << ")" << endl;
  }

  // check fork or not fork
  if( !model.reexec ){
    // 先行スレッドの分岐予測ミスより早くスレッドが開始できる場合、fork
    if( t.cd_limit <= exec_time ){
      return;
    }
  }else{
    // reexec mode, mode change
    if( !reexec.check_fork_process(exec_time) ){
      // forkしない
      return;
    }
  }

  if( model.debug(1) ){
    cerr << "  Func_Stack::fork_child_process()" << endl;
  }

  // check
  if( exec_time >= null_data ){
    error("Func_Stack::fork_child_process() exec time null");
  }

  //////// INIT THREAD

  // init icd
  icd.init_thread();

  // init reg flag, send_latency
  for( int i = 1; i < REG; i++ ){
    reg[i].flag = Reg_Flag( reg[i].flag & LAST_DEF_I );
    reg[i].write += model.send_latency;
  }

  // reg model
  switch( model.reg ){
  case Reg_None:
    for( int i = 0; i < REG; i++ ){
      reg[i].read = 0;
    }
    break;

  case Reg_Finite:
    reg_finite.init_thread(t.cd_limit);
    break;

  case Reg_Perfect:
    break;

  default:
    error("Func_Stack::fork_child_process() swithc mpdel.reg default");
  }

  // init mem model
  mem_sp.init_thread();
  mem_mp.init_thread(t.cd_limit);

  if( model.mp == MP_Predict && !model.mdpred_perf ){
    store_tid.init_thread(c.thread);
  }

  mem_sp.last_store_max = max(mem_sp.last_store_max, mem_sp.store_max);

  // max cd limit
  t.max_cd_limit = max( t.max_cd_limit, t.cd_limit );
  // change cd limit
  t.commit = t.cd_limit = exec_time;
  // change thread id
  c.thread ++;
  // set last fork tc
  c.set_last_fork_tc( reexec.tc() );
}

///////////////////////////////////////////////////////////////////////

// if last branch is not jump nor branch instruction
void Func_Stack::cd_ana_others(){
  if( model.debug(1) ){
    cerr << "  no call/return" << endl;
  }

  const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();
  const int tc = reexec.tc();

  if( last_fbb.bb >= fbb.bb && lp.header(fbb) ){
    // loop繰り返し
    if( model.loop_unroll && (sim_type & SP) ){
      if( c.last_con_tc(tc) ){
	// 以前の条件分岐命令の予測がMISSの場合はHITにする
	branch_pred.change_pred_hit();
      }
    }
  }

  Bb_Data data = data_read(fbb);
  int fork_time = null_data, send_time = -1;

  // 分岐予測ミスをしたかどうか？基本ブロック内の命令開始可能な時刻
  t.set_fork_send_time(fork_time, send_time);

  if( last_fbb.bb >= fbb.bb && lp.header(fbb) ){
    if( model.loop_unroll ){
      lp.count_unroll();
      data.lp_unroll = true;
    }else{
      data.send_time = send_time;
    }
  }else{
    // loop headerにはいる、header以外
    data.lp_unroll = false;
    data.send_time = send_time;

    if( last_fbb.bb < fbb.bb && lp.header(fbb) ){
      // loop headerにはいる
      if( model.loop_unroll ){
	data.lp_exec_time = fork_time;
      }
    }
  }

  data.order_tc = tc;

  if( sim_type == LP ){
    if( model.loop_unroll ){
      fork_child_process(data.lp_exec_time);
    }else{
      fork_child_process(data.exec_time);
    }

    data.exec_time = fork_time;
  }else if( sim_type == FC ){
    data.exec_time = fork_time;
  }else{
    // PD or CE
    if( model.loop_unroll && exec_loop_fork ){
      fork_child_process(data.lp_exec_time);
      if( reexec.mode() == Normal ){
	exec_loop_fork = false;
      }
    }else{
      fork_child_process(data.exec_time);
    }

    for( int bb = 0; bb < program.size(fbb.func); bb ++ ){
      if( pd.flag(fbb, bb) ){
	Bb_Data d = data_read(Func_Bb(fbb.func, bb));

	if( d.exec_time > fork_time ){
	  d.exec_time = fork_time;
	  data_write(Func_Bb(fbb.func, bb), d);
	}
      }
    }
    data.exec_time = null_data;
  }

  data_write(fbb, data);

  if( model.debug(1) ){
    cerr <<  "  other limit:" << t.cd_limit << endl;
  }
}


// if last branch is function return
void Func_Stack::cd_ana_jr_return(){
  if( model.debug(1) ){
    cerr << "  jr return" << endl;
  }

  const Func_Bb fbb = program.fbb();
  Bb_Data data = data_read(fbb);
  int fork_time = null_data;

  // 分岐予測ミスをしたかどうか？基本ブロック内の命令開始可能な時刻
  t.set_fork_send_time(fork_time, data.send_time);

  if( model.loop_unroll && lp.header(fbb) ){
    // ループに入った -> すべてのiterationのfork可能時刻は同じ
    data.lp_exec_time = fork_time;
  }

  data.lp_unroll = false;
  data.order_tc = reexec.tc();

  if( sim_type == LP ){
    data.exec_time = fork_time;
  }else if( sim_type == FC ){
    const int exec_time = data_read(Func_Bb(fbb.func, fbb.bb - 1)).exec_time;

    fork_child_process(exec_time);
    data.exec_time = fork_time;
  }else{
    // PD, CE
    const int exec_time = data_read(Func_Bb(fbb.func, fbb.bb - 1)).exec_time;

    fork_child_process(exec_time);

    for( int bb = 0; bb < program.size(fbb.func); bb ++ ){
      if( pd.flag(fbb, bb) ){
	Bb_Data d = data_read(Func_Bb(fbb.func, bb));

	if( d.exec_time > fork_time ){
	  d.exec_time = fork_time;
	  data_write(Func_Bb(fbb.func, bb), d);
	}
      }
    }
    data.exec_time = null_data;
  }

  data_write(fbb, data);

  if( model.debug(1) ){
    cerr << "  return limit:" << t.cd_limit << endl;
  }
}

// if last branch is function call
void Func_Stack::cd_ana_jall_call(){
  if( model.debug(1) ){
    cerr << "  jall call" << endl;
  }

  const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();

  // argment value depend
  icd.depend(last_fbb, ARG_REG);

  // stac old function's data
  push_stack();

  Bb_Data data = data_read(fbb);
  int fork_time = null_data;

  // 分岐予測ミスをしたかどうか？基本ブロック内の命令開始可能な時刻
  t.set_fork_send_time(fork_time, data.send_time);

  if( model.loop_unroll && lp.header(fbb) ){
    // ループに入った -> すべてのiterationのfork可能時刻は同じ
    data.lp_exec_time = fork_time;
  }

  data.lp_unroll = false;
  data.order_tc = reexec.tc();

  if( sim_type == LP || sim_type == FC ){
    data.exec_time = fork_time;
  }else{
    // if not loop nor function only mode
    for( int bb = 0; bb < program.size(fbb.func); bb ++ ){
      if( pd.flag(fbb, bb) ){
	Bb_Data d = data_read(Func_Bb(fbb.func, bb));

	d.exec_time = fork_time;
	data_write(Func_Bb(fbb.func, bb), d);
      }
    }
    data.exec_time = null_data;
  }

  data_write(fbb, data);

  if( model.debug(1) ){
    cerr << "  call limit:" << t.cd_limit << endl;
  }
}

///////////////////////////////////////////////////////////////////////

void Time::set_fork_send_time(int &fork_time, int &send_time){
  // 基本ブロック内の命令開始可能な時刻
  if( !branch_pred.hit() && c.last_branch_tc( reexec.tc() ) ){
  // 以前の命令がbranch(jalr, jr, brn)、かつ、分岐予測ミス
    if( !(sim_type & MF) ){
      branch = max(branch, last_branch + 1);
      // branch misprediction resolve time
      last_branch = branch;
    }
    cd_limit = branch;
  }

  // send time
  if( model.sp_exec & SP_Send ){
    // 投機send
    send_time = cd_limit;
  }else{
    // 確定
    send_time = commit;
  }

  // fork time
  if( model.sp_exec & SP_Fork ){
    // 投機fork
    fork_time = cd_limit + model.fork_latency;
  }else{
    // 確定
    fork_time = commit + model.fork_latency;
  }
}


///////////////////////////////////////////////////////////////////////

// cd_analysis not CD mode
void Func_Stack::analysis_no_CD(const Pipe_Inst &inst){
  const int tc = reexec.tc();

  if( !program.change_fbb(inst.pc, tc) ){
    // not change fbb
    return;
  }

  if( model.loop_unroll ){// if loop unroll
    const Func_Bb fbb = program.fbb(), last_fbb = program.last_fbb();

    if( c.last_return_tc(tc) ){
      if( model.debug(10) ){
	cerr << "  jr return" << endl;
      }

      if( model.statistic ){
	bb_statistic.count_bb(fbb);
      }
      if( lp.header(fbb) ){
	// ループヘッダに入った
	lp.count_header(fbb);
      }

      pop_stack();
    }else if( c.last_call_tc(tc) || c.last_j_call_tc(tc) ){
      if( model.debug(10) ){
	cerr << "  jall call" << endl;
      }

      if( model.statistic ){
	bb_statistic.count_func(fbb);
      }
      if( lp.header(fbb) ){
	// ループヘッダに入った
	lp.count_header(fbb);
      }

      push_stack();
    }else{
      // change bb
      if( model.statistic ){
	bb_statistic.count_bb(fbb);
      }

      Bb_Data data = data_read(fbb);

      if( last_fbb.bb >= fbb.bb && lp.header(fbb) ){
	data.lp_unroll = true;
	lp.count_unroll();

	// ループが繰り返された
	lp.count_bedge(fbb);

	if( (sim_type & SP) && c.last_con_tc(tc) ){
	  // 以前の条件分岐命令の予測がMISSの場合はHITにする
	  branch_pred.change_pred_hit();
	}
      }else{
	data.lp_unroll = false;

	if( last_fbb.bb < fbb.bb && lp.header(fbb) ){
	  // ループヘッダに入った
	  lp.count_header(fbb);
	}
      }

      data_write(fbb, data);
    }
  }// if loop unroll

  // 分岐予測ミス、関数呼び出し
  if( !branch_pred.hit() && c.last_branch_tc(tc) ){
    t.cd_limit = t.branch;
  }
}
