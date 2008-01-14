/*
 *  lexer.c
 *
 *  Created by Cameron Palmer on 10/16/07.
 *  Copyright 2007 University of North Texas. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "lexer.h"

char stringbuf[BSIZE];
int yyint;
char *yystring;

T yylex(void) { /* lexical analyzer */
	int c;
	
	while( isspace(c = getchar()) );
	
	if( c == EOF )
		return EOT;
		
	switch( c ) {
		case '=':
			return EQUAL;
			break;
		case ':':
			return COLON;
			break;
		case '(':
			return LPAREN;
			break;
		case ')':
			return RPAREN;
			break;
		case '{':
			return LCURLY;
			break;
		case '}':
			return RCURLY;
			break;
		case '~':
			return TILDE;
			break;
		case '-':
			return MINUS;
			break;
		case '+':
			return PLUS;
			break;
		case '*':
			return ASTERIX;
			break;
		case '/':
			return FSLASH;
			break;
		case '%':
			return PERCENT;
			break;
		case '<':
			return LTHAN;
			break;
		case '>':
			return GTHAN;
			break;
		case '^':
			return CARAT;
			break;
		case '|':
			return PIPE;
			break;
			
		case ',':
			return COMMA;
			break;
		case ';':
			return SEMI;
			break;
		case '.':
			return PERIOD;
			break;
	}
	
	/* INT token */
	if( isdigit(c) ) {
		stringbuf[0] = (char) c;
		int i = 1;
		c = getchar();
		while( isdigit(c) && i < (BSIZE - 1) ) {
			stringbuf[i] = (char) c;
			i++;
			c = getchar();
		}
		stringbuf[i] = '\0';
		
		ungetc(c, stdin);
		yyint = atoi( stringbuf );
		return INT;
	}
	
	/* ID token */
	else if( isalpha(c) || c == '_' ) {
		stringbuf[0] = (char) c;
		int i = 1;
		c = getchar();
		while( (isalnum(c) || c == '_') && (i < (BSIZE - 1) )) {
			stringbuf[i] = (char) c;
			i++;
			c = getchar();
		}
		stringbuf[i] = '\0';
		
		ungetc(c, stdin);
		yystring = strdup(stringbuf);
		
		return ID;
	} else { 
		return ERR; /* This is what happens when an unrecognized token is encountered */
	}
}
