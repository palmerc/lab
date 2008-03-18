//
// main.cc 
//

#include <iostream>
#include <fstream>
#include "main.h"

#include "bb.h"
#include "posdom.h"

// global
Sim_Model model;

//
// main
//
static void arg_check(const int &argc, char **argv);

int main(int argc, char **argv){
  // arg check
  arg_check(argc, argv);
  model.file_open();

  // construct
  Program_Info program;

  int pd_num = 0, ce_num = 0;

  // end init
  for( int func = 0 ; func < program.size(); func ++ ){
    // construct
    PosDom pd(program, func);

    pd.post_dominate(program);
    pd.print(model.fout_posdom);
    pd_num += pd.calc_bb_num();

    pd.dominate(program);
    pd.equivalence();
    pd.print(model.fout_ctrleq);
    ce_num += pd.calc_bb_num();

    // progress report
    std::cerr << "\r\t" << func << "/" << program.size() - 1;
  }

  std::cerr << std::endl << "generate " << model.posdom << " " << model.ctrleq << std::endl;

  std::cout << "pd_num: " << pd_num << " ce_num: " << ce_num << std::endl;

  return(0);
}


// arg check
static void arg_check(const int &argc, char **argv){
  if( argc == 3 ){
    std::string opt = argv[1];
    std::string dir = argv[2];

    if( opt == "-dir" ){
      if( dir[dir.size() - 1] != '/' ){
	dir += "/";
      }

      // filename
      model.bb_info = dir + "bb_info";
      model.posdom = dir + "posdom";
      model.ctrleq = dir + "ctrleq";

      std::cerr << "output file\nposdom_out: " << model.posdom
	   << "\nctrleq_out: " << model.ctrleq << std::endl;
    }else{
      usage(argv[0]);
    }
  }else{
    usage(argv[0]);
  }
}

//
// class Sim_Model
//

void Sim_Model::file_open(){
  // file stream
  fout_posdom.open(model.posdom.c_str());
  fout_ctrleq.open(model.ctrleq.c_str());

  if( !fout_posdom ){
    error("main() can't open " + posdom);
  }
  if( !fout_ctrleq ){
    error("main() can't open " + ctrleq);
  }
}
