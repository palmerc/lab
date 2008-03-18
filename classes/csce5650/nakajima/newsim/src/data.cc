//
// data.cc reg/mem dependence
//
//  Time-stamp: <04/01/22 19:02:56 nakajima>
//

#include <iostream>
#include <cstdio>
#include "data.h"

#include "print.h"
#include "re.h"
#include "sim.h"

Mem memory;
Reg_Count reg_finite;
Mem_Dep_MP mem_mp;
Mem_Dep_SP mem_sp;
Mem_Profile mem_profile;
Store_Thread store_tid;

///////////////////////////////////////////////////////////////////////////////

//
// class Mem_Dep_MP (blind and analysis model)
//

Mem_Dep_MP::Mem_Dep_MP(){
  profile = 0;// NULL
  viol_flag = NODEP;
  mem_limit = last_mem_limit = 0;

  mem_dep =  mem_viol = mem_sync = mem_nodep = 0;
  mem_dep_load =  mem_viol_load = 0;

  store_num = 0;
}

Mem_Dep_MP::~Mem_Dep_MP(){
  if( profile ){
    for( int func = 0; func < func_size; func ++ ){
      profile[func].clear();
    }
    delete[] profile;
  }
  profile_diff.clear();
}

// init thread
void Mem_Dep_MP::init_thread(const int &limit){
  // count
  if( reexec.mode() == Normal ){
    switch( viol_flag ){
    case DEP:
      mem_dep ++;
      break;

    case VIOL:
      mem_viol ++;
      break;

    case SYNC:
      mem_sync ++;
      break;

    default:
      mem_nodep ++;
      break;
    }

    last_mem_limit = mem_limit;
  }

  // init
  viol_flag = NODEP;
  mem_limit = limit;
}

// if violation, return mem_limit (blind, analysis)
const int Mem_Dep_MP::get_viol_limit(){
  if( viol_flag == VIOL ){
    // change blind / analysis limit
    if( model.debug(5) ){
      std::cerr << " mem_mp limit: " << mem_limit << std::endl;
    }
    return mem_limit;
  }

  return 0;
}

// mem_blind
void Mem_Dep_MP::blind(const Pipe_Inst &inst, int &limit){
  const int address = inst.address, load_byte = inst.m_size;
  int st_pc = 0;

  limit = std::max(limit, get_viol_limit() );

  for( int byte = 0; byte < load_byte; byte ++ ){// LOOP byte
    Mem::Mem_Data mem = memory.read_data(address + byte);

    if( st_pc == mem.pc ){
      // 以前と同じstore pcの場合
      continue;
    }else{
      st_pc = mem.pc;
    }

    if( !c.last_thread_defined(mem.thread) ){
      // スレッド間依存ではない
      continue;
    }

    if( viol_flag == NODEP ){
      viol_flag = DEP;
    }

    // store time > load命令が実行可能な時刻
    if( mem.write > limit ){
      // スレッド間メモリ依存違反
      viol_flag = VIOL;
      mem_limit = std::max(mem_limit, mem.write);

      if( model.debug(5) ){
	std::cerr << " -> " << mem_limit << std::endl;
      }
    }
  }// LOOP byte

  if( reexec.mode() == Normal ){
    switch( viol_flag ){
    case DEP:
      mem_dep_load ++;
      break;

    case VIOL:
      mem_viol_load ++;
      mem_dep_load ++;
      break;

    default:
      break;
    }
  }

  limit = std::max( limit, get_viol_limit() );
}

