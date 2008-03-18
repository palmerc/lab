// -*- C++ -*-
//
// data.h mem/reg
//

#ifndef DATA_H
#define DATA_H

#include <map>
#include "bb.h"
#include "trace.h"

//
// class Mem_Region (static/dynamic memory reference inst region type)
//

// ADDRESS: REGION
// 0x00000000 unused
// 0x00400000 start of text segment
// 0x10000000 start of data segment
// 0x7fffc000 stack base (grows down)

class Mem_Region{
  enum Mem_Region_Type{
    None   = 0x00,// not found
    R_Only = 0x01,// global region read only
    Global = 0x02,// global region load/store
    Stack  = 0x04,// stack region
    Heap   = 0x08 // heap region
  };

  static const int max_stack_region = 0x7fffc000;
  int min_stack_region;
  int max_heap_region;
  int min_heap_region;
  int max_global_region;
  static const int min_global_region = 0x10000000;

  class Mem_Address{
    Mem_Region_Type reg_type;
    Mem_Region_Type region_type;
    int freq;

  public:
    Mem_Address(const Mem_Region_Type &);

    void add_region(const Mem_Region_Type &);
    void print_result();
  };

  // define
  typedef std::map< int, Mem_Address > MAP;
  typedef MAP::iterator MI;

  // total function size
  int func_size;

  MAP *load_region;
  MAP *store_region;

public:
  // const/destructor
  Mem_Region(Program_Info &);
  ~Mem_Region();

  void check_region(const Pipe_Inst &, Program_Info &);
  void file_write(Program_Info &);

private:
  void check_load_region(const Pipe_Inst &, Program_Info &);
  void check_store_region(const Pipe_Inst &, Program_Info &);
};

#endif
