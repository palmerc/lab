//
//  re.cc reexec mode
//

#include <unistd.h>
#include <iostream>
#include "re.h"

#include "main.h"

//
// class Re_Exec
//

// spool next instruction
const bool Re_Exec::check(Pipe_Inst &inst){
  if( read( sim_fd, &(inst), sizeof(inst) ) == sizeof(inst) ){
    trace_count ++;
  }else{
    return false;// exit main loop
  }

  if( model.trace_limit < trace_count ){
    trace_count --;
    return false;// exit main loop
  }else{
    return true;
  }
}

void Re_Exec::init_trace_skip(){
  if( model.fastfwd_tc ){
    std::cerr << "Re_Exec::init_trace_skip() trace skip: "
	 << model.fastfwd_tc << " " << std::flush;

    Pipe_Inst inst;

    for( int tc = 1; tc <= model.fastfwd_tc; tc ++ ){// trace skip
      if( read( sim_fd, &(inst), sizeof(inst) ) != sizeof(inst) ){
	error("trace_skip() read Pipe_Inst");
      }

      if( tc % model.print_freq_def == 0 ){
	std::cerr << "." << std::flush;
      }
    }

    std::cerr << std::endl;
  }
}
