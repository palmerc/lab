//
//  sim.cc   sim model
//

#include <iostream>
#include "sim.h"

static string stamp("Time-stamp: <04/02/12 16:08:12 nakajima>");

#include "print.h"
#include "trace.h"

Sim_Type sim_type;
Sim_Model model;

//
// class simulation model
//

// constructor
Sim_Model::Sim_Model(){
  stamp = stamp.substr(stamp.find("<"), string::npos);
  stamp = stamp.substr(0, stamp.rfind(" ")) + ">\t";

  string msg = " ADD: MD predict, FIXED: setjmp/longjmp, unroll";
  fix_me = "FIX: ";

  time_stamp = stamp + msg;

  // reexec mode
  reexec = true;
  spool_size = 20000;

  // env
  print_freq = 1000000;
  print_freq_def = 5000000;
  debuglevel = 0;

  // trace range
  trace_limit = 100000000;
  fastfwd_tc = 0;

  // 静的分岐予測
  static_brn_pred = false;

  // func inline, loop unroll
  func_inline = loop_unroll = true;
  // perfect disambiguate (SP_REG, GP_REG)
  perf_disamb = true;
  // indirect control dependence
  icd = true;

  // data model
  reg = Reg_Perfect;
  sp = SP_Perfect;
  mp = MP_Perfect;

  // FIX
  vp = VP_Nopred;

  // 投機的send, fork
  sp_exec = SP_Both;
  send_latency = fork_latency = 0;

  // それぞれのループ繰り返し回数の平均を表示、関数の出現頻度
  statistic = false;

  // MP_Predict 初めは同期テーブルが空の状態、
  // falseの場合はプロファイルを読み出す
  mdpred_warmup = true;

  // MP_Predict 複数の静的なストアがあっても理想的に同期をとることができる
  mdpred_perf = false;
}

const bool Sim_Model::debug(const int &debug){ 
  return false;
//      return debuglevel >= debug;
}

