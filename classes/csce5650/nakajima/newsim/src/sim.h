// -*- C++ -*-
//
// sim.h
//
//  Time-stamp: <04/02/04 19:02:14 nakajima>
//

#ifndef SIM_H
#define SIM_H

using namespace std;

#include <string>

//
// class Sim_Model simulation model
//

// simulation processor type
enum Sim_Type{
  BASE   = 0x00,// base
  SP     = 0x01,// sp (branch prediction)
  CD     = 0x02,// (control dependence)
  MF     = 0x04,// (multiple control-flow)
  ORACLE = 0x08,// oracle 

  SP_CD  = 0x03,// sp + cd
  CD_MF  = 0x06,// cd + mf (posdom)

  PD     = 0x07,// PD (posdom sp + cd + mf)
  LP     = 0x17,// LP (loop sp + cd + mf)
  FC     = 0x27,// FC (func sp + cd + mf)
  CE     = 0x47 // CE (ctrl-eq sp + cd + mf)
};

// data dependence model
enum Data_Model{
  // 物理レジスタに関するモデル
  Reg_None,
  Reg_Finite,
  Reg_Perfect,
  // スレッド内メモリ依存に関するモデル
  SP_Sequential,
  SP_Reorder,
  SP_Perfect,
  // スレッド間メモリ依存に関するモデル
  MP_Blind,
  MP_Analysis,
  MP_Predict,
  MP_Perfect,
  MP_Profile,
  // 値予測
  VP_Nopred,
  VP_Send,  // 通信値予測
  VP_Result // 通信値を必要とする命令の結果値予測
};

// 投機的実行
enum Sp_Exec{
  SP_None = 0x00,// 非投機
  SP_Fork = 0x01,// 投機的スレッド生成
  SP_Send = 0x02,// 投機的通信
  SP_Both = 0x03 // SP_Fork + SP_Send
};

// simulation model
class Sim_Model{
  string time_stamp;
  string fix_me;
  string argv0;
  string data_dir;

public:
  // filename
  string bb_info;
  string posdom_out;
  string ctrleq_out;
  string icd_info;
  string brn_pred;
  string loop_del_pc;
  string mp_analysis;
  string val_pred_send;
  string val_pred_result;
  string bb_statistic;

  // branch pred static
  bool static_brn_pred;

  // inline unrolling
  // stack操作(add), call/return命令を削除
  bool func_inline;
  // loop unrolling
  bool loop_unroll;
  // indirect control dependence
  bool icd;

  // sp,gp perfect disambiguate
  bool perf_disamb;

  // 投機的fork, 投機的send
  Sp_Exec sp_exec;
  // send, fork latency
  int send_latency;
  int fork_latency;

  // data dependence model
  Data_Model reg;// 物理レジスタに関するモデル
  Data_Model sp;// スレッド内メモリ依存に関するモデル
  Data_Model mp;// スレッド間メモリ依存に関するモデル
  Data_Model vp;// 値予測

  // 物理レジスタ数
  int reg_physical;
  // MP_Analysis, MP_Predict store num
  int mp_store_num;
  // MP_Predict warmup/no warmup
  bool mdpred_warmup;
  // MP_Predict 複数の静的なストアがあっても理想的に同期をとることができる
  bool mdpred_perf;
  // value predictor threshold
  int val_pred_th;

  // reexec mode
  bool reexec;
  // reexec mode spool size
  int spool_size;

  // シミュレーションする命令数
  int trace_limit;
  // trace skip
  int fastfwd_tc;

  // print
  int debuglevel;
  int print_freq;
  int print_freq_def;

  // 統計
  bool statistic;

public:
  // constructor
  Sim_Model();

  // argment check
  void arg_check(const int &, char **);
  // print
  void print_argment();
  // debug flag
  const bool debug(const int &);
  void set_mp_profile();

private:
  const int check_atoi(const string);
  void usage();
};

//
// extern
//

extern Sim_Type sim_type;
extern Sim_Model model;

#endif
