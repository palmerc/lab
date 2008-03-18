//
// main.cc
//

#include <iostream>
#include <fstream>
#include "main.h"

#include "bb.h"
#include "du_chain.h"
#include "inst.h"
#include "loop.h"

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

  // init end
  for( int func = 0; func < program.size(); func ++ ){
    // construct
    DU_Chain du_chain(program, func);
    Inst_Type inst_type(program, func);
    Loop loop(program, func);

    loop.analysis(model.fout_loop, program, du_chain, inst_type);

    // progress report
    std::cerr << "\r\t" << func << "/" << program.size() - 1;
  }

  std::cerr << std::endl <<  "generate " << model.loop_del_pcs << std::endl;

  return(0);
}


// arg check
static void arg_check(int argc, char **argv){
  if( argc == 3 ){
    std::string opt = argv[1];
    std::string dir = argv[2];

    if( opt == "-dir" ){
      if( dir[dir.size() - 1] != '/' ){
	dir += "/";
      }

      std::cerr << "dir:" << dir << std::endl;

      model.asm_loop = dir + "asm_loop";
      model.bb_info = dir + "bb_info";
      model.posdom = dir + "posdom";
      model.loop_del_pcs = dir + "loop_del_pcs";
      model.du_chain = dir + "du_chain";
    }else{
      usage(argv[0]);
    }
  }else{
    usage(argv[0]);
  }
}

//
// class Sim_Model simulation model
//

// file open
void Sim_Model::file_open(){
  // file stream
  fin_du_chain.open(model.du_chain.c_str());
  fin_asm_loop.open(model.asm_loop.c_str());
  fin_posdom.open(model.posdom.c_str());
  fout_loop.open(model.loop_del_pcs.c_str());

  if( !fin_du_chain ){
    error("Sim_Model::file_open() can't open" + du_chain);
  }
  if( !fin_asm_loop ){
    error("Sim_Model::file_open() can't open " + asm_loop);
  }
  if( !fin_posdom ){
    error("Sim_Model::file_open() can't open " + posdom);
  }
  if( !fout_loop ){
    error("Sim_Model::file_open() can't open " + loop_del_pcs);
  }
}
