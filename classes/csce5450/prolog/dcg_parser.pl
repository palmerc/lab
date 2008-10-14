% ?-list2clause("f(X):-g(X),h(X).",Term).

:-op(300,fx,(`)).
:-op(200,xfx,(:)).


codes2term(Codes,Term):-
  tokenizer(Codes,Tokens),
  det_append(Tokens,[eoc],NewTokens),
  parser(NewTokens,Term).
  
list2clause(Chars,Clause):-
  tokenizer(Chars,Tokens),
  parser(Tokens,Clause).

% Horn clause + disj + if-then-else parser

parser(Tokens,Term):-parser(Tokens,Term,_,_).

parser(Tokens,Term)-->
  xset_dcg_state(Tokens),
    top_term(eoc,Term,_Dict),
  xget_dcg_state([]).

xset_dcg_state(Xs,_,Xs).

xget_dcg_state(Xs,Xs,Xs).

`X-->[X].

top_term(End,HB,D) --> term(H,D), body(End,H,HB,D).

match_end(End)--> `X, {X==End}.

test_end(End)--> xget_dcg_state(Xs),{nonvar(Xs),Xs=[End|_]}.

body(End,H,':-'(H,Bs),D) --> `iff,conj_seq(Bs,D),match_end(End).
body(End,H,H,_) --> match_end(End).

check_var(K,V,tree(K,V,_,_)):-!.
check_var(K,V,tree(LK,_,L,_)):-K@<LK,!,check_var(LK,V,L).
check_var(K,V,tree(_,_,_,R)):-check_var(K,V,R).

ints2float(I,D,N):-number_codes(I,Is),number_codes(D,Ds),[Dot]=".",det_append(Is,[Dot|Ds],Cs),number_codes(N,Cs).

term(V,D) --> `var(T),{check_var(T,V,D)}.
term(N,_) --> `num(I),`eoc,`num(D),!,{ints2float(I,D,N)}.
term(N,_) --> `num(N).
term(T,D) --> `const(F),args(Xs,D),{T=..[F|Xs]}.
term(L,D) --> `lbra,term_seq(L,D).
term(S,D) --> `lpar,spec_term(S,D).

args([T|Ts],D) --> `lpar,term(T,D),arglist(Ts,D).
args([],_)-->[].

arglist([],_) --> `rpar.
arglist([T|Ts],D) --> `comma,term(T,D),arglist(Ts,D).

term_seq([],_)--> `rbra,!.
term_seq([T|Ts],D) --> term(T,D),term_list_cont(Ts,D).

term_list_cont([T|Ts],D)--> `comma, term(T,D),term_list_cont(Ts,D).
term_list_cont(T,D)--> `bar, term(T,D), `rbra.
term_list_cont([],_)--> `rbra.

conj_seq(Xs,D)-->seq(comma,',',term,Xs,_End,D).

seq(InCons,OutCons,Inner,Bs,End,D)--> 
  dcg_call_fun(Inner,B,D),
  cons_args(InCons,OutCons,Inner,B,Bs,End,D).

cons_args(InCons,OutCons,Inner,G,T,End,D) --> `InCons, !, 
  {T=..[OutCons,G,Gs]},
  dcg_call_fun(Inner,NewG,D),
  cons_args(InCons,OutCons,Inner,NewG,Gs,End,D).   
cons_args(_,_,_,G,G,End,_) --> test_end(End). 

spec_term(Xs,D)--> disj_seq(Xs,D),`rpar.
spec_term(Xs,D)--> top_term(rpar,Xs,D).

disj_seq(Xs,D)-->seq(disj,';',disj_term,Xs,_,D).

disj_term(T,D)--> seq(comma,',',term,Xs,End,D),disj_term_cont(End,Xs,T,D).

disj_term_cont(if,Xs,(Xs->Then),D)--> `if, seq(comma,',',term,Then,_,D).
disj_term_cont(disj,Xs,Xs,_)-->[].
disj_term_cont(rpar,Xs,Xs,_)-->[].

% tokenizer

tokenizer(Cs,Ws):-tokenizer(Cs,Ws,_,_).

tokenizer(Cs,Ws)--> xset_dcg_state([32|Cs]),words(Ws),!,xget_dcg_state([]).

words(Ws)-->dcg_star(word,Ws),space.

word(W)-->space,token(W).

token(lpar)-->c("(").
token(rpar)-->c(")").
token(lbra)-->c("[").
token(rbra)-->c("]").
token(bar)-->c("|").
token(comma)-->c(",").
token(disj)-->c(";").
token(if)-->c("->").
token(eoc)-->c(".").
token(iff)-->c(":-").
token(Token)-->token(F,Xs),{name(N,Xs)},{Token=..[F,N]}.

token(num,Xs) --> dcg_plus(is_digit,Xs).
token(const,Xs) --> only_one(is_punct,Xs).
token(F,Xs) --> `X,sym(X,F,Xs).

sym(X,var,[X|Xs])-->{is_maj(X)},!,dcg_star(is_letter,Xs).
sym(X,const,[X|Xs])-->{is_min(X)},dcg_star(is_letter,Xs).

c([])-->[].
c([X|Xs]) --> `X,c(Xs).

space-->dcg_star(is_space,_).

% regexp tools with  AGs + high order

only_one(F,[X])--> dcg_call_fun(F,X).

dcg_star(F,[X|Xs])--> dcg_call_fun(F,X),!,dcg_star(F,Xs).
dcg_star(_,[])-->[].

dcg_plus(F,[X|Xs])--> dcg_call_fun(F,X),dcg_star(F,Xs).

dcg_call_fun(F,X,D,S1,S2):-FX=..[F,X,D,S1,S2],call(FX). %,println(called=FX).

dcg_call_fun(F,X,S1,S2):-FX=..[F,X,S1,S2],call(FX). %,println(called=FX).


is_space(X)--> `X, {member(X,[32,7,9,10,13])}.

is_letter(X)--> `X, {is_an(X)}.

is_punct(X)--> `X, {(is_spec(X);member(X,"!;`""'[]{}*"))}.

is_digit(X)--> `X, {is_num(X)}.

% recognizers - from BinProlog

is_num(X):-member(X,"0123456789").
is_maj(X):-member(X,"_ABCDEFGHIJKLMNOPQRSTUVWXYZ").
is_min(X):-member(X, "abcdefghijklmnopqrstuvwxyz").
is_spec(X):-member(X,"`$&*+-./:<=>?`\^`").

is_an(X):-is_min(X).
is_an(X):-is_maj(X).
is_an(X):-is_num(X).

% tests

data(
"f(X,s(X))."
).
data(
"f:-g,h."
).
data(
"f(X,s(X)):-
   a(Y1,13,2,  Y1 ),!,
   g(X,b).").
data(
"a([X|Xs],Ys,[X|Zs]):-a(Xs,Ys,Zs)."
).
data(
"go(Xs,Ys):-a([1,2,3],[4,5|Xs],Ys)."
).

data(
"a(X):- (a,b(X),c), d(X)."
).

data(
"b(X):- ((a(X);b(X));c(X)), d(X),e(X)."
).

data(
"c(X):- ( a(X) -> b(X) ; c(X) )."
).

data(
"d(X):- 
   ( a(X), b(X) -> c,d,e ; c(X)->d ;  f(X),g,((h)) 
   ).
"
).

data("d((H:-B)):-a(H),d((B->t;f)),show(A,B,(A:-B)).").

toktest:-data(Cs),write_codes(Cs),
  ( tokenizer(Cs,Ws)->write(Ws),nl,write('!!!yes')
  ; write('no!!!')
  ), nl,nl,fail.

write_codes(Cs):-atom_codes(A,Cs),write(A),nl.

parsertest:-
   data(Cs),
   write_codes(Cs),
   (list2clause(Cs,T)->
     write(T),nl,
     M='yes!!!'
     ;
     M='no!!!'
   ),
   write(M),nl,nl,
   fail.


