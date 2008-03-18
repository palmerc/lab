//
// loop.cc
//

#include <iostream>
#include "loop.h"

#include "main.h"

//
// class Loop
//

// constructor
Loop::Loop(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);
  bedge = program.backward_edge(func);
}

// destructor
Loop::~Loop(){
  bedge.clear();
  clear();
}

// clear
void Loop::clear(){
  loop_bbs.clear();
  exit_pcs.clear();
  const_pcs.clear();
  unify_pcs.clear();
  basic_induct_pcs.clear();
  induct_pcs.clear();
}

// analysis all loop of a function
void Loop::analysis(std::ofstream &fout, Program_Info &program, DU_Chain &du_chain,
		    Inst_Type &inst_type){
  fout << "{" << func << ":" << fname << std::endl;

  for( bedge_i = bedge.begin(); bedge_i != bedge.end(); bedge_i ++ ){
#ifdef DEBUG
    // loop tail
    fout << bedge_i->second << ":";
#endif
    // loop header
    fout << bedge_i->first << ":";

    // search natural loop
    if( !natural_loop(fout, program) ){
      clear();
      fout << ";" << std::endl;
      continue;
    }

    // search exit branch
    search_exit_branch(fout, program, inst_type);

    // search constant variable
    const_variable(fout, program, du_chain, inst_type);

    // basic induction variable
    // search candidate induct variable pcs
    candidate_pcs(program, du_chain, inst_type);

#ifdef DEBUG
    // candidate pcs
    fout << "candidate B is[";
    for( SI set_i = unify_pcs.begin(); set_i != unify_pcs.end(); set_i ++ ){
      fout << std::hex << *set_i << std::dec << ",";
    }
    fout << "] ";
#endif

    if( !unify_pcs.empty() ){
      // search basic induction variable
      basic_induct_variable(fout, program, du_chain, inst_type);
    }

    // delete candidate pcs
    for( SI set_i = basic_induct_pcs.begin();
	 set_i != basic_induct_pcs.end(); set_i ++ ){
      unify_pcs.erase(*set_i);
    }

    // induct variable
    if( !unify_pcs.empty() ){
#ifdef DEBUG
      // candidate pcs
      fout << "candidate I is[";
      for( SI set_i = unify_pcs.begin(); set_i != unify_pcs.end(); set_i ++ ){
	fout << std::hex << *set_i << std::dec << ",";
      }
      fout << "] ";
#endif

      if( !basic_induct_pcs.empty() ){
	// search induction variable
	induct_variable(fout, program, du_chain, inst_type);

	// 作成していない
	if( !induct_pcs.empty() ){
	  error("Loop::print() induct_pcs not empty");
	}
      }
    }

    fout << ";" << std::endl;
    clear();
  }

  model.fout_loop << "}" << std::endl;
}

// search natural loop
bool Loop::natural_loop(std::ofstream &fout, Program_Info &program){
  // "n --> d" is backward edge (n >= d)
  // d: header, n: tail
  const int d = bedge_i->first, n = bedge_i->second;

  // construct
  STACK st;
  SET loop_succ;
  bool check = false;

  if( n == d ){
    loop_succ.insert(n);
    check = true;
  }else{
    // dからnに到達可能であるかチェック
    loop_succ.insert(n);
    sub_insert(loop_succ, st, d);

    while( !st.empty() ){
      // construct
      const int m = st.top();
      Program_Info::SET succ = program.get_succ(Func_Bb(func, m));

      st.pop();

      for( Program_Info::SI set_i = succ.begin();
	   set_i != succ.end(); set_i ++ ){
	int bb = *set_i;

	sub_insert(loop_succ, st, bb);
	if( bb == n ){
	  // d --> ... --> n
	  check = true;
	}
      }
    }
  }

#ifdef DEBUG
  // loop bbs
  fout << "loop succ[";
  for( SI set_i = loop_succ.begin(); set_i != loop_succ.end(); set_i ++ ){
    fout << *set_i << ",";
  }
  fout << "] ";
#endif

  if( !check ){
#ifdef DEBUG
    fout << bedge_i->second << "->" << bedge_i->first
	 << ":not loop backward edge";
#endif
    return false;
  }

  // search natural loop (predecessor)
  loop_bbs.insert(d);
  sub_insert(loop_bbs, st, n);

  while( !st.empty() ){
    // construct
    const int m = st.top();
    Program_Info::SET pred = program.get_pred(Func_Bb(func, m));

    st.pop();

    for( Program_Info::SI set_i = pred.begin();
	 set_i != pred.end(); set_i ++ ){
      sub_insert(loop_bbs, st, *set_i);
      // ヘッダ以外にloopの入口がある場合
      // headerが0ではないが、0にたどり着いてしまう
      if( d != 0 && *set_i == 0 ){
  	check = false;
      }
    }
  }

#ifdef DEBUG
  // loop bbs
  fout << std::dec << "loop bbs[";
  for( SI set_i = loop_bbs.begin(); set_i != loop_bbs.end(); set_i ++ ){
    fout << *set_i << ",";
  }
  fout << "] ";
#endif

  if( !check ){
#ifdef DEBUG
    fout << ":not natural loop";
#endif
    return false;
  }

  return true;
}

