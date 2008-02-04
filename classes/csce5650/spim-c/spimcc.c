#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main (int argc, const char * argv[]) {
	yyparse();
	return 0;
}
