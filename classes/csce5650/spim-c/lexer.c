/*
 *  lexer.c
 *
 *  Created by Cameron Palmer on 1/18/08.
 *  Copyright 2007 University of North Texas. All rights reserved.
 * 
 *	This has potential if not real overflow issues. Should be reimplemented.
 * 	ASCII centric, should be more unicode friendly.
 * 
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "lexer.h"

char stringbuf[BSIZE];
long int col;
long int line;
char *yystring;

int isis( char c ) {
	if( c == 'u' || c == 'U' || c == 'l' || c == 'L' )
		return 1; /* true */
	else
		return 0; /* false */
}

int isfs( char c ) {
	if( c == 'f' || c == 'F' || c == 'l' || c == 'L' )
		return 1; /* true */
	else
		return 0; /* false */
}

T yylex(void) { /* lexical analyzer */
	int c;
	int i = 0; /* This variable is for use within loops, make sure you initialize it yourself */
	int escaped = 0; /* This variable is for strings and constants that have escaped characters */
	
	/* chew up spaces until a non-space comes along */
	while( isspace(c = getchar()) ) {
		if (c == '\n')
			line++;
	}
	
	if( c == EOF )
		return EOT;
	
	/* See what sort of character we found */
	switch( c ) {
		case '=': /* = or == */
			c = getchar();
			if ( c == '=' ) {
				return EQ_OP;
			} else {
				ungetc(c, stdin);
				return EQUAL;
			}
			break;
		case ':': /* : or :> */
			c = getchar();
			if ( c == '>' )
				return RBRACKET;
			else {
				ungetc(c, stdin);
				return COLON;
			}
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
		case '[':
			return LBRACKET;
			break;
		case ']':
			return RBRACKET;
			break;
		case '~':
			return TILDE;
			break;
		case '-':
			c = getchar();
			if ( c == '=' ) {
				return SUB_ASSIGN;
			} else if ( c == '-' ) {
				return DEC_OP;
			} else if ( c == '>' ) {
				return PTR_OP;
			} else {
				ungetc(c, stdin);
				return MINUS;
			}
			break;
		case '+': /* +, +=, and ++ */
			c = getchar();
			if ( c == '=' ) {
				return ADD_ASSIGN;
			} else if ( c == '+' ) {
				return INC_OP;
			} else {
				ungetc(c, stdin);
				return PLUS;
			}
			break;
		case '*': /* *, or *= */
			c = getchar();
			if ( c == '=' ) {
				return MUL_ASSIGN;
			} else {
				ungetc(c, stdin);
				return ASTERIX;
			}
			break;
		case '/': /* Capture a C-style comment, /, or /= */
			c = getchar();
			int in_comment = 0;
			i = 0;
			if ( c == '*' ) {
				in_comment = 1;
				while ( in_comment == 1 ) {
					c = getchar();
					if ( c == '*' ) {
						c = getchar();
						if ( c == '/' )
							in_comment = 0;
					}
				}
				return COMMENT;
			} else if ( c == '=' ) {
				return DIV_ASSIGN;
			} else {
				ungetc(c, stdin);
				return FSLASH;
			}
			break;
		case '\'': /* Constant in quotes */
			stringbuf[0] = '\'';
			c = getchar();
			
			int in_constant = 1;
			escaped = 0;
			i = 1;
			while ( in_constant == 1 && i < (BSIZE - 1)) {
				if ( c == '\'' && escaped == 0 ) {
					in_constant = 0;
				} else if ( c == '\\' && escaped == 0 ) {
					escaped = 1;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				} else {
					escaped = 0;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				}
			}
			stringbuf[i] = '\'';
			i++;
			stringbuf[i] = '\0';
			
			free(yystring);
			yystring = strdup(stringbuf);
			
			return CONSTANT;
			break;
			
		case '"': /* String Literal */
			stringbuf[0] = '"';
			
			c = getchar();
			int in_string = 1;
			escaped = 0;
			i = 1;
			while ( in_string == 1 && i < (BSIZE - 1)) {
				if ( c == '"' && escaped == 0 ) {
					in_string = 0;
				} else if ( c == '\\' && escaped == 0 ) {
					escaped = 1;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				} else {
					escaped = 0;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				}
			}
			stringbuf[i] ='"';
			i++;
			stringbuf[i] = '\0';
			
			free(yystring);
			yystring = strdup(stringbuf);
			
			return STRING_LITERAL;
			break;
		case '%': /* %, %=, or %> */
			c = getchar();
			if ( c == '>' ) {
				return RCURLY;
			} else if ( c == '=' ) {
				return MOD_ASSIGN;
			} else {
				ungetc(c, stdin);
				return PERCENT;
			}
			break;
		case '<': /* <, <%, <:, <=, <<, or <<= */
			c = getchar();
			if ( c == '%' ) {
				return LCURLY;
			} else if ( c == ':' ) {
				return LBRACKET;
			} else if ( c == '=' ) {
				return LE_OP;
			} else if ( c == '<' ) {
				c = getchar();
				if ( c == '=' ) {
					return LEFT_ASSIGN;
				} else {
					ungetc(c, stdin);
					return LEFT_OP;
				}
			} else {
				ungetc(c, stdin);
				return LTHAN;
			}
			break;
		case '>': /* >, >>, >>=, or >= */
			c = getchar();
			if ( c == '>' ) {
				c = getchar();
				if ( c == '=' ) {
					return RIGHT_ASSIGN;
				} else {
					ungetc(c, stdin);
					return RIGHT_OP;
				}
			} else if ( c == '=' ) {
				return GE_OP;
			} else {
				ungetc(c, stdin);
				return GTHAN;
			}
			break;
		case '^': /* ^, or ^= */
			c = getchar();
			if ( c == '=' ) {
				return XOR_ASSIGN;
			} else {
				ungetc(c, stdin);
				return CARAT;
			}
			break;
		case '|': /* |, |=, or || */
			c = getchar();
			if ( c == '=' ) {
				return OR_ASSIGN;
			} else if ( c == '|' ) {
				return OR_OP;
			} else {
				ungetc(c, stdin);
				return PIPE;
			}
			break;
		case '?':
			return QUESTION;
			break;
		case '!': /* !, or != */
			c = getchar();
			if ( c == '=' ) {
				return NE_OP;
			} else {
				ungetc(c, stdin);
				return EXCLAMATION;
			}
			break;
		case '&': /* &, &=, or && */
			c = getchar();
			if ( c == '=' ) {
				return AND_ASSIGN;
			} else if ( c == '&' ) {
				return AND_OP;
			} else {
				ungetc(c, stdin);
				return AMP;
			}
			break;			
		case ',':
			return COMMA;
			break;
		case ';':
			return SEMI;
			break;
		case '.': /* floating point number, ., or ... */
			c = getchar();
			if ( c == '.' ) {
				c = getchar();
				if ( c == '.' ) {
					return ELLIPSIS;
				} else {
					ungetc(c, stdin);
					return ERROR;
				}
			} else if ( isdigit(c) ) {
				stringbuf[0] = '.';
				int i = 1;
				
				stringbuf[i] = (char) c;
				i++;
			
				c = getchar();
				while( isdigit(c) && i < (BSIZE - 1) ) {
					stringbuf[i] = (char) c;
					i++;
					c = getchar();
				}
			
				/* Optional exponent section */
				if( (c == 'e' || c == 'E') && i < (BSIZE - 1) ) { /* exponent */
					stringbuf[i] = (char) c;
					i++;
			
					c = getchar();
					if( (c == '+' || c == '-') && i < (BSIZE - 1) ) {
						stringbuf[i] = (char) c;
						i++;
						c = getchar();
					}
					/* Must have at least one digit */
					int test = i;
					while( isdigit(c) && i < (BSIZE - 1) ) {
						stringbuf[i] = (char) c;
						i++;
						c = getchar();
					}
					if( test == i ) /* We didn't pass the test */
						return ERROR;			
				}
			
				if( isfs(c) && i < (BSIZE - 1) ) { /* Optional */
					stringbuf[i] = (char) c;
					i++;
				}
				
				stringbuf[i] = '\0';
		
				free(yystring);
				yystring = strdup(stringbuf);
				return CONSTANT;
			} else {
				ungetc(c, stdin);
				return PERIOD;
			}
			break;
	}
	
	/* CONSTANT token */
	if( isdigit(c) ) {
		/* Test if this is a hex constant */
		if ( c == '0' ) {
			c = getchar();
			if ( c == 'x' || c == 'X' ) {
				stringbuf[0] = '0';
				stringbuf[1] = (char) c;
				
				int i = 2;
				c = getchar();
				while( isxdigit(c) && i < (BSIZE - 2) ) {
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				}
				if( isis(c) ) {
					stringbuf[i] = (char) c;
					i++;
				} else {
					ungetc(c, stdin);
				}
				stringbuf[i] = '\0';
								
				free(yystring);
				yystring = strdup(stringbuf);
				return CONSTANT;
			} else {
				ungetc(c, stdin);
				c = '0';
			}
		}
		
		stringbuf[0] = (char) c;
		c = getchar();
		
		int i = 1;
		/* Suck up as many digits as possible */
		while( isdigit(c) && i < (BSIZE - 1) ) {
			stringbuf[i] = (char) c;
			i++;
			c = getchar();
		}
		
		/* then see if it is floating point, exponent, or has a u or l at the end */
		if( isis(c) ) {
			stringbuf[i] = (char) c;
			i++;
			
		} else if( c == 'e' || c == 'E' ) { /* exponent */
			stringbuf[i] = (char) c;
			i++;
			
			c = getchar();
			if( c == '+' || c == '-' ) {
				stringbuf[i] = (char) c;
				i++;
				c = getchar();
			}
			/* Must have at least one digit */
			int test = i;
			while( isdigit(c) && i < (BSIZE - 2) ) {
				stringbuf[i] = (char) c;
				i++;
				c = getchar();
			}
			if( test == i ) /* We didn't pass the test */
				return ERROR;
				
			/* Optional f or l at end, otherwise return the character to stdin */
			if( isfs(c) ) {
				stringbuf[i] = (char) c;
				i++;
			} else {
				ungetc(c, stdin);
			}
			
		} else if( c == '.' ) { /* floating point preceded by a digit */
			stringbuf[i] = (char) c;
			i++;
			
			c = getchar();
			while( isdigit(c) && i < (BSIZE - 1) ) {
				stringbuf[i] = (char) c;
				i++;
				c = getchar();
			}
			
			/* Optional exponent section */
			if( (c == 'e' || c == 'E') && i < (BSIZE - 1) ) { /* exponent */
				stringbuf[i] = (char) c;
				i++;
			
				c = getchar();
				if( (c == '+' || c == '-') && i < (BSIZE - 1) ) {
					stringbuf[i] = (char) c;
					i++;
					c = getchar();
				}
				/* Must have at least one digit */
				int test = i;
				while( isdigit(c) && i < (BSIZE - 1) ) {
					stringbuf[i] = (char) c;
					i++;
					c = getchar();
				}
				if( test == i ) /* We didn't pass the test */
					return ERROR;			
			}
			
			if( isfs(c) && i < (BSIZE - 1) ) { /* Optional */
				stringbuf[i] = (char) c;
				i++;
			} else {
				ungetc(c, stdin);
			}
		} else {
			ungetc(c, stdin);
		}
		
		stringbuf[i] = '\0';
		
		free(yystring);
		yystring = strdup(stringbuf);
		return CONSTANT;
			
	/* ID token */
	} else if( isalpha(c) || c == '_' ) {
		stringbuf[0] = (char) c;
		int i = 1;
		c = getchar();
		
		/* Check if it is a string literal or constant */
		if ( c == '\'') {
			stringbuf[i] = '\'';
			i++;
			c = getchar();
			
			int in_constant = 1;
			int escaped = 0;
			while ( in_constant == 1 && i < (BSIZE - 1)) {
				if ( c == '\'' && escaped == 0 ) {
					in_constant = 0;
				} else if ( c == '\\' && escaped == 0 ) {
					escaped = 1;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				} else {
					escaped = 0;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				}
			}
			stringbuf[i] = '\'';
			i++;
			stringbuf[i] = '\0';
			
			free(yystring);
			yystring = strdup(stringbuf);
			
			return CONSTANT;
		} else if ( c == '"' ) {
			stringbuf[i] = '"';
			i++;
			
			c = getchar();
			int in_string = 1;
			int escaped = 0;
			while ( in_string == 1 && i < (BSIZE - 1)) {
				if ( c == '"' && escaped == 0 ) {
					in_string = 0;
				} else if ( c == '\\' && escaped == 0 ) {
					escaped = 1;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				} else {
					escaped = 0;
					stringbuf[i] = (char) c;
					i++;
					
					c = getchar();
				}
			}
			stringbuf[i] ='"';
			i++;
			stringbuf[i] = '\0';
			
			free(yystring);
			yystring = strdup(stringbuf);
			
			return STRING_LITERAL;
		}
		
		
		while( (isalnum(c) || c == '_') && (i < (BSIZE - 1) )) {
			stringbuf[i] = (char) c;
			i++;
			c = getchar();
		}
		stringbuf[i] = '\0';
		
		ungetc(c, stdin);
		if ( strcmp(stringbuf, "auto") == 0 )
			return AUTO;
		else if (strcmp(stringbuf, "break") == 0 )
			return BREAK;
		else if (strcmp(stringbuf, "case") == 0 )
			return CASE;
		else if (strcmp(stringbuf, "char") == 0 )
			return CHAR;
		else if (strcmp(stringbuf, "const") == 0 )
			return CONST;
		else if (strcmp(stringbuf, "continue") == 0 )
			return CONTINUE;
		else if (strcmp(stringbuf, "default") == 0 )
			return DEFAULT;
		else if (strcmp(stringbuf, "do") == 0 )
			return DO;
		else if (strcmp(stringbuf, "double") == 0 )
			return DOUBLE;
		else if (strcmp(stringbuf, "else") == 0 )
			return ELSE;
		else if (strcmp(stringbuf, "enum") == 0 )
			return ENUM;
		else if (strcmp(stringbuf, "extern") == 0 )
			return EXTERN;
		else if (strcmp(stringbuf, "float") == 0 )
			return FLOAT;
		else if (strcmp(stringbuf, "for") == 0 )
			return FOR;
		else if (strcmp(stringbuf, "goto") == 0 )
			return GOTO;
		else if (strcmp(stringbuf, "if") == 0 )
			return IF;
		else if (strcmp(stringbuf, "int") == 0 )
			return INT;
		else if (strcmp(stringbuf, "long") == 0 )
			return LONG;
		else if (strcmp(stringbuf, "register") == 0 )
			return REGISTER;
		else if (strcmp(stringbuf, "return") == 0 )
			return RETURN;
		else if (strcmp(stringbuf, "short") == 0 )
			return SHORT;
		else if (strcmp(stringbuf, "signed") == 0 )
			return SIGNED;
		else if (strcmp(stringbuf, "sizeof") == 0 )
			return SIZEOF;
		else if (strcmp(stringbuf, "static") == 0 )
			return STATIC;
		else if (strcmp(stringbuf, "struct") == 0 )
			return STRUCT;
		else if (strcmp(stringbuf, "switch") == 0 )
			return SWITCH;
		else if (strcmp(stringbuf, "typedef") == 0 )
			return TYPEDEF;
		else if (strcmp(stringbuf, "union") == 0 )
			return UNION;
		else if (strcmp(stringbuf, "unsigned") == 0 )
			return UNSIGNED;
		else if (strcmp(stringbuf, "void") == 0 )
			return VOID;
		else if (strcmp(stringbuf, "volatile") == 0 )
			return VOLATILE;
		else if (strcmp(stringbuf, "while") == 0 )
			return WHILE;
		else {
			free(yystring);
			yystring = strdup(stringbuf);
			return ID;
		}
	} else { 
		return ERROR; /* This is what happens when an unrecognized token is encountered */
	}
}
