//
// bb.cc
//
//  Time-stamp: <04/01/26 14:56:30 nakajima>
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "bb.h"

#include "main.h"
#include "print.h"
#include "re.h"
#include "sim.h"

Program_Info program;
PosDom pd;
Bb_Statistic bb_statistic;

Bb_Statistic::Bb_Statistic() {
  // NULL
  func = 0;
  bb = 0;
  inst = 0;
}

Bb_Statistic::~Bb_Statistic(){
  if( func ){
    delete[] func;
  }
  if( bb ){
    for( int f = func_size - 1; f >= 0 ; f -- ){
      delete[] bb[f];
    }
    delete[] bb;
  }
  if( inst ){
    for( int f = func_size - 1; f >= 0 ; f -- ){
      delete[] inst[f];
    }
    delete[] inst;
  }
}

void Bb_Statistic::allocate(){
  func_size = program.size();

  try{
    func = new int[func_size];
    bb = new int*[func_size];
    inst = new int*[func_size];
  }
  catch( std::bad_alloc ){
    error("Bb_Statistic::allocate() bad_alloc");
  }

  for( int f = 0; f < func_size; f ++ ){
    func[f] = 0;

    try{
      bb[f] = new int[program.size(f)];
      inst[f] = new int[program.size(f)];
    }
    catch( std::bad_alloc ){
      error("Bb_Statistic::allocate() bad_alloc");
    }

    for( int b = 0; b < program.size(f); b ++ ){
      bb[f][b] = inst[f][b] = 0;
    }
  }
}


void Bb_Statistic::print(){
  int total_func = 0, total_inst = 0;
  int func_inst[func_size];

  // total
  for( int f = 0; f <func_size; f ++ ){
    func_inst[f] = 0;

    if( func[f] ){
      total_func += func[f];
    }

    for( int b = 0; b < program.size(f); b ++ ){
      func_inst[f] += inst[f][b];
    }

    total_inst += func_inst[f];
  }

  // print
  std::string file = model.bb_statistic;

  std::cout << "# Bb_Statistic::print() "
       << file.substr(file.rfind("/") + 1, std::string::npos) << std::endl;

  std::ofstream fout(file.c_str());

  if( !fout ){
    error("Mem_Profile::file_write() can't open " + model.statistic);
  }

  fout << "total inst: " << total_inst << std::endl
       << "total func: " << total_func << std::endl
       << std::endl;

  fout << "====== func statistic ======" << std::endl;

  for( int f = 0; f < func_size; f ++ ){
    fout << f << "(" << program.funcname(f) << ")" << std::endl;

    fout << "\tinst: " << (double)func_inst[f] / (double)total_inst * 100
	 << " %\t\tfunc_inst: " << func_inst[f] << std::endl;
  }

  fout << std::endl << "====== inst statistic ======" << std::endl;

  for( int f = 0; f < func_size; f ++ ){
    fout << f << "(" << program.funcname(f) << ")" << std::endl;

    fout << "\tfunc: " << (double)func[f] / (double)total_func * 100
	 << " %\t\tcall: " << func[f] << std::endl;
  }
}

//
// class Program_Info
//

// constructor
Program_Info::Program_Info(){
  // NULL
  info = 0;
  bb_size = 0;
  fname = 0;

  fbb_last = Func_Bb();
  fbb_now = Func_Bb();
}

// destructor
Program_Info::~Program_Info(){
  if( info ){
    for( int f = size() - 1; f >= 0; f -- ){
      delete[] info[f];
    }
    delete[] info;
  }
  if( bb_size ){
    delete[] bb_size;
  }
  if( fname ){
    delete[] fname;
  }
}

// change fbb check and search fbb_now
const bool Program_Info::change_fbb(const int &pc, const int &tc){
  const int func = fbb_now.func, bb = fbb_now.bb;

  if( pc < info[func][bb].start || info[func][bb].end < pc
      || c.last_branch_tc(tc) ){
    // change fbb_now
    fbb_last = fbb_now;

    // func change
    if( pc < info[func][0].start || info[func][size(func) - 1].end < pc
	|| c.last_call_tc(tc) || c.last_return_tc(tc) ){
      fbb_now.func = search_func(pc);

      if( model.debug(5) ){
	std::cerr << "================================" << std::endl
	     << "function change last func: " << func
	     << " " << fname[func]
	     << ", current func: " << fbb_now.func
	     << " " << fname[fbb_now.func] << std::endl;
      }
    }

    // bb change
    fbb_now.bb = search_bb(fbb_now.func, pc);

    if( model.debug(5) ){
      std::cerr << "----------------------------------" << std::endl
	   << "basic block change last bb:" << bb
	   << ", current bb:" << fbb_now.bb << std::endl;
    }
    return true;
  }

  return false;
}

// Search current function
const int Program_Info::search_func(const int &pc){
  int max = size(), result = 0;

  for( int min = max >> 1; min != 0; min >>= 1 ){
    int temp = result + min - 1, last_bb = bb_size[temp] - 1;

    if( info[temp][last_bb].end < pc ){
      result += min;
      min = max - result;
    }else{
      max = result + min;
    }
  }

  return result;
}

// Search current bb
const int Program_Info::search_bb(const int &func, const int &pc){
  int result = 0, max = size(func);

  for( int min = max >> 1; min != 0; min >>= 1 ){
    if( info[func][result + min - 1].end < pc ){
      result += min;
      min = max - result;
    }else{
      max = result + min;
    }
  }

  return result;
}

