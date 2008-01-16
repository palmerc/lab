/*
 *  parser.c
 *
 *  Created by Cameron Palmer on 01/16/08.
 *  Copyright 2008 University of North Texas. All rights reserved.
 *
 */

#include "lexer.h"
#include "parser.h"

typedef struct { 
	long int line_number;
	T type_of_token;
	char* lexeme_string;
} Token;

T type;
Token lookaheadT;

void get_token(void) {
	lookaheadT.type_of_token = yylex();
	lookaheadT.lexeme_string = strdup(yystring);
	lookaheadT.line_number = yyln;
}

void primary_expression(void) {
	match(IDENTIFIER);
	
	match(CONSTANT);
	
	match(STRING_LITERAL);
	
	match(LPAREN);
	expression();
	match(RPAREN);
}

void postfix_expression(void) {
	primary_expression();
	
	postfix_expression();
	match(LBRACKET);
	expression();
	match(RBRACKET);
	
	postfix_expression();
	match(LPAREN);
	match(RPAREN);
	
	postfix_expression();
	match(LPAREN);
	argument_expression_list();
	match(RPAREN);
	
	postfix_expression();
	match(DOT);
	match(IDENTIFIER);
	
	postfix_expression();
	match(PTR_OP);
	match(IDENTIFIER);
	
	postfix_expression();
	match(INC_OP);
	
	postfix_expression();
	match(DEC_OP);
}

void argument_expression_list(void) {
	assignment_expression();
	
	argument_expression_list();
	match(COMMA);
	assignment_expression();
}

void unary_expression(void) {
	postfix_expression();
	
	match(INC_OP);
	unary_expression();
	
	match(DEC_OP);
	unary_expression();
	
	unary_operator();
	cast_expression();
	
	match(SIZEOF);
	unary_expression();
	
	match(SIZEOF);
	match(LPAREN);
	type_name();
	match(RPAREN);
}

void unary_operator(void) {
	match(AMPERSAND);
	
	match(ASTERIX);
	
	match(PLUS);
	
	match(MINUS);
	
	match(TILDE);
	
	match(EXCLAMATION);
}

void cast_expression(void) {
	unary_expression();
	
	match(LPAREN);
	type_name();
	match(RPAREN);
	cast_expression();
}

void multiplicative_expression(void) {
	cast_expression();
	
	multiplicative_expression();
	match(ASTERIX);
	cast_expression();
	
	multiplicative_expression();
	match(FSLASH);
	cast_expression();
	
	multiplicative_expression();
	match(PERCENT);
	cast_expression();
}

void additive_expression(void) {
	multiplicative_expression();
	
	additive_expression();
	match(PLUS);
	multiplicative_expression();
	
	additive_expression();
	match(MINUS);
	multiplicative_expression();
}

void shift_expression(void) {
	additive_expression();
	
	shift_expression();
	match(LEFT_OP);
	additive_expression();
	
	shift_expression();
	match(RIGHT_OP);
	additive_expression();
}

void relational_expression(void) {
	shift_expression();
	
	relational_expression();
	match(LTHAN);
	shift_expression();
	
	relational_expression();
	match(RTHAN);
	shift_expression();
	
	relational_expression();
	match(LE_OP);
	shift_expression();
	
	relational_expression();
	match(GE_OP);
	shift_expression();
}

void equality_expression(void) {
	relational_expression();
	
	equality_expression();
	match(EQ_OP);
	relational_expression();
	
	equality_expression();
	match(NE_OP);
	relational_expression();
}

void and_expression(void) {
	equality_expression();
	
	and_expression();
	match(AMPERSAND);
	equality_expression();
}

void exclusive_or_expression(void) {
	and_expression();
	
	exclusive_or_expression();
	match(CARAT);
	and_expression();
}

void inclusive_or_expression(void) {
	exclusive_or_expression();
	
	inclusive_or_expression();
	match(PIPE);
	exclusive_or_expression();
}

void logical_and_expression(void) {
	inclusive_and_expression();
	
	logical_and_expression();
	match(AND_OP);
	inclusive_or_expression();
}

void logical_or_expression(void) {
	logical_and_expression();
	
	logical_or_expression();
	match(OR_OP);
	logical_and_expression();
}

void conditional_expression(void) {
	logical_or_expression();
	
	logical_or_expression();
	match(QUESTION);
	expression();
	match(COLON);
	conditional_expression();
}

void assignment_expression(void) {
	conditional_expression();
	
	unary_expression();
	assignment_operator();
	assignment_expression();
}

