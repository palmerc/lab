//
// main.cc main, inst analysis 
//
//  Time-stamp: <04/02/07 23:03:04 nakajima>
//

#include <iostream>
#include "main.h"

#include "bb.h"
#include "data.h"
#include "fork.h"
#include "icd.h"
#include "loop.h"
#include "pred.h"
#include "print.h"
#include "re.h"
#include "sim.h"

Counter c;
Time t;
Reg_Data reg[REG];

///////////////////////////////////////////////////////////////////////
// main function
///////////////////////////////////////////////////////////////////////
static void init_main();
static void inst_analysis(const Pipe_Inst &);
static void inst_load(const Pipe_Inst &);
static void inst_store(const Pipe_Inst &);
static void inst_other(const Pipe_Inst &);
static void inst_branch(const Pipe_Inst &);

int main(int argc, char **argv){
  // arg check and initialize
  model.arg_check(argc, argv);
  model.print_argment();
  init_main();

  // main loop
  Pipe_Inst inst;

  if( model.statistic ){
    bb_statistic.count_func(program.fbb());
  }

  while( reexec.check_mode(inst) ){
    // cd analysis
    if( sim_type & CD ){
      func_data.analysis(inst);
    }else{
      func_data.analysis_no_CD(inst);
    }

    // data dependence analysis
    print_debug_header(inst);
    inst_analysis(inst);
    print_progress_report();

    // delete current instruction, push next instructions
    reexec.pop_and_push_inst();
  }// main loop

  // print result data
  print_result_data();

  if( model.statistic ){
    bb_statistic.print();
  }

  if( model.mp == MP_Profile ){
    // mp profile for mp_analysis mode
    mem_profile.file_write();
  }

  cout << "return 0" << endl;

  return 0;
}


//
// initialize
//
static void init_main(){
  // file read
  program.file_read();// 初めに読み込むこと

  if( sim_type & SP ){
    if( model.static_brn_pred ){
      // init static branch predict table
      branch_pred.file_read();
    }else{
      // allocate BHT, PHT
      branch_pred.allocate();
    }
  }

  if( sim_type & CD ){
    pd.file_read();
    lp.file_read();
    icd.file_read();
  }else{
    // no CD
    if( model.loop_unroll ){
      lp.file_read();
    }
    if( model.statistic ){
      bb_statistic.allocate();
    }
  }

  // trace skip
  if( (sim_type & CD) || model.loop_unroll ){
    func_data.init_trace_skip();
  }else{
    // no reexec mode (no CD && no loop_unroll)
    reexec.init_trace_skip();
  }

  switch( model.mp ){
  case MP_Analysis:
    // static analysis
    model.set_mp_profile();
    mem_mp.file_read();
    break;

  case MP_Predict:
    if( model.mdpred_warmup ){
      // warm upにより並列性低下
      mem_mp.allocate();
    }else{
      // プロファイルを読むので、warm upによる性能低下は抑えられる 
      model.set_mp_profile();
      mem_mp.file_read();
    }
    break;

  case MP_Profile:
    model.set_mp_profile();
    mem_profile.allocate();
    break;

  default:
    // blind, perfect
    break;
  }

  if( model.vp != VP_Nopred ){
    // value predict
    value_pred.file_read();
  }

  if( model.reg == Reg_Finite ){
    reg_finite.init_thread(0);
  }

  // pipe inst spool
  reexec.init_inst_spool();

  cout << endl;
}


///////////////////////////////////////////////////////////////////////

//
// class Counter
//

Counter::Counter(){
  inst_nop = inst_load = inst_store = inst_other = 0;
  inst_procedure = inst_con_branch = inst_other_branch = 0;

  inline_call_c = inline_return_c = inline_other_c = 0;

  sp_load_c = sp_store_c = gp_load_c = gp_store_c = 0;

  thread = last_fork_tc = branch_tc = 0;

  // last branch type
  brn_type = CR_MISSMATCH;
}

// instruction analysis and count instruction
void Counter::count_inst(const Pipe_Inst &inst){
  if( reexec.mode() == Normal ){
    switch( inst.func ){
    case NOOP:
      inst_nop ++;
      break;

    case LOAD:
      inst_load ++;
      break;

    case STORE:
      inst_store ++;
      break;

    case OTHER:
      inst_other ++;
      break;

    case BRANCH:
      switch( inst.brn ){
      case JALL_CALL:
      case JR_RETURN:
	inst_procedure ++;
	break;

      case CON_DIRE:
	inst_con_branch ++;
	break;

      case UNCON_INDIRE:
      case UNCON_DIRE:
	inst_other_branch ++;
	break;

      default:
	error("Counter::count_inst() default switch");
      }
      break;

    default:
      error("Counter::count_inst() default switch");
    }
  }
}