void Mem_Dep_MP::analysis(const Pipe_Inst &inst, int &limit){
  const int ld_pc = inst.pc, address = inst.address, load_byte = inst.m_size;
  const int func = program.fbb().func;
  int st_pc = 0;

  limit = std::max(limit, get_viol_limit() );

  for( int byte = 0; byte < load_byte; byte ++ ){// LOOP byte
    // store data
    Mem::Mem_Data mem = memory.read_data(address + byte);

    if( st_pc == mem.pc ){
      // 以前と同じstore pcの場合
      continue;
    }else{
      st_pc = mem.pc;
    }

    if( !c.last_thread_defined(mem.thread) ){
      // スレッド間依存ではない
      continue;
    }

    if( viol_flag == NODEP ){
      viol_flag = DEP;
    }

    // store time > load命令が実行可能な時刻
    if( mem.write > limit ){
      // スレッド間メモリ依存違反
      if( model.debug(10) ){
	std::cerr << std::hex << " load pc:" << ld_pc << " store pc:" << st_pc << std::dec;
      }

      MI map_i;

      if( ( map_i = profile[func].find(ld_pc) ) != profile[func].end() ){
	// ロード命令に対応するエントリが存在
	if( find( map_i->second.begin(), map_i->second.end(), st_pc )
	    !=  map_i->second.end() ){
	  // 該当エントリ内にストア命令が存在する ->
	  // スレッド間メモリ依存はperfectに同期
  	  if( model.debug(10) ){
  	    std::cerr << " SYNC ";
  	  }

  	  if( viol_flag == DEP ){
  	    viol_flag = SYNC;
  	  }
	}else{
  	  // 該当エントリ内にストア命令が存在しない ->
	  // スレッド間メモリ依存違反
	  if( model.debug(10) ){
	    std::cerr << " VIOL ";
	  }
	  viol_flag = VIOL;
	  mem_limit = std::max(mem_limit, mem.write);
	}
      }else{
	// ロード命令に対応するエントリが存在しない -> blindと同様
	// スレッド間メモリ依存違反
	if( model.debug(10) ){
	  std::cerr << " VIOL ";
	}
	viol_flag = VIOL;
	mem_limit = std::max(mem_limit, mem.write);
      }

      if( model.debug(10) ){
	std::cerr << " -> " << mem_limit << std::endl;
      }
    }
  }// LOOP byte

  if( reexec.mode() == Normal ){
    switch( viol_flag ){
    case DEP:
      mem_dep_load ++;
      break;

    case VIOL:
      mem_viol_load ++;
      mem_dep_load ++;
      break;

    default:
      break;
    }
  }

  limit = std::max( limit, get_viol_limit() );
}