// natural loop subroutine
void Loop::sub_insert(SET &lp, STACK &st, const int &m){
  // "m" not include "loop"
  if( !lp.count(m) ){
    lp.insert(m);// loop = loop | {m}
    st.push(m);
  }
}

// search exit branch
void Loop::search_exit_branch(std::ofstream &fout, Program_Info &program,
			      Inst_Type &inst_type){
  // loopから出る分岐命令
  for( SI set_i = loop_bbs.begin(); set_i != loop_bbs.end(); set_i ++ ){
    const int loop_bb = *set_i;
    Program_Info::SET succ = program.get_succ(Func_Bb(func, loop_bb));

    for( Program_Info::SI set_i = succ.begin();
	 set_i != succ.end(); set_i ++ ){
      const int succ_bb = *set_i;

      if( !loop_bbs.count(succ_bb) ){
	Bb_Info info = program.get_info(Func_Bb(func, loop_bb));
	const int end_pc = info.end;
	Inst_Data type = inst_type.operand(end_pc);

	if( type.op == Branch ){
	  exit_pcs.insert(end_pc);
	}else if( type.op != Jump ){
	  std::cerr << std::endl << std::hex << end_pc << std::dec << std::endl;
	  error("Loop::search_exit_branch() exit pcs not branch");
	}
      }
    }
  }

  // exit branch pc
  for( SI set_i = exit_pcs.begin(); set_i != exit_pcs.end(); set_i ++ ){
    fout << "E" << std::hex << *set_i << std::dec << ",";
  }
}

// search constant variable
// algorithm 10.7
void Loop::const_variable(std::ofstream &fout, Program_Info &program,
			  DU_Chain &du_chain, Inst_Type &inst_type){
  // 定数、または全ての定義がLの外側から到達する演算だけで
  // 構成されている文に、不変の印をつける
  for( SI set_i = loop_bbs.begin(); set_i != loop_bbs.end(); set_i ++ ){
    Func_Bb fbb(func, *set_i);
    Bb_Info info = program.get_info(fbb);
    DU_Chain::MMAP use = du_chain.get_use(fbb.bb);

    for( int pc = info.start; pc <= info.end; pc += byte ){
      DU_Chain::MI_PAIR use_pair = use.equal_range(pc);
      Inst_Data type = inst_type.operand(pc);

      // B, J, JAL, JRを除外
      if( type.op & Ctrl ){
	continue;
      }

      if( use.count(pc) ){
	bool flag = true;

	for( DU_Chain::MI map_i = use_pair.first;
	     map_i != use_pair.second; map_i ++ ){
	  DU_Data def = map_i->second;

	  if( loop_bbs.count(def.bb) ){
	    flag = false;
	    break;
	  }
	}

	if( flag ){
	  const_pcs.insert(pc);
	}
      }else{
	const_pcs.insert(pc);
      }
    }
  }

  SET old_pcs;

  while( old_pcs != const_pcs ){
    old_pcs = const_pcs;

    // 不変の印のついた新しい文が出尽くすまで、以下を繰り返す
    for( SI set_i = loop_bbs.begin(); set_i != loop_bbs.end(); set_i ++ ){
      Func_Bb fbb(func, *set_i);
      Bb_Info info = program.get_info(fbb);
      DU_Chain::MMAP use = du_chain.get_use(fbb.bb);

      for( int pc = info.start; pc <= info.end; pc += byte ){
	if( const_pcs.count(pc) ){
	  continue;
	}
	// 不変の印のついていない文について
	// 到達定義が1つだけで、その定義には、Lのなかで既に
	// 不変の印がついているもの
	if( use.count(pc) == 1 ){
	  DU_Chain::MI map_i = use.equal_range(pc).first;
	  DU_Data def = map_i->second;

	  if( const_pcs.count(def.pc) ){
	    const_pcs.insert(pc);
	  }
	}
      }
    }
  }

  // constant variable
  for( SI set_i = const_pcs.begin(); set_i != const_pcs.end(); set_i ++ ){
    fout << "C" << std::hex << *set_i << std::dec << ",";
  }
}