void Counter::count_inline(const Pipe_Inst &inst){
  if( reexec.mode() == Normal ){
    switch( inst.func ){
    case OTHER:
      inline_other_c ++;
      break;

    case BRANCH:
      switch( inst.brn ){
      case JALL_CALL:
	inline_call_c ++;
	break;

      case JR_RETURN:
	inline_return_c ++;
	break;

      default:
	error("Counter::count_inline() call/return default");
      }
      break;

    default:
      error("Counter::count_inline() inst default");
    }
  }
}

void Counter::count_sp(const Funit_Type &type){
  if( reexec.mode() == Normal ){
    switch( type ){
    case LOAD:
      sp_load_c ++;
      break;

    case STORE:
      sp_store_c ++;
      break;

    default:
      error("Counter::count_sp() inst default");
    }
  }
}

void Counter::count_gp(const Funit_Type &type){
  if( reexec.mode() == Normal ){
    switch( type ){
    case LOAD:
      gp_load_c ++;
      break;

    case STORE:
      gp_store_c ++;
      break;

    default:
      error("Counter::count_gp() inst default");
    }
  }
}

void Counter::set_branch(const int &tc, const Branch_Type &type){
  branch_tc = tc;
  brn_type = type;
}

const bool Counter::last_call_tc(const int &tc){
  // call
  return( ( branch_tc + 1 == tc ) && brn_type == JALL_CALL );
}

const bool Counter::last_j_call_tc(const int &tc){
  const int func = program.fbb().func, last_func = program.last_fbb().func;

  if( last_func != func ){
    // 関数呼び出しだが、以前の分岐がj命令の場合
    if( ( branch_tc + 1 == tc ) &&  brn_type == UNCON_DIRE ){
      const string f = program.funcname(func);

      if( f == "__setjmp_aux" || f == "syscall_error" ){
	if( false && reexec.mode() == Normal ){
	  cerr << "# TC " << reexec.tc() << " last_j_call_tc" << endl;
	}
	return true;
      }
    }
    // 変な関数呼び出し
    if( ( branch_tc + 1 == tc ) &&  brn_type == CON_DIRE ){
      const string f = program.funcname(func);

      if( f[0] == '_' && f[1] == '_' ){
  	if( false && reexec.mode() == Normal ){
	  cerr << "# TC " << reexec.tc() << " last_j_call_tc" << endl;
  	}
	return true;
      }
    }
  }

  return false;
}

const bool Counter::last_return_tc(const int &tc){
  // return
  return( ( branch_tc + 1 == tc ) && brn_type == JR_RETURN );
}

const bool Counter::last_branch_tc(const int &tc){
  // all branch(brn, call, return, j, jmp)
  return( ( branch_tc + 1 == tc ) && brn_type != CR_MISSMATCH );
}

const bool Counter::last_con_tc(const int &tc){
  // conditional branch
  return( ( branch_tc + 1 == tc ) && brn_type == CON_DIRE );
}

void Counter::set_last_fork_tc(const int &tc){
  last_fork_tc = tc;
}

const bool Counter::last_thread_defined(const int &mem_thread_id){
//    return( tc && ( last_fork_tc > tc ) );
  return( mem_thread_id < thread );
}

///////////////////////////////////////////////////////////////////////

// instruction analysis and count instruction
static void inst_analysis(const Pipe_Inst &inst){
  switch( reexec.mode() ){
  case Change:
    if( model.debug(100) ){
      cerr << "inst_analysis() skip re == Change" << endl;
    }
    return;

  case Finish:
    error("inst_analysis() reexec.mode() == Finish");

  default:
    break;
  }

  reg[ZERO_REG] = Reg_Data(DEF_I);

  // perfect inline
  if( model.func_inline ){
    reg[SP_REG] = Reg_Data();
  }

  c.count_inst(inst);

  if( model.statistic ){
    switch( inst.func ){
    case LOAD:
    case STORE:
    case OTHER:
    case BRANCH:
      bb_statistic.count_inst(program.fbb());
      break;

    case NOOP:
      break;

    default:
      error("inst_analysis() switch");
    }
  }

  switch( inst.func ){
  case NOOP:
    break;

  case LOAD:
    inst_load(inst);
    break;

  case STORE:
    inst_store(inst);
    break;

  case OTHER:
    inst_other(inst);
    break;

  case BRANCH:
    inst_branch(inst);
    break;

  default:
    error("inst_analysis() switch");
  }
}