// MDP
void Mem_Dep_MP::md_predict(const Pipe_Inst &inst, Store_Thread &st_tid,
			    int &limit){
  const int ld_pc = inst.pc, address = inst.address, load_byte = inst.m_size;
  const int func = program.fbb().func;// ld_pc func num
  int st_pc = 0;

  limit = std::max(limit, get_viol_limit() );

  for( int byte = 0; byte < load_byte; byte ++ ){// LOOP byte
    // store data (pc, write_time, thread_id)
    Mem::Mem_Data mem = memory.read_data(address + byte);

    if( st_pc == mem.pc ){
      // 以前と同じstore pcの場合
      continue;
    }else{
      st_pc = mem.pc;
    }

    if( !c.last_thread_defined(mem.thread) ){
      // スレッド間依存ではない
      continue;
    }

    if( viol_flag == NODEP ){
      viol_flag = DEP;
    }

    // store time > load命令が実行可能な時刻
    if( mem.write > limit ){
      // スレッド間メモリ依存違反
      if( model.debug(10) ){
	std::cerr << std::hex << " load pc:" << ld_pc << " store pc:" << st_pc << std::dec;
      }

      // ロード命令に対応するエントリが存在するかどうか
      MI map_i;
      bool found = false;// 存在しない

      switch( reexec.mode() ){
      case Normal:
	if( ( map_i = profile[func].find(ld_pc) ) != profile[func].end() ){
	  found = true;
	}else{
	  // 存在しない
	  found = false;
	}
	break;

      case No:
      case Fork:
	if( ( map_i = profile_diff.find(ld_pc) ) != profile_diff.end() ){
	  found = true;
	}else{
	  if( ( map_i = profile[func].find(ld_pc) ) != profile[func].end() ){
	    found = true;
	  }else{
	    // どちらにも存在しない
	    found = false;
	  }
	}
	break;

      default:
	error("Mem_Dep_MP::md_predict() default switch");
      }

      // ロード命令に対応するエントリが存在するかどうか
      if( found ){
	// ロード命令に対応するエントリが存在
	if( find( map_i->second.begin(), map_i->second.end(), st_pc )
	    !=  map_i->second.end() ){
	  // 該当エントリ内にストア命令が存在する

	  if( model.mdpred_perf || store_tid.search(mem) ){
	    // ストア命令に該当するスレッド内に、その静的なストア命令が一つ
	    // そのストアを待ち合わせをするか、すでにロードが実行可能

	    // 複数あるが依存しているストアより先に実行が終了している
	    // blindで違反が発生しない

	    if( model.debug(10) ){
	      std::cerr << " SYNC ";
	    }

	    if( viol_flag == DEP ){
	      viol_flag = SYNC;
	    }
	  }else{
	    // ストア命令に該当するスレッド内に、その静的なストア命令が複数
	    // 依存しているものより先に
	    // blindと同様、登録はされているので、違反発生

	    // スレッド間メモリ依存違反
	    if( model.debug(10) ){
	      std::cerr << " VIOL(MULT) ";
	    }
	    viol_flag = VIOL;
	    mem_limit = std::max(mem_limit, mem.write);
	  }
	}else{
  	  // 該当エントリ内にストア命令が存在しない -> blindと同様
	  // テーブルの該当エントリに登録

	  // スレッド間メモリ依存違反
	  if( model.debug(10) ){
	    std::cerr << " VIOL(NO STORE) ";
	  }
	  viol_flag = VIOL;
	  mem_limit = std::max(mem_limit, mem.write);

	  // 削除・登録
	  switch( reexec.mode() ){
	  case Normal:
	    if( map_i != profile[func].find(ld_pc) ){
	      error("Mem_Dep_MP::md_predict() map_i != profile.find(ld_pc)");
	    }
	    map_i->second.push_back(st_pc);

	    while( map_i->second.size() > store_num ){
	      map_i->second.pop_front();
	    }

	    if( map_i->second.empty() ){
	      error("Mem_Dep_MP::md_predict() map_i->second.empty()");
	    }
	    break;

	  case No:
	  case Fork:
	    if( profile_diff[ld_pc].empty() ){
	      // 空の場合は、コピーする (なにもコピーしない場合もありうる)
	      if( ( map_i = profile[func].find(ld_pc) )
		  != profile[func].end() ){
		profile_diff[ld_pc] = map_i->second;
	      }
	    }

	    map_i = profile_diff.find(ld_pc);

	    map_i->second.push_back(st_pc);

	    while( map_i->second.size() > store_num ){
	      map_i->second.pop_front();
	    }

	    if( map_i->second.empty() ){
	      error("Mem_Dep_MP::md_predict() map_i->second.empty()");
	    }
	    break;

	  default:
	    error("Mem_Dep_MP::md_predict() default switch");
	  }
	}
      }else{
	// ロード命令に対応するエントリが存在しない -> blindと同様
	// テーブルの該当エントリに登録

	// スレッド間メモリ依存違反
	if( model.debug(10) ){
	  std::cerr << " VIOL(NO ENTRY) ";
	}
	viol_flag = VIOL;
	mem_limit = std::max(mem_limit, mem.write);

	// (どちらにも)エントリが存在しないので、登録
	switch( reexec.mode() ){
	case Normal:
	  if( profile[func][ld_pc].empty() ){
	    profile[func][ld_pc].push_back(st_pc);
	  }else{
	    error("Mem_Dep_MP::md_predict() profile[ld_pc].empty()");
	  }
	  break;

	case No:
	case Fork:
	  if( profile_diff[ld_pc].empty() ){
	    if( profile[func].find(ld_pc) == profile[func].end() ){
	      // 両方ともに空
	      profile_diff[ld_pc].push_back(st_pc);
	    }else{
	      error("Mem_Dep_MP::md_predict() profile.find(ld_pc) == .end()");
	    }
	  }else{
	    error("Mem_Dep_MP::md_predict() profile_diff[ld_pc].empty()");
	  }
	  break;

	default:
	  error("Mem_Dep_MP::md_predict() default switch");
	}
      }

      if( model.debug(10) ){
	std::cerr << " -> " << mem_limit << std::endl;
      }
    }
  }// LOOP byte

  if( reexec.mode() == Normal ){
    switch( viol_flag ){
    case DEP:
      mem_dep_load ++;
      break;

    case VIOL:
      mem_viol_load ++;
      mem_dep_load ++;
      break;

    default:
      break;
    }
  }

  limit = std::max( limit, get_viol_limit() );
}


