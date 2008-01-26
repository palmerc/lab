#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void yyparse() {
	T type;
	
	while ((type = yylex()) != EOT) {	
		switch (type) {
			case COMMENT:
				printf("<COMMENT>");
				break;
			case COMMA:
				printf("<COMMA>");
				break;
			case QUESTION:
				printf("<QUESTION>");
				break;
			case EXCLAMATION:
				printf("<EXCLAMATION>");
				break;
			case PERIOD:
				printf("<PERIOD>");
				break;
			case EQUAL:
				printf("<EQUAL>");
				break;
			case LTHAN:
				printf("<LTHAN>");
				break;
			case GTHAN:
				printf("<GTHAN>");
				break; 
			case PLUS:
				printf("<PLUS>");
				break;
			case MINUS:
				printf("<MINUS>");
				break;
			case ASTERIX:
				printf("<ASTERIX>");
				break;
			case PERCENT:
				printf("<PERCENT>");
				break;
			case CARAT:	
				printf("<CARAT>");
				break;
			case FSLASH:
				printf("<FSLASH>");
				break;
			case PIPE:
				printf("<PIPE>");
				break;
			case AMP:
				printf("<AMP>");
				break;
			case COLON:
				printf("<COLON>");
				break;
			case SEMI:
				printf("<SEMI>");
				break;
			case TILDE:
				printf("<TILDE>");
				break;
			case LBRACKET:
				printf("<LBRACKET>");
				break;
			case RBRACKET:
				printf("<RBRACKET>");
				break;
			case LCURLY:
				printf("<LCURLY>");
				break;
			case RCURLY:
				printf("<RCURLY>");
				break;
			case LPAREN:
				printf("<LPAREN>");
				break;
			case RPAREN:
				printf("<RPAREN>");
				break;
			case ID:
				printf("<ID %s>", yystring);
				break;
			case NUM:
				printf("<NUM %d>", yyint);
				break;
			case CONSTANT:
				printf("<CONSTANT>");
				break;
			case STRING_LITERAL:
				printf("<STRING_LITERAL>");
				break;
			case SIZEOF:
				printf("<SIZEOF>");
				break;
			case PTR_OP:
				printf("<PTR_OP>");
				break;
			case INC_OP:
				printf("<INC_OP>");
				break;
			case DEC_OP:
				printf("<DEC_OP>");
				break;
			case LEFT_OP:
				printf("<LEFT_OP>");
				break;
			case RIGHT_OP:
				printf("<RIGHT_OP>");
				break;
			case LE_OP:
				printf("<LE_OP>");
				break;
			case GE_OP:
				printf("<GE_OP>");
				break;
			case EQ_OP:
				printf("<EQ_OP>");
				break;
			case NE_OP:
				printf("<NE_OP>");
				break;
			case AND_OP:
				printf("<AND_OP>");
				break;
			case OR_OP:
				printf("<OR_OP>");
				break;
			case MUL_ASSIGN:
				printf("<MUL_ASSIGN>");
				break;
			case DIV_ASSIGN:
				printf("<DIV_ASSIGN>");
				break;
			case MOD_ASSIGN:
				printf("<MOD_ASSIGN>");
				break;
			case ADD_ASSIGN:
				printf("<ADD_ASSIGN>");
				break;
			case SUB_ASSIGN:
				printf("<SUB_ASSIGN>");
				break;
			case LEFT_ASSIGN:
				printf("<LEFT_ASSIGN>");
				break;
			case RIGHT_ASSIGN:
				printf("<RIGHT_ASSIGN>");
				break;
			case AND_ASSIGN:
				printf("<AND_ASSIGN>");
				break;
			case XOR_ASSIGN:
				printf("<XOR_ASSIGN>");
				break;
			case OR_ASSIGN:
				printf("<OR_ASSIGN>");
				break;
			case TYPE_NAME:
				printf("<TYPE_NAME>");
				break;	
			case TYPEDEF:
				printf("<TYPEDEF>");
				break;
			case EXTERN:
				printf("<EXTERN>");
				break;
			case STATIC:
				printf("<STATIC>");
				break;
			case AUTO:
				printf("<AUTO>");
				break;
			case REGISTER:
				printf("<REGISTER>");
				break;
			case CHAR:
				printf("<CHAR>");
				break;
			case SHORT:
				printf("<SHORT>");
				break;
			case INT:
				printf("<INT>");
				break;
			case LONG:
				printf("<LONG>");
				break;
			case SIGNED:
				printf("<SIGNED>");
				break;
			case UNSIGNED:
				printf("<UNSIGNED>");
				break;
			case FLOAT:
				printf("<FLOAT>");
				break;
			case DOUBLE:
				printf("<DOUBLE>");
				break;
			case CONST:
				printf("<CONST>");
				break;
			case VOLATILE:
				printf("<VOLATILE>");
				break;
			case VOID:
				printf("<VOID>");
				break;
			case STRUCT:
				printf("<STRUCT>");
				break;
			case UNION:
				printf("<UNION>");
				break;
			case ENUM:
				printf("<ENUM>");
				break;
			case ELLIPSIS:
				printf("<ELLIPSIS>");
				break;
			case CASE:
				printf("<CASE>");
				break;
			case DEFAULT:
				printf("<DEFAULT>");
				break;
			case IF:
				printf("<IF>");
				break;
			case ELSE:
				printf("<ELSE>");
				break;
			case SWITCH:
				printf("<SWITCH>");
				break;
			case WHILE:
				printf("<WHILE>");
				break;
			case DO:
				printf("<DO>");
				break;
			case FOR:
				printf("<FOR>");
				break;
			case GOTO:
				printf("<GOTO>");
				break;
			case CONTINUE:
				printf("<CONTINUE>");
				break;
			case BREAK:
				printf("<BREAK>");
				break;
			case RETURN:
				printf("<RETURN>");
				break;
			case ERROR:
				printf("<ERROR>");
				break;
		}
	}
}

int main (int argc, const char * argv[]) {
	yyparse();
	return 0;
}