///////////////////////////////////////////////////////////////////////
// LOAD [srcA and srcB:address]
///////////////////////////////////////////////////////////////////////
static void load_data_dep(const Pipe_Inst &);
static void load_data_dep_next(const Pipe_Inst &);

static void inst_load(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, dest = inst.dest;
  const int tc = reexec.tc();

  // perfect disambiguate
  if( srcA == SP_REG || srcB == SP_REG ){
    c.count_sp(inst.func);
  }else if( srcA == GP_REG || srcB == GP_REG ){
    c.count_gp(inst.func);
  }

  // calc destination register write time
  load_data_dep(inst);
  load_data_dep_next(inst);

  // exec time
  t.exec_max = max(t.exec_max, reg[dest].write);
  reexec.gain(reg[dest].write);

  if( srcA > 0 ){
    reg[srcA].flag = Reg_Flag(reg[srcA].flag | USE);
  }
  if( srcB > 0 ){
    reg[srcB].flag = Reg_Flag(reg[srcB].flag | USE);
  }

  reg[dest].flag = DEFINE;
  reg[dest].def_tc = tc;
  if( inst.double_reg ){
    reg[dest + 1].flag = DEFINE;
    reg[dest + 1].def_tc = tc;
  }

  if( model.debug(5) ){
    cerr << " Load address " << hex << inst.address << dec << endl
	 << "      src reg[" << srcA << "]: " << reg[srcA].write << endl
	 << "             [" << srcB << "]: " << reg[srcB].write << endl
	 << "    write reg[" << dest << "]: " << reg[dest].write << endl;
  }
}

// none, finite, sequential, reorder, blind, analysis
static void load_data_dep(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, dest = inst.dest;
  int write_time =  max(reg[srcA].write, reg[srcB].write);
  int limit = t.cd_limit;

  for( int byte = 0; byte < inst.m_size; byte ++ ){
    write_time = max(write_time, memory.read_data(inst.address + byte).write);
  }

  switch( model.reg ){
  case Reg_None:
    if( !c.thread || reg[dest].flag & DEFINE ){
      limit = max(limit, max(reg[dest].write, reg[dest].read));
    }
    break;

  case Reg_Finite:
    limit = max(limit, reg_finite.get_finite_limit());
    break;

  case Reg_Perfect:
    break;

  default:
    error("load_data_dep() swithc mpdel.reg default");
  }

  if( model.perf_disamb && ( srcA == SP_REG || srcB == SP_REG
  			     || srcA == GP_REG || srcB == GP_REG ) ){
    // perfect disambiguate
    // vp unpredict
    reg[dest].write = 1 + max(write_time, limit);
  }else{
    switch( model.sp ){
    case SP_Sequential:
      // max store time
      limit = max(limit, mem_sp.store);
      break;

    case SP_Reorder:
      // max load address time
      mem_sp.load = max(mem_sp.load, max(reg[srcA].write, reg[srcB].write));
      // max store address time
      limit = max(limit, mem_sp.store);
      break;

    case SP_Perfect:
      break;

    default:
      error("load_data_dep() swithc mpdel.sp default");
    }

    // mp_blind, mp_analysis, mp profile for mp_analysis mode
    switch( model.mp ){
    case MP_Blind:
      mem_mp.blind(inst, limit);
      break;

    case MP_Analysis:
      mem_mp.analysis(inst, limit);
      break;

    case MP_Predict:
      mem_mp.md_predict(inst, store_tid, limit);
      break;

    case MP_Profile:
      mem_profile.count_inst(inst, limit);
      break;

    case MP_Perfect:
      break;

    default:
      error("load_data_dep() switch model.mp default");
    }

    if( model.vp == VP_Nopred ){
      // vp unpredict
      reg[dest].write = 1 + max(write_time, limit);
    }else{
      // vp predict
      switch( value_pred.predict(inst) ){
      case NO_VP:
      case MISS_VP:
	// missペナルティーなし
	reg[dest].write = 1 + max(write_time, limit);
	break;

      case HIT_VP:
	reg[dest].write = 1 + limit;
	break;
      }
    }
  }

  reg[dest].commit = max(t.commit, reg[dest].write);
  if( inst.double_reg ){
    reg[dest + 1].commit = reg[dest].commit;
  }
}