void Mem_Dep_MP::allocate(){
  func_size = program.size();
  store_num = (unsigned)model.mp_store_num;

  try{
    profile = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Mem_Dep_MP::allocate() bad_alloc");
  }
}


void Mem_Dep_MP::file_read(){
  std::ifstream fin(model.mp_analysis.c_str());

  if( !fin ){
    error("Mem_Dep_MP::file_read() can't open " + model.mp_analysis);
  }
  func_size = program.size();
  store_num = (unsigned)model.mp_store_num;

  try{
    profile = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Mem_Dep_MP::file_read() bad_alloc");
  }

  std::string buf;
  int total_size = 0, total_set_size = 0;

  getline(fin, buf);
  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    const std::string fname = program.funcname(func);

    if( buf.find("{") == std::string::npos ){// function name
      error("Mem_Dep_MP::file_read() {");
    }
    if( fname != buf.substr(buf.find(":") + 1, std::string::npos) ){
      std::cerr << "bb_info funcname " << fname << ", analysis funcname "
	   <<  buf.substr(buf.find(":") + 1, std::string::npos);
      error("Mem_Dep_MP::file_read() funcname");
    }

    while(true){// LOOP load pc
      getline(fin, buf);
      if( buf == "}" ){
	break;
      }

      if( buf.find(":") == std::string::npos || buf.find(",") == std::string::npos
	  || buf.find(";") == std::string::npos ){
	std::cerr << buf << std::endl;
	error("Mem_Dep_MP::file_read() file error");
      }

      int ld_pc, total;

      sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &ld_pc );
      buf.erase(0, buf.find(":") + 1);
      total = atoi( buf.substr(0, buf.find(":")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      // duplicate check
      if( !profile[func][ld_pc].empty() ){
	std::cerr << "ld_pc: " << std::hex << ld_pc << std::dec << std::endl << std::endl;
	print();
	error("Mem_Dep_MP::file_read() duplicate");
      }

      while( buf != ";" ){// LOOP store pc
	int st_pc, freq;

	freq = atoi( buf.substr(0, buf.find(",")).c_str()  );
	buf.erase(0, buf.find(",") + 1);
	sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &st_pc );
	buf.erase(0, buf.find(":") + 1);

	if( store_num > profile[func][ld_pc].size() ){
	  // 各エントリが有限の場合
	  profile[func][ld_pc].push_front(st_pc);
	}else{
	  break;
	}
      }// LOOP store pc

      total_set_size += profile[func][ld_pc].size();
    }// LOOP load pc
    getline(fin, buf);

    total_size += profile[func].size();

    if( fin.eof() ){
      break;
    }
  }// LOOP func

  std::cout << "# Mem_Dep_MP::file_read() init end: " << total_size
       << "," << total_set_size
       << " (" << (double)total_set_size / (double)total_size << ")" << std::endl;
}

void Mem_Dep_MP::print(){
  std::cout << "Mem_Dep_MP::print()" << std::endl;

  for( int func = 0; func < func_size; func ++ ){// LOOP FUNC
    for( MI map_i = profile[func].begin();
	 map_i != profile[func].end(); map_i ++ ){// LOOP load_pc
      const int ld_pc = map_i->first;

      std::cout << std::hex << ld_pc << ":" << std::dec << map_i->second.size() << ":";

      for( LI list_i = map_i->second.begin();
	   list_i != map_i->second.end(); list_i ++ ){
	std::cout << std::hex << *list_i << std::dec << ",";
      }

      std::cout << ";" << std::endl;
    }// LOOP load pc
  }// LOOP func
}



