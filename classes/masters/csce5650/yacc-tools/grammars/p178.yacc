%token A B C D
%start s

%%
s
	: a A
	| B
	;

a
	: a C
	| s D
	| E
	; 
%%
