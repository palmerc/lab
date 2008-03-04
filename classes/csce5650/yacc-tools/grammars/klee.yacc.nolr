%token 
%start prog

%%
prog
	: expr
	;
expr
	: term expr_prime
	;
term
	: storable term_prime
	;
storable
	: factor S
	| factor
	;
factor
	: number
	| R
	| '(' expr ')'
	;
expr_prime
	: '+' term expr_prime
	| '-' term expr_prime
	;
term_prime
	: '*' storable term_prime
	| '/' storable term_prime
	;
%%
