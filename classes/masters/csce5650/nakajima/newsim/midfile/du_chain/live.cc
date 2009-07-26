//
// live.cc
//

#include <iostream>
#include <fstream>
#include "live.h"

//
// class Live_Reg
//

Live_Reg::Live_Reg(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);

  try{
    in = new BITSET[bb_size];
    out = new BITSET[bb_size];
  }
  catch( bad_alloc ){
    error("Reach_Def::Reach_Def() bad_alloc");
  }
}

// destructor
Live_Reg::~Live_Reg(){
  delete[] in;
  delete[] out;
}

//analysisi live reg
void Live_Reg::analysis(Program_Info &program, Def_Use &du){
  // all B in[B] = 0
  for( int bb = 0; bb < bb_size; bb ++ ){
    in[bb].reset();
  }

  bool change = true;

  while(change){
    change = false;

    for( int bb = 0; bb < bb_size; bb ++ ){
      // construction
      Program_Info::SET succ = program.get_succ(Func_Bb(func, bb));
      BITSET def_bb = du.get_def(bb);
      BITSET use_bb = du.get_use(bb);
      BITSET old_in;

      // out[B] = in(s0) | in(s1) | ... | in(sx)
      if( !succ.empty() ){
	for( Program_Info::SI set_i = succ.begin();
	     set_i != succ.end(); set_i ++ ){
	  out[bb] |= in[*set_i];
	}
      }
      // oldin = in[B]
      old_in = in[bb];
      // in[B] = out[B] - def[B] U use[B]
      in[bb] = ( out[bb] ^ ( out[bb] & def_bb ) ) | use_bb;

      // check change
      if( old_in != in[bb] ){
	change = true;
      }
    }
  }
}

void Live_Reg::print(ofstream &fout){
  fout << "livereg" << endl;


  fout << "}" << endl;
}
