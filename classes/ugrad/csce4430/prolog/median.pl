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
mean([], 0) .
mean(Tail, Mean) :-
	sum(Tail, Sum),
	list_length(Tail, Count),
	Mean is Sum / Count.
list_length([], 0).
list_length([_ | Tail], Count) :-
	list_length(Tail, Counter),
	Count is Counter + 1.
list_element([H|T], Pos, Value) :-
	Cur is Pos - 1,
	( Cur=\=0
	-> list_element(T, Cur, Value)
	; Value is H ).
even(List, Pos, Value) :-
	list_element(List, Pos, Result1),
	list_element(List, Pos + 1, Result2),
	Value is ((Result1 + Result2)/ 2).
odd(List, Pos, Value) :-
	list_element(List, Pos, Value).
middle(List, Middle) :-
	list_length(List, Count),
	Mod is mod(Count, 2),
	( Mod = 0
	-> even(List, Count / 2, Middle)
	; odd(List, truncate(Count / 2) + 1, Middle) ).
median(List, Median) :-
	quicksort(List, Sorted),
	middle(Sorted, Median).
	
	