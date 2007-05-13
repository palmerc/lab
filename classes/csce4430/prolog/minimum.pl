minpart([],_,[],[]).
minpart([X|Xs],Y,[X|Ls],Bs) :-
	X =< Y, minpart(Xs,Y,Ls,Bs).
minpart([X|Xs],Y,Ls,[X|Bs]) :-
	X > Y, minpart(Xs,Y,Ls,Bs).
minquick([],[]).
minquick([X|Xs],Ys) :-
	minpart(Xs,X,Littles,Bigs),
	minquick(Littles,Ls),
	minquick(Bigs,Bs),
	append(Ls,[X|Bs],Ys).
minimum(List, Minimum) :-
	minquick(List, [Minimum|_]).