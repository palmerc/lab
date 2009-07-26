//
// reach.cc
//

#include <iostream>
#include <fstream>
#include "reach.h"

//
// class Reach_Def
//

// constructor
Reach_Def::Reach_Def(Program_Info &program, const int &f){
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
Reach_Def::~Reach_Def(){
  delete[] in;
  delete[] out;
  du_chain.clear();
}

// data_flow equations
void Reach_Def::calc_dataflow_eq(Program_Info &program, Def_Use &du){
  // all B in[B] = 0
  for( int bb = 0; bb < bb_size; bb ++ ){
    in[bb].reset();
  }

  bool change = true;

  while(change){
    change = false;

    for( int bb = 0; bb < bb_size; bb ++ ){
      // construction
      Program_Info::SET pred = program.get_pred(Func_Bb(func, bb));
      BITSET gen_bb = du.get_gen(bb);
      BITSET kill_bb = du.get_kill(bb);
      BITSET old_out;

      // in[B] = out(p0) | out(p1) | ... | out(px)
      if( !pred.empty() ){
	for( Program_Info::SI set_i = pred.begin();
	     set_i != pred.end(); set_i ++ ){
	  in[bb] |= out[*set_i];
	}
      }
      // oldout = out[B]
      old_out = out[bb];
      // out[B] = in[B] - kill[B] U out[B]
      out[bb] = ( in[bb] ^ ( in[bb] & kill_bb ) ) | gen_bb;

      // check change
      if( old_out != out[bb] ){
	change = true;
      }
    }
  }
}

// make du_chain
void Reach_Def::make_du_chain(Program_Info &program, Def_Use &du){
  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    Func_Bb fbb(func, bb);
    const int start_id = du.get_id( program.get_info(fbb).start );
    const int end_id = du.get_id( program.get_info(fbb).end );

    for( int use_id = end_id; use_id >= start_id; use_id -- ){// LOOP id
      DU_Data use_data = du.get_data(use_id);
      DU_Data::SET use = use_data.get_use();

      // jal, jalr命令
      if( use_data.get_jal() ){
	for( int id = start_id; id < use_id; id ++ ){// LOOP id
	  DU_Data def_data = du.get_data(id);
	  int def_reg = def_data.get_def();
	  Chain chain( def_data.get_pc(), def_reg );

	  // 関数の引数の依存
	  if( ra0 <= def_reg && def_reg < (ra0 << 2) ){
	    du_chain.insert( make_pair(use_data.get_pc(), chain) );
	  }
	}// LOOP id
	continue;
      }

      // ソースレジスタ毎に解析
      for( DU_Data::SI set_i = use.begin(); set_i != use.end(); set_i ++ ){//
	const int use_reg = *set_i;
	int id;

	// 基本ブロック内で定義されているsrc reg
	for( id = use_id - 1; id >= start_id; id -- ){// LOOP id
	  DU_Data def_data = du.get_data(id);
	  int def_reg = def_data.get_def();
	  Chain chain( def_data.get_pc(), def_reg );

	  if( def_reg == 0 ){
	    continue;
	  }else if( use_reg == def_reg ){
	    du_chain.insert( make_pair(use_data.get_pc(), chain) );
	    break;
	  }
	}// LOOP id
	if( id >= start_id ){
	  continue;
	}

	/*
	// 直前の基本ブロックの最後の命令がjal,jalrの場合
	// 関数の戻り値の可能性あり
	if( start_id != 0 && rv0 <= use_reg && use_reg < (rv0 << 2) ){
	  const int id = start_id - 1;
	  DU_Data def_data = du.get_data(id);

	  // jal, jalr命令
	  if( def_data.get_jal() ){
	    Chain chain( def_data.get_pc(), use_reg );

	    du_chain.insert( make_pair(use_data.get_pc(), chain) );
	    continue;
	  }
	}
	*/

	// 基本ブロック外で定義されているレジスタ
	Def_Use::SET def_id = du.get_def_id(use_reg);

	for( Def_Use::SI set_i = def_id.begin();
	     set_i != def_id.end(); set_i ++ ){// LOOP def_id
	  const int id = *set_i;

	  if( in[bb][id] ){
	    DU_Data def_data = du.get_data(id);
	    Chain chain( def_data.get_pc(), def_data.get_def() );

	    du_chain.insert( make_pair(use_data.get_pc(), chain) );
	  }
	}// LOOP def_id
      }//
    }// LOOP id
  }// LOOP bb
}

// file out
void Reach_Def::print(ofstream &fout){
  fout << "{" << func << ":" << fname << endl;

  for( MI map_i = du_chain.begin(); map_i != du_chain.end(); map_i ++ ){
    fout << hex << map_i->second.get_pc() << "->" << map_i->first << "("
	 << dec << map_i->second.get_reg() << ")" << endl;
  }

  fout << "}" << endl;
}
