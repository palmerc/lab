% sets/lists

nat_(0).
nat_(s(N)):-nat_(N).

append_([],Ys, Ys).
append_([X|Xs],Ys, [X|Zs]):-append_(Xs,Ys,Zs).

sel_(X,Xs,Ys):-append_(_,[X|Ys],Xs).

member_(X,Xs):-sel_(X,Xs,_).

member__(X,Xs):-append_(_,[X|_],Xs).

% selects/inserts a value from/to a list
sel(X,[X|Xs],Xs).
sel(X,[Y|Xs],[Y|Ys]):-sel(X,Xs,Ys).

ins(X,Xs,Ys):-sel(X,Ys,Xs).
	
% permutations

perm([],[]).
perm([X|Xs],Zs):-
  perm(Xs,Ys),
  ins(X,Ys,Zs).

% subsets of K elements, one at a time
	
ksubset(0,_,[]).
ksubset(K,[X|Xs],[X|Rs]):-K>0,K1 is K-1,ksubset(K1,Xs,Rs).
ksubset(K,[_|Xs],Rs):-K>0,ksubset(K,Xs,Rs).

% all subsets of K elements
all_k_subsets(0,_,[[]]).
all_k_subsets(K,[],[]):-K>0.
all_k_subsets(K,[X|Xs],Yss):-
  K>0,K1 is K-1,
  all_k_subsets(K1,Xs,Lss),
  all_k_subsets(K,Xs,Rss),
  mapcons(X,Lss,Rss,Yss).
	
% subset generator	
subset_of([],[]).
subset_of([X|Xs],Zs):-
  subset_of(Xs,Ys),
  maybe_add_element(X,Ys,Zs).

maybe_add_element(_,Ys,Ys).
maybe_add_element(X,Ys,[X|Ys]).

% subset + complement generator
subset_and_complement_of([], [],[]).
subset_and_complement_of([X|Xs], NewYs,NewZs):-
  subset_and_complement_of(Xs,Ys,Zs),
  place_element(X,Ys,Zs,NewYs,NewZs).

place_element(X,Ys,Zs, [X|Ys],Zs).
place_element(X,Ys,Zs, Ys,[X|Zs]).

% powerset generator
all_subsets([],[[]]).
all_subsets([X|Xs],Zss):-
  all_subsets(Xs,Yss),
  extend_subsets(Yss,X,Zss).

extend_subsets([],_,[]).
extend_subsets([Ys|Yss],X,[Ys,[X|Ys]|Zss]):-extend_subsets(Yss,X,Zss).

% cartesian product of n sets

ncproduct(Xss,Rss):-
  append(Yss,[Xs],Xss),
  !,
  foldr(cproduct,Xs,Yss,Ps),
  map(arrow2list,Ps,Rss).

arrow2list(ABs,[A|Cs]):-compound(ABs),ABs=(A-Bs),!,arrow2list(Bs,Cs).
arrow2list(A,[A]).

% cartesian square
csqr(Xs,Ps):-cproduct(Xs,Xs,Ps).

pair_from(Xs,Ys,X-Y):-member(X,Xs),member(Y,Ys).

% cartesian product
cproduct([],_,[]).
cproduct([X|Xs],Ys,NewPs):-cproduct1(Ys,X,NewPs,Ps),cproduct(Xs,Ys,Ps).

cproduct1([],_,Ps,Ps).
cproduct1([Y|Ys],X,[X-Y|NewPs],Ps):-cproduct1(Ys,X,NewPs,Ps).
 
% f arrangements of a set with n elements
% int seq A000522: 1, 2, 5, 16, 65, 326,...  

arrangements_of(Xs, As):-subset_of(Xs,Ps),perm(Ps,As).

% arrangements of K elements of a set of n elements Xs
karrangements(K,Xs, As):-ksubset(K,Xs,Ps),perm(Ps,As).

% set partition generator  
set_partition_of(Xs,[Xs]).
set_partition_of(Xs,[Ls|Lss]):-
  ordered_set_split(Xs,Ls,Rs),
  set_partition_of(Rs,Lss).

ordered_set_split(Xs,[L|Ls],[R|Rs]):-
  subset_and_complement_of(Xs,[L|Ls],[R|Rs]),
  L@<R.
  
% generates pairs of partitions seen as members of the poset of partitions
% number of answers=>sloane A000258: 1,3,12,60,358,2471,19302
partition_poset(Xs,Pss,Rss):-
  set_partition_of(Xs,Pss),
  refinement_of(Pss,Rss).

refinement_of(Pss,Rss):-
  map(set_partition_of,Pss,Psss),
  appendN(Psss,Rss).

% partition in sets of length =<K  
k_set_partition(K,Xs,[Xs]):-length(Xs,L),L=<K.
k_set_partition(K,Xs,[Ls|Lss]):-
  ordered_set_split(Xs,Ls,Rs),
  length(Ls,L),L=<K,
  k_set_partition(K,Rs,Lss).
  
% partition into N subsets  
n_set_partition(N,Vs,Pss):-length(Pss,N),set_partition_of(Vs,Pss).

% partition into N subsets of length =<K  
nk_set_partition(N,K,Vs,Pss):-length(Pss,N),k_set_partition(K,Vs,Pss).
 
