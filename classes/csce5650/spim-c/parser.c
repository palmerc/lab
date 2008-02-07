/*
 *  parser.c
 *
 *  Created by Cameron Palmer on 01/16/08.
 *  Copyright 2008 University of North Texas. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

typedef struct { 
	long int line_number;
	T type;
	char* string;
} Token;

Token lookaheadT;

void get_token(void) {
	printf("get_token: entering\n"); 
	lookaheadT.type = yylex();
	free(lookaheadT.string);
	lookaheadT.string = strdup(yystring);
	lookaheadT.line_number = line;
	printf("get_token <%s, %s>\n", lookaheadT.type, lookaheadT.string);
	printf("get_token: exiting\n");
}

int match( T type ) {
	if( lookaheadT.type == type ) {
		return 1; /* true */
	} else {
		abort();
	} 
}

int yyparse(void) {
	printf("yyparse: entering\n");
	get_token();
	translation_unit();
	printf("yyparse: exiting\n");
	return 0;
}

int primary_expression(void) {
	if( lookaheadT.type == ID || 
		lookaheadT.type == CONSTANT || 
		lookaheadT.type == STRING_LITERAL ) {
		match(lookaheadT.type);
	} else if ( lookaheadT.type == LPAREN ) {
		match(LPAREN);
		expression();
		get_token();
		match(RPAREN);
	} else {
		abort();
	}
}

int postfix_expression(void) {
	if ( primary_expression() ) {
	} else if( postfix_expression() ) {
		if( lookaheadT.type == LBRACKET ) {
			match(LBRACKET);
			expression();
			match(RBRACKET);
		} else if( lookaheadT.type == LPAREN ) {
			match(LPAREN);
			if( argument_expression_list() ) {
			}
			match(RPAREN);
		} else if( lookaheadT.type == PERIOD ) {
			match(PERIOD);
			match(ID);
		} else if( lookaheadT.type == PTR_OP ) {	
			match(PTR_OP);
			match(ID);
		} else if( lookaheadT.type == INC_OP ) {
			match(INC_OP);
		} else if( lookaheadT.type == DEC_OP ) {	
			match(DEC_OP);
		} else {
			abort();
		}
	} else {
		abort();
	}
}

int argument_expression_list(void) {
	if( assignment_expression() ) {
	} else if( argument_expression_list() ) {
		match(COMMA);
		assignment_expression();
	} else {
		abort();
	}
}

int unary_expression(void) {
	if( postfix_expression() ) {
	} else if( lookaheadT.type == INC_OP ) {	
		match(INC_OP);
		unary_expression();
	} else if( lookaheadT.type == DEC_OP ) {
		match(DEC_OP);
		unary_expression();
	} else if( unary_operator() ) {
		cast_expression();
	} else if( lookaheadT.type == SIZEOF ) {
		match(SIZEOF);
		if( unary_expression() ) {
		} else if( lookaheadT.type == LPAREN ) {
			match(LPAREN);
			type_name();
			match(RPAREN);
		} else {
			abort();
		}
	} else {
		abort();
	}
}

int unary_operator(void) {
	switch( lookaheadT.type ) {
		case AMP: 
			match(AMP);
			break;
		case ASTERIX:
			match(ASTERIX);
			break;
		case PLUS:
			match(PLUS);
			break;
		case MINUS:
			match(MINUS);
			break;
		case TILDE:
			match(TILDE);
			break;
		case EXCLAMATION:
			match(EXCLAMATION);
			break;
		default:
			abort();
	}
}

int cast_expression(void) {
	if ( unary_expression() ) {
	} else if ( lookaheadT.type == LPAREN ) {
		match(LPAREN);
		type_name();
		match(RPAREN);
		cast_expression();
	} else {
		abort();
	}
}

int multiplicative_expression(void) {
	if(	cast_expression() ) {
	} else if (	multiplicative_expression() ) {
		switch( lookaheadT.type ) {
			case ASTERIX:
				match(ASTERIX);
				break;
			case FSLASH:	
				match(FSLASH);
				break;
			case PERCENT:
				match(PERCENT);
				break;
		}
		cast_expression();
	} else {
		abort();
	}
}

int additive_expression(void) {
	if( multiplicative_expression() ) {
	} else if( additive_expression() ) {
		switch( lookaheadT.type ) {
			case PLUS:
				match(PLUS);
				break;
			case MINUS:
				match(MINUS);
				break;
		}
		multiplicative_expression();
	} else {
		abort();
	}
}