void assignment_operator(void) {
	match(EQUAL);
	
	match(MUL_ASSIGN);
	
	match(DIV_ASSIGN);
	
	match(MOD_ASSIGN);
	
	match(ADD_ASSIGN);
	
	match(SUB_ASSIGN);
	
	match(LEFT_ASSIGN);
	
	match(RIGHT_ASSIGN);
	
	match(AND_ASSIGN);
	
	match(XOR_ASSIGN);
	
	match(OR_ASSIGN);
}

void expression(void) {
	assignment_expression();
	
	expression();
	match(COMMA);
	assignment_expression();
}

void constant_expression(void) {
	conditional_expression();
}

void declaration(void) {
	declaration_specifiers();
	match(SEMI);
	
	declaration_specifiers();
	init_declarator_list();
	match(SEMI);
}

void declaration_specifiers(void) {
	storage_class_specifier();
	
	storage_class_specifier();
	declaration_specifiers();
	
	type_specifier();
	
	type_specifier();
	declaration_specifiers();
	
	type_qualifier();
	
	type_qualifier();
	declaration_specifiers();
}

void init_declarator_list(void) {
	init_declarator();
	
	init_declarator();
	match(COMMA);
	init_declarator();	
}

void init_declarator(void) {
	declarator();
	
	declarator();
	match(EQUAL);
	initializer();
}

void storage_class_specifier(void) {
	match(TYPEDEF);
	
	match(EXTERN);
	
	match(STATIC);
	
	match(AUTO);
	
	match(REGISTER);
}

void type_specifier(void) {
	match(VOID);
	
	match(CHAR);
	
	match(SHORT);
	
	match(INT);
	
	match(LONG);
	
	match(FLOAT);
	
	match(DOUBLE);
	
	match(SIGNED);
	
	match(UNSIGNED);
	
	struct_or_union_specifier();
	
	enum_specifier();
	
	match(TYPE_NAME);
}

void struct_or_union_specifier(void) {
	struct_or_union();
	match(IDENTIFER);
	match(LCURLY);
	struct_declaration_list();
	match(RCURLY);
	
	struct_or_union();
	match(LCURLY);
	struct_declaration_list();
	match(RCURLY);
	
	struct_or_union();
	match(IDENTIFIER);
}

void struct_or_union(void) {
	match(STRUCT);
	
	match(UNION);
}

void struct_declaration_list(void) {
	struct_declaration();
	
	struct_declaration_list();
	struct_declaration();
}

void struct_declaration(void) {
	specifier_qualifier_list();
	struct_declarator_list();
	match(SEMI);
}

void specifier_qualifier_list(void) {
	type_specifier();
	specifier_qualifier_list();
	
	type_specifier();
	
	type_qualifier();
	specifier_qualifier_list();
	
	type_qualifier();
}

void struct_declarator_list(void) {
	struct_declarator();
	
	struct_declarator_list();
	match(COMMA);
	struct_declarator();
}

void struct_declarator(void) {
	declarator();
	
	match(COLON);
	constant_expression();
	
	declarator();
	match(COLON);
	constant_expression();
}

void enum_specifier(void) {
	match(ENUM);
	match(LCURLY);
	enumerator_list();
	match(RCURLY);
	
	match(ENUM);
	match(IDENTIFIER);
	match(LCURLY);
	enumerator_list();
	match(RCURLY);
	
	match(ENUM);
	match(IDENTIFIER);
}

void enumerator_list(void) {
	enumerator();
	
	enumerator_list();
	match(COMMA);
	enumerator();	
}

void enumerator(void) {
	match(IDENTIFIER);
	
	match(IDENTIFIER);
	match(EQUAL);
	constant_expression();
}

void type_qualifier(void) {
	match(CONST);
	
	match(VOLATILE);
}

void declarator(void) {
	pointer();
	direct_declarator();
	
	direct_declarator();
}

void direct_declarator(void) {
	match(IDENTIFIER);
	
	match(LPAREN);
	declarator();
	match(RPAREN);
	
	direct_declarator();
	match(LBRACKET);
	constant_expression();
	match(RBRACKET);
	
	direct_declarator();
	match(LBRACKET);
	match(RBRACKET);
	
	direct_declarator();
	match(LPAREN);
	parameter_type_list();
	match(RPAREN);

	direct_declarator();
	match(LPAREN);
	identifier_list();
	match(RPAREN);

	direct_declarator();
	match(LPAREN);
	match(RPAREN);
}

void pointer(void) {
	match(ASTERIX);
	
	match(ASTERIX);
	type_qualifier_list();
	
	match(ASTERIX);
	pointer();
	
	match(ASTERIX);
	type_qualifier_list();
	pointer();
}

