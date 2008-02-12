//
// main.cc
//

#include <iostream>
#include "main.h"

#include "data.h"

Sim_Model model;

///////////////////////////////////////////////////////////////////////
// main function
///////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
  model.arg_check(argc, argv);

  Mem_Profile mem_profile;

  mem_profile.file_read();
//    mem_profile.print();

  return 0;
}



void Sim_Model::arg_check(const int &argc, char **argv){
  argv0 = argv[0];

  if( argc == 1 ){
    cerr << "===== argment is required =====" << endl;
    usage();
  }

  string arg;

  for( int i = 1; i < argc; i ++){
    arg = argv[i];

    if( arg == "-f" ){
      // filename
      if( argc - 1 == i ){
	break;
      }

      mp_analysis = argv[++i];;
      cout << "# set fname: " << mp_analysis << endl;

      return;
    }else{
      break;
    }
  }

  cerr << "error option: " << arg << endl;
  usage();
}

// usage
void Sim_Model::usage(){
  cout << "Usage is:" << argv0 << " -f FILENAME" << endl;
  exit(1);
}

// atoi 自然数のみ値を返す
const int Sim_Model::check_atoi(const string str){
  int val = 0;

  if( str != "0" ){
    val = atoi(str.c_str());

    if( val <= 0 ){
      cerr << "Sim_Model::check_atoi() error option <val>: " << str << endl
	   << endl;
      usage();
    }
  }

  return val;
}