int shift_expression(void) {
	if( additive_expression() ) {
	} else if( shift_expression() ) {
		switch( lookaheadT.type ) {
			case LEFT_OP:
				match(LEFT_OP);
				break;
			case RIGHT_OP:
				match(RIGHT_OP);
				break;
		}
		additive_expression();
	} else {
		abort();
	}
}

int relational_expression(void) {
	if( shift_expression() ) {
	} else if( relational_expression() ) {
		switch( lookaheadT.type ) {
			case LTHAN:
				match(LTHAN);
				break;
			case GTHAN:
				match(GTHAN);
				break;
			case LE_OP:
				match(LE_OP);
				break;
			case GE_OP:
				match(GE_OP);
				break;	
		}
		shift_expression();
	} else {
		abort();
	}
}

int equality_expression(void) {
	if( relational_expression() ) {
	} else if( equality_expression() ) {
		switch( lookaheadT.type ) {
			case EQ_OP:
				match(EQ_OP);
				break;
			case NE_OP:	
				match(NE_OP);
				break;
		}
		relational_expression();
	} else {
		abort();
	}
}

int and_expression(void) {
	if( equality_expression() ) {
	} else if( and_expression() ) {
		match(AMP);
		equality_expression();
	} else {
		abort();
	}
}

int exclusive_or_expression(void) {
	if( and_expression() ) {
	} else if( exclusive_or_expression() ) {
		match(CARAT);
		and_expression();
	} else {
		abort();
	}
}

int inclusive_or_expression(void) {
	if( exclusive_or_expression() ) {
	} else if( inclusive_or_expression() ) {
		match(PIPE);
		exclusive_or_expression();
	}
}

int logical_and_expression(void) {
	if( inclusive_or_expression() ) {
	} else if( logical_and_expression() ) {
		match(AND_OP);
		inclusive_or_expression();
	}
}

int logical_or_expression(void) {
	if( logical_and_expression() ) {
	} else if( logical_or_expression() ) {
		match(OR_OP);
		logical_and_expression();
	}
}

int conditional_expression(void) {
	if( logical_or_expression() ) {
	} else if( logical_or_expression() ) {
		match(QUESTION);
		expression();
		match(COLON);
		conditional_expression();
	}
}

int assignment_expression(void) {
	if( conditional_expression() ) {
	} else if( unary_expression() ) {
		assignment_operator();
		assignment_expression();
	}
}

int assignment_operator(void) {
	switch( lookaheadT.type ) {
		case EQUAL:
			match(EQUAL);
			break;
		case MUL_ASSIGN:
			match(MUL_ASSIGN);
			break;	
		case DIV_ASSIGN:
			match(DIV_ASSIGN);
			break;
		case MOD_ASSIGN:
			match(MOD_ASSIGN);
			break;
		case ADD_ASSIGN:
			match(ADD_ASSIGN);
			break;
		case SUB_ASSIGN:
			match(SUB_ASSIGN);
			break;
		case LEFT_ASSIGN:
			match(LEFT_ASSIGN);
			break;
		case RIGHT_ASSIGN:
			match(RIGHT_ASSIGN);
			break;
		case AND_ASSIGN:
			match(AND_ASSIGN);
			break;
		case XOR_ASSIGN:
			match(XOR_ASSIGN);
			break;
		case OR_ASSIGN:
			match(OR_ASSIGN);
			break;
		default:
			abort();
	}
}

int expression(void) {
	if(	assignment_expression() ) {
	} else if ( expression() ) {
		match(COMMA);
		assignment_expression();
	} else {
		abort();
	}
}

int constant_expression(void) {
	conditional_expression();
}

int declaration(void) {
	declaration_specifiers();
	if( init_declarator_list() ) {
	}
	match(SEMI);
}

int declaration_specifiers(void) {
	if( storage_class_specifier() ) {
		if( declaration_specifiers() ) {
		}
	} else if( type_specifier() ) {
		if( declaration_specifiers() ) {
		}
	} else if( type_qualifier() ) {	
		if( declaration_specifiers() ) {
		}
	}
}

int init_declarator(void) {
	declarator();
	if( lookaheadT.type == EQUAL ) {
		match(EQUAL);
		initializer();
	}
}

int init_declarator_list(void) {
	if( init_declarator() ) {
	} else if( init_declarator_list() ) {
		match(COMMA);
		init_declarator();
	} else {
		abort();
	}	
}

int storage_class_specifier(void) {
	switch( lookaheadT.type ) {
		case TYPEDEF:
			match(TYPEDEF);
			break;
		case EXTERN:
			match(EXTERN);
			break;
		case STATIC:
			match(STATIC);
			break;
		case AUTO:
			match(AUTO);
			break;
		case REGISTER:
			match(REGISTER);
			break;
		default:
			abort();
	}
}

