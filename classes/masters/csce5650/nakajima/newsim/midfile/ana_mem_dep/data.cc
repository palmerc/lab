//
// data.cc mem profile analysis
//  Time-stamp: <03/08/14 15:06:41 nakajima>
//

#include <iostream>
#include <cstdio>
#include "data.h"

#include "main.h"

//
// class Mem_Profile
//

// constructor
Mem_Profile::Mem_Profile(){
  total_freq.resize(size);
  total_load = 0;
}

// deatructor
Mem_Profile::~Mem_Profile(){
  profile.clear();
  total_freq.clear();
}


void Mem_Profile::calc_total_load(){
  ifstream fin(model.mp_analysis.c_str());

  if( !fin ){
    error("Mem_Profile::file_read() can't open " + model.mp_analysis);
  }

  string buf;

  getline(fin, buf);
  for( int func = 0; ; func ++ ){// LOOP FUNC
    if( buf.find("{") == string::npos ){// function name
      error("Mem_Profile::file_read() {");
    }

    while(true){// LOOP load pc
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == string::npos || buf.find(",") == string::npos
	  || buf.find(";") == string::npos ){
	cerr << buf << endl;
	error("Mem_Profile::file_read() file error");
      }

      buf.erase(0, buf.find(":") + 1);
      total_load += atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);
    }// LOOP load pc
    getline(fin, buf);

    if( fin.eof() ){
      break;
    }
  }// LOOP func

  cout << "# total_load: " << total_load << endl;
}


void Mem_Profile::file_read(){
  calc_total_load();

  ifstream fin(model.mp_analysis.c_str());

  if( !fin ){
    error("Mem_Profile::file_read() can't open " + model.mp_analysis);
  }

  string buf;

  getline(fin, buf);
  for( int func = 0; ; func ++ ){// LOOP FUNC
    if( buf.find("{") == string::npos ){// function name
      error("Mem_Profile::file_read() {");
    }

    while(true){// LOOP load pc
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == string::npos || buf.find(",") == string::npos
	  || buf.find(";") == string::npos ){
	cerr << buf << endl;
	error("Mem_Profile::file_read() file error");
      }

      int ld_pc;
      double total;
      VEC temp;

      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &ld_pc );
      buf.erase(0, buf.find(":") + 1);
      total = atof( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      while( buf != ";" ){// LOOP store pc
	double freq;

        freq = atof( buf.substr(0, buf.find(",")).c_str()  );
//          buf.erase(0, buf.find(",") + 1);
//          sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &st_pc );
        buf.erase(0, buf.find(":") + 1);

	temp.push_back(freq);
      }// LOOP store pc

      profile[ld_pc].resize(size);

      double subtotal = 0, th = 0;

      for( int i = temp.size() - 1; i >= 0; i -- ){
	double freq = temp[i];

	for( ; th < size; th ++ ){
	  if( 100 * freq / total >= th ){
	    profile[ld_pc][th] = (total - subtotal) / total;
	  }else{
	    subtotal += freq;
	    break;
	  }
	}
      }
    }// LOOP load pc
    getline(fin, buf);

    for( MI map_i = profile.begin();
	 map_i != profile.end(); map_i ++ ){// LOOP freq
      for( int i = 0; i < size; i ++ ){
	total_freq[i] += map_i->second[i];
      }
    }// LOOP freq

//      cout << "{" << func << endl;
//      print();
//      cout << "}" << endl;

    profile.clear();

    if( fin.eof() ){
      break;
    }
  }// LOOP func

  // print freq
  cout << "# Mem_Profile::file_read() init end" << endl;

  double total = total_freq[0];

  for( int i = 0; i < size; i ++ ){
    cout << i << " " << 100 * total_freq[i] / total << endl;
  }
}

void Mem_Profile::print(){
  for( MI map_i = profile.begin();
       map_i != profile.end(); map_i ++ ){// LOOP freq
    cout << hex << map_i->first << dec << ":";

    for( int i = 0; i < map_i->second.size(); i ++ ){
      cout << map_i->second[i] << " ";
    }

    cout << endl;
  }// LOOP freq
}
