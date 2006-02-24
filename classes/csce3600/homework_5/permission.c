#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main( int argc, char **argv ) {
   int arg, octy = 0;
   char* path = NULL;
   char* newmode = NULL;
   struct stat statbuf;
   mode_t modes;
      
   sscanf(argv[1], "%o", &octy);
   
   for ( arg = 2; arg < argc; arg++ ) {
      path = argv[arg];
      
      chmod(path, octy);
      
      stat(path, &statbuf);
      modes = statbuf.st_mode;
      
      printf("file: %s, now has permissions set to %o\n", path, ~(S_IFMT) & modes);
   }
   return 0;
}
