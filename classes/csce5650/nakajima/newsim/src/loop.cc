//
// loop.cc
//
//  Time-stamp: <04/02/08 15:45:50 nakajima>
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include "loop.h"

#include "fork.h"
#include "print.h"
#include "re.h"
#include "sim.h"

Loop lp;

//
// class Loop
//

// constructor
Loop::Loop(){
  // NULL
  header_bb = 0;
  exit_pcs = 0;
  const_pcs = 0;
  induct_pcs = 0;
  loop_c = 0;

  // counter
  unroll_c = induct_c = const_c = 0;
}

// destructor
Loop::~Loop(){
  if( header_bb ){
    delete[] header_bb;
  }
  if( exit_pcs ){
    delete[] exit_pcs;
  }
  if( const_pcs ){
    delete[] const_pcs;
  }
  if( induct_pcs ){
    delete[] induct_pcs;
  }

  if( loop_c ){
    for( int f = func_size - 1; f >= 0; f -- ){
      delete[] loop_c[f];
    }
    delete[] loop_c;
  }
}

// ignore/skip pc check
Skip Loop::flag_check(const Pipe_Inst &inst){
  const int pc = inst.pc, func = program.fbb().func;
  MI map_i;

  Skip skip_flag = NOT_FOUND;

  if( inst.func == BRANCH && inst.brn == CON_DIRE ){
    if( exit_pcs[func].count(pc) ){
      if( model.debug(10) ){
	std::cerr << "exit_pcs[" << func<< "].count("
	     << std::hex << pc << std::dec << ") TRUE" << std::endl;
      }

      skip_flag = EXIT_BRANCH;
    }
  }else{
    if( (map_i = const_pcs[func].find(pc)) != const_pcs[func].end() ){
      const int loop_head = map_i->second;

      if( func_data.not_zero(Func_Bb(func, loop_head)) ){
	if( model.debug(10) ){
	  std::cerr << "const_pcs[" << func<< "].count("
	       << std::hex << pc << std::dec << ") TRUE" << std::endl;
	}

	skip_flag = CONST_PC;
      }else{
	if( model.debug(10) ){
	  std::cerr << "const_pcs[" << func<< "].count("
	       << std::hex << pc << std::dec << ") FALSE" << std::endl;
	}

	skip_flag =  CONST_CAND;
      }
    }else if( (map_i = induct_pcs[func].find(pc)) != induct_pcs[func].end() ){
      const int loop_head = map_i->second;

      if( func_data.not_zero(Func_Bb(func, loop_head)) ){
	if( model.debug(10) ){
	  std::cerr <<  "induct_pcs[" << func<< "].count("
	       << std::hex << pc << std::dec << ") TRUE" << std::endl;
	}

        skip_flag = INDUCT_PC;
      }else{
	if( model.debug(10) ){
	  std::cerr <<  "induct_pcs[" << func<< "].count("
	       << std::hex << pc << std::dec << ") FALSE" << std::endl;
	}

        skip_flag = INDUCT_CAND;
      }
    }
  }

  return skip_flag;
}

void Loop::count_unroll(){
  if( !model.reexec || reexec.mode() == Finish || reexec.mode() == Normal ){
    unroll_c ++;
  }
}

void Loop::count_loop_inst(const Skip & skip_flag){
  switch( skip_flag ){
  case INDUCT_PC:
    if( reexec.mode() == Normal ){
      induct_c ++;
    }
    if( model.debug(5) ){
      std::cerr<< "   *** induction pc ***" << std::endl;
    }
    break;

  case INDUCT_CAND:
    if( model.debug(5) ){
      std::cerr << "   *** induction candidate ***" << std::endl;
    }
    break;

  case CONST_PC:
    if( model.debug(5) ){
      std::cerr << "   *** constant pc ***" << std::endl;
    }
    if( reexec.mode() == Normal ){
      const_c ++;
    }
    break;

  case CONST_CAND:
    if( model.debug(5) ){
      std::cerr << "   *** constant candidate ***" << std::endl;
    }
    break;

  default:// not found
    break;
  }
}

// read loop_del_pc
void Loop::file_read(){
  std::ifstream fin(model.loop_del_pc.c_str());

  if( !fin ){
    error("Loop::file_read() can't open " + model.loop_del_pc);
  }

  func_size = program.size();

  try{
    exit_pcs = new MAP[func_size];
    const_pcs = new MAP[func_size];
    induct_pcs = new MAP[func_size];
    header_bb = new SET[func_size];
  }
  catch( std::bad_alloc ){
    error("Loop::file_read() bad_alloc");
  }

  std::string buf;

  getline(fin, buf);
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    std::string fname = program.funcname(func);

    if( buf.find("{") == std::string::npos ){// function name
      error("Loop::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcname " << fname << ", loop_del_pcs funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("Loop::file_read() funcname");
    }

    while(true){// LOOP LINE NUM
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      // this loop header does not include constant/induct variable
      if( buf.find(":;") != std::string::npos ){
	continue;
      }

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos
	  || buf.find(";") == std::string::npos ){
	std::cerr << func << " " << buf << std::endl;
	error("Loop::file_read() file format error");
      }

      int loop_header;

      loop_header = atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      header_bb[func].insert(loop_header);

      // constant/inducion variable
      while( buf != ";" ){
	int pc;

	if( buf[0] == 'E' ){
	  sscanf( buf.substr(1, buf.find(",")).c_str(), "%x", &pc );
	  exit_pcs[func].insert( std::make_pair(pc, loop_header) );
	  buf.erase(0, buf.find(",") + 1);
	}else if( buf[0] == 'C' ){
    	  sscanf( buf.substr(1, buf.find(",")).c_str(), "%x", &pc );
    	  const_pcs[func].insert( std::make_pair(pc, loop_header) );
	  buf.erase(0, buf.find(",") + 1);
	}else if( buf[0] == 'B' ){
	  sscanf( buf.substr(1, buf.find(",")).c_str(), "%x", &pc );
	  induct_pcs[func].insert( std::make_pair(pc, loop_header) );
	  buf.erase(0, buf.find(",") + 1);
	}else{
	  std::cerr << func << " " << buf << std::endl;
	  error("Loop::file_read() format error");
	}
      }
    }// LOOP LINE NUM

    getline(fin, buf);
    if( fin.eof() ){
      break;
    }
  }// LOOP FUNC

  // ループの平均繰り返し回数を計算する準備
  // (Func_Stack::analysis_no_CDの場合のみ計算する、sim_typeが oracle, sp等)
  if( !(sim_type & CD) ){
    allocate_loop_c();
  }

  std::cout << "# Loop::file_read() init end" << std::endl;
}

