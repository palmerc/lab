//
// bb.cc
//

#include <iostream>
#include <fstream>
#include <vector>
#include "bb.h"

//
// class Program_Info
//

// constructor (read bb_info and allocate)
Program_Info::Program_Info(){
  // construct
  std::vector< std::vector< SET > > v_succ, v_pred;
  std::vector< std::string > v_fname;
  std::ifstream fin(model.bb_info.c_str());

  if( !fin ){
    error("Program_Info::Program_Info() can't open" + model.bb_info);
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
      SET succ_s, pred_s;

      buf.erase(0, buf.find(":") + 1);
      buf.erase(0, buf.find(":") + 1);
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
      v_succ[func].push_back(succ_s);
      v_pred[func].push_back(pred_s);
    }
    getline(fin, buf);

    if( fin.eof() ){
      break;
    }
  }

  // init
  func_size = v_succ.size();
  try{
    succ = new SET*[size()];
    pred = new SET*[size()];
    bb_size = new int[size()];
    fname = new std::string[size()];
  }
  catch( std::bad_alloc ){
    error("Program_Info::Program_Info() bad_alloc");
  }

  int bb_total_size = 0;

  for( int f = 0; f < size(); f ++ ){
    bb_size[f] = v_succ[f].size();
    fname[f] = v_fname[f];

    bb_total_size += bb_size[f];

    // check bitset_size
    if( bitset_size < size(f) ){
      std::cerr << "bitset size" << bitset_size << " < " << size(f) << std::endl;
      error("Program_Info::Program_Info bitset size");
    }

    try{
      succ[f] = new SET[size(f)];
      pred[f] = new SET[size(f)];
    }
    catch( std::bad_alloc ){
      error("Program_Info::Program_Info() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      succ[f][b] = v_succ[f][b];
      pred[f][b] = v_pred[f][b];
    }
  }

  std::cerr << "bb size " << bb_total_size << std::endl;
  std::cerr << "end read bb_info" << std::endl;
}

// destructor
Program_Info::~Program_Info(){
  for( int f = size() - 1; f >= 0; f -- ){
    delete[] succ[f];
    delete[] pred[f];
  }
  delete[] succ;
  delete[] pred;
  delete[] bb_size;
  delete[] fname;
}

// check code
void Program_Info::print(){
  std::cout << "Program_Info::file_read()" << std::endl;

  for( int f = 0; f < size() ; f ++ ){// LOOP FUNC
    std::cout << "{" << std::endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){// LOOP BB
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
