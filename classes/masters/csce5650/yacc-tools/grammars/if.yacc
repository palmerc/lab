%token IF THEN ELSE A B
%start s
%%
s
	: if e then s
	| if e then s else s
	| A
	;
e
	: B
	;
if : IF ;
then : THEN ;
else : ELSE ;
%%
