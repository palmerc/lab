//
// main.cc
//

#include <iostream>
#include <fstream>
#include "main.h"

#include "bb.h"
#include "data.h"
#include "pred.h"
#include "re.h"

// global variable
Sim_Model model;

int main(int argc, char **argv){
  model.arg_check(argc, argv);

  // construct
  Pipe_Inst inst;
  Program_Info program;
  Re_Exec reexec;
  //Mem_Region mem_region(program);
  Branch_Predict branch_pred(program);
  Value_Predict value_pred(program);

  // trace skip  
  reexec.init_trace_skip();

  while( reexec.check(inst) ){
    // region check
    //mem_region.check_region(inst, program);
    // branch predict
    branch_pred.count_inst(inst, program);
    // value predict
    value_pred.count_inst(inst, program);

    if( reexec.tc() % model.print_freq == 0 ){
      cerr << "TC:" << reexec.tc() << endl;
    }
  }// main loop

  cout << reexec.tc() << " END" << endl;

  // print file
  //mem_region.file_write(program);
  branch_pred.file_write(program);
  value_pred.file_write(program);

  return 0;
}

//
// class Sim_Model
//

Sim_Model::Sim_Model(){
  trace_limit = 100000000;
  print_freq = print_freq_def = 10000000;
  fastfwd_tc = 0;
}

void Sim_Model::arg_check(const int &argc, char **argv){
  string dir;
  bool file_check = false;

  argv0 = argv[0];

  for( int i = 1; i < argc; i ++){
    string arg = argv[i];

    if( arg == "-dir" ){
      // filename
      if( argc - 1 == i ){
	break;
      }

      dir = argv[++i];

      if( dir[dir.size() - 1] != '/' ){
	dir +=  "/";
      }

      cout << "dir:" << dir << endl;

      // fin
      bb_info = dir + "bb_info";
      // fout
      mem_region = dir + "mem_region";
      brn_pred = dir + "brn_pred";
      val_pred = dir + "val_pred";

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
    }else if( arg ==  "-print" ||  arg ==  "-print_count" ){
      if( argc - 1 == i ){
	break;
      }

      print_freq = check_atoi(argv[++i]);

      if( print_freq <= 0 ){
	error("arg_check() -print");
      }
    }else if( arg == "-updrive" ){
      if( !file_check || i == argc ){
	error("arg_check() file_check or i == argc");
      }else{
	// init
	child_process(&(argv[++i]));
	cerr << "\t=== init end ===" << endl;
	return;
      }
    }else{
      break;
    }
  }

  usage();
}

void Sim_Model::usage(){
  cout << "branch predict, value dest predict" << endl
       << "usage is:" << endl
       << argv0 << " [option] -updrive ..." << endl;

  cout << "option:" << endl
       << "\t-trace <val>, -print <val>, -fastfwd <val>" << endl;

  exit(1);
}

// atoi 自然数のみ値を返す
const int Sim_Model::check_atoi(const string &str){
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
