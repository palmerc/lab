//
// print.cc
//
//  Time-stamp: <04/01/19 22:49:43 nakajima>
//

#include <iostream>
#include "print.h"

#include "data.h"
#include "loop.h"
#include "main.h"
#include "pred.h"
#include "re.h"
#include "sim.h"

static double calc_ipc();

// print data
void print_result_data(){
  if( !reexec.tc() ){
    return;
  }

  cout << endl << "# print_result_data()" << endl;

  // ipc
  c.print_result();

  // prediction
  branch_pred.print_result();
  value_pred.print_result();

  cout << "ipc (instance/clock): " << calc_ipc() << endl
       << endl;

  if( (sim_type & MF) && ( c.thread || model.reexec ) ){
    // THREAD
    if( !t.exec_max ){
      warning("print_tlp() t.exec_max == 0");
      return;
    }

    if( model.reexec ){
      reexec.print_result();
    }else{
      cout << "thread count : " << c.thread
	   << endl
	   << "inst per thread: " << (double)c.instance() / (double)c.thread
	   << endl;
    }

    mem_mp.print_viol();
  }
}


/////////////////////////////////////////////////////////////////////////

void Loop::print_result(){
  if( model.loop_unroll ){
    cout << "<loop unroll>" << endl 
	 << " loop unroll branch : " << unroll_c << endl
	 << " loop induction val : " << induct_c << endl
	 << " (loop const val    : " << const_c << " )" << endl;

    if( !(sim_type & CD) ){
      print_count_loop();
    }

    cout << endl;
  }
}

const int Loop::ignore_inst(){
  return( unroll_c + induct_c );
}

// instance
const int Counter::instance(){
  int inst_count = reexec.tc() - inst_nop;

  inst_count -= lp.ignore_inst();
  inst_count -= (inline_call_c + inline_return_c + inline_other_c);

  return inst_count;
}

void Re_Exec::print_result(){
  const int fork = gain_fork + no_gain_fork + eq_fork;
  const int no_fork = no_gain_no_fork + eq_no_fork;

  cout << "thread count: " << c.thread << ", re_count: " <<  re_count
       << " per: " << 100 * (double)c.thread / (double)re_count << "%"
       << endl
       << " fork: " << fork << " (gain: " << gain_fork
       << ", eq: " << eq_fork << ", nogain: " << no_gain_fork << ")" << endl
       << " no: " << no_fork << " (no_gain: " << no_gain_no_fork
       << ", eq: " << eq_no_fork << ")" << endl
       << "  (total: " << fork + no_fork << ")" << endl
       << endl
       << "inst per thread: " << (double)c.instance() / (double)c.thread
       << endl;
}

// print_instance
void Counter::print_result(){
  const int tc = reexec.tc();

  cout << "trace count         : " << tc << endl
       << " nop count          : " << inst_nop << endl
       << " call/return count  : " << inst_procedure << endl
       << " con branch count   : " << inst_con_branch << endl
       << " other branch count : " << inst_other_branch << endl
       << " load count         : " << inst_load << endl
       << " store count        : " << inst_store << endl
       << " other count        : " << inst_other << endl
       << "inst (trace - nop)  : " << tc - inst_nop << endl
       << endl;

  if( model.func_inline){
    cout << "<func inline>" << endl
	 << " inline call count  : " << inline_call_c << endl
	 << " inline return count: " << inline_return_c << endl
	 << " inline other count : " << inline_other_c << endl
	 << endl;
  }

  if( model.perf_disamb ){
    cout << "<perfect memory disambiguate (SP_REG, GP_REG)>" << endl;
  }else{
    cout << "<load/store count (SP_REG, GP_REG)>" << endl;
  }

  cout << " sp load count      : " << sp_load_c << endl
       << " sp store count     : " << sp_store_c << endl
       << " gp load count      : " << gp_load_c << endl
       << " gp store count     : " << gp_store_c << endl
       << endl;

  lp.print_result();

  cout << "instance count      : " << instance() << endl
       << "clock cycle         : " << t.exec_max << endl;
}