// search candidate induction variable pcs
void Loop::candidate_pcs(Program_Info &program, DU_Chain &du_chain,
			 Inst_Type &inst_type){
  SET def_reg_pcs[REG];

  // 単一代入のpcかつ演算が和差積商であるものを探しておく
  for( SI set_i = loop_bbs.begin(); set_i != loop_bbs.end(); set_i ++ ){
    Func_Bb fbb(func, *set_i);
    Bb_Info info = program.get_info(fbb);
    DU_Chain::MMAP use = du_chain.get_use(fbb.bb);

    for( int pc = info.start; pc <= info.end; pc += byte ){
      DU_Chain::MI_PAIR pair = use.equal_range(pc);

      for( DU_Chain::MI map_i = pair.first; map_i != pair.second; map_i ++ ){
	DU_Data def = map_i->second;

	if( loop_bbs.count(def.bb) ){
	  def_reg_pcs[def.reg].insert(def.pc);
	}
      }
    }
  }

  for( int reg = 0; reg < REG; reg ++ ){
    // 定義する命令のpcが集合Loop内で一つのみ
    if( def_reg_pcs[reg].size() == 1 ){
      SI set_i = def_reg_pcs[reg].begin();
      int pc = *set_i;
      Inst_Data type = inst_type.operand(pc);

      // check inst type (+-*/)
      switch( type.op ){
      case Add:
      case Sub:
      case Mult:
      case Div:
	if( !const_pcs.count(pc) ){
	  unify_pcs.insert(pc);
	}
	break;

      default:
	break;
      }
    }
  }
}

// search basic induction variable
// algorithm 10.9
void Loop::basic_induct_variable(std::ofstream &fout, Program_Info &program,
				 DU_Chain &du_chain, Inst_Type &inst_type){
  // search ALL basic induction variable
  for( SI set_i = unify_pcs.begin(); set_i != unify_pcs.end(); set_i ++ ){
    int pc = *set_i;
    Func_Bb fbb(func, program.search_bb(func, pc));
    DU_Chain::MMAP use = du_chain.get_use(fbb.bb);
    DU_Chain::MI_PAIR pair = use.equal_range(pc);
    Inst_Data type = inst_type.operand(pc);

    switch( type.op ){
    case Sub:
      // i = i - c
      if( type.dest == type.src1 ){
	if( type.src2 < 0 ){
	  // c is 0x##
	  basic_induct_pcs.insert(pc);
	}else if( check_const(pair, type.src2) ){
	  // c is const_pcs
	  basic_induct_pcs.insert(pc);
	}
      }
      break;

    case Add:
      if( type.dest == type.src1 ){
	// i = i + c
	if( type.src2 < 0 ){
	  // c is 0x##
	  basic_induct_pcs.insert(pc);
	}else if( check_const(pair, type.src2) ){
	  // c is const_pcs
	  basic_induct_pcs.insert(pc);
	}
      }else if( type.dest == type.src2 ){
	// i = c + i
	if( check_const(pair, type.src1) ){
	  // c is const_pcs
	  basic_induct_pcs.insert(pc);
	}
      }
      break;

    default:
      break;
    }
  }

  // basic induction variable
  for( SI set_i = basic_induct_pcs.begin();
       set_i != basic_induct_pcs.end(); set_i ++ ){
    fout << "B" << std::hex << *set_i << std::dec << ",";
  }
}