// read bb_info
void Program_Info::file_read(){
  // construct
  std::vector< std::vector< Bb_Info > > v_info;
  std::vector< std::string > v_fname;
  std::ifstream fin(model.bb_info.c_str());

  if( !fin ){
    error("Program_Info::file_read() can't open " + model.bb_info);
  }

  std::string buf;

  getline(fin, buf);
  for( int func = 0; true; func ++ ){// LOOP FUNC
    if( buf[0] != '{' ){
      error("Program_Info::file_read() {");
    }else{
      v_fname.push_back( buf.substr(buf.find(":") + 1, std::string::npos) );
    }

    // resize array for new function
    v_info.resize(func + 1);

    for( int bb = 0; ;bb ++ ){// LOOP BB
      getline(fin, buf);
      if( buf ==  "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(";") == std::string::npos ){
	std::cerr << func << "," << bb << " " << buf << std::endl;
	error("Program_Info::file_read() file error");
      }

      // construct
      Bb_Info bb_info;

      buf.erase(0, buf.find(":") + 1);
      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &bb_info.start );
      buf.erase(0, buf.find(":") + 1);
      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &bb_info.end );

      // insert bb_info
      v_info[func].push_back(bb_info);
    }
    getline(fin, buf);

    if( fin.eof() ){
      break;
    }
  }

  // init
  func_size = v_info.size();
  try{
    info = new Bb_Info*[func_size];
    bb_size = new int[func_size];
    fname = new std::string[func_size];
  }
  catch( std::bad_alloc ){
    error("Program_Info::file_read() bad_alloc");
  }

  for( int f = 0; f < size(); f ++ ){
    bb_size[f] = v_info[f].size();
    fname[f] = v_fname[f];

    try{
      info[f] = new Bb_Info[size(f)];
    }
    catch( std::bad_alloc ){
      error("Program_Info::file_read() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      info[f][b] = v_info[f][b];
    }
  }

  fbb_last = Func_Bb();
  fbb_now = Func_Bb();

  std::cout << "# Program_Info::file_read() init end" << std::endl;
}

// backup (for reexec mode)
void Program_Info::backup(){
  fbb_last_bak = fbb_last;
}

// restore (reexec mode)
void Program_Info::restore(){
  fbb_now = fbb_last_bak;
}

// check code
void Program_Info::print(){
  std::cout << "Program_Info::print()" << std::endl;

  for( int f = 0; f < size() ; f ++ ){
    std::cout << "{" << std::endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){
      std::cout << bb << ":" <<  info[f][bb].start
	   << ":" << info[f][bb].end << std::endl;
    }
    std::cout << "}" << std::endl;
  }
  exit(0);
}


//
// class PosDom use posdom/ctrl_eq info
//

// constructor
PosDom::PosDom(){
  pd = 0;// NULL
}

// destructor
PosDom::~PosDom(){
  if( pd ){
    for( int func = func_size - 1; func >= 0; func -- ){
      delete[] pd[func];
    }
    delete[] pd;
  }
}

// read file post dominate or ctrl eq info
void PosDom::file_read(){
  func_size = program.size();

  std::string fin_fname;

  switch( sim_type ){
  case CD:
  case SP_CD:
  case CD_MF:
  case PD:
    fin_fname = model.posdom_out;
    std::cout << "# PosDom::file_read() (post dominate) ";
    break;

  case CE:
    fin_fname = model.ctrleq_out;
    std::cout << "# PosDom::file_read() (ctrl eval) ";
    break;

  default:
    std::cout << "# PosDom::file_read() no read posdom" << std::endl;
    return;
  }

  std::ifstream fin(fin_fname.c_str());

  if( !fin ){
    error("PosDom::file_read() can't open " + fin_fname);
  }

  try{
    pd = new BITSET*[func_size];
  }
  catch( std::bad_alloc ){
    error("PosDom::file_read() bad_alloc pd = new");
  }

  std::string buf;

  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    const std::string fname = program.funcname(func);
    const int bb_size = program.size(func);

    getline(fin, buf);

    if( buf.find("{") == std::string::npos ){// function name
      error("PosDom::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcnaem " << fname << ", posdom funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("PosDom::file_read() funcname");
    }

    try{
      pd[func] = new BITSET[bb_size];
    }
    catch( std::bad_alloc ){
      error("PosDom::file_read() bad_alloc pd[func]");
    }

    for( int bb = 0; bb < bb_size; bb ++ ){// LOOP BB
      getline(fin, buf);

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos 
	  || buf.find(";") == std::string::npos ){
	std::cerr << func << "," << bb << " " << buf << std::endl;
	error("PosDom::file_read() file error");
      }

      buf.erase(0, buf.find(":") + 1);

      pd[func][bb].reset();

      while( buf != ";" ){
	const int val = atoi( buf.substr(0, buf.find(",")).c_str()  );

	pd[func][bb].set(val);
	buf.erase(0, buf.find(",") + 1);
      }
    }// LOOP BB
    getline(fin, buf);

    if( buf != "}" ){
      error("PosDom::file_read() }");
    }

    if( fin.eof() ){
      break;
    }
  }// LOOP FUNC

  std::cout << "init end" << std::endl;
}

// chech code
void PosDom::print(){
  std::cout << "PosDom::print() test" << std::endl;

  for( int func = 0; func < program.size(); func ++ ){
    std::cout << "{" << func << std::endl;
    for( int bb = 0; bb < program.size(func); bb ++){
      std::cout  << pd[func][bb] << std::endl;
    }
    std::cout << "}" << std::endl;
  }
  exit(0);
}
