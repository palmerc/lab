/*
 *  lexer.h
 *
 *  Created by Cameron Palmer on 10/16/07.
 *  Copyright 2007 University of North Texas. All rights reserved.
 *
 */

#define BSIZE 256

typedef enum { 	EOT, INT, ID, COMMA, SEMI, PERIOD,
				ERR, EQUAL, COLON, LPAREN, RPAREN, LCURLY,
				RCURLY, TILDE, MINUS, PLUS, ASTERIX, FSLASH,
				PERCENT, LTHAN, GTHAN, CARAT, PIPE } T;
extern int yyint;
extern char *yystring;