// argment check
void Sim_Model::arg_check(const int &argc, char **argv){
  argv0 = argv[0];

  if( argc == 1 ){
    cerr << "===== few argment =====" << endl;
    usage();
  }

  string arg;
  bool file_check = false;

  for( int i = 1; i < argc; i ++){
    arg = argv[i];

    if( arg == "-dir" ){
      // filename
      if( argc - 1 == i ){
	break;
      }

      data_dir = argv[++i];

      if( data_dir[data_dir.size() - 1] != '/' ){
	data_dir += "/";
      }

      cout << endl << "# set dir: " << data_dir << endl << endl;

      bb_info = data_dir + "bb_info";
      posdom_out = data_dir + "posdom";
      ctrleq_out = data_dir + "ctrleq";
      icd_info = data_dir + "icd_info";
      brn_pred = data_dir + "brn_pred";
      loop_del_pc = data_dir + "loop_del_pcs";
      val_pred_result = data_dir + "val_pred";
      bb_statistic = data_dir + "statistic";

      file_check = true;
    }else if( arg == "-trace" || arg == "-trace_limit" ){
      if( argc - 1 == i ){
	break;
      }

      trace_limit = check_atoi(argv[++i]);
    }else if( arg == "-fastfwd" ){
      if( argc - 1 == i ){
	break;
      }

      fastfwd_tc = check_atoi(argv[++i]);
    }else if( arg == "-no_inline" ){
      func_inline = false;
    }else if( arg ==  "-no_loop_unroll" ){
      loop_unroll = false;
    }else if( arg ==  "-no_disamb" ){
      perf_disamb = false;
    }else if( arg ==  "-perf_disamb" ){
      perf_disamb = true;
    }else if( arg == "-no_icd" ){
      icd = false;
    }else if( arg == "-sp_no" ){
      sp_exec = SP_None;
    }else if( arg == "-sp_fork" ){
      sp_exec = SP_Fork;
    }else if( arg == "-sp_send" ){
      sp_exec = SP_Send;
    }else if( arg == "-sp_ss" ){
      sp_exec = SP_Both;
    }else if( arg == "-bp_static" ){
      static_brn_pred = true;
    }else if( arg == "-bp_dynamic" ){
      static_brn_pred = false;
    }else if( arg ==  "-send_latency" ){
      if( argc - 1 == i ){
	break;
      }

      send_latency = check_atoi(argv[++i]);
    }else if( arg ==  "-fork_latency" ){
      if( argc - 1 == i ){
	break;
      }

      fork_latency = check_atoi(argv[++i]);
    }else if( arg ==  "-spool" ){
      if( argc - 1 == i ){
	break;
      }

      spool_size = check_atoi(argv[++i]);

      if( spool_size == 1 ){
	reexec = false;
      }
    }else if( arg == "-reg_none" ){
      reg = Reg_None;
    }else if( arg == "-reg_finite" ){
      if( argc - 1 == i ){
	break;
      }

      reg = Reg_Finite;
      // physical reg num
      reg_physical = check_atoi(argv[++i]);
    }else if( arg == "-reg_perfect" ){
      reg = Reg_Perfect;
    }else if( arg == "-sp_sequential" ){
      sp = SP_Sequential;
    }else if( arg == "-sp_reorder" ){
      sp = SP_Reorder;
    }else if( arg == "-sp_perfect" ){
      sp = SP_Perfect;
    }else if( arg == "-mp_blind" ){
      mp = MP_Blind;
    }else if( arg == "-mp_analysis" ){
      if( argc - 1 == i ){
	break;
      }

      mp = MP_Analysis;
      // MP_Analysis: store num
      mp_store_num = check_atoi(argv[++i]);
    }else if( arg == "-mp_predict_read" ){
      if( argc - 1 == i ){
	break;
      }

      mp = MP_Predict;
      // MP_Predict: store num
      mp_store_num = check_atoi(argv[++i]);
      mdpred_warmup = false;// file read
      mdpred_perf = false;
    }else if( arg == "-mp_predict" ){
      if( argc - 1 == i ){
	break;
      }

      mp = MP_Predict;
      // MP_Predict: store num
      mp_store_num = check_atoi(argv[++i]);
      mdpred_warmup = true;// not file read
      mdpred_perf = false;
    }else if( arg == "-mp_perf_pred" ){
      if( argc - 1 == i ){
	break;
      }

      mp = MP_Predict;
      // MP_Predict: store num
      mp_store_num = check_atoi(argv[++i]);
      mdpred_warmup = true;
      mdpred_perf = true;
    }else if( arg == "-mp_perfect" ){
      mp = MP_Perfect;
    }else if( arg == "-mp_profile" ){
      mp = MP_Profile;
    }else if( arg == "-vp_send" ){
      if( argc - 1 == i ){
	break;
      }

      vp = VP_Send;
      val_pred_th = check_atoi(argv[++i]);
    }else if( arg == "-vp_result" ){
      if( argc - 1 == i ){
	break;
      }

      vp = VP_Result;
      val_pred_th = check_atoi(argv[++i]);
    }else if( arg ==  "-print" ||  arg ==  "-print_count" ){
      if( argc - 1 == i ){
	break;
      }

      print_freq = check_atoi(argv[++i]);

      if( !print_freq ){
	error("arg_check() -print 0");
      }
    }else if( arg ==  "-debug" ){
      if( argc - 1 == i ){
	break;
      }

      debuglevel = check_atoi(argv[++i]);
    }else if( arg ==  "-statistic" ){
      statistic = true;
    }else if( arg ==  "-sim_type" ){
      if( argc - 1 == i ){
	break;
      }

      arg = argv[++i];

      if( arg == "base" ){
	sim_type = BASE;
      }else if( arg == "sp" ){
	sim_type = SP;
      }else if( arg == "cd" ){
	sim_type = CD;
      }else if( arg == "oracle" ){
	sim_type = ORACLE;
      }else if( arg == "sp_cd" ){
	sim_type = SP_CD;
      }else if( arg == "cd_mf" ){
	sim_type = CD_MF;
      }else if( arg == "FC" ){
	sim_type = FC;
      }else if( arg == "LP" ){
	sim_type = LP;
      }else if( arg == "PD" ){
	sim_type = PD;
      }else if( arg == "CE" ){
	sim_type = CE;
      }else{
	break;
      }
    }else if( arg == "-updrive" ){
      if( !file_check || i == argc - 1 ){
	error("arg_check() file_check or i == argc");
      }else{
	child_process(&(argv[++i]));
	return;
      }
    }else if( arg == "-help" ){
      usage();
    }else{
      break;
    }
  }

  cerr << "error option: " << arg << endl;
  usage();
}

