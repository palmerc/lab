//
// inst.cc read asm code and divide operand
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "inst.h" 

#include "main.h" 

//
// class Inst_Type
//

// constructor
Inst_Type::Inst_Type(Program_Info &program, const int &func){
  string buf;

  getline(model.fin_asm_loop, buf);
  if( model.fin_asm_loop.eof() ){
    error("Inst_Type::Inst_Type() EOF");
  }
  if( buf.find("{") == string::npos ){// function name
    error("Inst_Type::Inst_Type() {");
  }
  if( program.funcname(func) != buf.substr(buf.find(":") + 1, string::npos) ){
    cerr << "bb_info funcnaem " << program.funcname(func)
	 << ", asm_loop funcname "
	 <<  buf.substr(buf.find(":") + 1, string::npos);
    error("Inst_Type::Inst_Type() funcname");
  }

  while(true){// LOOP all add, sub, mult, div
    getline(model.fin_asm_loop, buf);
    if( buf == "}" ){
      break;;
    }

    if( buf.find(":") == string::npos || buf.find(";") == string::npos){
      cerr << buf << endl;
      error("Inst_Type::Inst_Type() file format error");
    }

    // construction
    string op;
    int pc;
    Inst_Data inst;

    op = buf.substr(0, buf.find(":"));
    buf.erase(0, buf.find(":") + 1);
    sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &pc );

    if( buf.find(":;") != string::npos ){
      // no register instruction
      if( op == "JAL" ){
	inst.op = Call;
      }else if( op == "J" || op == "JR" ){
	inst.op = Jump;
      }else if( op == "B" ){
	inst.op = Branch;
      }else{
	error("Inst_Type::Inst_Type() JAL J JR B");
      }
    }else{
      if( op == "ADD" ){
	inst.op = Add;
      }else if( op == "SUB" ){
	inst.op = Sub;
      }else if( op == "MUL" ){
	inst.op = Mult;
      }else if( op == "DIV" ){
	inst.op = Div;
      }else{
	cerr << endl << op;
	error("Inst_Type::Inst_Type() Other");
      }

      buf.erase(0, buf.find(":") + 1);

      if( buf.find(",") == string::npos || buf.find(" ") == string::npos ){
	cerr << buf << endl;
	error("Inst_Type::Inst_Type() file format error");
      }

      inst.dest = atoi( buf.substr(0, buf.find(",")).c_str() );
      buf.erase(0, buf.find(",") + 1);

      if( buf.substr(0, buf.find(" ")) != "C" ){
	inst.src1 = atoi( buf.substr(0, buf.find(" ")).c_str() );
      }
      buf.erase(0, buf.find(" ") + 1);

      if( buf.substr(0, buf.find(";")) != "C" ){
	inst.src2 = atoi( buf.substr(0, buf.find(";")).c_str() );
      }
    }

    // insert
    insts.insert( make_pair(pc, inst) );
  }// LOOP all add, sub, mult, div
}

// destructior
Inst_Type::~Inst_Type(){
  insts.clear();
}

// find instruction operand
Inst_Data Inst_Type::operand(const int &pc){
  MI map_i = insts.find(pc);
  Inst_Data def(Other);

  if( map_i != insts.end() ){
    return map_i->second;
  }else{
    return def;
  }
}

// check code
void Inst_Type::print(){
  for( MI map_i = insts.begin(); map_i != insts.end(); map_i ++ ){
    Inst_Data type = map_i->second;

    cerr << map_i->first << " "
	 << type.op << " "
	 << type.dest << " "
	 << type.src1 << " "
	 << type.src2 << endl;
  }
}
