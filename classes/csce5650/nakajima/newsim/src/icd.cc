//
// icd.cc
//
//  Time-stamp: <04/01/13 16:36:06 nakajima>
//

#include <iostream>
#include <fstream>
#include "icd.h"

#include "fork.h"
#include "main.h"
#include "print.h"
#include "sim.h"

ICD icd;

//
// class ICD (calculate register send time [determin/speculative])
//

ICD::ICD(){
  // NULL
  icd = 0;
}

// destructor
ICD::~ICD(){
  if( icd ){
    for( int func = func_size - 1; func >= 0; func -- ){
      delete[] icd[func];
    }
    delete[] icd;
  }
}

// indirect control dependence
void ICD::analysis(const Func_Bb &fbb){
  if( !model.icd ){
    return;
  }

  if( model.debug(10) ){
    std::cerr << "icd: ";
    for( int i = 0; i < icd_size; i ++ ){
      std::cerr << "[" << i << "]" << reg_icd[i];
    }
    std::cerr << std::endl;

    std::cerr << "reg: ";
    for( int r = 0; r < REG; r ++ ){
      if( reg[r].write ){
	std::cerr << "[" << r << "]" << reg[r].write;
      }
    }
    std::cerr << std::endl;
  }

  for( int reg_num = 1; reg_num < REG; reg_num ++ ){// LOOP REG
    if( reg[reg_num].flag & DEFINE ){
      continue;
    }

    int reach_limit = -1;

    if( !icd[fbb.func][fbb.bb].empty() ){
      // 到達定義
      reach_limit = reach(fbb, reg_num);
    }

    if( !(model.sp_exec & SP_Send) ){
      // 確定send
      reach_limit = std::max(reach_limit, reg[reg_num].commit);
    }

    reg[reg_num].write = std::max(reg[reg_num].write, reach_limit);
  }// LOOP REG

  if( model.debug(10) ){
    std::cerr << "reg: ";
    for( int r = 0; r < REG; r ++ ){
      if( reg[r].write ){
	std::cerr << "[" << r << "]" << reg[r].write;
      }
    }
    std::cerr << std::endl;
  }
}

// icd JR RETURN(v0) JALL CALL(a0)
void ICD::depend(const Func_Bb &fbb, const int &rva){
  if( !model.icd ){
    return;
  }

  for( int reg_num = rva; reg_num < (rva * 2); reg_num ++ ){// LOOP REG
    int reach_limit = -1;

    if( !icd[fbb.func][fbb.bb].empty() ){
      reach_limit = reach(fbb, reg_num);
    }

    reg_icd[reg_num] = std::max(reg_icd[reg_num], reach_limit);
  }// LOOP REG
}

// calc reach limit
const int ICD::reach(const Func_Bb &fbb, const int &r){
  MMI_PAIR map_er = icd[fbb.func][fbb.bb].equal_range(r);
  int reach_limit = -1;

  for( MMI map_i = map_er.first; map_i != map_er.second; map_i ++ ){
    Func_Bb fbb(fbb.func, map_i->second);
    Func_Stack::Bb_Data target_data = func_data.data_read(fbb);

    if( target_data.order_tc > reg[r].def_tc ){
      reach_limit = std::max(reach_limit, target_data.send_time);
    }
  }

  return reach_limit;
}

// change thread
void ICD::init_thread(){
  if( !model.icd ){
    return;
  }

  for( int reg_num = RET_REG; reg_num < icd_size; reg_num ++ ){
    reg[reg_num].write = std::max(reg[reg_num].write, reg_icd[reg_num]);
  }

  for( int i = 0; i < icd_size; i ++ ){
    reg_icd[i] = 0;
  }
}

// backup (for reexec mode)
void ICD::backup(){
  if( model.icd ){
    for( int i = 0; i < icd_size; i ++ ){
      reg_icd_bak[i] = reg_icd[i];
    }
  }
}

// restore (for reexec mode)
void ICD::restore(){
  if( model.icd ){
    for( int i = 0; i < icd_size; i ++ ){
      reg_icd[i] = reg_icd_bak[i];
    }
  }
}

// read icd info
void ICD::file_read(){
  if( !model.icd ){
    return;
  }

  std::ifstream fin(model.icd_info.c_str());

  if( !fin ){
    error("ICD::file_read() can't open " + model.icd_info);
  }

  func_size = program.size();

  try{
    icd = new MMAP*[func_size];
  }
  catch( std::bad_alloc ){
    error("ICD::file_read() bad_alloc");
  }

  std::string buf;

  getline(fin, buf);
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    const std::string fname = program.funcname(func);
    const int bb_size = program.size(func);

    if( buf.find("{") == std::string::npos ){// function name
      error("ICD::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcname " << fname << ", icd_info funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("ICD::file_read() funcname");
    }

    try{
      icd[func] = new MMAP[bb_size];
    }
    catch( std::bad_alloc ){
      error("ICD::file_read() bad_alloc");
    }

    while(true){// LOOP icd
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos
	  || buf.find(";") == std::string::npos ){
	std::cerr << func << " " << buf << std::endl;
	error("ICD::file_read() file error");
      }

      int bb, reg;

      bb = atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);
      reg = atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      while( buf != ";" ){
	const int val = atoi( buf.substr(0, buf.find(",")).c_str()  );

	icd[func][bb].insert( std::make_pair(reg, val) );
	buf.erase(0, buf.find(",") + 1);
      }
    }// LOOP icd
    getline(fin, buf);

    if( fin.eof() ){
      break;
    }
  }// LOOP FUNC

  std::cout << "# ICD::file_read() init end" << std::endl;
}

// check code
void ICD::print(){
  std::cout << "ICD::print() test" << std::endl;

  for( int f = 0; f < program.size(); f++ ){// LOOP FUNC
    std::cout << "{" << f << std::endl;
    for( int bb = 0; bb < program.size(f); bb ++ ){// LOOP BB
      if( icd[f][bb].empty() ){
	continue;
      }

      std::cout <<  bb << "# ";
      for( int r = 0; r < REG; r ++ ){// LOOP REG
	MMI_PAIR map_er = icd[f][bb].equal_range(r);

	if( map_er.first == map_er.second ){
	  continue;
	}

	std::cout <<  r <<  "[";
	for( MMI map_i = map_er.first; map_i != map_er.second; map_i ++ ){
	  std::cout << map_i->second << ",";
	}
	std::cout << "] ";
      }// LOOP REG
      std::cout << std::endl;
    }// LOOP BB
    std::cout << "}" << std::endl;
  }// LOOP FUNC
  exit(0);
}
