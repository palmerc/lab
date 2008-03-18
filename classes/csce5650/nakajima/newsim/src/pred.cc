//
// pred.cc
//
//  Time-stamp: <04/02/10 13:28:17 nakajima>
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include "pred.h"

#include "bb.h"
#include "print.h"
#include "re.h"
#include "sim.h"

Value_Predict value_pred;
Branch_Predict branch_pred;

//
// class Value_Predict (static value predictor)
//

// constructor
Value_Predict::Value_Predict(){
  val_table = 0;// NULL
  total = hit = 0;
}

// destructor
Value_Predict::~Value_Predict(){
  if( val_table ){
    for( int func = 0; func < func_size; func ++ ){
      val_table[func].clear();
    }
    delete[] val_table;
  }
}

const Value_Result Value_Predict::predict(const Pipe_Inst &inst){
  const int func = program.fbb().func, pc = inst.pc;

  MI map_i = val_table[func].find(pc);

  if( map_i == val_table[func].end() ){
    return NO_VP;
  }

  if( map_i->second.empty() ){
    error("Value_Predict::predict() SET empty");
  }

  switch( model.vp ){
  case VP_Result:
    // 結果値予測
    if( inst.func != OTHER && inst.func != LOAD ){
      return NO_VP;
    }

    if( reexec.mode() == Normal ){
      total ++;
    }

    if( map_i->second.count(inst.v_dest) ){
      if( model.debug(50) ){
	std::cerr << "Value_Predict::predict() HIT" << std::endl;
      }
      if( reexec.mode() == Normal ){
	hit ++;
      }
      return HIT_VP;
    }
    return MISS_VP;

  case VP_Send:
    // 通信値予測 (src)
    // FIX vp send
    return NO_VP;

  default:
    return NO_VP;
  }
}

void Value_Predict::file_read(){
  std::string fin_fname;

  switch( model.vp ){
  case VP_Send:
    // 通信値を必要とする命令の結果値予測
    fin_fname = model.val_pred_send;
    break;

  case VP_Result:
    // 通信値予測
    fin_fname = model.val_pred_result;
    break;

  default:
    return;
  }

  std::ifstream fin(fin_fname.c_str());

  if( !fin ){
    error("Value_Predict::file_read() can't open" + fin_fname);
  }

  func_size = program.size();

  try{
    val_table = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Value_Predict::file_read() bad_alloc");
  }

  std::string buf;
  int total_size = 0;

  getline(fin, buf);
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    const std::string fname = program.funcname(func);

    if( buf.find("{") == std::string::npos ){// function name
      error("Value_Predict::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcname " << fname << ", brn_pred funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("Value_Predict::file_read() funcname");
    }

    while(true){// LOOP pc
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos
	  || buf.find(";") == std::string::npos ){
	std::cerr << buf << std::endl;
	error("Value_Predict::file_read() file error");
      }

      int pc, total;

      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &pc );
      buf.erase(0, buf.find(":") + 1);
      total = atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      while( buf != ";" ){// LOOP val
	int val, freq;

	freq = atoi( buf.substr(0, buf.find(",")).c_str()  );
	buf.erase(0, buf.find(",") + 1);
	sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &val );
	buf.erase(0, buf.find(":") + 1);

	if( 100 * (double)freq / (double)total >= (double)model.val_pred_th ){
	  // 閾値の高いもののみinsert
	  val_table[func][pc].insert(val);
	}else{
	  break;
	}
      }// LOOP val
    }// LOOP pc
    getline(fin, buf);

    total_size += val_table[func].size();

    if( fin.eof() ){
      break;
    }
  }// LOOP FUNC

  std::cout << "# Value_Predict::file_read() init end: " << total_size << std::endl;
}

void Value_Predict::print(){
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    std::cout << "{" << func << ":" << program.funcname(func) << std::endl;

    for( MI map_i = val_table[func].begin();
	 map_i != val_table[func].end(); map_i ++ ){// LOOP pc
      std::cout << std::hex << map_i->first << std::dec << ":";

      for( SI set_i = map_i->second.begin();
	   set_i != map_i->second.end(); set_i ++ ){// LOOP val
	std::cout << *set_i << ",";
      }// LOOP val

      std::cout << std::endl;
    }// LOOP pc

    std::cout << "}" << std::endl;
  }// LOOP FUNC

  exit(0);
}


//
// class Branch_Predict (static/2-lev branch predector)
//