// none/finite, sequential/reorder
static void load_data_dep_next(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, dest = inst.dest;

  switch( model.reg ){
  case Reg_None:
    reg[srcA].read = reg[dest].write - 1;
    reg[srcB].read = reg[dest].write - 1;
    break;

  case Reg_Finite:
    reg_finite.counter(reg[dest], t.commit);
    break;

  case Reg_Perfect:
    break;

  default:
    error("load_data_dep_next() swithc mpdel.reg default");
  }

  if( model.perf_disamb && ( srcA == SP_REG || srcB == SP_REG
  			     || srcA == GP_REG || srcB == GP_REG ) ){
    // perfect disambiguate
  }else{
    switch( model.sp ){
    case SP_Sequential:
      mem_sp.load = max(mem_sp.load, reg[dest].write);
      mem_sp.last_type = LOAD;
      break;

    case SP_Reorder:
      mem_sp.last_type = LOAD;
      break;

    case SP_Perfect:
      break;

    default:
      error("load_data_dep_next() swithc mpdel.sp default");
    }
  }
}


///////////////////////////////////////////////////////////////////////
// STORE [srcA:data] [srcB,srcC:address]
///////////////////////////////////////////////////////////////////////
static const int store_deta_dep(const Pipe_Inst &);

// inst store analysis
static void inst_store(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, srcC = inst.s_C;
  const int pc = inst.pc, address = inst.address, store_byte = inst.m_size;

  // perfect disambiguate
  if( srcB == SP_REG || srcC == SP_REG ){
    c.count_sp(inst.func);
  }else if( srcB == GP_REG || srcC == GP_REG ){
    c.count_gp(inst.func);
  }

  if( model.debug(5) ){
    cerr << " Store address " << hex << inst.address << dec << endl
	 << "       src reg[" << srcA << "]: " << reg[srcA].write << endl
	 << "              [" << srcB << "]: " << reg[srcB].write << endl
	 << "              [" << srcC << "]: " << reg[srcC].write << endl;
  }

  const int write_time = store_deta_dep(inst);

  // Mem_Data (store pc, store write time, currend thread_id)
  Mem::Mem_Data mem_data(pc, write_time, c.thread);

  // write store data
  for( int byte = 0; byte < store_byte; byte ++ ){
    memory.write_data(address + byte, mem_data);
  }

  // スレッド内で複数静的なストア命令が出現するか？
  if( model.mp == MP_Predict ){
    if( !model.mdpred_perf ){
      store_tid.add_store_pc(pc, write_time);
    }
  }

  t.exec_max = max(t.exec_max, write_time);
  reexec.gain(write_time);

  if( model.reg == Reg_None ){
    reg[srcA].read = write_time - 1;
    reg[srcB].read = write_time - 1;
    reg[srcC].read = write_time - 1;
  }

  if( model.perf_disamb && ( srcB == SP_REG || srcC == SP_REG
  			     || srcB == GP_REG || srcC == GP_REG ) ){
    // perfect disambiguate
  }else{
    switch( model.sp ){
    case SP_Sequential:
      mem_sp.store = max(mem_sp.store, write_time);
      mem_sp.last_type = STORE;
      break;

    case SP_Reorder:
      mem_sp.last_type = STORE;
      break;

    case SP_Perfect:
      break;

    default:
      error("inst_store() swithc mpdel.sp default");
    }
  }

  if( srcA > 0 ){
    reg[srcA].flag = Reg_Flag(reg[srcA].flag | USE);
  }
  if( srcB > 0 ){
    reg[srcB].flag = Reg_Flag(reg[srcB].flag | USE);
  }
  if( srcC > 0 ){
    reg[srcC].flag = Reg_Flag(reg[srcC].flag | USE);
  }

  if( model.debug(5) ){
    cerr << "    write time: " << write_time << endl;
  }
}

