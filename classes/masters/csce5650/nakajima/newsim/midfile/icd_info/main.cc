//
// main.cc
//

#include <iostream>
#include <fstream>
#include "main.h"

#include "bb.h"
#include "icd.h"
#include "du_chain.h"

// global
Sim_Model model;


//
// main
//
static void arg_check(const int &argc, char **argv);

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
    ICD icd(program, func);

    icd.analysis(program, du_chain);
    icd.print(model.fout_icd);

    // progress report
    cerr << "\r\t" << func << "/" << program.size() - 1;
  }

  cerr << endl << "geneate " << model.icd_info << endl;

  return(0);
}

// arg check
static void arg_check(const int &argc, char **argv){
  if( argc == 3 ){
    string opt = argv[1];
    string dir = argv[2];

    if( opt == "-dir" ){
      if( dir[dir.size() - 1] != '/' ){
	dir += "/";
      }

      cerr << "dir:" << dir << endl;

      model.bb_info = dir + "bb_info";
      model.icd_info = dir + "icd_info";
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
  fout_icd.open(model.icd_info.c_str());

  if( !fin_du_chain ){
    error("Sim_Model::file_open() can't open " + du_chain);
  }
  if( !fout_icd ){
    error("Sim_Model::file_open() can't open " + icd_info);
  }
}
