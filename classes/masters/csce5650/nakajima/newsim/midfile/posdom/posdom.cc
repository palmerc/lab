//
// posdom.cc generate posdom/ctrl_eq data
//

#include <iostream>
#include <fstream>
#include "posdom.h"

#include "bb.h"


//
// class PosDom
//

// constructor
PosDom::PosDom(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);

  try{
    pos = new BITSET[bb_size];
    dom = new BITSET[bb_size];
  }
  catch( bad_alloc ){
    error("PosDom::PosDom() bad_alloc pd = new");
  }
}

// destructor
PosDom::~PosDom(){
  delete[] pos;
  delete[] dom;
}

// dominate (CE) 
//   P816 algorism 10.16
void PosDom::dominate(Program_Info &program){
  // D(n0) = {n0}
  dom[0].set(0);
  // for all of n = { N - {n0} } do D(n) = N;
  for( int n = 1; n < bb_size; n ++ ){
    dom[n].set();
  }

  bool change = true;// start while loop

  // init end
  while( change ){
    change = false;

    // for all of n = { N - n0 } do ...
    for( int n = 1; n < bb_size; n ++ ){
      // construct
      BITSET temp;
      Program_Info::SET pred = program.get_pred(Func_Bb(func, n));

      // D(p0) & D(p1) & ... & D(px)
      if( !pred.empty() ){
	temp.set();
	for( Program_Info::SI set_i = pred.begin();
	     set_i != pred.end(); set_i ++ ){
	  temp &= dom[*set_i];
	}
      }
      // D(n) = {n} | { & (p = in[n]) dom[p] }
      temp.set(n);

      if( dom[n] != temp ){// check change
	dom[n] = temp;
	change = true;
      }
    }
  }
}

// post dominate (PD)
void PosDom::post_dominate(Program_Info &program){
  // D(n0) = {n0}
  pos[bb_size - 1].set(bb_size - 1);
  // for all of n = { N - {n0} } do D(n) = N;
  for( int n = 0; n < bb_size - 1; n ++ ){
    pos[n].set();
  }

  bool change = true;// start while loop

  // init end
  while( change ){
    change = false;

    // for all of n = { N - n0 } do ...
    for( int n = 0; n < bb_size - 1; n ++ ){
      // construct
      BITSET temp;
      Program_Info::SET succ = program.get_succ(Func_Bb(func, n));

      // D(p0) & D(p1) & ... & D(px)
      if( !succ.empty() ){
	temp.set();
	for( Program_Info::SI set_i = succ.begin();
	     set_i != succ.end(); set_i ++ ){
	  temp &= pos[*set_i];
	}
      }
      // D(n) = {n} | { & (p = in[n]) D(p) }
      temp.set(n);

      if( pos[n] != temp ){// check change
	pos[n] = temp;
	change = true;
      }
    }
  }
}

// ctrl equivalence
void PosDom::equivalence(){
  for( int bb = 0; bb < bb_size; bb ++ ){
    for( int i = 0; i < bb_size; i ++ ){
      if( !dom[i][bb] ){
	pos[bb].reset(i);
      }
    }
  }
}

// print output file
void PosDom::print(ofstream &fout){
  fout << "{" << func << ":" << fname << endl;

  for( int bb = 0; bb < bb_size; bb ++){
    fout << bb << ":";

    for( int i = 0; i < bb_size; i ++){
      if( pos[bb][i] ){
	fout << i << ",";
      }
    }
    fout << ";" << endl;
  }

  fout << "}" << endl;
}


// data
const int PosDom::calc_bb_num(){
  int bb_num = 0;

  for( int bb = 0; bb < bb_size; bb ++){
    bb_num += pos[bb].count() - 1;
  }

  return bb_num;
}
