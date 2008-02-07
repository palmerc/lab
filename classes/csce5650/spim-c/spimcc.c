#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "parser.h"

int main (int argc, const char* argv[]) {
	char* clinput;
		
	if( argc == 2 ) {
		clinput = strdup(argv[1]);
		
		yyin = fopen(clinput, "r");
		if( yyin != NULL ) {
			yyparse();
		} else {
			abort();
		}
		fclose(yyin);
	} 

	return 0;
}
