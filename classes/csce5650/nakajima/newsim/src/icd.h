// -*- C++ -*-
//
// icd.h
//
//  Time-stamp: <03/12/05 02:13:47 nakajima>
//

#ifndef ICD_H
#define ICD_H

#include <map>
#include "bb.h"

//
// class Func_Bb_Reg
//

// search func, bb, reg segment
class Func_Bb_Reg : public Func_Bb{
public:
  int reg;

public:
  // constructor
  Func_Bb_Reg( const int &f = 0, const int &b = 0, const int &r = 0 )
    : Func_Bb(f, b) { reg = r; }
  Func_Bb_Reg( const Func_Bb &fbb, const int &r = 0 )
    : Func_Bb(fbb) { reg = r; }
};

//
// class ICD indirect control dependence
//

// class icd
class ICD{
  // icd size (2-7)
  static const int icd_size = 8;

  // define
  typedef std::multimap< int, int > MMAP;
  typedef MMAP::iterator MMI;
  typedef std::pair< MMI, MMI > MMI_PAIR;

  // function size
  int func_size;

  // define icd info
  MMAP **icd;

  // icd reg write time
  int reg_icd[icd_size];
  // icd reg write time backup (for reexec mode)
  int reg_icd_bak[icd_size];

public:
  // destructor
  ICD();
  ~ICD();

  // file read
  void file_read();

  // init reg_icd[]
  void init_thread();

  // indirect control dependence
  void analysis(const Func_Bb &);
  // icd JR RETURN(v0) JALL CALL(a0) 
  void depend(const Func_Bb &, const int &);

private:
  // calc reach limit
  const int reach(const Func_Bb &, const int &);

public:
  // icd backup, restore (for reexec mode)
  void backup();
  void restore();

private:
  // check code
  void print();
};

//
// extern
//

extern ICD icd;

#endif
