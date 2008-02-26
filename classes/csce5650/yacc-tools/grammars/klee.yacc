%token
%start prog

%%
prog : expr ;
expr : expr '+' term | expr '-' term | term ;
term : term '*' storable | term '/' storable | storable ;
storable : factor S | factor ;
factor : number | R | '(' expr ')' ;
%%