void type_qualifier_list(void) {
	type_qualifier();
	
	type_qualifier_list();
	type_qualifier();
}

void parameter_type_list(void) {
	parameter_list();
	
	parameter_list();
	match(COMMA);
	match(ELLIPSIS);
}

void parameter_list(void) {
	parameter_declaration();
	
	parameter_list();
	match(COMMA);
	parameter_declaration();
}

void parameter_declaration(void) {
	declaration_specifiers();
	declarator();
	
	declaration_specifiers();
	abstract_declarator();
	
	declaration_specifiers();
}

void identifier_list(void) {
	match(IDENTIFIER);
	
	identifier_list();
	match(COMMA);
	match(IDENTIFIER);
}

void type_name(void) {
	specifier_qualifier_list();
	
	specifier_qualifier_list();
	abstract_declarator();
}

void abstract_declarator(void) {
	pointer();
	
	direct_abstract_declarator();
	
	pointer();
	direct_abstract_declarator();
}

void direct_abstract_declarator(void) {
	match(LPAREN);
	abstract_declarator();
	match(RPAREN);
	
	match(LBRACKET);
	match(RBRACKET);
	
	match(LBRACKET);
	constant_expression();
	match(RBRACKET);
	
	direct_abstract_declarator();
	match(LBRACKET);
	match(RBRACKET);
	
	direct_abstract_declarator();
	match(LBRACKET);
	constant_expression();
	match(RBRACKET);
	
	match(LPAREN);
	match(RPAREN);
	
	match(LPAREN);
	parameter_type_list();
	match(RPAREN);
	
	direct_abstract_declarator();
	match(LPAREN);
	match(RPAREN);
	
	direct_abstract_declarator();
	match(LPAREN);
	parameter_type_list();
	match(RPAREN);
}

void initializer(void) {
	assignment_expression();
	
	match(LCURLY);
	initializer_list();
	match(RCURLY);
	
	match(LCURLY);
	initializer_list();
	match(COMMA);
	match(RCURLY);
}

void initializer_list(void) {
	initializer();
	
	initializer_list();
	match(COMMA);
	initializer();
}

void statement(void) {
	labeled_statement();
	
	compound_statement();
	
	expression_statement();
	
	selection_statement();
	
	iteration_statement();
	
	jump_statement();
}

void labeled_statement(void) {
	match(IDENTIFIER);
	match(COLON);
	statement();
	
	match(CASE);
	constant_expression();
	match(COLON);
	statement();
	
	match(DEFAULT);
	match(COLON);
	statement();
}

void compound_statement(void) {
	match(LCURLY);
	match(RCURLY);
	
	match(LCURLY);
	statement_list();
	match(RCURLY);
	
	match(LCURLY);
	declaration_list();
	match(RCURLY);
	
	match(LCURLY);
	declaration_list();
	statement_list();
	match(RCURLY);
}

void declaration_list(void) {
	declaration();
	
	declaration_list();
	declaration();
}

void statement_list(void) {
	statement();
	
	statement_list();
	statement();
}

void expression_statement(void) {
	match(SEMI);
	
	expression();
	match(SEMI);
}

void selection_statement(void) {
}

void iteration_statement(void) {
	match(WHILE);
	match(LPAREN);
	expression();
	match(RPAREN);
	statement();
	
	match(DO);
	statement();
	match(WHILE);
	match(LPAREN);
	expression();
	match(RPAREN);
	match(SEMI);
	
	match(FOR);
	match(LPAREN);
	expression_statement();
	expression_statement();
	match(RPAREN);
	statement();
	
	match(FOR);
	match(LPAREN);
	expression_statement();
	expression_statement();
	expression();
	match(RPAREN);
	statement();
}

void jump_statement(void) {
	match(GOTO);
	match(IDENTIFIER);
	match(SEMI);
	
	match(CONTINUE);
	match(SEMI);
	
	match(BREAK);
	match(SEMI);
	
	match(RETURN);
	match(SEMI);
	
	match(RETURN);
	expression();
	match(SEMI);
}

void translation_unit(void) {
	get_token();
	
	external_declaration();
	translation_unit();
}

void external_declaration(void) {
	function_definition();
	declaration();
}

void function_definition(void) {
	declaration_specifiers();
	declarator();
	declaration_list();
	compound_statement();

	declaration_specifiers();
	declarator();
	compound_statement();
	
	declarator();
	declaration_list();
	compound_statement();
	
	declarator();
	compound_statement();
}
