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
  vector< vector< SET > > v_succ, v_pred;
  vector< string > v_fname;
  ifstream fin(model.bb_info.c_str());

  if( !fin ){
    error("Program_Info::Program_Info() can't open" + model.bb_info);
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
    v_succ.resize(func + 1);
    v_pred.resize(func + 1);

    for( int bb = 0; ;bb ++ ){// LOOP BB
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == string::npos || buf.find(";") == string::npos ){
	cerr << func << "," << bb << " " << buf << endl;
	error("Program_Info::Program_Info() file error");
      }

      // construct
      SET succ_s, pred_s;

      buf.erase(0, buf.find(":") + 1);
      buf.erase(0, buf.find(":") + 1);
      buf.erase(0, buf.find(":") + 1);

      if( buf.find("exit") == string::npos &&
	  buf.find("break") == string::npos ){
	while( buf[0] != ':' ){
	  string val = buf.substr(0, buf.find(" "));

	  succ_s.insert( atoi(val.c_str()) );
	  buf.erase(0, buf.find(" ") + 1);
	}
      }

      buf.erase(0, buf.find(":") + 1);

      if( buf.find("IN") == string::npos ){
	while( buf != ";" ){
	  string val = buf.substr(0, buf.find(" "));

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
    fname = new string[size()];
  }
  catch( bad_alloc ){
    error("Program_Info::Program_Info() bad_alloc");
  }

  int bb_total_size = 0;

  for( int f = 0; f < size(); f ++ ){
    bb_size[f] = v_succ[f].size();
    fname[f] = v_fname[f];

    bb_total_size += bb_size[f];

    // check bitset_size
    if( bitset_size < size(f) ){
      cerr << "bitset size" << bitset_size << " < " << size(f) << endl;
      error("Program_Info::Program_Info bitset size");
    }

    try{
      succ[f] = new SET[size(f)];
      pred[f] = new SET[size(f)];
    }
    catch( bad_alloc ){
      error("Program_Info::Program_Info() bad_alloc");
    }

    for( int b = 0; b < size(f); b ++ ){
      succ[f][b] = v_succ[f][b];
      pred[f][b] = v_pred[f][b];
    }
  }

  cerr << "bb size " << bb_total_size << endl;
  cerr << "end read bb_info" << endl;
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
  cout << "Program_Info::file_read()" << endl;

  for( int f = 0; f < size() ; f ++ ){// LOOP FUNC
    cout << "{" << endl;
    for( int bb = 0; bb < size(f) ; bb ++ ){// LOOP BB
      // succ
      cout << bb << ":";
      for( int i = -1; i < size(f); i ++ ){
	if( succ[f][bb].count(i) ){
	  cout << i << " ";
	}
      }

      // pred
      cout << ":";
      for( int i = 0; i < size(f); i ++ ){
	if( pred[f][bb].count(i) ){
	  cout << i << " ";
	}
      }

      cout << endl;
    }// LOOP BB
    cout << "}" << endl;
  }// LOOP FUNC

  exit(0);
}
