maxpart([],_,[],[]).
maxpart([X|Xs],Y,[X|Ls],Bs) :-
	X =< Y, maxpart(Xs,Y,Ls,Bs).
maxpart([X|Xs],Y,Ls,[X|Bs]) :-
	X > Y, maxpart(Xs,Y,Ls,Bs).
maxquick([],[]).
maxquick([X|Xs],Ys) :-
	maxpart(Xs,X,Littles,Bigs),
	maxquick(Littles,Ls),
	maxquick(Bigs,Bs),
	append(Bs,[X|Ls],Ys).
maximum(List, Maximum) :-
	maxquick(List, [Maximum|_]).