///////////////////////////////////////////////////////////////////////////////

//
// class Mem_Profile
//

// constructor
Mem_Profile::Mem_Profile(){
  profile = 0;// NULL
}

// deatructor
Mem_Profile::~Mem_Profile(){
  if( profile ){
    for( int func = 0; func < func_size; func ++ ){
      profile[func].clear();
    }
    delete[] profile;
  }
}

// file write
void Mem_Profile::Profile_Data::add_data(const int &st_pc){
  profile_data[st_pc] ++;
  total ++;
}

void Mem_Profile::Profile_Data::file_write(std::ofstream &fout){
  // sort
  typedef std::multimap< int, int > MMAP;
  typedef MMAP::reverse_iterator MMI;

  MMAP sorted;

  for( MI map_i = profile_data.begin();
       map_i != profile_data.end(); map_i ++ ){// LOOP store_pc
    // sort "store_pc, count" --> "count, store_pc:"
    sorted.insert( std::make_pair(map_i->second, map_i->first) );
  }// LOOP store_pc

  fout << total << ":";

  for( MMI map_i = sorted.rbegin();
       map_i != sorted.rend(); map_i ++ ){// LOOP count
    // print "count, load_pc:"
    fout << map_i->first << "," << std::hex << map_i->second << std::dec << ":";
  }// LOOP count
}

// mem allocate
void Mem_Profile::allocate(){
  func_size = program.size();

  try{
    profile = new MAP[func_size];
  }
  catch( std::bad_alloc ){
    error("Mem_Profile::allocate() bad_alloc");
  }
}

// MP_Analysis用のプロファイルをとる
void Mem_Profile::count_inst(const Pipe_Inst &inst, const int &limit){
  if( reexec.mode() != Normal ){
    return;
  }

  const int ld_pc = inst.pc, address = inst.address, load_byte = inst.m_size;
  const int func = program.fbb().func;
  int st_pc = 0;

  for( int byte = 0; byte < load_byte; byte ++ ){
    Mem::Mem_Data mem = memory.read_data(address + byte);

    if( st_pc == mem.pc ){
      // 以前と同じstore pcの場合は、追加作業はしない
      continue;
    }else{
      st_pc = mem.pc;
    }

    if( c.last_thread_defined(mem.thread) ){
      // スレッド間メモリ依存が発生する組
      profile[func][ld_pc].add_data(st_pc);
    }
  }
}

void Mem_Profile::file_write(){
  std::ofstream fout(model.mp_analysis.c_str());

  if( !fout ){
    error("Mem_Profile::file_write() can't open " + model.mp_analysis);
  }

  int total_size = 0;

  for( int func = 0; func < func_size; func ++ ){// LOOP func
    fout << "{" << func << ":" << program.funcname(func) << std::endl;

    for( MI map_i = profile[func].begin();
	 map_i != profile[func].end(); map_i ++ ){// LOOP load_pc
      const int ld_pc = map_i->first;

      fout << std::hex << ld_pc << ":" << std::dec;
      // file write store_pcs
      map_i->second.file_write(fout);
      fout << ";" << std::endl;
    }// LOOP load_pc

    total_size += profile[func].size();

    fout << "}" << std::endl;
  }// LOOP func

  std::cout << "# Mem_Profile::file_write() size: " << total_size << std::endl;
}




///////////////////////////////////////////////////////////////////////////////

