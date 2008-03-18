//
// data.cc
//

#include <iostream>
#include <fstream>
#include "data.h"

#include "main.h"

//
// class Mem_Region
//

Mem_Region::Mem_Region(Program_Info &program){
  max_global_region = min_global_region;
  min_stack_region = max_stack_region;
  max_heap_region = min_heap_region;

  func_size = program.size();

  try{
    load_region = new MAP[func_size];
    store_region = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Value_Predict::Value_Predict() bad_alloc");
  }
}

Mem_Region::~Mem_Region(){
  if( load_region ){
    for( int func = 0; func < func_size; func ++ ){
      load_region[func].clear();
    }
    delete[] load_region;
  }

  if( store_region ){
    for( int func = 0; func < func_size; func ++ ){
      store_region[func].clear();
    }
    delete[] store_region;
  }
}

Mem_Region::Mem_Address::Mem_Address(const Mem_Region_Type &type = None){
  reg_type = type;
  region_type = None;
  freq = 0;
}

void Mem_Region::Mem_Address::add_region(const Mem_Region_Type &type){
  if( reg_type == None ){
    error("Mem_Address::add_region() reg_type == None");
  }else{
    region_type = Mem_Region_Type( region_type | type );
    freq ++;
  }
}

void Mem_Region::Mem_Address::print_result(){
  std::cout << freq << "," << region_type << std::endl;
}

void Mem_Region::file_write(Program_Info &program){
  std::ofstream fout(model.mem_region.c_str());

  if( !fout ){
    error("mem_Region::file_write() can't open " + model.mem_region);
  }

  // print load region
  for( int func = 0; func < func_size; func ++ ){// LOOP func
    fout << "{" << func << ":" << program.funcname(func) << std::endl;

    fout << "}" << std::endl;
  }// LOOP func

  // print store region
  for( int func = 0; func < func_size; func ++ ){// LOOP func
    fout << "{" << func << ":" << program.funcname(func) << std::endl;

    fout << "}" << std::endl;
  }// LOOP func

  std::cout << "Mem_Region::file_write() end" << std::endl;
}


void Mem_Region::check_load_region(const Pipe_Inst &inst,
				   Program_Info & program){
  const int pc = inst.pc, address = inst.address;
  const int func = program.search_func(pc);

  if( !load_region[func].count(pc) ){
    // LOAD [srcA and srcB:address]
    const int srcA = inst.s_A, srcB = inst.s_B;

    if( srcA == SP_REG || srcB == SP_REG ){
      // sp
      load_region[func].insert( std::make_pair(pc, Mem_Address(Stack)) );
    }else if( srcA == GP_REG || srcB == GP_REG ){
      // gp
      load_region[func].insert( std::make_pair(pc, Mem_Address(Global)) );
    }else{
      // other
      load_region[func].insert( std::make_pair(pc, Mem_Address(Heap)) );
    }
  }

  // check access region
  Mem_Region_Type region_type = None;

  if( min_global_region <= address && address <  min_stack_region ){
    // global (read only)
    region_type = R_Only;
    max_global_region = std::max(max_global_region, address);
  }else if( min_stack_region <= address && address <= max_stack_region ){
    region_type = Stack;
    min_stack_region = std::min(min_stack_region, address);
  }else if( min_heap_region <= address && address <= max_heap_region ){
    region_type = Heap;
    max_heap_region = std::max(max_heap_region, address);
  }

  load_region[func][pc].add_region(region_type);
}

void Mem_Region::check_store_region(const Pipe_Inst &inst,
				   Program_Info & program){
  const int pc = inst.pc, address = inst.address;
  const int func = program.search_func(pc);

  if( !store_region[func].count(pc) ){
    // STORE [srcA:data] [srcB,srcC:address]
    const int srcB = inst.s_B, srcC = inst.s_C;

    if( srcB == SP_REG || srcC == SP_REG ){
      // sp
      store_region[func].insert( std::make_pair(pc, Mem_Address(Stack)) );
    }else if( srcB == GP_REG || srcC == GP_REG ){
      // gp
      store_region[func].insert( std::make_pair(pc, Mem_Address(Global)) );
    }else{
      // other
      store_region[func].insert( std::make_pair(pc, Mem_Address(Heap)) );
    }
  }

  // check access region
  Mem_Region_Type region_type = None;

  if( min_global_region <= address && address <  min_stack_region ){
    region_type = Global;
    max_global_region = std::max(max_global_region, address);
  }else if( min_stack_region <= address && address <= max_stack_region ){
    region_type = Stack;
    min_stack_region = std::min(min_stack_region, address);
  }else if( min_heap_region <= address && address <= max_heap_region ){
    region_type = Heap;
    max_heap_region = std::max(max_heap_region, address);
  }

  store_region[func][pc].add_region(region_type);
}


void Mem_Region::check_region(const Pipe_Inst &inst, Program_Info & program){
  switch( inst.func ){
  case LOAD:
    check_load_region(inst, program);
    break;

  case STORE:
    check_store_region(inst, program);
    break;

  default:
    break;
  }
}
