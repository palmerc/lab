//
// du_chain.cc
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "du_chain.h"

#include "main.h"

//
// class DU_Chain
//

// constructor
DU_Chain::DU_Chain(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);

  try{
    use_chain = new MMAP[bb_size];
  }
  catch( bad_alloc ){
    cerr << func << endl;
    error("DU_Chain::DU_Chain() bad_alloc");
  }

  string buf;

  getline(model.fin_du_chain, buf);
  if( model.fin_du_chain.eof() ){
    error("DU_Chain::DU_Chain() EOF");
  }
  if( buf.find("{") == string::npos ){// function name
    error("DU_Chain::DU_Chain() {");
  }
  if( fname != buf.substr(buf.find(":") + 1, string::npos) ){
    cerr << "bb_info funcname " << fname << ", du_chain funcname "
	 <<  buf.substr(buf.find(":") + 1, string::npos);
    error("DU_Chain::DU_Chain() funcname");
  }

  // read DU
  while(true){// LOOP DU
    getline(model.fin_du_chain, buf);
    if( buf == "}" ){
      break;
    }

    if( buf.find("-") == string::npos || buf.find(">") == string::npos
	|| buf.find("(") == string::npos || buf.find(")") == string::npos ){
      cerr << func << ", " << buf << endl;
      error("DU_Chain::DU_Chain() file error");
    }

    // construct
    DU_Data def, use;

    sscanf( buf.substr(0, buf.find("-")).c_str(), "%x", &def.pc );
    def.bb = program.search_bb(func, def.pc);
    buf.erase(0, buf.find(">") + 1);

    sscanf( buf.substr(0, buf.find("(")).c_str(), "%x", &use.pc );
    use.bb = program.search_bb(func, use.pc);
    buf.erase(0, buf.find("(") + 1);

    def.reg = use.reg = atoi( buf.substr(0, buf.find(")")).c_str() );

    // insert
    use_chain[use.bb].insert( make_pair(use.pc, def) );
  }// LOOP DU
}

// destrutor
DU_Chain::~DU_Chain(){
  delete[] use_chain;
}
