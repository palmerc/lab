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
  vector< vector< Bb_Info > > v_info;
  vector< string > v_fname;
  ifstream fin(model.bb_info.c_str());

  if( !fin ){
    error("Program_Info::Program_Info() can't open " + model.bb_info);
  }

  string buf;

  getline(fin, buf);
  for( int func = 0; true; func ++ ){// LOOP FUNC
    if( buf[0] != '{' ){
      error("Program_Info::Program_Info() {");
    }else{
      v_fname.push_back( buf.substr(buf.find(":") + 1, string::npos) );
    }

    // resize array for new function
    v_info.resize(func + 1);

    for( int bb = 0; ;bb ++ ){// LOOP BB
      getline(fin, buf);
      if( buf ==  "}" ){
	break;
      }

      if( buf.find(":") == string::npos || buf.find(";") == string::npos ){
	cerr << func << "," << bb << " " << buf << endl;
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
    fname = new string[size()];
  }
  catch( bad_alloc ){
    error("Program_Info::Program_Info() bad_alloc");
  }

  for( int f = 0; f < size(); f ++ ){
    bb_size[f] = v_info[f].size();
    fname[f] = v_fname[f];

    try{
      info[f] = new Bb_Info[size(f)];
    }
    catch( bad_alloc ){
      error("Program_Info::Program_Info() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      info[f][b] = v_info[f][b];
    }
  }

  cerr << "Program_Info::Program_Info() init end" << endl;
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
  cout << "Program_Info::file_read()" << endl;

  for( int f = 0; f < size() ; f ++ ){// LOOP FUNC
    cout << "{" << endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){// LOOP BB
      // start/end
      cout << bb << hex << ":" <<  info[f][bb].start
	   << ":" << info[f][bb].end << dec << endl;
    }// LOOP BB
    cout << "}" << endl;
  }// LOOP FUNC

  exit(0);
}
