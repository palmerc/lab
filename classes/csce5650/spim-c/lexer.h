/*
 *  lexer.h
 *
 *  Created by Cameron Palmer on 10/16/07.
 *  Copyright 2007 University of North Texas. All rights reserved.
 *
 */
 
#ifndef LEXER_H_
#define LEXER_H_

#define BSIZE 256

typedef enum { 	EOT, INT, ID, COMMA, SEMI, PERIOD,
				ERR, EQUAL, COLON, LPAREN, RPAREN, LCURLY,
				RCURLY, TILDE, MINUS, PLUS, ASTERIX, FSLASH,
				PERCENT, LTHAN, GTHAN, CARAT, PIPE } T;
extern long int yyln;
extern int yyint;
extern char *yystring;

T yylex(void);

#endif /*LEXER_H_*/
