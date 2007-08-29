list_to_set([], _).
list_to_set([H|T], Set) :-
	( memberchk(H, Set)
	-> append(_, Set, Set)
	; append(H, Set, Set) ),
	list_to_set(T, Set).