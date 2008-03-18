// -*- C++ -*-
//
// data.h mem/reg
//
//  Time-stamp: <04/01/22 16:25:28 nakajima>
//

#ifndef DATA_H
#define DATA_H

#include <fstream>
#include <map>
#include <set>
#include <list>
#include "bb.h"
#include "main.h"
#include "trace.h"

//
// class Mem/Mem_Data (store write time)
//

class Mem{
public:
  class Mem_Data{
  public:
    // PUBLIC
    int pc;     // store pc
    int write;  // store write time
    int thread; // thread id

    // constructor
    Mem_Data( int p = 0, int wt = 0, int tid = 0){
      pc = p;
      write = wt;
      thread = tid;
    }
  };
  // define
  typedef std::map< int, Mem_Data > MAP;
  typedef MAP::iterator MI;

  // a[write address].{ Mem_Data }
  MAP mem;// mem (original data)
  MAP mem_diff;// mem diff (for reexec mode)

public:
  // destructor
  ~Mem(){
    mem.clear();
    mem_diff.clear();
  }
  void clear_diff() { mem_diff.clear(); }

  // load/store
  const Mem_Data read_data(const int &);
  void write_data(const int &, const Mem_Data &);

  // mem size
  void print_mem_size();
};


// ある静的なstoreについて、同一スレッド内に複数実行される場合に登録
class Store_Thread{
  // define
  typedef std::map< int, int > MAP;
  typedef MAP::iterator MI;
  typedef std::map< int, MAP > HMAP;
  typedef HMAP::iterator HMI;

  // a[store_pc][thread_id]{ min store time }
  HMAP st_tid_table;

  // あるスレッドについて、storeが一度実行されると記録/複数実行で記録
  // a[store_pc]{ min store time }
  MAP one_store_c;
  MAP more_store_c;

public:
  // const/destructor
  ~Store_Thread();

  // あるスレッドについて、storeが複数回実行されるかどうかを記録
  void add_store_pc(const int&, const int&);

  // 同一スレッド内に複数実行される場合について、st_pc, thread_idを登録して、
  // 一時記憶を消去
  void init_thread(const int &);

  // スレッド内の静的なストアが一つの場合、true
  bool search(const Mem::Mem_Data &);

private:
  void print();
};



//
// class Mem_Dep_MP (blind/analysis)
//
enum Mem_Viol{
  // 優先順位あり
  NODEP, // 先行スレッドと依存なしの状態
  DEP,   // 先行スレッドと依存あり、ただし、同期をとる必要はない
  SYNC,  // スレッド間メモリ依存あり、、analysis modeで回避 (同期)
  VIOL,  // スレッド間メモリ依存違反が発生し、先行するストア命令の実行を待つ
};


//
// class Store_Thread
//
class Mem_Dep_MP{
  // define
  typedef std::list< int > LIST;
  typedef LIST::iterator LI;
  typedef std::map< int, LIST > MAP;
  typedef MAP::iterator MI;

  // total function size
  int func_size;

  unsigned store_num;

  // a[ld_func][ld_pc].{ set of store_pcs }
  MAP *profile;

  // for reexec mode
  // a[ld_pc].{ set of store_pcs }
  MAP profile_diff;

  // mp blind, analysis
  Mem_Viol viol_flag;
  // memory violation limit
  int mem_limit;
  int last_mem_limit;

  // counter (thread)
  int mem_dep;
  int mem_viol;
  int mem_sync;
  int mem_nodep;

  // counter (load inst)
  int mem_viol_load;
  int mem_dep_load;

  // for reexec mode
  Mem_Viol viol_flag_bak;
  int mem_limit_bak;

public:
  // constructor
  Mem_Dep_MP();
  ~Mem_Dep_MP();

  // init analysis
  void file_read();
  // init md_predict
  void allocate();

  // thread change
  void init_thread(const int &);