void Mem_Dep_MP::print_viol(){
  switch( viol_flag ){
  case DEP:
    mem_dep ++;
    break;

  case SYNC:
    mem_sync ++;
    break;

  case VIOL:
    mem_viol ++;
    break;

  default:
    mem_nodep ++;
    break;
  }

  switch( model.mp ){
  case MP_Blind:
    cout << "==== model mp_blind ====" << endl
	 << "mem viol_t: " << mem_viol << endl
	 << "mem dep_t: " << mem_dep + mem_viol << endl
	 << "mem no dep_t: " << mem_nodep << endl
	 << "viol/thread : "
	 << 100.0 * (double)mem_viol / (double)c.thread << endl
	 << "viol_load/all_load : "
	 << 100.0 * (double)mem_viol_load / (double)c.load() << endl
	 << endl;
    break;

  case MP_Predict:
  case MP_Analysis:
    cout << "==== model mp_analysis ====" << endl
	 << "mem sync_t: " << mem_sync << endl
	 << "mem viol_t: " << mem_viol << endl
	 << "mem dep_t: " << mem_dep + mem_viol + mem_sync << endl
	 << "mem no dep_t: " << mem_nodep << endl
	 << "viol/thread : "
	 << 100.0 * (double)mem_viol / (double)c.thread << endl
	 << "viol_load/all_load : "
	 << 100.0 * (double)mem_viol_load / (double)c.load() << endl
	 << endl;
    break;

  case MP_Profile:
  case MP_Perfect:
    break;

  default:
    error("Mem_Dep_MP::print_viol() switch model.mp default");
  }

  cout << endl;
}

void Value_Predict::print_result(){
  if( model.vp != VP_Nopred ){
    double rate = 100.0 * (double)hit / (double)total;

    cout << "<value predict>" << endl
	 << "hit rate(hit/total): " << rate
	 << " % (" << hit << "/" << total << ")" << endl;
  }
}


void Branch_Predict::print_result(){
  if( pred ){
    double rate = 100.0 * (double)pred_hit / (double)pred;

    cout << "<branch predict>" << endl
	 << "branch prediction : " << rate
	 << " (" << pred_hit << "/" << pred << ")" << endl;
  }
}

////////////////////////////////////////////////////////////////////

// calc ipc
static double calc_ipc(){
  if( !t.exec_max ){
    warning("print_data() t.exec_max == 0");
    return (double)0;
  }else{
    return (double)c.instance() / (double)t.exec_max;
  }
}

void print_debug_header(const Pipe_Inst &inst){
  if( model.debug(2) ){
    cerr << "-----------------------------------------" << endl;

    switch( reexec.mode() ){
    case Normal:
      cerr << "TC:";
      break;

    case No:
      cerr << "NOTC:";
      break;

    case Fork:
      cerr << "FOTC:";
      break;

    case Change:
      cerr << "Change: " << reexec.tc() << endl;
      return;

    case Finish:
      cerr << "Finish: " << reexec.tc() << endl;
      return;
    }

    cerr << reexec.tc() << endl
	 << "pc ##" << hex << inst.pc << dec << endl
	 << " src[" << inst.s_A << "]: " << reg[inst.s_A].write
	 << ", [" << inst.s_B << "]: " << reg[inst.s_B].write
	 << ", dest[" << inst.dest << "]: " << reg[inst.dest].write
	 << endl;
  }
}

void print_progress_report(){
  if( reexec.mode() != Normal ){
    return;
  }
  if( reexec.tc() % model.print_freq ){
    return;
  }

  cout << "@ TC " << reexec.tc() << " cycle: " << t.exec_max
       << " ipc: " << calc_ipc();

  if( (sim_type & MF) && ( c.thread || model.reexec ) ){
    cout << " thread: " << c.thread;
    if( model.reexec ){
      cout << " re: " << reexec.get_re_count();
    }
  }

  cout << endl;
}