int type_specifier(void) {
	if( struct_or_union_specifier() ) {
	} else if( enum_specifier() ) {
	} else {
		switch( lookaheadT.type ) {
			case VOID:
				match(VOID);
				break;
			case CHAR:
				match(CHAR);
				break;
			case SHORT:
				match(SHORT);
				break;
			case INT:
				match(INT);
				break;
			case LONG:
				match(LONG);
				break;
			case FLOAT:
				match(FLOAT);
				break;
			case DOUBLE:
				match(DOUBLE);
				break;
			case SIGNED:
				match(SIGNED);
				break;
			case UNSIGNED:
				match(UNSIGNED);
				break;
			case TYPE_NAME: 
				match(TYPE_NAME);
				break;
		}
	}

}

int struct_or_union_specifier(void) {
	struct_or_union();
	
	if( lookaheadT.type == ID ) {
		match(ID);
	}
	if( lookaheadT.type == LCURLY ) {
		match(LCURLY);
		struct_declaration_list();
		match(RCURLY);
	}
}

int struct_or_union(void) {
	switch( lookaheadT.type ) {
		case STRUCT:
			match(STRUCT);
			break;
		case UNION:
			match(UNION);
			break;
		default:
			abort();
	}
}

int struct_declaration_list(void) {
	if( struct_declaration_list() ) {
	}
	struct_declaration();
}

int struct_declaration(void) {
	specifier_qualifier_list();
	struct_declarator_list();
	match(SEMI);
}

int specifier_qualifier_list(void) {
	if( type_specifier() ) {
		if( specifier_qualifier_list() ) {
		}
	} else if( type_qualifier() ) {
		if( specifier_qualifier_list() ) {
		}
	} else {
		abort();
	}
}

int struct_declarator_list(void) {
	if( struct_declarator_list() ) {
		match(COMMA);
	}
	struct_declarator();
}

int struct_declarator(void) {
	if( declarator() ) {
	}
	if( lookaheadT.type == COLON ) {
		match(COLON);
		constant_expression();
	}
}

int enum_specifier(void) {
	match(ENUM);
	
	if( lookaheadT.type == ID ) {
		match(ID);
	}
	if( lookaheadT.type == LCURLY ) {	
		match(LCURLY);
		enumerator_list();
		match(RCURLY);
	}
}

int enumerator_list(void) {
	if( enumerator_list() ) {
		match(COMMA);
	}
	enumerator();	
}

int enumerator(void) {
	match(ID);
	if( lookaheadT.type == EQUAL ) { 
		match(EQUAL);
		constant_expression();
	}
}

int type_qualifier(void) {
	switch( lookaheadT.type ) {
		case CONST:
			match(CONST);
			break;
		case VOLATILE:
			match(VOLATILE);
			break;
	}
}

int declarator(void) {
	if( pointer() ) {
	}
	direct_declarator();
}

int direct_declarator(void) {
	if( lookaheadT.type == ID ) {
		match(ID);
	} else if( lookaheadT.type == LPAREN ) {
		match(LPAREN);
		declarator();
		match(RPAREN);
	} else if( direct_declarator() ) {
		if( lookaheadT.type == LBRACKET ) {
			match(LBRACKET);
			if( constant_expression() ) {
			}
			match(RBRACKET);
		} else if( lookaheadT.type == LPAREN ) {
			match(LPAREN);
			if( parameter_type_list() ) {
			} else if ( identifier_list() ) {
			}
			match(RPAREN);
		}
	}
}

int pointer(void) {
	match(ASTERIX);
	if( type_qualifier_list() ) {
	}
	if( pointer() ) {
	}
}

int type_qualifier_list(void) {
	if( type_qualifier () ) {
	} else if ( type_qualifier_list() ) {
		type_qualifier();
	}
}

int parameter_type_list(void) {
	parameter_list();
	if( lookaheadT.type == COMMA ) {	
		match(COMMA);
		match(ELLIPSIS);
	}
}

int parameter_list(void) {
	if( parameter_declaration() ) {
	} else if( parameter_list() ) {
		match(COMMA);
		parameter_declaration();
	}
}

int parameter_declaration(void) {
	declaration_specifiers();
	if( declarator() ) {
	} else if( abstract_declarator() ) {
	}
}

int identifier_list(void) {
	if( lookaheadT.type == ID ) {
		match(ID);
	} else if( identifier_list() ) {
		match(COMMA);
		match(ID);
	}
}

