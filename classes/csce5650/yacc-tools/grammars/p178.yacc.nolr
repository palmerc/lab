%token A B C D
%start s

%%
s
	: a A
	| B
	;
a
	: B D a_prime
	| E a_prime
	;
a_prime
	: C a_prime
	| A D a_prime
	;
%%
