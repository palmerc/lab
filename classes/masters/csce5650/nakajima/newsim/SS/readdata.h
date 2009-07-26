/***************************************************************
 *    Copyright (c) 1999 Ryotaro Kobayashi Nagoya University.  * 
 *                     All Rights Reserved.                    *
 *    Do not remove this notice.                               *
 ***************************************************************/

enum Funit_Type{
  NOOP, 
  BRANCH, 
  LOAD, 
  STORE,
  OTHER
};

char *name_Funit_Type[5]={
  "NOOP",
  "BRAN",
  "LOAD",
  "STOR",
  "OTHR"
};

enum Branch_Type
{
  CR_MISSMATCH=0,
  JALL_CALL=1,   /* function call branch */
  JR_RETURN=2,   /* function return branch */
  UNCON_DIRE=3,  /* uncondition   direct */
  UNCON_INDIRE=4,/* uncondition indirect */
  CON_DIRE=5     /*   condition   direct */
};

struct Pip_Inst_Record{
  /* function type*/
  enum Funit_Type func_unit;
  /* source/destination reg num */
  int          sourceA;
  int          sourceB;
  int          sourceC;
  int          destination;
  /* PC (word) */
  int        pc;     
  /* branch target address or load/store address */
  int        address;
  /* branch type */
  enum Branch_Type brn_type;
  /* */
  int        m_size;
  /* source/destination reg data */
  int          v_sourceA;
  int          v_sourceB;
  int          v_sourceC;
  int          v_destination;
  /* if double reg */
  char            double_regs;
};