//
// class Mem (memory map)
//
const Mem::Mem_Data Mem::read_data(const int &address){
  MI map_i;

  switch( reexec.mode() ){
  case Normal:
    if( (map_i = mem.find(address)) != mem.end() ){
      return map_i->second;
    }
    break;

  case No:
  case Fork:
    if( (map_i = mem_diff.find(address)) != mem_diff.end() ){
      return map_i->second;
    }else if( (map_i = mem.find(address)) != mem.end() ){
      return map_i->second;
    }
    break;

  default:
    error("Mem::read_data() default");
  }

  // default value
  Mem::Mem_Data def;

  return def;
}

void Mem::write_data(const int &address, const Mem::Mem_Data &data){
  switch( reexec.mode() ){
  case Normal:
    mem[address] = data;
    break;

  case No:
  case Fork:
    mem_diff[address] = data;
    break;

  default:
    error("Mem::store() re default");
  }
}

void Mem::print_mem_size(){
  std::cout << "memory buffer:" <<  mem.size() << std::endl;
}


///////////////////////////////////////////////////////////////////////////////

//
// class Store_Thread
//

// destructor
Store_Thread::~Store_Thread(){
  st_tid_table.clear();
  one_store_c.clear();
  more_store_c.clear();
}

// あるスレッドについて、storeが複数回実行されるかどうかを記録
void Store_Thread::add_store_pc(const int &st_pc, const int &write_time){
  MI map_i;

  switch( reexec.mode() ){
  case Normal:
    if( model.debug(20) ){
      std::cout << "Store_Thread::add_store_pc " << std::hex << st_pc << std::dec << std::endl;
    }

    if( ( map_i = more_store_c.find(st_pc) ) != more_store_c.end() ){
      // 複数出現
      // min write timeを更新
      map_i->second = std::min( map_i->second, write_time );
    }else{
      // 複数出現していない
      if( ( map_i = one_store_c.find(st_pc) ) != one_store_c.end() ){
	// 2回出現したので、複数に登録、min write timeを更新
	more_store_c[st_pc] = std::min( map_i->second, write_time );
      }else{
	// 一度も出現されていないので、登録、write timeを記録
	one_store_c[st_pc] = write_time;
      }
    }
    break;

  case No:
  case Fork:
    break;

  default:
    error("Store_Thread::add_tid() default switch");
  }
}

// 同一スレッド内に複数実行される場合について、st_pc, thread_idを登録して、
// 一時記憶を消去
void Store_Thread::init_thread(const int &thread_id){
  // 同一スレッド内に複数実行されるst_pcに対応するエントリにthread_idを登録
  switch( reexec.mode() ){
  case Normal:
    for( MI map_i = more_store_c.begin();
	 map_i != more_store_c.end(); map_i ++ ){
      const int st_pc = map_i->first, min_write_time = map_i->second;

      // id, min write timeをテーブルに登録
      st_tid_table[st_pc].insert( std::make_pair(thread_id, min_write_time) );

      // 複数出現するものは、一回出るに登録されているので、重複を削除
      one_store_c.erase(st_pc);
    }

    // すべての先行スレッドでストアが一度だけ実行された場合
    // 対応するst_pcのエントリのみを作成、HMAP内のMAPはempty()
    for( MI map_i = one_store_c.begin();
	 map_i != one_store_c.end(); map_i ++ ){
      const int st_pc = map_i->first;

      if( st_tid_table.find(st_pc) == st_tid_table.end() ){
	st_tid_table[st_pc];
      }
    }

    // 前回のスレッド内の一時記憶を消去
    one_store_c.clear();
    more_store_c.clear();
    break;

  case Fork:
    break;

  default:
    error("Store_Thread::init_thread() default switch");
  }
}


