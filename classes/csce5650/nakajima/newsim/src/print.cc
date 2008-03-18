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

  std::cout << std::endl << "# print_result_data()" << std::endl;

  // ipc
  c.print_result();

  // prediction
  branch_pred.print_result();
  value_pred.print_result();

  std::cout << "ipc (instance/clock): " << calc_ipc() << std::endl
       << std::endl;

  if( (sim_type & MF) && ( c.thread || model.reexec ) ){
    // THREAD
    if( !t.exec_max ){
      warning("print_tlp() t.exec_max == 0");
      return;
    }

    if( model.reexec ){
      reexec.print_result();
    }else{
      std::cout << "thread count : " << c.thread
	   << std::endl
	   << "inst per thread: " << (double)c.instance() / (double)c.thread
	   << std::endl;
    }

    mem_mp.print_viol();
  }
}


/////////////////////////////////////////////////////////////////////////

void Loop::print_result(){
  if( model.loop_unroll ){
    std::cout << "<loop unroll>" << std::endl 
	 << " loop unroll branch : " << unroll_c << std::endl
	 << " loop induction val : " << induct_c << std::endl
	 << " (loop const val    : " << const_c << " )" << std::endl;

    if( !(sim_type & CD) ){
      print_count_loop();
    }

    std::cout << std::endl;
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

  std::cout << "thread count: " << c.thread << ", re_count: " <<  re_count
       << " per: " << 100 * (double)c.thread / (double)re_count << "%"
       << std::endl
       << " fork: " << fork << " (gain: " << gain_fork
       << ", eq: " << eq_fork << ", nogain: " << no_gain_fork << ")" << std::endl
       << " no: " << no_fork << " (no_gain: " << no_gain_no_fork
       << ", eq: " << eq_no_fork << ")" << std::endl
       << "  (total: " << fork + no_fork << ")" << std::endl
       << std::endl
       << "inst per thread: " << (double)c.instance() / (double)c.thread
       << std::endl;
}

// print_instance
void Counter::print_result(){
  const int tc = reexec.tc();

  std::cout << "trace count         : " << tc << std::endl
       << " nop count          : " << inst_nop << std::endl
       << " call/return count  : " << inst_procedure << std::endl
       << " con branch count   : " << inst_con_branch << std::endl
       << " other branch count : " << inst_other_branch << std::endl
       << " load count         : " << inst_load << std::endl
       << " store count        : " << inst_store << std::endl
       << " other count        : " << inst_other << std::endl
       << "inst (trace - nop)  : " << tc - inst_nop << std::endl
       << std::endl;

  if( model.func_inline){
    std::cout << "<func inline>" << std::endl
	 << " inline call count  : " << inline_call_c << std::endl
	 << " inline return count: " << inline_return_c << std::endl
	 << " inline other count : " << inline_other_c << std::endl
	 << std::endl;
  }

  if( model.perf_disamb ){
    std::cout << "<perfect memory disambiguate (SP_REG, GP_REG)>" << std::endl;
  }else{
    std::cout << "<load/store count (SP_REG, GP_REG)>" << std::endl;
  }

  std::cout << " sp load count      : " << sp_load_c << std::endl
       << " sp store count     : " << sp_store_c << std::endl
       << " gp load count      : " << gp_load_c << std::endl
       << " gp store count     : " << gp_store_c << std::endl
       << std::endl;

  lp.print_result();

  std::cout << "instance count      : " << instance() << std::endl
       << "clock cycle         : " << t.exec_max << std::endl;
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
    std::cout << "==== model mp_blind ====" << std::endl
	 << "mem viol_t: " << mem_viol << std::endl
	 << "mem dep_t: " << mem_dep + mem_viol << std::endl
	 << "mem no dep_t: " << mem_nodep << std::endl
	 << "viol/thread : "
	 << 100.0 * (double)mem_viol / (double)c.thread << std::endl
	 << "viol_load/all_load : "
	 << 100.0 * (double)mem_viol_load / (double)c.load() << std::endl
	 << std::endl;
    break;

  case MP_Predict:
  case MP_Analysis:
    std::cout << "==== model mp_analysis ====" << std::endl
	 << "mem sync_t: " << mem_sync << std::endl
	 << "mem viol_t: " << mem_viol << std::endl
	 << "mem dep_t: " << mem_dep + mem_viol + mem_sync << std::endl
	 << "mem no dep_t: " << mem_nodep << std::endl
	 << "viol/thread : "
	 << 100.0 * (double)mem_viol / (double)c.thread << std::endl
	 << "viol_load/all_load : "
	 << 100.0 * (double)mem_viol_load / (double)c.load() << std::endl
	 << std::endl;
    break;

  case MP_Profile:
  case MP_Perfect:
    break;

  default:
    error("Mem_Dep_MP::print_viol() switch model.mp default");
  }

  std::cout << std::endl;
}

void Value_Predict::print_result(){
  if( model.vp != VP_Nopred ){
    double rate = 100.0 * (double)hit / (double)total;

    std::cout << "<value predict>" << std::endl
	 << "hit rate(hit/total): " << rate
	 << " % (" << hit << "/" << total << ")" << std::endl;
  }
}


void Branch_Predict::print_result(){
  if( pred ){
    double rate = 100.0 * (double)pred_hit / (double)pred;

    std::cout << "<branch predict>" << std::endl
	 << "branch prediction : " << rate
	 << " (" << pred_hit << "/" << pred << ")" << std::endl;
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
    std::cerr << "-----------------------------------------" << std::endl;

    switch( reexec.mode() ){
    case Normal:
      std::cerr << "TC:";
      break;

    case No:
      std::cerr << "NOTC:";
      break;

    case Fork:
      std::cerr << "FOTC:";
      break;

    case Change:
      std::cerr << "Change: " << reexec.tc() << std::endl;
      return;

    case Finish:
      std::cerr << "Finish: " << reexec.tc() << std::endl;
      return;
    }

    std::cerr << reexec.tc() << std::endl
	 << "pc ##" << std::hex << inst.pc << std::dec << std::endl
	 << " src[" << inst.s_A << "]: " << reg[inst.s_A].write
	 << ", [" << inst.s_B << "]: " << reg[inst.s_B].write
	 << ", dest[" << inst.dest << "]: " << reg[inst.dest].write
	 << std::endl;
  }
}

void print_progress_report(){
  if( reexec.mode() != Normal ){
    return;
  }
  if( reexec.tc() % model.print_freq ){
    return;
  }

  std::cout << "@ TC " << reexec.tc() << " cycle: " << t.exec_max
       << " ipc: " << calc_ipc();

  if( (sim_type & MF) && ( c.thread || model.reexec ) ){
    std::cout << " thread: " << c.thread;
    if( model.reexec ){
      std::cout << " re: " << reexec.get_re_count();
    }
  }

  std::cout << std::endl;
}