Branch_Predict::Branch_Predict(){
  // NULL
  brn_table = 0;
  bh_table = bh_bak = ph_table = ph_bak = 0;

  sp = MISS;
  pred = pred_hit = 0;
}

Branch_Predict::~Branch_Predict(){
  // static
  if( brn_table ){
    for( int func = 0; func < func_size; func ++ ){
      brn_table[func].clear();
    }
    delete[] brn_table;
  }

  // 2-lev
  if( bh_table ){
    delete[] bh_table;
  }
  if( ph_table ){
    delete[] ph_table;
  }
  // for reexec mode
  if( bh_bak ){
    delete[] bh_bak;
  }
  if( ph_bak ){
    delete[] ph_bak;
  }
}

void Branch_Predict::allocate(){
  // init
  pa_i = 15;// pc
  pa_j = 15;// pc (pa_i >= pa_j)
  pa_k = 2;// BHT k-bit shift reg

  std::cout << "$ Branch_Predict::allocate() pa_i:" << pa_i
       << ", pa_j:" << pa_j << ", pa_k:" << pa_k << std::endl;

  if( pa_i < pa_j ){
    warning("Branch_Predict::allocate() pa_i < pa_j");
  }

  if( bitset_size < pa_i || bitset_size < (pa_j + pa_k) ){
    std::cout << "bitset_size "<< bitset_size << std::endl;
    error("Branch_Predict::allocate() size < i or size < j + k");
  }

  // allocate branch history table, pattern history table
  try{
    bh_table = new BITSET[pa_k] ;// k bit shift reg
    ph_table = new BITSET[2];// 2bc

    if( model.reexec ){
      bh_bak = new BITSET[pa_k] ;// k bit shift reg
      ph_bak = new BITSET[2];// 2bc
    }
  }
  catch( std::bad_alloc ){
    error("Branch_Predict::allocate() bad_alloc");
  }

  // init all 2bit counte "01"
  ph_table[0].set();
  ph_table[1].reset();

  if( model.reexec ){
    std::cout << "# Brnach_Predict::allocate() allocate BHT PHT (bak) end" << std::endl;
  }else{
    std::cout << "# Brnach_Predict::allocate() allocate BHT PHT end" << std::endl;
  }
}

// conditional branch prediction
void Branch_Predict::predict(const Pipe_Inst &inst){
  const int address = inst.address, pc = inst.pc;

  if( model.static_brn_pred ){
    // static branch predict
    const int func = program.fbb().func;
    MI map_i =  brn_table[func].find(pc);

    if( map_i == brn_table[func].end() ){
      std::cerr << "func: " << func << std::hex << " address:" << address
	   << " pc:" << pc << std::dec <<  std::endl;
      error("Branch_Predict::result() brn_table.find(pc)");
    }else{
      if( map_i->second != address ){
	sp = MISS;
      }else{
	sp = HIT;
      }
    }
  }else{
    // 2-lev branch predict

    // lev1:BHT, lev2:PHT
    const int lev1_i = ( (1 << pa_i) - 1 ) & (pc >> 3);
    const int lev2_j = ( (1 << pa_j) - 1 ) & (pc >> 3);
    int lev2_k = 0;

    // BHT
    for( int sreg = 0; sreg < pa_k; sreg ++ ){
      lev2_k |= (bh_table[sreg][lev1_i] << sreg);
    }

    const int lev2_jk = (lev2_j << pa_k) | lev2_k;
    const int nextpc = pc + 8;

    if( false ){
      std::cout << std::hex << "pc:" << pc << ","  << "p:" << (pc >> 3)
	   << ",i:" << lev1_i << ",j:" << lev2_j << ",k:" << lev2_k
	   << ",j+k:" << lev2_jk << ",addr:" << address << "\t" << std::dec;
    }

    // PHT
    if( ph_table[1][lev2_jk] ){
      // pred taken (1*)
      if( nextpc == address ){
	// not taken
	sp = MISS;

	// PHT 2bc UPDATE -1
	if( ph_table[0][lev2_jk] ){
	  // 11 -> 10
	  ph_table[0].reset(lev2_jk);

	  if( false ){
	    std::cout << "11 no miss 10" << std::endl;
	  }
	}else{
	  // 10 -> 01
	  ph_table[1].reset(lev2_jk);
	  ph_table[0].set(lev2_jk);

	  if( false ){
	    std::cout << "10 no miss 01" << std::endl;
	  }
	}
      }else{
	// taken
	sp = HIT;

	// PHT 2bc UPDATE +1
	// 1* -> 11
	ph_table[0].set(lev2_jk);

	if( false ){
	  std::cout << "1* ta hit 11" << std::endl;
	}
      }

      // next BHT
      lev2_k = (lev2_k << 1) | 0x01;
    }else{
      // pred not taken (0*)
      if( nextpc == address ){
	// not taken
	sp = HIT;

	// PHT 2bc UPDATE -1
	// 0* -> 00
	ph_table[0].reset(lev2_jk);

	if( false ){
	  std::cout << "0* no hit 00" << std::endl;
	}
      }else{
	sp = MISS;

	// PHT 2bc UPDATE +1
	if( ph_table[0][lev2_jk] ){
	  // 01 -> 10
	  ph_table[1].set(lev2_jk);
	  ph_table[0].reset(lev2_jk);

	  if( false ){
	    std::cout << "01 ta miss 10" << std::endl;
	  }
	}else{
	  // 00 -> 01
	  ph_table[0].set(lev2_jk);

	  if( false ){
	    std::cout << "00 ta miss 01" << std::endl;
	  }
	}
      }

      // next BHT
      lev2_k = (lev2_k << 1);
    }

    // next BHT UPDATE
    for( int sreg = 0; sreg < pa_k; sreg ++ ){
      if( 0x01 & (lev2_k >> sreg) ){
	bh_table[sreg].set(lev1_i);
      }else{
	bh_table[sreg].reset(lev1_i);
      }
    }
  }// 2lev branch predict

  // count
  if( reexec.mode() == Normal ){
    pred ++;
    if( sp == HIT ){
      pred_hit ++;
    }
  }
}