// sequential, reorder, finite, blind, analysis
static const int store_deta_dep(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, srcC = inst.s_C;
  int write_time = max(reg[srcA].write, max(reg[srcB].write, reg[srcC].write));

  if ( inst.double_reg ){
    write_time = max(write_time, reg[srcA + 1].write);
  }

  if( model.reg == Reg_Finite ){
    write_time = max(write_time, reg_finite.get_finite_limit());
  }

  write_time = max(write_time, t.cd_limit);

  if( model.perf_disamb && ( srcB == SP_REG || srcC == SP_REG
  			     || srcB == GP_REG || srcC == GP_REG ) ){
    // perfect disambiguate
  }else{
    switch( model.sp ){
    case SP_Sequential:
      write_time = max(write_time, max(mem_sp.load, mem_sp.store));
      break;

    case SP_Reorder:
      // max store address time
      mem_sp.store = max(mem_sp.store, max(reg[srcB].write, reg[srcC].write));
      // max load and store address time
      write_time = max(write_time, max(mem_sp.load, mem_sp.store));
      break;

    case SP_Perfect:
      break;

    default:
      error("store_data_dep() swithc mpdel.sp default");
    }

    if( model.mp != MP_Perfect && model.mp != MP_Profile ){
      write_time = max(write_time, mem_mp.get_viol_limit());
    }

    mem_sp.store_max = max(mem_sp.store_max, write_time);
  }

  return write_time + 1;
}


///////////////////////////////////////////////////////////////////////
// OTHER
///////////////////////////////////////////////////////////////////////
static void other_data_dep(const Pipe_Inst &);

// inst other analysis
static void inst_other(const Pipe_Inst &inst){
  const int dest = inst.dest, srcA = inst.s_A, srcB = inst.s_B;

  if( model.func_inline && dest == SP_REG
      && srcA == SP_REG && srcB == ZERO_REG ){
    // perfect inline
    // stack操作のadd命令は無視
    c.count_inline(inst);

    if( model.statistic ){
      bb_statistic.uncount_inst(program.fbb());
    }

    if( model.debug(5) ){
      cerr <<  "perfect inline unrolling dest == 29" << endl;
    }
  }else if( dest != ZERO_REG ){
    // data dependence
    other_data_dep(inst);

    t.exec_max = max(t.exec_max, reg[dest].write);
    reexec.gain(reg[dest].write);
  }

  if( srcA > 0 ){
    reg[srcA].flag = Reg_Flag(reg[srcA].flag | USE);
  }

  if( srcB > 0 ){
    reg[srcB].flag = Reg_Flag(reg[srcB].flag | USE);
  }

  if( model.debug(5) ){
    cerr << " Other limit " << t.cd_limit << endl
	 << "   write reg[" << dest << "]: " << reg[dest].write << endl;
  }
}

// none
static void other_data_dep(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B, dest = inst.dest;
  int write_time = max(reg[srcA].write, reg[srcB].write);
  int limit = t.cd_limit;

  if( model.mp != MP_Perfect && model.mp != MP_Profile ){
    limit = max(limit, mem_mp.get_viol_limit());
  }

  switch( model.reg ){
  case Reg_None:
    if( !c.thread || reg[dest].flag & DEFINE ){
      limit = max(limit, max(reg[dest].write, reg[dest].read));
    }
    break;

  case Reg_Finite:
    limit = max(limit, reg_finite.get_finite_limit());
    break;

  case Reg_Perfect:
    break;

  default:
    error("other_data_dep() swithc mpdel.reg default");
  }

  // loop unroll
  if( model.loop_unroll ){
    Skip flag = lp.flag_check(inst);

    lp.count_loop_inst(flag);
    switch( flag ){
    case INDUCT_PC:
      // induct val, reg[dest].write = 0
      reg[dest].write = 0;
      reg[dest].flag = DEF_I;
      return;

    case INDUCT_CAND:
      reg[dest].flag = DEF_I;
      break;

    case CONST_PC:
    case CONST_CAND:
    default:// not found
      // const variableは通常命令とする
      reg[dest].flag = DEFINE;
      break;
    }
  }else{
    reg[dest].flag = DEFINE;
  }

  if( model.vp == VP_Nopred ){
    // vp unpredict
    reg[dest].write = 1 + max(limit, write_time);
  }else{
    // vp predict
    switch( value_pred.predict(inst) ){
    case NO_VP:
    case MISS_VP:
      // missペナルティーなし
      reg[dest].write = 1 + max(limit, write_time);
      break;

    case HIT_VP:
      reg[dest].write = 1 + limit;
      break;
    }
  }

  // HI LO register
  if( dest == HI_REG && srcA != ZERO_REG && srcB != ZERO_REG ){
    reg[dest + 1].write = reg[dest].write;
    reg[dest + 1].def_tc = reexec.tc();
  }

  reg[dest].commit = max(t.commit, reg[dest].write);
  reg[dest].def_tc = reexec.tc();

  // 資源制約
  switch( model.reg ){
  case Reg_None:
    reg[srcA].read = reg[dest].write - 1;
    reg[srcB].read = reg[dest].write - 1;
    break;

  case Reg_Finite:
    reg_finite.counter(reg[dest], t.commit);
    break;

  case Reg_Perfect:
    break;

  default:
    error("other_data_dep() swithc mpdel.reg default");
  }
}


