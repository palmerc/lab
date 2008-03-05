%token A B C D E
%start starter
%%

starter
	: lr second third
	;

lr
	: lr
	| lr second
	| lr second third
	;

second
	: A
	| B
	| C
	| D
	| second
	;

third
	: lr second
	| lr third
	| second lr
	| second third
	;
%%