//
// ループの平均繰り返し回数
//

void Loop::allocate_loop_c(){
  try{
    loop_c = new Loop_Count*[func_size];
  }
  catch( std::bad_alloc ){
    error("Loop::allocate() bad_alloc");
  }

  for( int func = 0; func < func_size; func ++ ){
    const int bb_size = program.size(func);

    try{
      loop_c[func] = new Loop_Count[bb_size];
    }
    catch( std::bad_alloc ){
      error("Loop::allocate() bad_alloc");
    }
  }

  std::cout << "# Loop::allocate_loop_c() init end" << std::endl;
}

void Loop::count_bedge(const Func_Bb &fbb){
  loop_c[fbb.func][fbb.bb].bedge();
}

void Loop::count_header(const Func_Bb &fbb){
  loop_c[fbb.func][fbb.bb].header();
}

void Loop::print_count_loop(){
  // 重み付けをしない場合
  double sum_ave = 0.0, max_loop_ave = 0.0;
  int static_loop = 0;
  // ループ実行回数で重み付け
  int total_exec = 0, total_loop = 0;

  for( int func = 0; func < func_size; func ++ ){
    if( program.funcname(func) == "__do_global_dtors" ){
      if( static_loop ){
	std::cout << "====" << std::endl
	     << " dynamic loop num   : " << sum_ave / (double)static_loop
	     << " (sum_ave/num: " << sum_ave
	     << "/" << static_loop << ")" << std::endl
	     << " loop num (weighted): "
	     << (double)total_loop / (double)total_exec << std::endl
	     << "max loop ave: " << max_loop_ave << std::endl
	     << "====" << std::endl;
      }
    }

    for( int bb = 0; bb < program.size(func); bb ++ ){
      if( loop_c[func][bb].check() ){
	if( model.statistic ){
	  std::cout << "#fbb: " << func << "/" << bb << " -> ";
	  loop_c[func][bb].print();
	}

	sum_ave += loop_c[func][bb].calc_ave();
	static_loop ++;

	max_loop_ave = std::max(max_loop_ave, loop_c[func][bb].calc_ave());

	total_exec += loop_c[func][bb].exec();
	total_loop += loop_c[func][bb].loop();
      }
    }
  }

  if( static_loop ){
    std::cout << " dynamic loop num   : " << sum_ave / (double)static_loop
	 << " (sum_ave/num: " << sum_ave << "/" << static_loop << ")" << std::endl
	 << " loop num (weighted): " << (double)total_loop / (double)total_exec
	 << std::endl;
  }else{
    std::cout << " dynamic loop num   : NONE" << std::endl;
  }
}


// check code
void Loop::print(){
  // define
  typedef std::multimap< int, int > MMAP;
  typedef MMAP::iterator MMI;
  typedef std::pair< MMI, MMI > MMI_PAIR;

  std::cout << "Loop::print()" << std::endl;

  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    std::cout << "{" << func << std::endl;

    MMAP const_head;
    MMAP induct_head;

    // PAIR( pc, header ) ===> PAIR( header, pcs )
    for( MI map_i = const_pcs[func].begin();
	 map_i != const_pcs[func].end(); map_i ++ ){
      const_head.insert( std::make_pair(map_i->second, map_i->first) );
    }

    for( MI map_i = induct_pcs[func].begin();
	 map_i != induct_pcs[func].end(); map_i ++ ){
      induct_head.insert( std::make_pair(map_i->second, map_i->first) );
    }

    for( int bb = 0; bb < program.size(func); bb ++ ){// LOOP bb
      if( !const_head.count(bb) ){
	continue;
      }

      MMI_PAIR pair = const_head.equal_range(bb);

      std::cout <<  "header " << bb << ":";

      std::cout << "const_pc[";
      for( MMI map_i = pair.first; map_i != pair.second; map_i ++ ){
	std::cout <<  map_i->second << ", ";
      }
      std::cout << "]";

      if( induct_head.count(bb) ){
	MMI_PAIR pair = induct_head.equal_range(bb);

	std::cout << "induct_pc = [";
	for( MMI map_i = pair.first; map_i != pair.second; map_i ++ ){
	  std::cout <<  map_i->second << ", ";
	}
	std::cout << "]";
      }

      std::cout << std::endl;
    }// LOOP bb

    std::cout << "}" << std::endl;
  }// LOOP FUNC

  exit(0);
}