% set partitions, differently
spart_of(Xs,Ps):-
  length(Xs,L),
  length(Vs,L),
  mpart_of(Vs,Us),
  once(map(member,Xs,Vs)),
  once(map(close_list,Us,Ps)).

close_list(Xs,Ys):-append(Xs,[],Ys).
  
% partition-as-equality generator
% assumes list of vars as input 
% A000110: 1, 1, 2, 5, 15, 52, 203,...

mpart_of([],[]).
mpart_of([U|Xs],[U|Us]):-
  mcomplement_of(U,Xs,Rs),
  mpart_of(Rs,Us).

% subset + complement generator
mcomplement_of(_,[],[]).
mcomplement_of(U,[X|Xs],NewZs):-
  mcomplement_of(U,Xs,Zs),
  mplace_element(U,X,Zs,NewZs).

mplace_element(U,U,Zs,Zs).
mplace_element(_,X,Zs,[X|Zs]).

% backtracking integer partition iterator
 
integer_partition_of(N,Ps):-
  revnats(N,Is),
  split_to_sum(N,Is,Ps).

split_to_sum(0,_,[]).
split_to_sum(N,[K|Ks],R):-
  N>0,
  sum_choice(N,K,Ks,R).

sum_choice(N,K,Ks,[K|R]):-
  NK is N-K,
  split_to_sum(NK,[K|Ks],R).
sum_choice(N,_,Ks,R):-
  split_to_sum(N,Ks,R).

% all integer partitions
all_ipartitions(N,Pss):-
  revnats(N,Rs),
  sums(N,Rs,Pss).

sums(0,[],[[]]):-!.
sums(_,[],[]):- !.
sums(N,[K|Ks],R):-K > N,!,sums(N,Ks,R).
sums(N,[K|Ks],R):-N_K is N-K,
  sums(N_K,[K|Ks],RK),
  sums(N,Ks,End),
  mapcons(K,RK,End,R).

mapcons(_,[],End,End):-!.
mapcons(A,[Xs|Xss],End,[[A|Xs]|Yss]):-mapcons(A,Xss,End,Yss).


% reversed naturals to 1

revnats(1,[1]).
revnats(N,[N|Ns]):-
  N>1,
  N1 is N-1,
  revnats(N1,Ns).

% simple list processing

pref_split_of(Is,Ps,P):-append(Ps,[P|_],Is).

suf_split_of(Is,S,Ss):-append(_,[S|Ss],Is).

% sets operations - assumes standard ordering of elements

set_union([],Xs,Xs).
set_union(Xs,[],Xs):-Xs=[_|_].
set_union([X|Xs],[Y|Ys],[X|Zs]):-X==Y,
   set_union(Xs,Ys,Zs).
set_union([X|Xs],[Y|Ys],[X|Zs]):-
   before_in_set(X,Y),
   set_union(Xs,[Y|Ys],Zs).
set_union([X|Xs],[Y|Ys],[Y|Zs]):-
   before_in_set(Y,X),
   set_union([X|Xs],Ys,Zs).
   
set_intersection([],_,[]).
set_intersection(Xs,[],[]):-Xs=[_|_].
set_intersection([X|Xs],[Y|Ys],[X|Zs]):-X==Y,
   set_intersection(Xs,Ys,Zs).
set_intersection([X|Xs],[Y|Ys],Zs):-
   before_in_set(X,Y),
   set_intersection(Xs,[Y|Ys],Zs).
set_intersection([X|Xs],[Y|Ys],Zs):-
   before_in_set(Y,X),
   set_intersection([X|Xs],Ys,Zs).
   
set_difference([],_,[]).
set_difference(Xs,[],Xs):-Xs=[_|_].
set_difference([X|Xs],[Y|Ys],Zs):-X==Y,
   set_difference(Xs,Ys,Zs).
set_difference([X|Xs],[Y|Ys],[X|Zs]):-
   before_in_set(X,Y),
   set_difference(Xs,[Y|Ys],Zs).
set_difference([X|Xs],[Y|Ys],Zs):-
   before_in_set(Y,X),
   set_difference([X|Xs],Ys,Zs).
   
before_in_set(X,Y):-compare('<',X,Y).  

zip([],[],[]).
zip([X|Xs],[Y|Ys],[X-Y|Zs]):-zip(Xs,Ys,Zs).

zipWith(F,Xs,Ys,Zs):-zipWith2(Xs,Ys,F, Zs).

zipWith2([],[],_,[]).
zipWith2([X|Xs],[Y|Ys],F,[Z|Zs]):-call(F,X,Y,Z),zipWith2(Xs,Ys,F,Zs).

% intger sets

ints(From,To,Is):-findall(I,between(From,To,I),Is).

% transposes list of lists
transpose([R], Cs) :- !,
	list2columns(R, Cs).
transpose([R|Words], Cs) :- 
	transpose(Words, Cs0),
	put_columns(R, Cs0, Cs).
	
list2columns([], []).
list2columns([X|Xs], [[X]|Zs]) :- list2columns(Xs, Zs).

put_columns([], Cs, Cs).
put_columns([X|Xs], [C|Cs0], [[X|C]|Cs]) :- 
  put_columns(Xs, Cs0, Cs).
  
