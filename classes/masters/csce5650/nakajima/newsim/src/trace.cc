//
// trace.cc:
//
//  Time-stamp: <03/12/05 02:14:36 nakajima>
//

#include <unistd.h>

#include "trace.h"

// pipe
void child_process(char **cut_args){
  int simdes[2];
  extern char **environ;

  // create pipe
  pipe(simdes);

  if( !fork() ){
    // child
    close(simdes[0]);
    dup2(simdes[1], sim_fd);
    close(simdes[1]);
    execve(cut_args[0], cut_args, environ);
  }

  // parent
  close(simdes[1]);
  dup2(simdes[0], sim_fd);
  close(simdes[0]);
}