// スレッド内の静的なストアが一つの場合、true
bool Store_Thread::search(const Mem::Mem_Data &mem_data){
  const int st_pc = mem_data.pc, st_tid = mem_data.thread;
  HMI map_i;

  switch( reexec.mode() ){
  case Fork:
    if( c.thread == st_tid + 1 ){
      MI mi;

      if( ( mi = more_store_c.find(st_pc) ) != more_store_c.end() ){
	const int min_write_time = mi->second;

	return( min_write_time == mem_data.write );
      }else if( one_store_c.find(st_pc) != one_store_c.end() ){
	// スレッド内のストアは単独
	return true;
      }else{
	// 待ち合わせのストアが存在しない
	std::cerr << st_tid << " " << std::hex << st_pc << std::dec << std::endl;
	print();
	error("Store_Thread::search() not found");
      }
    }//else{ -> Normalと同様
  case Normal:
  case No:
    if( ( map_i = st_tid_table.find(st_pc) ) != st_tid_table.end() ){
      MI mi;

      if( ( mi = map_i->second.find(st_tid) ) != map_i->second.end() ){
	// このスレッド内に静的なst_pcが複数実行された
	// 最小時刻と同じならblindでも成功する
	const int min_write_time = mi->second;

	return( min_write_time == mem_data.write );
      }else{
	// スレッド内のストアは単独
	return true;
      }
    }else{
      // 待ち合わせのストアが存在しない
      std::cerr << "normal: tid:" << st_tid
	   << " st_pc:" << std::hex << st_pc << std::dec << std::endl;
      print();
      error("Store_Thread::search() not found");
    }
    break;// case Fork. Normal, No

  default:
    error("Store_Thread::search() default switch");
  }
}