void Sim_Model::set_mp_profile(){
  switch( sim_type ){
  case CD_MF:
    mp_analysis = data_dir + "CD_MF-analysis";
    break;

  case LP:
    mp_analysis = data_dir + "LP-analysis";
    break;

  case FC:
    mp_analysis = data_dir + "FC-analysis";
    break;

  case PD:
    mp_analysis = data_dir + "PD-analysis";
    break;

  case CE:
    mp_analysis = data_dir + "CE-analysis";
    break;

  default:
    error("Sim_Model::set_mp_profile() default switch");
  }

  cout << "# Sim_Model::set_mp_profile(): "
       << mp_analysis.substr(mp_analysis.rfind("/") + 1, string::npos) << endl;
}

void Sim_Model::print_argment(){
  cout << "$ ------Sim_Model::print_argment()-----" << endl
       << "$ " << time_stamp << endl
       << "$            " << fix_me << endl;

  if( debuglevel ){
    cout << "$ debug level:" << debuglevel << endl;
  }

  if( !(sim_type & MF) ){
    icd = false;
    reexec = false;
    spool_size = 1;
    mp = MP_Perfect;
  }

  if( reexec ){
    cout << "$ REEXEC MODE: spool size " << spool_size << endl;
  }else{
    cout << "$ NO REEXEC MODE" << endl;
  }

  cout << "$ trace limit: " << trace_limit << endl;
  cout << "$ print freq: " << print_freq << endl;

  if( func_inline ){
    cout << "$ func_call_inline" << endl;
  }
  if( loop_unroll ){
    cout << "$ loop unroll" << endl;
  }
  if( perf_disamb ){
    cout << "$ perfect disambiguate (SP_REG, GP_REG)" << endl;
  }

  if( static_brn_pred ){
    cout << "$ static brn_pred" << endl;
  }else{
    cout << "$ 2lev brn_pred" << endl;
  }

  // simulation type
  cout << "$ simulation type: ";

  switch( sim_type ){
  case BASE:
    cout << "base" << endl;
    break;

  case SP:
    cout << "sp" << endl;
    break;

  case CD:
    cout << "cd" << endl;
    break;

  case SP_CD:
    cout << "sp + cd" << endl;
    break;

  case ORACLE:
    cout << "oracle" << endl;
    break;

  case CD_MF:
    cout << "cd + mf";
    break;

  case LP:
    cout << "LP";
    break;

  case FC:
    cout << "FC";
    break;

  case PD:
    cout << "PD";
    break;

  case CE:
    cout << "CE";
    break;

  default:
    error("Sim_Type::print_argment() sim_type");
  }

  if( !(sim_type & CD) ){
    if( loop_unroll ){
      if( statistic ){
	cout << "$ print statistic" << endl;
      }
    }else{
      statistic = false;
    }
  }

  if( sim_type & MF ){
    if( icd ){
      cout << " (indirect CD)" << endl;
    }else{
      cout << endl;
    }

    switch( mp ){
    case MP_Blind:
      cout << "$ MP blind" << endl;
      break;

    case MP_Analysis:
      cout << "$ MP static analysis, store num: " << mp_store_num << endl;
      break;

    case MP_Predict:
      cout << "$ MP mem dep predict, store num: " << mp_store_num;
      if( mdpred_perf ){
	cout << " (perfect predict)";
      }
      if( mdpred_warmup ){
	cout << " (warmup)" << endl;
      }else{
	cout << " (file read)" << endl;
      }
      break;

    case MP_Profile:
      cout << "$ MP perfect make profile" << endl;
      break;

    case MP_Perfect:
      cout << "$ MP perfect" << endl;
      break;

    default:
      error("Sim_Model::print_argment() mp");
    }

    if( sp_exec & SP_Fork ){
      cout << "$ sp fork";
      if( fork_latency ){
	cout << " (fork latency: " << fork_latency << ")";
      }
      cout << endl;
    }

    if( sp_exec & SP_Send ){
      cout << "$ sp send";
      if( send_latency ){
	cout << " (send latency: " << send_latency << ")";
      }
      cout << endl;
    }
  }

  switch( sp ){
  case SP_Sequential:
    cout << "$ SP sequential" << endl;
    break;

  case SP_Reorder:
    cout << "$ SP reorder" << endl;
    break;

  case SP_Perfect:
    cout << "$ SP perfect" << endl;
    break;

  default:
    error("Sim_Model::print_argment() sp");
  }

  switch( reg ){
  case Reg_None:
    cout << "$ reg none" << endl;
    break;

  case Reg_Finite:
    cout << "$ reg finite register: " << reg_physical << endl;
    break;

  case Reg_Perfect:
    cout << "$ reg perfect" << endl;
    break;

  default:
    error("Sim_Model::print_argment() reg");
  }

  switch( vp ){
  case VP_Send:
    cout << "$ value predict send, th:" << val_pred_th << endl;
    break;

  case VP_Result:
    cout << "$ value predict result, th:" << val_pred_th << endl;
    break;

  case VP_Nopred:
    break;

  default:
    error("Sim_Model::print_argment() vp");
  }

  cout << "$ -------------------------------------" << endl << endl;
}