///////////////////////////////////////////////////////////////////////
// BRANCH
///////////////////////////////////////////////////////////////////////
static void branch_analysis(const Pipe_Inst &, const int &);

static void inst_branch(const Pipe_Inst &inst){
  const int srcA = inst.s_A, srcB = inst.s_B;
  const int limit = t.cd_limit;

  // branch type
  c.set_branch( reexec.tc(), inst.brn );

  branch_analysis(inst, limit);

  // change branch
  if( model.reg == Reg_Finite ){
    t.branch = max( t.branch,
		    max( t.cd_limit, reg_finite.get_finite_limit() ) + 1 );
  }

  if( model.mp != MP_Perfect && model.mp != MP_Profile ){
    t.branch = max(t.branch, mem_mp.get_viol_limit() + 1);
  }

  t.commit = max(t.commit, t.branch);

  if( sim_type == ORACLE ){
    branch_pred.set_hit();
  }

  if( srcA > 0 ){
    reg[srcA].flag = Reg_Flag(reg[srcA].flag | USE);
  }

  if( srcB > 0 ){
    reg[srcB].flag = Reg_Flag(reg[srcB].flag | USE);
  }


  if( model.debug(5) ){
    cerr << " Branch ";
    switch( inst.brn ){
    case JALL_CALL:
      cerr << "JALL_CALL: ";
      break;

    case JR_RETURN:
      cerr << "JR_RETURN: ";
      break;

    case CON_DIRE:
      cerr << "CON_DIRE: ";
      break;

    case UNCON_INDIRE:
      cerr << "UNCON_INDIRE: ";
      break;

    case UNCON_DIRE:
      cerr << "UNCON_DIRE: ";
      break;

    default:
      break;
    }

    cerr << " src[" << srcA << "]: " << reg[srcA].write << endl
	 << "           [" << srcB << "]: " << reg[srcB].write << endl
	 << "      limit: " << limit << endl;
  }

  if( model.debug(1) ){
    if( branch_pred.hit() ){
      cerr << "  branch pred :HIT" << endl;
    }else{
      cerr << "  branch pred :MISS" << endl;
    }
  }

  if( model.debug(5) ){
    cerr << "  target addr start time: " << t.branch << endl
	 << "        last branch time: " << t.last_branch << endl;
  }
}

//
static void branch_analysis(const Pipe_Inst &inst, const int &limit){
  const int srcA = inst.s_A, srcB = inst.s_B;
  int regA = 0, regB = 0;

  if( srcA > 0 ){
    regA = reg[srcA].write;
  }
  if( srcB > 0 ){
    regB = reg[srcB].write;
  }

  branch_pred.set_hit();

  switch( inst.brn ){
  case JALL_CALL:
    // jal
    if( model.func_inline ){
      // perfect inline
      // call命令は削除されている
      c.count_inline(inst);

      if( model.statistic ){
	bb_statistic.uncount_inst(program.fbb());
      }
    }else{
      t.branch = limit + 1;
      branch_pred.change_miss();// no SP
    }
    break;

  case JR_RETURN:
    // jr $31
    if( model.func_inline ){
      // perfect inline
      // return命令は削除されている
      c.count_inline(inst);

      if( model.statistic ){
	bb_statistic.uncount_inst(program.fbb());
      }
    }else{
      t.branch = limit + 1;
      branch_pred.change_miss();// no SP
    }
    break;

  case CON_DIRE:
    // brn
    t.branch = max(limit, max(regA, regB)) + 1;

    branch_pred.change_miss();// no SP

    if( (sim_type & SP) && sim_type != ORACLE ){
      // branch prediction
      branch_pred.predict(inst);
    }

    if( model.loop_unroll && lp.flag_check(inst) == EXIT_BRANCH ){
      branch_pred.change_unroll();
    }
    break;

  case UNCON_INDIRE:
    // jr $2 (switch)
    t.branch = max(regA, limit) + 1;
    branch_pred.change_miss();// no SP
    break;

  case UNCON_DIRE:
    // j
    t.branch = limit + 1;
    break;

  default:
    error("branch_analysis() default");
  }
}