// SPモデルではない場合、条件分岐以外のbranch()
void Branch_Predict::change_miss(){
  if( !(sim_type & SP) ){
    sp = MISS;
  }
}

// loop_unrollモデルのbedgeについて、以前の条件分岐がMISSの場合、HITに変更
void Branch_Predict::change_pred_hit(){
  if( !hit() ){
    // miss -> hit
    sp = HIT;
  }

  if( !model.reexec || reexec.mode() == Finish || reexec.mode() == Normal ){
    // Loop::count_unroll()で命令削除、ここで分岐予測uncount
    if( hit() ){
      pred_hit --;
    }
    pred --;
  }
}

void Branch_Predict::backup(){
  if( model.static_brn_pred ){
    return;
  }

  if( !bh_bak || !ph_bak ){
    error("Branch_Predict::backup() no allocate");
  }

  // BHT (k-bit shift-reg)
  for( int sreg = 0; sreg < pa_k; sreg ++ ){
    bh_bak[sreg] = bh_table[sreg];
  }

  // PHT (2bit)
  ph_bak[0] = ph_table[0];
  ph_bak[1] = ph_table[1];
}

void Branch_Predict::restore(){
  if( model.static_brn_pred ){
    return;
  }

  if( !bh_bak || !ph_bak ){
    error("Branch_Predict::backup() no allocate");
  }

  // BHT (k-bit shift-reg)
  for( int sreg = 0; sreg < pa_k; sreg ++ ){
    bh_table[sreg] = bh_bak[sreg];
  }

  // PHT (2bit)
  ph_table[0] = ph_bak[0];
  ph_table[1] = ph_bak[1];
}

// read branch info
void Branch_Predict::file_read(){
  std::ifstream fin(model.brn_pred.c_str());

  if( !fin ){
    error("Branch_Predict::file_read() can't open " + model.brn_pred);
  }

  func_size = program.size();

  try{
    brn_table = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Branch_Predict::Branch_Predict() bad_alloc");
  }

  std::string buf;

  getline(fin, buf);
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    const std::string fname = program.funcname(func);

    if( buf.find("{") == std::string::npos ){// function name
      error("Branch_Predict::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcname " << fname << ", brn_pred funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("Branch_Predict::file_read() funcname");
    }

    while(true){// LOOP branch inst
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos
	  || buf.find(";") == std::string::npos ){
	std::cerr << func << " " << buf << std::endl;
	error("Branch_Predict::file_read() file error");
      }

      int pc, target;

      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &pc );
      buf.erase(0, buf.find(":") + 1);
      sscanf( buf.substr(0, buf.find(",")).c_str(), "%x", &target );

      // insert
      brn_table[func].insert( std::make_pair( pc, target ) );
    }// LOOP branch inst
    getline(fin, buf);

    if( fin.eof() ){
      break;
    }
  }// LOOP FUNC

  std::cout << "# Brnach_Predict::file_read() init end" << std::endl;
}
