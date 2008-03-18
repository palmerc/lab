//
// bb.cc
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "bb.h"

#include "main.h"

//
// class Program_Info
//

// constructor
Program_Info::Program_Info(){
  // construct
  std::vector< std::vector< Bb_Info > > v_info;
  std::vector< std::string > v_fname;
  std::ifstream fin(model.bb_info.c_str());

  if( !fin ){
    error("Program_Info::Program_Info() can't open " + model.bb_info);
  }

  std::string buf;

  getline(fin, buf);
  for( int func = 0; true; func ++ ){// LOOP FUNC
    if( buf[0] != '{' ){
      error("Program_Info::Program_Info() {");
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
	error("Program_Info::Program_Info() file error");
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
    info = new Bb_Info*[size()];
    bb_size = new int[size()];
    fname = new std::string[size()];
  }
  catch( std::bad_alloc ){
    error("Program_Info::Program_Info() bad_alloc");
  }

  for( int f = 0; f < size(); f ++ ){
    bb_size[f] = v_info[f].size();
    fname[f] = v_fname[f];

    try{
      info[f] = new Bb_Info[size(f)];
    }
    catch( std::bad_alloc ){
      error("Program_Info::Program_Info() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      info[f][b] = v_info[f][b];
    }
  }

  std::cerr << "Program_Info::Program_Info() init end" << std::endl;
}

// destructor
Program_Info::~Program_Info(){
  for( int f = size() - 1; f >= 0; f -- ){
    delete[] info[f];
  }
  delete[] info;
  delete[] bb_size;
  delete[] fname;
}

// Search current function
const int Program_Info::search_func(const int &pc){
  int min, max = size(), result = 0;

  for( min = max >> 1; min != 0; min >>= 1 ){
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
  int result = 0, max = size(func), min;

  for( min = max >> 1; min != 0; min >>= 1 ){
    if( info[func][result + min - 1].end < pc ){
      result += min;
      min = max - result;
    }else{
      max = result + min;
    }
  }
  return result;
}

// check code
void Program_Info::print(){
  std::cout << "Program_Info::file_read()" << std::endl;

  for( int f = 0; f < size() ; f ++ ){// LOOP FUNC
    std::cout << "{" << std::endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){// LOOP BB
      // start/end
      std::cout << bb << std::hex << ":" <<  info[f][bb].start
	   << ":" << info[f][bb].end << std::dec << std::endl;
    }// LOOP BB
    std::cout << "}" << std::endl;
  }// LOOP FUNC

  exit(0);
}
