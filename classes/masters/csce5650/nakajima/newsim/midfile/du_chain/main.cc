//
// main.cc
//

#include <iostream>
#include <fstream>
#include "main.h"

#include "bb.h"
#include "def_use.h"
#include "live.h"
#include "reach.h"

// global
Sim_Model model;

//
// main
//
static void arg_check(int, char **);

int main(int argc, char **argv){
  // init
  arg_check(argc, argv);
  model.file_open();

  // construct
  Program_Info program;

  for( int func = 0; func < program.size(); func ++ ){
    // construct
    Def_Use du(program, func);
    Reach_Def reach(program, func);
    //    Live_Reg livereg(program, func);

    // make kill/gen for analysis reach definiton and make du-chain
    du.make_kill_gen(program);

    // data_flow equations
    reach.calc_dataflow_eq(program, du);
    // make du_chain
    reach.make_du_chain(program, du);
    // print du_chain
    reach.print(model.fout_du_chain);

    // analysis libereg
    //    livereg.analysis(program, du);
    // print livereg
    //    livereg.print(model.fout_du_chain);

    // progress report
    cerr << "\r\t" << func << "/" << program.size() - 1;
  }

  cerr << endl<< "generate " << model.du_chain << endl;

  return(0);
}

// arg check
static void arg_check(int argc, char **argv){
  if( argc == 3 ){
    string opt = argv[1];
    string dir = argv[2];

    if( opt == "-dir" ){
      if( dir[dir.size() - 1] != '/' ){
	dir += "/";
      }

      cerr << "dir:" << dir << endl;

      model.asm_du = dir + "asm_du";
      model.bb_info = dir + "bb_info";
      model.du_chain = dir + "du_chain";
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
  fin_asm_du.open(model.asm_du.c_str());
  fout_du_chain.open(model.du_chain.c_str());

  if( !fin_asm_du ){
    error("Sim_Model::file_open() can't open" + asm_du);
  }
  if( !fout_du_chain ){
    error("Sim_Model::file_open() can't open" + du_chain);
  }
}
