//
// pred.cc
//

#include <iostream>
#include <fstream>
#include <string>
#include "pred.h"

//
// class Value_Predict
//

Value_Predict::Value_Predict(Program_Info &program){
  func_size = program.size();

  try{
    val_table = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Value_Predict::Value_Predict() bad_alloc");
  }
}


Value_Predict::~Value_Predict(){
  for( int func = 0; func < func_size; func ++ ){
    val_table[func].clear();
  }
  delete[] val_table;
}

void Value_Predict::Value_Data::add_data(const int &v_dest){
  val_data[v_dest] ++;
  total ++;
}

void Value_Predict::Value_Data::file_write(std::ofstream &fout){
  // sort
  typedef std::multimap< int, int, std::greater_equal< int > > MMAP;
  typedef MMAP::iterator MMI;

  MMAP sorted;

  for( MI map_i = val_data.begin(); map_i != val_data.end(); map_i ++ ){// LOOP
    sorted.insert( std::make_pair(map_i->second, map_i->first) );
  }// LOOP val

  fout << total << ":";

  for( MMI map_i = sorted.begin(); map_i != sorted.end(); map_i ++ ){// LOOP
    // print "count,val:"
    fout << map_i->first << "," << std::hex << map_i->second << std::dec << ":";
  }// LOOP val

  fout << ";" << std::endl;
}

// count instruction
void Value_Predict::count_inst(const Pipe_Inst &inst, Program_Info &program){
  switch( inst.func ){
  case OTHER:
  case LOAD:
    if( !inst.s_A && !inst.s_B && !inst.s_C ){
      return;
    }
    break;

  default:
    return;
  }

  const int pc = inst.pc, func = program.search_func(pc);

  // dest reg
  val_table[func][pc].add_data(inst.v_dest);
}

// file out
void Value_Predict::file_write(Program_Info &program){
  std::ofstream fout(model.val_pred.c_str());

  if( !fout ){
    error("Value_Predict::print() can't open " + model.val_pred);
  }

  for( int func = 0; func < func_size; func ++ ){// LOOP func
    fout << "{" << func << ":" << program.funcname(func) << std::endl;

    for( MI map_i = val_table[func].begin();
	 map_i != val_table[func].end(); map_i ++ ){// LOOP pc
      fout << std::hex << map_i->first << std::dec << ":";

      map_i->second.file_write(fout);
    }// LOOP pc

    fout << "}" << std::endl;
  }// LOOP func

  std::cout << "Value_Predict::file_write() end" << std::endl;
}

//
// class Branch_Pre (static branch predector)
//

// constructor
Branch_Predict::Branch_Predict(Program_Info &program){
  func_size = program.size();

  try{
    branch_table = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Branch_Predict::Branch_Predict() bad_alloc");
  }
}

// destructtor
Branch_Predict::~Branch_Predict(){
  for( int func = 0; func < func_size; func ++ ){
    branch_table[func].clear();
  }
  delete[] branch_table;
}

// count branch instruction
void Branch_Predict::count_inst(const Pipe_Inst &inst, Program_Info &program){
  if( inst.func != BRANCH || inst.brn != CON_DIRE ){
    return;
  }

  const int pc = inst.pc, target_pc = inst.address;
  const int func = program.search_func(pc);

  branch_table[func][pc].total_count ++;

  if( target_pc == pc + byte ){
    // target is next pc
    branch_table[func][pc].next_count ++;
  }else{
    branch_table[func][pc].target_pc = target_pc;
  }
}

// print result data
void Branch_Predict::file_write(Program_Info &program){
  std::ofstream fout(model.brn_pred.c_str());

  if( !fout ){
    error("Branch_Predict::file_open() can't open" + model.brn_pred);
  }

  for( int func = 0; func < program.size(); func ++ ){// LOOP func
    fout << "{" << func << ":" << program.funcname(func) << std::endl;

    for( MI map_i = branch_table[func].begin();
	 map_i != branch_table[func].end(); map_i ++ ){// LOOP branch inst
      const int pc = map_i->first;
      Branch_Inst branch = map_i->second;

      fout << std::hex << pc << std::dec << ":";

      if( branch.total_count < (branch.next_count << 1) ){
	// next pc
	fout << std::hex << map_i->first + byte << std::dec << ",NEXT;" << std::endl;
      }else{
	// target pc
	fout << std::hex << map_i->second.target_pc << std::dec << ",TARGET;" << std::endl;
      }
    }// LOOP branch inst

    fout << "}" << std::endl;
  }// LOOP func

  std::cout << "Branch_Predict::file_write() end" << std::endl;
}