int type_name(void) {
	specifier_qualifier_list();
	if( abstract_declarator() ) {
	}
}

int abstract_declarator(void) {
	if( pointer() ) {
	}
	if( direct_abstract_declarator() ) {
	}
}

int direct_abstract_declarator(void) { 
	if( lookaheadT.type == LPAREN ) {
		match(LPAREN);
		if( abstract_declarator() ) {
		} else if( parameter_type_list() ) {
		}
		match(RPAREN);
	} else if ( lookaheadT.type == LBRACKET ) {
		match(LBRACKET);
		if( constant_expression() ) {}
		match(RBRACKET);
	} else if( direct_abstract_declarator() ) {
		match(LBRACKET);
		if( constant_expression() ) {}
		match(RBRACKET);
	} else if( direct_abstract_declarator() ) {
		match(LPAREN);
		if( parameter_type_list() ) {}
		match(RPAREN);
	} else {
		abort();
	}
}

int initializer(void) {
	if( assignment_expression() ) {
	} else if( lookaheadT.type == LCURLY ) {
		match(LCURLY);
		if( initializer_list() ) {}
		if( lookaheadT.type == COMMA ) {
			match(COMMA);
		}
		match(RCURLY);
	} else {
		abort();
	}
}

int initializer_list(void) {
	if( initializer() ) {
	} else if( initializer_list() ) {
		match(COMMA);
		initializer();
	}
}

int statement(void) {
	if( labeled_statement() ) {
	} else if( compound_statement() ) {
	} else if( expression_statement() ) {
	} else if( selection_statement() ) {
	} else if( iteration_statement() ) {
	} else if( jump_statement() ) {
	} else {
		abort();
	}
}

int labeled_statement(void) {
	if( lookaheadT.type == ID ) {
		match(ID);
		match(COLON);
		statement();
	} else if( lookaheadT.type == CASE ) {	
		match(CASE);
		constant_expression();
		match(COLON);
		statement();
	} else if( lookaheadT.type == DEFAULT ) {	
		match(DEFAULT);
		match(COLON);
		statement();
	} else {
		abort();
	}
}

int compound_statement(void) {
	match(LCURLY);
	if( statement_list() ) {
	} else if( declaration_list() ) {
		if( statement_list() ) {
		}
	} else {
		abort();
	}
	match(RCURLY);
}

int declaration_list(void) {
	if( declaration() ) {
	} else if( declaration_list() ) {
		declaration();
	} else {
		abort();
	}
}

int statement_list(void) {
	if( statement() ) {
	} else if( statement_list() ) {
		statement();
	} else {
		abort();
	}
}

int expression_statement(void) {
	if( lookaheadT.type == SEMI ) {
		match(SEMI);
	} else if( expression() ) {	
		match(SEMI);
	} else {
		abort();
	}
}

int selection_statement(void) {
	if( lookaheadT.type == IF ) {
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
		if( lookaheadT.type == ELSE ) {
			match(ELSE);
			statement();
		}
	} else if( lookaheadT.type == SWITCH ) {
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
	} else {
		abort();
	}
}

int iteration_statement(void) {
	if( lookaheadT.type == WHILE ) {
		match(WHILE);
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
	} else if( lookaheadT.type == DO ) {	
		match(DO);
		statement();
		match(WHILE);
		match(LPAREN);
		expression();
		match(RPAREN);
		match(SEMI);
	} else if( lookaheadT.type == FOR ) {
		match(FOR);
		match(LPAREN);
		expression_statement();
		expression_statement();
		if( expression() ) {}
		match(RPAREN);
		statement();
	} else {
		abort();
	}
}

int jump_statement(void) {
	switch( lookaheadT.type ) {
		case GOTO:
			match(GOTO);
			match(ID);
			break;
		case CONTINUE:
			match(CONTINUE);
			break;
	 	case BREAK:
			match(BREAK);
			break;
		case RETURN:
			match(RETURN);
			if( expression() ) {}
			break;
		default:
			abort();
	}
	match(SEMI);
}

int translation_unit(void) {
	if( external_declaration() ) {
	} else if( translation_unit() ) {
		external_declaration();
	} else {
		abort();
	}
}

int external_declaration(void) {
	if( function_definition() ) {
	} else if( declaration() ) {
	} else {
		abort();
	}
}

int function_definition(void) {
	if( declaration_specifiers() ) {
		declarator();
		
		if( declaration_list() ) {
		}
	} else if ( declarator() ) {
		if(	declaration_list() ) {
		}
	} else {
		abort();
	}
	compound_statement();
}
