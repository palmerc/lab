#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main (int argc, const char * argv[]) {
	T type;
	
	while ((type = yylex()) != EOT) {
		switch (type) {
			case INT:
				printf("<INT %d>\n", yyint);
				break;
			case ID:
				printf("<ID %s>\n", yystring);
				free(yystring);
				break;
			case SEMI:
				printf("<SEMI>\n");
				break;
			case COMMA:
				printf("<COMMA>\n");
				break;
			case PERIOD:
				printf("<PERIOD>\n");
				break;
			case ERR:
				printf("Unrecognized token\n");
				break;
		}
	}
	
	return 0;
}
