//
// bb.cc
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "bb.h"

//
// class Program_Info
//

// constructor
Program_Info::Program_Info(){
  // construct
  std::vector< std::vector< Bb_Info > > v_info;
  std::vector< std::vector< SET > > v_succ, v_pred;
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
    v_succ.resize(func + 1);
    v_pred.resize(func + 1);

    for( int bb = 0; ;bb ++ ){// LOOP BB
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(";") == std::string::npos ){
	std::cerr << func << "," << bb << " " << buf << std::endl;
	error("Program_Info::Program_Info() file error");
      }

      // construct
      Bb_Info bb_info;
      SET succ_s, pred_s;

      buf.erase(0, buf.find(":") + 1);
      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &bb_info.start );
      buf.erase(0, buf.find(":") + 1);
      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &bb_info.end );
      buf.erase(0, buf.find(":") + 1);

      if( buf.find("exit") == std::string::npos &&
	  buf.find("break") == std::string::npos ){
	while( buf[0] != ':' ){
	  std::string val = buf.substr(0, buf.find(" "));

	  succ_s.insert( atoi(val.c_str()) );
	  buf.erase(0, buf.find(" ") + 1);
	}
      }

      buf.erase(0, buf.find(":") + 1);

      if( buf.find("IN") == std::string::npos ){
	while( buf != ";" ){
	  std::string val = buf.substr(0, buf.find(" "));

	  pred_s.insert( atoi(val.c_str()) );
	  buf.erase(0, buf.find(" ") + 1);
	}
      }

      // insert bb_info
      v_info[func].push_back(bb_info);
      v_succ[func].push_back(succ_s);
      v_pred[func].push_back(pred_s);
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
    succ = new SET*[size()];
    pred = new SET*[size()];
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
      succ[f] = new SET[size(f)];
      pred[f] = new SET[size(f)];
    }
    catch( std::bad_alloc ){
      error("Program_Info::Program_Info() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      info[f][b] = v_info[f][b];
      succ[f][b] = v_succ[f][b];
      pred[f][b] = v_pred[f][b];
    }
  }

  std::cerr << "Program_Info::Program_Info() init end" << std::endl;
}

// destructor
Program_Info::~Program_Info(){
  for( int f = size() - 1; f >= 0; f -- ){
    delete[] succ[f];
    delete[] pred[f];
    delete[] info[f];
  }
  delete[] succ;
  delete[] pred;
  delete[] info;
  delete[] bb_size;
  delete[] fname;
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

// search backward edge
Program_Info::MMAP Program_Info::backward_edge(const int &func){
  MMAP edge;

  for( int bb = 0; bb < size(func); bb ++ ){
    SET pred = get_pred(Func_Bb(func, bb));

    for( SI set_i = pred.begin(); set_i != pred.end(); set_i ++ ){
      if( bb <= *set_i ){
	// std::make_pair( header, back_edge )
	edge.insert( std::make_pair(bb, *set_i) );
      }
    }
  }

  return edge;
}

// check code
void Program_Info::print(){
  std::cout << "Program_Info::file_read()" << std::endl;

  for( int f = 0; f < size() ; f ++ ){// LOOP FUNC
    std::cout << "{" << std::endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){// LOOP BB
      // start/end
      std::cout << bb << std::hex << ":" <<  info[f][bb].start
	   << ":" << info[f][bb].end << std::dec;

      // succ
      std::cout << bb << ":";
      for( int i = -1; i < size(f); i ++ ){
	if( succ[f][bb].count(i) ){
	  std::cout << i << " ";
	}
      }

      // pred
      std::cout << ":";
      for( int i = 0; i < size(f); i ++ ){
	if( pred[f][bb].count(i) ){
	  std::cout << i << " ";
	}
      }

      std::cout << std::endl;
    }// LOOP BB
    std::cout << "}" << std::endl;
  }// LOOP FUNC

  exit(0);
}

