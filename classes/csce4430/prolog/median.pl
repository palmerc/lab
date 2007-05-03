partition([],_,[],[]).
partition([X|Xs],Y,[X|Ls],Bs) :-
	X < Y, partition(Xs,Y,Ls,Bs).
partition([X|Xs],Y,Ls,[X|Bs]) :-
	X > Y, partition(Xs,Y,Ls,Bs).
quicksort([],[]).
quicksort([X|Xs],Ys) :-
	partition(Xs,X,Littles,Bigs),
	quicksort(Littles,Ls),
	quicksort(Bigs,Bs),
	append(Ls,[X|Bs],Ys).
sum([], 0).
sum([First | Rest], Total) :-
	sum(Rest, TotalOfRest),
        Total is First + TotalOfRest.
list_length([], 0).
list_length([_ | Tail], Count) :-
	list_length(Tail, Counter),
	Count is Counter + 1.
mean([], 0) .
mean(Tail, Mean) :-
	sum(Tail, Sum),
	list_length(Tail, Count),
	Mean is Sum / Count.
middle(List, Middle) :-
	list_length(List, Count),
	Mod is mod(Count, 2),
	( Mod = 0
	-> ceil(Mod, Ceil)
	; floor(Mod, ).
	Middle is Count / 2.