// usage
void Sim_Model::usage(){
  cout << "Sim_Model::usage()" << endl
       << "version: " << time_stamp << endl
       << "         " << fix_me << endl
       << endl;

  cout << "Usage: " << argv0
       << "  -sim_type [sim_type] [options] -updrive [exec sim-bpred]" << endl;

  cout << "sim_type:" << endl
       << "\tbase, sp, cd, sp_cd, cd_mf, oracle, FC, LP, PD, CE" << endl;

  cout << "options:" << endl
       << "\t-trace <val>, -fastfwd <val>, -print <val> -spool <val>" << endl
       << "\t-sp_no, -sp_send, -sp_fork, -sp_ss (send + fork)" << endl
       << "\t-send_latency <val>, -fork_latency <val>" << endl
       << "\t-reg_none, -reg_finite <p_reg>, -reg_perfect" << endl
       << "\t-sp_sequential, -sp_reorder, -sp_perfect" << endl
       << "\t-mp_blind, -mp_perfect" << endl
       << "\t-mp_analysis, -mp_predict_read, -mp_predict" << endl
       << "\t-vp_result, -vp_send" << endl
       << "\t-no_disamb -perf_disamb"
       << " -no_inline -no_loop_unroll -no_icd" << endl
       << endl
       << "\t-statistic" <<  endl;

  exit(1);
}

// atoi 自然数のみ値を返す
const int Sim_Model::check_atoi(const string str){
  int val = 0;

  if( str != "0" ){
    val = atoi(str.c_str());

    if( val <= 0 ){
      cerr << "Sim_Model::check_atoi() error option <val>: " << str << endl
	   << endl;
      usage();
    }
  }

  return val;
}