  // get
  const int get_viol_limit();
  const int get_last_limit() { return last_mem_limit; }
  const int get_viol_load() { return mem_viol_load; }
  const Mem_Viol flag() { return viol_flag; }

  // mem dependence
  void blind(const Pipe_Inst &, int &);

  // static analysis
  void analysis(const Pipe_Inst &, int &);

  // mem dependence predict
  void md_predict(const Pipe_Inst &, Store_Thread &, int &);

public:
  // for reexec mode
  void backup(){
    mem_limit_bak = mem_limit;
    viol_flag_bak = viol_flag;
  }
  void restore(){
    mem_limit = mem_limit_bak;
    viol_flag = viol_flag_bak;
    profile_diff.clear();
  }

  void print_viol();

private:
  void print();
};



//
// class Mem_Profile (make mp_analysis profile)
//

class Mem_Profile{
  class Profile_Data{
    // define
    typedef std::map< int, int > MAP;
    typedef MAP::iterator MI;

    // counter
    MAP profile_data;
    int total;

  public:
    // const/destructor
    Profile_Data() { total = 0; }
    ~Profile_Data() { profile_data.clear(); }

    void add_data(const int &);
    void file_write(std::ofstream &fout);
  };

  typedef std::map< int, Profile_Data > MAP;
  typedef MAP::iterator MI;

  // total function size
  int func_size;

  MAP *profile;

public:
  // const/destructor
  Mem_Profile();
  ~Mem_Profile();

  void allocate();

  // count instruction
  void count_inst(const Pipe_Inst &, const int &);
  // file out
  void file_write();
};


//
// class Reg_Count (register finite model)
//

class Reg_Count{
  class Range{
  public:
    int start;
    int end;
    int reg;

  public:
    Range(int s, int e, int r = 0) { start = s; end = e; reg = r; }
  };

  // define
  typedef std::list< Range > LIST;
  typedef LIST::iterator LI;

  // physical register counter
  LIST count;
  LIST count_diff;// for reeexec mode

  // cycle limit
  int finite_limit;
  // for reexec mode
  int finite_limit_bak;

public:
  // const/destructor
  Reg_Count() { finite_limit = 0; }
  ~Reg_Count() { count.clear(); count_diff.clear(); }

  // count phisical register and restrict limit
  void counter(const Reg_Data &, const int &);

  // get limit
  const int get_finite_limit() { return finite_limit; }
  // change thread
  void init_thread(const int &);

  // for reexec mode
  void backup();
  void restore();
};


//
// class Mem_Dep_SP (reorder, sequential)
//

class Mem_Dep_SP{
public:
  // last type LASD or STORE
  Funit_Type last_type;

  // sequential: max of data time
  // reorder: max of address time
  int load;
  int store;

  // mp_analysis: 全ストア命令の実行完了時刻
  int store_max;
  // mp_analysis: 先行スレッドの全ストア命令の実行完了時刻
  int last_store_max;

private:
  // for reexec mode
  Funit_Type last_type_bak;
  int load_bak;
  int store_bak;
  int store_max_bak;
  int last_store_max_bak;

public:
  // constructor
  Mem_Dep_SP(){
    load = store = store_max = last_store_max = 0;
    last_type = NOOP;
  }

  // init
  void init_thread(){
    last_type = NOOP;
    load = store = 0;
  }

  // for reexec mode
  void backup() {
    last_type_bak = last_type;
    load_bak = load;
    store_bak = store;
    store_max_bak = store_max;
    last_store_max_bak = last_store_max;
  }
  void restore() {
    last_type = last_type_bak;
    load = load_bak;
    store = store_bak;
    store_max = store_max_bak;
    last_store_max = last_store_max_bak;
  }
};

//
// extern
//
extern Mem memory;
extern Reg_Count reg_finite;
extern Mem_Dep_SP mem_sp;
extern Mem_Dep_MP mem_mp;
extern Mem_Profile mem_profile;
extern Store_Thread store_tid;

#endif