// check source register is constant variable
bool Loop::check_const(DU_Chain::MI_PAIR pair, const int &src){
  SET def_reg_pcs;

  for( DU_Chain::MI map_i = pair.first; map_i != pair.second; map_i ++ ){
    DU_Data def = map_i->second;

    if( def.reg == src && loop_bbs.count(def.bb) ){
      def_reg_pcs.insert(def.pc);
    }
  }

  if( def_reg_pcs.size() == 1 ){
    int pc = *def_reg_pcs.begin();

    if( const_pcs.count(pc) ){
      return true;
    }
  }

return false;
}

// search induction variable
// algorithm 10.9
void Loop::induct_variable(std::ofstream &fout, Program_Info &program,
			   DU_Chain &du_chain, Inst_Type &inst_type){
  MAP old_pcs;

  while( old_pcs != induct_pcs ){
    old_pcs = induct_pcs;

    for( SI set_i = unify_pcs.begin(); set_i != unify_pcs.end(); set_i ++ ){
      // Lの中で次の形を持つ変数kへの単一代入を全て捜し出す
      int pc = *set_i;
      Func_Bb fbb(func, program.search_bb(func, pc));
      DU_Chain::MMAP use = du_chain.get_use(fbb.bb);
      DU_Chain::MI_PAIR pair = use.equal_range(pc);
      Inst_Data type = inst_type.operand(pc);
      int pc_bi = 0;

      // 次の形: k = j [+-*/] c, k = c [+-*] j
      // j:基本誘導変数または他の誘導変数, k:ある変数, c:定数 

      // 1) jが基本誘導変数であれば、kは誘導変数
      // 2) jが基本誘導変数でなく、iの変数属に属す変数
      //    jへの代入とkへの代入の間にはiへの代入がない
      //    Lの外側にあるjの定義はkに到達しない

      ////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////
      //  1)の条件に一致するkが存在するかチェックしている。
      //  もしあれば、2)のチェックも必要であるが、SPECint95では
      //  存在していないので、2)のコードは省略している。
      ////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////

      switch( type.op ){
      case Add:
      case Sub:
      case Mult:
	if( basic_induct_pc(pair, type.src1)){
	  // k = i +-* c
	  if( type.src2 < 0 ){
	    // c is 0x##
	    induct_pcs.insert( std::make_pair(pc, pc_bi) );
	  }else if( check_const(pair, type.src2) ){
	    // c is const_pcs
	    induct_pcs.insert( std::make_pair(pc, pc_bi) );
	  }
	}else if( basic_induct_pc(pair, type.src2) ){
	  // k = c +-* i
	  if( check_const(pair, type.src1) ){
	    // c is const_pcs
	    induct_pcs.insert( std::make_pair(pc, pc_bi) );
	  }
	}
	break;

      case Div:
	if( basic_induct_pc(pair, type.src1) ){
	  // k = i / c
	  if( type.src2 < 0 ){
	    // c is 0x##
	    induct_pcs.insert( std::make_pair(pc, pc_bi) );
	  }else if( check_const(pair, type.src2) ){
	    // c is const_pcs
	    induct_pcs.insert( std::make_pair(pc, pc_bi) );
	  }
	}
	break;

      default:
	break;
      }
    }
  }

  // induction variable
  for( MI map_i = induct_pcs.begin(); map_i != induct_pcs.end(); map_i ++ ){
    fout << "I" << std::hex << map_i->first << std::dec << ",";
  }
}

// check source register is basic induction variable
bool Loop::basic_induct_pc(DU_Chain::MI_PAIR pair, const int &src){
  SET def_reg_pcs;

  for( DU_Chain::MI map_i = pair.first; map_i != pair.second; map_i ++ ){
    DU_Data def = map_i->second;

    if( def.reg == src && loop_bbs.count(def.bb) ){
      def_reg_pcs.insert(def.pc);
    }
  }

  if( def_reg_pcs.size() == 1 ){
    int pc = *def_reg_pcs.begin();

    if( basic_induct_pcs.count(pc) ){
      return true;
    }
  }

  return false;
}