void Store_Thread::print(){
  if( st_tid_table.empty() ){
    std::cout << "st_tid_table.empty()" << std::endl;
  }else{
    for( HMI map_i = st_tid_table.begin();
	 map_i != st_tid_table.end(); map_i ++ ){
      std::cout << std::hex << map_i->first << std::dec << ":";

      for( MI mi = map_i->second.begin(); mi != map_i->second.end(); mi ++ ){
	std::cout << mi->first << ", ";
      }
      std::cout << std::endl;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////

//
// class Reg_Count (count phisycal register and restrict limit)
//

// physical register counter
void Reg_Count::counter(const Reg_Data &reg_dest, const int &commit){
  if( model.reg != Reg_Finite ){
    return;
  }

  // 分岐ミスによる制限のほうが厳しい場合、finite_limitを更新
  finite_limit = std::max(finite_limit, t.cd_limit);

  int commit_order = std::max(finite_limit, commit);

  // commit orderの計算
  if( reg_dest.flag & DEFINE || !c.thread ){
    commit_order = std::max(commit_order, reg_dest.write);
  }

  if( model.debug(20) && reexec.mode() == Normal ){
    std::cerr << "limit: " << finite_limit
	 << " commit: " << commit_order << std::endl;
  }

#ifdef UNDEF
  // oracle mode 物理レジスタをたくさんはかりたいとき
  if( sim_type == ORACLE ){
    static int count_array[10000000];

    if( commit_order >= 10000000 ){
      error("");
    }

    for( int i = finite_limit; i <= commit_order; i ++ ){
      count_array[i] ++;
      if( count_array[i] == model.reg_physical ){
	finite_limit = i + 1;
      }
    }
    if( commit_order < finite_limit ){
      commit_order = finite_limit;
      count_array[commit_order] = 1;
    }

    return;
  }
  error("Reg_Count::counter() oracle mode");
#endif

  // finite_limit 〜 commit_orderのサイクル区間に物理レジスタを一つ割り当てる
  switch( reexec.mode() ){
  case Normal:
    // finite_limitの制限が厳しい場合、不要な要素を削除
    while( count.size() && count.front().end < finite_limit ){
      count.pop_front();
    }

    if( !count.size() ){
      // countの初期化
      count.push_back( Range(finite_limit, commit_order) );
    }else{
      // 不要な要素内の範囲を削除
      if( count.front().start < finite_limit ){
	count.front().start = finite_limit;
      }

      // 終端要素の新規追加
      if( count.back().end < commit_order ){
	count.push_back( Range(count.back().end + 1, commit_order) );
      }
    }

    // 物理レジスタを1つ割り当てる
    for( LI list_i = count.begin();
	 list_i != count.end(); list_i ++ ){
      list_i->reg ++;

      if( commit_order == list_i->end ){
	break;
      }else if( commit_order < list_i->end ){
	// 要素を分割
	LIST next;
	Range range(commit_order + 1, list_i->end, list_i->reg - 1);

	// 現在の要素のendを更新
	list_i->end = commit_order;
	// 挿入する要素一つのリスト
	next.push_back(range);
	// 次の要素の手前にリスト"next"を挿入
	// LI == LIST.end() の場合は、LIST.push_back(range) と同様
	list_i ++;
	count.splice(list_i, next);
	break;
      }
    }

    // レジスタを割り当てることができない
    if( count.front().reg == model.reg_physical ){
      // finite_limitを更新後、先頭要素を削除
      finite_limit = count.front().end + 1;
      count.pop_front();
    }
    break;

  case No:
  case Fork:
    // finite_limitの制限が厳しい場合、不要な要素を削除
    while( count_diff.size() && count_diff.front().end < finite_limit ){
      count_diff.pop_front();
    }

    if( !count_diff.size() ){
      // count_diffの初期化
      count_diff.push_back( Range(finite_limit, commit_order) );
    }else{
      // 不要な要素内の範囲を削除
      if( count_diff.front().start < finite_limit ){
	count_diff.front().start = finite_limit;
      }

      // 終端要素の新規追加
      if( count_diff.back().end < commit_order ){
	count_diff.push_back( Range(count_diff.back().end + 1, commit_order) );
      }
    }

    // 物理レジスタを1つ割り当てる
    for( LI list_i = count_diff.begin();
	 list_i != count_diff.end(); list_i ++ ){
      list_i->reg ++;

      if( commit_order == list_i->end ){
	break;
      }else if( commit_order < list_i->end ){
	// 要素を分割
	LIST next;
	Range range(commit_order + 1, list_i->end, list_i->reg - 1);

	// 現在の要素のendを更新
	list_i->end = commit_order;
	// 挿入する要素一つのリスト
	next.push_back(range);
	// 次の要素の手前にリスト"next"を挿入
	// LI == LIST.end() の場合は、LIST.push_back(range) と同様
	list_i ++;
	count_diff.splice(list_i, next);
	break;
      }
    }

    // レジスタを割り当てることができない
    if( count_diff.front().reg == model.reg_physical ){
      // finite_limitを更新後、先頭要素を削除
      finite_limit = count_diff.front().end + 1;
      count_diff.pop_front();
    }
    break;

  default:
    error("Reg_Count::counter() re default");
  }

  if( false && model.debug(20) && reexec.mode() == Normal ){
    for( LI list_i = count.begin(); list_i != count.end(); list_i ++ ){
      std::cerr << list_i->start << "," << list_i->reg << ":";
    }
    std::cerr << count.back().end << std::endl;
  }
}

// change thread
void Reg_Count::init_thread(const int &limit){
  if( model.reg != Reg_Finite ){
    return;
  }

  switch( reexec.mode() ){
  case Normal:
    count.clear();
    break;

  case Fork:
    count_diff.clear();
    break;

  default:
    error("Reg_Count::init() re default");
  }

  finite_limit = limit;
}

void Reg_Count::backup(){
  if( model.reg != Reg_Finite ){
    return;
  }

  if( model.debug(20) ){
    std::cerr << "Reg_Count::backup()" << std::endl;
  }

  finite_limit_bak = finite_limit;
  count_diff.clear();
  count_diff = count;
}

void Reg_Count::restore(){
  if( model.reg != Reg_Finite ){
    return;
  }

  switch( reexec.mode() ){
  case Finish:
    // re = ForkからFinishに遷移
    if( model.debug(20) ){
      std::cerr << "Reg_Count::restore() Finish not restore" << std::endl;
    }
    // restore data
    finite_limit = finite_limit_bak;
    break;

  case Change:
    // No -> Change -> (Fork) に遷移
    if( model.debug(20) ){
      std::cerr << "Reg_Count::restore() Change copy count to count_diff" << std::endl;
    }
    break;

  default:
    error("Reg_Count::restore() default");
  }
}
